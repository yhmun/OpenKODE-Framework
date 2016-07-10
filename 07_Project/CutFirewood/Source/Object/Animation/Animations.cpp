/* -----------------------------------------------------------------------------------
 *
 *      File            Animations.cpp
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
#include "Animations.h"
#include "Animation.h"
#include "Object/Camera/Camera.h"
#include "Object/Sprite/Sprite.h"

#define _ANI_NAME_MAX		18

cAnimations::cAnimations ( std::string sFileName, KDint z, Point tPoint )
: cDraw ( z, eObjectType_Animations )
, m_sFileName				( sFileName	)
, m_nSelectAnimationIndex	( 0			)
, m_bIsVisible				( true		)
, m_bIsStatic				( false		)
, m_tPoint					( tPoint	)
, m_nOpacity				( 255		)
, m_fRotation				( 0			)
, m_fScaleX					( 1			)
, m_fScaleY					( 1			)
, m_bIsFlip					( false		)
, m_bIsLoop					( true		)
, m_bIsPlay					( false		)
{
	// color
	m_tColor.r = 255;
	m_tColor.g = 255;
	m_tColor.b = 255;

	KDint32		nFileSize = 0;
	KDubyte*	pData = FileUtils::getInstance ( )->getFileData ( sFileName.c_str ( ), "rb", &nFileSize );

	// seek
	KDint		nSeek = 0;

	// init images
	std::vector<cAnimation::cImage*>	aImages;

	// image count
	KDint		nImageCount = 0;
	readToData ( pData, nFileSize, nSeek, &nImageCount, sizeof ( KDint ) );

	// image loop
	for ( KDint i = 0; i < nImageCount; i++ )
	{
		// init
		cAnimation::cImage*		pImage = new cAnimation::cImage ( );

		// uid
		readToData ( pData, nFileSize, nSeek, &pImage->nUid, sizeof ( KDint ) );

		// image file name
		KDint		nImageFileNameSize	= 0;
		KDchar		szImageFileName [ 260 ] = { 0, };
		readToData ( pData, nFileSize, nSeek, &nImageFileNameSize, sizeof ( KDint ) );
		readToData ( pData, nFileSize, nSeek, szImageFileName, sizeof ( KDchar ) * nImageFileNameSize );

		// sprite
		pImage->pSprite = new cSprite ( szImageFileName, z, Point::ZERO );		

		// add
		aImages.push_back ( pImage );
	}

	// animation count
	KDint		nAniCount = 0;
	readToData( pData, nFileSize, nSeek, &nAniCount, sizeof ( KDint ) );
								
	// animation loop
	for ( KDint i = 0; i < nAniCount; i++ )
	{
		// ani name
		KDint		nAniNameCount = 0;
		KDchar		szAniName [ _ANI_NAME_MAX + 1 ] = { 0, };
		readToData ( pData, nFileSize, nSeek, &nAniNameCount, sizeof ( KDint ) );
		readToData ( pData, nFileSize, nSeek, szAniName, sizeof ( KDchar ) * nAniNameCount );

		// init animation
		cAnimation*		pAnimation = new cAnimation ( z, szAniName );

		// images add
		for ( auto pImage : aImages )
		{
			pAnimation->addImage ( pImage );
		}

		// frame count
		KDint		nFrameCount = 0;
		readToData ( pData, nFileSize, nSeek, &nFrameCount, sizeof ( KDint ) );
								
		// frame read loop
		for ( KDint a = 0; a < nFrameCount; a++ )
		{
			// init frame
			cAnimation::cFrame*		pFrame = new cAnimation::cFrame ( );

			// delay
			readToData ( pData, nFileSize, nSeek, &pFrame->fDelay, sizeof ( KDfloat ) );
			pFrame->fDelay /= 1000;
					
			// point
			readToData ( pData, nFileSize, nSeek, &pFrame->tCenter.x, sizeof ( KDfloat ) );
			readToData ( pData, nFileSize, nSeek, &pFrame->tCenter.y, sizeof ( KDfloat ) );
	
			// att rect count
			KDint		nAttRectCount = 0;
			readToData ( pData, nFileSize, nSeek, &nAttRectCount, sizeof ( KDint ) );
					
			// rc att read loop
			for ( KDint z = 0; z < nAttRectCount; z++ )
			{
				Rect	tRect = Rect::ZERO;
				readToData ( pData, nFileSize, nSeek, &tRect.origin.x	, sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &tRect.origin.y	, sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &tRect.size.width , sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &tRect.size.height, sizeof ( KDfloat ) );

				pFrame->aRectAtts.push_back ( tRect );
			}

			// att rect count
			KDint		nDefRectCount = 0;
			readToData( pData, nFileSize, nSeek, &nDefRectCount, sizeof ( KDint ) );
						
			// rc def read loop
			for ( KDint z = 0; z < nDefRectCount; z++ )
			{
				Rect	tRect = Rect::ZERO;
				readToData ( pData, nFileSize, nSeek, &tRect.origin.x	, sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &tRect.origin.y	, sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &tRect.size.width	, sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &tRect.size.height, sizeof ( KDfloat ) );

				pFrame->aRectDefs.push_back ( tRect );
			}
	
			// rc Src count
			KDint		nSrcCount = 0;
			readToData ( pData, nFileSize, nSeek, &nSrcCount, sizeof ( KDint ) );
					
			// rc src loop
			for ( KDint z = 0; z < nSrcCount; z++ )
			{
				// init
				cAnimation::cFrameSrc*		pFrameSrc = new cAnimation::cFrameSrc ( );

				// uid
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->nUid, sizeof ( KDint ) );

				// rc Src
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->tSrcRect.origin.x   , sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->tSrcRect.origin.y   , sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->tSrcRect.size.width , sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->tSrcRect.size.height, sizeof ( KDfloat ) );

				// pt center
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->tCenter.x, sizeof ( KDfloat ) );
				readToData ( pData, nFileSize, nSeek, &pFrameSrc->tCenter.y, sizeof ( KDfloat ) );

				// add
				pFrame->aSrcs.push_back ( pFrameSrc );
			}

			// add
			pAnimation->addFrame ( pFrame );		
		}

		pAnimation->retain ( );
		m_aAnimations.push_back ( pAnimation );
	}

	// data delete
	CC_SAFE_FREE ( pData );
}

cAnimations::~cAnimations ( KDvoid )
{
	for ( auto pAnimation : m_aAnimations )
	{
		CC_SAFE_RELEASE ( pAnimation );
	}
	m_aAnimations.clear ( );
}

KDvoid cAnimations::setOpacity ( KDint nOpacity )
{
	if ( nOpacity <   0 )	nOpacity = 0;
	if ( nOpacity > 255 )	nOpacity = 255;

	m_nOpacity = nOpacity;
}

KDvoid cAnimations::setColor ( KDint r, KDint g, KDint b )
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

KDvoid cAnimations::setIsLoop ( KDbool bIsLoop )
{
	m_bIsLoop = bIsLoop;
}

KDvoid cAnimations::start ( KDvoid )
{
	m_bIsPlay = true;
	m_aAnimations [ m_nSelectAnimationIndex ]->start ( );
}

KDvoid cAnimations::stop ( KDvoid )
{
	m_bIsPlay = false;
	m_aAnimations [ m_nSelectAnimationIndex ]->stop ( );
}

KDvoid cAnimations::playAddTime ( KDdouble dTime )
{
	m_aAnimations [ m_nSelectAnimationIndex ]->playAddTime ( dTime );
}

KDvoid cAnimations::selectAnimation ( KDint nIndex )
{
	// check
	if ( nIndex < 0 || nIndex >= (KDint) m_aAnimations.size ( ) )
	{
		CCAssert ( false, "" );
	}

	// play
	if ( m_bIsPlay == true )
	{
		m_aAnimations [ m_nSelectAnimationIndex ]->start ( );
	}
	else
	{
		m_aAnimations [ m_nSelectAnimationIndex ]->stop ( );
	}

	// set
	m_nSelectAnimationIndex = nIndex;
}

cAnimation* cAnimations::getAnimation ( KDint nIndex )
{
	// check
	if (	nIndex < 0 || nIndex >= (KDint) m_aAnimations.size ( ) )
	{
		CCAssert ( false, "" );
	}

	// set
	return m_aAnimations [ nIndex ];
}

KDvoid cAnimations::setNowAnimationFrame ( KDint nFrame )
{
	m_aAnimations [ m_nSelectAnimationIndex ]->setFrame ( nFrame );
}

KDint cAnimations::getNowAnimationFrame ( KDvoid )
{
	return m_aAnimations [ m_nSelectAnimationIndex ]->getNowFrame ( );
}

KDvoid cAnimations::update ( KDdouble dLeftTime )
{
	// loop
	m_aAnimations [ m_nSelectAnimationIndex ]->setIsLoop ( m_bIsLoop );

	// update
	m_aAnimations [ m_nSelectAnimationIndex ]->update ( dLeftTime );

	// play
	m_bIsPlay = m_aAnimations [ m_nSelectAnimationIndex ]->getIsPlay ( );
}

KDvoid cAnimations::draw ( cCamera* pCamera )
{
	// visible check
	if ( m_bIsVisible == false )
	{
		return;
	}

	// init
	cAnimation*		pAnimation = m_aAnimations [ m_nSelectAnimationIndex ];

	// visible
	pAnimation->setIsVisible ( true );

	// static
	pAnimation->setIsStatic ( m_bIsStatic );

	// point
	pAnimation->setPoint ( m_tPoint );

	// rotation
	pAnimation->setRotation ( m_fRotation );

	// opacity
	pAnimation->setOpacity ( m_nOpacity );

	// color
	pAnimation->setColor ( m_tColor.r, m_tColor.g, m_tColor.b );

	// scale
	pAnimation->setScaleX ( m_fScaleX );
	pAnimation->setScaleY ( m_fScaleY );

	// flip
	pAnimation->setIsFlip ( m_bIsFlip );

	// draw
	pAnimation->draw ( pCamera );
}
