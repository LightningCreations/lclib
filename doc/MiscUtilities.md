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

## RelOps/StrictOrder ##

RelOps and StrictOrdering are template types which provide default definitions for most relational operators for a given type, given that a minimum set is provided by the that type. 
The absolute minimum for both RelOps and StrictOrdering is that the type overload the less-than operator. 

Both types are intended for use via CRTP. The operator overloads provided by RelOps and StrictOrdering are only guaranteed to be found by ADL. 
These intend to replace the similar namespace `std::rel_ops`, as the operators provided are guaranteed to be found via ADL Lookup, whereas the operators in `std::rel_ops` will only be found via qualified lookup. 

If a program specializes RelOps or StrictOrdering, the behavior is undefined. 

Specializations of RelOps and StrictOrdering are empty types, and satisfy all of *TrivialType*, *StandardLayoutType*, and *LiteralType*. Inheriting from an Specialization of RelOps or StrictOrdering will not disqualify a type from satisfying *BytesWriteable* or *BytesReadable*. 

If RelOps or StrictOrdering is instantiated for some `T`, where for some values `t1` and `t2` which are of type (possibly const-qualified) `T`, the expression `t1<t2` is not well-formed in an unevaluated context, the program is ill-formed, no diagnostic required. Given those values:

* `t1<t1` must be false, 
* At most one of `t1<t2` and `t2<t1` may be true, and
* If `t1` and `t2` are distinct then exactly one of `t1<t2` and `t2<t1` shall be true, where they are distinct if `!(t1==t2)` is false. If the the expression `t1==t2` is not well-formed, they are distinct if any state which is compared for the expression `t1<t2` is distinct

If either of those requirements are not met at the point that RelOps or StrictOrdering are instantiated for a type `T`, the behavior is undefined. 
 

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

### Comparison Operator Definitions ###

```cpp
//In class RelOps<T>
constexpr friend bool operator>(const T& t1,const T& t2); //(1)
constexpr friend bool operator<=(const T& t1,const T& t2); //(2)
constexpr friend bool operator>=(const T& t1,const T& t2); //(3)
constexpr friend bool operator!=(const T& t1,const T& t2); //(4)
//In class StrictOrder<T>
constexpr friend bool operator==(const T& t1,const T& t2); //(5)
```

(1): Defined as t2<t1.  

(2): Defined as !(t2<t1). 

(3): Defined as !(t1<t2).

(4): If `t1==t2` is a well-formed expression, then defined as `!(t1==t2)`. Otherwise, defined as `t1<t2||t2<t1`

(5): Defined as `!(t1<t2||t2<t1)`. 

## ValueDiscard ##

ValueDiscard is a class that can be assigned from any value of `T`. The assignment is a no-op, and has no effect. 
There is a full specialization for void that can be assigned from any type. 

A program may specialize ValueDiscard for a `User-specified type` as long as that type is a (possibly cv-qualified) class type, provided that the specialization is assignable from a (possibly const-qualified) lvalue of that type, the destructor is trivial, and the default constructor can be called in a constant expression. 
The assignment operator must not throw any exceptions unless `T` is volatile-qualified, and evaluating the argument results in an exception. 
The assignment operator of a user-provided specialization of `ValueDiscard` is permitted to evaluate its argument even if the type is not volatile-qualified, but such an evaluation may not throw any exceptions (unless the type is volatile-qualified). 
This permits specializations for user-provided types that emulate volatile (reading them itself is observeable behavior) to fully emulate the behavior of volatile.  

If `ValueDiscard<T>` is a user-provided specialization and `ValueDiscard<const T>` is not, then assigning to the `ValueDiscard<const T>` has the same result as assigning to `ValueDiscard<T>`. 


A program that instantiates ValueDiscard with a reference type, a function-type, or with cv-qualified void is ill-formed. 

```cpp
template<typename T=void> struct ValueDiscard{
public:
	constexpr ValueDiscard()=default;
	constexpr ValueDiscard& operator=(const T&);
};
template<> struct ValueDiscard<void>{
public:
	constexpr ValueDiscard()=default;
	template<typename T> constexpr ValueDiscard& operator=(T&&);
};
```

### Discard Assignment Operator ###

```cpp
constexpr ValueDiscard& operator=(const T&); //Member of Primary Template, (1)
template<typename T> constexpr ValueDiscard& operator=(const T&); //Member of specialization for void, (2)
```

