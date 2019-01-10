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
* `t` and `u`, a possibly const value of type `T`.
* `l`, an lvalue of type `T`
* `c`, a const lvalue of type `T`.
* `r`, an rvalue of type `T`
* `a`, a value of some type which satisfies *Numeric*. May be distinct values of distinct types depending on the expression. 
* `U`, some other type that satisfies *Numeric* (may be `T`). May be different depending the expression

`T` must either be an arithmetic type or satisfy the following requirements:

<table>
	<tr>
		<th>Expression</th>
		<th>Type</th>
		<th>Semantics</th>
	</tr>
	<tr>
		<td>T{}</td>
		<td>T</td>
		<td>T{} shall result in a value that is the equivalent of the value 0</td>
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
		<td>If supported, T{a}==T{a} shall be true. However a==T{a} need not be true (if it is valid), as the conversion may be narrowing</td>
	</tr>
	<tr>
		<td>t+u</td>
		<td>T</td>
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
		<td>True if and only if t and u have the same value</td>
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

Additionally, the following expressions may be optionally supported, but if they are supported, the semantics must be supported. In this context, all `U` shall always be able to fully represent all values of `T`. Further, when expressions include `a`, `U` shall also be able to fully represent all values of `a`. 


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
		<td>The value of U that is the product of t and u. t*u==u*t must be true</td>
	</tr>
	<tr>
		<td>t*a</td>
		<td>U</td>
		<td>Equivalent to Q{t}*Q{a}, where Q satisfies <i>Numeric</i>, is constructible from both T and decltype(a), and the result type is convertible to U. Also all values of both T and decltype(a) shall be representable as values of Q. t*a==a*t shall be true</td>
	</tr>
	<tr>
		<td>t/u</td>
		<td>U</td>
		<td>The value of U that is the quotient of t and u. If the result of the division is representable as a value of T, then T{(t/u)*u}==t is true. Otherwise it is unspecified if that equality holds. Additionally, t/t shall be a unique value that is representable as a value of T, and T{u*(t/t)}==u is true.</td>
	</tr>
	<tr>
		<td>t/a</td>
		<td>U</td>
		<td>Equivalent to Q{t}/Q{a}, where Q is defined the same way as it is for t*a</td>
	</tr>
	<tr>
		<td>a/t</td>
		<td>U</td>
		<td>Equivalent to Q{a}/Q{t}, where Q is defined the same way as it is for t*a and t/a</td>
	</tr>
	<tr>
		<td>t&lt;u</td>
		<td>bool</td>
		<td>True if and only if the value of t is less than the value of u. If t!=u, then one of t&lt;u and u&lt;t must be true. Additionally only one of t&lt;u and u&lt;t may be true</td>
	</tr>
	<tr>
		<td>t&gt;u</td>
		<td>bool</td>
		<td>Equivalent to u&lt;t</td>
	</tr>
	<tr>
		<td>t&lt;=u</td>
		<td>bool</td>
		<td>Equivalent to (t&lt;u)||(t==u)</td>
	</tr>
	<tr>
		<td>t&gt;=u</td>
		<td>bool</td>
		<td>Equivalent to (t&gt;u)||(t==u)</td>
	</tr>
</table>


