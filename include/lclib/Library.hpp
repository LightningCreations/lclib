/*
 * Library.hpp
 *
 *  Created on: Oct 16, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_LIBRARY_HPP__2018_10_16_08_58_59
#define __INCLUDE_LCLIB_CXX_LIBRARY_HPP__2018_10_16_08_58_59

#include <string>
#include <lclib/Version.hpp>

struct LIBLCAPI lclib_cxx{
	static const char name[];
	static const char copyright[];
	static const char license[];
	static const Version ver;
	static const uint16_t patch;
	static const char releaseDate[];
	static const char releaseTime[];
	static const char providerName[];
	static const char providerPublic[];
};


#endif /* __INCLUDE_LCLIB_CXX_LIBRARY_HPP__2018_10_16_08_58_59 */
