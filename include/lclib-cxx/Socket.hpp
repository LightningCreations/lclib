/*
 * Socket.hpp
 *
 *  Created on: Sep 5, 2018
 *      Author: connor
 */

#ifndef INCLUDE_SOCKET_HPP_
#define INCLUDE_SOCKET_HPP_
#include <memory>
#include <lclib-cxx/IOWrapper.hpp>
#include <string>
#include <stdexcept>
#include <functional>
#include <lclib-cxx/Config.hpp>
using std::function;

/**
 * A Base class that describes that an socket/networking specific error occured.
 * All exceptions thrown by any method in the SocketIO Library must be a subclass
 * of this type or the behavior of such a program is undefined.
 * Each subexception contains an implementation defined, but meaningful string
 * in its error descriptor (obtained via the what() method).
 * Some general exceptions are provided but implementations may also define
 * custom subclasses to indicate implementation specific errors.
 */
class LIBLCAPI SocketException:public std::exception{};
/**
 * Indicates that a General error regarding connecting to a server
 * (or binding) occurred
 */
class LIBLCAPI ConnectionException:public SocketException{
public:
	const char* what()const noexcept(true) override;
};
/**
 * Indicates that it is impossible to bind to a given address and port for some reason.
 */
class LIBLCAPI AddressBindException:public ConnectionException{
public:
	const char* what()const noexcept(true) override;
};
/**
 * Indicates that the connection failed because an implementation defined time limit
 * expired before the connection completed.
 */
class LIBLCAPI TimedOutException:public ConnectionException{
public:
	const char* what()const noexcept(true) override;
};
/**
 * Indicates that the connection failed because the address cannot be found, or connected to
 */
class LIBLCAPI UnknownHostException:public SocketException{
public:
	const char* what()const noexcept(true) override;
};
/**
 * Indicates that some concurrency error occurred in a socket implementation
 */
class LIBLCAPI SocketConcurrencyException:public SocketException{
public:
	const char* what()const noexcept(true) override;
};

/**
 * Indicates that an operation being performed is unsupported
 */
class LIBLCAPI SocketOperationUnsupportedException:public SocketException{
public:
	const char* what()const noexcept(true) override;
};

/**
 * Thrown in threads which are currently processing active IO Requests for a socket that was abruptly closed
 * (except by timing out).
 */
class LIBLCAPI SocketStreamClosedException:public SocketException{
public:
	const char* what()const noexcept(true) override;
};


/**
 * Class which wraps an implementation of a Socket, for use in an Spi model
 * Implementations are also called Handles (to avoid confusion with the implementation of the library),
 * By default an unspecified subclass of SocketImpl is used to handle socket calls.
 * This subclass must be a default conforming implementation.
 * That is, getHostname must be unconditionally well-defined behavior (see getHostname()),
 * and getConnectedName() and getConnectedPort() are well-defined if the socket is connected.
 * A connected Socket Handle may become unconnected without being destroyed.
 * If the Handle is owned by a ServerSocket, the handle may become unconnected if the ServerSocket is closed.
 * In addition, implementations may close the socket after an unspecified amount of time during any Blocking-IO Operation.
 * If this occurs, the function in question must immediately throw a TimedOutException,
 * regardless of whether or not sufficient bytes were read to fulfill the request.
 * If the socket becomes abruptly closed for any other reason while IO Operations are in progress,
 * those operations must either complete fully (without accessing the stream), or throw a SocketStreamClosedException.
 * Socket's and there handles may be used in multithreaded contexts. It is the responsibility of
 * the implementation to ensure that appropriate thread-safety guarantees are made.
 * If a Socket cannot be used in multiple threads, any operation may throw a SocketConcurrencyException
 *  when another thread has ownership of the socket.
 * In addition, Some Socket Functions (notably The IOWrapper Interface functions) may block until they complete.
 * If an implementation does not support blocking operations, then getInputStream()/getOutputStream() may throw
 * a SocketOperationUnsupportedException, or the specific operations may throw a SocketConcurrencyException
 * if they block. Default conforming implementations are required to support blocking IO, but not to support
 * other blocking functions.
 */
