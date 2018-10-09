/*
 * Math.hpp
 *
 *  Created on: Oct 9, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_MATH_HPP__2018_10_09_19_11_18
#define __INCLUDE_LCLIB_CXX_MATH_HPP__2018_10_09_19_11_18
#include <limits>
#include <type_traits>
#include <cmath>
#include <complex>

template<typename T,typename=void> struct dx_value{};
template<typename T> struct dx_value<T,std::void_t<decltype(std::numeric_limits<T>::epsilon())>>{
public:
	typedef dx_value type;
	constexpr static T value = std::numeric_limits<T>::epsilon();
	constexpr dx_value()=default;
	constexpr dx_value(const dx_value&)=default;
	constexpr dx_value& operator=(const dx_value&)=default;
	constexpr operator T()const{
		return value;
	}
	constexpr T operator()()const{
		return value;
	}
};
template<typename T> struct dx_value<std::complex<T>,std::void_t<decltype(dx_value<T>::value)>>{
public:
	typedef dx_value type;
	constexpr static std::complex<T> value = std::numeric_limits<T>::epsilon()+0i;
	constexpr dx_value()=default;
	constexpr dx_value(const dx_value&)=default;
	constexpr dx_value& operator=(const dx_value&)=default;
	constexpr operator std::complex<T>()const{
		return value;
	}
	constexpr std::complex<T> operator()()const{
		return value;
	}
};

template<typename T> constexpr T dx = dx_value<T>::value;
template<typename T> constexpr T inf = 1/dx<T>;


template<typename Fn> constexpr auto differentiate(Fn&& f){
	return [f](auto x)->std::invoke_result_t<Fn,decltype(x)>{
		using type = decltype(x);
		return (f(x+dx<type>)-f(x))/dx<type>;
	};
}

template<typename type,typename Fn> constexpr auto integrate(type lower,type upper,Fn&& f){
	return [lower,upper,f]()->std::invoke_result_t<Fn,type>{
		type d = 0;
		for(type i = 1;i<=(1/dx<type>);i++){
			d += (f((upper-lower)*(dx<type>)*i))*(upper-lower)*dx<type>;
		}
	};
}

template<typename Fn> constexpr auto antiderivative(Fn&& f){
	return [f](auto x)->std::invoke_result_t<Fn,decltype(x)>{
		using type = decltype(x);
		type d = 0;
		for(type i = 1;i<=inf<type>;i++){
			d += f(x*(dx<type>)*i)*x*dx<type>;
		}
	};
}

struct Gamma{
public:
	constexpr Gamma()=default;
	constexpr Gamma(const Gamma&)=default;
	constexpr Gamma& operator=(const Gamma&)=default;

	template<typename type> constexpr type operator()(type x)const{
		if(x>0&&std::round(x)==x){
			type ret = 1;
			for(type t = 1;t<x;t++)
				ret *= t;
			return ret;
		}
		return integrate(0,inf,[x](type t){
			return std::pow(t,x-1)*std::exp(-t);
		})();
	}
};

#endif /* __INCLUDE_LCLIB_CXX_MATH_HPP__2018_10_09_19_11_18 */
