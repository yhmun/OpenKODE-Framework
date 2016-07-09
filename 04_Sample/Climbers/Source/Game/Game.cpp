/* --------------------------------------------------------------------------
 *
 *      File            Game.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Sergey Tikhonov. All rights reserved. 
 *
 *      https://www.github.com/haqu/climbers
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
#include "Game.h"
#include "Intro.h"
#include "Hero.h"
#include "Rock.h"
#include "Grab.h"
#include "Star.h"

#define kNumLevels 15

enum
{
	kTagLabel	= 1	,
	kTagLabel2		,
	kTagLabel3		,
	kTagBottom		,
	kTagTop			,
	kTagWall		,
	kTagFlower		,
	kTagFlowerPS	,
};

KDbool Game::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	sw = m_tContentSize.cx;
	sh = m_tContentSize.cy;

	m_bDragInProgress	= KD_FALSE;
	m_pDragHero			= KD_NULL;
	m_pDragOtherHero	= KD_NULL;

	m_fRopeLength		= sh * 250   / 1024;
	m_fHeroStarDist		= sh * 48.0f / 1024;
	m_fHeroRockDist		= sh * 64.0f / 1024;
	m_fSnapDist			= sh * 64.0f / 1024;

	// sprite sheet
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "sprites.plist" );

	m_pBatch1 = CCSpriteBatchNode::create ( "sprites.png", 50 );
	this->addChild ( m_pBatch1 );

	m_pBatch2 = CCSpriteBatchNode::create ( "sprites.png", 50 );
	this->addChild ( m_pBatch2, 10 );

	// heroes and rope
	m_pHero1 = Hero::create ( ccp ( sw / 2 - m_fRopeLength / 2.0f, sh / 16 ) );
	m_pBatch2->addChild ( m_pHero1 );

	m_pHero2 = Hero::create ( ccp ( sw / 2 + m_fRopeLength / 2.0f, sh / 16 ) );
	m_pBatch2->addChild ( m_pHero2 );

	m_pRopeBatch = CCSpriteBatchNode::create ( "rope.png" );
	m_pRope = VRope::create ( m_pHero1->getPosition ( ), m_pHero2->getPosition ( ), m_pRopeBatch );
	m_pRope->retain ( );
	this->addChild ( m_pRopeBatch, 1 );

	// snap feedback
	m_pSnapFeedback = CCSprite::createWithSpriteFrameName ( "snapFeedback.png" );
	m_pBatch1->addChild ( m_pSnapFeedback );
	m_pSnapFeedback->setOpacity ( 0 );

	// rock
	m_pRock = Rock::create ( ccpz );
	m_pRock->setOpacity ( 0 );
	m_pBatch1->addChild ( m_pRock, 10 );

	// rock alert
	m_pRockAlert = CCSprite::createWithSpriteFrameName ( "rockAlert.png" );
	m_pBatch1->addChild ( m_pRockAlert, 13 );
	m_pRockAlert->setOpacity ( 0 );

	// star icon
	CCSprite*	pSprite;
	pSprite = CCSprite::createWithSpriteFrameName ( "starIcon.png" );
	pSprite->setPosition ( ccp ( 32, sh - 32 ) );
	m_pBatch1->addChild ( pSprite, 15 );
	m_pStarIcon = pSprite;

	// menu button
	pSprite = CCSprite::createWithSpriteFrameName ( "menuButton.png" );
	pSprite->setPosition ( ccp ( sw - 32, sh - 32 ) );
	m_pBatch1->addChild ( pSprite, 15 );
	m_pMenuButton = pSprite;

	// star counter
	KDfloat			fFontSize = 24;
	if ( IS_IPHONE ) 
	{
		fFontSize = 12;
	}

	CCLabelBMFont*	pLabel = CCLabelBMFont::create ( "0/0", "digits.fnt" );
	pLabel->setOpacity ( 128 );
	pLabel->setPosition ( ccp ( 60, sh - 32 ) );
	pLabel->setAnchorPoint ( ccp ( 0, 0.5f ) );
	this->addChild ( pLabel, 15 );
	m_pStarsCollectedLabel = pLabel;

	// arrays
	m_pGrabs = new CCArray ( );
	m_pStars = new CCArray ( );

	m_nCurrentLevel = 0;
	m_nNextLevel	= CCUserDefault::sharedUserDefault ( )->getIntegerForKey ( "currentLevel" ); 
	if ( !m_nNextLevel )
	{
		m_nNextLevel = 1;
	}
	this->resetLevel ( );

	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );

	pSE->preloadEffect ( "levelCompleted.mp3" );
	pSE->preloadEffect ( "levelFailed.mp3" );
	pSE->preloadEffect ( "click.mp3" );
	pSE->preloadEffect ( "grab.mp3" );
	pSE->preloadEffect ( "collectStar.mp3" );
	pSE->preloadEffect ( "dropRock.mp3" );

	pSE->stopBackgroundMusic ( );
	pSE->playBackgroundMusic ( "game.mp3" );

	this->scheduleUpdate ( );

	this->setTouchEnabled ( KD_TRUE );
	this->setTouchMode ( kCCTouchesOneByOne );

	return KD_TRUE;
}

Game::~Game ( KDvoid )
{
	m_pGrabs->release ( );
	m_pStars->release ( );

	m_pRope->release ( );
}

KDbool Game::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );

	CCPoint		tLocation = pTouch->getLocation ( );

	// menu button
	CCRect		tRect = CCRect ( m_pMenuButton->getPositionX ( ) - 32, m_pMenuButton->getPositionY ( ) - 32 + this->getPositionY ( ), 64, 64 );
	if ( tRect.containsPoint ( tLocation ) )
	{
		this->showPopupMenu ( );
		return KD_TRUE;
	}

	if ( !m_bGameInProgress )
	{
		this->resetLevel ( );
		pSE->playEffect ( "grab.mp3" );
		return KD_TRUE;
	}

	m_tLastTouchLocation = tLocation;
	tLocation = ccpSub ( tLocation, this->getPosition ( ) );
	
	static KDfloat	fTapRadius = 64.0f;
		
	if ( ccpDistance ( m_pHero1->getPosition ( ), tLocation ) < fTapRadius )
	{
		m_pDragHero = m_pHero1;
		m_pDragOtherHero = m_pHero2;
		if ( m_pDragOtherHero->getState ( ) != kHeroStateFall )
		{
			m_bDragInProgress = KD_TRUE;
			m_tDragOffset = ccpSub ( m_pDragHero->getPosition ( ), tLocation );
			m_pDragHero->setState ( kHeroStateDrag );
			pSE->playEffect ( "click.mp3" );
		}
		return KD_TRUE;
	}

	if ( ccpDistance ( m_pHero2->getPosition ( ), tLocation ) < fTapRadius )
	{
		m_pDragHero = m_pHero2;
		m_pDragOtherHero = m_pHero1;
		if ( m_pDragOtherHero->getState ( ) != kHeroStateFall )
		{
			m_bDragInProgress = KD_TRUE;
			m_tDragOffset = ccpSub ( m_pDragHero->getPosition ( ), tLocation );
			m_pDragHero->setState ( kHeroStateDrag );
			pSE->playEffect ( "click.mp3" );
		}
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid Game::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !m_bGameInProgress )
	{
		return;
	}
	
	m_tLastTouchLocation = pTouch->getLocation ( );
	
	if ( m_bDragInProgress )
	{
		this->updateDragHeroPositionWithTouchLocation ( m_tLastTouchLocation );
	}
}

KDvoid Game::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !m_bGameInProgress )
	{
		return;
	}

	if ( m_bDragInProgress )
	{
		m_bDragInProgress = KD_FALSE;
		m_pDragHero->setState ( kHeroStateFall );
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "grab.mp3" );
		if ( m_pSnapFeedback->getOpacity ( ) > 0 )
		{
			m_pSnapFeedback->runAction ( CCFadeOut::create ( 0.25f ) );
		}

		if ( m_pDragHero->getPositionY ( ) > m_pDragHero->getTopGroundY ( ) - m_fSnapDist && m_pDragHero->getPositionY ( ) <= m_pDragHero->getTopGroundY ( ) )
		{
			m_pDragHero->setPositionY ( m_pDragHero->getTopGroundY ( ) );
			m_pDragHero->setState ( kHeroStateIdle );
			return;
		}

		CCObject*		pObject;
		CCARRAY_FOREACH ( m_pGrabs, pObject )
		{
			Grab*		pGrab = (Grab*) pObject;
			if ( this->dragHeroNearGrab ( pGrab ) ) 
			{
				m_pDragHero->setPosition ( pGrab->getPosition ( ) );
				m_pDragHero->setState ( kHeroStateGrab );
				m_tCameraOffset = ccp ( 0, m_pDragHero->getPositionY ( ) - sh / 2 );
				if ( m_tCameraOffset.y < 0 )
				{
					m_tCameraOffset = ccpz;
				}
				this->updateCamera ( );
				return;
			}
		}

		if ( m_pDragHero->getPositionY ( ) < 200 )
		{
			m_tCameraOffset = ccpz;
			this->updateCamera ( );
		}
	}
}

KDvoid Game::update ( KDfloat fDelta )
{
	if ( m_bGameInProgress && this->numberOfRunningActions ( ) )
	{
		if ( m_bDragInProgress )
		{
			this->updateDragHeroPositionWithTouchLocation ( m_tLastTouchLocation );
		}
	}
	m_pHero1->update ( fDelta );
	m_pHero2->update ( fDelta );

	if ( m_bGameInProgress && !m_bDragInProgress )
	{
		// standing on top
		if ( m_pHero1->getPositionY ( ) >= m_pHero1->getTopGroundY ( ) && m_pHero2->getPositionY ( ) >= m_pHero2->getTopGroundY ( ) )
		{
			m_tCameraOffset = ccp ( 0, m_fLevelHeight - sh / 2 );
			this->updateCamera ( );
			this->levelCompleted ( );
		}

		// falling down
		if ( m_pHero1->getState ( ) != m_pHero2->getState ( ) && ( m_pHero1->getState ( ) == kHeroStateFall || m_pHero2->getState ( ) == kHeroStateFall ) ) 
		{
			KDfloat		fDist = ccpDistance ( m_pHero1->getPosition ( ), m_pHero2->getPosition ( ) );
			if ( fDist > m_fRopeLength )
			{
				m_pHero1->setState ( kHeroStateFall );
				m_pHero2->setState ( kHeroStateFall );

				KDfloat		vy = KD_MIN ( m_pHero1->getVelocity ( ).y, m_pHero2->getVelocity ( ).y );
				m_pHero1->setVelocity ( ccp ( 0, vy ) );
				m_pHero2->setVelocity ( ccp ( 0, vy ) );
				if ( m_pHero1->getPositionY ( ) > sh / 2 || m_pHero2->getPositionY ( ) > sh / 2 )
				{
					this->levelFailed ( );
				}
			}
		}
	}

	m_pRope->updateWithPoints ( m_pHero1->getPosition ( ), m_pHero2->getPosition ( ), fDelta ); 
	m_pRope->updateSprites ( );

	// if camera moving
	if ( this->numberOfRunningActions ( ) )
	{
		this->updateUIPosition ( );
		if ( m_pRockAlert->numberOfRunningActions ( ) && -this->getPositionY ( ) <= m_fLevelHeight - sh )
		{
			m_pRockAlert->setPositionY ( sh * 31 / 32 - this->getPositionY ( ) );
		}
	}
		
	if ( m_bGameInProgress && m_pRock->isFalling ( ) )
	{
		m_pRock->update ( fDelta );
				
		if ( !m_pRock->isFalling ( ) && m_pRock->getOpacity ( ) > 0 )
		{
			m_pRock->runAction ( CCFadeOut::create ( 0.5f ) ); 
		}
	
		if ( m_bGameInProgress && m_pRock->getOpacity ( ) > 0 && ( m_pHero1->getState ( ) != kHeroStateIdle || m_pHero2->getState ( ) != kHeroStateIdle ) )
		{
			if ( ccpDistance ( m_pHero1->getPosition ( ), m_pRock->getPosition ( ) ) < m_fHeroRockDist ||
			     ccpDistance ( m_pHero2->getPosition ( ), m_pRock->getPosition ( ) ) < m_fHeroRockDist )
			{
				m_pHero1->setState ( kHeroStateFall );
				m_pHero1->setVelocity ( m_pRock->getVelocity ( ) );
				m_pHero2->setState ( kHeroStateFall );
				m_pHero2->setVelocity ( m_pRock->getVelocity ( ) );
				this->levelFailed ( );
			}
		}		
	}	
}

KDvoid Game::loadLevel ( KDvoid )
{
	tinyxml2::XMLDocument	tDoc;
	std::string				sPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( ccszf ( "%02d.svg", m_nCurrentLevel ) );	
	tDoc.LoadFile ( sPath.c_str ( ) );

	tinyxml2::XMLElement*	pChild = tDoc.FirstChildElement ( "svg" )->FirstChildElement ( );

	for ( ; pChild != KD_NULL; pChild = pChild->NextSiblingElement ( ) )
	{
		if ( !kdStrcmp (  pChild->Attribute ( "id" ), "wall" ) )
		{
			m_fLevelHeight = pChild->FloatAttribute ( "height" );
			break;
		}		
	}

	m_fLevelHeight = m_fLevelHeight * sh / 1024;

	this->removeChildByTag ( kTagWall ); 

	CCSprite*	pWall = CCSprite::create ( "wall.png", CCRect ( 0, 0, sw, m_fLevelHeight ) );
	pWall->setPosition ( ccp ( sw / 2, m_fLevelHeight / 2.f ) );
	this->addChild ( pWall, -2, kTagWall );

	ccTexParams	tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	pWall->getTexture ( )->setTexParameters ( &tp );

	m_tStartPosition1 = ccp ( sw / 2 - m_fRopeLength / 3, sh / 16 );
	m_tStartPosition2 = ccp ( sw / 2 + m_fRopeLength / 3, sh / 16 );
	
	// grabs and stars
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pGrabs, pObject )
	{
		Grab*	pGrab = (Grab*) pObject;
		m_pBatch1->removeChild ( pGrab, KD_TRUE );
	}
	m_pGrabs->removeAllObjects ( );

	CCARRAY_FOREACH ( m_pStars, pObject )
	{
		Star*	pStar = (Star*) pObject;
		m_pBatch1->removeChild ( pStar, KD_TRUE );
	}	
	m_pStars->removeAllObjects ( );

	m_nStarsTotal = 0;

	Grab*		pGrab;
	Star*		pStar;
	KDfloat		x, y;
	KDbool		bIsGrab; // grab or star
	CCPoint		tPoint;
	
	while ( ( pChild = pChild->NextSiblingElement ( "circle" ) ) )
	{
		bIsGrab = KD_FALSE;

		const KDchar*	szId = pChild->Attribute ( "id" );

		if ( !kdStrncmp ( szId, "grab", 4 ) )
		{
			bIsGrab = KD_TRUE;
		}
		else if ( !kdStrncmp ( szId, "star", 4 ) )
		{
			bIsGrab = KD_FALSE;
		}

		x = pChild->FloatAttribute ( "cx" );
		y = pChild->FloatAttribute ( "cy" );

		tPoint = ccp ( x * sw / 768, sh - y * sh / 1024 );
		if ( bIsGrab ) 
		{
			pGrab = Grab::create ( tPoint );
			m_pGrabs->addObject ( pGrab );
			m_pBatch1->addChild ( pGrab, 5 );		
		} 
		else 
		{
			pStar = Star::create ( tPoint );			
			m_pStars->addObject ( pStar );
			m_pBatch1->addChild ( pStar, 6 );
			m_nStarsTotal++;
		}		
	}
	
	m_pBatch1->removeChildByTag ( kTagBottom );

	CCSprite*	pBottom = CCSprite::createWithSpriteFrameName ( "bottom.png" );
	pBottom->setPosition ( ccp ( sw / 2, sh / 32 ) );
	pBottom->setTag ( kTagBottom );
	m_pBatch1->addChild ( pBottom, 11 );
	
	m_pBatch1->removeChildByTag ( kTagTop );

	CCSprite*	pTop = CCSprite::createWithSpriteFrameName ( "top.png" );
	pTop->setPosition ( ccp ( sw / 2, m_fLevelHeight ) );
	pTop->setTag ( kTagTop );
	m_pBatch1->addChild ( pTop, 11 );

	m_pBatch1->removeChildByTag ( kTagFlower );

	CCSprite*	pFlower = CCSprite::createWithSpriteFrameName ( "flower.png" );
	pFlower->setPosition ( ccp ( sw / 2, m_fLevelHeight ) );
	pFlower->setTag ( kTagFlower );
	m_pBatch1->addChild ( pFlower, 12 );
}

KDvoid Game::resetLevel ( KDvoid )
{
	if ( m_nNextLevel != m_nCurrentLevel )
	{
		m_nCurrentLevel = m_nNextLevel;
		if ( m_nCurrentLevel > kNumLevels )
		{
			CCDirector::sharedDirector ( )->replaceScene ( Intro::scene ( ) );			
			return;
		}
		this->loadLevel ( );
	}

	this->removeChildByTag ( kTagLabel );
	this->removeChildByTag ( kTagLabel2 );
	this->removeChildByTag ( kTagLabel3 );
	this->removeChildByTag ( kTagFlowerPS );

	m_pHero1->setState ( kHeroStateIdle );
	m_pHero2->setState ( kHeroStateIdle );

	m_pHero1->setPosition ( m_tStartPosition1 );
	m_pHero2->setPosition ( m_tStartPosition2 );

	m_pHero1->setTopGroundY ( m_fLevelHeight + sh * 36 / 1024 );
	m_pHero2->setTopGroundY ( m_fLevelHeight + sh * 36 / 1024 );

	m_pRope->resetWithPoints ( m_pHero1->getPosition ( ), m_pHero2->getPosition ( ) );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pStars, pObject )
	{
		Star*	pStar = (Star*) pObject;
		pStar->setCollected ( KD_FALSE );
		pStar->runAction ( CCSpawn::createWithTwoActions ( CCScaleTo::create ( 0.5f, 1.f ), CCFadeIn::create ( 0.5f ) ) );

	}

	m_nStarsCollected = 0;
	this->updateStarsCollectedLabel ( );

	if ( m_pSnapFeedback->getOpacity ( ) > 0 )
	{
		m_pSnapFeedback->runAction ( CCFadeOut::create ( 0.25f ) );
	}

	m_tCameraOffset = ccpz;
	this->stopAllActions ( );
	this->setPosition ( ccpz );

	this->updateUIPosition ( );

	if ( m_pRock->isFalling ( ) )
	{
		m_pRock->setFalling ( KD_FALSE );
		m_pRock->setOpacity ( 0 );
	}

	m_bDragInProgress = KD_FALSE;
	m_bGameInProgress = KD_TRUE;
	this->scheduleRockAlert ( );
}

KDvoid Game::updateStarsCollectedLabel ( KDvoid )
{
	m_pStarsCollectedLabel->setString ( ccszf ( "%d/%d", m_nStarsCollected, m_nStarsTotal ) );
}

KDvoid Game::updateUIPosition ( KDvoid )
{
	KDfloat		fY = sh - 32 - this->getPositionY ( );
	m_pStarIcon->setPositionY ( fY );
	m_pStarsCollectedLabel->setPositionY ( fY );
	m_pMenuButton->setPositionY ( fY );
}

KDvoid Game::updateCamera ( KDvoid )
{
	this->runAction ( CCEaseInOut::create ( CCMoveTo::create ( 2, ccpNeg ( m_tCameraOffset ) ), 2 ) );
}

KDvoid Game::scheduleRockAlert ( KDvoid )
{
	KDfloat		fDelay = ( kdRand ( ) % 100 ) / 100.0f * 7.0f + 3.0f;	// 3-10 seconds
	this->scheduleOnce ( schedule_selector ( Game::showRockAlert ), fDelay );
}

KDvoid Game::showRockAlert ( KDfloat fDelta )
{
	if ( !m_bGameInProgress )
	{
		return;
	}

	if ( -this->getPositionY ( ) >= m_fLevelHeight - sh )
	{
		return;
	}

	KDfloat		fPadding = sw * 128 / 768;
	KDfloat		x = ( kdRand ( ) % (KDint) ( sw - fPadding * 2 ) ) + fPadding;
	m_pRockAlert->setPosition ( ccp ( x, sh * 31 / 32 - this->getPositionY ( ) ) );

	m_pRockAlert->runAction 
	(
		CCSequence::create 
		(
			CCFadeIn ::create ( 0.5f ),
			CCFadeOut::create ( 0.5f ),
			CCFadeIn ::create ( 0.5f ),
			CCFadeOut::create ( 0.5f ),
			CCFadeIn ::create ( 0.5f ),
			CCFadeOut::create ( 0.5f ),
			CCCallFunc::create ( this, callfunc_selector ( Game::dropRock ) ),
			KD_NULL
		)
	);
}

KDvoid Game::dropRock ( KDvoid )
{
	m_pRock->setPosition ( ccp ( m_pRockAlert->getPositionX ( ), m_fLevelHeight ) );
	m_pRock->setFalling ( KD_TRUE );
	m_pRock->runAction ( CCFadeIn::create ( 0.5f ) );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "dropRock.mp3" );
	this->scheduleRockAlert ( );
}

KDbool Game::dragHeroNearGrab ( Grab* pGrab )
{
	KDfloat		fDist = ccpDistance ( m_pDragHero->getPosition ( ), pGrab->getPosition ( ) );
	if ( fDist < m_fSnapDist )
	{
		return KD_TRUE;
	}
	return KD_FALSE;
}

KDvoid Game::sparkleAt ( const CCPoint& tPosition )
{
	CCParticleSystem*	pParticle = CCParticleExplosion::create ( );
	this->addChild ( pParticle, 12 );
	pParticle->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "stars.png" ) );
	pParticle->setPosition ( tPosition );
	pParticle->setLife ( 1.0f );
	pParticle->setLifeVar ( 1.0f );
	pParticle->setTotalParticles ( 60 );
	pParticle->setAutoRemoveOnFinish ( KD_TRUE );
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "collectStar.mp3" );
}

KDvoid Game::updateDragHeroPositionWithTouchLocation ( const CCPoint& tTouchLocation )
{
	CCPoint		tLevelLocation = ccpSub ( tTouchLocation, this->getPosition ( ) );
	KDfloat		fDist;

	m_pDragHero->setPosition ( ccpAdd ( tLevelLocation, m_tDragOffset ) );

	CCPoint		tHeroDelta = ccpSub ( m_pDragHero->getPosition ( ), m_pDragOtherHero->getPosition ( ) );

	fDist = ccpLength ( tHeroDelta );
	
	if ( fDist > m_fRopeLength )
	{
		CCPoint		tDir = ccpNormalize ( tHeroDelta );
		m_pDragHero->setPosition ( ccpAdd ( m_pDragOtherHero->getPosition ( ), ccpMult ( tDir, m_fRopeLength ) ) );
	}

	// collect m_pStars
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pStars, pObject )
	{
		Star*		pStar = (Star*) pObject;
		if ( pStar->isCollected ( ) )
		{
			continue;
		}

		fDist = ccpDistance ( m_pDragHero->getPosition ( ), pStar->getPosition ( ) );
		if ( fDist < m_fHeroStarDist )
		{
			pStar->setCollected ( KD_TRUE );
			m_nStarsCollected++;
			this->updateStarsCollectedLabel ( );
			pStar->runAction ( CCSpawn::createWithTwoActions ( CCScaleTo::create ( 0.5f, 0.3f ), CCFadeOut::create ( 0.5f ) ) );
			this->sparkleAt ( pStar->getPosition ( ) );
			break;
		}
	}
	// snap feedback
	KDbool		bSnapped = KD_FALSE;

	CCARRAY_FOREACH ( m_pGrabs, pObject )
	{
		Grab*		pGrab = (Grab*) pObject;
		KDfloat		fDist = ccpDistance ( m_pDragHero->getPosition ( ), pGrab->getPosition ( ) );
		if ( fDist < m_fSnapDist )
		{
			KDfloat		t = ( m_fSnapDist - fDist ) / m_fSnapDist;
			m_pSnapFeedback->setScale (  t * 0.75f + 0.25f );
			m_pSnapFeedback->setOpacity ( (GLubyte) ( t * 255.0f ) );
			m_pSnapFeedback->setPosition ( pGrab->getPosition ( ) );
			bSnapped = KD_TRUE;
			break;
		}
	}

	if ( !bSnapped && m_pSnapFeedback->getOpacity ( ) > 0 )
	{
		m_pSnapFeedback->setOpacity ( 0 );
	}
}

KDvoid Game::levelCompleted ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "levelCompleted.mp3" );
	m_bGameInProgress = KD_FALSE;

	KDfloat		fFontSize = 64.0f;
	if ( IS_IPHONE )
	{
		fFontSize = 32.0f;
	}

	CCLabelTTF*		pLabel;
	if ( m_nCurrentLevel == kNumLevels )
	{
		pLabel = CCLabelTTF::create ( "Yeah! You did it!", "ChalkboardSE.ttc", fFontSize );
		pLabel->setColor ( ccc3 ( 255, 255, 255 ) );
		pLabel->setPosition ( ccp ( sw / 2, m_fLevelHeight + sh * 3 / 8 ) );
		pLabel->setTag ( kTagLabel );
		this->addChild ( pLabel, 12 );
		
		pLabel = CCLabelTTF::create ( "Finally, you overcame all difficulties on your way. Great job!", "ChalkboardSE.ttc", fFontSize * 3 / 8 );
		pLabel->setColor ( ccc3 ( 255, 255, 255 ) );
		pLabel->setPosition ( ccp ( sw / 2, m_fLevelHeight + sh * 5 / 16 ) );
		pLabel->setTag ( kTagLabel2 );
		this->addChild ( pLabel, 12 );	
	} 
	else 
	{
		pLabel = CCLabelTTF::create ( "Well Done!", "ChalkboardSE.ttc", fFontSize );
		pLabel->setColor ( ccc3 ( 255, 255, 255 ) );
		pLabel->setPosition ( ccp ( sw / 2, m_fLevelHeight + sh * 3 / 8 ) );
		pLabel->setTag ( kTagLabel );
		this->addChild ( pLabel, 12 );
		
		pLabel = CCLabelTTF::create ( ccszf ( "Level %d completed" , m_nCurrentLevel ), "ChalkboardSE.ttc", fFontSize * 3 / 8 );
		pLabel->setColor ( ccc3 ( 255, 255, 255 ) );
		pLabel->setPosition ( ccp ( sw / 2, m_fLevelHeight + sh * 5 / 16 ) );
		pLabel->setTag ( kTagLabel2 );
		this->addChild ( pLabel, 12 );	
	}

	CCParticleSystem*	pParticle = CCParticleFireworks::create ( );
	pParticle->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage( "stars.png" ) );
	pParticle->setStartSize ( 4 );
//	pParticle->setEndSize ( 10.0f );
	pParticle->setSpeed ( 100 );
	pParticle->setPosition ( ccp ( sw / 2 - sw * 9 / 768, m_fLevelHeight + sh * 65 / 1024 ) );
	this->addChild ( pParticle, 10, kTagFlowerPS );
		
	m_nNextLevel = m_nCurrentLevel + 1;
	if ( m_nNextLevel > kNumLevels )
	{
		CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "currentLevel", 1 );
	}
	else 
	{
		CCUserDefault::sharedUserDefault ( )->setIntegerForKey ( "currentLevel", m_nNextLevel );
	}
	CCUserDefault::sharedUserDefault ( )->flush ( );
//	this->sparkleAt:ccp ( 384 - 8, m_fLevelHeight + 64) );
}

KDvoid Game::levelFailed ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "levelFailed.mp3" );
	m_bGameInProgress = KD_FALSE;

	if ( m_pRock->getOpacity ( ) > 0 )
	{
		m_pRock->runAction ( CCFadeOut::create ( 0.5f ) );
	}

	KDfloat		fFontSize = 64.0f;
	if ( IS_IPHONE )
	{
		fFontSize = 32.0f;
	}

	CCLabelTTF*		pLabel = CCLabelTTF::create ( "NO-O-O-O!", "ChalkboardSE.ttc", fFontSize );
	pLabel->setColor ( ccc3 ( 240, 0, 0 ) );
	pLabel->setPosition ( ccp ( sw / 2, sh * 7 / 8 - this->getPositionY ( ) ) );
	pLabel->setTag ( kTagLabel );
	this->addChild ( pLabel, 12 );
	m_nNextLevel = m_nCurrentLevel;
}

KDvoid Game::showPopupMenu ( KDvoid )
{
	m_bGameInProgress = KD_FALSE;

//	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Game Paused" message:nil delegate:self cancelButtonTitle:@"Continue" otherButtonTitles:@"Main Menu", nil );
//	[alert show );
//	[alert release );

	SimpleAudioEngine::sharedEngine ( )->pauseBackgroundMusic ( );
}

#if 0
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	[[SimpleAudioEngine sharedEngine] resumeBackgroundMusic );
	if(buttonIndex == 1) {
		// main menu
		[[CCDirector sharedDirector] pushScene:[Intro scene] );
	} else {
		if(this->getChildByTag:kTagLabel]) {
			// level completed or failed, nop
		} else {
			m_bGameInProgress = YES;
		}
	}
}

@end

#endif	// 0