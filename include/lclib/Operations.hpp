#ifndef __Operations_hpp_2018_08_07_10_37
#define __Operations_hpp_2018_08_07_10_37

#include <lclib/TypeTraits.hpp>

#if defined(__cpp_impl_three_way_comparison)&&__cpp_impl_three_way_comparison>=201711L

#if __has_include(<compare>)
#include <compare>
#define LIBLC_CXX_HAS_THREEWAY_COMPARE
#endif

#endif

class NotCopyable{
private:
    NotCopyable(const NotCopyable&)=delete;
    NotCopyable& operator=(const NotCopyable&)=delete;
public:
    constexpr NotCopyable(NotCopyable&&)=default;
    NotCopyable& operator=(NotCopyable&&)=default;
    constexpr NotCopyable()=default;
    ~NotCopyable()=default;
};

class NotMoveable{
private:
    NotMoveable(NotMoveable&&)=delete;
    NotMoveable& operator=(NotMoveable&&)=delete;
public:
    constexpr NotMoveable(const NotMoveable&)=default;
    NotMoveable& operator=(const NotMoveable&)=default;
    constexpr NotMoveable()=default;
    ~NotMoveable()=default;
};

class NotAssignable{
private:
    NotAssignable& operator=(const NotAssignable&)=delete;
    NotAssignable& operator=(NotAssignable&&)=delete;
public:
    constexpr NotAssignable(const NotAssignable&)=default;
    constexpr NotAssignable(NotAssignable&&)=default;
    constexpr NotAssignable()=default;
    ~NotAssignable()=default;
};

class Disable:NotMoveable,NotCopyable{
private:
    Disable()=delete;
    ~Disable()=delete;
};

template<typename T> class RelOps{
public:
	constexpr friend bool operator<=(const T& lhs,const T& rhs){
		return (lhs<rhs)||(lhs==rhs);
	}
	constexpr friend bool operator>=(const T& lhs,const T& rhs){
		return (rhs<=lhs);
	}
	constexpr friend bool operator> (const T& lhs,const T& rhs){
		return (rhs<lhs);
	}
	constexpr friend bool operator!=(const T& lhs,const T& rhs){
		return !(lhs==rhs);
	}
};

template<typename T> class StrictOrder{
public:
	constexpr friend bool operator==(const T& lhs,const T& rhs){
		return !(lhs<rhs)&&!(rhs<lhs);
	}
};



#endif
