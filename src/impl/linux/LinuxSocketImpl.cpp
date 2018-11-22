/*
 * LinuxSocketImpl.cpp
 *
 *  Created on: Sep 5, 2018
 *      Author: connor
 */
extern "C"{
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
};
#include <cstdio>
#include <lclib-cxx/Socket.hpp>
#include <lclib-cxx/IOWrapper.hpp>
#include <mutex>
#include <atomic>
#include <chrono>
#include <thread>
#include <lclib-cxx/Types.hpp>
#include <cstring>
#include <cstdlib>
#include <lclib-cxx/Callable.hpp>
extern int errno;
using namespace std::chrono_literals;
typedef decltype(socket(0,0,0)) socket_t;



/**
 * Linux Sockets may not support multithreading in socket operations (except read/write operations),
 * So Only one connect, bind, or update operation can be preformed at once.
 */
std::recursive_mutex globalSocketLock;

const auto dur = std::chrono::duration_cast<std::chrono::system_clock::duration>(2s+500ms);
const auto baseDur = std::chrono::duration_cast<std::chrono::system_clock::duration>(5s);
const auto conDur = std::chrono::duration_cast<std::chrono::system_clock::duration>(10s);
class SocketImplInputStream:public InputStream{
private:
	bool err;
	std::recursive_mutex* lock;
	socket_t socket;
public:
	SocketImplInputStream(socket_t sock,std::recursive_mutex& lock):socket(sock),
		lock(&lock),err(false){}
	size_t read(void* ptr,size_t size){
		std::lock_guard<std::recursive_mutex> sync(*lock);
		ssize_t retSize;
		if((retSize =recv(socket,ptr,size,0)<0))
			err=true,throw ConnectionException();
		return retSize;
	}
	int read(){
		std::lock_guard<std::recursive_mutex> sync(*lock);
		uint8_t val;
		if(read(&val,sizeof(val))==EOF)
			return -1;
		return val;
	}
	bool checkError()const noexcept(true){
		return err;
	}
	void clearError()noexcept(true){
		err= false;
	}
};

class SocketImplOutputStream:public OutputStream{
private:
	bool err;
	socket_t socket;
	std::recursive_mutex* lock;
public:
	SocketImplOutputStream(socket_t socket,std::recursive_mutex& lock):socket(socket),
	lock(&lock),err(false){}
	size_t write(const void* ptr,size_t size){
		std::lock_guard<std::recursive_mutex> sync(*lock);
		ssize_t retSize;
		if((retSize =send(socket,ptr,size,0))<0)
			err=true,throw ConnectionException();
		return retSize;
	}
	void write(uint8_t b){
		write(&b,sizeof(b));
	}
	bool checkError()const noexcept(true){
		return err;
	}
	void clearError()noexcept(true){
		err= false;
	}
};

/**
 * A Socket Handle which wraps a Linux/Unix Socket.
 * The Socket is connected using IP Family, using TCP Protocol.
 * The Implemenation is Default Conforming (valid as a default socket handler), if it is default constructed.
 */
class LinuxSocketImpl:public SocketImpl{
private:
	std::string hostname="127.0.0.1";
	std::string connectedName;
	uint16_t port;
	socket_t sock;
	std::recursive_mutex lock;
	SocketImplInputStream in;
	SocketImplOutputStream out;
	bool connected;
public:
	LinuxSocketImpl(socket_t sock,bool connected):sock(sock),in(sock,lock),out(sock,lock),connected(connected){}
	LinuxSocketImpl(socket_t sock):LinuxSocketImpl(sock,false){}
	LinuxSocketImpl():LinuxSocketImpl(socket(AF_INET,SOCK_STREAM,6)){
	}
	~LinuxSocketImpl(){
		shutdown(sock,SHUT_RDWR);
		std::lock_guard<std::recursive_mutex> await(lock);//If any streams are in use, block until they complete or error
	}
	void connect(const std::string& host,uint16_t port){
		std::lock_guard<std::recursive_mutex> gsync(globalSocketLock);
		std::lock_guard<std::recursive_mutex> sync(lock);
		char buff[6];//Size is known, max 5 characters + NUL
		struct addrinfo* info;
		int err;
		sprintf(buff,"%u",port);//May warn, but known to be safe
		if((err=getaddrinfo(host.c_str(),buff,NULL,&info))){
			if(err==EAI_NONAME)
				throw UnknownHostException();
		}
		try{
			connectedName = info->ai_canonname;
			this->port = port;
			::connect(sock,info->ai_addr,info->ai_addrlen);
			if(err&&errno!=EINPROGRESS){
				if(errno==ETIMEDOUT)
					throw TimedOutException();
				else if(errno==ENETUNREACH||errno==ECONNREFUSED||errno==EPROTOTYPE)
					throw UnknownHostException();
			}
			connected = true;
			freeaddrinfo(info);
		}catch(const SocketException&){//Catch any valid exception and clean up the info, then rethrow
			freeaddrinfo(info);
			throw;
		}
	}
	InputStream& getInputStream(){
		return in;
	}
	OutputStream& getOutputStream(){
		return out;
	}
	bool isConnected()const noexcept(true){
		return connected;
	}
	const std::string& getHostname()const noexcept(true){
		return hostname;
	}
	const std::string& getConnectedName()const{
		return connectedName;
	}
	uint16_t getPort()const{
		return port;
	}
};

