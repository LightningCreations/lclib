/*
 * DynamicArray.hpp
 *
 *  Created on: Oct 29, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_DYNAMICARRAY_HPP__2018_10_29_11_15_27
#define __INCLUDE_LCLIB_CXX_DYNAMICARRAY_HPP__2018_10_29_11_15_27

#include <memory>
#include <iterator>
#include <utility>
#include <tuple>
#include <lclib/Detectors.hpp>
#include <type_traits>
#include <functional>

namespace detail{
	template<typename T,typename Tuple> using detect_constructible =
				decltype(std::make_from_tuple<T>(std::declval<Tuple>()));
	template<typename Pointer,typename Allocator,typename Tuple,std::size_t... Idx>
		void do_construct_from_tuple(Allocator& a,Tuple&& t,Pointer p,std::index_sequence<Idx...>){
		std::allocator_traits<Allocator>::construct(a, p, std::get<Idx>(std::forward<Tuple>(p))...);
	}
	template<typename Pointer,typename Allocator,typename Tuple>
			int do_construct_from_tuple(Allocator& a,Tuple&& t,Pointer p){
			do_construct_from_tuple(a,std::forward<Tuple>(t),p,std::make_index_sequence<std::tuple_size_v<remove_cvref_t<Tuple>>>{});
			return 0;
		}
	template<typename Pointer,typename Allocator,typename TupleHorde,std::size_t... Idx>
		void do_piecewise_construct(Allocator& a,TupleHorde&& h,Pointer p,std::index_sequence<Idx...>){
			auto placeholder = {do_construct_from_tuple(a,std::get<Idx>(std::forward<TupleHorde>(h)),&p[Idx])...};
		}

}



template<typename T,typename Allocator=std::allocator<T>>
struct DynamicArray {
public:
	using allocator_type = Allocator;
	using value_type = T;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using reference = T & ;
	using const_reference = const T&;
	using iterator = T * ;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
private:
	pointer p;
	size_type sz;
	allocator_type a;
public:
	DynamicArray(const DynamicArray& o) :
		a{ std::allocator_traits<Allocator>::select_on_container_copy_construction(o.a) } {
		sz = o.sz;
		p = std::allocator_traits<Allocator>::allocate(a, sz);
		for (std::size_t n = 0; n < sz; n++)
			std::allocator_traits<Allocator>::construct(a, &p[n], o[n]);
		p = std::launder(p);
	}

	DynamicArray(DynamicArray&& o)
		noexcept(std::is_nothrow_move_constructible_v<allocator_type>) :a{ std::move(o.a) },
		p{ std::exchange(o.p,nullptr) }, sz{ o.sz }{}
	~DynamicArray() {
		if (p != nullptr) {
			for (std::size_t n = 0; n < sz; n++)
				std::allocator_traits<Allocator>::destroy(a, &p[n]);
			std::allocator_traits<Allocator>::deallocate(a, p, sz);
		}
	}
	DynamicArray& operator=(DynamicArray o) {
		swap(*this, o);
		return *this;
	}

	friend void swap(DynamicArray& arr1, DynamicArray& arr2)
		noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value||std::is_nothrow_swappable_v<Allocator>) {
		using std::swap;
		swap(arr1.p, arr2.p);
		swap(arr1.sz, arr2.sz);
		if constexpr(std::allocator_traits<Allocator>::propagate_on_container_swap::value)
			swap(arr1.a, arr2.a);
	}

	void swap(DynamicArray& arr) noexcept(std::allocator_traits<Allocator>::propagate_on_container_swap::value||std::is_nothrow_swappable_v<Allocator>){
		swap(*this,arr);
	}

	DynamicArray()noexcept(true) :p{ nullptr }, sz{ 0 }{}

	template< typename = std::enable_if_t<std::is_default_constructible_v<T>&&std::is_default_constructible_v<Allocator>>>
	explicit DynamicArray(size_type sz) noexcept(std::is_nothrow_default_constructible_v<T>&&std::is_nothrow_default_constructible_v<Allocator>) :sz{ sz } {
		p = std::allocator_traits<Allocator>::allocate(a, sz);
		for (std::size_t n = 0; n < sz; n++)
			std::allocator_traits<Allocator>::construct(a, &p[n]);
		p = std::launder(p);
	}

	template<typename Fn,std::enable_if_t<std::is_default_constructible_v<Allocator>&&std::is_constructible_v<T,std::invoke_result_t<Fn&&>>>* = 0>
		explicit DynamicArray(size_type sz,Fn&& fn) :sz{sz}{
			p = std::allocator_traits<Allocator>::allocate(a,sz);
			for(size_type n = 0;n<sz;n++)
				std::allocator_traits<Allocator>::construct(a,&p[n],std::invoke(std::forward<Fn>(fn)));
			p = std::launder(p);
		}
	template<typename Fn,std::enable_if_t<std::is_default_constructible_v<Allocator>&&std::is_constructible_v<T,std::invoke_result_t<Fn&&,size_type>>&&!std::is_invocable_v<Fn&&>>* = 0>
		explicit DynamicArray(size_type sz,Fn&& fn) :sz{sz}{
			p = std::allocator_traits<Allocator>::allocate(a,sz);
			for(size_type n = 0;n<sz;n++)
				std::allocator_traits<Allocator>::construct(a,&p[n],std::invoke(std::forward<Fn>(fn),n));
			p = std::launder(p);
		}

	template<typename = std::enable_if_t<std::is_default_constructible_v<T>>>
	explicit DynamicArray(size_type sz, allocator_type a) :sz{ sz }, a{ std::move(a) }{
		p = std::allocator_traits<Allocator>::allocate(a, sz);
		for (std::size_t n = 0; n < sz; n++)
			std::allocator_traits<Allocator>::construct(a, &p[n]);
		p = std::launder(p);
	}

	template<typename Fn,std::enable_if_t<std::is_constructible_v<T,std::invoke_result_t<Fn&&>>>* = 0>
		explicit DynamicArray(size_type sz,Fn&& fn,allocator_type a) :sz{sz},a{std::move(a)}{
			p = std::allocator_traits<Allocator>::allocate(a,sz);
			for(size_type n = 0;n<sz;n++)
				std::allocator_traits<Allocator>::construct(a,&p[n],std::invoke(std::forward<Fn>(fn)));
			p = std::launder(p);
		}
	template<typename Fn,std::enable_if_t<std::is_constructible_v<T,std::invoke_result_t<Fn&&,size_type>>&&!std::is_invocable_v<Fn&&>>* = 0>
		explicit DynamicArray(size_type sz,Fn&& fn,allocator_type a) :sz{sz},a{std::move(a)}{
			p = std::allocator_traits<Allocator>::allocate(a,sz);
			for(size_type n = 0;n<sz;n++)
				std::allocator_traits<Allocator>::construct(a,&p[n],std::invoke(std::forward<Fn>(fn),n));
			p = std::launder(p);
		}

	template<typename U, typename = std::enable_if_t<std::is_constructible_v<T, const U&>&&std::is_default_constructible_v<Allocator>>>
	explicit DynamicArray(const std::initializer_list<U>& il) :
		sz{ a.size() } {
		p = std::allocator_traits<Allocator>::allocate(a, sz);
		for (std::size_t n = 0; n < sz; n++)
			std::allocator_traits<Allocator>::construct(a, &p[n], il[n]);
		p = std::launder(p);
	}
	template<typename InputIt,
		typename = std::enable_if_t<std::is_constructible_v<
		T, typename std::iterator_traits<InputIt>::value_type>>>
		DynamicArray(InputIt begin, InputIt end) :sz{ std::distance(begin, end) } {
		p = std::allocator_traits<Allocator>::allocate(a, sz);
		for (std::size_t n = 0; n < sz; n++)
			std::allocator_traits<Allocator>::construct(a, &p[n], *begin++);
		p = std::launder(p);
	}
	template<typename Container,
		std::enable_if_t<
		std::is_constructible_v<DynamicArray, typename Container::const_iterator, typename Container::const_iterator>&&
		std::is_convertible_v<typename Container::value_type, T>&&std::is_default_constructible_v<Allocator>>* = 0>
		DynamicArray(const Container& c) :DynamicArray(c.cbegin(), c.cend()) {}

	template<typename Container,
		std::enable_if_t<
		std::is_constructible_v<DynamicArray, typename Container::const_iterator, typename Container::const_iterator> &&
		!std::is_convertible_v<typename Container::value_type, T>&&std::is_default_constructible_v<Allocator>>* = 0>
		explicit DynamicArray(const Container& c) :DynamicArray(c.cbegin(), c.cend()) {}
	template<typename... Tuple,
		typename = std::enable_if_t<std::conjunction_v<std::is_default_constructible<Allocator>, is_detected<detail::detect_constructible, T, Tuple>...>>
	>
		explicit DynamicArray(std::piecewise_construct_t, Tuple&&... t) :sz{ sizeof...(Tuple) } {
		p = std::allocator_traits<Allocator>::allocate(a, sz);
		detail::do_piecewise_construct(a, std::forward_as_tuple<Tuple...>(t...), p, std::index_sequence_for<Tuple...>{});
		p = std::launder(p);
	}

	size_type size()const noexcept(true) {
		return sz;
	}
	pointer data()noexcept(true) {
		return p;
	}
	const_pointer data()const noexcept(true) {
		return p;
	}

	reference operator[](size_type i) {
		return p[i];
	}
	const_reference operator[](size_type i)const {
		return p[i];
	}

	reference operator()(size_type i) {
		return p[i];
	}

	const_reference operator()(size_type i)const{
		return p[i];
	}

	iterator begin(){
		return p;
	}
	const_iterator begin()const{
		return p;
	}
	const_iterator cbegin()const{
		return p;
	}
	iterator end(){
		return p+sz;
	}
	const_iterator end()const{
		return p+sz;
	}
	const_iterator cend()const{
		return p+sz;
	}
	reverse_iterator rbegin(){
		return reverse_iterator{end()};
	}
	const_reverse_iterator rbegin()const{
		return const_reverse_iterator{end()};
	}
	const_reverse_iterator crbegin()const{
		return const_reverse_iterator{cend()};
	}
	reverse_iterator rend(){
		return reverse_iterator{begin()};
	}
	const_reverse_iterator rend()const{
		return reverse_iterator{begin()};
	}
	const_reverse_iterator crend()const{
		return reverse_iterator{cbegin()};
	}
};




#endif /* __INCLUDE_LCLIB_CXX_DYNAMICARRAY_HPP__2018_10_29_11_15_27 */
