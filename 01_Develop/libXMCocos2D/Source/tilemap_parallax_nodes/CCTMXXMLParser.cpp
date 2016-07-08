/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXXMLParser.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011		¬®¬Ñ¬Ü¬ã¬Ú¬Þ ¬¡¬Ü¬ã¬Ö¬ß¬à¬Ó
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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
#include "tilemap_parallax_nodes/CCTMXXMLParser.h"
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "support/zip_support/ZipUtils.h"
#include "support/base64.h"
#include "support/CCFileUtils.h"
#include "support/CCPointExtension.h"
#include <map>
#include <sstream>

NS_CC_BEGIN

inline static const KDchar* stringForKey ( std::map<std::string, std::string>* pDictionary, const KDchar* szKey )
{
	std::map<std::string, std::string>::iterator  iter = pDictionary->find ( szKey );
    
    if ( iter != pDictionary->end ( ) )
    {
        return iter->second.c_str ( );
    }
    else
    {
        return "";
    }
}

inline static KDuint uintForKey ( std::map<std::string, std::string>* pDictionary, const KDchar* szKey )
{
    std::map<std::string, std::string>::iterator  iter = pDictionary->find ( szKey );
    
    if ( iter != pDictionary->end ( ) )
    {
        return kdStrtoul ( iter->second.c_str ( ), KD_NULL, 0 );
    }
    else
    {
        return 0;
    }
}

inline static KDint intForKey ( std::map<std::string, std::string>* pDictionary, const KDchar* szKey )
{
    std::map<std::string, std::string>::iterator  iter = pDictionary->find ( szKey );
    
    if ( iter != pDictionary->end ( ) )
    {
        return kdStrtol ( iter->second.c_str ( ), KD_NULL, 0 );
    }
    else
    {
        return 0;
    }
}

inline static KDfloat floatForKey ( std::map<std::string, std::string>* pDictionary, const KDchar* szKey )
{
	std::map<std::string, std::string>::iterator  iter = pDictionary->find ( szKey );
	
    if ( iter != pDictionary->end ( ) )
    {
        return (KDfloat) kdStrtodKHR ( iter->second.c_str ( ), KD_NULL );
    }
    else
    {
        return 0.0f;
    }
}

//
// implementation CCTMXLayerInfo
//
CCTMXLayerInfo::CCTMXLayerInfo ( KDvoid )
{
    m_sName       = "";
    m_pTiles      = KD_NULL;
    m_bOwnTiles   = KD_TRUE;
    m_uMinGID     = 100000;
    m_uMaxGID     = 0;
    m_tOffset     = CCPointZero;
	m_pProperties = new CCDictionary ( );
}

CCTMXLayerInfo::~CCTMXLayerInfo ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCTMXLayerInfo = %p", this );
	CC_SAFE_RELEASE ( m_pProperties );

	if ( m_bOwnTiles )
	{
		CC_SAFE_DELETE_ARRAY ( m_pTiles );
	}
}

CCDictionary* CCTMXLayerInfo::getProperties ( KDvoid )
{
	return m_pProperties;
}

KDvoid CCTMXLayerInfo::setProperties ( CCDictionary* pProperties )
{
	CC_SAFE_RETAIN  ( pProperties );
	CC_SAFE_RELEASE ( m_pProperties );
	m_pProperties = pProperties;
}

//
// implementation CCTMXTilesetInfo
//
CCTMXTilesetInfo::CCTMXTilesetInfo ( KDvoid )
{
    m_uFirstGid  = 0;
    m_tTileSize  = CCSizeZero;
    m_uSpacing   = 0;
    m_uMargin    = 0;
    m_tImageSize = CCSizeZero;
}

CCTMXTilesetInfo::~CCTMXTilesetInfo ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCTMXTilesetInfo = %p", this );
}

