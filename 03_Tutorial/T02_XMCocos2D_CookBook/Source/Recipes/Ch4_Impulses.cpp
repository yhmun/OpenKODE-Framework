/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Impulses.cpp
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
#include "Ch4_Impulses.h"
#include "Libraries/GameMisc.h"
#include "Libraries/GameSensor.h"

KDbool Ch4_Impulses::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Shoot the ball in the hoop." );

	// Create level boundaries
	this->addLevelBoundaries ( );
	
	// Add level background
	CCSprite*	pBG = CCSprite::create ( "bball_bg.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	m_pGameNode->addChild ( pBG, 0 );
	
	// Add basketball
	this->addBasketball ( );
	
	// Add basketball net
	this->addBasketballNet ( );
	
	// Add shooter
	this->addShooter ( );

	return KD_TRUE;
}

KDvoid Ch4_Impulses::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = this->convertTouchToNodeSpace ( pTouch );

	// Apply an impulse when the user touches the screen 
	b2Vec2		tBodyPos = m_pBasketBall->getBody ( )->GetPosition ( );
	b2Vec2		tVect = b2Vec2 ( tPoint.x - tBodyPos.x * PTM_RATIO, tPoint.y - tBodyPos.y * PTM_RATIO );

	m_pBasketBall->getBody ( )->ApplyLinearImpulse ( b2Vec2 ( tVect.x / 20, tVect.y / 20 ) , tBodyPos );
}

/// Add level boundaries which are higher than normal to allow the shooter to arc the ball 
KDvoid Ch4_Impulses::addLevelBoundaries ( KDvoid )
{
	const CCSize&		tLyrSize = this->getContentSize ( );
	
	b2BodyDef			tGroundBodyDef;
	tGroundBodyDef.position.Set ( 0, 0 );

	b2Body*				pBody = m_pWorld->CreateBody ( &tGroundBodyDef );

	b2EdgeShape			tGroundBox;		
		
	b2FixtureDef		tGroundFixtureDef;
	tGroundFixtureDef.density = 1.0f;
	tGroundFixtureDef.friction = 1.0f;
	tGroundFixtureDef.restitution = 0.0f;

	tGroundBox.Set ( b2Vec2 ( 0, 1 ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 1 ) );
	tGroundFixtureDef.shape = &tGroundBox;
	pBody->CreateFixture ( &tGroundFixtureDef );
		
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO * 1.5f ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO * 1.5f ) );
	tGroundFixtureDef.shape = &tGroundBox;
	pBody->CreateFixture ( &tGroundFixtureDef );
		
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO * 1.5f ), b2Vec2 ( 0, 1 ) );
	tGroundFixtureDef.shape = &tGroundBox;
	pBody->CreateFixture ( &tGroundFixtureDef );
		
	tGroundBox.Set (b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO * 1.5f ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 1 ) );
	tGroundFixtureDef.shape = &tGroundBox;
	pBody->CreateFixture ( &tGroundFixtureDef );
}

/// Main collision handling routine 
KDvoid Ch4_Impulses::handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
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
}

