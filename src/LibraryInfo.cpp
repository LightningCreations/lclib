/*
 * LibraryInfo.cpp
 *
 *  Created on: Oct 16, 2018
 *      Author: connor
 */

#include <lclib/Version.hpp>
#include <lclib/Config.hpp>
#include <lclib/Library.hpp>

#include <cstdint>


using namespace version_literals;

const char lclib_cxx::name[]{"LCLib C++"};
const char lclib_cxx::copyright[]{"(c) 2018, Connor Horman and Lightning Creations"};
const char lclib_cxx::license[]{"GNU LGPLv3"};
const Version lclib_cxx::ver{1,0};
const uint16_t lclib_cxx::patch{2};
const char lclib_cxx::releaseDate[]{__DATE__};
const char lclib_cxx::releaseTime[]{__TIME__};

