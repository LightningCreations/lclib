# Type Traits Library #

Included from `<lclib/TypeTraits.hpp>`. 
Provides several custom type traits, to extend the ones from `<type_traits>`.<br/> 

## General Rules ##

All rules that apply to standard type traits apply here. 
That is, if a program instantiates a type trait (that does not explicitly state otherwise) with an incomplete type that is not (possibly cv-qualified) void, or an array of an unknown bound, the behavior is undefined. 
Additionally, if a program specializes any type trait (that does not explicitly state otherwise), the behavior is undefined. For such traits they may only be specialized by a program if at least one of the type paramaters is or depends on a user-defined type. 


Includes: `<type_traits>`
## Synopsis ##

```cpp
#include <type_traits>


template<typename T> struct type_identity;
template<typename T> struct remove_cvref;
template<typename T,typename U> struct mirror_const;
template<typename T,typename U> struct mirror_volatile;
template<typename T,typename U> struct mirror_cv;
template<typename T,typename U> struct copy_category;


template<typename T> struct is_complete;
template<typename T,typename U> struct can_contain;
template<typename T> struct is_byte;
template<typename T> struct is_char;
template<typename T> struct is_cstring;
template<typename... Ts> struct has_common_type;

template<typename T,typename... Ts> struct require_types;

template<template<typename> class Trait,typename... Ts> struct all_match;
template<template<typename> class Trait,typename... Ts> struct any_match;
template<template<typename> class Trait,typename... Ts> struct none_match;


template<typename T> using type_identity_t = typename type_identity<T>::type;
template<typename T> using remove_cvref_t = typename remove_cvref<T>::type;
template<typename T,typename U> using mirror_const_t = typename mirror_const<T,U>::type;
template<typename T,typename U> using mirror_volatile_t = typename mirror_volatile<T,U>::type;
template<typename T,typename U> using mirror_cv_t = typename mirror_cv<T,U>::type;
template<typename T,typename... Ts> using require_types_t = typename require_types<T,Ts...>::type;
template<typename T,typename U> using copy_category_t = typename copy_category<T,U>::type;
template<typename T> constexpr bool is_complete_v = is_complete<T>::value;
template<typename T,typename U> constexpr bool can_contain_v = can_contain<T,U>::value;
template<typename T> constexpr bool is_byte_v = is_byte<T>::value;
template<typename T> constexpr bool is_char_v = is_char<T>::value;
template<typename T> constexpr bool is_cstring_v = is_cstring<T>::value;
template<typename... Ts> constexpr bool has_common_type_v = has_common_type<Ts...>::value;
template<template<typename> class Trait,typename... Ts> constexpr bool all_match_v = all_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> constexpr bool any_match_v = any_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> constexpr bool none_match_v = none_match<Trait,Ts...>::value;

```

## Type Transformers ##
### struct `type_identity` ###

Contains a member typedef type, which aliases T. 
Useful for blocking type deduction.

#### `type_identity_t` ####

Alias for `typename type_identity<T>::type`. 

#### Notes ####

In `C++20` this is an official type trait (declared as `std::type_identity`). Both definitions are available if the official definition is supported. 

### struct `remove_cvref` ###

If T is a reference type U& or U&&, contains a member typedef type which is equal to U with top-level CV qualifiers removed. 
Otherwise, contains a member typedef type which is equal to T with top-level CV qualifiers removed. 

#### `remove_cvref_t` ####

Alias for `typename remove_cvref<T>::type`.

#### Notes ####

In `C++20` this is an official type trait (declared as `std::remove_cvref`). Both definitions are available if the official definition is supported.

### struct `mirror_const` ###

If `T` is a const-qualified type, or a const volatile-qualified type, then contains a member typedef type which is equal to `const U`.
Otherwise, contains a member typedef type which is equal to U with its top-level const qualification (if any) removed. 

#### `mirror_const_t` ####

Alias for `typename mirror_const<T>::type`.

### struct `mirror_volatile` ###

If `T` is a volatile-qualified type, or a const volatile-qualified type, then contains a member typedef type which is equal to `volatile U`. 
Otherwise, contains a member typedef type which is equal to U with its top-level volatile qualification (if any) removed.

#### `mirror_volatile_t` ####

Alias for `typename mirror_volatile<T>::type`

### struct `mirror_cv` ###

