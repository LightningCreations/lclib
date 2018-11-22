/*
 * Detectors.hpp
 *
 *  Created on: Oct 4, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_DETECTORS_HPP__2018_10_04_09_52_04
#define __INCLUDE_LCLIB_CXX_DETECTORS_HPP__2018_10_04_09_52_04
#include <type_traits>
#include <lclib/TypeTraits.hpp>

struct nonesuch{};

namespace detail{
	template<typename Default,typename AlwaysVoid,template<typename...> class Op,
		class... Args> struct detector{
		using value_t = std::false_type;
		using type = Default;
	};
	template<typename Default,template<typename...> class Op,class... Args> struct
		detector<Default,std::void_t<Op<Args...>>,Op,Args...>{
		using value_t = std::true_type;
		using type = Op<Args...>;
	};
}

template<template<typename...> class Op,class... Args> using is_detected = typename detail::detector<nonesuch,void,Op,Args...>::value_t;
template<template<typename...> class Op,class... Args> using detected_t = typename detail::detector<nonesuch,void,Op,Args...>::type;
template<typename Default,template<typename...> class Op,class... Args> struct detected_or: detail::detector<Default,void,Op,Args...>{};

template<template<typename...> class Op,class... Args> constexpr const bool is_detected_v = is_detected<Op,Args...>::value;
template<typename Default,template<typename...> class Op,class... Args> using detected_or_t = typename detected_or<Default,Op,Args...>::type;
template<typename Expected,template<typename...> class Op,class... Args> using is_detected_exact = typename std::is_same<Expected,detected_t<Op,Args...>>::type;
template<typename Expected,template<typename...> class Op,class... Args> using is_detected_convertible = typename std::is_convertible<Expected,detected_t<Op,Args...>>::type;

template<typename Expected,template<typename...> class Op,class... Args> constexpr const bool is_detected_exact_v = is_detected_exact<Expected,Op,Args...>::value;
template<typename Expected,template<typename...> class Op,class... Args> constexpr const bool is_detected_convertible_v = is_detected_convertible<Expected,Op,Args...>::value;

template<template<typename...> class Op,class... Args> using require_detected = std::enable_if<is_detected_v<Op,Args...>,detected_t<Op,Args...>>;
template<template<typename...> class Op,class... Args> using require_detected_t = typename require_detected<Op,Args...>::type;
#endif /* __INCLUDE_LCLIB_CXX_DETECTORS_HPP__2018_10_04_09_52_04 */
