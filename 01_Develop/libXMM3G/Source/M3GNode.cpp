/* --------------------------------------------------------------------------
 *
 *      File            M3GNode.cpp
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
#include <XMM3G/M3GNode.h>
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>
#include <XMM3G/M3GTransform.h>

M3GNode::M3GNode ( KDvoid )
: m_pParent ( KD_NULL )
, m_pDuplicated ( KD_NULL )
, m_bRenderingEnable ( KD_TRUE )
, m_bPickingEnable ( KD_TRUE )
, m_fAlphaFactor ( 1 )
, m_nScope ( -1 )
, m_pZRef ( KD_NULL )
, m_pYRef ( KD_NULL )
, m_nZTarget ( NONE )
, m_nYTarget ( NONE )
{

}

M3GNode::~M3GNode ( KDvoid )
{

}

KDvoid M3GNode::addAnimationTrack ( M3GAnimationTrack* animationTrack )
{
    KDint  nProperty = animationTrack->getTargetProperty ( );

    if ( nProperty == M3GAnimationTrack::ALPHA ||
         nProperty == M3GAnimationTrack::PICKABILITY ||
         nProperty == M3GAnimationTrack::VISIBILITY )
	{
		this->_addAnimationTrack ( animationTrack );   
    }
	else
	{
		M3GTransformable::addAnimationTrack ( animationTrack );
	}
}

KDint M3GNode::animate ( KDint time )
{
    M3GTransformable::animate ( time );

    KDbool   isAlphaModefied           = KD_FALSE;
    KDbool   isPickingEnableModefied   = KD_FALSE;
    KDbool   isRenderingEnableModefied = KD_FALSE;

    KDfloat  fAlpha           = 0;
    KDbool   bPickingEnable   = KD_FALSE;
    KDbool   bRenderingEnable = KD_FALSE;
  
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

			case M3GAnimationTrack::PICKABILITY :
			{
				KDfloat  fValue [ 1 ] = { 1 };
				
				pKeyframe->sample ( (KDint) fSequence, fValue );

				bPickingEnable = bPickingEnable || ( ( fValue [ 0 ] != 0 ) ? KD_TRUE : KD_FALSE );
				
				isPickingEnableModefied = KD_TRUE;
				
				break;
			}

			case M3GAnimationTrack:: VISIBILITY :
			{
				KDfloat  fValue [ 1 ] = { 1 };
				
				pKeyframe->sample ( (KDint) fSequence, fValue );

				bRenderingEnable = bRenderingEnable || ( ( fValue [ 0 ] != 0 ) ? KD_TRUE : KD_FALSE );
				
				isRenderingEnableModefied = KD_TRUE;
	
				break;
			}

			default :
			{
				// Unknown target should be ignored.
				// animate() of Base class (of Derived class) retrieve it.
			}
        }
    }

    if ( isAlphaModefied )
	{
        m_fAlphaFactor = KD_CLAMP ( 0, 1, fAlpha );
    }

    if ( isPickingEnableModefied ) 
	{
        m_bPickingEnable = bPickingEnable;
    }

    if ( isRenderingEnableModefied ) 
	{
        m_bRenderingEnable = bRenderingEnable;
    }

    return 0;
}

M3GObject3D* M3GNode::duplicate ( KDvoid ) const
{
	M3GNode*  pObj = new M3GNode ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GNode::_duplicate ( M3GNode* pObj ) const
{
	M3GTransformable::_duplicate ( pObj );

    pObj->m_pParent          = KD_NULL;
    pObj->m_bRenderingEnable = m_bRenderingEnable;
    pObj->m_bPickingEnable   = m_bPickingEnable;
    pObj->m_fAlphaFactor     = m_fAlphaFactor;
    pObj->m_nScope           = m_nScope;
	pObj->m_pZRef			 = m_pZRef;
	pObj->m_pYRef			 = m_pYRef;
	pObj->m_nZTarget		 = m_nZTarget;
	pObj->m_nYTarget		 = m_nYTarget;

    const_cast<M3GNode*> ( this )->m_pDuplicated = pObj;
}

KDvoid M3GNode::align ( const M3GNode* reference )
{
	M3GMatrix  Rz, Ry;
	M3GVector  tz ( 0, 0, 1 );
	M3GVector  ty ( 0, 1, 0 );

	if ( m_nZTarget == ORIGIN ) 
	{
		const M3GNode*  zRef = ( m_pZRef != KD_NULL ) ? m_pZRef : reference;

		if ( zRef != KD_NULL ) 
		{
			M3GTransform  t;
			zRef->getCompositeTransform ( &t );

			KDfloat m[16];
			t.get ( m );
			tz = M3GVector ( m[2], m[6], m[10] ).normalize ( );

			M3GVector axisZ = m3gCross ( M3GVector ( 0, 0, 1 ), tz );
			axisZ = ( axisZ.length ( ) > 0 ) ? axisZ.normalize ( ) : M3GVector ( 0, 1, 0 );

			KDfloat  angleZ = kdAcosf ( m3gDot ( M3GVector ( 0, 0, 1), tz ) ) * 360 / ( 2 * KD_PI_F );
			Rz.setRotate ( angleZ, axisZ.x, axisZ.y, axisZ.z );
		}

		if ( m_nYTarget == ORIGIN )
		{
			const M3GNode*  yRef = ( m_pYRef != KD_NULL ) ? m_pYRef : reference;

			if ( yRef != KD_NULL )
			{
				M3GTransform  t;
				yRef->getCompositeTransform ( &t );

				KDfloat m[16];
				t.get ( m );
				ty = M3GVector ( m[1], m[5], m[9] ).normalize ( );

				M3GVector  ty2 = Rz.getInverse ( ) * ty;
				ty2[2] = 0;
				ty2 = ( ty2.length ( ) > 0 ) ? ty2.normalize ( ) : M3GVector ( 0, 1, 0 );

				M3GVector  axisY  = ( ty2[0] > 0 ) ? tz * -1 : tz;
				axisY = ( axisY.length ( ) > 0 ) ? axisY.normalize ( ) : tz;

				KDfloat  angleY = kdAcosf ( m3gDot ( M3GVector ( 0, 1, 0 ), ty2 ) ) * 360 / ( 2 * KD_PI_F );
				Ry.setRotate ( angleY, axisY.x, axisY.y, axisY.z );
			}
		}
	} 
	else if ( m_nYTarget == ORIGIN ) 
	{
		const M3GNode*  yRef = ( m_pYRef != KD_NULL ) ? m_pYRef : reference;

		if ( yRef != KD_NULL )
		{
			M3GTransform  t;
			yRef->getCompositeTransform ( &t );

			KDfloat  m[16];
			t.get ( m );
			ty = M3GVector ( m[1], m[5], m[9] ).normalize ( );

			M3GVector  axisY = m3gCross ( M3GVector ( 0, 1, 0 ), ty );
			axisY = ( axisY.length ( ) > 0 ) ? axisY.normalize ( ) : M3GVector ( 0, 0, 1 );

			KDfloat  angleY = kdAcosf ( m3gDot ( M3GVector ( 0, 1, 0 ), ty ) ) * 360 / ( 2 * KD_PI_F );
			Ry.setRotate ( angleY, axisY.x, axisY.y, axisY.z );
		}
	}

	if ( m_nZTarget == ORIGIN || m_nYTarget == ORIGIN ) 
	{
		M3GQuaternion  A = m3gMatrix2quat ( Ry * Rz );
		KDfloat  angleAxis[4];
		A.getAngleAxis ( angleAxis );
		setOrientation ( angleAxis[0], angleAxis[1], angleAxis[2], angleAxis[3] );

	}
}

M3GNode* M3GNode::getAlignmentReference ( KDint axis ) const
{
	M3GNode*  pNode = KD_NULL;

    switch ( axis ) 
	{
		case Z_AXIS : pNode = m_pZRef; break;
		case Y_AXIS : pNode = m_pYRef; break;

		default : 
			
			M3GException ( "IllegalArgumentException", __FUNCTION__, "Axis is invalid, axis = %d.", axis );
    }

	return pNode;
}

KDint M3GNode::getAlignmentTarget ( KDint axis ) const
{
	KDint  nTarget = 0;

    switch ( axis ) 
	{
		case Z_AXIS : nTarget = m_nZTarget; break;
		case Y_AXIS : nTarget = m_nYTarget; break;

		default : 
			
			M3GException ( "IllegalArgumentException", __FUNCTION__, "Axis is invalid, axis = %d.", axis );
    }

    return nTarget;
}

KDfloat M3GNode::getAlphaFactor ( KDvoid ) const
{
	return m_fAlphaFactor;
}

M3GNode* M3GNode::getParent ( KDvoid ) const
{
	return m_pParent;
}

KDint M3GNode::getScope ( KDvoid ) const
{
	return m_nScope;
}

KDbool M3GNode::getTransformTo ( const M3GNode* target, M3GTransform* transform ) const
{
    if ( target == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Target is NULL." );
    }

    if ( transform == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Transform is NULL." );
    }

    const M3GNode *pRootA, *pRootB, *pNode;
  
    pNode = this;
    do 
	{
        pRootA = pNode;
    } while ( ( pNode = pNode->m_pParent ) );

    pNode = target;
    do 
	{
        pRootB = pNode;
    } while ( ( pNode = pNode->m_pParent ) );

    if ( pRootA != pRootB ) 
	{
         return KD_FALSE;
    }
 
    M3GMatrix  globalPoseA = this->getGlobalPose ( pRootA );
    M3GMatrix  globalPoseB = target->getGlobalPose ( pRootB );
    M3GMatrix  transformMatrix = globalPoseB.getInverse ( ) * globalPoseA;

    if ( kdFabsf ( transformMatrix.m[15] - 1 ) < KD_FLT_EPSILON )
	{
        transformMatrix.m[15] = 1;
    }

    transform->set ( transformMatrix.m );

    return KD_TRUE;
}

KDbool M3GNode::isPickingEnabled ( KDvoid ) const
{
	return m_bPickingEnable;
}

KDbool M3GNode::isRenderingEnabled ( KDvoid ) const
{
	return m_bRenderingEnable;
}

KDvoid M3GNode::setAlignment ( M3GNode* zRef, KDint zTarget, M3GNode* yRef, KDint yTarget )
{
    if ( !( zTarget == NONE || zTarget == ORIGIN ) ||
         !( yTarget == NONE || yTarget == ORIGIN ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Invalid target zTarget = %d, yTarget = %d.", zTarget, yTarget );
    }

    if ( zRef == this || yRef == this )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Reference is invalid, zRef = %p, yRef = %p, this = %p.", zRef, yRef, this );
    }

	m_pZRef = zRef;
	m_pYRef = yRef;

	m_nZTarget = zTarget;
	m_nYTarget = yTarget;
}

KDvoid M3GNode::setAlphaFactor ( KDfloat alphaFactor )
{
    if ( alphaFactor < 0 || alphaFactor > 1.f )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Alpha factor is invalid, alpha = %f.", alphaFactor );
    }

    m_fAlphaFactor = alphaFactor;
}

KDvoid M3GNode::setPickingEnable ( KDbool enable )
{
	m_bPickingEnable = enable;
}

KDvoid M3GNode::setRenderingEnable ( KDbool enable )
{
	m_bRenderingEnable = enable;
}

KDvoid M3GNode::setScope ( KDint scope )
{
	m_nScope = scope;
}

KDvoid M3GNode::render ( M3GRenderState& state ) const
{
	if ( !isGlobalRenderingEnabled ( ) ) 
	{
		return;
	}

	M3GTransformable::render ( state );

	state.m_fAlpha = getGlobalAlphaFactor ( );
}

KDfloat M3GNode::getGlobalAlphaFactor ( KDvoid ) const
{
    const M3GNode*  pNode = this;
    KDfloat  fAlpha = 1;

    do 
	{
        fAlpha *= pNode->getAlphaFactor ( );

    } while ( ( pNode = pNode->getParent ( ) ) != KD_NULL );

    return fAlpha;
}

M3GMatrix M3GNode::getGlobalPose ( const M3GNode* target ) const
{
    M3GMatrix  globalPose;
    if ( this == target ) 
	{
        return globalPose;
    }

	const M3GNode*  pNode = this;
    do
	{
        if ( pNode == KD_NULL ) 
		{
			M3GException ( "InternalException", __FUNCTION__, "Parent node is NULL, but not found target node." );
        }

        M3GTransform  trans;
        pNode->getCompositeTransform ( &trans );

        KDfloat m[16];
        trans.get ( m );

        globalPose = M3GMatrix ( m ) * globalPose;

    } while ( ( pNode = pNode->getParent ( ) ) != target );

	return globalPose;
}

M3GNode* M3GNode::getGlobalParent ( KDvoid ) const
{
    M3GNode*  pNode = const_cast<M3GNode*> ( this );

    while ( pNode->getParent ( ) != KD_NULL )
	{
        pNode = pNode->getParent ( );
	}

    return pNode;
}

KDbool M3GNode::isGlobalPickingEnabled ( KDvoid ) const
{
    const M3GNode*  pNode = this;
    
	do 
	{
        if ( !pNode->isPickingEnabled ( ) )
		{
            return KD_FALSE;
		}

    } while ( ( pNode = pNode->getParent ( ) ) != KD_NULL );

    return KD_TRUE;
}

KDbool M3GNode::isGlobalRenderingEnabled ( KDvoid ) const
{
    const M3GNode*  pNode = this;
   
	do 
	{
        if ( !pNode->isRenderingEnabled ( ) )
		{
            return KD_FALSE;
		}

    } while ( ( pNode = pNode->getParent ( ) ) != KD_NULL );

    return KD_TRUE;
}

KDvoid M3GNode::setParent ( M3GNode* node )
{
	m_pParent = node;
}