/* -----------------------------------------------------------------------------------
 *
 *      File            SprAni.cpp
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
#include "SprAni.h"

cSprAni::cSprAni ( Node* pNode, const KDchar* pPath, KDint z, Point tPoint, KDbool bIsTexLoad )
: m_pNode			( pNode					)
, m_pSprite			( new cSprite ( pNode ) )
, m_bIsTexLoad		( bIsTexLoad			)
, m_sTexPath		( ""					)
, m_bIsVisible		( true					)
, m_nZorder			( z						)
, m_cRed			( 255					)
, m_cGreen			( 255					)
, m_cBlue			( 255					)
, m_cAlpha			( 255					)
, m_tPoint			( tPoint				)
, m_eDirectionType	( eDirectionType_Left	)
, m_bLoop			( false					)
, m_bPlay			( false					)
, m_nNowFrame		( 0						)
, m_fLeftDelay		( 0						)
, m_bIsMove			( false					)
, m_tMoveBefore		( Point::ZERO			)
, m_tMove			( Point::ZERO			)
, m_dTimeMoveFull	( 0						)
, m_dTimeMoveLeft	( 0						)
, m_nId				( 0						)
{
	if ( m_bIsTexLoad == true )
	{
		m_pSprite->initTexture ( pPath, z, Point ( 0, 0 ) );
	}
	else
	{
		m_sTexPath = pPath;
	}
}

cSprAni::~cSprAni ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprite );
}

KDvoid cSprAni::changeNode ( Node* pNode, KDint z )
{
	m_pNode	= pNode;
	m_nZorder = z;

	m_pSprite->changeNode ( m_pNode, z );
}

KDvoid cSprAni::texLoad ( KDvoid )
{
	texLoad ( m_nZorder );
}

KDvoid cSprAni::texLoad ( KDint z )
{
	if ( m_bIsTexLoad == true )
	{
		return;
	}

	m_nZorder = z;
	m_bIsTexLoad = true;
	
	m_pSprite->initTexture ( m_sTexPath.c_str ( ), z, Point::ZERO );

	for ( auto& rFrame : m_aFrames )
	{
		m_pSprite->addFrame ( rFrame.tRectSrc );
	}

	Color3B		tColor;

	tColor.r = m_cRed;
	tColor.g = m_cGreen;
	tColor.b = m_cBlue;

	m_pSprite->get ( )->setVisible ( m_bIsVisible );
	m_pSprite->get ( )->setOpacity ( m_cAlpha );
	m_pSprite->get ( )->setColor ( tColor );

	if ( m_eDirectionType == eDirectionType_Left )	
	{
		m_pSprite->get ( )->setFlippedX ( false );
	}
	else
	{
		m_pSprite->get ( )->setFlippedX ( true );
	}

	setPoint ( m_tPoint );
}

KDvoid cSprAni::texRelease ( KDvoid )
{
	if ( m_bIsTexLoad == false )
	{
		return;
	}

	m_bIsTexLoad = false;

	m_pSprite->releaseTexture ( );
}

KDvoid cSprAni::addFrame ( Rect tRectSrc, float fDelay, bool bAtt, bool bDef, Point tCenter, Rect tRectAtt, Rect tRectDef )
{
	cFrame		aFrame;

	aFrame.tRectSrc		= tRectSrc;
	aFrame.fDelay		= fDelay /1000.0f;
	aFrame.bAtt			= bAtt;
	aFrame.bDef			= bDef;
	aFrame.tCenter		= tCenter;
	aFrame.tCenter.y	= aFrame.tRectSrc.size.height - aFrame.tCenter.y;
	aFrame.tRectAtt		= tRectAtt;
	aFrame.tRectDef		= tRectDef;
	aFrame.eDirection	= eDirectionType_Left;

	if ( m_bIsTexLoad == true )
	{
		m_pSprite->addFrame ( tRectSrc );
	}

	m_aFrames.push_back ( aFrame );
}

KDvoid cSprAni::setVisible ( KDbool bVisible )
{
	m_bIsVisible = bVisible;

	if ( m_bIsTexLoad == true )
	{
		m_pSprite->get ( )->setVisible ( bVisible );
	}
}

KDvoid cSprAni::setAlpha ( KDubyte cAlpha )
{
	m_cAlpha = cAlpha;

	if ( m_bIsTexLoad == true )
	{
		m_pSprite->get ( )->setOpacity ( cAlpha );
	}
}

KDubyte cSprAni::getAlpha ( KDvoid )
{
	return m_cAlpha;
}

KDvoid cSprAni::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	Color3B		tColor;

	tColor.r = m_cRed	= r;
	tColor.g = m_cGreen = g;
	tColor.b = m_cBlue  = b;

	if ( m_bIsTexLoad == true )
	{
		m_pSprite->get ( )->setColor ( tColor );
	}
}

KDvoid cSprAni::setPoint ( const Point& tDest )
{
	m_tPoint = tDest;

	if ( m_bIsTexLoad == false || m_aFrames.size ( ) == 0 )
	{
		return;
	}

	m_tPoint = tDest;

	Point	tCenter = m_aFrames [ m_nNowFrame ].tCenter;
	if ( m_eDirectionType == eDirectionType_Right )
	{
		tCenter.x = m_aFrames [ m_nNowFrame ].tRectSrc.size.width - tCenter.x;
	}

	m_pSprite->get ( )->setPosition ( m_tPoint - tCenter );
}

KDvoid cSprAni::anchorPoint ( const Point& tAnchor )
{
	m_pSprite->get ( )->setAnchorPoint ( tAnchor );
}

KDvoid cSprAni::setPointTime ( Point tPoint, KDdouble dLeftTime, KDbool bIsForciblyChange )
{
	if ( bIsForciblyChange == false && m_bIsMove == true )
	{
		return;
	}

	m_bIsMove		= true;
	m_tMove			= tPoint - m_tPoint;
	m_tMoveBefore	= m_tPoint;
	m_dTimeMoveFull	= dLeftTime;
	m_dTimeMoveLeft	= dLeftTime;
}

KDvoid cSprAni::setLoop ( KDbool bLoop )
{
	m_bLoop = bLoop;
}

KDvoid cSprAni::setDirection ( eDirectionType eType )
{
	m_eDirectionType = eType;

	if ( m_bIsTexLoad == true )
	{
		if ( m_eDirectionType == eDirectionType_Left )
		{
			m_pSprite->get ( )->setFlippedX ( false );
		}
		else
		{
			m_pSprite->get ( )->setFlippedX ( true );
		}
	}

	setFrame ( m_nNowFrame );
}

KDvoid cSprAni::setFrameInit ( KDint nFrame )
{
	if ( nFrame < 0 || nFrame >= (KDint) m_aFrames.size ( ) )
	{
		return;
	}

	m_fLeftDelay = m_aFrames [ nFrame ].fDelay;

	setFrame ( nFrame );
}

KDvoid cSprAni::setScale ( KDfloat fScaleX, KDfloat fScaleY )
{
	m_pSprite->get ( )->setScaleX ( fScaleX );
	m_pSprite->get ( )->setScaleY ( fScaleY );
}

KDvoid cSprAni::setFrame ( KDint nFrame )
{
	if ( nFrame < 0 || nFrame >= (KDint) m_aFrames.size ( ) )
	{
		return;
	}

	m_nNowFrame = nFrame;

	if ( m_bIsTexLoad == true )
	{
		m_pSprite->setFrame ( m_nNowFrame );

		Point	tCenter = m_aFrames [ m_nNowFrame ].tCenter;
		if ( m_eDirectionType == eDirectionType_Right )
		{
			tCenter.x = m_aFrames [ m_nNowFrame ].tRectSrc.size.width - tCenter.x;
		}

		m_pSprite->get ( )->setPosition ( m_tPoint - tCenter );
	}
}

KDvoid cSprAni::start ( KDvoid )
{
	if ( m_aFrames.size ( ) == 0 )
	{
		return;
	}

	m_bPlay	= true;
	m_nNowFrame	= 0;
	m_fLeftDelay = m_aFrames.front ( ).fDelay;

	setFrame ( m_nNowFrame );
}

KDvoid cSprAni::stop ( KDvoid )
{
	m_bPlay	= false;
}

KDbool cSprAni::update ( KDdouble dLeftTickCount, KDbool bIsMove )
{
	if ( m_aFrames.size ( ) == 0 )
	{
		return false;
	}

	if ( bIsMove == true )
	{
		updateMove ( dLeftTickCount );
	}

	if ( m_bPlay == true )
	{
		m_fLeftDelay -= dLeftTickCount;

		while ( m_fLeftDelay < 0 )
		{
			++m_nNowFrame;
			if ( m_nNowFrame < (KDint) m_aFrames.size ( ) )
			{
				m_fLeftDelay += m_aFrames [ m_nNowFrame ].fDelay;

				setFrame ( m_nNowFrame );
			}
			else
			{
				m_nNowFrame = 0;

				if ( m_bLoop == true )
				{
					m_fLeftDelay += m_aFrames [ m_nNowFrame ].fDelay;
				}
				else
				{
					m_bPlay = false;
				}

				setFrame ( m_nNowFrame );

				return true;
			}
		}
	}		

	return false;
}

KDvoid cSprAni::updateMove ( KDdouble dLeftTickCount )
{
	if ( m_bIsMove == false )
	{
		return;
	}

	m_dTimeMoveLeft -= dLeftTickCount;
	
	if ( m_dTimeMoveLeft < 0 )
	{
		setPoint ( m_tMoveBefore + m_tMove ); 
		m_bIsMove = false;
	}
	else
	{
		setPoint ( Point ( m_tMoveBefore.x + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.x / m_dTimeMoveFull,
					  	   m_tMoveBefore.y + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.y / m_dTimeMoveFull ) );
	}
}

cSprite* cSprAni::getSprite ( KDvoid )
{
	return m_pSprite;
}

KDvoid cSprAni::getFrameData ( KDint nIndex, KDbool& bIsSucces, cFrame& tFrame )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aFrames.size ( ) )
	{
		bIsSucces = false;

		return;
	}

	bIsSucces = true;

	tFrame = m_aFrames [ nIndex ];
}

Rect cSprAni::getNowFrameAttRect ( KDvoid )
{
	if ( m_aFrames.size ( ) == 0 )
	{
		return Rect::ZERO;
	}

	cFrame		tFrame = m_aFrames [ m_nNowFrame ];
	Rect		tRectAtt = Rect ( tFrame.tRectAtt.origin.x, _SCR_H - m_tPoint.y - tFrame.tRectSrc.size.height + tFrame.tCenter.y + tFrame.tRectAtt.origin.y, tFrame.tRectAtt.size.width, tFrame.tRectAtt.size.height );

	if ( m_eDirectionType == eDirectionType_Left )
	{
		tRectAtt.origin.x += m_tPoint.x - tFrame.tCenter.x;
	}
	else
	{
		tRectAtt.origin.x = m_tPoint.x - ( tFrame.tRectSrc.size.width - tFrame.tCenter.x ) + ( tFrame.tRectSrc.size.width - tRectAtt.origin.x - tRectAtt.size.width );
	}

	return tRectAtt;
}

Rect cSprAni::getNowFrameDefRect ( KDvoid )
{
	if ( m_aFrames.size ( ) == 0 )
	{
		return Rect::ZERO;
	}

	cFrame		tFrame = m_aFrames [ m_nNowFrame ];
	Rect		tRectDef = Rect ( tFrame.tRectDef.origin.x, _SCR_H - m_tPoint.y - tFrame.tRectSrc.size.height + tFrame.tCenter.y + tFrame.tRectDef.origin.y, tFrame.tRectDef.size.width, tFrame.tRectDef.size.height );

	if ( m_eDirectionType == eDirectionType_Left )
	{
		tRectDef.origin.x += m_tPoint.x - tFrame.tCenter.x;
	}
	else
	{
		tRectDef.origin.x = m_tPoint.x - ( tFrame.tRectSrc.size.width - tFrame.tCenter.x ) + ( tFrame.tRectSrc.size.width - tRectDef.origin.x - tRectDef.size.width );
	}

	return tRectDef;
}
