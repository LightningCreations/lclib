<h1>Type Traits Library</h1>

Included from `<lclib-cxx/TypeTraits.hpp>`. 
Provides several custom type traits, to extend the ones from `<type_traits>`.<br/> 
Note that the rules of standard type traits apply. 
Specializing a trait for any type, is undefined behavior, unless that trait states that it can be specialized. 

Includes: `<type_traits>`
<h2>Synopsis</h2>

```cpp
#include <type_traits>


template<typename T> struct type_identity;
template<typename T> struct remove_cvref;
template<typename T,typename U> struct mirror_const;
template<typename T,typename U> struct mirror_volatile;
template<typename T,typename U> struct mirror_cv;
template<typename T> struct remove_cvptr;

template<typename T> struct is_complete;
template<typename T,typename U> struct can_contain;
template<typename T> struct is_byte;
template<typename T> struct is_char;
template<typename T> struct is_cstring;

template<typename T,typename... Ts> struct require_types;

template<template<typename> class Trait,typename... Ts> struct all_match;
template<template<typename> class Trait,typename... Ts> struct any_match;
template<template<typename> class Trait,typename... Ts> struct none_match;

template<typename T> using type_identity_t = typename type_identity<T>::type;
template<typename T> using remove_cvref_t = typename remove_cvref<T>::type;
template<typename T> using remove_cvptr_t = typename remove_cvptr<T>::type;
template<typename T,typename U> using mirror_const_t = typename mirror_const<T,U>::type;
template<typename T,typename U> using mirror_volatile_t = typename mirror_volatile<T,U>::type;
template<typename T,typename U> using mirror_cv_t = typename mirror_cv<T,U>::type;
template<typename T,typename... Ts> using require_types_t = typename require_types<T,Ts...>;
template<typename T> constexpr bool is_complete_v = is_complete<T>::value;
template<typename T,typename U> constexpr bool can_contain_v = can_contain<T,U>::value;
template<typename T> constexpr bool is_byte_v = is_byte<T>::value;
template<typename T> constexpr bool is_char_v = is_char<T>::value;
template<typename T> constexpr bool is_cstring_v = is_cstring<T>::value;
template<template<typename> class Trait,typename... Ts> constexpr bool all_match_v = all_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> constexpr bool any_match_v = any_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> constexpr bool none_match_v = none_match<Trait,Ts...>::value;
```

<h2>Type Transformers</h2>
<h3>struct type_identity</h3>

Contains a member typedef type, which aliases T. Useful for blocking type deduction.

<h4>type_identity_t</h4>

Alias for `typename type_identity<T>::type`. 

<h4>Notes</h4>

In `C++20` this is an official type trait (declared as `std::type_identity`). Both definitions are available if the official definition is supported.

<h3>struct remove_cvref</h3>

If T is a reference type U& or U&&, contains a member typedef type which is equal to U with top-level CV qualifiers removed. 
Otherwise, contains a member typedef type which is equal to T with top-level CV qualifiers removed. 

<h4>remove_cvref_t</h4>

Alias for `typename remove_cvref<T>::type`.

<h4>Notes</h4>

In `C++20` this is an official type trait (declared as `std::remove_cvref`). Both definitions are available if the official definition is supported.

<h3>struct remove_cvptr</h3>

If T is a pointer type `U*`, `U* const`, `U* volatile`, or `U* const volatile`, contains a member typedef type which is equal to U with top-level CV qualifiers removed (even if U is itself a pointer type). 
Otherwise, contains a member typedef type which is equal to T with top-level CV qualifiers removed.

<h4>remove_cvptr_t</h4>

Alias for `typename remove_cvptr<T>::type`

<h3>struct mirror_const</h3>

If `T` is a const-qualified type, or a const volatile-qualified type, then contains a member typedef type which is equal to `const U`.
Otherwise, contains a member typedef type which is equal to U with its top-level const qualification (if any) removed. 

<h4>mirror_const_t</h4>

Alias for `typename mirror_const<T>::type`.

<h3>struct mirror_volatile</h3>

If `T` is a volatile-qualified type, or a const volatile-qualified type, then contains a member typedef type which is equal to `volatile U`. 
Otherwise, contains a member typedef type which is equal to U with its top-level volatile qualification (if any) removed.

<h4>mirror_volatile_t</h4>

Alias for `typename mirror_volatile<T>::type`

<h3>struct mirror_cv</h3>

If `T` is a const volatile-qualified type, then contains a member typedef type which is `const volatile U`.
Otherwise, if `T` is a const-qualified type, then contains a member typedef type which is `const S` where S is U with its top-level volatile qualification removed(if it exists). 
Otherwise, if `T` is a volatile-qualified type, then contains a member typedef type which is `volatile S` where S is U with its top-level const qualification removed(if it exists). 
Otherwise, contains a member typedef type which is `U` with its top-level cv-qualifiers removed. 

<h4>mirror_cv_t</h4>

Alias for `typename mirror_cv<T>::type`


<h3>struct require_types</h3>

