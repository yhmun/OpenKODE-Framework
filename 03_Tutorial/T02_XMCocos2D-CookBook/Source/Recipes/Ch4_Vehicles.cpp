/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Vehicles.cpp
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
#include "Ch4_Vehicles.h"
#include "Libraries/GameMisc.h"
#include "Libraries/TexturedPolygon.h"

enum 
{
	CB_GROUND	= 1 << 0,
	CB_CAR		= 1 << 2,
	CB_WHEEL	= 1 << 4,
};

KDbool Ch4_Vehicles::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Press and hold to drive car." );	
	
	// Initialization
	m_bPressedLeft  = KD_FALSE;
	m_bPressedRight = KD_FALSE;
			
	// Create level
	this->createLevel ( );

	// Add taxi
	this->addTaxi ( );

	return KD_TRUE;
}

KDvoid Ch4_Vehicles::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	this->ccTouchesMoved ( pTouches, pEvent );
}

KDvoid Ch4_Vehicles::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = this->convertTouchToNodeSpace ( pTouch );
	
	if ( tPoint.x < 240 )
	{
		m_bPressedLeft = KD_TRUE; m_bPressedRight = KD_FALSE;
	}
	else if ( tPoint.x >= 240 )
	{
		m_bPressedRight = KD_TRUE; m_bPressedLeft = KD_FALSE;
	}
}

KDvoid Ch4_Vehicles::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	m_bPressedLeft  = KD_FALSE;
	m_bPressedRight = KD_FALSE;
}

KDvoid Ch4_Vehicles::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	CCPoint		tTaxiPos = m_pTaxi->getSprite ( )->getPosition ( );
	m_pGameNode->setPosition ( ccp ( -tTaxiPos.x + 240, -tTaxiPos.y + 160 ) );

	// Front wheel drive
	
	// We apply some counter-torque to steady the car
	if ( m_bPressedRight )
	{
		m_pWheel2->getBody ( )->ApplyTorque ( -20.0f );
		m_pTaxi->getBody ( )->ApplyTorque ( 5.0f );
	}
	else if ( m_bPressedLeft )
	{
		m_pWheel1->getBody ( )->ApplyTorque ( 20.0f );
		m_pTaxi->getBody ( )->ApplyTorque ( -5.0f );	
	}
}

KDvoid Ch4_Vehicles::createLevel ( KDvoid )
{
	// Create a sine wave road for our car 
	b2BodyDef		tGroundBodyDef;
	tGroundBodyDef.position.Set ( 0, 0 );

	b2Body*			pBody = m_pWorld->CreateBody ( &tGroundBodyDef );
		
	b2EdgeShape		tGroundBox;	
	
	b2FixtureDef	tGroundFixtureDef;
	tGroundFixtureDef.restitution = 0.0f;
	tGroundFixtureDef.friction = 10.0f;						// The road has a lot of friction
	tGroundFixtureDef.filter.categoryBits = CB_GROUND;
	tGroundFixtureDef.filter.maskBits = CB_CAR | CB_WHEEL;

	tGroundBox.Set ( b2Vec2 ( -960 / PTM_RATIO, 0 ), b2Vec2 ( -960 / PTM_RATIO, 200 / PTM_RATIO ) );
	tGroundFixtureDef.shape = &tGroundBox;
	pBody->CreateFixture ( &tGroundFixtureDef );

	tGroundBox.Set ( b2Vec2 ( 960 / PTM_RATIO, 0 ), b2Vec2 ( 960 / PTM_RATIO, 200 / PTM_RATIO ) );
	tGroundFixtureDef.shape = &tGroundBox;
	pBody->CreateFixture ( &tGroundFixtureDef );

	KDfloat32	x1;
	KDfloat32	y1;

	for ( KDint u = -1; u < 2; u++ )
	{
		// Add Edge Shapes
		x1 = -15.0f;
		y1 = 2.0f * kdCosf ( x1 / 10.0f * b2_pi );	
		for ( KDint32 i = 0; i < 60; ++i )
		{
			KDfloat32	x2 = x1 + 0.5f;
			KDfloat32	y2 = 2.0f * kdCosf ( x2 / 10.0f * b2_pi );

			b2EdgeShape		tShape;
			tShape.Set ( b2Vec2 ( x1 + u * 960 / PTM_RATIO, y1 ), b2Vec2 ( x2 + u * 960 / PTM_RATIO, y2 ) );
			pBody->CreateFixture ( &tShape, 0.0f );

			x1 = x2;
			y1 = y2;
		}
		
		// Add corresponding graphics
		CCSprite*	pBG = CCSprite::create ( "road_bg.png" );
		pBG->setPosition ( ccp ( u * 960, 70 ) );
		m_pGameNode->addChild ( pBG, 0 );
	
		CCSprite*	pFG = CCSprite::create ( "road_fg.png" );
		pFG->setPosition ( ccp ( u * 960, 70 ) );
		m_pGameNode->addChild ( pFG, 2 );		
	}
	
	// Add two bricks walls so you can't drive off the course 
	this->addBrickWallSpriteAtPosition ( ccp (  970, 60 ) );
	this->addBrickWallSpriteAtPosition ( ccp ( -970, 60 ) );
}

