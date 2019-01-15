#ifndef __JTime_hpp_2018_05_16_08_23
#define __JTime_hpp_2018_05_16_08_23
#include <lclib/Config.hpp>
#include <lclib/Hash.hpp>
#include <stdint.h>
#include <chrono>
#include <limits>
#include <tuple>
#include <lclib/Operations.hpp>
#include <thread>
#include <type_traits>
enum class ChronoUnit{
    NANOSECONDS, MICROSECONDS, MILISECONDS, SECONDS,
    MINUTES, HOURS
};

using chrono_val_t = int64_t;
using seconds_t = int64_t;
using nanos_t = unsigned;

using seconds_duration = std::chrono::duration<seconds_t>;
using nanos_duration = std::chrono::duration<nanos_t,std::nano>;
using base_duration = std::chrono::duration<chrono_val_t,std::milli>;
using max_duration = std::chrono::duration<chrono_val_t,std::nano>;

using namespace std::chrono_literals;

/**
 * Converts a given value from unit to Seconds. This does not truncate.
 * If the value is less than 0, and the val,unit pair have greater than Seconds Precision,
 *  then the value is the result of truncating the value to seconds, -1.
 * Otherwise the value is truncated or extended
 * Returns 0 if unit is not a valid Enumerator of ChronoUnit.
 */
