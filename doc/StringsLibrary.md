# Strings Library #
Included with `<lclib/Strings.hpp>`. 

## Synopsis ##

```cpp
template<typename CharT> constexpr CharT* strend(CharT*)noexcept;

template<typename CharT,typename PredicateFn> struct case_insensitive_order;

template<typename PredicateFn> struct case_insensitive_order<void,PredicateFn>;

```

### strend function ###

```cpp
template<typename CharT> constexpr CharT* strend(CharT* str)noexcept;
```

Given str is a pointer to a C-style string, returns str+n, where n is the smallest positive value of type `std::ptrdiff_t` such that `*(str+n)` is equal to CharT{}. Equalivalent to `const_cast<CharT*>(&*(std::basic_string_view<std::remove_cv_t<CharT>>{str}.end()))`. 
The behavior is undefined if [str,str+n) is not a valid range. 

This method only participates in overload resolution if `is_cstring_v<CharT*>` is true. 

### case\_insensitive\_order ###

```cpp
template<typename CharT=void,typename PredicateFn=std::less<CharT>> struct case_insensitive_order{
private:
	PredicateFn pred; //Exposition Only
public:
	using is_transparent = std::true_type;
    constexpr case_insensitive_order()=default;
    constexpr case_insensitive_order(const case_insensitive_order&)=default;
    constexpr case_insensitive_order& operator=(const case_insensitive_order&)=default
    bool operator()(CharT,CharT) const;
    template<typename CharTraits> bool operator()(std::basic_string_view<CharT,CharTraits>,std::basic_string_view<CharT,CharTraits>) const;
	template<typename CharTraits,typename Alloc1,typename Alloc2> bool operator()(const std::basic_string<CharT,CharTraits,Alloc1>&,const std::basic_string<CharT,CharTraits,Alloc2>&) const;
	template<typename CharTraits,typename Alloc> bool operator()(const std::basic_string<CharT,CharTraits,Aloc>&,std::basic_string_view<CharT,CharTraits>)const;
	template<typename CharTraits,typename Alloc> bool operator()(std::basic_string_view<CharT,CharTraits>,const std::basic_string<CharT,CharTraits,Aloc>&)const;
	bool operator()(const CharT*,const CharT*) const;
};
//Deduction Guides
case_insensitive_order() -> case_insensitive_order<void>;
```

#### Template Parameters ###

```
typename CharT=void //(1)
typename PredicateFn=std::less<CharT> //(2)
```

1. The Character type to compare. If CharT is not void, then the program is ill-formed unless `is_char_v<CharT>` is true. 
2. The Predicate Function. The Program is Ill-formed unless both of `std::is_default_constructible_v<PredicateFn>` and `std::is_invocable_r_v<bool,PredicateFn,CharT,CharT>` are true. The behavior is undefined if PredicateFn is not a Comparator that imposes Strict Total-Order for values of type CharT. 

#### Call Operators ###

```cpp
bool operator()(CharT c1,CharT c2) const; //(1)
template<typename CharTraits> bool operator()(std::basic_string_view<CharT,CharTraits> s1,std::basic_string_view<CharT,CharTraits> s2) const; //(2)
template<typename CharTraits,typename Alloc1,typename Alloc2> bool operator()(const std::basic_string<CharT,CharTraits,Alloc1>& s1,const std::basic_string<CharT,CharTraits,Alloc2>& s2) const; //(3)
template<typename CharTraits,typename Alloc> bool operator()(const std::basic_string<CharT,CharTraits,Aloc>& s1,std::basic_string_view<CharT,CharTraits> s2)const; //(4)
template<typename CharTraits,typename Alloc> bool operator()(std::basic_string_view<CharT,CharTraits> s1,const std::basic_string<CharT,CharTraits,Aloc>& s2)const; //(5)
bool operator()(const CharT* s1,const CharT* s2) const; //(6)
```

1. Compares c1 and c2 case insensitively. Calls `pred(toupper(c1,l),toupper(c2,l))` given `using std::toupper` and that l is a default constructed object of type `std::locale`. 
2. Calls `std::lexicographical_compare(s1.begin(),s1.end(),s2.begin(),s2.end(),std::ref(*this))`
3. Same as (2)
4. Same as (2)
5. Same as (2)
6. Calls `std::lexicographical_compare(s1,strend(s1),s2,strend(s2))`. 

The program is ill-formed, no diagnostic required, if CharT does not satisfy the following requirements:
Given:
* `c`, a value of type CharT
* `l`, a value of type std::locale

`toupper(c,l)` shall be a well formed expression that is convertible to `CharT`, where `toupper` is looked up using ADL on the `CharT` parameter or in namespace std.

#### Exceptions ####

(1) throws anything that the call to `toupper` throws, or that calling predicate on `toupper` 


#### case\_insensitive\_order&lt;void&gt; ####
Specialization of `case_insensitive_order` for generic types. 

`std::is_default_constructible_v<PredicateFn>` must be true or the program is ill-formed. 
Additionally, for at least one type `CharT`, for which `is_char_v<CharT>` is true and `CharT` satisfies the requirements as defined for the unspecialized version, `std::is_invocable_r_v<bool,PredicateFn,CharT,CharT>` must be true or the program is ill-formed, no diagnostic required. 

All comparison methods defined in `case_insensitive_order` are defined, except they are templated on `typename CharT`, and use that type instead of the class template parameter `CharT`. 
These methods do not participate in overload resolution if `is_char_v<CharT>` is false, if `CharT` does not satisfy the requirements for the unspecialized version, or `std::is_invocable_r_v<bool,PredicateFn,CharT,CharT>` is false. 

#### Deduction Guides ####

```cpp
case_insensitive_order() -> case_insensitive_order<void>;
```

As with all generic comparison functions, `case_insensitive_order` provides a specialization for void, and a deduction guide to select the specialization. 
