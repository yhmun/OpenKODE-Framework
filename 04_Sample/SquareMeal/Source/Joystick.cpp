/* --------------------------------------------------------------------------
 *
 *      File            Joystick.cpp
 *
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "Joystick.h"

Joystick* Joystick::createWithCenter ( const CCPoint& tCenter, KDfloat fRadius, const KDchar* szJoystick, const KDchar* szStickBg, KDbool bIsFollowRole )
{
	Joystick*	pRet = new Joystick ( );

	if ( pRet && pRet->initWithCenter ( tCenter, fRadius, szJoystick, szStickBg, bIsFollowRole ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

KDvoid Joystick::active ( KDvoid )
{
	if ( !m_bActive )
	{
		m_bActive = KD_TRUE;
		schedule ( schedule_selector ( Joystick::updatePos ) );
		CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_FALSE );
	}
}

KDvoid Joystick::inactive ( KDvoid )
{
	if ( m_bActive )
	{
		m_bActive = KD_FALSE;
		this->unschedule ( schedule_selector ( Joystick::updatePos ) );
		CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );
	}
}

CCPoint Joystick::getDirection ( KDvoid )
{
	CCPoint		tPostion = ccpSub ( m_tCenterPoint, m_tCurrentPoint );
	if ( tPostion.x == 0.0f && tPostion.y == 0.0f )
	{
		return CCPointZero;
	}
	else
	{
		return ccpNormalize ( tPostion );
	}
}

KDfloat Joystick::getVelocity ( KDvoid )
{
	return ccpDistance ( m_tCenterPoint, m_tCurrentPoint );
}

KDfloat Joystick::getRadius ( KDvoid ) 
{
	return m_fRadius;
}

KDvoid Joystick::updatePos ( KDfloat fDelta )
{
	m_pSprite->setPosition ( ccpAdd ( m_pSprite->getPosition ( ),ccpMult ( ccpSub ( m_tCurrentPoint, m_pSprite->getPosition ( ) ),0.5 ) ) );
}

KDvoid Joystick::setJostickOpacity ( GLubyte cOpacity )
{
	if ( m_bActive )
	{
		m_pSprite->setOpacity ( cOpacity );
	}
}

KDvoid Joystick::setJoystickBackgroundOpacity ( GLubyte cOpacity )
{
	if ( m_bActive )
	{
		m_pBgSprite->setOpacity ( cOpacity );
	}
}

KDbool Joystick::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !m_bActive )
	{
		return KD_FALSE;
	}

	this->setVisible ( KD_TRUE );

	CCPoint		tTouchPoint = pTouch->getLocation ( );
	if ( !m_bIsFollowRole )
	{
		if ( ccpDistance ( tTouchPoint, m_tCenterPoint ) > m_fRadius )
		{
			return KD_FALSE;
		}
	}
	m_tCurrentPoint = tTouchPoint;
	if ( m_bIsFollowRole )
	{
		m_tCenterPoint = m_tCurrentPoint;
		m_pSprite->setPosition ( m_tCurrentPoint );
		m_pBgSprite->setPosition ( m_tCurrentPoint );
	}

	return KD_TRUE;
}

KDvoid  Joystick::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tTouchPoint = pTouch->getLocation ( );
	if ( ccpDistance ( tTouchPoint, m_tCenterPoint ) > m_fRadius )
	{
		m_tCurrentPoint = ccpAdd ( m_tCenterPoint,ccpMult ( ccpNormalize ( ccpSub ( tTouchPoint, m_tCenterPoint ) ), m_fRadius ) );
	}
	else 
	{
		m_tCurrentPoint = tTouchPoint;
	}
}

KDvoid Joystick::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_tCurrentPoint = m_tCenterPoint;
	if ( m_bIsFollowRole )
	{
		this->setVisible ( false );
	}
}

KDbool Joystick::initWithCenter ( const CCPoint& tCenter, KDfloat fRadius, const KDchar* szJoystick, const KDchar* szStickBg, KDbool bIsFollowRole )
{
	m_bIsFollowRole = bIsFollowRole;
	m_bActive = KD_FALSE;
	m_fRadius = fRadius;
	if ( !m_bIsFollowRole )
	{
		m_tCenterPoint = tCenter;
	}
	else
	{
		m_tCenterPoint = CCPointZero;
	}
	m_tCurrentPoint = m_tCenterPoint;
	m_pSprite = CCSprite::create ( szJoystick );
	m_pSprite->setPosition ( m_tCenterPoint );

	m_pBgSprite = CCSprite::create ( szStickBg );
	m_pBgSprite->setPosition ( m_tCenterPoint );
	m_pBgSprite->setTag ( 88 );
	
	this->addChild ( m_pBgSprite );
	this->addChild ( m_pSprite );

	if ( bIsFollowRole )
	{
		this->setVisible ( KD_FALSE );
	}
	this->active ( );

	return KD_TRUE;
}