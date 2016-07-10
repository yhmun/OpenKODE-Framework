/* -----------------------------------------------------------------------------------
 *
 *      File            Button.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
#include "Button.h"
#include "Object/Camera/Camera.h"
#include "Object/Sprite/Sprite.h"

cButton::cButton ( const KDchar* pNorFileName, const KDchar* pPushFileName, const ButtonCallback& pCallback, KDint z, KDint uid, Point tPoint )
: cDraw ( z, eObjectType_Button )
, m_pSpriteNor			( nullptr		)
, m_pSpritePush			( nullptr		)
, m_uId					( uid			)
, m_bIsTouchEnable		( true			)
, m_bIsTouch			( false			)
, m_nTouchID			( 0				)
, m_bIsVisible			( true			)
, m_bIsStatic			( true			)
, m_tAnchorPoint		( Point::ZERO	)
, m_tPoint				( tPoint		)
, m_tTouchCheckRect		( Rect::ZERO	)
, m_nOpacity			( 255			)
, m_fRotation			( 0				)
, m_fScaleX				( 1				)
, m_fScaleY				( 1				)
{
	// sprite
	m_pSpriteNor  = new cSprite ( pNorFileName, z, tPoint );
	m_pSpritePush = new cSprite ( pPushFileName, z, tPoint );	

	m_pSpriteNor ->retain ( );
	m_pSpritePush->retain ( );

	// touch check rect
	m_tTouchCheckRect.origin.x		= 0;
	m_tTouchCheckRect.origin.y		= 0;
	m_tTouchCheckRect.size.width	= m_pSpriteNor->getWidth  ( );
	m_tTouchCheckRect.size.height	= m_pSpriteNor->getHeight ( );

	// color
	m_tColor.r = 255;
	m_tColor.g = 255;
	m_tColor.b = 255;

	m_pCallback = pCallback;
}

cButton::~cButton ( KDvoid )
{
	// sprite
	CC_SAFE_RELEASE ( m_pSpriteNor );
	CC_SAFE_RELEASE ( m_pSpritePush );
}

KDvoid cButton::setOpacity ( KDint nOpacity )
{
	if ( nOpacity <   0 )	nOpacity = 0;
	if ( nOpacity > 255 )	nOpacity = 255;

	m_nOpacity = nOpacity;
}

KDvoid cButton::setColor ( KDint r, KDint g, KDint b )
{
	if ( r <   0 )	r = 0;
	if ( r > 255 )	r = 255;
	if ( g <   0 )	g = 0;
	if ( g > 255 )	g = 255;
	if ( b <   0 )	b = 0;
	if ( b > 255 )	b = 255;

	m_tColor.r = r;
	m_tColor.g = g;
	m_tColor.b = b;
}

KDbool cButton::onTouchBegan ( Touch* pTouch, Event* pEvent )
{
	// switch check
	if ( m_bIsTouchEnable == false )
	{
		return false;
	}

	// rect check in
	Rect	tCheckRect = m_tTouchCheckRect;
	tCheckRect.origin = tCheckRect.origin + m_tPoint;
	if ( tCheckRect.containsPoint ( pTouch->getLocation ( ) ) == false )
	{
		return false;
	}

	// switch
	m_bIsTouch = true;

	// id
	m_nTouchID = pTouch->getID ( );

	return true;
}

KDvoid cButton::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
	// switch check
	if ( m_bIsTouchEnable == false || m_bIsTouch == false || pTouch->getID ( ) != m_nTouchID )	
	{
		return;
	}

	// rect check in
	Rect	tCheckRect = m_tTouchCheckRect;
	tCheckRect.origin = tCheckRect.origin + m_tPoint;
	if ( tCheckRect.containsPoint ( pTouch->getLocation ( ) ) == false )
	{
		// switch
		m_bIsTouch = false;
	}
}

KDvoid cButton::onTouchEnded ( Touch* pTouch, Event* pEvent )
{
	// switch check
	if ( m_bIsTouchEnable == false || m_bIsTouch == false || pTouch->getID ( ) != m_nTouchID )
	{
		return;
	}

	// switch
	m_bIsTouch = false;

	// rect check in
	Rect	tCheckRect = m_tTouchCheckRect;
	tCheckRect.origin = tCheckRect.origin + m_tPoint;
	if ( tCheckRect.containsPoint ( pTouch->getLocation ( ) ) == false )
	{
		// switch
		m_bIsTouch = false;
	}

	// send message
    if ( m_pCallback )
    {
		m_pCallback ( this );
    }
}

KDvoid cButton::draw ( cCamera* pCamera )
{
	// visible check
	if ( m_bIsVisible == false )
	{
		return;
	}

	// get sprite
	cSprite*	pSprite = nullptr;
	if ( m_bIsTouch == false )
	{
		pSprite = m_pSpriteNor;
	}
	else
	{
		pSprite = m_pSpritePush;
	}

	// visible
	pSprite->setIsVisible ( true );

	// static
	pSprite->setIsStatic ( m_bIsStatic );

	// pt
	pSprite->setAnchorPoint ( m_tAnchorPoint );
	pSprite->setPoint ( m_tPoint );

	// rotation
	pSprite->setRotation ( m_fRotation );

	// opacity
	pSprite->setOpacity ( m_nOpacity );

	// color
	pSprite->setColor ( m_tColor.r, m_tColor.g, m_tColor.b );

	// scale
	pSprite->setScaleX ( m_fScaleX );
	pSprite->setScaleY ( m_fScaleY );

	// draw
	pSprite->draw ( pCamera );
}