(1): Performs no operation, effectively discarding the value of the right-hand side. 

If `T` is a volatile-qualfied type, it shall be a scalar type or the behavior is undefined (this does not apply for user-provided specializations). The discard operation shall be treated as a volatile access. 
If `T` is not a volatile-qualified type, then the parameter is guaranteed not to be read, copied, or moved. 

If `T` is a volatile-qualified type, then the assignment operator is not required to be constexpr (this permits implementations to provide this operator in the form of a partial specialization). 

(2):
Generic version of (1) for the void specialization. If `ValueDiscard<T>` is not a user-provided specialization of `ValueDiscard`, then it performs the operations defined above. Otherwise, default constructs an instance of the specialization and invokes the assignment operator on the instance with the parameter. 

As above, if `T` is a volatile-qualified type, then the assignment operator is not required to be constexpr (this permits implementations to provide this operator as an overload set).

### Exceptions ###

(1): The primary template performs no operation (except for the possible evaluation of volatile-qualified scalar types), and therefore will not throw an exception. 
Specializations for volatile-qualified types may throw an exception if evaluating the volatile object results in an exception, but otherwise they must not throw any exceptions. 

(2): If `T` is a volatile qualified class-type for which `ValueDiscard<T>` is a user-provided specialization of `ValueDiscard`, then 


## struct discard ##

discard is an output iterator that can be dereferenced to a value of some type which can be assigned from any value of any type. The result of that assignment is the same as the equivalent assignment to a glvalue of `ValueDiscard<void>`. 

Implementations are permitted to overload `operator=` in discard that can accept both lvalues and rvalues of any type, but are not required to. It is unspecified if this occurs. 

### Class Synopsis ###
```cpp
struct discard{
public:
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;
	using iterator_category = std::output_iterator_tag;
	constexpr discard()=default;
	constexpr discard& operator++();
	constexpr discard& operator++(int);
	constexpr /*unspecified*/ operator*();
};
```

### Type Aliases ###

```cpp
using value_type = void; //(1)
using difference_type = void; //(1)
using pointer = void; //(1)
using reference = void; //(1)
using iterator_category = std::output_iterator_tag; //(2)
```

(1): Defined to allow the primary template of `std::iterator_traits` to work properly with `discard`. 

(2): Specifies that `discard` is an output iterator.  

### Iterator Operations ###
```cpp
constexpr discard& operator++(); //(1)
constexpr discard& operator++(int); //(1)
constexpr /*unspecified*/ operator*(); //(2)
```

(1): Overloaded to satisfy *LegacyOutputIterator*. No-op. An instance of discard is always dereferencable and incrementable. 

(2): Results in a value of some type that satisfies *LiteralType* and can be assigned from any type. The result of the assignment shall be the same as the equivalent assignment to a glvalue of `ValueDiscard<void>`. 

#### Exceptions ####

None of these operators may throw an exception. 

## struct split ##

Provides an *LegacyOutputIterator* that splits writes between 2 *LegacyOutputIterator*s. 

`Predicate` shall have the signature `bool operator()(const T1& t)const`. 
Note that the parameter does not have to be `const&`, but the predicate must not modify the parameter.  

Additionally, There shall be at least one type `T`, which makes the following expressions well-formed, given `t` is an lvalue of type `T`, itr1 is an lvalue of Iterator1, itr2 is an lvalue of Iterator2, and pred is a possibly const-qualified value of Predicate.:

* `std::invoke(pred,t)`, which results in a `bool` or a value of a type which is contextually convertable to `bool`,  
* `*(itr1++) = std::move(t)`
* `*(itr2++) = std::move(t)`

If a program instantiates `split<Iterator1,Iterator2,Predicate>`, and no such valid type `T` is available to the program, the that program is ill-formed, no diagnostic required. 

split may be specialized if at least one iterator type is a user-defined type or the Predicate is or depends on a user-defined type, and the general contract of split is still kept. 

### Class Synopsis ###