class LIBLCAPI SocketImpl{
public:
	/**
	 * Attempts to connect to a server (see the documentation of Socket::connect for details an exceptions).
	 * If the socket is already connected, the behavior is undefined. It may also throw a subclass of SocketException
	 * If the socket represents a Server-side socket (returned from ServerSocketImpl::accept), the behavior is undefined
	 * If this method throws an exception it must be a subclass of SocketException or the behavior is undefined.
	 */
	virtual void connect(const std::string&,uint16_t)=0;
	/**
	 * Gets the InputStream associated with the socket. If the socket is not connected, the behavior is undefined.
	 * Otherwise this method returns a reference to an implementation defined subclass of InputStream which reads from the socket buffer or stream.
	 * The read methods of the stream may throw any subclass of SocketException.
	 * If the read methods throw any other exception type the behavior is undefined.
	 */
	virtual InputStream& getInputStream()=0;
	/**
	 * Gets the OutputStream associated with the socket. If the socket is not connected, the behavior is undefined.
	 * Otherwise this method returns a reference to an implementation defined subclass of OutputStream which writes to the socket buffer or stream.
	 * It is implementation defined whether the Stream will buffer data or write it immediately.
	 * flush should always be used if timing of the write is critical.
	 * The write methods and flush method may throw any subclass of SocketException
	 * (following all rules for OutputStream::write and OutputStream::flush throwing exceptions).
	 * If the write methods or flush throws any other exception type the behavior is undefined.
	 */
	virtual OutputStream& getOutputStream()=0;
	/**
	 * Closes the underlying socket and both underlying streams
	 */
	virtual ~SocketImpl()=default;


	/**
	 * Checks if the Socket is connected to something.
	 * Returns true if it is already connected, false otherwise.
	 * This may or may not be congruent to an imaginary function hasSuccessfullyConnected,
	 * which returns true if connect has successfully returned, as the connection to the server may or may not have closed.
	 * This predicate should be checked before using an connection sensitive functions to avoid undefined behavior.
	 * If a connection was established, but was closed for some reason, this function is false.
	 */
	virtual bool isConnected()const noexcept(true)=0;
	/**
	 * Gets the hostname bound to this socket.
	 * If this is not a default conforming implementation, and the socket is not connected the behavior is unspecified (and may be undefined).
	 * If the implementation is default conforming, then the result is either the public or private address that is associated with localhost
	 */
	virtual const std::string& getHostname()const=0;
	/**
	 * Gets the hostname of the Server this socket is connected to.
	 * If the socket is not connected, the behavior is undefined.
	 * Otherwise if the implementation is not default conforming, the behavior is unspecified(and may be undefined)
	 * If the implementation is default conforming, and the socket is connected, it returns either the hostname passed to connect or an equivalent hostname (including one that went through DNS Transformations)
	 */
	virtual const std::string& getConnectedName()const=0;
	/**
	 * Gets the port of the server this socket is connected to.
	 * If the socket is not connected, the behavior is undefined.
	 * Otherwise if the implementation is not default conforming, the behavior is unspecified (and may be undefined)
	 * If the implementation is default conforming, and the socket is connected, it returns the port passed to connect
	 */
	virtual uint16_t getPort()const=0;
};
/**
 * Represents a handle for a Server Socket.
 * Server Socket Handles act similarly to Socket Handles except they handle functions
 * specific to servers, and IO Streams are shared across all connections.
 * Server Socket Handles may be used in a multithreaded context.
 * Implementations are responsible for ensuring thread-safety.
 */
