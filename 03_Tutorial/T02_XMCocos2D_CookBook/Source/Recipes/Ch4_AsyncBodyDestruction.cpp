/* --------------------------------------------------------------------------
 *
 *      File            Ch4_AsyncBodyDestruction.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch4_AsyncBodyDestruction.h"
#include "Libraries/GameMisc.h"

KDbool Ch4_AsyncBodyDestruction::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Tap to throw a grenade." );
													
	// Create level boundaries
	this->addLevelBoundaries ( );
	
	// Add gunman
	this->addGunman ( );
	
	// Initialize explosion animation
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "explosion5.plist" );
	
	// Initialize grenade container
	m_pGrenades = CCArray::create ( );
	m_pGrenades->retain ( );

	m_pExplosions = CCArray::create ( );
	m_pExplosions->retain ( );

	return KD_TRUE;
}

KDvoid Ch4_AsyncBodyDestruction::onExit ( KDvoid )
{
	m_pGrenades->release ( );
	m_pExplosions->release ( );

	GameArea2D::onExit ( );
}

/// Animate the arm and launch a grenade 
KDvoid Ch4_AsyncBodyDestruction::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pGunmanArm->runAction
	(
		CCSequence::create
		(
			CCCallFunc::create ( this, callfunc_selector ( Ch4_AsyncBodyDestruction::showArm ) ),
			CCRotateTo::create ( 0.1f, -60 ),
			CCCallFunc::create ( this, callfunc_selector ( Ch4_AsyncBodyDestruction::throwGrenade ) ),
			CCRotateTo::create ( 0.3f, 30 ),
			CCCallFunc::create ( this, callfunc_selector ( Ch4_AsyncBodyDestruction::hideArm ) ),
			KD_NULL
		)
	);
}

KDvoid Ch4_AsyncBodyDestruction::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	// Grenade life cycle
	
	CCObject*	pObject;

	// Grenade removal container
	CCArray*	pGrenadesToRemove = CCArray::create ( );
	
	// Explode any grenades
	CCARRAY_FOREACH ( m_pGrenades, pObject )
	{
		GameMisc*	pGrenade = (GameMisc*) pObject;

		// If a grenade is out of life we mark it for destruction, do cleanup and finally animate an explosion
		KDfloat		fLife = pGrenade->getLife ( );

		fLife -= fDelta;
		pGrenade->setLife ( fLife );

		if ( fLife < 0 )
		{
			CCSprite*	pSprite = pGrenade->getSprite ( );

			this->explosionAt ( pSprite->getPosition ( ) );
			this->markBodyForDestruction ( pGrenade );
			pGrenadesToRemove->addObject ( pObject );			
			m_pGameNode->removeChild ( pSprite );
		}
	}

	// Remove marked grenades
	CCARRAY_FOREACH ( pGrenadesToRemove, pObject )
	{
		m_pGrenades->removeObject ( pObject );
	}

	// Explosion life cycle
	CCArray*	pExplosionsToRemove = CCArray::create ( );
	CCARRAY_FOREACH ( m_pExplosions, pObject )
	{
		GameMisc*	pExplosion = (GameMisc*) pObject;

		KDfloat		fLife = pExplosion->getLife ( );
		fLife -= fDelta;
		pExplosion->setLife ( fLife );

		if ( fLife < 0 )
		{
			m_pGameNode->removeChild ( pExplosion->getSprite ( ) );
			pExplosionsToRemove->addObject ( pExplosion );
		}
	}
	m_pExplosions->removeObjectsInArray ( pExplosionsToRemove );
}

/// Add a gunman sprite and set his arm position 
KDvoid Ch4_AsyncBodyDestruction::addGunman ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "gunman.plist" );

	m_pGunmanArm = CCSprite::createWithSpriteFrameName ( "gunman_stand_right_arm.png" );
	m_pGunmanArm->setPosition ( ccp ( 65, 110 ) );
	m_pGunmanArm->setAnchorPoint ( ccp ( 0.4f, 0.6f) );
	m_pGunmanArm->setRotation ( -90 );
	m_pGunmanArm->setVisible ( KD_FALSE );
	m_pGameNode->addChild ( m_pGunmanArm, 1 );

	m_pGunman = CCSprite::createWithSpriteFrameName ( "gunman_stand_right.png" );
	m_pGunman->setPosition ( ccp ( 70, 100 ) );
	m_pGameNode->addChild ( m_pGunman, 2 );
}

/// Callback for showing the arm using an animation 
KDvoid Ch4_AsyncBodyDestruction::showArm ( KDvoid )
{
	m_pGunmanArm->setVisible ( KD_TRUE );
	
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );	

	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 1.f );
	pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( "gunman_stand_right_no_arm.png" ) );
	m_pGunman->runAction ( CCAnimate::create ( pAnimation ) ); 
}

/// Callback for throwing the arm. This involves animating the arm and creating a grenade 
KDvoid Ch4_AsyncBodyDestruction::throwGrenade ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );	

	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 1.f );
	pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( "gunman_stand_right_arm_empty.png" ) );
	m_pGunmanArm->runAction ( CCAnimate::create ( pAnimation ) ); 

	GameMisc*		pGrenade = GameMisc::create ( );
	pGrenade->setLife ( 5.0f );
	pGrenade->setGameArea ( this );

	CCPoint			tGrenadePos = ccp ( 65, 150 );

	b2BodyDef*		pBodyDef = pGrenade->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tGrenadePos.x / PTM_RATIO, tGrenadePos.y / PTM_RATIO );
	pBodyDef->userData = pGrenade;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pGrenade->setBody ( pBody );
	pBody->SetTransform ( b2Vec2 ( tGrenadePos.x / PTM_RATIO, tGrenadePos.y / PTM_RATIO ), KD_PI_F / 2 );

	b2FixtureDef*	pFixtureDef = pGrenade->getFixtureDef ( );

	CCPoint			tTextureSize = ccp ( 16, 16 );
	CCPoint			tShapeSize = ccp ( 7, 7 );

	CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( "gunman_grenade.png" );
	pSprite->setPosition ( tGrenadePos );
	pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
	pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );
	pGrenade->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 1 );

	b2CircleShape*		pShape = new b2CircleShape ( );
	pGrenade->setCircleShape ( pShape );
	pShape->m_radius = tShapeSize.x / PTM_RATIO;

	pFixtureDef->shape = pShape;		
	pBody->CreateFixture ( pFixtureDef );

	m_pGrenades->addObject ( pGrenade );
	pBody->ApplyLinearImpulse ( b2Vec2 ( 1.0f, 2.0f ), pBody->GetPosition ( ) );
	pBody->SetAngularVelocity ( KD_PI_F );
}

/// Hide the arm when finished 
KDvoid Ch4_AsyncBodyDestruction::hideArm ( KDvoid )
{
	m_pGunmanArm->setVisible ( KD_FALSE );
	m_pGunmanArm->setRotation ( -90 );
	
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );	

	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 1.f );
	pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( "gunman_stand_right.png" ) );
	m_pGunman->runAction ( CCAnimate::create ( pAnimation ) ); 

	CCAnimation*	pAnimationArm = CCAnimation::create ( );
	pAnimationArm->setDelayPerUnit ( 1.f );
	pAnimationArm->addSpriteFrame ( pCache->spriteFrameByName ( "gunman_stand_right_arm.png" ) );
	m_pGunmanArm->runAction ( CCAnimate::create ( pAnimationArm ) ); 
}

/// Animate an explosion at a position 
KDvoid Ch4_AsyncBodyDestruction::explosionAt ( const CCPoint tPosition )
{
	KDfloat			fDelay = 0.035f;
	KDfloat			fDuration = 25 * fDelay;

	GameMisc*		pExplosion = GameMisc::create ( );
	pExplosion->setLife ( fDuration );

	CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( "explosion5_0000.png" );
	pSprite->setPosition ( tPosition );
	pSprite->setScale ( 2.f );
	pExplosion->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 2 );

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );	

	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( fDelay );

	for ( KDint i = 0; i < 10; i++ )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "explosion5_000%d.png", i ) ) );
	}
	for ( KDint i = 10; i < 25; i++ )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "explosion5_00%d.png", i ) ) );
	}
	pSprite->stopAllActions ( );
	pSprite->runAction 
	(
		CCSpawn::create 
		( 
			CCFadeOut::create ( fDuration ),
			CCAnimate::create ( pAnimation ),
			KD_NULL
		)
	); 

	m_pExplosions->addObject ( pExplosion );
	
	// Grenades that get caught in the blast are thrown in appropriate directions 
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pGrenades, pObject )
	{
		GameMisc*	pGrenade = (GameMisc*) pObject;
		b2Body*		pGrenadeBody = pGrenade->getBody ( );
		CCPoint		tGrenadePos = pGrenade->getSprite ( )->getPosition ( );

		if ( pGrenade->getLife ( ) > 0 && distanceBetweenPoints ( tGrenadePos, tPosition ) < 50 )
		{
			CCPoint		tVect = ccp ( tGrenadePos.x - tPosition.x, tGrenadePos.y - tPosition.y );
			pGrenadeBody->ApplyLinearImpulse ( b2Vec2 ( tVect.x / 10, tVect.y / 10 ), pGrenadeBody->GetPosition ( ) );
		}
	}
}