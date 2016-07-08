/* --------------------------------------------------------------------------
 *
 *      File            M3GFog.cpp
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
#include <XMM3G/M3GFog.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

M3GFog::M3GFog ( KDvoid )
: m_nMode ( LINEAR )
, m_nColor ( 0 )
, m_fDensity ( 1 )
, m_fNear ( 0 )
, m_fFar ( 1 )
{

}

M3GFog::~M3GFog ( KDvoid )
{

}

KDvoid M3GFog::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::COLOR			||
         nProperty == M3GAnimationTrack::DENSITY		||
         nProperty == M3GAnimationTrack::FAR_DISTANCE	||
         nProperty == M3GAnimationTrack::NEAR_DISTANCE  )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GObject3D::addAnimationTrack ( animationTrack );
	}
}

KDint M3GFog::animate ( KDint time )
{
	M3GObject3D::animate ( time );

	KDbool   isColorModefied   = KD_FALSE;
	KDbool   isDensityModefied = KD_FALSE;
	KDbool   isFarModefied     = KD_FALSE;
	KDbool   isNearModefied    = KD_FALSE;

	KDfloat  fRGB [ ] = { 0, 0, 0 };
	KDfloat  fDensity = 0;
	KDfloat  fFar     = 0;
	KDfloat  fNear    = 0;

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

			case M3GAnimationTrack::DENSITY :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fDensity += fValue [ 0 ] * fWeight;

				isDensityModefied = KD_TRUE;
				
				break;
			}

			case M3GAnimationTrack::FAR_DISTANCE : 
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fFar += fValue [ 0 ] * fWeight;

				isFarModefied = KD_TRUE;
		
				break;
			}

			case M3GAnimationTrack::NEAR_DISTANCE : 
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fNear += fValue [ 0 ] * fWeight;

				isNearModefied = KD_TRUE;

				break;
			}

			default: 
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

		m_nColor = ( r << 16 ) | ( g << 8 ) | ( b << 0 );
	}

	if ( isDensityModefied ) 
	{
		m_fDensity = KD_MAX ( 0.f, fDensity );
	}

	if ( isFarModefied ) 
	{
		m_fFar = fFar;
	}

	if ( isNearModefied ) 
	{
		m_fNear = fNear;
	}

	return 0;
}

M3GObject3D* M3GFog::duplicate ( KDvoid ) const
{
	M3GFog*  pObj = new M3GFog ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GFog::_duplicate ( M3GFog* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_nMode	 = m_nMode;
	pObj->m_nColor	 = m_nColor;
	pObj->m_fDensity = m_fDensity;
	pObj->m_fNear	 = m_fNear;
	pObj->m_fFar	 = m_fFar;
}

KDint M3GFog::getColor ( KDvoid ) const
{
	return m_nColor;
}

KDfloat M3GFog::getDensity ( KDvoid ) const
{
	return m_fDensity;
}

KDfloat M3GFog::getFarDistance ( KDvoid ) const
{
	return m_fFar;
}

KDint M3GFog::getMode ( KDvoid ) const
{
	return m_nMode;
}

KDfloat M3GFog::getNearDistance ( KDvoid ) const
{
	return m_fNear;
}

KDvoid M3GFog::setColor ( KDint RGB )
{
	m_nColor = RGB & 0x00ffffff;
}

KDvoid M3GFog::setDensity ( KDfloat density )
{
    if ( density < 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Density is invalid, density = %f.", density );
    }

    m_fDensity = density;
}

KDvoid M3GFog::setLinear ( KDfloat zNear, KDfloat zFar )
{
    if ( zNear == zFar ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Near and far are invalid, near = %f, far = %f.", zNear, zFar );
    }

    m_fNear = zNear;
    m_fFar  = zFar;
}

KDvoid M3GFog::setMode ( KDint mode )
{
    if ( mode != LINEAR && mode != EXPONENTIAL ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Mode is invalid, mode = %d.", mode );
    }

    m_nMode = mode;
}

KDvoid M3GFog::render ( KDvoid )
{
	glDisable ( GL_FOG );
}

KDvoid M3GFog::render ( M3GRenderState& state ) const
{
    if ( state.m_nPass != 2 )
	{
        return;
    }

    glEnable ( GL_FOG );

    GLfloat  fRGB [ 3 ] = { ( ( m_nColor & 0x00ff0000 ) >> 16 ) / 255.f,
							( ( m_nColor & 0x0000ff00 ) >> 8  ) / 255.f,
							( ( m_nColor & 0x000000ff ) >> 0  ) / 255.f };

    switch ( m_nMode ) 
	{
		case EXPONENTIAL :

			glFogf  ( GL_FOG_MODE   , GL_EXP );
			glFogf  ( GL_FOG_DENSITY, m_fDensity );
			glFogfv ( GL_FOG_COLOR  , fRGB );
			
			break;

		case LINEAR :

			glFogf  ( GL_FOG_MODE , GL_LINEAR );
			glFogf  ( GL_FOG_START, m_fNear );
			glFogf  ( GL_FOG_END  , m_fFar );
			glFogfv ( GL_FOG_COLOR, fRGB );
			
			break;

		default :

			M3GException ( "InternalException", __FUNCTION__, "Fog mode is unknown, mode = %d.", m_nMode );
    }
}