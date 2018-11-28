/*
 * BuildConfig.hpp
 *
 *  Created on: Nov 27, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_CONFIGURE_BUILDCONFIG_HPP__2018_11_27_09_29_20
#define __INCLUDE_CONFIGURE_BUILDCONFIG_HPP__2018_11_27_09_29_20

/*
 * Place any extra build configuration variables here.
 * These will be included by <lclib/Config.hpp> both when building the library,
 * and when building projects that use LClib-C++.
 *
 * This is not the place for defining use specific macros (unless they affect code generation within the library).
 * When building Projects using LCLib-C++, extra build configuration variables should be supplied
 * in a local <configure/UserConfig.hpp>.
 * If such a file exists, it is loaded whenever including <lclib/Config.hpp> outside of library build.
 *
 * The following Macros may be overriden by defining them here:
 * __UNIQUE__() (Generates a Unique identitifier, note that the __ is for compatability with OAPI).
 * __NOREFLECTION (Turns off reflection symbol generation. Presently unused)
 * __NOCONTRACTS (Turns off contracts. Only applies when contract attributes are enabled. This also disables assertions)
 * __NOCLIEXTENSIONS (Turns off CLIExtensions. Only applies when both USE_CLI_EXTENSIONS __cplusplus_cli is defined)
 * MINIMAL (Turns off all of the above)
 * __CXX_USE_CLI_EXTENSIONS (If this is defined and __NOCLIEXTENSIONS is not, then some code will generate using C++/CLI syntax when __cplusplus_cli is defined)
 * ASSERT(condtion) declares an assertion.
 * EXPECTS(condtion) declares a precondition of a function.
 * ENSURES(condtion,...) declares a postcondition of a function.
 * LIBLCCXX_ERROR_INCLUDE_WARNINGS causes the default definition of INCLUDE_WARNING to emmit an error instead of a warning
 * INCLUDE_WARNING (...) warns or errors with __FILE__ and __VA_ARGS__ whenever speficic files are included
 *
 */



#endif /* __INCLUDE_CONFIGURE_BUILDCONFIG_HPP__2018_11_27_09_29_20 */
