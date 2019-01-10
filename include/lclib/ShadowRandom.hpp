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
    LIBLCHIDE static uint64_t getConstant(size_t);
protected:
    uint32_t next(int);
    void initSeed(const unsigned char*,size_t);
    void do_setSeed(uint64_t);
    void do_nextBytes(uint8_t*,size_t)override;
public:
    static uint64_t transform(uint64_t);
    ShadowRandom();
    ShadowRandom(uint64_t);

    void seed();
};


#endif
