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

#endif
