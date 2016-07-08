/* --------------------------------------------------------------------------
 *
 *      File            M3GSprite3D.cpp
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
#include <XMM3G/M3GSprite3D.h>
#include <XMM3G/M3GImage2D.h>
#include <XMM3G/M3GAppearance.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>
#include <XMM3G/M3GCompositingMode.h>
#include <XMM3G/M3GFog.h>
#include <XMM3G/M3GCamera.h>
#include <XMM3G/M3GGraphics3D.h>
#include <XMM3G/M3GRayIntersection.h>

static KDfloat  l_Positions [ 12 ] =
{
	 0.5f, -0.5f,  0,
	 0.5f,  0.5f,  0,
	-0.5f, -0.5f,  0,
	-0.5f,  0.5f,  0,
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

M3GSprite3D::M3GSprite3D ( KDvoid )
: m_uIndicesID ( 0 )
, m_uPositionID ( 0 )
, m_uTexCoordID ( 0 )
, m_uTextureID ( 0 )
, m_bScaled ( KD_FALSE )
, m_pImage ( KD_NULL )
, m_pAppearance ( KD_NULL )
, m_fCropX ( 0 )
, m_fCropY ( 0 )
, m_fCropWidth ( 0 )
, m_fCropHeight ( 0 )
{
}

M3GSprite3D::M3GSprite3D ( KDbool scaled, M3GImage2D* image, M3GAppearance* appearance )
: m_uIndicesID ( 0 )
, m_uPositionID ( 0 )
, m_uTexCoordID ( 0 )
, m_uTextureID ( 0 )
, m_bScaled ( KD_FALSE )
, m_pImage ( KD_NULL )
, m_pAppearance ( KD_NULL )
, m_fCropX ( 0 )
, m_fCropY ( 0 )
, m_fCropWidth ( 0 )
, m_fCropHeight ( 0 )
{
	this->init ( scaled, image, appearance );
}

M3GSprite3D::~M3GSprite3D ( KDvoid )
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

KDvoid M3GSprite3D::init ( KDbool scaled, M3GImage2D* image, M3GAppearance* appearance )
{
    if ( image == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Image is NULL." );
    }

	m_bScaled     = scaled;
	m_pImage      = image;
	m_pAppearance = appearance;
    m_fCropWidth  = (KDfloat) image->getWidth  ( );
    m_fCropHeight = (KDfloat) image->getHeight ( );

	GLenum  nErr;

    glGenTextures ( 1, &m_uTextureID );

    nErr = glGetError ( );
    if ( nErr != GL_NO_ERROR )
	{
		M3GException ( "OpenGLException", __FUNCTION__, "Can't make texture object, err = %d.", nErr );
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

	this->setImage ( image );
}

KDvoid M3GSprite3D::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::CROP )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GNode::addAnimationTrack ( animationTrack );
	}
}

KDint M3GSprite3D::animate ( KDint time )
{
	M3GNode::animate ( time );

    KDbool    isCropModefied  = KD_FALSE;

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
		m_fCropX	  = fCropX;
		m_fCropY	  = fCropY;
		m_fCropWidth  = fCropWidth;
		m_fCropHeight = fCropHeight;
    }
  
    return 0;
}

M3GObject3D* M3GSprite3D::duplicate ( KDvoid ) const
{
	M3GSprite3D*  pObj = new M3GSprite3D ( m_bScaled, m_pImage, m_pAppearance );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GSprite3D::_duplicate ( M3GSprite3D* pObj ) const
{
	M3GNode::_duplicate ( pObj );
}

KDint M3GSprite3D::getReferences ( M3GObject3D** references ) const
{
	KDint  nNumber = M3GNode::getReferences ( references );

    if ( m_pImage )
	{
		if ( references )
		{
			references [ nNumber ] = m_pImage;
		}

		nNumber++;
	}

    if ( m_pAppearance )
	{
		if ( references )
		{
			references [ nNumber ] = m_pAppearance;
		}

		nNumber++;
	}

	return nNumber;
}

M3GAppearance* M3GSprite3D::getAppearance ( KDvoid ) const
{
	return m_pAppearance;
}

KDint M3GSprite3D::getCropHeight ( KDvoid ) const
{
	return (KDint) m_fCropHeight;
}

KDint M3GSprite3D::getCropWidth ( KDvoid ) const
{
	return (KDint) m_fCropWidth;
}

KDint M3GSprite3D::getCropX ( KDvoid ) const
{
	return (KDint) m_fCropX;
}

KDint M3GSprite3D::getCropY ( KDvoid ) const
{
	return (KDint) m_fCropY;
}

M3GImage2D* M3GSprite3D::getImage ( KDvoid ) const
{
	return m_pImage;
}

KDbool M3GSprite3D::isScaled ( KDvoid ) const
{
	return m_bScaled;
}

KDvoid M3GSprite3D::setAppearance ( M3GAppearance* appearance )
{
	m_pAppearance = appearance;
}

KDvoid M3GSprite3D::setCrop ( KDint cropX, KDint cropY, KDint cropWidth, KDint cropHeight )
{
	m_fCropX		= (KDfloat) cropX;
	m_fCropY		= (KDfloat) cropY;
	m_fCropWidth	= (KDfloat) cropWidth;
	m_fCropHeight	= (KDfloat) cropHeight;
}

KDvoid M3GSprite3D::setImage ( M3GImage2D* image )
{
    m_pImage = image;

    if ( image == KD_NULL )
	{
        return;
    }
	
    KDint     nWidth  = image->getWidth  ( );
    KDint     nHeight = image->getHeight ( );
    
	
    if ( ( nWidth & ( nWidth - 1 ) ) || ( nHeight & ( nHeight - 1 ) ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Image size must be power of 2. w = %d,h = %d", nWidth, nHeight );
    }

	KDubyte*  pPixels = image->getPixels ( );
	KDint	  nGLFormat = image->getOpenGLFormat ( );
	
	m_fCropX		= 0;
	m_fCropY		= 0;
	m_fCropWidth	= (KDfloat) nWidth;
	m_fCropHeight	= (KDfloat) nHeight;

	glEnable        ( GL_TEXTURE_2D );
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture   ( GL_TEXTURE_2D, m_uTextureID );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );
    glTexParameterf ( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );              
    glTexImage2D    ( GL_TEXTURE_2D, 0, nGLFormat, nWidth, nHeight, 0, nGLFormat, GL_UNSIGNED_BYTE, pPixels );
}	

KDvoid M3GSprite3D::render ( M3GRenderState& state ) const
{
    if ( !isGlobalRenderingEnabled ( ) ) 
	{
        return;
    }

    if ( m_pAppearance == KD_NULL )
	{
        return;
	}

	if ( state.m_nPass == -1 ) 
	{
		state.m_vLayers.push_back ( m_pAppearance->getLayer2 ( ) );
    }

    if ( state.m_nPass != 2 )
	{
        return;
    }

	if ( m_pAppearance->getLayer2 ( ) != state.m_nLayer )
	{
        return;
    }

    M3GNode::render ( state );

    M3GAppearance::render ( );
    M3GCompositingMode*  pCompositing = m_pAppearance->getCompositingMode ( );

    if ( pCompositing )
	{
        pCompositing->render ( state );
    }

    M3GFog*  pFog = m_pAppearance->getFog ( );
    if ( pFog )
	{
        pFog->render ( state );
    }

	const M3GCamera*  pCamera = state.m_pCamera;

    M3GVector  tCenter = getCenterPoint ( pCamera );
    KDfloat    tx = tCenter.x;
    KDfloat    ty = tCenter.y;
    KDfloat    tz = tCenter.z;

    M3GVector  tTexCoord0 = getTexCoord0 ();
    M3GVector  tTexCoord1 = getTexCoord1 ();
    KDfloat    s0 = tTexCoord0.x;
    KDfloat    t0 = tTexCoord0.y;
    KDfloat    s1 = tTexCoord1.x;
    KDfloat    t1 = tTexCoord1.y;

    KDfloat  width  = m_bScaled ? getScaledWidth  ( pCamera ) : getUnscaledWidth  ( );
    KDfloat  height = m_bScaled ? getScaledHeight ( pCamera ) : getUnscaledHeight ( );

    glMatrixMode   ( GL_PROJECTION );
    glPushMatrix   ( );
    glLoadIdentity ( );

    glMatrixMode   ( GL_MODELVIEW );
    glPushMatrix   ( );
    glLoadIdentity ( );
    glTranslatef   ( tx   , ty    , tz );
    glScalef       ( width, height, 1  );

    glMatrixMode   ( GL_TEXTURE );
    glPushMatrix   ( );
    glLoadIdentity ( );
    glTranslatef   ( s0   , t0     , 0 );
    glScalef       ( s1-s0, (t1-t0), 1 );

    glEnable              ( GL_TEXTURE_2D );
    glBindTexture         ( GL_TEXTURE_2D , m_uTextureID );
    glActiveTexture       ( GL_TEXTURE0 );
    glClientActiveTexture ( GL_TEXTURE0 );
    glTexEnvf             ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE    , GL_REPLACE );
    glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER  , GL_LINEAR  );
    glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER  , GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S      , GL_REPEAT  );
    glTexParameterf       ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T      , GL_REPEAT  );
    glDisable             ( GL_CULL_FACE );
    
    glEnableClientState ( GL_VERTEX_ARRAY );
    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

    glBindBuffer    ( GL_ARRAY_BUFFER, m_uPositionID );
    glVertexPointer ( 3, GL_FLOAT, 0, 0 );

    glBindBuffer      ( GL_ARRAY_BUFFER, m_uTexCoordID );
    glTexCoordPointer ( 2, GL_FLOAT, 0, 0 );
    
    glBindBuffer    ( GL_ELEMENT_ARRAY_BUFFER, m_uIndicesID );
    glDrawElements  ( GL_TRIANGLE_STRIP, sizeof ( l_nIndices ) / sizeof ( l_nIndices [ 0 ] ), GL_UNSIGNED_SHORT, 0 );
    
    glDisableClientState ( GL_VERTEX_ARRAY );
    glDisableClientState ( GL_TEXTURE_COORD_ARRAY );

    glMatrixMode ( GL_PROJECTION );
    glPopMatrix  ( );
    glMatrixMode ( GL_TEXTURE );
    glPopMatrix  ( );
    glMatrixMode ( GL_MODELVIEW );
    glPopMatrix  ( );
}

KDbool M3GSprite3D::intersect ( const M3GVector& org, const M3GVector& dir, const M3GCamera* camera, M3GRayIntersection* ri ) const
{
    if ( !isGlobalPickingEnabled ( ) )
	{
        return KD_FALSE;
    }

    M3GVector  tCenter = getCenterPoint ( camera );
    KDfloat    fWidth  = m_bScaled ? getScaledWidth ( camera )  : getUnscaledWidth  ( );
    KDfloat    fHeight = m_bScaled ? getScaledHeight ( camera ) : getUnscaledHeight ( );
    
    M3GVector  tV0 = tCenter + M3GVector (  fWidth / 2, -fHeight / 2, 0 );
    M3GVector  tV1 = tCenter + M3GVector (  fWidth / 2,  fHeight / 2, 0 );
    M3GVector  tV2 = tCenter + M3GVector ( -fWidth / 2, -fHeight / 2, 0 );
    M3GVector  tV3 = tCenter + M3GVector ( -fWidth / 2,  fHeight / 2, 0 );
    
    KDfloat    fU, fV, fT;
    KDbool     bHit;
    
    M3GVector*  pVertices[2][3] = { { &tV2, &tV1, &tV0 }, { &tV2, &tV3, &tV1 } };
    KDint       nIndices [2][3] = { { 2, 1, 0 }, { 2, 3, 1 } };

    for ( KDint  i = 0; i < 2; i++ )
	{
        bHit = M3GTriangleIntersect ( org, dir, 
                                      *pVertices [i][0], *pVertices [i][1], *pVertices [i][2],
                                      &fU, &fV, &fT );
        if ( bHit )
		{
            if ( ri )
			{
                *ri = M3GRayIntersection ( const_cast<M3GSprite3D*> ( this ),
										   org, dir, fT,
										   fU, fV,
										   3, nIndices [ i ],
										   0 );
            }

            return KD_TRUE;
        }
    }

	return KD_FALSE;
}

M3GVector M3GSprite3D::getCenterPoint ( const M3GCamera* camera ) const
{
    M3GTransform  tTransform;

    getTransformTo ( camera, &tTransform );
    M3GMatrix  tModelView = tTransform.getMatrix ();

    camera->getProjection ( &tTransform );
    M3GMatrix  tProjection = tTransform.getMatrix ( );

    M3GVector  tMDL = M3GVector ( 0, 0, 0 );
    M3GVector  tCAM = ( tModelView  * tMDL ).dividedByW ( );
    M3GVector  tNDC = ( tProjection * tCAM ).dividedByW ( );
    
    return tNDC;
}

KDfloat M3GSprite3D::getScaledWidth  ( const M3GCamera* camera ) const
{
    M3GTransform  tTransform;

    getTransformTo ( camera, &tTransform );
    M3GMatrix  tModelView = tTransform.getMatrix ();

    camera->getProjection ( &tTransform );
    M3GMatrix  tProjection = tTransform.getMatrix ();

    M3GVector  tCam  = ( tModelView * M3GVector ( 0, 0, 0 ) ).dividedByW ( );
    M3GVector  pxCam = ( tModelView * M3GVector ( 1, 0, 0 ) ).dividedByW ( );
    KDfloat    dxCam = kdFabsf ( pxCam.x - tCam.x );

    M3GVector  tNdc  = ( tProjection *   tCam                               ).dividedByW ( );
    M3GVector  pxNdc = ( tProjection * ( tCam + M3GVector ( dxCam, 0, 0 ) ) ).dividedByW ( );
    KDfloat    dxNdc = kdFabsf ( pxNdc.x - tNdc.x );

    return dxNdc;
}

KDfloat M3GSprite3D::getScaledHeight ( const M3GCamera* camera ) const
{
    M3GTransform  tTransform;

    getTransformTo ( camera, &tTransform );
    M3GMatrix  tModelView = tTransform.getMatrix ();

    camera->getProjection ( &tTransform );
    M3GMatrix  tProjection = tTransform.getMatrix ();

    M3GVector  tCam  = ( tModelView * M3GVector ( 0, 0, 0 ) ).dividedByW ( );
    M3GVector  pyCam = ( tModelView * M3GVector ( 0, 1, 0 ) ).dividedByW ( );
    KDfloat    dyCam = kdFabsf ( pyCam.y - tCam.y );

    M3GVector  tNdc  = ( tProjection *   tCam                               ).dividedByW ( );
    M3GVector  pyNdc = ( tProjection * ( tCam + M3GVector ( 0, dyCam, 0 ) ) ).dividedByW ( );
    KDfloat    dyNdc = kdFabsf ( pyNdc.y - tNdc.y );

    return dyNdc;
}

KDfloat M3GSprite3D::getUnscaledWidth  ( KDvoid ) const
{
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );

    KDfloat  fImageWidth    = (KDfloat) m_pImage->getWidth ( );
    KDfloat  fViewportWidth = (KDfloat) pG3D->getViewportWidth ( );
    KDfloat  dxNDC          = 2 * fImageWidth / fViewportWidth;

    return dxNDC;
}

KDfloat M3GSprite3D::getUnscaledHeight ( KDvoid ) const
{
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );

    KDfloat  fImageHeight    = (KDfloat) m_pImage->getHeight ( );
    KDfloat  fViewportHeight = (KDfloat) pG3D->getViewportHeight ( );
    KDfloat  dyNDC           = 2 * fImageHeight / fViewportHeight;

    return dyNDC;
}

M3GVector M3GSprite3D::getTexCoord0 ( KDvoid ) const
{
    KDfloat  w = (KDfloat) m_pImage->getWidth  ( );
    KDfloat  h = (KDfloat) m_pImage->getHeight ( );
    KDfloat  s = ( m_fCropWidth  >= 0 ) ? m_fCropX / w : ( m_fCropX - m_fCropWidth  ) / w;
    KDfloat  t = ( m_fCropHeight >= 0 ) ? m_fCropY / h : ( m_fCropY - m_fCropHeight ) / h;
    
    return M3GVector ( s, 1 - t, 0 );
}

M3GVector M3GSprite3D::getTexCoord1 ( KDvoid ) const
{
    KDfloat  w = (KDfloat) m_pImage->getWidth  ( );
    KDfloat  h = (KDfloat) m_pImage->getHeight ( );
	KDfloat  s = ( m_fCropWidth  >= 0 ) ? ( m_fCropX - m_fCropWidth  ) / w : m_fCropX / w;
	KDfloat  t = ( m_fCropHeight >= 0 ) ? ( m_fCropY - m_fCropHeight ) / h : m_fCropY / h;
    
    return M3GVector ( s, 1 - t, 0 );
}