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
}




#endif /* __INCLUDE_LCLIB_SECURITY_BIGNUMBER_HPP__2018_11_27_12_01_28 */
