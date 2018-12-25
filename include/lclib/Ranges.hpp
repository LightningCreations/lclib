/*
 * Ranges.hpp
 *
 *  Created on: Dec. 20, 2018
 *      Author: connor
 */

#ifndef INCLUDE_LCLIB_RANGES_HPP__2018_12_354_08_55_35
#define INCLUDE_LCLIB_RANGES_HPP__2018_12_354_08_55_35

#include <iterator>
#include <limits>
#include <utility>
#include <type_traits>
#include <lclib/Detectors.hpp>

namespace detail{
	template<typename T,typename IntegralConstant> using detect_representable = decltype(T{IntegralConstant::value});
	template<typename Range> using detect_reverse_iterator = typename Range::reverse_iterator;
	template<typename Range> using detect_const_reverse_iterator = typename Range::const_reverse_iterator;
	template<typename Range> using detect_rbegin = decltype(std::declval<Range>().rbegin());
	template<typename Range> using detect_crbegin = decltype(std::declval<Range>().crbegin());
}


template<typename T,T low=std::numeric_limits<T>::min(),T high=std::numeric_limits<T>::max()> struct numeric_range{
public:
	static_assert(low<high,"low must be less than high");
	using low_constant = std::integral_constant<T,low>;
	using high_constant = std::integral_constant<T,high>;
	struct iterator{
	private:
		T value;
	public:
		using value_type = T;
		using pointer = const T*;
		using reference = value_type;
		using difference_type = decltype(std::declval<const T>()-std::declval<const T>());
		using iterator_category = std::random_access_iterator_tag;
		constexpr explicit iterator(T val):value{val}{}
		constexpr explicit iterator():value{high}{}
		constexpr iterator(const iterator&)=default;
		constexpr iterator(iterator&&)=default;
		constexpr iterator& operator=(const iterator&)=default;
		constexpr iterator& operator=(iterator&&)=default;
		constexpr value_type operator*()const{
			return value;
		}
		constexpr iterator& operator++(){
			++value;
			return *this;
		}
		constexpr iterator operator++(int){
			return iterator{value++};
		}
		constexpr iterator& operator--(){
			--value;
			return *this;
		}
		constexpr iterator operator--(int){
			return iterator{value--};
		}
		constexpr iterator operator+(difference_type t)const{
			return iterator{static_cast<T>(value+t)};
		}
		constexpr iterator& operator+=(difference_type t){
			value+=t;
			return *this;
		}
		constexpr iterator& operator-=(difference_type t){
			value-=t;
			return *this;
		}
		constexpr iterator operator-(difference_type t)const{
			return iterator{static_cast<T>(value-t)};
		}
		constexpr difference_type operator-(const iterator& i)const{
			return value-i.value;
		}
		constexpr bool operator==(const iterator& i)const{
			return value==i.value;
		}
		constexpr friend bool operator!=(const iterator& i1,const iterator& i2)const{
			return !(i1==i2);
		}
		constexpr value_type operator[](difference_type t)const{
			return value+t;
		}
	};
	using const_iterator = iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using value_type = T;

	constexpr explicit numeric_range(){}
	constexpr explicit numeric_range(std::integral_constant<T,low>,std::integral_constant<T,high> = std::integral_constant<T,high>{}){}
	constexpr iterator begin()const{
		return iterator{low};
	}
	constexpr iterator end()const{
		return iterator{};
	}
	constexpr const_iterator cbegin()const{
		return iterator{low};
	}
	constexpr const_iterator cend()const{
		return iterator{};
	}
	constexpr reverse_iterator rbegin()const{
		return reverse_iterator{end()};
	}
	constexpr reverse_iterator rend()const{
		return reverse_iterator{begin()};
	}
	constexpr const_reverse_iterator crbegin()const{
		return const_reverse_iterator{cend()};
	}
	constexpr const_reverse_iterator crend()const{
		return const_reverse_iterator{cbegin()};
	}
	constexpr iterator::difference_type size()const{
		return high-low;
	}
	constexpr bool empty()const{
		return high==low;
	}
};

template<typename T,T high,T low,typename=std::enable_if_t<low<high>>
	explicit numeric_range(std::integral_constant<T,low>,std::integral_constant<T,high>)->numeric_range<T,low,high>;

template<typename T,T low> explicit numeric_range(std::integral_constant<T,low>)->numeric_range<T,low,std::numeric_limits<T>::max()>;


