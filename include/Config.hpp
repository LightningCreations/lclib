/*
 * Config.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: connor
 */

#ifndef _INCLUDE_CONFIG_HPP__2018_09_06_16_35
#define _INCLUDE_CONFIG_HPP__2018_09_06_16_35

#ifdef _WIN32
#define LCLIBEXPORT __declspec(dllexport)
#define LCLIBIMPORT __declspec(dllimport)
#else
#define LCLIBEXPORT __attribute((visibility("default")))
#define LCLIBIMPORT __attribute((visibility("hidden")))
#endif
#ifdef LCLIB_CXX_DEFINITION
#define LIBLCFUNC LCLIBEXPORT
#else
#define LIBLCFUNC LCLIBIMPORT
#endif



#endif /* INCLUDE_CONFIG_HPP_ */
