# LCLib-C++ General Library Rules #

## Unspecified Behavior ##

Unspecified Behavior is behavior that operates under a set of conditions, but is otherwise unbound in how the implementation chooses to operate. An example of unspecified behavior is passing a NaN value to DataOutputStream.writeFloat(), where it was unspecified if the NaN bit representation was preserved or if its collapsed to a single NaN (until 1.3, where it became undefined behavior to conform to LCLib Requirements). 

Unspecified Behavior may not result in exceptional behavior, or behavior that is otherwise forbidden by this specification. However, until 1.3 some behavior was documented as Unspecified (and may be Undefined). This means that implementations may choose to treat this as undefined behavior. As of 1.3 all of these were defined or collapsed into either pure unspecified behavior or pure undefined behavior. If an implementation chooses to treat it as undefined behavior, they may act exceptionally in that case. 

## Implementation-Defined Behavior ##

Implementation-Defined behavior is unspecified behavior which implementations must clearly document the result of that behavior. 

## Pointers ##
All pointers passed to functions in this library must point to a valid object unless otherwise specified. If a pointer is passed to a function in this library that does not point to a valid object, then the behavior is undefined.

## Exceptions ##

Unless a Library function or constructor is marked noexcept or otherwise stated in the documentation, calls to that library function may indicate an exceptional condition in an implementiation-defined set of circumstances, by throwing any unspecified exception that matches a handler of type `std::exception` and that's copy-constructor does not throw any exceptions. 
The exception to this rule is library functions that may call functions that are not defined by the library, in which case, they are allowed to throw any exception thrown by the called function, in addition to any function permitted above. This will be documented as "Throws any exception that *some function* throws".

If some precondition of a function is not met, the above rules do not apply. 

## Class Templates ##

Many class templates are defined in this library for various purposes. 

These templates act upon the general rules used for the C++ Standard Template Library. 

Specifically, if a program instantiates any class template with an incomplete type, unless that particular incomplete type is explicitly allowed by the template, the behavior is undefined. 

If a program specializes any class template defined by lclib-c++, that class template must support user provided specializations, the type may not be an incomplete type, except for an array of an unknown bound where such an incomplete type is allowed, and must be a `user-specified type`. The behavior of program that specializes a class template defined by lclib-c++ in violation of these rules is undefined.  
 Additionally, insantiating the primary template, or any partial specialization provided by the library, with that type must not result in the program being ill-formed or invoking undefined behavior at the point of instantiation (though it may have lead to undefined behavior, IE. in the case of ValueDiscard, specializing for a volatile qualified class type). A program that instantiates a class template defined by lclib-c++ in violation of the these rules is ill-formed, no diagnostic required. 

These prohibitions are general, the rules of the specific template always supersede these prohibitions if they are in contradiction with them. 

Unless otherwise specified, the results of instantiating a template with a cv-qualified

Note that following, a `User-Specified type` is specified as follows:

* A cv-qualified version of a `User-specified type`
* A reference to a `User-specified type`
* A pointer to, or an array of a `User-specified type`
* A function type that accepts or returns a parameter of a `User-specified type`