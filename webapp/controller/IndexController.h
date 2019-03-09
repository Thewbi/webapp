#pragma once

#include <sstream>

#include "BaseController.h"

class IndexController :
	public BaseController
{
public:
	IndexController();
	virtual ~IndexController();

	virtual std::string handleRequest(RequestData &requestData);
};

