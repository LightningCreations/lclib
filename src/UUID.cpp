#include <lclib-cxx/UUID.hpp>
#include <string>
#include <lclib-cxx/Hash.hpp>
#include <iomanip>
#include <sstream>
#include <lclib-cxx/StringHelper.hpp>
#include <cstdlib>
#include <lclib-cxx/ShadowRandom.hpp>
#include <lclib-cxx/JTime.hpp>
#include <chrono>

#include <openssl/sha.h>



LIBLCHIDE ShadowRandom uuidRandom{};



using std::string;
using std::stoull;
using std::ostringstream;
using std::ostream;
using std::istream;

const string sep("-");
const int sizes[] = {8,4,4,4,12};

static uint32_t clockSeq{3186784621};

const uint32_t clockSeqInc{3885574061};



UUID::UUID(string str){
	string currPart;
	string highPart;
	string lowPart;

	int i;
	for(i = 0;i<5;i++)
		currPart += tokenize(str,sep);
	highPart.assign(currPart,0,16);
	lowPart.assign(currPart,16,16);
	high = stoull(highPart,nullptr,16);
	low = stoull(lowPart,nullptr,16);
}

UUID::UUID(const char* str):UUID(string(str)){}


UUID UUID::fromString(const string& str){
	return UUID(str);
}


uint64_t randomVersion = 0x4000;
uint64_t versionMask = 0xf000;



ostream& operator<<(ostream& o,const UUID& id){
	uint64_t high = id.getHigh();
	uint64_t low = id.getLow();
	uint64_t high1 = high>>32;
	uint64_t high2 = (high>>16)&0xFFFF;
	uint64_t high3 = high&0xFFFF;
	uint64_t low1 = low>>48;
	uint64_t low2 = low&((1LL<<48)-1);
	o <<std::hex<<std::setw(8)<<std::setfill('0')<<high1<<
		sep<<std::hex<<std::setw(4)<<std::setfill('0')<<high2<<
		sep<<std::hex<<std::setw(4)<<std::setfill('0')<<high3<<
		sep<<std::hex<<std::setw(4)<<std::setfill('0')<<low1<<
		sep<<std::hex<<std::setw(12)<<std::setfill('0')<<low2;
	return o;
}

string UUID::toString()const{
	ostringstream str;
	str<<*this;
	return str.str();
}

istream& operator>>(istream& i,UUID& id){
	string s;
	i >> s;
	id= UUID::fromString(s);
	return i;
}

string operator+(const string& s,const UUID& id){
	return s+(id.toString());
}

#include <ratio>

using namespace std::chrono_literals;

using uuid_hnanos =
		std::chrono::duration<uint64_t,std::ratio<1,10000000>>;

using uuid_clock = std::chrono::system_clock;

using uuid_epoch_tp = std::chrono::time_point<uuid_clock,uuid_hnanos>;

const std::chrono::time_point<uuid_clock,std::chrono::seconds> epochSeconds{-12219292800s};
const auto uuidEpoch{std::chrono::time_point_cast<uuid_hnanos>(epochSeconds)};

const uint64_t now_version{0x1000};
const uint64_t now_variant{0xC000000000000000};
const uint64_t low_mask{0xffffffff00000000};
const uint64_t mid_mask{0xffff0000};
const uint64_t high_mask{0xfff};

const uint64_t seq_mask{0x1fff000000000000};
const uint64_t node_mask{0x7fffffffffffffff};
const uint64_t mcast_bit{0x8000000000000000};
UUID UUID::ofNow(){
	const auto now{std::chrono::time_point_cast<uuid_hnanos>(uuid_clock::now())};
	const auto dur = now-uuidEpoch;
	const uint64_t node = mcast_bit|(uuidRandom.nextLong()&(node_mask));
	const int64_t ts = dur.count();
	const uint64_t high = ((ts<<32)&low_mask)|((ts<<16)&mid_mask)|now_version|((ts>>48)&high_mask);
	uint64_t seq{clockSeq};
	clockSeq+=clockSeqInc;
	const uint64_t low = ((seq<<48)&seq_mask)|now_variant|node;
	return UUID{high,low};
}

UUID UUID::randomUUID(){
	char bytes[32];
	uint64_t longs[2];
	uuidRandom.nextBytes(reinterpret_cast<uint8_t*>(bytes),32);
	SHA256(reinterpret_cast<const unsigned char*>(bytes),32,reinterpret_cast<unsigned char*>(bytes));
	for(int i = 0;i<16;i++){
		bytes[i] = bytes[2*i]^bytes[2*i+1];
	}
	bytes[4] = (bytes[4]&0xf)|0x40;
	bytes[8] = (bytes[8]&0xcf)|0x80;
	longs[0] = uint64_t(bytes[0])<<56|uint64_t(bytes[1])<<48|uint64_t(bytes[2])<<40|uint64_t(bytes[3])<<3
			  |uint64_t(bytes[4])<<24|uint64_t(bytes[5])<<16|uint64_t(bytes[6])<<8|uint64_t(bytes[7]);
	longs[1] = uint64_t(bytes[8])<<56|uint64_t(bytes[9])<<48|uint64_t(bytes[10])<<40|uint64_t(bytes[11])<<3
			  |uint64_t(bytes[12])<<24|uint64_t(bytes[13])<<16|uint64_t(bytes[14])<<8|uint64_t(bytes[15]);
	return UUID(longs[0],longs[1]);
}

