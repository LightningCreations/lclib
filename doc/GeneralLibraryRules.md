# LCLib-C++ General Library Rules #

## Unspecified Behavior ##

Unspecified Behavior is behavior that operates under a set of conditions, but is otherwise unbound in how the implementation chooses to operate. An example of unspecified behavior is passing a NaN value to DataOutputStream.writeFloat(), where it was unspecified if the NaN bit representation was preserved or if its collapsed to a single NaN (until 1.3, where it became undefined behavior to conform to LCLib Requirements). 

Unspecified Behavior may not result in exceptional behavior, or behavior that is otherwise forbidden by this specification. However, until 1.3 some behavior was documented as Unspecified (and may be Undefined). This means that implementations may choose to treat this as undefined behavior. As of 1.3 all of these were defined or collapsed into either pure unspecified behavior or pure undefined behavior. If an implementation chooses to treat it as undefined behavior, they may act exceptionally in that case. 

## Implementation-Defined Behavior ##

Implementation-Defined behavior is unspecified behavior which implementations must clearly document the result of that behavior. 

## Pointers ##
All pointers passed to functions in this library must point to a valid object unless otherwise specified. 
If a pointer is passed to a function in this library that does not point to a valid object, then the behavior is undefined. 
Some functions explicitly permit null pointers, or pointers to uninitialized memory, returned from an allocation function. 
This will be explicilty documented. 

## Exceptions ##

Unless a Library function or constructor is marked noexcept or otherwise stated in the documentation, calls to that library function may indicate an exceptional condition in an implementiation-defined set of circumstances, by throwing any unspecified exception that matches a handler of type `std::exception` and that's copy-constructor does not throw any exceptions. 
The exception to this rule is library functions that may call functions that are not defined by the library, in which case, they are allowed to throw any exception thrown by the called function, in addition to any function permitted above. This will be documented as "Throws any exception that *some function* throws". 

If some precondition of a function is not met (thus resulting in undefined behavior), the above rules do not apply. 

The conditions which a library function may throw an exception are limited to conditions which are not documented by 

## Class Templates ##

Many class templates are defined in this library for various purposes. 

These templates act upon the general rules used for the C++ Standard Template Library. 

Specifically, if a program instantiates any class template with an incomplete type, unless that particular incomplete type is explicitly allowed by the template, the behavior is undefined. 

If a program specializes any class template defined by lclib-c++, that class template must support user provided specializations, the type may not be an incomplete type, except for an array of an unknown bound where such an incomplete type is allowed, and must be a `user-specified type`. The behavior of program that specializes a class template defined by lclib-c++ in violation of these rules is undefined.  
 Additionally, instantiating the primary template, or any partial specialization provided by the library, with that type must not result in the program being ill-formed or invoking undefined behavior at the point of instantiation (though it may have lead to undefined behavior, IE. in the case of ValueDiscard, specializing for a volatile qualified class type). A program that instantiates a class template defined by lclib-c++ in violation of the these rules is ill-formed, no diagnostic required. 

These prohibitions are general, the rules of the specific template always supersede these prohibitions if they are in contradiction with them. 

Unless otherwise specified, the results of instantiating a template with a cv-qualified

Note that following, a `User-Specified type` is specified as follows:

* A User-defined type
* A cv-qualified version of a `User-specified type`
* A reference to a `User-specified type`
* A pointer to, or an array of a `User-specified type`, including an array of an unknown bound
* A function type that accepts or returns a parameter of a `User-specified type`
* A Specialization of a User-defined class template
* A user-defined, explicit specialization of some class template, including a full specialization or a partial specialization.
* A Specialization of any other template, provided that at least one of the template parameters is a `user-specified type`

Library implementations may choose to provide explicit specializations of class templates which are not defined by LCLib, both partial and full, to conform to the rules defined by this library. These specializations shall be treated as though they were instantiations of the primary template. 

(For example, an implementation of `ValueDiscard` may specialize on `volatile T` to provide the explicit volatile requirements of `ValueDiscard`.) 

LCLib may define explicit specializations in its specification. These explicit specializations are specified by LCLib, and are exempt from these rules. 

## Special Cases ##

There are certain conditions which are not covered by this specification explicitly, and in which case, some guarantee is made by this specification that is not met. 
These either refer to exceptional cases, which no recovery may be possible, or incredibly unlikely cases which are simply not covered. 

The behavior of the library in these specific cases is undefined. 

The 2 special cases are as follows:

* Dynamic Memory allocation, performed by the library with a *new-expression* or a call to `operator new`, which no memory is available and none can be obtained by the installed *new-handler*.
* Calling the `Instant:now()` factory method, or using the `now_t` constructor of `Instant`, when the current time indicated by `instant_clock` is beyond the range which an `Instant` can represent. 

## Function Templates ##

LCLib also defines several function templates. These templates may have both deduced and specified template parameters. 
Only specified template parameters may be explicitly provided. The behavior of a program that explicitly specifies one or more deduced template parameters when instantiating a function template is undefined. 

Function templates defined by LCLib may not be specialized. 

A template parameter is deduced if at least one of the following is true:
* The template parameter is used in the declaration of one or more function arguments, and it is not used in a non-deduced context for that argument. 
* The template parameter as the type for the user-defined conversion operator. 

All template parameters in all operator overloads, conversion operators, and constructors are always deduced. 

Any template parameter which is not deduced is specified. 

Implementations may add additional, defaulted template parameters, to any function template that has deduced template parameters. 

Deduced template parameters do not need to be provided as is, or even exist. These template parameters are defined by the specification to encapsilate an overload set. 