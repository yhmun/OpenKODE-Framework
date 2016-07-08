/* --------------------------------------------------------------------------
 *
 *      File            CCTMXTiledMap.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
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
 *      version 2 of the License, or (at your option) any later version.
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
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "tilemap_parallax_nodes/CCTMXLayer.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN
    
//
// implementation CCTMXTiledMap
//

CCTMXTiledMap* CCTMXTiledMap::create ( const KDchar* szTMXPath )
{
	CCTMXTiledMap*  pRet = new CCTMXTiledMap ( );

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

CCTMXTiledMap* CCTMXTiledMap::createWithXML ( const KDchar* szTMXString, const KDchar* szResPath )
{
    CCTMXTiledMap*  pRet = new CCTMXTiledMap ( );

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

KDbool CCTMXTiledMap::initWithTMXFile ( const KDchar* szTMXPath )
{
	CCAssert ( szTMXPath != NULL && kdStrlen ( szTMXPath ) > 0, "TMXTiledMap: tmx file should not bi nil" );
	
	setContentSize ( CCSizeZero );

	CCTMXMapInfo*  pMapInfo = CCTMXMapInfo::createWithTMXFile ( szTMXPath );
    if ( !pMapInfo )
    {
        return KD_FALSE;
    }
	CCAssert ( pMapInfo->getTilesets ( )->count ( ) != 0, "TMXTiledMap: Map not found. Please check the filename." );

	buildWithMapInfo ( pMapInfo );

	return KD_TRUE;
}

KDbool CCTMXTiledMap::initWithXML ( const KDchar* szTMXString, const KDchar* szResPath )
{
    setContentSize ( CCSizeZero );

    CCTMXMapInfo*  pMapInfo = CCTMXMapInfo::createWithXML ( szTMXString, szResPath );

    CCAssert ( pMapInfo->getTilesets ( )->count ( ) != 0, "TMXTiledMap: Map not found. Please check the filename." );
    buildWithMapInfo ( pMapInfo );

    return KD_TRUE;
}

CCTMXTiledMap::CCTMXTiledMap ( KDvoid )
{
    m_tMapSize          = CCSizeZero;
    m_tTileSize         = CCSizeZero;
    m_pObjectGroups     = KD_NULL;
    m_pProperties       = KD_NULL;
    m_pTileProperties   = KD_NULL;
}

CCTMXTiledMap::~CCTMXTiledMap ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pProperties );
	CC_SAFE_RELEASE ( m_pObjectGroups );
	CC_SAFE_RELEASE ( m_pTileProperties );
}

CCArray* CCTMXTiledMap::getObjectGroups ( KDvoid )
{
	return m_pObjectGroups;
}

KDvoid CCTMXTiledMap::setObjectGroups ( CCArray* pObjectGroups )
{
	CC_SAFE_RETAIN  ( pObjectGroups );
	CC_SAFE_RELEASE ( m_pObjectGroups );
	m_pObjectGroups = pObjectGroups;
}

CCDictionary* CCTMXTiledMap::getProperties ( KDvoid )
{
	return m_pProperties;
}

KDvoid CCTMXTiledMap::setProperties ( CCDictionary* pProperties )
{
	CC_SAFE_RETAIN  ( pProperties );
	CC_SAFE_RELEASE ( m_pProperties );
	m_pProperties = pProperties;
}

CCTMXLayer* CCTMXTiledMap::parseLayer ( CCTMXLayerInfo* pLayerInfo, CCTMXMapInfo* pMapInfo )
{
	CCTMXTilesetInfo*  pTileset = tilesetForLayer ( pLayerInfo, pMapInfo );
	CCTMXLayer*        pLayer   = CCTMXLayer::create ( pTileset, pLayerInfo, pMapInfo );

	// tell the layerinfo to release the ownership of the tiles map.
	pLayerInfo->m_bOwnTiles = KD_FALSE;
	pLayer->setupTiles ( );

	return pLayer;
}

CCTMXTilesetInfo* CCTMXTiledMap::tilesetForLayer ( CCTMXLayerInfo* pLayerInfo, CCTMXMapInfo* pMapInfo )
{
	CCSize    tSize = pLayerInfo->m_tLayerSize;
	CCArray*  pTilesets = pMapInfo->getTilesets ( );
	if ( pTilesets && pTilesets->count ( ) > 0 )
	{
		CCTMXTilesetInfo*  pTileset = KD_NULL;
		CCObject*          pObject  = KD_NULL;
        CCARRAY_FOREACH_REVERSE ( pTilesets, pObject )
		{
			pTileset = (CCTMXTilesetInfo*) pObject;
			if ( pTileset )
			{
				for ( KDuint y = 0; y < tSize.cy; y++ )
				{
					for ( KDuint x = 0; x < tSize.cx; x++ ) 
					{
						KDuint  uPos = (KDuint) ( x + tSize.cx * y );
						KDuint  uGID = pLayerInfo->m_pTiles [ uPos ];

						// gid are stored in little endian.
						// if host is big endian, then swap
						//if ( o == CFByteOrderBigEndian )
						//	gid = CFSwapInt32( gid );
						/* We support little endian.*/

						// XXX: gid == 0 --> empty tile
						if ( uGID != 0 ) 
						{
							// Optimization: quick return
							// if the layer is invalid (more than 1 tileset per layer) an assert will be thrown later
							if ( ( uGID & kCCFlippedMask ) >= pTileset->m_uFirstGid )
							{
								return pTileset;
							}
						}
					}
				}		
			}
		}
	}

	// If all the tiles are 0, return empty tileset
	CCLOGINFO ( "XMCocos2D : Warning - TMX Layer '%s' has no tiles", pLayerInfo->m_sName.c_str ( ) );
	return KD_NULL;
}

