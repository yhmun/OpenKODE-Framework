/* --------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012 Jean-yves mengant on 2/6/12 week 6. All rights reserved.
 *
 *         http://www.raywenderlich.com/11338/cocos2d-x-for-ios-and-android-space-game
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

// Asteroid 
#define _KFILTERINGFACTOR_		 0.1f

// ShipLaser 
#define _KNUMLASERS_			 5
// Asteroid 
#define _KNUMASTEROIDS_			 15

GameScene::GameScene ( KDvoid )
{
	m_pAsteroids	= KD_NULL;
	m_pShipLasers	= KD_NULL;
	m_bGameOver		= KD_FALSE;
}

GameScene::~GameScene ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAsteroids );
	CC_SAFE_RELEASE ( m_pShipLasers );
}

KDbool GameScene::init ( KDvoid )
{
	if ( !CCLayer::initWithVisibleViewport ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	// Setup BatchNode
	m_pBatchNode = CCSpriteBatchNode::create ( "Image/Sprites.pvr.ccz" );
	this->addChild ( m_pBatchNode );

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "Image/Sprites.plist" );
	
	// Setup Ship
	m_pShip = CCSprite::createWithSpriteFrameName ( "SpaceFlier_sm_1.png" );	
	
	m_pShip->setPosition ( ccp ( tLyrSize.cx * 0.1f, tLyrSize.cy * 0.5f ) );
	m_pBatchNode->addChild ( m_pShip, 100 );		

	// 1. Create the CCParallaxNode
	m_pBackgroundNode = CCParallaxNode::create ( );
	this->addChild ( m_pBackgroundNode, -1 );

	// 2. Create the sprites we'll add to the CCParallaxNode
	m_pSpacedust1		= CCSprite::create ( "Image/bg_front_spacedust.png"	);
	m_pSpacedust2		= CCSprite::create ( "Image/bg_front_spacedust.png"	);
	m_pPlanetsunrise	= CCSprite::create ( "Image/bg_planetsunrise.png"	);
	m_pGalaxy			= CCSprite::create ( "Image/bg_galaxy.png"			);
	m_pSpacialanomaly	= CCSprite::create ( "Image/bg_spacialanomaly.png"	);
	m_pSpacialanomaly2	= CCSprite::create ( "Image/bg_spacialanomaly2.png"	);

	// 3. Determine relative movement speeds for space dust and background
	CCPoint	tDustSpeed	= ccp ( 0.1f , 0.1  );
	CCPoint	tBgSpeed	= ccp ( 0.05f, 0.05 );

	// 4. Add children to CCParallaxNode
	m_pBackgroundNode->addChild ( m_pSpacedust1		,  0, tDustSpeed, ccp (    0, tLyrSize.cy / 2 ) );
	m_pBackgroundNode->addChild ( m_pSpacedust2		,  0, tDustSpeed, ccp ( m_pSpacedust1->getContentSize ( ).cx, tLyrSize.cy / 2 ) );
	m_pBackgroundNode->addChild ( m_pGalaxy			, -1, tBgSpeed	 , ccp (    0, tLyrSize.cy * 0.7f ) );
	m_pBackgroundNode->addChild ( m_pPlanetsunrise	, -1, tBgSpeed  , ccp (  600, tLyrSize.cy * 0    ) );
	m_pBackgroundNode->addChild ( m_pSpacialanomaly	, -1, tBgSpeed  , ccp (  900, tLyrSize.cy * 0.3f ) );
	m_pBackgroundNode->addChild ( m_pSpacialanomaly2, -1, tBgSpeed  , ccp ( 1500, tLyrSize.cy * 0.9f ) );

	this->scheduleUpdate ( );

	// adding stars  
	this->addChild ( CCParticleSystemQuad::create ( "Image/Stars1.plist" ) );
	this->addChild ( CCParticleSystemQuad::create ( "Image/Stars2.plist" ) );
	this->addChild ( CCParticleSystemQuad::create ( "Image/Stars3.plist" ) );

	// We'll use accelerometer
	this->setAccelerometerEnabled ( KD_TRUE );

	// Asteroids
	m_pAsteroids = new CCArray ( );

	for ( KDint i = 0; i < _KNUMASTEROIDS_; ++i )
	{
		 CCSprite*	pAsteroid = CCSprite::createWithSpriteFrameName ( "asteroid.png" );
		 pAsteroid->setVisible ( KD_FALSE );
		 m_pBatchNode->addChild ( pAsteroid );
		 m_pAsteroids->addObject ( pAsteroid );		 
	}

	// shipLasers
	m_pShipLasers = new CCArray ( );
	for ( KDint i = 0; i < _KNUMLASERS_; ++i )
	{
		CCSprite*	pShipLaser = CCSprite::createWithSpriteFrameName ( "laserbeam_blue.png" );
		pShipLaser->setVisible ( KD_FALSE );
		m_pBatchNode->addChild ( pShipLaser );
		m_pShipLasers->addObject ( pShipLaser );
	}

	this->setTouchEnabled ( KD_TRUE );

	// GameOver Stuff
	m_nNextAsteroid  = 0;
	m_nNextShipLaser = 0;
	m_nLives = 3;

	// win when 30 seconds elapse
	m_fGameOverTime = KD_GET_UST2MSEC + 30000.f;

	// Adding sounds
	SimpleAudioEngine::sharedEngine ( )->playBackgroundMusic ( "Sound/SpaceGame.wav", KD_TRUE );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "Sound/explosion_large.wav" );
	SimpleAudioEngine::sharedEngine ( )->preloadEffect ( "Sound/laser_ship.wav" );

	return KD_TRUE;
}

KDvoid GameScene::update ( KDfloat fDelta )
{
	CCPoint		tBackgroundScrollVert = ccp ( -1000, 0 );
	m_pBackgroundNode->setPosition ( ccpAdd ( m_pBackgroundNode->getPosition ( ), ccpMult ( tBackgroundScrollVert, fDelta ) ) ); 

	CCArray*	pSpaceDusts = CCArray::createWithCapacity ( 2 );
	pSpaceDusts->addObject ( m_pSpacedust1 );
	pSpaceDusts->addObject ( m_pSpacedust2 );
	for ( KDuint i = 0; i < pSpaceDusts->count ( ); i++ )
	{
		CCSprite*	pSpaceDust = (CCSprite*) pSpaceDusts->objectAtIndex ( i );
		KDfloat		fPosX = m_pBackgroundNode->convertToWorldSpace ( pSpaceDust->getPosition ( ) ).x;
		KDfloat		fSize = pSpaceDust->getContentSize ( ).cx;
		if ( fPosX < -fSize ) 
		{
			m_pBackgroundNode->incrementOffset ( ccp ( pSpaceDust->getContentSize ( ).cx * 2, 0 ), pSpaceDust ); 
		}                                  
	} 

	CCArray*	pBackGrounds = CCArray::createWithCapacity ( 4 ) ;
	pBackGrounds->addObject ( m_pGalaxy );
	pBackGrounds->addObject ( m_pPlanetsunrise );
	pBackGrounds->addObject ( m_pSpacialanomaly );
	pBackGrounds->addObject ( m_pSpacialanomaly2 );
	for ( KDuint i = 0; i < pBackGrounds->count ( ); i++ ) 
	{
		CCSprite*	pBackground = (CCSprite*) pBackGrounds->objectAtIndex ( i );
		KDfloat		fPosX = m_pBackgroundNode->convertToWorldSpace ( pBackground->getPosition ( ) ).x;
		KDfloat		fSize = pBackground->getContentSize ( ).cx;
		if ( fPosX < -fSize )
		{
			m_pBackgroundNode->incrementOffset ( ccp ( 2000, 0 ), pBackground ); 
		}                                   
	}

	// Accelerometer stuff moving ship
	CCSize		tLyrSize = this->getContentSize ( );
	KDfloat		fMaxY = tLyrSize.cy - m_pShip->getContentSize ( ).cy / 2;
	KDfloat		fMinY = m_pShip->getContentSize ( ).cy / 2;

	KDfloat		fDiff = m_fShipPointsPerSecY * fDelta;
	KDfloat		fNewY = m_pShip->getPosition ( ).y + fDiff;
	fNewY = KD_MIN ( KD_MAX ( fNewY, fMinY ), fMaxY );
	m_pShip->setPosition ( ccp ( m_pShip->getPosition ( ).x, fNewY ) );    

	// dealing with asteroids
	// Add to bottom of update loop
	KDfloat		fCurTimeMillis = (KDfloat) KD_GET_UST2MSEC;
	
	if ( fCurTimeMillis > m_fNextAsteroidSpawn )
	{
		KDfloat		fRandSecs = CCRANDOM_BETWEEN ( 0.2f, 1.0f ) * 1000;	
		m_fNextAsteroidSpawn = fRandSecs + fCurTimeMillis;

		KDfloat		fRandY = CCRANDOM_BETWEEN ( 0, tLyrSize.cy );
		KDfloat		fRandDuration = CCRANDOM_BETWEEN ( 2.0f, 10.0 );

		CCSprite*	pAsteroid = (CCSprite*) m_pAsteroids->objectAtIndex ( m_nNextAsteroid );
		m_nNextAsteroid++;

		if ( m_nNextAsteroid >= (KDint) m_pAsteroids->count ( ) )
		{
			m_nNextAsteroid = 0;
		}

		pAsteroid->stopAllActions ( );

		pAsteroid->setPosition ( ccp ( tLyrSize.cx + pAsteroid->getContentSize ( ).cx / 2, fRandY ) );
		pAsteroid->setVisible ( KD_TRUE );
		pAsteroid->runAction
		( 
			CCSequence::createWithTwoActions
			(
				CCMoveBy::create ( fRandDuration, ccp ( -tLyrSize.cx - pAsteroid->getContentSize ( ).cx, 0 ) ) ,
				CCHide	::create (  )
			) 
		);
	}  

	// collision detection 
	CCObject*  pObject = KD_NULL;
	CCARRAY_FOREACH ( m_pAsteroids, pObject )
	{
		CCSprite*	pAsteroid = (CCSprite*) pObject;
		if ( !pAsteroid->isVisible ( ) )
		{
			continue;
		}

		CCObject*  pObject2 = KD_NULL;
		CCARRAY_FOREACH ( m_pShipLasers, pObject2 )
		{
			CCSprite*	pShipLaser = (CCSprite*) pObject2;
			if ( !pShipLaser->isVisible ( ) )
			{
				continue;
			}

			if ( pShipLaser->boundingBoxToWorld ( ).intersectsRect ( pAsteroid->boundingBoxToWorld ( ) ) )
			{
				SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/explosion_large.wav" );
				pShipLaser->setVisible ( KD_FALSE );
				pAsteroid ->setVisible ( KD_FALSE );
				continue ;
			}
		}

		if ( m_pShip->boundingBoxToWorld ( ).intersectsRect ( pAsteroid->boundingBoxToWorld ( ) ) )
		{
			pAsteroid->setVisible ( KD_FALSE );
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/explosion_large.wav" );
			m_pShip->runAction ( CCBlink::create ( 1.0f, 9 ) );
			m_nLives--;
		}
	}

	// Game Over stuff
	if ( m_nLives <= 0 ) 
	{
		m_pShip->stopAllActions ( );
		m_pShip->setVisible ( KD_FALSE );
		this->endScene ( KENDREASONLOSE );
	}
	else if ( fCurTimeMillis >= m_fGameOverTime )
	{
		this->endScene ( KENDREASONWIN );
	}
}

KDvoid GameScene::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound/laser_ship.wav" );
	  
	CCSprite*	pShipLaser = (CCSprite*) m_pShipLasers->objectAtIndex ( m_nNextShipLaser++ );
	if ( m_nNextShipLaser >= (KDint) m_pShipLasers->count ( ) )
	{
		m_nNextShipLaser = 0;
	}

	pShipLaser->setPosition ( ccpAdd ( m_pShip->getPosition ( ), ccp ( pShipLaser->getContentSize ( ).cx / 2, 0 ) ) );
	pShipLaser->setVisible ( KD_TRUE );
	pShipLaser->stopAllActions ( );
	pShipLaser->runAction
	( 
		CCSequence::createWithTwoActions 
		(
			CCMoveBy::create ( 0.5f, ccp ( tLyrSize.cx, 0 ) ),
			CCHide  ::create ( )
		)
	);
}

KDvoid GameScene::didAccelerate ( CCAcceleration* pAccelerationValue )
{
    const CCSize    tLyrSize        = this->getContentSize ( );    
    KDfloat         fPitch          = pAccelerationValue->y;
    KDfloat         fFilter         = _KFILTERINGFACTOR_;
    KDfloat         fAlpha          = fFilter;
    
    m_fPrevPitch = fPitch * fAlpha + m_fPrevPitch * ( 1.0f - fAlpha );
	m_fShipPointsPerSecY = tLyrSize.cy * 2.0f * ( m_fPrevPitch - 0.6f );
}

KDvoid GameScene::endScene ( EndReason eEndReason )
{
	if ( m_bGameOver )
	{
		return;
	}
	m_bGameOver = KD_TRUE;

	const CCSize		tLyrSize = this->getContentSize ( );

	CCLabelTTF*			pLabel = CCLabelTTF::create ( eEndReason == KENDREASONLOSE ? "You Win" : "You Loose", "Font/timesbd.ttf", 64 );
	pLabel->setScale ( 0.1f );
	pLabel->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy * 0.6f ) );
	this->addChild ( pLabel );  

	CCLabelTTF*			pRestartLabel = CCLabelTTF::create ( "Restart", "Font/timesbd.ttf", 64 );
	CCMenuItemLabel*	pRestartItem  = CCMenuItemLabel::create ( pRestartLabel, this, menu_selector ( GameScene::restartTapped ) );  
	pRestartItem->setScale ( 0.1f );
	pRestartItem->setPosition ( ccp ( tLyrSize.cx / 2, tLyrSize.cy * 0.4f ) );

	this->addChild ( CCMenu::create ( pRestartItem, KD_NULL ) );

	// clear label and menu
	pRestartItem->runAction ( CCScaleTo::create ( 0.5f, 1.0f ) );
	pLabel		->runAction ( CCScaleTo::create ( 0.5f, 1.0f ) );
	
	// Terminate update
	this->unscheduleUpdate ( );
}

KDvoid GameScene::restartTapped ( CCObject* pSender )
{
	CCScene*    pScene = CCScene::create ( );
	GameScene*  pLayer = GameScene::create ( );
	pScene->addChild ( pLayer );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionZoomFlipX::create ( 0.5f, pScene ) );
	
	// reschedule
	this->scheduleUpdate ( ); 
}