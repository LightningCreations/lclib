/*
 * ReflectionInfo.hpp
 *
 *  Created on: Sep 1, 2018
 *      Author: connor
 */

#ifndef INCLUDE_REFLECT_REFLECTIONINFO_HPP_2018_09_01_22_56
#define INCLUDE_REFLECT_REFLECTIONINFO_HPP_2018_09_01_22_56
#include <Config.hpp>
#include <type_traits>
#include <TypeInfo.hpp>
#include <FunctionTypeInfo.hpp>
#include <memory>
#include <exception>




namespace reflect{
	LCLIBAPI struct BadReflectionTypeException:public std::exception{
	public:
		const char* what()const noexcept(true)override;
	};
	struct reflection_info_base{
		std::ptrdiff_t fieldOffsets[8]={0,0,0,0,0,0,0,0};//Needs to know the layout of the object
		//Actual Uses and what offsets are stored here depend on the type of reflection key
		//Required offsets:
		//0 is hashEntry
		//1 is name
		//2 is decltypeCode (if supported), or Type Hash of the base-class/underlying-type (types)
		//3 is address (fields and methods only), or value (enumerators)
	protected:
		~reflection_info_base()=default;
	};
	enum ReflectionKeyType{
		TYPE = 1,
		FIELD,
		METHOD,
		ENUMERATOR,
		MEMBER
	};
	LCLIBAPI void registerReflectionKey(const reflection_info_base*,types::TypeCode,ReflectionKeyType);
	LCLIBAPI const reflection_info_base* getReflectionKey(types::TypeCode);
	LCLIBAPI void registerTypeAlias(types::TypeCode,const reflection_info_base*);
	template<typename T,T& field> struct field_reflection_info:reflection_info_base{
		types::TypeCode decltypeCode;
		T* address;
		const char* name;
		types::TypeCode hashEntry;
		const void* key;
		template<size_t N>  field_reflection_info(const char(&name)[N]):decltypeCode(types::TypeHash<T>{}()),
				address(&field),name(name),
				hashEntry(types::nameHashcode(name)),
				key(this){
			fieldOffsets[2] = (&decltypeCode)-this;
			fieldOffsets[3] = (&address)-this;
			fieldOffsets[1] = (&name)-this;
			fieldOffsets[0] = (&hashEntry)-this;
			registerReflectionKey(this,hashEntry,FIELD);
		}
	};
	namespace detail{
		extern"C" template<typename Ret,typename... Args> using cfunction = Ret(Args...);
		extern"C" template<typename Ret,typename... Args> using noexcept_cfunction = Ret(Args...);

