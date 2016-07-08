/* --------------------------------------------------------------------------
 *
 *      File            SideScrollerRecipe.cpp
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
#include "SideScrollerRecipe.h"

SideScrollerRecipe::SideScrollerRecipe ( KDvoid )
{
	m_pBoxes	= KD_NULL;
	m_pButtons	= KD_NULL;
}

SideScrollerRecipe::~SideScrollerRecipe ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pBoxes );
	CC_SAFE_RELEASE ( m_pButtons );
}

KDbool SideScrollerRecipe::init ( KDvoid )
{
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialization
	m_nGunmanDirection	= RIGHT;
	m_fJumpCounter		= -10.0f;
	m_bOnGround			= KD_FALSE;
	m_pBoxes			= new CCArray ( );
	m_pButtons			= new CCArray ( );
	
	// Add level boundaries
	this->addLevelBoundaries ( );

	// Initialize gunman
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "gunman.plist" );
	this->initGunman ( );

	// Initialize DPad
	pCache->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	m_pDPad = DPad::create ( );
	m_pDPad->setPosition ( ccp ( 75, 75 ) );
	m_pDPad->setOpacity ( 100 );
	this->addChild ( m_pDPad, 5 );
	
	// This sets off an animation chain reaction
	this->animateGunman ( );

	return KD_TRUE;
}

/// Process dPad and button touches 
KDvoid SideScrollerRecipe::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pDPad->ccTouchesBegan ( pTouches, pEvent );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesBegan ( pTouches, pEvent );	
	}
}

KDvoid SideScrollerRecipe::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pDPad->ccTouchesMoved ( pTouches, pEvent );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesMoved ( pTouches, pEvent );	
	}
}

KDvoid SideScrollerRecipe::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	m_pDPad->ccTouchesEnded ( pTouches, pEvent );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesEnded ( pTouches, pEvent );	
	}
}

KDvoid SideScrollerRecipe::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	b2Body*		pGunmanBody = m_pGunman->getBody ( );

	// Apply gunman running direction
	KDint		nDirection = m_pDPad->getDirection ( );
	if ( nDirection == LEFT || nDirection == UP_LEFT || nDirection == DOWN_LEFT )
	{
		m_nGunmanDirection = LEFT;
		pGunmanBody->ApplyForce ( b2Vec2 ( -35.0f, 0 ), pGunmanBody->GetPosition ( ) );
		( (CCSprite*) m_pGunman->getSprite ( )->getChildByTag ( 0 ) )->setFlipX ( KD_TRUE );	
	}
	else if ( nDirection == RIGHT || nDirection == UP_RIGHT || nDirection == DOWN_RIGHT )
	{
		m_nGunmanDirection = RIGHT;
		pGunmanBody->ApplyForce ( b2Vec2 ( 35.0f, 0 ), pGunmanBody->GetPosition ( ) );
		( (CCSprite*) m_pGunman->getSprite ( )->getChildByTag ( 0 ) )->setFlipX ( KD_FALSE );
	}

	// Decrement jump counter
	m_fJumpCounter -= fDelta;	

	// Did the gunman just hit the ground?
	if ( !m_bOnGround )
	{
		if ( ( pGunmanBody->GetLinearVelocity ( ).y - m_fLastYVelocity ) > 2 && m_fLastYVelocity < -2 )
		{
			pGunmanBody->SetLinearVelocity ( b2Vec2 ( pGunmanBody->GetLinearVelocity ( ).x, 0 ) );
			m_bOnGround = KD_TRUE;
		}
		else if ( pGunmanBody->GetLinearVelocity().y == 0 && m_fLastYVelocity == 0 )
		{
			pGunmanBody->SetLinearVelocity ( b2Vec2 ( pGunmanBody->GetLinearVelocity ( ).x, 0 ) );
			m_bOnGround = KD_TRUE;
		}
	}	
	
	// Did he just fall off the ground without jumping?
	if ( m_bOnGround )
	{
		if ( pGunmanBody->GetLinearVelocity ( ).y < -2.0f && m_fLastYVelocity < -2.0f && ( pGunmanBody->GetLinearVelocity ( ).y < m_fLastYVelocity ) )
		{
			m_bOnGround = KD_FALSE;
		}
	}

	// Store last velocity
	m_fLastYVelocity = pGunmanBody->GetLinearVelocity ( ).y;
	// Keep him upright on the ground
	if ( m_bOnGround )
	{
		pGunmanBody->SetTransform ( pGunmanBody->GetPosition ( ), 0 );
	}

	// Animate gunman if his speed changed significantly
	KDfloat		fSpeed = pGunmanBody->GetLinearVelocity ( ).x;
	if ( fSpeed < 0 )
	{
		fSpeed *= -1; 
	}

	if ( fSpeed > m_fLastXSpeed * 2 )
	{
		m_pGunman->getSprite ( )->getChildByTag ( 0 )->stopAllActions ( );		
		this->animateGunman ( );
	}
	
	// Keep the gunman in the level
	b2Vec2		tGunmanPos = pGunmanBody->GetPosition ( );
	if ( tGunmanPos.x > 530 / PTM_RATIO || tGunmanPos.x < ( -50 / PTM_RATIO ) || tGunmanPos.y < -100 / PTM_RATIO )
	{
		pGunmanBody->SetTransform ( b2Vec2 ( 2, 10 ), pGunmanBody->GetAngle ( ) );
	}

}

/// Collision handling 
KDvoid SideScrollerRecipe::handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
{
	KDint	nTypeA = pObjectA->getType ( );
	KDint	nTypeB = pObjectB->getType ( );

	// SENSOR to MISC collision
	if ( nTypeA == GO_TYPE_SENSOR && nTypeB == GO_TYPE_MISC )
	{
		this->handleCollisionWithSensor ( (GameSensor*) pObjectA, (GameMisc*) pObjectB );
	}
	else if ( nTypeA == GO_TYPE_MISC && nTypeB == GO_TYPE_SENSOR )
	{
		this->handleCollisionWithSensor ( (GameSensor*) pObjectB, (GameMisc*) pObjectA );
	}
	// MISC to MISC collision
	else if ( nTypeA == GO_TYPE_MISC && nTypeB == GO_TYPE_MISC )
	{
		this->handleCollisionWithMisc ( (GameMisc*) pObjectA, (GameMisc*) pObjectB );
	}
}

KDvoid SideScrollerRecipe::handleCollisionWithSensor ( GameSensor* pSensor, GameMisc* pMisc )
{
	// ABSTRACT
}

KDvoid SideScrollerRecipe::handleCollisionWithMisc ( GameMisc* pMiscA, GameMisc* pMiscB )
{
	// ABSTRACT
}

/// Initialize gunman 
KDvoid SideScrollerRecipe::initGunman ( KDvoid )
{
	m_pGunman = GameMisc::create ( );
	m_pGunman->setGameArea ( this );

	CCPoint			tGunmanPosition = ccp ( 240, 250 );

	b2BodyDef*		pBodyDef = m_pGunman->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tGunmanPosition.x / PTM_RATIO, tGunmanPosition.y / PTM_RATIO );
	pBodyDef->userData = m_pGunman;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pGunman->setBody ( pBody );

	CCPoint			tTextureSize = ccp ( 128, 128 );
	CCPoint			tShapeSize   = ccp (  25,  25 );

	CCSprite*		pSprite = CCSprite::create ( "blank.png" );	
	pSprite->setPosition ( tGunmanPosition );	
	pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2.25f );
	pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2.25f );
	pSprite->addChild ( CCSprite::create ( "blank.png" ), 1, 0 );
	m_pGunman->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 1 );

	b2PolygonShape*	pShape =  new b2PolygonShape ( );
	m_pGunman->setPolygonShape ( pShape );
	pShape->SetAsBox ( tShapeSize.x / PTM_RATIO, tShapeSize.y / PTM_RATIO );	

	b2FixtureDef*	pFixtureDef = m_pGunman->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = 2.0f;
	pFixtureDef->friction = 0.0f;
	pFixtureDef->restitution = 0.0f;
	pFixtureDef->filter.categoryBits = CB_GUNMAN;
	pFixtureDef->filter.maskBits = CB_OTHER;
	pBody->CreateFixture ( pFixtureDef );
	pBody->SetLinearDamping ( 2.f );
}

/// Animate the gunman 
KDvoid SideScrollerRecipe::animateGunman ( KDvoid )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	
	m_fLastXSpeed = m_pGunman->getBody ( )->GetLinearVelocity ( ).x;
	if ( m_fLastXSpeed == 0 )
	{
		m_fLastXSpeed = 0.0001f; 
	}

	if ( m_fLastXSpeed < 0 )
	{
		m_fLastXSpeed *= -1; 
	}

	// Animation delay is inverse speed
	KDfloat		fDelay = 0.5f / m_fLastXSpeed;
	if ( fDelay > 0.5f )
	{
		fDelay = 0.5f; 
	}

	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( fDelay );
	
	const KDchar*	szDirection;
	KDbool			bFlipX  = KD_FALSE;
	KDbool			bMoving = KD_TRUE;

	if ( m_fLastXSpeed < 0.2f )
	{
		bMoving = KD_FALSE; 
	}

	switch ( m_nGunmanDirection )
	{
		case LEFT	:	szDirection = "right";	bFlipX = KD_TRUE;	break;
		case RIGHT	:	szDirection = "right";						break;
	}

	// Our simple running loop
	if ( bMoving )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_run_%s_01.png", szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_run_%s_02.png", szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
	}
	else
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
	}

	// animateGunman calls itself indefinitely
	m_pGunman->getSprite ( )->getChildByTag ( 0 )->runAction 
	(
		CCSequence::create 
		(
			CCAnimate::create ( pAnimation ),
			CCCallFunc::create ( this, callfunc_selector ( SideScrollerRecipe::animateGunman ) ),
			KD_NULL
		) 
	);
}

/// Create a button with a position 
KDvoid SideScrollerRecipe::createButtonWithPosition ( const CCPoint& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	GameButton*		pButton = GameButton::create ( );
	pButton->setPosition ( tPosition );
	pButton->setOpacity ( 100 );
	pButton->setUpSpriteFrame ( szUpFrame );
	pButton->setDownSpriteFrame ( szDownFrame );
	pButton->setDisplayFrame ( pCache->spriteFrameByName ( pButton->getUpSpriteFrame ( ) ) );
	pButton->setName ( szName );
	this->addChild ( pButton, 5 );

	m_pButtons->addObject ( pButton );
}

/// Process jump 
KDvoid SideScrollerRecipe::processJump ( KDvoid )
{
	b2Body*		pBody = m_pGunman->getBody ( );
	if ( m_bOnGround && m_fJumpCounter < 0 )
	{	
		// Start a jump. Starting requires you to not be moving on the Y.
		m_fJumpCounter = 0.4f;
		pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 20.0f ), pBody->GetPosition ( ) );
		m_bOnGround = KD_FALSE;
	}
	else if ( m_fJumpCounter > 0 )
	{	
		// Continue a jump
		pBody->ApplyForce ( b2Vec2 ( 0, 65.0f ), pBody->GetPosition ( ) );
	}
}

/// Add a box at a position 
KDvoid SideScrollerRecipe::addBoxWithPosition ( const CCPoint& tPoint, const KDchar* szFile, KDfloat fDensity )
{	
	KDfloat			fTextureSize = 64.0f;
	KDfloat			fShapeSize = 20.0f;

	// Initialization
	GameMisc*		pBox = GameMisc::create ( );
	pBox->setTypeTag ( TYPE_OBJ_BOX );
	pBox->setGameArea ( this );
	pBox->setLife ( 5.f );

	// Physical body
	b2BodyDef*		pBodyDef = pBox->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pBox;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pBox->setBody ( pBody );

	b2PolygonShape*		pShape =  new b2PolygonShape ( );
	pBox->setPolygonShape ( pShape );
	pShape->SetAsBox ( fShapeSize / PTM_RATIO, fShapeSize / PTM_RATIO );	

	b2FixtureDef*	pFixtureDef = pBox->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = fDensity;
	pFixtureDef->friction = 0.5f;
	pFixtureDef->restitution = 0.0f;
	pFixtureDef->filter.categoryBits = CB_OTHER;
	pFixtureDef->filter.maskBits = CB_GUNMAN | CB_BULLET | CB_OTHER | CB_SHELL;
	pBody->CreateFixture ( pFixtureDef );

	// Sprite
	CCSprite*		pSprite = CCSprite::create ( szFile );
	pBox->setSprite ( pSprite );
	pSprite->setPosition ( tPoint );	
	pSprite->setScale ( fShapeSize / fTextureSize * 2 );		
	m_pGameNode->addChild ( pSprite, 2 );

	m_pBoxes->addObject ( pBox );	
}