/*
 * OAPIPP.hpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_DETAIL_OAPIPP_HPP__2018_11_22_10_11_20
#define __INCLUDE_DETAIL_OAPIPP_HPP__2018_11_22_10_11_20

#if defined(__OAPI_CPP_EXT)
#if __has_oapi_import("__UNIQUE__")>201705L
#pragma macros import("__UNIQUE__",) define("__UNIQUE__",)
#else
#pragma OAPI counter useshared
#endif
#if __has_oapi_import("__FHASH__")
#pragma macros import("__FHASH__") define("__FHASH__")
#endif
#if __has_oapi_import("__RANDOM__")
#pragma macros import("__RANDOM__")
#endif
#endif

#ifndef __RANDOM__
#define __RANDOM__ SEwubBpDbm0kexN9V1tBcxFDSw
#endif


#ifndef __UNIQUE__
#ifndef __FHASH__
#define __FHASH__ AZsZPiXGlruWXCNMslMSUGum
#endif
#define __UNIQUE__() __CONCAT5(UID,__FHASH__,__COUNTER__,__LINE__,__RANDOM__)
#endif


#define UNIQUEID(...) CONCAT(__VA_ARGS__,__,__UNIQUE__())

#endif /* __INCLUDE_DETAIL_OAPIPP_HPP__2018_11_22_10_11_20 */
