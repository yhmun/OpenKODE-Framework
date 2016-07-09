/* --------------------------------------------------------------------------
 *
 *      File            Enemy.h
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
#include "Enemy.h"
#include "Game.h"

Enemy* Enemy::create ( Game* pGame )
{
	Enemy*  pRet = new Enemy ( ); 

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

Enemy* Enemy::create ( Game* pGame, cpShape* pShape )
{
	Enemy*  pRet = new Enemy ( ); 

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

KDbool Enemy::initWithGame ( Game* pGame )
{
	cpShape*  pShape = pGame->getSpaceManager ( )->addCircleAt ( cpvzero, 50, 9 );

	return initWithGame ( pGame, pShape );
}

KDbool Enemy::initWithGame ( Game* pGame, cpShape* pShape )
{
	if ( !cpCCSprite::initWithFile ( pShape, "Images/elephant.png" ) )
	{
		return KD_FALSE;
	}

	m_pGame = pGame;

	// Free the shape when we are released
	this->setSpaceManager ( pGame->getSpaceManager ( ) );
	this->setAutoFreeShapeAndBody ( KD_TRUE );

	m_nDamage = 0;

	// Handle collisions
	pShape->collision_type = kEnemyCollisionType;

	return KD_TRUE;
}

KDvoid Enemy::addDamage ( KDint nDamage )
{
	m_nDamage += nDamage;
	
	if ( m_nDamage > 2 )
	{
		m_pGame->enemyKilled ( );
		
		CCSprite*  pPoof = CCSprite::create ( "Images/poof.png" );
		m_pGame->addChild ( pPoof, 10 );

		pPoof->setScale ( 0.1f );
		pPoof->setPosition ( this->getPosition ( ) );

		CCFiniteTimeAction*  pS = CCEaseBounceOut::create ( CCScaleTo::create ( 1, 1 ) );		
		CCFiniteTimeAction*  pD = CCDelayTime::create ( 0.3f );
		CCFiniteTimeAction*  pF = CCFadeOut  ::create ( 0.7f );
		CCFiniteTimeAction*  pC = CCCallFunc ::create ( pPoof, callfunc_selector ( CCSprite::removeFromParentAndCleanup ) );

		pPoof->runAction ( CCSequence::create ( pS, pD, pF, pC, KD_NULL ) );
		
		m_pGame->removeChild ( this, KD_TRUE );
	}
}
