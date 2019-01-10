# Special Object Types #
LCLib C++ Defines a set of special Object types, which have special privileges in LCLib.

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
<ul>
<li>Have all of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-assignment operator trivial or deleted</li>
<li>(As of 1.3) Have a trivial, constexpr, or deleted Default Constructor</li>
<li>Have a trivial, non-deleted, and non-virtual destructor.</li>
<li>Have at least one of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-Assignment Operator non-deleted</li>
<li>(Until 1.3)Not have any virtual member functions. (As of 1.3) glvalues of the type of that object may not be Polymorphic</li>
<li>Not be a volatile object</li>
<li>Not be of a Pointer, Pointer to Member, or Pointer to Function type</li>
<li>Not have any Reference non-static data members</li>
<li>May not have duplicate (direct or indirect) base classes, or a virtual base class type</li>
<li>(As of 1.3) Have no private or protected data members or base classes</li>
</ul>

Or must be one of the following:
<ul>
<li>A union object with all variant members satisfying BytesWriteable</li>
<li>A std::variant with all variant members satisfying BytesWriteable</li>
<li>A std::tuple or std::pair with each tuple element satisfying BytesWriteable</li>
<li>A std::optional of a BytesWriteable Type.</li>
</ul>
All Scalar types except for pointers satisfy this concept. All class types that are TriviallyCopyable and StandardLayout that do not have any pointer members also satisfy this concept. All const qualified versions of types which satisfy BytesWriteable 
In general, BytesWriteable is a less strict version of Plain-old-data type. Because of this, Most POD Types also satisfy BytesWriteable. 
The differences between Plain-old-datatype and BytesWriteable are as follows: 
<ul>
<li>POD Types mandates Trivial Default Constructor, BytesWriteable does not</li>
<li>POD Types allow Pointer members and include Pointer types, BytesWriteable does not</li>
</ul>
The decision to define a BytesWriteable concept was for Complete Target Independence. In most cases, TriviallyCopyable (and Potentially StandardLayout) is suitable for reading from the byte representation of a type. 
BytesWriteable types are suitable to be written to a file, and read into a Compatible BytesReadable type by similar implementations, which may not even be written in C++. (Hense why pointer types aren't BytesWriteable). 
It should also be noted, that multibyte scalar types, while satisfying BytesWriteable, may not be suited for IO, due to varying Endianess of systems and implementation. 
(Until 1.3) It should be noted that despite fwrite/fread only specifying TriviallyCopyable, they should only be used with BytesWriteable types and BytesReadable types respectively. 

## BytesReadable ##

BytesReadable types are types which are suitable for having there byte representation read from a byte stream. 
For a type to satisfy BytesReadable, objects of that type, and all subobjects of such an object must:
<ul>
<li>(As of 1.3) Have a trivial, constexpr, or deleted Default Constructor</li>
<li>Have a trivial, non-deleted, and non-virtual destructor.</li>
<li>Have at least one of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-Assignment Operator non-deleted</li>
<li>(Until 1.3)Not have any virtual member functions. (As of 1.3) glvalues of the type of that object may not be Polymorphic</li>
<li>Not be a const object or a volatile object</li>
<li>Not be of a Pointer, Pointer to Member, or Pointer to Function type.</li>
<li>May not have any reference non-static members</li>
<li>May not have duplicate (direct or indirect) base classes, or a virtual base class type</li>
</ul>

Or must be one of the following:
<ul>
<li>A union object with all variant members satisfying BytesReadable</li>
<li>A std::variant with all variant members satisfying BytesReadable</li>
<li>A std::tuple or std::pair with each tuple element satisfying BytesReadable</li>
<li>A std::optional of a BytesReadable Type.</li>
</ul>
In general, BytesReadable  is a strict subset of BytesWriteable. 
Specifically any type which satisfies BytesReadble satisfies BytesWriteable, and any type which satisfies BytesWriteable, is not const-qualified, and has no const subobjects also satisfies BytesReadable. 


## CharacterType ##

The CharacterType concept is defined for Types which use Strings or Characters. A CharacterType is any type T for which `is_char_v<T>` defined in `<lclib/TypeTraits.hpp>` is true. 
This trait has explicit specializations for `char`, `wchar_t`, `char16_t`, and `char32_t`. The trait may be specialized the user defined type `T`. Additionally, in C++20 mode, if `__cpp_char8_t` is defined, and is at least 201811L, then this trait is explicitly specialized for `char8_t`. 
If it is, then `std::char_traits` must be specialized for `T` given that the specialization satisfies `CharTraits`. 
In addition, type aliases for `std::basic_string<T>` and `std::basic_string_view<T>` should be provided. 
They may use a custom CharTraits type, and a custom allocator. It may also be prudent to alias `security::basic_string<T>` (defined in `<lclib/security/SecureAllocator.hpp>`), with the same rules. 

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
* `U`,`Q`, some other type that satisfies *Numeric* (may be `T`), which are as standins for a Generic type. `Q{t}` and `Q{a}` must both be well formed expressions, and may not result in narrowing conversions (that is, `Q` must be able to completely represent all possible values of `T` and of the type of a.) 

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

If `operator<` is overloaded, then all relational operators for a type must be defined and form a strict, total-order with `operator<`. This order must be consistent and deterministic. If `T{}<u` is true, then `t+u<t` is true, unless the result of `t+u` cannot be represented fully as a value of `T` (however, it may still be true even when that is the case). 

Note that for the purposes of determining what is an is not a value of the type, the special NaN values are not considered valid values. `NaN` values are therefore exempt from all the semantic rules defined above. This allows that x!=x is true when x is a `NaN` value. 

If `T` satisfies *Numeric*, then all of `const T`, `volatile T`, and `const volatile T` also satisfy *Numeric*.