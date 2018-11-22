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
#include <lclib-cxx/TypeTraits.hpp>

template<typename CharT> constexpr CharT* strend(CharT* cstr){
	for(;*cstr!=0;cstr++);
	return cstr;
}

template<typename CharT> constexpr const CharT* strend(const CharT* cstr){
	for(;*cstr!=0;cstr++);
	return cstr;
}



/**
 * Case Insensitive comparator for Strings and char types.
 * Specifically, a given specialization of case_insenitive_order can Compare
 * basic_strings with the given CharT, and CharTraits, and any Allocator (even basic_strings with different allocators),
 * basic_string_views with the given CharT and CharTraits,
 * values of CharT,
 * and between any of such types except cross-comparisons with CharT.
 * Compares using the Predicate
 * \template parameter CharT: The Character type. Must satisfy CharacterTypetype
 * \template parameter CharTraits: The Traits to use (defaults to std::char_traits<CharT>)
 * 	Must satisfy CharTraits, DefaultConstructible, and have typename CharTraits::char_type be CharT
 * \template parameter Predicate: The Underlying Comparator Predicate to use (defaults to std::less<CharT>)
 */
template<typename CharT,typename CharTraits=std::char_traits<CharT>,typename Predicate=std::less<CharT>> struct case_insensitive_order{
private:
	static_assert(is_char_v<CharT>,"case_insensitive_order requires a Char Type");
	static_assert(std::is_default_constructible_v<Predicate>,"Predicate must be default constructible");
	static_assert(std::is_same_v<CharT,typename CharTraits::char_type>,"CharTraits have CharT as its character type");
	Predicate pred;
public:
	case_insensitive_order()=default;
	case_insensitive_order(const case_insensitive_order&)=default;
	case_insensitive_order& operator=(const case_insensitive_order&)=default;
	template<typename A1,typename A2> bool operator()(const std::basic_string<CharT,CharTraits,A1>& s1,const std::basic_string<CharT,CharTraits,A2>& s2)const{
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),std::ref(*this));
	}
	bool operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string_view<CharT,CharTraits>& sv2)const{
		return std::lexicographical_compare(sv1.begin(),sv1.end(),sv2.begin(),sv2.end(),std::ref(*this));
	}
	template<typename Alloc> bool operator()(const std::basic_string<CharT,CharTraits,Alloc>& s1,const std::basic_string_view<CharT,CharTraits>& sv2)const{
		return std::lexicographical_compare(s1.begin(),s1.end(),sv2.begin(),sv2.end(),std::ref(*this));
	}
	template<typename Alloc> bool operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string<CharT,CharTraits,Alloc>& s2)const{
		return std::lexicographical_compare(sv1.begin(),sv1.end(),s2.begin(),s2.end(),std::ref(*this));
	}
	bool operator()(const CharT* s1,const CharT* s2){
		return std::lexicographical_compare(s1,strend(s1),s2,strend(s2),std::ref(*this));
	}
	bool operator()(CharT c1,CharT c2)const{
		std::locale l{};
		return pred(std::toupper(c1, l),toupper(c2,l));
	}
};



#endif /* __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32 */
