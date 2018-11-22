#ifndef __JTime_hpp_2018_05_16_08_23
#define __JTime_hpp_2018_05_16_08_23
#include <lclib/Config.hpp>
#include <lclib/Hash.hpp>
#include <stdint.h>
#include <chrono>
#include <limits>
#include <tuple>
#include <lclib/Operations.hpp>
enum class ChronoUnit{
    NANOSECONDS, MICROSECONDS, MILISECONDS, SECONDS,
    MINUTES, HOURS
};

using chrono_value_t = int64_t;
using seconds_t = int64_t;
using nanos_t = unsigned;

using namespace std::chrono_literals;

/**
 * Converts a given value from unit to Seconds. This does not truncate.
 * If the value is less than 0, and the val,unit pair have greater than Seconds Precision,
 *  then the value is the result of truncating the value to seconds, -1.
 * Otherwise the value is truncated or extended
 * Returns 0 if unit is not a valid Enumerator of ChronoUnit.
 */
constexpr seconds_t toSeconds(chrono_value_t val,ChronoUnit unit){
	if(val<0)
		switch(unit){
		case ChronoUnit::SECONDS: return val;
		case ChronoUnit::MINUTES: return val*60;
		case ChronoUnit::HOURS: return val*3600;
		case ChronoUnit::MILISECONDS: return (val/1000)-((val%1000)&&true);
		case ChronoUnit::MICROSECONDS: return (val/1000000)-((val%1000000)&&true);
		case ChronoUnit::NANOSECONDS: return (val/1000000000)-((val%1000000000)&&true);
		}
	else
		switch(unit){
		case ChronoUnit::SECONDS: return val;
		case ChronoUnit::MILISECONDS: return val/1000;
		case ChronoUnit::MICROSECONDS: return val/1000000;
		case ChronoUnit::NANOSECONDS: return val/1000000000;
		case ChronoUnit::MINUTES: return val*60;
		case ChronoUnit::HOURS: return val*3600;
		}
	return 0;
}

/**
 * Obtains the nanos-of-second component of the given unsigned time value.
 * Used indirectly by toNanos(chrono_value_t,ChronoUnit).
 * Returns 0 if unit is not a valid enumerator of ChronoUnit.
 */
constexpr nanos_t toNanos(uint64_t val,ChronoUnit unit){
	switch(unit){
	case ChronoUnit::SECONDS:
	case ChronoUnit::MINUTES:
	case ChronoUnit::HOURS:
		return 0;
	case ChronoUnit::MILISECONDS:
		return (val%1000)*1000000;
	case ChronoUnit::MICROSECONDS:
		return (val%1000000)*1000;
	case ChronoUnit::NANOSECONDS:
		return val%1000000000;
	}
	return 0;
}

/**
 * Converts the given seconds and nano adjustment to a value of ChronoUnit. (This truncates)
 *  if the precision of the pair is less than the precision of the unit.
 * As with toSeconds and toNanos, the behavior is undefined  if unit is not an Enumerator of ChronoUnit.
 * returns 0 if unit is not a valid Enumerator of ChronoUnit.
 */
constexpr chrono_value_t truncateTo(seconds_t seconds,nanos_t nano,ChronoUnit unit){
	switch(unit){
	case ChronoUnit::HOURS:
		return seconds/3600;
	case ChronoUnit::MINUTES:
		return seconds/60;
	case ChronoUnit::SECONDS:
		return seconds;
	case ChronoUnit::MILISECONDS:
		return seconds*1000+nano/1000000;
	case ChronoUnit::MICROSECONDS:
		return seconds*1000000+nano/1000;
	case ChronoUnit::NANOSECONDS:
		return seconds*1000000000+nano;
	}
	return 0;
}

