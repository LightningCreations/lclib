/*
 * Strings.hpp
 *
 *  Created on: Oct 5, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32
#define __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32
#include <lclib/Config.hpp>
#include <string>
#include <locale>
#include <lclib/TypeTraits.hpp>

template<typename CharT> constexpr CharT* strend(CharT* cstr){
	return const_cast<CharT*>(std::basic_string_view<CharT>{cstr}.end());
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
template<typename CharT=void,typename Predicate=std::less<CharT>> struct case_insensitive_order{
private:
	static_assert(is_char_v<CharT>,"case_insensitive_order requires a Char Type");
	static_assert(std::is_default_constructible_v<Predicate>,"Predicate must be default constructible");
	static_assert(std::is_invocable_r_v<bool,Predicate,CharT,CharT>,"Predicate must be a Binary Predicate accepting values of type CharT");
	Predicate pred;
public:
	using is_transparent = std::true_type;
	constexpr case_insensitive_order()=default;
	constexpr case_insensitive_order(const case_insensitive_order&)=default;
	constexpr case_insensitive_order& operator=(const case_insensitive_order&)=default;
	template<typename A1,typename A2,typename CharTraits> bool operator()(const std::basic_string<CharT,CharTraits,A1>& s1,const std::basic_string<CharT,CharTraits,A2>& s2)const{
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),std::ref(*this));
	}
	template<typename CharTraits> bool operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string_view<CharT,CharTraits>& sv2)const{
		return std::lexicographical_compare(sv1.begin(),sv1.end(),sv2.begin(),sv2.end(),std::ref(*this));
	}
	template<typename Alloc,typename CharTraits> bool operator()(const std::basic_string<CharT,CharTraits,Alloc>& s1,const std::basic_string_view<CharT,CharTraits>& sv2)const{
		return std::lexicographical_compare(s1.begin(),s1.end(),sv2.begin(),sv2.end(),std::ref(*this));
	}
	template<typename Alloc,typename CharTraits> bool operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string<CharT,CharTraits,Alloc>& s2)const{
		return std::lexicographical_compare(sv1.begin(),sv1.end(),s2.begin(),s2.end(),std::ref(*this));
	}
	bool operator()(const CharT* s1,const CharT* s2){
		return (*this)(std::basic_string_view<CharT>{s1},std::basic_string_view<CharT>{s2});
	}
	bool operator()(CharT c1,CharT c2)const{
		std::locale l{};
		return pred(std::toupper(c1, l),toupper(c2,l));
	}
};

template<typename Predicate> struct case_insensitive_order<void,Predicate>{
private:
	static_assert(std::is_default_constructible_v<Predicate>,"Predicate must be default constructible");
	Predicate pred;
public:
	using is_transparent = std::true_type;
	constexpr case_insensitive_order()=default;
	constexpr case_insensitive_order(const case_insensitive_order&)=default;
	constexpr case_insensitive_order& operator=(const case_insensitive_order&)=default;
	template<typename CharT,typename A1,typename A2,typename CharTraits> auto operator()(const std::basic_string<CharT,CharTraits,A1>& s1,const std::basic_string<CharT,CharTraits,A2>& s2)const
	 -> decltype((*this)(s1[0],s2[0])){
		return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),std::ref(*this));
	}
	template<typename CharT,typename CharTraits> auto operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string_view<CharT,CharTraits>& sv2)const
	-> decltype((*this)(sv1[0],sv2[0])){
		return std::lexicographical_compare(sv1.begin(),sv1.end(),sv2.begin(),sv2.end(),std::ref(*this));
	}
	template<typename CharT,typename Alloc,typename CharTraits> auto operator()(const std::basic_string<CharT,CharTraits,Alloc>& s1,const std::basic_string_view<CharT,CharTraits>& sv2)const
	-> decltype((*this)(s1[0],sv2[0])){
		return std::lexicographical_compare(s1.begin(),s1.end(),sv2.begin(),sv2.end(),std::ref(*this));
	}
	template<typename CharT,typename Alloc,typename CharTraits> auto operator()(const std::basic_string_view<CharT,CharTraits>& sv1,const std::basic_string<CharT,CharTraits,Alloc>& s2)const
	-> decltype((*this)(sv1[0],s2[0])){
		return std::lexicographical_compare(sv1.begin(),sv1.end(),s2.begin(),s2.end(),std::ref(*this));
	}
	template<typename CharT> auto operator()(const CharT* s1,const CharT* s2)const
	-> decltype((*this)(s1[0],s2[0])){
		return (*this)(std::basic_string_view<CharT>{s1},std::basic_string_view<CharT>{s2});
	}
	template<typename CharT> auto operator()(CharT c1,CharT c2)const -> std::enable_if_t<is_char_v<CharT>&&std::is_invocable_r_v<bool,Predicate,CharT,CharT>,bool>{
		using std::toupper;
		std::locale l{};
		return pred(toupper(c1, l),toupper(c2,l));
	}
};


case_insensitive_order() -> case_insensitive_order<void,std::less<void>>;



#endif /* __INCLUDE_LCLIB_CXX_STRINGS_HPP__2018_10_05_15_23_32 */
