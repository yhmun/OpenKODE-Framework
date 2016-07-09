/* --------------------------------------------------------------------------
 *
 *      File            Ninja.cpp    
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
#include "Ninja.h"

Ninja* Ninja::create ( b2World* pWorld, const CCPoint& tLocation, const KDchar* szBaseFileName )
{
	Ninja*	pRet = new Ninja ( );

	if ( pRet && pRet->InitWithWorld ( pWorld, tLocation, szBaseFileName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool Ninja::InitWithWorld ( b2World* pWorld, const CCPoint& tLocation, const KDchar* szBaseFileName )
{
	if ( !BodyNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pTheWorld			= pWorld;
	m_sBaseImageName	= szBaseFileName;
	m_tInitialLocation	= tLocation;
	m_bOnGround			= KD_FALSE;
	m_nCounter			= 0;

	// later we use initialLocation.x 

	this->createNinja ( );

	return KD_TRUE;
}

KDbool Ninja::isOnGround ( KDvoid )
{
	return m_bOnGround;
}

KDvoid Ninja::spriteInSlingState ( KDvoid )
{
	this->unschedule ( schedule_selector ( Ninja::blink ) );
	this->unschedule ( schedule_selector ( Ninja::openEyes ) );

	m_bOnGround = KD_FALSE;

	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_in_sling.png", m_sBaseImageName.c_str ( ) ) ) 
	);
}

KDvoid Ninja::spriteInStandingState ( KDvoid )
{
	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_standing.png", m_sBaseImageName.c_str ( ) ) ) 
	);

	this->schedule ( schedule_selector ( Ninja::blink ), CCRANDOM_BETWEEN ( 3.f, 8.f ) );
}

KDvoid Ninja::spriteInPulledBackSlingState ( KDvoid )
{
	this->unschedule ( schedule_selector ( Ninja::blink ) );
	this->unschedule ( schedule_selector ( Ninja::openEyes ) );

	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_roll.png", m_sBaseImageName.c_str ( ) ) ) 
	);
}

KDvoid Ninja::spriteInGroundState ( KDvoid )
{
	m_bOnGround = KD_TRUE;

	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_on_ground.png", m_sBaseImageName.c_str ( ) ) ) 
	);
}

KDvoid Ninja::spriteInRollStateWithAnimationFirst ( KDvoid )
{
	if ( !m_bOnGround )
	{		
		m_nCounter = 0;
		this->schedule ( schedule_selector ( Ninja::rollAnimation ), 1.0f / 60.0f );
	}			
}

KDvoid Ninja::rollAnimation ( KDfloat fDelta )
{
	m_nCounter++;

	if ( m_nCounter <= 3 )
	{		
		m_pSprite->setTexture 
		(
			CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_roll000%d.png", m_sBaseImageName.c_str ( ), m_nCounter ) ) 
		);
	}
	else 
	{	
		if ( !m_bOnGround ) 
		{
			m_pSprite->setTexture 
			(
				CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_roll.png", m_sBaseImageName.c_str ( ) ) ) 
			);
		}
		else
		{
			m_pSprite->setTexture 
			(
				CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_on_ground.png", m_sBaseImageName.c_str ( ) ) ) 
			);
		}
		
		this->unschedule ( schedule_selector ( Ninja::rollAnimation ) );
		
		m_nCounter = 0;
	}
}

KDvoid Ninja::spriteInAirState ( KDvoid )
{
	m_bOnGround = KD_FALSE;

	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_air.png", m_sBaseImageName.c_str ( ) ) ) 
	);
}

KDvoid Ninja::blink ( KDfloat fDelta )
{
	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_blink.png", m_sBaseImageName.c_str ( ) ) ) 
	);

	this->unschedule ( schedule_selector ( Ninja::blink ) );
	this->schedule   ( schedule_selector ( Ninja::openEyes ), 0.5f );
}

KDvoid Ninja::openEyes ( KDfloat fDelta )
{
	m_pSprite->setTexture 
	(
		CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "%s_standing.png", m_sBaseImageName.c_str ( ) ) ) 
	);

	this->unschedule ( schedule_selector ( Ninja::openEyes ) );
	this->schedule   ( schedule_selector ( Ninja::blink ), CCRANDOM_BETWEEN ( 3.f, 8.f ) );
}

KDvoid Ninja::createNinja ( KDvoid )
{
	m_sSpriteImageName = ccszf ( "%s_standing.png", m_sBaseImageName.c_str ( ) ); 

	m_bOnGround = KD_FALSE;

	// Define the dynamic body.
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_staticBody;					// or you could use b2DynamicBody to start the ninja as dynamic
	tBodyDef.position.Set ( m_tInitialLocation.x / PTM_RATIO, m_tInitialLocation.y / PTM_RATIO );

	b2CircleShape*	pShape = new b2CircleShape ( );
	pShape->m_radius = ( 40 / PTM_RATIO ) * 0.5f;	// increase or decrease 40 for a different circle size definition

	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = pShape;	
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 1.0f;
	tFixtureDef.restitution =  0.1f;

	this->createBodyWithSpriteAndFixture ( m_pTheWorld, &tBodyDef, &tFixtureDef, m_sSpriteImageName.c_str ( ) );
}
