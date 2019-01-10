# Time Library #

Defined in `<lclib/JTime.hpp>`




## Synopsis ##

```cpp
#include <chrono>

#define HAS_CXX20_CLOCK

enum class ChronoUnit;
using seconds_t = /*unspecified*/;
using nanos_t = /*unspecified*/;
using chrono_val_t = /*unspecified*/;
using seconds_duration = std::chrono::duration</*unspecified*/,std::ratio<1,1>>;
using nanos_duration = std::chrono::duration</*unspecified*/,std::ratio<1,1000000000>>;
using base_duration = std::chrono::duration<chrono_val_t,/*unspecified*/>>;
using max_duration = std::chrono::duration<chrono_val_t,/*unspecified*/>>;
constexpr seconds_t toSeconds(chrono_val_t,ChronoUnit);
constexpr nanos_t toNanos(chrono_val_t,ChronoUnit);
constexpr nanos_t toNanos(unsigned long long,ChronoUnit);
constexpr chrono_value_t truncateTo(seconds_t,nanos_t,ChronoUnit);
constexpr nanos_t NANOS_PER_SECOND;
using instant_clock = std::chrono::system_clock;
struct now_t;
const now_t now;
class Duration;
class Instant;
constexpr Instant EPOCH;
constexpr Instant MAX;
constexpr Instant MIN;
constexpr Duration ZERO;
constexpr Duration MAX_DURATION;
constexpr Duration MIN_DURATION;
constexpr Duration LOWEST_DURATION;
constexpr int32_t hashcode(seconds_t);
constexpr int32_t hashcode(nanos_t);
constexpr int32_t hashcode(const Duration&);
constexpr int32_t hashcode(const Instant&);
namespace std{
	template<> struct hash<Duration>;
	template<> struct hash<Instant>;
	template<> struct numeric_limits<Duration>;
	template<> struct numeric_limits<Instant>;
	template<> struct numeric_limits<seconds_t>;
	template<> struct numeric_limits<nanos_t>;
};
```

## Overview ##

The Time Library, along with the STL `<chrono>` library, is provided to interact with time. 

Note that the Time library is primarily for Interoperation. The standard implementation should be used in most circumstances. 

## Macros ##

### HAS_CXX20_CLOCK ###

Since 1.3

If defined, then the implementation supports the changes to the chrono library made in C++20. 
While this macro is defined and `__cplusplus` is greater than 201703L, the Time Library operates in C++20 mode, otherwise the time library operates in C++17 mode. 

If this macro is undefined, the implementation may still partially support the changes (for example, conversion from time_points of various clocks to Instants). 


## Type definitions ##

### seconds_t ###


A type that satisfies TriviallyCopyable, StandardLayout, LiteralType, BytesWriteable, *Numeric* (as of 1.3), and an integral value in the range [-31556889864401400,31556889864401400) is implicitly convertible to this type. 

`seconds_t` is not Required to be a user-defined type, but if it is, `std::numeric_limits` must be specialized for it. 

Can be written to a DataOutputStream and read from a DataInputStream as a long value

### nanos_t ###

A type that satisfies *TriviallyCopyable*, *StandardLayout*, *LiteralType*, and *BytesWriteable*, *Numeric* (as of 1.3), and an integral value in the range [0,1000000000) is implicitly convertible to this type. As of 1.3, it shall be possible to convert any value of type `unsigned` explicitly, using a `static_cast` to `nanos_t`. The the value overflows the range, then the resultant value shall be congruent to the initial value (modulo 1000000000). If a value that is not representable as a value of type `unsigned`, is converted to `nanos_t` this way, the behavior is undefined. 

`nanos_t` is not required to be a user-defined type, but if it is, `std::numeric_limits` must be specialized for it.

Can be written to a DataOutputStream and read from a DataInputStream as an int value.

### chrono_val_t ##

A signed integer type that is at least 64-bit. 

### seconds_duration ###

