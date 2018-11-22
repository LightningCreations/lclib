/*
 * Concepts.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: connor
 */

#ifndef __INCLUDE_LCLIB_CXX_NET_CONCEPTS_HPP__2018_10_17_09_50_16
#define __INCLUDE_LCLIB_CXX_NET_CONCEPTS_HPP__2018_10_17_09_50_16
#include <string>
#include <lclib-cxx/IOWrapper.hpp>
#include <type_traits>
#include <lclib-cxx/TypeTraits.hpp>
#include <lclib-cxx/Config.hpp>
#include <lclib-cxx/Span.hpp>
namespace net{
	/*
	 * Placeholder for an unspecified type.
	 */
	using unspecified_type = void*;
	struct SocketAddress{
	public:
		/*
		 * The type of the underlying Socket Implementation.
		 */
		using native_type = unspecified_type;
		/*
		 * The char_type of strings the SocketType can be converted to.
		 */
		using char_type = char;
		/**
		 * SocketAddress is constructible with an unspecified set of arguments,
		 * but must have at least one Non-default Constructor
		 */
		template<typename... Args> SocketAddress(Args&&...);
		/**
		 * SocketAddress is default Constructible.
		 * A default constructed SocketAddress represents the current machine.
		 * For example, if the SocketAddress represents an IPv4 address,
		 *  it can be 127.0.0.1 (localhost). It can also be the public local IP Address.
		 * The port associated with the Socket Address, if the socket address maps a port,
		 * is unspecified.
		 */
		SocketAddress();
		/**
		 * SocketAddresses are movable.
		 * If resources are used by a SocketAddress, then those resources are copied
		 * by the SocketAddress. It is unspecified if SocketAddresses are copyable.
		 * If resources are not used by a SocketAddress, this constructor should be trivial.
		 */
		SocketAddress(SocketAddress&&);
		/*
		 * If resources are used by a SocketAddress, then those resources are released by the destructor.
		 * Otherwise this should be trivial.
		 */
		~SocketAddress();
		/**
		 * SocketAddresses are Move-assignable.
		 * If a SocketAddress uses any resources, those resources should be cleaned up,
		 * then the resources of rhs should be moved to this.
		 * Otherwise, this operator should be trivial.
		 */
		SocketAddress& operator=(SocketAddress&& rhs);
		/**
		 * SocketAddresses are convertible to std::basic_string with CharT being char_type,
		 * an implementation defined CharTraits (possibly any CharTraits), and any Allocator.
		 * Whether or not a SocketAddress is convertible to any other string type is implementation defined.
		 * The String representation of a SocketAddress is implementation defined.
		 */
		template<typename CharTraits,typename Allocator>
			operator std::basic_string<char_type,CharTraits,Allocator>()const;

		/**
		 * Returns an unspecified native representation of this address.
		 * May return by value, or return by const reference.
		 */
		native_type /*or const native_type&*/ getNative()const;
	};
	/**
	 * SocketHandle concept
	 */
	class SocketHandle{
	private:
		SocketHandle(const SocketHandle&)=delete;
		SocketHandle& operator=(const SocketHandle&)=delete;
	public:
		/*
		 * The type of the Address to connect to.
		 * Must Satisfy SocketAddress;
		 */
		using address_type = SocketAddress;
		/**
		 * The native type of the Socket Handle.
		 * An unspecified type.
		 */
		using native_type = unspecified_type;

		/**
		 * Constructs a handle connected to the given address.
		 * If the connection fails, an unspecified exception may be thrown, with a meaningful error message.
		 */
		SocketHandle(address_type);
		/**
		 * Constructs a new Socket by Moving the given socket.
		 * If the socket is still open, its resources are moved to this.
		 * The moved-from socket is left in a valid, but unspecified state, which is considered closed.
		 */
		SocketHandle(SocketHandle&&);
		/**
		 * Moves a given socket to this.
		 * If both sockets are open, this method effectively closes this,
		 *  invalidates existing InputStreams and OutputStreams,
		 *  moves the given socket to this, as though directly, by the Move Constructor
		 * If the given socket is closed, but this socket is open, this is effecitvely close().
		 * If this socket is closed, but the given socket is open,
		 *  then this effectively move-constructs a new socket into this socket.
		 * If both sockets are closed, this method has no effect.
		 *
		 * The moved-from socket is left in a valid, but unspecified state, which is considered closed.
		 */
		SocketHandle& operator=(SocketHandle&&);
		/**
		 * Destroys this socket handle, by closing it.
		 * If the socket is already closed, the method has no effect
		 */
		~SocketHandle();
		/**
		 * Checks if the socket is open. Returns false if the socket was closed,
		 * either by a call to close(), being moved, or indirectly
		 *  (such as the owning server socket being destroyed), and true otherwise.
		 */
		bool isOpen()const;
		/**
		 * Closes the socket. If the socket is already closed, then has no effect.
		 * Otherwise, closes the connection, and frees the resources associated with the socket.
		 * Subsequent calls to isOpen() will return false until the Socket is move-assigned.
		 */
		void close() ENSURES(!isOpen());

		/**
		 * Obtains a reference to the address the socket is connected to.
		 * The behavior is undefined if the socket is closed.
		 */
		const address_type& getConnectedAddress()const EXPECTS(isOpen());
		/**
		 * Obtains an unspecified value or reference to the native handle type.
		 * The behavior is undefined if the socket is closed.
		 */
		native_type /*or const native_type&*/ getNative() EXPECTS(isOpen());
	};

