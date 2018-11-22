/*
 * StlTypeHash.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_STLTYPEHASH_HPP__2018_09_30_19_17_14
#define __INCLUDE_LCLIB_CXX_STLTYPEHASH_HPP__2018_09_30_19_17_14

#include <type_traits>
#include <TypeHash.hpp>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <utility>
#include <tuple>
#include <variant>
#include <any>
#include <functional>

namespace types{
	template<typename CharT,typename CharTraits,typename Allocator>
		struct TypeHash<std::basic_string<CharT,CharTraits,Allocator>,std::void_t<decltype(TypeHash<CharT>{}())>>:
		hash_type<TypeHash<CharT>{}()*TypeCode::LEVEL+name("std::basic_string")>{};
	template<typename T,typename Allocator>
		struct TypeHash<std::vector<T,Allocator>,std::void_t<decltype(TypeHash<T>{}())>>:
		hash_type<TypeHash<T>{}()*TypeCode::LEVEL+name("std::vector")>{};
	template<typename T,typename Allocator,typename Compare>
		struct TypeHash<std::set<T,Compare,Allocator>,std::void_t<decltype(TypeHash<T>{}())>>:
		hash_type<TypeHash<T>{}()*TypeCode::LEVEL+name("std::set")>{};
	template<typename K,typename V,typename Compare,typename Allocator>
		struct TypeHash<std::map<K,V,Compare,Allocator>,std::void_t<decltype(TypeHash<K>{}()),decltype(TypeHash<V>{}())>>:
		hash_type<(TypeHash<K>{}()+TypeHash<V>{}())*TypeCode::LEVEL+name("std::map")>{};
	template<> struct TypeHash<std::type_info>:hash_type<name("std::type_info")>{};
	template<> struct TypeHash<std::type_index>:hash_type<name("std::type_index")>{};
	template<typename T,T value> struct TypeHash<std::integral_constant<T,value>,std::void_t<decltype(TypeHash<T>{}())>>:
		hash_type<(TypeHash<T>{}()+static_cast<uint64_t>(value))*TypeCode::LEVEL+name("std::integral_constant")>{};
	template<typename T,T... Vals> struct TypeHash<std::integer_sequence<T,Vals...>,std::void_t<decltype(TypeHash<T>{}())>>:
		hash_type<(TypeHash<T>{}()+sum(Vals...))*TypeCode::LEVEL+name("std::integer_sequence")>{};
	template<typename T> struct TypeHash<std::integer_sequence<T>,std::void_t<decltype(TypeHash<T>{}())>>:
		hash_type<(TypeHash<T>{}())*TypeCode::LEVEL+name("std::integer_sequence")>{};
	template<typename... Ts> struct TypeHash<std::tuple<Ts...>,std::void_t<decltype(TypeHash<Ts>{}())...>>:
		hash_type<sum(TypeHash<Ts>{}()...)*TypeCode::LEVEL+name("std::tuple")>{};
	template<typename... Ts> struct TypeHash<std::variant<Ts...>,std::void_t<decltype(TypeHash<Ts>{}())...>>:
		hash_type<sum(TypeHash<Ts>{}()...)*TypeCode::LEVEL+name("std::tuple")>{};
	template<> struct TypeHash<std::any>:hash_type<name("std::any")>{};
	template<> struct TypeHash<std::piecewise_construct_t>:hash_type<name("std::piecewise_construct_t")>{};
	template<> struct TypeHash<std::in_place_t>:hash_type<name("std::in_place_t")>{};
	template<typename T> struct TypeHash<std::in_place_type_t<T>,std::void_t<decltype(TypeHash<T>{}())>>:
		hash_type<TypeHash<T>{}()*TypeCode::LEVEL+name("std::in_place_type_t")>{};
	template<std::size_t I> struct TypeHash<std::in_place_index_t<I>>:
		hash_type<I*TypeCode::LEVEL+name("std::in_place_index_t")>{};
	template<typename F> struct TypeHash<std::function<F>,std::void_t<decltype(TypeHash<F>{}())>>:
		hash_type<TypeHash<F>{}()*TypeCode::LEVEL+name("std::function")>{};
}

#endif /* __INCLUDE_LCLIB_CXX_STLTYPEHASH_HPP__2018_09_30_19_17_14 */