/**
 * Converts a given signed chrono value of unit to the nano-of-seconds component.
 * If unit has at most SECONDS precision, returns 0 unconditionally.
 * If val is less that 0, effectively returns 1000000000-toNanos(uint64_t(-val),unit).
 * Otherwise effectively returns toNanos(uint64_t(val),unit);
 * Returns 0 if unit is not a valid enumerator of ChronoUnit.
 */
constexpr nanos_t toNanos(chrono_value_t val,ChronoUnit unit){
	if(val<0)
		return (1000000000-toNanos(uint64_t(-val),unit))%1000000000;
	else
		return toNanos(uint64_t(val),unit);
}

constexpr const nanos_t NANOS_PER_SECOND = 1000000000;

/*
    A class that represents a Duration between 2 Instants.
    This Duration is precise to Nanoseconds and stores 64-bit seconds with 32-bit nanoseconds.
*/
class LIBLCAPI Duration:private StrictOrder<Duration>,private RelOps<Duration>{
private:
    seconds_t seconds;
    nanos_t nanos;

public:
    constexpr Duration():seconds(0),nanos(0){}
    constexpr Duration(const Duration&)=default;
    constexpr Duration(Duration&&)=default;
    Duration(const Duration&&)=delete;
    constexpr Duration& operator=(const Duration&)=default;
    constexpr Duration& operator=(Duration&&)=default;
    constexpr Duration& operator=(const Duration&&)=delete;
    ~Duration()=default;
    /*
        Produces a duration by a given number of seconds, adjusted by the given nanoseconds.
        The values are adjusted to constrain the number of nanoseconds to between 0 inclusive and 100000000 exclusive,
        similarily to Instant.ofEpochSeconds(seconds,nanoAdjustment);.
    */
    constexpr Duration(seconds_t s,nanos_t n=0):seconds{s},nanos{n}{
    	while(nanos>NANOS_PER_SECOND){
    		seconds++;
    		nanos-=NANOS_PER_SECOND;
    	}
    }
    /*
        Produces a duration by a given Number of a specific unit.
    */
    constexpr Duration(chrono_value_t val,ChronoUnit unit):seconds{toSeconds(val,unit)},nanos{toNanos(val,unit)}{}

    /**
     * Conversion constructor from a chrono duration.
     */
    template<typename Rep,typename Period> constexpr Duration(std::chrono::duration<Rep,Period> d):
    	seconds{std::chrono::duration_cast<std::chrono::seconds>(d).count()},
		nanos{(std::chrono::duration_cast<std::chrono::nanoseconds>(d)%1000000000ns).count()}{}
    /*
        Produces a new duration that equivalent to the given duration subtracted from this duration.
    */
    constexpr Duration subtract(const Duration& d)const{
    	if(nanos<d.nanos)
    		return Duration{seconds-d.seconds-1,1000000000+(nanos-d.nanos)};
    	return Duration{seconds-d.seconds,nanos-d.nanos};
    }
    /*
        Produces a new duration that is equivalent to the given duration added to this duration.
    */
    constexpr Duration add(const Duration& d)const{
    	return Duration{seconds+d.seconds,nanos+d.nanos};
    }
    /*
        Produces a new duration that is equivalent to this duration, with the given number of seconds added.
    */
    constexpr Duration addSeconds(int64_t s)const{
    	return Duration{seconds+s,nanos};
    }

