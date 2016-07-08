/* -----------------------------------------------------------------------------------
 *
 *      File            CCFileUtils.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "support/CCFileUtils.h"
#include "support/CCDirector.h"
#include "support/CCSAXParser.h"
#include "cocoa/CCDictionary.h"

#include <XMXml/parser.h>
#include <XMXml/tree.h>
#include <XMXml/xmlmemory.h>

#include <XMTinyXml/tinyxml2.h>

#include <XMZlib/zlib.h>
#include <XMZlib/contrib/minizip/unzip.h>

#include <stack>
#include <algorithm>

NS_CC_BEGIN
    
typedef enum 
{
    SAX_NONE	= 0	,
    SAX_KEY			,
    SAX_DICT		,
    SAX_INT			,
    SAX_REAL		,
    SAX_STRING		,
    SAX_ARRAY		,
} CCSAXState;

typedef enum
{
    SAX_RESULT_NONE	= 0	,
    SAX_RESULT_DICT		,
    SAX_RESULT_ARRAY	,
} CCSAXResult;

class CCDictMaker : public CCSAXDelegator
{
	public :

		CCSAXResult					m_eResultType;
		CCArray*					m_pRootArray;
		CCDictionary*				m_pRootDict;
		CCDictionary*				m_pCurDict;
		std::stack<CCDictionary*>	m_tDictStack;
		std::string					m_sCurKey;		// parsed key
		std::string					m_sCurValue;	// parsed value
		CCSAXState					m_eState;
		CCArray*					m_pArray;
		
		std::stack<CCArray*>		m_tArrayStack;
		std::stack<CCSAXState>		m_tStateStack;

	public :
		
		CCDictMaker ( KDvoid )
		{
			m_eResultType	= SAX_RESULT_NONE;
			m_pRootArray	= KD_NULL;
            m_pRootDict		= KD_NULL;
            m_pCurDict		= KD_NULL;
            m_eState		= SAX_NONE;
            m_pArray		= KD_NULL;	
		}
		
		CCDictionary*		dictionaryWithContentsOfFile ( const KDchar* szFileName )
		{
			CCSAXParser  tParser;
		
			m_eResultType = SAX_RESULT_DICT;

			if ( KD_FALSE == tParser.init ( "UTF-8" ) )
			{
				return KD_NULL;
			}
			
			tParser.setDelegator ( this );
			
			tParser.parse ( szFileName );
			
			return m_pRootDict;
		}
		
		CCArray*		arrayWithContentsOfFile ( const KDchar* szFileName )
		{			
			CCSAXParser  tParser;

			m_eResultType = SAX_RESULT_ARRAY;

			if ( KD_FALSE == tParser.init ( "UTF-8" ) )
			{
				return KD_NULL;
			}

			tParser.setDelegator ( this );

			tParser.parse ( szFileName );

			return m_pArray;
		}

		KDvoid			startElement ( KDvoid* pCtx, const KDchar* szFilePath, const KDchar** ppAtts )
		{
			CC_UNUSED_PARAM ( pCtx );
			CC_UNUSED_PARAM ( ppAtts );

			std::string  sName = szFilePath;
            
			if ( sName == "dict" )
			{
				m_pCurDict = new CCDictionary ( );
				if ( m_eResultType == SAX_RESULT_DICT && !m_pRootDict )
				{
					m_pRootDict = m_pCurDict;
					m_pRootDict->retain ( );
				}
				m_eState = SAX_DICT;

				CCSAXState  ePreState = SAX_NONE;
				if ( !m_tStateStack.empty ( ) )
				{
					ePreState = m_tStateStack.top ( );
				}

				if ( SAX_ARRAY == ePreState )
				{
					// add the dictionary into the array
					m_pArray->addObject ( m_pCurDict );
				}
				else if ( SAX_DICT == ePreState )
				{
					// add the dictionary into the pre dictionary
					CCAssert ( !m_tDictStack.empty ( ), "The state is wrong!" );
					CCDictionary* pPreDict = m_tDictStack.top ( );
					pPreDict->setObject ( m_pCurDict, m_sCurKey.c_str ( ) );
				}
				m_pCurDict->release ( );

				// record the dict state
				m_tStateStack.push ( m_eState );
				m_tDictStack.push ( m_pCurDict );
			}
			else if ( sName == "key" )
			{
				m_eState = SAX_KEY;
			}
			else if ( sName == "integer" )
			{
				m_eState = SAX_INT;
			}
			else if ( sName == "real" )
			{
				m_eState = SAX_REAL;
			}
			else if ( sName == "string" )
			{
				m_eState = SAX_STRING;
			}
			else if ( sName == "array" )
			{
				m_eState = SAX_ARRAY;
				m_pArray = new CCArray ( );
				
				if ( m_eResultType == SAX_RESULT_ARRAY && m_pRootArray == KD_NULL )
				{
					m_pRootArray = m_pArray;
					m_pRootArray->retain ( );
				}

				CCSAXState  ePreState = SAX_NONE;
				
				if ( !m_tStateStack.empty ( ) )
				{
					ePreState = m_tStateStack.top ( );
				}

				if ( ePreState == SAX_DICT )
				{
					m_pCurDict->setObject ( m_pArray, m_sCurKey.c_str ( ) );
				}
				else if ( ePreState == SAX_ARRAY )
				{
					CCAssert ( !m_tArrayStack.empty ( ), "The state is wrong!" );
					CCArray*  pPreArray = m_tArrayStack.top ( );
					pPreArray->addObject ( m_pArray );
				}
				
				m_pArray->release ( );
				// record the array state
				m_tStateStack.push ( m_eState );
				m_tArrayStack.push ( m_pArray );
			}
			else
			{
				m_eState = SAX_NONE;
			}
		}

		KDvoid		endElement ( KDvoid* pCtx, const KDchar* szFilePath )
		{
			CC_UNUSED_PARAM ( pCtx );
            
			CCSAXState  eCurState = m_tStateStack.empty ( ) ? SAX_DICT : m_tStateStack.top ( );
    
            std::string  sName = szFilePath;
            
			if ( sName == "dict" )
			{
				m_tStateStack.pop ( );
				m_tDictStack.pop ( );
				if ( !m_tDictStack.empty ( ) )
				{
					m_pCurDict = m_tDictStack.top ( );
				}
			}
			else if ( sName == "array" )
			{
				m_tStateStack.pop ( );
				m_tArrayStack.pop ( );
				if ( !m_tArrayStack.empty ( ) )
				{
					m_pArray = m_tArrayStack.top ( );
				}
			}
			else if ( sName == "true" )
			{
				CCString*  pStr = new CCString ( "1" );
				if ( SAX_ARRAY == eCurState )
				{
					m_pArray->addObject ( pStr );
				}
				else if ( SAX_DICT == eCurState )
				{
					m_pCurDict->setObject ( pStr, m_sCurKey.c_str ( ) );
				}
				pStr->release ( );
			}
			else if ( sName == "false" )
			{
				CCString*  pStr = new CCString ( "0" );
				if ( SAX_ARRAY == eCurState )
				{
					m_pArray->addObject ( pStr );
				}
				else if ( SAX_DICT == eCurState )
				{
					m_pCurDict->setObject ( pStr, m_sCurKey.c_str ( ) );
				}
				pStr->release ( );
			}
			else if ( sName == "string" || sName == "integer" || sName == "real" )
			{
				CCString*  pStrValue = new CCString ( m_sCurValue );

				if ( SAX_ARRAY == eCurState )
				{
					m_pArray->addObject ( pStrValue );
				}
				else if ( SAX_DICT == eCurState )
				{
					m_pCurDict->setObject ( pStrValue, m_sCurKey.c_str ( ) );
				}

				pStrValue->release ( );
				m_sCurValue.clear ( );
			}
            
			m_eState = SAX_NONE;
		}

		KDvoid		textHandler ( KDvoid* pCtx, const KDchar* szCh, KDint nLen )
		{
			CC_UNUSED_PARAM ( pCtx );
			if ( m_eState == SAX_NONE )
			{
				return;
			}

			CCSAXState  eCurState = m_tStateStack.empty ( ) ? SAX_DICT : m_tStateStack.top ( );
			CCString*   pText = new CCString ( std::string ( (KDchar*) szCh, 0, nLen ) );

			switch ( m_eState )
			{
				case SAX_KEY :
					m_sCurKey = pText->getCString ( );
					break;
				
				case SAX_INT :
				case SAX_REAL :
				case SAX_STRING :
				{
					if ( eCurState == SAX_DICT )
					{
						CCAssert ( !m_sCurKey.empty ( ), "key not found : <integer/real>" );
					}

					m_sCurValue.append ( pText->getCString ( ) );

					break;					
				}				
				
				default : break;
			}
			pText->release ( );
		}
};

/*
 * forward statement
 */
