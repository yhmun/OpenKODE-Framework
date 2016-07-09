/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *      Description     
 *      Author          Hae-Won Lee
 *      Contact         leehw000@gmail.com
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
#include "GameOver.h"

GameLayer::GameLayer ( KDvoid )
{
	m_pTargets = KD_NULL;
}

GameLayer::~GameLayer ( KDvoid )
{
	CC_SAFE_DELETE ( m_pTargets );
}

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	// 기본 배경 화면 설정
	CCSprite*	pBackground1 = CCSprite::create ( "Images/space_bg.png" );
	CCSprite*	pBackground2 = CCSprite::create ( "Images/space_bg.png" );

	pBackground1->setAnchorPoint ( ccp ( 0, 0 ) );
	pBackground2->setAnchorPoint ( ccp ( 0, 0 ) );

	CCParallaxNode*		pVoidNode = CCParallaxNode::create ( );
	pVoidNode->addChild ( pBackground1, 1, ccp ( 0 , 1.0f ), ccpz );
	pVoidNode->addChild ( pBackground2, 1, ccp ( 0 , 1.0f ), ccp ( 0, 480 ) );

	CCLayer*	pGameLayer = CCLayer::create ( );
	pGameLayer->addChild ( pVoidNode, 2 );

	pGameLayer->runAction 
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCMoveBy::create ( 4, ccp ( 0, -480 ) ),
				CCMoveBy::create ( 4, ccp ( 0,  480 ) ),
				KD_NULL
			)
		)
	);

	this->addChild ( pGameLayer );


	m_pTargets = new CCArray ( );

    // 벽돌 개수 지정
    BRICK_HEIGHT = 3;
    BRICK_WIDTH  = 6;

	m_bIsPlaying		= KD_FALSE;
	m_bIsPaddleTouched	= KD_FALSE;
    
    // 벽돌 초기화
	this->initializeBricks ( );
    
    // 공 초기화
    this->initializeBall   ( );
    
    // 패들 초기화
    this->initializePaddle ( );
    
    // dragon 초기화
    this->initializeDragon ( );
    
    // 2초후 게임 시작
	this->scheduleOnce ( schedule_selector ( GameLayer::startGame ), 1.f );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}		

KDvoid GameLayer::initializeBricks ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    KDint	nCount = 0;
    
    for ( KDint y = 0; y < BRICK_HEIGHT; y++ )
    {
        for ( KDint x = 0; x < BRICK_WIDTH; x++ )
        {
			CCSprite*	pBricks = CCSprite::create ( );
			pBricks->setTextureRect ( CCRect ( 0, 0, 50, 15 ) );
   
            // 벽돌의 색 지정
            switch ( nCount++ % 4 )
            {
                case 0 : pBricks->setColor ( ccc3 ( 255, 255,  50 ) ); break;
                case 1 : pBricks->setColor ( ccc3 ( 255,   0,   0 ) ); break;
				case 2 : pBricks->setColor ( ccc3 ( 255,  50, 255 ) ); break;
				case 3 : pBricks->setColor ( ccc3 (  75, 255,   0 ) ); break;
            }
            
            pBricks->setPosition ( ccp ( x * 50 + 35, ( y * 25 ) + tLyrSize.cy - 200 ) );
            
            this->addChild ( pBricks );
            
            m_pTargets->addObject ( pBricks );       
        }
    }
}

KDvoid GameLayer::initializeBall ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	m_pBall = CCSprite::create ( "Images/ball_2.png" );
	m_pBall->setPosition ( ccpMid ( tLyrSize ) );
	this->addChild ( m_pBall );
}

KDvoid GameLayer::initializePaddle ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	m_pPaddle = CCSprite::create ( "Images/paddle.png" );
	m_pPaddle->setPosition ( ccp ( tLyrSize.cx / 2, 50 ) );
	this->addChild ( m_pPaddle );
}

