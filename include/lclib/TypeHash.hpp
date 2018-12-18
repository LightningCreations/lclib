/*
 * TypeHash.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_TYPEHASH_HPP__2018_09_30_17_45_34
#define __INCLUDE_LCLIB_CXX_TYPEHASH_HPP__2018_09_30_17_45_34
#include <type_traits>
#include <cstdint>
#include <cstddef>
namespace types{
	enum class TypeCode:std::uint64_t{
		CONST = ~uint64_t{'^'},
		VOLATILE = ~uint64_t{'!'},
		CV = ~uint64_t{'^'} + ~uint64_t{'!'},
		LVALUE = ~uint64_t{'&'},
		XVALUE = ~uint64_t{'%'},
		POINTER = ~uint64_t{'*'},
		EXTENT = ~uint64_t{'['},
		FUNCTION = ~uint64_t{'@'},

		LEVEL = 5828412416416714837
	};
	TypeCode operator*(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)*static_cast<uint64_t>(b)};
	}
	TypeCode operator*(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)*b};
	}
	TypeCode operator*(uint64_t a,TypeCode b){
		return TypeCode{a*static_cast<uint64_t>(b)};
	}
	TypeCode operator+(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)+static_cast<uint64_t>(b)};
	}
	TypeCode operator+(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)+b};
	}
	TypeCode operator+(uint64_t a,TypeCode b){
		return TypeCode{a+static_cast<uint64_t>(b)};
	}
	TypeCode operator-(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)-static_cast<uint64_t>(b)};
	}
	TypeCode operator-(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)-b};
	}
	TypeCode operator-(uint64_t a,TypeCode b){
		return TypeCode{a-static_cast<uint64_t>(b)};
	}
	TypeCode operator/(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)/static_cast<uint64_t>(b)};
	}
	TypeCode operator/(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)/b};
	}
	TypeCode operator/(uint64_t a,TypeCode b){
		return TypeCode{a/static_cast<uint64_t>(b)};
	}
	TypeCode operator&(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)&static_cast<uint64_t>(b)};
	}
	TypeCode operator&(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)&b};
	}
	TypeCode operator&(uint64_t a,TypeCode b){
		return TypeCode{a&static_cast<uint64_t>(b)};
	}
	TypeCode operator|(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)|static_cast<uint64_t>(b)};
	}
	TypeCode operator|(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)|b};
	}
	TypeCode operator|(uint64_t a,TypeCode b){
		return TypeCode{a|static_cast<uint64_t>(b)};
	}
	TypeCode operator^(TypeCode a,TypeCode b){
		return TypeCode{static_cast<uint64_t>(a)^static_cast<uint64_t>(b)};
	}
	TypeCode operator^(TypeCode a,uint64_t b){
		return TypeCode{static_cast<uint64_t>(a)^b};
	}
	TypeCode operator^(uint64_t a,TypeCode b){
		return TypeCode{a^static_cast<uint64_t>(b)};
	}
	TypeCode operator~(TypeCode a){
		return TypeCode{~static_cast<uint64_t>(a)};
	}
	TypeCode operator-(TypeCode a){
		return TypeCode{-static_cast<uint64_t>(a)};
	}

	constexpr TypeCode name(const char* a){
		TypeCode hash{3409377983544443389};
		const TypeCode prime{11717323977941816153};
		const TypeCode p2{9027246189117317093};
		for(;*a!=0;a++){
			hash *= prime;
			hash += ~uint64_t{*a};
			hash ^= p2*(*a);
		}
		return hash;
	}

	template<typename T,typename=void> struct TypeHash{};

	template<TypeCode c> using hash_type = std::integral_constant<TypeCode,c>;

	template<> struct TypeHash<char>:hash_type<name("char")>{};
	template<> struct TypeHash<signed char>:hash_type<name("signed char")>{};
	template<> struct TypeHash<unsigned char>:hash_type<name("unsigned char")>{};
	template<> struct TypeHash<short>:hash_type<name("short")>{};
	template<> struct TypeHash<unsigned short>:hash_type<name("unsigned short")>{};
	template<> struct TypeHash<int>:hash_type<name("int")>{};
	template<> struct TypeHash<unsigned>:hash_type<name("unsigned")>{};
	template<> struct TypeHash<long>:hash_type<name("long")>{};
	template<> struct TypeHash<unsigned long>:hash_type<name("unsigned long")>{};
	template<> struct TypeHash<long long>:hash_type<name("long long")>{};
	template<> struct TypeHash<unsigned long long>:hash_type<name("unsigned long long")>{};
	template<> struct TypeHash<wchar_t>:hash_type<name("wchar_t")>{};
	template<> struct TypeHash<char16_t>:hash_type<name("char16_t")>{};
	template<> struct TypeHash<char32_t>:hash_type<name("char32_t")>{};
	template<> struct TypeHash<void>:hash_type<name("void")>{};


	template<typename T> struct TypeHash<const T,std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::CONST>{};
	template<typename T> struct TypeHash<volatile T,std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::VOLATILE>{};
	template<typename T> struct TypeHash<const volatile T,std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::CV>{};
	template<typename T> struct TypeHash<T[],std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::EXTENT>{};
	template<typename T,std::size_t N> struct TypeHash<T[N],std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::EXTENT+N>{};
	template<typename T> struct TypeHash<T&,std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::LVALUE>{};
	template<typename T> struct TypeHash<T&&,std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::XVALUE>{};
	template<typename T> struct TypeHash<T*,std::void_t<decltype(TypeHash<T>{}())>>
		:hash_type<TypeHash<T>{}()*TypeCode::LEVEL+TypeCode::POINTER>{};


	template<typename T,typename... Types> TypeCode sum(T v1,Types... vs){
		if constexpr(sizeof...(Types)==0)
			return v1;
		else
			return TypeCode{v1}+sum(vs...)*31;
	}
}

#endif /* __INCLUDE_LCLIB_CXX_TYPEHASH_HPP__2018_09_30_17_45_34 */
