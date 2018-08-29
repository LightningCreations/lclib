#ifndef __Base_hpp_2018_06_27_19_44
#define __Base_hpp_2018_06_27_19_44

#include <cmath>
#include <Hash.hpp>
#include <utility>
#include <type_traits>

template<typename To,typename From> constexpr std::enable_if_t<std::is_trivially_copyable_v<To>&&std::is_trivially_copyable_v<From>&&(sizeof(To)<=sizeof(From)),To> bit_cast(const From& f){
    union{
        From f;
        To t;
    } u = {f};
    return u.t;
}

struct Vec2{
    int x,y;
    constexpr Vec2():x(0),y(0){}
    constexpr Vec2(int x,int y):x(x),y(y){}
    constexpr Vec2(int(&arr)[2]):x(arr[0]),y(arr[1]){}


    constexpr Vec2(const Vec2&)=default;
    constexpr Vec2(Vec2&&)=default;
    Vec2(const Vec2&&)=delete;
    constexpr Vec2& operator=(const Vec2&)=default;
    constexpr Vec2& operator=(Vec2&&)=default;
    Vec2& operator=(const Vec2&&)=delete;

    constexpr Vec2 operator+(const Vec2& v)const{
        return Vec2{x+v.x,y+v.y};
    }
    constexpr Vec2& operator+=(const Vec2& v){
        x+=v.x;
        y+=v.y;
        return *this;
    }
    constexpr Vec2 operator-(const Vec2& v)const{
        return Vec2{x-v.x,y-v.y};
    }
    constexpr Vec2& operator-=(const Vec2& v){
        x-=v.x;
        y-=v.y;
        return *this;
    }
    constexpr int operator*(const Vec2& v)const{
        return x*v.x+y*v.y;
    }
    constexpr double magnetude()const{
        return sqrt(x*x+y*y);
    }
    constexpr double angle()const{
        return atan(double(x)/y);
    }
    constexpr static Vec2 fromMagnetudeDirection(double magnetude,double angle){
        return {int(magnetude*sin(angle)),int(magnetude*cos(angle))};
    }
    constexpr int32_t hashCode()const{
        return x*31+y;
    }

};
constexpr const Vec2 ORIGIN{0,0};


constexpr int32_t hashcode(const Vec2& v){
    return v.hashCode();
}


namespace std{
    template<> struct hash<Vec2>{
        constexpr hash()=default;
        constexpr size_t operator()(const Vec2& v){
            return hashcode(v);
        }
    };
};


#endif