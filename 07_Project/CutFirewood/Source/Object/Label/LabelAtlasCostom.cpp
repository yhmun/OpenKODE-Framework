/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlasCostom.cpp
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
#include "LabelAtlasCostom.h"
#include "Object/Camera/Camera.h"
#include "Object/Sprite/Sprite.h"

cLabelAtlasCostom::cLabelAtlasCostom ( const KDchar* pTextureFileName, KDint nHeight, KDint z, Point tPoint )
: cDraw ( z, eObjectType_LabelAtlasCostom )
, m_sTextureName	( pTextureFileName		)
, m_nZorder			( z						)
, m_bIsVisible		( true					)
, m_bIsStatic		( false					)
, m_tAnchorPoint	( Point::ZERO			)
, m_tPoint			( tPoint				)
, m_tSize			( Size ( 0, nHeight )	)
, m_nOpacity		( 255					)
, m_fRotation		( 0						)
, m_fScaleX			( 1						)
, m_fScaleY			( 1						)
, m_sString			( ""					)
{
	setColor ( 255, 255, 255 );
}

cLabelAtlasCostom::~cLabelAtlasCostom ( KDvoid )
{
	// character infos
	for ( auto pChar : m_aCharInfos )
	{
		CC_SAFE_RELEASE ( pChar.pSprite );
	}
	m_aCharInfos.clear ( );
	
	// string
	m_sTextureName.clear ( );
	m_sString.clear ( );
}

KDvoid cLabelAtlasCostom::addCharInfo ( KDchar c, KDint nSrcX, KDint nWidthSrc, KDfloat fWidthDest )
{
	// init
	sCharInfo		tCharInfo;
	
	// set
	tCharInfo.c			 = c;
	tCharInfo.nSrcX		 = nSrcX;
	tCharInfo.nWidthSrc	 = nWidthSrc;
	tCharInfo.fWidthDest = fWidthDest;
	tCharInfo.pSprite	 = new cSprite ( m_sTextureName.c_str ( ), m_nZorder, m_tPoint );
	tCharInfo.pSprite->retain ( );
	
	// add
	m_aCharInfos.push_back ( tCharInfo );
}

KDvoid cLabelAtlasCostom::setString ( const KDchar* pStr )
{
	// set
	m_sString = pStr;
	
	// size set
	m_tSize.width = 0;
	
	// loop
	for ( KDuint i = 0; i < m_sString.size ( ); i++ )
	{
		// init
		sCharInfo	tCharInfo;
		KDbool		bIsSuccess = false;
		
		//find
		findCharInfo ( pStr [ i ], tCharInfo, bIsSuccess );
		
		// check
		if ( bIsSuccess == false )
		{
			CCLOG ( "cLabelAtlasCostom::setString ()? %c", pStr [ i ] );
			
			continue;
		}
		
		// width add
		m_tSize.width += tCharInfo.fWidthDest;
	}
}

Point cLabelAtlasCostom::getScreenPoint ( cCamera* pCamera )
{
	if ( m_bIsStatic == false )	return m_tPoint - Point ( (KDint) ( pCamera->getX ( ) / getFar ( ) ), (KDint) ( pCamera->getY ( ) / getFar ( ) ) );
	else						return m_tPoint;
}

KDvoid cLabelAtlasCostom::setOpacity ( KDint nOpacity )
{
	if ( nOpacity <   0 )	nOpacity = 0;
	if ( nOpacity > 255 )	nOpacity = 255;
	
	m_nOpacity = nOpacity;
}

KDvoid cLabelAtlasCostom::setColor ( KDint r, KDint g, KDint b )
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

KDvoid cLabelAtlasCostom::findCharInfo ( KDchar c, sCharInfo& tCharInfo, KDbool& bIsSuccess )
{
	// init
	bIsSuccess = false;
	
	// find
	for ( KDuint i = 0; i < m_aCharInfos.size ( ); i++ )
	{
		if ( m_aCharInfos [ i ].c == c )
		{
			tCharInfo  = m_aCharInfos [ i ];
			bIsSuccess = true;
			return;
		}
	}
}

KDvoid cLabelAtlasCostom::draw ( cCamera* pCamera )
{
	// visible check
	if ( m_bIsVisible == false || m_sString.empty ( ) )
	{
		return;
	}

	// init
	const KDchar*	pStr2 = m_sString.c_str ( );
	KDfloat			fDestX = 0;

	// anchor point
	Point			tAnchorPoint = Point ( m_tSize.width * m_tAnchorPoint.x, m_tSize.height * m_tAnchorPoint.y );	

	// loop
	for ( KDuint i = 0; i < m_sString.size ( ); i++ )
	{
		// init
		sCharInfo	tCharInfo;
		KDbool		bIsSuccess = false;
		
		//find
		findCharInfo ( pStr2 [ i ], tCharInfo, bIsSuccess );
		
		// check
		if ( bIsSuccess == false )
		{
			CCLOG ( "cLabelAtlasCostom::setString()? %c", pStr2 [ i ] );
			
			continue;
		}

		// visible
		tCharInfo.pSprite->setIsVisible ( true );
		
		// texture rect
 		tCharInfo.pSprite->setTextureRect ( Rect ( tCharInfo.nSrcX, 0, tCharInfo.nWidthSrc, m_tSize.height ) );
		
		// rotation
		tCharInfo.pSprite->setRotation ( m_fRotation );
		
		// opacity
		tCharInfo.pSprite->setOpacity ( m_nOpacity );
		
		// color
		tCharInfo.pSprite->setColor ( m_tColor.r, m_tColor.g, m_tColor.b );
		
		// scale
		tCharInfo.pSprite->setScaleX ( m_fScaleX );
		tCharInfo.pSprite->setScaleY ( m_fScaleY );

		// anchor point
		tCharInfo.pSprite->setAnchorPoint ( Point ( ( tCharInfo.nWidthSrc - ( ( fDestX+tCharInfo.nWidthSrc ) - tAnchorPoint.x ) ) / tCharInfo.nWidthSrc, m_tAnchorPoint.y ) );

		// pt
		tCharInfo.pSprite->setPoint ( m_tPoint );
		
		// draw
		tCharInfo.pSprite->draw ( pCamera );

		// dest x
		fDestX += tCharInfo.fWidthDest;
	}
}

