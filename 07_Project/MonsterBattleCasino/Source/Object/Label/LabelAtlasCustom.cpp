/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlasCustom.cpp
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
#include "LabelAtlasCustom.h"

LabelAtlasCustom* LabelAtlasCustom::create ( const KDchar* pCharmapfile, KDfloat fHeight )
{
	LabelAtlasCustom*	pRet = new LabelAtlasCustom ( );

	if ( pRet && pRet->initWithCharmapfile ( pCharmapfile, fHeight ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool LabelAtlasCustom::initWithCharmapfile ( const KDchar* pCharmapfile, KDfloat fHeight )
{
	if ( !AtlasNode::initWithTileFile ( pCharmapfile, 0, 0, 0 ) )
	{
		return false;
	}

	m_fHeight			= fHeight;
	m_nBeforeStringLen	= 0;
	m_tContentSize		= Size::ZERO;

	return true;
}

KDvoid LabelAtlasCustom::draw ( KDvoid )
{
    CC_NODE_DRAW_SETUP ( );

    GL::blendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    GLfloat		aColors [ 4 ] = { m_tDisplayedColor.r / 255.0f, m_tDisplayedColor.g / 255.0f, m_tDisplayedColor.b / 255.0f, m_cDisplayedOpacity / 255.0f };
    getShaderProgram ( )->setUniformLocationWith4fv ( m_nUniformColor, aColors, 1 );

    m_pTextureAtlas->drawNumberOfQuads ( m_nBeforeStringLen, 0 );
}

KDvoid LabelAtlasCustom::addCharInfo ( KDchar c, KDint nWidth )
{
	sCharInfo		tCharInfo;
	tCharInfo.c		= c;
	tCharInfo.width	= nWidth;

	m_aMatchChars.push_back ( tCharInfo );
}

KDvoid LabelAtlasCustom::setString ( const std::string& sLabel )
{
	m_tString = sLabel;

	// resize
	if ( (KDint) sLabel.size ( ) != m_nBeforeStringLen )
	{
		m_nBeforeStringLen = sLabel.size ( );
		m_pTextureAtlas->resizeCapacity ( m_nBeforeStringLen );		
	}

	m_pTextureAtlas->removeAllQuads ( );

	// init
	m_tContentSize = Size ( 0, m_fHeight );

	// set
	KDfloat		fDestX			= 0;
	KDfloat		fPixelWidth		= (KDfloat) m_pTextureAtlas->getTexture ( )->getPixelsWide ( );
	KDfloat		fPixelHeight	= (KDfloat) m_pTextureAtlas->getTexture ( )->getPixelsHigh ( ); 
	KDfloat		fTexHeight		= m_fHeight / fPixelHeight;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//	m_pTextureAtlas->getTexture ( )->setAliasTexParameters ( );
#endif

	for ( KDuint i = 0; i < sLabel.size ( ); i++ )
	{
		KDfloat		fSrcX = 0;

		for ( KDuint a = 0; a < m_aMatchChars.size ( ); a++ )
		{
			if ( sLabel [ i ] != m_aMatchChars [ a ].c )
			{
				fSrcX += m_aMatchChars [ a ].width;
				continue;
			}

			KDfloat		fCharWidth	= m_aMatchChars [ a ].width;
			KDfloat		fRow		= fSrcX / fPixelWidth;
			KDfloat		fCol		= 0;
			KDfloat		fTexWidth	= fCharWidth / fPixelWidth;

			V3F_C4B_T2F_Quad	tQuad;

			tQuad.tl.texCoords.u = fRow;
			tQuad.tl.texCoords.v = fCol;
			tQuad.tr.texCoords.u = fRow + fTexWidth;
			tQuad.tr.texCoords.v = fCol;
			tQuad.bl.texCoords.u = fRow;
			tQuad.bl.texCoords.v = fCol + fTexHeight;
			tQuad.br.texCoords.u = fRow + fTexWidth;
			tQuad.br.texCoords.v = fCol + fTexHeight;

			tQuad.bl.vertices.x = fDestX;
			tQuad.bl.vertices.y = 0;
			tQuad.bl.vertices.z = 0.0f;
			tQuad.br.vertices.x = (KDint) ( fDestX + fCharWidth );
			tQuad.br.vertices.y = 0;
			tQuad.br.vertices.z = 0.0f;
			tQuad.tl.vertices.x = (KDint) ( fDestX );
		 	tQuad.tl.vertices.y = (KDint) ( m_fHeight );
			tQuad.tl.vertices.z = 0.0f;
			tQuad.tr.vertices.x = (KDint) ( fDestX + fCharWidth );
			tQuad.tr.vertices.y = (KDint) ( m_fHeight );
			tQuad.tr.vertices.z = 0.0f;

			m_pTextureAtlas->updateQuad ( &tQuad, i );			

			fDestX += fCharWidth;
			m_tContentSize.width += fCharWidth;

			break;
		}
	}

	// set size
	this->setContentSize ( m_tContentSize );
}

const std::string& LabelAtlasCustom::getString ( KDvoid ) const
{
	return m_tString;
}

cLabelAtlasCustom::cLabelAtlasCustom ( Node* pNode, const KDchar* pCharmapfile, KDint nHeight, KDint z )
: m_pNode			( pNode			)
, m_pLabel			( nullptr		)
, m_bIsMove			( false			)
, m_tMoveBefore		( Point::ZERO	)
, m_tMove			( Point::ZERO	)
, m_dTimeMoveFull	( 0				)
, m_dTimeMoveLeft	( 0				)
{
	m_pLabel = LabelAtlasCustom::create ( pCharmapfile, nHeight );

	m_pNode->addChild ( m_pLabel, z );
}

cLabelAtlasCustom::~cLabelAtlasCustom ( KDvoid )
{
	m_pNode->removeChild ( m_pLabel, true );
}

KDvoid cLabelAtlasCustom::addCharInfo ( KDchar c, KDint nWidth )
{
	m_pLabel->addCharInfo ( c, nWidth );
}

KDvoid cLabelAtlasCustom::setVisible ( KDbool bIsVisible )
{
	m_pLabel->setVisible ( bIsVisible );
}

KDvoid cLabelAtlasCustom::setString ( const KDchar* pStr )
{
	m_pLabel->setString ( pStr );
}

KDvoid cLabelAtlasCustom::setPosition ( Point tPoint )
{
	m_pLabel->setPosition ( tPoint );
}

KDvoid cLabelAtlasCustom::setPointTime ( Point tPoint, KDdouble dLeftTime )
{
	m_bIsMove		= true;
	m_tMove			= tPoint - m_pLabel->getPosition ( ); 
	m_tMoveBefore	= m_pLabel->getPosition ( );
	m_dTimeMoveFull	= dLeftTime;
	m_dTimeMoveLeft	= dLeftTime;
}

KDvoid cLabelAtlasCustom::setAlpha ( KDubyte alpha )
{
	m_pLabel->setOpacity ( alpha );
}

KDvoid cLabelAtlasCustom::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	Color3B		tColor;
	
	tColor.r = r;
	tColor.g = g;
	tColor.b = b;

	m_pLabel->setColor ( tColor );
}

KDvoid cLabelAtlasCustom::setColor ( KDubyte alpha, KDubyte r, KDubyte g, KDubyte b )
{
	m_pLabel->setOpacity ( alpha );

	Color3B		tColor;
	
	tColor.r = r;
	tColor.g = g;
	tColor.b = b;

	m_pLabel->setColor ( tColor );
}

Size cLabelAtlasCustom::getSize ( KDvoid )
{
	return m_pLabel->getContentSize ( );
}

Point cLabelAtlasCustom::getPoint ( KDvoid )
{
	return m_pLabel->getPosition ( );
}

Color3B cLabelAtlasCustom::getColor ( KDvoid )
{
	return m_pLabel->getColor ( );
}

KDvoid cLabelAtlasCustom::update ( KDdouble dLeftTime )
{
	if ( m_bIsMove == true )
	{
		m_dTimeMoveLeft -= dLeftTime;

		if ( m_dTimeMoveLeft < 0 )
		{
			Point		tPoint = Point ( m_tMoveBefore.x + m_tMove.x, m_tMoveBefore.y + m_tMove.y );
			m_pLabel->setPosition ( tPoint );
			m_bIsMove = false;
		}
		else
		{
			Point		tPoint = Point ( m_tMoveBefore.x + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.x / m_dTimeMoveFull,
									     m_tMoveBefore.y + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.y / m_dTimeMoveFull );

			m_pLabel->setPosition ( tPoint );
		}
	}
}