    /*
        Gets the seconds field of this Instant.
    */
    constexpr int64_t getSeconds()const{
    	return seconds;
    }
    /*
        Gets the nanoseconds field of this Instant.
    */
    constexpr int     getNanos()const{
    	return nanos;
    }
    /*
        Produces a new duration that is equivalent to the negation of this duration.
        For Any 2 Instants i and j, (i-j).negate() is equal to j-i.
        d.negate() is equal to -d.
    */
    constexpr Duration negate()const{
    	return Duration{-seconds-(nanos&&true),1000000000-nanos};
    }
    /*
        Compares this duration with another.
        This comparison will result in a negative value if this duration is less than the given duration,
        0 if this duration is the same as the given duration, and a positive value if this duration is greater than the other duration
    */
    constexpr int compareTo(const Duration& d)const{
    	if(*this<d)
    		return -1;
    	else if(*this>d)
    		return 1;
    	else
    		return 0;
    }
    /*
        Produces a new Duration, that is the absolute value of this Duration.
        If this duration is negative, then this is equivalent to negating this duration. Otherwise, it is equivalent to this duration.
        Taking the absolute value of the Minimum duration will result in an out_of_bounds exception being thrown.
    */
    constexpr Duration abs()const{
    	return seconds<0?negate():*this;
    }
    /*
        Checks if this duration is negative.
        A Duration is negative if it is less than the Constant ZERO
    */
    constexpr bool isNegative()const{
    	return seconds<0;
    }
    /*
        Truncates this Duration to a given chrono unit.
        This operation should be similar the same trucation with Instant
    */
    constexpr Duration truncateTo(ChronoUnit u)const{
    	if(u==ChronoUnit::NANOSECONDS)
    		return *this;
    	else
    		return Duration{::truncateTo(seconds,nanos,u),u};
    }
    /*
        Negates this Duration.
        Effectively the same as d.negate() for this duration d.
    */
    constexpr Duration operator-()const{
    	return negate();
    }
    /*
        Adds a Duration to this Duration.
        Effectively the same as the same call to add.
    */
    constexpr Duration operator+(const Duration& d)const{
    	return add(d);
    }
    /*
        Adds a Duration to this Duration.
        Effectively the same as the same call to subtract.
    */
    constexpr Duration operator-(const Duration& d)const{
    	return subtract(d);
    }

    /*
        Compares this duration to annother.
        For any Duration d and e, d<e is effectively equivalent to d.compareTo(e)<0
    */
    constexpr bool operator<(const Duration& d)const{
    	return std::tie(seconds,d.seconds)<std::tie(nanos,d.nanos);
    }

    /*
        Computes the hashcode of this Duration.
        This hashcode is computed from the hashcode of its seconds field and its nanoseconds field
    */
    constexpr int hashCode()const{
    	return hashcode(seconds)*31+hashcode(nanos);
    }
    /**
     * Waits for a specified Duration, then forwards the argument.
     */
    template<typename T> static T&& wait(const Duration& d,T&& t);

    template<typename Duration> constexpr operator Duration()const{
    	return std::chrono::duration_cast<Duration>(std::chrono::seconds{seconds}+std::chrono::nanoseconds{nanos});
    }


};

/**
 * The type which represents the clock used by Instants.
 * Note: Prior to C++20, time points from this clock were not guaranteed
 * 	to be relative to the unix epoch. (January 1st, 1970 at 12:00:00.0000000000 UTC)
 * Instants defined in any other version are relative to the epoch of the system clock.
 *
 * Interoperation with Other Instances of LCLib is only well defined if both uses the same epoch.
 * Interoperation with Java is only well defined if this epoch holds.
 */
using instant_clock = std::chrono::system_clock;

/**
 * Tag type for Constructing an instant inline from the system time (rather then using the factory method).
 */
struct now_t{
public:
	constexpr explicit now_t()=default;
};

const now_t now{};

/*
    Represents an Instant in time.
    Each Instant is precise to Nanoseconds, but may not be accurate to Nanoseconds.
    Instant's can be aquired using any of the Factory methods, including the "now()" Method with obtains an Instant from the system clock.
    Instant's are not time zone sensitive and resolve to UTC timer.
    The Instant class is effectively Immutable. All Methods are const qualified and return a unqiue instance instead of modifying the object, if they would change fields.
    The Instant class tracks signed 64-bit integer for seconds and 32-bit nanoseconds. (0-999999999)
    This represents a time since the Unix EPOCH, or 1970-01-01T00:00:00.000000000Z (January 1st, 1970, at 12:00:00 exactly, UTC)
*/
class LIBLCAPI Instant:private StrictOrder<Instant>,private RelOps<Instant>{
private:
    seconds_t seconds;
    nanos_t nanos;

public:
    /**
     * Constructs an Instant which is s seconds and n nanoseconds from the Unix EPOCH,
     * If n is not in the range (0,100000000], it will be constrained to that range,
     *  and the seconds field will be updated accordingly.
     * For example the Following 2 Instants have the same value:
     * \Instant{1000000,1}
     * \Instant{1000000,1000000001}.
     * As the nanos field is unsigned, only the upper bound needs to be checked.
     */
    constexpr Instant(seconds_t s,nanos_t n=0):seconds{s},nanos{n}{
    	while(nanos>NANOS_PER_SECOND)
    		(nanos -= NANOS_PER_SECOND),seconds++;
    }

