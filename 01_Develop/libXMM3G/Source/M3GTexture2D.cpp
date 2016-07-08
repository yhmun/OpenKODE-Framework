/* --------------------------------------------------------------------------
 *
 *      File            M3GTexture2D.cpp
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
#include <XMM3G/M3GTexture2D.h>
#include <XMM3G/M3GImage2D.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

M3GTexture2D::M3GTexture2D ( KDvoid )
: m_uID  ( 0 )
, m_pImage ( KD_NULL )
, m_nBlendColor ( 0x00000000 )
, m_nBlending ( FUNC_MODULATE )
, m_nLevelFilter ( FILTER_BASE_LEVEL )
, m_nImageFilter ( FILTER_NEAREST )
, m_nWrappingS ( WRAP_REPEAT )
, m_nWrappingT ( WRAP_REPEAT )
{

}

M3GTexture2D::M3GTexture2D ( M3GImage2D* image )
: m_uID  ( 0 )
, m_pImage ( KD_NULL )
, m_nBlendColor ( 0x00000000 )
, m_nBlending ( FUNC_MODULATE )
, m_nLevelFilter ( FILTER_BASE_LEVEL )
, m_nImageFilter ( FILTER_NEAREST )
, m_nWrappingS ( WRAP_REPEAT )
, m_nWrappingT ( WRAP_REPEAT )
{
	this->init ( image );
}

M3GTexture2D::~M3GTexture2D ( KDvoid )
{
    if ( m_uID > 0 )
	{
        glDeleteTextures ( 1, &m_uID );
    }
}

KDvoid M3GTexture2D::init ( M3GImage2D* image )
{
    if ( image == NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Image is NULL." );
    }

    KDint  nWidth  = image->getWidth  ( );
    KDint  nHeight = image->getHeight ( );

    if ( ( nWidth & ( nWidth - 1 ) ) || ( nHeight & ( nHeight - 1 ) ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Image size must be power of 2. w = %d, h = %d.", nWidth, nHeight  );
    }

    m_pImage = image;

    glGenTextures ( 1, &m_uID );

    GLenum  nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't make texture object, err = %d.", nErr );
    }

    setImage ( image );
}

KDvoid M3GTexture2D::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::COLOR )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GTransformable::addAnimationTrack ( animationTrack );
	}
}

KDint M3GTexture2D::animate ( KDint time )
{
    M3GTransformable:: animate ( time );

    KDbool   isColorModefied = KD_FALSE;
    KDfloat  fRGB [ ] = { 0, 0, 0 };

	for ( KDint  i = 0; i < getAnimationTrackCount ( ); i++ ) 
	{
        M3GAnimationTrack*      pTrack      = getAnimationTrack ( i );
        M3GKeyframeSequence*    pKeyframe   = pTrack->getKeyframeSequence ( );
        M3GAnimationController* pController = pTrack->getController ( );

        if ( !pController || !pController->isActive ( time ) )
		{
            continue;
        }
 
        KDfloat  fWeight   = pController->getWeight ( );
        KDfloat  fSequence = pController->getPosition ( time );

		switch ( pTrack->getTargetProperty ( ) ) 
		{
			case M3GAnimationTrack::COLOR :
			{
				KDfloat  fValue [ 3 ] = { 1, 1, 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fRGB [ 0 ] += fValue [ 0 ] * fWeight;
				fRGB [ 1 ] += fValue [ 1 ] * fWeight;
				fRGB [ 2 ] += fValue [ 2 ] * fWeight;

				isColorModefied = KD_TRUE;

				break;
			}

			default :
			{
				// Unknown target should be ignored.
				// animate() of Base class (of Derived class) retrieve it.
			}
        }
    }

    if ( isColorModefied ) 
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fRGB [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fRGB [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fRGB [ 2 ] ) * 255 );
        
        m_nBlendColor = ( m_nBlendColor & 0xff000000 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }

    return 0;
}

M3GObject3D* M3GTexture2D::duplicate ( KDvoid ) const
{
	M3GTexture2D*  pObj = new M3GTexture2D ( m_pImage );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GTexture2D::_duplicate ( M3GTexture2D* pObj ) const
{
	M3GTransformable::_duplicate ( pObj );

	pObj->m_nBlendColor  = m_nBlendColor;
	pObj->m_nBlending    = m_nBlending;
	pObj->m_nLevelFilter = m_nLevelFilter;
	pObj->m_nImageFilter = m_nImageFilter;
	pObj->m_nWrappingS   = m_nWrappingS;
	pObj->m_nWrappingT   = m_nWrappingT;
}

KDint M3GTexture2D::getReferences ( M3GObject3D** references ) const
{
    KDint  nNumber = M3GTransformable::getReferences ( references );

    if ( m_pImage )
	{
		if ( references )
		{
			references [ nNumber ] = m_pImage;
		}

		nNumber++;
	}

	return nNumber;
}

KDint M3GTexture2D::getBlendColor ( KDvoid ) const
{
	return m_nBlendColor;
}

KDint M3GTexture2D::getBlending ( KDvoid ) const
{
	return m_nBlending;
}

M3GImage2D* M3GTexture2D::getImage ( KDvoid ) const
{
	return m_pImage;
}

KDint M3GTexture2D::getImageFilter ( KDvoid ) const
{
	return m_nImageFilter;
}

KDint M3GTexture2D::getLevelFilter ( KDvoid ) const
{
	return m_nLevelFilter;
}

KDint M3GTexture2D::getWrappingS ( KDvoid ) const
{
	return m_nWrappingS;
}

KDint M3GTexture2D::getWrappingT ( KDvoid ) const
{
	return m_nWrappingT;
}

KDvoid M3GTexture2D::setBlendColor ( KDint RGB )
{
	m_nBlendColor = RGB & 0x00ffffff;
}

KDvoid M3GTexture2D::setBlending ( KDint func )
{
    if ( func != FUNC_ADD && func != FUNC_BLEND && func != FUNC_DECAL && func != FUNC_MODULATE && func != FUNC_REPLACE )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Blending function is invalid, func = %d.", func );
    }

    m_nBlending = func;
}

KDvoid M3GTexture2D::setFiltering ( KDint levelFilter, KDint imageFilter )
{
    if ( levelFilter != FILTER_BASE_LEVEL && levelFilter != FILTER_LINEAR && levelFilter != FILTER_NEAREST )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Level filter is invalid, levelFilter = %d.", levelFilter );
    }

    if ( imageFilter != FILTER_LINEAR && imageFilter != FILTER_NEAREST )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Image filter is invalid, imageFilter = %d.", imageFilter );
    }

    m_nLevelFilter = levelFilter;
    m_nImageFilter = imageFilter;
}

KDvoid M3GTexture2D::setImage ( M3GImage2D* image )
{
    if ( image == NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Image is NULL." );
    }

    KDint  nWidth  = image->getWidth  ( );
    KDint  nHeight = image->getHeight ( );
	
    if ( ( nWidth & ( nWidth - 1 ) ) || ( nHeight & ( nHeight - 1 ) ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Image size must be power of 2. w = %d, h = %d.", nWidth, nHeight  );
    }

    if ( m_uID == 0 )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Texture object is not ready." );
    }

	KDint  nGLFormat = image->getOpenGLFormat ( );
    m_pImage = image;

    glBindTexture   ( GL_TEXTURE_2D, m_uID );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );
    glTexParameterf ( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
	glTexImage2D    ( GL_TEXTURE_2D, 0, nGLFormat, nWidth, nHeight, 0, nGLFormat, GL_UNSIGNED_BYTE, image->getPixels ( ) );
}

KDvoid M3GTexture2D::setWrapping ( KDint wrapS, KDint wrapT )
{
    if ( wrapS != WRAP_CLAMP && wrapS != WRAP_REPEAT )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Wrap mode of S is invalid, wrapS = %d.", wrapS );
    }

    if ( wrapT != WRAP_CLAMP && wrapT != WRAP_REPEAT ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Wrap mode of T is invalid, wrapT = %d.", wrapT );
    }

    m_nWrappingS = wrapS;
    m_nWrappingT = wrapT;
}

KDvoid M3GTexture2D::render ( KDvoid )
{
    glTexEnvf       ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE  , GL_MODULATE );
    glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S    , GL_REPEAT );
    glTexParameterf ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T    , GL_REPEAT );
}

KDvoid M3GTexture2D::render ( M3GRenderState& state ) const
{
	if ( state.m_nPass != 2 ) 
	{
		return;
	}

	if ( m_pImage == KD_NULL || m_uID == 0 )
	{
		return;
	}

	glBindTexture ( GL_TEXTURE_2D, m_uID );

	M3GTransformable::render ( state );

	GLfloat  fColor [ 4 ] = { ( ( m_nBlendColor & 0x00ff0000 ) >> 16) / 255.f,
						      ( ( m_nBlendColor & 0x0000ff00 ) >> 8 ) / 255.f,
						      ( ( m_nBlendColor & 0x000000ff ) >> 0 ) / 255.f,
						      ( ( m_nBlendColor & 0xff000000 ) >> 24) / 255.f };

	switch ( m_nBlending )
	{
		case FUNC_ADD :
			glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
			break;

		case FUNC_BLEND :
			glTexEnvf  ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
			glTexEnvfv ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, fColor );
			break;

		case FUNC_DECAL :
			glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
			break;

		case FUNC_MODULATE : 
			glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
			break;

		case FUNC_REPLACE : 
			glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
			break;

		default :

			M3GException ( "IllegalStateException", __FUNCTION__, "Blending mode is invalid, mode = %d.", m_nBlending );
	}

	if ( m_nLevelFilter != FILTER_BASE_LEVEL && m_nLevelFilter != FILTER_NEAREST && m_nLevelFilter != FILTER_LINEAR )
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "Level filter is invalid, f = %d.", m_nLevelFilter );
	}

	if ( m_nImageFilter != FILTER_NEAREST && m_nImageFilter != FILTER_LINEAR ) 
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "Image filter is invalid, f = %d.", m_nImageFilter );
	}

	switch ( m_nImageFilter ) 
	{
		case FILTER_NEAREST : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); break;
		case FILTER_LINEAR  : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR  ); break;
	}

	switch ( m_nLevelFilter )
	{
		case FILTER_BASE_LEVEL :

			switch ( m_nImageFilter )
			{
				case FILTER_NEAREST : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); break;
				case FILTER_LINEAR  : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR  ); break;
			}

			break;

		case FILTER_NEAREST :

			switch ( m_nImageFilter )
			{
				case FILTER_NEAREST : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST ); break;
				case FILTER_LINEAR  : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST  ); break;
			}

			break;

		case FILTER_LINEAR :

			switch ( m_nImageFilter )
			{
				case FILTER_NEAREST : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ); break;
				case FILTER_LINEAR  : glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR  ); break;
			}

			break;
	}

	switch ( m_nWrappingS )
	{
		case WRAP_CLAMP  :  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); break;
		case WRAP_REPEAT :  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT        ); break;

		default : 
			
			M3GException ( "IllegalStateException", __FUNCTION__, "Wraping mode S is invalid, mode = %d.", m_nWrappingS );
	}

	switch ( m_nWrappingT ) 
	{
		case WRAP_CLAMP  :  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); break;
		case WRAP_REPEAT :  glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT        ); break;
		
		default :
			
			M3GException ( "IllegalStateException", __FUNCTION__, "Wraping mode T is invalid, mode = %d.", m_nWrappingT );
	}
}