constexpr seconds_t toSeconds(chrono_val_t val,ChronoUnit unit){
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
constexpr nanos_t toNanos(useconds_t val,ChronoUnit unit){
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
 * returns 0 if unit is not a valid Enumerator of ChronoUnit.
 */
constexpr chrono_val_t truncateTo(seconds_t seconds,nanos_t nano,ChronoUnit unit){
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
 * If val is less that 0, effectively returns 1000000000-toNanos(useconds_t(-val),unit).
 * Otherwise effectively returns toNanos(useconds_t(val),unit);
 * Returns 0 if unit is not a valid enumerator of ChronoUnit.
 */
constexpr nanos_t toNanos(chrono_val_t val,ChronoUnit unit){
	if(val<0)
		return (1000000000-toNanos(useconds_t(-val),unit))%1000000000;
	else
		return toNanos(useconds_t(val),unit);
}

constexpr const nanos_t NANOS_PER_SECOND{1000000000};

/*
    A class that represents a Duration between 2 Instants.
    This Duration is precise to Nanoseconds and stores 64-bit seconds with 32-bit nanoseconds.
*/
class LIBLCAPI Duration:private StrictOrder<Duration>,private RelOps<Duration>{
private:
    seconds_t seconds;
    nanos_t nanos;

public:
    constexpr Duration():seconds{0},nanos{0}{}
    constexpr Duration(const Duration&)=default;
    constexpr Duration(Duration&&)=default;
    constexpr Duration& operator=(const Duration&)=default;
    constexpr Duration& operator=(Duration&&)=default;
    ~Duration()=default;
    /*
        Produces a duration by a given number of seconds, adjusted by the given nanoseconds.
        The values are adjusted to constrain the number of nanoseconds to [0,1000000000)
        similarily to Instant.ofEpochSeconds(seconds,nanoAdjustment);.
    */
    constexpr explicit Duration(seconds_t s,nanos_t n=0):seconds{s},nanos{n}{
    	while(nanos>NANOS_PER_SECOND){
    		seconds++;
    		nanos-=NANOS_PER_SECOND;
    	}
    	if(seconds<-31556889864401400||seconds>=31556889864401400)
    		throw std::out_of_range("A Duration shall fall in the range [-31556889864401400s,31556889864401400s)");
    }
    /*
        Produces a duration by a given Number of a specific unit.
    */
    constexpr explicit Duration(chrono_val_t val,ChronoUnit unit):Duration{toSeconds(val,unit),toNanos(val,unit)}{}

    /**
     * Conversion constructor from a chrono duration.
     */
    template<typename Rep,typename Period,std::enable_if_t<std::is_constructible_v<base_duration,std::chrono::duration<Rep,Period>>>* = 0> constexpr Duration(std::chrono::duration<Rep,Period> d):
    	seconds{std::chrono::duration_cast<std::chrono::seconds>(d).count()},
		nanos{(std::chrono::duration_cast<std::chrono::nanoseconds>(d%1000000000ns)).count()}{}
	/**
	 * Conversion constructor from a chrono duration.
	 */
	template<typename Rep,typename Period,std::enable_if_t<!std::is_constructible_v<base_duration,std::chrono::duration<Rep,Period>>>* = 0> constexpr explicit Duration(std::chrono::duration<Rep,Period> d):
		seconds{std::chrono::duration_cast<std::chrono::seconds>(d).count()},
		nanos{(std::chrono::duration_cast<std::chrono::nanoseconds>(d%1000000000ns)).count()}{}
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
    constexpr Duration addSeconds(seconds_t s)const{
    	return Duration{seconds+s,nanos};
    }

    /*
        Gets the seconds field of this Instant.
    */
    constexpr seconds_t getSeconds()const{
    	return seconds;
    }
    /*
        Gets the nanoseconds field of this Instant.
    */
    constexpr nanos_t     getNanos()const{
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
    */
    constexpr Duration truncateTo(ChronoUnit u)const{
    	if(u==ChronoUnit::NANOSECONDS)
    		return *this;
    	else if(u==ChronoUnit::MICROSECONDS)
    		return Duration{seconds,(nanos/1000)*1000};
    	else if(u==ChronoUnit::MILISECONDS)
    		return Duration{seconds,(nanos/1000000)*1000000};
    	else if(u==ChronoUnit::SECONDS)
    		return Duration{seconds};
    	else if(u==ChronoUnit::MINUTES)
    		return Duration{(seconds/60)*60};
    	else if(u==ChronoUnit::HOURS)
    		return Duration{(seconds/3600)*3600};
    	else
    		return Duration{};
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


    template<typename Duration> constexpr explicit operator Duration()const{
    	return std::chrono::duration_cast<Duration>(std::chrono::seconds{seconds}+std::chrono::nanoseconds{nanos});
    }

    /**
	 * Waits for a specified Duration, then forwards the argument.
	 */
	template<typename T> static T&& wait(const Duration& d,T&& t){
		std::this_thread::sleep_for(static_cast<base_duration>(d));
		return static_cast<T&&>(t);
	}

    constexpr explicit operator bool()const{
    	return seconds||nanos;
    }


};

/**
 * The type which represents the clock used by Instants.
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
    	if(seconds<-31556889864401400||seconds>=31556889864401400)
    	    throw std::out_of_range("A Duration shall fall in the range [-31556889864401400s,31556889864401400s)");
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
    	if(seconds<-31556889864401400||seconds>=31556889864401400)
    	    throw std::out_of_range("A Duration shall fall in the range [-31556889864401400s,31556889864401400s)");
    }
    /*
     * Constructs an instant from the current system time inline.
     * Same as the now() factory method.
     * This is a clock sensitive constructor.
     */
    Instant(now_t)noexcept;
    ~Instant()=default;
    constexpr Instant(const Instant&)=default;
    constexpr Instant(Instant&&)=default;
    constexpr Instant& operator=(const Instant&)=default;
    constexpr Instant& operator=(Instant&&)=default;

    /*
        Obtains an Instant from the current system clock. This method should attempt to produce the most accurate result possible.
        This factory method can produce Inaccurate Instants, depending on the accuracy of the system clock.
        The accuracy of the resultant instant is the lesser of the accuracy of the system clock and nanosecond accuracy.
    	This is a clock sensitive Factory Method.
    */
    static Instant now()noexcept;

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
    constexpr seconds_t toEpochSecond() const{
    	return seconds;
    }
    /*
        Obtains the Nano field of this Instant, ranging from 0 inclusive to 1000000000 exclusive
    */
    constexpr nanos_t   getNanos() const{
    	return nanos;
    }
    /*
        Obtains the value of this instant, as a number of the given ChronoUnit since the Epoch.
        The resulting value is equivalent to the one produced from this instant, Truncated to that unit.
    */
    constexpr chrono_val_t get(ChronoUnit u)const{
    	switch(u){
    	case ChronoUnit::NANOSECONDS:
    		return seconds*100000000+nanos;
    	case ChronoUnit::MICROSECONDS:
    		return seconds*1000000+nanos/1000;
    	case ChronoUnit::MILISECONDS:
    		return seconds*1000+nanos/1000000;
    	case ChronoUnit::SECONDS:
    		return seconds;
    	case ChronoUnit::MINUTES:
    		return seconds/60;
    	case ChronoUnit::HOURS:
    		return seconds/3600;
    	}
    	return 0;
    }

    /*
        Gets the hashcode of the given Instant.
        This is computed from the hashcode of its seconds field and the hashcode of its nano's field.
    */
    constexpr int32_t hashCode()const{
    	return hashcode(seconds)*31+hashcode(nanos);
    }

	template<typename Duration> explicit operator std::chrono::time_point<instant_clock, Duration>()const {
		return std::chrono::time_point_cast<Duration>(std::chrono::time_point<instant_clock, std::chrono::seconds>{std::chrono::seconds{ seconds }}+std::chrono::nanoseconds{ nanos });
    }

	constexpr explicit operator bool()const{
		return seconds||nanos;
	}

};

/*
    Instant Constant Representing the Epoch (January 01, 1970 at 00:00:00 Exactly UTC)
*/
constexpr Instant EPOCH{};
/*
    Instant Constant Holding the Effective Max value for an instant (1<<63-1 seconds, 999999999 nanoseconds)
*/
constexpr Instant MAX{seconds_t{31556889864401399},nanos_t{999999999}};
/*
    Instant Constant Holding the Effective Minimum value for an instant (-1<<63 seconds)
*/
constexpr Instant MIN{seconds_t{-31556889864401400}};


constexpr int hashcode(const Instant& i){
	return i.hashCode();
}

constexpr int hashcode(const Duration& d){
	return d.hashCode();
}

constexpr Duration ZERO{};
constexpr Duration MAX_DURATION{seconds_t{31556889864401399},nanos_t{999999999}};
constexpr Duration MIN_DURATION{seconds_t{-31556889864401400}};
constexpr Duration LOWEST_DURATION{seconds_t{},nanos_t{1}};

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
	template<> struct numeric_limits<Instant>{
		constexpr static bool is_specialized{true};
		constexpr static bool is_signed{true};
		constexpr static bool is_integer{false};
		constexpr static bool is_exact{true};
		constexpr static bool has_infinity{false};
		constexpr static bool has_quiet_NaN{false};
		constexpr static bool has_signaling_NaN{false};
		constexpr static std::float_denorm_style has_denorm{std::denorm_absent};
		constexpr static bool has_denorm_loss{false};
		constexpr static std::float_round_style round_style{std::round_toward_zero};
		constexpr static bool is_iec559{false};
		constexpr static bool is_bounded{true};
		constexpr static bool is_modulo{false};
		constexpr static int digits{0};
		constexpr static int digits10{0};
		constexpr static int max_digits10{0};
		constexpr static int radix{0};
		constexpr static int min_exponent{0};
		constexpr static int min_exponent10{0};
		constexpr static int max_exponent{0};
		constexpr static int max_exponent10{0};
		constexpr static bool traps{false};
		constexpr static bool tinyness_before{false};
		constexpr static Instant min() noexcept{
			return MIN;
		}
		constexpr static Instant lowest() noexcept{
			return MIN;
		}
		constexpr static Instant max() noexcept{
			return MAX;
		}
		constexpr static Duration epsilon() noexcept{ //Yes this returns a Duration, not an Instant
			return LOWEST_DURATION;
		}
		constexpr static Instant round_error() noexcept{
			return Instant{};
		}
		constexpr static Instant infinity() noexcept{
			return Instant{};
		}
		constexpr static Instant quiet_NaN() noexcept{
			return Instant{};
		}
		constexpr static Instant signaling_NaN() noexcept{
			return Instant{};
		}
		constexpr static Instant denorm_min() noexcept{
			return Instant{};
		}
	};
	template<> struct numeric_limits<Duration>{
		constexpr static bool is_specialized{true};
		constexpr static bool is_signed{true};
		constexpr static bool is_integer{false};
		constexpr static bool is_exact{true};
		constexpr static bool has_infinity{false};
		constexpr static bool has_quiet_NaN{false};
		constexpr static bool has_signaling_NaN{false};
		constexpr static std::float_denorm_style has_denorm{std::denorm_absent};
		constexpr static bool has_denorm_loss{false};
		constexpr static std::float_round_style round_style{std::round_toward_zero};
		constexpr static bool is_iec559{false};
		constexpr static bool is_bounded{true};
		constexpr static bool is_modulo{false};
		constexpr static int digits{0};
		constexpr static int digits10{0};
		constexpr static int max_digits10{0};
		constexpr static int radix{0};
		constexpr static int min_exponent{0};
		constexpr static int min_exponent10{0};
		constexpr static int max_exponent{0};
		constexpr static int max_exponent10{0};
		constexpr static bool traps{false};
		constexpr static bool tinyness_before{false};
		constexpr Duration min() noexcept{
			return MIN_DURATION;
		}
		constexpr Duration max() noexcept{
			return MAX_DURATION;
		}
		constexpr Duration lowest() noexcept{
			return LOWEST_DURATION;
		}
		constexpr Duration epsilon() noexcept{
			return LOWEST_DURATION;
		}
		constexpr static Duration round_error() noexcept{
			return Duration{};
		}
		constexpr static Duration infinity() noexcept{
			return Duration{};
		}
		constexpr static Duration quiet_NaN() noexcept{
			return Duration{};
		}
		constexpr static Duration signaling_NaN() noexcept{
			return Duration{};
		}
		constexpr static Duration denorm_min() noexcept{
			return Duration{};
		}
	};
}

#endif
