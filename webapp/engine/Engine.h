#pragma once

#include <string>
#include <regex>

#include "../util.h"
#include "RequestData.h"

class Engine
{
public:
	static const std::string ENGINE_PREFIX;
	static const std::string REDIRECT_PREFIX;

public:
	Engine();
	virtual ~Engine();

	const std::string process(const RequestData &requestData);
};

