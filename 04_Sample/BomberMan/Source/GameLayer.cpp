/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "GameHero.h"
#include "GameEnemy.h"
#include "MainScene.h"

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	GameInf*	pGameinf = GameInf::sharedGameInf ( );

	m_nW	 = 416;
	m_nH	 = 288;
	m_nTileW = 32;
	m_nTileH = 32;	
	m_nLevel = pGameinf->getLevel ( );
	m_nScore = pGameinf->getScore ( );
	m_nTime  = 0;
        
	this->reInit ( );

	return KD_TRUE;
}

GameLayer::GameLayer ( KDvoid )
{
    m_pNullPoints = KD_NULL;
    m_pTileArray  = KD_NULL;
    m_pEnemyArray = KD_NULL;
}

GameLayer::~GameLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNullPoints );
	CC_SAFE_RELEASE ( m_pTileArray  );
	CC_SAFE_RELEASE ( m_pEnemyArray );
}

KDvoid GameLayer::initTile ( KDvoid )
{
	
	CCSpriteFrameCache*		pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	CCSpriteFrame*		pShitouFrame    = pFrameCache->spriteFrameByName ( "shitou.png" );
	CCSpriteFrame*		pZhuankuaiFrame = pFrameCache->spriteFrameByName ( "zhuankuai.png" );

	KDint	i, j;

	for ( i = 3; i <= m_nW / TileW; i += 2 )
	{
		for ( j = 2; j <= m_nH / TileH; j += 2 )
		{			
			CCSprite*	pShitou = CCSprite::createWithSpriteFrame ( pShitouFrame );
			this->addChild ( pShitou );
			pShitou->setTag ( 0 );
			pShitou->setPosition ( ccp ( ( i - 0.5 ) * TileW, ( j - 0.5 ) * TileH ) );
			m_pTileArray->addObject ( pShitou );			
		}
	}

	for ( i = 2; i <= m_nW / TileW + 1; i += 1 ) 
	{
		for ( j = 1; j <= m_nH / TileH + 1; j += 2 )
		{
			if ( ( i== 2 && j == 1 ) || ( i== 2 && j == 2 ) || ( i== 3 && j == 1 ) ) 
			{
				continue;
			}

			if ( kdRand ( ) % 10 < 1 ) 
			{
				CCSprite*	pZhuankuai = CCSprite::createWithSpriteFrame ( pZhuankuaiFrame );
				this->addChild ( pZhuankuai );
				pZhuankuai->setTag ( 1 );
				pZhuankuai->setPosition ( ccp ( ( i - 0.5 ) * TileW, ( j - 0.5 ) * TileH ) );
				m_pTileArray->addObject ( pZhuankuai );	
			}
			else 
			{
				CCPoint		tPoint = ccp ( ( i - 0.5 ) * TileW, ( j - 0.5 ) * TileH );
				m_pNullPoints->addObject ( CCPointValue::create ( tPoint ) );
			}	    
		}
	} 
}

KDvoid GameLayer::initHero ( KDvoid )
{
	m_pHero = GameHero::create ( m_tHeroStartPostion, this );
	m_pHero->setTileArray ( m_pTileArray );
}

KDvoid GameLayer::initEnemy ( KDvoid )
{
    for ( KDint i = 0; i < EnemyNum; i++ ) 
    {        
		CCPointValue*	pPoint = (CCPointValue*) m_pNullPoints->objectAtIndex ( kdRand ( ) % m_pNullPoints->count ( ) );
		GameEnemy*		pEnemy = GameEnemy::create ( pPoint->getValue ( ), this );
        m_pEnemyArray->addObject ( pEnemy );
		pEnemy->setTileArray ( m_pTileArray );
		m_pNullPoints->removeObject ( pPoint );  
    }
}

KDvoid GameLayer::initBomb ( KDvoid )
{
	m_pBomb = CCSprite::createWithSpriteFrameName ( "bomb.png" );
	m_pBomb->setVisible ( KD_FALSE );
	m_pBomb->runAction 
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCScaleTo::create ( 0.5f, 1.2f ),
				CCScaleTo::create ( 0.5f, 0.8f ),
				KD_NULL
			)
		)
	);

	this->addChild ( m_pBomb, -1 );
	m_pTileArray->addObject ( m_pBomb );

	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "dingshi.mp3" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "baozha.mp3" );
}

KDvoid GameLayer::gameUpdate ( KDfloat fDelta )
{
	this->updateTime     ( );
	this->checkHeroState ( );
	this->checkGameState ( );
}

