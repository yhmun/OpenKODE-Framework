/* -----------------------------------------------------------------------------------
 *
 *      File            CCTileMapAtlas.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/tilemap_parallax_nodes/CCTileMapAtlas.h"
#include "platform/CCFileUtils.h"
#include "2d/textures/CCTextureAtlas.h"
#include "ccConfig.h"
#include "base/CCDictionary.h"
#include "base/CCInteger.h"
#include "2d/CCDirector.h"

NS_CC_BEGIN

// implementation TileMapAtlas

TileMapAtlas* TileMapAtlas::create ( const KDchar* pTile, const KDchar* pMapFile, KDint nTileWidth, KDint nTileHeight )
{
    TileMapAtlas*	pRet = new TileMapAtlas ( );

    if ( pRet->initWithTileFile ( pTile, pMapFile, nTileWidth, nTileHeight ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

bool TileMapAtlas::initWithTileFile ( const KDchar* pTile, const KDchar* pMapFile, KDint nTileWidth, KDint nTileHeight )
{
	std::string		fullPath = FileUtils::getInstance ( )->fullPathForFilename ( pMapFile );

	this->releaseMap ( );

	m_pImage = kdGetImageATX ( fullPath.c_str ( ), KD_IMAGE_FORMAT_RGB888_ATX, KD_IMAGE_FLAG_FLIP_Y_ATX );
	if ( !m_pImage )
	{
		return KD_FALSE;
	}

	m_uWidth  = kdGetImageIntATX ( m_pImage, KD_IMAGE_WIDTH_ATX  );
	m_uHeight = kdGetImageIntATX ( m_pImage, KD_IMAGE_HEIGHT_ATX );
	m_pColors = (Color3B*) kdGetImagePointerATX ( m_pImage, KD_IMAGE_POINTER_BUFFER_ATX );

    this->calculateItemsToRender ( );

    if ( AtlasNode::initWithTileFile ( pTile, nTileWidth, nTileHeight, m_nItemsToRender ) )
    {
        m_pPosToAtlasIndex = new Dictionary ( );
        m_pPosToAtlasIndex->init ( );
        this->updateAtlasValues();
		this->setContentSize ( Size ( (KDfloat) ( m_uWidth * m_nItemWidth ), (KDfloat) ( m_uHeight * m_nItemHeight ) ) );
        return true;
    }

    return false;
}

TileMapAtlas::TileMapAtlas ( KDvoid )
{
    m_pImage			= KD_NULL;
    m_uWidth			= 0;
    m_uHeight			= 0;
    m_pColors			= 0;
    m_pPosToAtlasIndex	= KD_NULL;
    m_nItemsToRender	= 0;
}

TileMapAtlas::~TileMapAtlas ( KDvoid )
{
	this->releaseMap ( );
}

KDvoid TileMapAtlas::releaseMap ( KDvoid )
{
	if ( m_pImage )
	{
		kdFreeImageATX ( m_pImage );
		m_pImage = KD_NULL;
	}

    CC_SAFE_RELEASE ( m_pPosToAtlasIndex );
}

KDvoid TileMapAtlas::calculateItemsToRender ( KDvoid )
{
	CCASSERT ( m_pImage != NULL, "tgaInfo must be non-nil" );

    m_nItemsToRender = 0;
	for ( KDuint x = 0; x < m_uWidth; x++ )
	{
		for ( KDuint y = 0; y < m_uHeight; y++ )
		{
			Color3B		tValue = m_pColors [ x + y * m_uWidth ];
			if ( tValue.r )
			{
				++m_nItemsToRender;
			}
		}
	}
}

// TileMapAtlas - Atlas generation / updates
KDvoid TileMapAtlas::setTile ( const Color3B& tTile, const Point& tPosition )
{
	CCASSERT ( m_pImage != NULL, "tgaInfo must not be nil" );
    CCASSERT ( m_pPosToAtlasIndex != NULL, "posToAtlasIndex must not be nil" );
	CCASSERT ( tPosition.x < m_uWidth, "Invalid position.x" );
	CCASSERT ( tPosition.y < m_uHeight, "Invalid position.x" );
    CCASSERT ( tTile.r != 0, "R component must be non 0" );

	Color3B		tValue = m_pColors [ (KDuint) ( tPosition.x + tPosition.y * m_uWidth ) ];
    if ( tValue.r == 0 )
    {
        CCLOG ( "cocos2d: Value.r must be non 0." );
    } 
    else
    {
		m_pColors [ (KDuint) ( tPosition.x + tPosition.y * m_uWidth ) ] = tTile;

        // XXX: this method consumes a lot of memory
        // XXX: a tree of something like that shall be implemented
        Integer*	pNum = (Integer*) m_pPosToAtlasIndex->objectForKey
		(
			String::createWithFormat ( "%d,%d", (KDint32) tPosition.x, (KDint32) tPosition.y )->getCString ( ) 
		);
        this->updateAtlasValueAt ( tPosition, tTile, pNum->getValue ( ) );
    }    
}

Color3B TileMapAtlas::getTileAt ( const Point& tPosition ) const
{
	CCASSERT ( m_pImage != KD_NULL, "tgaInfo must not be nil" );
	CCASSERT ( tPosition.x < m_uWidth, "Invalid position.x" );
	CCASSERT ( tPosition.y < m_uHeight, "Invalid position.y" );

	Color3B		tValue = m_pColors [ (KDint) ( tPosition.x + tPosition.y * m_uWidth ) ];

    return tValue;    
}

KDvoid TileMapAtlas::updateAtlasValueAt ( const Point& tPos, const Color3B& tValue, KDint nIndex )
{
    CCASSERT ( nIndex >= 0 && nIndex < m_pTextureAtlas->getCapacity ( ), "updateAtlasValueAt: Invalid index" );

    V3F_C4B_T2F_Quad*	pQuad = & ( ( m_pTextureAtlas->getQuads ( ) ) [ nIndex ] );

    KDint		x = tPos.x;
    KDint		y = tPos.y;

    KDfloat		fRow = (KDfloat) ( tValue.r % m_nItemsPerRow );
    KDfloat		fCol = (KDfloat) ( tValue.r / m_nItemsPerRow );

    KDfloat		fTextureWide = (KDfloat) ( m_pTextureAtlas->getTexture()->getPixelsWide ( ) );
    KDfloat		fTextureHigh = (KDfloat) ( m_pTextureAtlas->getTexture()->getPixelsHigh ( ) );

    KDfloat		fItemWidthInPixels  = m_nItemWidth  * CC_CONTENT_SCALE_FACTOR ( );
    KDfloat		fItemHeightInPixels = m_nItemHeight * CC_CONTENT_SCALE_FACTOR ( );

#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
    KDfloat		fLeft	= ( 2 * fRow * fItemWidthInPixels + 1  ) / ( 2 * fTextureWide );
    KDfloat		fRight	= fLeft + ( fItemWidthInPixels * 2 - 2 ) / ( 2 * fTextureWide );
    KDfloat		fTop	= ( 2 * fCol * fItemHeightInPixels + 1 ) / ( 2 * fTextureHigh );
    KDfloat		fBottom	= fTop + ( fItemHeightInPixels * 2 - 2 ) / ( 2 * fTextureHigh );
#else
    KDfloat		fLeft	= ( fRow * fItemWidthInPixels  ) / fTextureWide;
    KDfloat		fRight	=  fLeft + fItemWidthInPixels    / fTextureWide;
    KDfloat		fTop	= ( fCol * fItemHeightInPixels ) / fTextureHigh;
    KDfloat		fBottom	=   fTop + fItemHeightInPixels   / fTextureHigh;
#endif

    pQuad->tl.texCoords.u = fLeft;
    pQuad->tl.texCoords.v = fTop;
    pQuad->tr.texCoords.u = fRight;
    pQuad->tr.texCoords.v = fTop;
    pQuad->bl.texCoords.u = fLeft;
    pQuad->bl.texCoords.v = fBottom;
    pQuad->br.texCoords.u = fRight;
    pQuad->br.texCoords.v = fBottom;

    pQuad->bl.vertices.x = (KDfloat) ( x * m_nItemWidth  );
    pQuad->bl.vertices.y = (KDfloat) ( y * m_nItemHeight );
    pQuad->bl.vertices.z = 0.0f;
    pQuad->br.vertices.x = (KDfloat) ( x * m_nItemWidth + m_nItemWidth );
    pQuad->br.vertices.y = (KDfloat) ( y * m_nItemHeight );
    pQuad->br.vertices.z = 0.0f;
    pQuad->tl.vertices.x = (KDfloat) ( x * m_nItemWidth);
    pQuad->tl.vertices.y = (KDfloat) ( y * m_nItemHeight + m_nItemHeight );
    pQuad->tl.vertices.z = 0.0f;
    pQuad->tr.vertices.x = (KDfloat) ( x * m_nItemWidth  + m_nItemWidth  );
    pQuad->tr.vertices.y = (KDfloat) ( y * m_nItemHeight + m_nItemHeight );
    pQuad->tr.vertices.z = 0.0f;

    Color4B		tColor ( m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity );
    pQuad->tr.colors = tColor;
    pQuad->tl.colors = tColor;
    pQuad->br.colors = tColor;
    pQuad->bl.colors = tColor;

    m_pTextureAtlas->setDirty ( true );
    KDint	nTotalQuads = m_pTextureAtlas->getTotalQuads ( );
    if ( nIndex + 1 > nTotalQuads )
	{
        m_pTextureAtlas->increaseTotalQuadsWith ( nIndex + 1 - nTotalQuads );
    }
}

KDvoid TileMapAtlas::updateAtlasValues ( KDvoid )
{
	CCASSERT ( m_pImage != KD_NULL, "tgaInfo must be non-nil" );

    KDint	nTotal = 0;

	for ( KDuint x = 0; x < m_uWidth; x++ )
    {
		for ( KDuint y = 0; y < m_uHeight; y++ )
        {
            if ( nTotal < m_nItemsToRender ) 
            {
				Color3B		tValue = m_pColors [ x + y * m_uWidth ];

                if ( tValue.r != 0 )
                {
                    this->updateAtlasValueAt ( Point ( x, y ), tValue, nTotal );

                    String*		pKey = String::createWithFormat ( "%d,%d", x, y );
                    Integer*	pNum = Integer::create ( nTotal );
                    m_pPosToAtlasIndex->setObject ( pNum, pKey->getCString ( ) );

                    nTotal++;
                }
            }
        }
    }
}

NS_CC_END
