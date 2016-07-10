/* -----------------------------------------------------------------------------------
 *
 *      File            Sprite.cpp
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
#include "Sprite.h"
#include "Object/Camera/Camera.h"

cSprite::cSprite ( const KDchar* pFileName, KDint z, Point tPoint )
: cDraw ( z, eObjectType_Sprite )
, m_nTextureUid	 ( 0			)
, m_bIsVisible	 ( true			)
, m_bIsStatic	 ( false		)
, m_tAnchorPoint ( Point::ZERO  )
, m_tPoint		 ( tPoint		)
, m_nOpacity	 ( 255			)
, m_fRotation	 ( 0			)
, m_fScaleX		 ( 1			)
, m_fScaleY		 ( 1			)
, m_bIsFlipX	 ( false		)
, m_bIsFlipY	 ( false		)
, m_tTextureRect ( Rect::ZERO	)
{
	// texture alloc
	m_nTextureUid = texture_alloc ( pFileName );

	// color
	m_tColor.r = 255;
	m_tColor.g = 255;
	m_tColor.b = 255;

	// get texture
	Sprite*		pTex = getTexture ( m_nTextureUid );

	//texture rect
	m_tTextureRect = pTex->getTextureRect ( );
}

cSprite::~cSprite ( KDvoid )
{
	// texture
	texture_release ( m_nTextureUid );
}

Point cSprite::getScreenPoint ( cCamera* pCamera )
{
	if ( m_bIsStatic == false )
	{
		return m_tPoint - Point ( (KDint) ( pCamera->getX ( ) / getFar ( ) ), (KDint) ( pCamera->getY ( ) / getFar ( ) ) );
	}
	else
	{
		return m_tPoint;
	}
}

KDfloat cSprite::getWidth ( KDvoid )
{
	// get texture
	Sprite*		pTex = getTexture ( m_nTextureUid );

	// return
	return pTex->getTexture ( )->getContentSize ( ).width;
}

KDfloat cSprite::getHeight ( KDvoid )
{
	// get texture
	Sprite*		pTex = getTexture ( m_nTextureUid );

	// return
	return pTex->getTexture ( )->getContentSize ( ).height;
}

KDvoid cSprite::setOpacity ( KDint nOpacity )
{
	if ( nOpacity <   0 )	nOpacity = 0;
	if ( nOpacity > 255 )	nOpacity = 255;

	m_nOpacity = nOpacity;
}

KDvoid cSprite::setColor ( KDint r, KDint g, KDint b )
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

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
KDvoid cSprite::setTextureRect ( Rect tRect )
{
	// init
	Sprite*		pTex			  = getTexture ( m_nTextureUid );
	Rect		tOriginalTexRect = pTex->getTextureRect ( );

	// fence
	if ( tRect.origin.x	  < 0								)	tRect.origin.x		= 0;
	if ( tRect.origin.y	  < 0								)	tRect.origin.y		= 0;
	if ( tRect.size.width  > tOriginalTexRect.size.width	)	tRect.size.width	= tOriginalTexRect.size.width;
	if ( tRect.size.height > tOriginalTexRect.size.height	)	tRect.size.height	= tOriginalTexRect.size.height;

	// set
	m_tTextureRect = tRect;
}

KDvoid cSprite::draw ( cCamera* pCamera )
{
	// visible check
	if ( m_bIsVisible == false )
	{
		return;
	}

	// get texture
	Sprite*		pTex = getTexture ( m_nTextureUid );

	// visible
	pTex->setVisible ( true );

	// texture rect
	pTex->setTextureRect ( m_tTextureRect );

	// rotation
	pTex->setRotation ( m_fRotation );

	// opacity
	pTex->setOpacity ( m_nOpacity );

	//color
	pTex->setColor ( m_tColor );

	// scale
	pTex->setScaleX ( m_fScaleX );
	pTex->setScaleY ( m_fScaleY );

	// anchor point
	pTex->setAnchorPoint ( m_tAnchorPoint );

	pTex->setFlippedX ( m_bIsFlipX );
	pTex->setFlippedY ( m_bIsFlipY );

	//pt
	if ( m_bIsStatic == false )	
	{
		pTex->setPosition ( m_tPoint - Point ( (KDint) ( pCamera->getX ( ) / getFar ( ) ), (KDint) ( pCamera->getY ( ) / getFar ( ) ) ) );
	}
	else
	{
		pTex->setPosition ( m_tPoint );
	}

	// draw
	pTex->visit ( );
}
