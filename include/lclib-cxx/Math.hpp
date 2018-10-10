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
#include <lclib-cxx/Callable.hpp>

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
	constexpr static std::complex<T> value = std::numeric_limits<T>::epsilon();
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

template<typename type> constexpr auto integrate(type lower,type upper){
	return [lower,upper](auto f)->std::invoke_result_t<decltype(f),type>{
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
	using std::tgamma;
	constexpr Gamma()=default;
	constexpr Gamma(const Gamma&)=default;
	constexpr Gamma& operator=(const Gamma&)=default;

	template<typename type> constexpr auto operator()(type x)const{
		return tgamma(x);//(Use ADL if applicable, otherwise std::tgamma
	}
};

template<typename Fn1,typename Fn2> constexpr auto convolution(Fn1&& f,Fn2&& g){
	return [f,g](auto t)->decltype(integrate(-inf<decltype(t)>,inf<decltype(t)>)([](auto t){return t;})){
		using type = decltype(t);
		return integrate(-inf<type>,inf<type>)([f,g,t](type x)->type{
			return f(x)*g(t-x);
		});
	};
}

#endif /* __INCLUDE_LCLIB_CXX_MATH_HPP__2018_10_09_19_11_18 */
