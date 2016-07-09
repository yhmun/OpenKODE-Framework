/* --------------------------------------------------------------------------
 *
 *      File            GameplayLayer.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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
#include "GameplayLayer.h"
#include "HudLayer.h"
#include "GameObstacle.h"
#include "CollectableItem.h"

KDbool GameplayLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }
    
    this->setTouchEnabled ( KD_TRUE );
    this->setTouchMode ( kCCTouchesOneByOne );

    this->setKeypadEnabled ( KD_TRUE );
    
    // Holds all the obstacles
    m_pObstaclesLayer = CCLayer::create ( );
    this->addChild ( m_pObstaclesLayer, 2 );
    
    // Holds all the items
    m_pItemsLayer = CCLayer::create ( );
    this->addChild ( m_pItemsLayer, 1 );
    
    this->scheduleUpdate ( );
    
    this->setSugarRushActive ( KD_FALSE );
    
    return true;
}

// DEBUG ONLY
KDvoid GameplayLayer::draw ( KDvoid )
{
    CCLayer::draw ( );
   
#if DEBUG_GAME

    if ( GameManager::sharedGameManager ( )->getGameState ( ) == kGameStatePlayGame )
    {
        if ( this->getChildByTag(kMiraSpriteTagValue ) ) 
		{
            CCRect		tMiraBoundingBox = m_pMiraPlayer->adjustedBoundingBox ( );
            this->drawPoly ( tMiraBoundingBox );
        }
    
        if ( m_pObstaclesLayer && m_pObstaclesLayer->getChildrenCount ( ) > 0 )
        {
            for ( KDuint i = 0; i < m_pObstaclesLayer->getChildrenCount ( ); i++ )
            {
                GameObstacle*	pObstacle = (GameObstacle*) m_pObstaclesLayer->getChildren ( )->objectAtIndex ( i );
                this->drawPoly ( pObstacle->adjustedBoundingBox ( ) );
            }
        }
    
        if ( m_pItemsLayer && m_pItemsLayer->getChildrenCount ( ) > 0 )
        {
            for ( KDuint i = 0; i < m_pItemsLayer->getChildrenCount ( ); i++ )
            {
                CollectableItem*	pItem = (CollectableItem*) m_pItemsLayer->getChildren ( )->objectAtIndex ( i );
                this->drawPoly ( pItem->adjustedBoundingBox ( ) );
            }
        }
    }

#endif
}

KDvoid GameplayLayer::update ( KDfloat fDelta )
{
	GameManager*	pManager = GameManager::sharedGameManager ( );

    if ( pManager->getGameState ( ) == kGameStatePlayGame )
    {
        DeviceType	eDevice = pManager->getDeviceType ( );
        if ( eDevice == kDeviceTypeiPhone5 || 
			 eDevice == kDeviceTypeiPhone4 || 
			 eDevice == kDeviceTypeiPhone ||
             eDevice == kDeviceTypeiPad3 || 
			 eDevice == kDeviceTypeiPad )
        {
			m_pMiraEmitter->setSourcePosition ( m_pMiraPlayer->getPosition ( ) );
        }
        
        this->checkCollisions ( );
        
        if ( pManager->getGameState ( ) == kGameStatePlayGame )
        {
            // Loop through updating position of game obstacles
            this->updateObstacles ( );
            
            // Loop through items updating position
            this->updateCollectables ( );
            
            this->performCleanup ( );
            
            // Sugar Rush
            if ( m_bSugarRushActive )
            {
                m_nSugarRushTick--;
                if ( m_nSugarRushTick <= 0)
                {
                    this->resetSugarRush ( );
                }
            }
            
            // Magnet
            if ( m_bMagnetActive )
            {
                m_nMagnetTick--;
                if ( m_nMagnetTick <= 0 )
                {
                    this->resetMagnet ( );
                }
            }
            
            // Increase distance
            if ( m_nDistanceTick >= 50 )
			{
                m_nDistanceTick = 0;
                if ( m_bSugarRushActive )
                {
                    pManager->increaseDistance ( 3 );
                }
                else
                {
                    pManager->increaseDistance ( 1 );
                }
            }
            m_nDistanceTick++;
        }
    }
    else if ( pManager->getGameState ( ) == kGameStateGameOver && m_pMiraPlayer != KD_NULL )
    {
        DeviceType	eDevice = pManager->getDeviceType ( );
        if ( eDevice == kDeviceTypeiPhone5 ||
			 eDevice == kDeviceTypeiPhone4 ||
			 eDevice == kDeviceTypeiPhone ||
             eDevice == kDeviceTypeiPad3 ||
			 eDevice == kDeviceTypeiPad )
        {
            m_pMiraEmitter->setSourcePosition ( m_pMiraPlayer->getPosition ( ) );
        }
        m_pMiraPlayer->setPosition ( ccp ( m_pMiraPlayer->getPosition ( ).x - 6.7f, m_pMiraPlayer->getPosition ( ).y - 6 ) );
        
        if ( m_pMiraPlayer->getPosition ( ).y <= -200 )
        {
            pManager->minVelocity ( );
            m_pMiraPlayer->stopAllActions ( );
            this->removeChild ( m_pMiraPlayer, KD_TRUE );
            pManager->endGame ( );
        }
    }
    
    // Check to make sure music is playing
    if (!SimpleAudioEngine::sharedEngine ( )->isBackgroundMusicPlaying ( ) )
    {
		pManager->playRandomSong ( );
    }
}

KDbool GameplayLayer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
    return KD_TRUE;
}

KDvoid GameplayLayer::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( m_pMiraPlayer != KD_NULL && GameManager::sharedGameManager ( )->getGameState ( ) == kGameStatePlayGame )
	{
        CCPoint		tTouchLocation = this->convertTouchToNodeSpace ( pTouch );
        CCPoint		tOldTouchLocation = this->convertToNodeSpace ( pTouch->getPreviousLocation ( ) );

        CCPoint		tTranslation = ccpSub ( tTouchLocation, tOldTouchLocation );
        this->translateMiraPosition ( tTranslation );
    }
}

KDvoid GameplayLayer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    if ( m_pMiraPlayer != KD_NULL && GameManager::sharedGameManager ( )->getGameState ( ) == kGameStatePlayGame )
	{
        CCAction*	pRotateAction = CCRotateTo::create ( 0.15f, 0 );
		m_pMiraPlayer->runAction ( pRotateAction );
    }
}

KDvoid GameplayLayer::createObjectOfType ( GameObjectType eObjectType, const CCPoint& tSpawnLocation, KDint zValue )
{

}

KDvoid GameplayLayer::updateObstacles ( KDvoid )
{
    for ( KDuint i = 0; i < m_pObstaclesLayer->getChildrenCount ( ); i++ )
    {
        GameObstacle*	pObstacle = (GameObstacle*) m_pObstaclesLayer->getChildren ( )->objectAtIndex ( i );
		GameManager*	pManager  = GameManager::sharedGameManager ( );
        if ( pObstacle->getCharacterState ( ) == kCharacterStateFlying || pObstacle->getCharacterState ( ) == kCharacterStateHit )
		{
            if ( pManager->getDeviceSize ( ) == kDeviceSizeNormal || pManager->getDeviceSize ( ) == kDeviceSizeSmall)
            {
                pObstacle->setPosition ( ccp ( pObstacle->getPosition ( ).x + pManager->getVelocity ( ) + ( pObstacle->getAdjustedVelocity ( ) * 0.5f ), pObstacle->getPosition ( ).y ) );
            }
            else
            {
                pObstacle->setPosition ( ccp ( pObstacle->getPosition ( ).x+ pManager->getVelocity ( ) + pObstacle->getAdjustedVelocity ( ), pObstacle->getPosition ( ).y ) );
            }
        }
    }
}

KDvoid GameplayLayer::updateCollectables ( KDvoid )
{
    for ( KDuint i = 0; i < m_pItemsLayer->getChildrenCount ( ); i++ )
    {
        CollectableItem*	pItem = (CollectableItem*) m_pItemsLayer->getChildren ( )->objectAtIndex ( i );
        if ( !m_bMagnetActive )
        {
            pItem->setPosition ( ccp ( pItem->getPosition ( ).x + GameManager::sharedGameManager ( )->getVelocity ( ) + 2, pItem->getPosition ( ).y ) );
        }
        else
        {
            CCPoint		tCurPos  = pItem->getPosition ( );
            CCPoint		tMiraPos = m_pMiraPlayer->getPosition ( );
            CCPoint		tNewPos;
                            
            tNewPos.x = tCurPos.x - ( tCurPos.x - tMiraPos.x ) * 0.2f;
            tNewPos.y = tCurPos.y - ( tCurPos.y - tMiraPos.y ) * 0.2f;
            
            pItem->setPosition ( tNewPos );
        }
    }    
}

KDvoid GameplayLayer::checkCollisions ( KDvoid )
{
	GameManager*		pManager = GameManager::sharedGameManager ( );
	SimpleAudioEngine*	pSound   = SimpleAudioEngine::sharedEngine ( );

    CCRect				tMiraBoundingBox = m_pMiraPlayer->adjustedBoundingBox ( );
    CharacterState		eMiraState = m_pMiraPlayer->getCharacterState ( );
    	
    // Check Obstacle Collisions
    if ( eMiraState != kCharacterStateHit )
    {		
        for ( KDuint i = 0; i < m_pObstaclesLayer->getChildrenCount ( ); i++ )
        {			
            GameObstacle*	pObstacle = (GameObstacle*) m_pObstaclesLayer->getChildren ( )->objectAtIndex ( i );
            // Avoid the logic if the character has already been hit
            if ( pObstacle->getCharacterState ( ) == kCharacterStateHit )
			{
                continue;
			}

            if ( ( eMiraState == kCharacterStateFlying && m_pMiraPlayer->getVulnerable ( ) ) && pObstacle->getCharacterState ( ) == kCharacterStateFlying && tMiraBoundingBox.intersectsRect ( pObstacle->adjustedBoundingBox ( ) ) )
            {
                if ( pObstacle->getCharacterState ( ) != kCharacterStateHit )
                {
                    pObstacle->changeState ( kCharacterStateHit );
                    if ( !m_bSugarRushActive )
                    {
                        pManager->takeLife ( );
                    }

                    if ( pManager->getLives ( ) <= 0 )
                    {
                        // Game is over
                        pManager->setGameState ( kGameStateGameOver );
                        
                        // Set Mira to Dead
                        m_pMiraPlayer->changeState ( kCharacterStateDead );
                        
                        // Unschedule selectors
                        this->unschedule ( schedule_selector ( GameplayLayer::launchGameObstacle ) );
                        this->unschedule ( schedule_selector ( GameplayLayer::launchCollectableItem ) );
                        
                        // todo force gameover action
                        m_pObstaclesLayer->stopAllActions ( );
                        this->removeChild ( m_pObstaclesLayer, KD_TRUE );
                        m_pItemsLayer->stopAllActions ( );
                        this->removeChild ( m_pItemsLayer, KD_TRUE );
                        
                        // Set Mira to Dead
                        m_pMiraPlayer->changeState ( kCharacterStateDead );
                        
                        // Animate Mira going down
                        pSound->setEffectsVolume ( 1.0f );
                        pSound->playEffect ( "sfx_thud.mp3", KD_FALSE );
                        return;
                    }
                    else
                    {
                        pSound->setEffectsVolume ( 1.0f );
                        pSound->playEffect ( "sfx_thud.mp3", KD_FALSE );
                        if ( !m_bSugarRushActive )
                        {
                            m_pMiraPlayer->changeState ( kCharacterStateHit );
                        }
                        else
                        {
                            // TODO popup score and fade out
                            pManager->increaseScore ( 100 );

                            CCLabelBMFont*		pBonusLabel = CCLabelBMFont::create ( "100", "myGlyphs.fnt" );
                            pBonusLabel->setPosition ( m_pMiraPlayer->getPosition ( ) );
                            this->addChild ( pBonusLabel, 99 );
                            
                            pBonusLabel->runAction
							(
								CCSequence::create
								(
									CCScaleTo::create ( 0.2f, 3.0f ),
									CCFadeOut::create ( 0.2f ),
									CCCallFuncN::create ( this, callfuncN_selector ( GameplayLayer::removeChildSpecified ) ),
									KD_NULL
								)
							);
                        }
                    }

                    if ( !m_bSugarRushActive )
                    {
                        this->cameraShakeAction ( );
                    }
                    break;
                }
            }
        }		
    }
    
    // Check Item Collisions
    for ( KDuint i = 0; i < m_pItemsLayer->getChildrenCount ( ); i++ )
    {
        CollectableItem*	pItem = (CollectableItem*) m_pItemsLayer->getChildren ( )->objectAtIndex ( i );
        if ( tMiraBoundingBox.intersectsRect ( pItem->adjustedBoundingBox ( ) ) )
        {
            if ( eMiraState != kCharacterStateHit )
            {
                if ( pItem->getGameObjectType ( ) == kGameObjectTypePowerUpCandy )
                {
                    this->activateSugarRush ( );
                }
                else if ( pItem->getGameObjectType ( ) == kGameObjectTypePowerUpMagnet )
                {
                    this->activateMagnet ( );
                }
            }
            
            // Increment the score
            if ( pItem->getGameObjectType ( ) != kGameObjectTypePowerUpMagnet && pItem->getGameObjectType ( ) != kGameObjectTypePowerUpCandy )
            {
                pManager->increaseScore ( 10 );
            }
            
            // Play sound
            pSound->setEffectsVolume ( 0.3f );
            pSound->playEffect ( "sfx_collect.mp3", KD_FALSE );
            
            // Remove
            pItem->stopAllActions ( );
            m_pItemsLayer->removeChild ( pItem, KD_TRUE );
        }
    }	
}

KDvoid GameplayLayer::performCleanup ( KDvoid )
{
    // Cleanup Obstacles
    if ( m_pObstaclesLayer->getChildrenCount ( ) > 0 )
    {
        for ( KDuint i = 0; i < m_pObstaclesLayer->getChildrenCount ( ); i++ )
        {
            GameObstacle*	pObstacle = (GameObstacle*) m_pObstaclesLayer->getChildren ( )->objectAtIndex ( i );
            if ( pObstacle->getPosition ( ).x <= -200 )
            {
                pObstacle->stopAllActions ( );
                m_pObstaclesLayer->removeChild ( pObstacle, true );
            }
        }
    }
    
    // Cleanup Items
    if ( m_pItemsLayer->getChildrenCount ( ) > 0)
    {
        for ( KDuint i = 0; i < m_pItemsLayer->getChildrenCount ( ); i++ )
        {
            CollectableItem*	pItem = (CollectableItem*) m_pItemsLayer->getChildren ( )->objectAtIndex ( i );
            if ( pItem->getPosition ( ).x <= -50 )
            {
                pItem->stopAllActions ( );
                m_pItemsLayer->removeChild ( pItem, KD_TRUE );
            }
        }
    }
}

KDvoid GameplayLayer::addObstacle ( KDfloat fDelta )
{
    
}

KDvoid GameplayLayer::startNewGame ( KDvoid )
{	
    m_nDistanceTick = 0;
    
    CCSprite*	pGetReadySprite = CCSprite::createWithSpriteFrameName ( "get_ready.png" );
    pGetReadySprite->setPosition ( ccp ( -pGetReadySprite->getContentSize ( ).cx, SCREEN_HEIGHT / 2 ) );
    pGetReadySprite->setTag ( kGameplayLayer_GetReadySprite );
    
    CCFiniteTimeAction*		pActionSequence = CCSequence::create
	(
        CCEaseIn	::create ( CCMoveTo::create ( 0.6f, ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ) ), 1.0f ),
        CCCallFunc	::create ( this, callfunc_selector ( GameplayLayer::normalVelocity ) ),
        CCDelayTime	::create ( 1.0f ),
        CCEaseOut	::create ( CCMoveTo::create ( 0.6f, ccp ( SCREEN_WIDTH * 2, SCREEN_HEIGHT / 2 ) ), 1.0f ),
        CCCallFunc	::create ( this, callfunc_selector ( GameplayLayer::initHud ) ),
        CCCallFunc	::create ( this, callfunc_selector ( GameplayLayer::initMiraPlayer ) ),
        NULL
	);
    pGetReadySprite->runAction ( pActionSequence );
    this->addChild ( pGetReadySprite );	    
}

KDvoid GameplayLayer::normalVelocity ( KDvoid )
{
    GameManager::sharedGameManager ( )->normalVelocity ( );
}

KDvoid GameplayLayer::initHud ( KDvoid )
{
    HudLayer*	pHudLayer = HudLayer::create ( );
    pHudLayer->setTag ( kHudLayer );
    pHudLayer->setPosition ( SCREEN_WIDTH / 2, SCREEN_HEIGHT );
    
    CCActionInterval*	pMoveToAction = CCMoveTo::create ( 0.6f, ccp ( SCREEN_WIDTH / 2, SCREEN_HEIGHT - ( pHudLayer->getContentSize ( ).cy / 2 ) ) );
    CCActionInterval*	pEaseInAction = CCEaseIn::create ( pMoveToAction, 1.0f );
    pHudLayer->runAction ( pEaseInAction );
    this->addChild ( pHudLayer );
}

KDvoid GameplayLayer::initMiraPlayer ( KDvoid )
{
    this->setMiraPlayer ( Mira::create ( ) );
    m_pMiraPlayer->retain ( );
    
    this->addChild ( m_pMiraPlayer, 10, kMiraSpriteTagValue );
    
    DeviceType	eDeviceType = GameManager::sharedGameManager ( )->getDeviceType ( );
	DeviceSize	eDeviceSize = GameManager::sharedGameManager ( )->getDeviceSize ( );

    if ( eDeviceType == kDeviceTypeiPhone5 || 
		 eDeviceType == kDeviceTypeiPhone4 || 
		 eDeviceType == kDeviceTypeiPhone ||
         eDeviceType == kDeviceTypeiPad3 || 
		 eDeviceType == kDeviceTypeiPad )
    {
        m_pMiraEmitter = CCParticleMeteor::create ( );
        m_pMiraEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "stars.png" ) );
        if ( eDeviceSize == kDeviceSizeXLarge || eDeviceSize == kDeviceSizeLarge )
        {
            m_pMiraEmitter->setStartSize ( 40 );
        }
        else
        {
            m_pMiraEmitter->setStartSize ( 20 );
        }
        m_pMiraEmitter->setEndSize ( 2 );
        m_pMiraEmitter->setRadialAccel ( 30 );
        m_pMiraEmitter->setGravity ( ccp ( -1430, 0 ) );
        m_pMiraEmitter->setPosition ( m_pMiraPlayer->getPosition ( ) );
        this->addChild ( m_pMiraEmitter);
    }
    
    m_pMiraPlayer->setPosition ( ccp ( -( m_pMiraPlayer->getContentSize ( ).cx * 2 ), SCREEN_HEIGHT / 2 ) );

    CCActionInterval*		pMoveMiraIn = CCMoveTo::create ( 0.3f, ccp ( SCREEN_WIDTH * 0.20f, SCREEN_HEIGHT / 2 ) );
    CCFiniteTimeAction*		pMoveInEffect = CCEaseIn::create ( pMoveMiraIn, 1.0f );
    CCFiniteTimeAction*		pInitGameplayAction = CCCallFunc::create ( this, callfunc_selector ( GameplayLayer::initGameplay ) );
    CCSequence*				pSequence = CCSequence::createWithTwoActions ( pMoveInEffect, pInitGameplayAction );
    m_pMiraPlayer->runAction ( pSequence );
}

KDvoid GameplayLayer::initGameplay ( KDvoid )
{
    SimpleAudioEngine::sharedEngine ( )->setEffectsVolume ( 1.0f );
    SimpleAudioEngine::sharedEngine ( )->playEffect ( "sfx_laugh.mp3", KD_FALSE );
    
    GameManager::sharedGameManager ( )->setGameState ( kGameStatePlayGame );
    
    this->schedule ( schedule_selector ( GameplayLayer::launchGameObstacle ), GameManager::randomValueBetween ( 2.0f, 3.0f ) );
    this->schedule ( schedule_selector ( GameplayLayer::launchCollectableItem ), 0.3f );
}

KDvoid GameplayLayer::launchCollectableItem ( KDfloat fDelta )
{
	KDint	nCollectableItemType = ( kdRand ( ) % 5 ) + 1;
	if ( ( kdRand ( ) % 10 ) == 5 )
	{
		if ( ( kdRand ( ) % 10 ) <= 2 ) 
		{
			nCollectableItemType = 7 - ( kdRand ( ) % 2 );
		}
	}

	GameObjectType		eType = kGameObjectTypeCollectableFlower1;
	switch ( nCollectableItemType )
	{
		case  2 :	eType = kGameObjectTypeCollectableFlower2;	break;
		case  3 :	eType = kGameObjectTypeCollectableFlower3;	break;
		case  4 :	eType = kGameObjectTypeCollectableFlower4;	break;
		case  5 :	eType = kGameObjectTypeCollectableFlower5;	break;
		case  6 :	eType = kGameObjectTypePowerUpCandy;		break;
		case  7 :	eType = kGameObjectTypePowerUpMagnet;		break;	        
		default :	eType = kGameObjectTypeCollectableFlower1;	break;
	}

	CollectableItem*	pItem = CollectableItem::create ( eType );
	switch ( m_nPattern ) 
	{
		case 1 :     // Horizontal alignment on the top
			if ( m_nPatternCount == 0 )
			{
				// Choose a Y value above the middle
				m_nLastYPos = (KDint) GameManager::randomValueBetween ( SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT - SCREEN_HEIGHT * 0.211f );
				m_nPatternCount++;
			}
			else if ( m_nPatternCount >= 10 ) 
			{
				this->changePattern ( );
			}
			else 
			{
				m_nPatternCount++;
			}
			pItem->setPosition ( ccp ( SCREEN_WIDTH + 50, m_nLastYPos ) );
			break;
		
		case 2 :     // ZigZag
			if ( m_nPatternCount == 0 )
			{
				m_nLastYPos = (KDint) GameManager::randomValueBetween ( SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT - SCREEN_HEIGHT * 0.211f );
				m_nPatternCount++;
				KDint	nMid = (KDint) ( ( SCREEN_HEIGHT - SCREEN_HEIGHT * 0.211f ) / 2 );
				if ( m_nLastYPos > nMid )
				{
					// going up
					m_nPatternDirection = 0;
				}
				else 
				{
					// going down
					m_nPatternDirection = 1;
				}
			} 
			else if ( m_nPatternDirection >= 20 )
			{
				this->changePattern ( );
			}
			else 
			{
				if ( m_nPatternDirection == 0 )
				{
					if ( ( m_nLastYPos - pItem->getContentSize ( ).cy ) < ( SCREEN_HEIGHT * 0.12f ) ) 
					{
						// go down
						m_nPatternDirection = 1;
					}
				} 
				else 
				{
					if ( ( m_nLastYPos + pItem->getContentSize ( ).cy ) > ( SCREEN_HEIGHT - SCREEN_HEIGHT * 0.211f ) )
					{
						// go up
						m_nPatternDirection = 0;
					}
				}
				if ( m_nPatternDirection == 0 )
				{
					m_nLastYPos = m_nLastYPos - (KDint) pItem->getContentSize ( ).cy;
				} 
				else 
				{
					m_nLastYPos = m_nLastYPos + (KDint) pItem->getContentSize ( ).cy;
				}
	            
				m_nPatternCount++;
			}
			pItem->setPosition ( ccp ( SCREEN_WIDTH + 50, m_nLastYPos ) );
			break;

		default :
			KDfloat		fRandYPos = GameManager::randomValueBetween ( SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT - SCREEN_HEIGHT * 0.211f );
			pItem->setPosition ( ccp ( SCREEN_WIDTH + 50, fRandYPos ) );
	        
			m_nPatternCount++;
	        
			if ( m_nPatternCount > 20 )
			{
				this->changePattern ( );
			}
	}

	m_pItemsLayer->addChild ( pItem );
	pItem->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 1.0f, 360 ) ) );	
}

KDvoid GameplayLayer::changePattern ( KDvoid )
{
    m_nPattern = kdRand ( ) % 3;
    m_nPatternCount = 0;
}

KDvoid GameplayLayer::launchGameObstacle ( KDfloat fDelta )
{
    KDint			nObstacleType = ( kdRand ( ) % 5 ) + 1;
    GameObjectType	eType = kGameObjectTypeObstacleRedBird;
    switch ( nObstacleType )
	{
        case  2 :	eType = kGameObjectTypeObstacleBlueBird;	break;            
        case  3 :	eType = kGameObjectTypeObstacleBat;			break;            
        case  4 :	eType = kGameObjectTypeObstacleSquirrel;	break;            
        case  5	:	eType = kGameObjectTypeObstacleLadyBug;		break;            
        default :	eType = kGameObjectTypeObstacleRedBird;		break;
    }
    
    GameObstacle*	pObstacle = GameObstacle::create ( eType );
    pObstacle->changeState ( kCharacterStateIdle );
    m_pObstaclesLayer->addChild ( pObstacle );
    
    KDfloat			fRandYPos = GameManager::randomValueBetween ( SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT - SCREEN_HEIGHT * 0.211f );
    pObstacle->setPosition ( ccp ( SCREEN_WIDTH * 0.9f, fRandYPos ) );
    
    this->runAction 
	(
		CCSequence::create
		(
			CCDelayTime::create ( GameManager::randomValueBetween ( 2.0f, 5.0f ) ), 
			CCCallFunc ::create ( pObstacle, callfunc_selector ( GameObstacle::changeStateToSpawning ) ),
			CCDelayTime::create ( 3.0f ), 
			CCCallFunc ::create ( pObstacle, callfunc_selector ( GameObstacle::changeStateToFlying ) ),
			KD_NULL
		) 
	);	
}

KDvoid GameplayLayer::drawPoly ( const CCRect& tRect )
{    
    // draw bounding box
    CCPoint		aVertices [ 4 ] = 
	{
        ccp ( tRect.origin.x				, tRect.origin.y ),
        ccp ( tRect.origin.x + tRect.size.cx, tRect.origin.y ),
        ccp ( tRect.origin.x + tRect.size.cx, tRect.origin.y + tRect.size.cy ),
        ccp ( tRect.origin.x				, tRect.origin.y + tRect.size.cy ),
    };
    ccDrawPoly ( aVertices, 4, KD_TRUE );
}

KDvoid GameplayLayer::translateMiraPosition ( const CCPoint& tTranslation )
{
    if ( m_pMiraPlayer->getCharacterState ( ) == kCharacterStateHit )
	{
        return;
	}
    
    CCPoint		tNewPos = ccpAdd ( m_pMiraPlayer->getPosition ( ), tTranslation );

    KDfloat		fMinY = SCREEN_HEIGHT * 0.12f;
    KDfloat		fMaxY = SCREEN_HEIGHT - ( SCREEN_HEIGHT * 0.211f );
    KDfloat		fMinX = SCREEN_WIDTH * 0.14f;
    KDfloat		fMaxX = SCREEN_WIDTH - ( SCREEN_WIDTH * 0.22f );

    if ( tNewPos.x < fMinX )
	{
        tNewPos.x = fMinX;
    }
	else if ( tNewPos.x > fMaxX )
	{
        tNewPos.x = fMaxX;
    }

    if ( tNewPos.y < fMinY )
	{
        tNewPos.y = fMinY;
    }
	else if ( tNewPos.y > fMaxY )
	{
        tNewPos.y = fMaxY;
    }
    
    KDfloat		fRotation = -( tTranslation.y * 1.4f );
    if ( fRotation > 20.0f )
	{
        fRotation = 20.0f;
    }
	else if ( fRotation < -20.0f )
	{
        fRotation = -20.0f;
    }
    
    m_pMiraPlayer->setRotation ( fRotation );
    m_pMiraPlayer->setPosition ( tNewPos );
}

KDvoid GameplayLayer::cameraShakeAction ( KDvoid )
{
	this->runAction 
	(
		CCRepeat::create 
		(
			CCSequence::create
			(
				CCDelayTime::create ( 0.023f ),
				CCCallFunc ::create ( this, callfunc_selector ( GameplayLayer::cameraShake ) ),
				KD_NULL 
			),
			4
		)		 
	);
}

KDvoid GameplayLayer::cameraShake ( KDvoid )
{
    BackgroundLayer*	pBackgroundLayer = GameManager::sharedGameManager ( )->getBackgroundLayer ( );
    CCPoint				tBackgroundPosition = pBackgroundLayer->getPosition ( );
    
    if ( pBackgroundLayer->getScale ( ) == 1.05f )
	{
        pBackgroundLayer->setScale ( 1.0f );
    }
    else
    {
        pBackgroundLayer->setScale ( 1.05f );
    }
    
    if ( this->getScale ( ) == 1.05f )
	{
        this->setScale ( 1.0f );
    }
    else
    {
        this->setScale ( 1.05f );
    }
}

KDvoid GameplayLayer::activateSugarRush ( KDvoid )
{
    m_nSugarRushTick = kMiraSugarRushTime;
    this->setSugarRushActive ( KD_TRUE );

    GameManager::sharedGameManager ( )->maxVelocity ( );
    GameManager::sharedGameManager ( )->getBackgroundLayer ( )->showWind ( );
    
	SimpleAudioEngine::sharedEngine ( )->setEffectsVolume ( 1.0f );
    SimpleAudioEngine::sharedEngine ( )->playEffect ( "sfx_laugh.mp3", KD_FALSE );
}

KDvoid GameplayLayer::resetSugarRush ( KDvoid )
{
    m_nSugarRushTick = kMiraSugarRushTime;
    GameManager::sharedGameManager ( )->normalVelocity ( );
    this->setSugarRushActive ( KD_FALSE );
    GameManager::sharedGameManager ( )->getBackgroundLayer ( )->hideWind ( );
}

KDvoid GameplayLayer::activateMagnet ( KDvoid )
{
    m_nMagnetTick = kMiraMagnetTime;
    this->setMagnetActive ( KD_TRUE );
}

KDvoid GameplayLayer::resetMagnet ( KDvoid )
{
    m_nMagnetTick = kMiraMagnetTime;
    this->setMagnetActive ( KD_FALSE );
}

KDvoid GameplayLayer::removeChildSpecified ( CCNode* pSender )
{
    CCSprite*	pSprite = (CCSprite*) pSender;
    this->removeChild ( pSprite, KD_TRUE );
}
