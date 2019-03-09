#pragma once

#include <string>
#include <map>

#include "../HTTPRequest.h"

class RequestData
{
public:
	HTTPRequest httpRequest_;
	std::string resourcePath_;
	std::map<std::string, std::string> model_;

public:
	RequestData();
	virtual ~RequestData();
};

