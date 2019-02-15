# C Varargs Interface #

In header `<lclib/CVarargsInterface.hpp>`

## Header Synopsis ##

```cpp
template<typename... Args> std::tuple<Args...> va_expand(va_list l);
template<typename T> struct varargs_iterator;
template<typename T> struct varargs_range;
```

### va_expand Function ###

```cpp
template<typename... Args> std::tuple<Args...> va_expand(va_list l);
```

Expands a varargs list into a tuple of Arguments.


Given that Args... Expands to `T_1,T_2,...T_n` where `n` is `sizeof...(Args)`:

If any of `T_1,T_2,...,T_n` does not satisfy *TriviallyCopyable* then this function is defined as deleted. 

Otherwise, returns `t`, where the element `t_i` of `t` has the type `T_i` for `i` in [1,n],  is copy constructed from `va_arg(l,PT_i)`, where `PT_i` is determined as follows:
* If `T_i` is a scoped enumeration type, a class type, or a union type, then `PT_i` is `T_i`.
* If `T_i` is float, then `PT_i` is double
* If `T_i` is char, signed char or short, then `PT_i` is int
* If `T_i` is unsigned char or unsigned short, then `PT_i` is unsigned
* If `T_i` is an unscoped enumeration type, then `PT_i` is determined as above, based on the underlying type of `T_i`
* Otherwise, `PT_i` is `T_i`.

The behavior is undefined unless the type of the ith argument in the list is of type `U_i`, where `U_i` meets one of the following requirements:
* `U_i` is exactly `T_i`
* `U_i` is exactly `PT_i`
* `T_i` is the signed variant of `U_i` or vice versa
* `PT_i` is the signed variant of `U_i` or vice versa


