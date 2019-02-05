# Configuration Macros #

In header `<lclib/Config.hpp>` and `<lclib/WinTKConfig.hpp>`

Defines at set of macros used by LCLib to introduce compilation elements and various preprocessor actions. 

These Macros can be overriden by the command line, by a file in the include path called `<configure/BuildConfig.hpp>` or by a file in the include path during library use called `<configure/UserConfig.hpp>`. 
If a macro overriden this way does not meet the requirements defined here, then the program is ill-formed, no diagnostic required. If that such a macro MUST NOT be overriden, then the program is ill-formed. 

Warning: Some of these macro's have unpredictable effects on the ABI of Many of the LCLib API Functions. These effects may result in incorrect calls to the library. 

Macros which are known to cause ABI Issues within LCLib will be marked *BUILD ONLY*. If *BUILD ONLY* Macro's are defined by `<configure/UserConfig.hpp>` for a program that uses LCLib-C++, then that program is ill-formed, no diagnostic required. 

Programs that use LCLib-C++ should not override the `<configure/BuildConfig.hpp>`. If `<configure/BuildConfig.hpp>` is provided when building the library, then a file that is identical must be provided when using the library. If `<configure/BuildConfig.hpp>` is not defined when building the library, it MUST NOT be provided when using the library. A program that violates these rules is ill-formed, no diagnostic required. 

`<lclib/Config.hpp>` provides default definitions for each of these macros. It also includes `<configure/BuildConfig.hpp>` and `<configure/UserConfig.hpp>` (the latter only when not building the library). 
Any definitions of macros defined there override the default definitions provided by the implementation. 

`<lclib/Config.hpp>` is implicitly included by all headers defined by LCLib C++, even if the header definition does not define that it does.

If `<lclib/Config.hpp>` is included by a translation unit, when `__cplusplus` is not at least 201703L, then the program that contains that translation unit is ill-formed. 


## LIBLCEXPORT ##

*BUILD ONLY*

A compiler dependent sequence of tokens that can appear where ever the sequence `__attribute__((visibility("default")))` can appear, as though the library were built with GCC. 
If this Macro is expanded in the declaration of any symbol, then that symbol must be visible to any application that declares an identical symbol with the `LIBLCIMPORT` macro, and links LCLib C++ (if it is defined by LCLib C++) or the library the declares it with this macro. 
If it appears on a class, then all Member functions of that class which are either that is accessible to the external API of the class, as well as all static data members, the vtable of that class (if applicable), and the typeinfo of that class (if applicable), as well as any nested classes shall be declared as though they had this attribute, except if that function or static data member is declared inline, constexpr, a template definition, a member of a template type, explicitly defined as defaulted, or defined as deleted. Additionally any other member functions and static data members may have this apply to them, however this is unspecified.

A function or static data member is accessible to the external API of a class if it is not a pure virtual function (except a pure virtual destructor) and at least one of the following is true:

* It is a destructor, global allocation, or global deallocation function
* It is a public member of the class, or a protected member of the class and the class is not marked final. 
* It is a virtual function or overrides a virtual function, except if it is declared as a pure virtual function. 
* The function or data member is odr-used by any inline function, constexpr function, template definition, or member of a template, and the use is well formed in C++. 

This directive is not required to be satisfied for any class or function that has internal linkage. 
 

## LIBLCIMPORT ##

Operates as the inverse of LIBLCEXPORT. It can import any symbol declared with LIBLCEXPORT. 
The macro shall expand to any sequence of tokens that can be used where LIBLCEXPORT can be used. 

Note that despite LIBLCIMPORT not being *BUILD ONLY* overriding the definition of LIBLCIMPORT in a user configuration file may have unexpected results. 

## LIBLCAPI ##

May not be overriden by the program. Defined as LIBLCEXPORT if `LCLIB_CXX_DEFINITION` is defined, otherwise defined as LIBLCIMPORT. 

## LIBLCHIDE ##

Defines that the symbol is hidden. 

## Contract Macros ##

The macro's defined below can be used to issue diagnostics, as well as declare preconditions, postconditions, and assertions. 

Contract Macros are unevaluated if the control macro `NOCONTRACTS` is defined, and it is implementation defined if they are evaluated otherwise. 

Contract Macros shall be considered attributes, where `EXPECTS` and `ENSURES` can be applied to function declarations (but not to function types), and `ASSERT` can be applied to a null statement (that is, a single semicolon). 
These macros may not be used in any other context. No diagnostic is required. 

The requirements for the `condition` of any contract macro are defined as the same as the requirements for the predicates of the contract attributes defined for C++20. 

### NOCONTRACTS ###

Undefined by default. If defined, then the contract macros, defined below are guaranteed to not be evaluated and may not expand to any of the contract attributes, except ones with a *contract-evaluation level* of `axiom`.

