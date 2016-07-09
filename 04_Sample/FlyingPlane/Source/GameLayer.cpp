/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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
#include "ShipSprite.h"
#include "BlockCache.h"
#include "RoofCache.h"

#define SESSION_SCORE_BEST "ScoreBest"

typedef enum
{
    GameLayerTagGame = 1,
} GameLayerTags;

typedef enum
{
    GameLayerNodeTagShip = 1,
    GameLayerNodeTagBlockCache,
    GameLayerNodeTagRoofCache,
} GameLayerNodeTags;


GameLayer::GameLayer ( KDvoid )
{
	m_bGameOver = KD_FALSE;

    m_fTotalTime = 0;
    m_nScore     = 0;

    m_pScoreBest     = KD_NULL;
    m_pScoreDistance = KD_NULL;
}


KDbool GameLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }

    this->setTouchEnabled ( KD_TRUE );
    
    // 화면 크기
	const CCSize&	tLyrSize = this->getContentSize ( );

    m_tScreenRect   = CCRectMake ( 0, 0, tLyrSize.width, tLyrSize.height );
    
    CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/game-art.plist" );
    
    // 비행기 생성
    this->addChild( ShipSprite::create ( this ), 0, GameLayerNodeTagShip );

    // 최고 점수 라벨
    KDint nScoreBest = CCUserDefault::sharedUserDefault ( )->getIntegerForKey(SESSION_SCORE_BEST);
    m_pScoreBest     = CCLabelTTF::create ( ccszf ( "BEST:%d", nScoreBest), "Font/arial.ttf", 20 );
    m_pScoreBest->setPosition( ccp ( m_pScoreBest->getContentSize ( ).cx / 2 + 10, 20 ) );
    this->addChild(m_pScoreBest, 100);
    
    // 현재 점수 라벨
    m_pScoreDistance = CCLabelTTF::create ( ccszf ( "DISTANCE:%d", 0 ), "Font/arial.ttf", 20 );
    m_pScoreDistance->setAnchorPoint ( ccp ( 1, 0 ) );
    m_pScoreDistance->setPosition ( CCPointMake ( m_tScreenRect.size.width - 10, 10 ) );
    this->addChild ( m_pScoreDistance, 100, 99 );
    
    // 현재 점수 라벨 색 변경 효과
    m_pScoreDistance->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCTintTo::create ( 2, 255,   0,   0 ),
				CCTintTo::create ( 2, 255, 255,   0 ),
				CCTintTo::create ( 2,   0, 255,   0 ),
				CCTintTo::create ( 2,   0, 255, 255 ),
				CCTintTo::create ( 2,   0,   0, 255 ),
				CCTintTo::create ( 2, 255,   0, 255 ),
				KD_NULL
			)
		)
	);

    // 배경 생성
    this->addChild ( BlockCache::create ( this ), 0, GameLayerNodeTagBlockCache );
    this->addChild ( RoofCache::create ( this ), 0, GameLayerNodeTagRoofCache );

    this->scheduleUpdate ( );
    
    return KD_TRUE;
}


ShipSprite* GameLayer::getShip ( KDvoid )
{    
	return ( ShipSprite* ) this->getChildByTag ( GameLayerNodeTagShip );
}


CCSize GameLayer::getScreenSize ( KDvoid )
{
    return m_tScreenRect.size;
}


KDvoid GameLayer::doGameOver ( KDvoid )
{
    // 현재 점수가 기록된 점수보다 크다면 저장한다.
    if ( m_nScore > CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( SESSION_SCORE_BEST ) )
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey ( SESSION_SCORE_BEST, m_nScore );
    }
    
    this->showGameOver ( );
    m_bGameOver = KD_TRUE;
}


