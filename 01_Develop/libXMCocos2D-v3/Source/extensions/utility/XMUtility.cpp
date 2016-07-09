/* --------------------------------------------------------------------------
 *
 *      File            XMUtility.cpp
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.                      
 *      
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
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

#include "extensions/Utility/XMUtility.h"
#include "2d/CCDirector.h"
#include "platform/CCImage.h"
#include "platform/CCEGLView.h"
#include "2d/misc_nodes/CCRenderTexture.h"

NS_CC_EXT_BEGIN

ActionInterval* XMUtility::createShakeAction ( const Point& tCenter, KDfloat fDuration, Size tRange, KDuint uCount )
{
	Array*		pActions = Array::create ( );
	
	tRange.cx = kdFabs ( tRange.cx );
	tRange.cy = kdFabs ( tRange.cy );

	for ( KDint i = 0; i < uCount; ++i )
	{		
		auto	pMove = MoveTo::create ( fDuration / uCount, Point ( tCenter.x + CCRANDOM_BETWEEN ( -tRange.cx, tRange.cx ), tCenter.y + CCRANDOM_BETWEEN ( -tRange.cy, tRange.cy ) ) );
		pActions->addObject ( pMove );
	}	
	pActions->addObject ( MoveTo::create ( fDuration / uCount, tCenter ) );

	return Sequence::create ( pActions );
}

KDvoid XMUtility::runShakeAction ( Node* pNode, KDfloat fDuration, const Size& tRange, KDuint uCount )
{
	pNode->runAction ( XMUtility::createShakeAction ( pNode->getPosition ( ), fDuration, tRange, uCount ) );
}

KDvoid XMUtility::vibrateDevice ( KDfloat fVibrateTime )
{
	if ( fVibrateTime > 0 )
	{
		xmVibrate ( (KDuint) ( fVibrateTime * 1000 ) );
	}
}

Texture2D* XMUtility::createTexture ( std::vector<std::string> aPaths, std::string sKey, KDbool bVertically )
{
	return nullptr;
}

Image* XMUtility::ScreenShot::takeAsImage ( const Rect& tRect, KDbool bIsGray )
{
	KDfloat		fScaleX   = EGLView::getInstance ( )->getScaleX ( );
	KDfloat		fScaleY   = EGLView::getInstance ( )->getScaleY ( );
	Rect		tViewRect = EGLView::getInstance ( )->getViewPortRect ( );
	Rect		tGLRect;

	tGLRect.origin = Point ( tRect.origin.x * fScaleX + tViewRect.origin.x, tRect.origin.y * fScaleY + tViewRect.origin.y );
	tGLRect.size   = Size  ( tRect.size.cx  * fScaleX, tRect.size.cy * fScaleY );

	// Create buffer for pixels
	KDuint		nBpp	= 24;
	KDubyte		uByte	= ( nBpp >> 3 );
	GLint		nX      = (GLint)  tGLRect.origin.x;
	GLint		nY      = (GLint)  tGLRect.origin.y;
	GLuint		uCol	= (GLuint) tGLRect.size.cx;
	GLuint		uRow	= (GLuint) tGLRect.size.cy;
	GLuint		uLen    = uCol * uByte;
	GLubyte*	pLine	= new GLubyte [ uLen ];
	GLubyte*	pPixels = new GLubyte [ uLen * uRow ];
	
	// Read Pixels from OpenGL
	glPixelStorei ( GL_PACK_ALIGNMENT, 1 );
	glReadPixels ( nX, nY, uCol, uRow, GL_RGB, GL_UNSIGNED_BYTE, pPixels );

	bIsGray = true;
	for ( KDuint y = 0; y < uRow / 2; y++ )
	{
		KDuint		uOff1 = uLen * y;
		KDuint		uOff2 = uLen * ( uRow - y - 1 );
		KDubyte		cTemp;

		if ( bIsGray )
		{
			for ( KDuint x = 0; x < uLen; x += uByte )
			{
				cTemp = ( pPixels [ uOff1 + x + 0 ] + pPixels [ uOff1 + x + 1 ] + pPixels [ uOff1 + x + 2 ] ) / 3;
				pPixels [ uOff1 + x + 0 ] = cTemp;
				pPixels [ uOff1 + x + 1 ] = cTemp;
				pPixels [ uOff1 + x + 2 ] = cTemp;
	
				cTemp = ( pPixels [ uOff2 + x + 0 ] + pPixels [ uOff2 + x + 1 ] + pPixels [ uOff2 + x + 2 ] ) / 3;
				pPixels [ uOff2 + x + 0 ] = cTemp;
				pPixels [ uOff2 + x + 1 ] = cTemp;
				pPixels [ uOff2 + x + 2 ] = cTemp;
			}
		}

		kdMemcpy ( pLine			 , &pPixels [ uOff1 ], uLen );
		kdMemcpy ( &pPixels [ uOff1 ], &pPixels [ uOff2 ], uLen );
		kdMemcpy ( &pPixels [ uOff2 ], pLine			 , uLen );
	}

	Image*		pImage = Image::create ( );
	pImage->initWithRawData ( pPixels, uLen * uRow, uCol, uRow, nBpp, true );

	CC_SAFE_DELETE_ARRAY ( pLine );
	CC_SAFE_DELETE_ARRAY ( pPixels );

	return pImage;
}

Texture2D* XMUtility::ScreenShot::takeAsTexture2D ( const Rect& tRect, KDbool bIsGray )
{
	return Texture2D::createWithImage ( takeAsImage ( tRect, bIsGray ) ); 
}

Sprite* XMUtility::ScreenShot::takeAsSprite	( const Rect& tRect, KDbool bIsGray )
{
	auto	pSprite = Sprite::createWithTexture ( takeAsTexture2D ( tRect, bIsGray ) );

	pSprite->setScale ( 1 / EGLView::getInstance ( )->getScaleX ( ), 1 / EGLView::getInstance ( )->getScaleY ( ) );

	return pSprite;
}

NS_CC_EXT_END