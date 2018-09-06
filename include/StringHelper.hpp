#ifndef __StringHelper_hpp_2018_05_16_08_24
#define __StringHelper_hpp_2018_05_16_08_24
#include <string>
#include <Config.hpp>

using std::string;

LIBLCFUNC string tokenize(string&,string);

LIBLCFUNC void replace(string&,string,string);

LIBLCFUNC int count(string,string);

LIBLCFUNC string toLowerCase(string&);
LIBLCFUNC string toUpperCase(string&);

/**
 * Given a pointer to a c-style string, returns a pointer to the end of that c-style string (which points to the Null terminator).
 * The behavior is undefined if there is not a valid c-style string at the given pointer.
 */
LIBLCFUNC const char* strend(const char*);

#endif
