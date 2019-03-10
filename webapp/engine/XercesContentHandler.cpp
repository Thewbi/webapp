#include "pch.h"
#include "XercesContentHandler.h"


XercesContentHandler::XercesContentHandler()
{
}


XercesContentHandler::~XercesContentHandler()
{
}


void XercesContentHandler::startElement(const XMLCh* const uri,
	const XMLCh* const localname,
	const XMLCh* const qname,
	const xercesc::Attributes& attributes)
{
	char* temp = xercesc::XMLString::transcode(localname);
	std::cout << temp << std::endl;
	xercesc::XMLString::release(&temp);
}
