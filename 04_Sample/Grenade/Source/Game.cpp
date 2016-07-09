/* --------------------------------------------------------------------------
 *
 *      File            Game.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2011 Robert Blackwood 01/20/11
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
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
#include "Game.h"
#include "Enemy.h"

#define SLING_POSITION			ccp ( 60, 157 )
#define SLING_BOMB_POSITION		ccpAdd ( SLING_POSITION, ccp ( 0, 9 ) )
#define SLING_MAX_ANGLE			245
#define SLING_MIN_ANGLE			110
#define SLING_TOUCH_RADIUS		25
#define SLING_LAUNCH_RADIUS		25
#define SERIALIZED_FILE			"GrenadeGameT1.xml"

KDbool Game::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	KDbool  bLoadIt = KD_TRUE;

	m_pBombs = new CCArray ( );
	m_pCurBomb = KD_NULL;
	m_nEnemiesLeft = 0;

	m_pSpaceManager = new CCSpaceManager ( );
	m_pSpaceManager->init ( );
	m_pSpaceManager->setConstantDelta ( 1 / 55.0f );

	m_pSpaceManager->addCollisionCallbackBetweenType ( kEnemyCollisionType, kGroundCollisionType, this, space_selector ( Game::handleEnemyCollision ), COLLISION_POSTSOLVE, KD_NULL );
	m_pSpaceManager->addCollisionCallbackBetweenType ( kEnemyCollisionType, kBlockCollisionType , this, space_selector ( Game::handleEnemyCollision ), COLLISION_POSTSOLVE, KD_NULL );
	m_pSpaceManager->addCollisionCallbackBetweenType ( kEnemyCollisionType, kBombCollisionType  , this, space_selector ( Game::handleEnemyCollision ), COLLISION_POSTSOLVE, KD_NULL );

	m_pSpaceManager->addCollisionCallbackBetweenType ( kBombCollisionType, kGroundCollisionType, this, space_selector ( Game::handleBombCollision ), COLLISION_POSTSOLVE, KD_NULL );
	m_pSpaceManager->addCollisionCallbackBetweenType ( kBombCollisionType, kBlockCollisionType , this, space_selector ( Game::handleBombCollision ), COLLISION_POSTSOLVE, KD_NULL );

	this->setupBackground ( );
	this->setupRestart    ( );

	if ( bLoadIt )
	{
		this->setupBombs   ( );
		this->setupEnemies ( );
		this->setupShapes  ( );
	}

	this->setupNextBomb ( );

	// start the manager!
	m_pSpaceManager->start ( );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

Game::~Game ( KDvoid )
{
	this->removeAllChildrenWithCleanup ( KD_TRUE );

	m_pBombs->release ( );
	m_pSpaceManager->release ( );
}

KDvoid Game::onExit ( KDvoid )
{
	m_pSpaceManager->stop ( );
	CCLayer::onExit ( );
}

KDvoid Game::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_FALSE );
}

KDbool Game::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint  tPoint = this->convertTouchToNodeSpace ( pTouch );	
	KDfloat  fRadiusSQ = SLING_TOUCH_RADIUS * SLING_TOUCH_RADIUS;
	
	// Get the vector of the touch
	CCPoint  tVector = ccpSub ( SLING_POSITION, tPoint );
	
	// Are we close enough to the slingshot?
	return ( ccpLengthSQ ( tVector ) < fRadiusSQ ) ? KD_TRUE : KD_FALSE;
}

KDvoid Game::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint  tPoint = this->convertTouchToNodeSpace ( pTouch );	

	// Get the vector, angle, length, and normal vector of the touch
	CCPoint  tVector       = ccpSub ( tPoint, SLING_BOMB_POSITION );
	CCPoint  tNormalVector = ccpNormalize ( tVector );
	KDfloat  fAngleRads    = ccpToAngle ( tNormalVector );
	KDint    nAngleDegs    = (KDint) CC_RADIANS_TO_DEGREES ( fAngleRads ) % 360;
	KDfloat  fLength       = ccpLength ( tVector );
	
	// Correct the Angle; we want a positive one
	while ( nAngleDegs < 0 )
	{
		nAngleDegs += 360;
	}
	
	// Limit the length
	if ( fLength > SLING_LAUNCH_RADIUS )
	{
		 fLength = SLING_LAUNCH_RADIUS;
	}
	
	// Limit the angle
	if ( nAngleDegs > SLING_MAX_ANGLE )
	{
		tNormalVector = ccpForAngle ( CC_DEGREES_TO_RADIANS ( SLING_MAX_ANGLE ) );
	}
	else if ( nAngleDegs < SLING_MIN_ANGLE)
	{
		tNormalVector = ccpForAngle ( CC_DEGREES_TO_RADIANS ( SLING_MIN_ANGLE ) );
	}
	
	// Set the position
	m_pCurBomb->setPosition ( ccpAdd ( SLING_BOMB_POSITION, ccpMult ( tNormalVector, fLength ) ) );
}

KDvoid Game::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint  tVector = ccpSub ( SLING_BOMB_POSITION, m_pCurBomb->getPosition ( ) );

	if ( m_pCurBomb )
	{
		cpShape*  pShape = m_pSpaceManager->morphShapeToActive ( m_pCurBomb->getShape ( ), 30 );;		
		m_pCurBomb->setShape ( pShape );
	}
	
	m_pCurBomb->applyImpulse ( cpvmult ( cpv ( tVector ), 240 ) );

	this->setupNextBomb ( );
}

KDvoid Game::setupShapes ( KDvoid )
{
	// add four walls to our screen
	m_pSpaceManager->addWindowContainmentWithFriction ( 1.0f, 1.0f, cpvzero );
	
	cpShape*  pGround;

	pGround = m_pSpaceManager->addSegmentAtWorldAnchor ( cpv ( 72,  13 ), cpv ( 480,  13 ), STATIC_MASS, 1 );
	pGround->collision_type = kGroundCollisionType;

	pGround = m_pSpaceManager->addSegmentAtWorldAnchor ( cpv ( 72,  13 ), cpv (  72, 133 ), STATIC_MASS, 1 );
	pGround->collision_type = kGroundCollisionType;

	pGround = m_pSpaceManager->addSegmentAtWorldAnchor ( cpv ( 72, 133 ), cpv (   0, 133 ), STATIC_MASS, 1 );
	pGround->collision_type = kGroundCollisionType;	
	
	this->createTriPillarsAt ( cpv ( 300,  13 ), 80, 20 );
	this->createTriPillarsAt ( cpv ( 320,  33 ), 40, 20 );
	this->createTriPillarsAt ( cpv ( 330,  53 ), 20, 70 );
	
	this->createTriangleAt   ( cpv ( 340, 135 ), 20, 40 );
	
	this->createTriPillarsAt ( cpv ( 400,  13 ), 72, 40 );
	this->createTriPillarsAt ( cpv ( 400,  53 ), 72, 20 );
	this->createTriPillarsAt ( cpv ( 400,  73 ), 72, 20 );
}

KDvoid Game::setupEnemies ( KDvoid )
{
	Enemy*  pEnemy = Enemy::create ( this );
	pEnemy->setPosition ( ccp ( 250, 23 ) );
	this->addChild ( pEnemy, 5 );
	m_nEnemiesLeft++;
	
	pEnemy = Enemy::create ( this );
	pEnemy->setPosition ( ccp ( 415, 23 ) );
	this->addChild ( pEnemy, 5 );
	m_nEnemiesLeft++;

	pEnemy = Enemy::create ( this );
	pEnemy->setPosition ( ccp ( 455, 23 ) );
	this->addChild ( pEnemy, 5 );
	m_nEnemiesLeft++;
	
	pEnemy = Enemy::create ( this );
	pEnemy->setPosition ( ccp ( 415, 100 ) );
	this->addChild ( pEnemy, 5 );
	m_nEnemiesLeft++;
	
	pEnemy = Enemy::create ( this );
	pEnemy->setPosition ( ccp ( 455, 100 ) );
	this->addChild ( pEnemy, 5 );
	m_nEnemiesLeft++;
}

KDvoid Game::setupBackground ( KDvoid )
{
	CCSprite*  pBackground = CCSprite::create ( "Images/smgrback.png" );
	pBackground->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pBackground, 0 );
	
	CCSprite*  pSling1 = CCSprite::create ( "Images/sling1.png" );
	CCSprite*  pSling2 = CCSprite::create ( "Images/sling2.png" );
	
	pSling1->setPosition ( SLING_POSITION );
	pSling2->setPosition ( ccpAdd ( SLING_POSITION, ccp ( 5, 10 ) ) );
	
	this->addChild ( pSling1, 10 );
	this->addChild ( pSling2,  1 );	
}

KDvoid Game::setupBombs	( KDvoid )
{
	for ( KDint i = 0; i < 3; i++ )
	{
		Bomb*  pBomb = Bomb::create ( this );
		pBomb->setPosition ( ccp ( 10 + i * 16, 143 ) );
		this->addChild ( pBomb, 5 );
		m_pBombs->addObject ( pBomb );
	}
}

KDvoid Game::setupNextBomb ( KDvoid )
{
	if ( m_pBombs->count ( ) )
	{
		m_pCurBomb = (Bomb*) m_pBombs->lastObject ( );

		m_pCurBomb->runAction ( CCMoveTo::create ( 0.7f, SLING_BOMB_POSITION ) );
		m_pBombs->removeLastObject ( );
	}
	else
	{
		m_pCurBomb = KD_NULL;
	}
}

KDvoid Game::setupRestart ( KDvoid )
{
	CCMenuItem*  pRestart = CCMenuItemImage::create 
	(
		"Images/restart.png", "Images/restartsel.png", this, menu_selector ( Game::restart ) 
	);

	pRestart->setPosition ( ccp ( 20, 20 ) );

	this->addChild ( CCMenu::createWithItem ( pRestart ), 100 ); 
}

KDvoid Game::restart ( CCObject* pSender )
{
	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( Game::create ( ) );

	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( .7f, pScene ) );
}

KDvoid Game::createTriangleAt ( cpVect tPos, cpFloat fSize , cpFloat fMass )
{
	cpVect  tD1 = cpvforangle ( CC_DEGREES_TO_RADIANS ( 330 ) );
	cpVect  tD2 = cpvforangle ( CC_DEGREES_TO_RADIANS ( 210 ) );
	cpVect  tD3 = cpvforangle ( CC_DEGREES_TO_RADIANS (  90 ) );
	
	tD1 = cpvmult ( tD1, fSize );
	tD2 = cpvmult ( tD2, fSize );
	tD3 = cpvmult ( tD3, fSize );
	
	cpShape*   pShape = m_pSpaceManager->addPolyAt ( tPos, fMass, 0, 3, tD1, tD2, tD3, KD_NULL );
	pShape->collision_type = kBlockCollisionType;
	
	cpShapeNode*  pNode = cpShapeNode::create ( pShape );
	pNode->setColor ( ccc3 ( 100, 20, 40 ) );
	
	this->addChild ( pNode );
}

KDvoid Game::createBlockAt ( cpVect tPos, cpFloat fWidth, cpFloat fHeight, cpFloat fMass )
{
	cpShape*  pShape = m_pSpaceManager->addRectAt ( tPos, fMass, fWidth, fHeight, 0 );
	pShape->collision_type = kBlockCollisionType;
	
	cpShapeNode*  pNode = cpShapeNode::create ( pShape );
	pNode->setColor ( ccc3 ( 56 + kdRand ( ) % 200, 56 + kdRand ( ) % 200, 56 + kdRand ( ) % 200 ) );
	
	this->addChild ( pNode, 5 );
}

KDvoid Game::createTriPillarsAt ( cpVect tPos, cpFloat fWidth, cpFloat fHeight )
{
	const cpFloat  W2 = fWidth / 2;
	const cpFloat  H2 = 8;
	const cpFloat  W1 = 8;
	const cpFloat  H1 = fHeight - H2;
	const cpFloat   M = 100;

	// pillars 1
	this->createBlockAt ( cpvadd ( tPos, cpv ( 0     , H1 / 2 ) ), W1, H1, M );
	this->createBlockAt ( cpvadd ( tPos, cpv ( W2    , H1 / 2 ) ), W1, H1, M );
	this->createBlockAt ( cpvadd ( tPos, cpv ( W2 * 2, H1 / 2 ) ), W1, H1, M );
	
	// floor 1
	this->createBlockAt ( cpvadd ( tPos, cpv (      W2 / 2 - W1 / 4, H1 + H2 / 2 ) ), W2 + W1 / 2, H2, M );
	this->createBlockAt ( cpvadd ( tPos, cpv ( W2 + W2 / 2 + W1 / 4, H1 + H2 / 2 ) ), W2 + W1 / 2, H2, M );
}

KDbool Game::handleEnemyCollision ( CollisionMoment eMoment, cpArbiter* pArbiter, cpSpace* pSpace )
{
	CP_ARBITER_GET_SHAPES ( pArbiter, pEnemyShape, pOtherShape );

	// Get a value for "force" generated by collision
	KDfloat  fForce = cpvdistsq ( pEnemyShape->body->v, pOtherShape->body->v );
	
	if ( fForce > 600 )
	{
		Enemy*  pEnemy = (Enemy*) pEnemyShape->data;
		if ( pEnemy )
		{
			pEnemy->addDamage ( (KDint) ( fForce / 600 ) );
		}
	}

	return KD_TRUE;
}

KDbool Game::handleBombCollision ( CollisionMoment eMoment, cpArbiter* pArbiter, cpSpace* pSpace )
{
	CP_ARBITER_GET_SHAPES ( pArbiter, pBombShape, pOtherShape );
	
	Bomb*  pBomb = (Bomb*) pBombShape->data;
	if ( pBomb )
	{
		pBomb->startCountDown ( );
	}
	
	return KD_TRUE;
}

KDvoid Game::enemyKilled ( KDvoid )
{
	m_nEnemiesLeft--;
	if ( m_nEnemiesLeft == 0 )
	{
		CCLabelTTF*  pLabel = CCLabelTTF:: create ( "You Win!", "Fonts/Marker Felt.ttf", 36 );
		pLabel->setPosition ( ccp ( 480,360 ) );
		this->addChild ( pLabel, 100 );
	}
}