/* --------------------------------------------------------------------------
 *
 *      File            Ch8_Tiled.cpp
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
#include "Ch8_Tiled.h"

KDbool Ch8_Tiled::init ( KDvoid )
{	
	// Load TMX tilemap file
	m_pTileMap = CCTMXTiledMap::create ( "tilemap.tmx" );

	// Set game area size based on tilemap size
	this->setGameAreaSize ( );

	// Superclass initialization and message
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	this->showMessage ( "Use the DPad to move the actor around." );

	// Set initial variables
	m_pSpawnPoint = KD_NULL;
	m_fCameraZoom = 1.0f;

	// Make sure 2D projection is set
	CCDirector::sharedDirector ( )->setProjection ( kCCDirectorProjection2D );

	// Add tile map
	m_pGameNode->addChild ( m_pTileMap, 0 );

	// Re-order layers according to their Y value. This creates isometric depth. 
	KDfloat			mw = m_pTileMap->getMapSize ( ).cx;
	KDfloat			mh = m_pTileMap->getMapSize ( ).cy;
	KDfloat			tw = m_pTileMap->getTileSize ( ).cx;
	KDfloat			th = m_pTileMap->getTileSize ( ).cy;

	// Our layers
	CCTMXLayer*		pCollidableLayer = m_pTileMap->layerNamed ( "Collidable" );	
	CCTMXLayer*		pGround = m_pTileMap->layerNamed ( "Ground" );
	CCTMXLayer*		pWall = m_pTileMap->layerNamed ( "Wall" );

	// Gather all the layers into a container
	CCDictionary*	pLayersToReorder = CCDictionary::create ( );

	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pTileMap->getChildren ( ), pObject )	
	{
		CCTMXLayer*		pChild = (CCTMXLayer*) pObject;

		// Skip tiles marked "Collidable", "Ground" and "Wall"
		if ( pChild == pGround )
		{
			continue; 
		}
		else if ( pChild == pWall )
		{
			continue; 
		}
		else if ( pChild == pCollidableLayer )
		{
			continue; 
		}
		
		// Gather all the layers
		for ( KDfloat x = 0; x < mw; x += 1 )
		{
			for ( KDfloat y = mh - 1; y >= 0; y -= 1 )
			{
				CCSprite*	pChildTile = pChild->tileAt ( ccp ( x, y ) );
				CCSprite*	pCollideTile = pCollidableLayer->tileAt ( ccp ( x, y ) );
				
				if ( pChildTile && pCollideTile )
				{
					pLayersToReorder->setObject ( CCString::createWithFormat ( "%d", (KDint) y ), pChild->getLayerName ( ) );
					x = mw;
					y = -1;
				}
			}
		}

	}

	// Re-order gathered layers
	CCArray*	pKeys = pLayersToReorder->allKeys ( );
	CCARRAY_FOREACH ( pKeys, pObject )	
	{
		CCString*		pKey  = (CCString*) pObject;
		const KDchar*	szKey = pKey->getCString ( );
		
		m_pTileMap->reorderChild ( m_pTileMap->layerNamed ( szKey ), ( (CCString*) pLayersToReorder->objectForKey ( szKey ) )->intValue ( ) );
	}
		
	// Set the ground to z=0
	m_pTileMap->reorderChild ( pGround, 0 );

	// Add Box2D boxes to represent all layers marked "Collidable"
	for ( KDfloat x = 0; x < mw; x += 1 ) 
	{
		for ( KDfloat y = 0; y < mh; y += 1 )
		{
			if ( pCollidableLayer->tileAt ( ccp ( x, y ) ) )
			{
				this->addBoxAtPoint ( ccp ( x * tw, mh * th - y * th ), ccs ( tw / 2, th / 2 ) ); 
			}
		}
	}

	// Remove the "Collidable" layer art as its only an indicator for the level editor
//	m_pTileMap->removeChild ( pCollidableLayer, KD_TRUE );
	pCollidableLayer->setVisible ( KD_FALSE );

	// Add DPad
	this->addDPad ( );

	this->hideDebugDraw ( );

	// Create Actor
	this->addActor ( );
			
	// Create level boundaries
	this->addLevelBoundaries ( );

	// This sets off a chain reaction
	if ( m_pActor )
	{
		m_pActor->animateActor ( ); 
	}

	return KD_TRUE;
}

KDvoid Ch8_Tiled::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSpawnPoint );

	CCDirector::sharedDirector ( )->setProjection ( kCCDirectorProjection3D );

	GameArea2D::onExit ( );
}

KDvoid Ch8_Tiled::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );
	
	// Process DPad
	b2Body*		pBody = m_pActor->getBody ( );

	if ( m_pDPad->isPressed ( ) )
	{		
		pBody->ApplyForce ( b2Vec2 ( m_pDPad->getPressedVector ( ).x * 15.0f, m_pDPad->getPressedVector ( ).y * 15.0f ), pBody->GetPosition ( ) );
		pBody->SetAngularVelocity ( 0.0f );

		CCPoint		tMovementVector = ccp ( pBody->GetLinearVelocity ( ).x, pBody->GetLinearVelocity  ().y );
		pBody->SetTransform ( pBody->GetPosition ( ), -1 * GameHelper::vectorToRadians ( tMovementVector ) + PI_CONSTANT / 2 );
	}
	else
	{
		m_pActor->stopRunning ( );
	}

	// Store last gunman direction
	KDint		nDirection = m_pDPad->getDirection ( );
	if ( nDirection != NO_DIRECTION )
	{ 
		m_pActor->setDirection ( nDirection );
	}

	// Flip the animated sprite if neccessary
	if ( nDirection == LEFT || nDirection == UP_LEFT || nDirection == DOWN_LEFT )
	{
		( (CCSprite*) m_pActor->getSprite ( )->getChildByTag ( 0 ) )->setFlipX ( KD_TRUE );	
	}
	else if ( nDirection != NO_DIRECTION )
	{
		( (CCSprite*) m_pActor->getSprite ( )->getChildByTag ( 0 ) )->setFlipX ( KD_FALSE );
	}
		
	// Store last velocity
	m_pActor->setLastVelocity ( ccp ( pBody->GetLinearVelocity ( ).x, pBody->GetLinearVelocity ( ).y ) );
	
	// Re-order the actor
	KDfloat		mh = m_pTileMap->getMapSize ( ).cy;
	KDfloat		th = m_pTileMap->getTileSize ( ).cy;	

	CCPoint		p = m_pActor->getSprite ( )->getPosition ( );
	KDfloat		z = -( p.y / th ) + mh;	
	m_pTileMap->reorderChild ( m_pActor->getSprite ( ), (KDint) z ); 

	this->cameraFollowActor ( );
}

KDbool Ch8_Tiled::hudBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesBegan ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDbool Ch8_Tiled::hudMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesMoved ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDbool Ch8_Tiled::hudEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool	bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesEnded ( pTouches, pEvent ); 

	if ( m_pDPad->isPressed ( ) || pTouches->count ( ) == 1 )
	{
		bHudTouched = KD_TRUE; 
	}

	return bHudTouched;
}

KDvoid Ch8_Tiled::setGameAreaSize ( KDvoid )
{
	// Set gameAreaSize based on tileMap size
	m_tGameAreaSize = ccs 
	(
		( m_pTileMap->getMapSize ( ).cx * m_pTileMap->getTileSize ( ).cx ) / PTM_RATIO, 
		( m_pTileMap->getMapSize ( ).cy * m_pTileMap->getTileSize ( ).cy ) / PTM_RATIO 
	);	// Box2d units
}

KDvoid Ch8_Tiled::addDPad ( KDvoid )
{
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	
	m_pDPad = DPad::create ( );
	m_pDPad->setPosition ( ccp ( 75, 75 ) );
	m_pDPad->setOpacity ( 100 );
	this->addChild ( m_pDPad, 5 ); 
}

KDvoid Ch8_Tiled::addActor ( KDvoid )
{
	// Get spawn point from tile object named "SpawnPoint"
	if ( !m_pSpawnPoint )
	{
		CCTMXObjectGroup*	pObjects = m_pTileMap->objectGroupNamed ( "Objects" );
		CCAssert ( pObjects != KD_NULL, "'Objects' object group not found" );

		CCDictionary*		pSpawnPoint = pObjects->objectNamed ( "SpawnPoint" );        
		CCAssert ( pSpawnPoint != KD_NULL, "SpawnPoint object not found" );

		KDint				x = ( (CCString*) pSpawnPoint->valueForKey ( "x" ) )->intValue ( );
		KDint				y = ( (CCString*) pSpawnPoint->valueForKey ( "y" ) )->intValue ( );
		m_pSpawnPoint = Vector3D::create ( (KDfloat) x, (KDfloat) y, 0 );
		m_pSpawnPoint->retain ( );
	}

	// Add actor
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "gunman_lightblue.plist" );

	m_pActor = GameActor::create ( );
	m_pActor->setGameArea ( this );
	
	CCSprite*		pSprite = CCSprite::create ( "blank.png" );
	m_pActor->setSprite ( pSprite );
	pSprite->setScale ( 0.5f );
	pSprite->addChild ( CCSprite::create ( "blank.png" ), 1, 0 ); 

	b2BodyDef*		pBodyDef = m_pActor->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( m_pSpawnPoint->x / PTM_RATIO, m_pSpawnPoint->y / PTM_RATIO );
	pBodyDef->userData = m_pActor;
		
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pActor->setBody ( pBody );
	
	b2CircleShape*	pShape = new b2CircleShape ( );
	m_pActor->setCircleShape ( pShape );
	pShape->m_radius = 0.5f;

	b2FixtureDef*	pFixtureDef = m_pActor->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	
	pBody->CreateFixture ( pFixtureDef );

	pBody->SetLinearDamping ( 2.0f );

	m_pTileMap->addChild ( pSprite ); //(KDint) m_pTileMap->layerNamed ( "0" )->getVertexZ ( ) );
}

KDvoid Ch8_Tiled::cameraFollowActor ( KDvoid )
{
	// Follow the actor with the camera
	CCPoint		tActorPosition = ccp ( m_pActor->getBody ( )->GetPosition ( ).x * PTM_RATIO, m_pActor->getBody ( )->GetPosition ( ).y * PTM_RATIO );
	this->centerCameraOnGameCoord ( tActorPosition );	
}