KDvoid CCTMXTiledMap::buildWithMapInfo ( CCTMXMapInfo* pMapInfo )
{
	m_tMapSize  = pMapInfo->getMapSize  ( );
	m_tTileSize = pMapInfo->getTileSize ( );
	m_nMapOrientation = pMapInfo->getOrientation ( );

    CC_SAFE_RELEASE ( m_pObjectGroups );
    m_pObjectGroups = pMapInfo->getObjectGroups ( );
    CC_SAFE_RETAIN  ( m_pObjectGroups );

    CC_SAFE_RELEASE ( m_pProperties );
    m_pProperties = pMapInfo->getProperties ( );
    CC_SAFE_RETAIN  ( m_pProperties );

    CC_SAFE_RELEASE ( m_pTileProperties );
    m_pTileProperties = pMapInfo->getTileProperties ( );
    CC_SAFE_RETAIN  ( m_pTileProperties );

    KDint  nIdx = 0;

    CCArray*  pLayers = pMapInfo->getLayers ( );
    if ( pLayers && pLayers->count ( ) > 0 )
    {
        CCTMXLayerInfo*  pLayerInfo = KD_NULL;
        CCObject*        pObject    = KD_NULL;
        CCARRAY_FOREACH ( pLayers, pObject )
        {
            pLayerInfo = (CCTMXLayerInfo*) pObject;
            if ( pLayerInfo && pLayerInfo->m_bVisible )
            {
                CCTMXLayer*  pChild = parseLayer ( pLayerInfo, pMapInfo );
                addChild ( (CCNode*) pChild, nIdx, nIdx );

                // update content size with the max size
                const CCSize&  tChildSize = pChild->getContentSize ( );
                CCSize         tCurrentSize = this->getContentSize ( );
                tCurrentSize.cx = KD_MAX ( tCurrentSize.cx, tChildSize.cx );
                tCurrentSize.cy = KD_MAX ( tCurrentSize.cy, tChildSize.cy );
                this->setContentSize ( tCurrentSize );

                nIdx++;
            }
        }
    }
}

/// public
CCTMXLayer* CCTMXTiledMap::layerNamed ( const KDchar* szLayerName )
{
	CCAssert ( szLayerName != KD_NULL && kdStrlen ( szLayerName ) > 0, "Invalid layer name!" );
    
	CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pChildren, pObject ) 
    {
        CCTMXLayer*  pLayer = dynamic_cast<CCTMXLayer*> ( pObject );
        
		if ( pLayer )
        {
            if ( 0 == kdStrcmp ( pLayer->getLayerName ( ), szLayerName ) )
            {
                return pLayer;
            }
        }
    }

    // layer not found
    return KD_NULL;
}

CCTMXObjectGroup * CCTMXTiledMap::objectGroupNamed ( const KDchar* szGroupName )
{
	CCAssert ( szGroupName != KD_NULL && kdStrlen ( szGroupName ) > 0, "Invalid group name!" );

    std::string  sGroupName = szGroupName;
    if ( m_pObjectGroups && m_pObjectGroups->count ( ) > 0 )
    {
        CCTMXObjectGroup*  pObjectGroup = KD_NULL;
        CCObject*          pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pObjectGroups, pObject )
        {
            pObjectGroup = (CCTMXObjectGroup*) pObject;
            if ( pObjectGroup && pObjectGroup->getGroupName ( ) == sGroupName )
            {
                return pObjectGroup;
            }
        }
    }

    // objectGroup not found
    return KD_NULL;
}

CCString* CCTMXTiledMap::propertyNamed ( const KDchar* szPropertyName )
{
	return (CCString*) m_pProperties->objectForKey ( szPropertyName );
}

CCDictionary* CCTMXTiledMap::propertiesForGID ( KDint nGID )
{
	return (CCDictionary*) m_pTileProperties->objectForKey ( nGID );
}


NS_CC_END