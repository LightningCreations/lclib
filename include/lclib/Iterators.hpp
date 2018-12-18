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

template<typename Itr1,typename Itr2,typename Predicate> struct split{
private:
	Itr1 i1;
	Itr2 i2;
	Predicate pred;
	static_assert(std::is_same_v<typename Itr1::element_type,typename Itr2::element_type>,"Requires Iterators that accept the same type");
public:
	using element_type = typename Itr1::element_type;
	template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<Predicate,Args...>>>
		constexpr explicit split(Itr1 i1,Itr2 i2,Args&&... args):i1(i1),i2(i2),pred(std::forward<Args>()...){}
	constexpr split& operator++(){
		return *this;
	}
	constexpr split& operator++(int){
		return *this;
	}
	constexpr split& operator*(){
		return *this;
	}
	constexpr element_type& operator=(const element_type& e){
		if(pred(e))
			return *(i1++) = e;
		else
			return *(i2++) = e;
	}
	constexpr element_type& operator=(element_type&& e){
		if(pred(e))
			return *(i1++) = std::move(e);
		else
			return *(i2++) = std::move(e);
	}
};

struct discard{
public:
	constexpr discard()=default;
	constexpr discard& operator++(){
		return *this;
	}
	constexpr discard& operator++(int){
		return *this;
	}
	constexpr discard& operator*(){
		return *this;
	}
	template<typename T> constexpr discard& operator=(T&& a){
		return *this;
	}
};




#endif /* __INCLUDE_LCLIB_CXX_ITERATORS_HPP__2018_10_29_13_15_29 */
