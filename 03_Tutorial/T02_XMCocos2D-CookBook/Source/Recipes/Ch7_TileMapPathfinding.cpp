/* --------------------------------------------------------------------------
 *
 *      File            Ch7_TileMapPathfinding.cpp
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
#include "Ch7_TileMapPathfinding.h"
#include "Libraries/RayCast.h"

KDbool Ch7_TileMapPathfinding::init ( KDvoid )
{	
	// Superclass initialization and message
	if ( !Ch8_Tiled::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Tap the screen move the actor to that point.\nTap again to queue another waypoint." );

	this->addDebugButton ( );

	// Shorter variable names
	KDfloat			mw = m_pTileMap->getMapSize ( ).cx;
	KDfloat			mh = m_pTileMap->getMapSize ( ).cy;
	KDfloat			tw = m_pTileMap->getTileSize ( ).cx;
	KDfloat			th = m_pTileMap->getTileSize ( ).cy;

	m_fActorRadius = m_pActor->getCircleShape ( )->m_radius;

	// Create 2D array (grid)
	m_pGrid = CCArray::createWithCapacity ( (KDint) mw );
	m_pGrid->retain ( );
	for ( KDint x = 0; x < (KDint) mw; x++ )
	{
		m_pGrid->addObject ( CCArray::createWithCapacity ( (KDint) mh ) );
	}
	
	// Create active and inactive nodes determined by the "Collidable" TMX layer
	CCTMXLayer*		pCollidableLayer = m_pTileMap->layerNamed ( "Collidable" );
	for ( KDint x = 0; x < mw; x++ )
	{
		for ( KDint y = 0; y < mh; y++ )
		{
			// Add a node
			AStarNode*		pNode = AStarNode::create ( );
			pNode->setPosition ( ccp ( x * tw + tw / 2, y * th + th / 2 ) );
			if ( pCollidableLayer->tileAt ( ccp ( x, y ) ) )
			{
				pNode->setActive ( KD_FALSE ); 
			}
			( (CCArray*) m_pGrid->objectAtIndex ( x ) )->addObject ( pNode );
		}
	}

	// Add neighbors
	for ( KDint x = 0; x < mw; x++ )
	{
		for ( KDint y = 0; y < mh; y++ )
		{			
			// Add a node
			AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );
			
			// Add self as neighbor to neighboring nodes
			this->addNeighbor ( pNode, x - 1, y - 1 );		// Top-Left
			this->addNeighbor ( pNode, x - 1, y     );		// Left
			this->addNeighbor ( pNode, x - 1, y + 1 );		// Bottom-Left
			this->addNeighbor ( pNode, x    , y - 1 );		// Top
			
			this->addNeighbor ( pNode, x    , y + 1 );		// Bottom
			this->addNeighbor ( pNode, x + 1, y - 1 );		// Top-Right
			this->addNeighbor ( pNode, x + 1, y     );		// Right
			this->addNeighbor ( pNode, x + 1, y + 1 );		// Bottom-Right		
		}
	}

	// Remove neighbors from positive RayCast tests
	for ( KDint x = 0; x < mw; x++ )
	{
		for ( KDint y = 0; y < mh; y++ )
		{	
			// Add a node
			AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );
						
			// If a node itself is colliding with an object we cut off all connections
			for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody; pBody = pBody->GetNext ( ) )
			{
				if ( pBody->GetUserData ( ) != KD_NULL )
				{
					GameObject*		pObject = (GameObject*) pBody->GetUserData ( );
					if ( pObject->getPolygonShape ( ) )
					{
						b2Vec2		tNodePosition = b2Vec2 ( pNode->getPosition ( ).x / PTM_RATIO, pNode->getPosition ( ).y / PTM_RATIO );
						
						// Test this node point against this polygon
						if ( pObject->getPolygonShape ( )->TestPoint ( pBody->GetTransform ( ), tNodePosition ) ) 
						{
							for ( KDuint i = 0; i < pNode->getNeighbors ( )->count ( ); i++ )
							{
								// Remove connections
								AStarNode*	pNeighbor = (AStarNode*) pNode->getNeighbors ( )->objectAtIndex ( i );
								pNode->getNeighbors ( )->removeObject ( pNeighbor );
								pNeighbor->getNeighbors ( )->removeObject ( pNode );
							}
						}
					}
				}	
			}	

			// Test all node to neighbor connections using a RayCast test
			for ( KDuint i = 0; i < pNode->getNeighbors ( )->count ( ); i++ )
			{
				AStarNode*	pNeighbor = (AStarNode*) pNode->getNeighbors ( )->objectAtIndex ( i );
				
				// Do a RayCast from the node to the neighbor.
				// If there is something in the way, remove the link
				b2Vec2		tNodeP = b2Vec2 ( pNode->getPosition ( ).x / PTM_RATIO, pNode->getPosition ( ).y / PTM_RATIO );
				b2Vec2		tNeighborP = b2Vec2 ( pNeighbor->getPosition ( ).x/PTM_RATIO, pNeighbor->getPosition ( ).y / PTM_RATIO );
				
				// Do 4 tests (based on actor size)
				for ( KDfloat x = -m_fActorRadius; x <= m_fActorRadius; x += m_fActorRadius * 2 )
				{
					for ( KDfloat y = -m_fActorRadius; y <= m_fActorRadius; y += m_fActorRadius * 2 )
					{						
						RayCastAnyCallback	tCallback;
						m_pWorld->RayCast ( &tCallback, b2Vec2 ( tNodeP.x + x, tNodeP.y + y ), b2Vec2 ( tNeighborP.x + x, tNeighborP.y + y ) );
								
						if ( tCallback.m_bHit )
						{
							// Remove connections
							pNode->getNeighbors ( )->removeObject ( pNeighbor );
							pNeighbor->getNeighbors ( )->removeObject ( pNode );
							break; 
						}					
					}
				}				
			}	
		}
	}

	// Initial variables
	m_pDPad->setVisible ( KD_FALSE );
	m_fCameraZoom = 0.75f;
	m_pActor->setRunSpeed ( 5.0f );

	// Add draw layer
	this->addDrawLayer ( );
	
	return KD_TRUE;
}

KDvoid Ch7_TileMapPathfinding::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pGrid );

	Ch8_Tiled::onExit ( );
}

KDvoid Ch7_TileMapPathfinding::step ( KDfloat fDelta )
{
	// DPad disabled
	m_pDPad->setPressed ( KD_FALSE );

	Ch8_Tiled::step ( fDelta );

	// Process actor waypoints
	m_pActor->processWaypoints ( );
	
	// Turn actor to face the proper direction
	b2Body*			pActorBody = m_pActor->getBody ( );
	if ( m_pActor->getWaypoints ( )->count ( ) > 0 )
	{
		CCPoint		tMovementVector = ccp ( pActorBody->GetLinearVelocity ( ).x, pActorBody->GetLinearVelocity ( ).y );
		pActorBody->SetTransform ( pActorBody->GetPosition ( ), -1 * GameHelper::vectorToRadians ( tMovementVector ) + PI_CONSTANT / 2 );
	}
}

/// DPad disabled 
KDbool Ch7_TileMapPathfinding::hudBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	return KD_FALSE;
}

KDbool Ch7_TileMapPathfinding::hudMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	return KD_FALSE;
}

KDbool Ch7_TileMapPathfinding::hudEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	return KD_FALSE;
}

/// Find a path and add it (as a set of waypoints) when we tap the screen 
KDvoid Ch7_TileMapPathfinding::tapWithPoint ( const CCPoint& tPoint )
{
	// Convert touch coordinate to physical coordinate
	CCPoint		tEndPoint = this->convertTouchCoord ( tPoint );
	if ( tEndPoint.x < 0 || tEndPoint.y < 0 || tEndPoint.x >= m_tGameAreaSize.cx * PTM_RATIO || tEndPoint.y >= m_tGameAreaSize.cy * PTM_RATIO )
	{
		return;
	}

	// Actor position
	CCPoint		tActorPosition = ccp ( m_pActor->getBody ( )->GetPosition ( ).x * PTM_RATIO, m_pActor->getBody ( )->GetPosition ( ).y * PTM_RATIO );

	// We use the last waypoint position if applicable
	if ( m_pActor->getWaypoints ( )->count ( ) > 0 )
	{
		tActorPosition = ( (GameWaypoint*) m_pActor->getWaypoints ( )->objectAtIndex ( m_pActor->getWaypoints ( )->count ( ) - 1 ) )->getPosition ( );
	}
	
	KDfloat		mw = m_pTileMap->getMapSize ( ).cx;
	KDfloat		mh = m_pTileMap->getMapSize ( ).cy;
	KDfloat		tw = m_pTileMap->getTileSize ( ).cx;
	KDfloat		th = m_pTileMap->getTileSize ( ).cy;
	
	// Starting node
	AStarNode*	pStartNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) ( tActorPosition.x / tw ) ) )->objectAtIndex ( (KDint) ( tActorPosition.y / th ) );
	
	// Make sure the start node is actually properly connected
	if ( pStartNode->getNeighbors ( )->count ( ) == 0 )
	{
		KDbool		bFound = KD_FALSE;
		KDfloat		n = 1;
		while ( !bFound )
		{
			// Search the nodes around this point for a properly connected starting node
			for ( KDfloat x = -n; x <= n; x += n )
			{
				for ( KDfloat y = -n; y <= n; y += n )
				{					
					if ( x == 0 && y == 0 )
					{
						continue; 
					}

					KDfloat		fIndexX = ( (KDint) ( tActorPosition.x / tw ) ) + x;
					KDfloat		fIndexY = ( (KDint) ( tActorPosition.y / th ) ) + y;
					
					if ( fIndexX >= 0 && fIndexY >= 0 && fIndexX < mw && fIndexY < mh )
					{						
						AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) fIndexX ) )->objectAtIndex ( (KDint) fIndexY );						
						if ( pNode->getNeighbors ( )->count ( ) > 0 )
						{
							pStartNode = pNode;
							bFound = KD_FALSE;
							break; break;
						}						
					}					
				}
			}
			n += 1;
		}	
	}
	
	// End node
	AStarNode*	pEndNode =(AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) ( tEndPoint.x / tw ) ) )->objectAtIndex ( (KDint) ( tEndPoint.y / th ) );		

	// Run the pathfinding algorithm
	CCArray*	pFoundPath = AStarPathNode::findPathFrom ( pStartNode, pEndNode );

	if ( !pFoundPath )
	{
		this->showMessage ( "No Path Found" );
	}
	else
	{
		this->showMessage ( "Found Path" );
		// Add found path as a waypoint set to the actor
		for ( KDint i = pFoundPath->count ( ) - 1; i >= 0; i-- )
		{
			CCPoint		tPathPoint = ( (CCPointValue*) pFoundPath->objectAtIndex ( i ) )->getValue ( );
			m_pActor->addWaypoint ( GameWaypoint::create ( tPathPoint, 1.0f ) ); 
		}
	}
}

/// Enable camera zooming 
KDvoid Ch7_TileMapPathfinding::setCameraZoom ( KDfloat fZoom )
{
	if ( fZoom < 0.1f ) { fZoom = 0.1f; }	// Lower limit
	if ( fZoom > 4.0f ) { fZoom = 4.0f; }	// Upper limit

	m_pGameNode->setScale ( fZoom );
	m_fCameraZoom = fZoom;
}

/// Disable camera following 
KDvoid Ch7_TileMapPathfinding::cameraFollowActor ( KDvoid )
{
	// Do nothing
}

/// Draw layer 
KDvoid Ch7_TileMapPathfinding::drawLayer ( KDvoid )
{
	if ( m_bDebugDraw )
	{
		// Draw AStarNodes
		ccDrawColor4B ( 32, 32, 32, 32 );

		KDfloat		mw = m_pTileMap->getMapSize ( ).cx;
		KDfloat		mh = m_pTileMap->getMapSize ( ).cy;

		for ( KDint x = 0; x < mw; x++ )	
		{
			for ( KDint y = 0; y < mh; y++ )
			{		
				// Draw node
				AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );			
				ccDrawPoint ( pNode->getPosition ( ) );
				
				// Draw neighbor lines (there is going to be a lot of them)
				for ( KDuint i = 0; i < pNode->getNeighbors ( )->count ( ); i++ )
				{
					AStarNode*	pNeighbor = (AStarNode*) pNode->getNeighbors ( )->objectAtIndex ( i );
					
					ccDrawLine ( pNode->getPosition ( ), pNeighbor->getPosition ( ) );
				}
			}
		}	
	}

	// Draw waypoints
	ccDrawColor4B ( 255, 255, 0, 128 );	

	b2Vec2		tPosition = m_pActor->getBody ( )->GetPosition ( );
	CCPoint		tActorPosition = ccp ( tPosition.x * PTM_RATIO, tPosition.y * PTM_RATIO );
		
	CCArray*	pWaypoints = m_pActor->getWaypoints ( );

	if ( pWaypoints->count ( ) == 1 )
	{
		GameWaypoint*	pWaypoint = (GameWaypoint*) pWaypoints->objectAtIndex ( 0 );
		ccDrawLine ( tActorPosition, pWaypoint->getPosition ( ) );
	}
	else if ( pWaypoints->count ( ) > 1 )
	{
		for ( KDuint i = 0; i < pWaypoints->count ( ) - 1; i++ )
		{			
			GameWaypoint*	pWaypoint	  = (GameWaypoint*) pWaypoints->objectAtIndex ( i );
			GameWaypoint*	pWaypointNext = (GameWaypoint*) pWaypoints->objectAtIndex ( i + 1 );
			
			if ( i == 0 )
			{
				// From actor to first waypoint
				ccDrawLine ( tActorPosition, pWaypoint->getPosition ( ) );
				ccDrawLine ( pWaypoint->getPosition ( ), pWaypointNext->getPosition ( ) );
			}
			else
			{
				// From this waypoint to next one
				ccDrawLine ( pWaypoint->getPosition ( ), pWaypointNext->getPosition ( ) );
			}	
		}
	}
}

/// Add neighbor helper method 
KDvoid Ch7_TileMapPathfinding::addNeighbor ( AStarNode* pNode, KDint x, KDint y )
{
	KDfloat		mw = m_pTileMap->getMapSize ( ).cx;
	KDfloat		mh = m_pTileMap->getMapSize ( ).cy;

	if ( x >= 0 && y >= 0 && x < mw && y < mh )
	{
		AStarNode*		pNeighbor = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );
		pNode->getNeighbors ( )->addObject ( pNeighbor );
	}
}

/// Add draw layer 
KDvoid Ch7_TileMapPathfinding::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGameNode->addChild ( m_pDrawLayer, 10000 );
}