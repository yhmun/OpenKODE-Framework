/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrame.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2011 Ricardo Quesada
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
#include "sprite_nodes/CCSpriteFrame.h"
#include "textures/CCTextureCache.h"
#include "support/CCDirector.h"

NS_CC_BEGIN
    
//
// implementation of CCSpriteFrame
//

CCSpriteFrame* CCSpriteFrame::createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	CCSpriteFrame*  pSpriteFrame = new CCSpriteFrame ( );

	if ( pSpriteFrame && pSpriteFrame->initWithTexture ( pTexture, tRect ) )
	{
		pSpriteFrame->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pSpriteFrame );
	}

	return pSpriteFrame;
}

CCSpriteFrame* CCSpriteFrame::create ( const KDchar* szFileName, const CCRect& tRect )
{
	CCSpriteFrame*  pSpriteFrame = new CCSpriteFrame ( );

	if ( pSpriteFrame && pSpriteFrame->initWithTextureFilename ( szFileName, tRect ) )
	{
		pSpriteFrame->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pSpriteFrame );
	}

	return pSpriteFrame;
}

CCSpriteFrame* CCSpriteFrame::createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize )
{
    CCSpriteFrame*  pSpriteFrame = new CCSpriteFrame ( );

	if ( pSpriteFrame && pSpriteFrame->initWithTexture ( pTexture, tRect, bRotated, tOffset, tOriginalSize ) )
	{
		pSpriteFrame->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pSpriteFrame );
	}

	return pSpriteFrame;
}

CCSpriteFrame* CCSpriteFrame::create ( const KDchar* szFileName, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize )
{
    CCSpriteFrame*  pSpriteFrame = new CCSpriteFrame ( );

	if ( pSpriteFrame && pSpriteFrame->initWithTextureFilename ( szFileName, tRect, bRotated, tOffset, tOriginalSize ) )
	{
		pSpriteFrame->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pSpriteFrame );
	}

	return pSpriteFrame;
}

KDbool CCSpriteFrame::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	CCRect  tRectInPixels = CC_RECT_POINTS_TO_PIXELS ( tRect );
	return initWithTexture ( pTexture, tRectInPixels, KD_FALSE, CCPointZero, tRect.size );
}

KDbool CCSpriteFrame::initWithTextureFilename ( const KDchar* szFileName, const CCRect& tRect )
{
	CCRect  tRectInPixels = CC_RECT_POINTS_TO_PIXELS ( tRect );
    return initWithTextureFilename ( szFileName, tRectInPixels, KD_FALSE, CCPointZero, tRect.size );
}

KDbool CCSpriteFrame::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize )
{
	m_pTexture = pTexture;
	CC_SAFE_RETAIN ( pTexture );	

	m_tRectInPixels			= tRect;
	m_tRect					= CC_RECT_PIXELS_TO_POINTS ( tRect );
	m_tOffsetInPixels		= tOffset;
	m_tOffset				= CC_POINT_PIXELS_TO_POINTS ( tOffset );
	m_tOriginalSizeInPixels	= tOriginalSize;
	m_tOriginalSize			= CC_SIZE_PIXELS_TO_POINTS ( tOriginalSize );
	m_bRotated				= bRotated;

	return KD_TRUE;
}

KDbool CCSpriteFrame::initWithTextureFilename ( const KDchar* szFileName, const CCRect& tRect, KDbool bRotated, const CCPoint& tOffset, const CCSize& tOriginalSize )
{
    m_pTexture				= KD_NULL;
    m_sTextureFile			= szFileName;
	m_tRectInPixels			= tRect;
	m_tRect					= CC_RECT_PIXELS_TO_POINTS ( tRect );
	m_tOffsetInPixels		= tOffset;
	m_tOffset				= CC_POINT_PIXELS_TO_POINTS ( tOffset );
	m_tOriginalSizeInPixels	= tOriginalSize;
	m_tOriginalSize			= CC_SIZE_PIXELS_TO_POINTS ( tOriginalSize );
	m_bRotated				= bRotated;

    return KD_TRUE;
}

CCSpriteFrame::~CCSpriteFrame ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCSpriteFrame = %p", this );

	CC_SAFE_RELEASE ( m_pTexture );
}

CCObject* CCSpriteFrame::copyWithZone ( CCZone* pZone )
{
	CC_UNUSED_PARAM ( pZone );

	CCSpriteFrame*  pCopy = new CCSpriteFrame ( );
	
    pCopy->initWithTextureFilename ( m_sTextureFile.c_str ( ), m_tRectInPixels, m_bRotated, m_tOffsetInPixels, m_tOriginalSizeInPixels );
	pCopy->setTexture ( m_pTexture );
	
	return pCopy;
}

KDvoid CCSpriteFrame::setRect ( const CCRect& tRect )
{
    m_tRect = tRect;
    m_tRectInPixels = CC_RECT_POINTS_TO_PIXELS ( tRect );
}

KDvoid CCSpriteFrame::setRectInPixels ( const CCRect& tRectInPixels )
{
    m_tRectInPixels = tRectInPixels;
    m_tRect = CC_RECT_PIXELS_TO_POINTS ( tRectInPixels );
}

const CCPoint& CCSpriteFrame::getOffset ( KDvoid )
{
    return m_tOffset;
}

KDvoid CCSpriteFrame::setOffset ( const CCPoint& tOffsets )
{
    m_tOffset = tOffsets;
    m_tOffsetInPixels = CC_POINT_POINTS_TO_PIXELS ( m_tOffset );
}

const CCPoint& CCSpriteFrame::getOffsetInPixels ( KDvoid )
{
    return m_tOffsetInPixels;
}

KDvoid CCSpriteFrame::setOffsetInPixels ( const CCPoint& tOffsetInPixels )
{
    m_tOffsetInPixels = tOffsetInPixels;
    m_tOffset = CC_POINT_PIXELS_TO_POINTS ( m_tOffsetInPixels );
}

KDvoid CCSpriteFrame::setTexture ( CCTexture2D* pTexture )
{
    if ( m_pTexture != pTexture ) 
	{
        CC_SAFE_RELEASE ( m_pTexture );
        CC_SAFE_RETAIN ( pTexture );
        m_pTexture = pTexture;
    }
}

CCTexture2D* CCSpriteFrame::getTexture ( KDvoid )
{
    if ( m_pTexture ) 
	{
        return m_pTexture;
    }

    if ( m_sTextureFile.length ( ) > 0 ) 
	{
        return CCTextureCache::sharedTextureCache ( )->addImage ( m_sTextureFile.c_str ( ) );
    }
    // no texture or texture filename
    return KD_NULL;
}

NS_CC_END