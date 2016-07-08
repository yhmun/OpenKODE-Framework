/* --------------------------------------------------------------------------
 *
 *      File            M3GTransformable.cpp
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
#include <XMM3G/M3GTransformable.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

M3GTransformable::M3GTransformable ( KDvoid )
{
	m_fScaling [ 0 ] = 1;
	m_fScaling [ 1 ] = 1;
	m_fScaling [ 2 ] = 1;

	m_fTranslation [ 0 ] = 0;
	m_fTranslation [ 1 ] = 0;
	m_fTranslation [ 2 ] = 0;

	m_pOrientation = new M3GQuaternion ( 0, 0, 0, 1 );
}

M3GTransformable::~M3GTransformable ( KDvoid )
{
	delete m_pOrientation;
}

KDvoid M3GTransformable::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::ORIENTATION  ||
         nProperty == M3GAnimationTrack::SCALE ||
         nProperty == M3GAnimationTrack::TRANSLATION )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GObject3D::addAnimationTrack ( animationTrack );
	}
}

KDint M3GTransformable::animate ( KDint time )
{
	M3GObject3D::animate ( time );

    KDbool	isOrientationModefied = KD_FALSE;
    KDbool	isScalingModefied     = KD_FALSE;
    KDbool	isTranslationModefied = KD_FALSE;

	KDfloat        fScaling     [ 3 ] = { 0, 0, 0 };
    KDfloat        fTranslation [ 3 ] = { 0, 0, 0 };
    M3GQuaternion  oOrientation = M3GQuaternion ( );

    oOrientation.setZero ( );

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
			case M3GAnimationTrack::ORIENTATION : 
			{
				KDfloat  fValue [ 4 ] = { 0, 0, 0, 0 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				M3GQuaternion  q;

				q.set ( fValue [ 0 ], fValue [ 1 ], fValue [ 2 ], fValue [ 3 ] );
				
				oOrientation = oOrientation + q * fWeight;

				isOrientationModefied = KD_TRUE;

				break;
			}

			case M3GAnimationTrack::SCALE :
			{
				KDfloat  fValue [ 3 ] = { 0, 0, 0 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				if ( pKeyframe->getComponentCount ( ) == 3 )
				{
					fScaling [ 0 ] = fValue [ 0 ] * fWeight;
					fScaling [ 1 ] = fValue [ 1 ] * fWeight;
					fScaling [ 2 ] = fValue [ 2 ] * fWeight;
				}
				else
				{
					fScaling [ 2 ] = fScaling [ 1 ] = fScaling [ 0 ] = fValue [ 0 ] * fWeight;;
				}

				isScalingModefied = KD_TRUE;
	 
				break;
			}

			case M3GAnimationTrack::TRANSLATION :
			{
				KDfloat  fValue [ 3 ] = { 0, 0, 0 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fTranslation [ 0 ] = fValue [ 0 ] * fWeight;
				fTranslation [ 1 ] = fValue [ 1 ] * fWeight;
				fTranslation [ 2 ] = fValue [ 2 ] * fWeight;

				isTranslationModefied = KD_TRUE;

				break;
			}
			
			default :
			{
				// Unknown target should be ignored.
				// animate() of Base class (of Derived class) retrieve it.
			}
        }
    }

    if ( isOrientationModefied ) 
	{
        *m_pOrientation = oOrientation.normalize ( );
    }

    if ( isScalingModefied ) 
	{
		kdMemcpy ( m_fScaling, fScaling, sizeof ( KDfloat ) * 3 );
    }

    if ( isTranslationModefied )
	{
        kdMemcpy ( m_fTranslation, fTranslation, sizeof ( KDfloat ) * 3 );
    }

    return 0;
}

M3GObject3D* M3GTransformable::duplicate ( KDvoid ) const
{
	M3GTransformable*  pObj = new M3GTransformable ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GTransformable::_duplicate ( M3GTransformable* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	kdMemcpy ( pObj->m_fScaling, m_fScaling, sizeof ( KDfloat ) * 3 );
	kdMemcpy ( pObj->m_fTranslation, m_fTranslation, sizeof ( KDfloat ) * 3 );

	pObj->m_tTransform = m_tTransform;

	pObj->m_pOrientation = new M3GQuaternion ( );
	*pObj->m_pOrientation = *m_pOrientation;
}

KDvoid M3GTransformable::getCompositeTransform ( M3GTransform* transform ) const
{
    if ( transform == NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Tranform is NULL." );
    }

    transform->setIdentity ( );
    transform->postTranslate ( m_fTranslation [ 0 ], m_fTranslation [ 1 ], m_fTranslation [ 2 ] );
    transform->postRotateQuat ( m_pOrientation->x, m_pOrientation->y, m_pOrientation->z, m_pOrientation->w );
    transform->postScale ( m_fScaling [ 0 ], m_fScaling [ 1 ], m_fScaling [ 2 ] );
    transform->postMultiply ( m_tTransform );
}

KDvoid M3GTransformable::getOrientation ( KDfloat* angleAxis ) const
{
    if ( angleAxis == NULL)
	{
		M3GException ( "NullPointerException", __FUNCTION__, "angleAxis is NULL." );
    }
  
    m_pOrientation->getAngleAxis ( angleAxis );
}

KDvoid M3GTransformable::getScale ( KDfloat* xyz ) const
{
    if ( xyz == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Scaling xyz is NULL." );
    }

    xyz [ 0 ] = m_fScaling [ 0 ];
    xyz [ 1 ] = m_fScaling [ 1 ];
    xyz [ 2 ] = m_fScaling [ 2 ];
}

KDvoid M3GTransformable::getTransform ( M3GTransform* transform ) const
{
    if ( transform == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Transform is NULL." );
    }

    transform->set ( &m_tTransform );
}

KDvoid M3GTransformable::getTranslation ( KDfloat* xyz ) const
{
    if ( xyz == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "xyz is NULL." );
    }

    xyz [ 0 ] = m_fTranslation [ 0 ];
    xyz [ 1 ] = m_fTranslation [ 1 ];
    xyz [ 2 ] = m_fTranslation [ 2 ];
}

KDvoid M3GTransformable::postRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az )
{
	*m_pOrientation = ( *m_pOrientation ) * M3GQuaternion ( angle, ax, ay, az );
}

KDvoid M3GTransformable::preRotate ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az )
{
	*m_pOrientation = M3GQuaternion ( angle, ax, ay, az ) * ( *m_pOrientation );
}

KDvoid M3GTransformable::scale ( KDfloat sx, KDfloat sy, KDfloat sz )
{
    m_fScaling [ 0 ] *= sx;
    m_fScaling [ 1 ] *= sy;
    m_fScaling [ 2 ] *= sz;
}

KDvoid M3GTransformable::setOrientation ( KDfloat angle, KDfloat ax, KDfloat ay, KDfloat az )
{
    if ( angle != 0 && ax == 0 && ay == 0 && az == 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Rotation axis is 0,0,0." );
    }

    *m_pOrientation = M3GQuaternion ( angle, ax, ay, az );
}

KDvoid M3GTransformable::setScale ( KDfloat sx, KDfloat sy, KDfloat sz)
{
    m_fScaling [ 0 ] = sx;
    m_fScaling [ 1 ] = sy;
    m_fScaling [ 2 ] = sz;
}

KDvoid M3GTransformable::setTransform ( const M3GTransform* transform )
{
	m_tTransform.set ( transform );
}

KDvoid M3GTransformable::setTranslation ( KDfloat tx, KDfloat ty, KDfloat tz )
{
    m_fTranslation [ 0 ] = tx;
    m_fTranslation [ 1 ] = ty;
    m_fTranslation [ 2 ] = tz;
}

KDvoid M3GTransformable::translate ( KDfloat tx, KDfloat ty, KDfloat tz )
{
    m_fTranslation [ 0 ] += tx;
    m_fTranslation [ 1 ] += ty;
    m_fTranslation [ 2 ] += tz;
}

KDvoid M3GTransformable::render ( M3GRenderState& state ) const
{
    M3GTransform  tTransform;

    getCompositeTransform ( &tTransform );
    
	tTransform.transpose ( );
    
	KDfloat  m[16];
    tTransform.get ( m );
    
	glMultMatrixf ( (GLfloat*) &m );
}