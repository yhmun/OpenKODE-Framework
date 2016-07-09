/* --------------------------------------------------------------------------
 *
 *      File            Ch4_BasicSetup.cpp
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
#include "Ch4_BasicSetup.h"

// 32 pixels = 1 Box2D unit of measure
#define PTM_RATIO		32

KDbool Ch4_BasicSetup::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	/// Box2D Initialization 
	
	// Set gravity
	b2Vec2		tGravity;
	tGravity.Set ( 0.0f, -10.0f );
	
	// Initialize world
	KDbool		bDoSleep = KD_TRUE;
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( bDoSleep );
	m_pWorld->SetContinuousPhysics ( KD_TRUE );
	
	// Initialize debug drawing
	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	KDuint32	uFlags = 0;
	uFlags += b2Draw::e_shapeBit;
	m_pDebugDraw->SetFlags ( uFlags );	
	
	// Create level boundaries
	this->addLevelBoundaries ( );
		
	// Add batch node for block creation
	CCSpriteBatchNode*		pBatch = CCSpriteBatchNode::create ( "blocks.png", 150 );
	this->addChild ( pBatch, 0, 0 );

	// Add a new block
	const CCSize&		tLyrSize = this->getContentSize ( );
	this->addNewSpriteWithCoords ( ccpMid ( tLyrSize ) );

	// Schedule step method
	this->schedule ( schedule_selector ( Ch4_BasicSetup::step ) );
	
	return KD_TRUE;
}

KDvoid Ch4_BasicSetup::onExit ( KDvoid )
{
	delete m_pWorld;
	delete m_pDebugDraw;

	Recipe::onExit ( );
}

KDvoid Ch4_BasicSetup::draw ( KDvoid )
{
	m_pWorld->DrawDebugData ( );
}

KDvoid Ch4_BasicSetup::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );
		CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );
	
		this->addNewSpriteWithCoords ( tLocation );
	}
}

/// Update graphical positions using physical positions 
KDvoid Ch4_BasicSetup::step ( KDfloat fDelta )
{
	KDint32			nVelocityIterations = 8;
	KDint32			nPositionIterations = 3;
	
	m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
	
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		if ( pBody->GetUserData ( ) != KD_NULL )
		{
			CCNode*		pNode = (CCNode*) pBody->GetUserData ( );

			pNode->setPosition ( ccp ( pBody->GetPosition ( ).x * PTM_RATIO, pBody->GetPosition ( ).y * PTM_RATIO ) );
			pNode->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );
		}	
	}
}

/// Adds a polygonal box around the view
KDvoid Ch4_BasicSetup::addLevelBoundaries ( KDvoid )
{
	const CCSize&		tLyrSize = this->getContentSize ( );
	
	b2BodyDef			tGroundBodyDef;
	tGroundBodyDef.position.Set ( 0, 0 );

	b2Body*				pBody = m_pWorld->CreateBody ( &tGroundBodyDef );

	b2EdgeShape			tGroundBox;		
		
	tGroundBox.Set ( b2Vec2 ( 0, 0 ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pBody->CreateFixture ( &tGroundBox, 0 );
		
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ) );
	pBody->CreateFixture ( &tGroundBox, 0 );
		
	tGroundBox.Set ( b2Vec2 ( 0, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( 0, 0 ) );
	pBody->CreateFixture ( &tGroundBox, 0 );
		
	tGroundBox.Set (b2Vec2 ( tLyrSize.cx / PTM_RATIO, tLyrSize.cy / PTM_RATIO ), b2Vec2 ( tLyrSize.cx / PTM_RATIO, 0 ) );
	pBody->CreateFixture ( &tGroundBox, 0 );
}

KDvoid Ch4_BasicSetup::addNewSpriteWithCoords ( const CCPoint& tPoint )
{
	CCSpriteBatchNode*	pBatch = (CCSpriteBatchNode*) this->getChildByTag ( 0 );
		
	KDint			nX = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );
	KDint			nY = ( CCRANDOM_0_1 ( ) > 0.5f ? 0 : 1 );
	CCSprite*		pSprite = CCSprite::createWithSpriteBatchNode (  pBatch, ccr ( 32 * nX, 32 * nY, 32, 32 ) );
	pBatch->addChild ( pSprite );
	
	pSprite->setPosition ( tPoint );
	
	b2BodyDef		tBodyDef;
	tBodyDef.type = b2_dynamicBody;

	tBodyDef.position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	tBodyDef.userData = pSprite;

	b2Body*			pBody = m_pWorld->CreateBody ( &tBodyDef );
	
	// Define another box shape for our dynamic body.
	b2PolygonShape	tDynamicBox;
	tDynamicBox.SetAsBox ( 0.5f, 0.5f );		// These are mid points for our 1m box
	
	// Define the dynamic body fixture.
	b2FixtureDef	tFixtureDef;
	tFixtureDef.shape = &tDynamicBox;	
	tFixtureDef.density = 1.0f;
	tFixtureDef.friction = 0.3f;
	pBody->CreateFixture ( &tFixtureDef );
}
