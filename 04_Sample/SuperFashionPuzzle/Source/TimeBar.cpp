/* --------------------------------------------------------------------------
 *
 *      File            TimeBar.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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
#include "TimeBar.h"
#include "PlayingScene.h"

TimeBar* TimeBar::create ( PlayingScene* pPlayingScene )
{
	TimeBar*	pRet = new TimeBar ( );

	if ( pRet && pRet->initWithPlayingScene ( pPlayingScene ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool TimeBar::initWithPlayingScene ( PlayingScene* pPlayingScene )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	// background
	CCSprite*		pBackground = CCSprite::create ( "time_bar_background.png" );
	this->addChild ( pBackground );

	// time bar
	m_pTimeBarSprite = CCSprite::create ( "time_bar.png" );
	this->addChild ( m_pTimeBarSprite );

	// foreground
	CCSprite*		pForeground = CCSprite::create ( "time_foreground.png" );
	this->addChild ( pForeground );

	// setup time and time bar sprite
	m_fDuration = TIMEBAR_DURATION_IN_EASY_MODE;
	m_fLeftTime = m_fDuration;
	m_pPlayingScene = pPlayingScene;

	this->scheduleUpdate ( );

	return KD_TRUE;
}

// m_timeBar position is in relationship with m_timeLeft and MAX_TIME
// When there is no time left, timeUp method s called in PlayingScene object
KDvoid TimeBar::update ( KDfloat fDelta )
{
	if ( !m_bIsActivated )
	{
		return;	
	}

	m_fLeftTime = m_fLeftTime - fDelta;
	if ( m_fLeftTime < 0 )
	{
		m_fLeftTime = 0;
	}

	// factor 1 -> posy=0 (you see a full bar)
	// factor 0 -> posy=-320 (time bar is down, not showed)
	KDfloat		fFactor = m_fLeftTime / m_fDuration;
	m_pTimeBarSprite->setPosition ( ccp ( 0, fFactor * 320 - 320 ) );

	if ( m_fLeftTime == 0 )
	{
		m_bIsActivated = KD_FALSE;

		// notify to PlayingGame
		m_pPlayingScene->timeUp ( );
	}
}

KDvoid TimeBar::activate ( KDvoid )
{
	m_bIsActivated = KD_TRUE;
}

KDvoid TimeBar::deactivate ( KDvoid )
{
	m_bIsActivated = KD_FALSE;
}

// called when user selects "play again"
KDvoid TimeBar::reset ( KDvoid )
{
	m_bIsActivated = KD_FALSE;
	m_fLeftTime = TIMEBAR_DURATION_IN_EASY_MODE;
	m_pTimeBarSprite->setPosition ( ccp ( 0, 0 ) );
}

KDvoid TimeBar::addSomeTime ( KDfloat fAmount )
{
	m_fLeftTime = m_fLeftTime + fAmount;
	if ( m_fLeftTime > m_fDuration )
	{
		m_fLeftTime = m_fDuration;
	}
}

KDvoid TimeBar::removeSomeTime ( KDfloat fAmount )
{
	m_fLeftTime = m_fLeftTime - fAmount;
	if ( m_fLeftTime < 0 )
	{
		m_fLeftTime = 0;
	}
}

KDvoid TimeBar::setDuration ( KDfloat fDuration )
{
	m_fDuration = fDuration;
	if ( m_fLeftTime > m_fDuration ) 
	{
		m_fLeftTime = m_fDuration;
	}
}
