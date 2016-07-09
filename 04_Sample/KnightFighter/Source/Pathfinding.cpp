/* --------------------------------------------------------------------------
 *
 *      File            Pathfinding.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 10/05/2011
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
#include "Pathfinding.h"

static KDbool	l_bPathfindingDebuggingTiles = KD_FALSE;

Pathfinding::Pathfinding ( KDvoid )
{
	m_pOpenList   = KD_NULL;
	m_pClosedList = KD_NULL;
}

Pathfinding::~Pathfinding ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pOpenList   );
	CC_SAFE_RELEASE ( m_pClosedList );
}

KDbool Pathfinding::init ( KDvoid )
{
	return KD_TRUE;
}

CCArray* Pathfinding::search ( const CCPoint& tStartTile, const CCPoint& tTargetTile )
{
	this->setOpenList ( CCArray::create ( ) );
	this->setClosedList ( CCArray::create ( ) );

	CCLOG ( "In search, within thread" );

	// Add the first node to the open list
	PathfindingNode*	pNode = PathfindingNode::create ( );
	pNode->setTilePos ( tStartTile );
	pNode->setParent ( KD_NULL );
	pNode->setG ( 0 );
	pNode->setH ( 0 );
	pNode->setF ( pNode->getG ( ) + pNode->getH ( ) );
	m_pOpenList->addObject ( pNode );

	this->searchLowestCostNodeInOpenList ( tTargetTile ); 	

	// Retrieve path
	CCArray*	pPathToPlayer = CCArray::create ( );
	pNode = this->isOnList ( tTargetTile, m_pClosedList );
	
	if ( pNode ) 
	{
		CCLOG ( "Path found..." );
		pPathToPlayer->addObject ( pNode );
		
		if ( l_bPathfindingDebuggingTiles )
		{
			// Debugging pathfinding
			GameMgr->getCoordinateFunctions ( )->debugTile ( pNode->getTilePos ( ) );
		}
		
		PathfindingNode*	pParentnode = pNode->getParent ( );
		while ( pParentnode )
		{
			CCLOG ( "%f %f", pNode->getTilePos ( ).x, pNode->getTilePos ( ).y );
			pNode = pParentnode;
			pParentnode = pNode->getParent ( );
			pPathToPlayer->addObject ( pNode );
			
			if ( l_bPathfindingDebuggingTiles )
			{
				// Debugging pathfinding
				GameMgr->getCoordinateFunctions ( )->debugTile  ( pNode->getTilePos ( ) );
			}
		}		
	} 
	else 
	{
		CCLOG ( "No path found" );
	}
	
	return pPathToPlayer;
}

KDvoid Pathfinding::searchLowestCostNodeInOpenList ( const CCPoint& tTargetTile )
{
	PathfindingNode*	pLowestCostNode = KD_NULL;	
	CCObject*			pObject;

	CCARRAY_FOREACH ( m_pOpenList, pObject )
	{
		PathfindingNode*	pNode = (PathfindingNode*) pObject;

		if ( pLowestCostNode == KD_NULL )
		{
			pLowestCostNode = pNode;
		}
		else
		{
			if ( pNode->getF ( ) < pLowestCostNode->getF ( ) )
			{
				pLowestCostNode = pNode;
			}
		}
	}

	m_pOpenList  ->removeObject ( pLowestCostNode );
	m_pClosedList->removeObject ( pLowestCostNode );
		
	CCArray*	pReachableTiles = this->reachableTiles ( pLowestCostNode, tTargetTile );

	CCARRAY_FOREACH ( pReachableTiles, pObject )
	{
		PathfindingNode*	pReachableTile = (PathfindingNode*) pObject;
		m_pOpenList->addObject ( pReachableTile );
	}

	if ( ( tTargetTile.x == pLowestCostNode->getTilePos ( ).x ) && ( tTargetTile.x == pLowestCostNode->getTilePos ( ).y ) )
	{
		CCLOG ( "Path found!!" );
	}
	else 
	{
		if ( m_pOpenList->count ( ) != 0 )
		{
			this->searchLowestCostNodeInOpenList ( tTargetTile );
		}
	}
}

PathfindingNode* Pathfinding::isOnList ( const CCPoint& tTilePos, CCArray* pList )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( pList, pObject )
	{
		PathfindingNode*	pNode = (PathfindingNode*) pObject;
	
		if ( pNode->getTilePos ( ).x == tTilePos.x && pNode->getTilePos ( ).y == tTilePos.y )
		{
			return pNode;
		}
	}
	return KD_NULL;
}

CCArray* Pathfinding::reachableTiles ( PathfindingNode* pFromTile, const CCPoint& tTargetTile )
{
	CCArray*	pReachableTiles = CCArray::create ( );
	
	CCPoint		tFromTilePos  = pFromTile->getTilePos ( );
	CCArray*	pTilesToCheck = CCArray::create 
	(
		CCPointValue::create ( ccp ( tFromTilePos.x - 1, tFromTilePos.y     ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x + 1, tFromTilePos.y     ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x - 1, tFromTilePos.y + 1 ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x    , tFromTilePos.y + 1 ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x + 1, tFromTilePos.y + 1 ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x - 1, tFromTilePos.y - 1 ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x    , tFromTilePos.y - 1 ) ),
		CCPointValue::create ( ccp ( tFromTilePos.x + 1, tFromTilePos.y - 1 ) ),
		KD_NULL
	);

	CCObject*	pObject;
	CCARRAY_FOREACH ( pTilesToCheck, pObject )
	{
		CCPointValue*	pTileToCheck  = (CCPointValue*) pObject;
		CCPoint			tAdjacentTile = pTileToCheck->getValue ( );
		
		if ( this->isReachableSquare ( tAdjacentTile ) ) 
		{			
			if ( !this->isOnList ( tAdjacentTile, m_pClosedList ) )
			{		
				
				KDint	nG;
				if ( ( tFromTilePos.x == tAdjacentTile.x ) || ( tFromTilePos.y == tAdjacentTile.y ) )
				{
					nG = 10;
				}
				else 
				{
					nG = 14;
				} 
				KDint	nH = this->manhattanDistance ( tFromTilePos, tTargetTile );
				KDint	nF = nG + nH;				
				
				PathfindingNode*	pExistingNode = this->isOnList ( pTileToCheck->getValue ( ), m_pOpenList );
				
				if ( pExistingNode )
				{
					
					if ( nF < pExistingNode->getF ( ) )
					{
						pExistingNode->setF ( nF );
						pExistingNode->setG ( nG );
						pExistingNode->setH ( nH );
						pExistingNode->setParent ( pFromTile );
						pReachableTiles->addObject ( pExistingNode );
					} 					
				} 
				else 
				{
					PathfindingNode*	pNode = PathfindingNode::create ( );
					pNode->setTilePos ( pTileToCheck->getValue ( ) );
					pNode->setF ( nF );
					pNode->setG ( nG );
					pNode->setH ( nH );
					pNode->setParent ( pFromTile );
					pReachableTiles->addObject ( pNode );
				}				
			}			
		}		
	}
	
	return pReachableTiles;
}

KDbool Pathfinding::isReachableSquare ( const CCPoint& tTilePos )
{
	if ( GameMgr->getCoordinateFunctions ( )->isTilePosIsWithinBounds ( tTilePos ) )
	{
		if ( !GameMgr->getCoordinateFunctions ( )->isTilePosBlocked ( tTilePos ) )
		{
			return KD_TRUE;
		}
	}
	return KD_FALSE;
}

KDint Pathfinding::manhattanDistance ( const CCPoint& tFromTile, const CCPoint& tToTile )
{
	return 10 * kdAbs ( (KDint) ( tFromTile.y - tToTile.y ) ) + kdAbs ( (KDint) ( tFromTile.x - tToTile.x ) );
}
