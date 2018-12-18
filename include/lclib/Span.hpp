/*
 * Span.hpp
 *
 *  Created on: Sep 27, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_SPAN_HPP__2018_09_27_09_16_01
#define __INCLUDE_LCLIB_CXX_SPAN_HPP__2018_09_27_09_16_01
#include <lclib/Config.hpp>
#include <lclib/TypeTraits.hpp>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include <algorithm>
#include <array>
constexpr const std::ptrdiff_t dynamic_extent{-1};

namespace detail{
	struct enabled_ctor_token_t{
		explicit constexpr enabled_ctor_token_t()=default;
	};
	constexpr const enabled_ctor_token_t enabled_ctor_token{};
	template<typename T,typename Container> constexpr auto enable_ctor_helper()
		->require_types_t<enabled_ctor_token_t,
			decltype(std::data(std::declval<Container&>())), decltype(std::size(std::declval<Container&>())),
			std::enable_if_t<std::is_convertible_v<
				std::remove_pointer_t<decltype(std::data(std::declval<Container&>()))>(*)[],
				T(*)[]
			 >>
		>
	{
		return enabled_ctor_token;
	}
	template<typename T> constexpr inline T& index_empty_span(){
		return *((T*)nullptr);//Shortcut to UB indexing empty spans
	}
}

/**
 * Implements span.
 * A Span is a View to a complete or part of a complete contiguous container.
 * All Iterators satisfy ContiguousIterator, ConstexprIterator, and RandomAccessIterator.
 * Span itself satisfies ContigousContainer, SequenceContainer, TriviallyCopyable, and LiteralType
 * By default, the extent of a span is unknown or dynamic.
 * If the extent of the span is known, use span<T,extent>.
 * Because a span does not own the memory it views,
 * there is no direct distinction between const-qualfied spans.
 * All Non-const iterators are mutable unless T is const-qualified.
 */
template<typename T,std::ptrdiff_t extent=dynamic_extent> struct span{
public:
	static_assert(extent>0,"Extent must be 0, positive, or dynamic_extent");
	using element_type = T;
	using value_type = std::remove_cv_t<T>;
	using index_type = std::ptrdiff_t;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator = element_type*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
	T* __ptr;
public:
	constexpr span(pointer start,index_type count) EXPECTS(start!=nullptr&&count==extent):__ptr(start){}
	constexpr span(pointer begin,pointer end) EXPECTS(start!=nullptr&&end!=nullptr&&(end-begin)==extent):__ptr(begin){}
	constexpr span(element_type(&arr)[extent],
			  detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(arr)>())noexcept(true):__ptr(arr){}
	constexpr span(std::array<value_type,extent>& arr,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(arr)>())noexcept(true):__ptr(std::data(arr)){}
	constexpr span(const std::array<value_type,extent>& arr,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(arr)>())noexcept(true):__ptr(std::data(arr)){}
	template<typename Container>
		constexpr span(Container& c,
				detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(c)>()) EXPECTS(std::size(c)==extent):__ptr(std::data(c)){}
	template<typename Container>
			constexpr span(const Container& c,
					detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(c)>()) EXPECTS(std::size(c)==extent):__ptr(std::data(c)){}
	template<typename U,typename=std::enable_if_t<std::is_convertible_v<U(*)[],T(*)[]>>>
		constexpr span(const span<U,extent>& e)noexcept(true):__ptr(e.__ptr){}
	constexpr span(const span&)noexcept(true)=default;
	constexpr iterator begin()const noexcept(true){
		return __ptr;
	}
	constexpr iterator end()const noexcept(true){
		return __ptr+extent;
	}
	constexpr const_iterator cbegin()const noexcept(true){
		return __ptr;
	}
	constexpr const_iterator cend()const noexcept(true){
		return __ptr+std::size_t(extent);
	}
	constexpr reverse_iterator rbegin()const noexcept(true){
		return reverse_iterator{end()};
	}
	constexpr reverse_iterator rend()const noexcept(true){
		return reverse_iterator{begin()};
	}
	constexpr const_reverse_iterator crbegin()const noexcept(true){
		return const_reverse_iterator{cend()};
	}
	constexpr const_reverse_iterator crend()const noexcept(true){
		return const_reverse_iterator{cbegin()};
	}
	constexpr pointer data()const noexcept(true){
		return __ptr;
	}
	constexpr index_type size()const noexcept(true){
		return extent;
	}
	constexpr index_type size_bytes()const noexcept(true){
		return extent*sizeof(T);
	}
	constexpr bool empty()const noexcept(true){
		return false;
	}
	template<std::ptrdiff_t Count> constexpr span<element_type,Count> first()const EXPECTS(Count<=extent){
		return span<element_type,Count>(data(),Count);
	}
	constexpr span<element_type,dynamic_extent> first(std::ptrdiff_t count)const EXPECTS(count<=extent){
		return span<element_type,dynamic_extent>(data(),count);
	}
	template<std::ptrdiff_t Count> constexpr span<element_type,Count> last()const EXPECTS(Count<=extent){
		return span<element_type,Count>(end()-Count,end());
	}
	constexpr span<element_type,dynamic_extent> last(std::ptrdiff_t count)const EXPECTS(count<=extent){
		return span<element_type,dynamic_extent>(end()-count,end());
	}
	template<std::ptrdiff_t Offset,std::ptrdiff_t Count=dynamic_extent>
		constexpr span<element_type,Count==dynamic_extent?extent-Offset:Count> subspan()const
					EXPECTS(Offset>=0&&Offset<=extent&&(Count==dynamic_extent||(Offset+Count<=extent))){
		if constexpr(Count==dynamic_extent)
			return span<element_type,extent-Offset>(begin()+Offset,end());
		else
			return span<element_type,Count>(begin()+Offset,Count);
	}
	constexpr span<element_type,dynamic_extent> subspan(std::ptrdiff_t offset,std::ptrdiff_t count=dynamic_extent) const
		EXPECTS(offset>=0&&offset<=extent&&(count==dynamic_extent||(offset+count<=extent))){
		return span<element_type,dynamic_extent>(begin()+offset,count==dynamic_extent?size()-offset:count);
	}

	constexpr reference operator[](index_type idx)const EXPECTS(idx>=0&&idx<extent){
		return __ptr[idx];
	}
	constexpr reference operator()(index_type idx)const EXPECTS(idx>=0&&idx<extent){
		return __ptr[idx];
	}
};

