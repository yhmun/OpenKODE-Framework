/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Joints.cpp
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
#include "Ch4_Joints.h"
#include "Libraries/GameMisc.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch4_Joints::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Drop the weight on the see-saw" );
	
	// Initialization
	m_pMovableObjects = CCArray::create ( );
	m_pMovableObjects->retain ( );

	m_bObjectGrabbed = KD_FALSE;
																											
	// Create level boundaries
	this->addLevelBoundaries ( );
			
	// Add objects
	this->addSeeSaw ( );
	this->addBoxWithPosition ( ccp ( 130, 120 ), "crate2.png", 1.0f );
	this->addBoxWithPosition ( ccp ( 160, 120 ), "crate2.png", 1.0f );
	this->addBoxWithPosition ( ccp ( 145, 150 ), "crate2.png", 1.0f );
	this->addBoxWithPosition ( ccp ( 270, 100 ), "weight.png", 15.0f );

	return KD_TRUE;
}

KDvoid Ch4_Joints::onExit ( KDvoid )
{
	m_pMovableObjects->release ( );

	GameArea2D::onExit ( );
}

KDvoid Ch4_Joints::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
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

KDvoid Ch4_Joints::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = this->convertTouchToNodeSpace ( pTouch );

	// Reposition the grabbed object 
	b2Body*		pBody = m_pGrabbedObject->getBody ( );
	pBody->SetTransform ( b2Vec2 ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO ), pBody->GetAngle ( ) );

	CCPoint		tSpritePos = m_pGrabbedObject->getSprite ( )->getPosition ( );
	b2Vec2		tMoveDistance = b2Vec2 ( ( tPoint.x / PTM_RATIO - tSpritePos.x / PTM_RATIO ), ( tPoint.y / PTM_RATIO - tSpritePos.y / PTM_RATIO ) );
	m_tLastObjectVelocity = b2Vec2 ( tMoveDistance.x * 20, tMoveDistance.y * 20 );
}

KDvoid Ch4_Joints::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Release the object 
	m_bObjectGrabbed = KD_FALSE;
	m_pGrabbedObject->getBody ( )->SetLinearVelocity ( m_tLastObjectVelocity );
}

KDvoid Ch4_Joints::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	// Suspend the grabbed object 
	if ( m_bObjectGrabbed )
	{
		m_pGrabbedObject->getBody ( )->SetLinearVelocity ( b2Vec2_zero );
	}
}

