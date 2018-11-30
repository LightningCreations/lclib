/*
 * FunctionTypeHash.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_FUNCTIONTYPEHASH_HPP__2018_09_30_20_42_33
#define __INCLUDE_LCLIB_CXX_FUNCTIONTYPEHASH_HPP__2018_09_30_20_42_33
#include <type_traits>
#include <lclib/TypeHash.hpp>
namespace types{
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...),
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...)*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......),
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,name("..."))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)const,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CONST)*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)volatile,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::VOLATILE)*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)const volatile,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CV)*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)const,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CONST,name("..."))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)volatile,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::VOLATILE,name("..."))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)const volatile,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CV,name("..."))*TypeCode::LEVEL+name("function")>{};

	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,name("..."),name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)const noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CONST,name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)volatile noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::VOLATILE,name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args...)const volatile noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CV,name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)const noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CONST,name("..."),name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)volatile noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::VOLATILE,name("..."),name("noexcept"))*TypeCode::LEVEL+name("function")>{};
	template<typename Ret,typename... Args> struct TypeHash<Ret(Args......)const volatile noexcept,
			std::void_t<decltype(TypeHash<Ret>{}()),decltype(TypeHash<Args>{}())...>>:
			hash_type<sum(TypeHash<Ret>{}(),TypeHash<Args>{}()...,TypeCode::CV,name("..."),name("noexcept"))*TypeCode::LEVEL+name("function")>{};
}


#endif /* __INCLUDE_LCLIB_CXX_FUNCTIONTYPEHASH_HPP__2018_09_30_20_42_33 */
