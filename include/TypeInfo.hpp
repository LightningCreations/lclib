/*
 * TypeInfo.hpp
 *
 *  Created on: Sep 7, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_TYPEINFO_HPP__2018_09_07_08_55
#define __INCLUDE_TYPEINFO_HPP__2018_09_07_08_55
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ccomplex>



namespace types{
	namespace detail{
		constexpr char charCode(){
			if constexpr(std::is_signed_v<char>)
				return 'C';//If char is signed, it is the same as SCHAR
			else if(std::is_unsigned_v<char>)
				return 'B';//Otherwise if it is unsigned, it is the same as UCHAR
			else
				return 'X';//Otherwise give it is own type code (should never occur)
		}
	};
	enum class TypeCode:uint64_t{
		NONE=0,
		MASK=0xffffffffff,
		LEVEL=0x1f,
		VARARGS=~0x3C6EF372FE94F82B,
		CHAR=detail::charCode(),
		UCHAR='B',
		SCHAR='C',
		SHORT='S',
		USHORT='A',
		INT='I',
		UINT='U',
		LONG='j',
		ULONG='q',
		LLONG='J',
		ULLONG='Q',
		VOID='V',
		NULLPTR='N',
		PTRDIFF='P',
		FLOAT='F',
		DOUBLE='d',
		LDOUBLE='D',
		WCHAR='W',
		CHAR16=0x100+detail::charCode(),
		CHAR32=0x200+detail::charCode(),
		CFLOAT=0x100+'F',
		CDOUBLE=0x100+'d',
		CLDOUBLE=0x100+'D',
		IFLOAT=0x200+'F',
		IDOUBLE=0x200+'d',
		ILDOUBLE=0x200+'D'
	};


	enum class LevelModifier:uint64_t{
		CONST=0x243F6A8885A308D3,
		VOLATILE=0xB7E151628AED2A6B,
		CV=0x93DE3BEA0F4E22B8
	};
	enum class Level:uint64_t{
		EXTENT=0x93C467E37DB0C7A4,
		POINTER=0x3707344A40938222,
		MEMPTR=0x6A09E667F3BCC908,
		REFERENCE=0xBB67AE8584CAA73B,
		XVALUE=~0xBB67AE8584CAA73B,
		FUNCTION=0x3C6EF372FE94F82B
	};

	/**
	 * Easy wrapper to hash a String. Intended for use with specializations of TypeHash
	 */
	template<size_t N> constexpr TypeCode nameHash(const char(&a)[N]){
		uint64_t h{0};
		for(size_t i=0;i<(N-1);i++)
			h = h*31+a[i];
		return TypeCode{h};
	}

	TypeCode operator+(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)+static_cast<uint64_t>(b)};
	}
	TypeCode operator-(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)-static_cast<uint64_t>(b)};
	}
	TypeCode operator*(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)*static_cast<uint64_t>(b)};
	}
	TypeCode operator/(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)/static_cast<uint64_t>(b)};
	}
	TypeCode operator^(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)^static_cast<uint64_t>(b)};
	}
	TypeCode operator|(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)|static_cast<uint64_t>(b)};
	}
	TypeCode operator&(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)&static_cast<uint64_t>(b)};
	}
	TypeCode operator+(TypeCode a,LevelModifier b){
		return TypeCode{static_cast<uint64_t>(a)+static_cast<uint64_t>(b)};
	}
	TypeCode operator-(TypeCode a,LevelModifier b){
		return TypeCode{static_cast<uint64_t>(a)-static_cast<uint64_t>(b)};
	}
	TypeCode operator*(TypeCode a,LevelModifier b){
		return TypeCode{static_cast<uint64_t>(a)*static_cast<uint64_t>(b)};
	}
	TypeCode operator^(TypeCode a,LevelModifier b){
		return TypeCode{static_cast<uint64_t>(a)^static_cast<uint64_t>(b)};
	}
	TypeCode operator|(TypeCode a,LevelModifier b){
		return TypeCode{static_cast<uint64_t>(a)|static_cast<uint64_t>(b)};
	}
	TypeCode operator&(TypeCode a,LevelModifier b){
		return TypeCode{static_cast<uint64_t>(a)&static_cast<uint64_t>(b)};
	}

	/**
	 * TypeHash is a Nullary Function Object which returns a Static Hashcode for a type (Which is Stored in a TypeCode Safe Integer),
	 * TypeHash is provided as an opt-in basis for systems which might use type-uniqueness
	 * TypeHash specializations must satisfy TypeHashSpecialization
	 * If TypeHash is specialized for a given type it is also specialized for cv-qualfied,
	 * pointers to, lvalue and rvalue references to, and Arrays of.
	 * In addition, any function type which accepts only types for which TypeHash is specialized,
	 * and returns such a type, also has a specialization provided for it.
	 * All primitive types have specializations of TypeHash, and most STL Types have specializations provided
	 * in <STLTypeInfo.hpp>.
	 * For template types, you can enable specializations if some or all of the template type parameters have specializations,
	 * using the enable_specialization (and enable_specialization_t) template
	 * The first template argument is the partial specialization to enable,
	 * and the remaining arguments are the types to check.
	 * The specialization is enabled if all of the provided types have valid specializations of TypeHash
	 * The TypeHashSpecialization requirement requires that All of the Following conditions be met:
	 * For some t which is an instance of the specialization of TypeHash (and given TypeHash in this context means the specific specialization):
	 * TypeHash{}, TypeHash{t}, TypeHash{std::move(t)} are all well-formed, and are a constant expression
	 * For some lvalue l of TypeHash,
	 * l = t, and l = std::move(t) are both well formed, and are a constant expression
	 * t() is well formed, is a constant expression, and returns a TypeCode.
	 * TypeHash's for polymorphic types may have more information assigned to it,
	 * including its base class, and the Specialization for its base class.
	 * If it does then it may enable optimizations in certain utilities which depend
	 * on polymorphism by using the type hash to check the current type and use non-checked cast,
	 * rather than using slower RTTI.
	 * In order to opt into these optimizations, the specialization must define member typedefs base_type and base_hash.
	 * When defining these types in a specialization for T, std::is_base_of_v<T,base_type> must be true,
	 * and base_type have a specialization of TypeHash or the behavior is undefined.
	 * Also the behavior is undefined if there exists some type U for which std::is_same_v<T,U>,
	 * and std::is_same_v<base_type,U> is not true and std::is_base_of_v<T,U> and std::base_of_v<U,base_type>
	 * is true (that is, T must directly inherit from base_type)
	 * In addition, base_hash must be a Specialization of TypeHash for base_type,
	 */
	template<typename T> struct TypeHash{
	private:
		TypeHash()=delete;
		TypeHash(const TypeHash&)=delete;
		TypeHash(TypeHash&&)=delete;
		TypeHash& operator=(const TypeHash&)=delete;
		TypeHash& operator=(TypeHash&&)=delete;
	};

	template<typename T,typename... Types> struct enable_specialization{
		using type = std::enable_if_t<std::conjunction_v<std::is_invocable_r<TypeHash<Types>,TypeCode>...>,T>;
	};

	template<typename T,typename... Types> using enable_specialization_t = typename enable_specialization<T,Types...>::type;



	template<typename T> struct TypeHash<enable_specialization_t<const T,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()^LevelModifier::CONST;
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<volatile T,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()^LevelModifier::VOLATILE;
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<const volatile T,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()^LevelModifier::CV;
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<T[],T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()*TypeCode::LEVEL^Level::EXTENT;
		}
	};
	template<typename T,size_t N> struct TypeHash<enable_specialization_t<T[N],T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()*TypeCode::LEVEL^(Level::EXTENT*TypeCode{N+1});
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<T*,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()*TypeCode::LEVEL^Level::POINTER;
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<T&,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()*TypeCode::LEVEL^Level::REFERENCE;
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<T&&,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()*TypeCode::LEVEL^Level::XVALUE;
		}
	};
	template<class C,typename T> struct TypeHash<enable_specialization_t<T(C::*),T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<C>{}()*TypeCode::LEVEL^Level::MEMPTR*TypeHash<T>{}();
		}
	};


	template<> struct TypeHash<void>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::VOID;
		}
	};
	template<> struct TypeHash<char>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::CHAR;
		}
	};
	template<> struct TypeHash<signed char>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::SCHAR;
		}
	};
	template<> struct TypeHash<unsigned char>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::UCHAR;
		}
	};
	template<> struct TypeHash<short>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::SHORT;
		}
	};
	template<> struct TypeHash<unsigned short>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::USHORT;
		}
	};
	template<> struct TypeHash<int>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::INT;
		}
	};
	template<> struct TypeHash<unsigned int>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::UINT;
		}
	};
	template<> struct TypeHash<long>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::LONG;
		}
	};
	template<> struct TypeHash<unsigned long>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::ULONG;
		}
	};
	template<> struct TypeHash<long long>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::LLONG;
		}
	};
	template<> struct TypeHash<unsigned long long>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::ULLONG;
		}
	};
	template<> struct TypeHash<decltype(nullptr)>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::NULLPTR;
		}
	};
	template<> struct TypeHash<decltype(((void*)0)-((void*)0))>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::PTRDIFF;
		}
	};
	template<> struct TypeHash<float>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::FLOAT;
		}
	};
	template<> struct TypeHash<double>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::DOUBLE;
		}
	};
	template<> struct TypeHash<long double>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::LDOUBLE;
		}
	};
	template<> struct TypeHash<wchar_t>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::WCHAR;
		}
	};
	template<> struct TypeHash<char16_t>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::CHAR16;
		}
	};
	template<> struct TypeHash<char32_t>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::CHAR32;
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<TypeHash<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("TypeHash");
		}
	};
	template<> struct TypeHash<TypeCode>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return nameHash("TypeCode");
		}
	};
	template<> struct TypeHash<LevelModifier>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return nameHash("LevelModifier");
		}
	};
	template<> struct TypeHash<Level>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return nameHash("Level");
		}
	};
	template<> struct TypeHash<double complex>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::CDOUBLE;
		}
	};
	template<> struct TypeHash<float complex>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::CFLOAT;
		}
	};
	template<> struct TypeHash<long double complex>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::CLDOUBLE;
		}
	};

	template<> struct TypeHash<double imaginary>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::IDOUBLE;
		}
	};
	template<> struct TypeHash<float imaginary>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::IFLOAT;
		}
	};
	template<> struct TypeHash<long double imaginary>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::ILDOUBLE;
		}
	};
	template<> struct TypeHash<std::byte>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode::UCHAR+0x400;
		}
	};
}

