#ifndef __Hash_hpp_2018_03_23_09_48
#define __Hash_hpp_2018_03_23_09_48
#pragma once
#include <lclib/Config.hpp>
#include <cstdint>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <utility>
#include <array>
#include <variant>
#include <lclib/Vector.hpp>
#include <lclib/TypeTraits.hpp>
#include <numeric>
#include <algorithm>
#include <cstdint>

class LIBLCAPI Hashable{
public:
	virtual int hashCode()const = 0;
	virtual ~Hashable()=default;
};

struct HashSum{
public:
	constexpr HashSum()=default;
	~HashSum()=default;
	constexpr HashSum(const HashSum&)=default;
	constexpr HashSum(HashSum&&)=default;
	constexpr HashSum& operator=(const HashSum&)=default;
	constexpr HashSum& operator=(HashSum&&)=default;
	template<typename T> constexpr int32_t operator()(int32_t total,const T& next){
		return total*31+hashcode(next);
	}
};

template<typename T> struct LCLibHash{
public:
	constexpr LCLibHash()=default;
	~LCLibHash()=default;
	constexpr LCLibHash(const LCLibHash&)=default;
	constexpr LCLibHash& operator=(const LCLibHash&)=default;
	constexpr std::size_t operator()(const T& t){
		return hashcode(t);
	}
};
  
constexpr int32_t hashcode(int i){
	return i;
}
constexpr int32_t hashcode(char c){
	return int(c);
}
constexpr int32_t hashcode(signed char c){
	return int(c)|(c&0x80?0xffffff00:0);
}
constexpr int32_t hashcode(unsigned char c){
	return int(c)&0xff;
}
constexpr int32_t hashcode(wchar_t c){
	return int(c);
}
constexpr int32_t hashcode(char16_t c){
	return int(c);
}
constexpr int32_t hashcode(char32_t c){
	return int(c);
}
constexpr int32_t hashcode(short s){
	return int(s)|(s&0x8000?0xffff0000:0);
}
constexpr int32_t hashcode(int64_t i){
	return int(i)^int(i>>32);
}
constexpr int32_t hashcode(unsigned short s){
	return int(s)&0xffff;
}
constexpr int32_t hashcode(unsigned int i){
	return int(i);
}
constexpr int32_t hashcode(uint64_t i){
	return int(i)^int(i>>32);
}

constexpr int32_t hashcode(float f){
	union{
		float f1;
		int bits;
	} u = {f};
	return u.bits;
}
constexpr int32_t hashcode(double d){
	union{
		double d1;
		uint64_t bits;
	}u = {d};
	return hashcode(u.bits);
}
constexpr int32_t hashcode(const void* v){
	return hashcode(reinterpret_cast<std::uintptr_t>(v));
}
constexpr int32_t hashcode(std::nullptr_t n){
	return 0;
}
constexpr int32_t hashcode(const char* str){
	int32_t h = 0;
	while(*str!=0)
		(h*=31,h+=hashcode(*str),str++);
	return h;
}

constexpr int32_t hashcode(const char16_t* str){
	int32_t h = 0;
	while(*str!=0)
		(h*=31,h+=hashcode(*str),str++);
	return h;
}

constexpr int32_t hashcode(const wchar_t* str){
	int32_t h = 0;
	while(*str!=0)
		(h*=31,h+=hashcode(*str),str++);
	return h;
}

constexpr int32_t hashcode(const char32_t* str){
	int32_t h = 0;
	while(*str!=0)
		(h*=31,h+=hashcode(*str),str++);
	return h;
}


/**
 * Fully Replace std::string with template because security::string
 */
template<typename CharT,typename CharTraits,typename Allocator> int32_t hashcode(const std::basic_string<CharT,CharTraits,Allocator>& s){
	int32_t h = 0;
	for(auto c:s)
		(h*=31,h+=hashcode(c));
	return h;
}
template<typename CharT,typename CharTraits> constexpr int32_t hashcode(const std::basic_string_view<CharT,CharTraits>& sv){
	int32_t h = 0;
	for(auto c:sv)
		(h*=31,h+=hashcode(c));
	return h;
}
inline int32_t hashcode(const std::type_info& t){
	std::size_t hash = 0xf194be82;
	const std::size_t p1 = 2583988571;
	const std::size_t p2 =  606598079;
	for(const char* _c=t.name();*_c!=0;_c++){
		char q = *_c;
		hash *= p1;
		hash = hash<<1|hash>>31;
		hash += q*p2;
	}
	return hash;
}



using std::array;

template<typename T,size_t size>  auto hashcode(const array<T,size>& a)->decltype(hashcode(a[0])){
	int32_t h = 0;
	for(int i = 0;i<size;i++){
		h*=31;
		h+=hashcode(a[i]);
	}
	return h;
} 

template<typename T,size_t size> constexpr auto hashcode(const T(&a)[size])->decltype(hashcode(a[0])){
	int32_t h = 0;
	for(const T& val:a){
		h*=31;
		h+=hashcode(val);
	}
	return h;
}



const int32_t uniqueSmartPrime = 11443717;
const int32_t sharedSmartPrime = 540283;
const int32_t weakSmartPrime = 53569;

template<typename T,typename Deleter> auto hashcode(const std::unique_ptr<T,Deleter>&ptr)->decltype(hashcode(ptr.get())){
	return hashcode(ptr.get())*uniqueSmartPrime;
}

template<typename T> auto hashcode(const std::shared_ptr<T>& ptr)->decltype(hashcode(ptr.get())){
	return hashcode(ptr.get())*sharedSmartPrime;
}

template<typename T> auto hashcode(const std::weak_ptr<T>& ptr)->decltype(hashcode(ptr.get())){
	return hashcode(ptr.get())*weakSmartPrime;
}

using std::pair;
template<typename T,typename U> auto hashcode(const std::pair<T,U>& p)->std::common_type_t<decltype(hashcode(p.first)),decltype(hashcode(p.second))>{
	return hashcode(p.first)*31+hashcode(p.second);
}


using std::begin;
using std::end;

template<typename Container> constexpr auto hashcode(const Container& c)->require_types<int32_t,decltype(hashcode(*begin(c))),decltype(end(c))>{
	return std::accumulate(begin(c),end(c),0,HashSum{});
}

template<typename... T> constexpr auto hashcode(const std::variant<T...>& var)->std::common_type_t<decltype(hashcode(std::declval<T>()))...>{
	return std::visit([](auto&& a)->std::common_type_t<decltype(hashcode(std::declval<T>()))...>{return hashcode(std::forward(a));},var);
}

template<typename E> constexpr int32_t hashcode(typename std::enable_if<std::is_enum<E>::value,E>::type e){
	return hashcode(static_cast<std::underlying_type_t<E>>(e));
}




#endif
