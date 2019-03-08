#include "pch.h"
#include "AppServer.h"
#include "../exception/WebServerException.h"


AppServer::AppServer(const std::string port) : port_(port), pAddrInfo_(NULL), 
	listenSocket_(INVALID_SOCKET), clientSocket_(INVALID_SOCKET)
{
	// initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (iResult != 0) 
	{
		std::stringstream message;
		message << "Winsock initialization WSAStartup() failed. Error Code: ";
		message << iResult;

		throw WebServerException(message.str());
	}
}


AppServer::~AppServer()
{
	if (pAddrInfo_ != NULL)
	{
		freeaddrinfo(pAddrInfo_);
		pAddrInfo_ = NULL;
	}

	if (listenSocket_ != INVALID_SOCKET)
	{
		closesocket(listenSocket_);
		listenSocket_ = INVALID_SOCKET;
	}

	if (clientSocket_ != INVALID_SOCKET)
	{
		closesocket(clientSocket_);
		clientSocket_ = INVALID_SOCKET;
	}

	WSACleanup();
}



void AppServer::determineAddressInfo()
{
	// the getaddrinfo function is used to determine the values in the sockaddr structure
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	INT iResult = getaddrinfo(NULL, port_.c_str(), &hints, &pAddrInfo_);
	if (iResult != 0) 
	{
		std::stringstream message;
		message << "getaddrinfo failed. Error Code: ";
		message << iResult;

		throw WebServerException(message.str());
	}

	//printf("getaddrinfo returned success\n");
}

void AppServer::bind()
{
	// create a SOCKET object called ListenSocket for the server to listen for client connections.
	//    SOCKET listenSocket = INVALID_SOCKET;

	// create a SOCKET for the server to listen for client connections
	listenSocket_ = socket(pAddrInfo_->ai_family, pAddrInfo_->ai_socktype, pAddrInfo_->ai_protocol);

	// check for errors to ensure that the socket is a valid socket.
	if (listenSocket_ == INVALID_SOCKET)
	{
		std::stringstream message;
		message << "Error at socket(). Error Code: ";
		message << WSAGetLastError();

		throw WebServerException(message.str());
	}

	// setup the TCP listening socket
	INT iResult = ::bind(listenSocket_, pAddrInfo_->ai_addr, (int) pAddrInfo_->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::stringstream message;
		message << "bind failed with error. Error Code: ";
		message << WSAGetLastError();

		closesocket(listenSocket_);
		listenSocket_ = INVALID_SOCKET;

		throw WebServerException(message.str());
	}

	// once the bind function is called, the address information returned by the getaddrinfo function is no longer needed.
	freeaddrinfo(pAddrInfo_);
	pAddrInfo_ = NULL;

	if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
	{
		std::stringstream message;
		message << "Listen failed with error. Error Code: ";
		message << WSAGetLastError();

		closesocket(listenSocket_);
		listenSocket_ = INVALID_SOCKET;

		throw WebServerException(message.str());		
	}
}

void AppServer::accept()
{
	// accept a client socket
	clientSocket_ = ::accept(listenSocket_, NULL, NULL);
	if (clientSocket_ == INVALID_SOCKET)
	{
		std::stringstream message;
		message << "Accept failed with error. Error Code: ";
		message << WSAGetLastError();

		closesocket(listenSocket_);
		listenSocket_ = INVALID_SOCKET;

		throw WebServerException(message.str());
	}
}

std::string AppServer::receiveMessage()
{
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	std::string request;
	INT iResult;

	char *begin = recvbuf;
	char *end = begin + DEFAULT_BUFLEN;
	std::fill(begin, end, 0);

	// receive until the peer shuts down the connection
	do
	{
		iResult = recv(clientSocket_, recvbuf, recvbuflen, 0);

		if (iResult > 0)
		{
			//request.append(recvbuf);
			//memset(&recvbuf[0], 0, recvbuflen	);
			std::string temp(recvbuf, 0, iResult);
			request.append(temp);

			char *begin = recvbuf;
			char *end = begin + DEFAULT_BUFLEN;
			std::fill(begin, end, 0);
		}
		else if (iResult == 0)
		{
			printf("Connection closing...\n");
		}
		else
		{
			std::stringstream message;
			message << "recv failed: Error Code: ";
			message << WSAGetLastError();

			closesocket(listenSocket_);
			listenSocket_ = INVALID_SOCKET;

			throw WebServerException(message.str());
		}

	} while (iResult >= DEFAULT_BUFLEN);

	return request;
}

