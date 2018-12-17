# Time Library #

Defined in `<lclib/JTime.hpp>`




## Synopsis ##

```cpp
#include <chrono>
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

## Type definitions ##

### `seconds_t` ###


A type that satisfies TriviallyCopyable, StandardLayout, LiteralType, BytesWriteable, and an integral value in the range (-31556889864401400,31556889864401400] is implicitly convertible to this type. 

`seconds_t` is not Required to be a user-defined type, but if it is, `std::numeric_limits` must be specialized for it. 

Can be written to a DataOutputStream and read from a DataInputStream as a long value

### `nanos_t` ###

A type that satisfies TriviallyCopyable, StandardLayout, LiteralType, and BytesWriteable, and an integral value in the range (0,1000000000] is implicitly convertible to this type. 

`nanos_t` is not required to be a user-defined type, but if it is, `std::numeric_limits` must be specialized for it.

Can be written to a DataOutputStream and read from a DataInputStream as an int value.

### `chrono_val_t` ##

A signed integer type that is at least 64-bit. 

### `seconds_duration` ###

A specialization of `std::chrono::duration` that can store any valid seconds field of a Duration. 

### `nanos_duration` ###

A specialization of `std::chrono::duration` that can store any valid nanos field of a Duration. 

### `base_duration` ###

A specialization of `std::chrono::duration` that can store at least `chrono_val_t` of an unspecified subsecond (or second level) period. 

It is also guaranteed that a value of type `base_duration` can store any valid value of `seconds_duration` without loss of precision. 

### `max_duration` ###

A specialization of `std::chrono::duration` that has the best period the implementation can deal with, represented using `chrono_val_t`. Guaranteed to be at least as precise as `base_duration`. 


### `instant_clock` ###

The clock type used by instants. Defined as a clock for which has the date and time 1970-01-01T00:00:00.000Z (the unix epoch) as its epoch, and the minimum interval between reported instants is at least `base_duration`. 

### `enum class ChronoUnit` ###

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

Note: Only `SECONDS`, `MINUTE`, and `HOURS` are required. `MILISECONDS`, `MICROSECONDS`, and `NANOSECONDS` are only required if the period of `max_duration` is at least as precise as `std::ratio<1,1000>`, `std::ratio<1,1000000>`, and `std::ratio<1,1000000000>` respectively. Other implementation defined units may be supported. 



