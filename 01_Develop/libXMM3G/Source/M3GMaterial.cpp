/* --------------------------------------------------------------------------
 *
 *      File            M3GMaterial.cpp 
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
#include <XMM3G/M3GMaterial.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

M3GMaterial::M3GMaterial ( KDvoid )
: m_bVertexColorTracking ( KD_FALSE )
, m_nAmbientColor ( 0x00333333 )
, m_nDiffuseColor ( 0xffcccccc ) 
, m_nEmissiveColor ( 0 ) 
, m_nSpecularColor ( 0 )
, m_fShininess ( 0 )
{

}

M3GMaterial::~M3GMaterial ( KDvoid )
{

}

KDvoid M3GMaterial::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::ALPHA          ||
         nProperty == M3GAnimationTrack::AMBIENT_COLOR  ||
         nProperty == M3GAnimationTrack::DIFFUSE_COLOR  ||
         nProperty == M3GAnimationTrack::EMISSIVE_COLOR ||
         nProperty == M3GAnimationTrack::SHININESS      || 
         nProperty == M3GAnimationTrack::SPECULAR_COLOR )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GObject3D::addAnimationTrack ( animationTrack );
	}
}

KDint M3GMaterial::animate ( KDint time )
{
	M3GObject3D::animate ( time );

    KDbool  isAlphaModefied         = KD_FALSE;
    KDbool  isAmbientColorModefied  = KD_FALSE;
    KDbool  isDiffuseColorModefied  = KD_FALSE;
    KDbool  isEmissiveColorModefied = KD_FALSE;
    KDbool  isSpecularColorModefied = KD_FALSE;
    KDbool  isShininessModefied     = KD_FALSE;

    KDfloat  fAmbientRGB  [ ] = { 0, 0, 0 };
    KDfloat  fDiffuseRGB  [ ] = { 0, 0, 0 };
    KDfloat  fEmissiveRGB [ ] = { 0, 0, 0 };
    KDfloat  fSpecularRGB [ ] = { 0, 0, 0 };

    KDfloat  fAlpha     = 0;
    KDfloat  fShininess = 0;

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

			case M3GAnimationTrack::AMBIENT_COLOR :
			{
				KDfloat  fValue [ 3 ] = { 1, 1, 1 }; 

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fAmbientRGB [ 0 ] += fValue [ 0 ] * fWeight;
				fAmbientRGB [ 1 ] += fValue [ 1 ] * fWeight;
				fAmbientRGB [ 2 ] += fValue [ 2 ] * fWeight;

				isAmbientColorModefied = KD_TRUE;

				break;
			}

			case M3GAnimationTrack::DIFFUSE_COLOR :
			{
				KDfloat  fValue [ 3 ] = { 1, 1, 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fDiffuseRGB [ 0 ] += fValue [ 0 ] * fWeight;
				fDiffuseRGB [ 1 ] += fValue [ 1 ] * fWeight;
				fDiffuseRGB [ 2 ] += fValue [ 2 ] * fWeight;

				isDiffuseColorModefied = KD_TRUE;
	
				break;
			}

			case M3GAnimationTrack::EMISSIVE_COLOR :
			{
				KDfloat  fValue [ 3 ] = { 1, 1, 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fEmissiveRGB [ 0 ] += fValue [ 0 ] * fWeight;
				fEmissiveRGB [ 1 ] += fValue [ 1 ] * fWeight;
				fEmissiveRGB [ 2 ] += fValue [ 2 ] * fWeight;

				isEmissiveColorModefied = KD_TRUE;

				break;
			}

			case M3GAnimationTrack::SPECULAR_COLOR : 
			{
				KDfloat  fValue [ 3 ] = { 1, 1, 1 };
				
				pKeyframe->sample ( (KDint) fSequence, fValue );

				fSpecularRGB [ 0 ] += fValue [ 0 ] * fWeight;
				fSpecularRGB [ 1 ] += fValue [ 1 ] * fWeight;
				fSpecularRGB [ 2 ] += fValue [ 2 ] * fWeight;

				isSpecularColorModefied = KD_TRUE;

				break;
			}

			case M3GAnimationTrack::SHININESS :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fShininess += fValue [ 0 ] * fWeight;

				isShininessModefied = KD_TRUE;

				break;
			}

			default : 
			{
				// Unknown target should be ignored.
				// animate() of Base class handle it.
			}
        }
    }

    if ( isAmbientColorModefied ) 
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fAmbientRGB [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fAmbientRGB [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fAmbientRGB [ 2 ] ) * 255 );

        m_nAmbientColor = ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }

    if ( isDiffuseColorModefied ) 
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fDiffuseRGB [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fDiffuseRGB [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fDiffuseRGB [ 2 ] ) * 255 );
        
        m_nDiffuseColor = ( m_nDiffuseColor & 0xff000000 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }

	if ( isEmissiveColorModefied )
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fEmissiveRGB [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fEmissiveRGB [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fEmissiveRGB [ 2 ] ) * 255 );
        
        m_nEmissiveColor = ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }
    
	if ( isAlphaModefied ) 
	{
        KDubyte  a = (KDubyte) ( KD_CLAMP ( 0, 1, fAlpha ) * 255 );

        m_nDiffuseColor = ( m_nDiffuseColor & 0x00ffffff ) | ( a << 24 );
    }
    
    if ( isSpecularColorModefied ) 
	{
        KDubyte  r = (KDubyte) ( KD_CLAMP ( 0, 1, fSpecularRGB [ 0 ] ) * 255 );
        KDubyte  g = (KDubyte) ( KD_CLAMP ( 0, 1, fSpecularRGB [ 1 ] ) * 255 );
        KDubyte  b = (KDubyte) ( KD_CLAMP ( 0, 1, fSpecularRGB [ 2 ] ) * 255 );
 
		m_nSpecularColor = ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    }

	if ( isShininessModefied ) 
	{
        m_fShininess = KD_CLAMP ( 0, 128, fShininess );
    }

    return 0;
}

M3GObject3D* M3GMaterial::duplicate ( KDvoid ) const
{
	M3GMaterial*  pObj = new M3GMaterial ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GMaterial::_duplicate ( M3GMaterial* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

    pObj->m_bVertexColorTracking = m_bVertexColorTracking;
    pObj->m_nAmbientColor        = m_nAmbientColor;
    pObj->m_nDiffuseColor        = m_nDiffuseColor;
    pObj->m_nEmissiveColor       = m_nEmissiveColor;
    pObj->m_nSpecularColor       = m_nSpecularColor;
    pObj->m_fShininess           = m_fShininess;
}

KDint M3GMaterial::getColor ( KDint target ) const
{
	KDint  nColor = 0;

    switch ( target )
	{
		case AMBIENT  : nColor = m_nAmbientColor;	break;
		case DIFFUSE  : nColor = m_nDiffuseColor;	break;
		case EMISSIVE : nColor = m_nEmissiveColor;	break;
		case SPECULAR : nColor = m_nSpecularColor;	break;

		default : 

			M3GException ( "IllegalArgumentException", __FUNCTION__ , "Target is invalid, target = %d.", target );
    }

	return nColor;
}

KDfloat M3GMaterial::getShininess ( KDvoid ) const
{
	return m_fShininess;
}

KDbool M3GMaterial::isVertexColorTrackingEnabled ( KDvoid ) const
{
	return m_bVertexColorTracking;
}

KDvoid M3GMaterial::setColor ( KDint target, KDint ARGB )
{
    if ( !( target & AMBIENT || target & DIFFUSE || target & EMISSIVE || target & SPECULAR ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Target color is invalid, target = %d.", target );
    }
    
    if ( target & AMBIENT )
	{
        m_nAmbientColor  = ARGB & 0x00ffffff;
    }

    if ( target & DIFFUSE )
	{
        m_nDiffuseColor  = ARGB;
    }

    if ( target & EMISSIVE )
	{
        m_nEmissiveColor = ARGB & 0x00ffffff;
    }

    if ( target & SPECULAR ) 
	{
        m_nSpecularColor = ARGB & 0x00ffffff;
    }
}

KDvoid M3GMaterial::setShininess ( KDfloat shininess )
{
    if ( shininess < 0 || shininess > 128 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__ , "Shininess is invalid, shininess = %f.", shininess );
    }

    m_fShininess = shininess;
}

KDvoid M3GMaterial::setVertexColorTrackingEnable ( KDbool enable )
{
	m_bVertexColorTracking = enable;
}

KDvoid M3GMaterial::render ( KDvoid )
{
    GLfloat  fGray  [ 4 ] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat  fDark  [ 3 ] = { 0.2f, 0.2f, 0.2f };
    GLfloat  fBlack [ 3 ] = { 0.0f, 0.0f, 0.0f };
    GLfloat  fZero        = 0.0f;

    glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT  , fDark  );
    glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE  , fGray  );
    glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR , fBlack );
    glMaterialf  ( GL_FRONT_AND_BACK, GL_SHININESS, fZero  );
    glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION , fBlack );

    glColor4f    ( fGray [ 0 ], fGray [ 1 ], fGray [ 2 ], fGray [ 3 ] );
}

KDvoid M3GMaterial::render ( M3GRenderState& state ) const
{
    if ( state.m_nPass != 2 ) 
	{
        return;
    }

    GLfloat  fAmbientRGB [ 4 ] = { ( ( m_nAmbientColor & 0x00ff0000 ) >> 16 ) / 255.f,
                                   ( ( m_nAmbientColor & 0x0000ff00 ) >> 8  ) / 255.f,
                                   ( ( m_nAmbientColor & 0x000000ff ) >> 0  ) / 255.f, 1 };

    glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT, fAmbientRGB );

    GLfloat  fDiffuseRGBA [ 4 ] = { ( ( m_nDiffuseColor & 0x00ff0000 ) >> 16 ) / 255.f,
                                    ( ( m_nDiffuseColor & 0x0000ff00 ) >> 8  ) / 255.f,
                                    ( ( m_nDiffuseColor & 0x000000ff ) >> 0  ) / 255.f,
								    ( ( m_nDiffuseColor & 0xff000000 ) >> 24 ) / 255.f * state.m_fAlpha };

    glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, fDiffuseRGBA );

    // Diffuse color is used as 'Color' too.
    glColor4f    ( fDiffuseRGBA [ 0 ], fDiffuseRGBA [ 1 ], fDiffuseRGBA [ 2 ], fDiffuseRGBA [ 3 ] );

    GLfloat  fSpecularRGB [ 4 ] = { ( ( m_nSpecularColor & 0x00ff0000 ) >> 16 ) / 255.f,
                                    ( ( m_nSpecularColor & 0x0000ff00 ) >> 8  ) / 255.f,
                                    ( ( m_nSpecularColor & 0x000000ff ) >> 0  ) / 255.f, 1 };

    glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR , fSpecularRGB );
    glMaterialf  ( GL_FRONT_AND_BACK, GL_SHININESS, m_fShininess );

    GLfloat  fEmissiveRGB [ 4 ] = { ( ( m_nEmissiveColor & 0x00ff0000 ) >> 16 ) / 255.f,
                                    ( ( m_nEmissiveColor & 0x0000ff00 ) >> 8  ) / 255.f,
                                    ( ( m_nEmissiveColor & 0x000000ff ) >> 0  ) / 255.f, 1 };

    glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION, fEmissiveRGB );

    if ( m_bVertexColorTracking ) 
	{
        glDisable ( GL_LIGHTING );

		if ( state.m_bVertexColorBufferReady )
		{
            glEnableClientState ( GL_COLOR_ARRAY );
        }
		else 
		{
			GLfloat  fVertexColorRGBA [ 4 ] = { ( ( state.m_nDefaultVertexColor & 0x00ff0000) >> 16) / 255.f,
                                                ( ( state.m_nDefaultVertexColor & 0x0000ff00) >> 8 ) / 255.f,
                                                ( ( state.m_nDefaultVertexColor & 0x000000ff) >> 0 ) / 255.f,
                                                ( ( state.m_nDefaultVertexColor & 0xff000000) >> 24) / 255.f * state.m_fAlpha };

            glColor4f  ( fVertexColorRGBA [ 0 ], fVertexColorRGBA [ 1 ], fVertexColorRGBA [ 2 ], fVertexColorRGBA [ 3 ] );
        }
    }
}

