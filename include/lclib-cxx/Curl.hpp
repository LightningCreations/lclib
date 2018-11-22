/*
 * Curl.hpp
 *
 *  Created on: Oct 5, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_CURL_HPP__2018_10_05_08_49_49
#define __INCLUDE_LCLIB_CXX_CURL_HPP__2018_10_05_08_49_49

#include <curl/curl.h>
#include <mutex>
#include <lclib-cxx/IOWrapper.hpp>
#include <lclib-cxx/Strings.hpp>
#include <utility>
#include <map>
#include <locale>

class CurlGlobal{
private:
	std::recursive_mutex lock;
public:
	CurlGlobal(){
		std::lock_guard sync(lock);
		curl_global_init(0);
	}
	~CurlGlobal(){
		std::lock_guard sync(lock);
		curl_global_cleanup();
	}
};


namespace message{
	enum class Method{
		GET, POST, PUT, DELETE, HEAD, TRACE
	};
	enum class ResponseCode{
		CONTINUE = 100,
		SWITCHING_PROTOCOLS = 101,
		PROCESSING = 102,
		EARLY_HINTS = 103,
		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		NON_AUTHORATIVE_INFORMATION = 203,
		NO_CONTENT = 204,
		RESET_CONTENT = 205,
		PARTIAL_CONTENT = 206,
		MULTISTATUS = 207,
		ALREADY_REPORTED = 208,
		IM_USED = 226,
		MULTIPLE_OPTIONS = 300,
		MOVED_PERMANENTLY = 301,
		FOUND = 302,
		SEE_OTHER = 303,
		NOT_MODIFIED = 304,
		USE_PROXY = 305,
		TEMPORARY_REDIRECT = 307,
		PERMANENT_REDIRECT = 308,
		BAD_REQUEST = 400,
		UNAUTHORIZED = 401,
		PAYMENT_REQUIRED = 402,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		NOT_ACCEPTABLE = 406,
		PROXY_AUTHENTICATION_REQUIRED = 407,
		REQUEST_TIMED_OUT = 408,
		CONFLICT = 409,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PRECONDITION_FAILED = 412,
		PAYLOAD_TOO_LARGE = 413,
		URI_TOO_LONG = 414,
		UNSUPPORTED_MEDIA_TYPE = 415,
		RANGE_NOT_SATISFIABLE = 416,
		EXPECTATION_FAILED = 417,
		IM_A_TEAPOT = 418,
		MISDIRECTED_REQUEST = 421,
		UNPROCESSABLE_ENTITY = 422,
		LOCKED = 423,
		FAILED_DEPENDENCY = 424,
		UPGRADE_REQUIRED = 426,
		PRECONDITION_REQUIRED = 428,
		TOO_MANY_REQUESTS = 429,
		REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
		UNAVAILBLE_FOR_LEGAL_REASONS = 451
	};
	struct request_tag{};
	struct response_tag{};
	template<typename tag,
		typename BodyT=std::string,
		typename HeadersT=std::map<std::string,std::string,case_insensitive_order<char>>>
			struct message_t{
				Method method;
				std::string_view path;
				HeadersT header;
				BodyT body;
			};
	template<typename BodyT,typename HeadersT>
		struct message_t<response_tag,BodyT,HeadersT>{
			ResponseCode code;
			HeadersT header;
			BodyT body;
		};
	template<typename BodyT=std::string,
		typename HeadersT=std::map<std::string,std::string,case_insensitive_order<char>>>
		using request = message_t<request_tag,BodyT,HeadersT>;
	template<typename BodyT=std::string,
		typename HeadersT=std::map<std::string,std::string,case_insensitive_order<char>>>
		using response = message_t<response_tag,BodyT,HeadersT>;
}

class Curl{
private:
	mutable std::recursive_mutex lock;
	CURL* handle;
	OutputStream* curlOut;
	std::string_view address;
	Curl(const Curl&)=delete;
	Curl& operator=(const Curl&)=delete;
public:
	Curl(OutputStream&,std::string_view);
};


#endif /* __INCLUDE_LCLIB_CXX_CURL_HPP__2018_10_05_08_49_49 */
