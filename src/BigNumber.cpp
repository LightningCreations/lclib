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




bool isPrime(const BigNumber<256>& bn){
	static std::set<BigNumber<256>> primes{2_bn, 3_bn, 5_bn, 7_bn, 11_bn, 13_bn, 17_bn, 19_bn, 23_bn, 29_bn, 31_bn, 37_bn, 41_bn, 43_bn,
		47_bn, 53_bn, 59_bn, 61_bn, 67_bn, 71_bn, 73_bn, 79_bn, 83_bn, 89_bn, 97_bn, 101_bn, 103_bn, 107_bn, 109_bn, 113_bn, 127_bn, 131_bn, 137_bn,
		139_bn, 149_bn, 151_bn, 157_bn, 163_bn, 167_bn, 173_bn, 179_bn, 181_bn, 191_bn, 193_bn, 197_bn, 199_bn, 211_bn, 223_bn, 227_bn, 229_bn,
		233_bn, 239_bn, 241_bn, 251_bn, 257_bn, 263_bn, 269_bn, 271_bn, 277_bn, 281_bn, 283_bn, 293_bn, 307_bn, 311_bn, 313_bn, 317_bn, 331_bn,
		337_bn, 347_bn, 349_bn, 353_bn, 359_bn, 367_bn, 373_bn, 379_bn, 383_bn, 389_bn, 397_bn, 401_bn, 409_bn, 419_bn, 421_bn, 431_bn, 433_bn,
		439_bn, 443_bn, 449_bn, 457_bn, 461_bn, 463_bn, 467_bn, 479_bn, 487_bn, 491_bn, 499_bn, 503_bn, 509_bn, 521_bn, 523_bn, 541_bn, 547_bn,
		557_bn, 563_bn, 569_bn, 571_bn, 577_bn, 587_bn, 593_bn, 599_bn, 601_bn, 607_bn, 613_bn, 617_bn, 619_bn, 631_bn, 641_bn, 643_bn, 647_bn,
		653_bn, 659_bn, 661_bn, 673_bn, 677_bn, 683_bn, 691_bn, 701_bn, 709_bn, 719_bn, 727_bn, 733_bn, 739_bn, 743_bn, 751_bn, 757_bn, 761_bn,
		769_bn, 773_bn, 787_bn, 797_bn, 809_bn, 811_bn, 821_bn, 823_bn, 827_bn, 829_bn, 839_bn, 853_bn, 857_bn, 859_bn, 863_bn, 877_bn, 881_bn,
		883_bn, 887_bn, 907_bn, 911_bn, 919_bn, 929_bn, 937_bn, 941_bn, 947_bn, 953_bn, 967_bn, 971_bn, 977_bn, 983_bn, 991_bn, 997_bn, 1009_bn,
		1013_bn, 1019_bn, 1021_bn, 1031_bn, 1033_bn, 1039_bn, 1049_bn, 1051_bn, 1061_bn, 1063_bn, 1069_bn, 1087_bn, 1091_bn, 1093_bn,
		1097_bn, 1103_bn, 1109_bn, 1117_bn, 1123_bn, 1129_bn, 1151_bn, 1153_bn, 1163_bn, 1171_bn, 1181_bn, 1187_bn, 1193_bn, 1201_bn,
		1213_bn, 1217_bn, 1223_bn, 1229_bn, 1231_bn, 1237_bn, 1249_bn, 1259_bn, 1277_bn, 1279_bn, 1283_bn, 1289_bn, 1291_bn, 1297_bn,
		1301_bn, 1303_bn, 1307_bn, 1319_bn, 1321_bn, 1327_bn, 1361_bn, 1367_bn, 1373_bn, 1381_bn, 1399_bn, 1409_bn, 1423_bn, 1427_bn,
		1429_bn, 1433_bn, 1439_bn, 1447_bn, 1451_bn, 1453_bn, 1459_bn, 1471_bn, 1481_bn, 1483_bn, 1487_bn, 1489_bn, 1493_bn, 1499_bn,
		1511_bn, 1523_bn, 1531_bn, 1543_bn, 1549_bn, 1553_bn, 1559_bn, 1567_bn, 1571_bn, 1579_bn, 1583_bn, 1597_bn, 1601_bn, 1607_bn,
		1609_bn, 1613_bn, 1619_bn, 1621_bn, 1627_bn, 1637_bn, 1657_bn, 1663_bn, 1667_bn, 1669_bn, 1693_bn, 1697_bn, 1699_bn, 1709_bn,
		1721_bn, 1723_bn, 1733_bn, 1741_bn, 1747_bn, 1753_bn, 1759_bn, 1777_bn, 1783_bn, 1787_bn, 1789_bn, 1801_bn, 1811_bn, 1823_bn,
		1831_bn, 1847_bn, 1861_bn, 1867_bn, 1871_bn, 1873_bn, 1877_bn, 1879_bn, 1889_bn, 1901_bn, 1907_bn, 1913_bn, 1931_bn, 1933_bn,
		1949_bn, 1951_bn, 1973_bn, 1979_bn, 1987_bn, 1993_bn, 1997_bn, 1999_bn, 2003_bn, 2011_bn, 2017_bn, 2027_bn, 2029_bn, 2039_bn,
		2053_bn, 2063_bn, 2069_bn, 2081_bn, 2083_bn, 2087_bn, 2089_bn, 2099_bn, 2111_bn, 2113_bn, 2129_bn, 2131_bn, 2137_bn, 2141_bn,
		2143_bn, 2153_bn, 2161_bn, 2179_bn, 2203_bn, 2207_bn, 2213_bn, 2221_bn, 2237_bn, 2239_bn, 2243_bn, 2251_bn, 2267_bn, 2269_bn,
		2273_bn, 2281_bn, 2287_bn, 2293_bn, 2297_bn, 2309_bn, 2311_bn, 2333_bn, 2339_bn, 2341_bn, 2347_bn, 2351_bn, 2357_bn, 2371_bn,
		2377_bn, 2381_bn, 2383_bn, 2389_bn, 2393_bn, 2399_bn, 2411_bn, 2417_bn, 2423_bn, 2437_bn, 2441_bn, 2447_bn, 2459_bn, 2467_bn,
		2473_bn, 2477_bn, 2503_bn, 2521_bn, 2531_bn, 2539_bn, 2543_bn, 2549_bn, 2551_bn, 2557_bn, 2579_bn, 2591_bn, 2593_bn, 2609_bn,
		2617_bn, 2621_bn, 2633_bn, 2647_bn, 2657_bn, 2659_bn, 2663_bn, 2671_bn, 2677_bn, 2683_bn, 2687_bn, 2689_bn, 2693_bn, 2699_bn,
		2707_bn, 2711_bn, 2713_bn, 2719_bn, 2729_bn, 2731_bn, 2741_bn, 2749_bn, 2753_bn, 2767_bn, 2777_bn, 2789_bn, 2791_bn, 2797_bn,
		2801_bn, 2803_bn, 2819_bn, 2833_bn, 2837_bn, 2843_bn, 2851_bn, 2857_bn, 2861_bn, 2879_bn, 2887_bn, 2897_bn, 2903_bn, 2909_bn,
		2917_bn, 2927_bn, 2939_bn, 2953_bn, 2957_bn, 2963_bn, 2969_bn, 2971_bn, 2999_bn, 3001_bn, 3011_bn, 3019_bn, 3023_bn, 3037_bn,
		3041_bn, 3049_bn, 3061_bn, 3067_bn, 3079_bn, 3083_bn, 3089_bn, 3109_bn, 3119_bn, 3121_bn, 3137_bn, 3163_bn, 3167_bn, 3169_bn,
		3181_bn, 3187_bn, 3191_bn, 3203_bn, 3209_bn, 3217_bn, 3221_bn, 3229_bn, 3251_bn, 3253_bn, 3257_bn, 3259_bn, 3271_bn, 3299_bn,
		3301_bn, 3307_bn, 3313_bn, 3319_bn, 3323_bn, 3329_bn, 3331_bn, 3343_bn, 3347_bn, 3359_bn, 3361_bn, 3371_bn, 3373_bn, 3389_bn,
		3391_bn, 3407_bn, 3413_bn, 3433_bn, 3449_bn, 3457_bn, 3461_bn, 3463_bn, 3467_bn, 3469_bn, 3491_bn, 3499_bn, 3511_bn, 3517_bn,
		3527_bn, 3529_bn, 3533_bn, 3539_bn, 3541_bn, 3547_bn, 3557_bn, 3559_bn, 3571_bn, 3581_bn, 3583_bn, 3593_bn, 3607_bn, 3613_bn,
		3617_bn, 3623_bn, 3631_bn, 3637_bn, 3643_bn, 3659_bn, 3671_bn, 3673_bn, 3677_bn, 3691_bn, 3697_bn, 3701_bn, 3709_bn, 3719_bn,
		3727_bn, 3733_bn, 3739_bn, 3761_bn, 3767_bn, 3769_bn, 3779_bn, 3793_bn, 3797_bn, 3803_bn, 3821_bn, 3823_bn, 3833_bn, 3847_bn,
		3851_bn, 3853_bn, 3863_bn, 3877_bn, 3881_bn, 3889_bn, 3907_bn, 3911_bn, 3917_bn, 3919_bn, 3923_bn, 3929_bn, 3931_bn, 3943_bn,
		3947_bn, 3967_bn, 3989_bn, 4001_bn, 4003_bn, 4007_bn, 4013_bn, 4019_bn, 4021_bn, 4027_bn, 4049_bn, 4051_bn, 4057_bn, 4073_bn,
		4079_bn, 4091_bn, 4093_bn, 4099_bn, 4111_bn, 4127_bn, 4129_bn, 4133_bn, 4139_bn, 4153_bn, 4157_bn, 4159_bn, 4177_bn, 4201_bn,
		4211_bn, 4217_bn, 4219_bn, 4229_bn, 4231_bn, 4241_bn, 4243_bn, 4253_bn, 4259_bn, 4261_bn, 4271_bn, 4273_bn, 4283_bn, 4289_bn,
		4297_bn, 4327_bn, 4337_bn, 4339_bn, 4349_bn, 4357_bn, 4363_bn, 4373_bn, 4391_bn, 4397_bn, 4409_bn, 4421_bn, 4423_bn, 4441_bn,
		4447_bn, 4451_bn, 4457_bn, 4463_bn, 4481_bn, 4483_bn, 4493_bn, 4507_bn, 4513_bn, 4517_bn, 4519_bn, 4523_bn, 4547_bn, 4549_bn,
		4561_bn, 4567_bn, 4583_bn, 4591_bn, 4597_bn, 4603_bn, 4621_bn, 4637_bn, 4639_bn, 4643_bn, 4649_bn, 4651_bn, 4657_bn, 4663_bn,
		4673_bn, 4679_bn, 4691_bn, 4703_bn, 4721_bn, 4723_bn, 4729_bn, 4733_bn, 4751_bn, 4759_bn, 4783_bn, 4787_bn, 4789_bn, 4793_bn,
		4799_bn, 4801_bn, 4813_bn, 4817_bn, 4831_bn, 4861_bn, 4871_bn, 4877_bn, 4889_bn, 4903_bn, 4909_bn, 4919_bn, 4931_bn, 4933_bn,
		4937_bn, 4943_bn, 4951_bn, 4957_bn, 4967_bn, 4969_bn, 4973_bn, 4987_bn, 4993_bn, 4999_bn, 5003_bn, 5009_bn, 5011_bn, 5021_bn,
		5023_bn, 5039_bn, 5051_bn, 5059_bn, 5077_bn, 5081_bn, 5087_bn, 5099_bn, 5101_bn, 5107_bn, 5113_bn, 5119_bn, 5147_bn, 5153_bn,
		5167_bn, 5171_bn, 5179_bn, 5189_bn, 5197_bn, 5209_bn, 5227_bn, 5231_bn, 5233_bn, 5237_bn, 5261_bn, 5273_bn, 5279_bn, 5281};
	static std::recursive_mutex mtx;
	const static BigNumber<256> pprod1{2*3*5*7*11*13*17*19*23*29*31*37*41*43*47*53*59*61*67*71};
	const static BigNumber<256> pprod2{2*5*11*13*19*29*37*47*59*67*157*163*167*173*179*181*191*193*197*199*211*223*227};
	const static BigNumber<256> pprod3{4979798797613481409_bn*3457666143637005227_bn};
	std::lock_guard lock{mtx};
	const BigNumber<256> m1{bn-1};
	if(primes.count(bn)!=0)
		return true;
	else for(const auto& a:primes){
		const BigNumber<256> s{a*a};
		if(s>bn)
			return primes.insert(bn),true;
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

	//Do some more Tests,

	primes.insert(bn);
	return true;
}



