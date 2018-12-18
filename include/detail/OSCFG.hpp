/*
 * OSCFG.hpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_LCLIB_CXX_OSCFG_HPP__2018_11_22_09_20_26
#define __INCLUDE_LCLIB_CXX_OSCFG_HPP__2018_11_22_09_20_26

#if defined(_WIN32)
#define LIBLCCXX_OS_WIN32
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


#ifdef LIBLCCXX_OS_WIN32
#ifdef __CYGWIN__
#define LIBLCCXX_POSIX
#endif
#endif


#endif /* __INCLUDE_LCLIB_CXX_OSCFG_HPP__2018_11_22_09_20_26 */
