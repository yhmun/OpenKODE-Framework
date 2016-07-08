/* --------------------------------------------------------------------------
 *
 *      File            Ch4_DifferentShapes.cpp
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
#include "Ch4_DifferentShapes.h"
#include "Libraries/GameMisc.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch4_DifferentShapes::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Tap to create shapes." );
			
	// Create level boundaries
	this->addLevelBoundaries ( );

	return KD_TRUE;
}

KDvoid Ch4_DifferentShapes::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );
		CCPoint		tLocation = this->convertTouchToNodeSpace ( pTouch );
	
		this->addNewObjectWithCoords ( tLocation );
	}
}

//	Here add an object randomly chosen from the following:
//		- a rectangle
//		- a square
//		- a circle
//		- a polygon
KDvoid Ch4_DifferentShapes::addNewObjectWithCoords ( const CCPoint& tPoint )
{
	// Initialize the object
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
	pFixtureDef->friction = 0.3f;
	pFixtureDef->restitution = 0.2f;
	
	// Pick a random shape, size and texture
	KDint		nNum = kdRand ( ) % 5;
	CCSprite*	pSprite = KD_NULL;

	if ( nNum == 0 )
	{
		// Square	
		KDfloat				fTextureSize = 128;
		KDfloat				fShapeSize	 = 16;
		
		pSprite = CCSprite::create ( "box2.png" );	
		pSprite->setPosition ( tPoint );
		pSprite->setScale ( fShapeSize / fTextureSize * 2 );
		pObject->setSprite ( pSprite );
		m_pGameNode->addChild ( pSprite );
		
		b2PolygonShape*		pShape = new b2PolygonShape ( );
		pObject->setPolygonShape ( pShape );
		pShape->SetAsBox ( fShapeSize / PTM_RATIO, fShapeSize / PTM_RATIO );		

		pFixtureDef->shape = pShape;		
		pBody->CreateFixture ( pFixtureDef );
	}
	else if ( nNum == 1 )
	{
		// Circle
		KDfloat				fTextureSize = 117;
		KDfloat				fShapeSize	 = 24;

		pSprite = CCSprite::create ( "ball2.png" );
		pSprite->setPosition ( tPoint );
		pSprite->setScale ( fShapeSize / fTextureSize * 2 );
		pObject->setSprite ( pSprite );
		m_pGameNode->addChild ( pSprite );
		
		b2CircleShape*		pShape = new b2CircleShape ( );
		pObject->setCircleShape ( pShape );
		pShape->m_radius = fShapeSize / PTM_RATIO;

		pFixtureDef->shape = pShape;		
		pFixtureDef->restitution = 0.9f;
		pBody->CreateFixture ( pFixtureDef );
	}
	else if ( nNum == 2 )
	{
		// Rectangle
		CCPoint				tTextureSize = ccp ( 54, 215 );
		CCPoint				tShapeSize   = ccp ( 12,  48 );
	
		pSprite = CCSprite::create ( "column.png" );
		pSprite->setPosition ( tPoint );
		pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2 );
		pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2 );
		pObject->setSprite ( pSprite );
		m_pGameNode->addChild ( pSprite );
	
		b2PolygonShape*		pShape = new b2PolygonShape ( );
		pObject->setPolygonShape ( pShape );
		pShape->SetAsBox ( tShapeSize.x / PTM_RATIO, tShapeSize.y / PTM_RATIO );		

		pFixtureDef->shape = pShape;	
		pBody->CreateFixture ( pFixtureDef );
	}
	else if ( nNum == 3 )
	{		
		// Convex Polygon
		KDfloat		fPolygonSize = 2.0f;
				
		CCPoint		aVertexArr [ ] =
		{
			ccp ( 0, 0.5f ), ccp ( 0.25f, 0 ), ccp ( 0.75f, 0     ), ccp ( 0.9f, 0.3f ),
			ccp ( 1, 0.8f ), ccp ( 0.80f, 1 ), ccp ( 0.40f, 0.95f ), ccp ( 0.1f, 0.8f ) 
		};

		KDint32		nNumVerts = 8;
		b2Vec2		aVertices [ 8 ];
		
		CCArray*	pVertexArray = CCArray::create ( );
		
		for ( KDint i = 0; i < nNumVerts; i++ )
		{
			aVertices [ i ].Set ( aVertexArr [ i ].x * fPolygonSize, aVertexArr [ i ].y * fPolygonSize );
			pVertexArray->addObject 
			(
				CCPointValue::create ( ccp ( aVertexArr [ i ].x * PTM_RATIO * fPolygonSize, aVertexArr [ i ].y * PTM_RATIO * fPolygonSize ) ) 
			);
		}
	
		ccTexParams		tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };

		pSprite = TexturedPolygon::create ( "box2.png", pVertexArray );
		pSprite->getTexture ( )->setTexParameters ( &tParams );
		pSprite->setPosition ( tPoint );
		pObject->setSprite ( pSprite );
		m_pGameNode->addChild ( pSprite, 1 );
			
		b2PolygonShape*		pShape = new b2PolygonShape ( );
		pObject->setPolygonShape ( pShape );
		pShape->Set ( aVertices, nNumVerts );

		pFixtureDef->shape = pShape;	
		pBody->CreateFixture ( pFixtureDef );
	}
	else if ( nNum == 4 )
	{
		// Concave Multi-Fixture Polygon
		pSprite = CCSprite::create ( "blank.png" );		
		pSprite->setPosition ( tPoint );
		pObject->setSprite ( pSprite );
		m_pGameNode->addChild ( pSprite, 1 );

		ccColor3B	tColor = ccc3 ( kdRand ( ) % 255, kdRand ( ) % 255, kdRand ( ) % 255 );
			
		for ( KDint i = 0; i < 2; i++ )
		{
			CCPoint		tShapeSize;
		
			if ( i == 0 )
			{
				tShapeSize = ccp ( 2.0f, 0.4f );
			}
			else
			{
				tShapeSize = ccp ( 0.4f, 2.0f );
			}
		
			CCPoint		aVertexArr [ ] = { ccp ( 0, 0 ), ccp ( tShapeSize.x, 0 ), ccp ( tShapeSize.x, tShapeSize.y ), ccp ( 0, tShapeSize.y ) };
					
			KDint32		nNumVerts = 4;
			b2Vec2		aVertices [ 4 ];
			
			CCArray*	pVertexArray = CCArray::create ( );
			
			for ( KDint i = 0; i < nNumVerts; i++ )
			{
				aVertices [ i ].Set ( aVertexArr [ i ].x, aVertexArr [ i ].y );
				pVertexArray->addObject 
				(
					CCPointValue::create ( ccp ( aVertexArr [ i ].x * PTM_RATIO, aVertexArr [ i ].y * PTM_RATIO ) ) 
				);
			}

			ccTexParams			tParams  = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
			CCSprite*			pPolygon = TexturedPolygon::create ( "box2.png", pVertexArray );
			pPolygon->getTexture ( )->setTexParameters ( &tParams );
			pPolygon->setPosition ( ccp ( 0, 0 ) );
			pPolygon->setColor ( tColor );
			pSprite->addChild ( pPolygon );
				
			b2PolygonShape*		pShape = new b2PolygonShape ( );
			pObject->setPolygonShape ( pShape );
			pShape->Set ( aVertices, nNumVerts );

			pFixtureDef->shape = pShape;	
			pBody->CreateFixture ( pFixtureDef );
		}		
	}
	
	// Set a random color
	pSprite->setColor ( ccc3 ( kdRand ( ) % 255, kdRand ( ) % 255, kdRand ( ) % 255 ) );
}