class LIBLCAPI ServerSocketImpl{
public:
	/**
	 * Binds this server socket to a specified address
	 * See ServerSocket::bind for exceptions.
	 * The behavior is undefined if the ServerSocket is already bound
	 */
	virtual void bind(const std::string&,uint16_t)=0;
	/**
	 * Accepts a connection and returns an implementation defined handle for the new connection.
	 * The behavior is undefined if the socket is not connected.
	 * If no connection is pending, then accept blocks until one is.
	 *  This may cause a SocketConcurrencyException to be thrown.
	 * The implementation is responsible for keeping track of all active connections, to close them when the server closes,
	 * or to manage those connections.
	 */
	virtual std::unique_ptr<SocketImpl> accept()=0;
	/**
	 * Returns a implementation defined input stream which reads data from all
	 * open connections. This is not required to be supported.
	 * If Shared Input Streams are not supported, a SocketOperationUnsupportedException may be thrown.
	 */
	virtual InputStream& getInputStream()=0;
	/**
	 * Returns an implementation defined output stream which sends written data to all open connection (Broadcast Stream).
	 * If Shared Input Streams are not supported, a SocketOperationUnsupportedException may be thrown.
	 */
	virtual OutputStream& getOutputStream()=0;
	/**
	 * Closes the Server. All connected Sockets are closed as a side-effect.
	 * This may cause SocketStreamClosedExceptions to be thrown in other threads.
	 */
	virtual ~ServerSocketImpl()=default;

};

class LIBLCAPI Socket{
private:
	std::unique_ptr<SocketImpl> impl;
	Socket(const Socket&)=delete;
	Socket& operator=(const Socket&)=delete;
public:
	/**
	 * Constructs a socket with a given implementation handle.
	 * If the handle is connected already the socket can be used as if it had been connected,
	 * otherwise it must be connected before it can be used.
	 */
	explicit Socket(std::unique_ptr<SocketImpl>&&);
	/**
	 * Default constructs a Socket. The underlying implementation is constructed in
	 * an implementation defined manor. The socket cannot be used until connect is called.
	 */
	Socket();
	/**
	 * Constructs a Socket with the implementation default Handler for socket functions,
	 * and connects the socket to the server.
	 * This constructor acts as if it default constructs the socket,
	 * then calls the connect function with the arguments.
	 */
	Socket(const std::string&,uint16_t);
	Socket(Socket&&)=default;
	Socket& operator=(Socket&&)=default;
	/**
	 * Gets the input stream of this socket.
	 * The behavior is undefined if the socket is not connected.
	 */
	InputStream& getInputStream();
	/**
	 * Gets the output stream of this socket.
	 * The behavior is undefined if the socket is not connected.
	 */
	OutputStream& getOutputStream();
	/**
	 * Attempts to connect to a server.
	 * The Socket first checks if it can find a server with the given hostname
	 * If it cannot then an UnknownHostException is thrown.
	 * Otherwise it attempts to connect to the server with the given hostname and port.
	 * The call will block until the connection completes or an unspecified ammount of time passes, in which case a TimedOutException is thrown
	 * If the server refuses the connection for some reason, a ConnectionException, or an unspecified subclass of ConnectionException is thrown.
	 * If the program is multithread, and cannot block the current thread,
	 * or attempts to create a new thread but cannot for any reason, then a SocketConcurrencyException is thrown.
	 * If the connection fails for any other reason, then an unspecified subclass of ConnectionException is thrown.
	 * If the socket is already connected the behavior is undefined.
	 * \Exceptions: The implementation may throw any of the exceptions listed above, including unspecified subclasses of ConnectionException.
	 * If the implementation throws any exception not detailed above, the behavior is undefined
	 * \Exception Safety: If an exception occurs, the socket is left in an valid but unspecified state, which is not connected to any server.
	 */
	void connect(const std::string&,uint16_t);
	/**
	 * Checks if the Socket is connected to a server already.
	 * Returns true if it has successfully connected to a server,
	 * false otherwise.
	 * \Exception Guarantee: This function will not throw any exceptions
	 */
	bool isConnected()const noexcept(true);
	/**
	 * Gets the hostname that the Socket is bound to. (If the socket is a local socket, this is almost always localhost)
	 * The behavior is unspecified (and may be undefined) if the socket is not connected and is not using the default Socket Implementation.
	 */
	const std::string& getHostname()const;
	/**
	 * Gets the hostname of the Server that the socket is connected to.
	 * The behavior is undefined if the socket is not connected to a server.
	 * The behavior is unspecified (and may be undefined) if the socket is not using the default Socket Implemenation.
	 */
	const std::string& getConnectedName()const;
	/**
	 * Gets the port which the server is connected to.
	 * The behavior is undefined if the socket is not connected to a server.
	 * The behavior is unspecified (and may be undefined) if the socket is not using the default Socket Implemenation.
	 */
	uint16_t getConnectedPort()const;
};

