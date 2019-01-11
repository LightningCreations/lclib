#ifndef __Types_hpp_2018_08_22_20_05
#define __Types_hpp_2018_08_22_20_05

#include <type_traits>

template<typename T=void> struct ValueDiscard{
public:
	static_assert(!std::is_reference_v<T>&&std::is_function_v<T>,"A program that instantiates ValueDiscard with a reference type is ill-formed");
    constexpr ValueDiscard()=default;
    constexpr ValueDiscard(const ValueDiscard&)=default;
    constexpr ValueDiscard(ValueDiscard&&)=default;
    ValueDiscard(const ValueDiscard&&)=delete;
    constexpr ValueDiscard& operator=(const ValueDiscard&)=default;
    constexpr ValueDiscard& operator=(ValueDiscard&&)=default;
    ValueDiscard& operator=(const ValueDiscard&&)=delete;
    constexpr ValueDiscard& operator=(const T&)noexcept{
        return *this;
    }
};
template<typename T> struct ValueDiscard<volatile T>{
public:
	static_assert(!std::is_void_v<T>,"ValueDiscard may not be instantiated with cv-qualified void");
    constexpr ValueDiscard()=default;
    ValueDiscard& operator=(const volatile T& t)noexcept{
    	static_cast<T>(t);
    	return *this;
    }
};
template<typename T> struct ValueDiscard<const T>:public ValueDiscard<T>{
public:
	static_assert(!std::is_void_v<T>,"ValueDiscard may not be instantiated with cv-qualified void");
};
template<> struct ValueDiscard<void>{
public:
    constexpr ValueDiscard()=default;
    template<typename T> constexpr ValueDiscard& operator=(const T& t)noexcept{
    	ValueDiscard<T>{}=t;
    	return *this;
	}
};

using noreturn_t = void;

ValueDiscard()->ValueDiscard<void>;

namespace detail{
	struct zero_literal_pmr_helper{};
}

using zero_literal_t = void(detail::zero_literal_pmr_helper::*)(char,signed char,unsigned char,short,unsigned short,int,unsigned,long,unsigned long,long long,unsigned long long,float,double,long double,wchar_t,char16_t,char32_t,detail::zero_literal_pmr_helper);

#endif
