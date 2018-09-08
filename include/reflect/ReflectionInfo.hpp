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
		//2 is decltypeCode (if supported), otherwise TypeHash of the class
		//3 is address (fields and methods only), or value (enumerators)
	protected:
		~reflection_info_base()=default;
	};
	LCLIBAPI void registerReflectionKey(const reflection_info_base*,types::TypeCode);
	LCLIBAPI const reflection_info_base* getReflectionKey(types::TypeCode);
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
		};
		template<typename Ret,typename... Args> struct extended_function_info<Ret(Args...)noexcept>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =1;
		};
		template<typename Ret,typename... Args> struct extended_function_info<cfunction<Ret,Args...>>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =2;
		};
		template<typename Ret,typename... Args> struct extended_function_info<noexcept_cfunction<Ret,Args...>>:reflection_info_base{
			types::TypeCode argsTypeCodes[sizeof...(Args)]={types::TypeHash<Args>{}()...};
			types::TypeCode retTypeCode = types::TypeHash<Ret>{}();
			uint8_t flags =3;
		};
	};
	template<typename T,T& method> struct function_reflection_info:detail::extended_function_info<T>{
		types::TypeCode decltypeCode;
		T* address;
		const char* name;
		types::TypeCode hashCode;
		template<size_t N> function_reflection_info(const char(&name)[N]):
			decltypeCode(types::TypeHash<T>()),
			address(&method),
			name(name),
			hashCode(types::nameHash(name)^types::TypeHash<T>{}()){
			fieldOffsets[0] = (&name)-this;
			fieldOffsets[1] = (&hashCode)-this;
			fieldOffsets[2] = (&decltypeCode)-this;
			fieldOffsets[3] = (&address)-this;
			fieldOffsets[4] = (&argsTypeCodes[0])-this;
			fieldOffsets[5] = (&retTypeCode)-this;
			fieldOffsets[6] = (&flags)-this;
		}
	};

	template<typename T> struct type_reflection_info{
		//TODO add some sort of export to the binary for reflection information
	};
	template<typename E,E val> struct enumerator_reflection_info{
		//TODO add some sort of export of reflection information
		//UB if the target is not an enum type
	};
	template<class target,typename... Args> struct constructor_reflection_info{
		//TODO export reflection info
		//UB if the class does not have a constructor with the give args types
		//(Note that if its defined but defined as defaulted its not UB)
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
	public:
		template<size_t N> reflected_method(const char(&name)[N]):inf(getReflectionKey(types::nameHash(name)^types::TypeHash<F>{}())){
			if(getTypeCode()!=types::TypeHash<T>{}())
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
		template<typename... Args> auto operator()(Args&&... args)const->decltype(std::declval<F>()(std::forward(args)...)){
			F& ref = **((F*const*))(inf+inf->fieldOffsets[3]));
			return ref(std::forward<Args>(args)...);
		};
		operator F*()const{
			return *((F*const*))(inf+inf->fieldOffsets[3]));
		}
	};
};
#define export_field(field) reflect::field_reflection_info<decltype(field),field> __reflect__f##__UNIQUE__;
#define export_function(function) reflect::function_reflection_info<decltype(function),function> __reflect__m##__UNIQUE__;


#endif /* INCLUDE_REFLECT_REFLECTIONINFO_HPP_ */
