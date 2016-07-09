/* --------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 27/04/2011
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
#include "GameScene.h"

CCScene* KnightFight::scene ( KDvoid )
{
	CCScene*		pScene = CCScene::create ( );						
	KnightFight*	pGameLayer = new KnightFight ( );	
	HudLayer*		pHudLayer  = HudLayer::create ( );
	
	pGameLayer->setHudLayer ( pHudLayer );
	pGameLayer->init ( );
	pGameLayer->autorelease ( );
	pScene->addChild ( pGameLayer );	
	pScene->addChild ( pHudLayer );
																										
	return pScene;
}

KnightFight::KnightFight ( KDvoid )
{
	m_pAttackers = KD_NULL;
	m_pBullets	 = KD_NULL;
	m_pDoors	 = KD_NULL;
	m_fTouchTick = 0;
}

KnightFight::~KnightFight ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAttackers );
	CC_SAFE_RELEASE ( m_pBullets );
	CC_SAFE_RELEASE ( m_pDoors );
}

KDbool KnightFight::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	SimpleAudioEngine*		pSE = SimpleAudioEngine::sharedEngine ( );

	pSE->preloadEffect ( "bullet.wav" );
	pSE->preloadEffect ( "ghostbirth.wav" );
	pSE->preloadEffect ( "ghostdeath.wav" );
	pSE->preloadEffect ( "neigh.wav" );

	this->setTouchMode ( kCCTouchesOneByOne );
	this->setTouchEnabled ( KD_TRUE );

	GameMgr->setGameScene ( this );
	GameMgr->setPlayerLives ( GameMgr->getMaxPlayerLives ( ) );

	this->resetGame ( );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid KnightFight::update ( KDfloat fDelta )
{
	m_fCollisionsTick				+= fDelta;
	m_fLinesOfSightTick				+= fDelta;
	m_fPositionZTick				+= fDelta;
	m_fStarUpdateTick				+= fDelta;
	m_fCollisionPlayerEnvironTick	+= fDelta;
	
	if ( m_fCollisionPlayerEnvironTick > 0.1f ) 
	{
		this->checkPlayerCollisionWithEnvironment ( );
		m_fCollisionPlayerEnvironTick = 0;
	}

	if ( m_fCollisionsTick > 0.1f )
	{
		this->checkCollisions ( );
		m_fCollisionsTick = 0;
	}

	if ( m_fLinesOfSightTick  > 1 )
	{
		this->checkLinesOfSight ( );
		m_fLinesOfSightTick = 0;
	}

	if ( m_fStarUpdateTick > 5 )
	{
//		this->updateAstarPaths ( );
		m_fStarUpdateTick = 0;
	}
			
	if ( m_fPositionZTick > 0.1f ) 
	{
		// set the player's Z position
		CCPoint		tTilePos = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( m_pPlayer->getLocation ( ) );
		m_pPlayer->updateVertexZ ( tTilePos, GameMgr->getTileMap ( ) );

		tTilePos = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( m_pKnight->getLocation ( ) );
		m_pKnight->updateVertexZ ( tTilePos, GameMgr->getTileMap ( ) );
		
		CCObject*	pObject;
		CCARRAY_FOREACH ( m_pAttackers, pObject )
		{
			Attacker*	pAttacker = (Attacker*) pObject;
			// set the attacker's z position
			tTilePos = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( pAttacker->getLocation ( ) );
			pAttacker->updateVertexZ ( tTilePos, GameMgr->getTileMap ( ) );
		}
	}
	
	// reset the viewpoint so they player is always in the centre
	this->setViewpointCenter ( m_pPlayer->getLocation ( ) );
}

KDbool KnightFight::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_TRUE;
}

KDvoid KnightFight::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( GameMgr->getGameState ( ) == GameOver )
	{
		CCLOG ( "Game over touch" );
		GameMgr->showMenu ( );
		return;
	}
	
	if ( GameMgr->getGameState ( ) == PowerUp )
	{
		CCDirector::sharedDirector ( )->resume ( );
		this->removeChild ( m_pPowerUpAlert, KD_TRUE );
	}

	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	KDfloat		fTouchTick = kdGetMilliseconds ( );
	KDfloat		fDiffTick  = fTouchTick - m_fTouchTick;

	if ( fDiffTick > 200.f )
	{
		if ( m_pPlayer->isAlive ( ) )
		{
			m_pPlayer->moveSpritePosition ( tLocation, m_pPlayer );
		}		

		m_fTouchTick = fTouchTick;
	}
	else
	{
		m_pPlayer->stopAllActions ( );
		this->fireBullet ( tLocation );


		CCPoint		tDiff = ccpSub ( m_pPlayer->getPosition ( ), tLocation );
		KDfloat		fDistance = kdSqrtf ( kdPowf ( tDiff.x, 2 ) + kdPowf ( tDiff.y, 2 ) );

		KDint		nBulletOffsetAngle = 5;
				
		if ( m_pPlayer->isTripleShotsActive ( ) ) 
		{
			CoordinateFunctions*	pCoordinateFunctions = GameMgr->getCoordinateFunctions ( );
			
			KDfloat		fOriginalAngle = pCoordinateFunctions->getAngleBetweenPoints ( m_pPlayer->getPosition ( ), tLocation );
			KDfloat		fBullet1Angle = fOriginalAngle + nBulletOffsetAngle;
			KDfloat		fBullet2Angle = fOriginalAngle - nBulletOffsetAngle;

			CCLOG ( "original angle %f", fOriginalAngle );
			CCLOG ( "bullet 1 angle %f", fBullet1Angle );
			CCLOG ( "bullet 2 angle %f", fBullet2Angle );
			
			CCPoint		tBullet1Target = pCoordinateFunctions->pointFromAngleAndDistance ( fBullet1Angle, m_pPlayer->getPosition ( ), fDistance );
			CCPoint		tBullet2Target = pCoordinateFunctions->pointFromAngleAndDistance ( fBullet2Angle, m_pPlayer->getPosition ( ), fDistance );

			this->fireBullet ( tBullet1Target );
			this->fireBullet ( tBullet2Target );
		}
		
		if ( GameMgr->isSoundOn ( ) )
		{
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "bullet.wav" );
		}
	}
}

KDvoid KnightFight::setViewpointCenter ( const CCPoint& tPosition )
{
	KDint	x = (KDint) ( -tPosition.x + ( m_tContentSize.cx / 2 ) );
	KDint	y = (KDint) ( -tPosition.y + ( m_tContentSize.cy / 2 ) );
	
	this->setPosition ( ccp ( x, y ) );
}

KDvoid KnightFight::setUpHouseContents ( KDvoid )
{
	CCTMXTiledMap*	pTileMap = GameMgr->getTileMap ( );
	CCTMXLayer*		pMetaLayer = pTileMap->layerNamed ( "Meta" );

	for ( KDint x = 0; x < (KDint) pTileMap->getMapSize ( ).cx; x++ )
	{
		for ( KDint y = 0; y < (KDint) pTileMap->getMapSize ( ).cy; y++ )
		{
			CCPoint		tTilePos = ccp ( x, y );
			KDuint		uMetaTileGID = pMetaLayer->tileGIDAt ( tTilePos );

			if ( uMetaTileGID > 0 )
			{
				CCDictionary*	pProperties = pTileMap->propertiesForGID ( uMetaTileGID );
				if ( pProperties ) 
				{
					const CCString*		pCollision = pProperties->valueForKey ( "Door" );
					if ( !pCollision->compare ( "True" ) )
					{
						CCLOG ( "Door found at %d %d", x, y );
						Door*	pDoor = Door::create ( );
						pDoor->setTilePos ( ccp ( x, y ) );
						m_pDoors->addObject ( pDoor );				
					} 
				}
			}
		}
	}
}

KDvoid KnightFight::resetGame ( KDvoid )
{
	this->removeAllChildrenWithCleanup ( KD_TRUE );

	if ( GameMgr->getPlayerLives ( ) <= 0 )
	{
		GameMgr->setGameState ( GameOver );
		CCDirector::sharedDirector ( )->pause ( );	
		m_pHudLayer->gameOver ( );
		return;
	}

	m_pHudLayer->updateLevel ( GameMgr->getLevel ( ) );
	m_pHudLayer->updateLives ( GameMgr->getPlayerLives ( ) );
	CCDirector::sharedDirector ( )->resume ( );

	GameMgr->setGameState ( Play );

	CC_SAFE_RELEASE ( m_pBullets );
	CC_SAFE_RELEASE ( m_pDoors );

	KDint			nMapLevel = ( ( GameMgr->getLevel ( ) - 1 ) % GameMgr->getMaxLevels ( ) ) + 1;
	const KDchar*	szMapName = ccszf ( "level-%d.tmx", nMapLevel );

	CCTMXTiledMap*	pTileMap = CCTMXTiledMap::create ( szMapName );
	GameMgr->setTileMap ( pTileMap );
	m_pMeta = pTileMap->layerNamed ( "Meta" );
	m_pMeta->setVisible ( KD_FALSE );
	this->addChild ( pTileMap, -1, TileMapNode );

	CoordinateFunctions*	pCoordinateFunctions = GameMgr->getCoordinateFunctions ( );
	pCoordinateFunctions->setPlayableAreaMin ( ccp ( 0, 0 ) );
	pCoordinateFunctions->setPlayableAreaMax ( ccp ( pTileMap->getMapSize ( ).cx - 1, pTileMap->getMapSize ( ).cy - 1 ) );

	m_pPlayer = Player::create ( );
	this->addChild ( m_pPlayer );

	CCPoint		tPlayerStart = this->getPlayerSpawn ( );
	pTileMap->setPosition ( pCoordinateFunctions->locationFromTilePos ( tPlayerStart ) );

	m_pPlayer->changeSpriteAnimation ( "W" );
	m_pPlayer->setVisible ( KD_TRUE );

	m_pHudLayer->showSpeedUpSprite ( KD_FALSE );
	m_pHudLayer->showTripleShotsSprite ( KD_FALSE );

	m_pKnight = Knight::create ( );
	this->addChild ( m_pKnight );

	// position knight to start in a random unblocked tile
	CCPoint		tKnightStart;
	KDbool		bFarFromPlayer = KD_TRUE;

	do
	{
		KDint		nStartX = kdRand ( ) % (KDint) pTileMap->getMapSize ( ).cx;
		KDint		nStartY = kdRand ( ) % (KDint) pTileMap->getMapSize ( ).cy;
		tKnightStart = ccp ( nStartX, nStartY );
			
		CCPoint		tDiffKnightPlayer = ccpSub ( tPlayerStart, tKnightStart );

		CCLOG ( "Knight is being spawned %f %f squares away", tDiffKnightPlayer.x, tDiffKnightPlayer.y );
		// Respawn if too close to player
		if ( ( kdFabsf ( tDiffKnightPlayer.x ) < m_tContentSize.cx / 2 ) && ( kdFabsf ( tDiffKnightPlayer.y ) < m_tContentSize.cy / 2 ) )
		{
			bFarFromPlayer = KD_FALSE;
		}
		
	} while ( pCoordinateFunctions->isTilePosBlocked ( tKnightStart ) || bFarFromPlayer );

	tKnightStart = pCoordinateFunctions->locationFromTilePos ( tKnightStart );
	tKnightStart = pCoordinateFunctions->pointRelativeToCentreFromLocation ( tKnightStart );
	tKnightStart.y = tKnightStart.y + m_pKnight->getContentSize ( ).cy / 4;
	m_pKnight->setPosition ( tKnightStart );
	m_pKnight->moveInRandomDirection ( );

	this->spawnAttackers ( );

	m_pBullets = new CCArray ( );
	m_pDoors   = new CCArray ( );

	this->setUpHouseContents ( );
}

KDvoid KnightFight::loseLife ( KDvoid )
{
	GameMgr->setPlayerLives ( GameMgr->getPlayerLives ( ) - 1 );
	m_pHudLayer->updateLives ( GameMgr->getPlayerLives ( ) );
}

KDvoid KnightFight::removeSpeedUp ( KDfloat fDelta )
{
	m_pPlayer->setSpeedUpActive ( KD_FALSE );	
	m_pHudLayer->showSpeedUpSprite ( KD_FALSE );
	this->unschedule ( schedule_selector ( KnightFight::removeSpeedUp ) );
	m_pPlayer->setVelocity ( m_pPlayer->getVelocityOrdinary ( ) );
}

KDvoid KnightFight::removeTripleShots ( KDfloat fDelta )
{
	m_pPlayer->setTripleShotsActive ( KD_FALSE );	
	m_pHudLayer->showTripleShotsSprite ( KD_FALSE );
	this->unschedule ( schedule_selector ( KnightFight::removeTripleShots ) );
}

CCPoint KnightFight::getPlayerSpawn ( KDvoid )
{
	CCTMXTiledMap*	pTileMap   = GameMgr->getTileMap ( );
	CCTMXLayer*		pMetaLayer = pTileMap->layerNamed ( "Meta" );

	for ( KDint x = 0; x < (KDint) pTileMap->getMapSize ( ).cx; x++ )
	{
		for ( KDint y = 0; y < (KDint) pTileMap->getMapSize ( ).cy; y++ )
		{
			CCPoint		tTilePos = ccp ( x, y );
			KDuint		uMetaTileGID = pMetaLayer->tileGIDAt ( tTilePos );

			if ( uMetaTileGID > 0 )
			{
				CCDictionary*	pProperties = pTileMap->propertiesForGID ( uMetaTileGID );

				if ( pProperties )
				{
					const CCString*	pPlayerSpawn = pProperties->valueForKey ( "PlayerSpawn" );
					if ( !pPlayerSpawn->compare ( "True" ) )
					{
						CCLOG ( "Player Spawn found at %d %d", x, y );
						return tTilePos;
					}
				}
			}
		}
	}

	CCLOG ( "Error: Player spawn not found on map");

	return ccpz;
}

Player* KnightFight::getPlayer ( KDvoid )
{
	return m_pPlayer;
}

KDvoid KnightFight::spawnAttackers ( KDvoid )
{
	if ( m_pAttackers ) 
	{
		CCObject*	pObject;
		CCARRAY_FOREACH ( m_pAttackers, pObject )
		{
			Attacker*	pAttacker = (Attacker*) pObject;
			this->removeChild ( pAttacker, KD_TRUE );
		}
	
		m_pAttackers->release ( );
	}

	m_pAttackers = new CCArray ( );

	CCTMXTiledMap*	pTileMap = GameMgr->getTileMap ( );
	CCTMXLayer*		pMetaLayer = pTileMap->layerNamed ( "Meta" );

	for ( KDint x = 0; x < (KDint) pTileMap->getMapSize ( ).cx; x++ )
	{
		for ( KDint y = 0; y < (KDint) pTileMap->getMapSize ( ).cy; y++ )
		{
			CCPoint		tTilePos = ccp ( x, y );
			KDuint		uMetaTileGID = pMetaLayer->tileGIDAt ( tTilePos );
				
			if ( uMetaTileGID > 0 )
			{
				CCDictionary*	pProperties = pTileMap->propertiesForGID ( uMetaTileGID );
				if ( pProperties ) 
				{
					const CCString*		pGhostSpawn = pProperties->valueForKey ( "GhostSpawn" );
					if ( !pGhostSpawn->compare ( "True" ) )
					{
						CCLOG ( "Ghost Spawn found at %d %d", x, y );

						Attacker*	pAttacker = Attacker::create ( );
						pAttacker->updateVertexZ ( tTilePos, pTileMap );

						CCPoint		tAttackerStart = GameMgr->getCoordinateFunctions ( )->locationFromTilePos ( tTilePos );
						tAttackerStart = GameMgr->getCoordinateFunctions ( )->pointRelativeToCentreFromLocation ( tAttackerStart );
						pAttacker->setPosition ( tAttackerStart );
						pAttacker->changeSpriteAnimation ( "S" );
						this->addChild ( pAttacker );
						m_pAttackers->addObject ( pAttacker );
					} 
				}
			}			
		}
	}
	
	CCLOG ( "Number of attackers %d", m_pAttackers->count ( ) );
}

KDvoid KnightFight::checkPlayerCollisionWithEnvironment ( KDvoid )
{
	CCPoint		tTilePos = GameMgr->getCoordinateFunctions ( )->tilePosFromLocation ( m_pPlayer->getLocation ( ) );

	if ( GameMgr->getCoordinateFunctions ( )->isTilePosBlocked ( tTilePos ) )
	{
		CCLOG ( "Collision!" );
		m_pPlayer->stopAllActions ( );
		m_pPlayer->setMoving ( KD_FALSE );
		m_pPlayer->setPosition ( m_pPlayer->getLastPosition ( ) );
	}
	else 
	{
		m_pPlayer->setLastPosition ( m_pPlayer->getPosition ( ) );
	}	
}

KDvoid KnightFight::checkCollisions ( KDvoid )
{
	CoordinateFunctions*	pCoordinateFunctions = GameMgr->getCoordinateFunctions ( );
	CCObject*				pObject;

	// Bullet collision detection
    CCArray*	pBulletsToDestroy = CCArray::create ( );
	CCARRAY_FOREACH ( m_pBullets, pObject )
	{
		Bullet*		pBullet = (Bullet*) pObject;
	
		CCPoint		tTilePos = pCoordinateFunctions->tilePosFromLocation ( pBullet->getPosition ( ) );
		
		if ( pCoordinateFunctions->isTilePosIsWithinBounds ( tTilePos ) && pBullet->isMoving ( ) )
		{
			// check bullet collision with knight
			if ( pCoordinateFunctions->spritesCollided ( pBullet, m_pKnight ) )
			{
				if ( m_pPlayer->isAlive ( ) )
				{				
					CCLOG ( "HIT KNIGHT" );
					if ( GameMgr->isSoundOn ( ) )
					{
						SimpleAudioEngine::sharedEngine ( )->playEffect ( "neigh.wav" );
					}
					
					m_pPowerUpAlert = CCSprite::create ( "ShootOut.png" );
					m_pPowerUpAlert->setPosition ( m_pPlayer->getPosition ( ) );
					this->addChild ( m_pPowerUpAlert );
					pBulletsToDestroy->addObject ( pBullet );

					this->scheduleOnce ( schedule_selector ( KnightFight::startShootOut ), 3 );

					CCDirector::sharedDirector ( )->pause ( );				
					GameMgr->setGameState ( ShootOut );
				}				
			} 
			else 
			{
				// Check collision of bullets with landscape
				if ( pCoordinateFunctions->isTilePosBlocked ( tTilePos ) )
				{
					CCLOG ( "Bullet collision with landscape!" );
                    pBulletsToDestroy->addObject ( pBullet );
				}
				else 
				{
					// Check collision of bullets with attackers
                    CCArray*	pAttackersToDestroy = CCArray::create ( );
					CCObject*	pObject2;
					CCARRAY_FOREACH ( m_pAttackers, pObject2 )
					{
						Attacker*	pAttacker = (Attacker*) pObject2;
					
                        if ( pAttacker->isAlive ( ) )
						{
                            if ( pCoordinateFunctions->spritesCollided ( pBullet, pAttacker ) )
							{
								pAttacker->deathSequence ( );
								pBulletsToDestroy->addObject ( pBullet );
                            }
                        }
					}

					m_pAttackers->removeObject ( pAttackersToDestroy );           
				}				
			}			
		} 
		else 
		{
			// bullet out of bounds
			CCLOG ( "Bullet out of bounds" );
            pBulletsToDestroy->addObject ( pBullet );
		}		
	}

	CCARRAY_FOREACH ( pBulletsToDestroy, pObject )
	{
		Bullet*		pBullet = (Bullet*) pObject;
		this->destroyBullet ( pBullet );
	}
	
	// Check if player is at door
	CCPoint		tTilePos = pCoordinateFunctions->tilePosFromLocation ( m_pPlayer->getLocation ( ) );
	if ( pCoordinateFunctions->atDoor ( tTilePos ) )
	{
	
		CCLOG ( "At door!!" );
		CCDirector::sharedDirector ( )->pause ( );
		GameMgr->setGameState ( PowerUp );

		const KDchar*	szPowerUp = this->getHouseContentsAtTilePos ( tTilePos );
		const KDchar*	szSpriteFilename = ccszf ( "%s.png", szPowerUp );

		m_pPowerUpAlert = CCSprite::create ( szSpriteFilename );
		m_pPowerUpAlert->setPosition ( m_pPlayer->getPosition ( ) );
		this->addChild ( m_pPowerUpAlert );

		CCLOG ( "Found power up: %s", szPowerUp );
		
		if ( kdStrcmp ( szPowerUp, "EmptyHouse" ) )
		{
			CCLOG ( "Setting house to be empty" );
			CCARRAY_FOREACH ( m_pDoors, pObject )
			{
				Door*	pDoor = (Door*) pObject;

				if ( pDoor->getTilePos ( ).x == tTilePos.x && pDoor->getTilePos ( ).y == tTilePos.y )
				{
					pDoor->setContents ( "EmptyHouse" );
					break;
				}
			}
		}
		
		if ( !kdStrcmp ( szPowerUp, "ExtraLife" ) )
		{
			GameMgr->setPlayerLives ( GameMgr->getPlayerLives ( ) + 1 );
			CCLOG ( "Extra life awarded" );
			m_pHudLayer->updateLives ( GameMgr->getPlayerLives ( ) );
		}

		if ( !kdStrcmp ( szPowerUp, "SpeedUp" ) )
		{
			if ( m_pPlayer->isSpeedUpActive ( ) )
			{
				this->unschedule ( schedule_selector ( KnightFight::removeSpeedUp ) );
			} 
			else 
			{
				m_pPlayer->setVelocity ( m_pPlayer->getVelocitySpeedUp ( ) );
			}
			this->schedule ( schedule_selector ( KnightFight::removeSpeedUp ), 15 );
			m_pPlayer->setSpeedUpActive ( KD_TRUE );
			m_pHudLayer->showSpeedUpSprite ( KD_TRUE );
			CCLOG ( "Speed Up awarded" );
		}
		
		if ( !kdStrcmp ( szPowerUp, "TripleShots" ) )
		{
			if ( m_pPlayer->isTripleShotsActive ( ) )
			{
				this->unschedule ( schedule_selector ( KnightFight::removeTripleShots ) );
			} 
			this->schedule ( schedule_selector ( KnightFight::removeTripleShots ), 15 );
			m_pPlayer->setTripleShotsActive ( KD_TRUE );
			m_pHudLayer->showTripleShotsSprite ( KD_TRUE );

			CCLOG ( "Triple Shots awarded" );			
		}
		
		if ( !kdStrcmp ( szPowerUp, "GhostRespawn" ) )
		{
			CCLOG ( "Ghost Respawn!" );
			this->spawnAttackers ( );
		}		
	}

    CCArray*	pAttackersToDestroy = CCArray::create ( );
	CCARRAY_FOREACH ( m_pAttackers, pObject )
	{
		Attacker*	pAttacker = (Attacker*) pObject;

		// Check collision of player with attackers
		if ( pCoordinateFunctions->spritesCollided ( pAttacker, m_pPlayer ) && pAttacker->isAlive ( ) )
		{
			m_pPlayer->deathSequence ( );
			pAttacker->deathSequence ( );
		}
	}
	m_pAttackers->removeObjectsInArray ( pAttackersToDestroy );  
	
	// Check collision of player with knight
	if ( pCoordinateFunctions->spritesCollided ( m_pKnight, m_pPlayer ) && m_pKnight->isAlive ( ) )
	{
		m_pPlayer->deathSequence ( );
	}
}

KDvoid KnightFight::checkLinesOfSight ( KDvoid )
{
	CCLOG ( "In Check Lines of sight for %d attackers", m_pAttackers->count ( ) );

	// check attackers' line of sight with player
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pAttackers, pObject )
	{
		Attacker*	pAttacker = (Attacker*) pObject;
	
		if ( pAttacker->checkIfPointIsInSight ( m_pPlayer->getLocation ( ), pAttacker ) )
		{						
			if ( pAttacker->isAlive ( ) ) 
			{				
				CCLOG ( "GOT LINE OF SIGHT - going straight at player!" );
				CCLOG ( "..." );
				CCLOG ( "Attacker is following path? %d", pAttacker->isFollowingPath ( ) );
				
				if ( pAttacker->isFollowingPath ( ) )
				{
					pAttacker->setFollowingPath ( KD_FALSE );
					if ( GameMgr->isIPad ( ) )
					{
						pAttacker->unschedule ( schedule_selector ( Attacker::updateAStarPath ) );						
					}
				}
				pAttacker->chasePlayer ( m_pPlayer );	
			} 
			else 
			{
				CCLOG ( "Attacker not alive" );
			}
			
		} 
		else 
		{		
			if ( pAttacker->isChasingPlayer ( ) )
			{
				pAttacker->setFollowingPath ( KD_TRUE );
				pAttacker->setChasingPlayer ( KD_FALSE );
				pAttacker->stopAllActions ( );
				CCLOG ( "Lost line of site... using A* pathfinding instead" );

				pAttacker->createPathToPlayer ( );
								
				if ( GameMgr->isIPad ( ) )
				{
					KDfloat		fDelay = ( kdRand ( ) % 9 ) / 3 + 4.f;
					pAttacker->scheduleOnce ( schedule_selector ( Attacker::updateAStarPath ), fDelay );
				}	
			}			
		}		
	}
}

KDvoid KnightFight::updateAstarPaths ( KDvoid )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pAttackers, pObject )
	{
		Attacker*	pAttacker = (Attacker*) pObject;

		if ( pAttacker->isFollowingPath ( ) )
		{
			CCLOG ( "Updating path" );
			pAttacker->stopAllActions ( );
			pAttacker->createPathToPlayer ( );
		}
	}
}

KDvoid KnightFight::setHudLayer ( HudLayer* pLayer )
{
	m_pHudLayer = pLayer;
}

const KDchar* KnightFight::getHouseContentsAtTilePos ( const CCPoint& tTilePos )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pDoors, pObject )
	{
		Door*	pDoor = (Door*) pObject;
		if ( pDoor->getTilePos ( ).x == tTilePos.x && pDoor->getTilePos ( ).y == tTilePos.y )
		{
			return pDoor->getContents ( );
		}
	}

	return KD_NULL;
}

KDvoid KnightFight::fireBullet ( const CCPoint& tTarget )
{
	Bullet*		pBullet = Bullet::create ( );	
	pBullet->setVisible ( KD_TRUE );
	pBullet->setPosition ( m_pPlayer->getPosition ( ) );
	this->addChild ( pBullet );
	m_pBullets->addObject ( pBullet );
	pBullet->shootBullet ( tTarget, m_pPlayer );
}

KDvoid KnightFight::destroyBullet ( Bullet* pBullet )
{
	pBullet->stopAllActions ( );
	this->removeChild ( pBullet, KD_TRUE );
	m_pBullets->removeObject ( pBullet );
}

KDvoid KnightFight::startShootOut ( KDfloat fDelta )
{
	this->removeChild ( m_pPowerUpAlert, KD_TRUE );
	GameMgr->shootOut ( );
}

