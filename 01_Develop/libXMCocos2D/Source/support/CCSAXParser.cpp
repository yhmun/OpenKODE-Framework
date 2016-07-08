/* -----------------------------------------------------------------------------------
 *
 *      File            CCSAXParser.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      ¬®¬Ñ¬Ü¬ã¬Ú¬Þ ¬¡¬Ü¬ã¬Ö¬ß¬à¬Ó
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/CCSAXParser.h"
#include "support/CCFileUtils.h"
#include "cocoa/CCDictionary.h"

#include <XMXml/parser.h>
#include <XMXml/tree.h>
#include <XMXml/xmlmemory.h>

NS_CC_BEGIN
    
CCSAXParser::CCSAXParser ( KDvoid )
{
    m_pDelegator = KD_NULL;
}

CCSAXParser::~CCSAXParser ( KDvoid )
{

}

KDbool CCSAXParser::init ( const KDchar* szEncoding )
{
    CC_UNUSED_PARAM ( szEncoding );
	// nothing to do
	return KD_TRUE;
}

KDbool CCSAXParser::parse ( const KDchar* pXMLData, KDuint uDataLength )
{
	//
	// this initializes the library and checks potential ABI mismatches
	// between the version it was compiled for and the actual shared
	// library used.
	//
	LIBXML_TEST_VERSION

	xmlSAXHandler  tSaxHandler;
	kdMemset ( &tSaxHandler, 0, sizeof ( tSaxHandler ) );

	// Using xmlSAXVersion ( &saxHandler, 2 ) generate crash as it sets plenty of other pointers...
	tSaxHandler.initialized  = XML_SAX2_MAGIC;  // so we do this to force parsing as SAX2.
	tSaxHandler.startElement = (startElementSAXFunc) ( &CCSAXParser::startElement );
	tSaxHandler.endElement   = (endElementSAXFunc  ) ( &CCSAXParser::endElement   );
	tSaxHandler.characters   = (charactersSAXFunc  ) ( &CCSAXParser::textHandler  );

	if ( xmlSAXUserParseMemory ( &tSaxHandler, this, pXMLData, uDataLength ) )
	{
		return KD_FALSE;
	}

	//
	// Cleanup function for the XML library.
	//
	xmlCleanupParser ( );

	//
	// this is to debug memory for regression tests
	//
	xmlMemoryDump ( );

    return KD_TRUE;
}

KDbool CCSAXParser::parse ( const KDchar* szFilePath )
{
	CCFileUtils*	pFileUtils = CCFileUtils::sharedFileUtils ( );
	std::string		sPath = pFileUtils->fullPathForFilename ( szFilePath );

	KDbool   bRet  = KD_FALSE;
    KDsize   uSize = 0;
	KDchar*  pBuffer = (KDchar*) pFileUtils->getFileData ( sPath.c_str ( ), "rt", &uSize );
    if ( pBuffer != KD_NULL && uSize > 0 )
    {
        bRet = parse ( pBuffer, uSize );
    }
    CC_SAFE_DELETE_ARRAY ( pBuffer );
    return bRet;
}

KDvoid CCSAXParser::startElement ( KDvoid* pCtx, const KDchar* szFilePath, const KDchar** pAtts )
{
	( (CCSAXParser *) pCtx )->m_pDelegator->startElement ( pCtx, szFilePath, pAtts );
}

KDvoid CCSAXParser::endElement ( KDvoid* pCtx, const KDchar* szFilePath )
{
	( (CCSAXParser *) pCtx )->m_pDelegator->endElement ( pCtx, szFilePath );
}

KDvoid CCSAXParser::textHandler ( KDvoid* pCtx, const KDchar* szFilePath, KDint nLen )
{
	( (CCSAXParser *) pCtx )->m_pDelegator->textHandler ( pCtx, szFilePath, nLen );
}

KDvoid CCSAXParser::setDelegator ( CCSAXDelegator* pDelegator )
{
	m_pDelegator = pDelegator;
}

NS_CC_END