KDvoid GameLayer::initializeDragon ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    // 드라곤 생성 및 날개짓 애니메이션 효과
	CCSprite*		pSprite = CCSprite::create ( "Images/dragon_animation.png" );
	CCAnimation*	pAnimation = CCAnimation::create ( );
    pAnimation->setDelayPerUnit ( 0.1f );
    
	m_pDragon = CCSprite::createWithTexture ( pSprite->getTexture ( ), CCRect ( 0, 0, 130, 140 ) );
 
    for ( KDint i = 0 ; i < 6 ; i++ )
    {
        KDuint	uIndex    = i % 4;
        KDuint	uRowIndex = i / 4;
        
		pAnimation->addSpriteFrameWithTexture ( pSprite->getTexture ( ), CCRectMake ( uIndex * 130 , uRowIndex * 140, 130, 140 ) );
    }

    m_pDragon->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy ) );
    this->addChild ( m_pDragon );
    
	CCAnimate*		pAnimate = CCAnimate::create ( pAnimation );
    m_pDragon->runAction ( CCRepeatForever::create ( pAnimate ) );
 
    // 드라곤 좌우 반복 이동
    m_pDragon->runAction 
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCFlipX ::create ( KD_FALSE ),
				CCMoveTo::create ( 2, ccp (  30, 450 ) ),
				CCFlipX ::create ( KD_TRUE ),
				CCMoveTo::create ( 2, ccp ( 290, 450 ) ),
				KD_NULL
			)
		)
	);
}

KDvoid GameLayer::startGame ( KDfloat fDelta )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

    m_pBall->setPosition ( ccpMid ( tLyrSize ) );
    m_tBallMovement = ccp ( 3, 3 );
 
    if ( kdRand ( ) % 100 < 50 )
    {
        m_tBallMovement.x = -m_tBallMovement.x;
    }

    m_bIsPlaying = KD_TRUE;
 
	this->schedule ( schedule_selector ( GameLayer::gameLogic ), 1 / 60.f / 2.f );
}

KDvoid GameLayer::gameLogic ( KDfloat fDelta )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	CCPoint		tBallPos   = ccpAdd ( m_pBall->getPosition ( ), m_tBallMovement );
	CCPoint		tPaddlePos = m_pPaddle->getPosition ( );
	CCPoint		tDragonPos = m_pDragon->getPosition ( );

    // paddle과 ball의 충돌시 처리
    KDbool		bPaddleCollision = tBallPos.y <= tPaddlePos.y + 13 &&
								   tBallPos.x >= tPaddlePos.x - 48 &&
								   tBallPos.x <= tPaddlePos.x + 48;

    if ( bPaddleCollision )
    {
        if ( tBallPos.y <= tPaddlePos.y + 13 && m_tBallMovement.y < 0 )
        {
			tBallPos = ccp ( tBallPos.x, tPaddlePos.y + 13 );
        }
        
        m_tBallMovement.y = -m_tBallMovement.y;
    }

    // brick과 ball과의 충돌시 처리
    KDbool		bThereAreSolidBricks = KD_FALSE;
    CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( m_pTargets, pObject )
	{
		CCSprite*	pBlick = (CCSprite*) pObject;

		if ( pBlick->getOpacity ( ) == 255 )
		{
			bThereAreSolidBricks = KD_TRUE;

			if ( m_pBall->boundingBox ( ).intersectsRect ( pBlick->boundingBox ( ) ) )
			{
				this->processCollision ( pBlick );
			}
		}
	}

    // Dragon과 ball의 충돌 처리
    KDbool	bDragonCollision = tBallPos.y >= tDragonPos.y - 70 &&
							   tBallPos.x >= tDragonPos.x - 65 &&
							   tBallPos.x <= tDragonPos.x + 65;
    
    if ( bDragonCollision )
    {
        if ( tBallPos.y >= tDragonPos.y - 70 && m_tBallMovement.y > 0 )
        {
            tBallPos = ccp ( tBallPos.x , tDragonPos.y - 70 );
        }
        
        m_tBallMovement.y = -m_tBallMovement.y;
    }

    // brick이 다 사라지고 드라곤에 공이 맞으면 종료 - 게임 종료 상태
    if ( !bThereAreSolidBricks )
    {
        if ( bDragonCollision )
        {
            // 미션 완료후 파티클 효과
			CCParticleSystem*	pSuccess = CCParticleFlower::create ( );			
			pSuccess->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars-grayscale.png" ) );
			pSuccess->setDuration ( 2.0 );            
			pSuccess->setPosition ( ccpMid ( tLyrSize ) );
			this->addChild ( pSuccess, 1, 1 );
            
            this->removeChild ( m_pDragon );            
            
            m_bIsPlaying = KD_FALSE;
            
            m_pBall->setOpacity ( 0 );
            
            this->unscheduleAllSelectors ( );    
            
            // 미션 성공 메시지 출력
			CCLabelTTF*		pLabel1 = CCLabelTTF::create ( "Mission Success!", "Font/Marker Felt.ttf", 32 );
			pLabel1->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 + 40 ) );
			this->addChild ( pLabel1 );			
            
			CCLabelTTF*		pLabel2 = CCLabelTTF::create ( "Click! Here", "Font/Marker Felt.ttf", 25 );									
			CCMenuItem*		pItem = CCMenuItemLabel::create ( pLabel2, this, menu_selector ( GameLayer::nextStepTapped ) );
			pItem->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy / 2 - 30 ) );
			this->addChild ( CCMenu::createWithItem ( pItem ), 1 );
		}
    }

    // ball과 wall의 출돌시 처리
    if ( tBallPos.x > ( tLyrSize.cx - 8 ) || tBallPos.x < 8 )
    {
        m_tBallMovement.x = -m_tBallMovement.x;
    }
    
	if ( tBallPos.y > tLyrSize.cy - 30 )
    {
        m_tBallMovement.y = -m_tBallMovement.y;
    }
    
    // paddle이  ball을 놓쳤을 때 처리 - 미션 실패
    // 패들의 위치 y값 + 패들 두께 / 2 + 화면 아래 마진
    if ( tBallPos.y < 50 + 5 + 8 )
    {
        m_bIsPlaying = KD_FALSE;

        m_pBall->setOpacity ( 0 );
        
        this->unscheduleAllSelectors ( );        

		CCScene*	pScene = CCScene::create ( );
		CCLayer*	pLayer = GameOver::create ( );
		pScene->addChild ( pLayer );

		CCDirector::sharedDirector ( )->replaceScene ( CCTransitionProgressRadialCW::create ( 0.5f, pScene ) );
    }

    // 현재 볼의 위치 계산
	m_pBall->setPosition ( tBallPos );
}

