/*
 * AES.hpp
 *
 *  Created on: Sep 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_SECURITY_AES_HPP__2018_09_17_17_15_15
#define __INCLUDE_SECURITY_AES_HPP__2018_09_17_17_15_15
#include <lclib-cxx/security/Concepts.hpp>
#include <cstddef>
#include <cstring>
#include <type_traits>
#include <lclib-cxx/TypeTraits.hpp>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <lclib-cxx/security/SecureAllocator.hpp>
#include <lclib-cxx/security/MessageDigest.hpp>
#include <lclib-cxx/security/SHA.hpp>
namespace security{

	template<typename byte=std::byte,typename=std::enable_if_t<is_byte_v<byte>>>  class PKCS5Padding{
	public:
		static_assert(is_byte_v<byte>,"PKCS5Padding must be instantiated with a byte type");
		using data_type = byte;
		static std::size_t paddedSize(std::size_t sz){
			return sz+(16-(sz&0xf));
		}
		static void pad(data_type d,std::size_t sz){
			const std::size_t N = 16-(sz&0xf);
			memset(d+sz,N,N);
		}
		static std::size_t unpad(data_type d,std::size_t sz){
			const std::size_t N = static_cast<std::size_t>(d[sz]);
			return sz-N;
		}
	};
	template<std::size_t N,typename byte=std::byte> class AESKey{
	private:
		static_assert(is_byte_v<byte>,"AESKey must be instantiated with a byte type");
		static_assert(N==128||N==192||N==256,"Only AES 128, 192, and 256 are supported");
		byte key[N/8];
	public:
		static constexpr const char algorithm[]= "AES";
		using data_type = const byte*;
		AESKey(const byte(&arr)[N/8]){
			memcpy(key,arr,N/8);
		}
		~AESKey(){
			memset(key,0,N/8);
		}
		void destroy(){
			memset(key,0,N/8);
		}
		data_type data()const{
			return key;
		}
		std::size_t size()const{
			return N/8;
		}
	};
	//Deduction Guides
	template<typename byte> AESKey(byte(&)[16])->AESKey<128,byte>;
	template<typename byte> AESKey(byte(&)[24])->AESKey<192,byte>;
	template<typename byte> AESKey(byte(&)[32])->AESKey<256,byte>;

	/**
	 * AES Algorithm Implementation.
	 * AESCipherAlgorithm satisfies SymmetricCipherAlgorithm.
	 * AESCipherAlgorithm is only defined for N=128, N=192, and N=256
	 */
	template<std::size_t N,typename byte=std::byte,typename Padding=PKCS5Padding<byte>>
			struct AESCipherAlgorithm{
	};
	template<typename byte,typename Padding> struct AESCipherAlgorithm<128,byte,Padding>{
		static_assert(is_byte_v<byte>,"AESCipherAlgorithm must be instantiated with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		using padding = Padding;
		using key = AESKey<128,byte>;
	private:
		EVP_CIPHER_CTX* _key;
	public:
		void init(const key& k,bool enc,input_type iv){
			_key = EVP_CIPHER_CTX_new();
			if(enc)
				EVP_EncryptInit(_key,EVP_aes_128_cbc(),reinterpret_cast<const unsigned char*>(k.data()),reinterpret_cast<const unsigned char*>(iv));
			else
				EVP_DecryptInit(_key,EVP_aes_128_cbc(),reinterpret_cast<const unsigned char*>(k.data()),reinterpret_cast<const unsigned char*>(iv));
			EVP_CIPHER_CTX_set_padding(_key,0);
		}
		void update(input_type in,std::size_t sz,output_type out){
			EVP_CipherUpdate(_key,reinterpret_cast<unsigned char*>(out),NULL,reinterpret_cast<const unsigned char*>(in),sz);
		}
		void doFinal(output_type out){
			EVP_CipherFinal(_key,reinterpret_cast<unsigned char*>(out),NULL);
			EVP_CIPHER_CTX_free(_key);
		}
	};
	template<typename byte,typename Padding> struct AESCipherAlgorithm<192,byte,Padding>{
		static_assert(is_byte_v<byte>,"AESCipherAlgorithm must be instantiated with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		using padding = Padding;
		using key = AESKey<192,byte>;
	private:
		EVP_CIPHER_CTX* _key;
	public:
		void init(const key& k,bool enc,input_type iv){
			_key = EVP_CIPHER_CTX_new();
			if(enc)
				EVP_EncryptInit(_key,EVP_aes_192_cbc(),reinterpret_cast<const unsigned char*>(k.data()),reinterpret_cast<const unsigned char*>(iv));
			else
				EVP_DecryptInit(_key,EVP_aes_192_cbc(),reinterpret_cast<const unsigned char*>(k.data()),reinterpret_cast<const unsigned char*>(iv));
			EVP_CIPHER_CTX_set_padding(_key,0);
		}
		void update(input_type in,std::size_t sz,output_type out){
			EVP_CipherUpdate(_key,reinterpret_cast<unsigned char*>(out),NULL,reinterpret_cast<const unsigned char*>(out),sz);
		}
		void doFinal(output_type out){
			EVP_CipherFinal(_key,reinterpret_cast<unsigned char*>(out),NULL);
			EVP_CIPHER_CTX_free(_key);
		}
	};
	template<typename byte,typename Padding> struct AESCipherAlgorithm<256,byte,Padding>{
		static_assert(is_byte_v<byte>,"AESCipherAlgorithm must be instantiated with a byte type");
		using byte_type = byte;
		using input_type = const byte*;
		using output_type = byte*;
		using padding = Padding;
		using key = AESKey<256,byte>;
	private:
		EVP_CIPHER_CTX* _key;
	public:
		void init(const key& k,bool enc,input_type iv){
			_key = EVP_CIPHER_CTX_new();
			if(enc)
				EVP_EncryptInit(_key,EVP_aes_256_cbc(),reinterpret_cast<const unsigned char*>(k.data()),reinterpret_cast<const unsigned char*>(iv));
			else
				EVP_DecryptInit(_key,EVP_aes_256_cbc(),reinterpret_cast<const unsigned char*>(k.data()),reinterpret_cast<const unsigned char*>(iv));
			EVP_CIPHER_CTX_set_padding(_key,0);
		}
		void update(input_type in,std::size_t sz,output_type out){
			EVP_CipherUpdate(_key,reinterpret_cast<unsigned char*>(out),NULL,reinterpret_cast<const unsigned char*>(out),sz);
		}
		void doFinal(output_type out){
			EVP_CipherFinal(_key,reinterpret_cast<unsigned char*>(out),NULL);
			EVP_CIPHER_CTX_free(_key);
		}
	};
	template<std::size_t N,typename byte=std::byte> class AESKeyGenerator{
	public:
		static_assert(is_byte_v<byte>,"AESKeyGenerator must be instantiated with a byte type");
		using key = AESKey<N,byte>;
		using byte_type = byte;
		using source_type = ShadowRandom;
	private:
		ShadowRandom rand;
	public:
		AESKeyGenerator()=default;
		AESKeyGenerator(source_type&& rand):rand(rand){}
		key newKey(){
			byte bytes[N/8];
			rand.nextBytes(bytes);
			return key{bytes};
		}
	};
	template<std::size_t N,typename byte=std::byte,typename MDAlgorithm=security::SHA<256,byte>> class AESKeyDerivationAlgorithm{
	private:
		MessageDigest<MDAlgorithm> digest;
	public:
		static_assert(MDAlgorithm::digestSize>=N/8);
		using algorithm = MDAlgorithm;
		using key = AESKey<N,byte>;
		using byte_type = byte;
		using input_type = const byte*;
		using password_type = const char*;
		using salt_type = const byte*;
		key deriveKey(input_type data,std::size_t sz){
			byte bytes[MDAlgorithm::digestSize];
			byte keyBytes[N/8];
			digest(data,sz,bytes);
			memcpy(keyBytes,bytes,N/8);
			key ret{keyBytes};
			memset(bytes,0,MDAlgorithm::digestSize);
			memset(keyBytes,0,N/8);
			return std::move(ret);
		}
		key deriveKey(password_type pwd,salt_type salt,std::size_t saltSz){
			const std::size_t sz = saltSz+strlen(pwd);
			const std::size_t plen = strlen(pwd);
			byte* buff = new byte[sz];
			memcpy(buff,pwd,plen);
			memcpy(buff+plen,salt,saltSz);
			key ret{deriveKey(buff,sz)};
			memset(buff,0,sz);
			delete[] buff;
			return std::move(ret);
		}
	};

}




#endif /* __INCLUDE_SECURITY_AES_HPP__2018_09_17_17_15_15 */
