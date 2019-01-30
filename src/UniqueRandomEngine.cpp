/*
 * UniqueRandomEngine.cpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#include <lclib/UniqueRandomEngine.hpp>
#include <atomic>

uint64_t detail::seedGenHelper(uint64_t clSalt){
	const auto tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	const static uint64_t cprime{182854975377317};
	static std::atomic<uint64_t> prime{242102480888143};
	uint64_t val{prime.load()};
	while(prime.compare_exchange_strong(val,val*cprime+1));
	return val*tp.time_since_epoch().count()+clSalt;
}