KDvoid GameLayer::processCollision ( CCSprite* pBrick )
{
	const CCPoint&	tBrickPos = pBrick->getPosition ( );
	const CCPoint&	tBallPos = m_pBall->getPosition ( );

    if ( m_tBallMovement.x > 0 && tBrickPos.x < tBallPos.x )
    {
        m_tBallMovement.x = -m_tBallMovement.x;
    }
    else if ( m_tBallMovement.x < 0 && tBrickPos.x < tBallPos.x )
    {
        m_tBallMovement.x = -m_tBallMovement.x;
    }
    
    if ( m_tBallMovement.y > 0 && tBrickPos.y > tBallPos.y )
    {
        m_tBallMovement.y = -m_tBallMovement.y;
    }
    else if ( m_tBallMovement.y < 0 && tBrickPos.y < tBallPos.y )
    {
        m_tBallMovement.y = -m_tBallMovement.y;
    }
    
    pBrick->setOpacity ( 0 );
}

KDvoid GameLayer::nextStepTapped ( CCObject* pSender )
{
	CCScene*	pScene = CCScene::create ( );
	CCLayer*	pLayer = GameLayer::create ( );
	pScene->addChild ( pLayer );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionProgressRadialCW::create ( 0.5f, pScene ) );
}

KDvoid GameLayer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( !m_bIsPlaying)
    {
        return;
    }
    
    CCTouch*	pTouch    = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );
	CCRect		tPadRect  = m_pPaddle->boundingBox ( );
    
	if ( tPadRect.containsPoint ( tLocation ) )
    {
        m_bIsPaddleTouched = KD_TRUE;
    }
    else
    {
        m_bIsPaddleTouched = KD_FALSE;
    }
}

KDvoid GameLayer::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( m_bIsPaddleTouched )
    {
		CCTouch*	pTouch    = (CCTouch*) pTouches->anyObject ( );
		CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );
        
        if ( tLocation.x < 40 )
        {
            tLocation.x = 40;
        }
        
        if ( tLocation.x > 280 )
        {
            tLocation.x = 280;
        }
        
        CCPoint		tNewLocation = ccp ( tLocation.x , m_pPaddle->getPosition ( ).y );
		m_pPaddle->setPosition ( tNewLocation );
    }
}