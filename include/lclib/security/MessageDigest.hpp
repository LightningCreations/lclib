/*
 * MessageDigest.hpp
 *
 *  Created on: Sep 18, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_SECURITY_MESSAGEDIGEST_HPP__2018_09_18_10_52_58
#define __INCLUDE_SECURITY_MESSAGEDIGEST_HPP__2018_09_18_10_52_58
#include <lclib/security/Concepts.hpp>
#include <type_traits>
#include <lclib/TypeTraits.hpp>

namespace security{
	/**
	 * MessageDigest Wrapper to hash data.
	 * MessageDigest satisfies FunctionObject, calling a MessageDigest will call the Hash Algorithm.
	 */
	template<typename Digest> struct MessageDigest{
	private:
		Digest target;
	public:
		static_assert(security::traits::is_message_digest_algorithm_v<Digest>,"MessageDigest Requires MessageDigestAlgorithm");
		using algorithm = Digest;
		using byte_type = typename Digest::byte_type;
		using input_type = typename Digest::input_type;
		using output_type = typename Digest::output_type;
		constexpr static const std::size_t digestSize{Digest::digestSize};
		MessageDigest()=default;
		MessageDigest(const MessageDigest&)=delete;
		MessageDigest(MessageDigest&&)=default;
		void digest(input_type in,std::size_t sz,output_type out){
			target.init();
			target.update(in,sz);
			target.doFinal(out);
		}
		void operator()(input_type in,std::size_t sz,output_type out){
			target.init();
			target.update(in,sz);
			target.doFinal(out);
		}
	};
}



#endif /* __INCLUDE_SECURITY_MESSAGEDIGEST_HPP__2018_09_18_10_52_58 */
