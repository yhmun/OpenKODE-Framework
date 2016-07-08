/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXLayer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "tilemap_parallax_nodes/CCTMXLayer.h"
#include "tilemap_parallax_nodes/CCTMXTiledMap.h"
#include "textures/CCTextureCache.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"

NS_CC_BEGIN   

/// CCTMXLayer - init & alloc & dealloc

CCTMXLayer* CCTMXLayer::create ( CCTMXTilesetInfo* pTilesetInfo, CCTMXLayerInfo* pLayerInfo, CCTMXMapInfo* pMapInfo )
{
	CCTMXLayer*  pRet = new CCTMXLayer ( );

	if ( pRet && pRet->initWithTilesetInfo ( pTilesetInfo, pLayerInfo, pMapInfo ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTMXLayer::initWithTilesetInfo ( CCTMXTilesetInfo* pTilesetInfo, CCTMXLayerInfo* pLayerInfo, CCTMXMapInfo* pMapInfo )
{	
	// XXX: is 35% a good estimate ?
	CCSize   tSize = pLayerInfo->m_tLayerSize;
	KDfloat  fTotalNumberOfTiles = tSize.cx * tSize.cy;
	KDfloat  fCapacity = fTotalNumberOfTiles * 0.35f + 1; // 35 percent is occupied ?
    
	CCTexture2D*  pTexture = KD_NULL;

	if ( pTilesetInfo )
	{
		pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( pTilesetInfo->m_sSourceImage.c_str ( ) );
	}

	if ( CCSpriteBatchNode::initWithTexture ( pTexture, (KDuint) fCapacity ) )
	{
		// layerInfo
		m_sLayerName  = pLayerInfo->m_sName;
		m_tLayerSize  = tSize;
		m_pTiles      = pLayerInfo->m_pTiles;
		m_uMinGID     = pLayerInfo->m_uMinGID;
		m_uMaxGID     = pLayerInfo->m_uMaxGID;
		m_cOpacity    = pLayerInfo->m_cOpacity;
		setProperties ( CCDictionary::createWithDictionary ( pLayerInfo->getProperties ( ) ) );

		// tilesetInfo
		m_pTileSet = pTilesetInfo;
		CC_SAFE_RETAIN ( m_pTileSet );

		// mapInfo
		m_tMapTileSize = pMapInfo->getTileSize ( );
		m_uLayerOrientation = pMapInfo->getOrientation ( );

		// offset (after layer orientation is set);
		CCPoint  tOffset = this->calculateLayerOffset ( pLayerInfo->m_tOffset );
		this->setPosition ( CC_POINT_PIXELS_TO_POINTS ( tOffset ) );

		m_pAtlasIndexArray = ccCArrayNew ( (KDuint) fTotalNumberOfTiles );

		this->setContentSize ( CC_SIZE_PIXELS_TO_POINTS ( CCSize ( m_tLayerSize.cx * m_tMapTileSize.cx, m_tLayerSize.cy * m_tMapTileSize.cy ) ) );

		m_bUseAutomaticVertexZ = KD_FALSE;
		m_nVertexZvalue   = 0;
	
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCTMXLayer::CCTMXLayer ( KDvoid )
{
    m_tLayerSize        = CCSizeZero;
    m_tMapTileSize      = CCSizeZero;
    m_pTiles            = KD_NULL;
    m_pTileSet          = KD_NULL;
    m_pProperties       = KD_NULL;
    m_sLayerName        = "";
    m_pReusedTile       = KD_NULL;
    m_pAtlasIndexArray  = KD_NULL;
}

CCTMXLayer::~CCTMXLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTileSet );
	CC_SAFE_RELEASE ( m_pReusedTile );
	CC_SAFE_RELEASE ( m_pProperties );
    
	if ( m_pAtlasIndexArray )
	{
		ccCArrayFree ( m_pAtlasIndexArray );
		m_pAtlasIndexArray = KD_NULL;
	}
    
	CC_SAFE_DELETE_ARRAY ( m_pTiles );
}

CCTMXTilesetInfo*  CCTMXLayer::getTileSet ( KDvoid )
{
	return m_pTileSet;
}

KDvoid CCTMXLayer::setTileSet ( CCTMXTilesetInfo* pTileSet )
{
	CC_SAFE_RETAIN ( pTileSet );
	CC_SAFE_RELEASE ( m_pTileSet );
	m_pTileSet = pTileSet;
}

KDvoid CCTMXLayer::releaseMap ( KDvoid )
{
	CC_SAFE_DELETE_ARRAY ( m_pTiles );

	if ( m_pAtlasIndexArray )
	{
		ccCArrayFree ( m_pAtlasIndexArray );
		m_pAtlasIndexArray = KD_NULL;
	}
}

/// CCTMXLayer - setup Tiles
KDvoid CCTMXLayer::setupTiles ( KDvoid )
{	
	// Optimization: quick hack that sets the image size on the tileset
	m_pTileSet->m_tImageSize = m_pTextureAtlas->getTexture ( )->getContentSizeInPixels ( );

	// By default all the tiles are aliased
	// pros:
	//  - easier to render
	// cons:
	//  - difficult to scale / rotate / etc.
	m_pTextureAtlas->getTexture ( )->setAliasTexParameters ( );

	//CFByteOrder o = CFByteOrderGetCurrent ( );

	// Parse cocos2d properties
	this->parseInternalProperties ( );

	for ( KDuint y = 0; y < m_tLayerSize.height; y++ ) 
	{
		for ( KDuint x = 0; x < m_tLayerSize.width; x++ ) 
		{
			KDuint uPos = (KDuint) ( x + m_tLayerSize.width * y );
			KDuint uGID = m_pTiles[ uPos ];

			// gid are stored in little endian.
			// if host is big endian, then swap
			//if ( o == CFByteOrderBigEndian )
			//	gid = CFSwapInt32( gid );
			/* We support little endian.*/

			// XXX: gid == 0 --> empty tile
			if ( uGID != 0 ) 
			{
				this->appendTileForGID ( uGID, ccp ( x, y ) );

				// Optimization: update min and max GID rendered by the layer
				m_uMinGID = KD_MIN ( uGID, m_uMinGID );
				m_uMaxGID = KD_MAX ( uGID, m_uMaxGID );
			}
		}
	}

	CCAssert ( m_uMaxGID >= m_pTileSet->m_uFirstGid && m_uMinGID >= m_pTileSet->m_uFirstGid, "TMX: Only 1 tileset per layer is supported" );	
}

/// CCTMXLayer - Properties
CCString* CCTMXLayer::propertyNamed ( const KDchar* szPropertyName )
{
	return (CCString*) m_pProperties->objectForKey ( szPropertyName );
}

KDvoid CCTMXLayer::parseInternalProperties ( KDvoid )
{
	// if cc_vertex=automatic, then tiles will be rendered using vertexz

	CCString*  pVertexz = propertyNamed ( "cc_vertexz" );
	if ( pVertexz ) 
	{
		// If "automatic" is on, then parse the "cc_alpha_func" too
		if ( pVertexz->m_sString == "automatic" )
		{
			m_bUseAutomaticVertexZ = KD_TRUE;

			CCString*  pAlphaFuncVal   = propertyNamed ( "cc_alpha_func" );
            KDfloat    fAlphaFuncValue = 0.0f;
            
			if ( pAlphaFuncVal != KD_NULL )
            {
                fAlphaFuncValue = pAlphaFuncVal->floatValue ( );
            }

            setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColorAlphaTest ) );

		#if defined ( USE_OPEN_GLES2 )
            GLint  nAlphaValueLocation = glGetUniformLocation ( getShaderProgram ( )->getProgram ( ), kCCUniformAlphaTestValue );

            // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
            getShaderProgram ( )->setUniformLocationWith1f ( nAlphaValueLocation, fAlphaFuncValue );
		#endif
		}
		else
		{
			m_nVertexZvalue = pVertexz->intValue ( );
		}
	}
}

KDvoid CCTMXLayer::setupTileSprite ( CCSprite* pSprite, CCPoint tPosition, KDuint uGID )
{
    pSprite->setPosition ( positionAt ( tPosition ) );
    pSprite->setVertexZ ( (KDfloat) vertexZForPos ( tPosition ) );
    pSprite->setAnchorPoint ( CCPointZero );
    pSprite->setOpacity ( m_cOpacity );

    //issue 1264, flip can be undone as well
	pSprite->setFlipX ( KD_FALSE );
    pSprite->setFlipY ( KD_FALSE );
    pSprite->setRotation ( 0.0f );
    pSprite->setAnchorPoint ( ccp ( 0, 0 ) );

    // Rotation in tiled is achieved using 3 flipped states, flipping across the horizontal, vertical, and diagonal axes of the tiles.
    if ( uGID & kCCTMXTileDiagonalFlag )
    {
        // put the anchor in the middle for ease of rotation.
        pSprite->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
        pSprite->setPosition ( ccp ( positionAt ( tPosition ).x + pSprite->getContentSize ( ).cy / 2,
                                     positionAt ( tPosition ).y + pSprite->getContentSize ( ).cx / 2 ) );

        KDuint  uFlag = uGID & ( kCCTMXTileHorizontalFlag | kCCTMXTileVerticalFlag );

        // handle the 4 diagonally flipped states.
        if ( uFlag == kCCTMXTileHorizontalFlag )
        {
            pSprite->setRotation ( 90.0f );
        }
        else if ( uFlag == kCCTMXTileVerticalFlag )
        {
            pSprite->setRotation ( 270.0f );
        }
        else if ( uFlag == ( kCCTMXTileVerticalFlag | kCCTMXTileHorizontalFlag ) )
        {
            pSprite->setRotation ( 90.0f );
            pSprite->setFlipX ( KD_TRUE );
        }
        else
        {
            pSprite->setRotation ( 270.0f );
            pSprite->setFlipX ( KD_TRUE );
        }
    }
    else
    {
        if ( uGID & kCCTMXTileHorizontalFlag )
        {
            pSprite->setFlipX ( KD_TRUE );
        }

        if ( uGID & kCCTMXTileVerticalFlag)
        {
            pSprite->setFlipY ( KD_TRUE );
        }
    }
}

CCSprite* CCTMXLayer::reusedTileWithRect ( CCRect tRect )
{
    if ( !m_pReusedTile )
	{
        m_pReusedTile = new CCSprite ( );
        m_pReusedTile->initWithTexture ( m_pTextureAtlas->getTexture ( ), tRect, KD_FALSE );
        m_pReusedTile->setBatchNode ( this );
    }
    else
    {
        // XXX HACK: Needed because if "batch node" is nil,
		// then the Sprite'squad will be reset
        m_pReusedTile->setBatchNode ( KD_NULL );

        // Re-init the sprite
        m_pReusedTile->setTextureRect ( tRect, KD_FALSE, tRect.size );
        
		// restore the batch node
        m_pReusedTile->setBatchNode ( this );
    }

    return m_pReusedTile;
}

/// CCTMXLayer - obtaining tiles/gids
CCSprite* CCTMXLayer::tileAt ( const CCPoint& tPosition )
{
	CCAssert ( tPosition.x < m_tLayerSize.cx && tPosition.y < m_tLayerSize.cy && tPosition.x >= 0 && tPosition.y >= 0, "TMXLayer: invalid position" );
	CCAssert ( m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released" );

	CCSprite* pTile = KD_NULL;
	KDuint    uGID  = this->tileGIDAt ( tPosition );

	// if GID == 0, then no tile is present
	if ( uGID ) 
	{
		KDint  nZOrder = (KDint) ( tPosition.x + tPosition.y * m_tLayerSize.cx );
		pTile = (CCSprite *) this->getChildByTag ( nZOrder );

		// tile not created yet. create it
		if ( !pTile ) 
		{
			CCRect  tRect = m_pTileSet->rectForGID ( uGID );
			tRect = CC_RECT_PIXELS_TO_POINTS ( tRect );

			pTile = new CCSprite ( );
			pTile->initWithTexture ( this->getTexture ( ), tRect );
			pTile->setBatchNode ( this );
			pTile->setPosition ( positionAt ( tPosition ) );
			pTile->setVertexZ ( (KDfloat) vertexZForPos ( tPosition ) );
			pTile->setAnchorPoint ( CCPointZero );
			pTile->setOpacity ( m_cOpacity );

			KDuint  uIndexForZ = atlasIndexForExistantZ ( nZOrder );
			this->addSpriteWithoutQuad ( pTile, uIndexForZ, nZOrder );
			pTile->release ( );
		}
	}
    
	return pTile;
}

KDuint CCTMXLayer::tileGIDAt ( const CCPoint& tPosition )
{
	return tileGIDAt ( tPosition, KD_NULL );
}

KDuint CCTMXLayer::tileGIDAt ( const CCPoint& tPosition, ccTMXTileFlags* pFlags )
{
	CCAssert ( tPosition.x < m_tLayerSize.cx && tPosition.y < m_tLayerSize.cy && tPosition.x >= 0 && tPosition.y >= 0, "TMXLayer: invalid position" );
	CCAssert ( m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released" );

    KDint   nIndex = (KDint) ( tPosition.x + tPosition.y * m_tLayerSize.cx );
    // Bits on the far end of the 32-bit global tile ID are used for tile flags
    KDuint  uTile = m_pTiles [ nIndex ];

    // issue1264, flipped tiles can be changed dynamically
    if ( pFlags ) 
    {
        *pFlags = (ccTMXTileFlags) ( uTile & kCCFlipedAll );
    }
    return ( uTile & kCCFlippedMask );
}

/// CCTMXLayer - adding helper methods
CCSprite* CCTMXLayer::insertTileForGID ( KDuint uGID, const CCPoint& tPosition )
{
	CCRect     tRect   = m_pTileSet->rectForGID ( uGID );
	tRect = CC_RECT_PIXELS_TO_POINTS ( tRect );

	KDintptr   nZOrder = (KDintptr) ( tPosition.x + tPosition.y * m_tLayerSize.cx );
	CCSprite*  pTile   = reusedTileWithRect ( tRect );

	setupTileSprite ( pTile, tPosition, uGID );

	// get atlas index
	KDuint  uIndexForZ = atlasIndexForNewZ ( nZOrder );

	// Optimization: add the quad without adding a child
	this->insertQuadFromSprite ( pTile, uIndexForZ );

	// insert it into the local atlasindex array
	ccCArrayInsertValueAtIndex ( m_pAtlasIndexArray, (KDvoid *) nZOrder, uIndexForZ );

	// update possible children
	if ( m_pChildren && m_pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( m_pChildren, pObject )
		{
			CCSprite*  pChild = (CCSprite*) pObject;
			if ( pChild )
			{
				KDuint  uIndex = pChild->getAtlasIndex ( );
				if ( uIndex >= uIndexForZ )
				{
					pChild->setAtlasIndex ( uIndex + 1 );
				}
			}
		}
	}
	
    m_pTiles [ nZOrder ] = uGID;
    
	return pTile;
}

CCSprite* CCTMXLayer::updateTileForGID ( KDuint uGID, const CCPoint& tPosition )	
{
	CCRect     tRect   = m_pTileSet->rectForGID ( uGID );
	KDintptr   nZOrder = (KDintptr) ( tPosition.x + tPosition.y * m_tLayerSize.cx );	
	CCSprite*  pTile   = reusedTileWithRect ( tRect );

	setupTileSprite ( pTile, tPosition, uGID );

	// get atlas index
	KDuint  uIndexForZ = atlasIndexForExistantZ ( nZOrder );
	pTile->setAtlasIndex ( uIndexForZ );
    pTile->setDirty ( KD_TRUE );
	pTile->updateTransform ( );
    
	m_pTiles [ nZOrder ] = uGID;

	return pTile;
}

//
// used only when parsing the map. useless after the map was parsed
// since lot's of assumptions are no longer true
//
CCSprite* CCTMXLayer::appendTileForGID ( KDuint uGID, const CCPoint& tPosition )
{
	CCRect     tRect   = m_pTileSet->rectForGID ( uGID );
	tRect = CC_RECT_PIXELS_TO_POINTS ( tRect );

	KDint      nZOrder = (KDint) ( tPosition.x + tPosition.y * m_tLayerSize.width );
	CCSprite*  pTile   = reusedTileWithRect ( tRect );
	
	setupTileSprite ( pTile, tPosition, uGID );

	// optimization:
	// The difference between appendTileForGID and insertTileforGID is that append is faster, since
	// it appends the tile at the end of the texture atlas
	KDuint  uIndexForZ = m_pAtlasIndexArray->num;

	// don't add it using the "standard" way.
	insertQuadFromSprite ( pTile, uIndexForZ );

	// append should be after addQuadFromSprite since it modifies the quantity values
	ccCArrayInsertValueAtIndex ( m_pAtlasIndexArray, (KDvoid *) nZOrder, uIndexForZ );

	return pTile;
}

/// CCTMXLayer - atlasIndex and Z
static inline KDint compareInts ( const KDvoid* pA, const KDvoid* pB )
{
	return ( *(KDint*) pA - *(KDint*) pB );
}

KDuint CCTMXLayer::atlasIndexForExistantZ ( KDuint uZOrder )
{
	KDint   nKey = uZOrder;
	KDint*  pItem = (KDint *) kdBsearch ( (KDvoid*) &nKey, (KDvoid*) &m_pAtlasIndexArray->arr [ 0 ], m_pAtlasIndexArray->num, sizeof ( KDvoid* ), (KDint (*)(const KDvoid*, const KDvoid*)) compareInts );

	CCAssert ( pItem, "TMX atlas index not found. Shall not happen" );

	KDint  nIndex = ( (KDsize) pItem - (KDsize) m_pAtlasIndexArray->arr ) / sizeof ( KDvoid* );
	return nIndex;
}

KDuint CCTMXLayer::atlasIndexForNewZ ( KDint nZOrder )
{
	// XXX: This can be improved with a sort of binary search
	KDuint  i = 0;
	for ( i = 0; i< m_pAtlasIndexArray->num ; i++ ) 
	{
		KDint  nIndex = (KDsize) m_pAtlasIndexArray->arr [ i ];
		if ( nZOrder < nIndex )
		{
			break;
		}
	}	

	return i;
}

/// CCTMXLayer - adding / remove tiles
KDvoid CCTMXLayer::setTileGID ( KDuint uGID, const CCPoint& tTileCoordinate )
{
	setTileGID ( uGID, tTileCoordinate, (ccTMXTileFlags) 0 );
}

KDvoid CCTMXLayer::setTileGID ( KDuint uGID, const CCPoint& tTileCoordinate, ccTMXTileFlags eFlags )
{
	CCAssert ( tTileCoordinate.x < m_tLayerSize.cx && tTileCoordinate.y < m_tLayerSize.cy && tTileCoordinate.x >= 0 && tTileCoordinate.y >= 0, "TMXLayer: invalid position" );
	CCAssert ( m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released" );
    CCAssert ( uGID == 0 || uGID >= m_pTileSet->m_uFirstGid, "TMXLayer: invalid gid" );

	ccTMXTileFlags  eCurrentFlags;
	KDuint uCurrentGID = tileGIDAt ( tTileCoordinate, &eCurrentFlags );

	if ( uCurrentGID != uGID || eCurrentFlags != eFlags ) 
	{
		KDuint  uGIDAndFlags = uGID | eFlags;

		// setting gid=0 is equal to remove the tile
		if ( uGID == 0 )
		{
			removeTileAt ( tTileCoordinate );
		}

		// empty tile. create a new one
		else if ( uCurrentGID == 0 )
		{
			insertTileForGID ( uGID, tTileCoordinate );
		}

		// modifying an existing tile with a non-empty tile
		else 
		{
			KDuint     uZOrder = (KDuint) ( tTileCoordinate.x + tTileCoordinate.y * m_tLayerSize.cx );
			CCSprite*  pSprite = (CCSprite*) getChildByTag ( uZOrder );
			if ( pSprite )
			{
				CCRect  tRect = m_pTileSet->rectForGID ( uGID );
				tRect = CC_RECT_PIXELS_TO_POINTS ( tRect );

				pSprite->setTextureRect ( tRect, KD_FALSE, tRect.size );
				if ( eFlags ) 
                {
                    setupTileSprite ( pSprite, pSprite->getPosition ( ), uGIDAndFlags );
                }
				m_pTiles [ uZOrder ] = uGIDAndFlags;
			} 
			else 
			{
				updateTileForGID ( uGIDAndFlags, tTileCoordinate );
			}
		}
	}
}

KDvoid CCTMXLayer::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{
	CC_UNUSED_PARAM ( pChild );
	CC_UNUSED_PARAM ( nZOrder );
	CC_UNUSED_PARAM ( nTag );
	CCAssert ( 0, "addChild: is not supported on CCTMXLayer. Instead use setTileGID:at:/tileAt:" );
}

KDvoid CCTMXLayer::removeChild ( CCNode* pNode, KDbool bCleanup )
{
	CCSprite*  pSprite = (CCSprite *) pNode;
	// allows removing nil objects
	if ( !pSprite )
	{
		return;
	}

	CCAssert ( m_pChildren->containsObject ( pSprite ), "Tile does not belong to TMXLayer" );

	KDuint  uAtlasIndex = pSprite->getAtlasIndex ( );
	KDuint  uZOrder = (KDsize) m_pAtlasIndexArray->arr [ uAtlasIndex ];
	m_pTiles [ uZOrder ] = 0;
    
	ccCArrayRemoveValueAtIndex ( m_pAtlasIndexArray, uAtlasIndex );
	CCSpriteBatchNode::removeChild ( pSprite, bCleanup );
}

KDvoid CCTMXLayer::removeTileAt ( const CCPoint& tPosition )
{
	CCAssert ( tPosition.x < m_tLayerSize.cx && tPosition.y < m_tLayerSize.cy && tPosition.x >= 0 && tPosition.y >= 0, "TMXLayer: invalid position" );
	CCAssert ( m_pTiles && m_pAtlasIndexArray, "TMXLayer: the tiles map has been released" );

	KDuint  uGID = tileGIDAt ( tPosition );

	if ( uGID ) 
	{
		KDuint  uZOrder = (KDuint) ( tPosition.x + tPosition.y * m_tLayerSize.cx );
		KDuint  uAtlasIndex = atlasIndexForExistantZ ( uZOrder );

		// remove tile from GID map
		m_pTiles [ uZOrder ] = 0;

		// remove tile from atlas position array
		ccCArrayRemoveValueAtIndex ( m_pAtlasIndexArray, uAtlasIndex );

		// remove it from sprites and/or texture atlas
		CCSprite*  pSprite = (CCSprite*) getChildByTag ( uZOrder );
		if ( pSprite )
		{
			CCSpriteBatchNode::removeChild ( pSprite, KD_TRUE );
		}
		else 
		{
			m_pTextureAtlas->removeQuadAtIndex ( uAtlasIndex );

			// update possible children
			if ( m_pChildren && m_pChildren->count ( ) > 0 )
			{
				CCObject*  pObject = KD_NULL;
				CCARRAY_FOREACH ( m_pChildren, pObject )
				{
					CCSprite*  pChild = (CCSprite*) pObject;
					if ( pChild )
					{
						KDuint  uIndex = pChild->getAtlasIndex ( );
						if ( uIndex >= uAtlasIndex )
						{
							pChild->setAtlasIndex ( uIndex - 1 );
						}
					}
				}
			}
		}
	}
}

/// CCTMXLayer - obtaining positions, offset
CCPoint CCTMXLayer::calculateLayerOffset ( const CCPoint& tPosition )
{
	CCPoint  tRet = CCPointZero;
    
	switch ( m_uLayerOrientation ) 
	{
		case CCTMXOrientationOrtho :
			tRet = ccp ( tPosition.x * m_tMapTileSize.cx, -tPosition.y * m_tMapTileSize.cy );
			break;

		case CCTMXOrientationIso :
			tRet = ccp ( ( m_tMapTileSize.cx / 2 ) *  ( tPosition.x - tPosition.y ), ( m_tMapTileSize.cy / 2 ) * ( -tPosition.x - tPosition.y ) );
			break;

		case CCTMXOrientationHex :
			CCAssert ( tPosition.equals ( CCPointZero ), "offset for hexagonal map not implemented yet" );
			break;
	}
    
	return tRet;	
}

CCPoint CCTMXLayer::positionAt ( const CCPoint& tPosition )
{
	CCPoint  tRet = CCPointZero;

	switch ( m_uLayerOrientation )
	{
		case CCTMXOrientationOrtho : tRet = positionForOrthoAt ( tPosition );  break;
		case CCTMXOrientationIso   : tRet = positionForIsoAt   ( tPosition );  break;
		case CCTMXOrientationHex   : tRet = positionForHexAt   ( tPosition );  break;
	}

	tRet = CC_POINT_PIXELS_TO_POINTS ( tRet );
	return tRet;
}

CCPoint CCTMXLayer::positionForOrthoAt ( const CCPoint& tPosition )
{
    return ccp ( tPosition.x * m_tMapTileSize.cx, ( m_tLayerSize.cy - tPosition.y - 1 ) * m_tMapTileSize.cy );
}

CCPoint CCTMXLayer::positionForIsoAt ( const CCPoint& tPosition )
{
    return ccp ( m_tMapTileSize.cx / 2 * ( m_tLayerSize.cx + tPosition.x - tPosition.y - 1 ),
                 m_tMapTileSize.cy / 2 * ( ( m_tLayerSize.cy * 2 - tPosition.x - tPosition.y ) - 2 ) );
}

CCPoint CCTMXLayer::positionForHexAt ( const CCPoint& tPosition )
{
	KDfloat  diffY = (KDint) tPosition.x % 2 == 1 ? -m_tMapTileSize.cy / 2 : 0;

    return ccp ( tPosition.x * m_tMapTileSize.cx * 3 / 4,
               ( m_tLayerSize.cy - tPosition.y - 1 ) * m_tMapTileSize.cy + diffY );
}

KDint CCTMXLayer::vertexZForPos ( const CCPoint& tPosition )
{
	KDint  nRet = 0;
	KDuint uMaxVal = 0;
    
	if ( m_bUseAutomaticVertexZ )
	{
		switch ( m_uLayerOrientation ) 
		{
			case CCTMXOrientationIso :
				uMaxVal = (KDuint) ( m_tLayerSize.cx + m_tLayerSize.cy );
				nRet = (KDint) ( -( uMaxVal - ( tPosition.x + tPosition.y ) ) );
				break;

			case CCTMXOrientationOrtho :
				nRet = (KDint) ( -( m_tLayerSize.cy - tPosition.y ) );
				break;

			case CCTMXOrientationHex :
				CCAssert ( 0, "TMX Hexa zOrder not supported" );
				break;

			default:
				CCAssert ( 0, "TMX invalid value" );
				break;
		}
	} 
	else
	{
		nRet = m_nVertexZvalue;
	}
    
	return nRet;
}

CCDictionary * CCTMXLayer::getProperties ( KDvoid )
{
	return m_pProperties;
}

KDvoid CCTMXLayer::setProperties ( CCDictionary* pProperties )
{
	CC_SAFE_RETAIN ( pProperties );
	CC_SAFE_RELEASE ( m_pProperties );
	m_pProperties = pProperties;
}
    
NS_CC_END