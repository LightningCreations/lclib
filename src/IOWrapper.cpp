#include <IOWrapper.hpp>
#include <string>

FileInputStream::FileInputStream(FILE* f):underlying(f){
    if(f==NULL||f==nullptr)
        throw FileNotFoundException();
}
FileInputStream::FileInputStream(const char* c):FileInputStream(fopen(c,"rb")){}
FileInputStream::FileInputStream(const std::string& str):FileInputStream(fopen(str.c_str(),"r")){}
FileInputStream::~FileInputStream(){
    if(underlying!=nullptr&&underlying!=NULL)
        fclose(underlying);
}

FileInputStream::FileInputStream(FileInputStream&& f):underlying(std::exchange(f.underlying,nullptr)){}
FileInputStream& FileInputStream::operator=(FileInputStream&& f){
    if(underlying!=nullptr&&underlying!=NULL)
        fclose(underlying);
    underlying = std::exchange(f.underlying,nullptr);
}
int FileInputStream::read(){
    uint8_t c;
    if(read(&c,1)==-1)
        return -1;
    return c;
}

size_t FileInputStream::read(void* ptr,size_t size){
    return fwrite(ptr,1,size,underlying);
}

DataInputStream::DataInputStream(InputStream& i):little(false),underlying(&i){}
DataInputStream::DataInputStream(InputStream& i,little_endian_t):little(true),underlying(&i){}

size_t DataInputStream::read(void* ptr,size_t size){
    return underlying->read(ptr,size);
}

int DataInputStream::read(){
    return underlying->read();
}

uint8_t DataInputStream::readUnsignedByte(){
    return read();
}
int8_t DataInputStream::readSignedByte(){
    return read();
}
uint16_t DataInputStream::readUnsignedShort(){
    if(little)
        return read()|read()<<8;
    return read()<<8|read();
}
int16_t DataInputStream::readSignedShort(){
    return readUnsignedShort();
}
int DataInputStream::readInt(){
    if(little)
        return read()|read()<<8|read()<<16|read()<<24;
    return read()<<24|read()<<16|read()<<8|read();
}
int64_t DataInputStream::readLong(){
    if(little)
        return readInt()|int64_t(readInt())<<32;
    return int64_t(readInt())<<32|readInt();
}
std::string DataInputStream::readString(){
    uint16_t len = this->readUnsignedShort();
    char* str = new char[len];
    read(str,len);
    string ret(str,len);
    delete[] str;
    return std::move(ret);
}
DataInputStream& DataInputStream::operator>>(uint8_t& u){
    u = this->readUnsignedByte();
    return *this;
}
DataInputStream& DataInputStream::operator>>(int8_t&u){
    u = this->readSignedByte();
    return *this;
}
DataInputStream& DataInputStream::operator>>(uint16_t& u){
    u = this->readUnsignedShort();
    return *this;
}
DataInputStream& DataInputStream::operator>>(int16_t& u){
    u = this->readSignedShort();
    return *this;
}
DataInputStream& DataInputStream::operator>>(uint32_t& u){
    u = this->readInt();
    return *this;
}
DataInputStream& DataInputStream::operator>>(int32_t& u){
    u = this->readInt();
    return *this;
}
DataInputStream& DataInputStream::operator>>(uint64_t& u){
    u = this->readLong();
    return *this;
}
DataInputStream& DataInputStream::operator>>(int64_t& u){
    u = this->readLong();
    return *this;
}
DataInputStream& DataInputStream::operator>>(std::string& u){
    u = this->readString();
    return *this;
}
DataInputStream& DataInputStream::operator>>(Version& v){
    uint16_t a = readUnsignedShort();
    v = a;
    return *this;
}
DataInputStream& DataInputStream::operator>>(UUID& u){
    uint64_t h,l;
    (*this) >> h >> l;
    u = UUID(h,l);
    return *this;
}

FileOutputStream::FileOutputStream(FILE* f):underlying(f){
    if(underlying==NULL||underlying==nullptr)
        throw FileNotFoundException();
}
FileOutputStream::FileOutputStream(const char* c):FileOutputStream(fopen(c,"wb")){}
FileOutputStream::FileOutputStream(const std::string& str):FileOutputStream(fopen(str.c_str(),"wb")){}
FileOutputStream::FileOutputStream(const char* c,append_t):FileOutputStream(fopen(c,"ab")){}
FileOutputStream::FileOutputStream(const std::string& str,append_t):FileOutputStream(fopen(str.c_str(),"ab")){}
FileOutputStream::FileOutputStream(FileOutputStream&& f):underlying(std::exchange(f.underlying,nullptr)){}
FileOutputStream::~FileOutputStream(){
    if(underlying!=NULL&&underlying!=nullptr)
        fclose(underlying);
}
FileOutputStream& FileOutputStream::operator=(FileOutputStream&& f){
    if(underlying!=NULL&&underlying!=nullptr)
        fclose(underlying);
    underlying = std::exchange(f.underlying,nullptr);
    return *this;
}
size_t FileOutputStream::write(const void* ptr,size_t size){
    return fwrite(ptr,1,size,underlying);
}
void FileOutputStream::write(uint8_t u){
    write(&u,1);
}

DataOutputStream::DataOutputStream(OutputStream& o):underlying(&o),little(false){}
DataOutputStream::DataOutputStream(OutputStream& o,little_endian_t):underlying(&o),little(true){}
size_t DataOutputStream::write(const void* v,size_t s){
    return underlying->write(v,s);
}
void DataOutputStream::write(uint8_t u){
    underlying->write(u);
}
void DataOutputStream::writeByte(int8_t i){
    write(i);
}
void DataOutputStream::writeShort(int16_t i){
    if(little){
        write(i);
        write(i>>8);
    }
    else{
        write(i>>8);
        write(i);
    }
}
void DataOutputStream::writeInt(int i){
    if(little){
        write(i);
        write(i>>8);
        write(i>>16);
        write(i>>24);
    }else{
        write(i>>24);
        write(i>>16);
        write(i>>8);
        write(i);
    }
}
void DataOutputStream::writeLong(int64_t l){
    if(little){
        writeInt(l);
        writeInt(l>>32);
    }else{
        writeInt(l>>32);
        writeInt(l);
    }
}
void DataOutputStream::writeString(const std::string& s){
    writeShort(s.length());
    for(char c:s)
        write(c);
}
DataOutputStream& DataOutputStream::operator<<(uint8_t i){
    write(i);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(int8_t i){
    write(i);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(uint16_t l){
    writeShort(l);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(int16_t l){
    writeShort(l);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(uint32_t i){
    writeInt(i);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(int32_t i){
    writeInt(i);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(uint64_t l){
    writeLong(l);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(int64_t l){
    writeLong(l);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(const std::string& s){
    writeString(s);
    return *this;
}
DataOutputStream& DataOutputStream::operator<<(const UUID& u){
    return (*this) << u.getHigh() << u.getLow();
}
DataOutputStream& DataOutputStream::operator<<(Version v){
    return (*this) << uint8_t(v.getMajor()-1) << uint8_t(v.getMinor());
}

const char* FileNotFoundException::what()const noexcept(true){
    return "File could not be found or opened";
}