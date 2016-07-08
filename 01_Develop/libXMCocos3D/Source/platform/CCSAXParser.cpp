/* -----------------------------------------------------------------------------------
 *
 *      File            CCSAXParser.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Максим Аксенов
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#include "platform/CCSAXParser.h"
#include "base/CCDictionary.h"
#include "platform/CCFileUtils.h"
#include <XMTinyXml/tinyxml2.h>

#include <vector> // because its based on windows 8 build :P

NS_CC_BEGIN

class XmlSaxHander : public tinyxml2::XMLVisitor
{
public:
	XmlSaxHander():_ccsaxParserImp(0){};
	
	virtual bool VisitEnter( const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute );
	virtual bool VisitExit( const tinyxml2::XMLElement& element );
	virtual bool Visit( const tinyxml2::XMLText& text );
	virtual bool Visit( const tinyxml2::XMLUnknown&){ return true; }

	void setSAXParserImp(SAXParser* parser)
	{
		_ccsaxParserImp = parser;
	}

private:
	SAXParser *_ccsaxParserImp;
};


bool XmlSaxHander::VisitEnter( const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute )
{
	//log(" VisitEnter %s",element.Value());

	std::vector<const char*> attsVector;
	for( const tinyxml2::XMLAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		//log("%s", attrib->Name());
		attsVector.push_back(attrib->Name());
		//log("%s",attrib->Value());
		attsVector.push_back(attrib->Value());
	}
    
    // nullptr is used in c++11
	//attsVector.push_back(nullptr);
    attsVector.push_back(NULL);

	SAXParser::startElement(_ccsaxParserImp, (const CC_XML_CHAR *)element.Value(), (const CC_XML_CHAR **)(&attsVector[0]));
	return true;
}
bool XmlSaxHander::VisitExit( const tinyxml2::XMLElement& element )
{
	//log("VisitExit %s",element.Value());

	SAXParser::endElement(_ccsaxParserImp, (const CC_XML_CHAR *)element.Value());
	return true;
}

bool XmlSaxHander::Visit( const tinyxml2::XMLText& text )
{
	//log("Visit %s",text.Value());
	SAXParser::textHandler(_ccsaxParserImp, (const CC_XML_CHAR *)text.Value(), strlen(text.Value()));
	return true;
}

SAXParser::SAXParser()
{
    m_pDelegator = NULL;
}

SAXParser::~SAXParser(void)
{
}

bool SAXParser::init(const char *pszEncoding)
{
    CC_UNUSED_PARAM(pszEncoding);
    // nothing to do
    return true;
}

bool SAXParser::parse(const char* pXMLData, unsigned int uDataLength)
{
	tinyxml2::XMLDocument tinyDoc;
	tinyDoc.Parse(pXMLData, uDataLength);
	XmlSaxHander printer;
	printer.setSAXParserImp(this);
	
	return tinyDoc.Accept( &printer );	
}

bool SAXParser::parse(const char *pszFile)
{
    bool ret = false;
    KDint32 size = 0;
    char* pBuffer = (char*)FileUtils::getInstance()->getFileData(pszFile, "rt", &size);
    if (pBuffer != NULL && size > 0)
    {
        ret = parse(pBuffer, size);
    }
    kdFree(pBuffer);
    return ret;
}

void SAXParser::startElement(void *ctx, const CC_XML_CHAR *name, const CC_XML_CHAR **atts)
{
    ((SAXParser*)(ctx))->m_pDelegator->startElement(ctx, (char*)name, (const char**)atts);
}

void SAXParser::endElement(void *ctx, const CC_XML_CHAR *name)
{
    ((SAXParser*)(ctx))->m_pDelegator->endElement(ctx, (char*)name);
}
void SAXParser::textHandler(void *ctx, const CC_XML_CHAR *name, int len)
{
    ((SAXParser*)(ctx))->m_pDelegator->textHandler(ctx, (char*)name, len);
}
void SAXParser::setDelegator(SAXDelegator* pDelegator)
{
    m_pDelegator = pDelegator;
}

NS_CC_END


