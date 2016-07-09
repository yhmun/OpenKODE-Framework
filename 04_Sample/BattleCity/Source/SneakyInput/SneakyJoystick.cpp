/* --------------------------------------------------------------------------
 *
 *      File            SneakyJoystick.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2009/02/15
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010	Sneakyness, llc. All rights reserved.
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
#include "SneakyJoystick.h"

NS_CC_BEGIN

#define SJ_RAD2DEG			180.f / KD_PI_F
#define SJ_DEG2RAD			KD_PI_F / 180.f

SneakyJoystick*	SneakyJoystick::create ( const CCRect& tRect )
{
	SneakyJoystick*	pRet = new SneakyJoystick ( );

	if ( pRet && pRet->initWithRect ( tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SneakyJoystick::initWithRect ( const CCRect& tRect )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_tStickPosition		= ccsz;
	m_fDegrees				= 0.0f;
	m_tVelocity				= ccsz;
	m_bAutoCenter			= KD_TRUE;
	m_bIsDPad				= KD_FALSE;
	m_bHasDeadzone			= KD_FALSE;
	m_uNumberOfDirections	= 4;

	this->setJoystickRadius ( tRect.size.cx / 2 );
	this->setThumbRadius ( 32.0f );
	this->setDeadRadius ( 0.0f );

	this->setPosition ( tRect.origin );
		
	return KD_TRUE;
}

KDvoid SneakyJoystick::onEnterTransitionDidFinish ( KDvoid )
{
	CCNode::onEnterTransitionDidFinish ( );

	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, kCCMenuHandlerPriority, KD_TRUE );
}

KDvoid SneakyJoystick::onExit ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );

	CCNode::onExit ( );
}

KDbool SneakyJoystick::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	// Do a fast rect check before doing a circle hit check:
	if ( tLocation.x < -m_fJoystickRadius || tLocation.x > m_fJoystickRadius || tLocation.y < -m_fJoystickRadius || tLocation.y > m_fJoystickRadius )
	{
		return KD_FALSE;
	}
	else
	{
		KDfloat		fSq = tLocation.x * tLocation.x + tLocation.y * tLocation.y;
		if ( m_fJoystickRadiusSq > fSq ) 
		{
			this->updateVelocity ( tLocation );
			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

KDvoid SneakyJoystick::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	this->updateVelocity ( tLocation );
}

KDvoid SneakyJoystick::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tLocation = ccsz;

	if ( !m_bAutoCenter )
	{
		tLocation = this->convertTouchToNodeSpace ( pTouch );
	}

	this->updateVelocity ( tLocation );
}

const CCPoint& SneakyJoystick::getStickPosition ( KDvoid )
{
	return m_tStickPosition;
}

KDfloat SneakyJoystick::getDegrees ( KDvoid )
{
	return m_fDegrees;
}

const CCPoint& SneakyJoystick::getVelocity ( KDvoid )
{
	return m_tVelocity;
}

KDbool SneakyJoystick::isAutoCenter ( KDvoid )
{
	return m_bAutoCenter;
}

KDvoid SneakyJoystick::setAutoCenter ( KDbool bAutoCenter )
{
	m_bAutoCenter = bAutoCenter;
}

KDbool SneakyJoystick::isDPad ( KDvoid )
{
	return m_bIsDPad;
}

KDvoid SneakyJoystick::setDPad ( KDbool bIsDPad )
{
	m_bIsDPad = bIsDPad;
	if( bIsDPad )
	{
		m_bHasDeadzone = KD_TRUE;
		this->setDeadRadius ( 10.0f );
	}
}

KDbool SneakyJoystick::hasDeadZone ( KDvoid )
{
	return m_bHasDeadzone;
}

KDvoid SneakyJoystick::setDeadZone ( KDbool bHasDeadZone )
{
	m_bHasDeadzone = bHasDeadZone;
}

KDuint SneakyJoystick::getNumberOfDirections ( KDvoid )
{
	return m_uNumberOfDirections;
}

KDvoid SneakyJoystick::setNumberOfDirections ( KDuint uNumberOfDirections )
{
	m_uNumberOfDirections = uNumberOfDirections;
}

KDfloat SneakyJoystick::getJoystickRadius ( KDvoid )
{
	return m_fJoystickRadius;
}

KDvoid SneakyJoystick::setJoystickRadius ( KDfloat fRadius )
{
	m_fJoystickRadius = fRadius;
	m_fJoystickRadiusSq = fRadius * fRadius;
}

KDfloat SneakyJoystick::getThumbRadius ( KDvoid )
{
	return m_fThumbRadius;
}

KDvoid SneakyJoystick::setThumbRadius ( KDfloat fRadius )
{
	m_fThumbRadius = fRadius;
	m_fThumbRadiusSq = fRadius * fRadius;	
}

KDfloat SneakyJoystick::getDeadRadius ( KDvoid )
{
	return m_fDeadRadius;
}

KDvoid SneakyJoystick::setDeadRadius ( KDfloat fRadius )
{
	m_fDeadRadius = fRadius;
	m_fDeadRadiusSq = fRadius * fRadius;
}

KDvoid SneakyJoystick::updateVelocity ( const CCPoint& tPoint )
{
	// Calculate distance and angle from the center.
	KDfloat		fX = tPoint.x;
	KDfloat		fY = tPoint.y;
	KDfloat		fSq = fX * fX + fY * fY;
	
	if ( fSq <= m_fDeadRadiusSq )
	{
		m_tVelocity = ccsz;
		m_fDegrees = 0.0f;
		m_tStickPosition = tPoint;
		return;
	}
	
	KDfloat		fAngle = kdAtan2f ( fY, fX ); 
	if ( fAngle < 0 )
	{
		fAngle += KD_2PI_F;
	}

	KDfloat		fCosAngle;
	KDfloat		fSinAngle;

	if ( m_bIsDPad )
	{
		KDfloat		fAnglePerSector = 360.0f / m_uNumberOfDirections * SJ_DEG2RAD;
		fAngle = kdRoundf ( fAngle / fAnglePerSector ) * fAnglePerSector;
	}

	fCosAngle = kdCosf ( fAngle );
	fSinAngle = kdSinf ( fAngle );

	// NOTE: Velocity goes from -1.0 to 1.0.
	if ( fSq > m_fJoystickRadiusSq || m_bIsDPad )
	{
		fX = fCosAngle * m_fJoystickRadius;
		fY = fSinAngle * m_fJoystickRadius;
	}
	
	m_tVelocity = CCPoint ( fX / m_fJoystickRadius, fY / m_fJoystickRadius );
	m_fDegrees = fAngle * SJ_RAD2DEG;
	
	// Update the thumb's position	
	m_tStickPosition = ccp ( fX, fY );
}

NS_CC_END