static tinyxml2::XMLElement*	generateElementForArray ( CCArray*     pArray, tinyxml2::XMLDocument* pDoc );
static tinyxml2::XMLElement*	generateElementForDict  ( CCDictionary* pDict, tinyxml2::XMLDocument* pDoc );

/*
 * Use tinyxml2 to write plist files
 */
KDbool CCFileUtils::writeToFile ( CCDictionary* pDict, const std::string& sFullPath )
{
    tinyxml2::XMLDocument*		pDoc = new tinyxml2::XMLDocument ( );
    if ( KD_NULL == pDoc )
	{
        return KD_FALSE;
	}
    
    tinyxml2::XMLDeclaration*	pDeclaration = pDoc->NewDeclaration ( "xml version=\"1.0\" encoding=\"UTF-8\"" );
    if (NULL == pDeclaration)
    {
        delete pDoc;
        return KD_FALSE;
    }
    
    pDoc->LinkEndChild ( pDeclaration );

    tinyxml2::XMLElement*	pDocType = pDoc->NewElement ( "!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"" );
    pDoc->LinkEndChild ( pDocType );
    
    tinyxml2::XMLElement*	pRootEle = pDoc->NewElement ( "plist" );
    pRootEle->SetAttribute ( "version", "1.0" );
    if ( KD_NULL == pRootEle )
    {
        delete pDoc;
        return KD_FALSE;
    }
    pDoc->LinkEndChild ( pRootEle );
    
    tinyxml2::XMLElement*	pInnerDict = generateElementForDict ( pDict, pDoc );
    if ( NULL == pInnerDict )
    {
        delete pDoc;
        return false;
    }
    pRootEle->LinkEndChild ( pInnerDict );
    
    KDbool	bRet = tinyxml2::XML_SUCCESS == pDoc->SaveFile ( sFullPath.c_str ( ) );
    delete pDoc;

    return bRet;
}

/*
 * Generate tinyxml2::XMLElement for CCObject through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForObject ( CCObject* pObject, tinyxml2::XMLDocument* pDoc )
{
    // object is CCString
    if ( CCString* pStr = dynamic_cast<CCString*> ( pObject ) )
    {
        tinyxml2::XMLElement*	pNode = pDoc->NewElement ( "string" );
        tinyxml2::XMLText*		pContent = pDoc->NewText ( pStr->getCString ( ) );
        pNode->LinkEndChild ( pContent );
        return pNode;
    }
    
    // object is CCArray
    if ( CCArray* pArray = dynamic_cast<CCArray*> ( pObject ) )
	{
        return generateElementForArray ( pArray, pDoc );
	}
    
    // object is CCDictionary
    if ( CCDictionary* pInnerDict = dynamic_cast<CCDictionary*> ( pObject ) )
	{
        return generateElementForDict ( pInnerDict, pDoc );
	}
    
    CCLOG ( "This type cannot appear in property list" );
    return NULL;
}

/*
 * Generate tinyxml2::XMLElement for CCDictionary through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForDict ( CCDictionary* pDict, tinyxml2::XMLDocument* pDoc )
{
    tinyxml2::XMLElement*	pRootNode = pDoc->NewElement ( "dict" );
    
    CCDictElement*			pDictElement = KD_NULL;

    CCDICT_FOREACH ( pDict, pDictElement )
    {
        tinyxml2::XMLElement*	pTmpNode = pDoc->NewElement ( "key" );
        pRootNode->LinkEndChild ( pTmpNode );
        tinyxml2::XMLText*		pContent = pDoc->NewText ( pDictElement->getStrKey ( ) );
        pTmpNode->LinkEndChild ( pContent );
        
        CCObject*				pObject  = pDictElement->getObject ( );
        tinyxml2::XMLElement*	pElement = generateElementForObject ( pObject, pDoc );
        if ( pElement )
		{
            pRootNode->LinkEndChild ( pElement );
		}
    }
    return pRootNode;
}

/*
 * Generate tinyxml2::XMLElement for CCArray through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForArray ( CCArray* pArray, tinyxml2::XMLDocument* pDoc )
{
    tinyxml2::XMLElement*	pRootNode = pDoc->NewElement ( "array" );
    
    CCObject*				pObject = KD_NULL;
    CCARRAY_FOREACH ( pArray, pObject )
    {
        tinyxml2::XMLElement*	pElement = generateElementForObject ( pObject, pDoc );
        if ( pElement )
		{
            pRootNode->LinkEndChild ( pElement );
		}
    }
    return pRootNode;
}

CCDictionary* CCFileUtils::createCCDictionaryWithContentsOfFile ( const std::string& sFilename )
{
    std::string  sFullPath = fullPathForFilename ( sFilename.c_str ( ) );
    CCDictMaker  tMaker;
    return tMaker.dictionaryWithContentsOfFile ( sFullPath.c_str ( ) );
}

CCArray* CCFileUtils::createCCArrayWithContentsOfFile ( const std::string& sFilename )
{
	std::string  sFullPath = fullPathForFilename ( sFilename.c_str ( ) );
    CCDictMaker  tMaker;
    return tMaker.arrayWithContentsOfFile ( sFullPath.c_str ( ) );
}



static CCFileUtils*  l_pFileUtils = KD_NULL;

CCFileUtils* CCFileUtils::sharedFileUtils ( KDvoid )
{
    if ( l_pFileUtils == KD_NULL )
    {
        l_pFileUtils = new CCFileUtils ( );
		l_pFileUtils->init ( );
    }
    return l_pFileUtils;
}

KDvoid CCFileUtils::purgeFileUtils ( KDvoid )
{
    if ( l_pFileUtils != NULL)
    {
        l_pFileUtils->purgeCachedEntries ( );
    }
    
    CC_SAFE_DELETE ( l_pFileUtils );
}

CCFileUtils::CCFileUtils ( KDvoid )
{
	m_pFilenameLookupDict = KD_NULL;
}

CCFileUtils::~CCFileUtils ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pFilenameLookupDict );
}

KDbool CCFileUtils::init ( KDvoid )
{
	m_sDefaultResRootPath = "/res/";
    m_aSearchPathArray.push_back ( m_sDefaultResRootPath );
    m_aSearchResolutionsOrderArray.push_back ( "" );
    return KD_TRUE;
}

KDvoid CCFileUtils::purgeCachedEntries ( KDvoid )
{
	m_aFullPathCache.clear ( );
}

KDint32 CCFileUtils::getClassTypeInfo ( KDvoid )
{
	static const KDint32  nID = getHashCodeByString ( typeid ( CCFileUtils ).name ( ) );
	return nID;
}

KDubyte* CCFileUtils::getFileData ( const KDchar* szFilePath, const KDchar* szMode, KDsize* pSize )
{
    KDubyte*  pBuffer = KD_NULL;
	CCAssert ( szFilePath != KD_NULL && pSize != KD_NULL && szMode != KD_NULL, "Invalid parameters." );
	*pSize = 0;
    do 
    {
		// read the file from hardware
        std::string  sFullPath = fullPathForFilename ( szFilePath );

        // read the file from hardware
		KDFile*   pFile = kdFopen ( sFullPath.c_str ( ), szMode );
        CC_BREAK_IF ( !pFile );

        kdFseek ( pFile, 0, KD_SEEK_END );
        *pSize  = kdFtell ( pFile );
		kdFseek ( pFile, 0, KD_SEEK_SET );

        pBuffer = new KDubyte [ *pSize ];                
        *pSize = kdFread ( pBuffer, sizeof ( KDubyte ), *pSize, pFile );        

        kdFclose ( pFile );
        
    } while ( 0 );

    if ( !pBuffer )
    {
        //std::string  sMsg = "Get data from file(";
        //sMsg.append ( szFilePath ).append ( ") failed!" );

		//CCLOG ( sMsg.c_str ( ) );
    }
    
    return pBuffer;
}

KDubyte* CCFileUtils::getFileDataFromZip ( const KDchar* szZipPath, const KDchar* szFilePath, KDsize* pSize )
{
    KDubyte*  pBuffer = KD_NULL;
    unzFile   pFile   = KD_NULL;
    *pSize = 0;

    do 
    {
        CC_BREAK_IF ( !szZipPath || !szFilePath );
        CC_BREAK_IF ( kdStrlen ( szZipPath ) == 0 );

        pFile = unzOpen ( szZipPath );
        CC_BREAK_IF ( !pFile );

        KDint  nRet = unzLocateFile ( pFile, szFilePath, 1 );
        CC_BREAK_IF ( UNZ_OK != nRet );

        KDchar  szFilePathA[260];
        unz_file_info  tFileInfo;
        nRet = unzGetCurrentFileInfo ( pFile, &tFileInfo, szFilePathA, sizeof ( szFilePathA ), KD_NULL, 0, KD_NULL, 0 );
        CC_BREAK_IF ( UNZ_OK != nRet );

        nRet = unzOpenCurrentFile ( pFile );
        CC_BREAK_IF ( UNZ_OK != nRet );

        pBuffer = new KDubyte [ tFileInfo.uncompressed_size ];
        KDint nSize = 0;
        nSize = unzReadCurrentFile ( pFile, pBuffer, tFileInfo.uncompressed_size );
        CCAssert ( nSize == 0 || nSize == (KDint) tFileInfo.uncompressed_size, "the file size is wrong" );

        *pSize = tFileInfo.uncompressed_size;
        unzCloseCurrentFile ( pFile );
        
    } while ( 0 );

    if ( pFile )
    {
        unzClose ( pFile );
    }

    return pBuffer;
}

std::string CCFileUtils::getNewFilename ( const KDchar* szFileName )
{
    const KDchar*  szNewFileName = KD_NULL;
    // in Lookup Filename dictionary ?
    CCString*  pFileNameFound = m_pFilenameLookupDict ? (CCString*) m_pFilenameLookupDict->objectForKey ( szFileName ) : KD_NULL;
    if ( KD_NULL == pFileNameFound || pFileNameFound->length ( ) == 0 )
	{
        szNewFileName = szFileName;
    }
    else 
	{
        szNewFileName = pFileNameFound->getCString ( );
    }
    return szNewFileName;
}

std::string CCFileUtils::getPathForFilename ( const std::string& sFilename, const std::string& sResolutionDirectory, const std::string& sSearchPath )
{
    std::string  sFile = sFilename;
    std::string  sFilepath = "";
    KDsize  uPos = sFilename.find_last_of ( "/" );
    if ( uPos != std::string::npos )
    {
        sFilepath = sFilename.substr ( 0, uPos + 1 );
        sFile = sFilename.substr ( uPos + 1 );
    }
    
    // searchPath + file_path + resourceDirectory
    std::string  sPath = sSearchPath;
    sPath += sFilepath;
    sPath += sResolutionDirectory;
    
    sPath = getFullPathForDirectoryAndFilename ( sPath, sFile );
    
    return sPath;
}

std::string CCFileUtils::fullPathForFilename ( const KDchar* szFileName )
{
    CCAssert ( szFileName != KD_NULL, "CCFileUtils: Invalid path" );
    
    if ( isAbsolutePath ( szFileName ) )
    {
        return szFileName;
    }
    
    // Already Cached ?
    std::map<std::string, std::string>::iterator  cacheIter = m_aFullPathCache.find ( szFileName );
    if ( cacheIter != m_aFullPathCache.end ( ) )
    {
        return cacheIter->second;
    }
    
    // Get the new file name.
    std::string  sNewFilename = getNewFilename ( szFileName );
    
	std::string  sFullpath = "";
    
    for ( std::vector<std::string>::iterator  searchPathsIter = m_aSearchPathArray.begin ( ); searchPathsIter != m_aSearchPathArray.end ( ); ++searchPathsIter ) 
	{
        for ( std::vector<std::string>::iterator resOrderIter = m_aSearchResolutionsOrderArray.begin ( ); resOrderIter != m_aSearchResolutionsOrderArray.end ( ); ++resOrderIter )
		{            
            sFullpath = this->getPathForFilename ( sNewFilename, *resOrderIter, *searchPathsIter );
            
            if ( sFullpath.length ( ) > 0 )
            {
                // Using the filename passed in as key.
                m_aFullPathCache.insert ( std::pair<std::string, std::string> ( szFileName, sFullpath ) );
                return sFullpath;
            }
        }
    }
    
    // The file wasn't found, return the file name passed in.
    return szFileName;
}

const KDchar* CCFileUtils::fullPathFromRelativeFile ( const KDchar* szFileName, const KDchar* szRelativeFile )
{
	CCString*  pRet = CCString::create ( "" );

	if ( szFileName [ 0 ] == '/' )
	{
		pRet->m_sString = szFileName;
	}
	else
	{
		std::string  sRelateFile = szRelativeFile;

		pRet->m_sString  = sRelateFile.substr ( 0, sRelateFile.rfind ( '/' ) + 1 );
		pRet->m_sString += getNewFilename ( szFileName );
	}
 
	return pRet->getCString ( );
}

const KDchar* CCFileUtils::fullPathFromWritablePath ( const KDchar* szFileName )
{
	CCString*  pRet = CCString::create ( "" );

	if ( szFileName [ 0 ] == '/' )
	{
		pRet->m_sString = szFileName;
	}
	else
	{
		pRet->m_sString = "/data/" + std::string ( szFileName );
	}
 
	return pRet->getCString ( );
}

KDvoid CCFileUtils::setSearchResolutionsOrder ( const std::vector<std::string>& aSearchResolutionsOrder )
{
    KDbool  bExistDefault = KD_FALSE;
	m_aFullPathCache.clear ( ); 
    m_aSearchResolutionsOrderArray.clear ( );
    for ( std::vector<std::string>::const_iterator iter = aSearchResolutionsOrder.begin ( ); iter != aSearchResolutionsOrder.end ( ); ++iter )
    {
        std::string  sResolutionDirectory = *iter;
        if ( !bExistDefault && sResolutionDirectory == "" )
        {
            bExistDefault = KD_TRUE;
        }
        
        if ( sResolutionDirectory.length ( ) > 0 && sResolutionDirectory [ sResolutionDirectory.length ( ) - 1 ] != '/' )
        {
            sResolutionDirectory += "/";
        }
        
        m_aSearchResolutionsOrderArray.push_back ( sResolutionDirectory );
    }

    if ( !bExistDefault )
    {
        m_aSearchResolutionsOrderArray.push_back ( "" );
    }
}

KDvoid CCFileUtils::addSearchResolutionsOrder ( const KDchar* szOrder )
{
    m_aSearchResolutionsOrderArray.push_back ( szOrder );
}

const std::vector<std::string>& CCFileUtils::getSearchResolutionsOrder ( KDvoid )
{
    return m_aSearchResolutionsOrderArray;
}

const std::vector<std::string>& CCFileUtils::getSearchPaths ( KDvoid )
{
    return m_aSearchPathArray;
}

KDvoid CCFileUtils::setSearchPaths ( const std::vector<std::string>& aSearchPaths )
{
    KDbool  bExistDefaultRootPath = KD_FALSE;
    
	m_aFullPathCache.clear ( ); 
    m_aSearchPathArray.clear ( );
    for ( std::vector<std::string>::const_iterator iter = aSearchPaths.begin ( ); iter != aSearchPaths.end ( ); ++iter )
    {
        std::string  sPrefix;
        std::string  sPath;
        if ( !isAbsolutePath ( *iter ) )
        { 
			// Not an absolute path
            sPrefix = m_sDefaultResRootPath;
        }
        sPath = sPrefix + ( *iter );
        if ( sPath.length ( ) > 0 && sPath [ sPath.length ( ) - 1 ] != '/' )
        {
            sPath += "/";
        }
        if ( !bExistDefaultRootPath && sPath == m_sDefaultResRootPath )
        {
            bExistDefaultRootPath = KD_TRUE;
        }
        m_aSearchPathArray.push_back ( sPath );
    }
    
    if ( !bExistDefaultRootPath )
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_aSearchPathArray.push_back ( m_sDefaultResRootPath );
    }
}

KDvoid CCFileUtils::addSearchPath ( const KDchar* szPath )
{
    std::string  sPrefix;
    std::string  sPath ( szPath );
    if ( !isAbsolutePath ( sPath ) )
    { 
		// Not an absolute path
        sPrefix = m_sDefaultResRootPath;
    }
    sPath = sPrefix + sPath;
    if ( sPath.length ( ) > 0 && sPath [ sPath.length ( ) - 1 ] != '/' )
    {
        sPath += "/";
    }
    m_aSearchPathArray.push_back ( sPath );
}

KDvoid CCFileUtils::removeSearchPath ( const KDchar* szPath )
{
	std::string		sPrefix;
	std::string		sPath ( szPath );
	if ( !isAbsolutePath ( sPath ) )
	{
		// Not an absolute path
		sPrefix = m_sDefaultResRootPath;
	}
	sPath = sPrefix + sPath;
	if ( sPath.length() > 0 && sPath [ sPath.length ( ) - 1 ] != '/' )
	{
		sPath += "/";
	}
	std::vector<std::string>::iterator	iter = std::find ( m_aSearchPathArray.begin(), m_aSearchPathArray.end ( ), sPath );
	m_aSearchPathArray.erase ( iter );
}

KDvoid CCFileUtils::removeAllPaths ( KDvoid )
{
	m_aSearchPathArray.clear ( );
}

KDvoid CCFileUtils::setFilenameLookupDictionary ( CCDictionary* pFilenameLookupDict )
{
    CC_SAFE_RELEASE ( m_pFilenameLookupDict );
    m_pFilenameLookupDict = pFilenameLookupDict;
    CC_SAFE_RETAIN ( m_pFilenameLookupDict );
}

KDvoid CCFileUtils::loadFilenameLookupDictionaryFromFile ( const KDchar* szFilename )
{
    std::string  sFullPath = this->fullPathForFilename ( szFilename );
    if ( sFullPath.length ( ) > 0 )
    {
        CCDictionary*  pDict = CCDictionary::createWithContentsOfFile ( sFullPath.c_str ( ) );
        if ( pDict )
        {
            CCDictionary*  pMetadata = (CCDictionary*) pDict->objectForKey ( "metadata" );
            KDint  nVersion = ( (CCString*) pMetadata->objectForKey ( "version" ) )->intValue ( );
            if ( nVersion != 1 )
            {
                CCLOG ( "XMCocos2D : ERROR: Invalid filenameLookup dictionary version: %d. Filename: %s", nVersion, szFilename );
                return;
            }
            setFilenameLookupDictionary ( (CCDictionary*) pDict->objectForKey ( "filenames" ) );
        }
    }
}

std::string CCFileUtils::getFullPathForDirectoryAndFilename ( const std::string& sDirectory, const std::string& sFilename )
{
    std::string  sRet = sDirectory + sFilename;
    if ( !isFileExist ( sRet ) )
	{
        sRet = "";
    }
    return sRet;
}

KDbool CCFileUtils::isAbsolutePath ( const std::string& sPath )
{
    return sPath[0] == '/' ? KD_TRUE : KD_FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static KDbool   l_bPopupNotify = KD_TRUE;

KDvoid CCFileUtils::setPopupNotify ( KDbool bNotify )
{
    l_bPopupNotify = bNotify;
}

KDbool CCFileUtils::isPopupNotify ( KDvoid )
{
    return l_bPopupNotify;
}



std::string CCFileUtils::getWritablePath ( KDvoid )
{
	return std::string ( "/data/" );
}

KDbool CCFileUtils::isFileExist ( const std::string& sFilePath )
{
	KDbool   bFound = KD_FALSE;
	KDFile*  pFile = kdFopen ( sFilePath.c_str ( ), "r" );
    if ( pFile )
    {
        bFound = KD_TRUE;
        kdFclose ( pFile );
    }
	return bFound;
}
    
NS_CC_END