#include "pch.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "factory/RequestFactory.h"
#include "server/AppServer.h"
#include "routing/DefaultRouter.h"
#include "engine/Engine.h"
#include "engine/RequestData.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define _WINSOCK_DEPCRECATED 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

// redirect
// https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
std::string redirect(const std::string redirectPath)
{
	std::string responseBody;
	responseBody.append("");

	std::stringstream ss;
	ss << "HTTP/1.1 303 See Other" << std::endl;
	ss << "Server: RapGod" << std::endl;
	ss << "Content-Length: " << responseBody.size() << std::endl;
	ss << "Connection: close" << std::endl;
	ss << "Content-Type: text/html" << std::endl;
	ss << "Location: " << redirectPath << std::endl;

	ss << std::endl;
	ss << responseBody;

	return ss.str();
}

std::string buildResponse(const std::string status, const std::string content)
{
	std::stringstream ss;
	ss << status << std::endl;
	ss << "Server: RapGod" << std::endl;
	ss << "Content-Length: " << content.size() << std::endl;
	ss << "Connection: close" << std::endl;
	ss << "Content-Type: text/html" << std::endl;

	ss << std::endl;
	ss << content;

	return ss.str();
}

// check what action the controller wants to perform (REDIRECT, ENGINE or direct answer)
const std::string prepareAnswer(const std::string controllerResponse, RequestData &requestData)
{
	if (starts_with(controllerResponse, Engine::ENGINE_PREFIX))
	{
		std::string filename = controllerResponse.substr(Engine::ENGINE_PREFIX.size());
		std::cout << "Loading: \"" << filename << "\"" << std::endl;
		requestData.resourcePath_ = filename;

		Engine engine;
		std::string result = engine.process(requestData);

		return buildResponse("HTTP/1.1 200 OK", result);
	}
	else if (starts_with(controllerResponse, Engine::REDIRECT_PREFIX))
	{
		std::string redirectPath = controllerResponse.substr(Engine::REDIRECT_PREFIX.size());
		std::cout << "Redirecting to: \"" << redirectPath << "\"" << std::endl;

		return redirect(redirectPath);
	}
	else
	{
		return controllerResponse;
	}
}

// https://docs.microsoft.com/en-us/windows/desktop/winsock/creating-a-socket-for-the-server
int main()
{
	AppServer appServer(DEFAULT_PORT);
	appServer.determineAddressInfo();
	appServer.outputAddressInfo();
	appServer.bind();
	appServer.accept();

	// retrieve and parse request
	std::string request = appServer.receiveMessage();
	std::cout << request << std::endl;
	RequestFactory requestFactory;
	HTTPRequest httpRequest = requestFactory.createRequest(request);

	// data object for carrying data through the controller and the engine
	RequestData requestData;
	requestData.httpRequest_ = httpRequest;

	// find a controller
	DefaultRouter defaultRouter;
	std::unique_ptr<BaseController> controller = defaultRouter.route(httpRequest);

	// hand the request to the controller
	// controller loads the model
	// controller can return a path to a view which is then put into the engine alongside the model and the result is served
	// controller can redirect
	std::string controllerResponse = controller->handleRequest(requestData);
	std::string responseAsString = prepareAnswer(controllerResponse, requestData);
	appServer.sendResponse(responseAsString);
	
	/*
	HTTP / 1.1 200 OK
	Server : Apache / 1.3.29 (Unix)PHP / 4.3.4
	Content - Length : 123456 (Größe von infotext.html in Byte)
	Content - Language : de(nach RFC 3282 sowie RFC 1766)
	Connection : close
	Content - Type : text / html
	*/

	/*
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
	*/

	return 0;
}