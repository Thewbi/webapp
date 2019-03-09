#pragma once

#include <string>

#include "../HTTPRequest.h"
#include "../engine/RequestData.h"

class BaseController
{
public:
	BaseController();
	virtual ~BaseController();

	virtual std::string handleRequest(RequestData &requestData) = 0;
};

