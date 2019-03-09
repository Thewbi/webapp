#include "pch.h"
#include "DefaultController.h"


DefaultController::DefaultController()
{
}


DefaultController::~DefaultController()
{
}

std::string DefaultController::handleRequest(RequestData &requestData)
{
	std::string responseBody;
	responseBody.append("<!DOCTYPE html><html><body>An internal server error occured</body></html>");

	std::stringstream ss;
	ss << "HTTP/1.1 500 Internal Server Error" << std::endl;
	ss << "Server: RapGod" << std::endl;
	ss << "Content-Length: " << responseBody.size() << std::endl;
	ss << "Connection: close" << std::endl;
	ss << "Content-Type: text/html" << std::endl;
	ss << std::endl;
	ss << responseBody;

	return ss.str();
}
