/* -----------------------------------------------------------------------------------
 *
 *      File            CCUserDefault.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "support/user_default/CCUserDefault.h"
#include "platform/CCCommon.h"
#include "platform/CCFileUtils.h"
#include "support/base64.h"
#include <XMTinyXml/tinyxml2.h>
// root name of xml
#define USERDEFAULT_ROOT_NAME		"userDefaultRoot"
#define XML_FILE_NAME				"UserDefault.xml"

using namespace std;

NS_CC_BEGIN

/**
 * define the functions here because we don't want to
 * export xmlNodePtr and other types in "CCUserDefault.h"
 */

static tinyxml2::XMLElement* getXMLNodeForKey(const char* pKey, tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc)
{
    tinyxml2::XMLElement* curNode = NULL;

    // check the key value
    if (! pKey)
    {
        return NULL;
    }

    do 
    {
 		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
		*doc = xmlDoc;
		//CCFileData data(UserDefault::getInstance()->getXMLFilePath().c_str(),"rt");
		KDint32 nSize;
		char* pXmlBuffer = (char*)FileUtils::getInstance()->getFileData(UserDefault::getInstance()->getXMLFilePath().c_str(), "rb", &nSize);
		//const char* pXmlBuffer = (const char*)data.getBuffer();
		if(NULL == pXmlBuffer)
		{
			CCLOG("can not read xml file");
			break;
		}
		xmlDoc->Parse(pXmlBuffer, nSize);
        kdFree(pXmlBuffer);
		// get root node
		*rootNode = xmlDoc->RootElement();
		if (NULL == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (NULL != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}
	} while (0);

	return curNode;
}

static void setValueForKey(const char* pKey, const char* pValue)
{
 	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	// check the params
	if (! pKey || ! pValue)
	{
		return;
	}
	// find the node
	node = getXMLNodeForKey(pKey, &rootNode, &doc);
	// if node exist, change the content
	if (node)
	{
        if (node->FirstChild())
        {
            node->FirstChild()->SetValue(pValue);
        }
        else
        {
            tinyxml2::XMLText* content = doc->NewText(pValue);
            node->LinkEndChild(content);
        }
	}
	else
	{
		if (rootNode)
		{
			tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey);//new tinyxml2::XMLElement(pKey);
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(pValue);//new tinyxml2::XMLText(pValue);
			tmpNode->LinkEndChild(content);
		}	
	}

    // save file and free doc
	if (doc)
	{
		doc->SaveFile(UserDefault::getInstance()->getXMLFilePath().c_str());
		delete doc;
	}
}

/**
 * implements of UserDefault
 */

UserDefault* UserDefault::m_pUserDefault = 0;
string UserDefault::m_sFilePath = string("");
bool UserDefault::m_bIsFilePathInitialized = false;

/**
 * If the user invoke delete UserDefault::getInstance(), should set _userDefault
 * to null to avoid error when he invoke UserDefault::getInstance() later.
 */
UserDefault::~UserDefault ( KDvoid )
{
	CC_SAFE_DELETE ( m_pUserDefault );
}

UserDefault::UserDefault ( KDvoid )
{
	m_pUserDefault = KD_NULL;
}

bool UserDefault::getBoolForKey(const char* pKey)
{
 return getBoolForKey(pKey, false);
}

bool UserDefault::getBoolForKey(const char* pKey, bool defaultValue)
{
    const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	bool ret = defaultValue;

	if (value)
	{
		ret = ( !kdStrcmp(value, "true"));
	}

    if (doc) delete doc;

	return ret;
}

int UserDefault::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int UserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
	const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	int ret = defaultValue;

	if (value)
	{
		ret = kdAtoi(value);
	}

	if(doc)
	{
		delete doc;
	}


	return ret;
}

float UserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float UserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
    float ret = (float)getDoubleForKey(pKey, (double)defaultValue);
 
    return ret;
}

double  UserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double UserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
	const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	double ret = defaultValue;

	if (value)
	{
		ret = kdAtof(value);
	}

    if (doc) delete doc;

	return ret;
}

std::string UserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string UserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
    const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	string ret = defaultValue;

	if (value)
	{
		ret = string(value);
	}

    if (doc) delete doc;

	return ret;
}

Data* UserDefault::getDataForKey(const char* pKey)
{
    return getDataForKey(pKey, NULL);
}