template<typename T> struct span<T,0>{
public:
	using element_type = T;
	using value_type = std::remove_cv_t<T>;
	using index_type = std::ptrdiff_t;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	constexpr span()noexcept(true){}
	constexpr span(pointer start,index_type size) EXPECTS(size==0){}
	constexpr span(pointer begin,pointer end) EXPECTS(begin==end){}
	template<typename Container> constexpr span(Container& c,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(c)>()) EXPECTS(std::size(c)==0){}
	template<typename Container> constexpr span(const Container& c,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(c)>()) EXPECTS(std::size(c)==0){}
	template<typename U,typename=std::enable_if_t<std::is_convertible_v<U(*)[],T(*)[]>>>
			constexpr span(const span<U,0>&)noexcept(true){}
	constexpr span(const span&)noexcept(true)=default;

	constexpr iterator begin()const{
		return nullptr;
	}
	constexpr iterator end()const{
		return nullptr;
	}
	constexpr const_iterator cbegin()const{
		return nullptr;
	}
	constexpr const_iterator cend()const{
		return nullptr;
	}
	constexpr reverse_iterator rbegin()const{
		return reverse_iterator{end()};
	}
	constexpr reverse_iterator rend()const{
		return reverse_iterator{begin()};
	}
	constexpr pointer data()const{
		return nullptr;
	}
	constexpr index_type size()const{
		return 0;
	}
	constexpr index_type size_bytes()const{
		return 0;
	}
	constexpr bool empty()const{
		return true;
	}
	constexpr reference operator[](index_type idx)const noexcept(true) EXPECTS(false){
		return detail::index_empty_span<T>();
	}
	constexpr reference operator()(index_type idx)const noexcept(true) EXPECTS(false){
		return detail::index_empty_span<T>();
	}
	template<std::ptrdiff_t Count> constexpr span<element_type,Count> first()const EXPECTS(Count==0){
		return span<element_type,Count>(data(),Count);
	}
	template<std::ptrdiff_t Count> constexpr span<element_type,Count> last()const EXPECTS(Count==0){
		return span<element_type,Count>(end()-Count,end());
	}
	constexpr span<element_type,dynamic_extent> first(std::ptrdiff_t count)const EXPECTS(count==0){
		return span<element_type,dynamic_extent>(data(),count);
	}
	constexpr span<element_type,dynamic_extent> last(std::ptrdiff_t count)const EXPECTS(count==0){
		return span<element_type,dynamic_extent>(end()-count,end());
	}
	template<std::ptrdiff_t Offset,std::ptrdiff_t Count=dynamic_extent>
		constexpr span<element_type,Count==dynamic_extent?0:Count> subspan()const EXPECTS(Offset==0&&(Count==0||Count==dynamic_extent)){
		if constexpr(Count==dynamic_extent)
			return *this;
		return first<Count>();
	}
	constexpr span<element_type,dynamic_extent> subspan(std::ptrdiff_t off,std::ptrdiff_t count=dynamic_extent)const EXPECTS(off==0&&(count==0||count==dynamic_extent)){
		return first(count);
	}
};