KDvoid GameLayer::checkHeroState ( KDvoid )
{
    switch ( m_pHero->getAct ( ) ) 
    {
        case  1 :	m_pHero->moveUp    ( );	break;
        case  2 :	m_pHero->moveLeft  ( );	break;
        case  3 :	m_pHero->moveRight ( );	break;
        case  4 :	m_pHero->moveDown  ( );	break;
        case  5 :	this->putBomb	   ( );	break;
        case  6 :	m_pHero->onStay    ( );	break;
		default :	break;
    }
	
    for ( KDuint i = 0; i < m_pEnemyArray->count ( ); i++ )
    {
        GameEnemy*	pEnemy = (GameEnemy*) m_pEnemyArray->objectAtIndex ( i );
        if ( ccpDistance ( pEnemy->getPosition ( ), m_pHero->getPosition ( ) ) < TileW - kdFabsf ( pEnemy->getSpeed ( ) ) )
        {
            this->gameOver ( );
        }            
    }	
}

KDvoid GameLayer::checkGameState ( KDvoid )
{
	if ( m_pEnemyArray->count ( ) == 0 &&
		 m_pHero->getPosition ( ).x == m_pKey->getPosition ( ).x &&
		 m_pHero->getPosition ( ).y == m_pKey->getPosition ( ).y ) 
    {
        this->gameWin ( );
    }
   
    if ( m_nTime <= 0 ) 
    {
		this->gameOver ( );
    }
}

KDvoid GameLayer::updateTime ( KDvoid )
{
    static KDint	i = 0;
    if ( i == 10 ) 
    {
        m_nTime--;
        m_pTimeLabel->setString ( ccszf ( "time: %d", m_nTime ) );
        i = 0;
    }
    i++;
}

KDvoid GameLayer::gameOver ( KDvoid )
{
	this->unschedule ( schedule_selector ( GameLayer::gameUpdate ) );

	GameInf*	pGameinf = GameInf::sharedGameInf ( );

    pGameinf->setScore ( 0 );
    pGameinf->setLevel ( 1 );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionShrinkGrow::create ( 3, MainScene::scene ( ) ) );
}

KDvoid GameLayer::gameWin ( KDvoid )
{
	this->unschedule ( schedule_selector ( GameLayer::gameUpdate ) );

	GameInf*	pGameinf = GameInf::sharedGameInf ( );

    pGameinf->setScore ( m_nScore );
    pGameinf->setLevel ( pGameinf->getLevel ( ) + 1 );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionShrinkGrow::create ( 3, MainScene::scene ( ) ) );
}

KDvoid GameLayer::reInit ( KDvoid )
{
	CCSprite*	pLeftBoard  = CCSprite::create ( "board.png" );
	pLeftBoard->setAnchorPoint ( ccpz );
	pLeftBoard->setPosition ( ccp ( 0, TileH ) );
	this->addChild ( pLeftBoard );

    CCSprite*	pRightBoard = CCSprite::create ( "board.png" );    
	pRightBoard->setAnchorPoint ( ccpz );
	pRightBoard->setPosition ( ccp ( 14 * TileW, TileH ) );	
	this->addChild ( pRightBoard );

	m_pScoreLabel = CCLabelTTF::create ( ccszf ( "score: %d", m_nScore ), "Marker Felt.ttf", 32 );
	m_pScoreLabel->setPosition ( ccp ( 11.5 * TileW, WinH - TileH / 2 ) );
	this->addChild ( m_pScoreLabel );

	m_pLevelLabel = CCLabelTTF::create ( ccszf ( "level: %d", m_nLevel ), "Marker Felt.ttf", 32 );
	m_pLevelLabel->setPosition ( ccp ( 2.5 * TileW, WinH - TileH / 2 ) );
	this->addChild ( m_pLevelLabel );

	m_pTimeLabel = CCLabelTTF::create ( ccszf ( "time: %d", m_nTime ), "Marker Felt.ttf", 32 );
	m_pTimeLabel->setPosition ( ccp ( 7 * TileW, WinH - TileH / 2 ) );
	this->addChild ( m_pTimeLabel );

	m_nTime   = 120;
	m_bIsBomb = KD_FALSE;
	m_bIsKey  = KD_FALSE;

    m_pNullPoints = new CCArray ( );
    m_pTileArray  = new CCArray ( );
    m_pEnemyArray = new CCArray ( );

	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "chaojimali.mp3", KD_TRUE );

	m_tHeroStartPostion = ccp ( 48, 16 );

	this->initTile  ( );
	this->initHero  ( );
	this->initEnemy ( );
	this->initBomb  ( );

	this->schedule ( schedule_selector ( GameLayer::gameUpdate ), 1 / 10.f );
}

