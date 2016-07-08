/* --------------------------------------------------------------------------
 *
 *      File            GameArea2D.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "GameArea2D.h"
#include "ObjectCallback.h"
#include "QueuedAction.h"
#include "BasicContactFilter.h"
#include "BasicContactListener.h"
#include "GameMisc.h"

#define TAP_VS_DRAG_DIST	30.0f

enum 
{
	GA_CAMERA_STATE_NONE	= 0,
	GA_CAMERA_STATE_PANNING = 1,
	GA_CAMERA_STATE_ZOOMING = 2
};

GameArea2D::GameArea2D ( KDvoid )
{
	m_pWorld						= KD_NULL;
	m_pDebugDraw					= KD_NULL;
	m_bDebugDraw					= KD_FALSE;
	m_pGameNode						= KD_NULL;

	m_pBodiesToDestroy				= new CCArray ( );
	m_pPostDestructionCallbacks		= new CCArray ( );
	m_pBodiesToCreate				= new CCArray ( );
	m_pQueuedActions				= new CCArray ( );

	m_fCameraZoom					= 1.0f;
	m_fCameraZoomTo					= 0.0f;
	m_fCameraZoomSpeed				= 0.0f;
	m_fLastMultiTouchZoomDistance	= 0.0f;
	m_tDraggedToPoint				= ccpz;
	m_tLastTouchedPoint				= ccpz;
	m_nCameraState					= GA_CAMERA_STATE_NONE;
	m_pAllTouches					= new CCDictionary ( );
}

/// Destroy the world upon exit 
GameArea2D::~GameArea2D ( KDvoid )
{
	CC_SAFE_DELETE ( m_pWorld );
	CC_SAFE_DELETE ( m_pDebugDraw );

	CC_SAFE_DELETE ( m_pBodiesToDestroy );
	CC_SAFE_DELETE ( m_pPostDestructionCallbacks );
	CC_SAFE_DELETE ( m_pBodiesToCreate );
	CC_SAFE_DELETE ( m_pQueuedActions );

	CC_SAFE_DELETE ( m_pAllTouches );
}

KDbool GameArea2D::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "" );

	CCSize		tSize;
	if ( m_tGameAreaSize.cx != 0 )
	{
		tSize = this->getContentSize ( );
	}

	// The gameNode is main node we'll attach everything to
	m_pGameNode = CCNode::create ( );
	m_pGameNode->setPosition ( ccpMid ( tSize ) );
	this->addChild ( m_pGameNode, 1 );

	m_pGameNode->setContentSize ( ccs ( m_tGameAreaSize.cx * PTM_RATIO, m_tGameAreaSize.cy * PTM_RATIO ) );	// Cocos2d units
	m_pGameNode->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	
	b2Vec2		tGravity;
	if ( m_tGameAreaSize.cx == 0 )
	{
		// Set Y gravity to -20.0f
		tGravity.Set ( 0.0f, -20.0f );
	}
	else
	{
		tGravity.Set ( 0.0f, 0.0f );
	}

	// Allow objects to sleep to save cycles
	KDbool		bDoSleep = KD_TRUE;
	m_pWorld = new b2World ( tGravity );
	m_pWorld->SetAllowSleeping ( bDoSleep );
	m_pWorld->SetContinuousPhysics ( KD_TRUE );

	// Set contact listener
	m_pWorld->SetContactFilter ( new BasicContactFilter );
	m_pWorld->SetContactListener ( new BasicContactListener );

	// Set up debug drawing routine
	m_pDebugDraw = new CCBox2DDebugDraw ( PTM_RATIO );
	m_pWorld->SetDebugDraw ( m_pDebugDraw );

	// Initialize then hide debug drawing
	this->initDebugDraw ( );

	if ( m_tGameAreaSize.cx == 0 )
	{
		this->hideDebugDraw ( );
	
		// Add button to hide/show debug drawing
		this->addDebugButton ( );
	}
	else
	{
		this->showMinimalDebugDraw ( );
	}
	
	// Schedule our every tick method call
	this->schedule ( schedule_selector ( GameArea2D::step ) );

	return KD_TRUE;
}

KDvoid GameArea2D::step ( KDfloat fDelta )
{
	// Update Physics
	KDint32			nVelocityIterations = 8;
	KDint32			nPositionIterations = 3;
	
	m_pWorld->Step ( fDelta, nVelocityIterations, nPositionIterations );
	
	// Set sprite positions by body positions
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody != KD_NULL; pBody = pBody->GetNext ( ) )
	{
		if ( pBody->GetUserData ( ) != KD_NULL )
		{
			GameObject*		pObject = (GameObject*) pBody->GetUserData ( );
			CCSprite*		pSprite = pObject->getSprite ( );

			if ( pSprite )
			{
				pSprite->setPosition ( ccp ( pBody->GetPosition ( ).x * PTM_RATIO, pBody->GetPosition ( ).y * PTM_RATIO ) );
				if ( m_tGameAreaSize.cx == 0 )
				{
					pSprite->setRotation ( -1 * CC_RADIANS_TO_DEGREES ( pBody->GetAngle ( ) ) );
				}
			}
		}	
	}

	// Process body destruction/creation
	this->destroyBodies		( );
	this->createBodies		( );
	this->runQueuedActions	( );

	// Process camera 
	//if ( m_tGameAreaSize.cx != 0 )
	{
		this->checkCameraBounds ( );
		this->processZoomStep	( );
	}
}

/// This is called from 'basicContactListener'. It will need to be overridden. 
KDvoid GameArea2D::handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
{
	/// ABSTRACT 
}

KDvoid GameArea2D::handleZMissWithObjA ( GameObject* pObjectA, GameObject* pObjectB )
{
	/// ABSTRACT 
}

/// When we show debug draw we add a number of flags to show specific information 
KDvoid GameArea2D::showDebugDraw ( KDvoid )
{
	m_bDebugDraw = KD_TRUE;

	KDuint32	uFlags = 0;

	uFlags += b2Draw::e_shapeBit;
	uFlags += b2Draw::e_jointBit;
	uFlags += b2Draw::e_aabbBit;
	uFlags += b2Draw::e_pairBit;
	uFlags += b2Draw::e_centerOfMassBit;

	m_pDebugDraw->SetFlags ( uFlags );
}

/// Minimal debug drawing only shows the shapeBit flag 
KDvoid GameArea2D::showMinimalDebugDraw ( KDvoid )
{
	m_bDebugDraw = KD_TRUE;

	KDuint32	uFlags = 0;
	
	uFlags += b2Draw::e_shapeBit;

	m_pDebugDraw->SetFlags ( uFlags );
}

/// Hide debug drawing 
KDvoid GameArea2D::hideDebugDraw ( KDvoid )
{
	m_bDebugDraw = KD_FALSE;

	KDuint32	uFlags = 0;

	m_pDebugDraw->SetFlags ( uFlags );
}

/// Debug information is drawn over everything 
KDvoid GameArea2D::initDebugDraw ( KDvoid )
{
	m_pDebugDrawNode = DebugDrawNode::createWithWorld ( m_pWorld );
	m_pDebugDrawNode->setPosition ( ccp ( 0, 0 ) );
	m_pGameNode->addChild ( m_pDebugDrawNode, 100000 );
}

/// Swap debug draw callback 
KDvoid GameArea2D::swapDebugDraw ( CCObject* pSender )
{
	if ( m_bDebugDraw )
	{
		this->hideDebugDraw ( );
	}
	else
	{
		this->showDebugDraw ( );
	}
}

KDvoid GameArea2D::addDebugButton ( KDvoid )
{
	CCMenuItemFont*		pSwapDebugDrawMIF  = CCMenuItemFont::create ( "Debug Draw", this, menu_selector ( GameArea2D::swapDebugDraw ) );
	CCMenu*				pSwapDebugDrawMenu = CCMenu::create ( pSwapDebugDrawMIF, KD_NULL );
	pSwapDebugDrawMenu->setPosition ( ccp ( 260, 20 ) );
	this->addChild ( pSwapDebugDrawMenu, 5 );
}

/// Add basic level boundary polygon. This is often overridden 
KDvoid GameArea2D::addLevelBoundaries ( KDvoid )
{	
	CCSize				tSize = m_tGameAreaSize;
	
	if ( tSize.cx == 0 )
	{
		tSize.cx = this->getContentSize ( ).cx / PTM_RATIO;
		tSize.cy = this->getContentSize ( ).cy / PTM_RATIO;
	}

	GameMisc*			pObject = GameMisc::create ( );
	pObject->setGameArea ( this );
	pObject->setTag ( GO_TAG_WALL );
			
	b2BodyDef*			pBodyDef = pObject->getBodyDef ( );
	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( 0, 0 );
	pBodyDef->userData = pObject;

	b2Body*				pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );
	
	b2PolygonShape*		pShape = new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );

	b2FixtureDef*		pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 1.0f;
	pFixtureDef->restitution = 0.0f;

	pShape->SetAsEdge ( b2Vec2 ( 0, 0 ), b2Vec2 ( tSize.cx, 0 ) );
	pBody->CreateFixture ( pFixtureDef );
		
	pShape->SetAsEdge ( b2Vec2 ( 0, tSize.cy ), b2Vec2 ( tSize.cx, tSize.cy ) );
	pBody->CreateFixture ( pFixtureDef );
		
	pShape->SetAsEdge ( b2Vec2 ( 0, tSize.cy ), b2Vec2 ( 0, 0 ) );
	pBody->CreateFixture ( pFixtureDef );
		
	pShape->SetAsEdge (b2Vec2 ( tSize.cx, tSize.cy ), b2Vec2 ( tSize.cx, 0 ) );
	pBody->CreateFixture ( pFixtureDef );
	
	m_pLevelBoundary = pObject;
}

KDvoid GameArea2D::addRandomPolygons ( KDint nNum )
{
	for ( KDint i = 0; i < nNum; i++ )
	{
		KDfloat		x = (KDfloat) ( kdRand ( ) % ( (KDint) m_tGameAreaSize.cx * PTM_RATIO ) );
		KDfloat		y = (KDfloat) ( kdRand ( ) % ( (KDint) m_tGameAreaSize.cy * PTM_RATIO ) );	
		
		this->addPolygonAtPoint ( ccp ( x, y ) );
	}
}

KDvoid GameArea2D::addRandomBoxes ( KDint nNum )
{
	for ( KDint i = 0; i < nNum; i++ )
	{
		KDfloat		x = (KDfloat) ( kdRand ( ) % ( (KDint) m_tGameAreaSize.cx * PTM_RATIO ) );
		KDfloat		y = (KDfloat) ( kdRand ( ) % ( (KDint) m_tGameAreaSize.cy * PTM_RATIO ) );	
		
		this->addBoxAtPoint ( ccp ( x, y ), ccs ( (KDfloat) ( kdRand ( ) % 200 ) + 100.0f, (KDfloat) ( kdRand ( ) % 50 ) + 30.0f ) );
	}
}

/// Adding a polygon 
KDvoid GameArea2D::addPolygonAtPoint ( CCPoint tPoint )
{
	// Random collection of points
	CCArray*	pPoints = CCArray::create ( );
	for ( KDint i = 0; i < ( kdRand ( ) % 5 + 3 ); i++ )
	{
		KDfloat		x = (KDfloat) ( kdRand ( ) % 100 ) + 10;
		KDfloat		y = (KDfloat) ( kdRand ( ) % 100 ) + 10;
		Vector3D*	v = Vector3D::create ( x, y, 0 );
		pPoints->addObject ( v );
	}

	// Convex polygon points
	CCArray*	pConvexPolygon = GameHelper::convexHull ( pPoints );

	// Convex Polygon
	KDfloat		fPolygonSize = 0.05f;
				
	KDint		nNumVerts = pConvexPolygon->count ( );
	b2Vec2*		pVertices;
	pVertices = new b2Vec2 [ nNumVerts ];
	
	CCArray*	pVertexArray = CCArray::create ( );
	
	CCPoint		tMaxSize = ccp ( 0, 0 );
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		Vector3D*	pVert = (Vector3D*) pConvexPolygon->objectAtIndex ( i );
		pVertices [ i ].Set ( pVert->x * fPolygonSize, pVert->y * fPolygonSize );
		pVertexArray->addObject ( CCPointValue::create ( ccp ( pVert->x * PTM_RATIO * fPolygonSize, pVert->y * PTM_RATIO * fPolygonSize ) ) );
		
		// Figure out max polygon size
		if ( tMaxSize.x < pVert->x * fPolygonSize ) { tMaxSize.x = pVert->x * fPolygonSize; }
		if ( tMaxSize.y < pVert->y * fPolygonSize ) { tMaxSize.y = pVert->y * fPolygonSize; }
	}
	
	// Keep polygon in game area
	if ( tPoint.x / PTM_RATIO + tMaxSize.x > m_tGameAreaSize.cx ) { tPoint.x = ( m_tGameAreaSize.cx - tMaxSize.x ) * PTM_RATIO; }
	if ( tPoint.y / PTM_RATIO + tMaxSize.y > m_tGameAreaSize.cy ) { tPoint.y = ( m_tGameAreaSize.cy - tMaxSize.y ) * PTM_RATIO; }
	if ( tPoint.x < 0 ) { tPoint.x = 0; }
	if ( tPoint.y < 0 ) { tPoint.y = 0; }

	GameMisc*		pObject = GameMisc::create ( );
	pObject->setGameArea ( this );
	pObject->setTag ( GO_TAG_WALL );
	
	b2BodyDef*		pBodyDef = pObject->getBodyDef ( );

	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pObject;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );
	
	b2PolygonShape*	pShape = new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );
	pShape->Set ( pVertices, nNumVerts );

	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	
	pBody->CreateFixture ( pFixtureDef );
}

/// Adding a polygon 
KDvoid GameArea2D::addBoxAtPoint ( CCPoint tPoint, const CCSize& tSize )
{
	// Random collection of points
	CCArray*	pPoints = CCArray::create ( );
	KDfloat		x = tSize.cx; 
	KDfloat		y = tSize.cy;
	
	pPoints->addObject ( Vector3D::create ( 0, 0, 0 ) );
	pPoints->addObject ( Vector3D::create ( x, 0, 0 ) );
	pPoints->addObject ( Vector3D::create ( x, y, 0 ) );
	pPoints->addObject ( Vector3D::create ( 0, y, 0 ) );

	KDfloat		fPolygonSize = 0.05f;
				
	KDint		nNumVerts = pPoints->count ( );
	b2Vec2*		pVertices;
	pVertices = new b2Vec2 [ nNumVerts ];
	
	CCArray*	pVertexArray = CCArray::create ( );
	
	CCPoint		tMaxSize = ccp ( 0, 0 );
	for ( KDint i = 0; i < nNumVerts; i++ )
	{
		Vector3D*	pVert = (Vector3D*) pPoints->objectAtIndex ( i );
		pVertices [ i ].Set ( pVert->x * fPolygonSize, pVert->y * fPolygonSize );
		pVertexArray->addObject ( CCPointValue::create ( ccp ( pVert->x * PTM_RATIO * fPolygonSize, pVert->y * PTM_RATIO * fPolygonSize ) ) );
		
		// Figure out max polygon size
		if ( tMaxSize.x < pVert->x * fPolygonSize ) { tMaxSize.x = pVert->x * fPolygonSize; }
		if ( tMaxSize.y < pVert->y * fPolygonSize ) { tMaxSize.y = pVert->y * fPolygonSize; }
	}

	// Keep polygon in game area
	if ( tPoint.x / PTM_RATIO + tMaxSize.x > m_tGameAreaSize.cx ) { tPoint.x = ( m_tGameAreaSize.cx - tMaxSize.x ) * PTM_RATIO; }
	if ( tPoint.y / PTM_RATIO + tMaxSize.y > m_tGameAreaSize.cy ) { tPoint.y = ( m_tGameAreaSize.cy - tMaxSize.y ) * PTM_RATIO; }
	if ( tPoint.x < 0 ) { tPoint.x = 0; }
	if ( tPoint.y < 0 ) { tPoint.y = 0; }

	GameMisc*		pObject = GameMisc::create ( );
	pObject->setGameArea ( this );
	pObject->setTag ( GO_TAG_WALL );
	
	b2BodyDef*		pBodyDef = pObject->getBodyDef ( );

	pBodyDef->type = b2_staticBody;
	pBodyDef->position.Set ( tPoint.x / PTM_RATIO, tPoint.y / PTM_RATIO );
	pBodyDef->userData = pObject;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pObject->setBody ( pBody );
	
	b2PolygonShape*	pShape = new b2PolygonShape ( );
	pObject->setPolygonShape ( pShape );
	pShape->Set ( pVertices, nNumVerts );

	b2FixtureDef*	pFixtureDef = pObject->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->restitution = 0.0f;
	pFixtureDef->friction = 1.0f;
	
	pBody->CreateFixture ( pFixtureDef );
}

/// Mark a body for destruction 
KDvoid GameArea2D::markBodyForDestruction ( GameObject* pObject )
{
	m_pBodiesToDestroy->addObject ( pObject );
}

/// Destroy queued bodies 

//TODO - Can we limit the number of created and destroyed bodies per tick?
//       This might be messed up by the fact that we are using GameObject pointers. If a GameObject has a new body created before its old
//       body is destroyed this creates a zombie body somewhere in memory.
KDvoid GameArea2D::destroyBodies ( KDvoid )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pBodiesToDestroy, pObject )
	{
		GameObject*		pGameObject = (GameObject*) pObject;
		
		if ( pGameObject->getBody ( ) && !pGameObject->isMarkedForDestruction ( ) )
		{
			pGameObject->getBody ( )->SetTransform ( b2Vec2 ( 0, 0 ), 0 );
			m_pWorld->DestroyBody ( pGameObject->getBody ( ) );
			pGameObject->setMarkedForDestruction ( KD_TRUE );
		}
	}

	m_pBodiesToDestroy->removeAllObjects ( );

	// Call all game object callbacks
	CCARRAY_FOREACH ( m_pPostDestructionCallbacks, pObject )
	{
		ObjectCallback*		pCallback = (ObjectCallback*) pObject;
		
		( pCallback->getObject ( )->*pCallback->getCallback ( ) ) ( );
	}

	m_pPostDestructionCallbacks->removeAllObjects ( );
}

/// Mark a body for creation 
KDvoid GameArea2D::markBodyForCreation ( GameObject* pObject )
{
	m_pBodiesToCreate->addObject ( pObject );	
}

/// Create all queued bodies 
KDvoid GameArea2D::createBodies ( KDvoid )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pBodiesToCreate, pObject )
	{
		GameObject*		pGameObject = (GameObject*) pObject;
		
		pGameObject->setBody ( m_pWorld->CreateBody ( pGameObject->getBodyDef ( ) ) );
		pGameObject->getBody ( )->CreateFixture ( pGameObject->getFixtureDef ( ) );
	}

	m_pBodiesToCreate->removeAllObjects ( );
}

/// Run any queued actions after creation/destruction 
KDvoid GameArea2D::runQueuedActions ( KDvoid )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pQueuedActions, pObject )
	{
		QueuedAction*		pQueuedAction = (QueuedAction*) pObject;
		
		pQueuedAction->getGameObject ( )->runAction ( pQueuedAction->getAction ( ) );
	}

	m_pQueuedActions->removeAllObjects ( );
}

/// Drawing 
KDvoid GameArea2D::drawLayer ( KDvoid )
{
	// ABSTRACT 
}

/// A camera bound limiting routine 
KDbool GameArea2D::checkCameraBoundsWithFailPosition ( CCPoint* pFailPosition )
{	
	KDbool		bPassed = KD_TRUE;
	
	// Return true for now
	return bPassed;

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

/// Check camera bounds and reset them if neccessary 
KDbool GameArea2D::checkCameraBounds ( KDvoid )
{
	CCPoint		tFailPosition;
	KDbool		bCameraInBounds = this->checkCameraBoundsWithFailPosition ( &tFailPosition );
	
	if ( !bCameraInBounds )
	{
		// Test failed.
		m_pGameNode->setPosition ( tFailPosition );
	}
	
	return bCameraInBounds;
}

/// Properly set the camera position using 'gameNode' 
KDvoid GameArea2D::setCameraPosition ( const CCPoint& tPosition )
{
	m_pGameNode->setPosition ( this->convertCamera ( tPosition ) );
}

/// Convert a camera position 
CCPoint GameArea2D::convertCamera ( const CCPoint& tPosition )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	CCPoint			tNewPoint = ccp ( tPosition.x * -1, tPosition.y * -1 );
	
	// Standard Camera Adjustment
	tNewPoint = ccp ( tNewPoint.x + ( tLyrSize.cx / 2) + ( m_tGameAreaSize.cx / 2 ), tNewPoint.y + ( tLyrSize.cy / 2 ) + ( m_tGameAreaSize.cy / 2 ) );
	
	// Zoom adjustment
	tNewPoint = ccp ( tNewPoint.x * m_fCameraZoom, tNewPoint.y * m_fCameraZoom );
	
	return tNewPoint;
}

/// Set the camera zoom with limits 
KDvoid GameArea2D::setCameraZoom ( KDfloat fZoom )
{
	if ( fZoom < 0.1f ) { fZoom = 0.1f; }	// Lower limit
	if ( fZoom > 4.0f ) { fZoom = 4.0f; }	// Upper limit
	m_pGameNode->setScale ( fZoom );
	m_fCameraZoom = fZoom;
}

/// Set cameraZoomTo and cameraZoomSpeed 
KDvoid GameArea2D::zoomTo ( KDfloat fZoom, KDfloat fSpeed )
{
	m_fCameraZoomTo = fZoom;
	m_fCameraZoomSpeed = fSpeed;
}

/// Process a zoom step 
KDvoid GameArea2D::processZoomStep ( KDvoid )
{
	KDfloat		fNewZoom = m_fCameraZoom;
	if ( fNewZoom != m_fCameraZoomTo ) 
	{
		if ( fNewZoom < m_fCameraZoomTo )
		{
			fNewZoom += m_fCameraZoomSpeed;
			if ( fNewZoom > m_fCameraZoomTo )
			{
				fNewZoom = m_fCameraZoomTo; 
			}
		}
		else if ( fNewZoom > m_fCameraZoomTo )
		{ 
			fNewZoom -= m_fCameraZoomSpeed;
			if ( fNewZoom < m_fCameraZoomTo )
			{
				fNewZoom = m_fCameraZoomTo; 
			}
		}
		this->setCameraZoom ( fNewZoom );
	}
}

/// Converts a touch coordinate to a game coordinate
CCPoint GameArea2D::convertTouchCoord ( const CCPoint& tTouchPoint )
{
	KDfloat		cz = m_fCameraZoom;
	CCPoint		tp = tTouchPoint;
	CCPoint		ap = m_pGameNode->getAnchorPoint ( );
	CCPoint		cs = m_pGameNode->getContentSize ( );
	CCPoint		ss = this->getContentSize ( );
	
	KDfloat		x = ( ( tp.x - ss.x / 2 ) / cz ) + ( ap.x - 0.5f ) * cs.x + cs.x / 2;
	KDfloat		y = ( ( tp.y - ss.y / 2 ) / cz ) + ( ap.y - 0.5f ) * cs.y + cs.y / 2;
	
	return ccp ( x, y );
}

/// Converts a game coordinate to a touch coordinate
CCPoint GameArea2D::convertGameCoord ( const CCPoint& tGamePoint )
{
	KDfloat		cz = m_fCameraZoom;
	CCPoint		gp = tGamePoint;
	CCPoint		ap = m_pGameNode->getAnchorPoint ( );
	CCPoint		cs = m_pGameNode->getContentSize ( );
	CCPoint		ss = this->getContentSize ( );
	
	KDfloat		x = gp.x * cz - cs.x * ( ap.x - 0.5f ) * cz - cs.x / 2 + ss.x / 2;
	KDfloat		y = gp.y * cz - cs.y * ( ap.y - 0.5f ) * cz - cs.y / 2 + ss.y / 2;

	return ccp ( x, y );
}

KDvoid GameArea2D::centerCameraOnGameCoord ( const CCPoint& tGamePoint )
{
	const CCSize&	tSize = m_pGameNode->getContentSize ( );
	m_pGameNode->setAnchorPoint ( ccp ( tGamePoint.x / tSize.cx, tGamePoint.y / tSize.cy ) );
}

/// Process touches 
KDvoid GameArea2D::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_tGameAreaSize.cx == 0 )
	{
		return;
	}

	KDbool		bHudTouched = KD_FALSE;
	bHudTouched = this->hudBegan ( pTouches, pEvent );

	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		m_pAllTouches->setObject ( pTouch, pTouch->getID ( ) );
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
}

KDvoid GameArea2D::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_tGameAreaSize.cx == 0 )
	{
		return;
	}

	KDbool		bHudTouched = KD_FALSE;
	bHudTouched = this->hudMoved ( pTouches, pEvent );
	
	if ( !bHudTouched )
	{
		if ( pTouches->count ( ) == 1 && m_pAllTouches->count ( ) == 1 )
		{
			CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
			CCPoint		tPoint = pTouch->getLocation ( );	
			
			// Process camera movement
			CCPoint		tLastTouchedDistance = ccp ( tPoint.x - m_tLastTouchedPoint.x, tPoint.y - m_tLastTouchedPoint.y );
			m_tDraggedToPoint = ccp ( m_tDraggedToPoint.x + tLastTouchedDistance.x, m_tDraggedToPoint.y + tLastTouchedDistance.y );
			
			// Should we start panning?
			if ( m_nCameraState == GA_CAMERA_STATE_NONE && this->distanceP1 ( ccp ( 0, 0 ), m_tDraggedToPoint ) > TAP_VS_DRAG_DIST )
			{
				m_nCameraState = GA_CAMERA_STATE_PANNING;
			}
								
			// Pan if we are already panning
			if ( m_nCameraState == GA_CAMERA_STATE_PANNING )
			{
				// Move camera
				const CCSize&	tSize = m_pGameNode->getContentSize ( );
				m_pGameNode->setAnchorPoint 
				(
					ccpSub ( m_pGameNode->getAnchorPoint ( ), ccp ( tLastTouchedDistance.x / m_fCameraZoom / tSize.cx, tLastTouchedDistance.y / m_fCameraZoom / tSize.cy ) )
				); 
			}
			m_tLastTouchedPoint = tPoint;
		}
		else if ( pTouches->count ( ) == 2 )
		{
			CCPoint			aPoints [ 2 ];
			KDint			i = 0;

			for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ )
			{
				CCTouch*	pTouch = (CCTouch*) ( *iter );
				aPoints [ i ] = pTouch->getLocation ( );
			}

			// Process camera zooming
			m_nCameraState = GA_CAMERA_STATE_ZOOMING;

			KDfloat			fCurrentDistance = this->distanceP1 ( aPoints [ 0 ], aPoints [ 1 ] );

			if ( m_fLastMultiTouchZoomDistance == 0 )
			{
				m_fLastMultiTouchZoomDistance = fCurrentDistance;
			}
			else
			{
				KDfloat		fDifference = fCurrentDistance - m_fLastMultiTouchZoomDistance;
				KDfloat		fNewZoom = ( m_fCameraZoom + ( fDifference / this->getContentSize ( ).cy ) * m_fCameraZoom );
				this->setCameraZoom ( fNewZoom );
				m_fLastMultiTouchZoomDistance = fCurrentDistance;
			}
		}		
	}
}

KDvoid GameArea2D::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_tGameAreaSize.cx == 0 )
	{
		return;
	}

	KDbool		bHudTouched = KD_FALSE;
	bHudTouched = this->hudEnded ( pTouches, pEvent );

	// Store all touches
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++ )
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		m_pAllTouches->removeObjectForKey ( pTouch->getID ( ) );
	}

	if ( !bHudTouched )
	{
		CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
		CCPoint		tPoint = pTouch->getLocation ( );
	
		if ( m_nCameraState == GA_CAMERA_STATE_NONE )
		{
			this->tapWithPoint ( tPoint );
			m_nCameraState = GA_CAMERA_STATE_NONE;
		}
		else if ( m_nCameraState == GA_CAMERA_STATE_PANNING )
		{
			m_tLastTouchedPoint = tPoint;
			m_tDraggedToPoint = ccp ( 0, 0 );
			m_nCameraState = GA_CAMERA_STATE_NONE;
		}
		else if ( m_nCameraState == GA_CAMERA_STATE_ZOOMING && m_pAllTouches->count ( ) == 0 )
		{
			m_fLastMultiTouchZoomDistance = 0.0f;
			m_nCameraState = GA_CAMERA_STATE_NONE;
		}
	}
}

KDbool GameArea2D::hudBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	// ABSTRACT 
	return KD_FALSE;
}	

KDbool GameArea2D::hudMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	// ABSTRACT 
	return KD_FALSE;
}

KDbool GameArea2D::hudEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// ABSTRACT 
	return KD_FALSE;
}

KDvoid GameArea2D::tapWithPoint ( const CCPoint& tPoint )
{
	// ABSTRACT
}

KDfloat GameArea2D::distanceP1 ( const CCPoint& tFirstTouch, const CCPoint& tSecondTouch )
{
	return kdSqrtf ( kdPowf ( ( tFirstTouch.x - tSecondTouch.x ), 2 ) + kdPowf ( ( tFirstTouch.y - tSecondTouch.y ), 2 ) );
}