		template<typename T> struct extended_function_info{};
		template<typename Ret,typename... Args> struct extended_function_info<Ret(Args...)>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =0;
			using decayed_function_type = std::decay_t<Ret>(std::decay_t<Ret>...);
		};
		template<typename Ret,typename... Args> struct extended_function_info<Ret(Args...)noexcept>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =1;
			using decayed_function_type = std::decay_t<Ret>(std::decay_t<Ret>...)noexcept;
		};
		template<typename Ret,typename... Args> struct extended_function_info<cfunction<Ret,Args...>>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =2;
			extern"C" using decayed_function_type = std::decay_t<Ret>(std::decay_t<Ret>...);
		};
		template<typename Ret,typename... Args> struct extended_function_info<noexcept_cfunction<Ret,Args...>>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =3;
			extern"C" using decayed_function_type = std::decay_t<Ret>(std::decay_t<Ret>...)noexcept;
		};
	};
	template<typename T,T& method> struct function_reflection_info:detail::extended_function_info<T>{
		types::TypeCode decltypeCode;
		T* address;
		const char* name;
		types::TypeCode hashKey;
		template<size_t N> function_reflection_info(const char(&name)[N]):
			decltypeCode(types::TypeHash<typename decayed_function_type>()),
			address(&method),
			name(name),
			hashKey(types::nameHash(name)^types::TypeHash<T>{}()){
			fieldOffsets[0] = offsetof(function_reflection_info,name);
			fieldOffsets[1] = offsetof(function_reflection_info,hashKey);
			fieldOffsets[2] = offsetof(function_reflection_info,decltypeCode);
			fieldOffsets[3] = offsetof(function_reflection_info,address);
			fieldOffsets[4] = (&argsTypeCodes[0])-this;
			fieldOffsets[5] = offsetof(function_reflection_info,retTypeCode);
			fieldOffsets[6] = offsetof(function_reflection_info,flags);
			registerReflectionKey(this,hash,FUNCTION);
		}
	};

	template<typename T> struct type_reflection_info:reflection_info_base{
		types::TypeCode hashKey;
		const char* name;
		types::TypeCode baseClass;
		template<size_t N> type_reflection_info(const char(&name)[N]):
				hashKey(types::TypeHash<T>{}()),
				name(name),
				baseClass(std::is_enum_v<T>?types::TypeHash<std::underlying_type_t<T>>:
						(is_polymorphic_hashable_v<T>?types::TypeHash<base_type_t<T>>{}():TypeCode::NONE)){
			fieldOffsets[0] = offsetof(type_reflection_info,name);
			fieldOffsets[1] = offsetof(type_reflection_info,typeCode);
			fieldOffsets[2] = offsetof(type_reflection_info,baseClass);
			registerReflectionKey(this,hashKey,TYPE);
			registerTypeAlias(types::nameHashcode(name),this);
		}

	};
	template<typename E,E val> struct enumerator_reflection_info{

	};


	template<typename T> struct reflected_field{
		const reflection_info_base* inf;
	public:
		template<size_t N> reflected_field(const char(&name)[N]):inf(getReflectionKey(types::nameHash(name))){
			if(getTypeCode()!=types::TypeHash<T>{}())
				throw BadReflectionTypeException();
		}
		const char* getName()const{
			return *((const char*const*)(inf+inf->fieldOffsets[0]));
		}
		types::TypeCode getTypeCode()const{
			return *((const types::TypeCode*)(inf+inf->fieldOffsets[2]));
		}
		operator T&()const{
			return **((T*const*)(inf+inf->fieldOffsets[3]));
		}
	};
	template<typename F> struct reflected_method{
		const reflection_info_base* inf;
		using decay_signature = typename detail::extended_function_info<F>::decay_signature_type;
	public:
		template<size_t N> reflected_method(const char(&name)[N]):
		inf(getReflectionKey(types::nameHash(name)^types::TypeHash<decay_signature>{}())){
			if(getTypeCode()!=types::TypeHash<decay_signature>{}())
				throw BadReflectionTypeException();
		}
		const char* getName()const{
			return *((const char*const*)(inf+inf->fieldOffsets[0]));
		}
		types::TypeCode getTypeCode()const{
			return *((const types::TypeCode*)(inf+inf->fieldOffsets[2]));
		}
		const types::TypeCode* getArgumentTypeCodes()const{
			return *((const types::TypeCode*const*)(inf+inf->fieldOffsets[4]));
		}
		types::TypeCode getReturnTypeCode()const{
			return *((const types::TypeCode*)(inf+inf->fieldOffsets[5]));
		}
		uint8_t getFlags()const{
			return *((const uint8_t*)(inf+inf->fieldOffsets[6]));
		}
		const char* getLanguageLinkage()const{
			if(getFlags()&2)
				return "C";
			else
				return "C++";
		}
		bool isNoexcept()const{
			return getFlags()&1;
		}
		template<typename... Args> auto operator()(Args&&... args)const noexcept(noexcept(std::declval<F>()(std::forward(args)...)))->decltype(std::declval<F>()(std::forward(args)...)){
			F& ref = **((F*const*))(inf+inf->fieldOffsets[3]));
			return ref(std::forward<Args>(args)...);
		};
		operator F*()const{
			return *((F*const*))(inf+inf->fieldOffsets[3]));
		}
	};
};
#define BIND(cl,name) cl::name
#ifndef __NOREFLECTION
#define export_field(field) extern const reflect::field_reflection_info<decltype(field),field> UNIQUEID(__reflect__f##field){#field};
#define export_function(function) extern const reflect::function_reflection_info<decltype(function),function> UNIQUEID(__reflect__m##function){#function};
#define export_type(type) extern const reflect::type_reflection_info<type> UNIQUEID(__reflect__t##type){#type};
#define export_function_overload(function,ret,...) extern const reflect::function_reflection_info<ret(__VA_ARGS__),function> UNIQUEID(__reflect__m){#function};
#define export_static_function(cl,function) extern const reflect::function_reflection_info<decltype(BIND(cl,function)),BIND(cl,function)> UNIQUEID(__reflect__m##cl##_1##function){STRINGIFY(BIND(cl,function))};
#else
#define export_field(field)
#define export_function(function)
#define export_type(type)
#define export_function_overload(name,ret,...)
#endif
#endif /* INCLUDE_REFLECT_REFLECTIONINFO_HPP_ */
