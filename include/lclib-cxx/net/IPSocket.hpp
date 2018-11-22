/*
 * IPSocket.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_NET_IPSOCKET_HPP__2018_10_24_09_09_29
#define __INCLUDE_LCLIB_CXX_NET_IPSOCKET_HPP__2018_10_24_09_09_29

#include <lclib-cxx/Config.hpp>
extern "C"{
#if defined(LIBLCCXX_OS_WINDOWS)
#include <winsock2.h>
#elif defined(LIBLCCXX_POSIX)
#include <sys/socket.h>
#include <netinet/in.h>
#define SOCKET int
#else
#error No suitable socket definitions exist
#endif
};

#include <string>
#include <cstdint>
#include <lclib-cxx/IOWrapper.hpp>

namespace net{
	class InetSocketAddress{
	private:
		sockaddr_in addr;
		std::string name;
		uint16_t port;
	public:
		using native_type = sockaddr_in;
		using char_type = char;
		InetSocketAddress(const std::string&,uint16_t);
		InetSocketAddress(std::string&&,uint16_t);
		InetSocketAddress();
		const std::string& getAddress()const;
		uint16_t getPort()const;
		template<typename Allocator>
			operator std::basic_string<char_type,std::char_traits<char_type>,Allocator>()const{
			std::basic_string<char_type,std::char_traits<char_type>,Allocator> ret{name.begin(),name.end()};
			ret += ":";
			ret += std::to_string(port);
			return std::move(ret);
		}
		const native_type& getNativeType()const{
			return addr;
		}
	};
	class InetSocketHandle;
	class InetServerSocketHandle;
	class InetSocketInputStream final:public InputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
	private:
		friend class InetSocketHandle;
		SOCKET sock;
		InetSocketInputStream(SOCKET);
	public:
		bool checkError()const noexcept(true);
		void clearError()noexcept(true);
		std::size_t read(void*,std::size_t);
		int read();
	};

	class InetSocketOutputStream final:public OutputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
	private:
		friend class InetSocketHandle;
		SOCKET sock;
		InetSocketOutputStream(SOCKET);
	public:
		bool checkError()const noexcept(true);
		void clearError()noexcept(true);
		std::size_t write(const void*,std::size_t);
		void write(uint8_t);
	};

	class InetSocketHandle{
	public:
		using native_type = SOCKET;
		using address_type = InetSocketAddress;
	private:
		friend class InetServerSocketHandle;
		native_type sock;
		address_type addr;
		InetSocketInputStream in;
		InetSocketOutputStream out;
		bool closed;
		InetSocketHandle(native_type sock);
	public:
		InetSocketHandle(address_type&& addr);
		InetSocketHandle(InetSocketHandle&&);
		InetSocketHandle& operator=(InetSocketHandle&&);
		~InetSocketHandle();
		void close();
		bool isOpen()const;
		const address_type& getConnectedAddr()const EXPECTS(isOpen());
		native_type getNative()EXPECTS(isOpen());
		InputStream& getInputStream()EXPECTS(isOpen());
		OutputStream& getOutputStream()EXPECTS(isOpen());
	};

	class InetServerSocketHandle{
	public:
		using socket_type = InetSocketHandle;
		using native_type = SOCKET;
		using address_type = InetSocketAddress;
	private:
		InetServerSocketHandle(address_type&& addr);
		InetServerSocketHandle(InetSocketHandle&&);
		InetServerSocketHandle& operator=(InetServerSocketHandle&&);
		~InetServerSocketHandle();
		bool isBound()const;
		void close();
		native_type getNative()EXPECTS(isBound());
		socket_type accept()EXPECTS(isBound());
		const address_type& getBoundAddress()const EXPECTS(isBound());
	};
}

#ifdef LIBLCCXX_POSIX
#undef SOCKET
#endif



#endif /* __INCLUDE_LCLIB_CXX_NET_IPSOCKET_HPP__2018_10_24_09_09_29 */