If `T` is a const volatile-qualified type, then contains a member typedef type which is `const volatile U`.
Otherwise, if `T` is a const-qualified type, then contains a member typedef type which is `const S` where S is U with its top-level volatile qualification removed(if it exists). 
Otherwise, if `T` is a volatile-qualified type, then contains a member typedef type which is `volatile S` where S is U with its top-level const qualification removed(if it exists). 
Otherwise, contains a member typedef type which is `U` with its top-level cv-qualifiers removed. 

#### `mirror_cv_t` ####

Alias for `typename mirror_cv<T>::type`


### struct `require_types` ###

Contains a member typedef type which is `T`. This can be used to assist with SFINAE overloads, in a similar manner to `std::void_t`, as it requires that all other types passed are valid types.  

#### `require_types_t` ####

Alias for `typename require_types<T,Ts...>::type`.

## Type Properties ##

### struct `is_complete` ###

If `T` is a complete object type, then inherits from `std::true_type`. Otherwise, inherits from `std::false_type`. 
Note that this will never be true for function types, or reference types, and will always be true for pointer types, even if the pointed to type is incomplete. 

If `T` is a class or union type or a cv-qualified variant thereof and `T` is completed in a translation unit where `is_complete<T>` or `is_complete_v<T>` is odr-used, then all odr-uses of `is_complete<T>` or `is_complete_v<T>` in the translation unit shall occur before `T` is completed or after `T` is completed or the behavior is undefined. If `is_complete<T>` or `is_complete_v<T>` is odr-used in the definition of `T`, the behavior is undefined. 

### constexpr bool `is_complete_v` ####

Equals `is_complete<T>::value`

### struct `can_contain` ###

If `U` reports that objects of its type can contain an object of type `T`, then inherits from `std::true_type`, otherwise inherits from `std::false_type`.

Both `T` and `U` shall be complete types, (possibly) cv-qualified void, or an array of unknown bound, or the behavior is undefined. 

If `T` or `U` is a reference type, a function type, or (possibly) cv-qualified void, the program is ill-formed. 

An object of type `U` "can contain" an object of type `U` if at least one of the following is true:
1. `T` and `U` are (possibly cv-qualified) variants of the same type.
2. `T` is a cv-qualified variant of some type `V`, and an object of type `U` can contain an object of type `V`
3. `U` is a cv-qualified variant of some type `V`, and an object of type `V` can contain an object of type `T`.
4. If `U` is an array of some type `V` (with a known or unknown bound), and an object of type `V` can contain an object of type `T`.
5. If `U` is a specialization of `std::optional` for some type `V`, and an object of type `V` can contain an object of type `U`.
6. If `U` is a specialization of `std::variant` for some list of types `Vs...` which contains at least once some type `V` for which objects of type `V` can contain an object of type `T`.
7. If `std::tuple_size<U>` extends `std::integral_constant<std::size_t,N>`, and if `Vs...` is the expansion of `std::tuple_element<U,Is...>` where `Is...` is `0,1,2,...,N`, and `Vs...` contains at least once, some type `V` for which objects of type `V` can contain an object of type `T`. 
8. `U` is a non-union class type which has some non-static data member of some type `V`, for which objects of type `V` can contain an object of type `T`. 
9. `U` is a union type, and at least one of its variant members has some type `V`, for which objects of type `V` can contain an object of type `T`.

A type `U` reports that objects of its type can contain an object of type `T`, if it satisfies at least one of the requirements from (1-7), except that for the purposes of those requirements, only types that report that they "can contain" a `T` are considered to be able to; or `can_contain` is specialized for that type.

The parameter `K` is provided solely for the purposes of specializing `can_contain`. If `can_contain` is instantiated with this parameter, that parameter must be `void` exactly or the program is ill-formed. 

A program may specialize `can_contain` for some types `T` and `U`, provided that `std::decay_t<T>` and `std::decay_t<U>` is the identity transformation, and at least one of `T` and `U` is a user-defined type, or is a user-provided type that is a full, user-provided specialization of a template. Additionally, `U` must be specified such that,   If these rules (and the general rules for specializing types) are not met, the behavior is undefined. 

If a program specializes `can_contain` for its `K` parameter, `K` must be `void` or the behavior is undefined. 


#### constexpr bool `can_contain_v` ####

Equal to `can_contain<T,U>::value`.

### struct `is_char` ###

