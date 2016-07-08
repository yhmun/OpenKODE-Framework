/* -----------------------------------------------------------------------------------
 *
 *      File            CCUserDefault.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      ForzeField Studios S.L. 
 *
 *         http://www.cocos2d-x.org      
 *         http://forzefield.com   
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
#include "support/user_default/CCUserDefault.h"
#include "support/CCFileUtils.h"

#include <XMXML/parser.h>
#include <XMXML/tree.h>

NS_CC_BEGIN
    
// root name of xml
#define USERDEFAULT_ROOT_NAME	"userDefaultRoot"
#define XML_FILE_NAME           "UserDefault.xml"

static  xmlDocPtr  l_pSharedDoc = KD_NULL;

static xmlNodePtr getXMLNodeForKey ( const KDchar* szKey, xmlNodePtr* pRootNode )
{
	xmlNodePtr  pCurNode = KD_NULL;

	// check the key value
	if ( !szKey )
	{
		return KD_NULL;
	}

	do 
	{
		// get root node
		*pRootNode = xmlDocGetRootElement ( l_pSharedDoc );
		if ( KD_NULL == *pRootNode )
		{
			CCLOG ( "read root node error" );
			break;
		}

		// find the node
		pCurNode = ( *pRootNode )->xmlChildrenNode;
		while ( KD_NULL != pCurNode )
		{
			if ( !xmlStrcmp ( pCurNode->name, BAD_CAST szKey ) )
			{
				break;
			}

			pCurNode = pCurNode->next;
		}
	} while ( 0 );

	return pCurNode;
}

static inline const KDchar* getValueForKey ( const KDchar* szKey )
{
	const KDchar*  szRet = KD_NULL;
    
	xmlNodePtr  pRootNode;
	xmlNodePtr  pNode = getXMLNodeForKey ( szKey, &pRootNode );

	// find the node
	if ( pNode )
	{
		szRet = (const KDchar*) xmlNodeGetContent ( pNode );
	}

	return szRet;
}

static KDvoid setValueForKey ( const KDchar* szKey, const KDchar* szValue )
{
	xmlNodePtr  pRootNode;
	xmlNodePtr  pNode;

	// check the params
	if ( !szKey || !szValue )
	{
		return;
	}

	// find the node
	pNode = getXMLNodeForKey ( szKey, &pRootNode );

	// if node exist, change the content
	if ( pNode )
	{
		xmlNodeSetContent ( pNode, BAD_CAST szValue );
	}
	else
	{
		if ( pRootNode )
		{
			// the node doesn't exist, add a new one
			// libxml in android donesn't support xmlNewTextChild, so use this approach
			xmlNodePtr  pTempNode = xmlNewNode ( KD_NULL, BAD_CAST szKey );
			xmlNodePtr  pContent  = xmlNewText ( BAD_CAST szValue );
            
			xmlAddChild ( pRootNode, pTempNode );
			xmlAddChild ( pTempNode, pContent );
		}	
	}
}

//
// implements of CCUserDefault
//
CCUserDefault*   l_pSharedUserDefault = KD_NULL;
std::string      l_sFilePath = "";
KDbool           l_bIsFilePathInitialized = KD_FALSE;

///
/// If the user invoke delete CCUserDefault::sharedUserDefault(), should set l_sharedUserDefault
/// to null to avoid error when he invoke CCUserDefault::sharedUserDefault() later.
///
CCUserDefault::CCUserDefault ( KDvoid )
{
	l_pSharedDoc = xmlReadFile ( getXMLFilePath ( ).c_str ( ), "utf-8", XML_PARSE_RECOVER );
}

CCUserDefault::~CCUserDefault ( KDvoid )
{
    flush ( );

    if ( l_pSharedDoc )
    {
        xmlFreeDoc ( l_pSharedDoc );
        l_pSharedDoc = KD_NULL;
    }
}

KDvoid CCUserDefault::purgeSharedUserDefault ( KDvoid )
{
	CC_SAFE_DELETE ( l_pSharedUserDefault );
	l_pSharedUserDefault = KD_NULL;
}

KDbool CCUserDefault::getBoolForKey ( const KDchar* szKey ) 
{
	return getBoolForKey ( szKey, KD_FALSE );
}

KDbool CCUserDefault::getBoolForKey ( const KDchar* szKey, KDbool bDefault )
{
	const KDchar*  szValue = getValueForKey ( szKey );
	KDbool         bRet = bDefault;

	if ( szValue )
	{
		bRet = ( !kdStrcmp ( szValue, "true" ) );
		xmlFree ( (KDvoid *) szValue );
	}

	return bRet;
}

KDint CCUserDefault::getIntegerForKey ( const KDchar* szKey ) 
{
	return getIntegerForKey ( szKey, 0 );
}

KDint CCUserDefault::getIntegerForKey ( const KDchar* szKey, KDint nDefault ) 
{
	const KDchar*  szValue = getValueForKey ( szKey );
	KDint          nRet = nDefault;

	if ( szValue )
	{
		nRet = kdStrtol ( szValue, KD_NULL, 0 );
		xmlFree ( (KDvoid *) szValue );
	}

	return nRet;
}

KDfloat CCUserDefault::getFloatForKey ( const KDchar* szKey ) 
{
	return getFloatForKey ( szKey, 0.0f );
}

KDfloat CCUserDefault::getFloatForKey ( const KDchar* szKey, KDfloat fDefault )
{
	KDfloat  fRet = (KDfloat) getDoubleForKey ( szKey, (KDdouble) fDefault );
 
	return fRet;
}

KDdouble CCUserDefault::getDoubleForKey ( const KDchar* szKey ) 
{
	return getDoubleForKey ( szKey, 0.0 );
}

KDdouble CCUserDefault::getDoubleForKey ( const KDchar* szKey, KDdouble dDefault )
{
	const KDchar*  szValue = getValueForKey ( szKey );
	KDdouble       dRet = dDefault;

	if ( szValue )
	{
		dRet = kdStrtodKHR ( szValue, KD_NULL );
		xmlFree ( (KDvoid *) szValue );
	}

	return dRet;
}

std::string CCUserDefault::getStringForKey ( const KDchar* szKey )
{
	return getStringForKey ( szKey, "" );
}

std::string CCUserDefault::getStringForKey ( const KDchar* szKey, const std::string sDefault )
{
	const KDchar*  szValue = getValueForKey ( szKey );
	std::string    sRet = sDefault;

	if ( szValue )
	{
		sRet = std::string ( szValue );
		xmlFree( (KDvoid *) szValue );
	}

	return sRet;
}

KDvoid CCUserDefault::setBoolForKey ( const KDchar* szKey, KDbool bValue )
{
    // save bool value as sring
	if ( KD_TRUE == bValue )
	{
		setStringForKey ( szKey, "true" );
	}
	else
	{
		setStringForKey ( szKey, "false" );
	}
}

KDvoid CCUserDefault::setIntegerForKey ( const KDchar* szKey, KDint nValue )
{
	// check key
	if ( !szKey )
	{
		return;
	}

	// format the value
	KDchar  szTemp[50];
	kdSprintfKHR ( szTemp, "%d", nValue );

	setValueForKey ( szKey, szTemp );
}

KDvoid CCUserDefault::setFloatForKey ( const KDchar* szKey, KDfloat fValue )
{
	setDoubleForKey ( szKey, fValue );
}

KDvoid CCUserDefault::setDoubleForKey ( const KDchar* szKey, KDdouble dValue )
{
	// check key
	if ( !szKey )
	{
		return;
	}

	// format the value
    KDchar  szTemp[50];
	kdSprintfKHR ( szTemp, "%f", dValue );

	setValueForKey ( szKey, szTemp );
}

KDvoid CCUserDefault::setStringForKey ( const KDchar* szKey, std::string sValue )
{
	// check key
	if ( !szKey )
	{
		return;
	}

	setValueForKey ( szKey, sValue.c_str ( ) );
}

CCUserDefault* CCUserDefault::sharedUserDefault ( KDvoid )
{
	initXMLFilePath ( );

	// only create xml file one time
	// the file exists after the programe exit
	if ( (!isXMLFileExist ( ) ) && ( !createXMLFile ( ) ) )
	{
		return KD_NULL;
	}

	if ( !l_pSharedUserDefault )
	{
		l_pSharedUserDefault = new CCUserDefault ( );
	}

	return l_pSharedUserDefault;
}

KDbool CCUserDefault::isXMLFileExist ( KDvoid )
{
	KDFile*  pFile = kdFopen ( l_sFilePath.c_str ( ), "r" );
	KDbool   bRet  = KD_FALSE;

	if ( pFile )
	{
		bRet = KD_TRUE;
		kdFclose ( pFile );
	}

	return bRet;
}

KDvoid CCUserDefault::initXMLFilePath ( KDvoid )
{
	if ( !l_bIsFilePathInitialized )
	{
		l_sFilePath += CCFileUtils::sharedFileUtils ( )->getWritablePath ( ) + XML_FILE_NAME;
		l_bIsFilePathInitialized = KD_TRUE;
	}	
}

// create new xml file
KDbool CCUserDefault::createXMLFile ( KDvoid )
{
	KDbool     bRet = KD_FALSE;
    xmlDocPtr  pDoc = KD_NULL;

	do 
	{
		// new doc
		pDoc = xmlNewDoc ( BAD_CAST"1.0" );
		if ( pDoc == KD_NULL )
		{
			CCLOG ( "can not create xml doc" );
			break;
		}

		// new root node
		xmlNodePtr  pRootNode = xmlNewNode ( KD_NULL, BAD_CAST USERDEFAULT_ROOT_NAME );
		if ( pRootNode == KD_NULL )
		{
			CCLOG ( "can not create root node" );
			break;
		}

		// set root node
		xmlDocSetRootElement ( pDoc, pRootNode );

		// save xml file
		xmlSaveFile ( l_sFilePath.c_str ( ), pDoc );

		bRet = KD_TRUE;
        
	} while ( 0 );

	// if doc is not null, free it
	if ( pDoc )
	{
		xmlFreeDoc ( pDoc );
	}

	return bRet;
}

const std::string& CCUserDefault::getXMLFilePath ( KDvoid )
{
	return l_sFilePath;
}

KDvoid CCUserDefault::flush ( KDvoid )
{
    // save to file
    if ( l_pSharedDoc )
    {
		xmlSaveFile ( CCUserDefault::sharedUserDefault ( )->getXMLFilePath ( ).c_str ( ), l_pSharedDoc );
    }
}

NS_CC_END