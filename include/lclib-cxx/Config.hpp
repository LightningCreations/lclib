/*
 * Config.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CONFIG_HPP__2018_09_06_16_35
#define _INCLUDE_CONFIG_HPP__2018_09_06_16_35

#if __cplusplus < 201703L
#error "This library requires C++17 Mode to function"
#endif
#if __cplusplus >201703L
#define LCLIBCXX_CXX2a
#endif
//If a User Configuration file is provided, and we are not Building the Library,
//Include that file first
#ifndef LCLIB_CXX_DEFINITION
#if __has_include(<UserConfig.hpp>)
#include <UserConfig.hpp>
#endif
#endif

//Then if a build configuration file has been generated, include that file
#if __has_include(<BuildConfig.hpp>)
#include <BuildConfig.hpp>
#endif

#define LCLIBEXPORT [[visibility("default"),dllexport]]
#define LCLIBIMPORT [[dllimport]]

#ifdef LCLIB_CXX_DEFINITION
#define LIBLCAPI LCLIBEXPORT
#else
#define LIBLCAPI LCLIBIMPORT
#endif
#define LIBLCFUNC LIBLCAPI
#define LIBLCHIDE [[visibility("hidden")]]

#ifdef MINIMAL
#define __NOREFLECTION
#define __NOCONTRACTS
#define __NOASSERTIONS
#endif





#define __CONCAT2A(a,b) a##b
#define __CONCAT3A(a,b,c) a##b##c
#define __CONCAT4A(a,b,c,d) a##b##c##d

#define CONCAT2(a,b) __CONCAT2A(a,b)
#define CONCAT3(a,b,c) __CONCAT3A(a,b,c)
#define CONCAT4(a,b,c,d) __CONCAT4A(a,b,c,d)

#define STRINGIFY(...) #__VA_ARGS__

#ifndef __UNIQUE__
//Unique Id's are used by reflection (though are also availble to users)
#if defined(__OAPI_PRAGMA_MACROS)&&defined(__HAS_IMPORT_UNIQUE)
#pragma macros import("__UNIQUE__") define("__UNIQUE__")
//On SNES-OS __UNIQUE__ expands to UID<filename hash><line number hash><Translation Time Random Token>QJJ
//__UNIQUE__() expands to the same
#else
#define __UNIQUE__() CONCAT4(UID00AAKKZ09,__COUNTER__,__LINE__,QJJ)
#endif
#endif

#ifndef UNIQUEID
#define UNIQUEID(prefix) CONCAT3(prefix,__,__UNIQUE__())
#endif



//Replace C assertion with a Throwing one. Disabled if __NOASSERTIONS or MINIMAL is defined
#ifndef __NOASSERTIONS
#ifndef ASSERT
#define ASSERTION_MESSAGE(...) "Assertion Failure: " STRINGIFY(__VA_ARGS__)"@" __FILE__ STRINGIFY(__LINE__) ":"
#define ASSERT(condition,...) if(!(condition))throw __DETAIL_ASSERTION_MESSAGE(__VA_ARGS__);
#endif
#else
#define ASSERT(conditon,message)
#endif



//Enable Contracts for catching Some UB If they are available.
//Disabled if __NOCONTRACTS or MINIMAL is defined
#if __has_cpp_attribute(assert)&& !defined(__NOCONTRACTS)
#define CONTRACTASSERT(condition) [[assert:condition]]
#else
#define CONTRACTASSERT(condition)
#endif
#if __has_cpp_attribute(expects)&& !defined(__NOCONTRACTS)
#define EXPECTS(condition) [[expects:condition]]
#else
#define EXPECTS(condition)
#endif
#if __has_cpp_attribute(ensures)&& !defined(__NOCONTRACTS)
#define ENSURES(condition) [[ensures:condition]]
#else
#define ENSURES(condition)
#endif

#if __has_include(<concepts>)&&(__cplusplus>201803L)
#include <concepts>
#if defined(__cpp_lib_concepts) &&(__cpp_lib_concepts>=201806L)
#define LIBLCCXX_HAS_CONCEPTS
#endif
#endif

#if defined(LIBLCCXX_ERROR_INCLUDE_WARNINGS)
#define INCLUDE_WARNING(...) static_assert(false,STRINGIFY(Including __FILE__ is not enabled: __VA_ARGS__))
#else
#define INCLUDE_WARNING(...) _Pragma(message STRINGIFY(Including __FILE__ is not enabled: __VA_ARGS__))
#endif
#endif /* INCLUDE_CONFIG_HPP_ */
