/* --------------------------------------------------------------------------
 *
 *      File            BilliardsTable.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/20
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "BilliardsTable.h"
#include "ContactListener.h"
#include "Board.h"
#include "Ball.h"

KDbool BilliardsTable::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	this->initBox2dWorld ( );

	this->enableBox2dDebugDrawing ( );

	// a bright background is desireable for this pinball table
	CCLayerColor*	pColorLayer = CCLayerColor::create ( ccc4 ( 222, 222, 222, 255 ) );
	this->addChild ( pColorLayer, -3 );

	m_pBoard = Board::create ( m_pWorld );
	this->addChild ( m_pBoard, -2, kTagBoard );

	this->setTouchMode ( kCCTouchesOneByOne );
	this->setTouchEnabled ( KD_TRUE );

	this->scheduleUpdate ( );

	return KD_TRUE;
}

KDvoid BilliardsTable::draw ( KDvoid )
{
	m_pWorld->DrawDebugData ( );
}

KDvoid BilliardsTable::update ( KDfloat fDelta )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pBoard->getChildren ( ), pObject )
	{
		Ball*	pBall = dynamic_cast<Ball*> ( pObject );
		if ( pBall && pBall->isInHall ( ) )
		{
			m_pBoard->removeChild ( pBall, KD_TRUE );
			
			if ( pBall->isMainBall ( ) )
			{
				m_pBoard->resetBalls ( );
				break;
			}
		}
	}
    
	// The number of iterations influence the accuracy of the physics simulation. With higher values the
	// body's velocity and position are more accurately tracked but at the cost of speed.
	// Usually for games only 1 position iteration is necessary to achieve good results.
	KDfloat			fTimeStep = 0.03f;
	KDint32			nVelocityIterations = 8;
	KDint32			nPositionIterations = 8;
	
	m_pWorld->Step ( fTimeStep, nVelocityIterations, nPositionIterations );
	
	// for each body, get its assigned BodyNode and update the sprite's position
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		BodyNode*	pBodyNode = (BodyNode*) pBody->GetUserData ( );
		if ( pBodyNode != KD_NULL )
		{
			CCSprite*	pBodySprite = pBodyNode->getSprite ( );

			if ( pBodySprite != KD_NULL )
			{
				// update the sprite's position to where their physics bodies are
				pBodySprite->setPosition ( Helper::toPixels ( pBody->GetPosition ( ) ) );			
				pBodySprite->setRotation ( -CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );
			}
		}	
	}
}

KDbool BilliardsTable::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint		tLocation = Helper::locationFromTouch ( pTouch );
    Ball*		pMainBall = (Ball*) m_pBoard->getChildByTag ( kTagMainBall );
    
    if ( pMainBall->isTouchForMe ( tLocation ) )
	{
        return pMainBall->ccTouchBegan ( pTouch, pEvent );
    }

    return KD_FALSE;
}

KDvoid BilliardsTable::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
    Ball*		pMainBall = (Ball*) m_pBoard->getChildByTag ( kTagMainBall );
    
	return pMainBall->ccTouchMoved ( pTouch, pEvent );
}

KDvoid BilliardsTable::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    Ball*		pMainBall = (Ball*) m_pBoard->getChildByTag ( kTagMainBall );
    
	return pMainBall->ccTouchEnded ( pTouch, pEvent );
}

KDvoid BilliardsTable::initBox2dWorld ( KDvoid )
{
	// Construct a world object, which will hold and simulate the rigid bodies.
	b2Vec2		tGravity ( 0.0f, 0.0f );
	KDbool		bDoSleep = KD_TRUE;
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( bDoSleep );

	m_pContactListener = new ContactListener ( );
	m_pWorld->SetContactListener ( m_pContactListener );
	
	// Define the static container body, which will provide the collisions at screen borders.
	b2BodyDef	tContainerBodyDef;
	b2Body*		pContainerBody = m_pWorld->CreateBody ( &tContainerBodyDef );
	
	// for the ground body we'll need these values
	KDfloat		fWidthInMeters		= m_tContentSize.cx / PTM_RATIO;
	KDfloat		fHeightInMeters		= m_tContentSize.cy / PTM_RATIO;
	b2Vec2		fLowerLeftCorner	= b2Vec2 ( 0, 0 );
	b2Vec2		fLowerRightCorner	= b2Vec2 ( fWidthInMeters, 0 );
	b2Vec2		fUpperLeftCorner	= b2Vec2 ( 0, fHeightInMeters );
	b2Vec2		fUpperRightCorner	= b2Vec2 ( fWidthInMeters, fHeightInMeters );
	
	// Create the screen box' sides by using a polygon assigning each side individually.
	b2PolygonShape		tScreenBoxShape;
	
    b2FixtureDef		tFixtureDef;
    tFixtureDef.shape = &tScreenBoxShape;
    tFixtureDef.density = 1.0f;
    tFixtureDef.restitution = 1.0f;
    tFixtureDef.friction = 1.0f;
    
	// left side
	tScreenBoxShape.SetAsEdge ( fUpperLeftCorner, fLowerLeftCorner );
	pContainerBody->CreateFixture ( &tFixtureDef );
	
	// right side
	tScreenBoxShape.SetAsEdge ( fUpperRightCorner, fLowerRightCorner );
	pContainerBody->CreateFixture ( &tFixtureDef );

	// top
	tScreenBoxShape.SetAsEdge ( fUpperLeftCorner, fUpperRightCorner );
	pContainerBody->CreateFixture ( &tFixtureDef );
	
	// bottom
	tScreenBoxShape.SetAsEdge ( fLowerLeftCorner, fLowerRightCorner );
	pContainerBody->CreateFixture ( &tFixtureDef );
}

KDvoid BilliardsTable::enableBox2dDebugDrawing ( KDvoid )
{
	// Debug Draw functions
	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );
	
	KDuint32	uFlags = 0;
	
//	uFlags |= b2Draw::e_shapeBit;
//	uFlags |= b2Draw::e_jointBit;
//	uFlags |= b2Draw::e_aabbBit;
//	uFlags |= b2Draw::e_pairBit;
//	uFlags |= b2Draw::e_centerOfMassBit;

	m_pDebugDraw->SetFlags ( uFlags );	
}
