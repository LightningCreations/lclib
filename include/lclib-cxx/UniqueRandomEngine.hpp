/*
 * UniqueRandomEngine.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_UNIQUERANDOMENGINE_HPP__2018_10_15_09_27_23
#define __INCLUDE_LCLIB_CXX_UNIQUERANDOMENGINE_HPP__2018_10_15_09_27_23

#include <lclib-cxx/Random.hpp>
#include <chrono>
#include <typeinfo>
#include <lclib-cxx/Hash.hpp>

namespace detail{
	inline uint64_t seedGenHelper(uint64_t clSalt){
		const auto tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
		const static uint64_t cprime{182854975377317};
		static uint64_t prime{242102480888143};
		uint64_t ret{((prime*cprime)>>16)+tp.time_since_epoch().count()*clSalt};
		prime *= cprime;
		prime++;
		return ret;
	}
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
