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
#include <lclib/Callable.hpp>
#include <lclib/Config.hpp>
#include <lclib/TypeTraits.hpp>
#include <lclib/Detectors.hpp>

/**
 * Template metaclass that is similar to integral-constant.
 * Specializations are provided for all types T
 *  for which std::numeric_limits<T>::epsilon() is defined (and non-zero),
 *  and instantiations of std::complex for those types.
 *  Programs may specialize dx_value for user defined types (provided they satisfy literal type)
 *  (however it may be recommended that they specialize std::numeric_limits instead).
 *  If dx_value is specialized, it must satisfy the following requirements:
 *  Given dx_value<T>, a specialization of dx_value,
 *  and c, a possibly const lvalue or rvalue of dx_value<T>:
 *  \\dx_value<T>::type must be defined as a type alias to dx_value<T>
 *  \\dx_value<T>::value a constant expression of type T,
 *     which represents the difference between the value 1 in that type,
 *      and the lowest representable value greater than 1.
 *  \\dx_value<T>{}: a rvalue constant expression of dx_value<T>
 *  \\dx_value<T>{c}: a rvalue constant expression of dx_value<T>
 *  \\T{c}: a constant expression of T, which is precisely dx_value<T>::value. (this may be an implicit conversion)
 *  \\c(): a constant expression of T, which is precisely dx_value<T>::value
 */
template<typename T,typename=void> struct dx_value{};
template<typename T> struct dx_value<T,
	std::void_t<decltype(std::numeric_limits<T>::epsilon()),
		std::enable_if_t<std::numeric_limits<T>::epsilon()!=0>
	>>{
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

/**
 * The smallest representable difference between values of T.
 * Defined as dx_value<T>::value if it exists.
 * Equivalent with lim[x->0+](x)
 * -dx is equivalent with lim[x->0-](x)
 */
template<typename T> constexpr T dx = dx_value<T>::value;
/**
 * The largest representable value of T.
 * Defined as 1/dx<T> if it exists
 * Equivalent with lim[x->∞](x).
 * -inf is equivalent with lim[x->-∞](x).
 * Should always be used in place of actual infinities.
 */
template<typename T> constexpr T inf = 1/dx<T>;

namespace detail{
	template<typename Fn> using d_detector = decltype(std::declval<Fn>().d());
	template<typename Fn>
		constexpr auto d(Fn&& f)->require_detected<d_detector,Fn>{
			return f.d();
		}
	template<typename Fn> constexpr auto d(Fn&& f){
		return [f](auto x)->std::invoke_result_t<Fn,decltype(x)>{
			using type = decltype(x);
			return (f(x+dx<type>)-f(x))/dx<type>;
		};
	}
}


/**
 * Returns a function of x, which is equivalent to (df/dx)(f).
 * If adl lookup finds a definition of d for Fn, returns the result of that.
 * Otherwise, if Fn has a member function d()
 * Otherwise, returns g(x) where g(x)=lim[h->0+](f(x+h)-f(x))/(h)
 *
 * Requires: Fn is Differentiable
 */
template<typename Fn> constexpr auto differentiate(Fn&& f){
	using detail::d;
	return d(f);
}


namespace detail{
	namespace _k{
		template<typename Fn,typename type> using intg_detector = decltype(std::declval<Fn>().intg(std::declval<type>(),std::declval<type>()));
		template<typename Fn,typename type> auto intg(Fn&& f,type low,type high)->require_detected<intg_detector,Fn,type>{
			return f.intg(low,high);
		}
		template<typename Fn,typename type> auto intg(Fn&& f,type low,type high)->std::invoke_result_t<Fn,type>{
			type d = 0;
			for(type i = 1;i<=(1/dx<type>);i++){
				d += (f((high-low)*(dx<type>)*i))*(high-low)*dx<type>;
			}
		}
	}
	template<typename Fn> struct IndefIntF{
	private:
		Fn f;
	public:
		using _k::intg;
		IndefIntF(Fn&& f):f(std::forward(f)){}
		template<typename type> auto operator()(type x)->decltype(f(x)){
			intg(f,0,x);
			return d;
		}
		auto d()const{
			return f;
		}
	};

}
/**
 * Returns a function that integrates annother function from lower to upper.
 * Calling the result with a function has the following effects:
 * First if adl lookup finds intg for the type of the function, then call it with f,lower,upper.
 * Otherwise, if the function has an accessible member function intg(), call it for f with lower,upper.
 * Otherwise, use the limit definition of an integral.
 */
template<typename type> constexpr auto integrate(type upper,type lower){
	using detail::_k::intg;
	return [upper,lower](auto&& f){
		return intg(f,lower,upper);
	};
}

/**
 * Returns a function that when called with an x, integrates f over 0 to x.
 * The resultant function integrates f in the same manner as described in f.
 * Differentiating the resultant function gives f.
 * \Requirements: f must be integratable for some type.
 *  the resultant function is callable for all types for which Fn is integratable with.
 */
template<typename Fn> constexpr auto antiderivative(Fn&& f){
	return detail::IndefIntF<Fn>{f};
}

/**
 * template function wrapper for the Gamma Function.
 * Wraps a tgamma function found with ADL, or std::tgamma if not applicable.
 */
struct Gamma{
public:
	using std::tgamma;
	constexpr Gamma()=default;
	constexpr Gamma(const Gamma&)=default;
	constexpr Gamma& operator=(const Gamma&)=default;

	template<typename type> constexpr auto operator()(type x)const ->decltype(tgamma(x)){
		return tgamma(x);//(Use ADL if applicable, otherwise std::tgamma)
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

template<typename Fn1,typename Fn2> constexpr auto compose(Fn1&& f,Fn2&& g){
	return [f,g](auto x)->decltype(f(g(x))){
		return f(g(x));
	};
}
template<typename Fn1,typename Fn2> constexpr auto multiply(Fn1&& f,Fn2& g){
	return [f,g](auto x)->decltype(f(x)*g(x)){
		return f(x)*g(x);
	};
}
template<typename Fn1,typename Fn2> constexpr auto add(Fn1&& f,Fn2&& g){
	return [f,g](auto x)->decltype(f(x)+g(x)){
		return f(x)+g(x);
	};
}



template<typename type> constexpr auto sum(type start,type max) EXPECTS(ceil(max-start)==(max-start)){
	return [start,max](auto&& f)->std::invoke_result_t<decltype(f),type>{
		std::invoke_result_t<decltype(f),type> s = 0;
		for(type t = start;t<max;t++)
			s += f(t);
		return s;
	};
}
template<typename type> constexpr auto product(type start,type max) EXPECTS(ceil(max-start)==(max-start)){
	return [start,max](auto&& f)->std::invoke_result_t<decltype(f),type>{
		std::invoke_result_t<decltype(f),type> p = 1;
		for(type t = start;t<max;t++)
			p *= f(t);
		return p;
	};
}



struct tetration{
	using std::pow;
	constexpr tetration()=default;
	constexpr tetration(const tetration&)=default;
	constexpr tetration& operator=(const tetration&)=default;
	template<typename type> constexpr type operator()(type val,int rep)const{
		type t = val;
		for(int i =1;i<rep;i++)
			t = pow(val,t);
		return t;
	}
};



#endif /* __INCLUDE_LCLIB_CXX_MATH_HPP__2018_10_09_19_11_18 */
