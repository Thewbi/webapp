#pragma once

#include <string>
#include <iostream>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// https://vichargrave.github.io/xml-parsing-with-dom-in-cpp/
// https://github.com/vichargrave/xmldom/blob/master/XmlDomDocument.cpp
class XmlDomDocument
{
public:
	xercesc::DOMDocument* m_doc;

public:
	XmlDomDocument(const char* xmlfile);

	virtual ~XmlDomDocument();

	std::string recurse();

	std::string getChildValue(const char* parentTag, int parentIndex,
		const char* childTag, int childIndex);

	std::string getChildAttribute(const char* parentTag,
		int parentIndex,
		const char* childTag,
		int childIndex,
		const char* attributeTag);

	int getRootElementCount(const char* rootElementTag);

	int getChildCount(const char* parentTag, int parentIndex,
		const char* childTag);

private:
	XmlDomDocument();
	XmlDomDocument(const XmlDomDocument&);
};

