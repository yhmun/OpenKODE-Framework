/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *      Description     
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Kirill Muzykov on 8/22/13.
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
#include "GameLayer.h"
#include "Waypoint.h"
#include "Enemy.h"
#include "Tower.h"

#define TOWER_COST		300

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

    // Initial field values.
    m_nWave			= 0;
    m_nPlayerHP		= 5;
    m_nPlayerGold	= 1000;

    CCSprite*	pBackground = CCSprite::create ( "bg.png" );
    pBackground->setPosition ( ccp ( m_tContentSize.cx * 0.5f, m_tContentSize.cy * 0.5f ) );
    this->addChild ( pBackground );

    // HP
    m_pHp = CCLabelBMFont::create ( ccszf ( "HP: %d", m_nPlayerHP ), "font_red_14.fnt" );
    m_pHp->setPosition ( ccp ( 35, m_tContentSize.cy - 12 ) );
    m_pHp->setAnchorPoint ( ccp ( 0, 0.5f ) );
    this->addChild ( m_pHp );    
    
    // WAVE
    m_pWave = CCLabelBMFont::create ( "", "font_red_14.fnt" );
    m_pWave->setPosition ( ccp ( 400, m_tContentSize.cy - 12 ) );
    m_pWave->setAnchorPoint ( ccp ( 0, 0.5f ) );
    this->addChild ( m_pWave );
    
    // GOLD
    m_pGold = CCLabelBMFont::create ( "", "font_red_14.fnt" );
    m_pGold->setPosition ( ccp ( 135, m_tContentSize.cy - 12 ) );
    m_pGold->setAnchorPoint ( ccp ( 0, 0.5f ) );
    this->addChild ( m_pGold );
	this->awardGold ( 0 );			// just to display initial ammount

	this->loadTowerPositions ( );
	this->addWaypoints ( );
	this->loadWave ( );

	this->setTouchEnabled ( KD_TRUE );
    
    // Playing background music
    SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "8bitDungeonLevel.mp3" );

	return KD_TRUE;
}

KDvoid GameLayer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( !this->canBuyTower ( ) )
	{
        return;
	}
    

    // Converting coords of touch point.
    CCTouch*	pTouch =  (CCTouch*) pTouches->anyObject ( );
    CCPoint		tLocation = pTouch->getLocation ( );
    	
    // Finding tower spot we've cliced on.
    CCObject*	pObject;
    CCARRAY_FOREACH ( &m_aTowerBases, pObject )
    {
        // Checking that there is no turret already.
        CCSprite*	pSprite = (CCSprite*) pObject;

        if ( !pSprite->getUserData ( ) && pSprite->boundingBox ( ).containsPoint ( tLocation ) )
        {
            SimpleAudioEngine::sharedEngine ( )->playEffect ( "tower_place.wav" );
            
            Tower*		pTower = Tower::create ( "tower.png" );

            pTower->setTheGame ( this );            
            pTower->setAnchorPoint ( ccp ( 0.5, 0.5 ) );
            pTower->setPosition ( pSprite->getPosition ( ) );

            this->addChild ( pTower );

            pSprite->setUserData ( pTower );
            
            // spending gold
            m_nPlayerGold -= TOWER_COST;
            this->awardGold ( 0 );
        }
    }
}

KDvoid GameLayer::awardGold ( KDint nGold )
{
    m_nPlayerGold += nGold;
    m_pGold->setString ( ccszf ( "GOLD %d", m_nPlayerGold ) );
}

KDvoid GameLayer::loadTowerPositions ( KDvoid )
{
    CCArray*	pTowerPositions = CCArray::createWithContentsOfFile ( "TowersPosition.plist" );
    CCObject*	pObject;
    CCARRAY_FOREACH ( pTowerPositions, pObject )
    {
        // Converting to actual obj type
        CCDictionary*	pTowerPos = (CCDictionary*) pObject;
        
        // Creating sprite at position
        CCSprite*		pTowerBase = CCSprite::create ( "open_spot.png" );
        CCPoint			tPoint = ccp
		(
			pTowerPos->valueForKey ( "x" )->floatValue ( ),
			pTowerPos->valueForKey ( "y" )->floatValue ( )
		);
        pTowerBase->setPosition ( tPoint );
        
        // Adding to layer
        this->addChild ( pTowerBase );
        
        // Adding to array
        m_aTowerBases.addObject ( pTowerBase );
    }
}