A specialization of `std::chrono::duration` that can store any valid seconds field of a Duration. 

### nanos_duration ###

A specialization of `std::chrono::duration` that can store any valid nanos field of a Duration. 

### base_duration ###

A specialization of `std::chrono::duration` that can store at least `chrono_val_t` of an implementation-defined period that is a some fraction of seconds or is seconds. 

It is also guaranteed that a value of type `base_duration` can store any valid value of `seconds_duration` without loss of precision, truncation, or overflow. 

`std::chrono::treat_as_floating_point_v<base_duration::rep>` must be false. 

### max_duration ###

An implementation-defined specialization of `std::chrono::duration` that has the best period the implementation can deal with, represented using `chrono_val_t`. Guaranteed to be at least as precise as `base_duration`. 


### instant_clock ###

As of 1.3: The clock type used by instants. Defined as a clock for which has the date and time 1970-01-01T00:00:00.000Z (the unix epoch) as its epoch, and the minimum interval between reported instants is at least `base_duration`. 

`instant_clock` shall satisfy *TrivialClock*. Specifically, it cannot alias `std::chrono::utc_clock` in C++20 mode. 

In C++20 mode, this shall alias `std::chrono:system_clock` or have a conversion to `std::chrono::system_clock`.

Until 1.3: Defined as `std::chrono::system_clock`. If `std::chrono::system_clock::epoch()` does not represent the unix epoch the behavior of calling any methods of Instant which involve this clock, including the now factory method and the now constructor, is undefined. In C++20 mode, this is guaranteed by the standard. 

### enum class ChronoUnit ###

A set of units which direct conversions from `chrono_val_t` to `seconds_t` and `nanos_t`. 

These units correspond to valid periods of durations with Rep being `chrono_val_t`.

#### Synopsis ####

```cpp
enum class ChronoUnit{
	NANOSECONDS,
	MICROSECONDS,
	MILISECONDS,
	SECONDS,
	MINUTES,
	HOURS,
	/* Additional Units */
}
```

Note: Only `SECONDS`, `MINUTE`, and `HOURS` are required. 
`MILISECONDS`, `MICROSECONDS`, and `NANOSECONDS` are only required if the period of `max_duration` is at least as precise as `std::ratio<1,1000>`, `std::ratio<1,1000000>`, and `std::ratio<1,1000000000>` respectively. 
Other implementation defined units may be supported by the implementation.  

### struct now_t ###

A Tag type that specifies to construct Instants from the current wall clock time. 

### class Duration ###

A class that represents a Duration of time.  The Duration is stored as a `seconds_t` number of seconds and a `nanos_t` number of additional nanoseconds. 

The Maximum range of a Duration is [-31556889864401400s,31556889864401400s), which is the same as the minimum range for `seconds_t`, except fractions of a second may be stored, down to nanosecond resolution. 

Duration satisfies *TriviallyCopyable*, *StandardLayoutType*, *LiteralType*, and *Numeric* (as of 1.3). Duration additionally satisfies *BytesReadable* and *BytesWriteable*, until the requirements changed in 1.3 to forbidden private/protected members. 

Duration overloads all comparison operators (even though the definition only contains operator<) as though it inherits privately from but `RelOps<Duration>` and `StrictOrder<Duration>`. 
The comparison operators form a strict, total order with all valid duration values, where longer positive durations are greater than shorter positive durations, longer negative durations are lesser than shorter negative durations, and positive durations are greater than negative durations. 
Note that it is not necessary to inherit from RelOps or StrictOrder, provided that the operators are implemented so that the expressions are equivalent. 

#### Class Synopsis ####

