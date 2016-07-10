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
#include "MenuLayer.h"
#include "SoundManager.h"

typedef enum
{
	eTagBackground		,	 
	eTagCraterLeft		,
	eTagCraterCenter	,
	eTagCraterRight		,
	eTagLabel			,	
	eTagPlayPauseBtn	,	
	eTagSoundOnOffBtn	,
	eTagPlayer			,
    eTagDrop			,
    eTagBigDrop			,
    eTagShield			,
} ETagGame;

GameLayer::GameLayer ( KDvoid )
{
	m_nTouchID			= -1;

	m_nLevel			= 0;

	m_nScore			= 0;
	m_pScoreLabel		= KD_NULL;

	m_pDrops		    = new CCArray ( );
	m_nDropPercent      = 20;

	m_nTime				= 0;
	m_bGameOver			= KD_FALSE;

	m_pShields	        = new CCArray ( );
	m_bShieldOn		    = KD_FALSE;
}

GameLayer::~GameLayer ( KDvoid )
{
	CC_SAFE_DELETE ( m_pDrops );
	CC_SAFE_DELETE ( m_pShields );
}

KDbool GameLayer::init ( KDvoid )
{
    if ( !CCLayer::init ( ) )
    {
        return KD_FALSE;
    }

	// 터치를 받도록 한다.
	this->setTouchEnabled ( KD_TRUE );
	
	// 레이어 크기
	m_tLyrSize = this->getContentSize ( );

	// 이미지를 모아서 사용하는 plist파일을 읽어온다. 
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Images/Volcano_image.plist" );

	// 게임배경을 보여준다.
	createBackground ( );
	
	// start이미지를 게임 시작전 띄워주고 게임시작과 동시에 없애주도록 한다.
    createStartTitle ( );

	// 라벨 생성
	createLabel ( );

	// 버튼 생성
	createBtn ( );	

	// 주인공 표시
	createPlayer ( );

	// 주인공 에니메이션 생성
	createPlayerAnimation ( );

	// 분화구 생성
	createCrater ( );

	// 보호막 생성
	createShield ( );

	// 스케줄러로 게임로직을 가동 시킨다.
	this->schedule ( schedule_selector ( GameLayer::onGameLogic ), 0.25f );

	// 충돌검사를 가동 시킨다.
	this->schedule ( schedule_selector ( GameLayer::onCollision ), 0.2f );

	// 시간을 표시한다.
	this->schedule ( schedule_selector ( GameLayer::onUpdateTimeLabel ), 1.0f );

    return KD_TRUE;
}

KDvoid GameLayer::createBackground ( KDvoid )
{
	CCSprite*		pBG = CCSprite::create ( "Images/bg.png" );
	pBG->setPosition ( ccpMid ( m_tLyrSize ) );
	this->addChild ( pBG, eTagBackground, eTagBackground );
}

KDvoid GameLayer::createStartTitle ( KDvoid )
{
	CCSprite*		pStart = CCSprite::createWithSpriteFrameName ( "start.png" );
	pStart->setPosition ( ccpMid ( m_tLyrSize ) );
	this->addChild ( pStart, eTagLabel, eTagLabel );
	pStart->runAction ( CCFadeOut::create ( 1.5f ) );
}

KDvoid GameLayer::createPlayer ( KDvoid )
{
	m_pPlayer = CCSprite::createWithSpriteFrameName ( "player_01.png" );
	m_pPlayer->setPosition ( ccp ( m_tLyrSize.cx / 2, 100 ) );
	this->addChild ( m_pPlayer, eTagPlayer, eTagPlayer );
}

