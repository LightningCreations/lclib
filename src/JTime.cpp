#include <chrono>
#include <lclib-cxx/JTime.hpp>


const extern int32_t hashPrime;

const int NANO_BOUND = 1000000000;




Instant Instant::now(){
	return Instant(instant_clock::now());
}

Instant::Instant(now_t):Instant(instant_clock::now()){}







