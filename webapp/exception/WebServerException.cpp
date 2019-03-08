#include "pch.h"
#include "WebServerException.h"




WebServerException::WebServerException(const std::string& message) : message_(message) {

}


WebServerException::~WebServerException()
{
}
