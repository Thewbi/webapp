#pragma once

#include <string>
#include <regex>
#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include "../util.h"
#include "RequestData.h"
#include "XercesContentHandler.h"
#include "XmlDomDocument.h"

/*
	Download: xerces - c - 3.1.0 - x86 - windows - vc - 9.0

	Microsoft Visual Studio Community 2017
	Microsoft Visual C++ 2017

	Add to project include folders :
	C:\Users\bischwol\Downloads\xerces-c-3.1.0-x86-windows-vc-9.0\include

	Add to project lib folders :
	C:\Users\bischwol\Downloads\xerces-c-3.1.0-x86-windows-vc-9.0\lib

	Add to linker input(DEBUG build) :
	xerces-c_3.lib

	Before starting your application, put the xerces.dll file next to your application so it can be loaded.
*/
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

