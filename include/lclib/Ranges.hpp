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
		constexpr friend bool operator!=(const iterator& i1,const iterator& i2){
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
	constexpr typename iterator::difference_type size()const{
		return high-low;
	}
	constexpr bool empty()const{
		return high==low;
	}
};

template<typename T,T high,T low,typename=std::enable_if_t<low<high>>
	explicit numeric_range(std::integral_constant<T,low>,std::integral_constant<T,high>)->numeric_range<T,low,high>;

template<typename T,T low> explicit numeric_range(std::integral_constant<T,low>)->numeric_range<T,low,std::numeric_limits<T>::max()>;




#endif /* INCLUDE_LCLIB_RANGES_HPP__2018_12_354_08_55_35 */
