/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Bullets.cpp
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
#include "Ch4_Bullets.h"

KDbool Ch4_Bullets::init ( KDvoid )
{	
	if ( !SideScrollerRecipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Press 'B' to fire some bullets." );

	// Add PLIST files
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "blastmark.plist" );
	pCache->addSpriteFramesWithFile ( "muzzleflash.plist" );
	pCache->addSpriteFramesWithFile ( "crate_break.plist" );

	// Initialization
	m_fFireCount  = 0;
	m_pBullets	  = new CCArray ( );
	m_pExplosions = new CCArray ( );
	
	// Initialize muzzleFlash
	m_pMuzzleFlash = CCSprite::createWithSpriteFrameName ( "muzzleflash_0010.png" );
	m_pMuzzleFlash->setPosition ( ccp ( 60, 5 ) );
	m_pMuzzleFlash->setScale ( 2.0f );
	m_pMuzzleFlash->setOpacity ( 0 );
	m_pGunman->getSprite ( )->addChild ( m_pMuzzleFlash, 2 );
	
	// Create buttons		
	this->createButtonWithPosition ( ccp ( 340, 75 ), "b_button_up.png", "b_button_down.png", "B" );
	this->createButtonWithPosition ( ccp ( 420, 75 ), "a_button_up.png", "a_button_down.png", "A" );

	// Add brick ground and crates
	for ( KDint x = 0; x < 480; x += 40 )
	{
		this->addBoxWithPosition ( ccp ( x, 40 ), "bricks.png", 5.0f );
	}

	for ( KDint i = 0; i < 12; i++ )
	{
		KDint		x = kdRand ( ) % 480;
		KDint		y = kdRand ( ) % 200 + 120;
		
		this->addBoxWithPosition ( ccp ( x, y ), "crate2.png", 1.0f );
	}

	return KD_TRUE;
}

KDvoid Ch4_Bullets::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pBullets );
	CC_SAFE_DELETE ( m_pExplosions );
	
	SideScrollerRecipe::onExit ( );
}

KDvoid Ch4_Bullets::step ( KDfloat fDelta )
{
	SideScrollerRecipe::step ( fDelta );

	// Set muzzleFlash position
	KDint		nDirection = m_pDPad->getDirection ( );
	if ( nDirection == LEFT || nDirection == UP_LEFT || nDirection == DOWN_LEFT )
	{
		m_pMuzzleFlash->setPosition ( ccp ( -60, 5 ) );	
	}
	else if ( nDirection == RIGHT || nDirection == UP_RIGHT || nDirection == DOWN_RIGHT )
	{
		m_pMuzzleFlash->setPosition ( ccp ( 60, 5 ) );	
	}

	// Process button input
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "B" ) )
		{
			this->fireGun ( );
		}
		else if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			this->processJump ( );
		}
		else if ( !pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			m_fJumpCounter = -10.f;
		}
	}

	// Weapon fire limiter
	m_fFireCount -= fDelta;
	
	// Bullet life cycle
	CCArray*	pBulletsToRemove = CCArray::create ( );
	CCARRAY_FOREACH ( m_pBullets, pObject )
	{
		GameMisc*		pBullet = (GameMisc*) pObject;
		KDfloat			fLife	= pBullet->getLife ( );
		fLife -= fDelta;
		pBullet->setLife ( fLife );
		if ( fLife < 0 )
		{
			this->markBodyForDestruction ( pBullet );
			pBulletsToRemove->addObject ( pBullet );
			m_pGameNode->removeChild ( pBullet->getSprite ( ), KD_FALSE );
		}
	}
	m_pBullets->removeObjectsInArray ( pBulletsToRemove );

	// Box life cycle
	CCArray*		pBoxesToRemove = CCArray::create ( );	
	CCARRAY_FOREACH ( m_pBoxes, pObject )
	{
		GameMisc*		pBox  = (GameMisc*) pObject;
		KDfloat			fLife = pBox->getLife ( );
		if ( fLife < 0 )
		{
			this->markBodyForDestruction ( pBox );
			pBoxesToRemove->addObject ( pBox );
			this->boxExplosionAt ( pBox->getSprite ( )->getPosition ( ), pBox->getSprite ( )->getRotation ( ) );
			m_pGameNode->removeChild ( pBox->getSprite ( ), KD_FALSE );
		}
	}
	m_pBoxes->removeObjectsInArray ( pBoxesToRemove );
}

