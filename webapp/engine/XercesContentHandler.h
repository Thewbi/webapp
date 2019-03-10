#pragma once

#include <iostream>

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

class XercesContentHandler : public xercesc::DefaultHandler
{
public:
	XercesContentHandler();
	virtual ~XercesContentHandler();

	void startElement(const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname,
		const xercesc::Attributes& attributes);
};