```cpp
class Duration{
public:
	constexpr Duration();
	constexpr explicit Duration(seconds_t seconds,nanos_t nanos=0);
	constexpr Duration(chrono_val_t val,ChronoUnit unit);
	template<typename Duration> constexpr EXPLICIT Duration(const Duration& d);
	constexpr Duration(const Duration&)=default;
	constexpr Duration(Duration&&)=default;
	constexpr Duration& operator=(const Duration&)=default;
	constexpr Duration& operator=(Duration&&)=default;
	~Duration()=default;
	constexpr Duration add(const Duration&)const;
	constexpr Duration subtract(const Duration&)const;
	constexpr Duration negate()const;
	constexpr Duration abs()const;
	constexpr bool isNegative()const;
	constexpr seconds_t getSeconds()const;
	constexpr nanos_t getNanos()const;
	constexpr Duration truncateTo(ChronoUnit u)const;
	constexpr /*unspecified*/ compareTo(const Duration& d)const;
	constexpr friend Duration operator+(const Duration& d1,const Duration& d2);
	constexpr friend Duration operator-(const Duration& d1,const Duration& d2);
	constexpr Duration operator-()const;
	constexpr friend bool operator<(const Duration& d1,const Duration& d2);
	/* OTHER RELATIONAL OPERATORS */
	template<typename T> constexpr static T&& wait(const Duration& d,T&& t);
	template<typename Duration> constexpr explicit operator Duration()const;
};
```

#### Constructors/Destructors/Operators ####

```cpp
constexpr Duration()noexcept; //(1)
constexpr explicit Duration(seconds_t seconds,nanos_t nanos=0); //(2)
constexpr explicit Duration(chrono_val_t val,ChronoUnit unit); //(3)
template<typename ChronoDuration> constexpr EXPLICIT Duration(const ChronoDuration& d); //(4)
constexpr Duration(const Duration&)=default; //(5)
constexpr Duration(Duration&&)=default; //(5)
constexpr Duration& operator=(const Duration&)=default; //(5)
constexpr Duration& operator=(Duration&&)=default; //(5)
~Duration()=default; //(5)
```

(1): Default constructs the Duration. Same as Duration{0}. 

(2): Constructs the Duration from a given number of seconds and nanoseconds. If nanos is outside of the range of nanoseconds, then it is reduced to that value and the appropriate ammount of seconds is added to the seconds.  

(3): Constructs the Duration from a given ammount of some unit. Provided for legacy purposes, this constructor is deprecated. 

(4): Constructs the Duration from a ChronoDuration. This does method does not participate in overload resolution unless ChronoDuration is some specialization of `std::chrono::duration`. 
The constructor is explicit if and only if `std::is_constructible_v<base_duration,ChronoDuration>` is false (that is, an explicit `duration_cast` is required to convert between the types). 
If `std::is_constructible_v<max_duration,ChronoDuration>` is false, it is unspecified if this constructor participates in overload resolution. 

(5): All Special Member functions (excluding default constructor) are defined as default. 

##### Exceptions #####

Default constructor, and all special constructors are noexcept. 

If any constructor would result in a length of time that is out of the valid range for Duration, an exception that matches a handler of type `std::out_of_range` is thrown with an unspecified, but meaningful message. 
Such a constructor call is not a constant expression. 
(As of 1.3): If such a constructor call would be used in a context that would require it to be evaluated at compile time if the constructor would be a constant expression, it is strongly encouraged of the implementation to issue a failing diagnostic instead of decaying the evaluation to runtime. 

#### Member Access and Truncation ###

```cpp
constexpr seconds_t getSeconds()const noexcept; //(1)
constexpr nanos_t getNanos()const noexcept; //(2)
constexpr Duration truncateTo(ChronoUnit u)const noexcept; //(3)
```

(1): Returns the seconds field of this Duration. 

(2): Returns the nanoseconds field of this Duration. 

(3): Truncates the Duration to a particular unit, discarding any precision that is weaker than the precision specified `u`. 
If `u` is not a valid enumerator of `ChronoUnit`, then the result is Duration{}. 
Otherwise, The result is the exact same duration, except that if the Duration is more precise then `u` specifies, then the Duration is rounded, towards 0, to the exact precision of ChronoUnit. 
Truncating to NANOSECONDS performs no conversion (returns a copy of *this). The conversion shall not result in any overflow, underflow, and may not result in a loss of precision, except the loss of precision that is inherit to this truncation. 

