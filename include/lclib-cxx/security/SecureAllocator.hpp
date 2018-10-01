/*
 * SecureDeleter.hpp
 *
 *  Created on: Sep 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_SECURITY_SECUREDELETER_HPP__2018_09_17_18_44_59
#define __INCLUDE_SECURITY_SECUREDELETER_HPP__2018_09_17_18_44_59
#include <utility>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_set>
#include <deque>
#include <map>
#include <unordered_map>
#include <list>
#include <forward_list>
#include <type_traits>

namespace security{
	template<typename T> struct SecureDeleter{
	public:
		void operator()(T* t)const{
			std::destroy_at(t);
			memset(t,0,sizeof(T));
			operator delete(t);
		}
	};
	template<typename T> struct SecureDeleter<T[]>{
		static_assert(!std::is_array_v<T[]>,"Secure Deleter cannot be instantiated with an Array Type due to problems with Dynamic Array Types");
	};
	template<typename T> struct SecureAllocator{
	public:
		using pointer = T*;
		using void_pointer = void*;
		using const_pointer = const T*;
		using const_void_pointer = const void*;
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::size_t;
		pointer allocate(size_type n){
			const size_type sz = n*sizeof(T);
			void* ret = operator new(sz);
			return (pointer*)ret;
		}
		void deallocate(T* t,size_type n){
			const size_type sz = n*sizeof(T);
			operator delete(t,sz);
		}
		template<typename... Args> void construct(T* t,Args&&... args){
			memset(t,0,sizeof(T));
			new(t) T(args...);
		}
		void destroy(T* t){
			std::destroy_at(t);
			memset(t,0,sizeof(T));
		}
	};
	template<typename T> using unique_ptr = std::unique_ptr<T,SecureDeleter<T>>;

	template<typename T,typename... Args> unique_ptr<T> make_unique(Args&&... args){
		return unique_ptr<T>{new T{std::forward(args)...}};
	}
	template<typename T> std::enable_if_t<std::is_trivially_copyable_v<T>> clear(T* t){
		const std::size_t sz = sizeof(t);
		memset((void*)t,0,sz);
	}
	/**
	 * A basic_string using the Secure Allocator
	 */
	template<typename CharT,typename CharTraits=std::char_traits<CharT>> using basic_string = std::basic_string<CharT,CharTraits,SecureAllocator<CharT>>;
	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;
	using u16string = basic_string<char16_t>;
	using u32string = basic_string<char32_t>;

	/**
	 * Secure Versions of all Allocator Aware Containers in the Standard Template library
	 */
	template<typename T> using vector = std::vector<T,SecureAllocator<T>>;
	template<typename T,typename Compare=std::less<T>> using set = std::set<T,Compare,SecureAllocator<T>>;
	template<typename K,typename V,typename Compare=std::less<K>> using map = std::map<K,V,Compare,SecureAllocator<std::pair<const K,V>>>;
	template<typename T,typename Hash=std::hash<T>,typename Equals=std::equal_to<T>> using unordered_set = std::unordered_set<T,Hash,Equals,SecureAllocator<T>>;
	template<typename K,typename V,typename Hash=std::hash<K>,typename Equals=std::equal_to<K>> using unordered_map = std::unordered_map<K,V,Hash,Equals,SecureAllocator<std::pair<const K,V>>>;
	template<typename T> using list = std::list<T,SecureAllocator<T>>;
	template<typename T,typename Compare=std::less<T>> using multiset = std::multiset<T,Compare,SecureAllocator<T>>;
	template<typename T,typename Hash=std::hash<T>,typename Equals=std::equal_to<T>> using unordered_multiset = std::unordered_multiset<T,Hash,Equals,SecureAllocator<T>>;
	template<typename K,typename V,typename Compare=std::less<K>> using multimap = std::multimap<K,V,Compare,SecureAllocator<std::pair<const K,V>>>;
	template<typename K,typename V,typename Hash=std::hash<K>,typename Equals=std::equal_to<K>> using unordered_multimap = std::unordered_multimap<K,V,Hash,Equals,SecureAllocator<std::pair<const K,V>>>;
	template<typename T> using forward_list = std::forward_list<T,SecureAllocator<T>>;
}

#endif /* __INCLUDE_SECURITY_SECUREDELETER_HPP__2018_09_17_18_44_59 */
