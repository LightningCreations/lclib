/*
 * SocketCommon.cpp
 *
 *  Created on: Sep 5, 2018
 *      Author: connor
 */
#include <Socket.hpp>
#include <IOWrapper.hpp>
#include <memory>

const char* ConnectionException::what()const noexcept(true){
	return "Connection Error: No other information";
}
const char* AddressBindException::what()const noexcept(true){
	return "Connection Error: Could not bind to address";
}

const char* TimedOutException::what()const noexcept(true){
	return "Connection Timed Out";
}

const char* UnknownHostException::what()const noexcept(true){
	return "Unable to resolve hostname";
}

const char* SocketConcurrencyException::what()const noexcept(true){
	return "Error using multi-threading with this Socket";
}
const char* SocketOperationUnsupportedException::what()const noexcept(true){
	return "The requested operation is unsupported or forbidden";
}

Socket::Socket(std::unique_ptr<SocketImpl>&& impl):impl(impl){}
Socket::Socket():impl(newSocketHandle()){}
Socket::Socket(const std::string& host,uint16_t port):impl(newSocketHandle()){
	impl->connect(host,port);
}

InputStream& Socket::getInputStream(){
	return impl->getInputStream();
}
OutputStream& Socket::getOutputStream(){
	return impl->getOutputStream();
}

void Socket::connect(const std::string& host,uint16_t port){
	impl->connect(host,port);
}

bool Socket::isConnected()const{
	return impl->isConnected();
}

const std::string& Socket::getHostname()const{
	return impl->getHostname();
}

const std::string& Socket::getConnectedName()const{
	return impl->getConnectedName();
}

uint16_t Socket::getConnectedPort()const{
	return impl->getPort();
}

ServerSocket::ServerSocket():impl(newServerSocketHandle()){}
ServerSocket::ServerSocket(const std::string& host,uint16_t port):impl(newServerSocketHandle()){
	impl->bind(host,port);
}
ServerSocket::ServerSocket(std::unique_ptr<ServerSocketImpl>&& impl):impl(impl){}


Socket ServerSocket::accept(){
	return Socket{impl->accept()};
}

void ServerSocket::bind(const std::string& host,uint16_t port){
	impl->bind(host,port);
}

InputStream& ServerSocket::getInputStream(){
	return impl->getInputStream();
}
OutputStream& ServerSocket::getOutputStream(){
	return impl->getOutputStream();
}



