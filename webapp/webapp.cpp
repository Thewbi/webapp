#include "pch.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "factory/RequestFactory.h"
#include "server/AppServer.h"


#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define _WINSOCK_DEPCRECATED 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

// https://docs.microsoft.com/en-us/windows/desktop/winsock/creating-a-socket-for-the-server
int main()
{
	AppServer appServer(DEFAULT_PORT);
	appServer.determineAddressInfo();
	appServer.outputAddressInfo();
	appServer.bind();
	appServer.accept();
	std::string request = appServer.receiveMessage();

	std::cout << request << std::endl;

	RequestFactory requestFactory;
	HTTPRequest httpRequest = requestFactory.createRequest(request);

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

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK" << std::endl;
	ss << "Server: RapGod" << std::endl;
	ss << "Content-Length: " << responseBody.size() << std::endl;
	ss << "Connection: close" << std::endl;
	ss << "Content-Type: text/html" << std::endl;
	ss << std::endl;
	ss << responseBody;

	appServer.sendResponse(ss.str());

	return 0;
}