#ifndef __Base_hpp_2018_06_27_19_44
#define __Base_hpp_2018_06_27_19_44

#include <cmath>
#include <lclib/Hash.hpp>
#include <utility>
#include <type_traits>
#include <lclib/Types.hpp>
#include <lclib/Operations.hpp>
#include <tuple>
#if __has_include(<bit>)&&__cplusplus>201703L
#if defined(__cpp_bit_cast) && __cpp_bit_cast > 201806
#include <bit>
using std::bit_cast;
#define __HAS_BIT_CAST
#endif
#endif
#ifndef __HAS_BIT_CAST
#include <cstddef>
#include <new>
template<typename To,typename From> constexpr std::enable_if_t<sizeof(To)==sizeof(From)&&std::is_trivially_copyable_v<To>&&std::is_trivially_copyable_v<From>&&std::is_trivially_default_constructible_v<To>,To>
	bit_cast(const From& f){
		return *std::launder(reinterpret_cast<const To*>(&f));
	}

#endif
using std::sqrt;
template<typename T> struct Vec2{
    T x,y;
    constexpr Vec2():x{0},y{0}{}
    explicit constexpr Vec2(T x,T y):x{x},y{y}{}
    template<typename Q,typename U,typename=std::enable_if_t<std::is_constructible_v<T,Q>&&std::is_constructible_v<T,U>>>
    	explicit constexpr Vec2(Q x,U y):x(x),y(y){}


    constexpr Vec2(const Vec2&)=default;
    constexpr Vec2(Vec2&&)=default;

    template<typename U,std::enable_if_t<std::is_constructible_v<T,U>&&std::is_convertible_v<U,T>>* = nullptr>
    	constexpr Vec2(const Vec2<U>& v):x(v.x),y(v.y){}
    template<typename U,std::enable_if_t<std::is_constructible_v<T,U>&&!std::is_convertible_v<U,T>>* = nullptr>
		constexpr explicit Vec2(const Vec2<U>& v):x(v.x),y(v.y){}

    constexpr Vec2& operator=(const Vec2&)=default;
    constexpr Vec2& operator=(Vec2&&)=default;

    template<typename U> constexpr auto operator+(const Vec2<U>& v)const -> Vec2<decltype(x+v.x)>{
        return Vec2<decltype(x+v.x)>{x+v.x,y+v.y};
    }
    template<typename U> constexpr auto operator+=(const Vec2<U>& v) -> require_types_t<Vec2&,decltype(x+=v.x)>{
        x+=v.x;
        y+=v.y;
        return *this;
    }
    template<typename U> constexpr auto operator-(const Vec2<U>& v)const -> Vec2<decltype(x-v.x)>{
        return Vec2<decltype(x-v.x)>{x-v.x,y-v.y};
    }
    template<typename U> constexpr auto operator-=(const Vec2<U>& v) -> require_types_t<Vec2&,decltype(x-=v.x)>{
        x-=v.x;
        y-=v.y;
        return *this;
    }
    template<typename U> constexpr auto operator*(const Vec2<U>& v)const -> decltype(x*v.x){
        return x*v.x+y*v.y;
    }

    template<typename U> constexpr auto operator*(U v)const -> Vec2<decltype(v*x)>{
    	return Vec2<decltype(v*x)>{v*x,v*y};
    }
    template<typename U> constexpr auto operator*=(U v)const -> require_types_t<Vec2&,decltype(x*=v)>{
    	x *= v;
    	y *= v;
    	return *this;
    }
    template<typename U> constexpr auto operator/(U v)const -> Vec2<decltype(x/v)>{
    	return Vec2<decltype(x/v)>{x/v,y/v};
    }
    template<typename U> constexpr auto operator*=(U v)const -> require_types_t<Vec2&,decltype(x/=v)>{
        	x /= v;
        	y /= v;
        	return *this;
        }

    constexpr int32_t hashCode()const{
        return hashcode(x)*31+hashcode(y);
    }

    template<typename U> constexpr auto operator==(const Vec2<U>& v)const ->decltype(x==v.x){
    	return std::tie(x,y)==std::tie(v.x,v.y);
    }
    template<typename U> constexpr friend auto operator!=(const Vec2& v1,const Vec2<U>& v2)->decltype(!(v1==v2)){
    	return !(v1==v2);
    }

    constexpr T magnetudeSquared()const{
    	return x*x+y*y;
    }

    auto magnetude()const ->decltype(sqrt(magnetudeSquared())){
    	return sqrt(magnetudeSquared());
    }
   	auto normalize()const -> Vec2<decltype(magnetude())> {
    	return *this/magnetude();
    }

};
template<typename T> constexpr const Vec2<T> ORIGIN{0,0};


template<typename T> constexpr int32_t hashcode(const Vec2<T>& v){
    return v.hashCode();
}

template<typename T> Vec2(T,T) -> Vec2<T>;
template<typename Q,typename U> Vec2(Q,U) -> Vec2<std::common_type_t<Q,U>>;
Vec2() -> Vec2<int>;

namespace std{
    template<typename T> struct hash<Vec2<T>>{
        constexpr hash()=default;
        constexpr size_t operator()(const Vec2<T>& v){
            return hashcode(v);
        }
    };
    template<typename Q,typename U> struct common_type<Vec2<Q>,Vec2<U>>:
		type_identity<Vec2<common_type_t<Q,U>>>{};
};


#endif
