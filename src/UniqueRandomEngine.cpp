/*
 * UniqueRandomEngine.cpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#include <lclib/UniqueRandomEngine.hpp>

uint64_t detail::seedGenHelper(uint64_t clSalt){
	const auto tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	const static uint64_t cprime{182854975377317};
	static uint64_t prime{242102480888143};
	uint64_t ret{((prime*cprime)>>16)+tp.time_since_epoch().count()*clSalt};
	prime *= cprime;
	prime++;
	return ret;
}