KDvoid GameLayer::createPlayerAnimation ( )
{
	// 플레이어가 달리는 애니메이션을 만든다.
	CCArray*		pAniFrames = CCArray::create ( );

	for ( KDint nIdx = 1; nIdx <= 4; nIdx++ )
	{
		CCSpriteFrame*	pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "player_%02d.png", nIdx ) );
        pAniFrames->addObject ( pFrame );
	}
	
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pAniFrames, 0.05f );
	pAnimation->setRestoreOriginalFrame ( KD_FALSE );

	m_pPlayerRunAnimate = CCAnimate::create ( pAnimation );	
	m_pPlayerRunAnimate->retain ( );

	// 플레이어가 죽는 애니메이션을 만든다.
	CCArray*		pAniFramesDie = CCArray::create ( );

	for ( KDint nIdx = 1; nIdx <= 3; nIdx++ )
	{
		CCSpriteFrame*	pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "die_%02d.png", nIdx ) );
        pAniFramesDie->addObject ( pFrame );
	}
	
	CCAnimation*		pAnimationDie = CCAnimation::createWithSpriteFrames ( pAniFramesDie, 0.05f );
	pAnimationDie->setRestoreOriginalFrame ( KD_FALSE );

	m_pPlayerDieAnimate = CCAnimate::create ( pAnimationDie );
	m_pPlayerDieAnimate->retain ( );
}


KDvoid GameLayer::createShield ( KDvoid )
{
	const CCPoint&		tPlayerPos = m_pPlayer->getPosition ( );

	m_pShield = CCSprite::create ( "Images/global2.png" );
	m_pShield->setPosition ( tPlayerPos );
	m_pShield->setOpacity ( 100 );
	m_pShield->setScale ( 2.0f );
	m_pShield->setVisible ( KD_FALSE );
	this->addChild ( m_pShield, eTagShield, eTagShield );

	m_bShieldOn = KD_FALSE;
}


KDvoid GameLayer::createCrater ( KDvoid )
{
	m_pCraterLeft = CCParticleFire::create ( );
	m_pCraterLeft->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pCraterLeft->setPosition ( ccp ( 65, 430 ) );
	m_pCraterLeft->setScale ( 0.0f );
	this->addChild ( m_pCraterLeft, eTagCraterLeft, eTagCraterLeft );

	m_pCraterCenter = CCParticleFire::create ( );
	m_pCraterCenter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pCraterCenter->setPosition ( ccp ( m_tLyrSize.cx / 2 + 5, 550 ) );	
	m_pCraterCenter->setScale ( 0.0f );
	this->addChild ( m_pCraterCenter, eTagCraterCenter, eTagCraterCenter );

	m_pCraterRight = CCParticleFire::create ( );
	m_pCraterRight->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pCraterRight->setPosition ( ccp ( m_tLyrSize.cx - 60, 390 ) );
	m_pCraterRight->setScale ( 0.0f );
	this->addChild ( m_pCraterRight, eTagCraterRight, eTagCraterRight );
}


KDvoid GameLayer::createLabel ( KDvoid )
{
	// 고정된 라벨
	CCLabelTTF*		pScoreLabel     = CCLabelTTF::create ( "Score"     , "Fonts/Marker Felt.ttf", 20 );
	CCLabelTTF*		pHighScoreLabel = CCLabelTTF::create ( "High Score", "Fonts/Marker Felt.ttf", 20 );
	CCLabelTTF*		pTimeLabel      = CCLabelTTF::create ( "Timer"     , "Fonts/Marker Felt.ttf", 20 );

	pScoreLabel		 ->setPosition ( ccp ( 50                , m_tLyrSize.cy - 20 ) );
	pHighScoreLabel	 ->setPosition ( ccp ( m_tLyrSize.cx / 2 , m_tLyrSize.cy - 20 ) );
	pTimeLabel		 ->setPosition ( ccp ( m_tLyrSize.cx - 50, m_tLyrSize.cy - 20 ) );

	this->addChild ( pScoreLabel	  , eTagLabel );
    this->addChild ( pHighScoreLabel  , eTagLabel );
	this->addChild ( pTimeLabel		  , eTagLabel );

	// 변하는 라벨
	m_pScoreLabel     = CCLabelTTF::create ( "0" , "Fonts/Marker Felt.ttf", 18 );
	m_pHighScoreLabel = CCLabelTTF::create ( "0" , "Fonts/Marker Felt.ttf", 18 );
	m_pTimeLabel      = CCLabelTTF::create ( "0" , "Fonts/Marker Felt.ttf", 18 );

	m_pScoreLabel	 ->setPosition ( ccp ( 50                , m_tLyrSize.cy - 40 ) );
	m_pHighScoreLabel->setPosition ( ccp ( m_tLyrSize.cx / 2 , m_tLyrSize.cy - 40 ) );
	m_pTimeLabel 	 ->setPosition ( ccp ( m_tLyrSize.cx - 50, m_tLyrSize.cy - 40 ) );

	this->addChild ( m_pScoreLabel     , eTagLabel );
	this->addChild ( m_pHighScoreLabel , eTagLabel );
	this->addChild ( m_pTimeLabel      , eTagLabel );

	m_pHighScoreLabel->setString ( ccszf ( "%d", CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( SESSION_HIGH_SCORE ) ) );
}

