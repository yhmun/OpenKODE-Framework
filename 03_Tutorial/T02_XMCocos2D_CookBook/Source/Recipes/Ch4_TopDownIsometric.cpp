/* --------------------------------------------------------------------------
 *
 *      File            Ch4_TopDownIsometric.cpp
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
#include "Ch4_TopDownIsometric.h"
#include "Libraries/GameButton.h"
#include "Libraries/TexturedPolygon.h"
#include "Libraries/IsometricContactListener.h"

static const KDfloat		scaleHeightMultiplier = 0.0005f;	// How much larger objects look in the air
static const KDfloat		zHeightModifier = 1.0f;				// How higher or lower objects look in the air

KDbool Ch4_TopDownIsometric::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_tGameAreaSize = ccs ( 2000, 1200 );

	m_pMessage->setString ( "Press A to jump. \nPress B to fire bouncy balls. \nPinch to zoom." );
			
	// Initialization
	m_pBalls = new CCArray ( );
	m_pTrees = new CCArray ( );
	m_fAnimCount = 0;
	
	m_fCameraZoom = 1.0f;
	m_fLastMultiTouchZoomDistance = 0.0f;
	m_tLastPressedVector = ccp ( 1, 0 );
	m_fFireCount = 0;
	m_nGunmanDirection = RIGHT;
	m_pButtons = new CCArray ( );
	
	// Special isometric gravity and contact listener
	m_pWorld->SetGravity ( b2Vec2 ( 0, 0 ) );
	m_pWorld->SetContactListener ( new IsometricContactListener );

	// Iso debug drawing
	CC_SAFE_DELETE ( m_pDebugDraw );
	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO, PERSPECTIVE_RATIO, m_tGameAreaSize );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	// Initialize gunman
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "gunman.plist" );
	this->initGunman ( );

	//Initialize DPad
	pCache->addSpriteFramesWithFile ( "dpad_buttons.plist" );
	m_pDPad = DPad::create ( );
	m_pDPad->setPosition ( ccp ( 75, 75 ) );
	m_pDPad->setOpacity ( 100 );
	this->addChild ( m_pDPad, 5 );

	// Create buttons		
	this->createButtonWithPosition ( ccp ( 340, 75 ), "b_button_up.png", "b_button_down.png", "B" );
	this->createButtonWithPosition ( ccp ( 420, 75 ), "a_button_up.png", "a_button_down.png", "A" );

	// This sets off a chain reaction
	this->animateGunman ( );
	
	// Add ground texture
	this->addGroundTexture ( );
	
	// Add trees
	for ( KDint i = 0; i < 20; i++ )
	{
		this->addTreeAtPoint 
		(
			ccp ( kdRand ( ) % ( (KDint) m_tGameAreaSize.cx - 200 ) + 100, kdRand ( ) % ( (KDint) m_tGameAreaSize.cy - 200 ) + 100 ) 
		);
	}
	
	// Add walls 
	
	// Horizontal
	for ( KDint i = 0; i < 10; i++ )
	{
		KDfloat		fLength = kdRand ( ) % 50 + 50.f;
		KDfloat		x = kdRand ( ) % ( (KDint) ( m_tGameAreaSize.cx - fLength * 8 ) ) + fLength * 4;
		KDfloat		y = kdRand ( ) % ( (KDint) m_tGameAreaSize.cy - 400 ) + 200.f;
		KDfloat		fHeight = kdRand ( ) % 200 * 1.f;
		this->addBrickWallFrom ( ccp ( x - fLength, y ), ccp ( x + fLength, y ), fHeight );
	}
	
	// Vertical
	for ( KDint i = 0; i < 10; i++ )
	{
		KDfloat		fLength = kdRand ( ) % 100 * 1.f;
		KDfloat		y = kdRand ( ) % ( (KDint) ( m_tGameAreaSize.cy - fLength * 8 ) ) + fLength * 4;
		KDfloat		x = kdRand ( ) % ( (KDint) m_tGameAreaSize.cx - 400 ) + 200.f;
		KDfloat		fHeight = kdRand ( ) % 50 + 50.f;
		this->addBrickWallFrom ( ccp ( x, y - fLength ), ccp ( x, y + fLength ), fHeight );
	}
	
	// Level Boundaries
	this->addBrickWallFrom ( ccp ( 150, 150 ), ccp ( m_tGameAreaSize.cx - 150, 150 ), 100.0f );
	this->addBrickWallFrom ( ccp ( 150, 150 ), ccp ( 150, m_tGameAreaSize.cy - 150 ), 100.0f );
	this->addBrickWallFrom ( ccp ( 150, m_tGameAreaSize.cy - 150 ), ccp ( m_tGameAreaSize.cx - 150, m_tGameAreaSize.cy - 150 ), 100.0f );
	this->addBrickWallFrom ( ccp ( m_tGameAreaSize.cx - 150, 150 ), ccp ( m_tGameAreaSize.cx - 150, m_tGameAreaSize.cy - 150 ), 100.0f );

	return KD_TRUE;
}

KDvoid Ch4_TopDownIsometric::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pBalls );
	CC_SAFE_DELETE ( m_pTrees );

	CC_SAFE_DELETE ( m_pButtons );

	GameArea2D::onExit ( );
}

/// Process touches 
KDvoid Ch4_TopDownIsometric::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	// Process dPad and button input
	KDbool		bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesBegan ( pTouches, pEvent );
	if ( m_pDPad->isPressed ( ) )
	{
		bHudTouched = KD_TRUE; 
	}

	if ( !bHudTouched )
	{
		if ( pTouches->count ( ) == 1 && m_pAllTouches->count ( ) == 1 )
		{
			CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
			CCPoint		tPoint = pTouch->getLocation ( );
			
			m_tDraggedToPoint   = ccp ( 0, 0 );
			m_tLastTouchedPoint = tPoint;
		}
		else if ( m_pAllTouches->count ( ) == 2 )
		{				
			m_fLastMultiTouchZoomDistance = 0.0f;
		}
	}

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesBegan ( pTouches, pEvent );	

		if ( pButton->isPressed ( ) )
		{
			bHudTouched = KD_TRUE;
		}
	}

	// Process zooming if we didn't touch the HUD
	if ( !bHudTouched )
	{
		m_fLastMultiTouchZoomDistance = 0.0f;
	}
}

KDvoid Ch4_TopDownIsometric::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	KDbool		bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesMoved ( pTouches, pEvent );
	if ( m_pDPad->isPressed ( ) )
	{
		bHudTouched = KD_TRUE; 
	}

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesMoved ( pTouches, pEvent );	

		if ( pButton->isPressed ( ) )
		{
			bHudTouched = KD_TRUE;
		}
	}
	
	// Proecss multi-touch zoom input	
	if ( pTouches->count ( ) == 2 )
	{			
		if ( !bHudTouched )
		{
			const CCSize&	tLyrSize = this->getContentSize ( );

			CCArray*	pTwoTouch = pTouches->allObjects ( );				
			CCTouch*	pTouch1 = (CCTouch*) pTwoTouch->objectAtIndex ( 0 );
			CCTouch*	pTouch2 = (CCTouch*) pTwoTouch->objectAtIndex ( 1 );

			CCPoint		tTouch1 = pTouch1->getLocation ( );
			CCPoint		tTouch2 = pTouch2->getLocation ( );

			KDfloat		fCurrentDistance = distanceBetweenPoints ( tTouch1, tTouch2 );
		
			if ( m_fLastMultiTouchZoomDistance == 0 )
			{
				m_fLastMultiTouchZoomDistance = fCurrentDistance;
			}
			else
			{
				KDfloat		fDifference = fCurrentDistance - m_fLastMultiTouchZoomDistance;
				KDfloat		fNewZoom = ( m_fCameraZoom + ( fDifference / tLyrSize.cy ) );
				
				if ( fNewZoom < 0.4f ) 
				{
					fNewZoom = 0.4f; 
				}

				if ( fNewZoom > 2.0f )
				{
					fNewZoom = 2.0f; 
				}

				this->setCameraZoom ( fNewZoom );
				m_fLastMultiTouchZoomDistance = fCurrentDistance;
			}
		}
	}
}

KDvoid Ch4_TopDownIsometric::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Process dPad and button input
	KDbool		bHudTouched = KD_FALSE;

	m_pDPad->ccTouchesEnded ( pTouches, pEvent );
	if ( m_pDPad->isPressed ( ) )
	{
		bHudTouched = KD_TRUE; 
	}

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		pButton->ccTouchesEnded ( pTouches, pEvent );	

		if ( pButton->isPressed ( ) )
		{
			bHudTouched = KD_TRUE;
		}
	}

	// Process zooming if we didn't touch the HUD
	if ( !bHudTouched )
	{
		m_fLastMultiTouchZoomDistance = 0.0f;
	}
}

/// Obligatory isometric collision handling routines 
KDvoid Ch4_TopDownIsometric::handleCollisionWithGroundWithObj ( GameObject* pObject )
{

}

KDvoid Ch4_TopDownIsometric::handleZMissWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
{

}

/// We override all physical calculations here 
KDvoid Ch4_TopDownIsometric::step ( KDfloat fDelta )
{
	// Update Physics
	
	KDint32		nVelocityIterations = 8;
	KDint32		nPositionIterations = 3;
	
	m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
	
	KDfloat		fDeltaMod = fDelta / 0.01666666667f;
	
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody; pBody = pBody->GetNext ( ) )
	{
		// Z Miss handling allows us to know when an object passes over or under another object
		pBody->SetHandleZMiss ( KD_TRUE );
		
		if ( pBody->GetUserData ( ) != KD_NULL )
		{
			// Synchronize the sprites position and rotation with the corresponding body
			GameIsoObject*		pGameObject  = (GameIsoObject*) pBody->GetUserData ( );
			CCSprite*			pGameSprite  = pGameObject->getSprite ( );
			CCSprite*			pGameShadow  = pGameObject->getSpriteShadow ( );
			b2BodyDef*			pGaneBodyDef = pGameObject->getBodyDef ( );
			b2Body*				pGameBody    = pGameObject->getBody ( );
							
			if ( pGameSprite )
			{				
				if ( pGaneBodyDef->type == b2_dynamicBody )
				{	
				
					// Process Z velocity and position
					pGameBody->SetZVelocity ( pGameBody->GetZVelocity ( ) - GRAVITY * fDeltaMod );
					pGameBody->SetZPosition ( pGameBody->GetZPosition ( ) + pGameBody->GetZVelocity ( ) * fDeltaMod );					
					
					// Process object bouncing and rolling
					if ( pGameBody->GetZPosition ( ) < ( -0.01f ) ) 
					{
						pGameBody->SetZPosition ( 0.01f );
						pGameBody->SetZVelocity ( pGameBody->GetZVelocity ( ) * -1 );
						
						b2Vec2		tWorldVector = pGameBody->GetLinearVelocityFromLocalPoint ( b2Vec2 ( 0, 0 ) );
						Vector3D*	pVector = KD_NULL;
						if ( absoluteValue ( pGameBody->GetZVelocity ( ) ) > 1.0f )
						{
							pVector = pGameObject->getBounceCoefficient ( );
							this->handleCollisionWithGroundWithObj ( pGameObject );
						}
						else
						{
							pVector = pGameObject->getRollCoefficient ( );
						}

						pGameBody->ApplyLinearImpulse ( b2Vec2 ( pVector->x * tWorldVector.x * -1, pVector->y * tWorldVector.y * -1 ), pGameBody->GetPosition ( ) );
						pGameBody->SetZVelocity ( pGameBody->GetZVelocity ( ) * ( 1 - pVector->z ) );
					}
				
					// Change sprite positions based on body positions
					pGameSprite->setPosition 
					(
						ccp 
						(
							convertPositionX ( m_tGameAreaSize, pBody->GetPosition ( ).x * PTM_RATIO ),
							convertPositionY ( m_tGameAreaSize, pBody->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO ) + pGameObject->getModifierY ( ) +  pGameBody->GetZPosition ( ) * zHeightModifier * PERSPECTIVE_RATIO 
						) 
					);
					pGameShadow->setPosition 
					(
						ccp 
						(
							convertPositionX ( m_tGameAreaSize, pBody->GetPosition ( ).x * PTM_RATIO ),
							convertPositionY ( m_tGameAreaSize, pBody->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO )
						) 
					);
			
					// Modify sprite scale based on Z (height)
					pGameSprite->setScale ( pGameBody->GetZPosition ( ) * scaleHeightMultiplier + pGameObject->getInGameSize ( )  / pGameObject->getActualImageSize ( ) );
					pGameShadow->setScale ( pGameBody->GetZPosition ( ) / 100 );
					if( pGameShadow->getScale ( ) > 1 )
					{
						pGameShadow->setScale ( 1 ); 
					}
					
					// Sprite depth testing based on Y (depth)
					this->setZOrderByBodyPosition ( pGameObject );					
				}
				else if ( pGaneBodyDef->type == b2_staticBody )
				{
					// Static bodies are only positioned and depth tested
					pGameSprite->setPosition 
					(
						ccp
						(
							convertPositionX ( m_tGameAreaSize, pBody->GetPosition ( ).x * PTM_RATIO ),
							convertPositionY ( m_tGameAreaSize, pBody->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO ) + pGameObject->getModifierY ( ) +  pGameBody->GetZPosition ( ) * zHeightModifier * PERSPECTIVE_RATIO 
						)
					);
					this->setZOrderByBodyPosition ( pGameObject );
					pGameShadow->setPosition 
					(
						ccp 
						(
							convertPositionX ( m_tGameAreaSize, pBody->GetPosition ( ).x * PTM_RATIO ),
							convertPositionY ( m_tGameAreaSize, pBody->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO )
						) 
					);
				}				
			}			
		}			
	}
		
	// Process body creation/destruction
	this->destroyBodies ( );
	this->createBodies ( );
	this->runQueuedActions ( );

	// Process gunman 	
	// Follow gunman with camera
	m_pGameNode->setPosition 
	(
		ccp 
		(
			-m_pGunman->getSpriteShadow ( )->getPosition ( ).x * m_fCameraZoom + 240, 
			-m_pGunman->getSpriteShadow ( )->getPosition ( ).y * m_fCameraZoom + 160 
		)
	);
	
	
	// Store last gunman direction
	KDint		nDPadDirction = m_pDPad->getDirection ( );
	if ( nDPadDirction != NO_DIRECTION )
	{ 
		m_nGunmanDirection   = nDPadDirction;
		m_tLastPressedVector = m_pDPad->getPressedVector ( );
	}
	
	// Process gunman running
	b2Body*		pGunmanBody = m_pGunman->getBody ( );
	pGunmanBody->ApplyForce ( b2Vec2 ( m_pDPad->getPressedVector ( ).x * 75.0f, m_pDPad->getPressedVector ( ).y * 75.0f ), pGunmanBody->GetPosition ( ) );
	
	// Flip the animated sprite if neccessary
	if ( nDPadDirction == LEFT || nDPadDirction == UP_LEFT || nDPadDirction == DOWN_LEFT )
	{
		( (CCSprite*) m_pGunman->getSprite ( )->getChildByTag ( 0 ) )->setFlipX ( KD_TRUE );	
	}
	else if ( nDPadDirction != NO_DIRECTION )
	{
		( (CCSprite*) m_pGunman->getSprite ( )->getChildByTag ( 0 ) )->setFlipX ( KD_FALSE );	
	}

	// Re-animate the gunman if neccessary
	m_fAnimCount += fDelta;
	if ( m_fAnimCount > 1.0f )
	{
		m_pGunman->getSprite ( )->getChildByTag ( 0 )->stopAllActions ( );
		this->animateGunman ( );
		m_fAnimCount = 0;
	}

	// Decrement fireCount
	m_fFireCount -= fDelta;
				
	// Store last velocity
	m_tLastVelocity = ccp ( pGunmanBody->GetLinearVelocity ( ).x, pGunmanBody->GetLinearVelocity ( ).y );	

	// Process camera 
	this->checkCameraBounds ( );
	this->processZoomStep ( );

	// Process button input 
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pButtons, pObject )
	{
		GameButton*		pButton = (GameButton*) pObject;
		
		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "B" ) )
		{
			this->fireBall ( );
		}

		if ( pButton->isPressed ( ) && !kdStrcmp ( pButton->getName ( ), "A" ) )
		{
			this->processJump ( );
		}
	}
}

/// Add a ground grass texture 
KDvoid Ch4_TopDownIsometric::addGroundTexture ( KDvoid )
{
	CCPoint			aGrassVertexArr [ ] = 
	{
		ccp ( 0, 0 ),
		ccp ( m_tGameAreaSize.cx, 0 ),
		ccp ( m_tGameAreaSize.cx, m_tGameAreaSize.cy ), 
		ccp ( 0, m_tGameAreaSize.cy ) 
	};

	KDint32			nGrassNumVerts = 4;
	CCArray*		pGrassVertices = CCArray::create ( );
	
	for ( KDint i = 0; i < nGrassNumVerts; i++ )
	{		
		pGrassVertices->addObject 
		(
			CCPointValue::create ( ccp ( aGrassVertexArr [ i ].x, aGrassVertexArr [ i ].y ) ) 
		);
	}
				
	ccTexParams			tParams  = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	TexturedPolygon*	pGrassPoly = TexturedPolygon::create ( "grass_texture.png", pGrassVertices );
	pGrassPoly->getTexture ( )->setTexParameters ( &tParams );
	pGrassPoly->setPosition ( ccp ( -m_tGameAreaSize.cx / 2, -m_tGameAreaSize.cy / 2 ) );
	m_pGameNode->addChild ( pGrassPoly, 0 );	
}

/// Init the gunman 
KDvoid Ch4_TopDownIsometric::initGunman ( KDvoid )
{
	m_pGunman = GameIsoObject::create ( );
	m_pGunman->setGameArea ( this );
	m_pGunman->setTypeTag ( TYPE_OBJ_GUNMAN );

	CCPoint			tGunmanPosition = ccp ( 300, 500 );

	b2BodyDef*		pBodyDef = m_pGunman->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tGunmanPosition.x / PTM_RATIO, tGunmanPosition.y / PTM_RATIO );

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pGunman->setBody ( pBody );

	CCPoint			tTextureSize = ccp ( 128, 128 );
	CCPoint			tShapeSize   = ccp (  25,  25 );

	m_pGunman->setModifierY ( tShapeSize.y );
	m_pGunman->setModifierZ ( -tShapeSize.y / 2 );
	m_pGunman->setActualImageSize ( tTextureSize.x );
	m_pGunman->setInGameSize ( tTextureSize.x / 2 );	

	CCSprite*		pSprite = CCSprite::create ( "blank.png" );	
	pSprite->setPosition ( tGunmanPosition );	
	pSprite->setScaleX ( tShapeSize.x / tTextureSize.x * 2.25f );
	pSprite->setScaleY ( tShapeSize.y / tTextureSize.y * 2.25f );
	pSprite->addChild ( CCSprite::create ( "blank.png" ), 1, 0 );
	m_pGunman->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 5 );

	b2CircleShape*	pShape =  new b2CircleShape ( );
	m_pGunman->setCircleShape ( pShape );
	pShape->m_radius = tShapeSize.x / PTM_RATIO;

	b2FixtureDef*	pFixtureDef = m_pGunman->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = 2.0f;
	pFixtureDef->friction = 0.0f;
	pFixtureDef->restitution = 0.0f;
	pBody->CreateFixture ( pFixtureDef );
	pBody->SetLinearDamping ( 2.f );

	// Gunman shadow
	m_pGunman->setSpriteShadow ( CCSprite::create ( "actor_shadow.png" ) );
	m_pGameNode->addChild ( m_pGunman->getSpriteShadow ( ), 4 );
}

/// Repeated gunman animation routine 
KDvoid Ch4_TopDownIsometric::animateGunman ( KDvoid )
{	
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Animation delay is inverse speed
	KDfloat			fDelay = 0.5f / kdSqrtf ( kdPowf ( m_tLastVelocity.x, 2 ) + kdPowf ( m_tLastVelocity.y, 2 ) );
	if ( fDelay > 0.5f )
	{
		fDelay = 0.5f; 
	}

	CCAnimation*	pAnimation = CCAnimation::create ( ); 
	pAnimation->setDelayPerUnit ( fDelay );

	const KDchar*	szDirection;
	KDbool			bFlipX  = KD_FALSE;
	KDbool			bMoving = KD_TRUE;
	
	if ( kdSqrtf ( kdPowf ( m_tLastVelocity.x, 2 ) + kdPowf ( m_tLastVelocity.y, 2 ) ) < 0.2f )
	{
		bMoving = KD_FALSE; 
	}
	
	switch ( m_nGunmanDirection )
	{
		case LEFT		:	szDirection = "right";		bFlipX = KD_TRUE;	break;
		case UP_LEFT	:	szDirection = "up_right";	bFlipX = KD_TRUE;	break;
		case UP			:	szDirection = "up";								break;
		case UP_RIGHT	:	szDirection = "up_right";						break;
		case RIGHT		:	szDirection = "right";							break;
		case DOWN_RIGHT	:	szDirection = "down_right";						break;
		case DOWN		:	szDirection = "down";							break;
		case DOWN_LEFT	:	szDirection = "down_right";	bFlipX = KD_TRUE;	break;
	}

	if ( bMoving )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_run_%s_01.png", szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_run_%s_02.png", szDirection ) ) );
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
	}
	else
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "gunman_stand_%s.png" , szDirection ) ) );
	}

	// animateGunman calls itself indefinitely
	m_pGunman->getSprite ( )->getChildByTag ( 0 )->runAction 
	(
		CCSequence::create 
		(
			CCAnimate::create ( pAnimation ),
			CCCallFunc::create ( this, callfunc_selector ( Ch4_TopDownIsometric::animateGunman ) ),
			KD_NULL
		)
	);
}

/// Create a button 
KDvoid Ch4_TopDownIsometric::createButtonWithPosition ( const CCPoint& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	GameButton*		pButton = GameButton::create ( );
	m_pButtons->addObject ( pButton );

	pButton->setPosition ( tPosition );
	pButton->setOpacity ( 100 );
	pButton->setUpSpriteFrame ( szUpFrame );
	pButton->setDownSpriteFrame ( szDownFrame );
	pButton->setDisplayFrame ( pCache->spriteFrameByName ( pButton->getUpSpriteFrame ( ) ) );
	pButton->setName ( szName );
	this->addChild ( pButton, 5 );
}

/// Fire a bouncy ball 
KDvoid Ch4_TopDownIsometric::fireBall ( KDvoid )
{
	if ( m_fFireCount < 0 )
	{
		b2Body*			pGunmanBody = m_pGunman->getBody ( );
		GameIsoObject*	pBall = this->addBallAtPoint 
		(
			ccp 
			(
				pGunmanBody->GetPosition ( ).x * PTM_RATIO + m_tLastPressedVector.x * 20.0f,
				pGunmanBody->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO + m_tLastPressedVector.y * 20.0f 
			)
		);

		b2Body*			pBallBody = pBall->getBody ( );

		pBallBody->ApplyLinearImpulse ( b2Vec2 ( m_tLastPressedVector.x * 1.75f, m_tLastPressedVector.y * 1.75f ), pBallBody->GetPosition ( ) );
		pBallBody->SetZVelocity ( pGunmanBody->GetZVelocity ( ) * 5.0f + 10.0f );
		pBallBody->SetZPosition ( pGunmanBody->GetZPosition ( ) + 40.0f );
		m_fFireCount = 10;
	}
	else
	{
		m_fFireCount--;
	}
}

/// Process a jump 
KDvoid Ch4_TopDownIsometric::processJump ( KDvoid )
{
	// You can only jump if you are standing or running. You also need to be on the ground.
	if ( m_pGunman->getBody ( )->GetZPosition ( ) > 1.0f )
	{	
		return;
	}
	
	// Make him jump
	m_pGunman->getSprite ( )->getChildByTag ( 0 )->stopAllActions ( );
	m_pGunman->getBody ( )->SetZVelocity ( 7.5f );
}

/// Convert a body position to a world position 
CCPoint Ch4_TopDownIsometric::getWorldPosition ( GameIsoObject* pGameIsoObject )
{
	return ccp 
	(
		pGameIsoObject->getBody ( )->GetPosition ( ).x * PTM_RATIO, 
		pGameIsoObject->getBody ( )->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO
	);
}

// Depth testing 
KDvoid Ch4_TopDownIsometric::setZOrderByBodyPosition ( GameIsoObject* pGameIsoObject )
{
	KDfloat		fFixedPositionY = m_tGameAreaSize.cy - ( pGameIsoObject->getBody ( )->GetPosition ( ).y * PTM_RATIO * PERSPECTIVE_RATIO )  + pGameIsoObject->getModifierZ ( );
	pGameIsoObject->getSprite ( )->getParent ( )->reorderChild ( pGameIsoObject->getSprite ( ), (KDint) fFixedPositionY );
}

/// A camera bound limiting routine 
KDbool Ch4_TopDownIsometric::checkCameraBoundsWithFailPosition ( CCPoint* pFailPosition )
{
	KDbool			bPassed  = KD_TRUE;
	const CCSize&	tLyrSize = this->getContentSize ( );

	KDfloat		fsx = ( m_tGameAreaSize.cx / 2 ) * m_fCameraZoom;
	KDfloat		fsy = ( m_tGameAreaSize.cy / 2 ) * m_fCameraZoom;
	KDfloat		ssx = tLyrSize.cx;
	KDfloat		ssy = tLyrSize.cy;
			
	if ( m_pGameNode->getPosition ( ).y < -( fsy - ssy ) ) 
	{
		( *pFailPosition ).y = -( fsy - ssy );
		bPassed = KD_FALSE;
	}
	else if ( m_pGameNode->getPosition ( ).y > fsy )
	{
		( *pFailPosition ).y = fsy;
		bPassed = KD_FALSE;
	}
	else
	{
		// Passed
		( *pFailPosition ).y = m_pGameNode->getPosition ( ).y;
	}

	if ( m_pGameNode->getPosition ( ).x < -( fsx - ssx ) ) 
	{
		( *pFailPosition ).x = -( fsx - ssx );
		bPassed = KD_FALSE;
	}
	else if ( m_pGameNode->getPosition ( ).x > fsx )
	{
		( *pFailPosition ).x = fsx;
		bPassed = KD_FALSE;
	}
	else 
	{ 
		// Passed
		( *pFailPosition ).x = m_pGameNode->getPosition ( ).x;
	}	
	
	return bPassed;
}

/// Set the camera zoom with limits 
KDvoid Ch4_TopDownIsometric::setCameraZoom ( KDfloat fZoom )
{
	if ( fZoom < 0.1f ) { fZoom = 0.1f; }	// Lower limit

	GameArea2D::setCameraZoom ( fZoom );
}

/// Add a brick wall 
KDvoid Ch4_TopDownIsometric::addBrickWallFrom ( CCPoint tPoint1, CCPoint tPoint2, KDfloat fHeight )
{
	// Walls can only be added at 90 degree angles
	// Depth is only used to give the wall some perceived depth. The wall is always just a line.

	// Determine whether this is a horizontal or vertical wall
	KDbool		bHoriz = KD_FALSE;
	if ( tPoint1.y == tPoint2.y )
	{
		bHoriz = KD_TRUE; 
	}

	// Make sure the wall is drawn in 1 direction only
	if ( bHoriz && tPoint1.x > tPoint2.x )
	{ 
		KDfloat		fPoint1X = tPoint1.x;
		KDfloat		fPoint2X = tPoint2.x;
		tPoint1 = ccp ( fPoint2X, tPoint1.y );
		tPoint2 = ccp ( fPoint1X, tPoint2.y );
	}
	else if ( !bHoriz && tPoint1.y > tPoint2.y )
	{
		KDfloat		fPoint1Y = tPoint1.y;
		KDfloat		fPoint2Y = tPoint2.y;
		tPoint1 = ccp ( tPoint1.x, fPoint2Y );
		tPoint2 = ccp ( tPoint2.x, fPoint1Y );	
	}

	// Convert global to local positions
	CCPoint			tMidPoint = midPoint ( tPoint1, tPoint2 );
	tPoint1 = ccp ( tMidPoint.x - tPoint1.x, tMidPoint.y - tPoint1.y );
	tPoint2 = ccp ( tMidPoint.x - tPoint2.x, tMidPoint.y - tPoint2.y );
	
	// Create hte wall
	GameIsoObject*	pWall = GameIsoObject::create ( );
	pWall->setGameArea ( this );

	b2BodyDef*		pBodyDef = pWall->getBodyDef ( );
	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( tMidPoint.x / PTM_RATIO, tMidPoint.y / PTM_RATIO / PERSPECTIVE_RATIO );	

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pWall->setBody ( pBody );

	CCPoint			tTextureSize = ccp ( 64, 64 );
	KDfloat			fDepth = 10.f;

	pBody->SetZSize ( fHeight * 2 );
	pBody->SetZPosition ( 0 );

	pWall->setModifierY ( 0 );
	pWall->setActualImageSize ( tTextureSize.x );
	pWall->setInGameSize ( tTextureSize.x / 2 );

	if ( !bHoriz )
	{
		pWall->setModifierZ ( ( tPoint2.y - tPoint1.y ) / 2 );
	}
	
	// The base sprite is blank. Then we add everything to that.
	CCSprite*		pSprite = CCSprite::create ( "blank.png" );	
	pWall->setSprite ( pSprite );
	m_pGameNode->addChild ( pSprite, 5 );

	// Wall Top
	CCArray*		pVertexArray = CCArray::create ( );
	KDint32			nNumVerts = 4;
	ccTexParams		tParams  = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };

	if ( bHoriz )
	{
		CCPoint		aVertexArr [ ] = 
		{
			ccp ( tPoint1.x, tPoint1.y + fHeight ),
			ccp ( tPoint2.x, tPoint2.y + fHeight ),
			ccp ( tPoint2.x, tPoint2.y + fHeight + fDepth / 2 ), 
			ccp ( tPoint1.x, tPoint1.y + fHeight + fDepth / 2 ) 
		};

		for ( KDint i = 0; i < nNumVerts; i++ )
		{		
			pVertexArray->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x, aVertexArr [ i ].y ) ) );
		}
	}
	else
	{
		CCPoint		aVertexArr [ ] = 
		{
			ccp ( tPoint1.x - fDepth / 2, tPoint1.y + fHeight ),
			ccp ( tPoint2.x - fDepth / 2, tPoint2.y + fHeight ),
			ccp ( tPoint2.x + fDepth / 2, tPoint2.y + fHeight ), 
			ccp ( tPoint1.x + fDepth / 2, tPoint1.y + fHeight ) 
		};

		for ( KDint i = 0; i < nNumVerts; i++ )
		{		
			pVertexArray->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x, aVertexArr [ i ].y ) ) );
		}
	}
	
	TexturedPolygon*	pWallPoly = TexturedPolygon::create ( "bricks_top.png", pVertexArray );
	pWallPoly->getTexture ( )->setTexParameters ( &tParams );
	pWallPoly->setPosition ( ccp ( 0, 0 ) );
	pSprite->addChild ( pWallPoly, 2, 0 );	

	// Wall Side
	pVertexArray = CCArray::create ( );

	if ( bHoriz )
	{
		CCPoint		aVertexArr [ ] = 
		{
			tPoint1,
			tPoint2,
			ccp ( tPoint2.x, tPoint2.y + fHeight ), 
			ccp ( tPoint1.x, tPoint1.y + fHeight ) 
		};

		for ( KDint i = 0; i < nNumVerts; i++ )
		{		
			pVertexArray->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x, aVertexArr [ i ].y ) ) );
		}
	}
	else
	{
		CCPoint		aVertexArr [ ] = 
		{
			ccp ( tPoint2.x - fDepth / 2, tPoint2.y ),
			ccp ( tPoint2.x + fDepth / 2, tPoint2.y ),
			ccp ( tPoint2.x + fDepth / 2, tPoint2.y + fHeight ), 
			ccp ( tPoint2.x - fDepth / 2, tPoint2.y + fHeight ) 
		};

		for ( KDint i = 0; i < nNumVerts; i++ )
		{		
			pVertexArray->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x, aVertexArr [ i ].y ) ) );
		}
	}

	TexturedPolygon*	pWallSide = TexturedPolygon::create ( "bricks.png", pVertexArray );
	pWallSide->getTexture ( )->setTexParameters ( &tParams );
	pWallSide->setPosition ( ccp ( 0, 0 ) );
	pSprite->addChild ( pWallSide, 1, 0 );	


	// Wall Shadow
	pVertexArray = CCArray::create ( );
	
	if ( bHoriz )
	{
		CCPoint		tShadowMod = ccp ( -fHeight / 4, fHeight / 4 );
		
		CCPoint		aVertexArr [ ] = 
		{
			tPoint1,
			tPoint2,
			ccp ( tPoint2.x + tShadowMod.x, tPoint2.y + fHeight + tShadowMod.y ), 
			ccp ( tPoint1.x + tShadowMod.x, tPoint1.y + fHeight + tShadowMod.y ) 
		};

		for ( KDint i = 0; i < nNumVerts; i++ )
		{		
			pVertexArray->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x, aVertexArr [ i ].y ) ) );
		}
	}
	else
	{	
		CCPoint		tShadowMod = ccp ( -fHeight / 4, fHeight );
		nNumVerts = 5;

		CCPoint		aVertexArr [ ] = 
		{
			ccp ( tPoint1.x - fDepth / 2 + tShadowMod.x, tPoint1.y + tShadowMod.y ),
			ccp ( tPoint1.x + tShadowMod.x, tPoint1.y + tShadowMod.y ),
			ccp ( tPoint1.x - fDepth / 2, tPoint1.y ), 
			ccp ( tPoint2.x - fDepth / 2, tPoint2.y ), 
			ccp ( tPoint2.x - fDepth / 2 + tShadowMod.x, tPoint2.y + tShadowMod.y )
		};

		for ( KDint i = 0; i < nNumVerts; i++ )
		{		
			pVertexArray->addObject ( CCPointValue::create ( ccp ( aVertexArr [ i ].x, aVertexArr [ i ].y ) ) );
		}
	}

	TexturedPolygon*	pShadow = TexturedPolygon::create ( "shadow_texture.png", pVertexArray );
	pShadow->getTexture ( )->setTexParameters ( &tParams );
	pShadow->setPosition ( ccp ( 0, 0 ) );
	pWall->setSpriteShadow ( pShadow );
	m_pGameNode->addChild ( pShadow, 1, 0 );	

	// Finally, add the simple shape
	b2PolygonShape*		pShape = new b2PolygonShape ( );
	pWall->setPolygonShape ( pShape ); 
	pShape->SetAsEdge 
	(
		b2Vec2 ( tPoint1.x / PTM_RATIO, tPoint1.y / PTM_RATIO / PERSPECTIVE_RATIO ),
		b2Vec2 ( tPoint2.x / PTM_RATIO, tPoint2.y / PTM_RATIO / PERSPECTIVE_RATIO )
	);

	// Wall properties
	b2FixtureDef*	pFixtureDef = pWall->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = 2.0f;
	pFixtureDef->friction = 0.0f;
	pFixtureDef->restitution = 0.0f;

	// Create the fixture
	pBody->CreateFixture ( pFixtureDef );
}

/// Add a tree object 
KDvoid Ch4_TopDownIsometric::addTreeAtPoint ( const CCPoint& tTreePosition )
{
	GameIsoObject*	pTree = GameIsoObject::create ( );
	pTree->setGameArea ( this );
	pTree->setTypeTag ( TYPE_OBJ_TREE );

	b2BodyDef*		pBodyDef = pTree->getBodyDef ( );
	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( tTreePosition.x / PTM_RATIO, tTreePosition.y / PTM_RATIO / PERSPECTIVE_RATIO );	

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pTree->setBody ( pBody );

	CCPoint			tTextureSize = ccp ( 156, 183 );
	CCPoint			tShapeSize   = ccp (  25,  25 );

	pTree->setModifierY ( 90.0f );
	pTree->setActualImageSize ( tTextureSize.x );
	pTree->setInGameSize ( tTextureSize.x / 2 );	

	pBody->SetZSize ( 100.0f );
	pBody->SetZPosition ( 0 );

	CCSprite*		pSprite = CCSprite::create ( "blank.png" );	
	pTree->setSprite ( pSprite );

	CCSprite*		pTreeSprite = CCSprite::create ( "tree.png" );	
	pTreeSprite->setPosition ( ccp ( -25, 0 ) );
	pSprite->addChild ( pTreeSprite );
	
	m_pGameNode->addChild ( pSprite, 5 );

	b2CircleShape*	pShape =  new b2CircleShape ( );
	pTree->setCircleShape ( pShape );
	pShape->m_radius = tShapeSize.x / PTM_RATIO;
	
	b2FixtureDef*	pFixtureDef = pTree->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = 2.0f;
	pFixtureDef->friction = 0.0f;
	pFixtureDef->restitution = 0.0f;

	pBody->CreateFixture ( pFixtureDef );
	
	// Tree shadow
	CCSprite*		pShadow = CCSprite::create ( "blank.png" );
	pTree->setSpriteShadow ( pShadow );
	m_pGameNode->addChild ( pShadow, 4 );

	CCSprite*		pTreeShadow = CCSprite::create ( "tree_shadow.png" );
	pTreeShadow->setPosition ( ccp ( -90.0f, 50.0f ) );
	pShadow->addChild ( pTreeShadow );

	pTree->setTag ( m_pTrees->count ( ) );
	m_pTrees->addObject ( pTree );
}

/// Add a ball with a random size at a position 
GameIsoObject* Ch4_TopDownIsometric::addBallAtPoint ( const CCPoint& tBallPosition )
{
	KDfloat		fScaleMod = ( kdRand ( ) % 5 ) / 5.0f + 0.5f;

	GameIsoObject*	pBall = GameIsoObject::create ( );
	pBall->setGameArea ( this );
	pBall->setTypeTag ( TYPE_OBJ_BALL );

	// Bounce and roll coefficients determine how high the ball boucnes and how fast the ball rolls
	pBall->setBounceCoefficient ( Vector3D::create ( 0.05f, 0.05f, 0.1f * fScaleMod ) );
	pBall->setRollCoefficient ( Vector3D::create ( 0.0005f, 0.0005f, 0.5f ) );

	b2BodyDef*		pBodyDef = pBall->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tBallPosition.x / PTM_RATIO, tBallPosition.y / PTM_RATIO / PERSPECTIVE_RATIO );
	pBodyDef->bullet = KD_TRUE;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pBall->setBody ( pBody );

	CCPoint			tTextureSize = ccp ( 128, 99 );
	CCPoint			tShapeSize   = ccp ( 10 * fScaleMod, 10 * fScaleMod );

	pBall->setModifierY ( 0.0f );
	pBall->setModifierZ ( 0.0f );
	pBall->setActualImageSize ( tTextureSize.x / 2 );
	pBall->setInGameSize ( tShapeSize.x );	

	pBody->SetZSize ( 5.0f );
	pBody->SetZPosition ( kdRand ( ) % 200 * 1.f );

	CCSprite*		pSprite = CCSprite::create ( "blank.png" );	
	pBall->setSprite ( pSprite );

	CCSprite*		pBallSprite = CCSprite::create ( "ball2.png" );	
	pBallSprite->setPosition ( ccp ( 0, 0 ) );
	// Random blal color
	pBallSprite->setColor ( ccc3 ( kdRand ( ) % 255, kdRand ( ) % 255, kdRand ( ) % 255 ) );
	pSprite->addChild ( pBallSprite );
	
	m_pGameNode->addChild ( pSprite, -1 );

	b2CircleShape*	pShape =  new b2CircleShape ( );
	pBall->setCircleShape ( pShape );
	pShape->m_radius = tShapeSize.x / PTM_RATIO;
	
	b2FixtureDef*	pFixtureDef = pBall->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 0.0f;
	pFixtureDef->restitution = 0.95f;

	pBody->CreateFixture ( pFixtureDef );
	
	// Ball shadow
	CCSprite*		pShadow = CCSprite::create ( "actor_shadow.png" );
	pBall->setSpriteShadow ( pShadow );
	pShadow->setPosition ( ccp ( -10000, -10000 ) );
	m_pGameNode->addChild ( pShadow, 2 );

	pBall->setTag ( m_pBalls->count ( ) );
	m_pBalls->addObject ( pBall );

	return pBall;
}