struct LinuxServerSocketImpl:public ServerSocketImpl{
	socket_t sock;
	std::recursive_mutex lock;
	bool bound;
public:
	LinuxServerSocketImpl(socket_t sock):sock(sock),bound(false){}
	LinuxServerSocketImpl():LinuxServerSocketImpl(socket(AF_INET,SOCK_STREAM,6)){}
	~LinuxServerSocketImpl(){
		shutdown(sock,SHUT_RDWR);
	}
	void bind(const std::string& host,uint16_t port){
		std::lock_guard<std::recursive_mutex> gsync(globalSocketLock);
		std::lock_guard<std::recursive_mutex> sync(lock);
		char buff[6];//Size is known, max 5 characters + NUL
		struct addrinfo* info;
		int err;
		sprintf(buff,"%u",port);//May warn, but known to be safe
		if((err=getaddrinfo(host.c_str(),buff,NULL,&info))){
			if(err==EAI_NONAME)
				throw UnknownHostException();
		}
		try{
			::bind(sock,info->ai_addr,info->ai_addrlen);
			if(err&&errno!=EINPROGRESS){
				if(errno==ETIMEDOUT)
					throw TimedOutException();
				else if(errno==ENETUNREACH||errno==ECONNREFUSED||errno==EPROTOTYPE)
					throw UnknownHostException();
			}
			listen(sock,128);
			freeaddrinfo(info);
		}catch(const SocketException&){//Catch any valid exception and clean up the info, then rethrow
			freeaddrinfo(info);
			throw;
		}
	}
	std::unique_ptr<SocketImpl> accept(){
		std::lock_guard<std::recursive_mutex> sync(lock);
		socket_t sock = ::accept(sock,NULL,NULL);
		return std::make_unique<LinuxSocketImpl>(sock,true);
	}
	InputStream& getInputStream(){
		throw SocketOperationUnsupportedException();
	}
	OutputStream& getOutputStream(){
		throw SocketOperationUnsupportedException();
	}
	bool isBound()const noexcept(true){
		return bound;
	}
};

function<std::unique_ptr<SocketImpl>()> sockGeneratorFunction = std::make_unique<LinuxSocketImpl>;
function<std::unique_ptr<ServerSocketImpl>()>  serverGeneratorFunction = std::make_unique<LinuxServerSocketImpl>;
void setDefaultSocketHandlerConstructor(function<std::unique_ptr<SocketImpl>()> generator){
	std::lock_guard<std::recursive_mutex> sync(globalSocketLock);
	sockGeneratorFunction = std::move(generator);
}

void setDefaultServerSocketHandlerConstructor(function<std::unique_ptr<ServerSocketImpl>()> generator){
	std::lock_guard<std::recursive_mutex> sync(globalSocketLock);
	serverGeneratorFunction = std::move(generator);
}

std::unique_ptr<SocketImpl> newSocketHandle(){
	std::lock_guard<std::recursive_mutex> sync(globalSocketLock);
	return sockGeneratorFunction();
}
std::unique_ptr<ServerSocketImpl> newServerSocketHandle(){
	std::lock_guard<std::recursive_mutex> sync(globalSocketLock);
	return serverGeneratorFunction();
}




