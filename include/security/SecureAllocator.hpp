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

namespace security{
	template<typename T> struct SecureDeleter{
	public:
		void operator()(T* t)const{
			std::destroy_at(t);
			memset(t,0,sizeof(T));
			operator delete(t);
		}
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
			memset(ret,0,sz);
			return (pointer*)ret;
		}
		void deallocate(T* t,size_type n){
			const size_type sz = n*sizeof(T);
			memset(t,0,sz);
			operator delete(t,sz);
		}
		template<typename... Args> void construct(T* t,Args&&... args){
			new(t) T(args...);
		}
		void destroy(T* t){
			std::destroy_at(t);
		}
	};
	template<typename T> using unique_ptr = std::unique_ptr<T,SecureDeleter<T>>;

	template<typename T,typename... Args> unique_ptr make_unique(Args&&... args){
		return unique_ptr{new T{std::forward(args)...}};
	}
	template<typename T> std::enable_if_t<std::is_trivially_copyable_v<T>> clear(T* t){
		const std::size_t sz = sizeof(t);
		memset((void*)t,0,sz);
	}

}



#endif /* __INCLUDE_SECURITY_SECUREDELETER_HPP__2018_09_17_18_44_59 */
