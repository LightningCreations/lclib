/*
 * Iterators.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_ITERATORS_HPP__2018_10_29_13_15_29
#define __INCLUDE_LCLIB_CXX_ITERATORS_HPP__2018_10_29_13_15_29

#include <type_traits>
#include <lclib/TypeTraits.hpp>
#include <iterator>
#include <utility>
#include <lclib/Types.hpp>

template<typename Itr1,typename Itr2,typename Predicate> struct split{
private:
	Itr1 i1;
	Itr2 i2;
	Predicate pred;
public:
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;
	using iterator_category = std::output_iterator_tag;
	template<typename Pred,typename Itar1,typename Itar2,
		typename=
		std::enable_if_t<
			std::is_same_v<std::decay_t<Predicate>,std::decay_t<Pred>>&&std::is_constructible_v<Predicate,Pred&&>
			&&std::is_same_v<std::decay_t<Itr1>,std::decay_t<Itar1>>&&std::is_constructible_v<Itr1,Itar1&&>
			&&std::is_same_v<std::decay_t<Itr2>,std::decay_t<Itar2>>&&std::is_constructible_v<Itr2,Itar2&&>>>
		constexpr explicit split(Itar1&& i1,Itar2&& i2,Pred&& pred):i1(std::forward<Itar1>(i1)),i2(std::forward<Itar2>(i2)),pred(std::forward<Pred>(pred)){}
	constexpr split& operator++(){
		return *this;
	}
	constexpr split& operator++(int){
		return *this;
	}
	constexpr split& operator*(){
		return *this;
	}
	template<typename T> constexpr auto operator=(T&& e) -> std::void_t<decltype(*(i1++) = std::forward<T>(e)),decltype(*(i2++) = std::forward<T>(e)),decltype(pred(e))>{
		if(pred(e))
			*(i1++) = std::forward<T>(e);
		else
			*(i2++) = std::forward<T>(e);
	}
};

template<typename Itr1,typename Itr2,typename Predicate> split(Itr1,Itr2,Predicate)->split<Itr1,Itr2,Predicate>;

struct discard{
public:
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;
	using iterator_category = std::output_iterator_tag;
	constexpr discard()=default;
	constexpr discard& operator++(){
		return *this;
	}
	constexpr discard& operator++(int){
		return *this;
	}
	constexpr ValueDiscard<> operator*(){
		return {};
	}

};




#endif /* __INCLUDE_LCLIB_CXX_ITERATORS_HPP__2018_10_29_13_15_29 */
