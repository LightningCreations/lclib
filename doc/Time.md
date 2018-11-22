#Time Library

Defined in `<lclib-cxx/JTime.hpp>`




##Synopsis

```cpp
#include <chrono>
enum class ChronoUnit;
using seconds_t = /*unspecified*/;
using nanos_t = /*unspecified*/;
using chrono_val_t = /*unspecified*/;
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
constexpr int32_t hashcode(const Duration&);
constexpr int32_t hashcode(const Instant&);
namespace std{
	template<> struct hash<Duration>;
	template<> struct hash<Instant>;
	template<> struct numeric_limits<Duration>;
	template<> struct numeric_limits<Instant>;
};
```

##Overview

The Time Library, along with the STL `<chrono>` library, is provided to interact with time. 

Note that the Time library is primarily for Interoperation. The standard implementation should be used in most circumstances. 

##Type definitions

###`seconds_t`

A signed integral type which can store all values in the range (-31556889864401400,31556889864401400]. 

###`nanos_t`

An unsigned integral type which can store all values between 0 and 1000000000 inclusive.

###`chrono_val_t`

A signed integral type which is at least 64-bits.

###`instant_clock`

The clock type used by instants. Defined as std::chrono::system_clock. 

Note that prior to C++20, this clock's Epoch is not fixed. 
Instants will be defined relative to this clock's epoch, rather than the Unix Epoch. 
Interoperation with another library implementation is only well defined if both implementations clock's epoch is the Unix Epoch (January 1st, 1970 at 00:00:00.000000000 UTC).  
Interoperation with java is only well defined if the implementation uses a clock with the above epoch. 

In C++20, the epoch of std::chrono::system_clock was defined as using the Unix Epoch, so these points are meaningless. 

