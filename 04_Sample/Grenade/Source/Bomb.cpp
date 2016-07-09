/* --------------------------------------------------------------------------
 *
 *      File            Bomb.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2011 Robert Blackwood 01/20/11
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "Bomb.h"
#include "Game.h"

Bomb* Bomb::create ( Game* pGame )
{
	Bomb*  pRet = new Bomb ( ); 

	if ( pRet && pRet->initWithGame ( pGame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

Bomb* Bomb::create ( Game* pGame, cpShape* pShape )
{
	Bomb*  pRet = new Bomb ( ); 

	if ( pRet && pRet->initWithGame ( pGame, pShape ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool Bomb::initWithGame ( Game* pGame )
{
	cpShape*  pShape = pGame->getSpaceManager ( )->addCircleAt ( cpvzero, STATIC_MASS, 7 );

	return initWithGame ( pGame, pShape );
}

KDbool Bomb::initWithGame ( Game* pGame, cpShape* pShape )
{
	if ( !cpCCSprite::initWithFile ( pShape, "Images/bomb.png" ) )
	{
		return KD_FALSE;
	}

	m_pGame = pGame;
	m_bCountDown = KD_FALSE;

	// Free the shape when we are released
	this->setSpaceManager ( pGame->getSpaceManager ( ) );
	this->setAutoFreeShapeAndBody ( KD_TRUE );

	// Handle collisions
	pShape->collision_type = kBombCollisionType;

	return KD_TRUE;
}

KDvoid Bomb::startCountDown ( KDvoid )
{
	// Only start it if we haven't yet
	if ( !m_bCountDown )
	{
		m_bCountDown = KD_TRUE;
		
		CCFiniteTimeAction*  pF1 = CCFadeTo::create ( 0.25f, 200 );
		CCFiniteTimeAction*  pF2 = CCFadeIn::create ( 0.25f );
		
		CCFiniteTimeAction*  pD = CCDelayTime::create ( 3 );
		CCFiniteTimeAction*  pC = CCCallFunc ::create ( this, callfunc_selector ( Bomb::blowup ) );
		
		this->runAction ( CCRepeatForever::create ( (CCActionInterval*) CCSequence::create ( pF1, pF2, KD_NULL ) ) );
		this->runAction ( CCSequence::create ( pD, pC, KD_NULL ) );
	}
}

KDvoid Bomb::blowup ( KDvoid )
{
	this->getSpaceManager ( )->applyLinearExplosionAt ( cpv ( this->getPosition ( ) ), 100, 4500 );

	CCParticleSun*  pExplosion = CCParticleSun::create ( );

	pExplosion->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	pExplosion->setTotalParticles ( 60 );
	pExplosion->setPosition ( this->getPosition ( ) );
	pExplosion->setDuration ( 0.5f );
	pExplosion->setSpeed ( 40 );
	pExplosion->setAutoRemoveOnFinish ( KD_TRUE );
	pExplosion->setBlendAdditive ( KD_FALSE );

	m_pGame->addChild ( pExplosion );
	m_pGame->removeChild ( this, KD_TRUE );
}