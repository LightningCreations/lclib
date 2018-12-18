/*
 * WindowsSocketImpl.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: connor
 */
extern "C"{
#include <winsock2.h>
};
#include <lclib/Socket.hpp>
#include <lclib/Config.hpp>
#include <mutex>

class WSAError:public SocketException{
public:
	const char* what()const noexcept(true)override{
		return "WinSock API Raised an error condition.";
	}
};

std::recursive_mutex wsa_global_lock;
class WinSockInit{
	WSADATA dt;
public:
	WinSockInit(){
		std::lock_guard<std::recursive_mutex> sync(wsa_global_lock);

		if(WSAStartup(0x2020,&dt))
			throw WSAError();
	}
	~WinSockInit(){
		std::lock_guard<std::recursive_mutex> sync(wsa_global_lock);
		WSACleanup();
	}
};

class WSASocketImplBase{
protected:
	SOCKET sock;
	std::recursive_mutex lock;
	std::string targetAddr;
	WSASocketImplBase(SOCKET sock):sock(sock){
		static const WinSockInit initWSA;//Ensure that the first
	}
};

class WinSocketImpl:private WSASocketImplBase,public SocketImpl{
private:
};

