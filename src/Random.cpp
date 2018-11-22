#include <lclib/Random.hpp>
#include <chrono>
#include <algorithm>


static seed_t number{876730097};
const seed_t cprime{2227123637};


static seed_t highResTime(){
	const auto tp{std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())};
	return tp.time_since_epoch().count();
}

static seed_t nextMultiplier(){
	seed_t val = number;
	number *= cprime;
	number += 1;
	return val*cprime;
}
LIBLCHIDE seed_t genUniqueSeed(){
	return highResTime()*nextMultiplier();
}
LIBLCHIDE seed_t initRandomizeSeed(seed_t seed){
	return (seed ^ 0x5DEECE66DL) & ((1L << 48) - 1);
}

uint32_t Random::next(int bits){
	std::lock_guard<recursive_mutex> sync(lock);
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
	return (int)(seed >> (48 - bits));
}
Random::Random():lock(){
	setSeed(genUniqueSeed());
	
}
Random::Random(seed_t s):lock(){
	setSeed(s);
}
void Random::setSeed(seed_t seed){
	std::lock_guard<recursive_mutex> sync(lock);
	this->seed = initRandomizeSeed(seed);
	this->haveNextNextGaussian = false;
}

int Random::nextInt(){
	return next(31);
}
int Random::nextInt(int bound){
	if(bound<=0)
		throw "Bound must greater then 0";

	if ((bound & -bound) == bound)  // i.e., bound is a power of 2
		return (int)((bound * (seed_t)next(31)) >> 31);

	int bits, val;
	do {
		bits = next(31);
		val = bits % bound;
	} while (bits - val + (bound-1) < 0);
		return val;
}

double Random::nextGuassian(){
	std::lock_guard<recursive_mutex> sync(lock);
	double ret;
	if (haveNextNextGaussian) {
			haveNextNextGaussian = false;
			return nextNextGaussian;
	} else {
		double v1, v2, s;
		do {
			v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
			v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
			s = v1 * v1 + v2 * v2;
		} while (s >= 1 || s == 0);
		double multiplier = sqrt(-2 * log(s)/s); // @suppress("Invalid arguments")
		nextNextGaussian = v2 * multiplier;
		haveNextNextGaussian = true;
		return v1 * multiplier;
	}
}
int64_t Random::nextLong(){
	return nextInt()<<32LL+nextInt();
}

float Random::nextFloat(){
	return  next(24) / ((float)(1 << 24));
}

double Random::nextDouble(){
	return ((uint64_t)next(26) << 27 + next(27))/((double)(1L << 53));
}

void Random::nextBytes(uint8_t* out,size_t size){
	for (int i = 0; i < size; )
		for (int rnd = nextInt(), n = std::min<size_t>(size - i, 4);
        	  n-- > 0; rnd >>= 8)
      		 out[i++] = (char)rnd;
}

bool Random::nextBoolean(){
	return next(1)!=0;
}


