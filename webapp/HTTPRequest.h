#pragma once

#include <map>

enum HTTP_METHOD_TYPE { GET, POST, PUT, PATCH, DELETE_, HEAD, TRACE, CONNECT, OPTIONS, UNKOWN };

class HTTPRequest
{
public:
	HTTP_METHOD_TYPE methodType_;
	std::string path_;
	std::map<std::string, std::string> headers_;

public:
	HTTPRequest();
	virtual ~HTTPRequest();
};