template<typename T> struct is_hashable_type:std::false_type{};
template<typename T> struct is_hashable_type<std::enable_if_t<std::is_invocable_r_v<types::TypeHash<T>,types::TypeCode>,T>>:std::true_type{};
template<typename T> constexpr const bool is_hashable_type_v = is_hashable_type<T>::value;

template<typename T,typename=void> struct is_polymorphic_hashable:std::false_type{};
template<typename T> struct is_polymorphic_hashable<T,std::void_t<std::enable_if_t<is_hashable_type_v<T>>,typename types::TypeHash<T>::base_type,
	typename types::TypeHash<T>::base_hash>>:std::true_type{};
template<typename T> constexpr const bool is_polymorphic_hashable_v = is_polymorphic_hashable<T>::value;

/**
 * Gets the direct base type of T, if T is polymorphic_hashable
 * Useful in Cache Optimizations
 */
template<typename T> using base_type_t = typename types::TypeHash<T>::base_type;
/**
 * Gets the hash implementation of the direct base type of T, if T is polymorphic_hashable
 * Useful in Hash-based polymorphism optimizations
 */
template<typename T> using base_hash_t = typename types::TypeHash<T>::base_hash;

#define NameHash(cl) \
	namespace types{\
		template<> struct TypeHash<cl>{\
		public:\
			constexpr TypeHash()=default;\
			constexpr TypeHash(const TypeHash&)=default;\
			constexpr TypeHash(TypeHash&&)=default;\
			TypeHash(const TypeHash&&)=delete;\
			constexpr TypeHash& operator=(const TypeHash&)=default;\
			constexpr TypeHash& operator=(TypeHash&&)=default;\
			TypeHash& operator=(const TypeHash&&)=delete;\
			constexpr TypeCode operator()(){\
				return nameHash(#cl);\
			}\
		};\
	}
#define NameBaseClassHash(cl,base)\
	namespace types{\
		template<> struct TypeHash<cl>{\
		public:\
			using base_class = base;\
			using base_hash = TypeHash<base_class>;\
			constexpr TypeHash()=default;\
			constexpr TypeHash(const TypeHash&)=default;\
			constexpr TypeHash(TypeHash&&)=default;\
			TypeHash(const TypeHash&&)=delete;\
			constexpr TypeHash& operator=(const TypeHash&)=default;\
			constexpr TypeHash& operator=(TypeHash&&)=default;\
			TypeHash& operator=(const TypeHash&&)=delete;\
			constexpr TypeCode operator()(){\
				return TypeHash<base_class>{}()^nameHash(#cl);\
			}\
		};\
	}

#endif /* __INCLUDE_TYPEINFO_HPP__2018_09_07_08_55 */
