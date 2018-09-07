/*
 * STLTypeInfo.hpp
 *
 *  Created on: Sep 7, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_STLTYPEINFO_HPP__2018_09_07_11_46
#define __INCLUDE_STLTYPEINFO_HPP__2018_09_07_11_46
#include <TypeInfo.hpp>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <variant>
#include <tuple>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <array>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <thread>
#include <any>
#include <complex>
#include <valarray>
#include <chrono>
#include <rational>
#include <exception>

namespace types{
	template<typename CharT,typename CharTraits,typename Allocator> struct TypeHash<enable_specialization_t<std::basic_string<CharT,CharTraits,Allocator>,CharT>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<CharT>{}()*TypeCode::LEVEL*nameHash("std::string");
		}
	};
	template<typename T,typename Deleter> struct TypeHash<enable_specialization_t<std::unique_ptr<T,Deleter>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("std::unique_ptr");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::shared_ptr<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("std::shared_ptr");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::weak_ptr<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("std::shared_ptr");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::optional<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("std::optional");
		}
	};
	template<typename... Types> struct TypeHash<enable_specialization_t<std::variant<Types...>,Types...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<Types>(){}+...)*nameHash("std::variant");
		}
	};
	template<typename... Types> struct TypeHash<enable_specialization_t<std::tuple<Types...>,Types...>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<Types>(){}+...)*nameHash("std::tuple");
		}
	};
	template<typename A,typename B> struct TypeHash<enable_specialization_t<std::pair<A,B>,A,B>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<A>{}()+TypeHash<B>{}())*nameHash("std::pair");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::reference_wrapper<T>,T>>{
		public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()()const{
			return TypeHash<T>{}()*TypeCode::LEVEL^Level::REFERENCE;
		}
	};
	template<typename T,typename Allocator> struct TypeHash<enable_specialization_t<std::vector<T,Allocator>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("std::vector");
		}
	};
	template<typename T,size_t N> struct TypeHash<enable_specialization_t<std::array<T,N>>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*TypeCode::LEVEL^Level::EXTENT*TypeCode{N+1};
		}
	};
	template<typename T,typename Allocator> struct TypeHash<enable_specialization_t<std::set<T,Allocator>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeHash<T>{}()*nameHash("std::set");
		}
	};

	template<typename Key,typename Value,typename Hash,typename Predicate> struct TypeHash<enable_specialization_t<std::unordered_map<Key,Value,Hash,Predicate>,Key,Value>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<Key>{}()+TypeHash<Value>{}())*nameHash("std::unordered_map");
		}
	};
	template<typename Key,typename Value,typename Compare,typename Allocator> struct TypeHash<enable_specialization_t<std::map<Key,Value,Compare,Allocator>,Key,Value>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<Key>{}()+TypeHash<Value>{}())*nameHash("std::map");
		}
	};
	template<typename F> struct TypeHash<enable_specialization_t<std::function<F>,F>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<F>{}())*nameHash("std::function");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::atomic<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<T>{}())*nameHash("std::atomic");
		}
	};
	template<> struct TypeHash<std::thread>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return nameHash("std::thread");
		}
	};
	template<> struct TypeHash<std::any>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return nameHash("std::any");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::hash<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode<T>{}()*nameHash("std::hash");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::complex<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode<T>{}()*nameHash("std::complex");
		}
	};
	template<typename T> struct TypeHash<enable_specialization_t<std::valarray<T>,T>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return TypeCode<T>{}()*nameHash("std::valarray");
		}
	};
	template<uintmax_t Num,uintmax_t Denom> struct TypeHash<std::ratio<Num,Denom>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (hashcode(Num)^hashcode(Denom))*nameHash("std::ratio");
		}
	};
	template<typename Rep,typename Period> struct TypeHash<enable_specialization_t<std::chrono::duration<Rep,Period>,Rep,Period>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<Rep>{}()^TypeHash<Period>{}())*nameHash("std::chrono::duration");
		}
	};
	template<typename Clock,typename Duration> struct TypeHash<enable_specialization_t<std::chrono::time_point<Clock,Duration>,Clock,Duration>>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		TypeHash& operator=(const TypeHash&&)=delete;
		constexpr TypeCode operator()(){
			return (TypeHash<Rep>{}()^TypeHash<Period>{}())*nameHash("std::chrono::time_point");
		}
	};
	template<> struct TypeHash<std::chrono::system_clock>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("std::chrono::system_clock");
		}
	};
	template<> struct TypeHash<std::chrono::steady_clock>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("std::chrono::steady_clock");
		}
	};
	template<> struct TypeHash<std::chrono::high_resolution_clock>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("std::chrono::high_resolution_clock");
		}
	};
	template<> struct TypeHash<std::exception>{
	public:
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return nameHash("std::exception");
		}
	};
	template<> struct TypeHash<std::logic_error>{
	public:
		using base_class = std::exception;
		using base_hash = TypeHash<std::exception>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::exception>{}()+nameHash("std::logic_error");
		}
	};
	template<> struct TypeHash<std::invalid_argument>{
	public:
		using base_class = std::logic_error;
		using base_hash = TypeHash<std::logic_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::logic_error>{}()+nameHash("std::invalid_argument");
		}
	};
	template<> struct TypeHash<std::domain_error>{
	public:
		using base_class = std::logic_error;
		using base_hash = TypeHash<std::logic_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::logic_error>{}()+nameHash("std::domain_error");
		}
	};
	template<> struct TypeHash<std::length_error>{
	public:
		using base_class = std::logic_error;
		using base_hash = TypeHash<std::logic_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::logic_error>{}()+nameHash("std::length_error");
		}
	};
	template<> struct TypeHash<std::out_of_range>{
	public:
		using base_class = std::logic_error;
		using base_hash = TypeHash<std::logic_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::logic_error>{}()+nameHash("std::out_of_range");
		}
	};
	template<> struct TypeHash<std::runtime_error>{
	public:
		using base_class = std::exception;
		using base_hash = TypeHash<std::exception>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::exception>{}()+nameHash("std::runtime_error");
		}
	};
	template<> struct TypeHash<std::range_error>{
	public:
		using base_class = std::runtime_error;
		using base_hash = TypeHash<std::runtime_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::runtime_error>{}()+nameHash("std::range_error");
		}
	};
	template<> struct TypeHash<std::overflow_error>{
	public:
		using base_class = std::runtime_error;
		using base_hash = TypeHash<std::runtime_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::runtime_error>{}()+nameHash("std::overflow_error");
		}
	};
	template<> struct TypeHash<std::range_error>{
	public:
		using base_class = std::runtime_error;
		using base_hash = TypeHash<std::runtime_error>;
		constexpr TypeHash()=default;
		constexpr TypeHash(const TypeHash&)=default;
		constexpr TypeHash(TypeHash&&)=default;
		TypeHash(const TypeHash&&)=delete;
		constexpr TypeHash& operator=(const TypeHash&)=default;
		constexpr TypeHash& operator=(TypeHash&&)=default;
		constexpr TypeCode operator()(){
			return TypeHash<std::runtime_error>{}()+nameHash("std::range_error");
		}
	};
}




#endif /* __INCLUDE_STLTYPEINFO_HPP__2018_09_07_11_46 */
