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
const Version lclib_cxx::ver{1,3};
const uint16_t lclib_cxx::patch{1};
const char lclib_cxx::releaseDate[]{__DATE__};
const char lclib_cxx::releaseTime[]{__TIME__};
const char lclib_cxx::providerName[]{"lightningcreations"};
const char lclib_cxx::providerPublic[]{"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlR/C3W9n2mSUniZKPiw0hbZg4N27BdmxnoyWfLlTN280xF2Kz/fc/7PtxhDYRluhK/B4zO7A/EF2jpTqUgtSL+hT4L4ZY8I5QVqWcopMdg60FXuc6jdyKp3XLJWn6MFe0lPQAQ7tGhPMmg/LMRuql6cg3ohqjxbOVFYVPpvQ3hUwtPNSdHC5EOrbrydzPYLPOqNZn+/ylj9UidRyMtvFmL1HFjDO6+JQq5DqlwM1N2Jg0ic448vsJ0H/TSaEvGMI+aoetu3arZ4WyMZW4BvDAY4lqv+mb0p6ufgbXDcfxrkdL5Nr9pH1ZrNLHZsDg+ZO/CdX4tYhmahUOrQ3XuZqAwIDAQAB"};

