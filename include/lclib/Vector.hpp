#ifndef __Base_hpp_2018_06_27_19_44
#define __Base_hpp_2018_06_27_19_44

#include <cmath>
#include <lclib/Hash.hpp>
#include <utility>
#include <type_traits>
#include <lclib/Types.hpp>
#include <lclib/Operations.hpp>
#include <lclib/TypeTraits.hpp>
#include <lclib/Detectors.hpp>
#include <tuple>
#include <functional>

#include <lclib/BitCast.hpp>



template<typename T> struct Matrix;

using std::sqrt;
template<typename T> struct Vec2{
	static_assert(!is_specialization_v<Vec2,T>&&!is_specialization_v<Matrix,T>,"T may not be a specialization of Vec2 or Matrix");
    T x,y;
    constexpr Vec2():x{},y{}{}
    constexpr Vec2(T x,T y):x{x},y{y}{}
    template<typename Q,typename U,typename=std::enable_if_t<std::is_constructible_v<T,Q>&&std::is_constructible_v<T,U>>>
    	explicit constexpr Vec2(Q x,U y) :x(x),y(y){}


    constexpr Vec2(const Vec2&)=default;
    constexpr Vec2(Vec2&&)=default;

    template<typename U,std::enable_if_t<std::is_constructible_v<T,U>&&std::is_convertible_v<U,T>>* = nullptr>
    	constexpr Vec2(const Vec2<U>& v):x(v.x),y(v.y){}
    template<typename U,std::enable_if_t<std::is_constructible_v<T,U>&&!std::is_convertible_v<U,T>>* = nullptr>
		constexpr explicit Vec2(const Vec2<U>& v):x(v.x),y(v.y){}

    constexpr Vec2& operator=(const Vec2&)=default;
    constexpr Vec2& operator=(Vec2&&)=default;

    constexpr auto operator+()const -> Vec2<decltype(+x)>{
    	return *this;
    }
    constexpr auto operator-()const -> Vec2<decltype(-x)>{
    	return {-x,-y};
    }

    template<typename U> constexpr auto operator+(const Vec2<U>& v)const -> Vec2<decltype(x+v.x)>{
        return {x+v.x,y+v.y};
    }
    template<typename U> constexpr auto operator+=(const Vec2<U>& v) -> require_types_t<Vec2&,decltype(x+=v.x)>{
        x+=v.x;
        y+=v.y;
        return *this;
    }
    template<typename U> constexpr auto operator-(const Vec2<U>& v)const -> Vec2<decltype(x-v.x)>{
        return {x-v.x,y-v.y};
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
    	return {v*x,v*y};
    }
    template<typename U> constexpr auto operator*=(U v)const -> require_types_t<Vec2&,decltype(x*=v)>{
    	x *= v;
    	y *= v;
    	return *this;
    }
    template<typename U> constexpr auto operator/(U v)const -> Vec2<decltype(x/v)>{
    	return {x/v,y/v};
    }
    template<typename U> constexpr friend auto operator*(U m,const Vec2& v)-> Vec2<decltype(v.x*m)>{
    	return {v.x*m,v.y*m};
    }
    template<typename U> constexpr auto operator/=(U v)const -> require_types_t<Vec2&,decltype(x/=v)>{
		x /= v;
		y /= v;
		return *this;
	}

    constexpr decltype(auto) hashCode()const{
        return hashcode(x)*31+hashcode(y);
    }

    template<typename U> constexpr auto operator==(const Vec2<U>& v)const ->decltype(x==v.x){
    	return std::tie(x,y)==std::tie(v.x,v.y);
    }

    template<typename U> constexpr friend auto operator!=(const Vec2& v1,const Vec2<U>& v2)->decltype(!(v1==v2)){
    	return !(v1==v2);
    }

    constexpr decltype(auto) magnetudeSquared()const{
    	return x*x+y*y;
    }

    constexpr auto magnetude()const ->decltype(sqrt(magnetudeSquared())){
    	return sqrt(magnetudeSquared());
    }
   	constexpr auto normalize()const -> Vec2<decltype(magnetude())> {
    	return *this/magnetude();
    }

   	template<typename=std::enable_if_t<std::is_constructible_v<bool,T>>> constexpr explicit operator bool()const{
   		return x||y;
   	}

};
template<typename T> constexpr const Vec2<T> ORIGIN{};


template<typename T> constexpr int32_t hashcode(const Vec2<T>& v){
    return v.hashCode();
}

template<typename T> Vec2(T,T) -> Vec2<T>;
template<typename Q,typename U> Vec2(Q,U) -> Vec2<std::common_type_t<Q,U>>;
Vec2() -> Vec2<int>;

template<typename T> struct Matrix{
public:
	static_assert(!is_specialization_v<Vec2,T>&&!is_specialization_v<Matrix,T>,"T may not be a specialization of Vec2 or Matrix");
	T a,b,c,d;

};

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
