/*
 * CVarargsInterface.hpp
 *
 *  Created on: Jan 14, 2019
 *      Author: chorm
 */

#ifndef INCLUDE_LCLIB_CVARARGSINTERFACE_HPP_2019_01_14_13_06
#define INCLUDE_LCLIB_CVARARGSINTERFACE_HPP_2019_01_14_13_06

#include <lclib/Config.hpp>
#include <cstdarg>
#include <tuple>
#include <lclib/TypeTraits.hpp>

template<typename T,bool=std::is_enum_v<T>> struct _va_promote:type_identity<T>{};
template<> struct _va_promote<float>:type_identity<double>{};
template<> struct _va_promote<unsigned char>:type_identity<unsigned>{};
template<> struct _va_promote<signed char>:type_identity<int>{};
template<> struct _va_promote<char>:std::conditional<std::is_signed_v<char>,int,unsigned>{};
template<> struct _va_promote<short>:type_identity<int>{};
template<> struct _va_promote<unsigned short>:type_identity<unsigned>{};
template<typename T> struct _va_promote<const T,false>:_va_promote<T>{};
template<typename T> struct _va_promote<volatile T,false>:_va_promote<T>{};
template<typename T> struct _va_promote<const volatile T,false>:_va_promote<T>{};
template<typename E> struct _va_promote<E,true>:
	std::conditional<std::is_convertible_v<E,std::underlying_type_t<E>>,typename _va_promote<std::underlying_type_t<E>>::type,E>{};

template<typename T> using _va_promote_t = typename _va_promote<T>::type;
/**
  Expands a varargs list into a tuple.
  If Any of the types in Types... are not trivially copyable, the function is defined as deleted.
  If list was not initialized via a call to va_start, va_copy, or has been released with a call to va_end, the behavior is undefined.
  For each type T_n in std::decay<Types>..., and the corresponding arugment type U_n, one of the following shall be true or the behavior is undefined:
   T_n and U_n are the same type
   T_n is an integral type (except for long, unsigned long, long long, or unsigned long long) and U_n is int or unsigned.
   T_n is long and U_n is unsigned long or vice versa
   T_n is long long and U_n is unsigned long long or vice versa.
   T_n is float and U_n is double
   T_n is a pointer to (possibly cv-qualified) void, and U_n is a pointer to (identically cv-qualified) character type
   T_n is a possibly cv-qualified variant of U_n.
*/
template<typename... Types,std::enable_if_t<std::conjunction_v<std::is_trivially_copyable<Types>...>>* = 0> std::tuple<Types...> va_expand(va_list list){
	return std::tuple<Types...>{((Types)va_arg(list,_va_promote_t<Types>))...};//Use sequencing 
}

template<typename... Types,std::enable_if_t<std::negation_v<std::conjunction<std::is_trivially_copyable<Types>...>>>* = 0> std::tuple<Types...> va_expand(va_list)=delete;




#endif /* INCLUDE_LCLIB_CVARARGSINTERFACE_HPP_2019_01_14_13_06 */