    /*
        Constructs an instant with both fields set to zero. Effectively the same as Copy-Constructing with the EPOCH constant.
    */
    constexpr Instant():seconds{0},nanos{0}{}
    /**
     * Constructs an instant from a time point of instant_clock.
     * This is a clock sensitive constructor.
     */
    template<typename Dur> Instant(std::chrono::time_point<instant_clock,Dur> tp){
    	const auto d{tp.time_since_epoch()};
    	seconds = std::chrono::duration_cast<std::chrono::seconds>(d).count();
    	nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(d%1000000000s).count();
    }
    /*
     * Constructs an instant from the current system time inline.
     * Same as the now() factory method.
     * This is a clock sensitive constructor.
     */
    Instant(now_t);
    ~Instant()=default;
    constexpr Instant(const Instant&)=default;
    constexpr Instant(Instant&&)=default;
    Instant(const Instant&&)=delete;
    constexpr Instant& operator=(const Instant&)=default;
    constexpr Instant& operator=(Instant&&)=default;
    Instant& operator=(const Instant&&)=delete;
    /*
        Obtains an Instant from the current system clock. This method should attempt to produce the most accurate result possible.
        This factory method can produce Inaccurate Instants, depending on the accuracy of the system clock.
        The accuracy of the resultant instant is the lesser of the accuracy of the system clock and nanosecond accuracy.
    	This is a clock sensitive Factory Method.
    */
    static Instant now();

    static constexpr Instant toEpoch(const Duration& d){
    	return Instant{d.getSeconds(),d.getNanos()};
    }

    constexpr Duration fromEpoch(){
    	return Duration{seconds,nanos};
    }

    /*
        Subtracts a given Duration from this instant.
        The resultant Instant is taken from this instant and offset by the inverse of the given duration, such that:
        for any Instant i and Duration d, Duration::between(i.subtract(d),i)==d.

    */
    constexpr Instant subtract(const Duration& d) const{
    	return Instant{}+(Duration{seconds,nanos}-d);
    }
    /*
        Adds a given Duration to this instant.
        The resultant Instant is taken from this instant and offset by the given duration, such that:
        for any Instant i and Duration d, Duration::between(i,i.add(d))==d.
        This method does not modify this Instant and returns a new Instant that the above is true for.
    */
    constexpr Instant add(const Duration& d) const{
    	return Instant{seconds+d.getSeconds(),nanos+d.getNanos()};
    }
    /*
        Truncates this instant to the given unit.
        If this Instant is not accurate to or is exactly accurate to the given unit, the resultant Instant is identical to this one.
        Otherwise, the resultant Instant has its fields set so that it accurately represents this Instant, to that unit.
        Any information that makes the resultant instant more accurate then the given unit is lost, and results in 0.
        For any Instant i, i.truncateTo(ChronoUnit::NANOSECONDS)==i is true.
        The resultant Instant is guarenteed to be accurate to at most the given Unit.
    */
    constexpr Instant truncateTo(ChronoUnit u)const{
    	return toEpoch(Duration{seconds,nanos}.truncateTo(u));
    }
    /*
        Checks if this Instant occurs before another in time.
    */
    constexpr bool isBefore(const Instant& i)const{
    	return *this < i;
    }
    /*
        Checks if this Instant occurs after another in time.
    */
    constexpr bool isAfter(const Instant& i)const{
    	return i < *this;
    }
    /*
        Compares this instant to another, based on its occurance in time.
        This comparison is guarenteed to be as accurate as the highest accuraccy between the 2 instants.
    */
    constexpr int compareTo(const Instant& i)const{
    	if(*this<i)
    		return -1;
    	else if(i<*this)
    		return 1;
    	else
    		return 0;
    }

