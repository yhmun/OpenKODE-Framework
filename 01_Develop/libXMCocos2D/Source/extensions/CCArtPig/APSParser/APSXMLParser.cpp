/* --------------------------------------------------------------------------
 *
 *      File            APSXMLParser.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCArtPig/APSXMLParser.h"
#include "support/CCFileUtils.h"
#include <XMXml/xmlreader.h>
#include <XMXml/tree.h>

NS_APS_BEGIN

const KDchar*  APXML_STRING_KEY					= "str";
const KDchar*  APXML_BOOLEAN_KEY				= "bool";
const KDchar*  APXML_INTEGER_KEY				= "int";
const KDchar*  APXML_REAL_KEY					= "real";
const KDchar*  APXML_ARRAY_KEY					= "array";
const KDchar*  APXML_DICTIONARY_KEY				= "dict";
const KDchar*  APXML_DICTIONARY_ITEM_NAME_KEY	= "key";
const KDchar*  APXML_VERSION_KEY				= "version";
const KDchar*  APXML_APLIST_KEY					= "APList";

// Returns a new xml node
xmlNodePtr  addNode ( xmlNodePtr pParentNode, APSObject* pObject );
// Makes an APSObject for the xml node.
APSObject*  objectForXmlNode ( xmlNodePtr pNode );

APSXMLParser::APSXMLParser ( APSDictionary* pDictionary, KDfloat fVersion )
{
	m_pDictionary = KD_NULL;
	m_fVersion    = fVersion;

    this->setDictionary ( pDictionary );
}

APSXMLParser::~APSXMLParser ( KDvoid )
{
    APS_SAFE_RELEASE ( this->m_pDictionary );
}

KDbool APSXMLParser::loadFile ( const KDchar* szPath ) 
{
    xmlDocPtr  pDoc;
    xmlChar*   pContents;
    
    const KDubyte*  pBuffer = KD_NULL;
 	KDsize  uBufferSize = 0;
    
 	pBuffer = CCFileUtils::sharedFileUtils ( )->getFileData ( szPath, "r", &uBufferSize );

	pDoc = xmlParseMemory ( (const KDchar*) pBuffer, uBufferSize );
	if ( pDoc == KD_NULL ) 
	{
		return KD_FALSE;
	}
    
	xmlNodePtr  pRoot = xmlDocGetRootElement ( pDoc );
    xmlNodePtr  pNode = pRoot->children;
    pContents = xmlNodeGetContent ( pNode );
    if ( pContents ) 
	{
        this->setVersion ( kdAtof ( (const KDchar*) pContents ) );
        
        pNode = pNode->next;

        APSAutoreleasePool::pushPool ( );
        this->setDictionary ( (APSDictionary*) objectForXmlNode ( pNode ) );
        APSAutoreleasePool::popPool ( );        
    }
    
    xmlFree ( pContents );
    xmlFreeDoc ( pDoc );
    
    if ( pBuffer ) 
	{
        delete [] pBuffer;
        pBuffer = NULL;
    }
    
    return KD_TRUE;
}

// Makes an APSObject for the xml node.
APSObject* objectForXmlNode ( xmlNodePtr pNode ) 
{ 
    xmlChar*  pContent;
    
    switch ( *pNode->name ) 
	{
        case 's' : 
		{
			// string
			pContent = xmlNodeGetContent ( pNode );
			APSString*  pString = APSString::stringWithString ( (const KDchar*) pContent );
			xmlFree ( pContent );
			return pString;
		}
		
		case 'b' : 
		{
            // boolean
            pContent = xmlNodeGetContent ( pNode );
            APSNumber*  pNumber = APSNumber::numberWithBooleanString ( (const KDchar*) pContent );
            xmlFree ( pContent );
            return pNumber;
        }
		
		case 'i' :
		{
            // integer
            pContent = xmlNodeGetContent ( pNode );
            APSNumber*  pNumber = APSNumber::numberWithLong ( kdAtoi ( (const KDchar*) pContent ) );
            xmlFree ( pContent );
            return pNumber;
        } 
		
		case 'r' : 
		{
            // real
            pContent = xmlNodeGetContent ( pNode );
            APSNumber*  pNumber = APSNumber::numberWithDouble ( kdAtof ( (const KDchar*) pContent ) );
            xmlFree ( pContent );
            return pNumber;
        } 
		
		case 'a' : 
		{
            // array
            APSArray*  pArray = APSArray::array ( );
            xmlNodePtr pIterNode = pNode->xmlChildrenNode;
            while ( pIterNode )
			{
                APSObject*  pObject = objectForXmlNode ( pIterNode );
                if ( pObject )
				{
                    pArray->addObject ( pObject );
                }
                
                pIterNode = pIterNode->next;
            }

            return pArray;
        }
		
		case 'd': 
		{
            // dictionary
            APSDictionary*  pDict = APSDictionary::dictionary ( );            
            xmlNodePtr  pIterNode = pNode->xmlChildrenNode;
            while ( pIterNode ) 
			{
                pContent = xmlNodeGetContent ( pIterNode );
                std::string  sKey = (const KDchar*) pContent;
                xmlFree ( pContent );
                
                if ( sKey.size ( ) > 0 )
				{
                    pIterNode = pIterNode->next;
                    APSObject*  pChildObject = objectForXmlNode ( pIterNode );
                    if ( pChildObject )
					{
                        pDict->setObjectForKey ( pChildObject, sKey );
                    }
                }
                
                pIterNode = pIterNode->next;
            }

            return pDict;
        }
    }
    
    return KD_NULL;
}

APSDictionary* APSXMLParser::getDictionary ( KDvoid ) const 
{
    return m_pDictionary;
}

KDvoid APSXMLParser::setDictionary ( APSDictionary* pDictionary ) 
{
    APS_SAFE_RETAIN ( pDictionary );
    APS_SAFE_RELEASE ( m_pDictionary );
    this->m_pDictionary = pDictionary;
}

NS_APS_END