CCRect CCTMXTilesetInfo::rectForGID ( KDuint uGID )
{
	CCRect  tRect;

	tRect.size = m_tTileSize;

    uGID &= kCCFlippedMask;
    uGID  = uGID - m_uFirstGid;

	KDint  nMax = (KDint) ( ( m_tImageSize.cx - m_uMargin * 2 + m_uSpacing ) / ( m_tTileSize.cx + m_uSpacing ) );
    
	tRect.origin.x = ( uGID % nMax ) * ( m_tTileSize.cx + m_uSpacing ) + m_uMargin;
	tRect.origin.y = ( uGID / nMax ) * ( m_tTileSize.cy + m_uSpacing ) + m_uMargin;

	return tRect;
}

//
// implementation CCTMXMapInfo
//
CCTMXMapInfo* CCTMXMapInfo::createWithTMXFile ( const KDchar* szTMXPath )
{
	CCTMXMapInfo*  pRet = new CCTMXMapInfo ( );

	if ( pRet && pRet->initWithTMXFile ( szTMXPath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCTMXMapInfo* CCTMXMapInfo::createWithXML ( const KDchar* szTMXString, const KDchar* szResPath )
{
	CCTMXMapInfo*  pRet = new CCTMXMapInfo ( );

	if ( pRet && pRet->initWithXML ( szTMXString, szResPath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid CCTMXMapInfo::internalInit ( const KDchar* szTMXFilename, const KDchar* szResPath )
{
    m_pTilesets = CCArray::create ( );
    m_pTilesets->retain ( );

    m_pLayers = CCArray::create ( );
    m_pLayers->retain ( );

    if ( szTMXFilename != KD_NULL )
    {
		m_sTMXFileName = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szTMXFilename );
    }
    
    if ( szResPath != KD_NULL )
    {
        m_sResources = szResPath;
    }
    
    m_pObjectGroups = CCArray::createWithCapacity ( 4 );
    m_pObjectGroups->retain ( );

    m_pProperties     = new CCDictionary ( );
    m_pTileProperties = new CCDictionary ( );

    // tmp vars
    m_sCurrentString	 = "";
    m_bStoringCharacters = KD_FALSE;
    m_nLayerAttribs		 = TMXLayerAttribNone;
    m_nParentElement	 = TMXPropertyNone;
	m_uCurrentFirstGID	 = 0;
}

KDbool CCTMXMapInfo::initWithXML ( const KDchar* szTMXString, const KDchar* szResPath )
{
    internalInit ( KD_NULL, szResPath );
    return parseXMLString ( szTMXString );
}

KDbool CCTMXMapInfo::initWithTMXFile ( const KDchar* szXMLPath )
{
    internalInit ( szXMLPath, KD_NULL );
    return parseXMLFile ( m_sTMXFileName.c_str ( ) );
}

CCTMXMapInfo::CCTMXMapInfo ( KDvoid )
{
    m_tMapSize           = CCSizeZero;
    m_tTileSize          = CCSizeZero;
    m_pLayers            = KD_NULL;
    m_pTilesets          = KD_NULL;
    m_pObjectGroups      = KD_NULL;
    m_nLayerAttribs      = 0;
    m_bStoringCharacters = KD_FALSE;
    m_pProperties        = KD_NULL;
    m_pTileProperties    = KD_NULL;
	m_uCurrentFirstGID	 = 0;
}

CCTMXMapInfo::~CCTMXMapInfo ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCTMXMapInfo = %p", this );
    
	CC_SAFE_RELEASE ( m_pTilesets );
	CC_SAFE_RELEASE ( m_pLayers );
	CC_SAFE_RELEASE ( m_pProperties );
	CC_SAFE_RELEASE ( m_pTileProperties );
	CC_SAFE_RELEASE ( m_pObjectGroups );
}

CCArray* CCTMXMapInfo::getLayers ( KDvoid )
{
	return m_pLayers;
}

KDvoid CCTMXMapInfo::setLayers ( CCArray* pLayers )
{
	CC_SAFE_RETAIN  ( pLayers );
	CC_SAFE_RELEASE ( m_pLayers );
	m_pLayers = pLayers;
}

CCArray* CCTMXMapInfo::getTilesets ( KDvoid )
{
	return m_pTilesets;
}

KDvoid CCTMXMapInfo::setTilesets ( CCArray* pTilesets )
{
	CC_SAFE_RETAIN  ( pTilesets );
	CC_SAFE_RELEASE ( m_pTilesets );
	m_pTilesets = pTilesets;
}

CCArray* CCTMXMapInfo::getObjectGroups ( KDvoid )
{
	return m_pObjectGroups;
}

KDvoid CCTMXMapInfo::setObjectGroups ( CCArray* pObjectGroups )
{
	CC_SAFE_RETAIN  ( pObjectGroups );
	CC_SAFE_RELEASE ( m_pObjectGroups );
	m_pObjectGroups = pObjectGroups;
}

CCDictionary* CCTMXMapInfo::getProperties ( KDvoid )
{
	return m_pProperties;
}

KDvoid CCTMXMapInfo::setProperties ( CCDictionary* pProperties )
{
	CC_SAFE_RETAIN  ( pProperties );
	CC_SAFE_RELEASE ( m_pProperties );
	m_pProperties = pProperties;
}

CCDictionary* CCTMXMapInfo::getTileProperties ( KDvoid )
{
	return m_pTileProperties;
}

KDvoid CCTMXMapInfo::setTileProperties ( CCDictionary* pTileProperties )
{
	CC_SAFE_RETAIN  ( pTileProperties );
	CC_SAFE_RELEASE ( m_pTileProperties );
	m_pTileProperties = pTileProperties;
}

KDbool CCTMXMapInfo::parseXMLString ( const KDchar* szXMLString )
{
    KDint  nLen = kdStrlen ( szXMLString );

    if ( szXMLString == KD_NULL || nLen <= 0) 
    {
        return KD_FALSE;
    }

    CCSAXParser  tParser;

    if ( KD_FALSE == tParser.init ( "UTF-8" ) )
    {
        return KD_FALSE;
    }

    tParser.setDelegator ( this );

    return tParser.parse ( szXMLString, nLen );
}

KDbool CCTMXMapInfo::parseXMLFile ( const KDchar* szXMLPath )
{
	CCSAXParser  tParser;

	if ( tParser.init ( "UTF-8" ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	tParser.setDelegator ( this );

	return tParser.parse ( CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szXMLPath ).c_str ( ) );
}

/// the XML parser calls here with all the elements
KDvoid CCTMXMapInfo::startElement ( KDvoid* pCtx, const KDchar* szName, const KDchar** pAtts )
{	
	CC_UNUSED_PARAM ( pCtx );

    CCSize  tSize;
	CCTMXMapInfo*  pTMXMapInfo = this;
	std::string    sElementName = szName;
	std::map<std::string, std::string>*  pAttributeDict = new std::map<std::string, std::string> ( );

	if ( pAtts && pAtts [ 0 ] )
	{
		for ( KDint i = 0; pAtts [ i ]; i += 2 ) 
		{
			std::string  sKey   = (KDchar *) pAtts [ i + 0 ];
			std::string  sValue = (KDchar *) pAtts [ i + 1 ];

			pAttributeDict->insert ( std::pair<std::string, std::string> ( sKey, sValue ) );
		}
	}
	if ( sElementName == "map" )
	{
		std::string  sVersion = stringForKey ( pAttributeDict, "version" );
		if ( sVersion != "1.0" )
		{
			CCLOG ( "XMCcos2D : TMXFormat - Unsupported TMX version: %s", sVersion.c_str ( ) );
		}
        
		std::string  sOrientationStr = stringForKey ( pAttributeDict, "orientation" );
		if      ( sOrientationStr == "orthogonal" )
		{
			pTMXMapInfo->setOrientation ( CCTMXOrientationOrtho );
		}
		else if ( sOrientationStr == "isometric" )
		{
			pTMXMapInfo->setOrientation ( CCTMXOrientationIso );
		}
		else if ( sOrientationStr == "hexagonal" )
		{
			pTMXMapInfo->setOrientation ( CCTMXOrientationHex );
		}
		else
		{
			CCLOG ( "XCocos2D : TMXFomat - Unsupported orientation: %d", pTMXMapInfo->getOrientation ( ) );
		}

		tSize.cx = floatForKey ( pAttributeDict, "width"  );
		tSize.cy = floatForKey ( pAttributeDict, "height" );
		pTMXMapInfo->setMapSize ( tSize );

		tSize.cx = floatForKey ( pAttributeDict, "tilewidth"  );
		tSize.cy = floatForKey ( pAttributeDict, "tileheight" );
		pTMXMapInfo->setTileSize ( tSize );

		// The parent element is now "map"
		pTMXMapInfo->setParentElement ( TMXPropertyMap );
	} 
	else if ( sElementName == "tileset" ) 
	{
		// If this is an external tileset then start parsing that
		std::string  sExternalTilesetFilename = stringForKey ( pAttributeDict, "source" );
		if ( sExternalTilesetFilename != "" )
		{
			// Tileset file will be relative to the map file. So we need to convert it to an absolute path
			if ( m_sTMXFileName.find_last_of ( "/" ) != std::string::npos )
            {
				std::string  sDir = m_sTMXFileName.substr ( 0, m_sTMXFileName.find_last_of ( "/" ) + 1 );
                sExternalTilesetFilename = sDir + sExternalTilesetFilename;
            }
            else 
            {
                sExternalTilesetFilename = m_sResources + "/" + sExternalTilesetFilename;
            }
			sExternalTilesetFilename = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( sExternalTilesetFilename.c_str ( ) );
            
			pTMXMapInfo->parseXMLFile ( sExternalTilesetFilename.c_str ( ) );
		}
		else
		{
			CCTMXTilesetInfo*  pTileset = new CCTMXTilesetInfo ( );
			pTileset->m_sName = stringForKey ( pAttributeDict, "name" );
			if ( m_uCurrentFirstGID == 0 )
			{
				pTileset->m_uFirstGid = uintForKey ( pAttributeDict, "firstgid" );
			}
			else
			{
				pTileset->m_uFirstGid = m_uCurrentFirstGID;
				m_uCurrentFirstGID = 0;
			}
			
			pTileset->m_uSpacing = uintForKey ( pAttributeDict, "spacing" );
			pTileset->m_uMargin  = uintForKey ( pAttributeDict, "margin"  );

			tSize.cx = floatForKey ( pAttributeDict, "tilewidth"  );
			tSize.cy = floatForKey ( pAttributeDict, "tileheight" );
			pTileset->m_tTileSize = tSize;

			pTMXMapInfo->getTilesets ( )->addObject ( pTileset );
			pTileset->release ( );
		}
	}
	else if ( sElementName == "tile" )
	{
		CCTMXTilesetInfo*  pInfo = (CCTMXTilesetInfo*) pTMXMapInfo->getTilesets ( )->lastObject ( );
		CCDictionary*      pDict = new CCDictionary ( );
        
		pTMXMapInfo->setParentGID ( pInfo->m_uFirstGid + intForKey ( pAttributeDict, "id" ) );
		pTMXMapInfo->getTileProperties ( )->setObject ( pDict, pTMXMapInfo->getParentGID ( ) );
		CC_SAFE_RELEASE ( pDict );
		
		pTMXMapInfo->setParentElement ( TMXPropertyTile );
	}
	else if ( sElementName == "layer" )
	{
		CCTMXLayerInfo*  pLayer = new CCTMXLayerInfo ( );
		pLayer->m_sName = stringForKey ( pAttributeDict, "name" );

		tSize.cx = floatForKey ( pAttributeDict, "width"  );
		tSize.cy = floatForKey ( pAttributeDict, "height" );
		pLayer->m_tLayerSize = tSize;

		std::string  sVisible = stringForKey ( pAttributeDict, "visible" );
		pLayer->m_bVisible = ! ( sVisible == "0" );

		std::string  sOpacity = stringForKey ( pAttributeDict, "opacity" );
		if ( sOpacity != "" )
		{
			pLayer->m_cOpacity = ( KDubyte ) ( 255 * kdStrtodKHR ( sOpacity.c_str ( ), KD_NULL ) );
		}
		else
		{
			pLayer->m_cOpacity = 255;
		}
        
		pLayer->m_tOffset = ccp ( floatForKey ( pAttributeDict, "x" ), floatForKey ( pAttributeDict, "y" ) );

		pTMXMapInfo->getLayers ( )->addObject ( pLayer );
		pLayer->release ( );

		// The parent element is now "layer"
		pTMXMapInfo->setParentElement ( TMXPropertyLayer );
	} 
	else if ( sElementName == "objectgroup" )
	{
		CCTMXObjectGroup*  pObjectGroup = new CCTMXObjectGroup ( );
		pObjectGroup->setGroupName ( stringForKey ( pAttributeDict, "name" ) );
        
		CCPoint  tPositionOffset;
		tPositionOffset.x = floatForKey ( pAttributeDict, "x" ) * pTMXMapInfo->getTileSize ( ).cx;
		tPositionOffset.y = floatForKey ( pAttributeDict, "y" ) * pTMXMapInfo->getTileSize ( ).cy;
		pObjectGroup->setPositionOffset ( tPositionOffset );

		pTMXMapInfo->getObjectGroups ( )->addObject ( pObjectGroup );
		pObjectGroup->release ( );

		// The parent element is now "objectgroup"
		pTMXMapInfo->setParentElement ( TMXPropertyObjectGroup );
	}
	else if ( sElementName == "image" )
	{
		CCTMXTilesetInfo*  pTileset = (CCTMXTilesetInfo*) pTMXMapInfo->getTilesets ( )->lastObject ( );
		std::string  sImagename = stringForKey ( pAttributeDict, "source" );	

		if ( m_sTMXFileName.find_last_of ( "/" ) != std::string::npos )
        {
			std::string  sDir = m_sTMXFileName.substr ( 0, m_sTMXFileName.find_last_of ( "/" ) + 1 );
            pTileset->m_sSourceImage = sDir + sImagename;
        }
        else 
        {
            pTileset->m_sSourceImage = m_sResources + ( m_sResources.size ( ) ? "/" : "" ) + sImagename;
        }
	} 
	else if ( sElementName == "data" )
	{
		std::string  sEncoding    = stringForKey ( pAttributeDict, "encoding" );
		std::string  sCompression = stringForKey ( pAttributeDict, "compression" );

		if ( sEncoding == "base64" )
		{
			KDint  nLayerAttribs = pTMXMapInfo->getLayerAttribs ( );
			pTMXMapInfo->setLayerAttribs ( nLayerAttribs | TMXLayerAttribBase64 );
			pTMXMapInfo->setStoringCharacters ( KD_TRUE );

			if      ( sCompression == "gzip" )
			{
				nLayerAttribs = pTMXMapInfo->getLayerAttribs ( );
				pTMXMapInfo->setLayerAttribs ( nLayerAttribs | TMXLayerAttribGzip );
			}
			else if ( sCompression == "zlib" )
			{
				nLayerAttribs = pTMXMapInfo->getLayerAttribs ( );
				pTMXMapInfo->setLayerAttribs ( nLayerAttribs | TMXLayerAttribZlib );
			}
            
			CCAssert ( sCompression == "" || sCompression == "gzip" || sCompression == "zlib", "TMX: unsupported compression method" );
		}
        
		CCAssert ( pTMXMapInfo->getLayerAttribs ( ) != TMXLayerAttribNone, "TMX tile map: Only base64 and/or gzip/zlib maps are supported" );
	} 
	else if ( sElementName == "object" )
	{
		KDchar  szBuffer [ 32 ] = { 0 };
		CCTMXObjectGroup*  pObjectGroup = (CCTMXObjectGroup*) pTMXMapInfo->getObjectGroups ( )->lastObject ( );

		// The value for "type" was blank or not a valid class name
		// Create an instance of TMXObjectInfo to store the object and its properties
		CCDictionary*  pDict = new CCDictionary ( );

		// Parse everything automatically
        const KDchar*  pArray [ ] = { "name", "type", "width", "height", "gid" };

		for ( KDsize i = 0; i < sizeof ( pArray ) / sizeof ( pArray [ 0 ] ); ++i )
        {
            const KDchar*  szKey   = pArray [ i ];
            CCString*      pObject = new CCString ( stringForKey ( pAttributeDict, szKey ) );
            if ( pObject )
            {
                pObject->autorelease ( );
                pDict->setObject ( pObject, szKey );
            }
        }

		// But X and Y since they need special treatment
        // X
		const KDchar*  szValue;

		szValue = stringForKey ( pAttributeDict, "x" );
		if ( szValue )
		{
			KDint  x = kdStrtol ( szValue, 0, 0 ) + (KDint) pObjectGroup->getPositionOffset ( ).x;
			kdSprintfKHR ( szBuffer, "%d", x );

			CCString*  pStr = new CCString ( szBuffer );
			pStr->autorelease ( );
			pDict->setObject ( pStr, "x" );
		}

		// Y
		szValue = stringForKey ( pAttributeDict, "y" );
		if ( szValue )
		{
			KDint  y = kdStrtol ( szValue, 0, 0 ) + (KDint) pObjectGroup->getPositionOffset ( ).y;

			// Correct y position. (Tiled uses Flipped, cocos2d uses Standard)
            y = (KDint) ( m_tMapSize.cy * m_tTileSize.cy ) - y - kdStrtol ( stringForKey ( pAttributeDict, "height" ), 0, 0 );
			kdSprintfKHR ( szBuffer, "%d", y );

			CCString*  pStr = new CCString ( szBuffer );
			pStr->autorelease ( );
			pDict->setObject ( pStr, "y" );
		}

		// Add the object to the objectGroup
		pObjectGroup->getObjects ( )->addObject ( pDict );
		pDict->release ( );

		// The parent element is now "object"
		pTMXMapInfo->setParentElement ( TMXPropertyObject );
	} 
	else if ( sElementName == "property" )
	{
		if ( pTMXMapInfo->getParentElement ( ) == TMXPropertyNone ) 
		{
			CCLOG ( "TMX tile map: Parent element is unsupported. Cannot add property named '%s' with value '%s'",
				stringForKey ( pAttributeDict, "name" ), stringForKey ( pAttributeDict, "value" ) );
		} 
		else if ( pTMXMapInfo->getParentElement ( ) == TMXPropertyMap )
		{
			// The parent element is the map
			CCString*  pValue = new CCString ( stringForKey ( pAttributeDict, "value" ) );
			std::string  skey = stringForKey ( pAttributeDict, "name" );
			pTMXMapInfo->getProperties ( )->setObject ( pValue, skey.c_str ( ) );
			pValue->release ( );
		} 
		else if ( pTMXMapInfo->getParentElement ( ) == TMXPropertyLayer )
		{
			// The parent element is the last layer
			CCTMXLayerInfo*  pLayer = (CCTMXLayerInfo*) pTMXMapInfo->getLayers ( )->lastObject ( );
			CCString*  pValue = new CCString ( stringForKey ( pAttributeDict, "value" ) );
			std::string  sKey = stringForKey ( pAttributeDict, "name" );
			// Add the property to the layer
			pLayer->getProperties ( )->setObject ( pValue, sKey.c_str ( ) );
			pValue->release ( );
		} 
		else if ( pTMXMapInfo->getParentElement ( ) == TMXPropertyObjectGroup ) 
		{
			// The parent element is the last object group
			CCTMXObjectGroup*  pObjectGroup = (CCTMXObjectGroup*) pTMXMapInfo->getObjectGroups ( )->lastObject ( );
			CCString*  pValue = new CCString ( stringForKey ( pAttributeDict, "value" ) );
			std::string  sKey = stringForKey ( pAttributeDict, "name" );
			pObjectGroup->getProperties ( )->setObject ( pValue, sKey.c_str ( ) );
			pValue->release ( );
		} 
		else if ( pTMXMapInfo->getParentElement ( ) == TMXPropertyObject )
		{
			// The parent element is the last object
			CCTMXObjectGroup*  pObjectGroup = (CCTMXObjectGroup*) pTMXMapInfo->getObjectGroups ( )->lastObject ( );
			CCDictionary*      pDict = (CCDictionary*) pObjectGroup->getObjects ( )->lastObject ( );

			std::string sPropertyName = stringForKey ( pAttributeDict, "name" );
			CCString*  pPropertyValue = new CCString ( stringForKey ( pAttributeDict, "value" ) );
			pDict->setObject ( pPropertyValue, sPropertyName.c_str ( ) );
			pPropertyValue->release ( );
		} 
		else if ( pTMXMapInfo->getParentElement ( ) == TMXPropertyTile ) 
		{
			CCDictionary*  pDictionary = (CCDictionary*) pTMXMapInfo->getTileProperties ( )->objectForKey ( pTMXMapInfo->getParentGID ( ) );

			std::string  sPropertyName = stringForKey ( pAttributeDict, "name" );
			CCString*   pPropertyValue = new CCString ( stringForKey ( pAttributeDict, "value" ) );
			pDictionary->setObject ( pPropertyValue, sPropertyName.c_str ( ) );
			pPropertyValue->release ( );
		}
	}
	else if ( sElementName == "polygon" ) 
	{
		// find parent object's dict and add polygon-points to it
        CCTMXObjectGroup*  pObjectGroup = (CCTMXObjectGroup*) m_pObjectGroups->lastObject ( );
        CCDictionary*  pDict = (CCDictionary*) pObjectGroup->getObjects ( )->lastObject ( );

        // get points value string
        const KDchar*  szValue = stringForKey ( pAttributeDict, "points" );

        if ( szValue )
        {
            CCArray*   pPointsArray = new CCArray;

            // parse points string into a space-separated set of points
			std::stringstream  sPointsStream ( szValue );
			std::string  sPointPair;
            while ( std::getline ( sPointsStream, sPointPair, ' ' ) )
            {
                // parse each point combo into a comma-separated x,y point
				std::stringstream  sPointStream ( sPointPair );
				std::string  sX, sY;
                KDchar  szBuffer[32] = {0};
                
                CCDictionary*  pPointDict = new CCDictionary;

                // set x
                if ( std::getline ( sPointStream, sX, ',' ) )
                {
                    KDint  x = kdAtoi ( sX.c_str ( ) ) + (KDint) pObjectGroup->getPositionOffset ( ).x;
                    kdSprintf ( szBuffer, "%d", x );
                    CCString*  pStr = new CCString ( szBuffer );
                    pStr->autorelease ( );
                    pPointDict->setObject ( pStr, "x" );
                }

                // set y
                if ( std::getline ( sPointStream, sY, ',' ) )
                {
                    KDint  y = kdAtoi ( sY.c_str ( ) ) + (KDint) pObjectGroup->getPositionOffset ( ).y;
                    kdSprintf ( szBuffer, "%d", y );
                    CCString*  pStr = new CCString ( szBuffer );
                    pStr->autorelease ( );
                    pPointDict->setObject ( pStr, "y" );
                }
                
                // add to points array
                pPointsArray->addObject ( pPointDict );
                pPointDict->release ( );
            }
            
            pDict->setObject ( pPointsArray, "points" );
            pPointsArray->release();
        }
	} 
	else if ( sElementName == "polyline" )
	{
		// find parent object's dict and add polyline-points to it
		// CCTMXObjectGroup* objectGroup = (CCTMXObjectGroup*)m_pObjectGroups->lastObject();
		// CCDictionary* dict = (CCDictionary*)objectGroup->getObjects()->lastObject();
		// TODO: dict->setObject:[attributeDict objectForKey:@"points"] forKey:@"polylinePoints"];
	}
    
	if ( pAttributeDict )
	{
		pAttributeDict->clear ( );
		delete pAttributeDict;
	}
}

KDvoid CCTMXMapInfo::endElement ( KDvoid* pCtx, const KDchar* szName )
{
	CC_UNUSED_PARAM ( pCtx );

	CCTMXMapInfo*  pTMXMapInfo = this;
	std::string    sElementName = szName;

	KDint  nLen = 0;

	if ( sElementName == "data" && pTMXMapInfo->getLayerAttribs ( ) & TMXLayerAttribBase64 ) 
	{
		pTMXMapInfo->setStoringCharacters ( KD_FALSE );

		CCTMXLayerInfo*  pLayer = (CCTMXLayerInfo*) pTMXMapInfo->getLayers ( )->lastObject ( );
		std::string  sCurrentString = pTMXMapInfo->getCurrentString ( );
		KDubyte*  pBuffer;
        
		nLen = base64Decode ( (KDubyte *) sCurrentString.c_str ( ), sCurrentString.length ( ), &pBuffer );
		if ( !pBuffer ) 
		{
			CCLOG ( "XMCocos2D : TiledMap - decode data error" );
			return;
		}

		if ( pTMXMapInfo->getLayerAttribs ( ) & ( TMXLayerAttribGzip | TMXLayerAttribZlib ) )
		{
			KDubyte*  pDeflated;
			
			KDint  nSizeHint = (KDint) ( pLayer->m_tLayerSize.cx * pLayer->m_tLayerSize.cy * sizeof ( KDuint ) );
			KDint  nInflatedLen = ZipUtils::ccInflateMemoryWithHint ( pBuffer, nLen, &pDeflated, nSizeHint );
			CCAssert ( nInflatedLen == nSizeHint, "" );

			nInflatedLen = (KDsize) &nInflatedLen; // XXX: to avoid warnings in compiler
				
			CC_SAFE_DELETE_ARRAY ( pBuffer );

			if ( !pDeflated ) 
			{
				CCLOG ( "XMCocos2D : TiledMap - inflate data error" );
				return;
			}

			pLayer->m_pTiles = (KDuint *) pDeflated;
		}
		else
		{
			pLayer->m_pTiles = (KDuint *) pBuffer;
		}

		pTMXMapInfo->setCurrentString ( "" );

	} 
	else if ( sElementName == "map" )
	{
		// The map element has ended
		pTMXMapInfo->setParentElement ( TMXPropertyNone );
	}	
	else if ( sElementName == "layer" )
	{
		// The layer element has ended
		pTMXMapInfo->setParentElement ( TMXPropertyNone );
	}
	else if ( sElementName == "objectgroup" )
	{
		// The objectgroup element has ended
		pTMXMapInfo->setParentElement ( TMXPropertyNone );
	} 
	else if ( sElementName == "object" ) 
	{
		// The object element has ended
		pTMXMapInfo->setParentElement ( TMXPropertyNone );
	}
}

KDvoid CCTMXMapInfo::textHandler ( KDvoid* pCtx, const KDchar* szName, KDint nLen )
{
	CC_UNUSED_PARAM ( pCtx );

	CCTMXMapInfo*  pTMXMapInfo = this;
	std::string    pText ( szName, 0, nLen );

	if ( pTMXMapInfo->getStoringCharacters ( ) )
	{
		std::string  sCurrentString = pTMXMapInfo->getCurrentString ( );
		sCurrentString += pText;
		pTMXMapInfo->setCurrentString ( sCurrentString.c_str ( ) );
	}
}

NS_CC_END