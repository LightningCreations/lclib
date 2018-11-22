/*
 * IOInterop.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_IOINTEROP_HPP__2018_10_17_13_13_45
#define __INCLUDE_LCLIB_CXX_IOINTEROP_HPP__2018_10_17_13_13_45

#include <lclib-cxx/IOWrapper.hpp>
#include <iostream>
#include <string>

template<typename CharT,typename CharTraits=std::char_traits<CharT>>
	class basic_interopstream_buff:public std::basic_streambuf<CharT,CharTraits>{
	private:
		union{
			InputStream* in;
			OutputStream* out;
		};
	public:
		basic_interopstream_buff(InputStream& in):in(&in),isRead(false){
			this->setg(nullptr, nullptr, nullptr);
		}
		basic_interopstream_buff(OutputStream& out):out(&out),isRead(true){
			this->setg(nullptr, nullptr, nullptr);
		}
	protected:
		std::streamsize xsgetn(char_type* ptr,std::streamsize sz){
			return in->read(ptr,sz);
		}
		int_type underflow(){
			return in->read();
		}
		std::streamsize xsputn(const char_type* ptr,std::streamsize sz){
			return out->write(ptr,sz);
		}
		int_type overflow(int_type ch){
			if(ch==eof)
				return eof;
			out->write(ch);
			return ch;
		}
	};

template<typename CharT,typename CharTraits=std::char_traits<CharT>> struct basic_interop_istream:std::basic_istream<CharT,CharTraits>{
private:
	basic_interopstream_buff<CharT,CharTraits> buff;
public:
	basic_interop_istream(InputStream& str):buff(str),basic_istream(&buff){}
};

template<typename CharT,typename CharTraits=std::char_traits<CharT>> struct basic_interop_ostream:std::basic_ostream<CharT,CharTraits>{
private:
	basic_interopstream_buff<CharT,CharTraits> buff;
public:
	basic_interop_ostream(OutputStream& str):buff(str),basic_ostream(&buff){}
};

typedef basic_interop_istream<char> interop_istream;
typedef basic_interop_ostream<char> interop_ostream;

#endif /* __INCLUDE_LCLIB_CXX_IOINTEROP_HPP__2018_10_17_13_13_45 */
