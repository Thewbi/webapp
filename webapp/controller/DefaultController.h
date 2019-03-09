#pragma once

#include <sstream>

#include "BaseController.h"

class DefaultController :
	public BaseController
{
public:
	DefaultController();
	virtual ~DefaultController();

	virtual std::string handleRequest(RequestData &requestData);
};

