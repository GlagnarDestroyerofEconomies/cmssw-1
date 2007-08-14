#ifndef CALIBRATIONXML_H 
#define CALIBRATIONXML_H 

#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <string>
#include <sstream>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

using namespace XERCES_CPP_NAMESPACE;

class CalibrationXML  
{
public:
	CalibrationXML();
	~CalibrationXML();
	
	/**
	* Open an XML file
	*/	
	void openFile(const std::string & xmlFileName);

	/**
	* Save DOM to file
	*/	
	void saveFile(const std::string & xmlFileName);


        void closeFile() 
        {
          if(errHandler) delete errHandler;
          if(parser) {  delete parser; XMLPlatformUtils::Terminate(); } 
          errHandler=0;
          parser=0;
        } 	
	/**
	* Return the root DOM Element of the opened XML calibration file
	*/
	DOMElement * calibrationDOM() { return m_calibrationDOM;}


//Static function to make everything easier, less transcode and type conversion
	/**
	* Helper static function to write an attribute in a DOM Element
	*/
           template <class T> static void writeAttribute(DOMElement *dom, const std::string & name, const T & value)
        {
            std::ostringstream buffer;
            buffer << value;
            XMLCh * nameStr = XMLString::transcode(name.c_str());
            XMLCh * valueStr = XMLString::transcode(buffer.str().c_str());
            dom->setAttribute(nameStr, valueStr );
            XMLString::release(&nameStr);
            XMLString::release(&valueStr);
        }

        /**
        * Helper static function to read an attribute in a DOM Element
        */
        template <class T> static T readAttribute(DOMElement *dom, const std::string & name)
        {
            XMLCh* nameStr  = XMLString::transcode(name.c_str());
            char * valueStr = XMLString::transcode(dom->getAttribute(nameStr));
            std::istringstream buffer(valueStr);
            T value;
            buffer >> value;
            XMLString::release(&nameStr);
            XMLString::release(&valueStr);
            return value;
        }
	
	/**
	* Helper static function to add a child in a DOM Element with indentation
	*/
        static DOMElement * addChild(DOMNode *dom,const std::string & name);
		
private:
	std::string m_xmlFileName;
	DOMElement * m_calibrationDOM;
	DOMDocument* doc;
	HandlerBase* errHandler;
	XercesDOMParser *parser;
};
#endif