If the period of `u` can be represented as the compile time ratio `Period`, then this is the same as casting to `std::chrono::duration<REP,Period>`, then constructing from that Duration, except that overload resolution for the conversion operator and the constructor is guaranteed to succeed, where `REP` can completely represent the any value of the Duration, up to the precision of `u`. 


#### Legacy Named Operators ####

```cpp
constexpr Duration add(const Duration& d)const noexcept; //(1)
constexpr Duration addSeconds(seconds_t s)const noexcept; //(2)
constexpr Duration subtract(const Duration& d)const noexcept; //(3)
constexpr Duration negate()const noexcept; //(4)
constexpr Duration abs()const noexcept; //(5)
constexpr bool isNegative()const noexcept; //(6)
constexpr /*unspecified*/ compareTo(const Duration& d)const noexcept; //(7)
```

(1): Adds another Duration to `*this` and returns the result (`*this` is not modified). Same as `Duration{this->getSeconds()+d.getSeconds(),this->getNanos()+d.getNanos()}`. If this would result in a duration that is out of range, the appropriate bound is returned instead. 

(2): Adds a number of seconds to `*this` and returns the result (`*this` is not modified). Same as `Duration{this->getSeconds()+s,this->getNanos()}`. 

(2): Subtracts another Duration from `*this` and returns the result (`*this` is not modified). Same as `Duration{this->getSeconds()-d.getSeconds()-b,static_cast<nanos_t>(1000000000-(this->getNanos()-d.getNanos()))}`, where b is 1 if `this->getNanos()<=d.getNanos()` is true, and 0 otherwise. If this would result in a duration that is out of range, the appropriate bound is returned instead.  

(3): Returns the negation of this duration. Same as `Duration{-this->getSeconds()-1,static_cast<nanos_t>(1000000000-this->getNanos())}`. If `(*this)==MIN_DURATION` is true, then returns `MAX_DURATION`. For any other duration d, d.negate().negate()==d is true.

(4): Returns the absolute value of this duration. The absolute value of a duration is the duration that has the same length, but is always positive. 

(5): Checks if this Duration is negative. Same as (*this)<Duration{}. 

(6): Compares 2 durations based on there length and signedness. The result is a type that can be compared with the integer literal 0 (comparing with any other value results in undefined behavior). For any valid durations, d and e, `d.compareTo(e) op 0`  is the same as `d op e`, where *op* is some valid comparison operator. This method is deprecated and will be removed in a future version in favor of a defaulted 3-way comparison operator. 


#### Operators ####

```cpp
constexpr friend Duration operator+(const Duration& d1,const Duration& d2)noexcept; //(1)
constexpr friend Duration operator-(const Duration& d1,const Duration& d2)noexcept; //(2)
constexpr Duration operator-()const noexcept; //(3)
constexpr friend bool operator<(const Duration& d1,const Duration& d2)const noexcept; //(4)
/*OTHER RELATIONAL OPERATORS*/
```

(1): Adds 2 durations together. For any Durations d1, d2, `d1+d2==d1.add(d2)` and `d2+d1==d1+d2` are both true. 

(2): Subtracts a Duration from another. For any Durations d1, d2, `d1-d2==d1.subtract(d2)` is true. 

(3): Negates a Duration. For any Duration d, `-d==d.negate()` is true. 

(4): Compares 2 durations. Returns true if d1 is less-than d2 and false otherwise. 
* A Negative Duration is less-than a positive Duration or 0.
* 0 is less-than a positive duration
* A short positive Duration is less-than a long positive Duration
* a long negative Duration is less-than a short negative duration

The other Comparison operators are provided, as though they used the definitions provided from inheriting privately from `RelOps<Duration>` and `StrictOrdering<Duration>`. 

