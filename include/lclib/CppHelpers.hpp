/*
 * CppHelpers.hpp
 *
 *  Created on: Sep 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_CPPHELPERS_HPP__2018_09_10_14_04
#define __INCLUDE_CPPHELPERS_HPP__2018_09_10_14_04
#include <cstddef>
#include <utility>
#include <lclib/Config.hpp>
namespace detail{
	const char directory[]{"ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
	template<std::size_t N> constexpr char nameHashChar(std::size_t& hash,const char(&str)[N],std::size_t n){
		hash *= 1027467391;
		hash += N;
		hash >>=1;
		hash += n%2?3144663959:1572674771;
		hash *= 1027467391;
		hash += n;
		for(std::size_t q=0;q<(N-1);q++)
			hash*=1027467391,hash+=~size_t{q},hash+=str[n];

		return directory[hash%(sizeof(directory)-1)];
	}

}


template<std::size_t N> constexpr const char* nameHash(const char(&str)[N],char out[]){
	std::size_t prev = 2846782441;
	for(std::size_t q=0;q<24;q++)
		out[q] = detail::nameHashChar(prev,str,q);
	out[24] = 0;
	return out;
}



#endif /* __INCLUDE_CPPHELPERS_HPP__2018_09_10_14_04 */