KDvoid Ch4_Bullets::handleCollisionWithMisc ( GameMisc* pMiscA, GameMisc* pMiscB )
{
	// If a bullet touches something we set life to 0 and process the impact on that object
	if ( pMiscA->getTypeTag ( ) == TYPE_OBJ_BULLET && pMiscB->getTypeTag ( ) == TYPE_OBJ_BOX && pMiscA->getLife ( ) > 0 )
	{
		pMiscA->setLife ( 0 );
		this->bulletImpactAt ( pMiscA->getSprite ( )->getPosition ( ), pMiscB );
		m_pMessage->setString ( "Bullet hit" );
	}
	else if ( pMiscA->getTypeTag ( ) == TYPE_OBJ_BOX && pMiscB->getTypeTag ( ) == TYPE_OBJ_BULLET && pMiscB->getLife ( ) > 0 )
	{
		pMiscB->setLife ( 0 );
		this->bulletImpactAt ( pMiscB->getSprite ( )->getPosition ( ), pMiscA );
		m_pMessage->setString ( "Bullet hit" );
	}
		
	// Reset our message
	this->runAction 
	(
		CCSequence::create
		(
			CCDelayTime::create ( 5.0f ), 
			CCCallFunc::create ( this, callfunc_selector ( Ch4_Bullets::resetMessage ) ), 
			KD_NULL 
		)
	);	
}

/// Fire the gun 
KDvoid Ch4_Bullets::fireGun ( KDvoid )
{
	// Fire 10 bullets per second
	if ( m_fFireCount > 0 )
	{
		return;
	}
	m_fFireCount = 0.2f;

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Animate muzzle flash
	CCAnimation*		pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 0.1f );	

	KDint	nNum = kdRand ( ) % 10 + 10;
	pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "muzzleflash_00%i.png", nNum ) ) );
	m_pMuzzleFlash->runAction 
	(
		CCSequence::create 
		(
			CCFadeIn ::create ( 0.0f ), 
			CCAnimate::create ( pAnimation ),
			CCFadeOut::create ( 0.3f ), 
			KD_NULL 
		)
	);	

	// Fire bullet in the correct direction
	KDfloat		fGunAngle = -m_pGunman->getBody ( )->GetAngle ( ) + KD_PI_F / 2;
	if ( m_nGunmanDirection == LEFT )
	{
		fGunAngle += KD_PI_F; 
	}

	CCPoint		tBulletVector = ccp ( kdSinf ( fGunAngle), kdCosf ( fGunAngle ) );

	// Create bullet and shell casing
	for ( KDint i = 0; i < 2; i++ )
	{
		GameMisc*	pBullet = GameMisc::create ( );
		pBullet->setGameArea ( this );
		pBullet->setTypeTag ( TYPE_OBJ_BULLET );
		pBullet->setLife ( 2.f );	

		if ( i == 1 )
		{
			pBullet->setTypeTag ( TYPE_OBJ_SHELL );
			pBullet->setLife ( 5.f );
		}

		CCPoint		tGunmanPosition = m_pGunman->getSprite ( )->getPosition ( );
		CCPoint		tBulletPosition ( tGunmanPosition.x + tBulletVector.x * 10, tGunmanPosition.y + tBulletVector.y * 10 );
		if ( i == 1 )
		{
			tBulletPosition = tGunmanPosition;
		}

		b2BodyDef*		pBodyDef = pBullet->getBodyDef ( );
		pBodyDef->type = b2_dynamicBody;
		if ( i == 0 )
		{
			pBodyDef->bullet = KD_TRUE;
		}
		pBodyDef->position.Set ( tBulletPosition.x / PTM_RATIO, tBulletPosition.y / PTM_RATIO );
		
		b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
		pBullet->setBody ( pBody );		
		pBody->SetTransform ( pBody->GetPosition ( ), fGunAngle );
			
		CCPoint			tTextureSize = ccp ( 17, 17 );
		CCPoint			tShapeSize   = ccp (  2,  2 );
		
		CCSprite*		pSprite = CCSprite::create ( "bullet.png" );	
		pSprite->setPosition ( tBulletPosition );
		pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2.25f );
		pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2.25f );
		if ( i == 1 )
		{
			pSprite->setColor ( ccc3 ( 255, 200, 0 ) );
		}
		pBullet->setSprite ( pSprite );
		m_pGameNode->addChild ( pSprite, 1 );

		b2PolygonShape*	pShape = new b2PolygonShape ( );
		pShape->SetAsBox ( tShapeSize.x / PTM_RATIO / 2, tShapeSize.y / PTM_RATIO );
	
		b2FixtureDef*	pFixtureDef = pBullet->getFixtureDef ( );
		pFixtureDef->shape = pShape;
		pFixtureDef->density = 20.0f;
		pFixtureDef->friction = 1.0f;
		pFixtureDef->restitution = 0.0f;
			
		if ( i == 0 )
		{
			pFixtureDef->filter.categoryBits = CB_BULLET;
			pFixtureDef->filter.maskBits = CB_OTHER;
		}
		else
		{
			pFixtureDef->filter.categoryBits = CB_SHELL;
			pFixtureDef->filter.maskBits = CB_OTHER | CB_SHELL;		
		}
										
		pBody->CreateFixture ( pFixtureDef );

		// Add this bullet to our container
		
		m_pBullets->addObject ( pBullet );
		
		if ( i == 0 )
		{
			// Fire the bullet by applying an impulse
			pBody->ApplyLinearImpulse ( b2Vec2 ( tBulletVector.x * 50, tBulletVector.y * 50 ), pBody->GetPosition ( ) );
		}
		else
		{
			// Eject the shell
			KDfloat		fRadians = vectorToRadians ( tBulletVector );
			fRadians += 1.85f * KD_PI_F;

			CCPoint		tShellVector = radiansToVector ( fRadians );
			if ( tShellVector.x > 0 )
			{
				tShellVector.y *= -1; 
			}
			
			pBody->ApplyLinearImpulse ( b2Vec2 ( tShellVector.x, tShellVector.y ), pBody->GetPosition ( ) );
		}		
	}
}

