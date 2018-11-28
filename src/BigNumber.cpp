/*
 * BigNumber.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: chorm
 */
#include <set>
#include <lclib/security/BigNumber.hpp>
#include <algorithm>
#include <functional>
#include <mutex>

using security::BigNumber;

using namespace security::big_integer_literals;


BigNumber<256> operator ""_bn(const char* c,std::size_t n){
	const char* end = c+n;
	BigNumber<256> ret{};
	while(c!=end){
		ret *= 10;
		ret += (*c)-'0';
	}
	return ret;
}

BigNumber<256> operator ""_bn(const char* c){
	return operator""_bn(c,strlen(c));
}

BigNumber<256> operator ""_bn(unsigned long long l){
	return l;
}



static void initPrimeTable(){
	BigNumber<256> n{2};
	for(unsigned i = 0;i<100;i++){
		n *= 2;
		isPrime(n+1);
	}
}



bool isPrime(const BigNumber<256>& bn){
	static std::set<BigNumber<256>> primes{2_bn, 3_bn, 5_bn, 7_bn, 11_bn, 13_bn, 17_bn, 19_bn, 23_bn, 29_bn, 31_bn, 37_bn, 41_bn, 43_bn,
		47_bn, 53_bn, 59_bn, 61_bn, 67_bn, 71_bn, 73_bn, 79_bn, 83_bn, 89_bn, 97_bn, 101_bn, 103_bn, 107_bn, 109_bn, 113_bn, 127_bn, 131_bn, 137_bn};
	static bool has_init{};
	static std::recursive_mutex mtx;
	const static BigNumber<256> pprod1{2*3*5*7*11*13*17*19*23*29*31*37*41*43*47*53*59*61*67*71};
	const static BigNumber<256> pprod2{2*5*11*13*19*29*37*47*59*67*157*163*167*173*179*181*191*193*197*199*211*223*227};
	const static BigNumber<256> pprod3{4979798797613481409_bn*3457666143637005227_bn};
	std::lock_guard lock{mtx};
	if(!has_init){
		has_init = true;
		initPrimeTable();
	}
	const BigNumber<256> m1{bn-1};
	if(primes.count(bn)!=0)
		return true;
	else for(const auto& a:primes){
		const BigNumber<256> s{a*a};
		if(s>bn)
			break;
		else if(bn%a==0)
			return false;
		else if(a.powMod(m1,bn)!=1)
			return false;
		else if(s.powMod(m1,bn)!=1)
			return false;
	}
	if(pprod1.powMod(m1,bn)!=1)
		return false;
	else if(pprod2.powMod(m1,bn)!=1)
		return false;
	else if(pprod3.powMod(m1,bn)!=1)
		return false;
	primes.insert(bn);
	return true;
}



