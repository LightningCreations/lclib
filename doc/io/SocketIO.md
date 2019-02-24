# SocketIO Library #

Included by `<lclib/Socket.hpp>` <br/>
Part of the io library and the Networking Library<br/>

## Deprecation ##

Deprecated as of 1.3

This library is deprecated for the new Template Based Networking Library. 

This library was based heavily on having a single implementation, using a Service Provider Interface system, and runtime polymorphism. 

The New Networking Library uses Template based SPIs, and static polymorphism to implement generic services, which is generally more efficient than dynamic (runtime) polymorphism. It also supports extended socket interfaces, such as Datagram Sockets, and Non IP Based Sockets. 

### Removal ###

Set for Removal in Version 1.4 (upcoming)

This header and all features documented within are set for removal in an upcoming version of lclib-c++. 
Therefore use of this header and its features are extremely discouraged, as the use will not be compatible with that version. 

Use of these headers and features is AT YOUR OWN RISK, and you acknowledge that in the future, any code which uses it will cease to work. Neither Lightning Creations nor the library vendor may be held liable for any damage caused by this removal.


## Synopsis ##

```cpp
class SocketException:public std::exception;
class ConnectionException:public SocketException;
class AddressBindException:public ConnectionException;
class TimedOutException:public ConnectionException;
class UnknownHostException:public SocketException;
class SocketConcurrencyException:public SocketException;
class SocketOperationUnsupportedException:public SocketException;
class SocketStreamClosedException:public SocketException;
class SocketImpl;
class ServerSocketImpl;
class Socket;
class ServerSocket;
void setDefaultSocketHandleConstructor(std::function<std::unique_ptr<SocketImpl>()>);
void setDefaultServerSocketHandleConstructor(std::function<std::unique_ptr<ServerSocketImpl>()>);
std::unique_ptr<SocketImpl> newSocketHandle();
std::unique_ptr<ServerSocketImpl> newServerSocketHandle();
```

## Overview ##

Provides a strong, consistent Socket Based Networking Interface built off of IOWrapper and the Service Provider Interface Model. The providers for Sockets/Servers are called Handles and the default Provider can be set via setDefaultSocketHandleConstructor and setDefaultServerSocketHandleConstructors. 
An implementation specific default handle constructor is provided so that sockets may be created. 
SocketException is the base class for exceptions reported by the SocketIO Library. It is guarenteed that only Exceptions of this type are thrown. The behavior of an implementation which throws any exceptions which do not derive from SocketException is undefined. 
IO Access to sockets are provided through unspecified subclasses of InputStream and OutputStream which are obtained through the handle's getInputStream/getOutputStream (which are accessed through the Socket itself).
It is implied that any undefined behavior created within functions that may throw an exception is allowed to throw an exception for purposes of the Implementation.


## Classes ##
### SocketImpl ###
SocketImpl is the base of the Socket SPI Model. It is provided by the Implementation. It is used by Socket and ServerSocketImpl.

#### Synopsis ####

```cpp
virtual void connect(const std::string&,uint16_t)=0;
virtual InputStream& getInputStream() EXPECTS(isConnected())=0;
virtual OutputStream& getOutputStream() EXPECTS(isConnected())=0;
virtual ~SocketImpl()=default;
virtual bool isConnected()const noexcept(true)=0;
virtual const std::string& getHostname()const=0;
virtual const std::string& getConnectedName()const=0;
virtual uint16_t getPort()const=0;

```

#### Destructor ####

```cpp
virtual ~SocketImpl()=default;
```

Closes the socket handle and the underlying streams. Releases any reasources held by this socket. 

#### Methods ####

##### connect #####

```cpp
virtual void connect(const std::string& host,uint16_t port)=0;
```

Connects the socket to the given host and port. Host describes an implementation defined Host Address. Port is the port to connect on (if the protocol uses a port numbering system).
The behavior is undefined if the host name is in an incorrect format for the protocol, or either the host or the port are not valid in the protocol.

The behavior is also undefined if the Socket has been previously connected.

###### Exceptions ######
Any unspecified subclass of SocketException may be thrown for any reason. 
In general an exception is thrown if:
<ul>
<li>The Host name is an invalid format (this is undefined behavior however)</li>
<li>The host cannot be reached,</li>
<li>Connecting to a socket acquires an operating system resource, and the OS blocks the acquisition, or such a resource is unavailable</li>
<li>The Low Level Implementation reports an error</li>
<li>The Connection is blocked by the host</li>
<li>The Connection is blocked in transit to the host</li>
<li>The Connection takes longer than the implementation permits (Timed Out)</li>
</ul>
The value of the what() string of the Exception is Unspecified, but should be meaningfully describe the error.

#### IO Interactions ####

```cpp
virtual InputStream& getInputStream()=0; //(1)
virtual OutputStream& getOutputStream()=0; //(2)
```

(1): Gets an InputStream which reads from the socket. Exceptions thrown by read methods are Unspecified but must be a subclass of SocketException. 
(2): Gets an OutputStream which writes to the Socket. Exceptions thrown by write methods are Unspecified but must be a subclass of SocketException. It is also Unspecified if the stream is buffered. The same rules of Buffered OutputStreams apply to the returned output stream if that is the case.

The streams are guaranteed to exist for the remaining lifetime of the Socket, though are not guaranteed to operate over this period (as the socket may close arbitrarily).  

The behavior is undefined if the socket has not succesfully connected, or has been closed.

###### Exceptions ######
Either method may throw any Subclass of SocketException to indicate failure, for any reason. Also the potentially throwing methods of InputStream/OutputStream may also throw any subclass of SocketException for any reason. The rules regarding exceptions in these methods are described in IOWrapper.md. The exception is that the exceptions, and conditions which these exceptions are thrown is Unspecified, and do not have to be clearly documented. 
Some causes for exceptions being thrown from these methods are as follows:
<ul>
<li>Creating the stream causes an OS resource to be acquired, and the OS prevents the acquisition of the resource, or the resource is otherwise unavailable</li>
<li>The resource acquisition permits limited access to the resource, and the access does not include the method required for the stream. (IE. getOutputStream() with an read-only underlying resource)</li>
<li>The Implementation does not support Socket IO (required for Default Conforming Implementations)</li>
</ul>

Some causes for exceptions being thrown from potentially throwing methods of the streams:
<ul>
<li>The socket is closed during the operation</li>
<li>The operation times out</li>
<li>The connection abruptly terminates.</li>
</ul>




