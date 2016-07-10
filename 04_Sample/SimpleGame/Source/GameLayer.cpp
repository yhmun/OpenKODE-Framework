/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2010      Ray Wenderlich
 *
 *         http://www.cocos2d-x.org  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "GameOverLayer.h"

GameLayer::GameLayer ( KDvoid )
{
	m_pTargets = KD_NULL;
	m_pProjectiles = KD_NULL;
	m_nProjectilesDestroyed = 0;
}

GameLayer::~GameLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTargets );
	CC_SAFE_RELEASE ( m_pProjectiles );
}

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayerColor::initWithColor ( ccc4 ( 32, 32, 32, 128 ) ) )
	{
		return KD_FALSE;
	}

	const CCRect&  tVisibleRect = CCEGLView::sharedOpenGLView ( )->getVisibleRect ( );
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCMenuItemImage*  pClose = CCMenuItemImage::create 
	(
		"Images/CloseNormal.png",
		"Images/CloseSelected.png",
		this, menu_selector ( GameLayer::onClose )
	);
	pClose->setPosition ( ccp ( tVisibleRect.getMaxX ( ) - 20, tVisibleRect.getMaxY ( ) - 20 ) );
	this->addChild ( CCMenu::createWithItem ( pClose ), 1 );

	CCSprite*  pPlayer = CCSprite::create ( "Images/Player.png" );
	pPlayer->setPosition ( ccp ( pPlayer->getContentSize ( ).cx / 2, tWinSize.cy / 2 ) );
	this->addChild ( pPlayer );

	this->schedule ( schedule_selector ( GameLayer::updateGame ) );
	this->schedule ( schedule_selector ( GameLayer::gameLogic ), 1.0 );
	this->setTouchEnabled ( KD_TRUE );

	m_pTargets     = new CCArray ( );
	m_pProjectiles = new CCArray ( );

	CCSound::sharedSound ( )->Open ( "Sounds/background-music-aac.wav", XM_SOUND_PLAY | XM_SOUND_REPEAT );
	m_uProjectileEffect = CCSound::sharedSound ( )->Open ( "Sounds/pew-pew-lei.wav", XM_SOUND_EFFECT );

	return KD_TRUE;
}

KDvoid GameLayer::onClose ( CCObject* pSender )
{  
	CCDirector::sharedDirector ( )->end ( );
}

KDvoid GameLayer::addTarget ( KDvoid )
{
	CCSprite*  pTarget = CCSprite::create ( "Images/Target.png" );

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	const CCSize&  tSize = pTarget->getContentSize ( );

	KDfloat  fMinY = tSize.cy / 2;
	KDfloat  fMaxY = tWinSize.cy -  pTarget->getContentSize ( ).cy / 2;

	KDint    nRangeY  = (KDint) ( fMaxY - fMinY );
	KDint    nActualY = ( kdRand ( ) % nRangeY ) + (KDint) fMinY;

	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	pTarget->setPosition ( ccp ( tWinSize.cx + ( tSize.cx / 2 ), nActualY ) );
	this->addChild ( pTarget );

	// Determine speed of the target
	KDint  nMinDufRation = 2;
	KDint  nMaxDufRation = 4;
	KDint  nRangeDufRation  = nMaxDufRation - nMinDufRation;
	KDint  nActualDufRation = ( kdRand ( ) % nRangeDufRation ) + nMinDufRation;

	pTarget->runAction ( CCSequence::createWithTwoActions 
	(
		CCMoveTo   ::create ( (KDfloat) nActualDufRation, ccp ( -pTarget->getContentSize ( ).cx / 2, nActualY ) ), 
		CCCallFuncN::create ( this, callfuncN_selector ( GameLayer::spriteMoveFinished ) )
	) );

	// Add to targets array
	pTarget->setTag ( 1 );
	m_pTargets->addObject ( pTarget );
}

KDvoid GameLayer::spriteMoveFinished ( CCNode* pSender )
{
	CCSprite*  pSprite = (CCSprite*) pSender;

	this->removeChild ( pSprite, KD_TRUE );

	if ( pSprite->getTag ( ) == 1 )			// target
	{
		m_pTargets->removeObject ( pSprite );

		CCScene*        pScene = CCScene::create ( );
		GameOverLayer*  pLayer = GameOverLayer::create ( );

		pLayer->getLabel ( )->setString ( "You Lose :(" );
		pScene->addChild ( pLayer );

		CCDirector::sharedDirector ( )->replaceScene ( pScene );
	}
	else if ( pSprite->getTag ( ) == 2 )	// tile
	{
		m_pProjectiles->removeObject ( pSprite );
	}
}

KDvoid GameLayer::gameLogic ( KDfloat fDelta )
{
	//this->addTarget ( );
}

KDvoid GameLayer::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );
}

KDbool GameLayer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	return KD_TRUE;
}