/// Process the bullet impact 
KDvoid Ch4_Bullets::bulletImpactAt ( CCPoint tPoint, GameMisc* pObject )
{
	// Here we use some trigonometry to determine exactly where the bullet impacted on the box.

	CCPoint		tObjPos = pObject->getSprite ( )->getPosition ( );

	KDfloat		fDist  = distanceBetweenPoints ( tPoint, tObjPos );		// Hypotenuse
	KDfloat		fDistX = tObjPos.x - tPoint.x;							// Opposite side
	KDfloat		fDistY = tObjPos.y - tPoint.y;							// Adjacent side

	KDfloat		fAngleX = kdAsinf ( fDistX / fDist );
	KDfloat		fAngleY = kdAcosf ( fDistY / fDist );

	KDfloat		fObjSize = pObject->getSprite ( )->getContentSize ( ).cx / 2 * pObject->getSprite ( )->getScale ( );
		
	KDfloat		fNewDistX = fDistX - kdSinf ( fAngleX ) * fObjSize;
	KDfloat		fNewDistY = fDistY - kdCosf ( fAngleY ) * fObjSize;
	
	tPoint = ccp ( tPoint.x + fNewDistX, tPoint.y + fNewDistY );
	
	// Animate bullet impact
	KDfloat		fDelay = 0.035f;
	KDfloat		fDuration = 8 * fDelay;

	GameMisc*	pBlastmark = GameMisc::create ( );

	CCSprite*	pSprite = CCSprite::createWithSpriteFrameName ( "blastmark_0000.png" );
	pBlastmark->setLife ( fDuration );
	pSprite->setPosition ( tPoint );
	pSprite->setScale ( 0.2f );
	pSprite->setOpacity ( 100 );
	
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	CCAnimation*			pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( fDelay );

	for ( KDint i = 0; i < 8; i++ )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "blastmark_000%d.png", i ) ) );
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
	
	m_pGameNode->addChild ( pSprite, 5 );
	m_pExplosions->addObject ( pBlastmark );
	
	// Decrement the box life
	pObject->setLife ( pObject->getLife ( ) - 1.0f );
}

/// Process a box exploding 
KDvoid Ch4_Bullets::boxExplosionAt ( const CCPoint& tPoint, KDfloat fRotate )
{
	KDfloat		fDelay = 0.035f;
	KDfloat		fDuration = 7 * fDelay;
	
	GameMisc*	pBox = GameMisc::create ( );
	CCSprite*	pSprite = CCSprite::createWithSpriteFrameName ( "crate_break_01.png" );
	
	pBox->setLife ( fDuration );
	pSprite->setPosition ( tPoint );
	pSprite->setRotation ( fRotate );
	
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	CCAnimation*			pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( fDelay );

	for ( KDint i = 1; i < 8; i++ ) 
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "crate_break_0%d.png", i ) ) );
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

	m_pGameNode->addChild ( pSprite, 5 );
	m_pExplosions->addObject ( pBox );
}