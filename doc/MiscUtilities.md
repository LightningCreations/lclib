# Miscellaneous Utilities #

Included from `<lclib/Operations.hpp>`, `<lclib/Types.hpp>`, `<lclib/Iterators.hpp>`, and `<lclib/Ranges.hpp>`. 

## Synopsis ##

```cpp
//In <lclib/Operations.hpp>
struct NotCopyable;
struct NotMoveable;
struct NotAssignable;
struct Disable;
template<typename T> struct RelOps;
template<typename T> struct StrictOrder;
//In <lclib/Types.hpp>
template<typename T=void> struct ValueDiscard;
template<> struct ValueDiscard<void>;
//In <lclib/Iterators.hpp>
#include <iterator>
struct discard;
template<typename Itr1,typename Itr2,typename Predicate> struct split;
//In <lclib/Ranges.hpp>
template<typename T,T min=std::numeric_limits<T>::min(),T max=std::numeric_limits<T>::max()> struct numeric_range;
template<typename Range> struct reverse;
```

## NotCopyable/NotMoveable/NotAssignable/Disable ##

(In header `<lclib/Operators.hpp>`)

Classes which define a specific set of special member functions as deleted (and define every other special member function as default). Used to explicitly disable a certain implicit operations. 

Disable defines all special members as deleted. This effectively makes classes that inherit from it (or Disable itself) unusable (as of course the Destructor is defined as deleted). This can be useful for selectively disabling certain instantiations of a template. However, in most cases, use of `static_assert` is preferred to indicate that certain template parameters are invalid. 
A conditional version of this template may be provided in the future. 

The classes (except Disable) are deprecated as of 1.3 and will be removed in a future version. 


```cpp
struct NotCopyable{
private:
	NotCopyable(const NotCopyable&)=delete;
	NotCopyable& operator=(const NotCopyable&)=delete;
public:
	constexpr NotCopyable(NotCopyable&&)=default;
	constexpr NotCopyable& operator=(NotCopyable&&)=default;
	constexpr NotCopyable()=default;
};
struct NotMoveable{
private:
	NotMoveable(NotMoveable&&)=delete;
	NotMoveable& operator=(NotMoveable&&)=delete;
public:
	constexpr NotCopyable(const NotCopyable&)=default;
	constexpr NotCopyable& operator=(const NotCopyable&)=default;
	constexpr NotCopyable()=default;
};
struct NotAssignable{
private:
	NotAssignable& operator=(const NotAssignable&)=delete;
public:
	constexpr NotAssignable(const NotAssignable&)=default;
	constexpr NotAssignable(NotAssignable&&)=default;
	constexpr NotAssignable()=default;
};
struct Disable{
private:
	Disable()=delete;
	Disable(const Disable&)=delete;
	Disable(Disable&&)=delete
	Disable& operator=(const Disable&)=delete;
	Disable& operator=(Disable&&)=delete;
	~Disable()=delete;
};
```

Notes: 

The definitions provided here are provided only as a guide. Any combiniation of special member functions may be defined as defaulted and defined as deleted and simply left undefined or implicit, as long as the desired effect is achieved. 

For example, Disable can disable the constructors by inheriting from NotCopyable and NotMoveable, or can simply delete the copy overloads (and leave the move overloads undefined). 

All of these types shall be empty types and satisfy *StandardLayoutType*. Additionally all of these types, except Disable, shall satisfy *TrivialType* and *LiteralType*.

### RelOps/StrictOrdering ###

RelOps and StrictOrdering are template types which provide default definitions for most relational operators for a given type, given that a minimum set is provided by the that type. 
The absolute minimum for both RelOps and StrictOrdering is that the type overload the less-than operator. 

Both types are intended for use via CRTP. The operator overloads provided by RelOps and StrictOrdering are only guaranteed to be found by ADL. 
These intend to replace the similar namespace `std::rel_ops`, as the operators provided are guaranteed to be found via ADL Lookup, whereas the operators in `std::rel_ops` will only be found via qualified lookup. 

If a program specializes RelOps or StrictOrdering, the behavior is undefined. 

Specializations of RelOps and StrictOrdering are empty types, and satisfy all of *TrivialType*, *StandardLayoutType*, and *LiteralType*. Inheriting from an Specialization of RelOps or StrictOrdering will not disqualify a type from satisfying *BytesWriteable* or *BytesReadable*. 

A objects of a type that inherits from a specialization RelOps or StrictOrdering shall not be deleteable by a pointer to either the specialization or a pointer to any base classes the specialization may have. 

```cpp
template<typename T> struct RelOps{
protected:
	~RelOps();
public:
	constexpr RelOps()=default;
	constexpr RelOps(const RelOps&)=default;
	constexpr RelOps& operator=(const RelOps&)=default;
	constexpr friend bool operator>(const T& t1,const T& t2);
	constexpr friend bool operator<=(const T& t1,const T& t2);
	constexpr friend bool operator>=(const T& t1,const T& t2);
	constexpr friend bool operator!=(const T& t1,const T& t2);
};
template<typename T> struct StrictOrder{
protected:
	~StrictOrder();
public:
	constexpr StrictOrder()=default;
	constexpr StrictOrder(const StrictOrder&)=default;
	constexpr StrictOrder& operator=(const StrictOrder&)=default;
	constexpr friend bool operator==(const T& t1,const T& t2);
	
};
```


