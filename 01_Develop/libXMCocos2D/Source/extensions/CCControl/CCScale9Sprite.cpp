/* --------------------------------------------------------------------------
 *
 *      File            CCScale9Sprite.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Neofect (Jung Sang-Taik on 12. 3. 16)
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
#include "extensions/CCControl/CCScale9Sprite.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

enum EPositions
{
	eCentre = 0,
	eTop,
	eLeft,
	eRight,
	eBottom,
	eTopRight,
	eTopLeft,
	eBottomRight,
	eBottomLeft
};

CCScale9Sprite::CCScale9Sprite ( KDvoid )
{
	m_fInsetLeft			= 0;
	m_fInsetTop				= 0;
	m_fInsetRight			= 0;
	m_fInsetBottom			= 0;
	m_bSpritesGenerated		= KD_FALSE;
	m_bSpriteFrameRotated	= KD_FALSE;
	m_bPositionsAreDirty	= KD_FALSE;
	m_pScale9Image			= KD_NULL;
	m_pTopLeft				= KD_NULL;
	m_pTop					= KD_NULL;
	m_pTopRight				= KD_NULL;
	m_pLeft					= KD_NULL;
	m_pCentre				= KD_NULL;
	m_pRight				= KD_NULL;
	m_pBottomLeft			= KD_NULL;
	m_pBottom				= KD_NULL;
	m_pBottomRight			= KD_NULL;
	m_bOpacityModifyRGB		= KD_FALSE;
	m_cOpacity				= 255;
	m_tColor				= ccWHITE;
}

CCScale9Sprite::~CCScale9Sprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTopLeft	 );
	CC_SAFE_RELEASE ( m_pTop		 );
	CC_SAFE_RELEASE ( m_pTopRight	 );
	CC_SAFE_RELEASE ( m_pLeft		 );
	CC_SAFE_RELEASE ( m_pCentre		 );
	CC_SAFE_RELEASE ( m_pRight		 );
	CC_SAFE_RELEASE ( m_pBottomLeft  );
	CC_SAFE_RELEASE ( m_pBottom		 );
	CC_SAFE_RELEASE ( m_pBottomRight );
	CC_SAFE_RELEASE ( m_pScale9Image );
}

KDbool CCScale9Sprite::init ( KDvoid )
{
    return this->initWithBatchNode ( KD_NULL, CCRectZero, CCRectZero );
}

KDbool CCScale9Sprite::initWithBatchNode ( CCSpriteBatchNode* pBatchNode, CCRect tRect, CCRect tCapInsets )
{
	return initWithBatchNode ( pBatchNode, tRect, KD_FALSE, tCapInsets );
}

KDbool CCScale9Sprite::initWithBatchNode ( CCSpriteBatchNode* pBatchNode, CCRect tRect, KDbool bRotated, CCRect tCapInsets )
{
    if ( pBatchNode )
    {
        this->updateWithBatchNode ( pBatchNode, tRect, bRotated, tCapInsets );
        this->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    }

    this->m_bPositionsAreDirty = KD_TRUE;
    
    return KD_TRUE;
}

#define TRANSLATE_X( x, y, xtranslate )   x += xtranslate;                       
#define TRANSLATE_Y( x, y, ytranslate )   y += ytranslate;                       

KDbool CCScale9Sprite::updateWithBatchNode ( CCSpriteBatchNode* pBatchNode, CCRect tRect, KDbool bRotated, CCRect tCapInsets )
{
	GLubyte    cOpacity = getOpacity ( );
    ccColor3B  tColor   = getColor ( );

    // Release old sprites
    this->removeAllChildrenWithCleanup ( KD_TRUE );

	CC_SAFE_RELEASE ( m_pTopLeft	 );
	CC_SAFE_RELEASE ( m_pTop		 );
	CC_SAFE_RELEASE ( m_pTopRight	 );
	CC_SAFE_RELEASE ( m_pLeft		 );
	CC_SAFE_RELEASE ( m_pCentre		 );
	CC_SAFE_RELEASE ( m_pRight		 );
	CC_SAFE_RELEASE ( m_pBottomLeft  );
	CC_SAFE_RELEASE ( m_pBottom		 );
	CC_SAFE_RELEASE ( m_pBottomRight );

    if ( this->m_pScale9Image != pBatchNode )
    {
        CC_SAFE_RELEASE ( m_pScale9Image );
        m_pScale9Image = pBatchNode;
        CC_SAFE_RETAIN ( m_pScale9Image );
    }

    m_pScale9Image->removeAllChildrenWithCleanup ( KD_TRUE );

    m_tCapInsets = tCapInsets;
	
	// If there is no given rect
	if ( tRect.equals ( CCRectZero ) )
	{
		// Get the texture size as original
		CCSize  tTexSize = m_pScale9Image->getTextureAtlas ( )->getTexture ( )->getContentSize ( );
		
		tRect = CCRectMake ( 0, 0, tTexSize.cx, tTexSize.cy );
	}
	
	// Set the given rect's size as original size
	m_tSpriteRect		 = tRect;
	m_tOriginalSize		 = tRect.size;
	m_tPreferredSize	 = m_tOriginalSize;
	m_tCapInsetsInternal = tCapInsets;
	
	// Get the image edges
	KDfloat	 w = tRect.size.cx;
	KDfloat	 h = tRect.size.cy;

    // If there is no specified center region
    if ( m_tCapInsetsInternal.equals ( CCRectZero ) )
    {
		m_tCapInsetsInternal = CCRectMake ( w / 3, h / 3, w / 3, h / 3 );
    }

    KDfloat  left_w   = m_tCapInsetsInternal.origin.x;
    KDfloat  center_w = m_tCapInsetsInternal.size.cx;
    KDfloat  right_w  = tRect.size.cx - ( left_w + center_w );

    KDfloat  top_h    = m_tCapInsetsInternal.origin.y;
    KDfloat  center_h = m_tCapInsetsInternal.size.cy;
    KDfloat  bottom_h = tRect.size.cy - ( top_h + center_h );

    // calculate rects

    // ... top row
    KDfloat  x = 0.0;
    KDfloat  y = 0.0;

    // top left
    CCRect  lefttopbounds = CCRectMake ( x, y, left_w, top_h );

    // top center
    TRANSLATE_X ( x, y, left_w );
    CCRect  centertopbounds = CCRectMake ( x, y, center_w, top_h );
        
    // top right
    TRANSLATE_X ( x, y, center_w );
    CCRect righttopbounds = CCRectMake ( x, y, right_w, top_h );

    // ... center row
    x = 0.0;
    y = 0.0;
    TRANSLATE_Y ( x, y, top_h );

    // center left
    CCRect  leftcenterbounds = CCRectMake ( x, y, left_w, center_h);

    // center center
    TRANSLATE_X ( x, y, left_w );
    CCRect  centerbounds = CCRectMake ( x, y, center_w, center_h);

    // center right
    TRANSLATE_X ( x, y, center_w );
    CCRect  rightcenterbounds = CCRectMake ( x, y, right_w, center_h);

    // ... bottom row
    x = 0.0;
    y = 0.0;
    TRANSLATE_Y ( x, y, top_h );
    TRANSLATE_Y ( x, y, center_h);

    // bottom left
    CCRect  leftbottombounds = CCRectMake ( x, y, left_w, bottom_h);

    // bottom center
    TRANSLATE_X ( x, y, left_w );
    CCRect  centerbottombounds = CCRectMake ( x, y, center_w, bottom_h);

    // bottom right
    TRANSLATE_X ( x, y, center_w );
    CCRect  rightbottombounds = CCRectMake ( x, y, right_w, bottom_h);

    if ( !bRotated ) 
	{
        // CCLog("!rotated");

        CCAffineTransform  t = CCAffineTransformMakeIdentity ( );
        t = CCAffineTransformTranslate ( t, tRect.origin.x, tRect.origin.y );

        centerbounds		= CCRectApplyAffineTransform ( centerbounds		 , t );
        rightbottombounds	= CCRectApplyAffineTransform ( rightbottombounds , t );
        leftbottombounds	= CCRectApplyAffineTransform ( leftbottombounds	 , t );
        righttopbounds		= CCRectApplyAffineTransform ( righttopbounds	 , t );
        lefttopbounds		= CCRectApplyAffineTransform ( lefttopbounds	 , t );
        rightcenterbounds	= CCRectApplyAffineTransform ( rightcenterbounds , t );
        leftcenterbounds	= CCRectApplyAffineTransform ( leftcenterbounds	 , t );
        centerbottombounds	= CCRectApplyAffineTransform ( centerbottombounds, t );
        centertopbounds		= CCRectApplyAffineTransform ( centertopbounds	 , t );

        // Centre
        m_pCentre = new CCSprite ( );
        m_pCentre->initWithTexture ( m_pScale9Image->getTexture ( ), centerbounds );
        m_pScale9Image->addChild ( m_pCentre, 0, eCentre );
        
        // Top
        m_pTop = new CCSprite ( );
        m_pTop->initWithTexture ( m_pScale9Image->getTexture ( ), centertopbounds );
        m_pScale9Image->addChild ( m_pTop, 1, eTop );
        
        // Bottom
        m_pBottom = new CCSprite ( );
        m_pBottom->initWithTexture ( m_pScale9Image->getTexture ( ), centerbottombounds );
        m_pScale9Image->addChild ( m_pBottom, 1, eBottom );
        
        // Left
        m_pLeft = new CCSprite ( );
        m_pLeft->initWithTexture ( m_pScale9Image->getTexture ( ), leftcenterbounds );
        m_pScale9Image->addChild ( m_pLeft, 1, eLeft );
        
        // Right
        m_pRight = new CCSprite ( );
        m_pRight->initWithTexture ( m_pScale9Image->getTexture ( ), rightcenterbounds );
        m_pScale9Image->addChild ( m_pRight, 1, eRight );
        
        // Top left
        m_pTopLeft = new CCSprite ( );
        m_pTopLeft->initWithTexture ( m_pScale9Image->getTexture ( ), lefttopbounds );
        m_pScale9Image->addChild ( m_pTopLeft, 2, eTopLeft );
        
        // Top right
        m_pTopRight = new CCSprite ( );
        m_pTopRight->initWithTexture ( m_pScale9Image->getTexture ( ), righttopbounds );
        m_pScale9Image->addChild ( m_pTopRight, 2, eTopRight );
        
        // Bottom left
        m_pBottomLeft = new CCSprite ( );
        m_pBottomLeft->initWithTexture ( m_pScale9Image->getTexture ( ), leftbottombounds );
        m_pScale9Image->addChild ( m_pBottomLeft, 2, eBottomLeft );
        
        // Bottom right
        m_pBottomRight = new CCSprite ( );
        m_pBottomRight->initWithTexture ( m_pScale9Image->getTexture ( ), rightbottombounds );
        m_pScale9Image->addChild ( m_pBottomRight, 2, eBottomRight );
    } 
	else 
	{
        // set up transformation of coordinates
        // to handle the case where the sprite is stored rotated
        // in the spritesheet
        // CCLog("rotated");

        CCAffineTransform  t = CCAffineTransformMakeIdentity ( );

        CCRect  rotatedcenterbounds			= centerbounds;
        CCRect  rotatedrightbottombounds	= rightbottombounds;
        CCRect  rotatedleftbottombounds		= leftbottombounds;
        CCRect  rotatedrighttopbounds		= righttopbounds;
        CCRect  rotatedlefttopbounds		= lefttopbounds;
        CCRect  rotatedrightcenterbounds	= rightcenterbounds;
        CCRect  rotatedleftcenterbounds		= leftcenterbounds;
        CCRect  rotatedcenterbottombounds	= centerbottombounds;
        CCRect  rotatedcentertopbounds		= centertopbounds;
        
        t = CCAffineTransformTranslate ( t, tRect.size.cy+tRect.origin.x, tRect.origin.y );
        t = CCAffineTransformRotate ( t, 1.57079633f);
        
        centerbounds		= CCRectApplyAffineTransform ( centerbounds	 	 , t );
        rightbottombounds	= CCRectApplyAffineTransform ( rightbottombounds , t );
        leftbottombounds	= CCRectApplyAffineTransform ( leftbottombounds	 , t );
        righttopbounds		= CCRectApplyAffineTransform ( righttopbounds	 , t );
        lefttopbounds		= CCRectApplyAffineTransform ( lefttopbounds	 , t );
        rightcenterbounds	= CCRectApplyAffineTransform ( rightcenterbounds , t );
        leftcenterbounds	= CCRectApplyAffineTransform ( leftcenterbounds	 , t );
        centerbottombounds	= CCRectApplyAffineTransform ( centerbottombounds, t );
        centertopbounds		= CCRectApplyAffineTransform ( centertopbounds	 , t );

        rotatedcenterbounds.origin		 = centerbounds.origin;
        rotatedrightbottombounds.origin  = rightbottombounds.origin;
        rotatedleftbottombounds.origin	 = leftbottombounds.origin;
        rotatedrighttopbounds.origin	 = righttopbounds.origin;
        rotatedlefttopbounds.origin		 = lefttopbounds.origin;
        rotatedrightcenterbounds.origin  = rightcenterbounds.origin;
        rotatedleftcenterbounds.origin	 = leftcenterbounds.origin;
        rotatedcenterbottombounds.origin = centerbottombounds.origin;
        rotatedcentertopbounds.origin	 = centertopbounds.origin;

        // Centre
        m_pCentre = new CCSprite ( );
        m_pCentre->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedcenterbounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pCentre, 0, eCentre );
        
        // Top
        m_pTop = new CCSprite ( );
        m_pTop->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedcentertopbounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pTop, 1, eTop );
        
        // Bottom
        m_pBottom = new CCSprite ( );
        m_pBottom->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedcenterbottombounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pBottom, 1, eBottom );
        
        // Left
        m_pLeft = new CCSprite ( );
        m_pLeft->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedleftcenterbounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pLeft, 1, eLeft );
        
        // Right
        m_pRight = new CCSprite ( );
        m_pRight->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedrightcenterbounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pRight, 1, eRight );
        
        // Top left
        m_pTopLeft = new CCSprite ( );
        m_pTopLeft->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedlefttopbounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pTopLeft, 2, eTopLeft );
        
        // Top right
        m_pTopRight = new CCSprite ( );
        m_pTopRight->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedrighttopbounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pTopRight, 2, eTopRight );
        
        // Bottom left
        m_pBottomLeft = new CCSprite ( );
        m_pBottomLeft->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedleftbottombounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pBottomLeft, 2, eBottomLeft );
        
        // Bottom right
        m_pBottomRight = new CCSprite ( );
        m_pBottomRight->initWithTexture ( m_pScale9Image->getTexture ( ), rotatedrightbottombounds, KD_TRUE );
        m_pScale9Image->addChild ( m_pBottomRight, 2, eBottomRight );
    }

    this->setContentSize ( tRect.size );
    this->addChild ( m_pScale9Image );
    
    if ( m_bSpritesGenerated )
    {
        // Restore color and opacity
        this->setOpacity ( cOpacity );
        this->setColor ( tColor );
    }
    m_bSpritesGenerated = KD_TRUE;

	return KD_TRUE;
}

KDvoid CCScale9Sprite::setContentSize ( const CCSize& tSize )
{
	CCNode::setContentSize ( tSize );
	this->m_bPositionsAreDirty = KD_TRUE;
}

KDvoid CCScale9Sprite::updatePositions ( KDvoid )
{
    // Check that instances are non-NULL
    if ( ! ( ( m_pTopLeft ) && ( m_pTopRight ) && ( m_pBottomRight ) && ( m_pBottomLeft ) && ( m_pCentre ) ) ) 
	{
        // if any of the above sprites are NULL, return
        return;
    }

	CCSize   tSize = this->m_tContentSize;
	
	KDfloat  fSizableW = tSize.cx - m_pTopLeft->getContentSize ( ).cx - m_pTopRight   ->getContentSize ( ).cx;
	KDfloat  fSizableH = tSize.cy - m_pTopLeft->getContentSize ( ).cy - m_pBottomRight->getContentSize ( ).cy;
	KDfloat  fScaleH = fSizableW / m_pCentre->getContentSize ( ).cx;
	KDfloat  fScaleV = fSizableH / m_pCentre->getContentSize ( ).cy;
	
	m_pCentre->setScaleX ( fScaleH );
	m_pCentre->setScaleY ( fScaleV );

	KDfloat  fRescaledW = m_pCentre->getContentSize ( ).cx * fScaleH;
	KDfloat  fRescaledH = m_pCentre->getContentSize ( ).cy * fScaleV;
	
	KDfloat  fLeftW   = m_pBottomLeft->getContentSize ( ).cx;
    KDfloat  fBottomH = m_pBottomLeft->getContentSize ( ).cy;

    m_pBottomLeft ->setAnchorPoint ( CCPointZero );
    m_pBottomRight->setAnchorPoint ( CCPointZero );
    m_pTopLeft	  ->setAnchorPoint ( CCPointZero );
    m_pTopRight	  ->setAnchorPoint ( CCPointZero );
    m_pLeft		  ->setAnchorPoint ( CCPointZero );
    m_pRight	  ->setAnchorPoint ( CCPointZero );
    m_pTop		  ->setAnchorPoint ( CCPointZero );
    m_pBottom	  ->setAnchorPoint ( CCPointZero );
    m_pCentre	  ->setAnchorPoint ( CCPointZero );

    // Position corners
    m_pBottomLeft ->setPosition ( ccp ( 0, 0 ) );
    m_pBottomRight->setPosition ( ccp ( fLeftW + fRescaledW, 0 ) );
    m_pTopLeft    ->setPosition ( ccp ( 0, fBottomH + fRescaledH ) );
    m_pTopRight   ->setPosition ( ccp ( fLeftW + fRescaledW, fBottomH + fRescaledH ) );

	// Scale and position borders
    m_pLeft	 ->setPosition ( ccp ( 0, fBottomH ) );
    m_pLeft	 ->setScaleY   ( fScaleV );
    m_pRight ->setPosition ( ccp ( fLeftW + fRescaledW, fBottomH ) );
    m_pRight ->setScaleY   ( fScaleV );
    m_pBottom->setPosition ( ccp ( fLeftW, 0 ) );
    m_pBottom->setScaleX   ( fScaleH );
    m_pTop	 ->setPosition ( ccp ( fLeftW, fBottomH + fRescaledH ) );
	m_pTop	 ->setScaleX   ( fScaleH );
	
	// Position m_pCentre
    m_pCentre->setPosition ( ccp ( fLeftW, fBottomH ) );
}

KDbool CCScale9Sprite::initWithFile ( const KDchar* szFile, CCRect tRect, CCRect tCapInsets )
{
	CCAssert ( szFile != KD_NULL, "Invalid file for sprite" );
    
    CCSpriteBatchNode*  pBatchNode = CCSpriteBatchNode::create ( szFile, 9 );

    return this->initWithBatchNode ( pBatchNode, tRect, tCapInsets );
}

CCScale9Sprite* CCScale9Sprite::create ( const KDchar* szFile, CCRect tRect, CCRect tCapInsets )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithFile ( szFile, tRect, tCapInsets ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCScale9Sprite::initWithFile ( const KDchar* szFile, CCRect tRect )
{
	CCAssert ( szFile != KD_NULL, "Invalid file for sprite" );
    return this->initWithFile ( szFile, tRect, CCRectZero );
}

CCScale9Sprite* CCScale9Sprite::create ( const KDchar* szFile, CCRect tRect )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithFile ( szFile, tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCScale9Sprite::initWithFile ( CCRect tCapInsets, const KDchar* szFile )
{
    return this->initWithFile ( szFile, CCRectZero, tCapInsets );
}

CCScale9Sprite* CCScale9Sprite::create ( CCRect tCapInsets, const KDchar* szFile )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithFile ( tCapInsets, szFile ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCScale9Sprite::initWithFile ( const KDchar* szFile )
{
    return this->initWithFile ( szFile, CCRectZero );	
}

CCScale9Sprite* CCScale9Sprite::create ( const KDchar* szFile )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithFile ( szFile ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCScale9Sprite::initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, CCRect tCapInsets )
{
	CCTexture2D*  pTexture = pSpriteFrame->getTexture ( );
	CCAssert ( pTexture != KD_NULL, "CCTexture must be not nil" );
    
    CCSpriteBatchNode*  pBatchNode = CCSpriteBatchNode::createWithTexture ( pSpriteFrame->getTexture ( ), 9 );
	CCAssert ( pBatchNode != KD_NULL, "CCSpriteBatchNode must be not nil" );

	return this->initWithBatchNode ( pBatchNode, pSpriteFrame->getRect ( ), pSpriteFrame->isRotated ( ), tCapInsets );
}

CCScale9Sprite* CCScale9Sprite::createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame, CCRect tCapInsets )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithSpriteFrame ( pSpriteFrame, tCapInsets ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;	
}

KDbool CCScale9Sprite::initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
	CCAssert ( pSpriteFrame != KD_NULL, "Invalid spriteFrame for sprite" );
    return this->initWithSpriteFrame ( pSpriteFrame, CCRectZero );
}

CCScale9Sprite* CCScale9Sprite::createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithSpriteFrame ( pSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;	
}

KDbool CCScale9Sprite::initWithSpriteFrameName ( const KDchar* szSpriteFrame, CCRect tCapInsets )
{
    CCAssert ( ( CCSpriteFrameCache::sharedSpriteFrameCache ( ) ) != KD_NULL, "sharedSpriteFrameCache must be non-NULL" );

	CCSpriteFrame*  pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szSpriteFrame );
	CCAssert ( pFrame != KD_NULL, "CCSpriteFrame must be non-NULL" );
    
	if ( KD_NULL == pFrame )
	{
		return KD_FALSE;
	}
    
    return this->initWithSpriteFrame ( pFrame, tCapInsets );
}

CCScale9Sprite* CCScale9Sprite::createWithSpriteFrameName ( const KDchar* szSpriteFrame, CCRect tCapInsets )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithSpriteFrameName ( szSpriteFrame, tCapInsets ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;	
}

KDbool CCScale9Sprite::initWithSpriteFrameName ( const KDchar* szSpriteFrame )
{
    return this->initWithSpriteFrameName ( szSpriteFrame, CCRectZero );
}

CCScale9Sprite* CCScale9Sprite::createWithSpriteFrameName ( const KDchar* szSpriteFrame )
{
	CCAssert ( szSpriteFrame != KD_NULL, "spriteFrameName must be non-NULL" );

	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithSpriteFrameName ( szSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
		CCLOG ( "Could not allocate CCScale9Sprite()" );
	}

	return pRet;
}

CCScale9Sprite* CCScale9Sprite::resizableSpriteWithCapInsets ( CCRect tCapInsets )
{
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->initWithBatchNode ( m_pScale9Image, m_tSpriteRect, tCapInsets ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCScale9Sprite* CCScale9Sprite::create ( KDvoid )
{ 
	CCScale9Sprite*  pRet = new CCScale9Sprite ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid CCScale9Sprite::setPreferredSize ( CCSize tPreferredSize )
{
    this->setContentSize ( tPreferredSize );
    this->m_tPreferredSize = tPreferredSize;
}

CCSize CCScale9Sprite::getPreferredSize ( KDvoid )
{
    return this->m_tPreferredSize;
}

KDvoid CCScale9Sprite::setCapInsets ( CCRect tCapInsets )
{
	CCSize  tSize = this->m_tContentSize;
    this->updateWithBatchNode ( this->m_pScale9Image, this->m_tSpriteRect, m_bSpriteFrameRotated, tCapInsets );
    this->setContentSize ( tSize );
}

CCRect CCScale9Sprite::getCapInsets ( KDvoid )
{
	return m_tCapInsets;
}

KDvoid CCScale9Sprite::updateCapInset ( KDvoid )
{
    CCRect  tInsets;

    if ( m_fInsetLeft == 0 && m_fInsetTop == 0 && m_fInsetRight == 0 && m_fInsetBottom == 0 )
    {
        tInsets = CCRectZero;
    }
    else
    {
        if ( m_bSpriteFrameRotated )
        {
            tInsets = CCRectMake
			(
				m_fInsetBottom,
                m_fInsetLeft,
                m_tSpriteRect.size.cx - m_fInsetRight - m_fInsetLeft,
                m_tSpriteRect.size.cy - m_fInsetTop - m_fInsetBottom
			);
        }
        else
        {
            tInsets = CCRectMake
			(
				m_fInsetLeft,
                m_fInsetTop,
                m_tSpriteRect.size.cx - m_fInsetLeft - m_fInsetRight,
                m_tSpriteRect.size.cy - m_fInsetTop - m_fInsetBottom
			);
        }
    }
    this->setCapInsets ( tInsets );
}

KDvoid CCScale9Sprite::setOpacityModifyRGB ( KDbool bOpacityModifyRGB )
{
	m_bOpacityModifyRGB = bOpacityModifyRGB;

	CCObject*  pChild;
    CCArray*   pChildren = m_pScale9Image->getChildren ( );
    CCARRAY_FOREACH ( pChildren, pChild ) 
	{
		CCRGBAProtocol*  pNode = dynamic_cast<CCRGBAProtocol*> ( pChild );
		if ( pNode )
        {
            pNode->setOpacityModifyRGB ( m_bOpacityModifyRGB );
        }
	}
}

KDbool CCScale9Sprite::isOpacityModifyRGB ( KDvoid )
{
	return m_bOpacityModifyRGB;
}

KDvoid CCScale9Sprite::setSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
    CCSpriteBatchNode*  pBatchnode = CCSpriteBatchNode::createWithTexture ( pSpriteFrame->getTexture ( ), 9 );
    this->updateWithBatchNode ( pBatchnode, pSpriteFrame->getRect ( ), pSpriteFrame->isRotated ( ), CCRectZero );

    // Reset insets
    this->m_fInsetLeft	 = 0;
    this->m_fInsetTop	 = 0;
    this->m_fInsetRight  = 0;
    this->m_fInsetBottom = 0;
}

KDfloat CCScale9Sprite::getInsetLeft ( KDvoid )
{
    return this->m_fInsetLeft;
}

KDfloat CCScale9Sprite::getInsetTop ( KDvoid )
{
    return this->m_fInsetTop;
}

KDfloat CCScale9Sprite::getInsetRight ( KDvoid )
{
    return this->m_fInsetRight;
}

KDfloat CCScale9Sprite::getInsetBottom ( KDvoid )
{
    return this->m_fInsetBottom;
}

KDvoid CCScale9Sprite::setInsetLeft ( KDfloat fInsetLeft )
{
    this->m_fInsetLeft = fInsetLeft;
    this->updateCapInset ( );
}

KDvoid CCScale9Sprite::setInsetTop ( KDfloat fInsetTop )
{
    this->m_fInsetTop = fInsetTop;
    this->updateCapInset ( );
}

KDvoid CCScale9Sprite::setInsetRight ( KDfloat fInsetRight )
{
    this->m_fInsetRight = fInsetRight;
    this->updateCapInset ( );
}

KDvoid CCScale9Sprite::setInsetBottom ( KDfloat fInsetBottom )
{
    this->m_fInsetBottom = fInsetBottom;
    this->updateCapInset ( );
}

KDvoid CCScale9Sprite::visit ( KDvoid )
{
    if ( this->m_bPositionsAreDirty )
    {
        this->updatePositions ( );
        this->m_bPositionsAreDirty = KD_FALSE;
    }

    CCNode::visit ( );
}

KDvoid CCScale9Sprite::setColor ( const ccColor3B& tColor )
{
    m_tColor = tColor;

    CCObject*  pChild;
    CCArray*   pChildren = m_pScale9Image->getChildren ( );
    CCARRAY_FOREACH ( pChildren, pChild )
    {
        CCRGBAProtocol* pNode = dynamic_cast<CCRGBAProtocol*> ( pChild );
        if ( pNode )
        {
            pNode->setColor ( tColor );
        }
    }
}

KDvoid CCScale9Sprite::setOpacity ( GLubyte cOpacity )
{
    m_cOpacity = cOpacity;

    CCObject*  pChild;
    CCArray*   pChildren = m_pScale9Image->getChildren ( );
    CCARRAY_FOREACH ( pChildren, pChild )
    {
        CCRGBAProtocol*  pNode = dynamic_cast<CCRGBAProtocol*> ( pChild );
        if ( pNode )
        {
            pNode->setOpacity ( cOpacity );
        }
    }
}

NS_CC_END