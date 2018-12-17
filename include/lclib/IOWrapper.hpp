#ifndef __IOWrapper_hpp_2018_07_26_22_33
#define __IOWrapper_hpp_2018_07_26_22_33
#include <cstdio>
#include <cstdint>
#include <string>
#include <lclib/Version.hpp>
#include <lclib/UUID.hpp>
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <lclib/Vector.hpp>
#include <lclib/Config.hpp>
#include <lclib/Span.hpp>
#if __has_include(<filesystem>)
#include <filesystem>
#define USE_PATH_CTORS
#endif
#include <lclib/TypeTraits.hpp>
#undef EOF

#if defined(__cpp_char8_t)&&__cpp_char8_t>=201811L
	typedef char8_t u8char;
#else
	typedef char u8char;
#endif
 class FileNotFoundException:public std::exception{
public:
	 const char* what()const noexcept(true) override;
};



 class EOFException:public std::exception{
public:
	 const char* what()const noexcept(true) override;
};

/**
 * Tag type for little endian overloads of DataInputStream/DataOutputStream constructors
 * Directs the stream to write multibyte primitives in Little-Endian Mode (default Big-Endian)
 */
struct little_endian_t{
public:
    constexpr explicit little_endian_t()=default;
};

const little_endian_t little_endian{};

const std::size_t EOF=-1;

/**
 * Abstract Base class which represents an Source of Binary Data
 * The class uses the RAII model (that is, any resources held by the stream are aquired when the
 * object is created, and released when it is destroyed.
 * Because of obvious restrictions with RAII types Input Stream is move-only.
 * It is the the responsibility of any subclasses to ensure that resources are correctly moved in these cases.
 */
class LIBLCAPI InputStream{
    InputStream(const InputStream&)=delete;
    InputStream& operator=(const InputStream&)=delete;
protected:
    ~InputStream()=default;
public:
    void* operator new(std::size_t)=delete;
    void operator delete(void*)=delete;
    InputStream()=default;

    InputStream(InputStream&&)=default;
    InputStream& operator=(InputStream&&)=default;
    /**
     * Reads a up to the given size_t bytes from the stream into the provided buffer.
     * The size read is returned or EOF.
     * If an object exists at the address passed, it must be a TriviallyCopyable type,
     * that is not a pointer (including pointer to member or function pointer types),
     * that has no pointer non-static members (including pointer to member, or function pointer types), reference non-static members,
     * virtual functions (including virtual destructors), cv-qualified members, or reference members, 
     *  or the behavior of this function is undefined.
     * If the object is a multibyte scalar data-type (or has any multibyte scalar members),
     *  it is unspecified whether or not the data is read in Big-Endian or Little-Endian.
     * If The byte order of scalar data is required to be fixed, use the DataInputStream class.
     * If there is a complete, non-array object at the target of the pointer, and the returned size
     * is less then the supplied size it is undefined behavior to use that object
     * If the object at the given pointer is an array then up to n complete non-array subobjects
     * of the array may be accessed where n is the floor of the size returned/the size of the
     * complete non-array type, and accessing any objects passed this point is undefined behavior.
     * If there is not a complete object at the pointer given then the pointer shall be one returned
     * from operator new(size_t) with a given size at least the value passed to this function, or the behavior is undefined.
     * If the size of the object at the given pointer is less then the given size, then the behavior of this function is undefined.
     * If the object at the target is a char, signed char, unsigned char, or std::byte or an array of such,
     * the byte's passed where this function reads to are untouched and accessing them is not undefined behavior.
     * Note that if the object is an array of char which models a C-style string,
     * and the full string is not read, then passing the string to any string sensitive operations
     * (strlen, strcpy, or std::string(const char*)) is undefined behavior.
     * Subclasses may impose further restrictions on what may be read to.
     * These subclasses are required to detail what results in undefined behavior and what may cause an exception to be thrown.
     * If an exception is thrown, the stream is left in an unspecified state.
     * All objects being read to are invalidated in this case, and there use is undefined behavior
     * \Exception Guarantee: Implementations may throw an exception from this function
     */
    virtual size_t read(void*,size_t)=0;
    /**
     * Reads a single byte from the stream.
     * If a byte is successfully read, then the byte is 0-extended to an int.
     * Otherwise -1 is returned.
     * \Exception Guarantee: Implementations may throw an exception.
     */
    virtual int read()=0;