/// SENSOR to MISC collision 
KDvoid Ch4_Impulses::handleCollisionWithSensor ( GameSensor* pSensor, GameMisc* pMisc )
{
	KDint	nSensorType = pSensor->getTypeTag ( );
	KDint	nMiscType   =   pMisc->getTypeTag ( );

	if ( nMiscType == TYPE_OBJ_BASKETBALL && nSensorType == TYPE_OBJ_NET_SENSOR )
	{
		// Animate the net when the shooter makes a basket
		this->runAction
		( 
			CCSequence::create 
			(
				CCDelayTime::create ( 0.5f ), 
				CCCallFunc::create ( this, callfunc_selector ( Ch4_Impulses::resetMessage ) ),
				KD_NULL
			)
		);
			
		CCAnimation*	pAnimation = CCAnimation::create ( );
		pAnimation->setDelayPerUnit ( 0.1f );
		
		for ( KDint i = 1; i <= 4; i++ )
		{
			pAnimation->addSpriteFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "bball_net_front_0%d.png", i ) ) );
		}
		
		for ( KDint i = 3; i >= 1; i-- )
		{
			pAnimation->addSpriteFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "bball_net_front_0%d.png", i ) ) );
		}		

		m_pBasketBallNet->runAction ( CCAnimate::create ( pAnimation ) );
	}
	else if ( nMiscType == TYPE_OBJ_BASKETBALL && nSensorType == TYPE_OBJ_SHOOTER )
	{
		// Animate the shooter's arm and apply an impulse when he touches the ball 
		m_pBasketBallShooterArm->runAction 
		(
			CCSequence::create 
			(
				CCRotateBy::create ( 0.1f, -90 ),
				CCRotateBy::create ( 0.5f,  90 ),
				KD_NULL
			)
		);
	
		b2Body*		pBody = m_pBasketBall->getBody ( );
		pBody->SetLinearVelocity ( b2Vec2 ( 0, 0 ) );
		pBody->ApplyLinearImpulse ( b2Vec2 ( 3.5f, 7 ), pBody->GetPosition ( ) );
	}
}

/// Add a basketball 
KDvoid Ch4_Impulses::addBasketball ( KDvoid )
{
	m_pBasketBall = GameMisc::create ( );
	m_pBasketBall->setTypeTag ( TYPE_OBJ_BASKETBALL );
	m_pBasketBall->setGameArea ( this );

	b2BodyDef*		pBodyDef = m_pBasketBall->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( 100 / PTM_RATIO, 100 / PTM_RATIO );
	pBodyDef->userData = m_pBasketBall;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pBasketBall->setBody ( pBody );

	b2FixtureDef*	pFixtureDef = m_pBasketBall->getFixtureDef ( );
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 0.3f;
	pFixtureDef->restitution = 0.6f;

	// Circle
	KDfloat			fTextureSize = 128;
	KDfloat			fShapeSize = 12;

	CCSprite*		pSprite = CCSprite::create ( "bball.png" );
	pSprite->setPosition ( ccp ( 100, 100 ) );
	pSprite->setScale ( fShapeSize / fTextureSize * 2 );
	m_pBasketBall->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 1 );

	b2CircleShape*		pShape = new b2CircleShape ( );
	m_pBasketBall->setCircleShape ( pShape );
	pShape->m_radius = fShapeSize / PTM_RATIO;

	pFixtureDef->shape = pShape;		
	pBody->CreateFixture ( pFixtureDef );
}

