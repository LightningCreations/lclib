/*
 * BitCast.hpp
 *
 *  Created on: Jan 3, 2019
 *      Author: chorm
 */

#ifndef INCLUDE_LCLIB_BITCAST_HPP_2019_01_03_12_21
#define INCLUDE_LCLIB_BITCAST_HPP_2019_01_03_12_21

#include <lclib/Config.hpp>

#if defined(__cpp_bit_cast) && __cpp_bit_cast >= 201806
#include <bit>
using std::bit_cast;
#define __HAS_BIT_CAST
#endif
#ifndef __HAS_BIT_CAST
#include <cstddef>
#include <new>
#include <type_traits>


template<typename To,typename From> constexpr std::enable_if_t<sizeof(To)==sizeof(From)&&std::is_trivially_copyable_v<To>&&std::is_trivially_copyable_v<From>&&std::is_trivially_default_constructible_v<To>,To>
	bit_cast(const From& f){
		if constexpr(std::is_enum_v<From>){
			if constexpr(std::is_same_v<To,std::underlying_type_t<From>>)
				return static_cast<std::underlying_type_t<From>>(f);
			else
				return bit_cast<To>(static_cast<std::underlying_type_t<From>>(f));
		}else if constexpr(std::is_enum_v<To>){
			if constexpr(std::is_same_v<From,std::underlying_type_t<To>>)
				return static_cast<To>(f);
			else
				return static_cast<To>(bit_cast<std::underlying_type_t<From>>(f));
		}else if constexpr((std::is_integral_v<To>)&&std::is_integral_v<From>)
			return static_cast<To>(f);
		else{
			union{
				From f;
				To t;
			} u = {f};
			return *std::launder(static_cast<To*>(const_cast<void*>(static_cast<const void*>(&u.t))));
		}
	}

#endif
#undef __HAS_BIT_CAST

#endif /* INCLUDE_LCLIB_BITCAST_HPP_2019_01_03_12_21 */