KDvoid GameLayer::createBtn ( KDvoid )
{
	const CCSize&	tLyrSize = this->getContentSize ( );
	CCSize			tSize = ccsz;
	CCPoint			tPos  = ccpz;

	// 일시정지 버튼
	m_pPlayMenuItem   = CCMenuItemImage::create ( "Images/play.png"  , KD_NULL, this, menu_selector ( GameLayer::playPauseMenuCallback ) );
	m_pPauseMenuItem  = CCMenuItemImage::create ( "Images/pause.png" , KD_NULL, this, menu_selector ( GameLayer::playPauseMenuCallback ) );

	tSize = m_pPlayMenuItem->getContentSize ( );
	tPos  = ccp ( tLyrSize.cx - tSize.cx / 2 - 15, tLyrSize.cy - tSize.cy * 2 );

	m_pPlayMenuItem->setPosition ( tPos );
	m_pPauseMenuItem ->setPosition ( tPos );

	this->addChild ( CCMenu::create ( m_pPlayMenuItem, m_pPauseMenuItem, KD_NULL ), eTagPlayPauseBtn, eTagPlayPauseBtn );

	playStopGame ( KD_TRUE );

	
	// 음소거 버튼
	m_pSoundOnMenuItem   = CCMenuItemImage::create ( "Images/soundOn.png"  , KD_NULL, this, menu_selector ( GameLayer::soundOnOffMenuCallback ) );
	m_pSoundOffMenuItem  = CCMenuItemImage::create ( "Images/soundOff.png" , KD_NULL, this, menu_selector ( GameLayer::soundOnOffMenuCallback ) );
	
	tSize = m_pSoundOnMenuItem->getContentSize ( );
	tPos  = ccp ( tLyrSize.cx - tSize.cx / 2 - 15, tLyrSize.cy - tSize.cy * 2 - 70 );

	m_pSoundOnMenuItem->setPosition ( tPos );
	m_pSoundOffMenuItem ->setPosition ( tPos );

	this->addChild ( CCMenu::create ( m_pSoundOnMenuItem, m_pSoundOffMenuItem, KD_NULL ), eTagSoundOnOffBtn, eTagSoundOnOffBtn );
	
	SoundManager::sharedSoundManager ( )->soundOnOff ( m_pSoundOnMenuItem, m_pSoundOffMenuItem );
}