	/**
	 * Subconcept which defines types socket types which have an associated InputStream,
	 * and an associated OutputStream.
	 */
	class StreamSocketHandle:public SocketHandle{
	public:
		/**
		 * Obtains the InputStream associated with this socket handle.
		 * Invalidated if the socket is closed, or moved.
		 * The behavior is undefined if the socket is closed when the method is called.
		 */
		InputStream& getInputStream() EXPECTS(isOpen());
		/**
		 * Obtains the output stream associated with this socket handle.
		 * Invalidated if the socket is closed or moved.
		 * The behavior is undefined if the socket is closed when the method is called.
		 */
		OutputStream& getOutputStream() EXPECTS(isOpen());
	};

	/**
	 * Concept which represents a DatagramPacket.
	 * DatagramPackets represent a contiguous sequence of bytes.
	 * There is no other requirements, other then that it satisfies ContiguousContainer and SequenceContainer,
	 * and has a member type byte_type, that aliases element_type.
	 */
	class DatagramPacket{
	public:
		using byte_type = std::byte;
		using element_type = byte_type;
		using pointer = element_type*;
		using const_pointer = const element_type*;
		using reference = element_type&;
		using const_reference = const element_type&;
		using iterator = element_type*;
		using const_iterator = const element_type*;
		using size_type = std::size_t;
		using index_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		pointer data()noexcept(true);
		const_pointer data()const noexcept(true);
		size_type size()const noexcept(true);
		reference operator[](index_type idx) EXPECTS(idx<size());
		const_reference operator[](index_type)const EXPECTS(idx<size());
		iterator begin();
		const_iterator begin()const;
		const_iterator cbegin()const;
		iterator end();
		const_iterator end()const;
		const_iterator cend()const;
	};

	/**
	 * A Subconcept which represents a Datagram Socket.
	 */
	class DatagramSocketHandle:public SocketHandle{
	public:
		/**
		 * The packet type. Must satisfy DatagramPacket
		 */
		using packet_type = DatagramPacket;
		/*
		 * The size type of the packet.
		 */
		using size_type = typename packet_type::size_type;
		/**
		 * Sends a given packet.
		 * The behavior is undefined if the socket is closed
		 */
		void send(const packet_type&) EXPECTS(isOpen());
		/**
		 * Recieves a packet with a given size.
		 * The behavior is undefined if the socket is closed.
		 */
		packet_type recieve(size_type) EXPECTS(isOpen());
	};

	class ServerSocketHandle{
	public:
		/*
		 * The Socket Handle type returned by accept(). Must satisfy SocketHandle
		 */
		using socket_type = SocketHandle;

		/**
		 * The address_type the socket can bind to.
		 * May be distinct from typename socket_type::address_type.
		 */
		using address_type = SocketAddress;

		/**
		 * The native handle type of this server socket.
		 */
		using native_type = unspecified_type;
		/**
		 * Constructs a new ServerSocket bound to the given address.
		 * May throw any exception if binding the socket fails.
		 */
		ServerSocketHandle(address_type);
		/**
		 * Constructs a new ServerSocket by moving the resources of the given ServerSocket.
		 * If the given ServerSocket is closed, this socket is closed.
		 * Otherwise, closes the given server socket,
		 * and constructs a new socket bound to the same address as the given server socket was bound to (without reconnected).
		 * Does not close any sockets owned by the given server socket.
		 * The given socket is left in a valid, but unspecified state, which is considered to be closed.
		 */
		ServerSocketHandle(ServerSocketHandle&&);
		/**
		 * Move-assigns the given handle to this.
		 * If both sockets are bound, then closes this socket, and moves the given socket to this, as though by move constructing it inline.
		 * If the given socket is bound, but this is not, moves the given socket to this as though by move constructing the socket inline.
		 * If this socket is bound, but the given socket is not, then effectively closes this socket.
		 * If neither socket is bound, has no effect.
		 * The given socket is left in a valid, but unspecified state, which is considered to be closed.
		 */
		ServerSocketHandle& operator=(ServerSocketHandle&&);
		/**
		 * If this socket is bound, it is closed.
		 * All sockets returned by accept are also closed indirectly.
		 */
		~ServerSocketHandle();
		/**
		 * Returns false if the socket is closed,
		 * either by moving it, or indirectly for some reason, and true otherwise.
		 */
		bool isBound()const;
		/**
		 * Closes this socket if its bound.
		 * Otherwise, has no effect.
		 * Subsequent calls to isBound() will return false until the socket is move-assigned.
		 */
		void close() ENSURES(!isBound());
		/**
		 * Blocks until a socket is attempting to connect to this server socket,
		 * and returns a handle to it. The socket is owned by this server socket.
		 * If the socket is closed (except by moving it), the socket will be closed,
		 * as though by calling its close method().
		 * The behavior is undefined if this socket is closed.
		 */
		socket_type accept() EXPECTS(isBound());
		/**
		 * Obtains the address that this server socket is bound to.
		 * The behavior is undefined if this socket is closed.
		 */
		const address_type& getBoundAddress()const EXPECTS(isBound());
		/**
		 * Returns the native handle of this Socket.
		 * The behavior is undefined if this socket is closed.
		 */
		native_type /*or const native_type&*/ getNative() EXPECTS(isBound());
	};

	namespace traits{
		//TODO Add Networking Concept Type Traits
	}
}




#endif /* __INCLUDE_LCLIB_CXX_NET_CONCEPTS_HPP__2018_10_17_09_50_16 */