template<typename T> struct span<T,dynamic_extent>{
public:
	using element_type = T;
	using value_type = std::remove_cv_t<T>;
	using index_type = std::ptrdiff_t;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
	pointer __ptr;
	std::size_t __size;
public:
	constexpr span() noexcept(true):__ptr(nullptr),__size(0){}
	constexpr span(pointer start,index_type sz) EXPECTS(sz>=0&&(sz!=0||start!=nullptr)):__ptr(start),__size(sz){}
	constexpr span(pointer start,pointer end) EXPECTS(end>=start&&(end==start||(end!=nullptr&&start!=null))):__ptr(start),__size(end-start){}
	template<std::size_t N> constexpr span(element_type(&arr)[N],
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(arr)>())noexcept(true):__ptr(arr),__size(N){}
	template<std::size_t N> constexpr span(std::array<value_type,N>& arr,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(arr)>())noexcept(true):__ptr(arr),__size(N){}
	template<std::size_t N> constexpr span(const std::array<value_type,N>& arr,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(arr)>())noexcept(true):__ptr(arr),__size(N){}
	template<typename Container> constexpr span(Container& c,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(c)>()):__ptr(std::data(c)),__size(std::size(c)){}
	template<typename Container> constexpr span(const Container& c,
				detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(c)>()):__ptr(std::data(c)),__size(std::size(c)){}
	template<typename U,std::ptrdiff_t extent> constexpr span(const span<U,extent>& s,
			detail::enabled_ctor_token_t=detail::enable_ctor_helper<T,decltype(s)>())noexcept(true):__ptr(std::data(s)),__size(std::size(s)){}
	constexpr span(const span&)noexcept(true)=default;

	constexpr pointer data()const noexcept(true){
		return __ptr;
	}
	constexpr index_type size()const noexcept(true){
		return __size;
	}
	constexpr index_type size_bytes()const noexcept(true){
		return __size*sizeof(T);
	}
	constexpr reference operator[](index_type idx)const noexcept(true) EXPECTS(idx>=0&&idx<__size){
		return __ptr[idx];
	}
	constexpr reference operator()(index_type idx)const noexcept(true) EXPECTS(idx>=0&&idx<__size){
		return __ptr[idx];
	}

	constexpr iterator begin()const noexcept(true){
		return __ptr;
	}
	constexpr iterator end()const noexcept(true){
		return __ptr+__size;
	}
	constexpr const_iterator cbegin()const noexcept(true){
		return __ptr;
	}
	constexpr const_iterator cend()const noexcept(true){
		return __ptr+__size;
	}
	constexpr reverse_iterator rbegin()const noexcept(true){
		return reverse_iterator{end()};
	}
	constexpr reverse_iterator rend()const noexcept(true){
		return reverse_iterator{begin()};
	}
	constexpr const_reverse_iterator crbegin()const noexcept(true){
		return const_reverse_iterator{cend()};
	}
	constexpr const_reverse_iterator crend()const noexcept(true){
		return const_reverse_iterator{cbegin()};
	}
	template<std::ptrdiff_t Count> constexpr span<element_type,Count> first()const noexcept(true) EXPECTS(Count>=0&&Count<__size){
		return span<element_type,Count>(begin(),Count);
	}
	constexpr span<element_type,dynamic_extent> first(std::ptrdiff_t count)const noexcept(true) EXPECTS(count>=0&&count<__size){
		return span<element_type,dynamic_extent>(begin(),count);
	}
	template<std::ptrdiff_t Count> constexpr span<element_type,Count> last()const noexcept(true) EXPECTS(Count>=0&&Count<__size){
		return span<element_type,Count>(end()-Count,end());
	}
	constexpr span<element_type,dynamic_extent> last(std::ptrdiff_t count)const noexcept(true) EXPECTS(count>=0&&count<__size){
		return span<element_type,dynamic_extent>(end()-count,end());
	}
	template<std::ptrdiff_t Offset,std::ptrdiff_t Count=dynamic_extent>
		constexpr span<element_type,Count> subspan()const noexcept(true)
		EXPECTS(Offset>=0&&Offset<=size()&&(Count==dynamic_extent||(Offset+Count<=size()))){
		return span<element_type,Count>(begin()+Offset,Count==dynamic_extent?size()-Offset:Count);
	}
	constexpr span<element_type,dynamic_extent> subspan(std::ptrdiff_t offset,std::ptrdiff_t count=dynamic_extent)const noexcept(true)
		EXPECTS(offset>=0&&offset<=size()&&(count==dynamic_extent||(offset+count<=size()))){
		return span<element_type,dynamic_extent>(begin()+offset,count==dynamic_extent?size()-offset:count);
	}
	constexpr bool empty()const noexcept(true){
		return __size==0;
	}
};

