/*
 * Config.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CONFIG_HPP__2018_09_06_16_35
#define _INCLUDE_CONFIG_HPP__2018_09_06_16_35

#if __cplusplus < 201703L
#error This library requires C++17 Mode to function (Compile with -std=c++1z, -std=c++17, or -std=c++2a). In MSVC you have to specify C++17 or C++latest and 
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

#include <detail/PPHacks.hpp>

#ifndef __UNIQUE__
//Unique Id's are used by reflection (though are also available to users)
#include <detail/OAPIPP.hpp>
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

#ifndef INCLUDE_WARNING
#if defined(LIBLCCXX_ERROR_INCLUDE_WARNINGS)
#define INCLUDE_WARNING(...) static_assert(false,STRINGIFY(Including __FILE__ is not enabled: __VA_ARGS__))
#else
#define INCLUDE_WARNING(...) _Pragma(message STRINGIFY(Including __FILE__ is not enabled: __VA_ARGS__))
#endif
#endif

#if defined(_MSC_VER)
#define LIBLCCXX_IS_MSVCXX
#elif defined(__GNUC__)
#define LIBLCCXX_IS_GLIBCXX
#elif define(__clang__)
#define LIBLCCXX_IS_CLANG
#endif

#include <detail/OSCFG.hpp>


#ifdef LIBLCCXX_OS_WIN32
#ifdef __WIN64
#define LIBLCCXX_ARCH_64
#else
#define LIBLCCXX_ARCH_32
#endif
#endif

#endif /* INCLUDE_CONFIG_HPP_ */
