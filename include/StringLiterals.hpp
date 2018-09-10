/*
 * StringLiterals.hpp
 *
 *  Created on: Sep 10, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_STRINGLITERALS_HPP__2018_09_10_08_27
#define __INCLUDE_STRINGLITERALS_HPP__2018_09_10_08_27

#include <cstddef>
#include <iterator>

class StringLiteral{
private:
	const char* __begin;
	const char* __end;
	typedef char value_type;
	typedef const char* pointer;
	typedef const char* iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
public:
	template<size_t N> constexpr StringLiteral(const char(&str)[N]):__begin(str),
			__end(str+N){}
	constexpr StringLiteral(const char(&)[0]):__begin(nullptr),__end(nullptr){}
	constexpr operator const char*()const{
		return __begin;
	}
	constexpr const char* data()const{
		return __begin;
	}
	constexpr pointer c_str()const{
		return __begin;
	}
	constexpr value_type operator[](std::size_t n)const{
		return __begin[n];
	}
	constexpr iterator begin()const{
		return __begin;
	}
	constexpr iterator end()const{
		return __end;
	}
	constexpr iterator cbegin()const{
		return __begin;
	}
	constexpr iterator cend()const{
		return __end;
	}
	constexpr reverse_iterator rbegin()const{
		return reverse_iterator{end()};
	}
	constexpr reverse_iterator rend()const{
		return reverse_iterator{begin()};
	}
#if __cplusplus > 201703 //C++2a
	constexpr auto operator<=>(const StringLiteral&)const=default;
#endif
};



#endif /* __INCLUDE_STRINGLITERALS_HPP__2018_09_10_08_27 */