### ASSERT ###

ASSERT is defined as a function-like macro `ASSERT(condition)`. `condition` must be an expression which is contextually convertible to bool (see below). 

`ASSERT(condition)` can be used anywhere that an attribute that applies to a null statement can appear. If `ASSERT` appears anywhere else, the program is ill-formed, no diagnostic required. 

If `NOCONTRACTS` is defined, than `ASSERT(condition)` expands to an unspecified sequence which does not evaluate `condition`. 
It is unspecified if `condition` is odr-used, except that it is not odr-used if it is a contextually-converted constant expression of type bool. 
If `condition` is not such a constant expression, then it shall be treated as odr-used. 
The program is ill-formed, no diagnostic required if any of the following is true:
* `condition` is not a valid expression, a prvalue expression of `void`, or the result of `condition` is not contextually convertible to `bool`.
* The odr-use of `condition` would result in a violation of the One Definition Rule
* `condition` causes the implicit instantiation of a templated entity, and the instantiation of that templated entity would cause the program to be ill-formed. 

If `condition` would evaluate to `false`, or is a contextually-converted constant expression of type `bool` that equals `false`, then the behavior is undefined. 

If `NOCONTRACTS` is defined, the program cannot override `ASSERT` or the program is ill-formed, no diagnostic required. 

Otherwise, it expands to an unspecified sequence which the above rules apply, except that it is implementation-defined whether or not the assertion is evaluated. 

If the assertion is evaluated, then `condition` is odr-used if it would be treated as odr-used above, otherwise it is not. 
An evaluated assertion for which `condition` evaluates to false may not be used in a core constant expression. 
If the `condition` evaluates to false, then an implementation-defined diagnostic message is printed to the console, which includes the program location, then the program terminates as though the default handler for `std::terminate()` is invoked. 

If `NOCONTRACTS` is not defined, and the feature test macro `__has_cpp_attribute(assert)` is at least 201806L, then the default expansion of `ASSERT(condition)` shall be one of `[[assert:condition]]`, `[[assert audit:condition]]`, or `[[assert axiom:condition]]`.  
If `NOCONTRACTS` is defined, it may not expand to any of these attributes, expect for `[[assert axiom:condition]]`. 

If the assertion is not evaluated, then `condition` is treated as odr-used as above. If `condition` would evaluate to `false` the behavior is undefined. 

`ASSERT(condition)` may expand to `assert(bool{condition})` where `assert` is defined as by the header `<cassert>`. 

`ASSERT` is evaluated at the point that the statement which is declared with `ASSERT` as an attribute is executed.

### EXPECTS ###

`EXPECTS(condition)` shall be similar to `ASSERT(condition)` except that it shall be usable in the context of an attribute which applies to a function declaration (not a function-type). 

If `NOCONTRACTS` is not defined, and the feature test macro `__has_cpp_attribute(expects)` is at least 201806L, then the default expansion of `EXPECTS(condition)` must be one of `[[expects:condition]]`, `[[expects audit:condition]]`, or `[[expects axiom:condition]]`. 
If `NOCONTRACTS` is defined, the default expansion may not be any of these, except for `[[expects axiom:condition]]`. 

If `EXPECTS(condition)` annotates a function, then it is evaluated at the point where that function is called by the program. 

Neither `EXPECTS` nor `ENSURES` may annotate a function that is defined as deleted. No diagnostic is required.  

`EXPECTS` may annotate a constructor or assignment operator which is defined as defaulted. 

### ENSURES ###

`ENSURES(condition,...)` shall be similar to `EXPECTS(condition)`, except that `...` may optionally declare exactly one name which are odr-used by `condition`. 

If `NOCONTRACTS` is not defined, and the feature test macro `__has_cpp_attribute(ensures)` is at least 201806L, then the default expansion of `ENSURES(condition,...)` must be one of `[[expects __VA_ARGS__:condition]]`, `[[expects audit __VA_ARGS__:condition]]`, or `[[expects axiom __VA_ARGS__:condition]]`. 

If `...` declares a name which is odr-used by `condition`, then that name evaluates to the return value of the function. If the function returns `void` the program is ill-formed, no diagnostic required.

If `condition` odr-uses any parameters, then the behavior is undefined if the body of the function modifies any of those parameters (though the body of the function may modify the object pointed to or referred to by that parameter, specifically this only applies to non-reference parameters). 

If `ENSURES(condition,...)` annotates a function, it is evaluated immediately after any return statement of the program executes, or (if the function returns void) the function reaches the ending curly-brace of the function body. 

Annotating a function with `ENSURES` is conditionally-supported if that function is a constructor or assignment operator which is defined as defaulted. 

Neither `EXPECTS` nor `ENSURES` may annotate a function that is defined as deleted. No diagnostic is required.  
