/*
 * SHA.hpp
 *
 *  Created on: Sep 18, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_SECURITY_SHA_HPP__2018_09_18_10_25_35
#define __INCLUDE_SECURITY_SHA_HPP__2018_09_18_10_25_35
#include <lclib/TypeTraits.hpp>
#include <lclib/security/Concepts.hpp>
#include <openssl/sha.h>
#include <cstddef>
#include <lclib/security/SecureAllocator.hpp>
#include <detail/SHA2.hpp>

namespace security{
	/**
	 * Template for SHA Algorithms
	 * Only Directly supports SHA1 (Deprecated), SHA224, SHA256, SHA384, and SHA512
	 * Does NOT Support SHA0, SHA3, SHA512/224, or SHA512/256.
	 * Notes:
	 * Support for SHA0 is not happening.
	 * Support for SHA3 is not planned
	 * Support for SHA512/224 SHA512/256 is whenever direct support (non-evp) for these variations is added to openssl
	 */
	template<std::size_t M,typename byte=std::byte> struct SHA{
		 static_assert(M==1,"SHA Cannot be instantiated for any Target that is not specialized");
	};
	template<typename byte> [[deprecated("SHA-1 is deprecated by the NSA as insecure. Use a SHA-2 Algorithm instead")]]
							 struct SHA<1,byte>{
	private:
		SHA_CTX ctx;
	public:
		static_assert(is_byte_v<byte>,"Can Only Instantiate SHA with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		constexpr static const std::size_t digestSize{20};
		~SHA(){
			clear(&ctx);
		}
		void init(){
			SHA1_Init(&ctx);
		}
		void update(input_type buff,std::size_t sz){
			SHA1_Update(&ctx,buff,sz);
		}
		void doFinal(output_type buff){
			SHA1_Final(reinterpret_cast<unsigned char*>(buff),&ctx);
		}
	};
	template<typename byte> struct SHA<224,byte>{
	private:
		SHA256_CTX ctx;
	public:
		static_assert(is_byte_v<byte>,"Can Only Instantiate SHA with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		constexpr static const std::size_t digestSize{28};
		~SHA(){
			clear(&ctx);
		}
		void init(){
			SHA224_Init(&ctx);
		}
		void update(input_type in,std::size_t sz){
			SHA224_Update(&ctx,in,sz);
		}
		void doFinal(output_type out){
			SHA224_Final(reinterpret_cast<unsigned char*>(out),&ctx);
		}
	};
	template<typename byte> struct SHA<256,byte>{
	private:
		SHA256_CTX ctx;
	public:
		static_assert(is_byte_v<byte>,"Can Only Instantiate SHA with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		constexpr static const std::size_t digestSize{32};
		~SHA(){
			clear(&ctx);
		}
		void init(){
			SHA256_Init(&ctx);
		}
		void update(input_type in,std::size_t sz){
			SHA256_Update(&ctx,in,sz);
		}
		void doFinal(output_type out){
			SHA256_Final(reinterpret_cast<unsigned char*>(out),&ctx);
		}
	};
	template<typename byte> struct SHA<384,byte>{
	private:
		SHA512_CTX ctx;
	public:
		static_assert(is_byte_v<byte>,"Can Only Instantiate SHA with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		constexpr static const std::size_t digestSize{48};
		~SHA(){
			clear(&ctx);
		}
		void init(){
			SHA384_Init(&ctx);
		}
		void update(input_type in,std::size_t sz){
			SHA384_Update(&ctx,in,sz);
		}
		void doFinal(output_type out){
			SHA384_Final(reinterpret_cast<unsigned char*>(out),&ctx);
		}
	};
	template<typename byte> struct SHA<512,byte>{
	private:
		SHA512_CTX ctx;
	public:
		static_assert(is_byte_v<byte>,"Can Only Instantiate SHA with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		constexpr static const std::size_t digestSize{64};
		~SHA(){
			clear(&ctx);
		}
		void init(){
			SHA512_Init(&ctx);
		}
		void update(input_type in,std::size_t sz){
			SHA512_Update(&ctx,in,sz);
		}
		void doFinal(output_type out){
			SHA512_Final(reinterpret_cast<unsigned char*>(out),&ctx);
		}
	};
}


#endif /* __INCLUDE_SECURITY_SHA_HPP__2018_09_18_10_25_35 */
