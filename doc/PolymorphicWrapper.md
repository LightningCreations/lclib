<h1>PolymorphicWrapper</h1>

In header `<lclib-cxx/Wrappers.hpp>`<br/>
Provides a Type Erased Wrapper on a polymorphic type and its subclass.


<h2>Synopsis</h2>

```cpp
template<typename T> class PolymorphicWrapper;
template<typename T> PolymorphicWrapper(const T&)->PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(T&&)->PolymorphicWrapper<T>;
template<typename T> PolymorphicWrapper(std::in_place_type_t<T>)->PolymorphicWrapper<T>;
template<typename T,typename... Args> PolymorphicWrapper(std::in-place_type_t<T>,Args&&...)->PolymorphicWrapper<T>;
template<typename T> void swap(PolymorphicWrapper<T>&,PolymorphicWrapper<T>&);
```

<h2>PolymorphicWrapper</h2>

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
	PolymorphicWrapper(const PolymorphicWrapper&)=delete;
	template<typename U> PolymorphicWrapper(const U&);
	template<typename U> PolymorphicWrapper(U&&);
	template<typename U> PolymorphicWrapper(const U&&)=delete;
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
```

PolymorphicWrapper is a Type Erased Wrapper that owns an object of T, or a subclass. 
PolymorphicWrapper knows about the objects dynamic type, so it can be casted (either using static_cast, or the checkedcast method) to derived types. 
You can also query the dynamic type of the object or check if the object is an instance of a given type. 
PolymorphicWrappers are Move-Only types. When moved, they transfer ownership of the owned object without constructing a new object. 

PolymorphicWrappers can only be instantiated with class types. A program which instantiates PolymorphicWrapper for any type except a non-union class type. 

PolymorphicWrapper may only be instatiated with polymorphic types. The behavior of a program which constructs a PolymorphicWrapper (except by default constructing it), or assigns a PolymorphicWrapper for a non-polymorphic type is undefined. 

The behavior of a program which specializes PolymorphicWrapper is undefined. 

<h3>Member Types</h3>

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

<h3>Constructors/Destructor/Assignment Operators</h3>

```cpp
PolymorphicWrapper(); //(1)
~PolymorphicWrapper(); //(2)

```
 