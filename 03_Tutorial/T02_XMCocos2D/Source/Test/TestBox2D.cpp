/* --------------------------------------------------------------------------
 *
 *      File            TestBox2D.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestBox2D.h"

#define PTM_RATIO         32

enum 
{
    kTagParentNode		 = 1,
};

KDvoid TestBox2D::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

	b2Vec2  tGravity;
	tGravity.Set ( 0.0f, -10.0f );

	m_pWorld = new b2World ( tGravity );

	// Do we want to let bodies sleep?
	m_pWorld->SetAllowSleeping ( KD_TRUE );

	m_pWorld->SetContinuousPhysics ( KD_TRUE );
	
	// Define the ground body.
	b2BodyDef  tGroundBodyDef;
	tGroundBodyDef.position.Set ( 0, 0 );	// bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape ( also from a pool ).
	// The body is also added to the world.
	b2Body*  pGroundBody = m_pWorld->CreateBody ( &tGroundBodyDef );
	
	// Define the ground box shape.
	b2EdgeShape  tGroundBox;		
	
	// bottom
	tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
	// top
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
	// left
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( 0,0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );
	
	// right
	tGroundBox.Set ( b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pGroundBody->CreateFixture ( &tGroundBox, 0 );

	// Set up sprite
	m_pSpriteTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/blocks.png" );
    CCNode*  pParent = CCNode::create ( );
	this->addChild ( pParent, 0, kTagParentNode );

	this->scheduleUpdate  ( );
	this->setTouchEnabled ( KD_TRUE );
}

KDvoid TestBox2D::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pWorld );

	TestBasic::onExit ( );
}

KDuint TestBox2D::count ( KDvoid )
{
	return 1;
}

const KDchar* TestBox2D::subtitle ( KDvoid )
{
	return "Tap screen";
}

KDvoid TestBox2D::addNewSpriteWithCoords ( const CCPoint& tPosition )
{
	// Define the dynamic body.
	//Set up a 1m squared box in the physics world
	b2BodyDef  tBodyDef;
	tBodyDef.type = b2_dynamicBody;
	tBodyDef.position.Set ( tPosition.x / PTM_RATIO, tPosition.y / PTM_RATIO );
	//tBodyDef.userData = sprite;

	b2Body*  pBody = m_pWorld->CreateBody ( &tBodyDef );
	
	// Define another box shape for our dynamic body.
	b2PolygonShape  tDynamicBox;
	tDynamicBox.SetAsBox ( 0.5f, 0.5f );
	
	// Define the dynamic body fixture.
	b2FixtureDef  tFixtureDef;
	tFixtureDef.shape = &tDynamicBox;	
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.3f;
	pBody->CreateFixture ( &tFixtureDef );

	CCNode*  pParent = this->getChildByTag ( kTagParentNode );

	// We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
	// just randomly picking one of the images
	KDint  nOffsetX = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );
	KDint  nOffsetY = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );

    CCPhysicsSprite*  pSprite = CCPhysicsSprite::createWithTexture ( m_pSpriteTexture, ccr ( 32 * nOffsetX, 32 * nOffsetY, 32, 32 ) );
    pParent->addChild ( pSprite );
    pSprite->setB2Body  ( pBody );
    pSprite->setPTMRatio ( PTM_RATIO );
    pSprite->setPosition ( tPosition );
}

KDvoid TestBox2D::update ( KDfloat fDelta )
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	KDint	nVelocityIterations = 8;
	KDint	nPositionIterations = 1;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
}

KDvoid TestBox2D::ccTouchesEnded ( CCSet* touches, CCEvent* event )
{
	// Add a new body/atlas sprite at the touched location
	for ( CCSetIterator it = touches->begin ( ); it != touches->end ( ); it++ ) 
	{
		CCTouch*  pTouch = (CCTouch*) ( *it );
	
		addNewSpriteWithCoords ( this->convertTouchToNodeSpace ( pTouch ) );
	}
}
