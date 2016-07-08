/* --------------------------------------------------------------------------
 *
 *      File            M3GAnimationController.cpp
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
#include <XMM3G/M3GAnimationController.h>

M3GAnimationController::M3GAnimationController ( KDvoid )
: m_fActiveIntervalStart ( 0 )
, m_fActiveIntervalEnd ( 0 )
, m_fBlendingWeight ( 1 )
, m_fSpeed ( 1 )
, m_fRefWorldTime ( 0 )
, m_fRefSequenceTime ( 0 ) 
{

}

M3GAnimationController::~M3GAnimationController ( KDvoid )
{

}

M3GObject3D* M3GAnimationController::duplicate ( KDvoid ) const
{
	M3GAnimationController*  pObj = new M3GAnimationController ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GAnimationController::_duplicate ( M3GAnimationController* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_fActiveIntervalStart = m_fActiveIntervalStart;
    pObj->m_fActiveIntervalEnd   = m_fActiveIntervalEnd;
    pObj->m_fBlendingWeight		 = m_fBlendingWeight;
    pObj->m_fSpeed				 = m_fSpeed;
    pObj->m_fRefWorldTime		 = m_fRefWorldTime;
    pObj->m_fRefSequenceTime	 = m_fRefSequenceTime;
}

KDint M3GAnimationController::getActiveIntervalEnd ( KDvoid ) const
{
	return (KDint) m_fActiveIntervalEnd;
}

KDint M3GAnimationController::getActiveIntervalStart ( KDvoid ) const
{
	return (KDint) m_fActiveIntervalStart;
}

KDfloat M3GAnimationController::getPosition ( KDint worldTime ) const
{
	return m_fRefSequenceTime + m_fSpeed * ( worldTime - m_fRefWorldTime );
}

KDint M3GAnimationController::getRefWorldTime ( KDvoid ) const
{
	return (KDint) m_fRefWorldTime;
}

KDfloat M3GAnimationController::getSpeed ( KDvoid ) const
{
	return m_fSpeed;
}

KDfloat M3GAnimationController::getWeight ( KDvoid ) const
{
	return m_fBlendingWeight;
}

KDvoid M3GAnimationController::setActiveInterval ( KDint start, KDint end )
{
	m_fActiveIntervalStart = (KDfloat) start;
	m_fActiveIntervalEnd   = (KDfloat) end;
}

KDvoid M3GAnimationController::setPosition ( KDfloat sequenceTime, KDint worldTime )
{
	m_fRefSequenceTime = sequenceTime;
	m_fRefWorldTime = (KDfloat) worldTime;
}

KDvoid M3GAnimationController::setSpeed ( KDfloat speed, KDint worldTime )
{
    m_fRefSequenceTime  = getPosition ( worldTime );
    m_fRefWorldTime     = (KDfloat) worldTime;
    m_fSpeed			= speed;
}

KDvoid M3GAnimationController::setWeight ( KDfloat weight )
{
	m_fBlendingWeight = weight;
}

KDbool M3GAnimationController::isActive ( KDint worldTime ) const
{
    if ( m_fBlendingWeight == 0 )
	{
        return KD_FALSE;
	}
    else if ( m_fActiveIntervalStart == m_fActiveIntervalEnd )
	{
        return KD_TRUE;
	}
    else if ( worldTime >= m_fActiveIntervalStart && worldTime < m_fActiveIntervalEnd )
	{
        return KD_TRUE;
	}
    else
	{
        return KD_FALSE;
	}
}