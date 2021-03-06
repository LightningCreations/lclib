# Vector Library #

Included from `<lclib/Vector.hpp>`

## Header Synopsis ##

```cpp
template<typename T> struct Vec2;
template<typename T> struct Matrix;
template<typename T> constexpr const Vec2<T> ORIGIN{};
template<typename T> constexpr /*see below*/ hashcode(const Vec2<T>&);
namespace std{
	template<typename T> struct hash<Vec2<T>>;
	template<typename Q,typename U> struct common_type<Vec2<Q>,Vec2<U>>;
}
```

## class Vec2 ##

A class that represents a Point in the 2d plane, where the position on both the x and y axis are stored as a value of `T`. 

`Vec2<T>` satisfies the requirements of *Numeric*. Additionally if `T` satisfies any of the following Named Requirements, `Vec2<T>` also satisfies that Named Requirement:

* TriviallyCopyable
* StandardLayoutType
* LiteralType
* EqualityComparable
* BytesReadable
* BytesWritable


### Notes ###

`NaN` values are not acceptable values for any function or constructor of Vec2. 
If a `NaN` value is a paramater for a constructor or method call, or an operand of any of Vec2's overload operators, or is assigned to either member of a `Vec2` object, the behavior is undefined. 


### Type Requirements ###

`T` shall satisfy the requirements of *Numeric* or the program is ill-formed, no diagnostic required.

If `T` is a specialization of Vec2 or of Matrix, the program is ill-formed. 

### Class Synopsis ###

```cpp
template<typename T> struct Vec2{
	T x;
	T y;
	constexpr explicit Vec2();
	constexpr Vec2(T x,T y);
	template<typename Q,typename U> constexpr EXPLICIT Vec2(Q x,U y);
	constexpr Vec2(const Vec2&)=default;
	constexpr Vec2(Vec2&&)=default;
	template<typename U> constexpr EXPLICIT Vec2(const Vec2<U>&);
	constexpr Vec2& operator=(const Vec2&)=default;
	constexpr Vec2& operator=(Vec2&&)=default;
	constexpr Vec2</*see below*/> operator+(const Vec2& v)const;
	constexpr Vec2& operator+=(const Vec2& v);
	constexpr Vec2</*see below*/> operator-(const Vec2& v)const;
	constexpr Vec2& operator-=(const Vec2& v);
	constexpr Vec2</*see below*/> operator*(T scalar)const;
	constexpr Vec2& operator*=(T scalar);
	constexpr friend Vec2</*see below*/> operator*(T scalar,const Vec2& v)const noexcept(/*see below*/);
	constexpr Vec2</*see below*/> operator/(T)const;
	constexpr Vec2& operator/=(T scalar);
	constexpr friend /*see below*/ operator*(const Vec2& v1,const Vec2& v2)const;
	template<typename U> constexpr friend Vec2</*see below*/> operator+(const Vec2& v1,const Vec2<U>& v2);
	template<typename U> constexpr friend Vec2</*see below*/> operator-(const Vec2& v1,const Vec2<U>& v2);
	template<typename U> constexpr friend Vec2</*see below*/> operator*(const Vec2& v1,U m);
	template<typename U> constexpr friend Vec2</*see below*/> operator/(const Vec2& v1,U m);
	template<typename U> constexpr friend /*see below*/ operator*(const Vec2& v1,const Vec2<U>& v2);
	constexpr friend bool operator==(const Vec2& v1,const Vec2& v2);
	constexpr friend bool operator!=(const Vec2& v1,const Vec2& v2);
	template<typename U> constexpr friend bool operator==(const Vec2& v1,const Vec2<U>& v2);
	template<typename U> constexpr friend bool operator!=(const Vec2& v1,const Vec2<U>& v2);
	constexpr /*see below*/ hashcode()const;
	constexpr /*see below*/ magnetudeSquared()const;
	constexpr /*see below*/ magnetude()const;
	constexpr Vec2</*see below*/> normalize()const;
	constexpr explicit operator bool()const;
};
template<typename T> Vec2(T,T)->Vec2<T>;
template<typename Q,typename U> Vec2(Q,U)->Vec2<std::common_type_t<Q,U>>;
Vec2()->Vec2<int>;
```

