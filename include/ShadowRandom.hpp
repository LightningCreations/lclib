#ifndef __PrimeRandom_hpp_2018_06_16_22_39
#define __PrimeRandom_hpp_2018_06_16_22_39

#include <Random.hpp>

#include <cstdint>
#include <cstddef>
#include <Config.hpp>


class ShadowRandom final:public Random{
private:
    uint64_t state[32];
    static uint64_t getConstant(size_t);
protected:
    LIBLCFUNC uint32_t next(int);
    LIBLCFUNC void initSeed(const unsigned char*,size_t);
public:
    LIBLCFUNC static uint64_t transform(uint64_t);
    LIBLCFUNC ShadowRandom();
    LIBLCFUNC ShadowRandom(uint64_t);
    LIBLCFUNC void setSeed(uint64_t);
    LIBLCFUNC void seed();
    LIBLCFUNC void nextBytes(uint8_t*,size_t)override;
};


#endif
