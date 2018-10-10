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
#include <lclib-cxx/Detectors.hpp>

namespace security{
	namespace detail{
		template<typename Allocator> using value_type_t = typename std::allocator_traits<Allocator>::value_type;
		template<typename Allocator> using void_pointer_t = typename std::allocator_traits<Allocator>::void_pointer;
		template<typename Allocator> using const_void_pointer_t = typename std::allocator_traits<Allocator>::const_void_pointer;
		template<typename Allocator> using pointer_t = typename std::allocator_traits<Allocator>::pointer;
		template<typename Allocator> using const_pointer_t = typename std::allocator_traits<Allocator>::const_pointer;
		template<typename Allocator,typename U> using rebind_t = typename std::allocator_traits<Allocator>::template rebind<U>::other;
		template<typename Allocator,typename... Args> using construct_detector =
			std::void_t<decltype(std::declval<Allocator>().construct(std::declval<pointer_t<Allocator>>(),std::declval<Args>()...))>;
		template<typename Allocator> using destroy_detector =
			std::void_t<decltype(std::declval<Allocator>().destroy(std::declval<pointer_t<Allocator>>()))>;
		template<typename Allocator> using size_type_t = typename std::allocator_traits<Allocator>::size_type;
		template<typename Allocator> using difference_type_t = typename std::allocator_traits<Allocator>::difference_type;
	}
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
	/**
	 * AllocatorWrapper on a given Allocator that zeros allocated memory before constructing
	 * and after destroying the object.
	 * If no Allocator Parameter is provided, or the Allocator parameter is std::allocator<T>,
	 * the memory is allocated and deallocated using operator new/operator delete.
	 */
	template<typename T,typename Allocator=std::allocator<T>> struct SecureAllocator{
	public:
		static_assert(std::is_same_v<T,typename std::allocator_traits<Allocator>::value_type>,"Allocator must be for T");
		using value_type = detail::value_type_t<Allocator>;
		using pointer = detected_or_t<value_type*,detail::pointer_t,Allocator>;
		using void_pointer = detected_or_t<void*,detail::void_pointer_t,Allocator>;
		using const_void_pointer = detected_or_t<const void*,detail::const_void_pointer_t,Allocator>;
		using const_pointer = detected_or_t<const value_type*,detail::const_pointer_t,Allocator>;
		using size_type = detected_or_t<std::size_t,detail::size_type_t,Allocator>;
		using difference_type = detected_or_t<std::ptrdiff_t,detail::difference_type_t,Allocator>;

		template<typename U> struct rebind{
			using other = SecureAllocator<U,require_detected_t<typename detail::rebind_t,Allocator,U>>;
		};
	private:
		Allocator a;
	public:
		template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<Allocator,Args...>>>
		SecureAllocator(Args&&... args)
				noexcept(std::is_nothrow_constructible_v<Allocator,Args...>):
				a(std::forward(args)...){}
		template<typename=std::enable_if_t<std::is_default_constructible_v<Allocator>>>
				SecureAllocator()=default;
		SecureAllocator(const std::enable_if_t<std::is_copy_constructible_v<Allocator>,SecureAllocator>&)=default;
		SecureAllocator(std::enable_if_t<std::is_move_constructible_v<Allocator>,SecureAllocator>&&)=default;
		SecureAllocator(const SecureAllocator&&)=delete;
		SecureAllocator& operator=(const std::enable_if_t<std::is_copy_assignable_v<Allocator>,SecureAllocator>&)=default;
		SecureAllocator& operator=(std::enable_if_t<std::is_move_assignable_v<Allocator>,SecureAllocator>&&)=default;
		pointer allocate(size_type n){
			return a.allocate(n);
		}
		void deallocate(pointer t,size_type n){
			a.deallocate(t,n);
		}
		template<typename... Args,typename=require_detected_t<detail::construct_detector,Allocator,Args...>>
			void construct(pointer p,Args&&... args){
				std::memset(p,0,sizeof(T));
				a.construct(p,std::forward(args)...);
			}
		template<typename... Args,typename=std::enable_if_t<
			!is_detected_v<detail::construct_detector,Allocator,Args...>&&
			std::is_constructible_v<T,Args>>> void construct(pointer p,Args&& args){
				std::memset(p,0,sizeof(T));
				new(p) T(std::forward(args)...);
			}
		std::void_t<require_detected_t<detail::destroy_detector,Allocator>> destroy(pointer p){
			a.destroy(p);
			std::memset(p,0,sizeof(T));
		}
		void destroy(std::enable_if_t<!is_detected_v<detail::destroy_detector,Allocator>,pointer> p){
			std::destroy_at(p);
			std::memset(p,0,sizeof(T));
		}
	};
	template<typename T,typename Allocator> struct SecureAllocator<T,SecureAllocator<T,Allocator>>:SecureAllocator<T,Allocator>{};
	template<typename T> struct SecureAllocator<T,std::allocator<T>>{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using void_pointer = void*;
		using const_void_pointer = const void*;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		template<typename U> struct rebind{
			using other = SecureAllocator<U>;
		};

		template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<T,Args...>>>
			void construct(pointer p,Args&&... args){
				std::memset(p,0,sizeof(T));
				new(p) T(std::forward(args)...);
			}
		void destroy(pointer p){
			std::destroy_at(p);
			std::memset(p,0,sizeof(T));
		}
		pointer allocate(std::size_t n){
			return (pointer)operator new(n*sizeof(T));
		}
		void deallocate(pointer p){
			operator delete(p);
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
	template<typename CharT,typename CharTraits=std::char_traits<CharT>,typename Allocator=std::allocator<CharT>> using basic_string = std::basic_string<CharT,CharTraits,SecureAllocator<CharT,Allocator>>;
	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;
	using u16string = basic_string<char16_t>;
	using u32string = basic_string<char32_t>;

	/**
	 * Secure Versions of all Allocator Aware Containers in the Standard Template library
	 */
	template<typename T,typename Allocator=std::allocator<T>> using vector = std::vector<T,SecureAllocator<T,Allocator>>;
	template<typename T,typename Compare=std::less<T>,typename Allocator=std::allocator<T>> using set = std::set<T,Compare,SecureAllocator<T,Allocator>>;
	template<typename K,typename V,typename Compare=std::less<K>,typename Allocator=std::allocator<std::pair<const K,V>>>
		using map = std::map<K,V,Compare,SecureAllocator<std::pair<const K,V>,Allocator>>;
	template<typename T,typename Hash=std::hash<T>,typename Equals=std::equal_to<T>,typename Allocator=std::allocator<T>>
		using unordered_set = std::unordered_set<T,Hash,Equals,SecureAllocator<T,Allocator>>;
	template<typename K,typename V,typename Hash=std::hash<K>,typename Equals=std::equal_to<K>,typename Allocator=std::allocator<std::pair<const K,V>>>
		using unordered_map = std::unordered_map<K,V,Hash,Equals,SecureAllocator<std::pair<const K,V>,Allocator>>;
	template<typename T,typename Allocator=std::allocator<T>>
		using list = std::list<T,SecureAllocator<T,Allocator>>;
	template<typename T,typename Compare=std::less<T>,typename Allocator=std::allocator<T>>
		using multiset = std::multiset<T,Compare,SecureAllocator<T,Allocator>>;
	template<typename T,typename Hash=std::hash<T>,typename Equals=std::equal_to<T>,typename Allocator=std::allocator<T>>
		using unordered_multiset = std::unordered_multiset<T,Hash,Equals,SecureAllocator<T,Allocator>>;
	template<typename K,typename V,typename Compare=std::less<K>,typename Allocator=std::allocator<std::pair<const K,V>>>
		using multimap = std::multimap<K,V,Compare,SecureAllocator<std::pair<const K,V>,Allocator>>;
	template<typename K,typename V,typename Hash=std::hash<K>,typename Equals=std::equal_to<K>,typename Allocator=std::allocator<std::pair<const K,V>>>
		using unordered_multimap =std::unordered_multimap<K,V,Hash,Equals,SecureAllocator<std::pair<const K,V>,Allocator>>;
	template<typename T,typename Allocator=std::allocator<T>>
		using forward_list = std::forward_list<T,SecureAllocator<T,Allocator>>;
}

#endif /* __INCLUDE_SECURITY_SECUREDELETER_HPP__2018_09_17_18_44_59 */