### Fields ###

```cpp
T x; //(1)
T y; //(2)
```

1. The x component of the vector. 

### Constructors/Operators ###

The Destructor of Vec2 is implicitly declared. 

```cpp
constexpr explicit Vec2()noexcept(/*see below*/); //(1)
constexpr Vec2(T x,T y)noexcept(/*see below*/); //(2)
template<typename Q,typename U> constexpr EXPLICIT Vec2(Q x,U y)noexcept(/*see below*/); //(3)
constexpr Vec2(const Vec2&)noexcept(/*see below*/)=default; //(4)
constexpr Vec2(Vec2&&)noexcept(/*see below*/)=default; //(5)
template<typename U> constexpr EXPLICIT Vec2(const Vec2<U>&)noexcept(/*see below*/); //(6)
constexpr Vec2& operator=(const Vec2&)noexcept(/*see below*/)=default; //(7)
constexpr Vec2& operator=(Vec2&&)noexcept(/*see below*/)=default; //(8)
```

(1): Value Initializes the x and y components. Same as Vec2(T{},T{})

(2),(3): Copy-initializes the x and y components with the given parameters. 

(3) only participates in overload resolution if `T` is constructible from both `U` and `Q` separately. 

(3) is explicit if either or `U` or `Q` is not convertible to `T`, or the conversion sequence from either `U` or `Q` to `T`requires one or more narrowing conversions. 

(4),(5),(7),(8): Defaulted Copy constructor. Initializes the x and y components from the x and y components of the other vector.

(6): Initializes the x and y components from the x and y components of the other vector.

This constructor does not participate in overload resolution unless `T` is constructible from `U`. 

This constructor is explicit if `U` is not convertible to `T` or the conversion sequence from either `U` to `T` requires one or more narrowing conversions. 

#### Noexcept Specification ####

(1): `noexcept(std::is_nothrow_default_constructible_v<T>)`

(2): `noexcept(std::is_nothrow_copy_constructible_v<T>)`

(3): `noexcept(std::is_nothrow_constructible_v<T,U>&&std::is_nothrow_constructible_v<T,Q>)`

(4): `noexcept(std::is_nothrow_copy_constructible_v<T>)`

(5): `noexcept(std::is_nothrow_move_constructible_v<T>)`

(6): `noexcept(std::is_nothrow_constructible_v<T,const U&>)`

(7): `noexcept(std::is_nothrow_copy_assignable_v<T>)`

(8): `noexcept(std::is_nothrow_move_assignable_v<T>)`


#### Exceptions ####

Throws any exceptions thrown by the underlying constructor/operator calls.   

### Arithmetic Operators ###

```cpp
constexpr Vec2</*see below*/> operator+(const Vec2& v)const noexcept(/*see below*/); //(1)
constexpr Vec2</*see below*/> operator-(const Vec2& v)const noexcept(/*see below*/); //(2)
constexpr Vec2</*see below*/> operator*(T t)const noexcept(/*see below*/); //(3)
constexpr Vec2</*see below*/> operator/(T t)const noexcept(/*see below*/); //(4)
constexpr /*see below*/ operator*(const Vec2& v)const; //(5)
constexpr friend Vec2</*see below*/> operator*(T t,const Vec2& v) noexcept(/*see below*/); //(6)
template<typename U> constexpr friend Vec2</*see below*/> operator+(const Vec2& v1,const Vec2<U>& v2)const noexcept(/*see below*/); //(7)
template<typename U> constexpr friend Vec2</*see below*/> operator-(const Vec2& v1,const Vec2<U>& v2)const noexcept(/*see below*/); //(8)
template<typename U> constexpr friend Vec2</*see below*/> operator*(const Vec2& v1,U m)const noexcept(/*see below*/); //(9)
template<typename U> constexpr friend Vec2</*see below*/> operator*(U m,const Vec2& v1)const noexcept(/*see below*/); //(10)
template<typename U> constexpr friend Vec2</*see below*/> operator/(const Vec2& v1,U m)const noexcept(/*see below*/); //(11)
template<typename U> constexpr friend /*see below*/ operator*(const Vec2& v1,const Vec2<U>& v2)const noexcept(/*see below*/); //(12)
```