KDvoid GameLayer::onGameLogic ( KDfloat fDelta )
{
	// 가운데 분화구
	if ( m_nTime >= 2 )
	{
		if ( m_nTime == 2 && m_nLevel == 0 )
		{
				m_nLevel++;
				this->shakeScreen ( );
				m_pCraterCenter->setScale ( 1.0f );
		}
		if (  kdRand ( ) % 100 < m_nDropPercent )
		{
			addDrops ( eTagCraterCenter );
		}
		if ( m_nTime % 5 == 0 && m_nDropPercent != 100)
		{
			m_nDropPercent++;
		}
	}

	// 오른쪽 분화구 시작 (20초후)
	if ( m_nTime >= 20 )
	{
		if ( m_nTime == 20 && m_nLevel == 1 )
		{
			m_nLevel++;
			this->shakeScreen ( );
			m_pCraterRight->setScale ( 0.5f );
		}
		if ( kdRand ( ) % 10 < 5  )
		{
			addDrops ( eTagCraterRight );
		}
	}


	// 왼쪽 분화구 시작 (30초후)
	if ( m_nTime >= 30 )
	{
		if ( m_nTime == 30 && m_nLevel == 2 )
		{
			m_nLevel++;
			this->shakeScreen ( );
			m_pCraterLeft->setScale ( 0.5f );
		}
		if ( kdRand ( ) % 10 < 5  )
		{
			addDrops ( eTagCraterLeft );
		}
	}

	// 25초 화면을 흔들고 화산을 크게 한다.
	if ( m_nTime == 40 && m_nLevel == 3 )
	{
		m_nLevel++;
		this->shakeScreen ( );
		m_pCraterCenter->setScale ( 2 );
	}

	// 쉴드를 30% 간격으로 떨어 트린다.
	if ( kdRand ( ) % 30 == 0 )
	{
		addShield ( );
	}
}

KDvoid GameLayer::onCollision ( KDfloat fDelta )
{	
	// 플레이어와의 충돌 처리 
	CCObject*	pObject = KD_NULL;

	const CCSize&		tPlaySize = m_pPlayer->getContentSize ( );
	const CCPoint&		tPlayPos  = m_pPlayer->getPosition ( );

	CCARRAY_FOREACH ( m_pDrops, pObject )
	{
		CCSprite*	pDrop = (CCSprite*) pObject;
			
		CCRect tPlayRect = ccr
		(
			tPlayPos.x - ( tPlaySize.cx / 2 ) + 20,
			tPlayPos.y - ( tPlaySize.cy / 2 ) + 20,
			tPlaySize.cx - 20,
			tPlaySize.cy - 20
		);

		// 플레이어의 바운딩박스와 화산재의 바운딩박스가 닿았을 경우
		if ( tPlayRect.intersectsRect ( pDrop->boundingBox ( ) ) )				
		{
			if ( m_bShieldOn )
			{
				m_pShield->setVisible ( KD_FALSE );
				m_bShieldOn = KD_FALSE;
			}
			else
			{
				//	모든 스케줄러를 중지 시킨다.
				this->unscheduleAllSelectors ( );			
				gameOver ( );
				break;
			}
		}
	}

	CCObject*	pObj = KD_NULL;
	CCARRAY_FOREACH ( m_pShields, pObj )
	{
		CCSprite*	pShield = (CCSprite*) pObj;
		if ( !m_bShieldOn && m_pPlayer->boundingBox ( ).intersectsRect ( ( pShield->boundingBox ( )  ) ) )				
		{
			pShield->setVisible ( KD_FALSE );
			m_pShield->setPosition ( tPlayPos );
			m_pShield->setVisible ( KD_TRUE );
			m_bShieldOn = KD_TRUE;
			break;
		}
	}
}

KDvoid GameLayer::onUpdateTimeLabel ( KDfloat fDelta )
{
	if ( !m_bGameOver )
	{
		m_pTimeLabel->setString ( ccszf ( "%d", m_nTime++ ) );
	}
}