KDvoid GameLayer::putBomb ( KDvoid )
{
    if ( m_bIsBomb ) 
    {
        return;
    }
    m_pBomb->setVisible ( KD_TRUE );
	
	this->schedule ( schedule_selector ( GameLayer::bombDown ), 3 );   

	m_pBomb->setPosition ( this->convertToTilePos ( m_pHero->getPosition ( ) ) );
    m_bIsBomb = KD_TRUE;

    SimpleAudioEngine::sharedEngine ( )->playEffect ( "dingshi.mp3" );
}

KDvoid GameLayer::putBombEffect ( KDvoid )
{
	m_pBombEffect = CCSprite::createWithSpriteFrameName ( "bombEffects.png" );
	m_pBombEffect->setScale ( 0.333f );
	m_pBombEffect->setPosition ( m_pBomb->getPosition ( ) );
	m_pBombEffect->runAction 
	(
		CCSequence::create 
		(
			CCScaleTo::create ( 0.3f, 1 ),
			CCHide::create ( ),
			KD_NULL
		)
	);
	this->addChild ( m_pBombEffect, -1 );
}

KDvoid GameLayer::bombDown ( KDfloat fDelta )
{	
    for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
    {
		CCSprite*	pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
	
        if ( ccpDistance ( pTile->getPosition ( ), m_pBomb->getPosition ( ) ) <= TileH && pTile->getTag ( ) == 1 ) 
        {
			m_pTileArray->removeObject ( pTile );
			pTile->removeFromParentAndCleanup ( KD_FALSE );
                   
            m_nScore += 10;

            i--;

            m_pScoreLabel->setString ( ccszf ( "score: %d", m_nScore ) );
        }

        if ( m_pTileArray->count ( ) == 28 && !m_bIsKey )
        {				
            m_pKey = CCSprite::create ( "yaoshi.png" );
			this->addChild ( m_pKey, -1 );
           
			CCSprite*	pTile2 = (CCSprite*) m_pTileArray->objectAtIndex ( kdRand ( ) % 3 + 24 );
			m_pKey->setPosition ( pTile2->getPosition ( ) );
            m_bIsKey = KD_TRUE;			           
        }              
    }

	for ( KDuint i = 0; i < m_pEnemyArray->count ( ); i++ ) 
    {
		GameEnemy*	pEnemy = (GameEnemy*) m_pEnemyArray->objectAtIndex ( i );

        if ( ( kdFabsf ( m_pBomb->getPosition ( ).x - pEnemy->getPosition ( ).x ) < TileW || 
			   kdFabsf ( m_pBomb->getPosition ( ).y - pEnemy->getPosition ( ).y ) < TileH ) && 
			   ccpDistance ( m_pBomb->getPosition ( ), pEnemy->getPosition ( ) ) <= TileW * 2 ) 
        {
            m_pEnemyArray->removeObject ( pEnemy );
			pEnemy->unschedule ( schedule_selector ( GameEnemy::enemyMove ) );
            pEnemy->enemyKill ( );
            m_nScore += 100;
            i--;
            m_pScoreLabel->setString ( ccszf ( "score: %d", m_nScore ) );            
        }
    }

    if ( ( kdFabsf ( m_pBomb->getPosition ( ).x - m_pHero->getPosition ( ).x ) < TileW || 
		   kdFabsf ( m_pBomb->getPosition ( ).y - m_pHero->getPosition ( ).y ) < TileH ) && 
		   ccpDistance ( m_pBomb->getPosition ( ), m_pHero->getPosition ( ) ) < TileW * 2 )
    {
        this->gameOver ( );
    }
	
    m_pBomb->setVisible ( KD_FALSE );
    
    m_bIsBomb = KD_FALSE;
    this->putBombEffect ( );
    m_pBomb->setPosition ( ccp ( -16, -16 ) );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "baozha.mp3" );

	this->unschedule ( schedule_selector ( GameLayer::bombDown ) );
}

KDvoid GameLayer::loadNextlevel ( KDint nLevel )
{
    m_nLevel = nLevel;
    
	this->reInit ( );
}

CCPoint GameLayer::convertToTilePos ( const CCPoint& tPos )
{
	KDint	x = (KDint) ( tPos.x / TileW );
	KDint	y = (KDint) ( tPos.y / TileH );
	return ccp ( ( x + 0.5f ) * TileW, ( y + 0.5f ) * TileH );
}

GameHero* GameLayer::getHero ( KDvoid )
{
	return m_pHero;
}