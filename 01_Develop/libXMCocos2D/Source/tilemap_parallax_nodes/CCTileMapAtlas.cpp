/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXTiledMap.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Neophit
 *      Copyright (c) 2010      Ricardo Quesada
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
#include "tilemap_parallax_nodes/CCTileMapAtlas.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCInteger.h"
#include "textures/CCTextureAtlas.h"
#include "support/CCFileUtils.h"

NS_CC_BEGIN
    
//
// implementation CCTileMapAtlas
//

CCTileMapAtlas* CCTileMapAtlas::create ( const KDchar* szTilePath, const KDchar* szMapPath, KDuint uTileWidth, KDuint uTileHeight )
{
	CCTileMapAtlas*  pRet = new CCTileMapAtlas ( );

	if ( pRet && pRet->initWithTileFile ( szTilePath, szMapPath, uTileWidth, uTileHeight ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTileMapAtlas::initWithTileFile ( const KDchar* szTilePath, const KDchar* szMapPath, KDuint uTileWidth, KDuint uTileHeight )
{
	std::string  sMapPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szMapPath );

	m_pImage = kdGetImageATX ( sMapPath.c_str ( ), KD_IMAGE_FORMAT_RGB888_ATX, 0 );
	if ( !m_pImage )
	{
		return KD_FALSE;
	}

	m_uWidth  = kdGetImageIntATX ( m_pImage, KD_IMAGE_WIDTH_ATX );
	m_uHeight = kdGetImageIntATX ( m_pImage, KD_IMAGE_HEIGHT_ATX );
	m_pColors = (ccColor3B *) kdGetImagePointerATX ( m_pImage, KD_IMAGE_POINTER_BUFFER_ATX );
	
	this->calculateItemsToRender ( );
	if ( CCAtlasNode::initWithTileFile ( szTilePath, uTileWidth, uTileHeight, m_nItemsToRender ) )
	{
		m_pPosToAtlasIndex = new CCDictionary ( );
		this->updateAtlasValues ( );
		this->setContentSize ( CCSizeMake ( (KDfloat) ( m_uWidth  * m_uItemWidth ),
									 	    (KDfloat) ( m_uHeight * m_uItemHeight ) ) );
		return KD_TRUE;
	}

	return KD_FALSE;
}

CCTileMapAtlas::CCTileMapAtlas ( KDvoid )
{
    m_pImage			= KD_NULL;
    m_uWidth			= 0;
    m_uHeight			= 0;
    m_pColors			= 0;
    m_pPosToAtlasIndex	= KD_NULL;
    m_nItemsToRender	= 0;
}

CCTileMapAtlas::~CCTileMapAtlas ( KDvoid )
{
	releaseMap ( );
}

KDvoid CCTileMapAtlas::releaseMap ( KDvoid )
{
	if ( m_pImage )
	{
		kdFreeImageATX ( m_pImage );
		m_pImage = KD_NULL;
	}

	CC_SAFE_RELEASE ( m_pPosToAtlasIndex );
}

KDvoid CCTileMapAtlas::calculateItemsToRender ( KDvoid )
{
	CCAssert ( m_pImage != KD_NULL, "tgaInfo must be non-nil");

	m_nItemsToRender = 0;
	for ( KDuint x = 0; x < m_uWidth; x++ ) 
	{
		for ( KDuint y = 0; y < m_uHeight; y++ ) 
		{			
			ccColor3B  tColor = m_pColors [ x + y * m_uWidth ];
			if( tColor.r )
			{
				++m_nItemsToRender;
			}
		}
	}
}

/// CCTileMapAtlas - Atlas generation / updates
KDvoid CCTileMapAtlas::setTile ( const ccColor3B& tColor, const CCPoint& tPos )
{
	CCAssert ( m_pImage != KD_NULL, "tgaInfo must not be nil" );
	CCAssert ( m_pPosToAtlasIndex != KD_NULL, "posToAtlasIndex must not be nil" );
	CCAssert ( (KDuint) tPos.x < m_uWidth, "Invalid position.x" );
	CCAssert ( (KDuint) tPos.y < m_uHeight, "Invalid position.x" );
	CCAssert ( tColor.r != 0, "R component must be non 0" );

	ccColor3B  tTemp = m_pColors [ (KDuint) ( tPos.x + tPos.y * m_uWidth ) ];
	if ( tTemp.r == 0 )
	{
		CCLOGINFO ( "XMCocos2D : Value.r must be non 0.");
	} 
	else
	{
		m_pColors [ (KDuint) ( tPos.x + tPos.y * m_uWidth ) ] = tColor;

		// XXX: this method consumes a lot of memory
		// XXX: a tree of something like that shall be implemented
		CCInteger*  pNum = (CCInteger*) m_pPosToAtlasIndex->objectForKey 
		(
			CCString::createWithFormat ( "%ld,%ld", (KDint32) tPos.x, (KDint32) tPos.y 
		)->getCString ( ) );

		this->updateAtlasValueAt ( tPos, tColor, pNum->getValue ( ) );
	}
}

ccColor3B CCTileMapAtlas::tileAt ( const CCPoint& tPos )
{
	CCAssert ( m_pImage != KD_NULL, "tgaInfo must not be nil" );
	CCAssert ( (KDuint) tPos.x < m_uWidth, "Invalid position.x" );
	CCAssert ( (KDuint) tPos.y < m_uHeight, "Invalid position.y" );

	ccColor3B  tColor = m_pColors [ (KDint) ( tPos.x + tPos.y * m_uWidth ) ];

	return tColor;	
}

KDvoid CCTileMapAtlas::updateAtlasValueAt ( const CCPoint& tPos, const ccColor3B& tValue, KDuint uIndex )
{
	CCAssert ( uIndex >= 0 && uIndex < m_pTextureAtlas->getCapacity ( ), "updateAtlasValueAt: Invalid index" );

	ccV3F_C4B_T2F_Quad*		pQuad = &( ( m_pTextureAtlas->getQuads ( ) ) [ uIndex ] );

    KDfloat		fRow = (KDfloat) ( tValue.r % m_uItemsPerRow );
    KDfloat		fCol = (KDfloat) ( tValue.r / m_uItemsPerRow );

    KDfloat		fWide = (KDfloat) ( m_pTextureAtlas->getTexture ( )->getPixelsWide ( ) );
    KDfloat		fHigh = (KDfloat) ( m_pTextureAtlas->getTexture ( )->getPixelsHigh ( ) );

    KDfloat		fItemWidthInPixels  = m_uItemWidth  * CC_CONTENT_SCALE_FACTOR ( );
    KDfloat		fItemHeightInPixels = m_uItemHeight * CC_CONTENT_SCALE_FACTOR ( );

    KDfloat		fLeft   = ( fRow * fItemWidthInPixels )  / fWide;
    KDfloat		fRight  =  fLeft + fItemWidthInPixels    / fWide;
    KDfloat		fTop    = ( fCol * fItemHeightInPixels ) / fHigh;
    KDfloat		fBottom =   fTop + fItemHeightInPixels   / fHigh;

    pQuad->tl.texCoords.u = fLeft;
    pQuad->tl.texCoords.v = fTop;
    pQuad->tr.texCoords.u = fRight;
    pQuad->tr.texCoords.v = fTop;
    pQuad->bl.texCoords.u = fLeft;
    pQuad->bl.texCoords.v = fBottom;
    pQuad->br.texCoords.u = fRight;
    pQuad->br.texCoords.v = fBottom;

	pQuad->bl.vertices.x = (KDfloat) ( tPos.x * m_uItemWidth );
	pQuad->bl.vertices.y = (KDfloat) ( tPos.y * m_uItemHeight );
	pQuad->bl.vertices.z = 0.0f;
	pQuad->br.vertices.x = (KDfloat) ( tPos.x * m_uItemWidth + m_uItemWidth );
	pQuad->br.vertices.y = (KDfloat) ( tPos.y * m_uItemHeight );
	pQuad->br.vertices.z = 0.0f;
	pQuad->tl.vertices.x = (KDfloat) ( tPos.x * m_uItemWidth );
	pQuad->tl.vertices.y = (KDfloat) ( tPos.y * m_uItemHeight + m_uItemHeight );
	pQuad->tl.vertices.z = 0.0f;
	pQuad->tr.vertices.x = (KDfloat) ( tPos.x * m_uItemWidth + m_uItemWidth );
	pQuad->tr.vertices.y = (KDfloat) ( tPos.y * m_uItemHeight + m_uItemHeight );
	pQuad->tr.vertices.z = 0.0f;

	ccColor4B  tColor = { m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity };

    pQuad->tl.colors = tColor;
    pQuad->tr.colors = tColor;
    pQuad->bl.colors = tColor;
    pQuad->br.colors = tColor;

	m_pTextureAtlas->setDirty ( KD_TRUE );
	KDuint		uTotalQuads = m_pTextureAtlas->getTotalQuads ( );
	if ( uIndex + 1 > uTotalQuads )
	{
		m_pTextureAtlas->increaseTotalQuadsWith ( uIndex + 1 - uTotalQuads );
	}
}

KDvoid CCTileMapAtlas::updateAtlasValues ( KDvoid )
{
	CCAssert ( m_pImage != KD_NULL, "tgaInfo must be non-nil" );

	KDint nTotal = 0;

	for ( KDuint x = 0;x < m_uWidth; x++ ) 
	{
		for ( KDuint y = 0; y < m_uHeight; y++ ) 
		{
			if ( nTotal < m_nItemsToRender ) 
			{
				ccColor3B  tColor = m_pColors [ x + y * m_uWidth ];

				if ( tColor.r != 0 )
				{
					this->updateAtlasValueAt ( ccp ( x, y ), tColor, nTotal );

					CCString*   pKey = CCString::createWithFormat ( "%d,%d", x, y );
                    CCInteger*  pNum = CCInteger::create ( nTotal );
                    m_pPosToAtlasIndex->setObject ( pNum, pKey->getCString ( ) );

					nTotal++;
				}
			}
		}
	}
}


NS_CC_END