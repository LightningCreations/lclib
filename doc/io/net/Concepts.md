# Networking Library Concepts #

Included by `<lclib/Concepts.hpp>`<br/>
In `namespace net`<br/>

Part of the IO Library and the Networking Library.<br/>

## Overview ##
This library provides concepts (if supported), named requirements, and type traits for various networking concepts, used by the Networking Library.  

### SocketAddress ###

Represents a socket address, that can be connected to. 

#### Concept Definition ####

```cpp
struct SocketAddress{
	using native_type = /*unspecified*/; //(1)
	using char_type = CharType; //(2)
	SocketAddress(/*unspecified argument list*/); //(3)
	SocketAddress(); //(4)
	SocketAddress(const SocketAddress&); //(5)
	SocketAddress(SocketAddress&&); //(6)
	~SocketAddress(); //(7)
	SocketAddress& operator=(const SocketAddress&); //(8)
	SocketAddress& operator=(SocketAddress&&); //(9)
	native_type or const native_type& getNative()const; //(10)
	template<typename Allocator> operator std::basic_string<char_type,CharTraits,Allocator>()const; //(11)
};
```

(1): The result of getNative()<br/>
(2): The Character type of the Strings that the SocketAddress is convertable to<br/>
(3): Constructs a socket address from an implementation defined, non-empty argument list. The resultant address is implementation defined.<br/>
(4): Default-constructs a socket address. The resultant socket address is an implementation defined address which represents the current machine<br/>
(5),(6),(8),(9): Copy/Move Constructs/Assigns a socket address. The resultant address is the same as the input address before the construction/assignment. 
If resources are used by the address type, they are cleaned up in the assigned address (if applicable), then are copied/moved from the source. Otherwise these should all be trivial. If resources are used then move construction/assignment leaves the moved-from-object in a valid but unspecified state. <br/>
(7): If an resources are used by the address type, those resources are cleaned up. Otherwise this should be trivial. <br/>
(10): Returns an unspecified native address type. <br/>
(11): Converts the socket address to an unspecified string representation. CharTraits is implementation defined (though may be a template). Other string conversions are unspecified. <br/>






### SocketHandle ###

#### Concept Definition ####

```cpp
struct SocketHandle{
public:
using native_type = /*unspecified*/;
using address_type = SocketAddress;
SocketHandle(const SocketHandle&)=delete;
SocketHandle(SocketHandle&&);
SocketHandle& operator=(const SocketHandle&)=delete;
SocketHandle& operator=(SocketHandle&&);
~SocketHandle();
SocketHandle(address_type&&);
void close();
bool isOpen();
const address_type& getConnectedAddress()const [[expects: isOpen()]];
native_type or cv native_type& getNative() [[expects: isOpen()]];
};
```

