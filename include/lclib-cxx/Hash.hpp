#ifndef __Hash_hpp_2018_03_23_09_48
#define __Hash_hpp_2018_03_23_09_48
#pragma once
#include <lclib-cxx/Config.hpp>
#include <cstdint>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <array>
#include <variant>
#include <lclib-cxx/Vector.hpp>


class LIBLCAPI Hashable{
public:
	virtual int hashCode()const = 0;
	virtual ~Hashable()=default;
};


  
constexpr int32_t hashcode(int i){
	return i;
}
constexpr int32_t hashcode(char c){
	return int(c)|(c&0x80?0xffffff00:0);
}
constexpr int32_t hashcode(short s){
	return int(s)|(s&0x8000?0xffff0000:0);
}
constexpr int32_t hashcode(int64_t i){
	return int(i)^int(i>>32);
}
constexpr int32_t hashcode(unsigned char c){
	return int(c)&0xff;
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
	return int32_t(v);
}
constexpr int32_t hashcode(nullptr_t n){
	return 0;
}
constexpr int32_t hashcode(const char* str){
	int32_t h = 0;
	while(*str!=0)
		(h*=31,h+=hashcode(*str),str++);
	return h;
}

LIBLCAPI int32_t hashcode(const std::string&);
LIBLCAPI int32_t hashcode(const std::type_info&);

using std::vector;

template<typename T> auto hashcode(const vector<T>& v)->decltype(hashcode(v[0])){
	int32_t h = 0;
	for(int i = 0;i<v.size();i++){
		h*=31;
		h+=hashcode(v[i]);
	}
	return h;
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

using std::unique_ptr;
using std::shared_ptr;

const int32_t uniqueSmartPrime = 11443717;
const int32_t sharedSmartPrime = 540283;

template<typename T> auto hashcode(const unique_ptr<T>&ptr)->decltype(hashcode(ptr.get())){
	return hashcode(ptr.get())*uniqueSmartPrime;
}

template<typename T> auto hashcode(const shared_ptr<T>& ptr)->decltype(hashcode(ptr.get())){
	return hashcode(ptr.get())*sharedSmartPrime;
}

using std::pair;
template<typename T,typename U> auto hashcode(const pair<T,U>& p)->std::common_type_t<decltype(hashcode(p.first)),decltype(hashcode(p.second))>{
	return hashcode(p.first)*31+hashcode(p.second);
}

using std::map;

template<typename K,typename V> int32_t hashcode(const map<K,V>& m){
	int32_t hash = 0;
	for(const pair<K,V>& p:m){
		hash*=31;
		hash+=hashcode(p);
	}
	return hash;
}
template<typename... T> constexpr auto hashcode(const std::variant<T...>& var)->std::common_type_t<decltype(hashcode(std::declval<T>()))...>{
	return std::visit([](auto&& a)->std::common_type_t<decltype(hashcode(std::declval<T>()))...>{return hashcode(std::forward(a));},var);
}



namespace detail{
	template<typename T> constexpr int32_t hashcodes(T&& t){
		return hashcode(t);
	}
	constexpr int32_t hashcodes(){
		return 0;
	}
	template<typename T,typename... Ts> constexpr int32_t hashcodes(T&& t,Ts&&... valList){
		return hashcode(std::forward(t))*31+hashcodes(std::forward(valList)...);
	}
	template<typename Tuple,std::size_t... Ids> constexpr auto hashcode(const Tuple& t,std::index_sequence<Ids...>)
		->std::common_type_t<decltype(hashcode(std::get<Ids>(t)))...>{
		return hashcodes(std::get<Ids>(t)...);
	}
}

template<typename Tuple,typename=std::void_t<
			decltype(detail::hashcode(std::declval<const Tuple&>(),std::make_index_sequence<std::tuple_size_v<Tuple>>{}))
		>
	>
	constexpr decltype(auto) hashcode(const Tuple& t){
	return detail::hashcode(t,std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}


template<typename E> constexpr int32_t hashcode(typename std::enable_if<std::is_enum<E>::value,E>::type e){
	return hashcode(static_cast<std::underlying_type_t<E>>(e));
}



#endif