KDvoid GameLayer::addDrops ( KDint nTagCrater )
{
	// 화산재가 랜덤한 x좌표에 생성될수 있도록 랜덤값을 만든다.
	KDfloat		fPosX	  = kdRand ( ) % 480 * 1.f;
	KDfloat		fDuration = kdRand ( ) % 10 + 2.f;

	// 화산재를 랜덤한 x좌표에 생성하고 오브젝트 관리자에 추가한다.
	CCSprite*		pDrop = CCSprite::create ( );

	switch ( nTagCrater )
	{
		case eTagCraterLeft:
			pDrop->setPosition ( ccp ( 65, 430 ) );
			break;

		case eTagCraterCenter:
			pDrop->setPosition ( ccp ( m_tLyrSize.cx / 2 + kdRand ( ) % 50, 550 ) );
			break;

		case eTagCraterRight:
			pDrop->setPosition ( ccp ( m_tLyrSize.cx - 60, 390 ) );
			break;	
	}

	this->addChild ( pDrop, eTagDrop );
	m_pDrops->addObject ( pDrop );

	// 이미지를 넣고 회전시키기
	CCSprite*		pDropImage = CCSprite::createWithSpriteFrameName ( "projectile.png" );
	pDropImage->runAction ( CCRepeatForever::create ( CCRotateBy::create ( 0.5, kdRand ( ) % 360 * 1.f ) ) );
	pDrop->addChild ( pDropImage );

	// 화산재에 꼬리 달기
	CCParticleSystem*	  pFire = CCParticleFire::create ( );
	pFire->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
    pFire->setScale ( 0.2f );
	pFire->setPosition ( ccpMid ( pDrop->getContentSize ( ) ) );
	pDrop->addChild ( pFire, 10 );

	// 가끔 크게 한다.
	if ( m_nTime >= 40 && kdRand ( ) % 8 == 0 )
	{
		pDrop->setTag ( eTagBigDrop );
		pDrop->setScale ( 1 );
	}
	else
	{
		pDrop->setScale ( 0.5 );
	}

	pDrop->setOpacity ( 50 );

	// 화산재의 움직임을 설정한다.
	pDrop->runAction
	( 
		CCSequence::createWithTwoActions
		(			
			CCJumpTo::create ( fDuration, ccp ( fPosX, 70 ), 500.0f, 1),			
			CCCallFuncN	::create ( this, callfuncN_selector ( GameLayer::removeDrops ) )
		)
	);
	pDrop->runAction ( CCScaleTo::create ( fDuration / 5, pDrop->getScale ( ) * 2 ) );
	
	// 투명에서 서서히 보이게 하기
	pDrop->runAction ( CCFadeTo::create ( fDuration / 5, 255 ) );	
}


KDvoid GameLayer::addShield ( KDvoid )
{
	KDfloat		fPosX	  = kdRand ( ) % 480 * 1.f;
	KDfloat		fDuration = kdRand ( ) % 10 + 2.f;

	CCSprite* pShield = CCSprite::create ( "Images/feveritem.png" );
	pShield->setPosition ( ccp ( fPosX, 800 ) );
	this->addChild ( pShield , eTagShield );
	m_pShields->addObject ( pShield );

	pShield->runAction
	( 
		CCSequence::createWithTwoActions
		(
			CCMoveTo	::create ( fDuration, ccp ( fPosX, 95 ) ), 
			CCCallFuncN	::create ( this, callfuncN_selector ( GameLayer::removeShield ) )
		) 
	);
}


KDvoid GameLayer::removeDrops ( CCNode* pSender )
{
	CCSprite*  pSprite = (CCSprite*) pSender;
	KDbool bBigDrop = ( pSprite->getTag ( ) == eTagBigDrop ) ? KD_TRUE : KD_FALSE;

	this->removeChild ( pSprite, KD_TRUE );

	m_pDrops->removeObject ( pSprite );
	
	if ( !m_bGameOver )
	{
		// 오브젝트가 사라질때마다 score value값을 증가시킨다.
		m_nScore += 1 * kdAbs ( m_nTime / 15 + 1) + ( bBigDrop ? 100 : 0 );
		m_pScoreLabel->setString ( ccszf ( "%d", m_nScore ) );
	}

	CCParticleSystem *pEmitter = CCParticleFire::create ( );
	pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("Images/fire.png"));
	pEmitter->setTotalParticles(200);
	pEmitter->setAutoRemoveOnFinish( KD_TRUE );
	pEmitter->setScale ( 0.5f * ( bBigDrop ? 2 : 1 ) );
	pEmitter->setLife ( 0.5f );
	pEmitter->setDuration ( 0.5f );
	pEmitter->setPosition ( pSprite->getPosition ( ) );
	this->addChild ( pEmitter );
}


KDvoid GameLayer::removeShield ( CCNode* pSender )
{
	CCSprite*  pSprite = (CCSprite*) pSender;
	this->removeChild ( pSprite, KD_TRUE );
	m_pShields->removeObject ( pSprite );
}