Contains a member typedef type which is `T`. This can be used to assist with SFINAE overloads, in a similar manner to `std::void_t`, as it requires that all other types passed are valid types.  

<h4>require_types_t</h4> 

Alias for `typename require_types<T,Ts...>::type`.

<h2>Type Properties</h2>

<h3>struct is_complete</h3>

If `T` is a complete object type, then inherits from `std::true_type`. Otherwise, inherits from `std::false_type`. 
Note that this will never be true for function types, or reference types, and will always be true for pointer types, even if the pointed to type is incomplete. 

<h4>constexpr bool is_complete_v</h4>

Equals `is_complete<T>::value`

<h3>struct can_contain</h3>

If `U` can contain a `T` in its object representation, then inherits from `std::true_type`. Otherwise inherits from `std::false_type`. 
A Type `U` can contain a `T` if any of the following is true:
<ul>
<li>`U` is `T`, or a more or less cv-qualified version of `T` or a type which can contain `T`</li>
<li>`U` is an array with a known, non-zero bound of `T` or a type which can contain `T`</li>
<li>`U` is an instantiation of std::array, with a non-zero bound, and the type is `T` or a type which can contain `T`</li>
<li>`U` is a std::variant and at least one variant member is `T` or a type which can contain `T`</li>
<li>The trait `std::tuple_size_v<U>` is valid, a non-zero number, and at least one `std::tuple_element_t<U,I>` is `T` or a type which can contain `T`, where `I` is a `std::size_t` in the range `(0,std::tuple_size_v<U>]`. </li>
<li>`U` is a std::optional of `T` or a type which can contain a `T`</li>
</ul>

`can_contain` may be specialized on User-defined types, however this is not required if `std::tuple_size` and `std::tuple_element` are specialized, as it will have a partial specialization provided for these "tuple-like types". 
The behavior is undefined if `can_contain` is specialized for `U`, unless that type satisfies one of the following requirements: 
<ul>
<li>`U` is a union type, and `T` or some type which can contain a `T` is a variant member of `U`</li>
<li>`U` is a structure type, and `T` or some type which can contain a `T` is a member of the type (not a base class, or the member of a base class)</li>
</ul>

<h4>constexpr bool can_contain_v</h4>

Equal to `can_contain<T,U>::value`.

<h3>struct is_char</h3>

If `T` satisfies CharacterType, inherits from `std::true_type`. Otherwise inherits from `std::false_type`. 
This trait can be specialized for user defined types, as specified in the definition of CharacterType in SpecialObjectTypes.md. The behavior of a program which specializes `is_char` in violation of these rules is undefined. 
The trait is specialized for `char`, `wchar_t`, `char16_t`, and `char32_t` as well as cv-qualified variations of all types for which `is_char` is specialized. 

<h4>constexpr bool is_char_v</h4>

Equal to `is_char<T>::value`.

<h3>struct is_byte</h3>

If `T` satisfies Byte, inherits from `std::true_type`. Otherwise inherits from `std::false_type`. 
This trait can be specialized for user defined types if they would otherwise satisfy the Byte concept defined in SpecialObjectTypes.md. The behavior of a program which specializes `is_byte` in violation of these rules is undefined. 
The trait is specialized for `char`, `unsigned char`, `signed char`, and `std::byte`, as well as const-qualified variations (but non volatile-qualified variations) of all types for which `is_byte` is specialized. 

<h4>constexpr bool is_byte_v</h3>

Equal to `is_byte<T>::value`.

<h3>struct is_cstring</h3>

If `T` Satisfies CString, inherits from `std::true_type`. Otherwise inherits from `std::false_type`.  

<h4>constexpr bool is_cstring_v</h4>

Equal to `is_cstring<T>::value`.

<h3>struct all_match</h3>

Checks if all types in `Ts...` match `Trait` (short circuiting). 
If any type `T` in `Ts...` except the last does not match `Trait`, no further instantiations are made, and this inherits from `Trait<T>`. 
Otherwise given the last type `T` in `Ts...` inherits from `Trait<T>`. 
`Trait` must be a Unary Property TypeTrait. 


<h4>constexpr bool all_match_v</h4>

Equal to `all_match<Trait,Ts...>::value`.

<h3>struct any_match</h3>

Checks if any type in `Ts...` matches `Trait` (short circuiting). 
If any type `T` in `Ts...` except the last matches `Trait`, no further instantiations are made, and this inherits from `Trait<T>`. 
Otherwise given the last type `T` in `Ts...` inherits from `Trait<T>`. 
`Trait` must be a Unary Property TypeTrait. 

<h4>constexpr bool any_match_v</h4>

Equal to `any_match<Trait,Ts...>::value`

<h3>struct none_match</h3>

Checks if any type in `Ts...` matches `Trait` (short circuiting). 
Inherits from `std::negation<any_match<Trait,Ts...>>`. 
`Trait` must be a Unary Property TypeTrait. 

<h4>constexpr bool none_match_v</h4>

Equal to `none_match<Trait,Ts...>::value`
