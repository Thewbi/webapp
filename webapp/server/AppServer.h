#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <sstream>

#define DEFAULT_BUFLEN 512
//#define DEFAULT_BUFLEN 1024

class AppServer
{
public:
	WSADATA wsaData_;
	std::string port_;
	struct addrinfo *pAddrInfo_;
	SOCKET listenSocket_;
	SOCKET clientSocket_;

public:
	AppServer(const std::string port);
	virtual ~AppServer();

	void determineAddressInfo();
	void outputAddressInfo();
	void bind();
	void accept();
	std::string receiveMessage();
	void sendResponse(const std::string response);
};

