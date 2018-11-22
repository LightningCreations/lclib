#ifndef Random_hpp_2018_02_26_08_26
#define Random_hpp_2018_02_26_08_26
#pragma once
#include <cstdint>
#include <ctime>
#include <vector>
#include <cmath>
#include <mutex>
#include <thread>
#include <array>
#include <lclib/Config.hpp>
#include <lclib/TypeTraits.hpp>
typedef uint64_t seed_t;

using std::recursive_mutex;
using std::try_lock;
using std::array;

/**
	Random Number Generator Definition provided by java standard library:
	Generator location is: https://docs.oracle.com/javase/8/docs/api/java/util/Random.html
	This file follows the specification for the Random class defined by that document.
	As per the specification this class comes with 2 Warnings:
	The Random class is thread-safe, however using an instance of Random across multiple threads may have unpredictable effects.
	The Random class is not cryptographically-secure.
	See the warnings on https://docs.oracle.com/javase/8/docs/api/java/util/Random.html for more details on these warnings
	Code implementation provided by Connor Horman.
	Compatibility Notice: Instances of the Random Class given the same seed,
	with the same method calls as an instances of java.util.Random will generate the same sequence of return values.
	Instances of the Random class cannot be copied (but can be Moved).
	The Specification of the Random class detailed in the documentation is effectively the same as the one in the above documentation.
*/

class LIBLCAPI Random{
private:
	seed_t seed;
	double nextNextGaussian;
	bool haveNextNextGaussian;	
    Random(const Random&)=delete;
    Random& operator=(const Random&)=delete;
protected:
    /**
     * Generates the next bits in the sequence.
     * This method acts is by atomically updating the seed to (seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1),
     * and returning (seed >> (48-bits))
     */
    virtual uint32_t next(int bits);
    recursive_mutex lock;
public:
	/*
	    Constructs a Pseudorandom Number generator.
        This generate seeds the generator with a seed that is almost guarenteed to be unique from other 
        invocations of this constructor
	*/
    explicit Random();
    /*
        Constructs a Pseudorandom Number Generator with a given seed.
        The Pseudorandom Number Generator is initialized with the seed as though it was passed to setSeed.
    */
    explicit Random(seed_t);
    Random(Random&&)=default;
    virtual ~Random()=default;
    /*
        Sets the Seed of the Pseudorandom Number Generator.
        This method also has the side effect of clearing the nextNextGaussian.
        If the same sequence of methods are executed from this Pseudorandom Number generator with the same seed,
        the outputs are the same. (This guarentee also applies to Pseudorandom Number generators using the java implementation:
        java.util.Random)
        This method acts as if by atomically updating the seed to (s^0x5DEECE66DLL)&((1LL<<48)-1),
        and clearing the haveNextNextGaussian flag.
    */
    virtual void setSeed(seed_t);
    
    /*
        Obtains the next int in the pseudorandom number generator
        The output of the generator is a value from 0 to 2147483647.
    */
    int nextInt();
    
    /*
        Obtains the next int in the pseudorandom number generator
        The output of this function is a value from 0 inclusive to the bound exclusive.
        If bound is negative or 0, then the method throws an exception.
    */
    int nextInt(int);
    /*
        Obtains a double from the pseudorandom number generator using Guassian/Normal distribution.
        The distribution has mean 0 and standard deviation 1. The numbers generated are extremely like to be close to 0,
        and extremely unlikely to be close to +/- Infinity.
        If the generated numbers need (approximately) uniform distribution, the nextDouble() function is more
        appropriate for use.
    */
    double nextGuassian();
    /*
        Obtains the next Long produced from this psuedorandom number generator.
        Note that because the generator only uses a 48-bit seed, the function may not
        produce all possible long values.
    */
    int64_t nextLong();
    /*
        Obtains a pseudorandom floating-point number from 0 inclusive to 1 exclusive.
        Approximately every real number(that can be represented by the float type) in this range is generated with equal probability
    */
    float nextFloat();
    /*
        Obtains a pseudorandom floating-point number from 0 inclusive to 1 exclusive
        Approximately every real number(that can be represented by the double type) in this range are generated with equal probability.
        If Normal Distribution doubles are required the nextGuassian() function is more appropriate for use.
    */
    double nextDouble();
    /*
        Obtains a number of psuedorandom bytes and fills the passed buffer with them.
    */
    virtual void nextBytes(uint8_t*,size_t);
    /*
        Fills an array with its size known at compile time with a bunch of psuedo-random bytes.
    */
	template<typename byte, size_t size> std::enable_if_t<is_byte_v<byte>> nextBytes(array<byte,size>& bytes){
        this->nextBytes(bytes.data(),size);
    }
	template<typename byte,size_t N> std::enable_if_t<is_byte_v<byte>> nextBytes(byte(&arr)[N]){
		this->nextBytes(reinterpret_cast<uint8_t*>(arr),N);
	}
	/**
	 * Obtains a Pseudorandom boolean that is (approximately) equal in probability of being true or false
	 */
	bool nextBoolean();
	
};



#endif
