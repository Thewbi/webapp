#include "pch.h"
#include "RequestFactory.h"

#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}


RequestFactory::RequestFactory()
{
}


RequestFactory::~RequestFactory()
{
}


HTTPRequest RequestFactory::createRequest(const std::string request)
{
	HTTPRequest httpRequest;

	std::stringstream stringstream(request);
	



	std::regex r("[\\r\\n]+");
	std::sregex_token_iterator newlineIter = std::sregex_token_iterator(request.begin(), request.end(), r, -1);
	std::sregex_token_iterator rend;
	int lineIdx = 0;

	while (newlineIter != rend)
	{
		std::cout << *newlineIter << std::endl;
		std::string line = *newlineIter;
		newlineIter++;

		if (lineIdx == 0)
		{
			if (line.rfind("GET", 0) == 0)
			{
				httpRequest.methodType_ = GET;
			}

			std::regex rr("[ ]+");
			std::sregex_token_iterator firstLineIter = std::sregex_token_iterator(line.begin(), line.end(), rr, -1);
			std::sregex_token_iterator rend;

			firstLineIter++;
			std::string path = *firstLineIter;
			httpRequest.path_ = path;
		}
		else
		{
			std::regex e("([^:]+)");

			std::regex_iterator<std::string::iterator> rit(line.begin(), line.end(), e);
			std::regex_iterator<std::string::iterator> rend;

			std::string key;
			std::string value;

			int idx = 0;
			while (rit != rend)
			{
				//std::cout << rit->str() << std::endl;
				if (idx == 0)
				{
					key = rit->str();
				}
				else
				{
					if (idx > 1)
					{
						value.append(":");
					}
					value.append(rit->str());
				}

				++rit;

				idx++;
			}

			trim(value);
			httpRequest.headers_.insert(std::make_pair(key, value));
		}

		lineIdx++;
	}

	/*

	int lineIdx = 0;
	while (std::getline(stringstream, line, "")) 
	{
		if (lineIdx == 0)
		{
			if (line.rfind("GET", 0) == 0)
			{
				httpRequest.methodType_ = GET;
			}
		}
		else
		{
			std::regex e("([^:]+)");

			std::regex_iterator<std::string::iterator> rit(line.begin(), line.end(), e);
			std::regex_iterator<std::string::iterator> rend;

			std::string key;
			std::string value;

			int idx = 0;
			while (rit != rend) 
			{
				//std::cout << rit->str() << std::endl;
				if (idx == 0)
				{
					key = rit->str();
				} 
				else
				{
					if (idx > 1)
					{
						value.append(":");
					}
					value.append(rit->str());
				}

				++rit;

				idx++;
			}
			
			httpRequest.headers_.insert(std::make_pair(key, value));
		}

		lineIdx++;
	}

	*/

	return httpRequest;
}
