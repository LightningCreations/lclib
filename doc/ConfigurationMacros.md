# Configuration Macros #

In header `<lclib/Config.hpp>` and `<lclib/WinTKConfig.hpp>`

Defines at set of macros used by LCLib to introduce compilation elements and various preprocessor actions. 

These Macros can be overrided by the command line, by a file in the include path called `<configure/BuildConfig.hpp>` or by a file in the include path during library use called `<configure/UserConfig.hpp>`. 

Warning: Some of these macro's have unpredictable effects on the ABI of Many of the LCLib API Functions. These effects may result in incorrect calls to the library. 

Macros which are known to cause ABI Issues within LCLib will be marked *BUILD ONLY*. If *BUILD ONLY* Macro's are defined by `<configure/UserConfig.hpp>` for a program that uses LCLib-C++, then that program is ill-formed, no diagnostic required. 

Programs that use LCLib-C++ should not override the `<configure/BuildConfig.hpp>`. If `<configure/BuildConfig.hpp>` is provided when building the library, then a file that is identical must be provided when using the library. If `<configure/BuildConfig.hpp>` is not defined when building the library, it MUST NOT be provided when using the library. A program that violates these rules is ill-formed, no diagnostic required. 



## LIBLCEXPORT ##

*BUILD ONLY*

A compiler dependent sequence of tokens that can appear where ever the sequence `__attribute__((visibility("default")))` can appear, as though the library were built with GCC. If this Macro is expanded in the declaration of any symbol, then that symbol must be visible to any application that declares an identical symbol with the `LIBLCIMPORT` macro, and links LCLib C++ or the library the declares it with this macro. 
If it appears on a class, then all Member functions of that class which are either that is accessible to the external API of the class, as well as all static data members, the vtable of that class (if applicable), and the typeinfo of that class (if applicable), as well as any nested classes shall be declared as though they had this attribute, except if that function or static data member is declared inline, constexpr, a template definition, a member of a template type, explicitly defined as defaulted, or defined as deleted. Additionally any other member functions and static data members may have this apply to them, however this is unspecified.

A function or static data member is accessible to the external API of a class if it is not a pure virtual function (except a pure virtual destructor) and at least one of the following is true:

* It is a destructor, global allocation, or global deallocation function.
* It is a public member of the class, or a protected member of the class and the class is not marked final.
* It is a virtual function or overrides a virtual function, except if it is declared as a pure virtual function. 
* The function or data member is odr-used by any inline function, constexpr function, template definition, or member of a template, and the use is well formed in C++. 

This directive is not required to be satisfied for any class or function that has internal linkage. 
 

## LIBLCIMPORT ##

Operates as the inverse of LIBLCIMPORT. It can import any symbol declared with LIBLCEXPORT. 
The macro shall expand to any sequence of tokens that can be used where LIBLCEXPORT can be used. 

Note that despite LIBLCIMPORT not being *BUILD ONLY* overriding the definition of LIBLCIMPORT in a user configuration file may have unexpected results. 

## LIBLCAPI ##

May not be overriden. Defined as LIBLCEXPORT if `LCLIB_CXX_DEFINITION` is defined, otherwise defined as LIBLCIMPORT. 

## LIBLCHIDE ##

Defines that the symbol is hidden. 
 