(1): Adds the x components and the y components of each vector. The result type is `Vec2<decltype(x+v.x)>`. Identical to Vec2{this->x+v.x,this->y+v.y} with the Deduction guides provided. Both `x+v.x` and `v.x+x` must be equivalent expressions or the behavior is undefined. 

(2): Subtracts the x components and the y components of each vector. The result type is `Vec2<decltype(x-v.x)>`. Identical to Vec2{this->x-v.x,this->y-v.y} with the Deduction guides provided. 

(3),(6),(9),(10): Scales the x and y components of the vector by a factor of t. The result type is `Vec2<decltype(x*t)>`. This method only participates in overload resolution if the expression `x*t` is well formed in an unevaluated context. `t*x` must be an equalivalent to `x*t` otherwise the behavior is undefined. 

(9) and (10) Additionally do not participate in overload resolution if `U` is a specialization of `Vec2` or a specialization of `Matrix`. 

(4),(11): Scales the x and y components of the vector, reducing them by a factor of t. The result type is `Vec2<decltype(x/t)>`. This method only participates in overload resolution if the expression`x/t` is well formed in an unevaluated context. 

(11) additionally does not participate in overload resolution if `U` is a specialization of `Vec2`. 

(5),(12): Computes the dot product of 2 vectors, reducing them to a scalar quantity. This method only participates in overload resolution if the expression `x*t` is well formed in an unevaulated context, the result being q, and `q+q'` is also well formed in an unevaluated context, where q' is another value of the same type as q. The result type is the type of `q+q'`. `q+q'` must be an equivalent operation to `q'+q` as with `x*t` and `t*x` or the behavior is undefined.  

(7): Adds the components of v1 and v2 together. The result type is `Vec2<decltype(v1.x+v2.x)>`. This method only participates in overload resolution if `v1.x+v2.x` is well formed in an unevaluated context. `v1.x+v2.x` shall be an equivalent expression to `v2.x+v1.x` or the behavior is undefined. 

(8): Subtracts the components of v1 and v2. The result type is `Vec2<decltype(v1.x-v2.x)>`. This method only participates in overload resolution if the expression `v1.x-v2.x` is well formed in an unevaluated context. 

There are also equivalent compound operators, when the chosen compound operator is well formed. 

#### Exceptions ####

Each method is noexcept if the expression's checked by the method is known not to throw any exceptions. 

Otherwise the methods throws any exception that the expression throws when it is evaluated. 

### Comparison Operators ###

```cpp
template<typename U> constexpr bool operator==(const Vec2& v1,const Vec2<U>& v2)noexcept(/*see below*/); //(1)
template<typename U> constexpr bool operator!=(const Vec2& v1,const Vec2<U>& v2)noexcept(/*see below*/); //(2)
```

(1): Compares the x and y components of v1 and v2. This method does not participate in overload resolution unless `v1.x==v2.x` is well formed in an unevaluated context. Returns true if and only if the vector represents the same point. 

(2): Compares the x and y components of v1 and v2. This method does not participate in overload resolution unless `v1.x!=v2.x` is well formed in an unevaluated context. Equal to `!(v1.x==v2.x)`. 

Both are noexcept if the expression `v1.x==v2.x` are not known to throw any exceptions. 

### Vector Specific Functions ###

```cpp
constexpr /*see below*/ magnetudeSquared()const noexcept(/*see below*/); //(1)
constexpr /*see below*/ magnetude()const noexcept(/*see below*/); //(2)
constexpr /*see below*/ normalize()const noexcept(/*see below*/); //(3)
constexpr /*see below*/ theta()const noexcept(/*see below*/); //(4)
```

(1): Computes the squared magnetude of this vector. Same as `(*this)*(*this)`. This method does not participate in overload resolution if that expression is not well formed in an unevaluated context. The result type is the result type of that expression. 

(2): Computes the magnetude of this vector. Same as `sqrt(magnetudeSquared())` where sqrt is looked up using ADL and in namespace std. This method does not participate in overload resolution, unless `magnetudeSquared()` is well formed and returns a value of type `Q`, and overload resolution for `sqrt(Q)` succeeds, where `sqrt` is looked up using ADL and in namespace std. The result type is the return type of the method that was resolved. 

(3): Normalizes the vector (reduces it to a vector with length 1). This is the same as `(*this)/magnetude()`. If that expression is not well formed, then the method does not participate in overload resolution. The result type is the result type of that expression. 

(4): Returns the angle of this vector. This is the same as `atan2(x,y)` where atan2 is looked up using ADL and in namespace std. This method does not participate in overload resolution if that expression is not well-formed in an unevaluated context.  The result type is the result type of that expression. 

#### Exceptions ####

All methods are noexcept if the expressions they are equivalent to are known not to throw any exceptions. 

### hashCode ###

```cpp
constexpr /*see below*/ hashCode()const
//Non-member
template<typename T> constexpr /*see below*/ hashcode(const Vec2<T>& v);
```

Computes the hashcode of the vector. This is the same as `hashcode(x)*31+hashcode(y)`. This method does not participate in overload resolution if those calls are not well formed. The result type of the method is the result type of that expression. 

### conversion to bool ###

```cpp
constexpr explicit operator bool()const;
```

Contextually converts the vector to bool. This is the same as (*this)!=Vec2{}. This method does not participate in overload resolution unless T is contextually convertible to bool. 


### Deduction Guides ###
```cpp
template<typename T> Vec2(T,T)->Vec2<T>; //(1)
template<typename Q,typename U> Vec2(Q,U)->Vec2<std::common_type_t<Q,U>>; //(2)
Vec2()->Vec2<int>; //(3)
```

(1): Basic deduction guide

(2): Allows you to deduce a Vector from 2 different types. This deduction guide only applies if `has_common_type_v<Q,U>` is true. 

(3): Default Deduction Guide. 

## class Matrix ##

Implements a 2x2 Matrix of some Numeric type. 

Matrix satisfies the requirements of *Numeric*. 
If `T` satisfies any of the following requirements, `Matrix<T>` also satisfies those requirements: 
* *LiteralType*
* *TriviallyCopyable*
* *StandardLayoutType*
* *BytesReadable*
* *BytesWriteable*

### Notes ###

The Same restrictions on `NaN` values for `Vec2` specializations apply to specializations of `Matrix`. 

### Type Requirements ###

`T` shall satisfy the requirements of *Numeric* or the program is ill-formed, no diagnostic required. 

Additionally, `T` may not be a specialization of `Vec2` or `Matrix` or the program is ill-formed. 


### Class Synopsis ###

```cpp
template<typename T> struct Matrix{
public:
	T a,b,c,d;
	constexpr Matrix();
	constexpr Matrix(const Matrix&);
	constexpr Matrix(const Vec2<T>&,const Vec2<T>&);
	template<typename Q,typename R,typename S,typename U> constexpr EXPLICIT Matrix(const std::pair<Q,R>&,const std::pair<S,U>&);
	template<typename U> constexpr EXPLICIT Matrix(const Matrix<U>&);
	constexpr friend Matrix</*see below*/> operator*(const Matrix&,const Matrix&);
	template<typename U> constexpr friend Matrix</*see below*/> operator*(const Matrix&,const Matrix<U>&);
	template<typename U> constexpr friend Matrix</*see below*/> operator*(const Matrix&,const U&);
	template<typename U> constexpr friend Matrix</*see below*/> operator*(const U&,const Matrix&);
	constexpr friend Matrix</*see below*/> operator*(const T&,const Matrix&);
	constexpr friend Matrix</*see below*/> operator*(const Matrix&,const T&);
	constexpr friend Matrix</*see below*/> operator/(const Matrix&,const T&);
	template<typename U> constexpr friend Matrix</*see below*/> operator/(const Matrix&,const U&);
	constexpr friend Matrix</*see below*/> operator+(const Matrix&,const Matrix&);
	template<typename U> constexpr friend Matrix</*see below*/> operator+(const Matrix&,const Matrix<U>&);
	constexpr friend Matrix</*see below*/> operator-(const Matrix&,const Matrix&);
	template<typename U> constexpr friend Matrix</*see below*/> operator-(const Matrix&,const Matrix<U>&);
	template<typename U> constexpr friend Vec2</*see below*/> operator*(const Matrix&,const Vec2<U>&);
	template<typename U> constexpr friend Vec2</*see below*/> operator*(const Vec2<U>&,const Matrix&);
	constexpr friend bool operator==(const Matrix&,const Matrix&);
	template<typename U> constexpr friend bool operator==(const Matrix&,const Matrix<U>&);
	constexpr friend bool operator!=(const Matrix&,const Matrix&);
	template<typename U> constexpr friend bool operator!=(const Matrix&,const Matrix<U>&);
};
```

### Fields ###

```cpp
T a;
T b;
T c;
T d;
```

A `Matrix` object with those variables set to a given value represents the 2x2 matrix of [[a,b],[c,d]]. 

### Constructors ###

```cpp
constexpr Matrix(); //(1)
constexpr Matrix(const Matrix&)=default; //(2)
template<typename U> constexpr EXPLICIT Matrix(const Matrix<U>&); //(3)
constexpr Matrix(const Vec2<T>& v1,const Vec2<T>& v2); //(4)
template<typename Q,typename R,typename S,typename U> constexpr EXPLICIT Matrix(const std::pair<Q,R>& p1,const std::pair<S,U>& p2); //(5)
```

(1): Default constructor. The default matrix is [[0,0],[0,0]]. Value initializes each of the members.

(2): Defaulted Copy Constructor

(3): Converting Copy constructor. This constructor only participates in overload resolution if `std::is_constructible_v<T,U>` is true. This constructor is explicit if and only if `std::is_convertible_v<Vec2<U>,Vec2<T>>` is true. 

(4): Constructor from 2 vectors. The resultant matrix is [[v1.x,v2.x],[v1.y,v2.y]]. 

(5): Constructor from a pair of generic values. The resultant matrix is [[p1.first,p2.first],[p1.second,p2.second]]. 
This constructor only participates in overload resolution if `std::is_constructible_v<Vec2<T>,Q,R>` and `std::is_constructible_v<Vec2<T>,S,U>`. This constructor is explicit if either of the constructors `T(Q,R)` and `T(S,U)` are explicit. 

### Arithmetic Operators ###

```cpp
constexpr friend Matrix</*see below*/> operator*(const Matrix& m1,const Matrix& m2); //(1)
template<typename U> constexpr friend Matrix</*see below*/> operator*(const Matrix& m1,const Matrix<U>& m2); //(2)
template<typename U> constexpr friend Matrix</*see below*/> operator*(const Matrix& m,const U& v); //(3)
template<typename U> constexpr friend Matrix</*see below*/> operator*(const U& v,const Matrix& m); //(3)
constexpr friend Matrix</*see below*/> operator*(const T& v,const Matrix& m); //(4)
constexpr friend Matrix</*see below*/> operator*(const Matrix& m,const T& v); //(4)
constexpr friend Matrix</*see below*/> operator/(const Matrix& m,const T& v); //(5)
template<typename U> constexpr friend Matrix</*see below*/> operator/(const Matrix& m,const U& v); //(6)
constexpr friend Matrix</*see below*/> operator+(const Matrix& m1,const Matrix& m2); //(7)
template<typename U> constexpr friend Matrix</*see below*/> operator+(const Matrix& m1,const Matrix<U>& m2); //(8)
constexpr friend Matrix</*see below*/> operator-(const Matrix& m1,const Matrix& m2); //(9)
template<typename U> constexpr friend Matrix</*see below*/> operator-(const Matrix& m1,const Matrix<U>& m2); //(10)
template<typename U> constexpr friend Vec2</*see below*/> operator*(const Matrix& m,const Vec2<U>& v); //(11)
template<typename U> constexpr friend Vec2</*see below*/> operator*(const Vec2<U>& v,const Matrix& m); //(11)
```

(1), (2): Performs Matrix Multiplication. The resultant matrix is `[[m1.a*m2.c,m1.b*m2.d],[m1.c*m2.a,m1.d*m2.b]]`. This operator only participates in overload resolution if `m1.a*m2.a` is well-formed in an unevaluated context. The result type is `Matrix<K>` where `K` is the type of the above expression (after applying decay conversions).

(3), (4): Performs scalar multiplication. The resultant matrix is `[[m.a*v,m.b*v],[m.c*v,m.d*v]]`. 
This operator only participates in overload resolution if `m1.a*v` is well-formed in an unevaluated context, and `U` is not a specialization of either `Vec2` or `Matrix`. The result type is `Matrix<K>` where `K` is the type of the above expression (after applying decay conversions). 

(5), (6): Performs scalar division. The resultant matrix is `[[m.a/v,m.b/v],[m.c/v,m.d/v]]`. This operator only participates in overload resolution if `m1.a/v` is well-formed in an unevaluated context. The result type is `Matrix<K>` where `K` is the type of the above expression (after applying decay conversions). 

(7), (8): Adds 2 matricies together. The resultant matrix is `[[m1.a+m2.a,m1.b+m2.b],[m1.c+m2.c,m1.d+m2.d]]`. This operator only participates in overload resolution if `m1.a+m2.a` is well-formed in an unevaluated context. The result type is `Matrix<K>` where `K` is the type of the above expression (after applying decay conversions). 

(9), (10): Subtracts one matrix from annother. The resultant matrix is `[[m1.a-m2.a,m1.b-m2.b],[m1.c-m2.c,m1.d-m2.d]]`. This operator only participates in overload resolution if `m1.a-m2.a` is well-formed in an unevaluated context. The result type is `Matrix<K>` where `K` is the type of the above expression (after applying decay conversions).

(11): Multiplies a matrix to a vector. The result is `Vec2{m.a*v.x,m.c*v.x}+Vec2{m.b*v.y,m.d*v.y}` This operator only participates in overload resolution if the above expression is well-formed in an unevaluated context. The result type is the result type of that expression, after applying decay conversions. The template arguments of `Vec2` are deduced as though by the Deduction Guides given above. 

## struct common_type&lt;Vec2&lt;T&gt;,Vec2&lt;U&gt;&gt; ##

Determines the common type between 2 vectors. The specialization has a member type `type` which names `Vec2<std::common_type_t<T,U>>` if and only if `has_common_type_v<T,U>` is true. Otherwise there is no member type. 

## struct common_type&lt;Matrix&lt;T&gt;,Vec2&lt;U&gt;&gt; ##

Determines the common type between 2 matricies. The specialization has a member type `type` which names `Matrix<std::common_type_t<T,U>>` if and only if `has_common_type_v<T,U>` is true. Otherwise there is no member type. 

## struct hash&lt;Vec2&lt;T&gt;&gt; ##

Computes the hash of a Vector.
The result is the result of hashcode. The specialization is disabled if `hashcode(v)` is not well-formed in an unevaulated context, given that v is an object of `Vec2<T>`.  

## ORIGIN ##

```cpp
template<typename T> constexpr const Vec2<T> ORIGIN{};
```

