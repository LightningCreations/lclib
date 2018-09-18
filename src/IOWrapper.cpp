#include <IOWrapper.hpp>
#include <string>
#include <Vector.hpp>
#include <reflect/ReflectionInfo.hpp>

export_type(InputStream)
export_type(FileInputStream)
export_type(FilterInputStream)
export_type(DataInputStream)
export_type(OutputStream)
export_type(FileOutputStream)
export_type(FilterOutputStream)
export_type(DataOutputStream)
export_type(little_endian_t)
export_type(append_t)
export_type(FileNotFoundException)
export_type(EOFException)
export_field(little_endian)
export_field(append)

FileInputStream::FileInputStream(FILE* f):underlying(f){
    if(f==NULL||f==nullptr)
        throw FileNotFoundException();
}
FileInputStream::FileInputStream(const char* c):FileInputStream(fopen(c,"rb")){}
FileInputStream::FileInputStream(const std::string& str):FileInputStream(fopen(str.c_str(),"rb")){}
FileInputStream::FileInputStream(const std::filesystem::path& p):FileInputStream(fopen(p.c_str(),"rb")){}
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

FilterInputStream::FilterInputStream(InputStream& i):underlying(&i){}
FilterInputStream::~FilterInputStream(){}

size_t FilterInputStream::read(void* ptr,size_t size){
	return underlying->read(ptr,size);
}
int FilterInputStream::read(){
	return underlying->read();
}

DataInputStream::DataInputStream(InputStream& i):FilterInputStream(i),little(false){}
DataInputStream::DataInputStream(InputStream& i,little_endian_t):FilterInputStream(i),little(true){}



int DataInputStream::readSingle(){
	int ret = underlying->read();
	if(ret<0)
		throw EOFException();
	return ret;
}

void DataInputStream::readFully(void* ptr,size_t size){
	if(read(ptr,size)!=size)
		throw EOFException();
}

uint8_t DataInputStream::readUnsignedByte(){
	return readSingle();
}
int8_t DataInputStream::readSignedByte(){
    return readUnsignedByte();
}
uint16_t DataInputStream::readUnsignedShort(){
    if(little)
        return readSingle()|readSingle()<<8;
    return readSingle()<<8|readSingle();
}
int16_t DataInputStream::readSignedShort(){
    return readUnsignedShort();
}
int DataInputStream::readInt(){
    if(little)
        return readSingle()|readSingle()<<8|readSingle()<<16|readSingle()<<24;
    return readSingle()<<24|readSingle()<<16|readSingle()<<8|readSingle();
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
float DataInputStream::readFloat(){
	int i = readInt();
	return bit_cast<float>(i);
}
double DataInputStream::readDouble(){
	uint64_t l = readLong();
	return bit_cast<double>(l);
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
    uint8_t M,m;
    *this >> M >> m;
    v = Version(int(M)+1,m);
    return *this;
}
DataInputStream& DataInputStream::operator>>(UUID& u){
    uint64_t h,l;
    (*this) >> h >> l;
    u = UUID(h,l);
    return *this;
}
DataInputStream& DataInputStream::operator>>(float& f){
	f = readFloat();
	return *this;
}
DataInputStream& DataInputStream::operator>>(double& d){
	d = readDouble();
	return *this;
}

void OutputStream::flush(){}

FileOutputStream::FileOutputStream(FILE* f):underlying(f){
    if(underlying==NULL||underlying==nullptr)
        throw FileNotFoundException();
}
FileOutputStream::FileOutputStream(const char* c):FileOutputStream(fopen(c,"wb")){}
FileOutputStream::FileOutputStream(const std::string& str):FileOutputStream(fopen(str.c_str(),"wb")){}
FileOutputStream::FileOutputStream(const std::filesystem::path& p):FileOutputStream(fopen(p.c_str(),"wb")){}
FileOutputStream::FileOutputStream(const char* c,append_t):FileOutputStream(fopen(c,"ab")){}
FileOutputStream::FileOutputStream(const std::string& str,append_t):FileOutputStream(fopen(str.c_str(),"ab")){}
FileOutputStream::FileOutputStream(const std::filesystem::path& p,append_t):FileOutputStream(fopen(p.c_str(),"ab")){}
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

void FileOutputStream::flush(){
	fflush(underlying);
}

FilterOutputStream::FilterOutputStream(OutputStream& o):underlying(&o){}
FilterOutputStream::~FilterOutputStream(){}
size_t FilterOutputStream::write(const void* ptr,size_t size){
	return underlying->write(ptr,size);
}
void FilterOutputStream::write(uint8_t val){
	underlying->write(val);
}
void FilterOutputStream::flush(){
	underlying->flush();
}

DataOutputStream::DataOutputStream(OutputStream& o):FilterOutputStream(o),little(false){}
DataOutputStream::DataOutputStream(OutputStream& o,little_endian_t):FilterOutputStream(o),little(true){}

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
void DataOutputStream::writeFloat(float f){
	writeInt(bit_cast<int>(f));
}
void DataOutputStream::writeDouble(double d){
	writeLong(bit_cast<uint64_t>(d));
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
DataOutputStream& DataOutputStream::operator<<(float f){
	writeFloat(f);
	return *this;
}
DataOutputStream& DataOutputStream::operator<<(double d){
	writeDouble(d);
	return *this;
}

std::size_t ByteArrayInputStream::read(void* vptr,size_t n){
	if(n>(bufferSize-bufferPosition)){
		memcpy(vptr,buffer+bufferPosition,(bufferSize-bufferPosition));
		return (bufferSize-bufferPosition);
	}
	memcpy(vptr,buffer+bufferPosition,n);
	bufferPosition+=n;
	return n;
}
int ByteArrayInputStream::read(){
	if(bufferSize==bufferPosition)
		return -1;
	else
		return static_cast<int>(buffer[bufferPosition++]);
}

std::size_t ByteArrayOutputStream::write(const void* vptr,size_t n){
	const std::size_t pos = buffer.size();
	buffer.reserve(pos+n);
	memcpy(buffer.data()+pos,vptr,n);
	return n;
}
void ByteArrayOutputStream::write(uint8_t u){
	buffer.push_back(static_cast<std::byte>(u));
}
const std::byte* ByteArrayOutputStream::getBuffer()const{
	return buffer.data();
}

const char* FileNotFoundException::what()const noexcept(true){
    return "File could not be found or opened";
}

const char* EOFException::what()const noexcept(true){
	return "End Of File Reached before completing read";
}
