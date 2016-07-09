/* --------------------------------------------------------------------------
 *
 *      File            Enemy.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-11-03
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
#include "Enemy.h"

KDbool Enemy::init ( KDvoid )
{
	if ( !EnemyBase::initWithFile ( "Image 138 at frame 10.png" ) )
	{
		return KD_FALSE;
	}

	this->setScale  ( IOSSCALE );
	this->setActive ( KD_FALSE );

	return KD_TRUE;
}

KDvoid Enemy::active ( KDvoid )
{
	if ( this->isActive ( ) )
	{
		return;
	}
	this->setActive ( KD_TRUE );

    if ( this->getType ( ) == 0 ) 
	{
		CCArray*	pFrames = CCArray::create 
		(
			CCSpriteFrame::create ( "Image 138 at frame 10.png", CCRect ( 0, 0, 23.5f, 36 ) ),
			CCSpriteFrame::create ( "Image 139 at frame 10.png", CCRect ( 0, 0, 24, 35.5f ) ),
			KD_NULL
		);

		m_pActIdle = CCRepeatForever::create ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.2f ) ) );
		this->runAction ( m_pActIdle );
    }

	this->schedule ( schedule_selector ( Enemy::checkUpdate ), 1 / 20.f );
}

KDbool Enemy::beenAttack ( KDint nType )
{
	if ( !this->isActive ( ) )
	{
		return KD_FALSE;
	}

	if ( this->getType ( ) < 4 )
	{
		this->runDeadAnimation ( );
	}

    return KD_FALSE;
}

KDvoid Enemy::fire ( KDvoid )
{
	this->setFiring ( KD_TRUE );
}

KDvoid Enemy::runDeadAnimation ( KDvoid )
{
	CCArray*	pFrames = CCArray::create 
	(
		CCSpriteFrame::create ( "Image 231 at frame 10.png", CCRect ( 0, 0, 35.5f, 35 ) ),
		CCSpriteFrame::create ( "Image 232 at frame 10.png", CCRect ( 0, 0, 35.5f, 35 ) ),
		CCSpriteFrame::create ( "Image 233 at frame 10.png", CCRect ( 0, 0, 35.5f, 35 ) ),
		CCSpriteFrame::create ( "Image 234 at frame 10.png", CCRect ( 0, 0, 35.5f, 35 ) ),
		CCSpriteFrame::create ( "Image 235 at frame 10.png", CCRect ( 0, 0, 35.5f, 35 ) ),
		KD_NULL
	);

	this->runAction ( CCAnimate::create ( CCAnimation::createWithSpriteFrames ( pFrames, 0.2f ) ) );
}

KDvoid Enemy::checkUpdate ( KDfloat fDelta )
{
	if ( !this->isFiring ( ) )
	{

	}
}	