/// Create a vertical wall at a position 
KDvoid Ch4_Vehicles::addBrickWallSpriteAtPosition ( const CCPoint& tPoint )
{
	KDfloat		fPolygonSize = 4;			
	CCPoint		aVertexArr [ ] = { ccp ( 0, 0 ), ccp ( 0.1f, 0 ), ccp ( 0.1f, 1 ), ccp ( 0, 1 ) };
	KDint32		nNumVerts = 4;

	CCArray*	pVertexArray = CCArray::create ( );
	
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		pVertexArray->addObject 
		(
			CCPointValue::create ( ccp ( aVertexArr [ i ].x * PTM_RATIO * fPolygonSize, aVertexArr [ i ].y * PTM_RATIO * fPolygonSize ) ) 
		);
	}

	ccTexParams			tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	TexturedPolygon*	pSprite = TexturedPolygon::create ( "bricks2.png", pVertexArray );
	pSprite->getTexture ( )->setTexParameters ( &tParams );
	pSprite->setPosition ( tPoint );
	m_pGameNode->addChild ( pSprite, 1 );
}

KDvoid Ch4_Vehicles::addTaxi ( KDvoid )
{
	// NOTE: In b2Settings.h we increased the b2_maxPolygonVertices definition:
	// #define b2_maxPolygonVertices	16

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "taxi.plist" );

	CCPoint			tTaxiPosition = ccp (-960, 80 );
	KDfloat			fTaxiScale = 0.2f;
	
	m_pTaxi = GameMisc::create ( );
	m_pTaxi->setGameArea ( this );

	b2BodyDef*		pBodyDef = m_pTaxi->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tTaxiPosition.x / PTM_RATIO, tTaxiPosition.y / PTM_RATIO );
	
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pTaxi->setBody ( pBody );

	b2FixtureDef*	pFixtureDef = m_pTaxi->getFixtureDef ( );
	pFixtureDef->density = 0.5f;
	pFixtureDef->friction = 0.25f;
	pFixtureDef->restitution = 0.0f;
	pFixtureDef->filter.categoryBits = CB_CAR;
	pFixtureDef->filter.maskBits = CB_GROUND;

	// Polygon
	CCPoint		tPolygonSize = ccp ( 8.875f * fTaxiScale, 4.218f * fTaxiScale );
	KDint32		nNumVerts = 11;
	CCPoint		aVertexArr [ ] = 
	{
		ccp ( 0.95f, 0.08f ), ccp ( 0.98f, 0.18f ), ccp ( 0.94f, 0.38f ), ccp ( 0.81f, 0.67f ), ccp ( 0.66f, 0.83f ), 
		ccp ( 0.53f, 0.95f ), ccp ( 0.38f, 0.94f ), ccp ( 0.24f, 0.8f  ), ccp ( 0.12f, 0.58f ), ccp ( 0.03f, 0.34f ), ccp ( 0.03f, 0.1f )  
	};
	b2Vec2		aVertices [ 11 ];
	
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		aVertices [ i ].Set ( aVertexArr [ i ].x * tPolygonSize.x, aVertexArr [ i ].y * tPolygonSize.y );
	}
			
	CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( "taxi_main.png" );
	pSprite->setPosition ( tTaxiPosition );
	pSprite->setAnchorPoint ( ccp ( 0, 0 ) );
	pSprite->setScale ( fTaxiScale );
	m_pTaxi->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 1 );

	b2PolygonShape*	pShape = new b2PolygonShape ( );
	m_pTaxi->setPolygonShape ( pShape );
	pShape->Set ( aVertices, nNumVerts );

	pFixtureDef->shape = pShape;		
	pBody->CreateFixture ( pFixtureDef );

	// Wheels
	CCPoint		aWheelPosition [ ] = { ccp ( tTaxiPosition.x + 16, tTaxiPosition.y ), ccp ( tTaxiPosition.x + 43, tTaxiPosition.y ) };
	
	for ( KDint i = 0; i < 2; i++ )
	{	
		GameMisc*	pWheel = GameMisc::create ( );
	
		if ( i == 0 )
		{
			m_pWheel1 = pWheel;
		}
		else
		{
			m_pWheel2 = pWheel;
		}
			
		pWheel->setGameArea ( this );
		
		b2BodyDef*		pBodyDef = pWheel->getBodyDef ( );
		pBodyDef->type = b2_dynamicBody;
		pBodyDef->position.Set ( aWheelPosition [ i ].x / PTM_RATIO, aWheelPosition [ i ].y / PTM_RATIO );
		
		b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
		pWheel->setBody ( pBody );		
		pBody->SetTransform ( b2Vec2 ( aWheelPosition [ i ].x / PTM_RATIO, aWheelPosition [ i ].y / PTM_RATIO ), KD_PI_F / 2 );
		
		b2FixtureDef*	pFixtureDef = pWheel->getFixtureDef ( );

		pFixtureDef->filter.categoryBits = CB_WHEEL;
		pFixtureDef->filter.maskBits = CB_GROUND;
		pFixtureDef->density = 10.0f;
		pFixtureDef->friction = 10.0f;
		pFixtureDef->restitution = 0.0f;
		
		CCPoint			tTextureSize = ccp ( 52, 51 );
		CCPoint			tShapeSize   = ccp (  9,  9 );
		
		CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( "taxi_wheel.png" );
		pWheel->setSprite ( pSprite );		
		pSprite->setPosition ( aWheelPosition [ i ] );
		pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
		pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );			
		m_pGameNode->addChild ( pSprite, 1 );		
			
		b2CircleShape*	pShape = new b2CircleShape ( );
		pWheel->setCircleShape ( pShape );
		pShape->m_radius = tShapeSize.x / PTM_RATIO;

		pFixtureDef->shape = pShape;		
		pBody->CreateFixture ( pFixtureDef );	
		pBody->SetAngularDamping ( 1.0f );
			
		// Add Joint to connect wheel to the taxi
		b2RevoluteJointDef	tRjd;
		b2RevoluteJoint*	pJoint;
		
		tRjd.Initialize ( pWheel->getBody ( ), m_pTaxi->getBody ( ), b2Vec2 ( aWheelPosition [ i ].x / PTM_RATIO, aWheelPosition [ i ].y / PTM_RATIO ) );

		pJoint = (b2RevoluteJoint*) m_pWorld->CreateJoint ( &tRjd );		
	}
}