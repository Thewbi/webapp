#pragma once

#include <string>

class WebServerException : public std::exception
{
private:
	std::string message_;

public:
	explicit WebServerException(const std::string& message);
	virtual ~WebServerException();

	virtual const char* what() const throw() {
		return message_.c_str();
	}
};

