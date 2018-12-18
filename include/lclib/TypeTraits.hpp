#ifndef __TypeTraits_hpp_2018_08_22_16_29
#define __TypeTraits_hpp_2018_08_22_16_29
#include <type_traits>
#include <tuple>
#include <variant>
#include <optional>
#include <cstddef>

#include <lclib/Config.hpp>

/**
 * A type alias for integral constants that can accept values of any integral or enumeration type.
 */
template<auto val> using auto_constant = std::enable_if_t<std::is_integral_v<decltype(val)>||std::is_enum_v<decltype(val)>,std::integral_constant<decltype(val),val>>;

namespace detail{
	template<typename T> auto is_complete_helper_fn(const std::decay_t<T>&) -> std::bool_constant<sizeof(T)==sizeof(T)>;
	template<typename T> auto is_complete_helper_fn(...) -> std::false_type;
	template<typename T> using is_complete_helper = decltype(is_complete_helper_fn<T>(std::declval<const T&>()));
}

/**
 * A UnaryTypeTrait that checks if a given type is a complete, object type (that is not a reference type, function type, (possibly cv-qualified) void,
 *  an array of an unknown bound, or (possibly cv-qualified) forward declared class or union type).
 * Otherwise (for such a type), is_complete<T> inherits from std::false_type.
 *
 * If T is a class or union type, then the behavior is undefined if
 *  is_complete is instantiated within the definition of T in any context therein that would treat T as being incomplete
 *  (that is, in the base-class-specifier, or in the declaration of any non-static member object.
 *
 * Additionally if is_complete<T> or is_complete_v<T> is odr-used in a translation unit that completes T,
 *  (except when is_complete<T> is instantiated after the T is completed), the behavior is undefined.
 *
 * Aside from the above conditions, instantiating is_complete with an incomplete type is well defined.
 */
template <typename T>
struct is_complete : detail::is_complete_helper<T>  {};

template<> struct is_complete<void> : std::false_type{};



template<typename T> const constexpr bool is_complete_v = is_complete<T>::value;

/*
 * Defines the identity Transformation Trait, that is, for any T that is either a complete type,
 *  (possibly cv-qualified) void, or an array of an unknown bound,
 *   then declares a member typedef type which is T.
 *
 * This is provided for compatability with C++20.
 */
template<typename T> struct type_identity{
	typedef T type;
};
template<typename T> using type_identity_t = typename type_identity<T>::type;

/**
 * Defines a transformation type trait that combines remove_reference and remove_cv.
 * For any type T:
 *  If T is a reference type (U& or U&&), then provides a member typedef type which is U with its top-level cv-qualifiers removed
 *  Otherwise (T is not a reference type), then provides a member typedef type which is T with its top-level cv-qualifiers removed
 *
 *  This is provided for compatability with C++20
 */
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
template<typename T,typename U,typename K=void> struct can_contain:std::false_type{
	static_assert(std::is_void_v<K>);
};
template<typename T> struct can_contain<T,T>:std::true_type{};
template<typename T> struct can_contain<T,std::optional<T>>:std::true_type{};
template<typename T,typename U> struct can_contain<T,std::optional<U>>:can_contain<T,U>{};
template<typename T,typename... Ts> struct can_contain<T,std::variant<Ts...>>:std::disjunction<can_contain<T,Ts>...>{};
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

namespace detail{
	template<typename T, typename Tuple, std::size_t... Ids> auto can_contain_tuple_helper_f(const T&, const Tuple& t, std::index_sequence<Ids...>)
		->std::conjunction<std::bool_constant<(std::tuple_size_v<Tuple> != 0)>,
		std::disjunction<can_contain<T, std::tuple_element_t<Ids, Tuple>>...>
		>;
	template<typename T,typename U> using can_contain_tuple_helper = decltype(can_contain_helper_tuple_f(std::declval<const T&>(),std::declval<const U&>(),std::make_index_sequence<std::tuple_size_v<U>>{}));
}

template<typename T,typename U> struct can_contain<T,U,std::void_t<decltype(std::tuple_size_v<U>)>>:detail::can_contain_tuple_helper<T,U>{};

template<typename T,typename U> constexpr const bool can_contain_v = can_contain<T,U>::value;