    template<typename T,std::size_t N,typename=std::enable_if_t<is_byte_v<T>&&!std::is_const_v<T>>>
    		std::size_t read(T(&arr)[N]){
    	return read(arr,N);
    }
    template<typename T,std::size_t N,typename=std::enable_if_t<is_byte_v<T>&&!std::is_const_v<T>>>
			std::size_t read(std::array<T,N>& arr){
		return read(arr.data(),N);
	}
    virtual bool checkError()const noexcept(true)=0;
    explicit operator bool()const noexcept(true);
    bool operator!()const noexcept(true);
    virtual void clearError()noexcept(true)=0;
};



/**
 * An Input Stream which reads from (and wraps) a FILE*
 * This class can be constructed from an existing file, a c-style string, or a
 * std::string.
 */
class LIBLCAPI FileInputStream final:public InputStream{
private:
    FILE* underlying;
public:
    /**
     * Constructs a FileInputStream from a FILE*.
     * Behavior is undefined if the passed pointer is not NULL or nullptr,
     * and does not point to an object created by fopen.
     * If The file is created with fopen behavior is unspecified(and may be undefined) if the file is not open in
     * binary read mode (fopen(<name>,"rb")).
     * \Exceptions: This constructor throws a FileNotFoundException if the passed FILE is a null pointer
     */
    FileInputStream(FILE*);

    /**
     * Constructs a FileInputStream from a c string
     * The stream is constructed as if by FileInputStream(fopen(str,"rb"))
     * \Exceptions: This constructor throws a FileNotFoundException if the file cannot be found or opened for any reason.
     */
    FileInputStream(const char*);
    /**
     * Constructs a FileInputStream from a std::string.
     * The stream is constructed as if by FileInputStream(fopen(str.c_str(),"rb")
     * \Exceptions: This constructor throws a FileNotFoundException if the file cannot be found or opened for any reason.
     */
    FileInputStream(const std::string&);

#ifdef USE_PATH_CTORS
    /**
     * Constructs a FileInputStream from a std::filesystem::path
     * The stream is constructed as if by FileInputStream(fopen(p.u8string().c_str(),"rb"))
     * \Exceptions: If the path cannot be opened in read mode (it does not exist or you do not have read access to the file), a FileNotFoundException is thrown
     */
    FileInputStream(const std::filesystem::path&);
#endif

    /**
     * Constructs a new FileInputStream from a moved from object.
     * The moved from object will not have an underlying file after this call
     */
    FileInputStream(FileInputStream&&);
    /**
     * Closes the underlying file then moves the given object to this one.
     * If there is no underlying file (the object has been moved from),
     * the underlying file is not closed.
     * The moved from object will not have an underlying file after this call
     */
    FileInputStream& operator=(FileInputStream&&);
    /**
     * Closes the underlying file if it exists. Otherwise does nothing
     */
    ~FileInputStream(); // @suppress("Class has a virtual method and non-virtual destructor")
    /**
     * Reads bytes from the underlying file into the given array.
     * The Requirements and Restrictions are the same as in InputStream.
     * for the pointer ptr, and size n, this call acts as return fread(ptr,1,n,underlying);
     * except with the requirements and restrictions for the pointed to object from InputStream.read
     * \Exception Guarantee: This method will not throw an exception
     */
    size_t read(void*,size_t);
    /**
     * Reads a single byte from the stream.
     * This method acts as if a byte is read using read, and returned if the size is 1,
     * or -1 if no bytes are available.
     */
    int read();

    /**
     * Gets the underlying file controlled by the stream,
     * or a null pointer if the stream has been moved from
     */
    FILE* getUnderlying()const noexcept(true);

    bool checkError()const noexcept(true);
    void clearError()noexcept(true);
};


class LIBLCAPI FilterInputStream:public InputStream{
private:
	InputStream* underlying;
protected:
	 ~FilterInputStream();//Force Abstract
public:
	FilterInputStream(InputStream&);

	 size_t read(void*,size_t);
	 int read();
	 bool checkError()const noexcept(true);
	 void clearError()noexcept(true);
};

