#pragma once

#include <string>
#include <sstream>
#include <regex>
#include <iostream>

#include "../HTTPRequest.h"

class RequestFactory
{
public:
	RequestFactory();
	virtual ~RequestFactory();

	HTTPRequest createRequest(const std::string request);
};

