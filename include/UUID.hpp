#ifndef __UUID_hpp_2018_05_16_08_22
#define __UUID_hpp_2018_05_16_08_22
#include <string>
#include <cstdint>
#include "Hash.hpp"
#include <iostream>
#include <TypeInfo.hpp>

using std::string;
using std::ostream;
using std::istream;

class UUID{
private:
	uint64_t high;
	uint64_t low;
public:
	
	LIBLCFUNC static UUID fromString(const string&);
	LIBLCFUNC static UUID randomUUID();
	LIBLCFUNC static UUID uuidFromNamespace(const string)&;
	LIBLCFUNC static UUID ofNow();
	constexpr static UUID nilUUID(){
		return UUID{};
	}
	constexpr UUID(uint64_t h,uint64_t l):high(h),low(l){}
	constexpr UUID():high(0),low(0){}
	LIBLCFUNC UUID(string);
	LIBLCFUNC UUID(const char*);
	constexpr UUID(const UUID&)=default;
	constexpr UUID(UUID&&)=default;
	UUID(const UUID&&)=delete;
	constexpr UUID& operator=(const UUID&)=default;
	constexpr UUID& operator=(UUID&&)=default;
	UUID& operator=(const UUID&&)=delete;
	constexpr uint64_t getHigh()const{
		return high;
	}
	constexpr uint64_t getLow()const{
		return low;
	}
	constexpr int32_t hashCode()const{
		return hashcode(high)*31+hashcode(low);
	}
	LIBLCFUNC string toString()const;
	LIBLCFUNC operator string()const;
	constexpr bool operator==(const UUID& u)const{
		return high==u.high&&low==u.low;
	}
	constexpr bool operator!=(const UUID& u)const{
		return !(*this==u);
	}
	constexpr bool operator< (const UUID& u)const{
		return high<u.high||(high==u.high&&low<u.low);
	}
	constexpr bool operator> (const UUID& u)const{
		return !(*this<=u);
	}
	constexpr bool operator<=(const UUID& u)const{
		return *this<u||*this==u;
	}
	constexpr bool operator>=(const UUID& u)const{
		return !(*this<u);
	}
	
};

LIBLCFUNC ostream& operator<<(ostream&,const UUID&);
LIBLCFUNC istream& operator>>(istream&,UUID&);
LIBLCFUNC string  operator+(const string&,const UUID&);

constexpr int32_t hashcode(const UUID& u){
	return u.hashCode();
}

namespace std{
	template<> struct hash<UUID>{
		size_t operator()(const UUID& u){
			return u.hashCode();
		}
	};
}
namespace types{
	template<> struct TypeHash<UUID>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("UUID")^TypeHash<uint64_t>{}();
		}
	};
}

#endif
