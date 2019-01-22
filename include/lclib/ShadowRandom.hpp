#ifndef __PrimeRandom_hpp_2018_06_16_22_39
#define __PrimeRandom_hpp_2018_06_16_22_39
#include <lclib/Config.hpp>
#include <lclib/Random.hpp>

#include <cstdint>
#include <cstddef>



class LIBLCAPI ShadowRandom final:public Random{
private:
    uint64_t state[32];
    bool is_seeded;
    void initSeed(const unsigned char*,size_t);
    static uint64_t getConstant(size_t);
    static uint64_t transform(uint64_t);
protected:
    uint32_t next(int);
    void do_setSeed(uint64_t);
    void do_nextBytes(uint8_t*,size_t)override;
public:
    ShadowRandom();
    ShadowRandom(uint64_t);
    void seed();
};


#endif
