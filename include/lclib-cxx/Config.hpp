/*
 * Config.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CONFIG_HPP__2018_09_06_16_35
#define _INCLUDE_CONFIG_HPP__2018_09_06_16_35

#if __cplusplus < 201703L
#error This library requires C++17 Mode to function (Compile with -std=c++1z, -std=c++17, or -std=c++2a)
#endif
#if __cplusplus >201703L
#define LCLIBCXX_CXX2a
#endif
//If a User Configuration file is provided, and we are not Building the Library,
//Include that file first
#ifndef LCLIB_CXX_DEFINITION
#if __has_include(<configure/UserConfig.hpp>)
#include <configure/UserConfig.hpp>
#endif
#endif

//Then if a build configuration file is provided, include that file
#if __has_include(<configure/BuildConfig.hpp>)
#include <configure/BuildConfig.hpp>
#endif

#ifndef LCLIBEXPORT
#ifdef _WIN32
#define LCLIBEXPORT __declspec(dllexport)
#else
#define LCLIBEXPORT __attribute__((visibility("default")))
#endif
#endif
#ifndef LCLIBIMPORT
#ifdef _WIN32
#define LCLIBIMPORT __declspec(dllimport)
#else
#define LCLIBIMPORT
#endif
#endif
#ifndef LIBLCAPI
#ifdef LCLIB_CXX_DEFINITION
#define LIBLCAPI LCLIBEXPORT
#else
#define LIBLCAPI LCLIBIMPORT
#endif
#endif
#define LIBLCFUNC LIBLCAPI
#ifndef LIBLCHIDE
#ifndef _WIN32
#define LIBLCHIDE __attribute__((visibility("internal")))
#else
#define LIBLCHIDE
#endif
#endif

#ifdef MINIMAL
//Minimal Implementation (Disables Reflection, Contracts, Assertions, and CLI Extensions
#define __NOREFLECTION
#define __NOCONTRACTS
#define __NOCLIEXTENSIONS
#endif


#if defined(_WIN32)&&!defined(__NOCLIEXTENSIONS)
#define __CXX_USE_CLI_EXTENSIONS //Use CLI Extensions if availble.
#endif

#if defined(__CXX_USE_CLI_EXTENSIONS)&&defined(__cplusplus_cli)
#define LCLIBCXX_HAS_CLI
#endif

#define __EVAL0(m) m
#define __EVAL(m) __EVAL0(m)

#define __CONCAT2A(a,b) a##b
#define __CONCAT3A(a,b,c) a##b##c
#define __CONCAT4A(a,b,c,d) a##b##c##d

#define CONCAT2(a,b) __CONCAT2A(a,b)
#define CONCAT3(a,b,c) __CONCAT3A(a,b,c)
#define CONCAT4(a,b,c,d) __CONCAT4A(a,b,c,d)

#define STRINGIFY(...) #__VA_ARGS__

#ifndef __UNIQUE__
//Unique Id's are used by reflection (though are also availble to users)
#if defined(__OAPI_ENHANCED_CPP)&&defined(__HAS_IMPORT_UNIQUE)
#pragma macros import("__UNIQUE__") define("__UNIQUE__")
//On SNES-OS __UNIQUE__ expands to UID<filename hash><line number><in file counter><Translation Time Random Token>QJJ
//__UNIQUE__() expands to the same, except that in a Function-like macro, it expands recursively.
#define __OAPI_USE_SHARED_COUNTER
#else
#if defined(__OAPI_ENHANCED_CPP)&&defined(__COUNTER__)
#define __OAPI_USE_SHARED_COUNTER
//If __OAPI_USE_SHARED_COUNTER is defined before the first use of__COUNTER__
//Then __COUNTER__ values will be shared between files.
#endif
#define __UNIQUE__() CONCAT4(UID00AAKKZ09,__COUNTER__,__LINE__,QJJ)
#endif
#endif


#ifndef UNIQUEID
#define UNIQUEID(prefix) CONCAT3(prefix,__,__UNIQUE__())
#endif





//Enable Contracts for catching Some UB If they are available.
//Disabled if __NOCONTRACTS or MINIMAL is defined
#if __has_cpp_attribute(assert)&& !defined(__NOCONTRACTS)
#define ASSERT(condition) [[assert:condition]]
#else
#define ASSERT(condition)
#endif
#if __has_cpp_attribute(expects)&& !defined(__NOCONTRACTS)
#ifdef CONTRACTS_LEVEL
#if CONTRACTS_LEVEL ==1
#define EXPECTS(condition) [[expects audit:condition]]
#elif CONTRACTS_LEVEL ==-1
#define EXPECTS(condition) [[expects axiom:condition]]
#else
#define EXPECTS(condition) [[expects:condition]]
#endif
#else
#define EXPECTS(condition) [[expects:condition]]
#endif
#else
#define EXPECTS(condition)
#endif

#if __has_cpp_attribute(ensures)&& !defined(__NOCONTRACTS)
#ifdef CONTRACTS_LEVEL
#if CONTRACTS_LEVEL==1
#define ENSURES(condition,...) [[ensures audit __VA_ARGS__:condition]]
#elif CONTRACTS_LEVEL==-1
#define ENSURES(condition,...) [[ensures axiom __VA_ARGS__:condition]]
#else
#define ENSURES(condition,...) [[ensures __VA_ARGS__:condition]]
#endif
#else
#define ENSURES(condition,...) [[ensures __VA_ARGS__:condition]]
#endif
#else
#define ENSURES(condition,...)
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

#if defined(_MSC_VER)
#define LIBLCCXX_IS_MSVCXX
#elif defined(__GNUC__)
#define LIBLCCXX_IS_GLIBCXX
#elif define(__clang__)
#define LIBLCCXX_IS_CLANG
#endif

#if defined(__WIN32)
#define LIBLCCXX_IS_WIN32
#define LIBLCCXX_OS_NAME "windows"
#define LIBLCCXX_OS_CODE 0
#elif defined(__linux__)
#define LIBLCCXX_OS_LINUX
#define LIBLCCXX_POSIX
#define LIBLCCXX_OS_NAME "linux"
#define LIBLCCXX_OS_CODE 1
#elif defined(__APPLE__)
#define LIBLCCXX_OS_APPLE
#define LIBLCCXX_OS_NAME "apple"
#define LIBLCCXX_OS_CODE 2
#elif defined(__snesos__)
#define LIBLCCXX_OS_SNES
#define LIBLCCXX_POSIX
#define LIBLCCXX_OS_NAME "snesos"
#define LIBLCCXX_OS_CODE 3
#elif defined(__ANDROID__)
#define LIBLCCXX_OS_ANDROID
#define LIBLCCXX_OS_NAME "android"
#define LIBLCCXX_OS_CODE 4
#elif defined(__unix__)
#define LIBLCCXX_OS_UNIX
#define LIBLCCXX_POSIX
#define LIBLCCXX_OS_NAME "unix"
#define LIBLCCXX_OS_CODE 5
#elif defined(__POSIX_VERSION)
#define LIBLCCXX_POSIX
#define LIBLCCXX_OS_NAME "posix"
#else
#define LIBLCCXX_OS_NAME "unknown"
#endif

#ifdef LIBLCCXX_OS_WINDOWS
#ifdef __WIN64
#define LIBLCCXX_ARCH_64
#else
#define LIBLCCXX_ARCH_32
#endif
#endif
#endif /* INCLUDE_CONFIG_HPP_ */
