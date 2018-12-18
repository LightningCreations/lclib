# PolymorphicWrapper #

In header `<lclib/Wrappers.hpp>`<br/>
Provides a Type Erased Wrapper on a polymorphic type and its subclass.


## Synopsis ##

```cpp
template<typename T> class PolymorphicWrapper;
```

## PolymorphicWrapper ##

```cpp
template<typename T> class PolymorphicWrapper{
	typedef std::remove_cv_t<T> value_type;
	typedef /*see below*/ pointer;
	typedef /*see below*/ const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type&& rvalue_reference;
	template<typename U> using derived_type = /*see below*/;
	template<typename U> using derived_reference = /*see below*/;
	template<typename U> using const_derived_reference = /*see below*/;
	template<typename U> using derived_rvalue_reference = /*see below*/;
	template<typename U> using derived_pointer = /*see below*/;
	template<typename U> using const_derived_pointer = /*see below*/;
	template<typename U> using derived_polymorphic_wrapper = /*see below*/;
	PolymorphicWrapper();
	~PolymorphicWrapper();
	PolymorphicWrapper(const value_type&);
	PolymorphicWrapper(value_type&&);
	PolymorphicWrapper(const value_type&&)=delete;
	PolymorphicWrapper(PolymorphicWrapper&&);
	template<typename U> PolymorphicWrapper(PolymorphicWrapper<U>&&);
	PolymorphicWrapper(const T&);
	PolymorphicWrapper(T&&);
	template<typename U> PolymorphicWrapper(const U&);
	template<typename U> PolymorphicWrapper(U&&);
	template<typename U> PolymorphicWrapper(std::in_place_type_t<U>);
	template<typename U,typename... Args> PolymorphicWrapper(std::in_place_type_t<U>,Args&&...);
	template<typename U> PolymorphicWrapper(PolymorphicWrapper<U>&&);
	pointer operator->();
	const_pointer operator->()const;
	reference operator*()&;
	const_reference operator*()const&;
	rvalue_reference operator*()&&;
	operator reference()&;
	operator const_reference()const&;
	operator rvalue_reference()&&;
	template<typename U> explicit operator U&()&;
	template<typename U> explicit operator const U&()const&;
	template<typename U> explicit operator U&&()&&;
	template<typename U> derived_reference<U> checkedcast()&;
	template<typename U> const_derived_reference<U> checkedcast()const&;
	template<typename U> derived_rvalue_reference<U> checkedcast()&&;
	template<typename U> bool instanceof();
	PolymorphicWrapper& operator=(PolymorphicWrapper&&);
	template<typename U> PolymorphicWrapper& operator=(PolymorphicWrapper<U>&&);
	PolymorphicWrapper& operator=(const PolymorphicWrapper&)=delete;
};
//Non-class Members:
template<typename T> friend void swap(PolymorphicWrapper<T>&,PolymorphicWrapper<T>&);
//Deduction Guides
template<typename T> PolymorphicWrapper(T)->PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(std::in_place_type_t<T>) -> PolymorphicWrapper<T>;
template<typename T,typename... Args> PolymorphicWrapper(std::in_place_type_t<T>,Args...) -> PolymorphicWrapper<T>;
```

PolymorphicWrapper is a Type Erased Wrapper that owns an object of T, or a subclass. 
PolymorphicWrapper knows about the objects dynamic type, so it can be casted (either using static_cast, or the checkedcast method) to derived types. 
You can also query the dynamic type of the object or check if the object is an instance of a given type. 
PolymorphicWrappers are Move-Only types. When moved, they transfer ownership of the owned object without constructing a new object. 

PolymorphicWrappers can only be instantiated with class types. A program which instantiates PolymorphicWrapper for any type except a non-union class type is ill-formed. 

PolymorphicWrappers can only be used with polymorphic types. If `T` is not a polymorphic type the the behavior is undefined. 

The behavior of a program which specializes PolymorphicWrapper is undefined. 

### Member Types ###

```cpp
typedef std::remove_cv_t<T> value_type;//(1)
typedef /*see below*/ pointer;//(2)
typedef /*see below*/ const_pointer; //(3)
typedef value_type& reference; //(4)
typedef const value_type& const_reference; //(5)
typedef value_type&& rvalue_reference; //(6)
template<typename U> using derived_type = /*see below*/; //(7)
template<typename U> using derived_reference = /*see below*/; //(8)
template<typename U> using const_derived_reference = /*see below*/; //(9)
template<typename U> using derived_rvalue_reference = /*see below*/; //(10)
template<typename U> using derived_pointer = /*see below*/; //(11)
template<typename U> using const_derived_pointer = /*see below*/; //(12)
template<typename U> using derived_polymorphic_wrapper = /*see below*/; //(13)
```

