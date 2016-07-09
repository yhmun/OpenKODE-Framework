/* --------------------------------------------------------------------------
 *
 *      File            SneakyJoystickSkinnedBase.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2010/02/18
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010	    Sneakyness, llc. All rights reserved.
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
#include "SneakyJoystickSkinnedBase.h"
#include "SneakyJoystick.h"

NS_CC_BEGIN

SneakyJoystickSkinnedBase* SneakyJoystickSkinnedBase::create ( KDvoid )
{
	SneakyJoystickSkinnedBase*	pRet = new SneakyJoystickSkinnedBase ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool SneakyJoystickSkinnedBase::init ( KDvoid )
{
	m_pBackgroundSprite = KD_NULL;
	m_pThumbSprite = KD_NULL;
	m_pJoystick = KD_NULL;

	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	this->schedule ( schedule_selector ( SneakyJoystickSkinnedBase::updatePositions ) );

	return KD_TRUE;
}

KDvoid SneakyJoystickSkinnedBase::setContentSize ( const CCSize& tContentSize )
{
	m_tContentSize = tContentSize;

	if ( m_pBackgroundSprite )
	{
		m_pBackgroundSprite->setContentSize ( tContentSize );
	}
}

KDvoid SneakyJoystickSkinnedBase::updatePositions ( KDfloat fDelta )
{
	if ( m_pJoystick && m_pThumbSprite )
	{
		m_pThumbSprite->setPosition ( m_pJoystick->getStickPosition ( ) );
	}
}

CCSprite* SneakyJoystickSkinnedBase::getBackgroundSprite ( KDvoid )
{
	return m_pBackgroundSprite;
}

KDvoid SneakyJoystickSkinnedBase::setBackgroundSprite ( CCSprite* pSprite )
{
	if ( m_pBackgroundSprite )
	{
		this->removeChild ( m_pBackgroundSprite, KD_TRUE );
	}

	if ( pSprite )
	{
		this->addChild ( pSprite, 0 );
		
		this->setContentSize ( pSprite->getContentSize ( ) );
	}

	m_pBackgroundSprite = pSprite;
}

CCSprite* SneakyJoystickSkinnedBase::getThumbSprite ( KDvoid )
{
	return m_pThumbSprite;
}

KDvoid SneakyJoystickSkinnedBase::setThumbSprite ( CCSprite* pSprite )
{
	if ( m_pThumbSprite )
	{
		this->removeChild ( m_pThumbSprite, KD_TRUE );
	}

	if ( pSprite )
	{
		this->addChild ( pSprite, 1 );
		
		if ( m_pJoystick )
		{
			m_pJoystick->setThumbRadius ( pSprite->getContentSize ( ).cx / 2 );
		}
	}

	m_pThumbSprite = pSprite;
}

SneakyJoystick* SneakyJoystickSkinnedBase::getJoystick ( KDvoid )
{
	return m_pJoystick;
}

KDvoid SneakyJoystickSkinnedBase::setJoystick ( SneakyJoystick* pJoystick )
{
	if ( m_pJoystick )
	{
		this->removeChild ( m_pJoystick, KD_TRUE );
	}

	if ( pJoystick )
	{
		this->addChild ( pJoystick, 2 );
		
		if ( m_pThumbSprite )
		{
			pJoystick->setThumbRadius ( m_pThumbSprite->boundingBox ( ).size.cx / 2 );
		}
		else
		{
			pJoystick->setThumbRadius ( 0 );
		}
		
		if ( m_pBackgroundSprite )
		{
			pJoystick->setJoystickRadius ( m_pBackgroundSprite->boundingBox ( ).size.cx / 2 );
		}
	}

	m_pJoystick = pJoystick;
}

NS_CC_END

