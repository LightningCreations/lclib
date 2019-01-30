#include <chrono>
#include <lclib/JTime.hpp>


const int NANO_BOUND = 1000000000;

Instant Instant::now()noexcept{
	return Instant(instant_clock::now());
}

Instant::Instant(now_t)noexcept:Instant(instant_clock::now()){}







