#ifndef __UUID_hpp_2018_05_16_08_22
#define __UUID_hpp_2018_05_16_08_22
#include <string>
#include <cstdint>
#include <lclib-cxx/Hash.hpp>
#include <iostream>
#include <codecvt>

using std::ostream;
using std::istream;

struct current_timestamp_t{
	constexpr explicit current_timestamp_t()=default;
};

const current_timestamp_t current_timestamp{};

struct md5_namespace_t{
	constexpr explicit md5_namespace_t()=default;
};

const md5_namespace_t md5_namespace{};

struct sha1_namespace_t{
	constexpr explicit sha1_namespace_t()=default;
};

const sha1_namespace_t sha1_namespace{};

class LIBLCAPI UUID{
private:
	uint64_t high;
	uint64_t low;
public:
	
	static UUID fromString(std::string_view);
	static UUID randomUUID();
	static UUID uuidFromNamespace(std::string_view);
	static UUID uuidFromSHA1Namespace(std::string_view);
	static UUID ofNow();
	constexpr static UUID nilUUID(){
		return UUID{};
	}
	constexpr UUID(uint64_t h,uint64_t l):high(h),low(l){}
	constexpr UUID():high(0),low(0){}
	UUID(current_timestamp_t);
	UUID(std::string_view);
	UUID(const char*);
	UUID(std::string_view,md5_namespace_t);
	UUID(std::string_view,sha1_namespace_t);
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
	std::string toString()const;
	operator std::string()const;
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

LIBLCAPI ostream& operator<<(ostream&,const UUID&);
LIBLCAPI istream& operator>>(istream&,UUID&);
LIBLCAPI std::string   operator+(const std::string&,const UUID&);

constexpr int32_t hashcode(const UUID& u){
	return u.hashCode();
}

namespace uuid_literals{
	inline UUID operator""uuid(const char* str,std::size_t sz){
		return UUID{std::string_view{str,sz}};
	}
	inline UUID operator""md5(const char* str,std::size_t sz){
		return UUID{std::string_view{str,sz},md5_namespace};
	}
	inline UUID operator""sha1(const char* str,std::size_t sz){
		return UUID{std::string_view{str,sz},sha1_namespace};
	}
}

namespace std{
	template<> struct hash<UUID>{
		size_t operator()(const UUID& u){
			return u.hashCode();
		}
	};
}

#endif
