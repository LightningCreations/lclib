# Random Number Generation #

Provided by `<lclib/Random.hpp>`, `<lclib/ShadowRandom.hpp>`, `<lclib/ThreadLocalRandom.hpp>`, `<lclib/UniqueRandomEngine.hpp>`.

## LCLib Specification Reference ##

`[lclib.rand]`

An implementation of LCLib shall at the minimum provide a Facility which can generate pseudo-random numbers based on the formula defined for [java.util.Random](https://docs.oracle.com/javase/8/docs/api/java/util/Random.html). 
It shall at a minimum provide equivalents of the `nextInt()`, `nextInt(bound)` `nextLong()`, `nextFloat()`, `nextDouble()`, `nextGaussian()`, `nextBoolean()`, `setSeed(long)` methods defined there. 

All floating point calculations shall occur in fp-strict mode (`[lclib.strictfp]`). 

Generators do not need to be thread-safe, but if multithreading is supported by the target language, it is strongly recommended. Even if the generator is thread-safe, it is not required to be efficient to use across multiple threads.

Implementations are strongly recommended to link to the above document, and provide the Security Warning defined there. 

`[lclib.secure]`

Additionally, implementations of LCLib are also strongly encouraged to provide a facility to generate pseudo-random numbers in cryptographically strong way. It should be possible to provide equivalents of the methods defined above, but the `next()` method is computed in an unspecified manner. The facility shall at least support `nextBytes()`, defined above, except that the way the bytes are generated is unspecified. 

It should be infiesable to derive the internal state of the generator from any one particular result, and therefore at least equally infiesable to derive any subsequent outputs. 

`[lclib.rand.thread]`

Implementations of LCLib may choose to supply a Thread-Local random Number generator which is seeded in an unspecified manner. It is recommended that it is provided in implementations that support multithreading and either the basic Random Number Generator is not thread-safe or is not efficient to use across multiple threads. 

## Other References ##

The Implementation of the Random class provided here is based on the definition of [java.util.Random](https://docs.oracle.com/javase/8/docs/api/java/util/Random.html).  

## Header Synopsis ##

```cpp
#include <lclib/Random.hpp>

class Random{
private:
	Random& operator=(const Random&)=delete;
	Random(const Random&)=delete;
protected:
	std::recursive_mutex lock;
	virtual std::uint32_t next(int);
	virtual void do_nextBytes(std::uint8_t*,std::size_t);
	virtual void do_setSeed(std::uint64_t);
public:
	using result_type = std::uint32_t;
	explicit Random();
	Random(Random&&);
	virtual ~Random();
	explicit Random(std::uint64_t);
	void setSeed(std::uint64_t);
	int nextInt();
	int nextInt(int bound);
	float nextFloat();
	std::int64_t nextLong();
	double nextDouble();
	double nextGaussian();
	bool nextBoolean();
	void nextBytes(std::uint8_t*,std::size_t);
	template<typename Byte,std::size_t N> void nextBytes(Byte(&arr)[N]);
	template<typename Byte,std::size_t N> void nextBytes(std::array<Byte,N>&);
	template<typename Byte> void nextByte(span<Byte>);
	std::uint32_t operator()();
};

#include <lclib/ShadowRandom.hpp>
class ShadowRandom final:public Random{
protected:
	std::uint32_t next(int);
	void do_setSeed(std::uint64_t);
	void do_nextBytes(std::uint8_t*,std::size_t);
public:
	explicit ShadowRandom();
	explicit ShadowRandom(std::uint64_t);
	void seed();
};

#include <lclib/UniqueRandomEngine.hpp>
template<typename T> struct UniqueRandomEngine{
protected:
	static Random& rand();
	~UniqueRandomEngine();
};

#include <lclib/ThreadLocalRandom.hpp>
struct TLRPool{
public:
	static Random& threadLocalRandom();
};
```

## Classes ##

### class Random ###

The Random class is an extendable class which by default generates random numbers based on the specification of java.util.Random, and (As of 1.2) the LCLib Random specification. 

Formally, as of 1.2, `nextGaussian()` is required to execute in FP-Strict mode. 

Random is thread-safe, and this safety is enforced by a protected mutex (which is accessible to the base-class). 
It is unspecified if the variable actually has the type `std::recursive_mutex`, however it must meet the requirements of *Lockable* and it must be well-defined for a single thread to attempt to aquire the lock, even if it already owns it. 

Using the Random class across multiple threads may encounter contention. It is not recommended for Random instances to be shared accross threads. 

(As of 1.3): Random satisfies the requirements of *UniqueRandomBitGenerator* in C++. The `result_type` is `std::uint32_t`. Random::min() is 0 and Random::max() is `std::numeric_limits<std::uint32_t>::max()`. The quality the randomness of bits is approximately uniform. This change was made so that the consistency of the generator guaranteed by LCLib can be used for various distributions provided by both the standard library and external libraries. 

Random does not, and never will, entirely meet the requirements of *RandomNumberEngine*. The primary reason for this is the fact that the requirements of *RandomNumberEngine* specify that all default constructed generators will produce the same sequence of bits, but the Random default constructor seeds itself in a unique way. This will always be the case. 

#### Class Synopsis ####

```cpp
class Random{
private:
	std::uint64_t SEED; // exposition only
	double NEXTNEXTGUASSIAN; // exposition only
	bool HAVENEXTNEXTGUASSIAN; // exposition
	Random& operator=(const Random&)=delete;
	Random(const Random&)=delete;
protected:
	std::recursive_mutex lock;
	virtual std::uint32_t next(int);
	virtual void do_setSeed(std::uint64_t);
	virtual void do_nextBytes(std::uint8_t*,std::size_t);
public:
	using result_type = std::uint32_t;
	explicit Random();
	Random(Random&&);
	virtual ~Random();
	Random(std::uint64_t);
	void setSeed(std::uint64_t);
	int nextInt();
	int nextInt(int bound);
	float nextFloat();
	std::int64_t nextLong();
	double nextDouble();
	double nextGaussian();
	bool nextBoolean();
	void nextBytes(std::uint8_t*,std::size_t);
	template<typename Byte,std::size_t N> void nextBytes(Byte(&arr)[N]);
	template<typename Byte,std::size_t N> void nextBytes(std::array<Byte,N>&);
	template<typename Byte> void nextByte(span<Byte>);
	std::uint32_t operator()();
};
```

#### Constructors/Destructors ####

```cpp
explicit Random():Random(/*unique*){} //(1)
explicit Random(std::uint64_t seed); //(2)
Random(Random&& r)noexcept; //(3)
virtual ~Random(); //(4)
Random& operator=(const Random&)=delete; //(5)
Random(const Random&)=delete; //(6)
```

(1): Automatically seeds the generator with a unspecified, unique seed. It is extraordinarily likely that 2 generators initialized this way (either in the same execution of the program or in distinct executions of the program, possibly with a different implementation) will result in distinct seeds. 
All operations on global variables shall be atomic and may not be locked explicitly, however it is valid to use locking atomic operations. 

(2): Initializes the random number generator to the state predetermined by the given seed. Specifically, this constructor produces a generator that is equivalent to one after a call to setSeed() with the given seed. 

(3): Initializes the random number generator, directly, with the seed that maps the state of r, as well as the nextNextGuassian, if the generator has a nextNextGuassian. This method is not thread safe. It is unspecified if the lock owned by r is moved. 

(4): Virtual Destructor. It is safe to delete an object of a type that extends from random via a pointer to random. 

(5): Instances of Random may not be assigned. It is unspecified if the move assignment operator is explicitly defined as deleted. 

(6): Deleted Copy Constructor. Random is a move-only type (this may change in the future). 

##### LCLib References #####

(1)

`[lclib.rand.autoseed]`

Random Number facilities shall provide a way to initialize the generator to a (usually) unique initial seed, either explicitly or at construction. 

It is unspecified if it is possible to reinitialize the state this way. If the generator is not automatically seeded at construction, it shall be automatically initialized either to an implementation-defined default seed or with this facility before any random bits are generated. 

It is unspecified how this seed is obtained, but there should be incredibly low probability that initializing a generator this way 2 or more times will have each of those generators set to the same state, whether is occurs in a single execution of the program or in distinct executions.  


If the language supports multithreading, the automatic seeding facility shall be thread-safe for different generators. It is unspecified if the automatic seeding facility is thread-safe for the same generator. 

(2)

`[lclib.rand.setseed]`

Random Number Facilities shall provide a way to initialize or reinitialize random number generators to a predetermined state. 

This acts as though atomically (if thread-safety is provided) updating the seed of the generator to the (seed ^ 0x5DEECE66DL)&(0xFFFFFFFFFFFFL), then atomically deleting the nextNextGaussian if the generator has one. 

If thread-safety is provided for a generator, this action shall be safe to perfrom concurrently with calls to `next()` and calls to `nextGaussian()` and the entire operation is bulk. 

The method to reinitialize a generator shall be henseforth called `setSeed(long)`. 

##### Exceptions #####

None of the methods are permitted to throw any exceptions. 
 
 
#### protected methods ####

```cpp
virtual std::uint32_t next(int bits); //(1)
virtual void do_setSeed(std::uint64_t s); //(2)
virtual void do_nextBytes(std::uint8_t* arr,std::size_t sz); //(3)
```

(1): Generates *bits* pseudo-random bits based on the generator. The default implementation  locks the object-wide mutex, then updates SEED to (SEED * 0x5DEECE66DuLL + 0xBuLL) & (0xFFFFFFFFFFFFuLL), and returns std::uint32_t(SEED >> (48-bits));. Bits shall be at most 32 or the behavior is undefined 

(2): Initializes or reinitializes the generator for a given seed. The default implementation atomically updates SEED to (SEED ^ 0x5DEECE66DuLL)&(0xFFFFFFFFFFuLL). 

(3): Generates a sequence of bytes and stores it in arr. The default implementation follows the definition layed out in java.util.Random. [arr,arr+sz) must be a valid range. 


The default implmentations follow the requirements layed out by java.util.Random, except that do_setSeed will not remove the nextNextGuassian, that is handled directly by setSeed. 


##### Exceptions #####

None of the default implementations may throw any exceptions. 

#### Random Number Generation ####

```cpp
int nextInt(); //(1)
int nextInt(int bound); //(2)
float nextFloat(); //(3)
std::int64_t nextLong(); //(4)
double nextDouble(); //(5)
double nextGuassian(); //(6)
bool nextBoolean(); //(7)
void nextBytes(std::uint8_t* arr,std::size_t sz); //(8)
template<typename Byte,std::size_t N> void nextBytes(Byte(&arr)[N]); //(10)
template<typename Byte,std::size_t N> void nextBytes(std::array<Byte,N>& arr); //(11)
template<typename Byte> void nextBytes(span<Byte> s); //(12)
```

(1): Generates a random integer in [0,2^31) and returns it. Equivalent to next(31).  

(2): Generates a random integer in [0,bound) and returns it. The algorithm is described below. 

(3): Generates a random floating point number i nn0[ n

