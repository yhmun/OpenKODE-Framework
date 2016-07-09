/* --------------------------------------------------------------------------
 *
 *      File            IntroLayer.cpp    
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "IntroLayer.h"
#include "TheLevel.h"
#include "GameSounds.h"

KDbool IntroLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*	pBackground = CCSprite::create ( "IntroLayer.png" );
	pBackground->setPosition ( ccpMid ( m_tContentSize ) );
	this->addChild ( pBackground );

	this->scheduleOnce ( schedule_selector ( IntroLayer::makeTransition ), 0.5 );

	return KD_TRUE;
}

KDvoid IntroLayer::makeTransition ( KDfloat fDelta )
{
	CCLOG ( "Make Transition to Level" );

	GameSounds::sharedGameSounds ( )->preloadSounds ( );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1, TheLevel::scene ( ), ccWHITE ) );
}