KDvoid GameLayer::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_bGameOver )
	{
		return;
	}

	CCAction* runAction = m_pPlayer->runAction 
	( 
		CCRepeatForever::create 
		( 
			m_pPlayerRunAnimate
		) 
	);
	runAction->setTag ( 1 );

	// 아무 터치 받아 오는법
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	// 멀티 터치 탐색용
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ )
	{
		pTouch = (CCTouch*) ( *it );

		// 확보된 터치가 없으면, 이 터치 아이디를 기억한다.
		if ( m_nTouchID == -1 )
		{	
			m_nTouchID = pTouch->getID ( );

			// 플레이어 움직임 시작
			this->schedule ( schedule_selector ( GameLayer::onMovingPlayer ), 0.01f );

			m_tPlayerVel = pTouch->getLocation ( );
			m_tPlayerVel = this->convertToNodeSpace ( m_tPlayerVel );
			break;
		}
	}	
}


KDvoid GameLayer::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	// 아무 터치 받아 오는법
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ )
	{
		pTouch = (CCTouch*) ( *it );
		
		// 확보된 터치가 없으면, 이 터치 아이디를 기억한다.
		if ( pTouch->getID ( ) == m_nTouchID )
		{			
			m_tPlayerVel = pTouch->getLocation ( );
			m_tPlayerVel = this->convertToNodeSpace ( m_tPlayerVel );
			break;
		}
	}
}


KDvoid GameLayer::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_bGameOver )
	{
		return;
	}

	m_pPlayer->stopActionByTag ( 1 );

	CCTouch*	pTouch = KD_NULL;

	// 멀티 터치 탐색용
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ )
	{
		pTouch = (CCTouch*) ( *it );
		
		if ( pTouch->getID ( ) == m_nTouchID )
		{
			m_nTouchID = -1;

			this->unschedule ( schedule_selector ( GameLayer::onMovingPlayer ) );
		}
		break;
	}
}

KDvoid GameLayer::onMovingPlayer ( KDfloat fDelta )
{
	CCPoint		tPoint = m_pPlayer->getPosition ( );
	
	// 플레이어가 오른쪽에 있으면
	if ( m_tPlayerVel.x > m_tLyrSize.cx / 2 )
	{
		tPoint = ccpAdd ( tPoint, ccp ( 4, 0 ) );
		m_pPlayer->setFlipX ( KD_FALSE );
	}
	// 플레이어가 왼쪽에 있으면
	else
	{
		tPoint = ccpAdd ( tPoint, ccp ( -4, 0 ) );
		m_pPlayer->setFlipX ( KD_TRUE );
	}
	
	// 플레이어가 화면 왼쪽을 벗어나면 오른쪽끝으로 이동
	if ( tPoint.x < 0 )
	{
		tPoint.x = m_tLyrSize.cx;
	}
	// 플레이어가 화면 오른쪽을 벗어나면 왼쪽끝으로 이동
	if ( tPoint.x > m_tLyrSize.cx )
	{
		tPoint.x = 0;
	}
	
	m_pPlayer->setPosition ( tPoint );
	if ( m_bShieldOn && m_pShield->isVisible ( ) )
	{
		m_pShield->setPosition ( tPoint );
	}
}

KDvoid GameLayer::shakeScreen ( KDvoid )
{
	xmVibrate ( 1000 );

	CCArray*	pActions = CCArray::create ( );

	for ( KDint i = 0; i < FPS; ++i )
	{
		CCFiniteTimeAction*		pMove = CCMoveTo::create ( 1.f / FPS, ccp ( 30 - kdRand ( ) % 30, 15 - kdRand ( ) % 30 ) );
		pActions->addObject ( pMove );
	}
	
	CCFiniteTimeAction*		pReset = CCMoveTo::create ( 1.f / FPS, ccp ( 0, 0 ) );
	pActions->addObject ( pReset );

	this->runAction ( CCSequence::create ( pActions ) );
}


