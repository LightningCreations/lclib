/*
 * ThreadLocalRandom.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_THREADLOCALRANDOM_HPP__2018_10_15_09_18_43
#define __INCLUDE_LCLIB_CXX_THREADLOCALRANDOM_HPP__2018_10_15_09_18_43
#include <lclib/Random.hpp>
class TLRPool final{
private:
	~TLRPool()=delete;
	TLRPool()=delete;
	TLRPool(const TLRPool&)=delete;
public:
	static Random& threadLocalRandom();
};



#endif /* __INCLUDE_LCLIB_CXX_THREADLOCALRANDOM_HPP__2018_10_15_09_18_43 */