KDvoid GameLayer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCPoint  tLocation = pTouch->getLocation ( );

	// Set up initial location of projectile
	CCSprite*  pProjectile = CCSprite::create ( "Images/Projectile.png" );
	pProjectile->setPosition ( ccp ( 120, tWinSize.cy / 2 ) );

	// Determinie offset of location to projectile
	KDfloat  fOffX = tLocation.x - pProjectile->getPosition ( ).x;
	KDfloat  fOffY = tLocation.y - pProjectile->getPosition ( ).y;

	// Bail out if we are shooting down or backwards
	if ( fOffX <= 0 )
	{
		return;
	}

	// Ok to add now - we've double checked position
	this->addChild ( pProjectile );

	// Determine where we wish to shoot the projectile to
	KDfloat  fRealX = tWinSize.cx + ( pProjectile->getContentSize ( ).cx / 2 );
	KDfloat  fRatio = fOffY / fOffX;
	KDfloat  fRealY = ( ( fRealX - pProjectile->getPosition ( ).x ) * fRatio ) + pProjectile->getPosition ( ).y;
	CCPoint  tRealDest = ccp ( fRealX, fRealY );

	// Determine the fLength of how far we're shooting
	KDfloat  fOffRealX = fRealX - pProjectile->getPosition ( ).x;
	KDfloat  fOffRealY = fRealY - pProjectile->getPosition ( ).y;
	KDfloat  fLength   = kdSqrtf ( ( fOffRealX * fOffRealX ) + ( fOffRealY * fOffRealY ) );
	KDfloat  fVelocity = 480 / 1; // 480pixels/1sec
	KDfloat  fRealMoveDufRation = fLength / fVelocity;

	// Move projectile to actual endpoint
	pProjectile->runAction ( CCSequence::createWithTwoActions 
	(
		CCMoveTo   ::create ( fRealMoveDufRation, tRealDest ),
		CCCallFuncN::create ( this, callfuncN_selector ( GameLayer::spriteMoveFinished ) )
	) );

	// Add to projectiles array
	pProjectile->setTag ( 2 );
	m_pProjectiles->addObject ( pProjectile );

	CCSound::sharedSound ( )->Rewind ( m_uProjectileEffect );
}

KDvoid GameLayer::updateGame ( KDfloat fDelta )
{
	CCArray*   pProjectilesToDelete = new CCArray ( );
	CCObject*  pObject1;
	CCObject*  pObject2;

    CCARRAY_FOREACH ( m_pProjectiles, pObject1 )
    {
        CCSprite*  pProjectile = (CCSprite*) pObject1;
		CCPoint    tProjectilePos  = pProjectile->getPosition ( );
		CCSize     tProjectileSize = pProjectile->getContentSize ( );
		CCRect     tProjectileRect = CCRect ( tProjectilePos.x - tProjectileSize.cx / 2, tProjectilePos.y - tProjectileSize.cy / 2, tProjectileSize.cx, tProjectileSize.cy );

		CCArray*   pTagetsToDelete = new CCArray ( );
		CCARRAY_FOREACH ( m_pTargets, pObject2 )
		{
			CCSprite*  pTarget = (CCSprite*) pObject2;
			CCPoint    tTargetPos  = pTarget->getPosition ( );
			CCSize     tTargetSize = pTarget->getContentSize ( );
			CCRect     tTargetRect = CCRect ( tTargetPos.x - tTargetSize.cx / 2, tTargetPos.y - tTargetSize.cy / 2, tTargetSize.cx, tTargetSize.cy );

			if ( tProjectileRect.intersectsRect ( tTargetRect ) )
			{
				pTagetsToDelete->addObject ( pTarget );
			}
		}

		CCARRAY_FOREACH ( pTagetsToDelete, pObject2 )
		{
			CCSprite*  pTarget = (CCSprite*) pObject2;
			
			m_pTargets->removeObject ( pTarget );
			this->removeChild ( pTarget, KD_TRUE );

			m_nProjectilesDestroyed++;

			if ( m_nProjectilesDestroyed >= 5 )
			{
				CCScene*        pScene = CCScene::create ( );
				GameOverLayer*  pLayer = GameOverLayer::create ( );

				pLayer->getLabel ( )->setString ( "You Win!" );
				pScene->addChild ( pLayer );

				CCDirector::sharedDirector ( )->replaceScene ( pScene );
			}
		}

		if ( pTagetsToDelete->count ( ) > 0 )
		{
			pProjectilesToDelete->addObject ( pProjectile );
		}

		pTagetsToDelete->release ( );
    } 

	CCARRAY_FOREACH ( pProjectilesToDelete, pObject1 )
	{
		CCSprite*  pProjectile = (CCSprite*) pObject1;

		m_pProjectiles->removeObject ( pProjectile );
		this->removeChild ( pProjectile, KD_TRUE );
	}

	pProjectilesToDelete->release ( );
}

