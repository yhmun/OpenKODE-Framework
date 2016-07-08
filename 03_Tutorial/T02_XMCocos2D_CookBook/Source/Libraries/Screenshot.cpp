/* --------------------------------------------------------------------------
 *
 *      File            Screenshot.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#include "Precompiled.h"
#include "Screenshot.h"

CCImage* Screenshot::takeAsUIImage ( KDvoid )
{
	const CCSize&	tFrameSize = CCEGLView::sharedOpenGLView ( )->getFrameSize ( );
	
	// Create buffer for pixels
	GLuint		uCol	= (GLuint) tFrameSize.cx;
	GLuint		uRow	= (GLuint) tFrameSize.cy;
	GLuint		uLen    = uCol * 4;
	GLubyte*	pLine	= (GLubyte*) kdMalloc ( uLen );
	GLubyte*	pPixels = (GLubyte*) kdMalloc ( uLen * uRow );
	
	// Read Pixels from OpenGL
	glReadPixels ( 0, 0, uCol, uRow, GL_RGBA, GL_UNSIGNED_BYTE, pPixels );

	for ( KDuint y = 0; y < uRow / 2; y++ )
	{
		KDuint	uOff1 = uLen * y;
		KDuint	uOff2 = uLen * ( uRow - y - 1 );

		kdMemcpy ( pLine			 , &pPixels [ uOff1 ], uLen );
		kdMemcpy ( &pPixels [ uOff1 ], &pPixels [ uOff2 ], uLen );
		kdMemcpy ( &pPixels [ uOff2 ], pLine			 , uLen );
	}

	CCImage*		pImage = CCImage::create ( );
	pImage->initWithRawData ( pPixels, uLen * uRow, uCol, uRow, 32 );

	kdFree ( pLine );
	kdFree ( pPixels );

	return pImage;
}

CCTexture2D* Screenshot::takeAsTexture2D ( KDvoid )
{
	return CCTexture2D::createWithImage ( Screenshot::takeAsUIImage ( ) ); 
}

CCData* Screenshot::takeAsPNG ( KDvoid )
{
	return KD_NULL;
}