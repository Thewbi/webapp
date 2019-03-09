#include "pch.h"
#include "DefaultRouter.h"


DefaultRouter::DefaultRouter()
{
}


DefaultRouter::~DefaultRouter()
{
}



std::unique_ptr<BaseController> DefaultRouter::route(const HTTPRequest &request)
{
	if (request.path_ == "/")
	{
		return std::make_unique<IndexController>();
	}

	return std::make_unique<DefaultController>();
}