KDbool GameLayer::loadWave ( KDvoid )
{
    CCArray*	pWaveData = CCArray::createWithContentsOfFile ( "Waves.plist" );    
    if ( m_nWave >= (KDint) pWaveData->count ( ) )
    {
        return KD_FALSE;
    }
    
    CCArray*	pCurrentWaveData = CCArray::createWithArray ( (CCArray*) pWaveData->objectAtIndex ( m_nWave ) );
    
    CCObject*	pObject;
    CCARRAY_FOREACH ( pCurrentWaveData, pObject )
    {		
        CCDictionary*	pEnemyData = (CCDictionary*) pObject;
        Enemy*			pEnemy = Enemy::create ( "enemy.png" );

        pEnemy->setTheGame ( this );
        
        // Finding starting waypoint (which is last in array).
        // Setting enemy to its position and directing to next waypoint.
        Waypoint*		pLastWaypoint = (Waypoint*) m_aWaypoints.lastObject ( );

        pEnemy->setPosition ( pLastWaypoint->getPosition ( ) );
        pEnemy->setDestinationWaypoint ( pLastWaypoint->getNextWaypoint ( ) );
        pEnemy->schedule ( schedule_selector ( Enemy::doActivate ), pEnemyData->valueForKey ( "spawnTime" )->floatValue ( ) );
        
        this->addChild ( pEnemy );
		
        m_aEnemies.addObject ( pEnemy );
    }
   
    // Updagin counter & label.
    m_nWave++;
    m_pWave->setString ( ccszf ( "WAVE: %d", m_nWave ) );
   
    return KD_TRUE;
}

KDvoid GameLayer::addWaypoints ( KDvoid )
{
    // Adding path waypoints. Starting from our base and moving to start of enemies.	
    Waypoint*	pWaypoint1 = this->addWaypoint ( ccp ( 420,  35 ), KD_NULL );
    Waypoint*	pWaypoint2 = this->addWaypoint ( ccp (  35,  35 ), pWaypoint1 );
    Waypoint*	pWaypoint3 = this->addWaypoint ( ccp (  35, 130 ), pWaypoint2 );
    Waypoint*	pWaypoint4 = this->addWaypoint ( ccp ( 445, 130 ), pWaypoint3 );
    Waypoint*	pWaypoint5 = this->addWaypoint ( ccp ( 445, 220 ), pWaypoint4 );

    this->addWaypoint ( ccp ( -40, 220 ), pWaypoint5 );
}

Waypoint* GameLayer::addWaypoint ( const CCPoint& tPosition, Waypoint* pNext )
{
    Waypoint*	pWaypoint = Waypoint::create ( );

    pWaypoint->setPosition ( tPosition );
    pWaypoint->setNextWaypoint ( pNext );
    
    this->addChild ( pWaypoint );

    m_aWaypoints.addObject ( pWaypoint );
    
    return pWaypoint;
}

KDvoid GameLayer::doGameOver ( KDvoid )
{
    if ( !m_bGameEnded )
    {
        m_bGameEnded = KD_FALSE;

        CCDirector::sharedDirector ( )->replaceScene ( CCTransitionRotoZoom::create ( 1.0f, GameLayer::scene ( ) ) );
    }  
}

KDbool GameLayer::canBuyTower ( KDvoid )
{
	return ( m_nPlayerGold - TOWER_COST >= 0 );
}

KDbool GameLayer::checkCirclesCollide ( const CCPoint& tCenter1, KDfloat fRadius1, const CCPoint& tCenter2, KDfloat fRadius2 )
{
    KDfloat		fDistance = ccpDistance ( tCenter1, tCenter2 );

    return fDistance <= fRadius1 + fRadius2;
}

KDvoid GameLayer::getHpDamage ( KDvoid )
{
    SimpleAudioEngine::sharedEngine ( )->playEffect ( "life_lose.wav" );
    
    m_nPlayerHP--;
    if ( m_nPlayerHP <= 0 )
    {
        m_nPlayerHP = 0;
        this->doGameOver ( );
    }
    
    m_pHp->setString ( ccszf ( "HP: %d", m_nPlayerHP ) );
}

KDvoid GameLayer::enemyGotKilled ( Enemy* pEnemy )
{
	m_aEnemies.removeObject ( pEnemy );    

	if ( m_aEnemies.count ( ) <= 0 )
	{
		if ( !this->loadWave ( ) )
		{
			CCLOG ( "You win" );

			CCDirector::sharedDirector ( )->replaceScene ( CCTransitionSplitCols::create ( 1, GameLayer::scene ( ) ) );
		}
	}
}

const CCArray& GameLayer::getEnemies ( KDvoid )
{
    return m_aEnemies;
}