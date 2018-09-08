/*
 * Config.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CONFIG_HPP__2018_09_06_16_35
#define _INCLUDE_CONFIG_HPP__2018_09_06_16_35

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
#endif




#define __CONCAT2A(a,b) a##b
#define __CONCAT3A(a,b,c) a##b##c
#define __CONCAT4A(a,b,c,d) a##b##c##d

#define CONCAT2(a,b) __CONCAT2A(a,b)
#define CONCAT3(a,b,c) __CONCAT3A(a,b,c)
#define CONCAT4(a,b,c,d) __CONCAT4A(a,b,c,d)

#define STRINGIFY(...) #__VA_ARGS__

#if defined(__OAPI_MACROS)&&defined(__HAS_IMPORT_UNIQUE)
#pragma macros import("__UNIQUE__") define("__UNIQUE__")
//On SNES-OS __UNIQUE__ expands to UID<filename hash><line number hash><Translation Time Random Token>QJJ
#elif !defined(__UNIQUE__)
#define __UNIQUE__() CONCAT4(UID00AAKKZ09,__COUNTER__,__LINE__,QJJ)
#endif

#define UNIQUEID(prefix) CONCAT3(prefix,__,__UNIQUE__())


#endif /* INCLUDE_CONFIG_HPP_ */