/**
 * A InputStream Wrapper which can write primitive data types to
 * an underlying stream.
 * The byte order can be configured (Big-Endian By default)
 * The behavior is undefined if the Target InputStream's lifetime ends before
 * this object's (even if no read methods are used between those points)
 */
class LIBLCAPI DataInputStream final:public FilterInputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
private:
    bool little;
    LIBLCHIDE int readSingle();
public:
    /*
     * Constructs a new DataInputStream which reads from a given stream.
     * Multibyte datatypes are written in Big Endian (network) Byte Order.
     */
     DataInputStream(InputStream&);
    /**
     * Tag Disambiguation Constructor which constructs a DataInputStream
     * that reads from a given stream.
     * Multibyte datatype are written in Little Endian (native) Byte Order
     */
     DataInputStream(InputStream&,little_endian_t);

    /**
     * Attempts to read n bytes into the provided pointer.
     * This call is guaranteed to read that many bytes. If the end of the stream is reached before reading bytes,
     * an EOFException is thrown.
     * If an EOFException is thrown, the object read is invalidated. Using that object is undefined behavior.
     *\Exceptions: If the End-Of-Stream is reached an EOFException is thrown. If the underlying stream's read method throws an exception, it is propagated through this call
     */
     void readFully(void*,size_t);
    /**
     * Reads a single byte from the stream and treats it as a uint8_t.
     * If no bytes are available on the stream, an EOFException is thrown.
     * If the byte was written by a DataOutputStream or a similar utility from an external language,
     * It is undefined behavior unless the write function used outputs a single byte (unsigned or signed).
     * (For example, the writeByte method of DataOutputStream)
     * \Exceptions: Same as readFully
     */
     uint8_t readUnsignedByte();
    /**
     * Reads a single byte from the stream and treats it as an int8_t
     * If no bytes are available on the stream, an EOFException is thrown
     * If the byte was written by a DataOutputStream or a similar utility from an external language,
     * it is undefined behavior unless the write function used outputs a single byte (unsigned or signed)
     * (For example, the writeByte method of DataOutputStream)
     * \Exceptions: Same as readFully
     */
     int8_t readSignedByte();
    /**
     * Reads 2 bytes from the stream and converts them to a uint16_t, using the byte order this DataInputStream is constructed with.
     * If there is not at least 2 bytes available, an EOFException is thrown.
     * If the bytes where written by a DataOutputStream or a similar utility from an external language,
     * it is undefined behavior unless both bytes were written with the same utility, and the write function used
     * is suitable for writing 2 byte numbers (signed or unsigned)
     */
     uint16_t readUnsignedShort();
    /**
	 * Reads 2 bytes from the stream and converts them to an int16_t, using the byte order this DataInputStream is constructed with.
	 * If there is not at least 2 bytes available, an EOFException is thrown.
	 * If the bytes where written by a DataOutputStream or a similar utility from an external language,
	 * it is undefined behavior unless both bytes were written with the same utility, and the write function used
	 * is suitable for writing 2 byte numbers (signed or unsigned)
	 * It is also undefined behavior unless the byte order of the writing utility is the same as the byte order if this stream.
	 * (By default DataOutputStream using Big Endian byte order)
	 */
     int16_t readSignedShort();
    /**
	 * Reads 4 bytes from the stream and converts them to an int, using the byte order this DataInputStream is constructed with.
	 * If there is not at least 4 bytes available, an EOFException is thrown.
	 * If the bytes where written by a DataOutputStream or a similar utility from an external language,
	 * it is undefined behavior unless all bytes were written with the same utility, and the write function used
	 * is suitable for writing 4 byte numbers (signed or unsigned)
	 * It is also undefined behavior unless the byte order of the writing utility is the same as the byte order if this stream.
	 * (By default DataOutputStream using Big Endian byte order)
	 */
     int readInt();
    /**
	 * Reads 8 bytes from the stream and converts them to a int64_t, using the byte order this DataInputStream is constructed with.
	 * If there is not at least 8 bytes available, an EOFException is thrown.
	 * If the bytes where written by a DataOutputStream or a similar utility from an external language,
	 * it is undefined behavior unless all bytes were written with the same utility, and the write function used
	 * is suitable for writing 8 byte numbers (signed or unsigned)
	 * It is also undefined behavior unless the byte order of the writing utility is the same as the byte order if this stream.
	 * (By default DataOutputStream using Big Endian byte order)
	 */
     int64_t readLong();
    /**
     * Reads a string from the stream.
     * An Unsigned short length prefix is read first, as if by readUnsignedShort
     * then that many bytes are read into a non-null terminated string.
     * The length and the string are then converted into a std::string.
     * The behavior is undefined if the string contains any embedded nul bytes,
     * and using the string in an encoding sensitive way is undefined behavior if the string contains any code-point errors.
     * The encoding of the string is utf-8.
     * If 2 bytes are not availble for reading the length, or that many bytes are not available for reading the string,
     * an EOFException is thrown.
     * \Exceptions: Same as readFully
     */
     std::basic_string<u8char> readString();
    /**
     * Reads a 4-byte value, as if by readInt(), from the stream which is then interpreted as a float.
     * The behavior is undefined unless the value was written by the writeFloat method of DataOutputStream,
     * or any similar utility in the same byte order.
     * In addition the behavior is undefined if the implementation does not support IEEE binary32 float.
     * If NaN is on stream, it is implementation defined if the bit representation of the NaN is preserved.
     * Relying on the bit representation of the read float is undefined behavior if the read value is NaN.
     * \Exceptions: Same as readFully
     * \Other Errors: If the read value is a Signaling NaN the method may raise an FP_ENV error and quiet the NaN (Implementation defined)
     */
     float readFloat();
    /**
     * Reads an 8-byte value, as if by readLong(), from the stream which is then interpreted as a double.
     * The behavior is undefined unless the value was written by the writeDouble method of DataOutputStream,
     * or any similar utility in the same byte-order.
     * In addition the behavior is undefined if the implementation does not support IEEE binary64 double.
     * If NaN is on stream, it is implementation defined if the bit representation of the NaN is preserved.
     * Relying on the bit representation of the read double is undefined behavior if the read value is NaN.
     * \Exceptions: Same as readFully
     * \Other Errors: If the read value is a Signaling NaN the method may raise an FP_ENV error and quiet the NaN (Implementation defined)
     */
     double readDouble();
    /**
     * Reads an Enum from the stream, by reading the underlying value and then converting it to the Enum Type.
     * It is undefined behavior unless the bytes were written by the corresponding writeEnum method
     * of a DataOutputStream using the same byte order.
     * Note that this method is Not Compatible with java's DataOutputStream (even in big-endian mode).
     * If it is necessary to provide cross language support, the enum should be written and read with read/writeInt()
     * \Exceptions: Same as readFully
     */
    template<typename E> std::enable_if_t<std::is_enum_v<E>,E> readEnum(){
    	std::underlying_type_t<E> val;
    	*this >> val;
    	return E{val};
    }
    /**
     * Overload of the Read-from-Stream operator for uint8_t.
     * This function acts as if b = readUnsignedByte()
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(uint8_t&);
    /**
     * Overload of the Read-from-Stream operator for int8_t.
     * This function acts as if b = readSignedByte()
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(int8_t&);
    /**
     * Overload of the Read-From-Stream operator for uint16_t.
     * This function acts as if s = readUnsignedShort()
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(uint16_t&);
    /**
     * Overload of the Read-From-Stream operator for int16_t.
     * This function acts as if s = readSignedShort()
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(int16_t&);
    /**
     * Overload of the Read-From-Stream operator for uint32_t
     * This function acts as if i = uint32_t(readInt());
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(uint32_t&);
    /**
     * Overload of the Read-From-Stream operator for int32_t.
     * This function acts as if i = readInt();
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(int32_t&);
    /**
     * Overload of the Read-From-Stream operator for uint64_t.
     * This function acts as if l = uint64_t(readLong())
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(uint64_t&);
    /**
     * Overload of the Read-From-Stream operator for int64_t.
     * This function acts as if l = readLong()
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(int64_t&);
    /**
     * Overload of the Read-From-Stream operator
     * This function acts as if s = readString()
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(std::string&);
#if defined(__cpp_char8_t)&&__cpp_char8_t>=201811L
     DataInputStream& operator>>(std::u8string&);
#endif
    /**
     * Overload of the Read-From-Stream operator for Verison
     * This function acts as if the version is initialized with a 2-byte value read in big-endian byte order as though by readUnsignedShort()
     * The behavior is undefined if the bytes on the stream were not written by a DataInputStream (either via write with a Version) or writeUnsignedShort in Big-Endian Mode.
     * This method is byte-order insensitive.
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(Version&);
    /**
     * Overload of the Read-From-Stream operator for UUID.
     * This function acts as if 2 longs are read via readLong() and are used to initialize the UUID.
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(UUID&);
    /**
     * Overload of Read-From-Stream operator for float.
     * This function acts as if by f = readFloat();
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(float&);
    /**
     * Overload of Read-From-Stream operator for double.
     * This function acts as if by d = readDouble();
     * \Exceptions: Same as readFully
     */
     DataInputStream& operator>>(double&);
    /**
     * Overload of Read-From-Stream operator for enum types.
     * This function acts as if by e = readEnum<E>();
     * \Exceptions: Same as readFully
     */
    template<typename E> std::enable_if_t<std::is_enum_v<E>,DataInputStream&> operator>>(E& e){
    	std::underlying_type_t<E> val;
    	*this >> val;
    	e = E{val};
    	return *this;
    }
};

