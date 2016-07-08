/* --------------------------------------------------------------------------
 *
 *      File            M3GCamera.cpp
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
#include <XMM3G/M3GCamera.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

M3GCamera::M3GCamera ( KDvoid )
: m_nType ( GENERIC ) 
, m_fFovy ( 0 )
, m_fAspect ( 0 )
, m_fNear ( 0 )
, m_fFar ( 0 )
, m_tProjection ( )
{

}

M3GCamera::~M3GCamera ( KDvoid )
{

}

M3GObject3D* M3GCamera::duplicate ( KDvoid ) const
{
	M3GCamera*  pObj = new M3GCamera ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GCamera::_duplicate ( M3GCamera* pObj ) const
{
	M3GNode::_duplicate ( pObj );

	pObj->m_nType		= m_nType;
	pObj->m_fFovy		= m_fFovy;
	pObj->m_fAspect		= m_fAspect;
	pObj->m_fNear		= m_fNear;
	pObj->m_fFar		= m_fFar;
	pObj->m_tProjection = m_tProjection;
}

KDvoid M3GCamera::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::FAR_DISTANCE  ||
         nProperty == M3GAnimationTrack::FIELD_OF_VIEW ||
         nProperty == M3GAnimationTrack::NEAR_DISTANCE )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GNode::addAnimationTrack ( animationTrack );
	}
}

KDint M3GCamera::animate ( KDint time )
{
	M3GNode::animate ( time );

	KDbool   isFarModefied  = KD_FALSE;
	KDbool   isFovyModefied = KD_FALSE;
	KDbool   isNearModefied = KD_FALSE;

	KDfloat  fFar   = 0;
	KDfloat  fFovy  = 0;
	KDfloat  fNear  = 0;

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
			case M3GAnimationTrack:: FAR_DISTANCE :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fFar += fValue [ 0 ] * fWeight;
				
				isFarModefied = KD_TRUE;
			
				break;
			}

			case M3GAnimationTrack::FIELD_OF_VIEW :
			{
				KDfloat  fValue [ 1 ] = { 1 };

				pKeyframe->sample ( (KDint) fSequence, fValue );
				
				fFovy += fValue [ 0 ] * fWeight;
				
				isFovyModefied = KD_TRUE;
				
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

			default :
			{
				// Unknown target should be ignored.
				// animate() of Base class (of Derived class) retrieve it.
			}
		}
	}

	if ( isNearModefied ) 
	{
		switch ( m_nType )
		{
			case PERSPECTIVE : m_fNear = KD_MAX ( KD_FLT_EPSILON, fNear);	break;
			case PARALLEL    : m_fNear = fNear;								break;
		}
	}

	if ( isFarModefied ) 
	{
		switch ( m_nType ) 
		{
			case PERSPECTIVE : m_fFar = KD_MAX ( KD_FLT_EPSILON, fFar );	break;
			case PARALLEL    : m_fFar = fFar;								break;
		}
	}

	if ( isFovyModefied ) 
	{
		switch ( m_nType )
		{
			case PERSPECTIVE : m_fFovy = KD_CLAMP ( KD_FLT_EPSILON, 180 - KD_FLT_EPSILON, fFovy );	break;
			case PARALLEL    : m_fFovy = KD_MAX ( KD_FLT_EPSILON, fFovy );							break;
		}
	}

	if ( isFovyModefied || isNearModefied || isFarModefied )
	{
		switch ( m_nType )
		{
			case PERSPECTIVE : setPerspective ( m_fFovy, m_fAspect, m_fNear, m_fFar ); break;
			case PARALLEL    : setParallel    ( m_fFovy, m_fAspect, m_fNear, m_fFar ); break;
		}
	}

	return 0;
}

KDint M3GCamera::getProjection ( KDfloat* params ) const
{
    if ( params ) 
	{
        params [ 0 ] = m_fFovy;
        params [ 1 ] = m_fAspect;
        params [ 2 ] = m_fNear;
        params [ 3 ] = m_fFar;
    }

    return m_nType;
}

KDint M3GCamera::getProjection ( M3GTransform* transform ) const
{
    if ( transform )
	{
        transform->set ( &m_tProjection );
    }

    return m_nType;
}

KDvoid M3GCamera::setGeneric ( const M3GTransform* transform )
{
    m_nType = GENERIC;
    m_tProjection.set ( transform );
}

KDvoid M3GCamera::setParallel ( KDfloat fovy, KDfloat aspectRatio, KDfloat zNear, KDfloat zFar )
{
    if ( fovy <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Height is invalid, h = %f.", fovy );
    }

    if ( aspectRatio <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Aspect ratio is invalid, ratio = %f", aspectRatio );
    }

    if ( zNear == zFar ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Near and far are invalid, near = %f, far = %f.", zNear, zFar );
    }

    m_nType   = PARALLEL;
    m_fFovy   = fovy;
    m_fAspect = aspectRatio;
    m_fNear   = zNear;
    m_fFar    = zFar;

	M3GMatrix  proj = m3gMakeParallelProjection ( fovy, aspectRatio, zNear, zFar );
	m_tProjection.set ( proj.m );
}

KDvoid M3GCamera::setPerspective ( KDfloat fovy, KDfloat aspectRatio, KDfloat zNear, KDfloat zFar )
{
    if ( fovy <= 0 || fovy >= 180 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Fov is invalid, fov = %f.", fovy );
    }

    if ( aspectRatio <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Aspect ratio is invalid, ratio = %f", aspectRatio );
    }

    if ( zNear <= 0 || zFar <= 0 || zNear == zFar ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Near and far are invalid, near = %f, far = %f.", zNear, zFar );
    }

    m_nType   = PERSPECTIVE;
    m_fFovy   = fovy;
    m_fAspect = aspectRatio;
    m_fNear   = zNear;
    m_fFar    = zFar;

	M3GMatrix  proj = m3gMakePerspectiveProjection ( fovy, aspectRatio, zNear, zFar );
	m_tProjection.set ( proj.m );
}

KDvoid M3GCamera::render ( M3GRenderState& state ) const
{
	if ( state.m_nPass != 0 )
	{
		return;
	}

	glMatrixMode ( GL_PROJECTION );

	M3GMatrix  tProj = m_tProjection.getMatrix ( ).transpose ( );
	glMultMatrixf ( tProj.m );

	glMatrixMode ( GL_MODELVIEW );
	M3GMatrix  tModel = getGlobalPose ( KD_NULL ).invert ( ).transpose ( );

	glMultMatrixf ( tModel.m );
}

KDvoid M3GCamera::lookAt ( KDfloat fromX, KDfloat fromY, KDfloat fromZ,
						   KDfloat   toX, KDfloat   toY, KDfloat   toZ, 
						   KDfloat   upX, KDfloat   upY, KDfloat   upZ )
{
	if ( fromX == toX && fromY == toY && fromZ == toZ ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Viewing from ( %f, %f, %f ) - to ( %f, %f, %f ) is invalid.", fromX, fromY, fromZ, toX, toY, toZ );
	}

	if ( upX == 0 && upY == 0 && upZ == 0 ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Up ( %f, %f, %f ) vector is invalid.", upX, upY, upZ );
	}

	M3GVector  tFrom (         fromX,       fromY,       fromZ );
	M3GVector  tView (   toX - fromX, toY - fromY, toZ - fromZ );
	M3GVector  tUp   (           upX,         upY,         upZ );
	M3GVector  tRight = m3gCross ( tView, tUp );
	

	if ( tRight.length ( ) < KD_FLT_EPSILON )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Cross product of view and up vector is invalid." );
	}

	 tView.normalize ( );
	   tUp.normalize ( );
	tRight.normalize ( );

	tUp = m3gCross ( tRight, tView );

	KDfloat  m[16] = 
	{
		tRight.x, tUp.x, -tView.x, tFrom.x,
		tRight.y, tUp.y, -tView.y, tFrom.y,
		tRight.z, tUp.z, -tView.z, tFrom.z,
		       0,     0,        0,       1 
	};

	M3GTransform  tTransform;
	tTransform.set ( m );

	setTransform ( &tTransform );
}