/* --------------------------------------------------------------------------
 *
 *      File            M3GLight.cpp
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
#include <XMM3G/M3GLight.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GAnimationController.h>
#include <XMM3G/M3GKeyframeSequence.h>

M3GLight::M3GLight ( KDvoid )
: m_nMode ( DIRECTIONAL )
, m_nColor ( 0x00ffffff )
, m_fIntensity ( 1 )
, m_fConstant ( 1 )
, m_fLinear ( 0 )
, m_fQuadratic ( 0 )
, m_fAngle ( 45 )
, m_fExponent ( 0 )
{

}

M3GLight::~M3GLight ( KDvoid )
{

}

KDvoid M3GLight::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::COLOR  ||
         nProperty == M3GAnimationTrack::INTENSITY ||
		 nProperty == M3GAnimationTrack::SPOT_ANGLE ||
         nProperty == M3GAnimationTrack::SPOT_EXPONENT )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GNode::addAnimationTrack ( animationTrack );
	}
}

KDint M3GLight::animate ( KDint time )
{
	M3GNode::animate ( time );

	KDbool   isColorModefied        = KD_FALSE;
	KDbool   isIntensityModefied    = KD_FALSE;
	KDbool   isSpotAngleModefied    = KD_FALSE;
	KDbool   isSpotExponentModefied = KD_FALSE;

	KDfloat  fRGB [ ]   = { 0, 0, 0 };
	KDfloat  fIntensity = 0;
	KDfloat  fAngle     = 0;
	KDfloat  fExponent  = 0;

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

			case M3GAnimationTrack::INTENSITY :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fIntensity += fValue [ 0 ] * fWeight;

				isIntensityModefied = KD_TRUE;

				break;
			}

			case M3GAnimationTrack::SPOT_ANGLE : 
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fAngle += fValue [ 0 ] * fWeight;

				isSpotAngleModefied = KD_TRUE;

				break;
			}

			case M3GAnimationTrack::SPOT_EXPONENT :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );

				fExponent += fValue [ 0 ] * fWeight;

				isSpotExponentModefied = KD_TRUE;

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
		
		m_nColor = ( m_nColor & 0xff000000 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
	}

	if ( isIntensityModefied )
	{
		m_fIntensity = fIntensity;
	}

	if ( isSpotAngleModefied )
	{
		m_fAngle = KD_CLAMP ( 0, 90, fAngle );
	}

	if ( isSpotExponentModefied )
	{
		m_fExponent = KD_CLAMP ( 0, 128, fExponent );
	}

	return 0;
}

M3GObject3D* M3GLight::duplicate ( KDvoid ) const
{
	M3GLight*  pObj = new M3GLight ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GLight::_duplicate ( M3GLight* pObj ) const
{
	M3GNode::_duplicate ( pObj );

	pObj->m_nMode		= m_nMode;
	pObj->m_nColor		= m_nColor;
	pObj->m_fIntensity	= m_fIntensity;
	pObj->m_fConstant	= m_fConstant;
	pObj->m_fLinear		= m_fLinear;
	pObj->m_fQuadratic	= m_fQuadratic;
	pObj->m_fAngle		= m_fAngle;
	pObj->m_fExponent	= m_fExponent;
}

KDint M3GLight::getColor ( KDvoid ) const
{
	return m_nColor;
}

KDfloat M3GLight::getConstantAttenuation ( KDvoid ) const
{
	return m_fConstant;
}

KDfloat M3GLight::getIntensity ( KDvoid ) const
{
	return m_fIntensity;
}

KDfloat M3GLight::getLinearAttenuation ( KDvoid ) const
{
	return m_fLinear;
}

KDint M3GLight::getMode ( KDvoid ) const
{
	return m_nMode;
}

KDfloat M3GLight::getQuadraticAttenuation ( KDvoid ) const
{
	return m_fQuadratic;
}

KDfloat M3GLight::getSpotAngle ( KDvoid ) const
{
	return m_fAngle;
}

KDfloat M3GLight::getSpotExponent ( KDvoid ) const
{
	return m_fExponent;
}

KDvoid M3GLight::setAttenuation ( KDfloat constant, KDfloat linear, KDfloat quadratic )
{
    if ( constant < 0 || linear < 0 || quadratic < 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Attenuation parameters is invalid, constant = %f, linear = %f, quadratic = %f.", constant, linear, quadratic );
    }

    if ( constant == 0 && linear == 0 && quadratic == 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Attenuation parameters is invalid, constant = %f, linear = %f, quadratic = %f.", constant, linear, quadratic );
    }

    m_fConstant  = constant;
    m_fLinear    = linear;
    m_fQuadratic = quadratic;
}

KDvoid M3GLight::setColor ( KDint RGB )
{
	m_nColor = RGB & 0x00ffffff;
}

KDvoid M3GLight::setIntensity ( KDfloat intensity )
{
	m_fIntensity = intensity;
}

KDvoid M3GLight::setMode ( KDint mode )
{
    if ( mode != AMBIENT && mode != DIRECTIONAL && mode != OMNI && mode != SPOT )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Mode is invalid, mode = %d.", mode );
    }

    m_nMode = mode;
}

KDvoid M3GLight::setSpotAngle ( KDfloat angle )
{
    if ( angle < 0 || angle > 90 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Spot angle is invalid, angle = %f.", angle );
    }

    m_fAngle = angle;
}

KDvoid M3GLight::setSpotExponent ( KDfloat exponent )
{
    if ( exponent < 0 || exponent > 128 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Spot exponent is invalid, exponent = %f.", exponent );
    }

	m_fExponent = exponent;
}

KDvoid M3GLight::render ( M3GRenderState& state ) const
{
	if ( state.m_nPass != 1 )
	{
		return;
	}

	KDint    nIndex = state.m_nLightIndex++;

	GLfloat  fBlack [ 4 ] = { 0, 0, 0, 1 };
	GLfloat  fRGBA  [ 4 ];

	fRGBA [ 0 ] = m_fIntensity * ( ( m_nColor & 0x00ff0000 ) >> 16 ) / 255.f;
	fRGBA [ 1 ] = m_fIntensity * ( ( m_nColor & 0x0000ff00 ) >>  8 ) / 255.f;
	fRGBA [ 2 ] = m_fIntensity * ( ( m_nColor & 0x000000ff ) >>  0 ) / 255.f;
	fRGBA [ 3 ] = m_fIntensity * ( ( m_nColor & 0xff000000 ) >> 24 ) / 255.f;

	M3GTransform  tTransform;
	getCompositeTransform ( &tTransform );

	KDfloat  m[16];
	tTransform.get ( m );

	glEnable ( GL_LIGHTING );
	glEnable ( GL_LIGHT0 + nIndex );

	if ( m_nMode == AMBIENT )
	{
		glLightfv ( GL_LIGHT0 + nIndex, GL_AMBIENT    , fRGBA  );
		glLightfv ( GL_LIGHT0 + nIndex, GL_DIFFUSE    , fBlack );
		glLightfv ( GL_LIGHT0 + nIndex, GL_SPECULAR   , fBlack );
		glLightf  ( GL_LIGHT0 + nIndex, GL_SPOT_CUTOFF, 180.0f );
	}
	else if ( m_nMode == DIRECTIONAL )
	{
		GLfloat xyzw[4] = {m[2], m[6], m[10], 0};

		glLightfv ( GL_LIGHT0 + nIndex, GL_POSITION   , xyzw );
		glLightfv ( GL_LIGHT0 + nIndex, GL_AMBIENT    , fBlack );
		glLightfv ( GL_LIGHT0 + nIndex, GL_DIFFUSE    , fRGBA  );
		glLightfv ( GL_LIGHT0 + nIndex, GL_SPECULAR   , fRGBA  );
		glLightf  ( GL_LIGHT0 + nIndex, GL_SPOT_CUTOFF, 180.0f );
	}
	else if ( m_nMode == OMNI ) 
	{
		GLfloat  fXYZW [ 4 ] = { m[3], m[7], m[11], 1.0f };

		glLightfv ( GL_LIGHT0 + nIndex, GL_POSITION   , fXYZW  );
		glLightfv ( GL_LIGHT0 + nIndex, GL_AMBIENT    , fBlack );
		glLightfv ( GL_LIGHT0 + nIndex, GL_DIFFUSE    , fRGBA  );
		glLightfv ( GL_LIGHT0 + nIndex, GL_SPECULAR   , fRGBA  );
		glLightf  ( GL_LIGHT0 + nIndex, GL_SPOT_CUTOFF, 180.f  );
	}
	else if ( m_nMode == SPOT ) 
	{
		GLfloat  fPos [ 4 ] = {  m[3],  m[7],  m[11], 1 };
		GLfloat  fDir [ 4 ] = { -m[2], -m[6], -m[10], 0 };

		glLightfv ( GL_LIGHT0 + nIndex, GL_POSITION      , fPos   );
		glLightfv ( GL_LIGHT0 + nIndex, GL_AMBIENT       , fBlack );
		glLightfv ( GL_LIGHT0 + nIndex, GL_DIFFUSE       , fRGBA  );
		glLightfv ( GL_LIGHT0 + nIndex, GL_SPECULAR      , fRGBA  );
		glLightfv ( GL_LIGHT0 + nIndex, GL_SPOT_DIRECTION, fDir   );
		glLightf  ( GL_LIGHT0 + nIndex, GL_SPOT_EXPONENT , m_fExponent );
		glLightf  ( GL_LIGHT0 + nIndex, GL_SPOT_CUTOFF   , m_fAngle );
	}
	else
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Invalid light mode, mode = %d.", m_nMode );
	}

	glLightf ( GL_LIGHT0 + nIndex, GL_CONSTANT_ATTENUATION , m_fConstant  );
	glLightf ( GL_LIGHT0 + nIndex, GL_LINEAR_ATTENUATION   , m_fLinear    );
	glLightf ( GL_LIGHT0 + nIndex, GL_QUADRATIC_ATTENUATION, m_fQuadratic );
}