/// Create a complex see-saw object 
KDvoid Ch4_Joints::addSeeSaw ( KDvoid )
{
	// The triangle is the static base of the see-saw 
	CCPoint			tTriPos = ccp ( 240, 50 );
	
	GameMisc*		pTriangle = GameMisc::create ( );
	pTriangle->setGameArea ( this );
	
	b2BodyDef*		pTriBodyDef = pTriangle->getBodyDef ( );
	pTriBodyDef->type = b2_staticBody;
	pTriBodyDef->position.Set ( tTriPos.x / PTM_RATIO, tTriPos.y / PTM_RATIO );

	b2Body*			pTriBody = m_pWorld->CreateBody ( pTriBodyDef );
	pTriangle->setBody ( pTriBody );
	
	// Our triangle polygon
	KDfloat			fPolygonSize = 2.0f;
			
	CCPoint			aVertexArr [ ] = { ccp ( 0, 0 ), ccp ( 1, 0 ), ccp ( 0.5f, 1 ) };
	KDint32			nNumVerts = 3;
	b2Vec2			aVertices [ 3 ];
	
	CCArray*	pVertexArray = CCArray::create ( );
	
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		aVertices [ i ].Set ( aVertexArr [ i ].x * fPolygonSize, aVertexArr [ i ].y * fPolygonSize );
		pVertexArray->addObject 
		(
			CCPointValue::create ( ccp ( aVertexArr [ i ].x * PTM_RATIO * fPolygonSize, aVertexArr [ i ].y * PTM_RATIO * fPolygonSize ) ) 
		);
	}

	ccTexParams		tTriParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };

	CCSprite*		pTriSprite = TexturedPolygon::create ( "box.png", pVertexArray );
	pTriSprite->getTexture ( )->setTexParameters ( &tTriParams );
	pTriSprite->setPosition ( tTriPos );
	pTriangle->setSprite ( pTriSprite );
	m_pGameNode->addChild ( pTriSprite, 1 );
	
	b2PolygonShape*	pTriShape = new b2PolygonShape ( );
	pTriShape->Set ( aVertices, nNumVerts );
	pTriangle->setPolygonShape ( pTriShape );

	b2FixtureDef*	pTriFixtureDef = pTriangle->getFixtureDef ( );
	pTriFixtureDef->shape = pTriShape;	
	pTriBody->CreateFixture ( pTriFixtureDef );

	// The plank is the dynamic part of the see-saw 
	CCPoint			tPlankPos = ccp ( 270, 80 );	
	
	GameMisc*		pPlank = GameMisc::create ( );
	pPlank->setGameArea ( this );
	
	b2BodyDef*		pPlankBodyDef = pPlank->getBodyDef ( );
	pPlankBodyDef->type = b2_dynamicBody;
	pPlankBodyDef->position.Set ( tPlankPos.x / PTM_RATIO, tPlankPos.y / PTM_RATIO );

	b2Body*			pPlankBody = m_pWorld->CreateBody ( pPlankBodyDef );
	pPlank->setBody ( pPlankBody );
	pPlankBody->SetTransform ( b2Vec2 ( tPlankPos.x / PTM_RATIO, tPlankPos.y / PTM_RATIO ), KD_PI_F / 2 );

	CCPoint			tTextureSize = ccp ( 54, 215 );
	CCPoint			tShapeSize   = ccp ( 12, 180 );

	CCSprite*		pPlankSprite = CCSprite::create ( "column2.png" );
	pPlankSprite->setPosition ( tPlankPos );
	pPlankSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
	pPlankSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );
	pPlank->setSprite ( pPlankSprite );
	m_pGameNode->addChild ( pPlankSprite, 1 );

	b2PolygonShape*	pPlankShape = new b2PolygonShape ( );
	pPlankShape->SetAsBox ( tShapeSize.x / PTM_RATIO, tShapeSize.y / PTM_RATIO );
	pPlank->setPolygonShape ( pPlankShape );

	b2FixtureDef*	pPlankFixtureDef = pPlank->getFixtureDef ( );
	pPlankFixtureDef->shape = pPlankShape;	
	pPlankBody->CreateFixture ( pPlankFixtureDef );

	// We initialize a revolute joint linking the plank to the triangle 
	b2RevoluteJointDef		tRjd;
	b2RevoluteJoint*		pJoint;

	tRjd.Initialize ( pPlankBody, pTriBody, b2Vec2 ( tTriPos.x / PTM_RATIO + fPolygonSize / 2, tTriPos.y / PTM_RATIO + fPolygonSize / 2 ) );
	pJoint = (b2RevoluteJoint*) m_pWorld->CreateJoint ( &tRjd );
}

// Add a box object at a position 
KDvoid Ch4_Joints::addBoxWithPosition ( const CCPoint& tPoint, const KDchar* szFile, KDfloat fDensity )
{
	KDfloat			fTextureSize = 64.0f;
	KDfloat			fShapeSize = 20.0f;

	GameMisc*		pObject = GameMisc::create ( );
	pObject->setGameArea ( this );

	b2BodyDef*		pBodyDef = pObject->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pObject;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );

	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->density = fDensity;
	pFixtureDef->friction = 0.5f;
	pFixtureDef->restitution = 0.0f;

	CCSprite*		pSprite = CCSprite::create ( szFile );
	pObject->setSprite ( pSprite );
	pSprite->setPosition ( tPoint );	
	pSprite->setScale ( fShapeSize / fTextureSize * 2 );		
	m_pGameNode->addChild ( pSprite, 2 );
	
	b2PolygonShape*		pShape =  new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );
	pShape->SetAsBox ( fShapeSize / PTM_RATIO, fShapeSize / PTM_RATIO );		

	pFixtureDef->shape = pShape;
	pBody->CreateFixture ( pFixtureDef );

	m_pMovableObjects->addObject ( pObject );
	m_pGrabbedObject = pObject;
}