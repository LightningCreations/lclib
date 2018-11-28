/*
 * BigNumber.hpp
 *
 *  Created on: Nov 27, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_LCLIB_SECURITY_BIGNUMBER_HPP__2018_11_27_12_01_28
#define __INCLUDE_LCLIB_SECURITY_BIGNUMBER_HPP__2018_11_27_12_01_28

#include <lclib/TypeTraits.hpp>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <lclib/security/SecureAllocator.hpp>
#include <algorithm>
#include <limits>
#include <lclib/Random.hpp>
#include <lclib/Span.hpp>

namespace security{

	template<std::size_t N,typename byte=std::byte> struct BigNumber{
	private:
		byte block[N];
		using TopIntegralType = std::conditional_t<N==1,unsigned char,
				std::conditional_t<N==2,unsigned short,
				std::conditional_t<N<=4, unsigned,
				unsigned long long>>>;
		bool isMultiple2()const{
			return !(static_cast<unsigned char>(block[0])&0x1);
		}

		friend BigNumber devide(BigNumber& divisor,BigNumber dividend){
			BigNumber quotient{};
			while(divisor>dividend)
				divisor-=dividend,quotient++;
			return quotient;
		}


	public:
		static_assert(is_byte_v<byte>,"BigNumber requires Byte");
		using byte_type = byte;
		constexpr BigNumber() noexcept:block{}{}
		BigNumber(const byte(&b)[N]) noexcept:block{}{
			memcpy(block,b,N);
		}
		constexpr BigNumber(unsigned char c) noexcept:block{}{
			block[0] = static_cast<byte>(c);
		}
		~BigNumber(){
			memset(block,0,N);
		}
		template<std::enable_if_t<2<=N>* = 0> constexpr BigNumber(unsigned short s) noexcept:block{}{
			block[0] = static_cast<byte>(s&0xff);
			block[1] = static_cast<byte>((s>>8)&0xff);
		}
		template<std::enable_if_t<N<2>* = 0> constexpr explicit BigNumber(unsigned short s) noexcept:block{}{
			block[0] = static_cast<byte>(s&0xff);
		}
		template<std::enable_if_t<4<=N>* = 0> constexpr BigNumber(unsigned u) noexcept:block{}{
			block[0] = static_cast<byte>(u&0xff);
			block[1] = static_cast<byte>((u>>8)&0xff);
			block[2] = static_cast<byte>((u>>16)&0xff);
			block[3] = static_cast<byte>((u>>24)&0xff);
		}
		template<std::enable_if_t<N==3>* = 0> constexpr explicit BigNumber(unsigned u) noexcept:block{}{
			block[0] = static_cast<byte>(u&0xff);
			block[1] = static_cast<byte>((u>>8)&0xff);
			block[2] = static_cast<byte>((u>>16)&0xff);
		}
		template<std::enable_if_t<N<3>* = 0> constexpr explicit BigNumber(unsigned u) noexcept:BigNumber(static_cast<unsigned short>(u)){}


		template<std::enable_if_t<8<=N>* = 0> constexpr  BigNumber(unsigned long long l):block{}{
			block[0] = static_cast<byte>(l&0xff);
			block[1] = static_cast<byte>((l>>8)&0xff);
			block[2] = static_cast<byte>((l>>16)&0xff);
			block[3] = static_cast<byte>((l>>24)&0xff);
			block[4] = static_cast<byte>((l>>32)&0xff);
			block[5] = static_cast<byte>((l>>40)&0xff);
			block[6] = static_cast<byte>((l>>48)&0xff);
			block[7] = static_cast<byte>((l>>56)&0xff);
		}
		template<std::enable_if_t<N==7>* = 0> constexpr explicit BigNumber(unsigned long long l):block{}{
			block[0] = static_cast<byte>(l&0xff);
			block[1] = static_cast<byte>((l>>8)&0xff);
			block[2] = static_cast<byte>((l>>16)&0xff);
			block[3] = static_cast<byte>((l>>24)&0xff);
			block[4] = static_cast<byte>((l>>32)&0xff);
			block[5] = static_cast<byte>((l>>40)&0xff);
			block[6] = static_cast<byte>((l>>48)&0xff);
		}
		template<std::enable_if_t<N==6>* = 0> explicit BigNumber(unsigned long long l):block{}{
			block[0] = static_cast<byte>(l&0xff);
			block[1] = static_cast<byte>((l>>8)&0xff);
			block[2] = static_cast<byte>((l>>16)&0xff);
			block[3] = static_cast<byte>((l>>24)&0xff);
			block[4] = static_cast<byte>((l>>32)&0xff);
			block[5] = static_cast<byte>((l>>40)&0xff);
		}
		template<std::enable_if_t<N==5>* = 0> explicit BigNumber(unsigned long long l):block{}{
			block[0] = static_cast<byte>(l&0xff);
			block[1] = static_cast<byte>((l>>8)&0xff);
			block[2] = static_cast<byte>((l>>16)&0xff);
			block[3] = static_cast<byte>((l>>24)&0xff);
			block[4] = static_cast<byte>((l>>32)&0xff);
		}
		template<std::enable_if_t<N<=4>* = 0> explicit BigNumber(unsigned long long l):BigNumber(static_cast<unsigned>(l)){}

		explicit BigNumber(Random& r):block{}{
			r.nextBytes(block);
		}

		template<std::size_t N1,std::enable_if_t<(N1<N)>* = 0> BigNumber(const BigNumber<N1>& o){
			memcpy(block,o.block,N1);
		}
		template<std::size_t N1,std::enable_if_t<(N1>N)>* = 0> explicit BigNumber(const BigNumber<N1>& o){
			memcpy(block,o.block,N);
		}
		template<std::size_t N1> friend BigNumber<(N<N1)?N1:N,byte_type> operator+(const BigNumber& n1,const BigNumber<N1>& n2){
			const std::size_t K = (N<N1)?N:N1;
			const std::size_t K2 = (N<N1)?N1:N;
			BigNumber<(N<N1)?N1:N,byte_type> r{};
			volatile bool carry{};
			for(std::size_t i = 0;i<K;i++){
				volatile unsigned short s = static_cast<unsigned char>(n1.block[i]);
				s += static_cast<unsigned char>(n2.block[i]);
				if(carry)
					s++;
				carry = s&0x100;
				r.block[i] = static_cast<byte>(s&0xff);
			}
			for(std::size_t i = K;i<K2;i++){
				volatile unsigned short s{};
				s += static_cast<unsigned char>((N<N1)?n2.block[i]:n1.block[i]);
				if(carry)
					s++;
				carry = s&0x100;
				r.block[i] = static_cast<byte>(s&0xff);
			}
			return r;
		}

		template<std::size_t N1> friend BigNumber<(N<N1)?N1:N,byte_type> operator-(const BigNumber& n1,const BigNumber<N1>& n2){
			const std::size_t K = (N<N1)?N:N1;
			const std::size_t K2 = (N<N1)?N1:N;
			BigNumber<(N<N1)?N1:N,byte_type> r{};
			volatile bool carry{};
			for(std::size_t i = 0;i<K;i++){
				volatile unsigned short s = static_cast<unsigned char>(n1.block[i]);
				s -= static_cast<unsigned char>(n2.block[i]);
				if(carry)
					s--;
				carry = s&0x100;
				r.block[i] = static_cast<byte>(s&0xff);
			}
			for(std::size_t i = K;i<K2;i++){
				volatile unsigned short s{(N<N1)?0:static_cast<unsigned char>(n1.block[i])};
				s -= (N<N1)?static_cast<unsigned char>(n2.block[i]):0;
				if(carry)
					s--;
				carry = s&0x100;
				r.block[i] = static_cast<byte>(s&0xff);
			}
			return r;
		}
		friend bool operator<(const BigNumber& n1,const BigNumber& n2){
			return std::lexicographical_compare(n1.block,n1.block+N,n2.block,n2.block+N);
		}
		friend bool operator==(const BigNumber& n1,const BigNumber& n2){
			return std::equal_to(n1.block,n1.block+N,n2.block,n2.block+N);
		}
		template<std::size_t N1> friend bool operator==(const BigNumber& n1,const BigNumber<N1,byte>& n2){
			if constexpr(N<N1)
				return std::equal_to(n1.block,n1.block+N,n2.block,n2.block+N)&&std::all_of(n2.block+N, n2.block+N1, [](auto a){return a==static_cast<decltype(a)>(0);});
			else
				return n2==n1;
		}

		friend bool operator!=(const BigNumber& n1,const BigNumber& n2){
			return !(n1==n2);
		}
		template<std::size_t N1> friend bool operator!=(const BigNumber& n1,const BigNumber<N1,byte>& n2){
			return !(n1==n2);
		}
		friend bool operator>(const BigNumber& n1,const BigNumber& n2){
			return std::lexicographical_compare(n1.block,n1.block+N,n2.block,n2.block+N,std::greater{});
		}
		friend bool operator<=(const BigNumber& n1,const BigNumber& n2){
			return (n1<n2)||(n1==n2);
		}
		friend bool operator>=(const BigNumber& n1,const BigNumber& n2){
			return (n1>n2)||(n1==n2);
		}

		template<std::size_t N1> friend bool operator<(const BigNumber& n1,const BigNumber<N1,byte>& n2){
			if constexpr(N<N1)
				return std::lexicographical_compare(n1.block,n1.block+N,n2.block,n2.block+N);
			else
				return !(n2<n1)&&!(n2==n1);
		}
		template<std::size_t N1> friend bool operator<=(const BigNumber& n1,const BigNumber<N1,byte>& n2){
			return (n1<n2)||(n1==n2);
		}
		template<std::size_t N1> friend bool operator>(const BigNumber& n1,const BigNumber<N1,byte>& n2){
			if constexpr(N1<N)
				return std::lexicographical_compare(n1.block,n1.block+N1,n2.block,n2.block+N1,std::greater{});
			else
				return !(n1<=n2);
		}
		template<std::size_t N1> friend bool operator>=(const BigNumber& n1,const BigNumber<N1,byte>& n2){
			return (n2<=n1);
		}

		friend bool operator==(const BigNumber& n1,unsigned long long l){
			return n1==BigNumber<8,byte>(l);
		}
		friend bool operator!=(const BigNumber& n1,unsigned long long l){
			return n1!=BigNumber<8,byte>(l);
		}
		friend bool operator<(const BigNumber& n1,unsigned long long l){
			return n1<static_cast<BigNumber<8,byte>>(l);
		}
		friend bool operator>(const BigNumber& n1,unsigned long long l){
			return n1>static_cast<BigNumber<8,byte>>(l);
		}
		friend bool operator<=(const BigNumber& n1,unsigned long long l){
			return n1<=static_cast<BigNumber<8,byte>>(l);
		}
		friend bool operator>=(const BigNumber& n1,unsigned long long l){
			return n1>=static_cast<BigNumber<8,byte>>(l);
		}
		friend bool operator==(unsigned long long l,const BigNumber& n1){
			return n1==l;
		}
		friend bool operator!=(unsigned long long l,const BigNumber& n1){
			return n1!=l;
		}
		friend bool operator<(unsigned long long l,const BigNumber& n1){
			return n1>l;
		}
		friend bool operator>(unsigned long long l,const BigNumber& n1){
			return n1<l;
		}
		friend bool operator<=(unsigned long long l,const BigNumber& n1){
			return n1>=l;
		}
		friend bool operator>=(unsigned long long l,const BigNumber& n1){
			return n1<=l;
		}
		explicit operator bool()const{
			return !std::any_of(block,block+N,[](auto a){ return static_cast<unsigned char>(a)==0;});
		}


		template<std::size_t N1> BigNumber& operator+=(const BigNumber<N1,byte>& n){
			return (*this = BigNumber{(*this)+n});
		}
		template<std::size_t N1> BigNumber& operator-=(const BigNumber<N1,byte>& n){
			return (*this = BigNumber{(*this)+n});
		}
		template<std::size_t N1> friend BigNumber<N<N1?N1:N,byte> operator*(const BigNumber& b1,const BigNumber<N1,byte>& b){
			BigNumber<N<N1?N1:N,byte> ret{b1};
			if(!b)
				return 0;
			while(b){
				if(!b.isMultipleTwo())
			}
			return b1;
		}

		friend BigNumber operator/(BigNumber b1,const BigNumber& b2){
			return divide(b1,b2);
		}
		BigNumber& operator/=(const BigNumber& b2){
			return (*this = devide(*this,b2));
		}
		BigNumber& operator %=(const BigNumber& b2){
			devide(*this,b2);
			return *this;
		}
		friend BigNumber operator%(BigNumber b1,const BigNumber& b2){
			devide(b1,b2);
			return b1;
		}
		template<std::size_t N1> friend BigNumber<N<N1?N1:N> operator/(const BigNumber& b1,const BigNumber<N1,byte>& b2){
			BigNumber<N<N1?N1:N> b{b1};
			return devide(b,b2);
		}
		template<std::size_t N1> friend BigNumber<N<N1?N1:N> operator%(const BigNumber& b1,const BigNumber<N1,byte>& b2){
			BigNumber<N<N1?N1:N> b{b1};
			devide(b,b2);
			return b;
		}

		BigNumber powMod(BigNumber power,const BigNumber& rem)const{
			BigNumber ret{*this%rem};
			while(power!=0){
				if(!power.isMultiple2())
					ret = (ret * *this)%rem,power-=1;
				else{
					ret = (ret*ret)%rem;
					power/=2;
				}
			}
		}
		template<std::size_t N1,std::size_t N2> friend BigNumber<N1> powMod(const BigNumber& base,BigNumber<N2,byte> power,const BigNumber<N1>& mod){
			BigNumber<N1,byte> ret{base%mod};
			while(power!=0){
				if(!power.isMultiple2())
					ret = (ret * base)%mod,power-=1;
				else{
					ret = (ret*ret)%mod;
					power/=2;
				}
			}
			return ret;
		}
		span<const byte,N> getBytes()const{
			return span{block};
		}
		//Hashcode of BigNumber based on CRC-32
		std::size_t hash_code()const{
			static const std::size_t crcTable[256] = {
			   0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
			   0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
			   0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
			   0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
			   0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
			   0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
			   0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
			   0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
			   0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
			   0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
			   0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
			   0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
			   0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
			   0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
			   0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
			   0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
			   0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
			   0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
			   0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
			   0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
			   0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
			   0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
			   0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
			   0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
			   0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
			   0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
			   0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
			   0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
			   0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
			   0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
			   0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
			   0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
			   0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
			   0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
			   0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
			   0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
			   0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D };
			std::size_t crc32{0xffffffff};
			for(byte b:block){
				std::size_t idx{(crc32^static_cast<unsigned char>(b))&0xff};
				crc32 = (crc32<<8)^crcTable[idx];
			}
			return crc32;
		}
	};

	bool isPrime(const BigNumber<256>&);
	template<std::size_t N,typename byte,typename=std::enable_if_t<N<=256>> BigNumber<N,byte> findPrime(BigNumber<N,byte> bn){
		if(isPrime(bn))
			bn+=1;
		while(!isPrime(bn))
			bn = 2*bn+1;
		return bn;
	}

	explicit BigNumber() -> BigNumber<256>;
	template<typename byte,std::size_t N> BigNumber(const byte(&arr)[N]) -> BigNumber<N,byte>;
	template<typename Integral> BigNumber(Integral) -> BigNumber<256>;
	BigNumber(Random&)->BigNumber<256>;

	namespace big_integer_literals{
		BigNumber<256> operator ""_bn(const char*);
		BigNumber<256> operator ""_bn(const char*,std::size_t);
	}
}

namespace std{
	template<std::size_t N1,std::size_t N2,typename byte> struct common_type<security::BigNumber<N1,byte>,security::BigNumber<N2,byte>>{
		typedef security::BigNumber<(N1<N2)?N2:N1,byte> type;
	};

	template<std::size_t N,typename byte> struct numeric_limits<security::BigNumber<N,byte>>{
	private:
		using BNType = security::BigNumber<N,byte>;
	public:
		constexpr inline static bool is_specialized{true};
		constexpr inline static bool is_exact{true};
		constexpr inline static bool is_signed{false};
		constexpr inline static bool is_integer{true};
		constexpr inline static bool has_infinity{false};
		constexpr inline static bool has_quiet_NaN{false};
		constexpr inline static bool has_signalling_NaN{false};
		constexpr inline static std::float_denorm_style has_denorm{std::denorm_absent};
		constexpr inline static bool has_denorm_loss{false};
		constexpr inline static std::float_round_style round_style{std::round_toward_zero};
		constexpr inline static bool is_iec559{false};
		constexpr inline static bool is_bounded{true};
		constexpr inline static bool is_modulo{true};
		constexpr inline static int digits{N*CHAR_BIT};
		constexpr inline static int digits10{static_cast<int>(digits*0.30102999566)};
		constexpr inline static int max_digits10{};
		constexpr inline static int radix{2};
		constexpr inline static int min_exponent{};
		constexpr inline static int max_exponent{};
		constexpr inline static int min_exponent10{};
		constexpr inline static int max_exponent10{};
		constexpr inline static bool traps{true};
		constexpr inline static bool tinyness_before{false};
		static inline BNType min(){
			return BNType{};
		}
		static inline BNType max(){
			return BNType{}-1;
		}
		static inline BNType lowest(){
			return BNType{};
		}
		static inline BNType roundError(){
			return BNType{};
		}
		static inline BNType infinity(){
			return BNType{};
		}
		static inline BNType quiet_NaN(){
			return BNType{};
		}
		static inline BNType signalling_NaN(){
			return BNType{};
		}
		static inline BNType denorm_min(){
			return BNType{};
		}
		static inline BNType epsilon(){
			return BNType{};
		}
	};
	template<std::size_t N,typename Byte> struct hash<security::BigNumber<N,Byte>>{
	public:
		constexpr hash()=default;
		std::size_t operator()(const security::BigNumber<N,Byte>& bn)const{
			return bn.hash_code();
		}
	};
}




#endif /* __INCLUDE_LCLIB_SECURITY_BIGNUMBER_HPP__2018_11_27_12_01_28 */
