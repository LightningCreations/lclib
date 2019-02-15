# IOWrapper #

Included by `<lclib/IOWrapper.hpp>`<br/>
Part of The IO Library<br/>
## Synopsis ##

```cpp
struct append_t;
const append_t append;

struct little_endian_t;
const little_endian_t little_endian;

const size_t EOF;

class FileNotFoundException:public std::exception;
class EOFException:public std::exception;

class InputStream;
class FilterInputStream:public InputStream;
class FileInputStream:public InputStream;
class DataInputStream:public FilterInputStream;
class NullDeviceInputStream:public InputStream;
class ZeroDeviceInputStream:public InputStream;
class ByteArrayInputStream:public InputStream;

class OutputStream;
class FilterOutputStream:public OutputStream;
class FileOutputStream:public OutputStream;
class DataOutputStream:public OutputStream;
class NullDeviceOutputStream:public OutputStream;
class ByteArrayOutputStream:public OuputStream;
```

## Overview ##

The IOWrapper library provides an OO, Generic, and Extensible interface for reading and writing data.
The Interface of the IOWrapper library is designed to be consistent with java's Stream IO Library (java.io). 
Only InputStream/OutputStreams are provided, Writers/Readers, and PrintStream are not supported. 
Character IO should be preformed using C++ &lt;iostream&gt; library. 

## Classes ##
### class FileNotFoundException ###
Thrown by Constructors of FileInputStream/FileOutputStream when the file cannot be found or located. (Or when passing a null file to the FILE* constructor)

#### Base Classes ####
Extends (public) std::exception

#### Methods ####

```cpp
virtual const char* what()const noexecept(true);
```
Obtains a Implementation Defined, meaningful exception message

### class EOFException ###
Thrown by the readFully, and read<Type> methods of DataInputStream when EOF is reported by the underlying stream

#### Base Classes ####
Extends (public) std::exception

#### Methods ####

```cpp
virtual const char* what()const noexcept(true);
```
Obtains an Implementation Defined, meaningful exception message

### struct append_t ###
Disambugation Tag Type for the constructors of FileOutputStream to instruct them to append the file if it exists, rather than overwrite the contents.

#### Constructors ####

```cpp
constexpr explicit append_t()=default;
```
Constructs an instance of append_t

### struct little\_endian\_t ###
Disambugation Tag Type for the constructor of DataInputStream and DataOutputStream to read/write multibyte scalar types in Little-Endian Byte Order (default is Big-Endian)

#### Constructors ####

```cpp
constexpr explicit little_endian_t()=default;
```

### class InputStream ###
Base class of the Input portion of the library.
InputStream models a source of bytes from some source which is defined by subclasses.
InputStream has no "open/close" features.
Instead InputStreams are opened in there constructor and closed in there destructor. If the InputStream uses a resource, that resource is guarenteed to be availble throughout the lifespan of the Stream, unless explicitly specified otherwise, or the stream is moved from. If the stream indicates that it can arbitrarily close because the enclosed resource becomes unavailble, using the stream is undefined behavior if that resource is lost. Implementations which have this must provide a way to test if the stream has closed abruptly.

Due to the Resource Owning Nature of InputStreams, InputStream (and concequently its subclasses), cannot be copied (but can be moved). Using any read method on a stream that has been moved from is Undefined Behavior. It is unspecified if any other methods can be used on a moved from stream. It is always valid to assign and destroy a moved from stream.

InputStream is not inheritly thread-safe. The effects of using a single input stream across multiple threads is unspecified, and may be undefined. 

(Until 1.3): Objects of types derived from `InputStream` shall not be deleteable by a pointer-to-base. 
(As of 1.3): A program that attempts to construct an object of `InputStream` by an allocating (non-placement) new expression or delete a pointer to such an object with a delete expression is ill-formed. 

#### Constructors, Destructors, Assignment Operators ####

```cpp
public:
InputStream()=default; //(1)
InputStream(InputStream&&)=default; //(2)
InputStream(const InputStream&)=delete; //(3)
InputStream& operator=(InputStream&&)=default; //(5)
InputStream& operator=(const InputStream&)=delete; //(6)
void operator delete(void*)=delete; //(7)
void* operator new(std::size_t)=delete; //(8)
protected:
~InputStream()=default; //(4)
```

(1): Trivial Default constructor
(2): Defaulted Move Construcotr
(3): Deleted Copy Constructor
(4): Protected, explicitly defaulted Destructor.
(5): Defaulted Move Assignment
(6): Deleted Copy Assignment
(7): Deleted Deallocation Function
(8): Deleted Allocation Function

#### read methods ####

```cpp
virtual size_t read(void* ptr,size_t size)=0; //(1)
virtual int read()=0; //(2)
template<size_t N> size_t read(uint8_t(&arr)[N]); //(3)
template<size_t N> size_t read(int8_t(&arr)[N]); //(4)
template<size_t N> size_t read(std::byte(&arr)[N]); //(5)
```

(1): read(void* ptr,size_t size)

Reads size bytes from the stream and places it in the buffer pointed to by ptr, then returns the number of bytes read. 
If there exists a valid object `o` at `ptr`, the type of `o` shall satisfy *BytesReadable*. 
Additionally, if `size` is greater than the size of `o`, then there shall exist some object `k`, which the size of `k` is greater than `size`, the type of `k` satisfies *BytesReadable*, and `k` is *pointer-interconvertible* with `o`. 
Otherwise the behavior is undefined. 

