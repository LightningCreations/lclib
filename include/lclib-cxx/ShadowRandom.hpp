#ifndef __PrimeRandom_hpp_2018_06_16_22_39
#define __PrimeRandom_hpp_2018_06_16_22_39
#include <lclib-cxx/Config.hpp>
#include <lclib-cxx/Random.hpp>

#include <cstdint>
#include <cstddef>



class LIBLCAPI ShadowRandom final:public Random{
private:
    uint64_t state[32];
    LIBLCHIDE static uint64_t getConstant(size_t);
protected:
    uint32_t next(int);
    void initSeed(const unsigned char*,size_t);
public:
    static uint64_t transform(uint64_t);
    ShadowRandom();
    ShadowRandom(uint64_t);
    void setSeed(uint64_t);
    void seed();
    void nextBytes(uint8_t*,size_t)override;
	using Random::nextBytes;
};


#endif
