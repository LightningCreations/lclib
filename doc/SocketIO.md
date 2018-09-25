<h1>SocketIO Library</h1>
Included by &lt;Socket.hpp&gt;
<h2>Synopsis</h2>

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


<h2>Overview</h2>
Provides a strong, consistent Socket Based Networking Interface built off of IOWrapper and the Service Provider Interface Model. The providers for Sockets/Servers are called Handles and the default Provider can be set via setDefaultSocketHandleConstructor and setDefaultServerSocketHandleConstructors. 
An implementation specific default handle constructor is provided so that sockets may be created. 
SocketException is the base class for exceptions reported by the SocketIO Library. It is guarenteed that only Exceptions of this type are thrown. The behavior of an implementation which throws any exceptions which do not derive from SocketException is undefined. 
IO Access to sockets are provided through unspecified subclasses of InputStream and OutputStream which are obtained through the handle's getInputStream/getOutputStream (which are accessed through the Socket itself).

<h2>Classes</h2>
<h3>SocketException</h3>