If there is not a valid object at `ptr`, then `ptr` shall be the result of a call to a memory-allocation function, where the size of the allocated memory is at least `size`, or the behavior is undefined. 
If this is case, let `n` be the value that would be returned from the function. 
If `n` is greater than 0, and not `EOF` then an object of type `unsigned char[n]` is constructed at `ptr`, then the read occurs as though `ptr` pointed to that object. 

The read method reads up to `size` bytes from the stream (the number that can be successfully read is `n`), and writes them to the first `n` bytes of the object at `ptr`, or the smallest object which is at least `size` bytes large that is *pointer-interconvertible* with the object at `ptr`.

If `size` bytes cannot be read from the stream without blocking, it is unspecified if the function blocks or reads the maximum number of bytes which can be read successfully, however, in this case, at least one byte shall be read, or the end-of-stream must be reached. 
Implementations may also throw an exception if it cannot successfully complete a request read without blocking. 

If `size` is 0 it is unspecified if a read actually occurs. 
If the stream has reached the end of file, it is unspecified if 0 is returned or EOF is returned. 
Additionally, it is unspecified if any exceptions that may result from reading from the stream still occur. 


If `size` is EOF the behavior is undefined. 

(2): int read()<br/>
Reads a single byte from the stream and returns it. If EOF is reached returns EOF instead. 

This method blocks until at least one byte can be read from the stream, EOF is reached, or an exception is thrown. 
<br/><br/>

(3),(4),(5): Reads N bytes from the stream into arr. Effectively read(arr,N);

<h5>Exceptions</h5>
(1),(3),(4),(5): Any subclass of InputStream may throw any Exception from the read method. 
If a subclass can throw an exception, it must be clearly detailed which exceptions can be thrown and in what cases they are. 
If an exception is thrown by the read method, the object being read is invalidated, and using in any way (except for assignment operators/destructors) is undefined behavior.

(2): Same as (1). As no object is being read to, the point about object invalidation is not applicable. 

#### Stream Error Analysis ####

```cpp
virtual bool checkError()const noexcept(true)=0; //(1)
virtual void clearError()noexcept(true)=0; //(2)
explicit operator bool()const noexcept(true); //(3)
bool operator!()const noexcept(true); //(4)
```

(1),(4): Checks if an error is on stream. If there is an error the stream should not be used (may be closed or moved). (4) is implemented in terms of (1).<br/> 
(3): opposite of checkError(), IE. checks if the stream is OK.<br/>
(2): Clears any error which may be on stream. Has no effect if there is no error, or the error cannot be cleared.

##### Exceptions #####
All are non-throwing.


### class FilterInputStream ###

Abstract base class for InputStreams which wrap an existing stream, to add functionality.

#### Base classes ####

Extends (public) InputStream

#### Constructors and Destructors ####

```cpp
FilterInputStream(InputStream&); //(1)
protected:
~FilterInputStream(); //(2)
```

(1): Constructs a FilterInputStream binding it to the specific InputStream.
(2): Protected destructor to prevent direct construction. 

#### Read Methods ####

```cpp
size_t read(void* ptr,size_t size); //(1)
int read(); //(2)
```
(1),(2): Forwards to the equivalent method in the wrapped stream.
Any exceptions thrown by the equivalent method in the wrapped stream propagate through this call.