/**
 * Base class for Binary Output Streams.
 * The class uses the RAII model for streams (and is therefore Move-only),
 * that is, any resources used by the stream are acquired at construction,
 * and released at destruction.
 * Subclasses are allowed to implement this interface in any manor which satisfies the requirements.
 * In particular, subclasses are allowed to buffer written stream data until flush is called or it is destroyed
 * If automatically committed writes are required, consult the specific subclass.
 * If the specific subclass is unknown, then flush should be called when the timing of write is mandated.
 * If a subclass of OutputStream does buffer data, then it is unspecified when this data is written
 * except if flush is called or stream is destroyed. (In which case any buffered data must be immediately written).
 */
class LIBLCAPI OutputStream{
private:
    OutputStream(const OutputStream&)=delete;
    OutputStream& operator=(const OutputStream&)=delete;
    void* operator new(std::size_t)=delete;
    void operator delete(void*)=delete;
protected:
    /**
	 * Destroys the OutputStream.
	 * This should free any resources used, and if the stream is buffered, the buffer must be flushed.
	 * If this causes an exception to be thrown, it may not propagate outside of the destructor.
	 * If this occurs the stream must be placed in a valid but unspecified state as described by flush,
	 * the buffer may or may not be flushed if this occurs however. It is unspecified how or if
	 * it is noted that such an error occurred.
	 */
    ~OutputStream()=default;
public:
    OutputStream()=default;
    OutputStream(OutputStream&&)=default;


