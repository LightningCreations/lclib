# Library Information #

In header `<lclib/Library.hpp>`. 

This header contains information about the LCLib Implementation. 

Various macros can be defined here, if supported. Implementations may choose to add macros to this header to indicate various things. In the future, LCLib-C++ may specify a set of feature test macros. These will be defined here, as well as in the appropriate header. 

Additionally, a class is defined with a bunch of static members. These members indicate traits about the library and the provider. 

## struct liblc_cxx ##
A special type defined with a set static data members. These members are guaranteed constant initialization, and are const objects. 

### Synopsis ###

```cpp
struct liblc_cxx{
	static const char name[];
	static const char copyright[];
	static const char license[];
	static const Version ver;
	static const std::uint16_t patch;
	static const char releaseDate[];
	static const char releaseTime[];
	static const char providerName[];
	static const char providerPublic[];
};
```

### Library Name ###

```cpp
static const char name[];
```

Returns the Name of the library. Always the String literal "LCLib C++". 

### Copyright Information ###

```cpp
static const char copyright[]; //(1)
static const char license[]; //(2)
```

1. A line describing the copyright of the library implementation. The official implementation has this presently defined as "(c) 2018, Connor Horman and Lightning Creations". 
2. The License that applies to the implementation. The official implementation uses the GNU Lesser General Public License at Version 3, and therefore defines this as "GNU LGPLv3". 

### Library Version Information ###

```cpp
static const Version ver; //(1)
static const std::uint16_t patch; //(2)
static const char releaseDate[]; //(3)
static const char releaseTime[]; //(4)
``` 


1. The Present version of LCLib supported by the implementation. The present version this specification defines is 1.3
2. The patch version of the implementation. This is an implementation defined version constant. 
3. The date that this implementation was built. 
4. The time that this implementation was built. 


### Provider Information ###

```cpp
static const char providerName[]; //(1)
static const char providerPublic[]; //(2)
```

1. The "human-name" field of a CCI Provider that is releasing the library. May be empty. 
2. The "pkey" field of a CCI Provider that is releasing the library. May be empty. 

Note that further validation may be required to assert that the provider of the library is actually the provider named in those fields. 
