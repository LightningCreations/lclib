/*
 * Concepts.hpp
 *
 *  Created on: Sep 18, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_CONCEPTS_HPP__2018_09_18_08_27_33
#define __INCLUDE_CONCEPTS_HPP__2018_09_18_08_27_33
#include <lclib/Config.hpp>
#ifdef LIBLCCXX_HAS_CONCEPTS
#include <lclib/TypeTraits.hpp>
#include <type_traits>
#include <concepts>
template<typename T> concept Byte = is_byte_v<T>;
template<typename T> concept Complete = is_complete_v<T>;


#endif
#endif /* __INCLUDE_CONCEPTS_HPP__2018_09_18_08_27_33 */
