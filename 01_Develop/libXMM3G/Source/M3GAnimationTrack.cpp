/* --------------------------------------------------------------------------
 *
 *      File            M3GAnimationTrack.cpp
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
#include <XMM3G/M3GAnimationTrack.h>
#include <XMM3G/M3GKeyframeSequence.h>
#include <XMM3G/M3GAnimationController.h>

static KDbool m3gIsPropertyHasValidComponent ( KDint _property, KDint componentCount )
{
	KDbool  bRet = KD_FALSE;

	switch ( _property )
	{
		case M3GAnimationTrack::ALPHA			:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::AMBIENT_COLOR	:	bRet = ( componentCount == 3 );		break;
		case M3GAnimationTrack::COLOR			:	bRet = ( componentCount == 3 );		break;
		case M3GAnimationTrack::CROP			:	bRet = ( componentCount == 2 || componentCount == 4 );	break;
		case M3GAnimationTrack::DENSITY			:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::DIFFUSE_COLOR	:	bRet = ( componentCount == 3 );		break;
		case M3GAnimationTrack::EMISSIVE_COLOR	:	bRet = ( componentCount == 3 );		break;
		case M3GAnimationTrack::FAR_DISTANCE	:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::FIELD_OF_VIEW	:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::INTENSITY		:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::MORPH_WEIGHTS	:	bRet = ( componentCount >= 1 );		break;
		case M3GAnimationTrack::NEAR_DISTANCE	:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::ORIENTATION		:	bRet = ( componentCount == 4 );		break;
		case M3GAnimationTrack::PICKABILITY		:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::SCALE			:	bRet = ( componentCount == 1 || componentCount == 3 );	break;
		case M3GAnimationTrack::SHININESS		:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::SPECULAR_COLOR	:	bRet = ( componentCount == 3 );		break;
		case M3GAnimationTrack::SPOT_ANGLE		:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::SPOT_EXPONENT	:	bRet = ( componentCount == 1 );		break;
		case M3GAnimationTrack::TRANSLATION		:	bRet = ( componentCount == 3 );		break;
		case M3GAnimationTrack::VISIBILITY		:	bRet = ( componentCount == 1 );		break;

		default : 
			
			M3GException ( "InternalException", __FUNCTION__, "Unknown property, property = %d.", _property );
	}

	return bRet;
}

M3GAnimationTrack::M3GAnimationTrack ( KDvoid )
: m_nProperty ( 0 )
, m_pKeyframeSequence ( KD_NULL )
, m_pAnimationController ( KD_NULL )
{

}

M3GAnimationTrack::M3GAnimationTrack ( M3GKeyframeSequence* sequence, KDint _property )
: m_nProperty ( 0 )
, m_pKeyframeSequence ( KD_NULL )
, m_pAnimationController ( KD_NULL )
{
	if ( sequence == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "KeyframeSequence is NULL." );
	}

	if ( _property < ALPHA || _property > VISIBILITY ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Property is unkwon, property = %d.", _property );
	}

	if ( !m3gIsPropertyHasValidComponent ( _property, sequence->getComponentCount ( ) ) )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Component count of KeyframeSequence is invalid for this property, count = %d, property = %d", sequence->getComponentCount ( ), _property );
	}

	this->init ( sequence, _property );
}

M3GAnimationTrack::~M3GAnimationTrack ( KDvoid )
{

}

KDvoid M3GAnimationTrack::init ( M3GKeyframeSequence* sequence, KDint _property )
{
	m_nProperty = _property;
	m_pKeyframeSequence = sequence;
}

M3GObject3D* M3GAnimationTrack::duplicate ( KDvoid ) const
{
	M3GAnimationTrack*  pObj = new M3GAnimationTrack ( m_pKeyframeSequence, m_nProperty );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GAnimationTrack::_duplicate ( M3GAnimationTrack* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_pAnimationController = m_pAnimationController;
}

KDint M3GAnimationTrack::getReferences ( M3GObject3D** references ) const
{
    KDint  nNumber = M3GObject3D::getReferences ( references );

    if ( m_pKeyframeSequence )
	{
		if ( references )
		{
			references [ nNumber ] = m_pKeyframeSequence;
		}

		nNumber++;
	}

    if ( m_pAnimationController )
	{
		if ( references )
		{
			references [ nNumber ] = m_pAnimationController;
		}

		nNumber++;
	}

	return nNumber;
}

M3GAnimationController* M3GAnimationTrack::getController ( KDvoid ) const
{
	return m_pAnimationController;
}

M3GKeyframeSequence* M3GAnimationTrack::getKeyframeSequence ( KDvoid ) const
{
	return m_pKeyframeSequence;
}

KDint M3GAnimationTrack::getTargetProperty ( KDvoid ) const
{
	return m_nProperty;
}

KDvoid M3GAnimationTrack::setController ( M3GAnimationController* controller )
{
	m_pAnimationController = controller;
}