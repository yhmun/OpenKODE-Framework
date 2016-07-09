/* --------------------------------------------------------------------------
 *
 *      File            SneakyButton.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2010/02/10
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sneakyness, llc. All rights reserved.
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
#include "SneakyButton.h"

NS_CC_BEGIN

SneakyButton* SneakyButton::create ( const CCRect& tRect )
{
	SneakyButton*	pRet = new SneakyButton ( );

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

SneakyButton* SneakyButton::create ( KDvoid )
{
	SneakyButton*	pRet = new SneakyButton ( );

	if ( pRet && pRet->initWithRect ( CCRectZero ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SneakyButton::initWithRect ( const CCRect& tRect )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_tBounds		= CCRect ( 0, 0, tRect.size.cx, tRect.size.cy );		
	m_tCenter		= CCPoint ( tRect.size.cx / 2, tRect.size.cy / 2 );
	m_bStatus		= KD_TRUE;
	m_bActive		= KD_FALSE;
	m_bValue		= KD_FALSE;
	m_bIsHoldable	= KD_FALSE;
	m_bIsToggleable = KD_FALSE;
	m_fRadius		= 32.f;
	m_fRateLimit	= 1 / 120.f;

	this->setPosition ( tRect.origin );
		
	return KD_TRUE;
}

KDvoid SneakyButton::onEnterTransitionDidFinish ( KDvoid )
{
	CCNode::onEnterTransitionDidFinish ( );

	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, kCCMenuHandlerPriority, KD_TRUE );
}

KDvoid SneakyButton::onExit ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->removeDelegate ( this );

	CCNode::onExit ( );
}

KDvoid SneakyButton::limiter ( KDfloat fDelta )
{
	m_bValue = 0;
	this->unschedule ( schedule_selector ( SneakyButton::limiter ) );
	m_bActive = KD_FALSE;
}

KDfloat SneakyButton::getRadius ( KDvoid )
{
	return m_fRadius;
}

KDvoid SneakyButton::setRadius ( KDfloat fRadius )
{
	m_fRadius	= fRadius;
	m_fRadiusSq = fRadius * fRadius;
}

KDbool SneakyButton::getStatus ( KDvoid )
{
	return m_bStatus;
}

KDvoid SneakyButton::setStatus ( KDbool bStatus )
{
	m_bStatus = bStatus;
}

KDbool SneakyButton::getValue ( KDvoid )
{
	return m_bValue;
}

KDbool SneakyButton::isActive ( KDvoid )
{
	return m_bActive;
}

KDbool SneakyButton::isHoldable ( KDvoid )
{
	return m_bIsHoldable;
}

KDvoid SneakyButton::setHoldable ( KDbool bIsHoldable )
{
	m_bIsHoldable = bIsHoldable;
}

KDbool SneakyButton::isToggleable ( KDvoid )
{
	return m_bIsToggleable;
}

KDvoid SneakyButton::setToggleable ( KDbool bIsToggleable )
{
	m_bIsToggleable = bIsToggleable;
}

KDfloat SneakyButton::getRateLimit ( KDvoid )
{
	return m_fRateLimit;
}

KDvoid SneakyButton::setRateLimit ( KDfloat fRateLimit )
{
	m_fRateLimit = fRateLimit;
}

KDbool SneakyButton::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_bActive )
	{
		return KD_FALSE;
	}
	
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	// Do a fast rect check before doing a circle hit check:
	if ( tLocation.x < -m_fRadius || tLocation.x > m_fRadius || tLocation.y < -m_fRadius || tLocation.y > m_fRadius )
	{
		return KD_FALSE;
	}
	else
	{
		KDfloat		fSq = tLocation.x * tLocation.x + tLocation.y * tLocation.y;

		if ( m_fRadiusSq > fSq )
		{
			m_bActive = KD_TRUE;
			
			if ( !m_bIsHoldable && !m_bIsToggleable )
			{
				m_bValue = KD_TRUE;
				this->schedule ( schedule_selector ( SneakyButton::limiter ), m_fRateLimit );
			}
			
			if ( m_bIsHoldable )
			{
				m_bValue = KD_TRUE;
			}

			if ( m_bIsToggleable )
			{
				m_bValue = !m_bValue;
			}

			return KD_TRUE;
		}
	}	
	
	return KD_FALSE;
}

KDvoid SneakyButton::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !m_bActive )
	{
		return;
	}

	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );
	
	// Do a fast rect check before doing a circle hit check:
	if ( tLocation.x < -m_fRadius || tLocation.x > m_fRadius || tLocation.y < -m_fRadius || tLocation.y > m_fRadius )
	{
		return;
	}
	else
	{
		KDfloat		fSq = tLocation.x * tLocation.x + tLocation.y * tLocation.y;
		if ( m_fRadiusSq > fSq ) 
		{
			if ( m_bIsHoldable )
			{
				m_bValue = KD_TRUE;
			}
		}
		else
		{
			if ( m_bIsHoldable )
			{
				m_bValue  = KD_FALSE;
				m_bActive = KD_FALSE;
			}
		}
	}
}

KDvoid SneakyButton::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !m_bActive )
	{
		return;
	}

	if ( m_bIsHoldable )
	{
		m_bValue = KD_FALSE;
	}

	if ( m_bIsHoldable || m_bIsToggleable )
	{
		m_bActive = KD_FALSE;
	}
}

NS_CC_END