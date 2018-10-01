<h1>IOWrapper</h1>
<b>Included by &lt;IOWrapper.hpp&gt;</b><br/>
<h2>Synopsis</h2>

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

<h2>Overview</h2>

The IOWrapper library provides an OO, Generic, and Extensible interface for reading and writing data.
The Interface of the IOWrapper library is designed to be consistent with java's Stream IO Library (java.io). Only InputStream/OutputStreams are provided, Writers/Readers, and PrintStream are not supported. Character IO should be preformed using C++ &lt;iostream&gt; library. 

<h2>Classes</h2>
<h3>class FileNotFoundException</h3>
Thrown by Constructors of FileInputStream/FileOutputStream when the file cannot be found or located. (Or when passing a null file to the FILE* constructor)

<h4>Base Classes</h4>
Extends (public) std::exception

<h4>Methods</h4>

```cpp
virtual const char* what()const noexecept(true);
```
Obtains a Implementation Defined, meaningful exception message

<h3>class EOFException</h3>
Thrown by the readFully, and read<Type> methods of DataInputStream when EOF is reported by the underlying stream

<h4>Base Classes</h4>
Extends (public) std::exception

<h4>Methods</h4>

```cpp
virtual const char* what()const noexcept(true);
```
Obtains an Implementation Defined, meaningful exception message

<h3>struct append_t</h3>
Disambugation Tag Type for the constructors of FileOutputStream to instruct them to append the file if it exists, rather than overwrite the contents.

<h4>Constructors</h4>

```cpp
constexpr explicit append_t()=default;
```
Constructs an instance of append_t

<h3>struct little_endian_t</h3>
Disambugation Tag Type for the constructor of DataInputStream and DataOutputStream to read/write multibyte scalar types in Little-Endian Byte Order (default is Big-Endian)

<h4>Constructors</h4>

```cpp
constexpr explicit little_endian_t()=default;
```

<h3>class InputStream</h3>
Base class of the Input portion of the library.
InputStream models a source of bytes from some source which is defined by subclasses.
InputStream has no "open/close" features.
Instead InputStreams are opened in there constructor and closed in there destructor. If the InputStream uses a resource, that resource is guarenteed to be availble throughout the lifespan of the Stream, unless explicitly specified otherwise, or the stream is moved from. If the stream indicates that it can arbitrarily close because the enclosed resource becomes unavailble, using the stream is undefined behavior if that resource is lost. Implementations which have this must provide a way to test if the stream has closed abruptly.

Due to the Resource Owning Nature of InputStreams, InputStream (and concequently its subclasses), cannot be copied (but can be moved). Using any read method on a stream that has been moved from is Undefined Behavior. It is unspecified if any other methods can be used on a moved from stream. It is always valid to assign and destroy a moved from stream.

InputStream is not inheritly thread-safe. The effects of using a single input stream across multiple threads is unspecified, and may be undefined.

<h4>Constructors, Destructors, Assignment Operators</h4>

```cpp
InputStream()=default; //(1)
InputStream(InputStream&&)=default; //(2)
InputStream(const InputStream&)=delete; //(3)
virtual ~InputStream()=default; //(4)
InputStream& operator=(InputStream&&)=default; //(5)
InputStream& operator=(const InputStream&)=delete; //(6)
```

(1): Defaulted Nullary constructor
(2): Defaulted Move Construcotr
(3): Deleted Copy Constructor
(4): Explicitly defined Defaulted virtual destructor
(5): Defaulted Move Assignment
(6): Deleted Copy Assignment

<h4>read methods</h4>

```cpp
virtual size_t read(void* ptr,size_t size)=0; //(1)
virtual int read()=0; //(2)
template<size_t N> size_t read(uint8_t(&arr)[N]); //(3)
template<size_t N> size_t read(int8_t(&arr)[N]); //(4)
template<size_t N> size_t read(std::byte(&arr)[N]); //(5)
```