    /**
     * Writes an object to the stream, then returns the number of byte written.
     * It is undefined behavior to write any object, unless the object is a TriviallyCopyable type
     * that is not a Pointer (including Pointer to Member or Function Pointer type), or a type which contains
     * non-static members of any pointer type (including pointer to member or function pointer type), or Reference type, or any virtual functions
     * There is also addition restrictions on reading objects (See InputStream#read)
     * If multibyte scalar types are written it is implementation defined whether they are written in Big-Endian or Little-Endian byte order.
     * If byte order is necessary, these types should be written using the equivalent method in DataOuputStream
     * If there is not a complete object of any type at the given pointer the behavior is undefined.
     * Subclasses may impose additional restrictions on what can be written,
     * including what causes undefined behavior and what throws an exception.
     * If an exception is thrown by the subclass, and stream buffering is used,
     * it is guaranteed that the stream will be reversed to the point where the buffer was last written,
     * (Possibly to where the buffer was before the call to write).
     * If the subclass does not use stream buffering, what data has been written after an exception is thrown is unspecified.
     */
    virtual size_t write(const void*,size_t)=0;
    /*
     * Writes a single byte to the stream.
     */
    virtual void write(uint8_t)=0;
    /**
     * Writes an array of bytes to a stream.
     * This function acts as if by write(arr,N)
     */
    template<typename T,size_t N,typename=std::enable_if_t<is_byte_v<T>>> size_t write(const T(&arr)[N]){
        return write(arr,N);
    }
    template<typename T,size_t N,typename=std::enable_if_t<is_byte_v<T>>> size_t write(const std::array<T,N>& arr){
		return write(arr.data(),N);
	}
    OutputStream& operator=(OutputStream&&)=default;

