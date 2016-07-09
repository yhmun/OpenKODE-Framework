/* --------------------------------------------------------------------------
 *
 *      File            GameWaypoint.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "GameWaypoint.h"

GameWaypoint* GameWaypoint::create ( const CCPoint& tPoint, KDfloat fSpeed )
{
	GameWaypoint*	pRet = new GameWaypoint ( );

	if ( pRet && pRet->initWithPosition ( tPoint, fSpeed ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

GameWaypoint::GameWaypoint ( KDvoid )
{
	m_pPreCallback	= KD_NULL;
	m_pPostCallback = KD_NULL;
}

GameWaypoint::~GameWaypoint ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pPreCallback  );
	CC_SAFE_RELEASE ( m_pPostCallback );
}

KDbool GameWaypoint::initWithPosition ( const CCPoint& tPoint, KDfloat fSpeed )
{
	m_tPosition		= tPoint;
	m_fSpeedMod		= fSpeed;
	m_fLastDistance = 1000000.0f;

	CC_SAFE_RELEASE ( m_pPreCallback  );
	CC_SAFE_RELEASE ( m_pPostCallback );

	return KD_TRUE;
}

KDvoid GameWaypoint::processPreCallback ( KDvoid )
{
	if ( !m_pPreCallback )
	{
		return;
	}

	CCObject*		pTarget   = m_pPreCallback->getObject   ( );
	SEL_CallFunc	pCallfunc = m_pPreCallback->getCallback ( );

	( pTarget->*pCallfunc ) ( );

	this->setPreCallback ( KD_NULL );
}

KDvoid GameWaypoint::processPostCallback ( KDvoid )
{
	if ( !m_pPostCallback )
	{
		return;
	}

	CCObject*		pTarget   = m_pPostCallback->getObject   ( );
	SEL_CallFunc	pCallfunc = m_pPostCallback->getCallback ( );

	( pTarget->*pCallfunc ) ( );
	
	this->setPostCallback ( KD_NULL );
}

const CCPoint& GameWaypoint::getPosition ( KDvoid )
{
	return m_tPosition;
}

KDvoid GameWaypoint::setPosition ( const CCPoint& tPoint )
{
	m_tPosition = tPoint;
}


KDfloat GameWaypoint::getSpeedMod ( KDvoid )
{
	return m_fSpeedMod;
}	

KDvoid GameWaypoint::setSpeedMod ( KDfloat fSpeedMod )
{
	m_fSpeedMod = fSpeedMod;
}

KDfloat GameWaypoint::getLastDistance ( KDvoid )
{
	return m_fLastDistance;
}

KDvoid GameWaypoint::setLastDistance ( KDfloat fLastDistance )
{
	m_fLastDistance = fLastDistance;
}

ObjectCallback* GameWaypoint::getPreCallback ( KDvoid )
{
	return m_pPreCallback;
}

KDvoid GameWaypoint::setPreCallback ( ObjectCallback* pCallback )
{
	CC_SAFE_RELEASE ( m_pPreCallback  );
	m_pPreCallback = pCallback;
	CC_SAFE_RETAIN  ( m_pPreCallback  );
}

ObjectCallback* GameWaypoint::getPostCallback ( KDvoid )
{
	return m_pPostCallback;
}

KDvoid GameWaypoint::setPostCallback ( ObjectCallback* pCallback )
{
	CC_SAFE_RELEASE ( m_pPostCallback );
	m_pPostCallback = pCallback;
	CC_SAFE_RETAIN  ( m_pPostCallback );
}