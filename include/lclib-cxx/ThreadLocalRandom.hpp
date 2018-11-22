/*
 * ThreadLocalRandom.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_THREADLOCALRANDOM_HPP__2018_10_15_09_18_43
#define __INCLUDE_LCLIB_CXX_THREADLOCALRANDOM_HPP__2018_10_15_09_18_43
#include <lclib-cxx/Random.hpp>
class TLRPool{
public:
	static thread_local Random rand;
};



#endif /* __INCLUDE_LCLIB_CXX_THREADLOCALRANDOM_HPP__2018_10_15_09_18_43 */