    /**
     * Flushes the stream's buffer if it exists. Otherwise has no effect.
     * If subclasses buffer output data, it is required that this function be implemented
     * to write all data that was buffered.
     * In addition, subclasses which buffer output data are required to flush the
     * buffer as though by this method when the stream is closed (by the destructor)
     * If the implementation throws an exception the stream is guaranteed to be reversed,
     * to the point where the buffer was last committed.
     * The state of the underlying buffer after an exception is thrown is unspecified.
     */
     virtual void flush();

     virtual bool checkError()const noexcept(true)=0;
     virtual void clearError()noexcept(true)=0;
     explicit operator bool()const noexcept(true);
     bool operator!()const noexcept(true);

};

class LIBLCAPI FilterOutputStream:public OutputStream{
private:
	OutputStream* underlying;
protected:
	~FilterOutputStream();
public:
	 FilterOutputStream(OutputStream&);
	 size_t write(const void*,size_t);
	 void write(uint8_t);
	 void flush();
	 bool checkError()const noexcept(true);
	 void clearError()noexcept(true);
};
/**
 * Tag type for disambugating appending constructors of FileOutputStream
 */
class append_t{
public:
    constexpr explicit append_t()=default;
};

const append_t append{};

/**
 * An OutputStream which wraps (and manages) a FILE*.
 * FileOutputStream is a buffered stream, that is, when bytes are written,
 * they are not committed to the underlying file descriptor immediately.
 * Instead they are placed in an internal buffer of an unspecified size.
 * Written bytes are committed when the buffer fills, flush is called,
 * or the stream is destroyed.
 */
class LIBLCAPI FileOutputStream:public OutputStream{
private:
    FILE* underlying;
public:
    /**
     * Constructs a new stream from an existing file.
     * The behavior is undefined if the given FILE is not NULL or nullptr, and it was not obtained from fopen,
     * or was opened in read-only mode.
     * The behavior is unspecified(and may be undefined), if the FILE was not opened in binary write mode (fopen(<name>,"wb") or fopen(<name>,"ab"))
     * \Exceptions: If the passed file is a null pointer, then a FileNotFoundException is thrown.
     */
     FileOutputStream(FILE*);
    /**
     * Constructs a new stream from a file with the given name.
     * The Stream is constructed as if by FileOutputStream(fopen(str,"wb"))
     * The behavior is undefined if the pointer is null
     * \Exceptions: If the passed file cannot be opened or does not exist and cannot be created, a FileNotFoundException is thrown
     */
     FileOutputStream(const char*);
    /**
     * Constructs a new stream from a file with the given name.
     * The stream is constructed as if by FileOutputStream(fopen(str.c_str(),"wb"))
     * \Exceptions: If the passed file cannot be opened or does not exist and cannot be created, a FileNotFoundException is thrown
     */
     FileOutputStream(const std::string&);

#ifdef USE_PATH_CTORS
     /**
     * Constructs a new stream from a path.
     * The stream is constructed as if by FileOutputStream(fopen(p.c_str(),"wb"))
     * \Exceptions: If the file cannot be opened in write mode (does not exist and cannot be created), or you have insufficient permission, a FileNotFoundException is throw
     */
    FileOutputStream(const std::filesystem::path&);
#endif
    /**
     * Constructs a new output stream with the given name, in append mode.
     * The Stream is constructed as if by FileOutputStream(fopen(str,"ab"))
     * \Exceptions: If the passed file cannot be opened or does not exist and cannot be created, a FileNotFoundException is thrown
     */
     FileOutputStream(const char*,append_t);
    /**
     * Constructs a new stream from a file with the given name, in append mode
     * The stream is constructed as if by FileOutputStream(fopen(str.c_str(),"ab"))
     * \Exceptions: If the passed file cannot be opened or does not exist and cannot be created, a FileNotFoundException is thrown
     */
     FileOutputStream(const std::string&,append_t);
#ifdef USE_PATH_CTORS
        /**
     * Constructs a new stream from a path, in append mode
     * The stream is constructed as if by FileOutputStream(fopen(p.c_str(),"ab"))
     * \Exceptions: If the file cannot be opened in write mode (does not exist and cannot be created), or you have insufficient permission, a FileNotFoundException is throw
     */
    FileOutputStream(const std::filesystem::path&,append_t);
#endif