void AppServer::sendResponse(const std::string response)
{
	// send the response back to the sender
	INT iSendResult = ::send(clientSocket_, response.c_str(), response.size(), 0);
	if (iSendResult == SOCKET_ERROR)
	{
		//printf("send failed: %d\n", WSAGetLastError());
		std::stringstream message;
		message << "send failed: Error Code: ";
		message << WSAGetLastError();

		closesocket(listenSocket_);
		listenSocket_ = INVALID_SOCKET;

		throw WebServerException(message.str());
	}
	//printf("Bytes sent: %d\n", iSendResult);

	// shutdown the send half of the connection since no more data will be sent
	INT iResult = shutdown(clientSocket_, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		//printf("shutdown failed: %d\n", WSAGetLastError());

		std::stringstream message;
		message << "shutdown failed: Error Code: ";
		message << WSAGetLastError();

		::closesocket(clientSocket_);
		clientSocket_ = INVALID_SOCKET;
		
		throw WebServerException(message.str());
	}
}

void AppServer::outputAddressInfo()
{
	/**/
	INT iRetval;
	int i = 1;
	struct sockaddr_in  *sockaddr_ipv4;
	LPSOCKADDR sockaddr_ip;
	char ipstr[INET6_ADDRSTRLEN];
	WCHAR ipstringbuffer[46];
	DWORD ipbufferlength = 46;

	struct addrinfo *ptr;

	// retrieve each address and print out the hex bytes
	for (ptr = pAddrInfo_; ptr != NULL; ptr = ptr->ai_next)
	{
		printf("getaddrinfo response %d\n", i++);
		printf("\tFlags: 0x%x\n", ptr->ai_flags);
		printf("\tFamily: ");

		switch (ptr->ai_family)
		{
		case AF_UNSPEC:
			printf("Unspecified\n");
			break;

		case AF_INET:
			printf("AF_INET (IPv4)\n");
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			inet_ntop(ptr->ai_family, ptr->ai_addr, (PSTR)ipstr, sizeof(ipstr));
			printf("\tIPv4 address %s\n", ipstr);

			break;

		case AF_INET6:
			printf("AF_INET6 (IPv6)\n");
			// the InetNtop function is available on Windows Vista and later
			//sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
			//printf("\tIPv6 address %s\n",
			// InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );

			// We use WSAAddressToString since it is supported on Windows XP and later
			sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;

			// The buffer length is changed by each call to WSAAddresstoString
			// So we need to set it for each iteration through the loop for safety
			ipbufferlength = 46;
			iRetval = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
				ipstringbuffer, &ipbufferlength);
			if (iRetval)
			{
				printf("WSAAddressToString failed with %u\n", WSAGetLastError());
			}
			else
			{
				printf("\tIPv6 address %s\n", ipstringbuffer);
			}
			break;

		case AF_NETBIOS:
			printf("AF_NETBIOS (NetBIOS)\n");
			break;

		default:
			printf("Other %ld\n", ptr->ai_family);
			break;
		}

		printf("\tSocket type: ");
		switch (ptr->ai_socktype)
		{
		case 0:
			printf("Unspecified\n");
			break;
		case SOCK_STREAM:
			printf("SOCK_STREAM (stream)\n");
			break;
		case SOCK_DGRAM:
			printf("SOCK_DGRAM (datagram) \n");
			break;
		case SOCK_RAW:
			printf("SOCK_RAW (raw) \n");
			break;
		case SOCK_RDM:
			printf("SOCK_RDM (reliable message datagram)\n");
			break;
		case SOCK_SEQPACKET:
			printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_socktype);
			break;
		}

		printf("\tProtocol: ");
		switch (ptr->ai_protocol)
		{
		case 0:
			printf("Unspecified\n");
			break;
		case IPPROTO_TCP:
			printf("IPPROTO_TCP (TCP)\n");
			break;
		case IPPROTO_UDP:
			printf("IPPROTO_UDP (UDP) \n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_protocol);
			break;
		}
		printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
		printf("\tCanonical name: %s\n", ptr->ai_canonname);
	}
	
}
