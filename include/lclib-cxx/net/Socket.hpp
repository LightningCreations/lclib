/*
 * Socket.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_NET_SOCKET_HPP__2018_10_17_11_40_37
#define __INCLUDE_LCLIB_CXX_NET_SOCKET_HPP__2018_10_17_11_40_37
#include <type_traits>
#include <lclib-cxx/TypeTraits.hpp>
#include <utility>
#include <lclib-cxx/IOWrapper.hpp>
#include <lclib-cxx/Config.hpp>
namespace net{
	template<typename SocketHandle> class Socket{
	private:
		SocketHandle handle;
	public:
		using handle_type = SocketHandle;
		using native_type = typename SocketHandle::native_type;
		using address_type = typename SocketHandle::address_type;
		template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<address_type,Args...>>>
				Socket(Args&&... args):handle(address_type{std::forward<Args>(args)...}){}
		Socket(handle_type&& h):handle(std::move(h)){}
		Socket(address_type&& a):handle(std::move(a)){}
		bool isOpen(){
			return handle.isOpen();
		}
		auto getInputStream() -> decltype(handle.getInputStream()) EXPECTS(isOpen()){
			return handle.getInputStream();
		}
		auto getOutputStream() -> decltype(handle.getOutputStream()) EXPECTS(isOpen()){
			return handle.getOutputStream();
		}
		void close() EXPECTS(isOpen()){
			handle.close();
		}
		decltype(auto) getNative() EXPECTS(isOpen()){
			return handle.getNative();
		}
		const address_type& getConnectedAddress() EXPECTS(isOpen()){
			return handle.getConnectedAddress();
		}
		explicit operator bool()const{
			return handle.isOpen();
		}
		bool operator!()const{
			return !handle.isOpen();
		}

		typename handle_type::packet_type recieve() EXPECTS(isOpen()){
			return handle.recieve();
		}
		void send(const typename handle_type::packet_type& p) EXPECTS(isOpen()){
			handle.send(p);
		}
	};
	template<typename ServerSocketHandle> class ServerSocket{
	private:
		ServerSocketHandle handle;
	public:
		using handle_type = ServerSocketHandle;
		using socket_handle_type = typename ServerSocketHandle::socket_type;
		using address_type = typename ServerSocketHandle::address_type;
		using native_type = typename ServerSocketHandle::native_type;
		using socket_type = Socket<socket_handle_type>;
		template<typename... Args,typename=std::enable_if_t<std::is_constructible_v<address_type,Args...>>>
				ServerSocket(Args&&... args):handle(address_type{std::forward<Args>(args)...}){}
		ServerSocket(address_type&& a):handle(std::move(a)){}
		bool isBound()const{
			return handle.isBound();
		}
		socket_type accept()const EXPECTS(isBound()){
			return socket_type{handle.accept()};
		}
		void close(){
			handle.close();
		}
		const address_type& getConnectedAddress()const EXPECTS(isBound()){
			return handle.getConnectedAddress();
		}
		decltype(auto) getNative() EXPECTS(isBound()){
			return handle.getNative();
		}

		explicit operator bool()const{
			return handle.isBound();
		}
		bool operator!()const{
			return !handle.isBound();
		}
	};
}



#endif /* __INCLUDE_LCLIB_CXX_NET_SOCKET_HPP__2018_10_17_11_40_37 */
