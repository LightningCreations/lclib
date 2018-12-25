#ifndef __Types_hpp_2018_08_22_20_05
#define __Types_hpp_2018_08_22_20_05

template<typename T=void> struct ValueDiscard{
public:
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
    constexpr ValueDiscard& operator=(T&&)noexcept{
        return *this;
    }
    ValueDiscard& operator=(const T&&)=delete;
};
template<> struct ValueDiscard<void>{
public:
    constexpr ValueDiscard()=default;
    constexpr ValueDiscard(const ValueDiscard&)=default;
    constexpr ValueDiscard(ValueDiscard&&)=default;
    template<typename T> constexpr ValueDiscard& operator=(const T&)noexcept{
    	return *this;
    }
    template<typename T> constexpr ValueDiscard& operator=(T&&)noexcept{
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
