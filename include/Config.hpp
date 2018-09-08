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

#if defined(__OAPI_MACROS)&&defined(__HAS_IMPORT_UNIQUE)
#pragma macros import("__UNIQUE__") define("__UNIQUE__")
#elif !defined(__UNIQUE__)
#define __UNIQUE__ UID##__COUNTER__##__LINE__##QJJ
#endif

#endif /* INCLUDE_CONFIG_HPP_ */