#### Stream Error Analysis ####

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```

(1),(2): Same as the equivalent method in the underlying stream.

### class FileInputStream ###

Represents a InputStream that reads from a raw C File Handle. The handle is owned and usually opened by the input stream. Additionally the file is closed in its destructor. 

If multiple FileInputStreams are open to the same resource, (Until 1.3): the behavior is undefined (As of 1.3): the files read from the resource independently of each other. 

If any the lifetime of any `FileInputStream` objects overlaps the lifetime of a `FileOutputStream` and both are opened to the same File resource, the behavior is undefined. 

#### Base Class</h4> ####
Extends (public) InputStream

#### Constructors, Destructors, and Assignment Operations ####

```cpp
FileInputStream(FILE* f); //(1)
FileInputStream(const char* c); //(2)
FileInputStream(const std::string& str); //(3)
FileInputStream(const std::filesystem::path& p); //(4)
FileInputStream(FileInputStream&& in); //(5)
~FileInputStream(); //(6)
FileInputStream& operator=(FileInputStream&& in); //(7)
```

(1): Constructs the FileInputStream from an already constructed FILE object.
The behavior is undefined if the file is not opened in read mode, is not a null pointer and was not the return value of fopen or (as of 1.3) some similar Unspecified utility, or was previously closed. This method is intended to be called directly with the result of fopen, and the input should not be stored. 

If the pointer passed to the FileInputStream is stored, it should not be used in any manner. In particular, passing it to the constructor of annother FileInputStream or FileOutputStream is undefined behavior. (Until 1.3): If the Pointer is stored, it may only be passed to functions that the result of `getUnderlying` may be passed to (the underlying handle remains intact). The Pointer may be used for standard C-style IO operations before it is passed to this constructor. (As of 1.3): Passing the pointer given to this constructor to any function that has preconditions on the underlying handle, or that modifies the underlying handle results in undefined behavior (The implementation of this constructor is not required to leave the underlying handle of the passed pointer intact, and may chose to close and reopen the handle.) The pointer may not have been passed to any C-style IO operations prior to passing it to this constructor.

In addition, (Until 1.3) the behavior is unspecified (and may be undefined), if the stream was not opened in binary read mode. (As of 1.3) The behavior is undefined if the handle cannot be read from. 

Exceptions: Throws FileNotFoundException if the passed file is a null pointer


(2),(3),(4): Constructs the FileInputStream from the given filename. Constructs the stream as though by `FileInputStream(fopen(c,"rb"))`, `FileInputStream(fopen(str.c_str(),"rb"))`, or `FileInputStream(fopen(p.c_str(),"rb"))` respectively.

Exceptions: Throws FileNotFoundException if given file does not exist or cannot be openned.

(5):Constructs a new FileInputStream by moving in. Control of the File object wrapped by in is moved to the new stream.

(6): Destroys this stream and closes the underlying file if it has not been moved.

(7):Moves in to this. If this has control of an underlying file has not been moved, 
 it is closed, then the control of the underlying file wrapped by in is moved to this.

#### Read Methods ####

```cpp
size_t read(void* ptr,size_t size); //(1)
int read(); //(2)
```

(1): Reads size bytes from the underlying file into the object pointed to by ptr. ptr must satisfy the same requirements as specified by InputStream::read 


(2): Reads a single byte from the underlying file and returns it or EOF if the file has reached the end of file.

##### Exception Guarantee #####

It is unspecified if and when exceptions are thrown. However, if an exception is thrown, the stream shall be left in a valid (but unspecified) state where checkError() returns true. It is unspecified if this error is supressible. 

If an exception occurs, the objects at ptr are left in an invalid state, as defined by InputStream::read.  

#### Raw Stream Access ####

```cpp
FILE* getUnderlying()const noexcept(true);
```
Gets the underlying file or a null pointer if control of the underlying file has been moved. 

This pointer can only be passed to observer methods and (until 1.3) `clearerror`.

(Until 1.3): If this FileInputStream was constructed with the FILE* constructor, then calls to getUnderlying() return the same Pointer as was passed. 

##### Deprecated #####
The method has no usage out side of testing if the file has been moved. Calling `ferror`/`feof` on the handle is unnecessary (as InputStream's have there own method of checking errors). Prior to Version 1.3, the only operations that could be performed on returned handles were observer methods, or `clearerror`. As of Version 1.3, clearing the error through this handle is undefined behavior. All observers may be called on this method however. 

This method will be removed in a future version of LCLib-C++. 

##### Exception Guarantee #####
noexcept(true)

#### Stream Error Analysis ####

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1): Checks if an error is on stream. The result is the same result obtained by calling `feof(getUnderlying())||ferror(getUnderlying())`. 
(2): Clears any error on stream. After this call, `checkError` will return false, and subsequent calls to `feof(getUnderlying())` and to `ferror(getUnderlying())` return false until a read operation is performed.   

### DataInputStream ###
A wrapper on basic InputStreams, which provides Byte-Order sensitive read operations for scalar and other types. 
By Default DataInputStream reads multibyte scalar types in Big Endian (network) Byte Order. If Reading in Little Endian (native) Byte Order is required, the disambugation tag little_endian may be passed to the constructor. If the raw read or readFully method is used to read objects of a multibyte scalar type, the behavior is undefined. 
When in Big-Endian mode, scalar types (except Enum types) are read in a manor functionally equivalent to the equivalent read from java.io.DataInputStream, and as such, values of the same type written by a java.io.DataOutputStream are suitable to be read by this class. Strings read by readString are treated as if they are in UTF-8 or Ascii Encoding and are read simiarly to java.io.DataInputStream.readUTF, and therefore strings written by java.io.DataOutputStream's writeUTF method are suitable for being read by this class.

`DataInputStream` satisfies the requirements for LCLib BinaryDataIO and qualifies as a BinaryDataInput Utility. The Byte-Order mode is the one specified at the construction of the Stream.  


#### Base classes ####
Extends (public) FilterInputStream

#### Constructors ####

```cpp
DataInputStream(InputStream&); //(1)
DataInputStream(InputStream&,little_endian_t); //(1)
```

(1): Constructs a DataInputStream wrapping the given stream in BigEndian mode. The raw read methods of this DataInputStream forward to this stream.
(2): Same as (1), except the stream is constructed in LittleEndian mode.

#### Methods ####
##### readFully #####

```cpp
void readFully(void* ptr,size_t size);
```
Reads exactly size bytes from the underlying stream into the object given by ptr. The object pointed to by ptr must satisfy the requirements of InputStream::read(void*, size_t). 
If size bytes cannot be read, an EOFException is thrown. Otherwise, equivalent to `read(ptr,size)`

(Until 1.3): If `size` is 0, it is unspecified by any call to read takes place. 
(As of 1.3): Unlike InputStream::read, if `size` is 0, then no read takes place (the method returns immediately). Specifically, the read method of the underlying stream is not called. 

###### Exceptions ######

If `size` is greater than 0 then throws an EOFException if the underlying read method call reports an EOF prior to reading `size` bytes. 

If `size` is greater than 0 then (As of 1.3) propagates any exceptions thrown by the underlying stream's read method. 

(As of 1.3): If `size` is 0, then the method will not throw any exceptions. 


##### Special read methods #####

```cpp
uint8_t readUnsignedByte(); //(1)
int8_t readSignedByte(); //(2)
uint16_t readUnsignedShort(); //(3)
int16_t readSignedShort(); //(4)
int32_t readInt(); //(5)
int64_t readLong(); //(6)
float readFloat(); //(7)
double readDouble(); //(8)
std::string readString();//(9)
template<typename E> E readEnum();//(10)
```
(1),(2): reads a single byte from the stream and returns it as an unsigned or signed byte respectively. The behavior is undefined unless the byte on stream was written by a BinaryDataOutput Utility as a Signed or Unsigned Byte value. 

(3),(4): reads 2 bytes from the stream and returns them as an unsigned or signed short, respectively. The behavior is undefined unless the bytes on stream make up a Signed or Unsigned Short value written by a BinaryDataOutput Utility operating in the same Byte-Order mode. 

(5): reads 4 bytes from the stream and returns them as an int. The behavior is undefined unless the bytes on stream make up a Signed or Unsigned Int value written by a BinaryDataOutput Utility operating in the same Byte-Order mode. 

(6): reads 8 bytes from the stream and returns them as an int64_t. The behavior is undefined unless the bytes on stream make up a Signed or Unsigned Long value written by a BinaryDataOutput Utility operating in the same Byte-Order mode. 

(7): reads 4 bytes from the stream and returns them as a 32-bit single-precision float. The behavior is undefined unless the bytes on stream make up a Float value written by a BinaryDataOutput Utility operating in the same Byte-Order mode. In addition, the behavior is undefined if the default float on the system is not an IEEE754 single-precision binary32 floating-point number. 

(8): reads 8 bytes from the stream and returns them as a 64-bit double. The behavior is undefined unless the bytes on stream make up a Double value written by a BinaryDataOutput Utility operating in the same Byte-Order mode. In addition, the behavior is undefined if the default double on the system is not an IEEE754 double-precision binary64 floating point number.

(9): reads a 2-byte length, as though by readUnsignedShort from the stream, and then reads that many bytes, treating them as a utf-8 encoded string (non-nul terminated), and returns the resultant std::string. The behavior is undefined unless the bytes on stream make up a String value written by a BinaryDataOutput Utility operating in the same Byte-Order mode. 

(10): reads a number of bytes based on the underlying type of E as if by the best matching read method for that type, and returns to bytes treated as a value of type E. The behavior is undefined unless all bytes read were written by an equivalent call to writeEnum of DataOutputStream, and if the enum type is not 1 byte long, in the same byte order mode. If E is std::byte it may additionally be used to read byte sequences suitable for the readSignedByte() and readUnsignedByte()  methods. This method only participates in overload resolution if `std::is_enum_v<E>` is true. This method is not specified by LCLib, and values of this type are not compatible with values of the underlying type. However the byte-order mode is respected. 

(Until 1.3): It is unspecified if the bytes are read by a single bulk operation or individual calls to the underlying stream's `read()` method. The bytes may also be read by different methods and this is not required to be consistent

(As of 1.3): A method for exposition only, called `readSingle()` which returns `std::uint8_t`, is defined. The method returns the result of calling `read()` on the underlying stream, and throws an EOFException if the result is EOF (and propagates any other exceptions thrown by the call to `read()`). `readByte()`/`readUnsignedByte()` and any call to `readEnum` for `E`, such that `sizeof(E)` is 1, shall read its bytes using this method. It is unspecified if the bytes for any other methods are read using `readSingle()` or `readFully()` but one of these methods must be called. If `readFully()` is used, then no bytes are read by `readSingle()` and vice-versa and only a single call is made. The method used to read the bytes must be consistent for a given operation in a given Byte-Order mode during any execution of the program. 

###### Exceptions ######
If any read operation results in EOF then an EOFException is thrown. 

(Until 1.3): If any invocation of the method called to read bytes from the stream throws an exception, the exception is propagated

(As of 1.3): If the call to `readFully()` (if any) throws any exception, that exception is propagated. If bytes are read using `readSingle()`, and any individual call to `readSingle()` throws an exception, that exception is propagated and no more calls to `readSingle()` are made. 

##### Stream Operator Overloads ####
Reads a given value from the stream based on the type of the argument, and assigns the target reference to that value

```cpp
DataInputStream& operator>>(uint8_t& b); //(1)
DataInputStream& operator>>(int8_t& b); //(2)
DataInputStream& operator>>(uint16_t& s); //(3)
DataInputStream& operator>>(int16_t& s); //(4)
DataInputStream& operator>>(int32_t& i); //(5)
DataInputStream& operator>>(uint32_t& i); //(6)
DataInputStream& operator>>(int64_t& l); //(7)
DataInputStream& operator>>(uint64_t& l); //(8)
DataInputStream& operator>>(std::string& str); //(9)
DataInputStream& operator>>(Version& v); //(10)
DataInputStream& operator>>(UUID& u); //(11)
DataInputStream& operator>>(float& f); //(12)
DataInputStream& operator>>(double& d); //(13)
template<typename E> DataInputStream& operator>>(E& e); //(14)
```
(1),(2),(3),(4),(5),(7),(9),(12),(13): assigns the argument to the value returned by the equivalent Specialized read method
(6),(8): assigns the argument to the value returned by readInt, or readLong respectively.
(10): Reads a Version from the stream, and assigns it to v. Effectively v= Version{int(readUnsignedByte())+1,readUnsignedByte()}; The behavior is undefined unless the bytes read were written as such by DataOutputStream or a similar utility, or were written by the writeShort or writeUnsignedShort method of a DataOutputStream in Big-Endian byte order mode, or a similar utility.
(11): Reads a UUID from the stream, and assigns it to u. Effectively u=UUID{readLong(),readLong()};
(14): Reads an enum value from the stream as if by `e = readEnum<E>();` This operator does not participate in overload resolution unless `std::is_enum_v<E>` is true.


### class NullDeviceInputStream ###
NullDeviceInputStream is a Special InputStream which reads from the Null Symbolic Device (on Linix/Unix, this is at /dev/null). 
Read Methods will unconditionally yield EOF, and no bytes are read. 
Note that Despite this referencing a symbolic device, no actual read is made, the methods simply do nothing. 


#### Read Methods ####

```cpp
std::size_t read(void* ptr,std::size_t sz); //(1)
int read(); //(2)
```
(1), (2): Reads from the stream. The NullDevice is allways empty, and these methods unconditionally return EOF. Unlike InputStream::read(void*,std::size_t), this method will NOT invalidate any objects at ptr. 
Despite this, the objects at ptr must still satisfy BytesReadable. 

##### Exceptions #####
Neither method will throw any exceptions.

#### Stream Error Analysis ####

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1):Checks if an error is on stream. As No reads are made this method unconditionally returns false.
(2): Clears the error on stream. Due to no errors being reported, this method has no effect.

### class ZeroDeviceInputStream ###
ZeroDeviceInputStream is a InputStream that reads from the Symbol Zero Device (On Linux/Unix, this is at /dev/zeros). 
Read methods will result in a stream of Zeros, that is, will fill the buffer provided with 0s and return zero from read(). 
Despite referencing /dev/zeros, no actual device read is made, the implementation is in terms of memset. This may be a more secure method of destroying objects, due to the optimizations of memset implementations are allowed to perform. Note that this only applies if the object being destroyed satisfies BytesReadable, as the behavior is still undefined for objects that do not (despite the guaranteed result of the operation). 

#### Read Methods ####

```cpp
std::size_t read(void* ptr,std::size_t sz); //(1)
int read(); //(2)
```
(1): Reads sz bytes from the stream into ptr. This sets all bytes in the object indicated by ptr to 0, then returns sz. (As of 1.3) This method will not invalidate any objects at ptr. However all objects at ptr must still satisfy BytesReadable. 
(2): Reads a byte from the stream and returns it. Always returns 0;



##### Exceptions #####
Neither method will throw any exceptions.


#### Stream Error Analysis ####

```cpp
bool checkError()const noexcept(true);//(1)
void clearError()noexcept(true);//(2)
```
(1): Checks if an error is on stream. As no read is actually preformed, this always returns false
(2): Clears the error on stream. As no errors are reported, this method has no effect

### ByteArrayInputStream ###
Reads bytes from a internal buffer. An unsupressable error condition is reported if the end of the buffer has been reached.

#### Constructors ####

```cpp
template<typename Byte> ByteArrayInputStream(const Byte* buff,std::size_t blen); //(1)
template<typename Byte,std::size_t N> ByteArrayInputStream(const Byte(&arr)[N]); //(2)
template<typename Byte,std::ptrdiff_t extent> ByteArrayInputStream(const span<Byte,extent>& s); //(3)
template<typename Byte> ByteArrayInputStream(const span<Byte,dynamic_extent>& s); //(4)
```
(1): Constructs a new ByteArrayInputStream over the given buffer. The behavior is undefined if the combiniation of `buff` and `blen` cannot be validly passed to the `write(const void*,std::size_t)` method of OutputStream. 
 

This method does not participate in overload resolution unless `is_byte_v<Byte>` is true. 
(2): Constructs a new ByteArrayInputStream over the given array, with total size N. This method does not participate in Overload resolution unless `is_byte_v<Byte>` is true. This method is identical to (1), called with &arr[0] as `buff` and N as `blen`. 
(3),(4): Constructs a new ByteArrayInputStream over s.data(), with total size s.size(). This method does not participate in Overload resolution unless `is_byte_v<Byte>` is true. (3) does not participate in Overload resolution if extent&lt;0. Byte may be a const qualified byte type. This method is identical to (1), called with s.data() and s.size(). 

#### Read Methods ####

```cpp
std::size_t read(void* ptr,std::size_t sz); //(1)
int read(); //(2)
```
(1): reads sz bytes from the buffer to ptr. If there are no bytes remaining in the buffer, returns EOF. 
Otherwise, if there is n bytes remaining in buffer, where n is less than sz, then n bytes are copied from the buffer to ptr, the buffer is advanced to the end, and n is returned. This may invalidate objects at ptr as specified in InputStream.read(void*,std::size_t). Otherwise, sz bytes are copied to ptr, the buffer is advanced by sz bytes, and sz is returned

The ranges of `[buff,buff+blen)` and `[ptr,ptr+sz)` must not overlap, or the behavior is undefined. 

(2): reads a single byte from the stream. If there are no bytes remaining in the buffer, returns EOF. 
Otherwise, returns the byte at the current position of the buffer, and advances the buffer by 1.

##### Exceptions #####
Neither method will throw an exception

#### Stream Error Analysis ####

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1): Checks if there is an error on stream. Returns true if the buffer is at the end of the stream, false otherwise.
(2): Clears any error on the stream. This method has no effect as the only error is unsuppressable (end of stream).

### class OutputStream ###
OutputStream is the abstract base class of the output portion of the IOWrapper Library, mirroring InputStream. 
Like InputStreams, OutputStreams have ownership of any resources they use, and aquires and releases those resources during construction and destruction respectively. 
Subclasses of OutputStream may buffer output before commiting writes in the stream. 
If a subclass does, it must override the flush method to flush the internal buffer and commiting all previous writes to the resource if they have not already been written. 
In addition the buffer must be flushed when the stream is destroyed or reassigned.  
If flush is not called it is unspecified when writes are reflected in the underlying resource. 
If the OutputStream does not buffer input then writes are immediately reflected in the underlying resource, and flushing the stream has no effect. 
OutputStreams are not inheritly thread safe. The effects of using a single OutputStream across multiple threads is unspecified, and may be undefined. 

(Until 1.3): Objects of types that derive from `OutputStream` shall not be deleteable through a pointer to base. 
(As of 1.3): A program which attempts to construct an object of a type that derives from `OutputStream` via an allocating (non-placement) new expression is ill-formed. 

#### Constructors, Destructors, and Assignment Operator ####

```cpp

