#include <lclib/UUID.hpp>
#include <string>
#include <lclib/Hash.hpp>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <lclib/ShadowRandom.hpp>
#include <lclib/JTime.hpp>
#include <chrono>
#include <stdexcept>
#include <lclib-cxx/security/SHA.hpp>
#include <lclib-cxx/security/MessageDigest.hpp>

#include <openssl/md5.h>



static ShadowRandom uuidRandom{};



using std::string;
using std::stoull;
using std::ostringstream;
using std::ostream;
using std::istream;

const string sep("-");
const int sizes[] = {8,4,4,4,12};

static uint32_t clockSeq{3186784621};

const uint32_t clockSeqInc{3885574061};

static uint64_t toHex(std::string_view sv){
	uint64_t v{0};
	for(char c:sv){
		if('A'<=c&&c<='F')
			v|=(c-'A')+0xa;
		else if('a'<=c&&c<='f')
			v |=(c-'a')+0xa;
		else if('0'<=c&&c<='9')
			v |= c-'0';
		else
			throw std::invalid_argument(std::string(sv)+" is not a hexadecimal number");
		v <<=4;
	}
	return v;
}


LIBLCAPI UUID::UUID(std::string_view sv):high{toHex(sv.substr(0, 8))<<32|toHex(sv.substr(9,4))<<16|toHex(sv.substr(13,4))}
	,low{toHex(sv.substr(17,4))<<48|toHex(sv.substr(21,12))}{}

LIBLCAPI UUID::UUID(const char* str):UUID(std::string_view(str)){}

LIBLCAPI UUID UUID::fromString(std::string_view sv){
	return UUID{sv};
}


uint64_t randomVersion = 0x4000;
uint64_t versionMask = 0xf000;



LIBLCAPI ostream& operator<<(ostream& o,const UUID& id){
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

std::string fromHex(uint64_t u,int digits){
	std::string ret;
	for(;digits>0;digits--,u>>=4){
		uint8_t v = u&0xf;
		if(v<10)
			ret += '0'+v;
		else
			ret += 'A'+(v-0xa);
	}
	return std::move(ret);
}

LIBLCAPI string UUID::toString()const{
	std::string ret{};
	ret += fromHex(high>>32,8);
	ret += '-';
	ret += fromHex((high>>16)&0xffff,4);
	ret += '-';
	ret += fromHex(high&0xffff,4);
	ret += '-';
	ret += fromHex(low>>48,4);
	ret += '-';
	ret += fromHex(low&0xffffffffffff,12);
	return std::move(ret);
}

LIBLCAPI istream& operator>>(istream& i,UUID& id){
	std::string s;
	i >> s;
	id= UUID::fromString(s);
	return i;
}

std::string operator+(const std::string& s,const UUID& id){
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
LIBLCAPI UUID UUID::ofNow(){
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

LIBLCAPI UUID::UUID(current_timestamp_t):UUID(ofNow()){}

LIBLCAPI UUID::UUID(std::string_view sv,md5_namespace_t):UUID(uuidFromNamespace(sv)){}
LIBLCAPI UUID::UUID(std::string_view sv,sha1_namespace_t):UUID(uuidFromSHA1Namespace(sv)){}

LIBLCAPI UUID UUID::randomUUID(){
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

LIBLCAPI UUID UUID::uuidFromNamespace(std::string_view sv){
	unsigned char bytes[16];
	MD5(reinterpret_cast<const unsigned char*>(sv.data()),sv.length(),bytes);
	bytes[4] = (bytes[4]&0xf)|0x30;
	bytes[8] = (bytes[8]&0xcf)|0x80;
	uint64_t high = uint64_t(bytes[0])<<56|uint64_t(bytes[1])<<48|uint64_t(bytes[2])<<40|uint64_t(bytes[3])<<3
		  |uint64_t(bytes[4])<<24|uint64_t(bytes[5])<<16|uint64_t(bytes[6])<<8|uint64_t(bytes[7]);
	uint64_t low = uint64_t(bytes[8])<<56|uint64_t(bytes[9])<<48|uint64_t(bytes[10])<<40|uint64_t(bytes[11])<<3
		  |uint64_t(bytes[12])<<24|uint64_t(bytes[13])<<16|uint64_t(bytes[14])<<8|uint64_t(bytes[15]);
	return UUID{high,low};
}

LIBLCAPI UUID UUID::uuidFromSHA1Namespace(std::string_view sv){
	char bytes[20];
	MessageDigest<SHA<1,char>>{}(sv.data(),sv.length(),bytes);
	bytes[4] = (bytes[4]&0xf)|0x30;
	bytes[8] = (bytes[8]&0xcf)|0x80;
	uint64_t high = uint64_t(bytes[0])<<56|uint64_t(bytes[1])<<48|uint64_t(bytes[2])<<40|uint64_t(bytes[3])<<3
		  |uint64_t(bytes[4])<<24|uint64_t(bytes[5])<<16|uint64_t(bytes[6])<<8|uint64_t(bytes[7]);
	uint64_t low = uint64_t(bytes[8])<<56|uint64_t(bytes[9])<<48|uint64_t(bytes[10])<<40|uint64_t(bytes[11])<<3
		  |uint64_t(bytes[12])<<24|uint64_t(bytes[13])<<16|uint64_t(bytes[14])<<8|uint64_t(bytes[15]);
	return UUID{high,low};
}