```cpp
template<typename Iterator1,typename Iterator2,typename Predicate> struct split{
private:
	Iterator1 itr1; //exposition only
	Iterator2 itr2; //exposition only
	Predicate pred; //exposition only
public:
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;
	using iterator_category = std::output_iterator_tag;
	template<typename Pred,typename Itr1,typename Itr2> constexpr explicit split(Itr1&& itr1,Itr2&& itr2,Pred&& pred);
	constexpr split& operator++();
	constexpr split& operator++(int);
	constexpr split& operator*();
	template<typename T> constexpr void operator=(T&& val);
};
```

### Iterator typedefs ###

```cpp
using value_type = void;
using difference_type = void;
using pointer = void;
using reference = void;
using iterator_category = std::output_iterator_tag;
```

Provided for convience, to allow use of `std::iterator_traits` with specializations of `split`. 

### Constructor ###

```cpp
template<typename Pred,typename Itr1,typename Itr2> constexpr explicit split(Itr1&& itr1,Itr2&& itr2,Pred&& pred);
```

Constructs the internal iterators and predicate by forwarding its parameters. 
This constructor may be provided as a set of overloads, rather than a template. 

This constructor does not participate in overload resolution unless for each type `T` in `Itr1`, `Itr2`, `Pred`, and the associated type `U` in `Iterator1`, `Iterator2`, `Predicate` respectively; `std::is_same_v<std::decay_t<T>,std::decay_t<U>>` and `std::is_constructible_v<T,U&&>` are both true. 

Specifically this template is defined to allow users to construct `split` from any combination of lvalues and rvalues and call the appropriate constructor, and may be provided as an overload set of constructors. 

### Iterator Operations ###

```cpp
constexpr split& operator++(); //(1)
constexpr split& operator++(int); //(1)
constexpr split& operator*(); //(1)
template<typename T> constexpr void operator=(T&& t); //(2)
```

(1): All are no-ops that simply return `*this`.

(2): If the result of the expression `std::invoke(pred,t)`, contextually converted to bool, is true, increments and dereferences itr1 and assigns `std::forward<T>(t)` to it. Otherwise increments and dereferences itr2 and assigns `std::forward<T>(t)` to it. 

The predicate shall be called as if by `std::invoke(pred,t)`, assigning to itr1 should be done as if by `(*itr1++) = std::forward<T>(t)`, assigning itr2 should be done as if by `(*itr2++) = std::forward<T>(t)`. 

This method does not participate in overload resolution unless pred is Callable with an lvalue of `T`, and both `itr1` and `itr2` can be referenced and the result assigned with an xvalue of `T` (if `T` is not a reference type), or an lvalue of `std::remove_reference_t<T>` (if `T` is an lvalue reference type). 

#### Exceptions ####

(1): These operations shall not throw any exceptions

(2): Throws any of the exceptions that are thrown by the evaulation of any of the specified above that were evaluated. 

### struct numeric_range ###

In header `<lclib/Ranges.hpp>`

`numeric_range` is a range that iterates over values between an minimum and a maximum value of some type.

`numeric_range` satisfies *TriviallyCopyable*, *StandardLayoutType*, *LiteralType*. 

#### Type Requirements ####

If class non-type template parameters are supported, then `T` shall satisfy *StrictNumeric* or the program is ill-formed, no diagnostic required. 
Otherwise `T` shall be an integral type that is not `bool`. 
Additionally, the expressions `++t` and `--t` shall be well-formed. 

Additionally, `min<max` shall be true or the program is ill-formed. 

#### Class Synopsis ####

```cpp
template<typename T,T min=std::numeric_limits<T>::min(),T max=std::numeric_limits<T>::max()> struct numeric_range{
public:
	struct iterator;
	using const_iterator = iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using value_type = T;
	using low_constant = std::integral_constant<T,min>;
	using high_constant = std::integral_constant<T,max>;
	constexpr explicit numeric_range()=default;
	oonstexpr explicit numeric_range(low_constant,high_constant=high_constant());c
	constexpr numeric_range(const numeric_range&)=default;
	constexpr numeric_range& operator=(const numeric_range&)=default;
	~numeric_range()=default;
	constexpr iterator begin() const;
	constexpr iterator end() const;
	constexpr const_iterator cbegin()const;
	constexpr const_iterator cend()const;
	constexpr reverse_iterator rbegin()const;
	constexpr reverse_iterator rend()const;
	constexpr const_reverse_iterator crbegin()const;
	constexpr const_reverse_iterator crend()const;
	constexpr iterator::difference_type size()const;
	constexpr bool empty()const;
};
```


