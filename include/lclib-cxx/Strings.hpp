/*
 * Strings.hpp
 *
 *  Created on: Oct 5, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32
#define __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32
#include <lclib-cxx/Config.hpp>
#include <string>
#include <locale>
/**
 * Case Insensitive comparator for Strings and char types.
 * Specifically, a given specicialization of case_insenitive_order can Compare
 * basic_strings with the given CharT, and CharTraits, and any Allocator (even basic_strings with different allocators),
 * basic_string_views with the given CharT and CharTraits,
 *  and Values of type CharT.
 * Compares using CharTraits::lt, in a case insensitive manner.
 */
template<typename CharT,typename CharTraits=std::char_traits<CharT>,typename CType=std::ctype<CharT>,typename Predicate=std::less<CharT>> struct case_insensitive_order{
public:
	template<typename A1,typename A2> bool operator()(const std::basic_string<CharT,CharTraits,A1>& s1,const std::basic_string<CharT,CharTraits,A2>& s2)const{
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),*this);
	}
	bool operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string_view<CharT,CharTraits>& sv2)const{
		return std::lexicographical_compare(sv1.begin(),sv1.end(),sv2.begin(),sv2.end(),*this);
	}
	template<typename Alloc> bool operator()(const std::basic_string<CharT,CharTraits,Alloc>& s1,const std::basic_string_view<CharT,CharTraits>& sv2)const{
		return std::lexicographical_compare(s1.begin(),s1.end(),sv2.begin(),sv2.end(),*this);
	}
	template<typename Alloc> bool operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string<CharT,CharTraits,Alloc>& s2)const{
		return std::lexicographical_compare(sv1.begin(),sv1.end(),s2.begin(),s2.end());
	}
	bool operator()(CharT c1,CharT c2)const{
		CType ctype;
		Predicate pred;
		return pred(ctype.to_upper(c1),ctype.to_upper(c2));
	}
};



#endif /* __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32 */