KDvoid GameLayer::gameOver ( KDvoid )
{	
	m_bGameOver = KD_TRUE;

	SoundManager::sharedSoundManager ( )->playEffectDie ( );


	// 달리는 액션을 중지 시킨다.
	m_pPlayer->stopActionByTag ( 1 );	
	// 죽는 모션을 보여준다.
	m_pPlayer->runAction  ( m_pPlayerDieAnimate );

	//this->stopAllActions ( );

	// 게임 라벨 생성
	CCSprite*           pGameOver = CCSprite::createWithSpriteFrameName ( "gameover.png" );
	pGameOver->setPosition ( ccp ( m_tLyrSize.width / 2, m_tLyrSize.height - m_tLyrSize.height / 4 ) );
	this->addChild ( pGameOver, 100, 100 );

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
    pGameOver->runAction ( CCRepeatForever::create ( CCJumpBy::create ( 3, CCPointZero, 50, 1 ) ) );

	// 메뉴 배경 이미지
	CCSprite* pMenuBg = CCSprite::create ( "Images/menu_bg.png" );
	pMenuBg->setPosition( ccp ( m_tLyrSize.width / 2, m_tLyrSize.height / 2 ) );
	
	// 메뉴 버튼
	CCMenu* pMenu    = CCMenu::create
	(
		CCMenuItemLabel::create ( CCLabelTTF::create ( "Retry", "Fonts/Marker Felt.ttf", 40 ), this, menu_selector ( GameLayer::onRetry ) ),
		CCMenuItemLabel::create ( CCLabelTTF::create ( "Menu" , "Fonts/Marker Felt.ttf", 40 ), this, menu_selector ( GameLayer::onMenu ) ),
		KD_NULL
	);
	pMenu->alignItemsVerticallyWithPadding ( 20 );

	const CCSize& tBgSize = pMenuBg->getContentSize ( );

	pMenu->setPosition ( ccp ( tBgSize.width / 2, tBgSize.height / 2 ) );
	pMenuBg->addChild ( pMenu );

	this->addChild( pMenuBg );

	pMenuBg->runAction ( CCFadeIn::create ( 1.0f ) );
	pMenu->runAction ( CCFadeIn::create ( 1.0f ) );

	// 최고점수 설정
	if ( m_nScore > CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( SESSION_HIGH_SCORE ))
	{
		CCUserDefault::sharedUserDefault()->setIntegerForKey( SESSION_HIGH_SCORE, m_nScore ) ;
	}
}


KDvoid	GameLayer::onRetry ( CCObject* pObject )
{
	replaceScene ( GameLayer::create ( ) );
}


KDvoid	GameLayer::onMenu ( CCObject* pObject )
{
	replaceScene ( MenuLayer::create ( ) );
}


KDvoid GameLayer::replaceScene ( CCLayer* pLayer )
{
	CCScene    *pScene = CCScene::create ( );
    pScene->addChild ( pLayer );
	CCDirector::sharedDirector ( )->replaceScene ( pScene );
}

KDvoid GameLayer::playPauseMenuCallback ( CCObject* pSender )
{

	KDbool bGamePause = !m_pPauseMenuItem->isVisible ( );
	playStopGame ( bGamePause );
}

KDvoid GameLayer::playStopGame ( KDbool bGamePause )
{

	if ( bGamePause )
	{
		m_pPlayMenuItem->setVisible ( KD_FALSE );
		m_pPauseMenuItem->setVisible ( KD_TRUE );
		
		CCDirector::sharedDirector ( )->resume ( );
	} 
	else 
	{
		m_pPlayMenuItem->setVisible ( KD_TRUE );
		m_pPauseMenuItem->setVisible ( KD_FALSE );

		CCDirector::sharedDirector ( )->pause ( );
	}
}

KDvoid GameLayer::soundOnOffMenuCallback ( CCObject* pSender )
{
	SoundManager::sharedSoundManager ( )->soundOnOff ( m_pSoundOnMenuItem, m_pSoundOffMenuItem, !m_pSoundOffMenuItem->isVisible ( )  );
}
