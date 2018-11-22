#include <lclib/Hash.hpp>
#include <array>
#include <utility>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <typeinfo>
#include <cstdint>
#include <memory>
#include <chrono>






using std::type_info;

int32_t hashcode(const type_info& t){
	std::size_t hash = 0xf194be82;
	const std::size_t p1 = 2583988571;
	const std::size_t p2 = 606598079;
	for(const char* _c=t.name();*_c!=0;_c++){
		char q = *_c;
		hash *= p1;
		hash = hash<<1|hash>>31;
		hash += q*p2;
	}
	return hash;
}

int32_t hashcode(const std::string& str){
	int32_t h = 0;
	for(char c:str)
		(h*=31,h+=c);
	return h;
}

int32_t Hashable::hashCode() const{
	return ::hashcode((void*)this);
}

namespace std{
	template<> class hash<Hashable>{
	public:
		typedef Hashable argument_type;
		typedef size_t result_type;
		size_t operator()(const Hashable& h)throw(){
			return h.hashCode();
		}
	};
};

