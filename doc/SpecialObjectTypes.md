<h1>Special Object Types</h1>
LCLib C++ Defines a set of special Object types, which have special privileges in LCLib.
<h2>Byte Types</h2>
Byte types are special case of scalar types which have sizeof(T)==1. Byte types are not invalidated when arrays of such are incompletely written to.<br/>
The C++ Standard only specifies 4 byte types, which are char, unsigned char, signed char, and std::byte (and does not officially name this Concept). LCLib C++ Extends this definition to encompass more types.<br/>
Byte types (and arrays of such) are considered layout compatible with all Standard Structure Types of the same size. (See below)<br/>
A Byte type must be one of the following:
<ul>
<li>char, unsigned char, or signed char</li>
<li>A enum type with one of the above as its underlying type, and no enumerators (including std::byte)</li>
<li>A Safe Integer Type (see below) which has one of the above as its underlying type</li>
</ul>
In addition: All Comparison operators may not be user supplied.
<h2>Standard Structure Type</h2>