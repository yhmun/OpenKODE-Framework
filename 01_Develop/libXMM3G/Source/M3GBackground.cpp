/* --------------------------------------------------------------------------
 *
 *      File            M3GBackground.cpp
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
#include <XMM3G/M3GBackground.h>
#include <XMM3G/M3GImage2D.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

static KDint16  l_Positions [ 12 ] =
{
	1, 0, 0,
	1, 1, 0,
	0, 0, 0,
	0, 1, 0
};

static KDfloat  l_TexCoords [ 8 ] =
{
	1, 0,
	1, 1,
	0, 0,
	0, 1
};

static KDuint16  l_nIndices [ 4 ] =
{
	0, 1, 2, 3 
};

M3GBackground::M3GBackground ( KDvoid )
: m_uIndicesID ( 0 )
, m_uPositionID ( 0 )
, m_uTexCoordID ( 0 )
, m_uTextureID ( 0 )
, m_bColorClearEnable ( KD_TRUE )
, m_bDepthClearEnable ( KD_TRUE )
, m_nColor ( 0 )
, m_pImage ( KD_NULL )
, m_nModeX ( BORDER )
, m_nModeY ( BORDER )
, m_fCropX ( 0 )
, m_fCropY ( 0 )
, m_fCropWidth ( 0 )
, m_fCropHeight ( 0 )
{
	GLenum  nErr;

    glGenTextures ( 1, &m_uTextureID );

    nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
//		M3GException ( "OpenGLException", __FUNCTION__, "Can't make texture object, err = %d.", nErr );
    }

    glGenBuffers ( 1, &m_uPositionID );
    glBindBuffer ( GL_ARRAY_BUFFER, m_uPositionID );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( l_Positions ), l_Positions, GL_STATIC_DRAW );

    glGenBuffers ( 1, &m_uTexCoordID );
    glBindBuffer ( GL_ARRAY_BUFFER, m_uTexCoordID );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( l_TexCoords ), l_TexCoords, GL_STATIC_DRAW );

    glGenBuffers ( 1, &m_uIndicesID );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_uIndicesID );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( l_nIndices ), l_nIndices, GL_STATIC_DRAW );

    nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't make vertex buffer object, err = %d.", nErr );
    }
}

M3GBackground::~M3GBackground ( KDvoid )
{
    if ( m_uTextureID > 0 ) 
	{
        glDeleteTextures ( 1, &m_uTextureID );
    }

    if ( m_uPositionID > 0 )
	{
        glDeleteBuffers ( 1, &m_uPositionID );
    }

    if ( m_uTexCoordID > 0 )
	{
        glDeleteBuffers ( 1, &m_uTexCoordID );
    }

    if ( m_uIndicesID > 0 ) 
	{
        glDeleteBuffers ( 1, &m_uIndicesID );
    }
}

KDvoid M3GBackground::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::ALPHA ||
         nProperty == M3GAnimationTrack::COLOR ||
         nProperty == M3GAnimationTrack::CROP  )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GObject3D::addAnimationTrack ( animationTrack );
	}
}

KDint M3GBackground::animate ( KDint time )
{
	M3GObject3D::animate ( time );

    KDbool    isColorModefied = KD_FALSE;
    KDbool    isAlphaModefied = KD_FALSE;
    KDbool    isCropModefied  = KD_FALSE;

    KDfloat   fColor [ ]  = { 0, 0, 0 };
    KDfloat   fAlpha      = 0;
    KDfloat   fCropX      = 0;
	KDfloat   fCropY      = 0;
	KDfloat   fCropWidth  = 0;
	KDfloat   fCropHeight = 0;
    
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
			case M3GAnimationTrack::ALPHA : 
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fAlpha += fValue [ 0 ] * fWeight;
				
				isAlphaModefied = KD_TRUE;
				
				break;
			}

			case M3GAnimationTrack::CROP : 
			{
				KDfloat  fValue [ 4 ] = { 0, 0, 0, 0 }; 

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				if ( pKeyframe->getComponentCount ( ) == 4 ) 
				{
					fCropX      += fValue [ 0 ] * fWeight;
					fCropY      += fValue [ 1 ] * fWeight;
					fCropWidth  += fValue [ 2 ] * fWeight;
					fCropHeight += fValue [ 3 ] * fWeight;
				} 
				else
				{
					fCropX      += fValue [ 0 ]  * fWeight;
					fCropY      += fValue [ 1 ]  * fWeight;
					fCropWidth  += m_fCropWidth  * fWeight;
					fCropHeight += m_fCropHeight * fWeight;
		
				}

				isCropModefied = KD_TRUE;
				
				break;
			}

			case M3GAnimationTrack::COLOR :
			{
				KDfloat  fValue [ 3 ] = { 1, 1, 1 }; 
				
				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fColor [ 0 ] += fValue [ 0 ] * fWeight;
				fColor [ 1 ] += fValue [ 1 ] * fWeight;
				fColor [ 2 ] += fValue [ 2 ] * fWeight;

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

    if ( isCropModefied ) 
	{
        fCropWidth  = KD_MAX ( 0.f, fCropWidth  );
        fCropHeight = KD_MAX ( 0.f, fCropHeight );

		m_fCropX	  = fCropX;
		m_fCropY	  = fCropY;
		m_fCropWidth  = fCropWidth;
		m_fCropHeight = fCropHeight;
    }

    if ( isColorModefied ) 
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fColor [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fColor [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fColor [ 2 ] ) * 255 );

        m_nColor = ( m_nColor & 0xff000000 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }
    
	if ( isAlphaModefied ) 
	{
		KDubyte  a = (KDubyte) ( KD_CLAMP ( 0, 1, fAlpha ) * 255 );

        m_nColor = ( m_nColor & 0x00ffffff ) | ( a << 24 );
    }

	return 0;
}

M3GObject3D* M3GBackground::duplicate ( KDvoid ) const
{
	M3GBackground*  pObj = new M3GBackground ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GBackground::_duplicate ( M3GBackground* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_bColorClearEnable	= m_bColorClearEnable;
	pObj->m_bDepthClearEnable	= m_bDepthClearEnable;
	pObj->m_nColor				= m_nColor;
	pObj->m_nModeX				= m_nModeX;
	pObj->m_nModeY				= m_nModeY;
	pObj->m_fCropX				= m_fCropX;
	pObj->m_fCropY				= m_fCropY;
	pObj->m_fCropWidth			= m_fCropWidth;
	pObj->m_fCropHeight			= m_fCropHeight;

	pObj->setImage ( m_pImage );
}

KDint M3GBackground::getReferences ( M3GObject3D** references ) const
{
	KDint  nNumber = M3GObject3D::getReferences ( references );

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

KDint M3GBackground::getColor ( KDvoid ) const
{
	return m_nColor;
}

KDint M3GBackground::getCropHeight ( KDvoid ) const
{
	return (KDint) m_fCropHeight;
}

KDint M3GBackground::getCropWidth ( KDvoid ) const
{
	return (KDint) m_fCropWidth;
}

KDint M3GBackground::getCropX ( KDvoid ) const
{
	return (KDint) m_fCropX;
}

KDint M3GBackground::getCropY ( KDvoid ) const
{
	return (KDint) m_fCropY;
}

M3GImage2D* M3GBackground::getImage ( KDvoid ) const
{
	return m_pImage;
}

KDint M3GBackground::getImageModeX ( KDvoid ) const
{
	return m_nModeX;
}

KDint M3GBackground::getImageModeY ( KDvoid ) const
{
	return m_nModeY;
}

KDbool M3GBackground::isColorClearEnabled ( KDvoid ) const
{
	return m_bColorClearEnable;
}

KDbool M3GBackground::isDepthClearEnabled ( KDvoid ) const
{
	return m_bDepthClearEnable;
}

KDvoid M3GBackground::setColor ( KDint ARGB )
{
	m_nColor = ARGB;
}

KDvoid M3GBackground::setColorClearEnable ( KDbool enable )
{
	m_bColorClearEnable = enable;
}

KDvoid M3GBackground::setCrop ( KDint cropX, KDint cropY, KDint cropWidth, KDint cropHeight )
{
	m_fCropX		= (KDfloat) cropX;
	m_fCropY		= (KDfloat) cropY;
	m_fCropWidth	= (KDfloat) cropWidth;
	m_fCropHeight	= (KDfloat) cropHeight;
}

KDvoid M3GBackground::setDepthClearEnable ( KDbool enable )
{
	m_bDepthClearEnable = enable;
}

KDvoid M3GBackground::setImage ( M3GImage2D* image )
{
    m_pImage = image;

    if ( image == KD_NULL )
	{
        return;
    }

	KDint     nFormat = image->getFormat ( );	
    if ( nFormat != M3GImage2D::RGB && nFormat != M3GImage2D::RGBA )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Image format is invalid, format = %d.", nFormat );
    }

    KDint     nWidth  = image->getWidth  ( );
    KDint     nHeight = image->getHeight ( );
    KDubyte*  pPixels = image->getPixels ( );
	KDint     nGLFormat = image->getOpenGLFormat ( );

	m_fCropX		= 0;
	m_fCropY		= 0;
	m_fCropWidth	= (KDfloat) nWidth;
	m_fCropHeight	= (KDfloat) nHeight;

    glActiveTexture ( GL_TEXTURE0 );
    glEnable        ( GL_TEXTURE_2D );
    glBindTexture   ( GL_TEXTURE_2D, m_uTextureID );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );
    glTexParameterf ( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); 
    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);               
    glTexImage2D    ( GL_TEXTURE_2D, 0, nGLFormat, nWidth, nHeight, 0, nGLFormat, GL_UNSIGNED_BYTE, pPixels );
}

KDvoid M3GBackground::setImageMode ( KDint modeX, KDint modeY )
{
    if ( ( modeX != REPEAT && modeX != BORDER ) ||
         ( modeY != REPEAT && modeY != BORDER ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Mode is invalid, x = %d, y = %d.", modeX, modeY );
    }

    m_nModeX = modeX;
    m_nModeY = modeY;
}

KDvoid M3GBackground::render ( KDvoid )
{
    glDepthMask   ( GL_TRUE );
    glColorMask   ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glClearColor  ( 0, 0, 0, 0 );   
    glClearDepthf ( 1.0f );
    glClear       ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

KDvoid M3GBackground::render ( M3GRenderState& state ) const
{
	if ( state.m_nPass != 0 ) 
	{
		return;
	}

	KDfloat  r = ( ( m_nColor & 0x00ff0000 ) >> 16 ) / 255.f;
	KDfloat  g = ( ( m_nColor & 0x0000ff00 ) >>  8 ) / 255.f;
	KDfloat  b = ( ( m_nColor & 0x000000ff ) >>  0 ) / 255.f;
	KDfloat  a = ( ( m_nColor & 0xff000000 ) >> 24 ) / 255.f;

	glDepthMask   ( GL_TRUE );
	glColorMask   ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glClearColor  ( r, g, b, a );
	glClearDepthf ( 1.f );

	if ( m_bDepthClearEnable )
	{
		glClear ( GL_DEPTH_BUFFER_BIT );
	}

	if ( m_bColorClearEnable )
	{
		glClear ( GL_COLOR_BUFFER_BIT );

		if ( m_pImage )
		{
			KDfloat  fWidth  = (KDfloat) m_pImage->getWidth  ( );
			KDfloat  fHeight = (KDfloat) m_pImage->getHeight ( );
			KDfloat  fScaleX = ( m_nModeX == BORDER ) ? fWidth  : ( ( m_fCropX + m_fCropWidth  - 1) / fWidth  + 1 ) * fWidth;
			KDfloat  fScaleY = ( m_nModeY == BORDER ) ? fHeight : ( ( m_fCropY + m_fCropHeight - 1) / fHeight + 1 ) * fHeight;
			KDfloat  fScaleS = ( m_nModeX == BORDER ) ? 1       :   ( m_fCropX + m_fCropWidth  - 1) / fWidth  + 1;
			KDfloat  fScaleT = ( m_nModeY == BORDER ) ? 1       :   ( m_fCropY + m_fCropHeight - 1) / fHeight + 1;

			glActiveTexture       ( GL_TEXTURE0 );
			glClientActiveTexture ( GL_TEXTURE0 );
			glEnable              ( GL_TEXTURE_2D );
			glBindTexture         ( GL_TEXTURE_2D , m_uTextureID );
			glTexEnvf             ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE  , GL_MODULATE );
			glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S    , GL_REPEAT );
			glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T    , GL_REPEAT );
			glDepthMask           ( GL_FALSE );
			glDisable             ( GL_LIGHTING );
			glDisable             ( GL_CULL_FACE );
			glColor4f             ( 1, 1, 1, 1 );

			glMatrixMode   ( GL_PROJECTION );
			glPushMatrix   ( );
			glLoadIdentity ( );
			glOrthof       ( m_fCropX, m_fCropX + m_fCropWidth, m_fCropY + m_fCropHeight, m_fCropY , -1, 1 );

			glMatrixMode   ( GL_TEXTURE );
			glPushMatrix   ( );
			glLoadIdentity ( );
			glScalef       ( fScaleS, -fScaleT, 1 );
	        
			glMatrixMode   ( GL_MODELVIEW );
			glPushMatrix   ( );
			glLoadIdentity ( );
			glScalef       ( fScaleX, fScaleY, 1 );
	        
			glEnableClientState ( GL_VERTEX_ARRAY );
			glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

			glBindBuffer    ( GL_ARRAY_BUFFER, m_uPositionID );
			glVertexPointer ( 3, GL_SHORT, 0, 0 );

			glBindBuffer      ( GL_ARRAY_BUFFER, m_uTexCoordID );
			glTexCoordPointer ( 2, GL_FLOAT, 0, 0 );

			glBindBuffer    ( GL_ELEMENT_ARRAY_BUFFER, m_uIndicesID );
			//glDrawElements  ( GL_TRIANGLE_STRIP, sizeof ( l_nIndices ) / sizeof ( l_nIndices [ 0 ] ), GL_UNSIGNED_SHORT, 0 );
	        
			glDisableClientState ( GL_VERTEX_ARRAY );
			glDisableClientState ( GL_TEXTURE_COORD_ARRAY );

			glMatrixMode ( GL_PROJECTION );
			glPopMatrix  ( );
			glMatrixMode ( GL_TEXTURE );
			glPopMatrix  ( );
			glMatrixMode ( GL_MODELVIEW );
			glPopMatrix  ( );

			glDepthMask   ( GL_TRUE );
		}
	}
}