#include "pch.h"
#include "Engine.h"

const std::string Engine::ENGINE_PREFIX = "ENGINE:";
const std::string Engine::REDIRECT_PREFIX = "REDIRECT:";


Engine::Engine()
{
}


Engine::~Engine()
{
}


const std::string Engine::process(const RequestData &requestData)
{
	std::string filename(requestData.resourcePath_);	
	filename.insert(0, "../webapp/resources/");

	std::string fileData = readFileIntoString(filename);

	std::string value = requestData.model_.at("title");

	//return std::regex_replace(fileData, std::regex("\${title}"), value);


	std::string a = value;
	std::string b = fileData;
	return std::regex_replace(b, std::regex(R"(\$\{title\})"), a);
}
