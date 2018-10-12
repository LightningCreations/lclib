/*
 * Version.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: Connor Horman
 */
#include <lclib-cxx/Version.hpp>
#include <lclib-cxx/StringHelper.hpp>
#include <string>
#include <stdexcept>

using std::string;
using std::to_string;
using std::stoi;
using std::invalid_argument;





ostream& operator <<(ostream& o,const Version& v){
	return o<<std::string{v};
}
istream& operator >>(istream& i,Version& v){
	string s;
	i >> s;
	v = std::string_view{s};
	return i;
}

Version::operator std::string()const{
	return to_string(int(major)+1)+"."+to_string(minor);
}




