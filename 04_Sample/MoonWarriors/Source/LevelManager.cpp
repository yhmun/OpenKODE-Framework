/* --------------------------------------------------------------------------
 *
 *      File            LevelManager.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors    
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
#include "LevelManager.h"
#include "Enemy.h"
#include "Ship.h"

struct Level 
{
	KDbool		bRepeat;
	KDint		nShowTime;
	KDint		nTypesLen;
	KDint		nTypes[10];
};

static struct Level  l_aLevelTable [] =
{
	{ KD_TRUE, 2, 3, { 0, 1, 2 } },
	{ KD_TRUE, 5, 3, { 3, 4, 5 } },
};

LevelManager::LevelManager ( GameLayer* pLayer )
: m_pGameLayer ( KD_NULL )
, m_nLevel ( 0 )
, m_nTick ( 0 )
{
	CCAssert ( !m_pGameLayer, "gameLayer must be non-nil" );

	m_pGameLayer = pLayer;
}

KDvoid LevelManager::loadLevelResource ( KDfloat fTime )
{
	m_nTick += (KDint) fTime;

	KDint  nMax = sizeof ( l_aLevelTable ) / sizeof ( l_aLevelTable [ 0 ] );
	for ( KDint i = 0; i < nMax; i++ )
	{
		Level*  pLevel = &l_aLevelTable [ i ];
		if ( pLevel->bRepeat )
		{
			if ( m_nTick % pLevel->nShowTime == 0 )
			{
				for ( KDint j = 0; j < pLevel->nTypesLen; j++ )
				{
					addEnemyToGameLayer ( pLevel->nTypes [ j ] ); 
				}
			}
		}
		else
		{
			if ( m_nTick == pLevel->nShowTime )
			{
				for ( KDint j = 0; j < pLevel->nTypesLen; j++ )
				{
					addEnemyToGameLayer ( pLevel->nTypes [ j ] ); 
				}
			}
		}
	}
}

KDvoid LevelManager::addEnemyToGameLayer ( KDint nType )
{
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	Enemy*  pAddEnemy = Enemy::create ( nType );
	pAddEnemy->setPosition ( ccp ( 80 + ( tWinSize.cx - 160 ) * CCRANDOM_0_1 ( ), tWinSize.cy ) );

	CCPoint				 tPoint;
	CCFiniteTimeAction*  pAction;

	switch ( pAddEnemy->getMoveType ( ) )
	{
		case eMoveAttack :
		{
			Ship*  pShip = m_pGameLayer->getShip ( );
			
			if ( !pShip )
			{
				return;
			}

			tPoint  = pShip->getPosition ( );
			pAction = CCMoveTo::create ( 1, tPoint );

		}	break;

		case eMoveVertical :

			tPoint  = ccp ( 0, -tWinSize.cy - pAddEnemy->getContentSize ( ).cy );
			pAction = CCMoveBy::create ( 4, tPoint );

			break;

		case eMoveHorizontal :	
		{
			tPoint = ccp ( 0, -100 - 200 * CCRANDOM_0_1 ( ) );

			CCActionInterval*  pDelay  = CCDelayTime::create ( 1 );
			CCActionInterval*  pMoveBy = CCMoveBy::create ( 1, ccp ( 100 + 100 * CCRANDOM_0_1 ( ), 0 ) );

			pAction = CCSequence::create
			(
				CCMoveBy::create ( 0.5f, tPoint ),
				CCMoveBy::create ( 1, ccp ( -50 - 100 * CCRANDOM_0_1 ( ), 0 ) ),
				CCRepeat::create 
				(
					(CCActionInterval *) CCSequence::create ( pDelay, pMoveBy, pDelay->copy ( ), pMoveBy->reverse ( ), KD_NULL ), 1000
				),
				KD_NULL
			);

		}	break;

		case eMoveOverlap :

			tPoint.x = ( pAddEnemy->getPosition ( ).x <= tWinSize.cx / 2 ) ? 320.f : -320.f;
			pAction = CCSequence::createWithTwoActions 
			(
				CCMoveBy::create ( 4, ccp (  tPoint.x, -240 ) ),
				CCMoveBy::create ( 4, ccp ( -tPoint.x, -320 ) )
			);

			break;
	}

	pAddEnemy->runAction ( pAction );
	m_pGameLayer->addChild ( pAddEnemy, 1000, eTagEnemy );
	Global::pEnemyContainer->addObject ( pAddEnemy );
}