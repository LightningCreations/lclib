#ifndef __SHA256_hpp_2018_05_31_10_18
#define __SHA256_hpp_2018_05_31_10_18
#include <cstddef>
#include <Config.hpp>
/*
  Implementation of SHA256.
  This function will compute the SHA256 hash of a region of bytes.
  See https://en.wikipedia.org/wiki/SHA-2
*/
LIBLCAPI void SHA256(const char*,size_t,char (&)[32]);


#endif
