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

	/*
	XmlDomDocument* doc = new XmlDomDocument(filename.c_str());
	if (doc) 
	{
		for (int i = 0; i < doc->getRootElementCount("html"); i++) 
		{
			printf("%d\n", i + 1);
			//std::string value = doc->getChildAttribute("bookstore", 0, "book", i, "category");
		}
		delete doc;
	}
	*/

	std::string result;

	XmlDomDocument* xmlDomDocument = new XmlDomDocument(filename.c_str());
	if (xmlDomDocument)
	{
		result = xmlDomDocument->recurse();

		delete xmlDomDocument;
	}

	
	/*
	try
	{
		// initialize Xerces infrastructure
		xercesc::XMLPlatformUtils::Initialize();

		//xercesc::XercesDOMParser parser;

		
		xercesc::SAX2XMLReader* reader = xercesc::XMLReaderFactory::createXMLReader();
		xercesc::SAX2XMLReader* filter = NULL;

		xercesc::SAX2XMLReader* parser = reader;

		XMLSize_t errorCount = 0;
		int errorCode = 0;
		try
		{
			XercesContentHandler xercesContentHandler;
			//SAX2PrintHandlers handler(encodingName, unRepFlags, expandNamespaces);
			parser->setContentHandler(&xercesContentHandler);
			//parser.setErrorHandler(&handler);
			parser->parse(filename.c_str());
			errorCount = parser->getErrorCount();
		}
		catch (const xercesc::OutOfMemoryException&)
		{
			//XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
			errorCode = 5;
		}
		catch (const xercesc::XMLException& toCatch)
		{
			//XERCES_STD_QUALIFIER cerr << "\nAn error occurred\n  Error: "
			//	<< StrX(toCatch.getMessage())
			//	<< "\n" << XERCES_STD_QUALIFIER endl;
			errorCode = 4;
		}

		if (errorCode) {
			//XMLPlatformUtils::Terminate();
			//return errorCode;
			std::cout << "An error occured!" << std::endl;
		}


		// shutdown Xerces infrastructure
		xercesc::XMLPlatformUtils::Terminate();
	}
	catch (xercesc::XMLException& e)
	{
			char* message = xercesc::XMLString::transcode(e.getMessage());
			//std::cerr << "XML toolkit initialization error: " << message << std::endl;
			xercesc::XMLString::release(&message);
			// throw exception here to return ERROR_XERCES_INIT
	}
	*/

	//std::string fileData = readFileIntoString(filename);
	//std::string value = requestData.model_.at("title");
	//return std::regex_replace(fileData, std::regex(R"(\$\{title\})"), value);


	return result;
}