/**
 * Checks if T is a byte type, a special subset of the scalar types with sizeof(T)==1.
 * By default is_byte is only specialized for char, unsigned char, signed char, or std::byte (to inherit from true_type),
 * is_byte inherits from false_type for all other types.
 * This trait may be specialized for any user-defined type T
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

template<typename T,typename=void> struct is_char:std::false_type{};
template<> struct is_char<char>:std::true_type{};
#if defined(__cpp_char8_t)&&__cpp_char8_t>201811L
template<> struct is_char<char8_t>:std::true_type{};
#endif
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
template<typename T,std::size_t N> struct is_cstring<T(*)[N]>:is_char<T>{};
template<typename T> struct is_cstring<T(*)[]>:is_char<T>{};
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
template<template<typename> class Trait,typename... Ts> struct any_match: std::disjunction<Trait<Ts>...>{};
template<template<typename> class Trait,typename... Ts> constexpr const bool any_match_v = any_match<Trait,Ts...>::value;
template<template<typename> class Trait,typename... Ts> struct none_match:std::conjunction<std::negation<Trait<Ts>>...>{};
template<template<typename> class Trait,typename... Ts> constexpr const bool none_match_v = none_match<Trait,Ts...>::value;

template<typename T,typename U> struct copy_category:type_identity<U>{};
template<typename T,typename U> struct copy_category<T&,U>:type_identity<U&>{};
template<typename T,typename U> struct copy_category<T&&,U>:type_identity<U&&>{};

template<typename T,typename U> using copy_category_t = typename copy_category<T,U>::type;



/*
 * Checks a set of types that Share a single common type.
 * has_common_type has the following properties:
 * has_common_type implies commutability, that is (has_common_type<T,U>::value==has_common_type<U,T>)
 * It implies convertibility. Any expression that expects a value of type std::common_type_t<T,U> can accept values of either T or U,
 *  if has_common_type<T,U>::value is true.
 * It is reflexive (has_common_type<T,T> is true for all T)
 * It is transitive with a decreasing number of arguments
 *  (if has_common_type<Ts...> is true, then for each Us...
 *   that is a valid partial combination of types in Ts..., has_common_type<Us...> is also true)
 *
 * As with most type traits, a program that instantiates has_common_type with a
 *
 *
 *
 * If sizeof...(Ts) is 1 or for each type Ti and Tj in Ts... std::is_same_v<Ti,Tj> is true, then inherits from std::true_type.
 * Otherwise, if sizeof...(Ts) is 2, then inherits from std::true_type if all of the following requirements are met (otherwise inherits from std::false_type):
 *  Given that std::remove_cv_t<Ts>... expands to the types T,U:
 *  std::common_type_t<T,U> and std::common_type_t<U,T> must be valid types (Must share a common type)
 *  std::is_same_v<std::common_type_t<T,U>,std::common_type_t<U,T>> must be true (Commutative Property)
 *  std::common_type_t<T,std::common_type_t<T,U>> must exist and be the same as std::common_type_t<T,U>
 *  std::common_type_t<U,std::common_type_t<T,U>> must exist and be the same as std::common_type_t<T,U>
 *  Both T and U must be convertible to std::common_type_t<T,U>
 * Otherwise, if sizeof...(Ts) is 3, then inherits from std::true_type if all of the following requirements are met (otherwise inherits from std::false_type):
 *  Given that Ts... expands to the types T,U,V
 *  std::common_type_t<T,U,V> must be a valid type
 *  All of has_common_type<T,U>, has_common_type<T,V>, and has_common_type<U,V> must be true.
 *  T must share a common type (as per the above rules) with std::common_type_t<U,V>
 *  U must share a common type (as per the above rules) with std::common_type_t<T,V>
 *  V must share a common type (as per the above rules) with std::common_type_t<T,U>
 *  The common type of each pair in <T,U,V> (<T,U>,<T,V>,and <U,V>) must share a common type which each other pair.
 *  The common type between each of T, U, V and std::common_type_t<T,U,V> must be std::common_type_t<T,U,V>
 * Otherwise, if sizeof...(Ts) is 4, then inherits from std::true_type if all of the following requirements are met (otherwise inherits from std::false_type):
 *  Given that Ts... expands to the types T,U,V,W
 *  std::common_type_t<T,U,V,W> must be a valid type
 *  Each Pair or triplet of types in <T,U,V,W> must share a common type.
 *  T must share a common type with the common type of each pair of types in <U,V,W> and with std::common_type_t<U,V,W>.
 *  U must share a common type with the common type of each pair of types in <T,V,W> and with std::common_type_t<T,V,W>.
 *  V must share a common type with the common type of each pair of types in <T,U,W> and with std::common_type_t<T,U,W>.
 *  W must share a common type with the common type of each pair of types in <T,U,V> and with std::common_type_t<T,U,V>.
 *  For each pair of types in <T,U,V,W>, that pair must share a common type with each other pair.
 *  For each triplet of types in <T,U,V,W>, that triplet must share a common type with each other triplet.
 *  The common type between Each of T, U, V, W and std::common_type_t<T,U,V,W> must be std::common_type<T,U,V,W>
 * Otherwise, inherits from std::true_type if all of the following requirements are met (inherits from std::false_type otherwise):
 *  Given that Ts... expands to the types T,U,Rest...
 *  T and U must share a common type
 *  Each type in Rest... must share a common type
 *  T must share a common type with each type in Rest... and with std::common_type_t<Rest...>
 *  U must share a common type with each type in Rest... and with std::common_type_t<Rest...>
 *  std::common_type_t<T,U> must share a common type with each type in Rest... and with std::common_type_t<Rest...>
 *  The common type between T, U, and each type in Rest... and std::common_type_t<T,U,Rest...> must be std::common_type_t<T,U,Rest...>
 * Otherwise, inherits from std::false_type
 */