(1): The base type of the PolymorphicWrapper. There is no distinction between PolymorphicWrappers instantiated with cv-qualified types. <br/>
(2): The pointer type of the PolymorphicWrapper. 
Defined as either `value_type*`, or some type for which `std::pointer_traits<pointer>::element_type` is `value_type`.
<br/>
(3): The constant pointer type of the PolymorphicWrapper. Defined as `std::pointer_traits<pointer>::rebind<const value_type>`<br/>
(4): The reference type of PolymorphicWrapper.<br/>
(5): The constant reference type of the PolymorphicWrapper.<br/>
(6): The rvalue reference type of the PolymorphicWrapper. Used when converting to base/dereferencing an rvalue PolymorphicWrapper.<br/>
(7): A specific derived type. 
If `U` is the same type as `T`, defined as `value_type`. 
Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `Q`. 
Otherwise undefined. <br/>
(8): The reference type to a specific derived type. 
If `U` is the same type as `T`, defined as `reference`. Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `Q&`. 
Otherwise undefined. <br/>
(9): The constant reference type to a specific derived type. 
If `U` is the same type as `T`, defined as `const_reference`. 
Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `const Q&`. 
Otherwise undefined. <br/>
(10): The rvalue reference type to a specific derived type. 
If `U` is the same type as `T`, defined as `rvalue_reference`. 
Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `Q&&`. 
Otherwise undefined. <br/>
(11): The pointer type for a specific derived type. 
If `U` is the same type as `T`, defined as `pointer`. 
Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `std::pointer_traits<pointer>::rebind<Q>`. 
Otherwise undefined. <br/>
(12): The constant pointer for a specific derived type. 
If `U` is the same type as `T`, defined as `const_pointer`. 
Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `std::pointer_traits<pointer>::rebind<const Q>`. 
Otherwise undefined.<br/>
(13): The PolymorphicWrapper instantiation for a specific derived type. 
If `U` is the same type as `T`, defined as the current instantiation. 
Otherwise, given the type `Q` which is `std::remove_cv_t<U>`, if `std::is_base_of_v<T,Q>` is true, defined as `PolymorphicWrapper<Q>`. 
Otherwise undefined.<br/>

### Constructors/Destructor/Assignment Operators ###

```cpp
constexpr PolymorphicWrapper() noexcept; //(1)
~PolymorphicWrapper(); //(2)
PolymorphicWrapper(PolymorphicWrapper&& rhs)noexcept; //(3)
PolymorphicWrapper(const PolymorphicWrapper&)=delete; //(4)
template<typename U> PolymorphicWrapper(PolymorphicWrapper<U>&& rhs)noexcept; //(5)
PolymorphicWrapper(const T& val) noexcet(/*see below*/); //(6)
PolymorphicWrapper(T&& val) noexcept(/*see below*/); //(7)
template<typename U> PolymorphicWrapper(const U& val) noexcept(/*see below*/); //(8)
template<typename U> PolymorphicWrapper(U&& val) noexcept(/*see below*/); //(9)
template<typename U> PolymorphicWrapper(std::in_place_type_t<U>) noexcept(/*see below*/); //(10)
template<typename U,typename... Args> PolymorphicWrapper(std::in_place_type_t<U>,Args&&... args) noexcept(/*see below*/); //(11)
PolymorphicWrapper& operator=(PolymorphicWrapper&&) noexcept; //(12)
PolymorphicWrapper& operator=(const PolymorphicWrapper&)=delete; //(13)
template<typename U> PolymorphicWrapper& operator=(PolymorphicWrapper<U>&&) noexcept; //(14)
```

1. Default Constructs a Polymorphic wrapper with no owned value. The dynamic type of this wrapper is `T`. 
2. Destroys a Polymorphic Wrapper. If the wrapper has an owned value, then the destructor of the polymorphic wrapper's dynamic type is called. If `T` does not have a virtual destructor, the behavior is undefined if the wrapper owns a value. 
3. Move Constructor. If rhs owns a value, then constructs a PolymorphicWrapper that owns the value owned by rhs (after this, rhs ceases to own a value). If rhs does not own a value, acts as the default constructor. The dynamic type of the constructed polymorphic wrapper is the dynamic type of rhs. After the constructor the dynamic type of rhs is `T`. 
4. Deleted Copy Constructor. PolymorphicWrapper is Move Only. 
5. Move from derived polymorphic wrapper. Acts identically to the normal move constructor. This constructor only participates in overload resolution if `std::is_base_of_v<U,T>` is true. 
6. Constructs the owned value using the Copy Constructor of `T` with `T` as the dynamic type. This constructor only participates in overload resolution if `std::is_copy_constructible_v<T>` is true. 
7. Constructs the owned value using the Move Constructor of `T` with `T` as the dynamic type. This constructor only participates in overload resolution if `std::is_move_constructible_v<T>` is true. 
8. If `std::is_copy_constructible_v<U>` is true, constructs the owned value using the copy constructor of `U` with `U` as the dynamic type. Otherwise, defined as deleted. This constructor only participates in overload resolution if `std::is_base_of_v<U,T>` is true. 
9. If `std::is_move_constructible_v<U>` is true, constructs the owned value using the move constructor of `U` with `U` as the dynamic type. Otherwise, defined as deleted. This constructor only participates in overload resolution if `std::is_base_of_v<U,T>` is true. 
10. In-place default constructor. Constructs the owned value using the default constructor of `U` with `U` as the dynamic type. This constructor only participates in overload resolution if `std::is_base_of_v<U,T>` and `std::is_default_constructible_v<U>` are both true.
11. In-place constructor. Constructs the owned value using the default constructor of `U` with `U` as the dynamic type. This constructor only participates in overload resolution if `std::is_base_of_v<U,T>` and `std::is_constructible_v<U,Args...>` are both true. 
12. Move Assignment Operator. Same as `swap(*this,rhs); return *this;` 
13. Deleted Copy Assignment Operator. 
14. Move Assign from Derived Polymorphic Wrapper. Effectively the same as `return *this=PolymorphicWrapper{rhs};`. 

