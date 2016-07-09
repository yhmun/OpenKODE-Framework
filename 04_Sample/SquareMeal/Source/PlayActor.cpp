/* --------------------------------------------------------------------------
 *
 *      File            PlayActor.cpp
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
#include "PlayActor.h"
#include "Global.h"

static PlayActor*		l_pSharedPlayActor = KD_NULL;

PlayActor::PlayActor ( KDvoid )
{
	m_pJoystick	= KD_NULL;
	m_pActor	= KD_NULL;
	m_pEnemies	= KD_NULL;
}

PlayActor::~PlayActor ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pJoystick );
	CC_SAFE_RELEASE ( m_pActor );
	CC_SAFE_RELEASE ( m_pEnemies );

	CC_SAFE_RELEASE ( l_pSharedPlayActor );
}

PlayActor* PlayActor::sharedPlayActor ( KDvoid )
{
	if ( !l_pSharedPlayActor )
	{
		l_pSharedPlayActor = new PlayActor ( );
		l_pSharedPlayActor->init ( );
	}

	return l_pSharedPlayActor;
}

KDbool PlayActor::init ( KDvoid )
{
	m_pJoystick = Joystick::createWithCenter ( ccp ( 40.0f, 40.0f ), 40.0f, s_JoystickPath, s_JoystickBgPath, KD_TRUE );
	m_pJoystick->retain ( );

	m_pActor = Monster::create ( );
	m_pActor->retain ( );

	m_pEnemies = CCArray::create ( );
	m_pEnemies->retain ( );

	return KD_TRUE;
}

Enemy* PlayActor::createEnemy ( KDint nType )
{
	return Enemy::create ( );
}

Joystick* PlayActor::getJoyStick ( KDvoid )
{
	return m_pJoystick;
}

Monster* PlayActor::getMonster ( KDvoid )
{
	return m_pActor;
}

CCArray* PlayActor::getEnemies ( KDvoid )
{
	return m_pEnemies;
}
