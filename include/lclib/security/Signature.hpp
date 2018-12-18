/*
 * Signature.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_SECURITY_SIGNATURE_HPP__2018_10_17_14_25_57
#define __INCLUDE_LCLIB_CXX_SECURITY_SIGNATURE_HPP__2018_10_17_14_25_57

#include <lclib/security/Concepts.hpp>
#include <lclib/security/Cipher.hpp>
#include <lclib/security/MessageDigest.hpp>
#include <lclib/TypeTraits.hpp>
#include <cstring>

namespace security{
	template<typename AsymmCipherAlgo,typename MDAlgo,typename Byte=std::byte> struct Sign{
	private:
		static_assert(security::traits::is_asymmetric_cipher_algorithm_v<AsymmCipherAlgo>,"Sign Requires an Assymmetric Cipher Algorithm");
		static_assert(security::traits::is_message_digest_algorithm_v<MDAlgo>,"Sign Requires a Message Digest Algorithm");
		static_assert(is_byte_v<Byte>,"Sign requires a Byte type");
	public:
		using byte_type = Byte;
		using input_type = const byte_type*;
		using output_type = byte_type*;
		using cipher_algorithm = AsymmCipherAlgo;
		using md_algorithm = MDAlgo;
		using public_key = typename AsymmCipherAlgo::public_key;
		using private_key = typename AsymmCipherAlgo::private_key;
	public:
		static void sign(const private_key& key,input_type file,std::size_t sz,output_type signature){
			MessageDigest<cipher_algorithm> digest;
			AsymmCipherAlgo algo;
			byte_type buffer[md_algorithm::digestSize];
			digest(file,sz,buffer);
			algo.init(key,true,nullptr);
			algo.update(buffer,sizeof(buffer),signature);
		}
		static bool verify(const public_key& key,input_type file,std::size_t sz,input_type signature,std::size_t sigSz){
			MessageDigest<cipher_algorithm> digest;
			AsymmCipherAlgo algo;
			byte_type buffer[md_algorithm::digestSize];
			byte_type oBuffer[md_algorithm::digestSize];
			digest(file,sz,buffer);
			algo.init(key,false,nullptr);
			algo.update(signature,sigSz,oBuffer);
			return std::memcmp(buffer, oBuffer, sizeof(buffer))==0;
		}
	};
}




#endif /* __INCLUDE_LCLIB_CXX_SECURITY_SIGNATURE_HPP__2018_10_17_14_25_57 */
