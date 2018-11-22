<h1>Special Object Types</h1>
LCLib C++ Defines a set of special Object types, which have special privileges in LCLib.

<h2>Byte Types</h2>

Byte types are special case of scalar types which have sizeof(T)==1. Byte types are not invalidated when arrays of such are incompletely written to.<br/>
The C++ Standard only specifies 4 byte types, which are char, unsigned char, signed char, and std::byte (and does not officially name this Concept). LCLib C++ Extends this definition to encompass more types.<br/>
Byte types (and arrays of such) are considered layout compatible with all Standard Structure Types of the same size. (See below)<br/>
A Byte type must be one of the following:
<ul>
<li>char, unsigned char, or signed char</li>
<li>A enum type with one of the above as its underlying type, and no enumerators (including std::byte)</li>
</ul>
In addition: No comparison operators may be user supplied. 

<h2>BytesWriteable</h2>

BytesWriteable concept defines types which can have there raw byte representation written to a stream.
For a Type to satisfy BytesWriteable Concept, objects of that type, and all subobjects of such an object must:
<ul>
<li>Have all special member functions (Except Default Constructor) trivial or deleted</li>
<li>Have a trivial, non-deleted, and non-virtual destructor.</li>
<li>Have at least one of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-Assignment Operator non-deleted</li>
<li>Not have any virtual member functions</li>
<li>Not be a volatile object</li>
<li>Not be of a Pointer, Pointer to Member, or Pointer to Function type</li>
<li>Not have any Reference non-static members</li>
<li>May not have duplicate (direct or indirect) base classes, or a virtual base class type</li>
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
It should also be noted, that multibyte scalar types, while BytesWriteable, may not be suited for IO, due to varying Endianess of systems and implementation. 
It should be noted that despite fwrite/fread only specifying TriviallyCopyable, they should only be used with BytesWriteable types and BytesReadable types respectively.

<h2>BytesReadable</h2>

BytesReadable types are types which are suitable for having there byte representation read from a byte stream. 
For a type to satisfy BytesReadable, objects of that type, and all subobjects of such an object must:
<ul>
<li>Have all special member functions (Except Default Constructor) trivial or deleted</li>
<li>Have a trivial, non-deleted, and non-virtual destructor.</li>
<li>Have at least one of Copy-Constructor, Move-Constructor, Copy-Assignment Operator, or Move-Assignment Operator non-deleted</li>
<li>Not have any virtual member functions</li>
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


<h2>CharacterType</h2>

The CharacterType concept is defined for Types which use Strings or Characters. A CharacterType is any type T for which `is_char_v<T>` defined in `<lclib-cxx/TypeTraits.hpp>` is true. 
This trait has explicit specializations for `char`, `wchar_t`, `char16_t`, and `char32_t`. The trait may be specialized the user defined type `T`. 
If it is, then `std::char_traits` should be specialized for `T` given that the specialization satisfies `CharTraits`. 
In addition, type aliases for `std::basic_string<T>` and `std::basic_string_view<T>` should be provided. 
They may use a custom CharTraits type, and a custom allocator. It may also be prudent to alias `security::basic_string<T>` (defined in `<lclib-cxx/security/SecureAllocator.hpp>`), with the same rules. 

<h2>CString</h2>

The CString concept is defined for types which can model Null-terminated strings. Unlike other concepts, only specific types satisfy this concept. 

A type `T` satisfies CString if and only if it is one of the following: 
<ul>
<li>An array of a type which satisfies CharacterType</li>
<li>A pointer to such an array</li>
<li>A pointer to a type which satisfies CharacterType</li>
<li>A const-qualified, volatile-qualified, or const volatile-qualified variation of the above</li>
<li>A reference to any of the above</li>
</ul>