/// Add a basketball net with a sensor 
KDvoid Ch4_Impulses::addBasketballNet ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "bball_net.plist" );

	m_pBasketBallNet = CCSprite::createWithSpriteFrameName ( "bball_net_front_01.png" );
	m_pBasketBallNet->setPosition ( ccp ( 375, 190 ) );
	m_pBasketBallNet->setScale ( 0.25f );
	m_pGameNode->addChild ( m_pBasketBallNet, 2 );

	CCSprite*			pNetBack = CCSprite::createWithSpriteFrameName ( "bball_net_back.png" );
	pNetBack->setPosition ( ccp ( 375, 190 ) );
	pNetBack->setScale ( 0.25f );
	m_pGameNode->addChild ( pNetBack, 1 );

	b2BodyDef			tNetBodyDef;
	tNetBodyDef.position.Set ( 0, 0 );

	b2Body*				pNetBody = m_pWorld->CreateBody ( &tNetBodyDef );

	b2EdgeShape			tNetShape;		
		
	b2FixtureDef		tNetFixtureDef;
	tNetFixtureDef.restitution = -1.0f;

	// Rim front
	tNetShape.Set ( b2Vec2 ( 345.0f / PTM_RATIO, 220.0f / PTM_RATIO ), b2Vec2 ( 355.0f / PTM_RATIO, 190.0f / PTM_RATIO ) );
	tNetFixtureDef.shape = &tNetShape;
	pNetBody->CreateFixture ( &tNetFixtureDef );
		
	// Rim back
	tNetShape.Set ( b2Vec2 ( 390.0f / PTM_RATIO, 190.0f / PTM_RATIO ), b2Vec2 ( 410.0f / PTM_RATIO, 230.0f / PTM_RATIO ) );
	tNetFixtureDef.shape = &tNetShape;
	pNetBody->CreateFixture ( &tNetFixtureDef );
		
	// Backboard
	tNetShape.Set ( b2Vec2 ( 410.0f / PTM_RATIO, 300.0f / PTM_RATIO ), b2Vec2 ( 410.0f / PTM_RATIO, 210.0f / PTM_RATIO ) );
	tNetFixtureDef.shape = &tNetShape;
	pNetBody->CreateFixture ( &tNetFixtureDef );
		
	// Add net sensor
	GameSensor*		pGameObjSensor = GameSensor::create ( );
	pGameObjSensor->setTypeTag ( TYPE_OBJ_NET_SENSOR );
	pGameObjSensor->setGameArea ( this );

	b2BodyDef*		pBodyDef = pGameObjSensor->getBodyDef ( );
	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( 0, 0 );
	pBodyDef->userData = pGameObjSensor;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pGameObjSensor->setBody ( pBody );

	b2PolygonShape*	pShape = new b2PolygonShape ( );
	pGameObjSensor->setPolygonShape ( pShape );
	pShape->SetAsEdge ( b2Vec2 ( 370.0f / PTM_RATIO, 200.0f / PTM_RATIO ), b2Vec2 ( 380.0f / PTM_RATIO, 200.0f / PTM_RATIO ) );		

	b2FixtureDef*	pFixtureDef = pGameObjSensor->getFixtureDef ( );
	pFixtureDef->shape = pShape;		
	pFixtureDef->isSensor = KD_TRUE;
	pBody->CreateFixture ( pFixtureDef );
}

/// Add a shooter with reverse karate chop action! 
KDvoid Ch4_Impulses::addShooter ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "bball_shooter.plist" );

	CCPoint		tTextureSize = ccp ( 128, 512 );
	CCPoint		tShapeSize   = ccp (  24,  96 );

	m_pBasketBallShooterArm = CCSprite::createWithSpriteFrameName ( "bball_shooter_arm.png" );
	m_pBasketBallShooterArm->setPosition ( ccp ( 65, 148 ) );
	m_pBasketBallShooterArm->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
	m_pBasketBallShooterArm->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );
	m_pBasketBallShooterArm->setAnchorPoint ( ccp ( 0.5f, 0.8f ) );
	m_pBasketBallShooterArm->setRotation ( 0 );
	m_pGameNode->addChild ( m_pBasketBallShooterArm, 2 );

	GameSensor*		pShooter = GameSensor::create ( );
	pShooter->setTypeTag ( TYPE_OBJ_SHOOTER );
	pShooter->setGameArea ( this );

	b2BodyDef*		pBodyDef = pShooter->getBodyDef ( );
	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( 70 / PTM_RATIO, 100 / PTM_RATIO );
	pBodyDef->userData = pShooter;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pShooter->setBody ( pBody );

	CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( "bball_shooter_body.png" );

	pSprite->setPosition ( ccp ( 70, 100 ) );
	pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
	pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );
	
	pShooter->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 1 );

	b2PolygonShape*		pShape = new b2PolygonShape ( );
	pShooter->setPolygonShape ( pShape );
	pShape->SetAsBox ( tShapeSize.x / PTM_RATIO, tShapeSize.y / PTM_RATIO );		

	b2FixtureDef*		pFixtureDef = pShooter->getFixtureDef ( );
	pFixtureDef->shape = pShape;		
	pFixtureDef->isSensor = KD_TRUE;
	pBody->CreateFixture ( pFixtureDef );
}