    /**
     * Moves an existing FileOutputStream to this.
     * The underlying file of the moved-from object is reused by this object.
     * The old stream no longer has an underlying file after this call.
     */
     FileOutputStream(FileOutputStream&&);
    /**
     * Destroys this stream.
     * If this has an underlying file, that file is closed
     */
     ~FileOutputStream(); // @suppress("Class has a virtual method and non-virtual destructor")
    /**
     * Moves an existing FileOutputStream.
     * If this has an underlying file, that file is closed.
     * Then this acts as though a new FileOutputStream is created via the move constructor
     */
     FileOutputStream& operator=(FileOutputStream&&);
    /**
     * Writes to the file.
     * This is implemented as if by fwrite(ptr,1,size,underlying);
     * \Exception Guarantee: This method will not throw an exception
     */
     size_t write(const void*,size_t);
    /**
     * Writes a single byte to the file.
     * This acts as if implemented as write(&b,1);
     * \Exception Guarantee: This method will not throw an exception
     */
     void write(uint8_t);

    /**
     * Flushes the bytes written to the stream.
     */
     void flush();

    /**
     * Obtains the underlying file, or a null pointer if the underlying file has been moved. 
     */
     FILE* getUnderlying()const noexcept(true);
     bool checkError()const noexcept(true);
     void clearError()noexcept(true);
};

/**
 * A Wrapper type on a basic OutputStream which allows safe, byte-order sensitive
 * writing of primitive types.
 * When in BigEndian mode this class is compatible with Java's java.io.DataOuputStream, in both interface and implementation.
 * Data written by such a stream can be read by Java's DataInputStream.
 * The behavior is undefined if the wrapped OutputStream's lifetime ends
 * before this object's, even if no write methods are called.
 * DataOutputStream does not buffer it's writes, though the underlying stream may.
 * flush is overridden as a convenience method for underlying->close();
 * The destructor of DataOutputStream will NOT flush the underlying stream.
 */
class LIBLCAPI DataOutputStream final:public FilterOutputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
private:
    bool little;
public:
    /**
     * Constructs a new DataOutputStream wrapping the given OutputStream.
     * Multibyte types are written in Big Endian (network) byte order
     */
     DataOutputStream(OutputStream&);
    /**
     * Constructs a new DataOutputStream wrapping the given OutputStream, in little endian mode.
     * Multibyte types are written in Little Endian (native) byte order
     */
     DataOutputStream(OutputStream&,little_endian_t);

    /**
     * Writes a single byte to the stream.
     * The byte written here is suitable to be read by any readSignedByte or readUnsignedByte call in a DataInputStream (or similar utility)
     * \Exceptions: same as write
     */
     void writeByte(int8_t);
    /**
	 * Writes a 2-byte short to the stream.
	 * The bytes written here are suitable to be read by any readSignedShort or readUnsignedShort call in a DataInputStream in the same byte-order mode
	 * \Exceptions: same as write
	 */
     void writeShort(int16_t);
    /**
	 * Writes a 4-byte int to the stream.
	 * The bytes written here are suitable to be read by any readInt call in a DataInputStream in the same byte-order mode
	 * \Exceptions: same as write
	 */
     void writeInt(int);
    /**
	 * Writes a 8-byte long to the stream.
	 * The bytes written here are suitable to be read by any readLong call in a DataInputStream in the same byte-order mode
	 * \Exceptions: same as write
	 */
     void writeLong(int64_t);
    /**
	 * Writes a string to the stream. The length prefix is written as an unsigned short as if by writeShort,
	 * then that many bytes starting from std::begin(str) are written.
	 * The behavior is undefined if any byte sequence in the string forms an invalid utf-8 code-point,
	 * or the string contains embedded nul bytes.
	 * \Exceptions: same as write
	 */
     void writeString(const std::string&);
#if defined(__cpp_char8_t)&&__cpp_char8_t>=201811L
     void writeString(const std::u8string&);
