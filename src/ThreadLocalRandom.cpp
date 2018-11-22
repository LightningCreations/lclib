/*
 * ThreadLocalRandom.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: connor
 */
#include <lclib-cxx/ThreadLocalRandom.hpp>
#include <lclib-cxx/Random.hpp>
#include <chrono>
#include <thread>
#include <functional>

static uint64_t genTLRSeed(){
	static uint64_t number{19884305634001};
	static const uint64_t cprime{154431600453379};
	const auto tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	const auto tidH = std::hash<std::thread::id>{}(std::this_thread::get_id());
	const uint64_t ret = (tp.time_since_epoch().count()*number+tidH);
	number *= cprime;
	number += 1;
	return ret;
}

thread_local Random TLRPool::rand{genTLRSeed()};



