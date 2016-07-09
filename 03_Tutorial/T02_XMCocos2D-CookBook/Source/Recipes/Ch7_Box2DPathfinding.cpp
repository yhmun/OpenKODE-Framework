/* --------------------------------------------------------------------------
 *
 *      File            Ch7_Box2DPathfinding.cpp
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
#include "Ch7_Box2DPathfinding.h"
#include "Libraries/RayCast.h"

KDbool Ch7_Box2DPathfinding::init ( KDvoid )
{	
	// Set game area size
	m_tGameAreaSize = ccp ( 1000 / PTM_RATIO, 600 / PTM_RATIO );		// Box2d units

	// Superclass initialization and message
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Tap the screen move the actor to that point.\nTap again to queue another waypoint." );

	// Add some geometry
	this->addRandomPolygons ( 5 );
	this->addRandomBoxes ( 5 );
	
	// Initial variables
	m_fNodeSpace	= 32.0f;
	m_fCameraZoom	= 0.45f;
	m_fActorRadius	= m_fNodeSpace / PTM_RATIO / 3;
	
	// Add draw layer
	this->addDrawLayer ( );
	
	// Create level boundaries
	this->addLevelBoundaries ( );
	

	// Create 2D array (grid)
	m_nGridSizeX = (KDint) ( m_tGameAreaSize.cx * PTM_RATIO / m_fNodeSpace );
	m_nGridSizeY = (KDint) ( m_tGameAreaSize.cy * PTM_RATIO / m_fNodeSpace );
	m_pGrid = CCArray::createWithCapacity ( m_nGridSizeX );
	m_pGrid->retain ( );

	for ( KDint x = 0; x < m_nGridSizeX; x++ )
	{
		m_pGrid->addObject ( CCArray::createWithCapacity ( m_nGridSizeY ) );
	}	
	
	// Create AStar nodes
	for ( KDint x = 0; x < m_nGridSizeX; x++ )
	{
		for ( KDint y = 0; y < m_nGridSizeY; y++ )
		{
			// Add a node
			AStarNode*	pNode = AStarNode::create ( );
			pNode->setPosition ( ccp ( x * m_fNodeSpace + m_fNodeSpace / 2, y * m_fNodeSpace + m_fNodeSpace / 2 ) );
			( (CCArray*) m_pGrid->objectAtIndex ( x ) )->addObject ( pNode );
		}
	}

	// Add neighbors
	for ( KDint x = 0; x < m_nGridSizeX; x++ )
	{
		for ( KDint y = 0; y < m_nGridSizeY; y++ )
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
				
	// Remove neighbors from positive TestPoint and RayCast tests	
	for ( KDint x = 0; x < m_nGridSizeX; x++ )
	{
		for ( KDint y = 0; y < m_nGridSizeY; y++ )
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
	
	// Create Actor
	this->addActor ( );

	return KD_TRUE;
}

KDvoid Ch7_Box2DPathfinding::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pGrid );

	GameArea2D::onExit ( );
}

KDvoid Ch7_Box2DPathfinding::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

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

/// Draw layer 
KDvoid Ch7_Box2DPathfinding::drawLayer ( KDvoid )
{
	// Draw AStarNodes
	ccDrawColor4B ( 128, 128, 128, 64 );

	for ( KDint x = 0; x < m_nGridSizeX; x++ )
	{
		for ( KDint y = 0; y < m_nGridSizeY; y++ )
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

/// Find a path and add it (as a set of waypoints) when we tap the screen 
KDvoid Ch7_Box2DPathfinding::tapWithPoint ( const CCPoint& tPoint )
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

	// Starting node
	AStarNode*	pStartNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) ( tActorPosition.x / m_fNodeSpace ) ) )->objectAtIndex ( (KDint) ( tActorPosition.y / m_fNodeSpace ) );

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

					KDfloat		fIndexX = ( (KDint) ( tActorPosition.x / m_fNodeSpace ) ) + x;
					KDfloat		fIndexY = ( (KDint) ( tActorPosition.y / m_fNodeSpace ) ) + y;
					
					if ( fIndexX >= 0 && fIndexY >= 0 && fIndexX < m_nGridSizeX && fIndexY < m_nGridSizeY )
					{						
						AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) fIndexX ) )->objectAtIndex ( (KDint) fIndexY );						
						if ( pNode->getNeighbors ( )->count ( ) > 0 )
						{
							pStartNode = pNode;
							bFound = KD_FALSE;
							break;	break;
						}						
					}					
				}
			}
			n += 1;
		}	
	}
	
	// End node
	AStarNode*	pEndNode =(AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) ( tEndPoint.x / m_fNodeSpace ) ) )->objectAtIndex ( (KDint) ( tEndPoint.y / m_fNodeSpace ) );		

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

/// Add neighbor helper method 
KDvoid Ch7_Box2DPathfinding::addNeighbor ( AStarNode* pNode, KDint x, KDint y )
{
	CCArray*	pNeighbors = pNode->getNeighbors ( );
	if ( x >= 0 && y >= 0 && x < m_nGridSizeX && y < m_nGridSizeY )
	{
		AStarNode*		pNeighbor = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );  
		if ( !AStarNode::isNode ( pNeighbor, pNeighbors ) )
		{
			pNeighbors->addObject ( pNeighbor );
		}
	}	
}

/// Add actor 
KDvoid Ch7_Box2DPathfinding::addActor ( KDvoid )
{
	m_pActor = GameActor::create ( );
	m_pActor->setGameArea ( this );
	
	b2BodyDef*		pBodyDef = m_pActor->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( 240 / PTM_RATIO, 160 / PTM_RATIO );
	pBodyDef->userData = m_pActor;
		
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pActor->setBody ( pBody );
	
	b2CircleShape*	pShape = new b2CircleShape ( );
	m_pActor->setCircleShape ( pShape );
	pShape->m_radius = m_fActorRadius;

	b2FixtureDef*	pFixtureDef = m_pActor->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->isSensor = KD_FALSE;
	
	pBody->CreateFixture ( pFixtureDef );
}

/// Add draw layer 
KDvoid Ch7_Box2DPathfinding::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGameNode->addChild ( m_pDrawLayer, 10 );
}
