/*
 * PPHacks.hpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_DETAIL_PPHACKS_HPP__2018_11_22_09_52_36
#define __INCLUDE_DETAIL_PPHACKS_HPP__2018_11_22_09_52_36

#define __MACRO_SELECT_SIZE_I(e0,e1,e2,e3,e4,e5,e6,e7,size,...) size

#define __EVAL0(...) __VA_ARGS__
#define __EVAL(...) __EVAL0(__VA_ARGS__)

#define __CONCAT2A(a,b) a##b
#define __CONCAT2(a,b) __CONCAT2A(a,b)

#define __CONCAT3(a,b,c) __CONCAT2(a,__CONCAT2(b,c))
#define __CONCAT4(a,b,c,d) __CONCAT2(__CONCAT2(a,b),__CONCAT2(c,d))
#define __CONCAT5(a,b,c,d,e) __CONCAT2(__CONCAT2(a,b),__CONCAT3(c,d,e))


#define __CONCAT_MANY1(a) a
#define __CONCAT_MANY2(a,b) __CONCAT2(a,b)
#define __CONCAT_MANY3(a,b,c) __CONCAT3(a,b,c)
#define __CONCAT_MANY4(a,b,c,d) __CONCAT4(a,b,c,d)
#define __CONCAT_MANY5(a,b,c,d,e) __CONCAT2(__CONCAT3(a,b,c),__CONCAT2(a,b))
#define __CONCAT_MANY6(a,b,c,d,e,f) __CONCAT2(__CONCAT3(a,b,c),__CONCAT3(d,e,f))
#define __CONCAT_MANY7(a,b,c,d,e,f,g) __CONCAT2(__CONCAT3(a,b,c),__CONCAT4(d,e,f,g))

#define __CONCAT_MANY0()

#define __CONCAT_MANY_SELECT(...) __CONCAT2(__CONCAT_MANY,__MACRO_SELECT_SIZE_I(__VA_ARGS__,7,6,5,4,3,2,1,0))(__VA_ARGS__)


#define CONCAT(...) __CONCAT_MANY_SELECT(__VA_ARGS__)


#define __STRINGIFY0(...) #__VA_ARGS__
#define STRINGIFY(...) __STRINGIFY0(__VA_ARGS__)

#endif /* __INCLUDE_DETAIL_PPHACKS_HPP__2018_11_22_09_52_36 */
