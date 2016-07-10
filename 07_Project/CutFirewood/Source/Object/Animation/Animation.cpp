/* -----------------------------------------------------------------------------------
 *
 *      File            Animation.cpp
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
#include "Animation.h"
#include "Object/Camera/Camera.h"

cAnimation::cAnimation ( KDint z, std::string sName )
: cDraw ( z, eObjectType_Animation )
, m_sName		( sName			)
, m_bIsVisible	( true			)
, m_bIsStatic	( false			)
, m_tPoint		( Point::ZERO	)
, m_nOpacity	( 255			)
, m_fRotation	( 0				)
, m_fScaleX		( 1				)
, m_fScaleY		( 1				)
, m_bIsFlip		( false			)
, m_bIsLoop		( true			)
, m_bPlay		( false			)
, m_nNowFrame	( 0				)
, m_dLeftDelay	( 0				)
{
	// color
	m_tColor.r = 255;
	m_tColor.g = 255;
	m_tColor.b = 255;
}

cAnimation::~cAnimation ( KDvoid )
{
	// images
	for ( auto pImage : m_aImages )
	{
		CC_SAFE_RELEASE ( pImage );
	}
	m_aImages.clear ( );
	
	// frames
	for ( auto pFrame : m_aFrames )
	{
		CC_SAFE_RELEASE ( pFrame );
	}
	m_aFrames.clear ( );
}

KDvoid cAnimation::copy ( cAnimation* pAnimation )
{
	// clear
	for ( auto pImage : pAnimation->m_aImages )
	{
		CC_SAFE_RELEASE ( pImage->pSprite );
		CC_SAFE_RELEASE ( pImage );
	}
	pAnimation->m_aImages.clear ( );

	// copy
	for ( auto pImage : m_aImages )
	{
		cImage*		pNewImage = new cImage ( );

		pNewImage->nUid	   = pImage->nUid;
		pNewImage->pSprite = pImage->pSprite;
		pNewImage->pSprite->retain ( );

		pAnimation->m_aImages.push_back ( pNewImage );
	}

	// frames
	for ( auto pFrame : pAnimation->m_aFrames )
	{
		CC_SAFE_RELEASE ( pFrame );
	}
	pAnimation->m_aFrames.clear ( );

	// copy
	for ( auto pFrame : m_aFrames )
	{
		cFrame*		pNewFrame = new cFrame ( );

		pNewFrame->tCenter = pFrame->tCenter;
		pNewFrame->fDelay  = pFrame->fDelay;

		for ( auto tRectAtt : pFrame->aRectAtts )
		{
			pNewFrame->aRectAtts.push_back ( tRectAtt );
		}

		for ( auto tRectDef : pFrame->aRectDefs )
		{
			pNewFrame->aRectDefs.push_back ( tRectDef );
		}

		for ( auto pSrc : pFrame->aSrcs )
		{
			cFrameSrc*		pNewFrameSrc = new cFrameSrc ( );

			pNewFrameSrc->nUid		= pSrc->nUid;
			pNewFrameSrc->tSrcRect	= pSrc->tSrcRect;
			pNewFrameSrc->tCenter	= pSrc->tCenter;

			pNewFrame->aSrcs.push_back ( pNewFrameSrc );
		}

		pAnimation->m_aFrames.push_back ( pNewFrame );
	}

	// other---------------------------
	pAnimation->m_sName		 = m_sName;
	pAnimation->m_bIsVisible = m_bIsVisible;
	pAnimation->m_bIsStatic	 = m_bIsStatic;
	pAnimation->m_tPoint	 = m_tPoint;
	pAnimation->m_nOpacity	 = m_nOpacity;
	pAnimation->m_tColor	 = m_tColor;
	pAnimation->m_fRotation	 = m_fRotation;
	pAnimation->m_fScaleX	 = m_fScaleX;
	pAnimation->m_fScaleY	 = m_fScaleY;
	pAnimation->m_bIsFlip	 = m_bIsFlip;
	pAnimation->m_bIsLoop	 = m_bIsLoop;
	pAnimation->m_bPlay		 = m_bPlay;
	pAnimation->m_nNowFrame	 = m_nNowFrame;
	pAnimation->m_dLeftDelay = m_dLeftDelay;
}

KDvoid cAnimation::addImage ( cImage* pImage )
{
	pImage->retain ( );
	pImage->pSprite->retain ( );
	m_aImages.push_back ( pImage );
}

KDvoid cAnimation::addFrame ( cFrame* pFrame )
{
	// image uid revision
	for ( auto pSrc : pFrame->aSrcs )
	{
		// init
		KDint		nUid	= pSrc->nUid;
		KDbool		bIsFind = false;

		// find
		KDint		a = 0;
		for ( auto pImage : m_aImages )
		{
			if ( pImage->nUid == nUid )
			{
				bIsFind = true;
				pSrc->nUid = a;
				break;
			}

			a++;
		}

		// check
		CCAssert ( bIsFind, "" );

		pSrc->retain ( );
	}

	// add
	pFrame->retain ( );
	m_aFrames.push_back ( pFrame );
}

KDvoid cAnimation::setOpacity ( KDint nOpacity )
{
	if ( nOpacity <   0 )	nOpacity = 0;
	if ( nOpacity > 255 )	nOpacity = 255;

	m_nOpacity = nOpacity;
}

KDvoid cAnimation::setColor ( KDint r, KDint g, KDint b )
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

KDvoid cAnimation::setFrame ( KDint nFrame )
{
	// check
	if ( nFrame < 0 || nFrame >= (KDint) m_aFrames.size ( ) )
	{
		return;
	}

	// set
	m_nNowFrame = nFrame;
}

cAnimation::cFrame* cAnimation::getFrameData ( KDint nIndex )
{
	// check
	if ( nIndex < 0 || nIndex >= (KDint) m_aFrames.size ( ) )
	{
		return nullptr;
	}

	return m_aFrames [ nIndex ];
}

KDvoid cAnimation::getNowFrameAttRects ( std::vector<Rect>& aRects )
{
	// check
	if ( m_aFrames.size ( ) == 0 )
	{
		CCAssert ( false, "" );
	}

	// loop
	for ( auto tRect : m_aFrames [ m_nNowFrame ]->aRectAtts )
	{
		// revision for flip
		if ( m_bIsFlip == true )
		{
			tRect.origin.x = -( tRect.origin.x + tRect.size.width );
		}

		// revision for position
		tRect.origin.x += m_tPoint.x;
		tRect.origin.y += m_tPoint.y;

		//add
		aRects.push_back ( tRect );
	}
}

KDvoid cAnimation::getNowFrameDefRects ( std::vector<Rect>& aRects )
{
	// check
	if ( m_aFrames.size ( ) == 0 )
	{
		CCAssert ( false, "" );
	}

	// loop
	for ( auto tRect : m_aFrames [ m_nNowFrame ]->aRectDefs )
	{		
		// revision for flip
		if ( m_bIsFlip == true )
		{
			tRect.origin.x = -( tRect.origin.x + tRect.size.width );
		}

		// revision for position
		tRect.origin.x += m_tPoint.x;
		tRect.origin.y += m_tPoint.y;

		// add
		aRects.push_back ( tRect );
	}
}

KDvoid cAnimation::setIsLoop ( KDbool bIsLoop )
{
	m_bIsLoop = bIsLoop;
}

KDvoid cAnimation::start ( KDvoid )
{
	if ( m_aFrames.size ( ) == 0 )
	{
		return;
	}

	m_bPlay		 = true;
	m_nNowFrame	 = 0;
	m_dLeftDelay = m_aFrames [ m_nNowFrame ]->fDelay;

	setFrame ( m_nNowFrame );
}

KDvoid cAnimation::stop ( KDvoid )
{
	m_bPlay	= false;
}

KDvoid cAnimation::playAddTime ( KDdouble dTime )
{
	m_dLeftDelay -= dTime;

	while ( m_dLeftDelay > m_aFrames [ m_nNowFrame ]->fDelay )
	{
		--m_nNowFrame;
		if ( m_nNowFrame < 0 )
		{
			m_nNowFrame = (KDint) m_aFrames.size ( ) - 1;
		}
		
		m_dLeftDelay -= m_aFrames [ m_nNowFrame ]->fDelay;
		
		setFrame ( m_nNowFrame );
	}
	
	while ( m_dLeftDelay < 0 )
	{
		++m_nNowFrame;
		if( m_nNowFrame >= (KDint) m_aFrames.size ( ) )
		{
			m_nNowFrame = 0;
		}
		
		m_dLeftDelay += m_aFrames [ m_nNowFrame ]->fDelay;
		
		setFrame ( m_nNowFrame );
	}
}

KDvoid cAnimation::update ( KDdouble dLeftTime )
{
	if ( m_aFrames.size ( ) == 0 )
	{
		return;
	}

	if ( m_bPlay == true )
	{
		m_dLeftDelay -= dLeftTime;

		while ( m_dLeftDelay < 0 )
		{
			++m_nNowFrame;
			if ( m_nNowFrame < (KDint) m_aFrames.size ( ) )
			{
				m_dLeftDelay += m_aFrames [ m_nNowFrame ]->fDelay;

				setFrame ( m_nNowFrame );
			}
			else
			{
				m_nNowFrame = 0;

				if ( m_bIsLoop == true )
				{
					m_dLeftDelay += m_aFrames [ m_nNowFrame ]->fDelay;
				}
				else
				{
					m_bPlay = false;
					break;
				}

				setFrame ( m_nNowFrame );				
			}
		}
	}
}

KDvoid cAnimation::draw ( cCamera* pCamera )
{
	// visible check
	if ( m_bIsVisible == false )
	{
		return;
	}

	// get now frame
	cFrame*		pFrame = m_aFrames [ m_nNowFrame ];

	// loop
	for ( auto pFrameSrc : pFrame->aSrcs )
	{
		// get sprite
		cSprite*	pSprite = m_aImages [ pFrameSrc->nUid ]->pSprite;

		// visible
		pSprite->setIsVisible ( true );

		// static
		pSprite->setIsStatic ( m_bIsStatic );

		// set texture rect
		pSprite->setTextureRect ( pFrameSrc->tSrcRect );

		// anchor point
		pSprite->setAnchorPoint ( Point::ZERO );

		// get center point
		Point		tCenter = pFrame->tCenter + pFrameSrc->tCenter;

		// center point flip revision
		if ( m_bIsFlip == true )
		{
			tCenter.x = pFrameSrc->tSrcRect.size.width - tCenter.x;
		}

		// point
		pSprite->setPoint ( m_tPoint - tCenter );

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
}
