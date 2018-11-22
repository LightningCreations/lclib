/*
 * Cipher.hpp
 *
 *  Created on: Sep 18, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_SECURITY_CIPHER_HPP__2018_09_18_08_41_26
#define __INCLUDE_SECURITY_CIPHER_HPP__2018_09_18_08_41_26
#include <lclib-cxx/Config.hpp>
#include <lclib-cxx/security/Concepts.hpp>
#include <lclib-cxx/IOWrapper.hpp>
namespace security{
	template<typename Algorithm> class CipherOutputStream final:public FilterOutputStream{
	public:
		static_assert(traits::is_cipher_algorithm_v<Algorithm>,"CipherOutputStream requires CipherAlgorithm");
		using byte_type = typename Algorithm::byte_type;
		using padding = typename Algorithm::padding;
		using input_type = typename Algorithm::input_type;
		using output_type = typename Algorithm::output_type;
	private:
		Algorithm underlying;
		byte_type* buffer;
		std::size_t bufferSize;
		std::size_t bufferPos;
		void resize(std::size_t nSize){
			byte_type* nbuff = new byte_type[bufferSize+nSize];
			memcpy(nbuff,buffer,bufferSize);
			delete[] buffer;
			buffer = nbuff;
			bufferSize = nSize;
		}
	public:
		template<typename Key,typename=decltype(underlying.init(std::declval<Key>(),true,std::declval<input_type>()))>
			CipherOutputStream(OutputStream& in,const Key& k,input_type iv):FilterOutputStream(in),buffer(nullptr),bufferSize{0},bufferPos{0}{
			underlying.init(k,true,iv);
		}
		~CipherOutputStream(){
			this->flush();
		}
		std::size_t write(const void* vptr,std::size_t sz){
			if((bufferSize-bufferPos)<padding::getPaddedSize(sz))
				resize(padding::getPaddedSize(sz));
			memcpy(buffer,vptr,sz);
			bufferPos += padding::getPaddedSize(sz);
			return sz;
		}
		void write(uint8_t b){
			write(&b,sizeof(b));
		}
		void flush(){
			if(bufferPos>0){
				padding::pad(buffer,bufferPos);
				bufferPos = padding::getPaddedSize(bufferPos);
				underlying.update(reinterpret_cast<input_type>(buffer),bufferPos,reinterpret_cast<output_type>(buffer));
				this->FilterOutputStream::write(buffer,bufferPos);
			}
			this->FilterOutputStream::flush();
		}
	};
	template<typename Algorithm> class CipherInputStream final:public FilterInputStream{
	private:
		Algorithm underlying;
	public:
		static_assert(traits::is_cipher_algorithm_v<Algorithm>,"CipherInputStream requires CipherAlgorithm");
		using algorithm = Algorithm;
		using byte_type = typename Algorithm::byte_type;
		using input_type = typename Algorithm::input_type;
		using output_type = typename Algorithm::output_type;
		using padding = typename Algorithm::padding;
		template<typename Key,typename=decltype(underlying.init(std::declval<const Key&>(),false,std::declval<input_type>()))>
			CipherInputStream(InputStream& in,const Key& k,input_type iv):FilterInputStream(in){
			underlying.init(k,false,iv);
		}
		/**
		 * Reads n bytes from the underlying stream into ptr.
		 * The specific number of bytes read is dependent on the Padding of the agorithm.
		 * This method will read that many bytes, then decrypt them, and storing the result in ptr.
		 * This method is intended to read the entire stream at once.
		 * \Exception Guarantee: If an exception is thrown, ptr is left in the original state, however the underlying stream is left in an Unspecified State
		 */
		std::size_t read(void* ptr,std::size_t bytes){
			std::size_t sz = padding::getPaddedSize(bytes);
			byte_type* buff = new byte_type[sz];
			try{
				this->FilterInputStream::read(buff,sz);
			}catch(...){
				delete[] buff;
				throw;
			}
			underlying.update(reinterpret_cast<input_type>(buff),sz,reinterpret_cast<output_type>(buff));
			sz = padding::unpad(buff,sz);
			memcpy(ptr,buff,sz);

			delete[] buff;
			return sz;
		}
	};
}
#endif /* __INCLUDE_SECURITY_CIPHER_HPP__2018_09_18_08_41_26 */