    /*
        Compares this instant to another.
        For any Instants i and j, i<j is effectively equivalent to i.compareTo(j)<0.
        An instant is greater than another if it occurs before the other in time.
    */
    constexpr friend bool operator<(const Instant& i1,const Instant& i2){
    	return std::tie(i1.seconds,i1.nanos)<std::tie(i2.seconds,i2.nanos);
    }

    /*
        Add d to i.
        For Any Instant i and Duration d, i+d is effectively equivalent to i.add(d) and result in the same Instant
    */
    constexpr friend Instant operator+(const Instant& i,const Duration& d){
    	return Instant{i.seconds+d.getSeconds(),i.nanos+d.getNanos()};
    }
    /*
        Subtracts d from i.
        For any Instant i and Duration d, i-d is effectively equivalent to i.subtract(d) and result in the same Instant
    */
    constexpr friend Instant operator-(const Instant& i,const Duration& d){
    	return Instant{}+(Duration{i.seconds,i.nanos}-d);
    }
    /*
        Gets the Number of seconds since the Epoch this Instant Describes.
    */
    constexpr int64_t toEpochSecond() const{
    	return seconds;
    }
    /*
        Obtains the Nano field of this Instant, ranging from 0 inclusive to 1000000000 exclusive
    */
    constexpr int     getNanos() const{
    	return nanos;
    }
    /*
        Obtains the value of this instant, as a number of the given ChronoUnit since the Epoch.
        The resulting value is equivalent to the one produced from this instant, Truncated to that unit.
    */
    constexpr int64_t get(ChronoUnit u)const{
    	return ::truncateTo(seconds,nanos,u);
    }

    /*
        Gets the hashcode of the given Instant.
        This is computed from the hashcode of its seconds field and the hashcode of its nano's field.
    */
    constexpr int32_t hashCode()const{
    	return hashcode(seconds)*31+hashcode(nanos);
    }

    template<typename Duration> operator std::chrono::time_point<instant_clock,Duration>()const{
    	return std::chrono::time_point_cast<Duration>(std::chrono::time_point<instant_clock,std::chrono::seconds>(seconds)+std::chrono::nanoseconds(nanos));
    }

};
/*
    Instant Constant Representing the Epoch (January 01, 1970 at 00:00:00 Exactly UTC)
*/
constexpr Instant EPOCH{};
/*
    Instant Constant Holding the Effective Max value for an instant (1<<63-1 seconds, 999999999 nanoseconds)
*/
constexpr Instant MAX{31556889864401399LL,999999999};
/*
    Instant Constant Holding the Effective Minimum value for an instant (-1<<63 seconds)
*/
constexpr Instant MIN{-31556889864401400LL};


constexpr int hashcode(const Instant& i){
	return i.hashCode();
}

constexpr int hashcode(const Duration& d){
	return d.hashCode();
}

constexpr Duration ZERO{};
constexpr Duration MAX_DURATION{31556889864401399,999999999};
constexpr Duration MIN_DURATION{-31556889864401400};

namespace std{
	template<> struct hash<Instant>{
	public:
		constexpr hash()=default;
		constexpr std::size_t operator()(const Instant& i)const{
			return i.hashCode();
		}
	};
	template<> struct hash<Duration>{
	public:
		constexpr hash()=default;
		constexpr std::size_t operator()(const Duration& i)const{
			return i.hashCode();
		}
	};
}

#endif
