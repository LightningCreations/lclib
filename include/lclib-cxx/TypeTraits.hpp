#ifndef __TypeTraits_hpp_2018_08_22_16_29
#define __TypeTraits_hpp_2018_08_22_16_29
#include <type_traits>
#include <tuple>
#include <variant>
#include <optional>
#include <cstddef>



template <typename T,typename=void>
struct is_complete : std::false_type  {};
template<typename T> struct is_complete<T,std::enable_if_t<sizeof(T)==sizeof(T)>>:std::true_type{};

template<typename T> const constexpr bool is_complete_v = is_complete<T>::value;

template<typename T> struct type_identity{
	typedef T type;
};
template<typename T> using type_identity_t = typename type_identity<T>::type;
template<typename T> struct remove_cvref{
	using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template<typename T> using remove_cvref_t = typename remove_cvref<T>::type;

/**
 * Declares a member typedef type which is U with the const qualification of T mirrored.
 * If T is const qualified the type member is std::add_const_t<U>, otherwise its std::remove_const_t<U>
 */
template<typename T,typename U> struct mirror_const{
	using type = std::conditional_t<std::is_const_v<T>,std::add_const_t<U>,std::remove_const_t<U>>;
};
template<typename T,typename U> using mirror_const_t = typename mirror_const<T,U>::type;
/**
 * Declares a member typedef type which is U with the volatile qualification of T mirrored.
 * If T is volatile then the type member is std::add_volatile_t<U>, otherwise its std::remove_voatile_t<U>
 */
template<typename T,typename U> struct mirror_volatile{
	using type = std::conditional_t<std::is_volatile_v<T>,std::add_volatile_t<U>,std::remove_volatile_t<U>>;
};
template<typename T,typename U> using mirror_volatile_t = typename mirror_volatile<T,U>::type;

/**
 * Mirrors all cv-qualifiers from T onto U.
 * If T is neither const qualified nor volatile qualified, the member typedef is equivalent to std::remove_cv_t<U>
 * If T is const qualified but not volatile qualified, the member typedef is equivalent to
 * std::add_const_t<std::remove_volatile_t<U>>,
 * If T is volatile qualified but not const the member typedef is equivalent to std::add_volatile_t<std::remove_const_t<U>>
 * If T is const and volatile qualified, then the member typedef is equivalent to std::add_cv_t<U>
 */
template<typename T,typename U> struct mirror_cv{
	using type = mirror_const_t<T,mirror_volatile_t<T,U>>;
};
template<typename T,typename U> using mirror_cv_t = typename mirror_cv<T,U>::type;

/**
 * Designates that an Object of U may contain an object of T as a subobject
 * By default:
 * If U is (possibly more or less cv-qualified) T, then can_contain inherits from true_type
 * Else if U is an optional of (possibly more or less cv-qualified) T, then can_contain inherits from true_type
 * Else if U is an optional of V, and V can contain a T, then can_contain inherits from true_type
 * Else if U is a tuple or variant of Ts... and at least one of Ts... is either (possibly more or less cv-qualified) T,
 * or some type V, for which V can contain a T, then can_contain inherits from true type.
 * Else if U is a pair of V and W, and either V or W is (possibly more or less cv-qualified) T,
 * or either V or W can contain a T, then can_contain inherits from true_type.
 * Else if U is an array (c-style or std::array) (of a fixed bound) or either (possibly more or less cv-qualified) T,
 * or some V, such that V can contain a T, then can_contain inherits from true_type.
 * Note that Arrays of Unknown bound do not contain a T, nor do references to arrays.
 * Otherwise can_contain inherits from false_type.
 */
template<typename T,typename U> struct can_contain:std::false_type{};
template<typename T> struct can_contain<T,T>:std::true_type{};
template<typename T> struct can_contain<T,std::optional<T>>:std::true_type{};
template<typename T,typename U> struct can_contain<T,std::optional<U>>:can_contain<T,U>{};
template<typename T,typename... Ts> struct can_contain<T,std::tuple<Ts...>>:std::disjunction<can_contain<T,Ts>...>{};
template<typename T,typename... Ts> struct can_contain<T,std::variant<Ts...>>:std::disjunction<can_contain<T,Ts>...>{};
template<typename T,typename U,typename V> struct can_contain<T,std::pair<U,V>>:std::disjunction<can_contain<T,U>,can_contain<T,V>>{};
template<typename T> struct can_contain<const T,T>:std::true_type{};
template<typename T> struct can_contain<volatile T,T>:std::true_type{};
template<typename T> struct can_contain<const volatile T,T>:std::true_type{};
template<typename T> struct can_contain<T,const T>:std::true_type{};
template<typename T> struct can_contain<T,volatile T>:std::true_type{};
template<typename T> struct can_contain<T,const volatile T>:std::true_type{};
template<typename T,typename U> struct can_contain<T,const U>:can_contain<T,U>{};
template<typename T,typename U> struct can_contain<T,volatile U>:can_contain<T,U>{};
template<typename T,typename U> struct can_contain<T,const volatile U>:can_contain<T,U>{};
template<typename T,size_t N> struct can_contain<T,T[N]>:std::true_type{};
template<typename T,typename U,size_t N> struct can_contain<T,U[N]>:can_contain<T,U>{};
template<typename T,size_t N> struct can_contain<T,std::array<T,N>>:std::true_type{};
template<typename T,typename U,size_t N> struct can_contain<T,std::array<U,N>>:can_contain<T,U>{};
template<typename T,typename U> constexpr const bool can_contain_v = can_contain<T,U>::value;

/**
 * Checks if T is a byte type, a special subset of the scalar types with sizeof(T)==1.
 * By default is_byte is only specialized for char, unsigned char, signed char, or std::byte (to inherit from true_type),
 * is_byte inherits from false_type for all other types.
 * This trait may be specialized for T
 * The behavior is undefined if it is specialized for any T which is not a scalar type with sizeof(T)==1.
 * The behavior is additionally undefined if it is specialized for any T which does not follow the rules layed out for
 * Byte data types as specified by Special Object types.
 * If T is a byte type, then const T is also a byte type (but not volatile T).
 */
template<typename T> struct is_byte:std::false_type{};
template<> struct is_byte<char>:std::true_type{};
template<> struct is_byte<unsigned char>:std::true_type{};
template<> struct is_byte<signed char>:std::true_type{};
template<> struct is_byte<std::byte>:std::true_type{};
template<typename T> struct is_byte<const T>:is_byte<T>{};
template<typename T> constexpr const bool is_byte_v = is_byte<T>::value;

template<typename T> struct remove_cvptr:type_identity<std::remove_cv_t<std::remove_pointer_t<T>>>{};
template<typename T> using remove_cvptr_t = typename remove_cvptr<T>::type;

template<typename T> struct is_char:std::false_type{};
template<> struct is_char<char>:std::true_type{};
template<> struct is_char<wchar_t>:std::true_type{};
template<> struct is_char<char16_t>:std::true_type{};
template<> struct is_char<char32_t>:std::true_type{};
template<typename T> struct is_char<const T>:is_char<T>{};
template<typename T> struct is_char<volatile T>:is_char<T>{};
template<typename T> struct is_char<const volatile T>:is_char<T>{};
template<typename T> constexpr const bool is_char_v = is_char<T>::value;

template<typename T> struct is_cstring:std::false_type{};
template<typename T,std::size_t N> struct is_cstring<T[N]>:is_char<T>{};
template<typename T> struct is_cstring<T[]>:is_char<T>{};
template<typename T> struct is_cstring<T*>:is_char<T>{};
template<typename T> struct is_cstring<T&>:is_cstring<T>{};
template<typename T> struct is_cstring<const T>:is_cstring<T>{};
template<typename T> struct is_cstring<volatile T>:is_cstring<T>{};
template<typename T> struct is_cstring<const volatile T>:is_cstring<T>{};
template<typename T> constexpr const bool is_cstring_v = is_cstring<T>::value;

template<typename T,typename...> struct require_types:type_identity<T>{};

template<typename T,typename... Types> using require_types_t = typename require_types<T,Types...>::type;

template<template<typename> class Trait,typename... Ts> struct all_match: std::conjunction<Trait<Ts>...>{};
template<template<typename> class Trait,typename... Ts> constexpr const bool all_match_v = all_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> struct any_match: std::conjunction<Trait<Ts>...>{};
template<template<typename> class Trait,typename... Ts> constexpr const bool any_match_v = any_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> struct none_match:std::conjunction<std::negation<Trait<Ts>>...>{};
template<template<typename> class Trait,typename... Ts> constexpr const bool none_match_v = none_match<Trait,Ts...>::value;

#endif