Data* UserDefault::getDataForKey(const char* pKey, Data* defaultValue)
{
    const char* encodedData = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        encodedData = (const char*)(node->FirstChild()->Value());
	}
    
	Data* ret = defaultValue;
    
	if (encodedData)
	{
        unsigned char * decodedData = NULL;
        int decodedDataLen = base64Decode((unsigned char*)encodedData, (unsigned int)strlen(encodedData), &decodedData);
        
        if (decodedData) {
            ret = Data::create(decodedData, decodedDataLen);
        
            kdFree(decodedData);
        }
	}
    
    if (doc) delete doc;
    
	return ret;    
}


void UserDefault::setBoolForKey(const char* pKey, bool value)
{
    // save bool value as string

    if (true == value)
    {
        setStringForKey(pKey, "true");
    }
    else
    {
        setStringForKey(pKey, "false");
    }
}

void UserDefault::setIntegerForKey(const char* pKey, int value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    // format the value
    char tmp[50];
    kdMemset(tmp, 0, 50);
    kdSprintf(tmp, "%d", value);

    setValueForKey(pKey, tmp);
}

void UserDefault::setFloatForKey(const char* pKey, float value)
{
    setDoubleForKey(pKey, value);
}

void UserDefault::setDoubleForKey(const char* pKey, double value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    // format the value
    char tmp[50];
    kdMemset(tmp, 0, 50);
    kdSprintf(tmp, "%f", value);

    setValueForKey(pKey, tmp);
}

void UserDefault::setStringForKey(const char* pKey, const std::string & value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    setValueForKey(pKey, value.c_str());
}

void UserDefault::setDataForKey(const char* pKey, const Data& value) {
    // check key
    if (! pKey)
    {
        return;
    }

    char *encodedData = 0;
    
    base64Encode(value.getBytes(), value.getSize(), &encodedData);
        
    setValueForKey(pKey, encodedData);
    
    if (encodedData)
        kdFree(encodedData);
}

UserDefault* UserDefault::getInstance ( KDvoid )
{
    if ( !m_pUserDefault )
    {
        m_pUserDefault = new UserDefault ( );

		initXMLFilePath ( );

		// only create xml file one time
		// the file exists after the program exit
		if ( ( !isXMLFileExist ( ) ) && ( !createXMLFile ( ) ) )
		{
			return KD_NULL;
		}
    }

    return m_pUserDefault;
}

KDvoid UserDefault::destroyInstance ( KDvoid )
{
	CC_SAFE_DELETE ( m_pUserDefault );
}

KDbool UserDefault::isXMLFileExist ( KDvoid )
{
    KDFile*		fp = kdFopen ( m_sFilePath.c_str ( ), "r" );
	KDbool		bRet = false;

	if ( fp )
	{
		bRet = true;
		kdFclose ( fp );
	}

	return bRet;
}

KDvoid UserDefault::initXMLFilePath ( KDvoid )
{
    if ( !m_bIsFilePathInitialized )
    {
        m_sFilePath += FileUtils::getInstance ( )->getWritablePath ( ) + XML_FILE_NAME;
        m_bIsFilePathInitialized = true;
    }    
}

// create new xml file
KDbool UserDefault::createXMLFile ( KDvoid )
{
	KDbool						bRet = false;  
    tinyxml2::XMLDocument*		pDoc = new tinyxml2::XMLDocument ( ); 
    if ( KD_NULL == pDoc )  
    {  
        return false;  
    }  
	tinyxml2::XMLDeclaration*	pDeclaration = pDoc->NewDeclaration ( nullptr );  
	if ( KD_NULL == pDeclaration )  
	{  
		return false;  
	}  
	pDoc->LinkEndChild ( pDeclaration ); 
	tinyxml2::XMLElement*		pRootEle = pDoc->NewElement ( USERDEFAULT_ROOT_NAME );  
	if ( KD_NULL == pRootEle )  
	{  
		return false;  
	}  
	pDoc->LinkEndChild ( pRootEle );  
	bRet = tinyxml2::XML_SUCCESS == pDoc->SaveFile ( m_sFilePath.c_str ( ) );

	if ( pDoc )
	{
		delete pDoc;
	}

	return bRet;
}

const string& UserDefault::getXMLFilePath ( KDvoid )
{
    return m_sFilePath;
}

KDvoid UserDefault::flush ( KDvoid )
{

}

NS_CC_END

