#ifndef __Types_hpp_2018_08_22_20_05
#define __Types_hpp_2018_08_22_20_05

template<typename T> struct ValueDiscard{
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
    ValueDiscard(const ValueDiscard&&)=delete;
    constexpr ValueDiscard& operator=(const ValueDiscard&)=default;
    constexpr ValueDiscard& operator=(ValueDiscard&&)=default;
    ValueDiscard& operator=(const ValueDiscard&&)=delete;
    template<typename T> constexpr ValueDiscard& operator=(const T&)noexcept{
    	return *this;
    }
    template<typename T> constexpr ValueDiscard& operator=(T&&)noexcept{
		return *this;
	}
    template<typename T> ValueDiscard& operator=(const T&&)=delete;
};

using noreturn_t = void;



#endif
