//#include "stdafx.h"
#include "pch.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512


#define _WINSOCK_DEPCRECATED 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1



// https://docs.microsoft.com/en-us/windows/desktop/winsock/creating-a-socket-for-the-server
int main()
{
    std::cout << "Hello World!\n";

	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	// The getaddrinfo function is used to determine the values in the sockaddr structure
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	
	printf("getaddrinfo returned success\n");
	/*
//	WSADATA wsaData;
//	int iResult;
	INT iRetval;
//	DWORD dwRetval;
	int i = 1;
	struct sockaddr_in  *sockaddr_ipv4;
	LPSOCKADDR sockaddr_ip;
	//char ipstringbuffer[46];
	char ipstr[INET6_ADDRSTRLEN];
	LPWSTR ipstringbuffer;
	DWORD ipbufferlength = 46;

	// Retrieve each address and print out the hex bytes
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		printf("getaddrinfo response %d\n", i++);
		printf("\tFlags: 0x%x\n", ptr->ai_flags);
		printf("\tFamily: ");

		switch (ptr->ai_family) {
		case AF_UNSPEC:
			printf("Unspecified\n");
			break;
		case AF_INET:
			printf("AF_INET (IPv4)\n");
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			//printf("\tIPv4 address %s\n", inet_ntoa(sockaddr_ipv4->sin_addr));
			//inet_ntop(sockaddr_ipv4->sin_addr));

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
		switch (ptr->ai_socktype) {
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
		switch (ptr->ai_protocol) {
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
	*/

	// Create a SOCKET object called ListenSocket for the server to listen for client connections.
	SOCKET ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	// Check for errors to ensure that the socket is a valid socket.
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Once the bind function is called, the address information returned by the getaddrinfo function is no longer needed.
	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}



	char recvbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	std::string request;

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			request.append(recvbuf);
			
		}
		else if (iResult == 0)
		{
			printf("Connection closing...\n");
		}
		else 
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

		

	} while (iResult >= DEFAULT_BUFLEN);

	/*
	HTTP / 1.1 200 OK
	Server : Apache / 1.3.29 (Unix)PHP / 4.3.4
	Content - Length : 123456 (Größe von infotext.html in Byte)
	Content - Language : de(nach RFC 3282 sowie RFC 1766)
	Connection : close
	Content - Type : text / html
	*/

	std::string responseBody;
	responseBody.append("<!DOCTYPE html><html><body>test</body></html>");

	/*
	std::string response;
	response.append("HTTP / 1.1 200 OK").append("\n");
	response.append("Server : RapGod").append("\n");
	response.append("Content - Length : ").append(responseBody.size()).append("\n");
	response.append("Content - Language : de").append("\n");
	response.append("Connection : close").append("\n");
	response.append("Content - Type : text / html").append("\n");

	response.append(responseBody);
	*/

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK" << std::endl;
	ss << "Server: RapGod" << std::endl;
	ss << "Content-Length: " << responseBody.size() << std::endl;
//	ss << "Content-Language : de" << std::endl;
	ss << "Connection: close" << std::endl;
	ss << "Content-Type: text/html" << std::endl;
	ss << std::endl;
	ss << responseBody;

	// Echo the buffer back to the sender
	iSendResult = send(ClientSocket, ss.str().c_str(), ss.str().size(), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	printf("Bytes sent: %d\n", iSendResult);

	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}