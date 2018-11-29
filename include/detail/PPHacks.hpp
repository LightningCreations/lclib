/*
 * PPHacks.hpp
 *
 *  Created on: Nov 22, 2018
 *      Author: chorm
 */

#ifndef __INCLUDE_DETAIL_PPHACKS_HPP__2018_11_22_09_52_36
#define __INCLUDE_DETAIL_PPHACKS_HPP__2018_11_22_09_52_36

#define __MACRO_SELECT_SIZE_I(e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12,e13,e14,e15,e16,e17,e18,e19,e20,e21,e22,e23,e24,e25,e26,e27,e28,e29,e30,e31,size,...) size

#define __EVAL0(...) __VA_ARGS__
#define __EVAL(...) __EVAL0(__VA_ARGS__)

#define __CONCAT2A(a,b) a##b
#define __CONCAT2(a,b) __CONCAT2A(_EVAL(a),_EVAL(b))

#define __CONCAT3(a,b,c) __CONCAT2(a,__CONCAT2(b,c))
#define __CONCAT4(a,b,c,d) __CONCAT2(__CONCAT2(a,b),__CONCAT2(c,d))
#define __CONCAT_MANY4(a,b,c,d,...) __CONCAT2(__CONCAT4(a,b,c,d),__CONCAT_MANY(__VA_ARGS__))
#define __CONCAT_MANY1(a) __EVAL(a)
#define __CONCAT_MANY2(a,b) __CONCAT2(a,b)
#define __CONCAT_MANY3(a,b,c) __CONCAT3(a,b,c)
#define __CONCAT_MANY0()
#define __CONCAT_MANY_SELECT(...) __CONCAT2(__CONCAT_MANY,__MACRO_SELECT_SIZE_I(__VA_ARGS__,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,2,1,0))

#define __CONCAT_MANY(...) __CONCAT_MANY_SELECT(__VA_ARGS__)

#define CONCAT(...) __CONCAT_MANY(__VA_ARGS__)

#define __STRINGIFY0(...) #__VA_ARGS__
#define STRINGIFY(...) __STRINGIFY0(__VA_ARGS__)

#endif /* __INCLUDE_DETAIL_PPHACKS_HPP__2018_11_22_09_52_36 */
