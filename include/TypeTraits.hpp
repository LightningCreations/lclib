#ifndef __TypeTraits_hpp_2018_08_22_16_29
#define __TypeTraits_hpp_2018_08_22_16_29
#include <type_traits>

template <typename T>
struct is_complete_helper {
    template <typename U>
    static auto test(U*)  -> std::integral_constant<bool, sizeof(U) == sizeof(U)>;
    static auto test(...) -> std::false_type;
    using type = decltype(test((T*)0));
};

template <typename T>
struct is_complete : typename is_complete_helper<T>::type {};

template<typename T> const constexpr bool is_complete_v = is_complete<T>::value;

#endif