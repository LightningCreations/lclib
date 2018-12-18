/*
 * UniqueRandomEngine.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_UNIQUERANDOMENGINE_HPP__2018_10_15_09_27_23
#define __INCLUDE_LCLIB_CXX_UNIQUERANDOMENGINE_HPP__2018_10_15_09_27_23

#include <lclib/Random.hpp>
#include <chrono>
#include <typeinfo>
#include <lclib/Hash.hpp>

namespace detail{
	uint64_t seedGenHelper(uint64_t clSalt);
}

/**
 * A type which provides a Unique Random Number generator for a class.
 * Intended for use with CRTP
 */
template<typename T> struct UniqueRandomProvider{
private:
	static uint64_t classSeedGenerator(){
		return detail::seedGenHelper(hashcode(typeid(T)));
	}
public:
	static Random& rand(){
		static Random r{classSeedGenerator()};
		return r;
	}
};



#endif /* __INCLUDE_LCLIB_CXX_UNIQUERANDOMENGINE_HPP__2018_10_15_09_27_23 */