#### Noexcept Specification ####

(1), (3), (5), (12), (14): noexcept

(6): `noexcept(std::is_nothrow_copy_constructible_v<T>)`

(7): `noexcept(std::is_nothrow_move_constructible_v<T>)`

(8): `noexcept(std::is_nothrow_copy_constructible_v<U>)` unless deleted

(9): `noexcept(std::is_nothrow_move_constructible_v<U>)` unless deleted

(10): `noexcept(std::is_nothrow_default_constructible_v<U>)`

(11): `noexcept(std::is_nothrow_constructible_v<U,Args...>)`


#### Exceptions ####

(1), (3), (5), (12), (15): noexcept

(6), (7), (8), (9), (10), (11): Throws anything that the constructor of `T` or `U` mandated by the specific constructor throws. 

### Explicit dynamic type operations ###

```cpp
template<typename U> derived_reference<U> checkedcast() &; //(1)
template<typename U> const_derivied_reference<U> checkedcast() const&; //(1)
template<typename U> derived_rvalue_reference<U> checkedcast() &&; //(1)
template<typename U> bool isinstance() noexcept; //(2)
const std::type_info& type() noexcept; //(3)
```

1. Explicitly performs a checked cast of the Owned object to U. Same as `static_cast<U&>(*this)`, `static_cast<const U&>(*this)`, or `static_cast<U&&>(*this)` respectively. 

2. Checks if this has an owned object, and that they dynamic type of the owned object is either `U` or a type that derives publicly and unambiguously from `U`. The program is ill-formed if `std::is_base_of_v<U,T>` is false. 

3. Obtains the dynamic type of the owned object. If there is an owned object then returns typeid(**this), otherwise returns typeid(T). 

#### Exceptions ###

(1): Throws an exception that matches `std::bad_cast` as specified above. 

(2), (3): noexcept

### Pointer Operations ###

```cpp
T& operator*()& noexcept; //(1)
const T& operator*()const& noexcept; //(1)
T&& operator*()&& noexcept; //(1)
T* operator->() noexcept; //(2)
const T* operator->() const noexcept; //(2)
```

1. Obtains a reference to the owned object. The behavior is undefined if this does not have an owned object. 
2. Allows indirection through a PolymorphicWrapper to the Owned Object. 

#### Exceptions ####

noexcept


### Conversion and Cast operators ###

```cpp
operator T&()& noexcept; //(1)
operator const T&()const T& noexcept; //(1)
operator T&&()&& noexcept; //(1)
template<typename U> explicit operator U&()&; //(2)
template<typename U> explicit operator const U&()const&; //(3)
template<typename U> explicit operator U&&()&&; //(4)
```

1. Same as (`**this`).
2. If the dynamic type of this object is `U`, then given v which equals `this->operator->()`, returns the result of `*static_cast<U*>(dynamic_cast<void*>(v))`. Otherwise, if the dynamic type of this object, `D` is a public and unambiguous subclass of `U`, then returns the result of `*dynamic_cast<U*>(static_cast<D*>(dynamic_cast<void*>(v)))`. Otherwise, throws an exception which matches a handler of type `std::bad_cast`. Does not participate in overload resolution unless `std::is_base_of_v<U,T>` is true. 
3. Returns `const_cast<const U&>(static_cast<U&>(const_cast<PolymorphicWrapper<T>&>(*this))`. 
4. Returns `std::move(static_cast<U&>(*this))` given that *this is an lvalue.

#### Exceptions ####

(1) noexcept

(2), (3), (4) Throws an exception of type `std::bad_cast` if the dynamic type of the owned object is neither `U` nor a public and unambiguous subclass of `U`. 

### Swap ###

```cpp
template<typename T> friend void swap(PolymorphicWrapper<T>& lhs,PolymorphicWrapper<T>& rhs) noexcept;
```

Swaps lhs with rhs. If lhs owns an object, then that object will be owned by rhs after the swap, and vice-versa. If neither lhs nor rhs owns an object, then the swap has no effect. 

#### Exceptions ####

noexcept

## Deduction Guides ##

```cpp
template<typename T> PolymorphicWrapper(T) -> PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(std::in_place_type_t<T>) -> PolymorphicWrapper<T>;
template<typename T,typename... Args> PolymorphicWrapper(std::in_place_type_t<T>,Args...) -> PolymorphicWrapper<T>;
```


