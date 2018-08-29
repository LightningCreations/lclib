#ifndef __IOWrapper_hpp_2018_07_26_22_33
#define __IOWrapper_hpp_2018_07_26_22_33
#include <cstdio>
#include <cstdint>
#include <string>
#include <Version.hpp>
#include <UUID.hpp>
#include <stdexcept>
class FileNotFoundException:public std::exception{
public:
    const char* what()const noexcept(true) override;
};

struct little_endian_t{
public:
    constexpr explicit little_endian_t()=default;
};

const little_endian_t little_endian{};



class InputStream{
    InputStream(const InputStream&)=delete;
    InputStream& operator=(const InputStream&)=delete;
public:
    InputStream()=default;
    virtual ~InputStream()=default;
    virtual size_t read(void*,size_t)=0;
    virtual int read()=0;
    template<size_t N> size_t read(uint8_t(&arr)[N]){
        return read(arr,N);
    }
};

class FileInputStream:public InputStream{
private:
    FILE* underlying;
public:
    FileInputStream(FILE*);
    FileInputStream(const char*);
    FileInputStream(const std::string&);
    FileInputStream(FileInputStream&&);
    FileInputStream& operator=(FileInputStream&&);
    ~FileInputStream();
    size_t read(void*,size_t);
    int read();
};

class DataInputStream:public InputStream{
private:
    InputStream* underlying;
    bool little;
public:
    DataInputStream(InputStream&);
    DataInputStream(InputStream&,little_endian_t);
    size_t read(void*,size_t);
    int read();
    uint8_t readUnsignedByte();
    int8_t readSignedByte();
    uint16_t readUnsignedShort();
    int16_t readSignedShort();
    int readInt();
    int64_t readLong();
    std::string readString();
    DataInputStream& operator>>(uint8_t&);
    DataInputStream& operator>>(int8_t&);
    DataInputStream& operator>>(uint16_t&);
    DataInputStream& operator>>(int16_t&);
    DataInputStream& operator>>(uint32_t&);
    DataInputStream& operator>>(int32_t&);
    DataInputStream& operator>>(uint64_t&);
    DataInputStream& operator>>(int64_t&);
    DataInputStream& operator>>(std::string&);
    DataInputStream& operator>>(Version&);
    DataInputStream& operator>>(UUID&);
};

class OutputStream{
    OutputStream(const OutputStream&)=delete;
    OutputStream& operator=(const OutputStream&)=delete;
public:
    OutputStream()=default;
    virtual ~OutputStream()=default;
    virtual size_t write(const void*,size_t)=0;
    virtual void write(uint8_t)=0;
    template<size_t N> size_t write(uint8_t(&arr)[N]){
        return write(arr,N);
    }
};

class append_t{
public:
    constexpr explicit append_t()=default;
};

const append_t append{};

class FileOutputStream:public OutputStream{
private:
    FILE* underlying;
    bool __app;
public:
    FileOutputStream(FILE*);
    FileOutputStream(const char*);
    FileOutputStream(const std::string&);
    FileOutputStream(std::string&&);
    FileOutputStream(const char*,append_t);
    FileOutputStream(const std::string&,append_t);
    FileOutputStream(std::string&&,append_t);
    FileOutputStream(FileOutputStream&&);
    ~FileOutputStream();
    FileOutputStream& operator=(FileOutputStream&&);
    size_t write(const void*,size_t);
    void write(uint8_t);
};

class DataOutputStream:public OutputStream{
private:
    OutputStream* underlying;
    bool little;
public:
    DataOutputStream(OutputStream&);
    DataOutputStream(OutputStream&,little_endian_t);
    size_t write(const void*,size_t);
    void write(uint8_t);
    void writeByte(int8_t);
    void writeShort(int16_t);
    void writeInt(int);
    void writeLong(int64_t);
    void writeString(const std::string&);
    DataOutputStream& operator<<(uint8_t);
    DataOutputStream& operator<<(int8_t);
    DataOutputStream& operator<<(uint16_t);
    DataOutputStream& operator<<(int16_t);
    DataOutputStream& operator<<(uint32_t);
    DataOutputStream& operator<<(int32_t);
    DataOutputStream& operator<<(uint64_t);
    DataOutputStream& operator<<(int64_t);
    DataOutputStream& operator<<(const std::string&);
    DataOutputStream& operator<<(const UUID&);
    DataOutputStream& operator<<(Version);
};

#endif