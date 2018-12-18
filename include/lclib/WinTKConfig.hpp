/*
 * WinTKConfig.hpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_LCLIB_CXX_WINTKCONFIG_HPP__2018_11_22_09_22_10
#define __INCLUDE_LCLIB_CXX_WINTKCONFIG_HPP__2018_11_22_09_22_10

#include <lclib/Config.hpp>
#include <lclib/CppHelpers.hpp>

#if __has_include(<gtk/gtk.h>)
#define LIBLCCXX_HAS_GNOME
#define LIBLCCXX_HAS_WTK
#endif

#if __has_include(<GL/gl.h>)
#define LIBLCCXX_HAS_OPENGL
#define LIBLCCXX_HAS_WTK
#endif

#if __has_include(<vulkan/vulkan.h>)
#define LIBLCCXX_HAS_VULKAN
#define LIBLCCXX_HAS_WTK
#endif

#ifdef LCLIBCXX_HAS_CLI
#define LIBLCCXX_HAS_CLIWTK
#define LIBLCCXX_HAS_WTK
#endif

#ifdef LIBLCCXX_OS_WIN32
#define LIBLCCXX_HAS_WIN32WTK
#define LIBLCCXX_HAS_WTK
#endif

#ifndef LIBLCCXX_HAS_WTK
#define LIBLCCXX_HEADLESS
#warning "Including UI Header without a windowing toolkit"
#endif

#endif /* __INCLUDE_LCLIB_CXX_WINTKCONFIG_HPP__2018_11_22_09_22_10 */
