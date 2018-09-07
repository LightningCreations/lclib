#ifndef __StringHelper_hpp_2018_05_16_08_24
#define __StringHelper_hpp_2018_05_16_08_24
#include <string>
#include <Config.hpp>

using std::string;

LIBLCAPI string tokenize(string&,string);

LIBLCAPI void replace(string&,string,string);

LIBLCAPI int count(string,string);

LIBLCAPI string toLowerCase(string&);
LIBLCAPI string toUpperCase(string&);

/**
 * Given a pointer to a c-style string, returns a pointer to the end of that c-style string (which points to the Null terminator).
 * The behavior is undefined if there is not a valid c-style string at the given pointer.
 */
LIBLCAPI const char* strend(const char*);

#endif
