# Special Object Types #
LCLib C++ Defines a set of Concepts, which have special privileges in LCLib. 

## Byte Types ##

Byte types are special case of scalar types which have sizeof(T)==1. Byte types are not invalidated when arrays of such are incompletely written to.<br/>
The C++ Standard only specifies 4 byte types, which are char, unsigned char, signed char, and std::byte (and does not officially name this Concept). LCLib C++ Extends this definition to encompass more types.<br/>
Byte types (and arrays of such) are considered layout compatible with all Standard Structure Types of the same size. (See below)<br/>
A Byte type must be one of the following:
<ul>
<li>char, unsigned char, or signed char</li>
<li>A enum type with one of the above as its underlying type, and no enumerators (including std::byte)</li>
</ul>
In addition: No comparison operators may be user supplied. In C++20 mode, the type shall have strong structural ordering. 

## BytesWriteable ##

BytesWriteable concept defines types which can have there raw byte representation written to a stream.
For a Type to satisfy BytesWriteable Concept, objects of that type, and all subobjects of such an object must:

* Have all of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-assignment operator trivial or deleted
* (As of 1.3) Have a trivial, constexpr, or deleted Default Constructor
* Have a trivial, non-deleted, and non-virtual destructor.
* Have at least one of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-Assignment Operator non-deleted
* May not declare or inherit any virtual member functions
* Not be a volatile object
* Not be of a Pointer, Pointer to Member, or Pointer to Function type
* Not have any Reference non-static data members
* May not have duplicate (direct or indirect) base classes, or a virtual base class type
* (As of 1.3) Have no private or protected data members or base classes, except for empty base classes.
* (As of 1.3) May not be *over-aligned*

Or shall be one of the follow:
* An array of a component type which satisfies *BytesWriteable*, including an array of an unknown bound
* 

All Scalar types except for pointers satisfy this concept. All class types that are TriviallyCopyable and StandardLayout that do not have any pointer members also satisfy this concept. 

Except as noted below, all base classes and non-static data members of a type which satisfies *BytesWriteable* shall also satisfy *BytesWriteable*. 