If `T` satisfies CharacterType, inherits from `std::true_type`. Otherwise inherits from `std::false_type`. 
This trait can be specialized for a user defined type `T`, if `T` satisfies `TriviallyCopyable`, `StandardLayout`, `LiteralType`, and if `std::char_traits` is specialized for `T` under the rules defined for `std::char_traits`. 
The trait is specialized for `char`, `wchar_t`, `char16_t`, and `char32_t` as well as cv-qualified variations of all types for which `is_char` is specialized. 

#### constexpr bool `is_char_v` ####

Equal to `is_char<T>::value`.

### struct `is_byte` ###

If `T` satisfies Byte, inherits from `std::true_type`. Otherwise inherits from `std::false_type`. 
This trait can be specialized for a user defined type `T`. If it is, then `T` must be an enumeration type with a fixed underlying type Q for which `std::is_byte_v<Q>` is true or the behavior is undefined. 

#### constexpr bool `is_byte_v` ####

Equal to `is_byte<T>::value`.

### struct `is_cstring` ###

If `T` Satisfies CString, inherits from `std::true_type`. Otherwise inherits from `std::false_type`.  

#### constexpr bool `is_cstring_v` ####

Equal to `is_cstring<T>::value`.

### struct `has_common_type` ###

Checks if a set of types share a common type under a set of rules: 

* If sizeof...(Ts) is 0, inherits from `std::false_type`. 
* Otherwise, if for each type `Ti` and `Tj` in `Ts...`, `std::is_same<Ti,Tj>` is true, inherits from `std::true_type`. (This includes when sizeof...(Ts) is 1)  
* Otherwise, if sizeof...(Ts) is 2, and `Ts...` expands to `T,U`, inherits from `std::true_type` if all of the following requirements are met (and inherits from `std::false_type` otherwise):
    * `std::common_type_t<T,U>` and `std::common_type_t<U,T>` must exist and be the same type  
    * Both T and U must be convertible to `std::common_type_t<T,U>`
    * The common type between T and `std::common_type_t<T,U>` must be the same as `std::common_type_t<T,U>`. The same must apply for U.
* Otherwise, given that `Ts...` expands to `T,U,Rest...`
     * `std::common_type_t<T,U,Rest...> must exist and name a type
     * T and U must share a common type (as above)
     * All types in Rest... must share a common type
     * T must share a common type with each type in Rest... and with `std::common_type_t<Rest...>`
     * U must share a common type with each type in Rest... and with `std::common_type_t<Rest...>`
     * `std::common_type_t<T,U>` must share a common type with each type in Rest... and with `std::common_type_t<Rest...>`
     * The common type between T and `std::common_type<T,U,Rest...>` must be `std::common_type_t<T,U,Rest...>. The same applies for U and each type K in Rest....

#### constexpr bool `has_common_type_v` ####

Same as `has_common_type<T>::value`

### struct `all_match` ###

Checks if all types in `Ts...` match `Trait` (short circuiting). 
If any type `Ti` in `Ts...` except the last does not match `Trait`, no further instantiations are made, and this inherits from `Trait<Ti>`. 
Otherwise given the last type `T` in `Ts...` inherits from `Trait<T>`. 
`Trait` must satisfy UnaryTypeTrait with a BaseTrait that is contextually convertible to `bool` in a constant expression or the program is ill-formed.  


#### constexpr bool `all_match_v` ####

Equal to `all_match<Trait,Ts...>::value`.

### struct `any_match` ###

Checks if any type in `Ts...` matches `Trait` (short circuiting). 
If any type `Ti` in `Ts...` except the last matches `Trait`, no further instantiations are made, and this inherits from `Trait<T>`. 
Otherwise given the last type `T` in `Ts...` inherits from `Trait<T>`. 
`Trait` must satisfy UnaryTypeTrait with a BaseTrait that is contextually convertible to `bool` in a constant expression or the program is ill-formed. 

#### constexpr bool `any_match_v` ####

Equal to `any_match<Trait,Ts...>::value`

### struct `none_match` ###

Checks if any type in `Ts...` matches `Trait` (short circuiting). 
Inherits from `std::negation<any_match<Trait,Ts...>>`. 
`Trait` must be a Unary Property TypeTrait or the program is ill-formed.

#### constexpr bool `none_match_v` ####

Equal to `none_match<Trait,Ts...>::value`
