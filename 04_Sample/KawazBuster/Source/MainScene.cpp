/* --------------------------------------------------------------------------
 *
 *      File            MainScene.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/27 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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
#include "MainScene.h"
#include "TitleScene.h"
#include "KawazTan.h"

MainScene::MainScene ( KDvoid )
{
	m_pTargets = KD_NULL;
}

MainScene::~MainScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTargets );
}

KDbool MainScene::init ( KDvoid )
{
	if ( !KWScene::init ( ) )
	{
		return KD_FALSE;
	}

	m_nScore	= 0;
	m_bActive	= KD_FALSE;
	m_bHurryUp	= KD_FALSE;

	CCUserDefault*		pUserDefault = CCUserDefault::sharedUserDefault ( );
	m_nHighScore = pUserDefault->getIntegerForKey ( "HighScore" );

	m_pScoreLabel	  = CCLabelTTF::create ( ccszf ( "%d", m_nScore     ), "font/Marker Felt.ttf", 48 );
	m_pHighScoreLabel = CCLabelTTF::create ( ccszf ( "%d", m_nHighScore ), "font/Marker Felt.ttf", 48 );
	m_pTimerLabel	  = KWTimerLabel::create ( "0", "font/Marker Felt.ttf", 48 );

	KWSprite*	pBG0 = KWSprite::create ( "image/main/main_background-hd.png" );
	KWSprite*	pBG1 = KWSprite::create ( "image/main/main_layer0-hd.png" );
	KWSprite*	pBG2 = KWSprite::create ( "image/main/main_layer1-hd.png" );
    pBG0->setPosition ( ccpMid ( m_tLyrSize ) );
    pBG1->setPosition ( ccpMid ( m_tLyrSize ) );
    pBG2->setPosition ( ccpMid ( m_tLyrSize ) );
    this->addChild ( pBG0, 0 );
    this->addChild ( pBG1, 1000 );
    this->addChild ( pBG2, 2000 );

	m_pTargets = CCArray::create ( );
	m_pTargets->retain ( );

	for ( KDint i = 0; i < 6; ++i )
	{
		KawazTan*	pKawaztan = KawazTan::create ( ccp ( 50 + 100 * i, 40 ) );
		m_pTargets->addObject ( pKawaztan );
		this->addChild ( pKawaztan, 1001 + ( 6 - i ) );
	}

    for ( KDint i = 0; i < 3; ++i )
	{
		KawazTan*	pKawaztan = KawazTan::create ( ccp ( m_tLyrSize.cx - 320 + 100 * i, 140 ) );
		m_pTargets->addObject ( pKawaztan );
		this->addChild ( pKawaztan, 1 + ( 3 - i ) );
    }

	m_pTimerLabel->setTime ( 0, 1, 0 ); 

	CCLabelTTF*		pScoreLabel     = CCLabelTTF::create ( "Score"    , "font/Marker Felt.ttf", 48 );
	CCLabelTTF*		pHighScoreLabel = CCLabelTTF::create ( "HighScore", "font/Marker Felt.ttf", 48 );
	CCLabelTTF*		pTimeLabel      = CCLabelTTF::create ( "Time"     , "font/Marker Felt.ttf", 48 );

	pScoreLabel		 ->setPosition ( ccp ( 100, m_tLyrSize.cy -  40 ) );
	m_pScoreLabel	 ->setPosition ( ccp ( 100, m_tLyrSize.cy - 100 ) );
	pHighScoreLabel	 ->setPosition ( ccp ( m_tLyrSize.cx - 160, m_tLyrSize.cy -  40 ) );
	m_pHighScoreLabel->setPosition ( ccp ( m_tLyrSize.cx - 160, m_tLyrSize.cy - 100 ) );
	pTimeLabel		 ->setPosition ( ccp ( m_tLyrSize.cx / 2, m_tLyrSize.cy -  40 ) );
	m_pTimerLabel	 ->setPosition ( ccp ( m_tLyrSize.cx / 2, m_tLyrSize.cy - 100 ) );

    this->addChild ( pScoreLabel		);
    this->addChild ( m_pScoreLabel		);
    this->addChild ( pHighScoreLabel	);
    this->addChild ( m_pHighScoreLabel	);
    this->addChild ( pTimeLabel			);
	this->addChild ( m_pTimerLabel		);

	this->runAction
	(
		CCSequence::create 
		(
			CCDelayTime::create ( 1 ),
			CCCallFunc ::create ( this, callfunc_selector ( MainScene::playMusic ) ),
			KD_NULL
		)
	);
        
	m_pTimerLabel->setTimerCompletionListener ( this, callfunc_selector ( MainScene::endGame ) );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDbool MainScene::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !m_bActive )
	{
		return KD_FALSE; 
	}

	CCPoint		tPoint = this->convertToWorldSpace ( this->convertTouchToNodeSpace ( pTouch ) ); 
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pTargets, pObject )
	{
		KawazTan*	pTarget = (KawazTan*) pObject;

		if ( pTarget->collideWithPoint ( tPoint ) && pTarget->canTouch ( ) )
		{
			if ( pTarget->tab ( ) )
			{
				this->getScore ( pTarget->getScore ( ) );

				if ( pTarget->getType ( ) == KawazTanTypeBomb ) 
				{
					this->shakeScreen ( );
				}
			}

			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

KDvoid MainScene::update ( KDfloat fDelta )
{
	KWScene::update ( fDelta );

	KDint	nRate = m_bHurryUp ? 10 : 40;
	
	if ( m_bActive && kdRand ( ) % nRate == 0 )
	{ 
		this->popTarget ( );	
	}

	if ( m_pTimerLabel->leaveSecond ( ) <= HURRY_UP_TIME && !m_bHurryUp )
	{
		this->hurryUp ( );
	}
}

KDvoid MainScene::playMusic ( KDvoid )
{
	KWMusicManager*		pMusic = KWMusicManager::sharedManager ( );
	pMusic->playMusic ( "music/bgm.mp3", "music/bgm_int.mp3", KD_TRUE );
	pMusic->setIntroMusicCompletionListener ( this, callfunc_selector ( MainScene::startGame ) );
	
	KWSprite*	pReady = KWSprite::create ( "image/main/text/ready-hd.png" );
	pReady->setScale ( 0 );
	pReady->setPosition ( ccpMid ( m_tLyrSize ) );
	pReady->runAction ( CCEaseExponentialIn::create ( CCScaleTo::create ( 0.5f, 1.0f ) ) );
	this->addChild ( pReady, 5000, 1 ); 	
}

KDvoid MainScene::startGame ( KDvoid )
{
	KWSprite*	pReady = (KWSprite*) this->getChildByTag ( 1 );
	KWSprite*	pGo = KWSprite::create ( "image/main/text/go-hd.png" );
	pGo->setScale ( 0 );
	pGo->setPosition ( ccpMid ( m_tLyrSize ) );

	pReady->runAction
	(
		CCSequence::create
		(
			CCEaseExponentialOut::create ( CCScaleTo::create ( 0.25f, 0 ) ),
			CCRemoveSelf		::create ( KD_TRUE ),
			KD_NULL
		)	
	);

	pGo->runAction 
	(
		CCSequence::create
		(
			CCEaseExponentialIn::create ( CCScaleTo::create ( 0.25f, 1.f ) ),
			CCDelayTime::create ( 0.5f ),		
			CCSpawn::createWithTwoActions 
			(
				CCRotateBy	::create ( 0.5f, 720 ),
				CCMoveTo	::create ( 0.5f, ccp ( m_tLyrSize.cx * 1.5f, m_tLyrSize.cy * 1.5f ) )
			),
			CCRemoveSelf::create ( KD_TRUE ),
			KD_NULL
		)	
	);

	this->addChild ( pGo, 5000, 2 ); 
	m_bActive = KD_TRUE; 
	m_pTimerLabel->play ( );
}

KDvoid MainScene::hurryUp ( KDvoid )
{
	m_bHurryUp = KD_TRUE;
	KWMusicManager::sharedManager ( )->playMusic ( "music/hurry.mp3", "music/hurry_int.mp3", KD_TRUE ); 
}

KDvoid MainScene::endGame ( KDvoid )
{
	this->stopAllActions ( );

	this->setPosition ( ccpz );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/time_up.wav" );

	KWSprite*	pFinish = KWSprite::create ( "image/main/text/finish-hd.png" );
	pFinish->setScale ( 0 );
	pFinish->setPosition ( ccpMid ( m_tLyrSize ) );

	pFinish->runAction 
	(
		CCSequence::create 
		(
			CCEaseExponentialIn	::create ( CCScaleTo::create ( 0.25f, 1.0f ) ),
			CCDelayTime			::create ( 1.f ),
			CCEaseExponentialOut::create ( CCScaleTo::create ( 0.25f, 0.0f ) ),
			CCRemoveSelf		::create ( KD_TRUE ),
			KD_NULL
		)
	);
	this->addChild ( pFinish, 5000 );

	KWMusicManager::sharedManager ( )->fadeout ( 2.f );
	m_bActive = KD_FALSE; 

	this->runAction
	(
		CCSequence::create 
		(			
			CCDelayTime	::create ( 2.f ),	
			CCCallFunc	::create ( this, callfunc_selector ( MainScene::onGameOver ) ),
			CCDelayTime	::create ( 4.f ),
			CCCallFunc	::create ( this, callfunc_selector ( MainScene::showResult ) ),
			KD_NULL
		)
	);
}
 
KDvoid MainScene::showResult ( KDvoid )
{
	CCMenu*		pMenu = CCMenu::create
	(
		CCMenuItemImage::create ( "image/result/retry_button-hd.png" , "image/result/retry_button_selected-hd.png" , this, menu_selector ( MainScene::pressRetryButton  ) ),
		CCMenuItemImage::create ( "image/result/return_button-hd.png", "image/result/return_button_selected-hd.png", this, menu_selector ( MainScene::pressReturnButton ) ),
		KD_NULL
	);
	pMenu->alignItemsHorizontally ( );
	pMenu->setPosition ( ccp ( m_tLyrSize.cx / 2, 50 ) );
	this->addChild ( pMenu, 5000 );

	if ( m_nScore > m_nHighScore )
	{
		KWMusicManager::sharedManager ( )->playMusic ( "music/high_score.mp3", KD_FALSE );

		m_nHighScore = m_nScore;
		m_pHighScoreLabel->setString ( ccszf ( "%d", m_nHighScore ) ); 

		CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "HighScore", m_nHighScore );
	}
}

KDvoid MainScene::popTarget ( KDvoid )
{
	KDint		n = kdRand ( ) % m_pTargets->count ( );
	KDfloat		fWait = 1.f;

	if ( m_bHurryUp )
	{
		fWait = m_pTimerLabel->leaveSecond ( ) * 0.008f + 0.5f;
	}
	else
	{
		fWait = m_pTimerLabel->leaveSecond ( ) * 0.01f + 0.2f;
	}

	( (KawazTan*) m_pTargets->objectAtIndex ( n ) )->start ( fWait );
}

KDvoid MainScene::shakeScreen ( KDvoid )
{
	xmVibrate ( 1000 );

	CCArray*	pActions = CCArray::create ( );

	for ( KDint i = 0; i < FPS; ++i )
	{
		CCFiniteTimeAction*		pMove = CCMoveTo::create ( 1.f / FPS, ccp ( 30 - kdRand ( ) % 60, 15 - kdRand ( ) % 60 ) );
		pActions->addObject ( pMove );
	}
	
	CCFiniteTimeAction*		pReset = CCMoveTo::create ( 1.f / FPS, ccp ( 0, 0 ) );
	pActions->addObject ( pReset );

	this->runAction ( CCSequence::create ( pActions ) );
}

KDvoid MainScene::getScore ( KDint nScore )
{
	m_nScore += nScore;
	if ( m_nScore < 0 )
	{
		m_nScore = 0;
	}
	m_pScoreLabel->setString ( ccszf ( "%d", m_nScore ) ); 
}

KDvoid MainScene::pressRetryButton ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );
	CCTransitionCrossFade*	pTransition = CCTransitionCrossFade::create ( 0.5f, MainScene::scene ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( pTransition );
}

KDvoid MainScene::pressReturnButton ( CCObject* pSender )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "se/pico.wav" );
	CCTransitionCrossFade*	pTransition = CCTransitionCrossFade::create ( 0.5f, TitleScene::scene ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( pTransition );
}

KDvoid MainScene::onGameOver ( KDvoid )
{
	this->schedule ( schedule_selector ( MainScene::onShowResult ), 4.f );

	KWMusicManager::sharedManager ( )->playMusic ( "music/game_over.mp3", KD_FALSE );
}

KDvoid MainScene::onShowResult ( KDfloat fDelta )
{
	this->showResult ( );
}