/* -----------------------------------------------------------------------------------
 *
 *      File            ColorLayer.cpp
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
#include "ColorLayer.h"
#include "Object/Camera/Camera.h"

cColorLayer::cColorLayer ( Color4B tColor, KDint z, Size tSize, Point tPoint )
: cDraw ( z, eObjectType_ColorLayer )
, m_pColorLayer	 ( nullptr		)
, m_bIsVisible	 ( true			)
, m_bIsStatic	 ( false		)
, m_tAnchorPoint ( Point::ZERO	)
, m_tPoint		 ( tPoint		)
, m_tSize		 ( tSize		)
, m_nOpacity	 ( 255			)
, m_tColor		 ( tColor		)
, m_fRotation	 ( 0			)
, m_fScaleX		 ( 1			)
, m_fScaleY		 ( 1			)
{
	m_pColorLayer = LayerColor::create ( tColor,tSize.width, tSize.height );
	m_pColorLayer->retain ( );
	m_pColorLayer->setPosition ( tPoint );
}

cColorLayer::~cColorLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pColorLayer );
}

Point cColorLayer::getScreenPoint ( cCamera* pCamera )
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

KDvoid cColorLayer::setOpacity ( KDint nOpacity )
{
	if ( nOpacity <   0 )	nOpacity = 0;
	if ( nOpacity > 255 )	nOpacity = 255;

	m_nOpacity = nOpacity;
}

KDvoid cColorLayer::setColor ( KDint r, KDint g, KDint b, KDint a )
{
	if ( r <   0 )	r = 0;
	if ( r > 255 )	r = 255;
	if ( g <   0 )	g = 0;
	if ( g > 255 )	g = 255;
	if ( b <   0 )	b = 0;
	if ( b > 255 )	b = 255;
	if ( a <   0 )	a = 0;
	if ( a > 255 )	a = 255;

	m_tColor.r = r;
	m_tColor.g = g;
	m_tColor.b = b;
	m_tColor.a = a;
	m_nOpacity = a;
}

KDvoid cColorLayer::draw ( cCamera* pCamera )
{
	// visible check
	if ( m_bIsVisible == false )
	{
		return;
	}

	// visible
	m_pColorLayer->setVisible ( true );
	
	// rotation
	m_pColorLayer->setRotation ( m_fRotation );

	// opacity
	m_pColorLayer->setOpacity ( m_nOpacity );

	// color
	Color3B		tColor;
	tColor.r = m_tColor.r;
	tColor.g = m_tColor.g;
	tColor.b = m_tColor.b;
	m_pColorLayer->setColor ( tColor );

	// scale
	m_pColorLayer->setScaleX ( m_fScaleX );
	m_pColorLayer->setScaleY ( m_fScaleY );

	// anchor point
	m_pColorLayer->setAnchorPoint ( m_tAnchorPoint );

	// pt
	if ( m_bIsStatic == false )
	{
		m_pColorLayer->setPosition ( m_tPoint - Point ( (KDint) ( pCamera->getX ( ) / getFar ( ) ), (KDint) ( pCamera->getY ( ) / getFar ( ) ) ) );
	}
	else
	{
		m_pColorLayer->setPosition ( m_tPoint );
	}

	// draw
	m_pColorLayer->visit ( );
}
