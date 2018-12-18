/*
 * Version.hpp
 * Class for Version Management.
 * Versions are comparable and Hashable. 
 *  Created on: Apr 4, 2018
 *      Author: Connor Horman
 */

#ifndef __VERSION_HPP__18_04_04_11_12_42
#define __VERSION_HPP__18_04_04_11_12_42
#include <string>
#include <iostream>
#include <lclib/Hash.hpp>
#include <lclib/Config.hpp>
#include <cmath>
#include <stdexcept>
#include <lclib/Operations.hpp>
#include <tuple>

#undef major
#undef minor

using std::istream;
using std::ostream;

using namespace std::string_literals;

/*
 * Represents a version of the code.
 * This specification only defines the Major and Minor parts of the version and is encoded
 * using The Sentry Version Encoding.
 * This class also provides utilities for obtaining versions from strings, and from a given
 * major and minor, as well as reading and writing to Data Streams (and text streams).
 * The Version class provides both the read from (istream >>) and the write to (ostream <<) operators
 * To write in string form.
 */
class LIBLCAPI Version:private RelOps<Version>,private StrictOrder<Version>{
private:
	unsigned char major;
	unsigned char minor;
	static constexpr unsigned int fromString(std::string_view sv){
		unsigned int ret{0};
		for(char c:sv){
			ret += c-'0';
			ret *= 10;
		}
		return ret;
	}
public:
	/*
	 * Constructs the Default Version (1.0)
	 */
	constexpr Version():major{0},minor{0}{

	}
	/*
	 * Obtains a version given in an encoded form (Ex. 0x0000).
	 * This follows the sentry format for encoding versions (2-bytes BE, High-byte is Major version -1, Low-byte is minor version)
	 * This Constructor should be used only when you are dealing with Embedded and Encoded Version constants
	 */
 	constexpr explicit Version(uint16_t encoded):major(encoded>>8),minor(encoded){}
	/*
	 * Parses a given string in the form <Mj>.<mi> and produces a version given those 2 inputs.
	 * Both Mj and mi must be valid integers, with Mj being between 1 and 256 and Mi being between 0 and 255
	 */
 	constexpr Version(std::string_view sv):major{0},minor{0}{
 		std::string_view::size_type sz = sv.find(".");
 		if(sz==std::string_view::npos)
 			throw std::invalid_argument(std::string(sv)+" is an invalid string"s);
 		major = fromString(sv.substr(0,sz));
 		minor = fromString(sv.substr(sz));
 	}
	/*
	 * Obtains a version based on a given Major and minor version.
	 * Major must be between 1 and 256 and minor must be between 0 and 255
	 */
	constexpr Version(uint32_t maj,uint8_t min):major(maj-1),minor{min}{}
	
	constexpr Version(const Version&)=default;
	constexpr Version(Version&&)=default;
	constexpr Version& operator=(const Version&)=default;
	constexpr Version& operator=(Version&&)=default;

	/*
	 * Gets the major version, ranging from 1 to 256
	 */
	constexpr uint32_t getMajor()const{
		return uint32_t(major)+1;
	}
	/*
	 * Gets the minor version, ranging from 0 to 255
	 */
	constexpr uint32_t getMinor()const{
		return uint32_t(minor);
	}
	/*
	 * Returns the version in encoded form.
	 * The resultant value is 2-bytes consisting of major-1<<8|minor.
	 */
	constexpr int getEncoded()const{
		return int(major)<<8|minor;
	}
	/*
	 * Obtains the Origin of this Version. The origin of a Version is equal to the Version
	 * that has the same Major version, but a minor version of 0.
	 */
	constexpr Version getOrigin()const{
		return Version(int(major)+1,0);
	}
	/*
	 * Returns this Version as a string.
	 * The Resultant String is in the form <major>.<minor>
	 */
	explicit operator std::string()const;
	/*
	 * Computes the hashcode of this Version.
	 * This is effectively major*31+minor
	 */
	constexpr int32_t hashCode()const{
		return (int(major)+1)*31+int(minor);
	}

	constexpr bool operator<(const Version& v)const{
		return std::tie(major,minor)<std::tie(v.major,v.minor);
	}

};

/*
 * Reads a version from a text stream.
 * The input text is assumed to be in the form <Mj>.<mi>
 * This is effectively the same as reading a string from the stream,
 * and Constructing the associated version.
 *
 */
LIBLCAPI istream& operator>>(istream&,Version&);
/*
 * Writes this string to a text stream.
 * The output text is will be in the form <Mj>.<mi>
 * This is effectively the same as writing that version's toString()
 */
LIBLCAPI ostream& operator<<(ostream&,const Version&);

namespace version_literals{
	template<char... c> constexpr Version operator""v(){
		const char str[sizeof...(c)]{c...};
		return Version{ std::string_view{str,sizeof...(c)} };
	}
	constexpr Version operator""v(const char* str,std::size_t sz){
		return Version{std::string_view{str,sz}};
	}
}

constexpr int32_t hashcode(Version v){
	return v.hashCode();
}

namespace std{
	template<> struct hash<Version>{
		constexpr hash()=default;
		constexpr size_t operator()(Version v){
			return v.hashCode();
		}
	};
}

#endif /* __VERSION_HPP__18_04_04_11_12_42 */
