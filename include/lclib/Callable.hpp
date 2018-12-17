/*
 * Callable.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CALLABLE_HPP__2018_09_06_9_27_41
#define _INCLUDE_CALLABLE_HPP__2018_09_06_9_27_41
#include <utility>
#include <type_traits>
#include <mutex>
#include <lclib/Config.hpp>
#include <tuple>
#include <cstddef>
#include <lclib/TypeTraits.hpp>

/**
 * A type satisfying FunctionObject, which models the identity function,
 *  that is, calling the function forwards its argument.
 */
class identity{
public:
	constexpr identity()=default;
	template<typename T> constexpr T&& operator()(T&& t)const noexcept(true){
		return std::forward(t);
	}
};

namespace detail{

	template<typename Func,typename Tuple,std::size_t... Is>
		using rebind_tuple_result = std::tuple<std::invoke_result_t<Func,std::tuple_element<Is,Tuple>>...>;
	template<typename Func,typename Tuple,std::size_t... Is>
		constexpr bool rebind_tuple_nothrow =
					std::conjunction_v<std::is_nothrow_invocable<Func,std::tuple_element<Is,Tuple>>...>;

template<typename Tuple,typename Func,std::size_t... Is>
	auto rebindTuple_helper(Tuple&& t,Func&& f,std::index_sequence<Is...>)
		noexcept(rebind_tuple_nothrow<Tuple,Func,Is...>) -> rebind_tuple_result<Func,Tuple,Is...>
		{
			return std::forward_as_tuple(std::forward<Func>(f)(std::get<Is>(std::forward<Tuple>(t)))...);
		}
}

template<typename Tuple,typename Func>
	auto rebindTuple(Tuple&& t,Func&& f)
		noexcept(noexcept(rebindTuple_helper(std::declval<Tuple>(),std::declval<Func>(),std::make_index_sequence<std::tuple_size_v<Tuple>>{})))
		->decltype(rebindTuple_helper(std::forward<Tuple>(t),std::forward(f),std::make_index_sequence<std::tuple_size_v<Tuple>>{}))
		{
			return rebindTuple_helper(std::forward<Tuple>(t),std::forward(f),std::make_index_sequence<std::tuple_size_v<Tuple>>{});
		}

template<typename T,typename U> constexpr copy_category<U&&,T> forward_from(U&& u,std::remove_reference_t<T>&& val) noexcept{
	return static_cast<copy_category<U&&,T>>(val);
}
template<typename T,typename U> constexpr copy_category<U&&,T> forward_from(U&& u,std::remove_reference_t<T>& val) noexcept{
	return static_cast<copy_category<U&&,T>>(val);
}


#endif /* INCLUDE_CALLABLE_HPP_ */
