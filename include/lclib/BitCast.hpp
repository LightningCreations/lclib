/*
 * BitCast.hpp
 *
 *  Created on: Jan 3, 2019
 *      Author: chorm
 */

#ifndef INCLUDE_LCLIB_BITCAST_HPP_2019_01_03_12_21
#define INCLUDE_LCLIB_BITCAST_HPP_2019_01_03_12_21

#if defined(__cpp_bit_cast) && __cpp_bit_cast > 201806
#include <bit>
using std::bit_cast;
#define __HAS_BIT_CAST
#endif
#ifndef __HAS_BIT_CAST
#include <cstddef>
#include <new>

namespace detail{
	template<typename To,typename From,std::enable_if_t<!std::is_enum_v<To>&&!std::is_enum_v<From>&&!std::is_same_v<std::decay<To>,std::decay<From>>>* = 0> To bit_cast_helper(const From& f){
		union{
			From f1;
			To t1;
		} u = {f};
		return *std::launder(static_cast<To*>(static_cast<void*>(std::launder(&u))));
	}
	template<typename To,std::enable_if_t<std::is_enum_v<To>>* = 0> constexpr To bit_cast_helper(const std::underlying_type_t<To>& t){
		return static_cast<To>(t);
	}
	template<typename To,typename From,std::enable_if_t<std::is_enum_v<To>&&!std::is_enum_v<From>>* = 0> constexpr To bit_cast_helper(const From& f){
		return static_cast<To>(bit_cast_helper<std::underlying_type_t<To>>(f));
	}
	template<typename To,typename From,std::enable_if_t<!std::is_enum_v<To>&&std::is_enum_v<From>>* = 0> constexpr To bit_cast_helper(const From& f){
			return static_cast<To>(bit_cast_helper<std::underlying_type_t<To>>(f));
		}
	template<typename To,typename From,std::enable_if_t<std::is_enum_v<From>&&!std::is_enum_v<To>>* = 0,std::enable_if_t<std::is_same_v<std::decay_t<To>,std::underlying_type_t<From>>>* =0> constexpr To bit_cast_helper(const From& f){
		return static_cast<To>(f);
	}
	template<typename To,typename From,std::enable_if_t<std::is_enum_v<From>&&!std::is_enum_v<To>>* = 0,std::enable_if_t<!std::is_same_v<std::decay_t<To>,std::underlying_type_t<From>>>* =0> constexpr To bit_cast_helper(const From& f){
			return bit_cast_helper<To>(static_cast<std::underlying_type_t<From>>(f));
		}
	template<typename To,typename From,std::enable_if_t<std::is_enum_v<From>&&std::is_enum_v<To>&&!std::is_same_v<std::decay_t<To>,std::decay_t<From>>>* = 0>
		constexpr To bit_cast_helper(const From& f){
		return static_cast<To>(static_cast<std::underlying_type_t<From>>(f));
	}
	template<typename To,typename From,std::enable_if_t<std::is_same_v<std::decay<To>,std::decay<From>>>* = 0> constexpr To bit_cast_helper(const From& f){
		return f;
	}
	template<typename To,typename From,std::enable_if_t<std::is_integral_v<To>&&std::is_integral_v<From>&&!std::is_same_v<std::decay<To>,std::decay<From>>>* = 0> constexpr To bit_cast(const From& f){
		return static_cast<To>(f);
	}
}

template<typename To,typename From> constexpr std::enable_if_t<sizeof(To)==sizeof(From)&&std::is_trivially_copyable_v<To>&&std::is_trivially_copyable_v<From>&&std::is_trivially_default_constructible_v<To>,To>
	bit_cast(const From& f){
		return detail::bit_cast_helper<To>(f);
	}

#endif
#undef __HAS_BIT_CAST

#endif /* INCLUDE_LCLIB_BITCAST_HPP_2019_01_03_12_21 */