template<typename... Ts> struct has_common_type:std::false_type{};
template<> struct has_common_type<>:std::false_type{};
template<typename T> struct has_common_type<T>:std::true_type{};
template<typename T> struct has_common_type<T, T> :std::true_type {};
template<typename T,typename U> struct has_common_type<T,U>:std::conjunction<
	std::is_same<std::common_type_t<T,U>,std::common_type_t<U,T>>,
	std::is_same<std::common_type_t<T,std::common_type_t<T,U>>,std::common_type_t<T,U>>,
	std::is_same<std::common_type_t<U,std::common_type_t<T,U>>,std::common_type_t<T,U>>,
	std::is_convertible<T,std::common_type_t<T,U>>,
	std::is_convertible<U,std::common_type_t<T,U>>>{};
template<typename T,typename U,typename V> struct has_common_type<T,U,V>:std::conjunction<
	has_common_type<T,U>,
	has_common_type<U,V>,
	has_common_type<T,V>,
	has_common_type<T,std::common_type_t<U,V>>,
	has_common_type<U,std::common_type_t<T,V>>,
	has_common_type<V,std::common_type_t<T,U>>,
	has_common_type<std::common_type_t<T,U>,std::common_type_t<U,V>>,
	has_common_type<std::common_type_t<T,U>,std::common_type_t<T,V>>,
	has_common_type<std::common_type_t<U,V>,std::common_type_t<T,V>>,
	std::is_same<std::common_type_t<T,std::common_type_t<T,U,V>>,std::common_type_t<T,U,V>>,
	std::is_same<std::common_type_t<U,std::common_type_t<T,U,V>>,std::common_type_t<T,U,V>>,
	std::is_same<std::common_type_t<V,std::common_type_t<T,U,V>>,std::common_type_t<T,U,V>>
>{};
template<typename T,typename U,typename V,typename W> struct has_common_type<T,U,V,W>:std::conjunction<
	has_common_type<T,U>,
	has_common_type<T,V>,
	has_common_type<T,W>,
	has_common_type<U,V>,
	has_common_type<U,W>,
	has_common_type<V,W>,
	has_common_type<T,U,V>,
	has_common_type<T,V,W>,
	has_common_type<T,U,W>,
	has_common_type<U,V,W>,
	has_common_type<T,U,std::common_type_t<V,W>>,
	has_common_type<T,V,std::common_type_t<U,W>>,
	has_common_type<T,W,std::common_type_t<U,V>>,
	has_common_type<U,V,std::common_type_t<T,W>>,
	has_common_type<U,W,std::common_type_t<T,V>>,
	has_common_type<V,W,std::common_type_t<T,U>>,
	has_common_type<T,std::common_type_t<U,V,W>>,
	has_common_type<U,std::common_type_t<T,V,W>>,
	has_common_type<V,std::common_type_t<T,U,W>>,
	has_common_type<W,std::common_type_t<T,U,V>>,
	std::is_same<std::common_type_t<T,std::common_type_t<T,U,V,W>>,std::common_type_t<T,U,V,W>>,
	std::is_same<std::common_type_t<U,std::common_type_t<T,U,V,W>>,std::common_type_t<T,U,V,W>>,
	std::is_same<std::common_type_t<V,std::common_type_t<T,U,V,W>>,std::common_type_t<T,U,V,W>>,
	std::is_same<std::common_type_t<W,std::common_type_t<T,U,V,W>>,std::common_type_t<T,U,V,W>>
>{};

template<typename T, typename U, typename... Rest> struct has_common_type<T, U, Rest...> :std::disjunction<
std::conjunction<
	std::is_same<T,U>,
	std::is_same<T,Rest>...
>,std::conjunction<
	has_common_type<T, Rest...>,
	has_common_type<U, Rest...>,
	has_common_type<Rest...,T>,
	has_common_type<Rest...,U>,
	has_common_type<Rest...>,
	has_common_type<std::common_type_t<T,U>,Rest...>,
	has_common_type<T,U,std::common_type_t<Rest...>>,
	has_common_type<std::common_type_t<T,U>,std::common_type_t<Rest...>>,
	has_common_type<std::common_type_t<T,U>,std::common_type_t<T,Rest...>,std::common_type_t<U,Rest...>>,std::common_type_t<Rest...>>,
	std::is_same<std::common_type_t<T,std::common_type_t<T,U,Rest...>>,std::common_type_t<T,U,Rest...>>,
	std::is_same<std::common_type_t<U,std::common_type_t<T,U,Rest...>>,std::common_type_t<T,U,Rest...>>,
	std::is_same<std::common_type_t<Rest,std::common_type_t<T,U,Rest...>>,std::common_type_t<T,U,Rest...>>...
	>{};//Yes this gets unwieldy quickly.
	//But there's not much choice. has_common_type requires comutative operations

template<typename... Ts> constexpr bool has_common_type_v = has_common_type<Ts...>::value;

#endif
