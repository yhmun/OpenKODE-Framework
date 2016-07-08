/* --------------------------------------------------------------------------
 *
 *      File            M3GImage2D.cpp   
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com   
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

#include "M3GInternal.h"
#include <XMM3G/M3GImage2D.h>

M3GImage2D::M3GImage2D ( KDvoid )
: m_nFormat ( 0 )
, m_nWidth ( 0 )
, m_nHeight ( 0 )
, m_pImage ( KD_NULL )
, m_bImmutable ( KD_TRUE )
{

}

M3GImage2D::M3GImage2D ( KDint format, KDint width, KDint height )
: m_nFormat ( 0 )
, m_nWidth ( 0 )
, m_nHeight ( 0 )
, m_pImage ( KD_NULL )
, m_bImmutable ( KD_FALSE )
{
    if ( width <= 0 || height <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Width or height is invalid, width = %d, height = %d.", width, height );
    }

    if ( format != ALPHA && format != LUMINANCE && format != LUMINANCE_ALPHA && format != RGB && format != RGBA )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Format is invalid, format = %d", format );
    }

	this->init ( format, width, height );
}

M3GImage2D::M3GImage2D ( KDint format, KDint width, KDint height, KDubyte* pixels )
: m_nFormat ( 0 )
, m_nWidth ( 0 )
, m_nHeight ( 0 )
, m_pImage ( KD_NULL )
, m_bImmutable ( KD_FALSE )
{
    if ( width <= 0 || height <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Width or height is invalid, width = %d, height = %d.", width, height );
    }

    if ( format != ALPHA && format != LUMINANCE && format != LUMINANCE_ALPHA && format != RGB && format != RGBA )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Format is invalid, format = %d.", format );
    }

    if ( pixels == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Image is NULL." );
    }

	this->init ( format, width, height, pixels );
}

M3GImage2D::M3GImage2D ( KDint format, KDint width, KDint height, KDubyte* pixels, KDubyte* palette )
: m_nFormat ( 0 )
, m_nWidth ( 0 )
, m_nHeight ( 0 )
, m_pImage ( KD_NULL )
, m_bImmutable ( KD_FALSE )
{
    if ( width <= 0 || height <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Width or height is invalid, width = %d, height = %d.", width, height );
    }

    if ( format != ALPHA && format != LUMINANCE && format != LUMINANCE_ALPHA && format != RGB && format != RGBA )
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Format is invalid, format = %d", format );
    }

    if ( pixels == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Image is NULL." );
    }

    if ( palette == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Palette is NULL." );
    }

	this->init ( format, width, height, pixels, palette );
}

M3GImage2D::~M3GImage2D ( KDvoid )
{
	delete [] m_pImage;
}	

KDvoid M3GImage2D::init ( KDint format, KDint width, KDint height, KDubyte* pixels, KDubyte* palette )
{
	m_nFormat = format;
	m_nWidth  = width;
	m_nHeight = height;

    KDint  nBpp = m3gGetBpp ( format );
    KDint  nLength = height * width * nBpp;

	m_pImage = new KDubyte [ nLength ];

	if ( pixels )
	{
		m_bImmutable = KD_TRUE;

		if ( palette )
		{
			for ( KDint  y = 0; y < height; y++ )
			{
				for ( KDint  x = 0; x < width; x++ ) 
				{
					KDint  nIndex = pixels [ y * width + x ];
					kdMemcpy ( m_pImage + ( y * width + x ) * nBpp, (KDubyte*) palette + nIndex * nBpp, nBpp );
				}
			}
		}
		else
		{
			 kdMemcpy ( m_pImage, pixels, nLength );
		}
	}
	else
	{
		kdMemset ( m_pImage, 0, nLength );
	}
}

M3GObject3D* M3GImage2D::duplicate ( KDvoid ) const
{
	M3GImage2D*  pObj = KD_NULL;

    if ( m_bImmutable )
	{
        pObj = new M3GImage2D ( m_nFormat, m_nWidth, m_nHeight, m_pImage );
	}
    else
	{
        pObj = new M3GImage2D ( m_nFormat, m_nWidth, m_nHeight );
	}

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GImage2D::_duplicate ( M3GImage2D* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );
}

KDint M3GImage2D::getFormat ( KDvoid ) const
{
	return m_nFormat;
}

KDint M3GImage2D::getHeight ( KDvoid ) const
{
	return m_nHeight;
}

KDint M3GImage2D::getWidth ( KDvoid ) const
{
	return m_nWidth;
}

KDbool M3GImage2D::isMutable ( KDvoid ) const
{
	return !m_bImmutable;
}

KDvoid M3GImage2D::set ( KDint x, KDint y, KDint width, KDint height, const KDubyte* image )
{
	if ( m_bImmutable )
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "This image is immutable." );
	}

	if ( x < 0 || y >= m_nWidth || y < 0 || y >= m_nHeight ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Coordinate ( x, y ) is invalid, x = %d, y = %d.", x, y );
	}

	if ( width < 0 || width > m_nWidth || height < 0 || height > m_nHeight )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Size ( width, height ) is invalid, width = %d, height = %d.", width, height );
	}

	if ( image == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Image is NULL." );
	}

    KDint  nBpp = m3gGetBpp ( m_nFormat );
  
	for ( KDint  j = y; j < y + height; j++ )
	{
		kdMemcpy ( m_pImage + j * m_nWidth * nBpp + x * nBpp, (KDchar*) image + ( j - y ) * width * nBpp, width * nBpp );
	}
}

KDubyte* M3GImage2D::getPixels ( KDvoid )
{
	return m_pImage;
}

KDint M3GImage2D::getOpenGLFormat ( KDvoid )
{
	KDint  nFormat = 0;

    switch ( m_nFormat ) 
	{
		case M3GImage2D::ALPHA           : nFormat = GL_ALPHA;				break;
		case M3GImage2D::LUMINANCE       : nFormat = GL_LUMINANCE;			break;
		case M3GImage2D::LUMINANCE_ALPHA : nFormat = GL_LUMINANCE_ALPHA;	break;
		case M3GImage2D::RGB             : nFormat = GL_RGB;				break;
		case M3GImage2D::RGBA            : nFormat = GL_RGBA;				break;

		default :
			M3GException ( "InternalException", __FUNCTION__, "Image format is unknown, format = %d.", m_nFormat );
    }

	return nFormat;
}

KDint m3gGetBpp ( KDint format )
{
	KDint  nBpp = 0;

    switch ( format ) 
	{
		case M3GImage2D::ALPHA           : nBpp = 1; break;
		case M3GImage2D::LUMINANCE       : nBpp = 1; break;
		case M3GImage2D::LUMINANCE_ALPHA : nBpp = 2; break;
		case M3GImage2D::RGB             : nBpp = 3; break;
		case M3GImage2D::RGBA            : nBpp = 4; break;

		default : 
			
			M3GException ( "InternalException", __FUNCTION__, "Format is unknown, format = %d.", format );
    }

	return nBpp;
}