public:
OutputStream()=default; //(1)
OutputStream(const OutputStream&)=delete; //(2)
OutputStream(OutputStream&&)=default; //(3)
OutputStream& operator=(const OutputStream&)=delete; //(5)
OutputStream& operator=(OutputStream&&)=default; //(6)
void operator delete(void*)=delete; //(7)
void* operator new(std::size_t)=delete; //(8)
protected:
~OutputStream(); //(4)
```
(1): Default Constructor is defaulted. 
(2): Deleted Copy Constructor
(3): Defaulted Move Constructor
(4): Protected, defaulted destructor. 
(5): Deleted Copy Assignment Operator
(6): Defaulted Move Assignment Operator
(7): Deleted deallocation function
(8): Deleted allocation function

#### Write Methods ####

```cpp
virtual size_t write(const void* ptr,size_t size)=0; //(1)
virtual void write(uint8_t b)=0; //(2)
template<size_t N> size_t write(uint8_t(&arr)[N]); //(3)
template<size_t N> size_t write(int8_t(&arr)[N]); //(4)
template<size_t N> size_t write(std::byte(&arr)[N]); //(5)
```
(1): writes size bytes from the object pointed to by ptr to the stream, and returns the total number of bytes written. 
`ptr` shall be a pointer to a complete object which is of a type which satsifies *BytesWriteable*. 
Additionally, the object at `ptr` must either be at least `size` bytes or there shall exist an object `o` which is *pointer-interconvertible* with the object at `ptr` which is at least `size` bytes and the type of `o` satisfies *BytesWriteable* otherwise the behavior is undefined. 

Like InputStream::read, subclasses may impose other restrictions on what objects may be passed to the write method. Those subclasses must detail these additional restrictions and the effects of violating them if they exist.

If the subclass buffers writes it is unspecified if the previous buffer, if any, is flushed partially or completely, or if any part of the new buffer is flushed. 

(2): writes a single byte to the stream. 

(3),(4),(5): writes an array of `uint8_t`, `int8_t`, or `std::byte` to the stream. Effectively read(arr,N);

##### Exceptions #####
(1),(2): Subclasses may throw any exception from the write method. If they do, it must be detailed which exceptions can be thrown, and the conditions under which they are thrown. If an exception is thrown from a stream that buffers writes, the stream is rolled-back to state it was in prior to the write call, and the internal buffer is left in an unspecified state. If an exception is thrown from a stream that does not buffer writes, the stream is left in an unspecified state.
(3),(4),(5): Forwards any exceptions thrown by write.

#### flush ####

```cpp
virtual void flush();
```
Flushes the stream. Has no effect if the OutputStream does not buffer input. Otherwise the internal buffer must be written to the resource.

##### Exceptions #####
Subclasses may throw any exception from flush. If they do, it must be detailed which exceptions can be thrown, and the conditions under which they are thrown. If an exception is thrown, both the stream and the internal buffer are left in an unspecified state.

#### Stream Error Analysis ####

```cpp
virtual bool checkError()const noexcept(true)=0; //(1)
virtual void clearError()noexcept(true)=0;//(2)
explicit operator bool()const noexcept(true);//(3)
bool operator!()const noexcept(true); //(4)
```

(1),(4): Checks if an error is on stream. The stream should not be used if an error is on the stream until it is cleared. (4) is implemented in terms of (1).
(3): Checks if there is no error on stream. Effectively the Inverse of checkError()
(2): Clears any error on stream. Has No effect if there is no error, or the error cannot be cleared.

##### Exceptions #####
All methods are non-throwing

#### class FilterOutputStream ###
FilterOutputStream wraps an existing OutputStream to be extended to provide extra functionality. The Stream is Not explicitly managed by FilterOutputStream or any subclasses. Destroying the FilterOutputStream will neither close nor flush the underlying stream object.

#### Base Classes ####
Extends (public) OutputStream

#### Constructors ####

```cpp
public:
FilterOutputStream(OutputStream& out); //(1)
protected:
~FilterOutputStream()=0; //(2)
```
(1): Constructs a FilterOutputStream that wraps (but does not own) out.
The behavior is undefined if out is destroyed before the current object.
(2): Destructor defined as pure to prevent direct construction of FilterOutputStream

#### Methods ####

```cpp
size_t write(const void* ptr,size_t size); //(1)
void write(uint8_t b); //(2)
void flush(); //(3)
bool checkError()const noexcept(true);//(4)
void clearError()noexcept(true);//(5)
```
(1),(2): calls the corresponding write method in the underlying stream
(3): Calls the flush method of the underlying stream
(4): Calls the checkError method of the underlying stream
(5): Calls the clearError method of the underlying stream.

#### Exceptions ####
(1),(2),(3) Any exception thrown by the respective method in the underlying stream propagates through all methods
(4),(5): Non-throwing

### FileOutputStream ###
FileOutputStream is an OutputStream which wraps a c FILE*. The file is managed by the stream and is closed when it is destroyed. FileOutputStream will buffer writes if the underlying c IO implementation buffers writes.

#### Constructors, Destructors, and Assignment Operators ####

```cpp
FileOutputStream(FILE* f);//(1)
FileOutputStream(const char* c);//(2)
FileOutputStream(const std::string& str);//(3)
FileOutputStream(const std::filesystem::path& p);//(4)
FileOutputStream(const char* c,append_t);//(5)
FileOutputStream(const std::string& str,append_t);//(6)
FileOutputStream(const std::filesystem::path& p,append_t);//(7)
FileOutputStream(FileOutputStream&& out);//(8)
~FileOutputStream();//(9)
FileOutputStream& operator=(FileOutputStream&& out);//(10)
```

(1):  Constructs the FileInputStream from an already constructed FILE object.
The behavior is undefined if the file is not opened in read mode, is not a null pointer and was not the return value of fopen or (as of 1.3) some similar Unspecified utility, or was previously closed. This method is intended to be called directly with the result of fopen, and the input should not be stored. 

If the pointer passed to the FileInputStream is stored, it should not be used in any manner. In particular, passing it to the constructor of annother FileInputStream or FileOutputStream is undefined behavior. (Until 1.3): If the Pointer is stored, it may only be passed to functions that the result of `getUnderlying` may be passed to (the underlying handle remains intact). The Pointer may be used for standard C-style IO operations before it is passed to this constructor. (As of 1.3): Passing the pointer given to this constructor to any function that has preconditions on the underlying handle, or that modifies the underlying handle results in undefined behavior (The implementation of this constructor is not required to leave the underlying handle of the passed pointer intact, and may chose to close and reopen the handle.) The pointer may not have been passed to any C-style IO operations prior to passing it to this constructor. 

(2),(3),(4): Constructs a new FileOutputStream from the given path name. 
This constructor acts as if by `FileOutputStream(fopen(c,"wb"))`, `FileOutputStream(fopen(str.c_str(),"wb"))`, or `FileOutputStream(fopen(p.c_str(),"wb"))`. The behavior is undefined if the argument to (2) is a null pointer.
(5),(6),(7): Disambugation Constructors for opening in append mode. Same as (2),(3),(4) respectively, except that the stream is opened in "ab" mode instead of "wb" mode. 
If the file does not exist, then the effect is the same as (2),(3), or (4) respectively. 
(8): Move Constructor. Control of the underlying file object of out is moved to the new object
(9): Destructor. If control of the underlying file has not been moved from this object, the underlying file is closed. If the underlying file buffers writes, it is also flushed.
(10): Move Assignment Operator. If control of the underlying file has not been moved from this object it is closed and potentially flushed. Then control of the underlying file of out is moved to this object.

##### Exceptions #####
(1): If the passed file is a null pointer, the constructor throws a FileNotFoundException
(2),(3),(4),(5),(6),(7): If the named file does not exist or cannot be created, or the named file cannot be opened in write mode, the constructor throws a FileNotFoundException

#### Write and Flush Methods ####

```cpp
size_t write(const void* ptr,size_t size); //(1)
void write(uint8_t); //(2)
void flush(); //(3)
```
(1): writes size bytes from the object pointed to by ptr to the underlying file. The requirements for the object written is detailed in OutputStream::write.
(2): writes a single byte to the underlying file
(3): If the underlying file buffers writes then flushes that file. Otherwise does nothing. The implementation may not buffer the writes directly. 

##### Exceptions #####

It is unspecified the conditions under which the implementation throws an exception, or even if an exception is thrown. If an exception is thrown, the stream is left in a valid, but unspecified state. The internal handle is additionally left in an unspecified state. Subsequent calls to `checkError()` will return true until cleared. It is unspecified if the error condition is unsupressible.  

#### Raw File Access and Stream Error Analysis ####

```cpp
FILE* getUnderlying()const noexcept(true); //(1)
bool checkError()const noexcept(true);//(2)
void clearError()noexcept(true); //(3)
```
(1): Similar to getUnderlying() called on an instance of FileInputStream. All rules and restrictions as above apply to this function. This function is deprecated as of LCLib-C++ 1.3 and will be removed in future versions. 
(2): Checks if there is an error on the stream. Effectively ferror(getUnderlying())||feof(getUnderlying()) 
(3): Clears any error on the stream. Effectively clearerror(getUnderlying())


##### Exceptions #####
noexcept(true)

### class DataOutputStream ###
DataOutputStream wraps an OutputStream to add consistent write methods for scalar data-types and some other types. By Default DataOutputStream reads multibyte scalar types in Big-Endian (network) Byte Order. If Little-Endian (native) Byte Order is desired, the `little_endian_t` disambugation constructors may be used.

`DataOutputStream` satisfies the requirements of a BinaryDataOutput utility, as specified in LCLib BinaryDataIO. The Byte-Order Mode is given by the constructor used to open the stream. 

#### Base Classes ####
Extends (public) FilterOutputStream

#### Constructors ####

```cpp
DataOutputStream(OutputStream& out); //(1)
DataOutputStream(OutputStream& out,little_endian_t); //(2)
```

(1): Constructs a DataOutputStream that wraps (but does not own) out, and is open in Big-Endian Byte Order Mode. 

(2): Constructs a DataOutputStream that wraps (but does not own) out, and is open in Little-Endian Byte Order Mode.

#### Methods ####
##### Specialized write Methods #####

```cpp
void writeByte(int8_t b); //(1)
void writeShort(int16_t s); //(2)
void writeInt(int32_t i); //(3)
void writeLong(int64_t l); //(4)
void writeString(const std::string& str); //(5)
void writeFloat(float f); //(6)
void writeDouble(double d); //(7)
template<typename E> void writeEnum(E e); //(8)
```
(1): writes b to the underlying stream. The value is written as a Byte value under LCLib BinaryDataIO. 

(2): writes s to the underlying stream. The value is written as a Short value under LCLib BinaryDataIO. 

(3): writes i to the underlying stream. The value is written as an Int value under LCLib BinaryDataIO. 

(4): writes l to the underlying stream.  The value is written as a Long value under LCLib BinaryDataIO. 

(5): writes the length of str to the stream as if by writeShort, then writes that many bytes to the stream from str.data(). If the length of str is greater then 65536, only the first 65536 bytes of str are written.  The value is written as a String value under LCLib BinaryDataIO. 

(6): writes f to the underlying stream.  The value is written as a Float value under LCLib BinaryDataIO.  The behavior is undefined if the default float on the system is not an IEEE754 single-precision binary32 floating-point number. If the passed float is NaN, the behavior is undefined 

(7): writes d to the underlying stream. The value is written as a Byte value under LCLib BinaryDataIO. The behavior is undefined if the default double on the system is not an IEEE754 double-precision binary64 floating-point number. If the passed double is NaN, the behavior is undefined. 

(8): writes e to the underlying stream. The bytes written are suitable for being read by the equivalent readEnum method of a DataInputStream open in the same byte order mode. Unlike other methods writeEnum is not compatible with any similar interfaces. This method does not participate in overload resolution unless `std::is_enum_v<E>` is true. This method is not specified by LCLib, but will still write the value in the correct Byte-Order Mode. 

(Until 1.3): The bytes are written using any combination of the various write calls. 

(As of 1.3): For `writeByte` or `writeEnum` for an `E` such that `sizeof(E)` is 1, the byte is written with a single call to `write(std::uint8_t)`. For all other methods, it is unspecified whether the bytes are written using a single call to `write(const void*,std::size_t)` or individual calls to `write(std::uint8_t)`. 


##### Exceptions #####

Any exceptions thrown by any invocation of the write methods of the underlying stream propagates through these methods.

#### Stream Write Operator Overloads ####

```cpp
DataOutputStream& operator<<(uint8_t b); //(1)
DataOutputStream& operator<<(int8_t b); //(2)
DataOutputStream& operator<<(uint16_t s); //(3)
DataOutputStream& operator<<(int16_t s); //(4)
DataOutputStream& operator<<(uint32_t i); //(5)
DataOutputStream& operator<<(int32_t i); //(6)
DataOutputStream& operator<<(uint64_t l); //(7)
DataOutputStream& operator<<(int64_t l); //(8)
DataOutputStream& operator<<(const std::string& str); //(9)
DataOutputStream& operator<<(const UUID& u); //(10)
DataOutputStream& operator<<(Version v); //(11)
DataOutputStream& operator<<(float f); //(12)
DataOutputStream& operator<<(double d); //(13)
template<typename E> DataOutputStream& operator<<(E e); //(14)
```

(2),(4),(6),(8),(9),(12),(13): Effectively the same as the equivalent write function

(1),(3),(5),(7): Calls the write function with the arguments converted to there signed equivalents.

(10): Writes the UUID as though by writeLong(u.getHigh()) and writeLong(u.getLow()).

(11): Writes the Version as though by writeByte(v.getMajor()-1) and writeByte(v.getMinor())

(14): Writes the enum value as though by the writeEnum method. This operator only participates in overload resolution if `std::is_enum_v<E>` is true.

### class NullDeviceOutputStream ###
NullDeviceOutputStream is a stream that writes to the Null Symbolic Device (/dev/null on Linux/Unix). Write methods will have no effect, no writes actually take place. 
Despite referencing the Null Device, no handle to that device is created. 
NullDeviceOutputStream is unbuffered.

#### Write Methods ####

```cpp
std::size_t write(const void* ptr,std::size_t sz); //(1)
void write(uint8_t b); //(2)
```
(1): Writes sz bytes from ptr to the stream. This method will have no effect and return sz. Note that this is because the Null Device traditionally acts as a Byte Sink, that is, all writes are accepted by it and discarded. This is in contrast to reading from the Null Device which always yields EOF. 
(2): Writes a single byte to the stream. This method has no effect

##### Exceptions #####
Neither method will throw any exceptions.

#### Stream Error Analysis ####

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1): Checks if there is an error on stream. As no writes actually take place, this method unconditionally returns false. 
(2): Clears any error on the stream. As no errors are reported, this method has no effect.



### class ByteArrayOutputStream ###
Inverse of ByteArrayInputStream. ByteArrayOutputStream writes bytes to an internal resizable buffer, which can be accessed by the program. ByteArrayOutputStream will not buffer writes.

<h4>Write Methods</h4>

```cpp
std::size_t write(const void* ptr,std::size_t sz); //(1)
void write(uint8_t b); //(2)
```
(1): writes sz bytes from ptr to the underlying buffer. The appropriate number of bytes are reserved in the underlying buffer, then the bytes are copied from ptr to the buffer. Returns sz. 
(2): writes a single byte to the underlying buffer. 

<h5>Exceptions</h5>
Neither method will throw any exceptions.

<h4>Buffer access and Stream Error Reporting</h4>

```cpp
span<const std::byte> getBuffer()const noexcept(true); //(1)
std::size_t getSize()const noexcept(true); //(2)
bool checkError()const noexcept(true); //(3)
void clearError()noexcept(true); //(4)
```
(1): Gets the underlying buffer. This provides a read access, dynamic_extent span which views the memory stored in the internal buffer. This span is invalidated if a subsequent call to either write method is made. 

(2): Gets the size of the underlying buffer. 

(3): Checks if there is an error on stream. No errors are reported, so this method unconditionally returns false.

(4): Clears any error on stream. As no errors are reported, this method has no effect.


## Fields ##

```cpp
const append_t append{}; //(1)
const little_endian_t little_endian{}; //(2)
const std::size_t EOF{-1}; //(3)
```

(1): Tag for FileOutputStream constructors to select the overloads that open the file in append mode instead of write mode. 

(2): Tag for DataOutputStream/DataInputStream constructors to open the stream in little-endian byte order mode instead of Big-Endian byte order mode 

(3): Value returned from `read()` calls to indicate that the end-of-file was reached. 
