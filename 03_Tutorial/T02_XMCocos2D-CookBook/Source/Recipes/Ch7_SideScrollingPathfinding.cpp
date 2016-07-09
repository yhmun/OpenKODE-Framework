/* --------------------------------------------------------------------------
 *
 *      File            Ch7_SideScrollingPathfinding.cpp
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
#include "Ch7_SideScrollingPathfinding.h"

KDbool Ch7_SideScrollingPathfinding::init ( KDvoid )
{	
	// Set game area size
	m_tGameAreaSize = ccp ( 1000 / PTM_RATIO, 600 / PTM_RATIO );		// Box2d units

	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}
	m_pMessage->setString ( "Tap the screen move the actor to that point.\nTap again to queue another waypoint." );

	// Set 2D side scrolling gravity
	m_pWorld->SetGravity ( b2Vec2 ( 0, -20.0f ) );
	
	// Add three platforms 
	this->addBoxAtPoint ( ccp ( 625, 200 ), ccs ( 200, 50 ) );
	this->addBoxAtPoint ( ccp (  75, 300 ), ccs ( 230, 50 ) );
	this->addBoxAtPoint ( ccp ( 125, 100 ), ccp ( 220, 50 ) );

	// Create Actor
	this->addActor ( );
	
	// Add draw layer
	this->addDrawLayer ( );
	
	// Create level boundaries
	this->addLevelBoundaries ( );
	
	// Initial variables
	m_fCameraZoom = 0.45f;
	
	// Create SSAStarNodes 
	m_pNodes = new CCArray ( );
	
	// distance between nodes that the actor can run between
	KDfloat		fNodeRunDistInterval = 100.0f;
	
	// How far to search for nodes the actor can jump to
	KDfloat		fMaxJumpSearchDist = 500.0f;
	
	// Add some nodes to the bottom of the level
	for ( KDfloat x = 20.0f; x <= m_tGameAreaSize.cx * PTM_RATIO - 20.0f; x += fNodeRunDistInterval )
	{
		SSAStarNode*	pNode = SSAStarNode::create ( );
		pNode->setPosition ( ccp ( x, 20.0f ) );
		pNode->setBody ( m_pLevelBoundary->getBody ( ) );
		pNode->setActor ( m_pActor );
		m_pNodes->addObject ( pNode );
	}
	
	// Link those nodes together as 'run neighbors'
	for ( KDuint i = 0; i < m_pNodes->count ( ) - 1; i++ )
	{
		SSAStarNode*	pN1 = (SSAStarNode*) m_pNodes->objectAtIndex ( i );
		SSAStarNode*	pN2 = (SSAStarNode*) m_pNodes->objectAtIndex ( i + 1 );
		
		this->linkRunNeighbor ( pN1, pN2 );
	}
	
	// Add nodes to all level platforms 
	for ( b2Body* pBody = m_pWorld->GetBodyList ( ); pBody; pBody = pBody->GetNext ( ) )
	{		
		if ( pBody->GetUserData ( ) != KD_NULL ) 
		{			
			GameObject*		pObject = (GameObject*) pBody->GetUserData ( );
			
			if ( pObject->getTag ( ) == GO_TAG_WALL && pObject->getPolygonShape ( ) )
			{				
				// Nodes on this body only
				CCArray*	pNodesThisBody = CCArray::create ( );
				
				// Process each polygon vertex
				b2PolygonShape*		pShape = pObject->getPolygonShape ( );
				for ( KDint i = 0; i < pShape->m_vertexCount; i++ )
				{					
					b2Vec2		tVertex = pShape->m_vertices [ i ];
					
					// All nodes are 1 unit above their corresponding platform
					b2Vec2		tNodePosition = b2Vec2 ( tVertex.x + pBody->GetPosition ( ).x, tVertex.y + pBody->GetPosition ( ).y + 1.0f );
					
					// Move nodes inward to lessen chance of missing a jump
					if ( pShape->m_centroid.x < tVertex.x )
					{
						tNodePosition = b2Vec2 ( tNodePosition.x - 0.5f, tNodePosition.y );
					}
					else
					{
						tNodePosition = b2Vec2 ( tNodePosition.x + 0.5f, tNodePosition.y );
					}
			
					// If this node position is not inside the polygon we create an SSAStarNode
					if ( !pShape->TestPoint ( pBody->GetTransform ( ), tNodePosition ) ) 
					{			
						SSAStarNode*	pNode = SSAStarNode::create ( );
						pNode->setPosition ( ccp ( tNodePosition.x * PTM_RATIO, tNodePosition.y * PTM_RATIO ) );
						pNode->setBody ( pBody );
						pNode->setActor ( m_pActor );
						m_pNodes->addObject ( pNode );
						pNodesThisBody->addObject ( pNode );
					}					
				}				
				
				// Add in-between nodes (for running)
				KDbool	bDone = KD_FALSE;
				while ( !bDone )
				{
					if ( pNodesThisBody->count ( ) == 0 )
					{
						break; 
					}
				
					bDone = KD_TRUE;
					for ( KDuint i = 0; i < pNodesThisBody->count ( ) - 1; i++ )
					{					
						SSAStarNode*	pNode1 = (SSAStarNode*) pNodesThisBody->objectAtIndex ( i );
						SSAStarNode*	pNode2 = (SSAStarNode*) pNodesThisBody->objectAtIndex ( i + 1 );
							
						if ( GameHelper::absoluteValue ( pNode1->getPosition ( ).y - pNode2->getPosition ( ).y ) > 0.1f )
						{
							// These are not side by side
							continue;
						}
						
						if ( GameHelper::distanceP1 ( pNode1->getPosition ( ), pNode2->getPosition ( ) ) > fNodeRunDistInterval )
						{
							
							CCPoint		tMidPoint = GameHelper::midPointP1 ( pNode1->getPosition ( ), pNode2->getPosition ( ) );
							b2Vec2		tMp = b2Vec2 ( tMidPoint.x / PTM_RATIO, tMidPoint.y / PTM_RATIO );
							
							//If node is not in the polygon, add it
							if ( !pShape->TestPoint ( pBody->GetTransform ( ), tMp ) )
							{
								SSAStarNode*	pNode = SSAStarNode::create ( );
								pNode->setPosition ( GameHelper::midPointP1 ( pNode1->getPosition ( ), pNode2->getPosition ( ) ) );
								pNode->setBody ( pBody );
								pNode->setActor ( m_pActor );
								pNodesThisBody->insertObject ( pNode, i + 1 );
								m_pNodes->insertObject ( pNode, m_pNodes->indexOfObject ( pNode2 ) );
								bDone = KD_FALSE;
								break;
							}		
						}						
					}							
				}
								
				// Link all of the neighboring nodes on this body
				for ( KDuint i = 0; i < pNodesThisBody->count ( ) - 1; i++ )
				{
					if ( pNodesThisBody->count ( ) == 0 )
					{
						break; 
					}
				
					SSAStarNode*	pNode1 = (SSAStarNode*) pNodesThisBody->objectAtIndex ( i );
					SSAStarNode*	pNode2 = (SSAStarNode*) pNodesThisBody->objectAtIndex ( i + 1 );
				
					if ( GameHelper::absoluteValue ( pNode1->getPosition ( ).y - pNode2->getPosition ( ) .y ) > 0.1f )
					{
						// These are not side by side
						continue;
					}
					
					// Two-way link
					this->linkRunNeighbor ( pNode1, pNode2 );
				}				
			}			
		}
	}
		
	// Neighbor all other nodes (for jumping)
	for ( KDuint i = 0; i < m_pNodes->count ( ); i++ )
	{
		for ( KDuint j = 0; j < m_pNodes->count ( ); j++ )
		{
			if ( i == j )
			{
				continue; 
			}
		
			SSAStarNode*	pNode1 = (SSAStarNode*) m_pNodes->objectAtIndex ( i );
			SSAStarNode*	pNode2 = (SSAStarNode*) m_pNodes->objectAtIndex ( j );
		
			if ( pNode1->getBody ( ) == pNode2->getBody ( ) )
			{
				continue; 
			}
		
			if ( GameHelper::distanceP1 ( pNode1->getPosition ( ), pNode2->getPosition ( ) ) <= fMaxJumpSearchDist )
			{
				CCPoint		tSrc = ccp ( pNode1->getPosition ( ).x / PTM_RATIO, pNode1->getPosition ( ).y / PTM_RATIO );
				CCPoint		tDst = ccp ( pNode2->getPosition ( ).x / PTM_RATIO, pNode2->getPosition ( ).y / PTM_RATIO );
				
				// Calculate our jump "launch" vector
				Vector3D*	pLaunchVector3D = SSGameActor::canJumpFrom ( tSrc, tDst, m_pActor->getCircleShape ( )->m_radius * 1.5f, m_pWorld, m_pActor->getMaxSpeed ( ) );
				
				if ( pLaunchVector3D )
				{
					// Only neighbor up if a jump can be made
					// 1-way link
					if ( !pNode1->containsNeighborForNode ( pNode2 ) )
					{
						SSNeighborNode*		pNeighbor = SSNeighborNode::create ( );
						pNeighbor->setLaunchVector ( ccp ( pLaunchVector3D->x, pLaunchVector3D->y ) );
						pNeighbor->setNode ( pNode2 );
						pNeighbor->setMoveType ( MOVE_TYPE_JUMP );
						pNeighbor->setCost ( pNode1->costToNeighbor ( pNeighbor ) );
						pNode1->getNeighbors ( )->addObject ( pNeighbor );
					}
				}
			}
		}
	}
	
	return KD_TRUE;
}

KDvoid Ch7_SideScrollingPathfinding::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pNodes );

	GameArea2D::onExit ( );
}

KDvoid Ch7_SideScrollingPathfinding::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	// Process actor waypoints
	m_pActor->processWaypoints ( );
	
	// Rotate actor toward waypoints
	b2Body*		pBody = m_pActor->getBody ( );
	if ( m_pActor->getWaypoints ( )->count ( ) > 0 )
	{
		CCPoint		tMovementVector = ccp ( pBody->GetLinearVelocity ( ).x, pBody->GetLinearVelocity ( ).y );
		pBody->SetTransform ( pBody->GetPosition ( ), -1 * GameHelper::vectorToRadians ( tMovementVector ) + PI_CONSTANT / 2 );
	}
	
	// The actor never has angular velocity (he never rolls)
	pBody->SetAngularVelocity ( 0.0f );
}

/// Draw layer 
KDvoid Ch7_SideScrollingPathfinding::drawLayer ( KDvoid )
{
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

	// Draw AStarNodes
	for ( KDuint i = 0; i < m_pNodes->count ( ); i++ )
	{
		// Draw node
		SSAStarNode*	pNode = (SSAStarNode*) m_pNodes->objectAtIndex ( i );	

		ccDrawColor4B ( 255, 255, 255, 128 );	
		ccDrawPoint ( pNode->getPosition ( ) );
		
		// Draw neighbor lines (there is going to be a lot of them)
		for ( KDuint j = 0; j < pNode->getNeighbors ( )->count ( ); j++ )
		{
			SSNeighborNode*		pNeighbor = (SSNeighborNode*) pNode->getNeighbors ( )->objectAtIndex ( j );
			
			ccDrawColor4B ( 128, 128, 128, 64 );
			ccDrawLine ( pNode->getPosition ( ), pNeighbor->getNode ( )->getPosition ( ) );
		}
		
	}	
}

/// Find a path and add it (as a set of waypoints) when we tap the screen 
KDvoid Ch7_SideScrollingPathfinding::tapWithPoint ( const CCPoint& tPoint )
{	
	// Convert touch coordinate to physical coordinate
	CCPoint		p = this->convertTouchCoord ( tPoint );

	SSAStarNode*	pStartNode;
	SSAStarNode*	pEndNode;
	
	// Find the two closest nodes to the actor position (start) and where you tapped (end)
	b2Body*			pBody = m_pActor->getBody ( );
	CCPoint			tActorPosition = ccp ( pBody->GetPosition ( ).x * PTM_RATIO, pBody->GetPosition ( ).y * PTM_RATIO );
	if ( m_pActor->getWaypoints ( )->count ( ) > 0 )
	{
		SSGameWaypoint*		pWaypoint = (SSGameWaypoint*) m_pActor->getWaypoints ( )->objectAtIndex ( m_pActor->getWaypoints ( )->count ( ) - 1 );
		tActorPosition = pWaypoint->getPosition ( );
	}

	pStartNode = this->nodeClosestToPoint ( tActorPosition );
	pEndNode   = this->nodeClosestToPoint ( p );
		
	// Add the start node as the initial waypoint
	m_pActor->addWaypoint ( SSGameWaypoint::create ( pStartNode->getPosition ( ), MOVE_TYPE_RUN, 1.0f ) );

	// Run the pathfinding algorithm
	CCArray*	pFoundPath = SSAStarPathNode::findPathFrom ( pStartNode, pEndNode );

	if ( !pFoundPath )
	{
		this->showMessage ( "No Path Found" );
	}
	else if ( pFoundPath->count ( ) == 1 )
	{
		this->showMessage ( "No Path Found" );
		SSAStarPathNode*	pNode = (SSAStarPathNode*) pFoundPath->objectAtIndex ( 0 );
		m_pActor->addWaypoint ( pNode->getWaypoint ( ) );
	}
	else
	{
		this->showMessage ( "Found Path" );
		// Add all waypoints on path to actor
		for ( KDint i = pFoundPath->count ( ) - 1; i >= 0; i-- )
		{
			SSAStarPathNode*	pNode = (SSAStarPathNode*) pFoundPath->objectAtIndex ( i );
			m_pActor->addWaypoint ( pNode->getWaypoint ( ) );
			// Add a run waypoint after a jump
			if ( pNode->getWaypoint ( )->getMoveType ( ) == MOVE_TYPE_JUMP )
			{
				SSGameWaypoint*	pWaypoint = SSGameWaypoint::create ( pNode->getWaypoint ( )->getPosition ( ), MOVE_TYPE_RUN, 0.5f );
				m_pActor->addWaypoint ( pWaypoint );
			}
		}
	}
}

KDvoid Ch7_SideScrollingPathfinding::linkRunNeighbor ( SSAStarNode* pNode1, SSAStarNode* pNode2 )
{
	// 2-way link
	if ( !pNode1->containsNeighborForNode ( pNode2 ) )
	{
		SSNeighborNode*		pNeighbor = SSNeighborNode::create ( );
		pNeighbor->setMoveType ( MOVE_TYPE_RUN );
		pNeighbor->setNode ( pNode2 );
		pNeighbor->setCost ( pNode1->costToNeighbor ( pNeighbor ) );
		pNode1->getNeighbors ( )->addObject ( pNeighbor );
	}

	if ( !pNode2->containsNeighborForNode ( pNode1 ) )
	{
		SSNeighborNode*		pNeighbor = SSNeighborNode::create ( );
		pNeighbor->setMoveType ( MOVE_TYPE_RUN );
		pNeighbor->setNode ( pNode1 );
		pNeighbor->setCost ( pNode2->costToNeighbor ( pNeighbor ) );
		pNode2->getNeighbors ( )->addObject ( pNeighbor );
	}
}

/// Add actor 
KDvoid Ch7_SideScrollingPathfinding::addActor ( KDvoid )
{
	m_pActor = SSGameActor::create ( );
	m_pActor->setGameArea ( this );
	
	b2BodyDef*		pBodyDef = m_pActor->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( 240 / PTM_RATIO, 160 / PTM_RATIO );
	pBodyDef->userData = m_pActor;
		
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pActor->setBody ( pBody );
	
	b2CircleShape*	pShape = new b2CircleShape ( );
	m_pActor->setCircleShape ( pShape );
	pShape->m_radius = 32.0f / PTM_RATIO / 3;

	b2FixtureDef*	pFixtureDef = m_pActor->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->friction = 1.0f;
	pFixtureDef->restitution = 0.0f;
	pFixtureDef->isSensor = KD_FALSE;
	
	pBody->CreateFixture ( pFixtureDef );
}

/// Add draw layer 
KDvoid Ch7_SideScrollingPathfinding::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGameNode->addChild ( m_pDrawLayer, 10 );
}

SSAStarNode* Ch7_SideScrollingPathfinding::nodeClosestToPoint ( const CCPoint& tPoint )
{
	SSAStarNode*	pFoundNode = KD_NULL;
	for ( KDuint i = 0; i < m_pNodes->count ( ); i++ )
	{
		SSAStarNode*	pNode = (SSAStarNode*) m_pNodes->objectAtIndex ( i );	
		if ( i == 0 || 
			GameHelper::distanceP1 ( pNode->getPosition ( ), tPoint ) < 
			GameHelper::distanceP1 ( pFoundNode->getPosition ( ), tPoint ) )
		{
			pFoundNode = pNode;
		}	
	}
	return pFoundNode;
}

CCArray* Ch7_SideScrollingPathfinding::findPathFrom ( SSAStarNode* pStartNode, SSAStarNode* pEndNode )
{
	return SSAStarPathNode::findPathFrom ( pStartNode, pEndNode );	
}