The decision to define a BytesWriteable concept was for Complete Target Independence. In most cases, TriviallyCopyable (and Potentially StandardLayout) is suitable for reading from the byte representation of a type. 
BytesWriteable types are suitable to be written to a file, and read into a Compatible BytesReadable type by similar implementations, which may not even be written in C++. (Hense why pointer types aren't BytesWriteable). 
It should also be noted, that multibyte scalar types, while satisfying BytesWriteable, may not be suited for IO, due to varying Endianess of systems and implementation. 
(Until 1.3) It should be noted that despite fwrite/fread only specifying TriviallyCopyable, they should only be used with BytesWriteable types and BytesReadable types respectively. 

No class types defined by the C++ Standard Template Library satisfy these requirements, due to implementation differences. 

No type in LCLib-C++ satisfy these requirements, unless explicitly stated. 
Note that some types in LCLib-C++ do not satisfy these requirements, but do not prevent types that inherit from them from satisfying these requirements.

*BytesWriteable* implies *TriviallyCopyable* and (as of 1.3) *StandardLayoutType*. 

## BytesReadable ##

BytesReadable types are types which are suitable for having there byte representation read from a byte stream. 

A type satisfies *BytesReadable* if it satisfies *BytesWriteable*, is not const-qualified, and has no const subobjects. 

## CharacterType ##

The CharacterType concept is defined for Types which use Strings or Characters. A CharacterType is any type T for which `is_char_v<T>` defined in `<lclib/TypeTraits.hpp>` is true. 
This trait has explicit specializations for `char`, `wchar_t`, `char16_t`, and `char32_t`. The trait may be specialized the user defined type `T`. 
Additionally if `__cpp_char8_t` is defined, and is at least 201811L, then this trait is explicitly specialized for `char8_t`. 

If it is, then `std::char_traits` must be specialized for `T` given that the specialization satisfies *CharTraits*. 



## CString ##

The CString concept is defined for types which can model Null-terminated strings. Unlike other concepts, only specific types satisfy this concept. 

A type `T` satisfies CString if and only if it is one of the following: 
<ul>
<li>An array of a type `U`, such that `U` satisfies CharacterType</li>
<li>A pointer to such an array</li>
<li>A pointer to a type `U` which satisfies CharacterType</li>
<li>A const-qualified, volatile-qualified, or const volatile-qualified variation of the above</li>
<li>A reference to any of the above</li>
</ul>

## Numeric ##

The Numeric concept defines types that can be used as an Arithmetic-type. 

Given:


* A type `T`, which satisfies *Numeric*
* `t`, `u`, `v` which are possibly const values of type `T`.
* `l`, an lvalue of type `T`
* `c`, a const lvalue of type `T`.
* `r`, an rvalue of type `T`
* `i`, Some value for which `t*i` is a well formed expression and the result is equal to `t`
* `z`, the null/empty/zero value for `T`, where `t+z==t` is true. 
* `a`, a value of any type in a potentially constrained set of types which satisfy *Numeric*. May be distinct values of distinct types depending on the expression. 
* `U`,`Q`, some other types that satisfies *Numeric* (may be `T`), which are as standins for a Generic type. `Q{t}` and `Q{a}` must both be well formed expressions, and may not result in narrowing conversions (that is, `Q` must be able to completely represent all possible values of `T` and of the type of a.) 

`T` must satisfy the following requirements (note that where Brace Construction is used, it either means Brace Construction or Parenthesis Construction, and which one it means in the context is unspecified):

<table>
	<tr>
		<th>Expression</th>
		<th>Type</th>
		<th>Semantics</th>
	</tr>
	<tr>
		<td>T{}</td>
		<td>T</td>
		<td>T{}==z shall be true</td>
	</tr>
	<tr>
		<td>T{0} (optional)</td>
		<td>T</td>
		<td>T{0}==z shall be true</td>
	</tr>
	<tr>
		<td>T{c}</td>
		<td>T</td>
		<td>T{c}==c shall be true</td>
	</tr>
	<tr>
		<td>T{r}</td>
		<td>T</td>
		<td>If r==t, then T{r}==t shall be true</td>
	</tr>
	<tr>
		<td>T{a} (optional)</td>
		<td>T</td>
		<td>If supported, T{a}==T{a} shall be true. However a==T{a} need not be true (if it is valid), as the conversion may be narrowing.</td>
	</tr>
	<tr>
		<td>l=t</td>
		<td>T&</td>
		<td>Assigns the value of t to l. If before the assignment u==t, then after the assignment, l==u must be true. Additionally, if before the assigment l==u is true, then after the assignment l!=u is true, unless u==t before the assignment.</td>
	</tr>
	<tr>
		<td>t+u</td>
		<td>U</td>
		<td>The result of the sum of t and u. t+T{}==t must be true. Additionally, u+t==t+u shall be true</td>
	</tr>
	<tr>
		<td>t-u</td>
		<td>U</td>
		<td>The difference between t and u. t-T{}==t must be true.</td>
	</tr>
	<tr>
		<td>-t (optional)</td>
		<td>U</td>
		<td>If supported, -t==T{}-t is true. Additionally, t-u==t+(-u) and u-t==-(t-u).</td>
	</tr>
	<tr>
		<td>t==u</td>
		<td>bool</td>
		<td>True if and only if t and u have the same value. t==t shall be true, and if t==u is true, then t==v shall be true if and only if u==v</td>
	</tr>
	<tr>
		<td>t==a (optional)</td>
		<td>bool</td>
		<td>If the value of a is representable as a value of T, same as T{a}==t, otherwise false</td>
	</tr>
	<tr>
		<td>t!=u</td>
		<td>bool</td>
		<td>!(t==u)</td>
	</tr>
	<tr>
		<td>t!=a (optional)</td>
		<td>bool</td>
		<td>!(t==a)</td>
	</tr>
	<tr>
		<td>bool{t} (optional)</td>
		<td>bool</td>
		<td>Same as t!=T{}</td>
	</tr>
</table>

Additionally, the following expressions may be optionally supported, but if they are supported, the semantics must be as defined


<table>
	<tr>
		<td>t+a</td>
		<td>U</td>
		<td>Equivalent to U{t}+U{a}</td>
	</tr>
	<tr>
		<td>t-a</td>
		<td>U</td>
		<td>Equivalent to U{t}-U{a}</td>
	</tr>
	<tr>
		<td>a-t</td>
		<td>U</td>
		<td>Equivalent to U{a}-U{t}</td>
	</tr>
	<tr>
		<td>t*u</td>
		<td>U</td>
		<td>The value of U that is the product of t and u. t*u==u*t must be true. Additionally, t*z==z must be true for all values t.</td>
	</tr>
	<tr>
		<td>t*a</td>
		<td>U</td>
		<td>Usually equivalent to Q{t}*Q{a}. t*a==a*t shall be true</td>
	</tr>
	<tr>
		<td>t/u</td>
		<td>U</td>
		<td>The value of U that is the quotient of t and u. If the result of the division is representable as a value of T, then T{(t/u)*u}==t is true. Otherwise it is unspecified if that equality holds. Additionally, t/t shall be a unique value that is representable as a value of T, and T{u*(t/t)}==u is true. If u==T{}, then the behavior is undefined.</td>
	</tr>
	<tr>
		<td>t/a</td>
		<td>U</td>
		<td>Divides t by a. See below for equivalences. </td>
	</tr>
	<tr>
		<td>a/t</td>
		<td>U</td>
		<td>Divides a by t. See below for equivalences. </td>
	</tr>
	<tr>
		<td>t&lt;u</td>
		<td>bool</td>
		<td>True if and only if the value of t is less than the value of u. One of t&lt;u, u&lt;t, or u==t must be true. Additionally only one of t&lt;u and u&lt;t may be true. If t&lt;u is true, and u&lt;v is true, then t&lt;v must be true.</td>
	</tr>
	<tr>
		<td>t&lt;a</td>
		<td>bool</td>
		<td>Equivalent to Q{t}&lt;Q{a}.</td>
	</tr>
	<tr>
		<td>a&lt;t</td>
		<td>bool</td>
		<td>Equivalent to Q{a}&lt;Q{t}.</td>
	</tr>
	<tr>
		<td>t&gt;u</td>
		<td>bool</td>
		<td>Equivalent to u&lt;t</td>
	</tr>
	<tr>
		<td>t&gt;a</td>
		<td>bool</td>
		<td>Equivalent to Q{a}&lt;U{t}</td>
	</tr>
	<tr>
		<td>a&gt;t</td>
		<td>bool</td>
		<td>Equivalent to U{t}&lt;U{a}</td>
	</tr>
	<tr>
		<td>t&lt;=u</td>
		<td>bool</td>
		<td>Equivalent to (t&lt;u)||(t==u)</td>
	</tr>
	<tr>
		<td>t&lt;=a</td>
		<td>bool</td>
		<td>Equivalent to U{t}&lt;
	<tr>
		<td>t&gt;=u</td>
		<td>bool</td>
		<td>Equivalent to (t&gt;u)||(t==u)</td>
	</tr>
</table>

The expression `t/a` is equivalent to at least one of the following, unless the semantics of the expression are explicitly defined:

* `t/T{a}`, if and only if `T` can represent the value of `a` (this is permitted even if `T` cannot represent ALL values of the type of `a`)
* `Q{t}/Q{a}`
* `Q{t}/a`
* `U{t}/a`, where `U` can represent the value of `t`. 
* `t/Q{a}`
* `t/U{a}`, where `U` can represent the value of `a`.
* `U{t}/U{a}`, where `U` can represent the values of both `t` and `a`. (This differs from `Q{t}/Q{a}`, as `Q` is required to be able to represent all values of `T` and of the type of `a`, whereas `U` is not explicitly required to, it only needs to be able to represent the value of `t` and `a`.)

The expression `a/t` is equivalent to at least one of the following, unless the semantics of the expression are explicitly defined:
* `T{a}/t`, if and only if `T` can represent the value of `a` (this is permitted even if `T` cannot represent ALL values of the type of `a`)
* `Q{a}/Q{t}`
* `Q{a}/t`
* `U{a}/t`, where `U` can represent the value of `a`
* `a/Q{t}`
* `a/U{t}`, where `U` can represent the value of `t`
* `U{a}/U{t}`, where `U` can represent the values of both `t` and `a`.
* `i/(t/a)`


Additionally, T may not be abstract and may not be `bool`. All of the operations defined above may not throw any exceptions. 
If T satisfies *LiteralType*, any expression that applys any of the above operators to one or more values of (possibly const-qualified) T and no other values, except the assignment operator, shall be a valid constant expression if all of its operands are a valid rvalue constant expression. 

If `operator<` is overloaded, then all relational operators for a type must be defined and form a strict, total-order with `operator<`. 
This order must be consistent and deterministic. If `T{}<u` is true, then `t+u<t` is true, unless the result of `t+u` cannot be represented fully as a value of `T` (however, it may still be true even when that is the case). 
Comparisons do not need to form a strong order (there can be distinct values of the type which compare equals). 

Note that for the purposes of determining what is an is not a value of the type, the special NaN values are not considered valid values. `NaN` values are therefore exempt from all the semantic rules defined above. This allows that x!=x is true when x is a `NaN` value. 

If `T` satisfies *Numeric*, then all of `const T`, `volatile T`, and `const volatile T` also satisfy *Numeric*. 

All arithmetic types satisfy these requirements. Additionally, due to integral promotion rules, `bool` would if it was not explicitly excluded by the requirements. 

Default Constructing (`T t;`) a value of a type that satisfies *Numeric* does not need to have well-defined results, provided that value-constructing `T{}` a value of such a type does. 
Default construction is optionally supported. If it is supported, then the result shall be the same as `T{}`. 
Otherwise, if the value of `t` is used by the program before `t` is assigned, the behavior is undefined (unless `T` also satisfies *Byte*, in which case the result is an indeterminate value, which may be a trap representation). 
If unsupported, variables of type `T` shall require an initializer if declared as part of a constant expression or as a const-qualified lvalue. 
The value of `t` is used whenever `t`:
* undergoes lvalue-to-rvalue decay
* is used as an operand of any of the operators defined here, except on the left-hand-side of an assignment operator
* is converted to any type, explicitly or implicitly, or is passed to the copy/move-constructor of `T`
* Is swapped with any other lvalue of `T`. 

## StrictNumeric ##

*StrictNumeric* is a concept that extends *Numeric*, and provides certain additional guarantees. 

Given:

* `T`, a type that satisfies *StrictNumeric*
* `t` and `u`, values of type (possibly const-qualified) `T`
* `c` and `d`, integral constant expressions which can be represented fully by `T`, except for the values 0 or 1. 
* `z`, the additive identity value of `T`, as described in the requirements for *Numeric* (Equals the default constructed instance and 
* `i`, the multiplicative identity value of `T`, as described in the requirements for *Numeric*
* `j`, a placeholder for any value of any numeric type.

`T` must satisfy *Numeric*, *TriviallyCopyable*, and *LiteralType*. All comparison operators defined by *Numeric* must also be supported, and form a strong order (that is, there may not be 2 distinct values of the same type that compare equal). `std::numeric_limits<T>` must be a specialization of `std::numeric_limits`. Additionally the following expressions must be supported:

<table>
	<tr>
		<th>Expression</th>
		<th>Result Type</th>
		<th>Semantics</th>
	</tr>
	<tr>
		<td>T{0}</td>
		<td>T</td>
		<td>T{0}==T{} and T{0}==z shall be true</td>
	</tr>
	<tr>
		<td>T{1}</td>
		<td>T</td>
		<td>T{1}==i shall be true</td>
	</tr>
	<tr>
		<td>T{c}</td>
		<td>T</td>
		<td>Results in the distinct value of T which represents c. Shall be an equality preserving function (that is T{c}&lt;T{d} is true if and only if c&lt;d is true)</td>
	</tr>
	<tr>
		<td>t&lt;=&gt;u (C++20 only)</td>
		<td>std::strong_ordering</td>
		<td>The result of the 3-way comparison shall be consistent with the relational operators</td>
	</tr>
	<tr>
		<td>bool{t}</td>
		<td>bool</td>
		<td>Same semantics as defined in Numeric, but the conversion is mandatory</td>
	</tr>
	<tr>
		<td>T{k}</td>
		<td>T</td>
		<td>If the conversion from the type of k to T is non-narrowing, and the type of k is a integral type that is not bool, then the result is the value of T that is the value of k. Otherwise it is optionally supported. If the specific value of k can be represented as a value of T, then the conversion shall be equality preserving. Otherwise, it is implementation-defined how k is converted to T. (k is an arbitrary value of some integral type other than bool)</td>
	</tr>
</table>

Additionally, it must be possible to implicitly convert `c` to a value of `T`. The result of all arithmetic operations involving only values of `T` shall be `T`. 

If `T{a}` (as specified by *Numeric*) is supported and is syntactically non-narrowing, that is `T` can completely represent all values of the type of `a`, named `K`, then this conversion can be done implicitly, and is required to be equality preserving in all cases (That is, `T{a}==T{a}` shall be true, as should `K{T{a}}==a`) and it shall be possible to perform this conversion implicity. 
Otherwise, the expression is only required to be equality preserving if the value of `a` can be represented as a value of `T`, and it is unspecified if this conversion can be done implicitly. 

The fact that the 3-way comparison operator must be consistent with the relational operator means all of the following expressions must be equivalent expressions (the result of each expression shall be equal and all observeable behavior of one shall be performed by the other):

* `t<u` and `(t<=>u) < 0`
* `t>u` and `(t<=>u) < 0`
* `t==u` and `(t<=>u) == 0`
* `t!=u` and `(t<=>u) != 0`
* `t<=u` and `(t<=>u) <= 0`
* `t>=u` and `(t<=>u) >= 0`

Additionally, all of `t<a`, `a<t`, `t>a`, `a>t`, `a==t`, `t==a`, `t!=a`, `a!=t`, `t<=a`, `a<=t`, `t>=a`, `a>=t`, `(t<=>a)` shall all be supported if `T{a}` is a well-formed expression (otherwise they are optionally supported). 
The comparison operators shall form a strong order if all values of the type of a can be represented as a value of `T`, otherwise it may form a week order, or even a partial order (for example, if `a` has a floating-point type that supports NaN values). 

Finally, the results of arithmetic operations should be consistent. That is, if `u` is positive, `t+u>t` should be true, unless the result of `t+u` cannot be represented as a value of type `t`. 

If the result of any of the following expressions is not representable as a value of `T`, the expression is optionally-supported and it is implementation-defined 

All integral types, except bool satisfy this constraint. 

If all operands to any of the operators defined by either *StrictNumeric* and *Numeric* are constant expressions, then the operator shall be a constant expression. 
