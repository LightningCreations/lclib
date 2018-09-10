#ifndef __TypeTraits_hpp_2018_08_22_16_29
#define __TypeTraits_hpp_2018_08_22_16_29
#include <type_traits>



template <typename T>
struct is_complete : std::false_type  {};
template<typename T> struct is_complete<std::enable_if_t<sizeof(T)==sizeof(T),T>>:std::true_type{};

template<typename T> const constexpr bool is_complete_v = is_complete<T>::value;

template<typename T> struct type_identity{
	using type = T;
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



#endif
