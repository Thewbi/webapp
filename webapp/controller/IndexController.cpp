#include "pch.h"
#include "IndexController.h"


IndexController::IndexController()
{
}


IndexController::~IndexController()
{
}

/**/
std::string IndexController::handleRequest(RequestData &requestData)
{
	std::string key("title");
	std::string value("TTTTTTTTIIIIIIITTTTTTTLLLLLLLLEEEEEEEE");

	requestData.model_.insert(std::make_pair(key, value));

	return "ENGINE:views/pages/index.html";
}

/*
std::string IndexController::handleRequest(const HTTPRequest &request)
{
	return "REDIRECT:/flup";
}
*/

/*
std::string IndexController::handleRequest(const HTTPRequest &request)
{
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

	return ss.str();
}
*/

/*
// redirect
// https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
std::string IndexController::handleRequest(const HTTPRequest &request)
{
	std::string responseBody;
	responseBody.append("");

	std::stringstream ss;
	ss << "HTTP/1.1 303 See Other" << std::endl;
	ss << "Server: RapGod" << std::endl;
	ss << "Content-Length: " << responseBody.size() << std::endl;
	ss << "Connection: close" << std::endl;
	ss << "Content-Type: text/html" << std::endl;
	ss << "Location: /flup" << std::endl;
	
	ss << std::endl;
	ss << responseBody;

	return ss.str();
}
*/
