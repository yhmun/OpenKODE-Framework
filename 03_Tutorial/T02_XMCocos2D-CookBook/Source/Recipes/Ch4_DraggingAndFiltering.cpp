/* --------------------------------------------------------------------------
 *
 *      File            Ch4_DraggingAndFiltering.cpp
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
#include "Ch4_DraggingAndFiltering.h"
#include "Libraries/GameMisc.h"

enum 
{
	// Collision bits for filtering
	CB_GROUND	= 1 << 0,
	CB_FRUIT	= 1 << 2,
	CB_BOWL		= 1 << 4,
};

KDbool Ch4_DraggingAndFiltering::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Pick up the fruit." );
																								
	// Create level boundaries
	this->addLevelBoundaries ( );
			
	// Add fruit bowl
	this->addFruitBasket ( );
	
	// Initialization of any variables
	m_bFruitGrabbed = KD_FALSE;

	return KD_TRUE;
}

KDvoid Ch4_DraggingAndFiltering::onExit ( KDvoid )
{
	m_pFruitObjects->release ( );

	GameArea2D::onExit ( );
}

KDvoid Ch4_DraggingAndFiltering::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	// Grab the nearest fruit 
	b2Vec2		tGrabbedPos = m_pGrabbedFruit->getBody ( )->GetPosition ( );
	KDfloat		fGrabbedDistance = distanceBetweenPoints ( tLocation, ccp ( tGrabbedPos.x * PTM_RATIO, tGrabbedPos.y * PTM_RATIO ) );

	for ( KDuint i = 0; i < m_pFruitObjects->count ( ); i++ )
	{
		GameMisc*	pFruit = (GameMisc*) m_pFruitObjects->objectAtIndex ( i );
		b2Vec2		tFruitPos = pFruit->getBody ( )->GetPosition ( );
		KDfloat		fThisDistance = distanceBetweenPoints ( ccp ( tFruitPos.x * PTM_RATIO, tFruitPos.y * PTM_RATIO ), tLocation );

		if ( fThisDistance < fGrabbedDistance ) 
		{
			m_pGrabbedFruit = pFruit;
			fGrabbedDistance = fThisDistance;
		}
	}
	
	m_bFruitGrabbed = KD_TRUE;
	
	this->ccTouchesMoved ( pTouches, pEvent );	
}

KDvoid Ch4_DraggingAndFiltering::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );

	b2Body*		pBody = m_pGrabbedFruit->getBody ( );
	pBody->SetTransform ( b2Vec2 ( tLocation.x / PTM_RATIO, tLocation.y / PTM_RATIO ), pBody->GetAngle ( ) );

	// Reposition the grabbed fruit 
	CCPoint		tSpritePos = m_pGrabbedFruit->getSprite ( )->getPosition ( );
	b2Vec2		tMoveDistance = b2Vec2 ( ( tLocation.x / PTM_RATIO - tSpritePos.x / PTM_RATIO ), ( tLocation.y / PTM_RATIO - tSpritePos.y / PTM_RATIO ) );
	m_tLastFruitVelocity = b2Vec2 ( tMoveDistance.x * 20, tMoveDistance.y * 20 );
}

KDvoid Ch4_DraggingAndFiltering::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Release the fruit 
	m_bFruitGrabbed = KD_FALSE;
	m_pGrabbedFruit->getBody ( )->SetLinearVelocity ( m_tLastFruitVelocity );
}

KDvoid Ch4_DraggingAndFiltering::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );
	
	// Suspend the fruit in mid-air while it is grabbed 
	if ( m_bFruitGrabbed )
	{
		m_pGrabbedFruit->getBody ( )->SetLinearVelocity ( b2Vec2_zero );
	}
}

KDvoid Ch4_DraggingAndFiltering::addFruit ( const KDchar* szSpriteFrame, const CCPoint& tPoint, const KDchar* szShapeType )
{
	GameMisc*		pFruit = GameMisc::create ( );
	pFruit->setGameArea ( this );

	b2BodyDef*		pBodyDef = pFruit->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pFruit;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pFruit->setBody ( pBody );

	b2FixtureDef*	pFixtureDef = pFruit->getFixtureDef ( );
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 0.3f;
	pFixtureDef->restitution = 0.4f;
	pFixtureDef->filter.categoryBits = CB_FRUIT;
	pFixtureDef->filter.maskBits = CB_GROUND | CB_BOWL;		// Fruit does not collide with other fruit

	CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( szSpriteFrame );
	pFruit->setSprite ( pSprite );
	pSprite->setPosition ( tPoint );

	if ( !kdStrcmp ( szShapeType, "circle" ) )
	{
		// Circle
		KDfloat		fTextureSize = 160;
		KDfloat		fShapeSize	 = 40;

		pSprite->setScale ( fShapeSize / fTextureSize * 2 );
		m_pGameNode->addChild ( pSprite, 2 );
		
		b2CircleShape*		pShape = new b2CircleShape ( );
		pFruit->setCircleShape ( pShape );
		pShape->m_radius = fShapeSize / PTM_RATIO;
		pFixtureDef->shape = pShape;	
	}
	else if ( !kdStrcmp ( szShapeType, "rect" ) )
	{
		// Rectangle
		CCPoint		tTextureSize = ccp ( 300, 100 );
		CCPoint		tShapeSize   = ccp (  60,  20 );
	
		pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
		pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );
		m_pGameNode->addChild ( pSprite, 2 );
	
		b2PolygonShape*		pShape =  new b2PolygonShape ( );
		pFruit->setPolygonShape ( pShape );
		pShape->SetAsBox ( tShapeSize.x / PTM_RATIO, tShapeSize.y / PTM_RATIO );		
		pFixtureDef->shape = pShape;
	}

	pBody->CreateFixture ( pFixtureDef );

	m_pFruitObjects->addObject ( pFruit );
	m_pGrabbedFruit = pFruit;
}

KDvoid Ch4_DraggingAndFiltering::addFruitBasket ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "fruit_bowl.plist" );
	
	CCSprite*		pBowlBack = CCSprite::createWithSpriteFrameName ( "fruit_bowl_back.png" ); 
	pBowlBack->setPosition ( ccp ( 240, 120 ) );
	pBowlBack->setScale ( 0.5f );
	m_pGameNode->addChild ( pBowlBack, 1 );
	
	CCSprite*		pBowlFront = CCSprite::createWithSpriteFrameName ( "fruit_bowl_front.png" );
	pBowlFront->setPosition ( ccp ( 240, 70 ) );
	pBowlFront->setScale ( 0.5f );
	m_pGameNode->addChild ( pBowlFront, 3 );

	// Add physical parts
	b2BodyDef		tBowlBodyDef;
	tBowlBodyDef.position.Set ( 0, 0 );
	tBowlBodyDef.type = b2_staticBody;

	b2Body*			pBody = m_pWorld->CreateBody ( &tBowlBodyDef );
		
	b2EdgeShape		tBowlShape;		

	b2FixtureDef	tBowlFixtureDef;
	tBowlFixtureDef.restitution = 0.5f;
	tBowlFixtureDef.filter.categoryBits = CB_BOWL;
	tBowlFixtureDef.filter.maskBits = CB_FRUIT;
	
	// Rim left
	tBowlShape.Set ( b2Vec2 ( 120.0f / PTM_RATIO, 120.0f / PTM_RATIO ), b2Vec2 ( 180.0f / PTM_RATIO,0.0f / PTM_RATIO ) );
	tBowlFixtureDef.shape = &tBowlShape;
	pBody->CreateFixture ( &tBowlFixtureDef );	
	
	// Rim right
	tBowlShape.Set ( b2Vec2 ( 300.0f / PTM_RATIO, 0.0f / PTM_RATIO ), b2Vec2 ( 360.0f / PTM_RATIO, 120.0f / PTM_RATIO ) );
	tBowlFixtureDef.shape = &tBowlShape;
	pBody->CreateFixture ( &tBowlFixtureDef );
	
	// Bottom
	tBowlShape.Set ( b2Vec2 ( 140.0f / PTM_RATIO, 70.0f / PTM_RATIO ), b2Vec2 ( 340.0f / PTM_RATIO, 70.0f / PTM_RATIO ) );
	tBowlFixtureDef.shape = &tBowlShape;
	pBody->CreateFixture ( &tBowlFixtureDef );
	
	// Add fruit 
	m_pFruitObjects = CCArray::create ( );
	m_pFruitObjects->retain ( );
	
	this->addFruit ( "fruit_banana.png", ccp ( 210, 200 ), "rect"   );
	this->addFruit ( "fruit_apple.png" , ccp ( 230, 200 ), "circle" );
	this->addFruit ( "fruit_grapes.png", ccp ( 250, 200 ), "rect"   );
	this->addFruit ( "fruit_orange.png", ccp ( 270, 200 ), "circle" );
}