(1): read(void* ptr,size_t size)<br/>
Reads size bytes from the stream and places it in the buffer pointed to by ptr, then returns the number of bytes read.
If there exists a complete (that is, well-formed object) at the address given by ptr, then that object, and all of its sub-objects must satisfy the following requirements or the behavior is undefined:
<ul>
<li>The object must satisfy TriviallyCopyable</li>
<li>The object must not a pointer (including pointer to function or pointer to member) and may not have any pointer non-static members</li>
<li>The object must not have any virtual or duplicate (direct or indirect) base classes</li>
<li>The object must not have any virtual functions or destructors</li>
<li>The object must not have any reference non-static data members</li>
<li>The object must not have any cv-qualified non-static data members</li>
</ul>
Further, if the object or any of its sub-objects are a multibyte scalar data-type, the byte order of those values is unspecified. If the byte-order 
In addition, subclasses of InputStream may impose other restrictions on objects which can be written. If a subclass imposes additional restrictions, they must be clearly detailed including the result of violating such a restriction, whether it be undefined, unspecified, implementation defined, or it results in an exception.
If the number of bytes read is less then size, this will be indicated in the return value. If an unexpected number of bytes was read, the object is invalidated, and using the object is undefined behavior (except for assignment operators and destructors), unless the object is char, unsigned char, signed char, or std::byte or an array there of, or the object is an array. If the object is an array of any type except for char, unsigned char, signed char, or std::byte, then the total number of valid elements in the array is the number of complete elements read, and using elements passed this point in any way except the above is undefined behavior. If the object is an array of char, unsigned char, signed char, or std::byte, then all elements of the array are valid, and the elements passed the end of the read retain the value which they held before the call to read. 
If there is not a complete object at the address given by ptr, then ptr must refer to a pointer returned from an allocation function (such as operator new, malloc, calloc, etc.), which was allocated with at least size bytes, or the behavior is undefined. Passing a null pointer to read is undefined behavior.
If the size of the object indicated by ptr is not at least size, the behavior is undefined.
<br/><br/>

(2): int read()<br/>
Reads a single byte from the stream and returns it. If EOF is reached returns -1 instead.
<br/><br/>

(3),(4),(5): Reads N bytes from the stream into arr. Effectively read(arr,N);

<h5>Exceptions</h5>
(1): Any subclass of InputStream may throw any Exception from the read method. If a subclass can throw an exception, it must be clearly detailed which exceptions can be thrown and in what cases they are. If an exception is thrown by the read method, the object being read is invalidated, and using in any way (except for assignment operators/destructors) is undefined behavior.
(2),(3),(4),(5): Same as (1). 

<h4>Stream Error Analysis</h4>

```cpp
virtual bool checkError()const noexcept(true)=0; //(1)
virtual void clearError()noexcept(true)=0; //(2)
explicit operator bool()const noexcept(true); //(3)
bool operator!()const noexcept(true); //(4)
```

(1),(4): Checks if an error is on stream. If there is an error the stream should not be used (may be closed or moved). (4) is implemented in terms of (1).<br/> 
(3): opposite of checkError(), IE. checks if the stream is OK.<br/>
(2): Clears any error which may be on stream. Has no effect if there is no error, or the error cannot be cleared.
<h5>Exceptions</h5>
All are non-throwing.


<h3>class FilterInputStream</h3>

Abstract base class for InputStreams which wrap an existing stream, to add functionality.

<h4>Base classes</h4>

Extends (public) InputStream

<h4>Constructors and Destructors</h4>

```cpp
FilterInputStream(InputStream&); //(1)
~FilterInputStream()=0; //(2)
```

(1): Constructs a FilterInputStream binding it to the specific InputStream.
(2): Virtual Destructor to prevent direct Construction of FilterInputStreams

<h4>Read Methods</h4>

```cpp
size_t read(void* ptr,size_t size); //(1)
int read(); //(2)
```
(1),(2): Forwards to the equivalent method in the wrapped stream.
Any exceptions thrown by the equivalent method in the wrapped stream propagate through this call.

