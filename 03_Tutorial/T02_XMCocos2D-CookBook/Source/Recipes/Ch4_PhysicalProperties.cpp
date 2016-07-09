/* --------------------------------------------------------------------------
 *
 *      File            Ch4_PhysicalProperties.cpp
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
#include "Ch4_PhysicalProperties.h"
#include "Libraries/GameMisc.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch4_PhysicalProperties::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Friction and restitution" );
	
	// Variable initialization
	m_pMovableObjects = CCArray::create ( );
	m_pMovableObjects->retain ( );

	m_bObjectGrabbed = KD_FALSE;
																																																					
	// Create level boundaries
	this->addLevelBoundaries ( );
			
	// Add a crate, a block of ice, bouncing balls and a ledge
	this->addBlockWithSpriteFile ( "crate2.png", 0.4f, 64.0f, 20.0f, ccp ( 130, 250 ) );
	this->addBlockWithSpriteFile ( "ice_block.png", 0.0f, 70.0f, 20.0f, ccp ( 10, 250 ) );

	this->addBallWithShapeSize (  5.0f, 0.9f, ccp ( 450, 200 ), ccc3 ( 255,   0, 0 ) );
	this->addBallWithShapeSize ( 10.0f, 0.8f, ccp ( 400, 200 ), ccc3 ( 255, 128, 0 ) );
	this->addBallWithShapeSize ( 15.0f, 0.7f, ccp ( 350, 200 ), ccc3 ( 255, 255, 0 ) );
	this->addBallWithShapeSize ( 20.0f, 0.6f, ccp ( 300, 200 ), ccc3 (   0, 255, 0 ) );

	this->addLedge ( );

	return KD_TRUE;
}


KDvoid Ch4_PhysicalProperties::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pMovableObjects );

	GameArea2D::onExit ( );
}

KDvoid Ch4_PhysicalProperties::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = this->convertTouchToNodeSpace ( pTouch );

	// Grab the nearest object 
	b2Vec2		tGrabbedPos = m_pGrabbedObject->getBody ( )->GetPosition ( );
	KDfloat		fGrabbedDistance = distanceBetweenPoints ( tPoint, ccp ( tGrabbedPos.x * PTM_RATIO, tGrabbedPos.y * PTM_RATIO ) );

	for ( KDuint i = 0; i < m_pMovableObjects->count ( ); i++ )
	{
		GameMisc*	pObject = (GameMisc*) m_pMovableObjects->objectAtIndex ( i );
		b2Vec2		tObjectPos = pObject->getBody ( )->GetPosition ( );
		KDfloat		fThisDistance = distanceBetweenPoints ( ccp ( tObjectPos.x * PTM_RATIO, tObjectPos.y * PTM_RATIO ), tPoint );

		if ( fThisDistance < fGrabbedDistance ) 
		{
			m_pGrabbedObject = pObject;
			fGrabbedDistance = fThisDistance;
		}
	}
	
	m_bObjectGrabbed = KD_TRUE;
	
	this->ccTouchesMoved ( pTouches, pEvent );	
}

KDvoid Ch4_PhysicalProperties::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = this->convertTouchToNodeSpace ( pTouch );

	// Reposition the grabbed object 
	b2Body*		pBody = m_pGrabbedObject->getBody ( );
	pBody->SetTransform ( b2Vec2 ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO ), pBody->GetAngle ( ) );

	// Reposition the grabbed fruit 
	CCPoint		tSpritePos = m_pGrabbedObject->getSprite ( )->getPosition ( );
	b2Vec2		tMoveDistance = b2Vec2 ( ( tPoint.x / PTM_RATIO - tSpritePos.x / PTM_RATIO ), ( tPoint.y / PTM_RATIO - tSpritePos.y / PTM_RATIO ) );
	m_tLastObjectVelocity = b2Vec2 ( tMoveDistance.x * 20, tMoveDistance.y * 20 );
}

KDvoid Ch4_PhysicalProperties::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Release the object 
	m_bObjectGrabbed = KD_FALSE;
	m_pGrabbedObject->getBody ( )->SetLinearVelocity ( m_tLastObjectVelocity );
}

KDvoid Ch4_PhysicalProperties::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );
	
	// Suspend the grabbed object 
	if ( m_bObjectGrabbed )
	{
		m_pGrabbedObject->getBody ( )->SetLinearVelocity ( b2Vec2_zero );
	}
}

/// Add a block with a certain texture, size, position and friction 
KDvoid Ch4_PhysicalProperties::addBlockWithSpriteFile ( const KDchar* szFile, KDfloat fFriction, KDfloat fTextureSize, KDfloat fShapeSize, const CCPoint tPoint )
{
	GameMisc*		pObject = GameMisc::create ( );
	pObject->setGameArea ( this );
	
	b2BodyDef*		pBodyDef = pObject->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pObject;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );
	
	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = fFriction;
	pFixtureDef->restitution = 0.25f;	

	CCSprite*		pSprite = CCSprite::create ( szFile );	
	pSprite->setPosition ( tPoint );
	pSprite->setScale ( fShapeSize / fTextureSize * 2 );
	pObject->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 2 );
					
	b2PolygonShape*		pShape = new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );
	pShape->SetAsBox ( fShapeSize / PTM_RATIO, fShapeSize / PTM_RATIO );		

	pFixtureDef->shape = pShape;		
	pBody->CreateFixture ( pFixtureDef );

	m_pMovableObjects->addObject ( pObject );
	m_pGrabbedObject = pObject;
}

/// Add a ball with a certain size, position, color and restitution 
KDvoid Ch4_PhysicalProperties::addBallWithShapeSize ( KDfloat fShapeSize, KDfloat fRestitution, const CCPoint& tPoint, const ccColor3B& tColor )
{
	GameMisc*		pObject = GameMisc::create ( );
	pObject->setGameArea ( this );
	
	b2BodyDef*		pBodyDef = pObject->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pObject;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );
	
	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 1.0f;
	pFixtureDef->restitution = fRestitution;	

	KDfloat			fTextureSize = 128.f;

	CCSprite*		pSprite = CCSprite::create ( "ball2.png" );	
	pSprite->setPosition ( tPoint );
	pSprite->setColor ( tColor );
	pSprite->setScale ( fShapeSize / fTextureSize * 2 );
	pObject->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 2 );

	b2CircleShape*		pShape = new b2CircleShape ( );
	pObject->setCircleShape ( pShape );
	pShape->m_radius = fShapeSize / PTM_RATIO;

	pFixtureDef->shape = pShape;		
	pBody->CreateFixture ( pFixtureDef );
		
	m_pMovableObjects->addObject ( pObject );
	m_pGrabbedObject = pObject;
}

/// Add a ledge polygon 
KDvoid Ch4_PhysicalProperties::addLedge ( KDvoid )
{
	GameMisc*		pObject = GameMisc::create ( );
	pObject->setGameArea ( this );

	b2BodyDef*		pBodyDef = pObject->getBodyDef ( );
	pBodyDef->position.Set ( 0, 100.f / PTM_RATIO );

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );
			
	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 0.3f;
	pFixtureDef->restitution = 0.2f;
			
	KDfloat			fPolygonSize = 4;
	CCPoint			aVertexArr [ ] = { ccp ( 0, 0.8f ), ccp ( 2, 0.5f ), ccp ( 2, 0.7f ), ccp ( 0, 1 ) };
	KDint32			nNumVerts = 4;
	b2Vec2			aVertices [ 4 ];
		
	CCArray*	pVertexArray = CCArray::create ( );
	
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		aVertices [ i ].Set ( aVertexArr [ i ].x * fPolygonSize, aVertexArr [ i ].y * fPolygonSize );
		pVertexArray->addObject 
		(
			CCPointValue::create ( ccp ( aVertexArr [ i ].x * PTM_RATIO * fPolygonSize, aVertexArr [ i ].y * PTM_RATIO * fPolygonSize ) ) 
		);
	}
				
	ccTexParams			tParams  = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	CCSprite*			pPolygon = TexturedPolygon::create ( "bricks2.png", pVertexArray );
	pPolygon->getTexture ( )->setTexParameters ( &tParams );
	pPolygon->setPosition ( ccp ( 0, 100 ) );
	m_pGameNode->addChild ( pPolygon, 1 );	

	b2PolygonShape*		pShape = new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );
	pShape->Set ( aVertices, nNumVerts );

	pFixtureDef->shape = pShape;	
	pBody->CreateFixture ( pFixtureDef );
}