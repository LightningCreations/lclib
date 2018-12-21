/*
 * ThreadLocalRandom.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: connor
 */
#include <lclib/ThreadLocalRandom.hpp>
#include <lclib/Random.hpp>
#include <chrono>
#include <thread>
#include <functional>
#include <atomic>

static uint64_t genTLRSeed(){
	static std::atomic<std::uint64_t> number{19884305634001};
	static const uint64_t cprime{154431600453379};
	const auto tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	const auto tidH = std::hash<std::thread::id>{}(std::this_thread::get_id());
	uint64_t val{number.load()};
	while(number.compare_exchange_strong(val, val*cprime+1));
	return val*tp.time_since_epoch().count()+tidH;
}

Random& TLRPool::threadLocalRandom() {
	thread_local static Random val{ genTLRSeed() };
	return val;
}