<h4>Stream Error Analysis</h4>

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```

(1),(2): Same as the equivalent method in the underlying stream.

<h3>class FileInputStream</h3>

<h4>Base Class</h4>
Extends (public) InputStream

<h4>Constructors, Destructors, and Assignment Operations</h4>

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
The behavior is undefined if the file is not opened in read mode, is not a null pointer and was not the return value of fopen, or was previously closed. 

In addition, the behavior is unspecified (and may be undefined), if the stream was not opened in binary read mode.

Exceptions: Throws FileNotFoundException if the passed file is a null pointer


(2),(3),(4):Constructs the FileInputStream from the given filename. Constructs the stream as though by `FileInputStream(fopen(c,"rb"))`, `FileInputStream(fopen(str.c_str(),"rb"))`, or `FileInputStream(fopen(p.c_str(),"rb"))` respectively.

Exceptions: Throws FileNotFoundException if given file does not exist or cannot be openned.

(5):Constructs a new FileInputStream by moving in. Control of the File object wrapped by in is moved to the new stream.

(6): Destroys this stream and closes the underlying file if it has not been moved.

(7):Moves in to this. If this has control of an underlying file has not been moved, 
 it is closed, then the control of the underlying file wrapped by in is moved to this.

<h4>Read Methods</h4>

```cpp
size_t read(void* ptr,size_t size); //(1)
int read(); //(2)
```

(1): Reads size bytes from the underlying file into the object pointed to by ptr. ptr must satisfy the same requirements as specified by InputStream::read
(2): Reads a single byte from the underlying file and returns it or EOF if the file has reached the end of file.

<h5>Exception Guarantee</h5>
Neither method will throw any exceptions
<h4>Raw Stream Access</h4>

```cpp
FILE* getUnderlying()const noexcept(true);
```
Gets the underlying file or a null pointer if control of the underlying file has been moved. This allows for users to query the status of the underlying file (such as with ferror). If the underlying file is closed, reopened, used for raw c io operations, or passed to the constructor of FileInputStream, the behavior is undefined.
<h5>Exception Guarantee</h5>
noexcept(true)

<h4>Stream Error Analysis</h4>

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1): Checks if an error is on stream. Checks the stream's Error and EOF flags.  Effectively the same as ferror(getUnderlying())||feof(getUnderlying()). 
(2): Clears any error on stream. Effectively the same as clearerr(getUnderlying()).

<h3>DataInputStream</h3>
A wrapper on basic InputStreams, which provides Byte-Order sensitive read operations for scalar and other types. 
By Default DataInputStream reads multibyte scalar types in Big Endian (network) Byte Order. If Reading in Little Endian (native) Byte Order is required, the disambugation tag little_endian may be passed to the constructor. If the raw read or readFully method is used to read objects of a multibyte scalar type, the behavior is undefined. 
When in Big-Endian mode, scalar types (except Enum types) are read in a manor functionally equivalent to the equivalent read from java.io.DataInputStream, and as such, values of the same type written by a java.io.DataOutputStream are suitable to be read by this class. Strings read by readString are treated as if they are in UTF-8 or Ascii Encoding and are read simiarly to java.io.DataInputStream.readUTF, and therefore strings written by java.io.DataOutputStream's writeUTF method are suitable for being read by this class.

<h4>Base classes</h4>
Extends (public) FilterInputStream

<h4>Constructors</h4>

```cpp
DataInputStream(InputStream&); //(1)
DataInputStream(InputStream&,little_endian_t); //(1)
```

(1): Constructs a DataInputStream wrapping the given stream in BigEndian mode. The raw read methods of this DataInputStream forward to this stream.
(2): Same as (1), except the stream is constructed in LittleEndian mode.

<h4>Methods</h4>
<h5>readFully</h5>

```cpp
void readFully(void* ptr,size_t size);
```
Reads exactly size bytes from the underlying stream into the object given by ptr. The object pointed to by ptr must satisfy the requirements of InputStream::read(void*, size_t)
If size bytes cannot be read, an EOFException is thrown.

<h5>Special read methods</h5>

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
(1),(2): reads a single byte from the stream and returns it as an unsigned or signed byte respectively. The behavior is undefined unless the byte on stream was not written by the writeByte or writeUnsignedByte methods of a DataOutputStream or a similar utility.
(3),(4): reads 2 bytes from the stream and returns them as an unsigned or signed short, respectively. The behavior is undefined unless both bytes on stream were written by the writeShort or writeUnsignedShort methods of a DataOutputStream in the same byte order mode or a similar utility, and both were written as part of the same call.
(5): reads 4 bytes from the stream and returns them as an int. The behavior is undefined unless all 4 bytes on stream were written by the writeInt method of a DataOutputStream in the same byte order mode or a similar utility, and they were  all written as part of the same call.
(6): reads 8 bytes from the stream and returns them as an int64_t. The behavior is undefined unless all 8 bytes on the stream were written by the writeLong method of a DataOutputStream in the same byte order mode or a similar utility, and they were all written as part of the same call.
(7): reads 4 bytes from the stream and returns them as a 32-bit single-precision float. The behavior is undefined unless all 4 bytes on the stream were written by the writeFloat method of a DataOutputStream in the same byte order mode or a similar utility, and they were all written as part of the same call. In addition, the behavior is undefined if the default float on the system is not an IEEE754 single-precision binary32 floating-point number. If the value read is a NaN, it is unspecified whether the binary representation of the NaN is preserved. If the NaN is a signaling NaN it is also unspecified whether the NaN is quieted and a floating point error is raised.
(8): reads 8 bytes from the stream and returns them as a 64-bit double. The behavior is undefined unless all 8 bytes on the stream were written by the writeDouble method of a DataOutputStream in the same byte order mode or a similar utility, and they were all written as part of the same call. In addition, the behavior is undefined if the default double on the system is not an IEEE754 double-precision binary64 floating point number. If the value read is a NaN, it is unspecified whether the binary representation of the NaN is preserved. If the NaN is a signaling NaN it is also unspecified whether the NaN is quieted and a floating point error is raised.
(9): reads a 2-byte length, as though by readUnsignedShort from the stream, and then reads that many bytes, treating them as a utf-8 encoded string (non-nul terminated), and returns the resultant std::string. The behavior is undefined unless all bytes read from the stream were written by the writeString method of a DataOutputStream in the same byte order mode or a similar utility, the first byte read was the first byte written by that call, and all bytes were written by that call.
(10): reads a number of bytes based on the underlying type of E as if by the best matching read method for that type, and returns to bytes treated as a value of type E. The behavior is undefined unless all bytes read were written by an equivalent call to writeEnum of DataOutputStream, and if the enum type is not 1 byte long, in the same byte order mode. If E is std::byte it may additionally be used to read byte sequences suitable for the readSignedByte() and readUnsignedByte()  methods. This method only participates in overload resolution if `std::is_enum_v<E>` is true. 

If any of the methods reach an EOF before finishing the read, an EOFException is thrown

<h5>Stream Operator Overloads</h5>
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


<h3>class NullDeviceInputStream</h3>
NullDeviceInputStream is a Special InputStream which reads from the Null Symbolic Device (on Linix/Unix, this is at /dev/null). 
Read Methods will unconditionally yield EOF, and no bytes are read. 
Note that Despite this referencing a symbolic device, no actual read is made, the methods simply do nothing. 


<h4>Read Methods</h4>

```cpp
std::size_t read(void* ptr,std::size_t sz); //(1)
int read(); //(2)
```
(1), (2): Reads from the stream. The NullDevice is allways empty, and these methods unconditionally return EOF. Unlike InputStream::read(void*,std::size_t), this method will NOT invalidate any objects at ptr. 
However, despite the fact that the method has no effect, ptr must still follow all rules layed out in InputStream::read(void*,std::size_t);

<h5>Exceptions</h5>
Neither method will throw any exceptions.

<h4>Stream Error Analysis</h4>

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1):Checks if an error is on stream. As No reads are made this method unconditionally returns false.
(2): Clears the error on stream. Due to no errors being reported, this method has no effect.

<h3>class ZeroDeviceInputStream</h3>
ZeroDeviceInputStream is a InputStream that reads from the Symbol Zero Device (On Linux/Unix, this is at /dev/zeros). 
Read methods will result in a stream of Zeros, that is, will fill the buffer provided with 0s and return zero from read(). 
Despite referencing /dev/zeros, no actual device read is made, the implementation is in terms of memset. This may be a more secure method of destroying objects, due to the optimizations of memset, which are not present in this file (as IO operations cannot be optimized due to the AS-IS Rule). 

<h4>Read Methods</h4>

```cpp
std::size_t read(void* ptr,std::size_t sz); //(1)
int read(); //(2)
```
(1): Reads sz bytes from the stream into ptr. This sets all bytes in the object indicated by ptr to 0, then returns sz. 
(2): Reads a byte from the stream and returns it. Always returns 0;

<h5>Exceptions</h5>
Neither method will throw any exceptions.


<h4>Stream Error Analysis</h4>

```cpp
bool checkError()const noexcept(true);//(1)
void clearError()noexcept(true);//(2)
```
(1): Checks if an error is on stream. As no read is actually preformed, this always returns false
(2): Clears the error on stream. As no errors are reported, this method has no effect

<h3>ByteArrayInputStream</h3>
Reads bytes from a internal buffer. An unsupressable error condition is reported if the end of the buffer has been reached.

<h4>Constructors</h4>

```cpp
template<typename Byte> ByteArrayInputStream(const Byte* buff,std::size_t blen); //(1)
template<typename Byte,std::size_t N> ByteArrayInputStream(const Byte(&arr)[N]); //(2)
template<typename Byte,std::ptrdiff_t extent> ByteArrayInputStream(const span<Byte,extent>& s); //(3)
template<typename Byte> ByteArrayInputStream(const span<Byte,dynamic_extent>& s); //(4)
```
(1): Constructs a new ByteArrayInputStream over the given buffer. buff must be a pointer to a valid object which is at least blen bytes in size. The behavior is undefined if buff is a null pointer, an invalid pointer, or points to an object which is not TriviallyCopyable, or either the object or any subobject meets any of the following conditions:
<ul>
<li>Is a volatile object</li>
<li>Is a Pointer type (including Pointer to member and Function Pointer types)</li>
<li>Is a Reference type</li>
</ul>
The requirements are effectively the same as for OutputStream.write(const void*,std::size_t).<br/>
The Total length of the resultant buffer is sz. 
This method does not participate in overload resolution unless `is_byte_v<Byte>` is true. 
(2): Constructs a new ByteArrayInputStream over the given array, with total size N. This method does not participate in Overload resolution unless `is_byte_v<Byte>` is true. 
(3),(4): Constructs a new ByteArrayInputStream over s.data(), with total size s.size(). This method does not participate in Overload resolution unless `is_byte_v<Byte>` is true. (3) does not participate in Overload resolution if extent&lt;0. Byte may be a const qualified byte type.

<h4>Read Methods</h4>

```cpp
std::size_t read(void* ptr,std::size_t sz); //(1)
int read(); //(2)
```
(1): reads sz bytes from the buffer to ptr. If there are no bytes remaining in the buffer, returns EOF. 
Otherwise, if there is n bytes remaining in buffer, where n is less than sz, then n bytes are copied from the buffer to ptr, the buffer is advanced to the end, and n is returned. This may invalidate objects at ptr as specified in InputStream.read(void*,std::size_t). Otherwise, sz bytes are copied to ptr, the buffer is advanced by sz bytes, and sz is returned<br/>
(2): reads a single byte from the stream. If there are no bytes remaining in the buffer, returns EOF. 
Otherwise, returns the byte at the current position of the buffer, and advances the buffer by 1.

<h5>Exceptions</h5>
Neither method will throw an exception

<h4>Stream Error Analysis</h4>

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1): Checks if there is an error on stream. Returns true if the buffer is at the end of the stream, false otherwise.
(2): Clears any error on the stream. This method has no effect as the only error is unsuppressable (end of stream).

<h3>class OutputStream</h3>
OutputStream is the abstract base class of the output portion of the IOWrapper Library, mirroring InputStream. Like InputStreams, OutputStreams have ownership of any resources they use, and aquires and releases those resources during construction and destruction respectively. Subclasses of OutputStream may buffer output before commiting writes in the stream. If a subclass does, it must override the flush method to flush the internal buffer and commiting previous writes to the resource. In addition the buffer must be flushed when the stream is destroyed or reassigned.  If flush is not called it is unspecified when writes are reflected in the underlying resource. If the OutputStream does not buffer input then writes are immediately reflected in the underlying resource, and flushing the stream has no effect. 
OutputStreams are not inheritly thread safe. The effects of using a single OutputStream across multiple threads is unspecified, and may be undefined.

<h4>Constructors, Destructors, and Assignment Operators</h4>

```cpp
OutputStream()=default; //(1)
OutputStream(const OutputStream&)=delete; //(2)
OutputStream(OutputStream&&)=default; //(3)
virtual ~OutputStream()=default; //(4)
OutputStream& operator=(const OutputStream&)=delete; //(5)
OutputStream& operator=(OutputStream&&)=default; //(6)
```
(1): Defaulted Nullary Constructor
(2): Deleted Copy Constructor
(3): Defaulted Move Constructor
(4): Virtual Defaulted Destructor
(5): Deleted Copy Assignment Operator
(6): Defaulted Move Assignment Operator

<h4>Write Methods</h4>

```cpp
virtual size_t write(const void* ptr,size_t size)=0; //(1)
virtual void write(uint8_t b)=0; //(2)
template<size_t N> size_t write(uint8_t(&arr)[N]); //(3)
template<size_t N> size_t write(int8_t(&arr)[N]); //(4)
template<size_t N> size_t write(std::byte(&arr)[N]); //(5)
```
(1): writes size bytes from the object pointed to by ptr to the stream, and returns the total number of bytes written. The behavior is undefined if ptr is null or does not point to a complete object. In addition, if ptr points to a complete object, the behavior is undefined unless all of the objects and all subobjects meet the following conditions:
<ul>
<li>The Object must be TriviallyCopyable</li>
<li>The Object may not be a pointer (including pointer to function or pointer to member), or have any pointer non-static data members</li>
<li>The Object must not have virtual or duplicate (direct or indirect) base classes</li>
<li>The Object must not have virtual methods or destructors</li>
<li>The Object must not have any reference members</li>
<li>The object must not be volatile or have any volatile members</li>
</ul>
In addition if the object or any of its subobjects are of multibyte scalar types the byte order which they are written to the stream is unspecified. 
In general, the requirements of the object passed to write mirrors those for objects passed to InputStream's read method, except that they may have const qualified members. 
Note that because it is intended that reads an writes are mirrored, if the type has any const qualfied members it would be undefined behavior to read to an object of such a type. 
In general, read objects should have the same (possibly less cv-qualfied) type as the written object, though this is not enforced. It is mearly enough that the read and written objects are of a Layout compatible type. If this is not the case the behavior is undefined.
If objects are intended for persistance accross platforms, languages, or even compilers, the object should be StandardLayout in addition to the above, as non-StandardLayout types may not be Layout Compatible with the same type on a different compiler. If Long Term Persistance is required, or the layout of the data is required to be fixed, DataOutputStream is a better utility then a raw OutputStream.
Like InputStream::read, subclasses may impose other restrictions on what objects may be passed to the write method. Those subclasses must detail these additional restrictions and the effects of violating them if they exist.

(2): writes a single byte to the stream. 
(3),(4),(5): writes an array of `uint8_t`, `int8_t`, or `std::byte` to the stream. Effectively read(arr,N);

<h5>Exceptions</h5>
(1),(2): Subclasses may throw any exception from the write method. If they do, it must be detailed which exceptions can be thrown, and the conditions under which they are thrown. If an exception is thrown from a stream that buffers writes, the stream is rolled-back to state it was in prior to the write call, and the internal buffer is left in an unspecified state. If an exception is thrown from a stream that does not buffer writes, the stream is left in an unspecified state.
(3),(4),(5): Forwards any exceptions thrown by write.

<h4>flush</h4>

```cpp
virtual void flush();
```
Flushes the stream. Has no effect if the OutputStream does not buffer input. Otherwise the internal buffer must be written to the resource.

<h5>Exceptions</h5>
Subclasses may throw any exception from flush. If they do, it must be detailed which exceptions can be thrown, and the conditions under which they are thrown. If an exception is thrown, both the stream and the internal buffer are left in an unspecified state.

<h4>Stream Error Analysis</h4>

```cpp
virtual bool checkError()const noexcept(true)=0; //(1)
virtual void clearError()noexcept(true)=0;//(2)
explicit operator bool()const noexcept(true);//(3)
bool operator!()const noexcept(true); //(4)
```

(1),(4): Checks if an error is on stream. The stream should not be used if an error is on the stream until it is cleared. (4) is implemented in terms of (1).
(3): Checks if there is no error on stream. Effectively the Inverse of checkError()
(2): Clears any error on stream. Has No effect if there is no error, or the error cannot be cleared.

<h5>Exceptions</h5>
All methods are non-throwing

<h3>class FilterOutputStream</h3>
FilterOutputStream wraps an existing OutputStream to be extended to provide extra functionality. The Stream is Not explicitly managed by FilterOutputStream or any subclasses. Destroying the FilterOutputStream will neither close nor flush the underlying stream object.

<h4>Base Classes</h4>
Extends (public) OutputStream

<h4>Constructors</h4>

```cpp
FilterOutputStream(OutputStream& out); //(1)
~FilterOutputStream()=0; //(2)
```
(1): Constructs a FilterOutputStream that wraps (but does not own) out.
The behavior is undefined if out is destroyed before the current object.
(2): Destructor defined as pure to prevent direct construction of FilterOutputStream

<h4>Methods</h4>

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

<h4>Exceptions</h4>
(1),(2),(3) Any exception thrown by the respective method in the underlying stream propagates through all methods
(4),(5): Non-throwing

<h3>FileOutputStream</h3>
FileOutputStream is an OutputStream which wraps a c FILE*. The file is managed by the stream and is closed when it is destroyed. FileOutputStream will buffer writes if the underlying c IO implementation buffers writes.

<h4>Constructors, Destructors, and Assignment Operators</h4>

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

(1): Constructs a new FileOutputStream, binding the underlying file to f and taking control of f. The behavior is undefined if f is not a null pointer, and was not obtained from a call to fopen, was closed, or is open in read-only mode.
In addition, the behavior is unspecified (and may be undefined), if f was not opened in binary write or binary append mode.
(2),(3),(4): Constructs a new FileOutputStream from the given path name. 
This constructor acts as if by `FileOutputStream(fopen(c,"wb"))`, `FileOutputStream(fopen(str.c_str(),"wb"))`, or `FileOutputStream(fopen(p.c_str(),"wb"))`. The behavior is undefined if the argument to (2) is a null pointer.
(5),(6),(7): Disambugation Constructors for openning in append mode. Same as (2),(3),(4) respectively, except that the stream is openned in "ab" mode instead of "wb" mode.
(8): Move Constructor. Control of the underlying file object of out is moved to the new object
(9): Destructor. If control of the underlying file has not been moved from this object, the underlying file is closed. If the underlying file buffers writes, it is also flushed.
(10): Move Assignment Operator. If control of the underlying file has not been from this object it is closed and potentially flushed. Then control of the underlying file of out is moved to this object.

<h5>Exceptions</h5>
(1): If the passed file is a null pointer, the constructor throws a FileNotFoundException
(2),(3),(4),(5),(6),(7): If the named file does not exist or cannot be created, or the named file cannot be opened in write mode, the constructor throws a FileNotFoundException

<h4>Write and Flush Methods</h4>

```cpp
size_t write(const void* ptr,size_t size); //(1)
void write(uint8_t); //(2)
void flush(); //(3)
```
(1): writes size bytes from the object pointed to by ptr to the underlying file. The requirements for the object written is detailed in OutputStream::write.
(2): writes a single byte to the underlying file
(3): If the underlying file buffers writes then flushes that file. Otherwise does nothing

<h5>Exceptions</h5>
None of these methods will throw an exception

<h4>Raw File Access and Stream Error Analysis</h4>

```cpp
FILE* getUnderlying()const noexcept(true); //(1)
bool checkError()const noexcept(true);//(2)
void clearError()noexcept(true); //(3)
```
(1):Gets the underlying file of the InputStream. This file may be inspected by feof, ferror or similar inspection functions. Closing or flushing the file, constructing a new FileOutputStream with the file, or using the file in raw c io calls is undefined behavior. 
(2): Checks if there is an error on the stream. Effectively ferror(getUnderlying())||feof(getUnderlying()) 
(3): Clears any error on the stream. Effectively clearerror(getUnderlying())


<h5>Exceptions</h5>
noexcept(true)

<h3>class DataOutputStream</h3>
DataOutputStream wraps an OutputStream to add consistent write methods for scalar data-types and some other types. By Default DataOutputStream reads multibyte scalar types in Big-Endian (network) Byte Order. If Little-Endian (native) Byte Order is desired, the little_endian_t disambugation constructors may be used.
The byte sequences produced by given write methods of a DataOutputStream in Big-Endian byte order mode are consistent with the ones produced by the same write calls of java.io.DataOutputStream, and is therefore are suitable for reading by java.io.DataInputStream. In addition, strings written by writeString are similar to strings written by java.io.DataOutputStream.writeUTF, and therefore may be read by java.io.DataInputStream.readUTF. The effects of writing a multibyte scalar data-type or objects with non-static data members of such a type with a raw write call is undefined.

<h4>Base Classes</h4>
Extends (public) FilterOutputStream

<h4>Constructors</h4>

```cpp
DataOutputStream(OutputStream& out); //(1)
DataOutputStream(OutputStream& out,little_endian_t); //(2)
```

(1): Constructs a DataOutputStream wrapping (but not owning), out, in Big-Endian Byte order mode
(2): Disabugation overload for Little-Endian byte order mode. The Stream wraps (but does not own) out.

<h4>Methods</h4>
<h5>Specialized write Methods</h5>

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
(1): writes b to the underlying stream. The byte is suitable for being read by  the readSignedByte or readUnsignedByte methods of a DataInputStream or similar utility
(2): writes s to the underlying stream. The bytes written are suitable for being read by the readShort or readUnsignedShort methods of a DataInputStream in the same byte order mode, or a similar utility
(3): writes i to the underlying stream. The bytes written are suitable for being read by the readInt method of a DataInputStream in the same byte order mode, or a similar utility
(4): writes l to the underlying stream. The bytes written are suitable for being read by the readLong method of a DataInputStream in the same byte order mode, or a similar utility
(5): writes the length of str to the stream as if by writeShort, then writes that many bytes to the stream from str.data(). If the length of str is greater then 65536, only the first 65536 bytes of str are written. The bytes written are suitable for being read by the readString method of a DataInputStream in the same byte order mode, or a similar utility. The behavior is undefined if str contains embedded NUL bytes.
(6): writes f to the underlying stream. The bytes written are suitable for being read by the readFloat method of a DataInputStream in the same byte order mode, or a similar utility. The behavior is undefined if the default float on the system is not an IEEE754 single-precision binary32 floating-point number. If the passed float is NaN, it is unspecified whether the bit representation of the NaN is preserved or not.
(7): writes d to the underlying stream. The bytes written are suitable for being read by the readDouble method of a DataInputStream in the same byte order mode, or a similar utility. The behavior is undefined if the default double on the system is not an IEEE754 double-precision binary64 floating-point number. If the passed double is NaN, it is unspecified whether the bit representation of the NaN is preserved or not.
(8): writes e to the underlying stream. The bytes written are suitable for being read by the equivalent readEnum method of a DataInputStream open in the same byte order mode. Unlike other methods writeEnum is not compatible with any similar interfaces. This method does not participate in overload resolution unless `std::is_enum_v<E>` is true

<h5>Exceptions</h5>
Any exceptions thrown by the write method of the underlying stream propagates through these methods.

<h4>Stream Write Operator Overloads</h4>

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

<h3>class NullDeviceOutputStream</h3>
NullDeviceOutputStream is a stream that writes to the Null Symbolic Device (/dev/null on Linux/Unix). Write methods will have no effect, no writes actually take place. 
Despite referencing the Null Device, no handle to that device is created. 
NullDeviceOutputStream is unbuffered.

<h4>Write Methods</h4>

```cpp
std::size_t write(const void* ptr,std::size_t sz); //(1)
void write(uint8_t b); //(2)
```
(1): Writes sz bytes from ptr to the stream. This method will have no effect and return sz. Note that this is because the Null Device traditionally acts as a Byte Sink, that is, all writes are accepted by it and discarded. This is in contrast to reading from the Null Device which always yields EOF. 
(2): Writes a single byte to the stream. This method has no effect

<h5>Exceptions</h5>
Neither method will throw any exceptions.

<h4>Stream Error Analysis</h4>

```cpp
bool checkError()const noexcept(true); //(1)
void clearError()noexcept(true); //(2)
```
(1): Checks if there is an error on stream. As no writes actually take place, this method unconditionally returns false. 
(2): Clears any error on the stream. As no errors are reported, this method has no effect.

<h3>class ByteArrayOutputStream</h3>
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
const std::byte* getBuffer()const noexcept(true); //(1)
std::size_t getSize()const noexcept(true); //(2)
bool checkError()const noexcept(true); //(3)
void clearError()noexcept(true); //(4)
```
(1): Gets the underlying buffer. This provides a read access pointer to the start of the buffer. This pointer is invalidated if a subsequent call to either write method is made. 
(2): Gets the size of the underlying buffer. 
(3): Checks if there is an error on stream. No errors are reported, so this method unconditionally returns false.
(4): Clears any error on stream. As no errors are reported, this method has no effect.


<h2>Fields</h2>

```cpp
const append_t append{}; //(1)
const little_endian_t little_endian{}; //(2)
const std::size_t EOF{-1}; //(3)
```

(1): Tag for FileOutputStream constructors to disambugate the overloads that open the file in append mode instead of write mode.
(2): Tag for DataOutputStream/DataInputStream constructors to disambugate overloads that write/read in little-endian byte order mode instead of Big-Endian byte order mode
(3): Value returned from read() to indicate that the end-of-file was reached.
