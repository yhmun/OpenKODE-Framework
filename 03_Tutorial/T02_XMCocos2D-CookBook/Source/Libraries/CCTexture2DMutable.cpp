/* --------------------------------------------------------------------------
 *
 *      File            CCTexture2DMutable.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Lam Hoang Pham
 *      Improved by     Manuel Martinez-Almeida
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include "CCTexture2DMutable.h"

CCTexture2DMutable*	CCTexture2DMutable::createWithImage ( CCImage* pImage )
{
	CCTexture2DMutable*		pRet = new CCTexture2DMutable ( );

	if ( pRet && pRet->initWithImage ( pImage ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCTexture2DMutable*	CCTexture2DMutable::createWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize )
{
	CCTexture2DMutable*		pRet = new CCTexture2DMutable ( );

	if ( pRet && pRet->initWithString ( szText, szFontPath, fFontSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCTexture2DMutable::initWithData ( const KDvoid* pData, CCTexture2DPixelFormat ePixelFormat, const CCSize& tContentSize, KDuint uMipmaps )
{
	if ( !CCTexture2D::initWithData ( pData, ePixelFormat, tContentSize, uMipmaps ) )
	{
		return KD_FALSE;
	}

	m_pData = (KDvoid*) pData;

	return KD_TRUE;
}

ccColor4B CCTexture2DMutable::pixelAt ( const CCPoint& tPos )
{
	ccColor4B	tColor = { 0, 0, 0, 0 };
	if ( !m_pData )
	{
		return tColor;
	}

	if ( tPos.x < 0 || tPos.y < 0 )
	{
		return tColor;
	}

	if ( tPos.x >= m_tContentSize.cx || tPos.y >= m_tContentSize.cy ) 
	{
		return tColor;
	}
	
	KDuint		x = (KDuint) tPos.x;
	KDuint		y = (KDuint) tPos.y;
	KDuint		w = (KDuint) m_tContentSize.cx;

	if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGBA8888 )
	{
		KDuint*		pPixel = (KDuint*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		tColor.r = *pPixel & 0xff;
		tColor.g = ( *pPixel >>  8 ) & 0xff;
		tColor.b = ( *pPixel >> 16 ) & 0xff;
		tColor.a = ( *pPixel >> 24 ) & 0xff;
	} 
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGBA4444 )
	{
		GLushort*	pPixel = (GLushort*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		tColor.a = ( ( *pPixel & 0xf ) << 4 ) | ( *pPixel & 0xf );
		tColor.b = ( ( ( *pPixel >>  4 ) & 0xf ) << 4 ) | ( ( *pPixel >>  4 ) & 0xf );
		tColor.g = ( ( ( *pPixel >>  8 ) & 0xf ) << 4 ) | ( ( *pPixel >>  8 ) & 0xf );
		tColor.r = ( ( ( *pPixel >> 12 ) & 0xf ) << 4 ) | ( ( *pPixel >> 12 ) & 0xf );
	} 
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGB5A1 )
	{
		GLushort*	pPixel = (GLushort*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		tColor.r = ( ( *pPixel >> 11 ) & 0x1f ) << 3;
		tColor.g = ( ( *pPixel >>  6 ) & 0x1f ) << 3;
		tColor.b = ( ( *pPixel >>  1 ) & 0x1f ) << 3;
		tColor.a = ( *pPixel & 0x1 ) * 255;
	} 
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGB565 )
	{
		GLushort*	pPixel = (GLushort*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		tColor.b = ( *pPixel & 0x1f ) << 3;
		tColor.g = ( ( *pPixel >>  5 ) & 0x3f ) << 2;
		tColor.r = ( ( *pPixel >> 11 ) & 0x1f ) << 3;
		tColor.a = 255;
	} 
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_A8 )
	{
		GLubyte*	pPixel = (GLubyte*) m_pData;
		tColor.a = pPixel [ ( y * w ) + x ];
		// Default white
		tColor.r = 255;
		tColor.g = 255;
		tColor.b = 255;
	}

	return tColor;
}

KDbool CCTexture2DMutable::setPixelAt (const CCPoint& tPos, const ccColor4B& c )
{
	if ( !m_pData )
	{
		return KD_FALSE;
	}

	if ( tPos.x < 0 || tPos.y < 0 ) 
	{
		return KD_FALSE;
	}

	if ( tPos.x >= m_tContentSize.cx || tPos.y >= m_tContentSize.cy )
	{
		return KD_FALSE;
	}

	KDuint		x = (KDuint) tPos.x;
	KDuint		y = (KDuint) tPos.y;
	KDuint		w = (KDuint) m_tContentSize.cx;
	
	m_bDirty = KD_TRUE;
	
	//	Shifted bit placement based on little-endian, let's make this more
	//	portable =/
	
	if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGBA8888 )
	{
		KDuint*		pPixel = (KDuint*) m_pData;
		pPixel [ ( y * w ) + x ] = ( c.a << 24 ) | ( c.b << 16 ) | ( c.g << 8 ) | c.r;
	} 
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGBA4444 )
	{
		GLushort*	pPixel = (GLushort*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		*pPixel = ( ( c.r >> 4 ) << 12 ) | ( ( c.g >> 4 ) << 8 ) | ( ( c.b >> 4 ) << 4 ) | ( c.a >> 4 );
	} 
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGB5A1 )
	{
		GLushort*	pPixel = (GLushort*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		*pPixel = ( ( c.r >> 3 ) << 11 ) | ( ( c.g >> 3) << 6 ) | ( ( c.b >> 3 ) << 1 ) | ( c.a > 0 );
	}
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_RGB565 )
	{
		GLushort*	pPixel = (GLushort*) m_pData;
		pPixel = pPixel + ( y * w ) + x;
		*pPixel = ( ( c.r >> 3 ) << 11 ) | ( ( c.g >> 2 ) << 5 ) | ( c.b >> 3 );
	}
	else if ( m_ePixelFormat == kCCTexture2DPixelFormat_A8 )
	{
		GLubyte*	pPixel = (GLubyte*) m_pData;
		pPixel [ (y * w ) + x ] = c.a;
	}
	else 
	{
		m_bDirty = KD_FALSE;
		return KD_FALSE;
	}

	return KD_FALSE;
}

KDvoid CCTexture2DMutable::fill ( const ccColor4B& tColor )
{
	for ( KDint r = 0; r < (KDint) m_tContentSize.cy; ++r )
	{
		for ( KDint c = 0; c < (KDint) m_tContentSize.cx; ++c )
		{
			this->setPixelAt ( ccp ( c, r ), tColor );
		}
	}
}

CCTexture2D* CCTexture2DMutable::copyMutable ( KDbool bIsMutable )
{
	CCTexture2D*	pRet = KD_NULL;

	if ( bIsMutable )
	{
		pRet = new CCTexture2DMutable ( );
	}
	else
	{
		pRet = new CCTexture2D ( );
	}

	if ( pRet && pRet->initWithData ( m_pData, m_ePixelFormat, m_tContentSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCTexture2DMutable*	CCTexture2DMutable::copy ( KDvoid )
{
	return (CCTexture2DMutable*) this->copyMutable ( KD_TRUE );
}

KDvoid CCTexture2DMutable::copy ( CCTexture2DMutable* pTextureToCopy, const CCPoint& tOffset )
{
	for ( KDint r = 0; r < (KDint) m_tContentSize.cy; ++r )
	{
		for ( KDint c = 0; c < (KDint) m_tContentSize.cx; ++c )
		{
			this->setPixelAt ( ccp ( c + tOffset.x, r + tOffset.y ), pTextureToCopy->pixelAt ( ccp ( c, r ) ) );
		}
	}
}

KDvoid CCTexture2DMutable::apply ( KDvoid )
{
	if ( !m_pData )
	{
		return;
	}
	
	this->initWithData ( m_pData, m_ePixelFormat, m_tContentSize );

	m_bDirty = KD_FALSE;
}

KDvoid* CCTexture2DMutable::getTexData ( KDvoid )
{
	return m_pData;
}

KDvoid CCTexture2DMutable::setTexData ( KDvoid* pTexData )
{
	m_pData = pTexData;
}