template<typename Range> struct reverse{
private:
	template<bool> struct select_existing_reverse_iterator{
		using reverse_iterator = typename Range::reverse_iterator;
	};
	template<bool> struct select_existing_const_reverse_iterator{
		using const_reverse_iterator = typename Range::const_reverse_iterator;
	};
	template<> struct select_existing_reverse_iterator<false>{
		using reverse_iterator = std::reverse_iterator<typename Range::iterator>;
	};
	template<> struct select_existing_const_reverse_iterator<false>{
		using const_reverse_iterator = std::reverse_iterator<typename Range::const_iterator>;
	};
public:
	using value_type = typename Range::value_type;

private:
	Range r;
	auto can_reverse(std::bidirectional_iterator_tag)->std::true_type;
	auto can_reverse(...)->std::false_type;


	static_assert(decltype(can_reverse(typename std::iterator_traits<typename Range::iterator>::iterator_category{}))::value,"");
public:
	explicit constexpr reverse(Range&& r):r{std::forward<Range>(r)}{}
	template<std::enable_if_t<is_detected_v<detail::detect_rbegin,Range>>* =0>
		constexpr decltype(auto) begin() noexcept(noexcept(r.rbegin())){
		return r.rbegin();
	}
	template<std::enable_if_t<is_detected_v<detail::detect_rbegin,const Range>>* =0>
		decltype(auto) begin() const noexcept(noexcept(r.rbegin())){
		return r.rbegin();
	}
	template<std::enable_if_t<is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) cbegin() const noexcept(noexcept(r.crbegin())){
		return r.rbegin();
	}
	template<std::enable_if_t<!is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) begin() noexcept(noexcept(std::reverse_iterator{r.end()})){
		return std::reverse_iterator{r.end()};
	}
	template<std::enable_if_t<!is_detected_v<detail::detect_rbegin,const Range>>* =0>
	constexpr decltype(auto) begin() const noexcept(noexcept(std::reverse_iterator{r.end()})){
		return std::reverse_iterator{r.end()};
	}
	template<std::enable_if_t<!is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) cbegin() const noexcept(noexcept(std::reverse_iterator{r.cend()})){
		return std::reverse_iterator{r.cend()};
	}
	template<std::enable_if_t<is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) end() noexcept(noexcept(r.rend())){
		return r.rend();
	}
	template<std::enable_if_t<is_detected_v<detail::detect_rbegin,const Range>>* =0>
	constexpr decltype(auto) end() const noexcept(noexcept(r.rend())){
		return r.rend();
	}
	template<std::enable_if_t<is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) cend() const noexcept(noexcept(r.crend())){
		return r.rbend();
	}
	template<std::enable_if_t<!is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) end() noexcept(noexcept(std::reverse_iterator{r.begin()})){
		return std::reverse_iterator{r.begin()};
	}
	template<std::enable_if_t<!is_detected_v<detail::detect_rbegin,const Range>>* =0>
	constexpr decltype(auto) end() const noexcept(noexcept(std::reverse_iterator{r.begin()})){
		return std::reverse_iterator{r.begin()};
	}
	template<std::enable_if_t<!is_detected_v<detail::detect_rbegin,Range>>* =0>
	constexpr decltype(auto) cend() const noexcept(noexcept(std::reverse_iterator{r.cbegin()})){
		return std::reverse_iterator{r.cbegin()};
	}
	constexpr decltype(auto) rbegin()noexcept(noexcept(r.begin())){
		return r.begin();
	}
	constexpr decltype(auto) rbegin()const noexcept(noexcept(r.begin())){
		return r.begin();
	}
	constexpr decltype(auto) crbegin()const noexcept(noexcept(r.cbegin())){
		return r.cbegin();
	}
	constexpr decltype(auto) rend()noexcept(noexcept(r.end())){
		return r.begin();
	}
	constexpr decltype(auto) rend()const noexcept(noexcept(r.end())){
		return r.begin();
	}
	constexpr decltype(auto) crend()const noexcept(noexcept(r.cend())){
		return r.cbegin();
	}
	using iterator = decltype(std::declval<reverse>().begin());
	using const_iterator = decltype(std::declval<const reverse>().cbegin());
	using reverse_iterator = decltype(std::declval<reverse>().rbegin());
	using const_reverse_iterator = decltype(std::declval<const reverse>().crbegin());
};

template<typename Range> reverse(Range)->reverse<Range>;
template<typename Range> reverse(Range&)->reverse<Range&>;


#endif /* INCLUDE_LCLIB_RANGES_HPP__2018_12_354_08_55_35 */