class LIBLCAPI ServerSocket{
private:
	std::unique_ptr<ServerSocketImpl> impl;
	ServerSocket(const ServerSocket&)=delete;
	ServerSocket& operator=(const ServerSocket&)=delete;
public:
	/**
	 * Default Constructs a ServerSocket.
	 * The Handle is constructed in an implementation defined manor, and is not bound.
	 * The server must be bound to a host before it can be used.
	 */
	ServerSocket();
	/**
	 * Constructs a ServerSocket, as if by the Default Constructor,
	 * and binds it to the given hostname and port.
	 */
	ServerSocket(const std::string&,uint16_t);
	/**
	 * Constructs a new ServerSocket with a given implemenation.
	 */
	explicit ServerSocket(std::unique_ptr<ServerSocketImpl>&&);
	ServerSocket(ServerSocket&&)=default;
	ServerSocket& operator=(ServerSocket&&)=default;
	/**
	 * Accepts an incoming connection and returns are Socket that handles that connection.
	 * If the server is not bound the behavior is undefined.
	 * If there is no pending connection, the call may block. This may throw a SocketConcurrencyException as per ServerSocketImpl::accept
	 */
	Socket accept();
	/**
	 * Binds the server socket to a given hostname and port.
	 * If the current host cannot open a server at the given hostname (as it is an external ip address, or a domain which resolves to such an ip address,
	 * or another socket (potentially in a different process) already has exclusive access to the ip and port),
	 * an AddressBindException is thrown.
	 * Otherwise if exception is a domain name, and the domain name cannot be resolved, an UnknownHostException is thrown.
	 * Otherwise the server is bound and opened at the specified hostname and port.
	 * If the server is already bound the behavior is undefined
	 */
	void bind(const std::string&,uint16_t);
	/**
	 * Obtains an implementation defined reference to an InputStream which reads from all open connections.
	 * The behavior is undefined if the server is not bound.
	 * Server Wide IO is not mandated, and thus this method may throw a SocketOperationUnsupportedException
	 */
	InputStream& getInputStream();
	/**
	 * Obtains an implemenation defined reference to an OutputStream which writes to all open connections.
	 * The behavior is undefined if the server is not bound.
	 * Server Wide IO is not mandated, and thus this method may throw a SocketOperationUnssuportedException
	 */
	OutputStream& getOutputStream();
};

/**
 * Attempts to set the default supplier for creating socket handles.
 * The library implementation is not required to acknowledge this request.
 * If the implementation does not want to update the default handler, a SocketOperationUnsupportedException is thrown.
 * The behavior is undefined unless the function returns a unique instance of a consistent subclass of SocketImpl,
 * which is default conforming.
 */
LIBLCAPI void setDefaultSocketHandleConstructor(function<std::unique_ptr<SocketImpl>()>);
/**
 * Attempts to set the default supplier for creating server socket handles.
 * The library implementation is not required to acknowledge this request.
 * If the implementation does not want to update the default handler, a SocketOperationUnsupportedException is thrown.
 * The behavior is undefined unless the function returns a unique instance of a consistent subclass of ServerSocketImpl.
 */
LIBLCAPI void setDefaultServerSocketHandleConstructor(function<std::unique_ptr<ServerSocketImpl>()>);
/**
 * Constructs a new socket handle. If the default Socket Handle constructor is set,
 * it obtains a new instance from that and returns it.
 * Otherwise the handle is constructed in an implementation defined way.
 */
LIBLCAPI std::unique_ptr<SocketImpl> newSocketHandle();
/**
 * Constructs a new server socket handle. If the default Server Socket Handle constructor is set,
 * it obtains a new instance from that and returns it.
 * Otherwise the handle is constructed in an implementation defined way.
 */
LIBLCAPI std::unique_ptr<ServerSocketImpl> newServerSocketHandle();

#endif /* INCLUDE_SOCKET_HPP_ */