#endif
    /**
     * Writes a 4-byte float to the stream.
     * The float is interpreted as a 4-byte value, then written as if by writeInt, except that it cannot be read by readInt.
     * If the default float on the implementation is not IEEE binary32, the behavior is undefined.
     * If the written value is NaN it is implementation defined if the bit representation of the NaN is preserved.
     */
     void writeFloat(float);
    /**
	 * Writes a 8-byte double to the stream.
	 * The double is interpreted as a 8-byte value, then written as if by writeLong, except that it cannot be read by readLong.
	 * If the default float on the implementation is not IEEE binary64, the behavior is undefined.
	 * If the written value is NaN it is implementation defined if the bit representation of the NaN is preserved.
	 */
     void writeDouble(double);
    /**
     * Writes an enum to the stream, as if by invoking the correct write<T> for the underlying type of E,
     * This enum is only suitable to be read by an equivalent readEnum call
     * in DataInputStream.
     * Note that this method is not compatible with any interfaces, except for this one
     */
    template<typename E> std::enable_if_t<std::is_enum_v<E>> writeEnum(E e){
    	*this << static_cast<std::underlying_type_t<E>>(e);
    }
     DataOutputStream& operator<<(uint8_t);
     DataOutputStream& operator<<(int8_t);
     DataOutputStream& operator<<(uint16_t);
     DataOutputStream& operator<<(int16_t);
     DataOutputStream& operator<<(uint32_t);
     DataOutputStream& operator<<(int32_t);
     DataOutputStream& operator<<(uint64_t);
     DataOutputStream& operator<<(int64_t);
     DataOutputStream& operator<<(const std::string&);

#if defined(__cpp_char8_t)&&__cpp_char8_t>201811L
     DataOutputStream& operator<<(const std::u8string&);
#endif
    /**
     * Writes a given UUID to the stream, by writing the high then the low bits as if by writeLong
     */
     DataOutputStream& operator<<(const UUID&);
    /**
     * Writes a given Version to the stream, as if by writing the encoded form of the version with writeShort in big-endian byte order
     */
     DataOutputStream& operator<<(Version);
     DataOutputStream& operator<<(float);
     DataOutputStream& operator<<(double);
    template<typename E> DataOutputStream& operator<<(E e){
    	return *this << static_cast<std::underlying_type_t<E>>(e);
    }


};

class LIBLCAPI ByteArrayInputStream final:public InputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
private:
	const std::byte* buffer;
	std::size_t bufferSize;
	std::size_t bufferPosition;
public:
	template<typename byte,typename=std::enable_if_t<is_byte_v<byte>>>
		ByteArrayInputStream(const byte* buff,std::size_t sz):buffer{reinterpret_cast<const std::byte*>(buff)},bufferSize{sz},
		bufferPosition{0}{}
	template<typename byte,std::size_t N,typename=std::enable_if_t<is_byte_v<byte>>>
		ByteArrayInputStream(const byte(&arr)[N]):buffer{reinterpret_cast<const std::byte*>(arr)},bufferSize{N},
		bufferPosition{0}{}
	template<typename byte,std::ptrdiff_t extent,typename=std::enable_if_t<is_byte_v<byte>>>
			ByteArrayInputStream(span<const byte,extent> s):buffer{reinterpret_cast<const std::byte*>(s.data())},bufferSize{s.size()},
			bufferPosition{0}{}

	std::size_t read(void*,std::size_t);
	int read();
	bool checkError()const noexcept(true);
	void clearError()noexcept(true);
};
#include <vector>
class LIBLCAPI ByteArrayOutputStream final:public OutputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
private:
	std::vector<std::byte> buffer;
public:
	std::size_t write(const void*,std::size_t);
	void write(uint8_t);
	span<const std::byte> getBuffer()const;
	bool checkError()const noexcept(true);
	void clearError()noexcept(true);
};

class LIBLCAPI NullDeviceOutputStream final:public OutputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
public:
	std::size_t write(const void*,std::size_t);
	void write(uint8_t);
	bool checkError()const noexcept(true);
	void clearError()noexcept(true);
};

class LIBLCAPI NullDeviceInputStream final:public InputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
public:
	std::size_t read(void*,std::size_t);
	int read();
	bool checkError()const noexcept(true);
	void clearError()noexcept(true);
};

class LIBLCAPI ZeroDeviceInputStream final:public InputStream{ // @suppress("Class has a virtual method and non-virtual destructor")
public:
	std::size_t read(void*,std::size_t);
	int read();
	bool checkError()const noexcept(true);
	void clearError()noexcept(true);
};





#undef USE_PATH_CTORS

#endif