KDvoid GameLayer::showGameOver ( KDvoid )
{    
    CCObject*          pObj1           = KD_NULL;
    CCObject*          pObj2           = KD_NULL;
    
    CCARRAY_FOREACH ( this->getChildren ( ), pObj1 )
    {
        CCNode* node = ( CCNode* ) pObj1;
        CCARRAY_FOREACH(node->getChildren ( ), pObj2 )
        {
            CCNode* subnode = ( CCNode* ) pObj2;
            subnode->unscheduleAllSelectors ( );
        }
        node->unscheduleAllSelectors ( );
    }
    this->unscheduleAllSelectors ( );

	const CCSize&      tLyrSize        = this->getContentSize ( );

    // 게임 종료 라벨
    CCLabelTTF*        pGameOver       = CCLabelTTF::create ( "GAME OVER", "Font/arial.ttf", 60 );
    pGameOver->setPosition ( CCPointMake ( tLyrSize.width / 2, tLyrSize.height / 3 ) );
    this->addChild ( pGameOver, 100, 100 );
    
	// 게임 종료 라벨 색 변경 효과
    pGameOver->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCTintTo::create ( 2, 255,   0,   0 ),
				CCTintTo::create ( 2, 255, 255,   0 ),
				CCTintTo::create ( 2,   0, 255,   0 ),
				CCTintTo::create ( 2,   0, 255, 255 ),
				CCTintTo::create ( 2,   0,   0, 255 ),
				CCTintTo::create ( 2, 255,   0, 255 ),
				KD_NULL
			)
		)
	);

	// 게임 종료 라벨 회전 효과
    pGameOver->runAction
	(
		CCRepeatForever::create
		(
			CCSequence::create
			(
				CCEaseBounceInOut::create ( CCRotateTo::create ( 2,  3 ) ),
				CCEaseBounceInOut::create ( CCRotateTo::create ( 2, -3 ) ),
				KD_NULL
			)
		)
	);

	// 게임 종료 라벨 점프 효과
    pGameOver->runAction
	(
		CCRepeatForever::create
		(
			CCJumpBy::create ( 3, CCPointZero, tLyrSize.height / 3, 1 )
		)
	);

	// 화면을 누르면 다시 게임 시작을 한다.
    CCLabelTTF*        pTouch          = CCLabelTTF::create( "tap screen to play again", "Font/arial.ttf", 20);
    pTouch->setPosition( CCPointMake ( tLyrSize.width / 2, tLyrSize.height / 4) );
    this->addChild ( pTouch ,100, 101 );
  
	// 다시 게임 시작 문구의 깜빡인 효과
    pTouch->runAction
	(
		CCRepeatForever::create
		(
			CCBlink::create ( 10, 20 )
		)
	);
}


KDvoid GameLayer::doResetGame ( KDvoid )
{
	CCScene   *pScene = CCScene::create ( );
    CCLayer   *pLayer = GameLayer::create ( );
    pScene->addChild ( pLayer );
    CCDirector::sharedDirector ( )->replaceScene( pScene );
}


KDvoid GameLayer::ccTouchesBegan ( CCSet *pTouches, CCEvent *pEvent )
{
    if ( !m_bGameOver )
    {
        this->getShip ( )->up ( );
    }
}


KDvoid GameLayer::ccTouchesEnded ( CCSet *pTouches, CCEvent *pEvent )
{
    if ( m_bGameOver )
    {
        this->doResetGame ( );
    }
    else
    {
        this->getShip ( )->down ( );
    }
}


KDvoid GameLayer::ccTouchesCancelled ( CCSet *pTouches, CCEvent *pEvent )
{
    this->getShip ( )->down ( );
}


KDvoid GameLayer::update ( KDfloat fDelta )
{
    m_fTotalTime += fDelta;
    KDint nCurrentTime = ( KDint ) ( m_fTotalTime * 50 );
    if ( m_nScore < nCurrentTime )
    {
        m_nScore = nCurrentTime;
        m_pScoreDistance->setString ( ccszf ( "DISTANCE:%d", m_nScore ) );
    }
}
