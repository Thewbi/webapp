#pragma once

#include "../controller/BaseController.h"
#include "../controller/IndexController.h"
#include "../controller/DefaultController.h"
#include "../HTTPRequest.h"

class DefaultRouter
{
public:
	DefaultRouter();
	~DefaultRouter();

	std::unique_ptr<BaseController> route(const HTTPRequest &request);
};