template<typename T,std::ptrdiff_t extent,std::ptrdiff_t S=(extent==dynamic_extent?dynamic_extent:extent*sizeof(T))>
	span<const std::byte,S> as_bytes(const span<T,extent>& s){
	return span<const std::byte,S>{reinterpret_cast<const std::byte*>(s.data()),s.size_bytes()};
}
template<typename T,std::ptrdiff_t extent,std::ptrdiff_t S=(extent==dynamic_extent?dynamic_extent:extent*sizeof(T))>
	std::enable_if_t<!std::is_const_v<T>,span<std::byte,S>> as_writeable_bytes(const span<T,extent>& s){
	return span<std::byte,S>{reinterpret_cast<std::byte*>(s.data()),s.size_bytes()};
}

template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
	constexpr bool operator==(span<T1,E1> s1,span<T2,E2> s2){
	return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end());
}
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
	constexpr bool operator<(span<T1,E1> s1,span<T2,E2> s2){
	return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end());
}

template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
	constexpr bool operator!=(span<T1,E1> s1,span<T2,E2> s2){
	return !(s1==s2);
}

template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
	constexpr bool operator<=(span<T1,E1> s1,span<T2,E2> s2){
	return s1<s2||s1==s2;
}
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
	constexpr bool operator>=(span<T1,E1> s1,span<T2,E2> s2){
	return !(s1<s2);
}
template<typename T1,std::ptrdiff_t E1,typename T2,std::ptrdiff_t E2>
	constexpr bool operator>(span<T1,E1> s1,span<T2,E2> s2){
	return !(s1<=s2);
}

template<typename T,std::ptrdiff_t N> span(span<T,N>)->span<T,N>;
template<typename T,std::size_t N> span(T(&)[N]) -> span<T,N>;
template<typename T,std::size_t N> span(std::array<T,N>&) -> span<T,N>;
template<typename T,std::size_t N> span(const std::array<T,N>&) -> span<const T,N>;
template<typename T> span(T*,T*) -> span<T>;
template<typename T> span(T*,std::size_t) ->span<T>;
template<typename Container> span(Container&)->span<std::remove_reference_t<decltype(*std::data(std::declval<Container&>()))>>;
template<typename Container> span(const Container& c)->span<std::remove_reference_t<decltype(*std::data(std::declval<const Container&>()))>>;

#endif /* __INCLUDE_LCLIB_CXX_SPAN_HPP__2018_09_27_09_16_01 */
