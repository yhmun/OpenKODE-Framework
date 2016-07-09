/* --------------------------------------------------------------------------
 *
 *      File            SSAStarPathNode.cpp
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
#include "SSAStarPathNode.h"

SSAStarPathNode* SSAStarPathNode::create ( SSAStarNode* pNode )
{
	SSAStarPathNode*	pRet = new SSAStarPathNode ( );

	if ( pRet && pRet->initWithAStarNode ( pNode ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SSAStarPathNode::SSAStarPathNode ( KDvoid )
{
	m_pWaypoint = KD_NULL;
	m_pNode		= KD_NULL;
	m_pPrevious = KD_NULL;
	m_fCost		= 0.0f;
}

SSAStarPathNode::~SSAStarPathNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pWaypoint );
	CC_SAFE_RELEASE ( m_pNode );
	CC_SAFE_RELEASE ( m_pPrevious );
}

KDbool SSAStarPathNode::initWithAStarNode ( SSAStarNode* pNode )
{
	m_fCost = 0.0f;

	this->setNode ( pNode );

	return KD_TRUE;
}

/// Our implementation of the A* search algorithm 
CCArray* SSAStarPathNode::findPathFrom ( SSAStarNode* pFromNode, SSAStarNode* pToNode )
{

	CCArray*			pFoundPath = CCArray::create ( );

	
	CCPoint				tFromPos = pFromNode->getPosition ( );
	CCPoint				tToPos   = pToNode  ->getPosition ( );

	if ( tFromPos.x == tToPos.x && tFromPos.y == tToPos.y )
	{
		return KD_NULL;
	} 

	CCArray*			pOpenList   = CCArray::create ( );
	CCArray*			pClosedList = CCArray::create ( );
	
	SSAStarPathNode*	pCurrentNode = KD_NULL;
	SSAStarPathNode*	pNode = KD_NULL;
	
	SSAStarPathNode*	pStartNode = SSAStarPathNode::create ( pFromNode );
	pStartNode->setWaypoint ( SSGameWaypoint::create ( pStartNode->getNode ( )->getPosition ( ), MOVE_TYPE_RUN, 1.0f ) );

	SSAStarPathNode*	pEndNode = SSAStarPathNode::create ( pToNode );
	pEndNode->setWaypoint ( SSGameWaypoint::create ( pEndNode->getNode ( )->getPosition ( ), MOVE_TYPE_RUN, 1.0f ) );

	pOpenList->addObject ( pStartNode );
					
	while ( pOpenList->count ( ) > 0 )
	{		
		pCurrentNode = SSAStarPathNode::lowestCostNodeInArray ( pOpenList );
		
		CCPoint		tCurrentPos = pCurrentNode->getNode ( )->getPosition ( );
		CCPoint		tEndPos		= pEndNode	  ->getNode ( )->getPosition ( );

		if ( tCurrentPos.x == tEndPos.x && tCurrentPos.y == tEndPos.y )
		{	
			// Path Found!
			pNode = pCurrentNode;
			while ( pNode->getPrevious ( ) != KD_NULL )
			{
				// Mark path
				pFoundPath->addObject ( pNode );
				pNode = pNode->getPrevious ( );
			}
			pFoundPath->addObject ( pNode );
			return pFoundPath;		
		}			
		else
		{	
			
			// Still searching
			pClosedList->addObject ( pCurrentNode );
			pOpenList->removeObject ( pCurrentNode );
						
			for ( KDuint i = 0; i < pCurrentNode->getNode ( )->getNeighbors ( )->count ( ); i++ )
			{				
				SSNeighborNode*		pNeighbor = (SSNeighborNode*) pCurrentNode->getNode ( )->getNeighbors ( )->objectAtIndex ( i );
				SSAStarPathNode*	pNode = SSAStarPathNode::create ( pNeighbor->getNode ( ) );
					
				pNode->setCost ( pCurrentNode->getCost ( ) + pCurrentNode->getNode ( )->costToNeighbor ( pNeighbor ) + pNode->getNode ( )->costToNode ( pEndNode->getNode ( ) ) );				
				pNode->setPrevious ( pCurrentNode );
				pNode->setWaypoint ( SSGameWaypoint::create ( pNode->getNode ( )->getPosition ( ), pNeighbor->getMoveType ( ), 1.0f ) );
				pNode->getWaypoint ( )->setLaunchVector ( pNeighbor->getLaunchVector ( ) );

				if ( pNode->getNode ( )->isActive ( ) && !SSAStarPathNode::isPathNode ( pNode, pOpenList ) && !SSAStarPathNode::isPathNode ( pNode, pClosedList ) )
				{
					pOpenList->addObject ( pNode );
				}	
			}				
		}		
	}
	
	// No Path Found
	pFoundPath->addObject ( pStartNode );

	return pFoundPath;
}

/// Helper method: Find the lowest cost node in an array 
SSAStarPathNode* SSAStarPathNode::lowestCostNodeInArray ( CCArray* pArray )
{
	SSAStarPathNode*	pLowest = KD_NULL;

	for ( KDuint i = 0; i < pArray->count ( ); i++ )
	{
		SSAStarPathNode*	pNode = (SSAStarPathNode*) pArray->objectAtIndex ( i );
		if ( !pLowest || pNode->getCost ( ) < pLowest->getCost ( ) )
		{
			pLowest = pNode;
		}
	}

	return pLowest;
}

/// Helper method: Is a path node in a given list? 
KDbool SSAStarPathNode::isPathNode ( SSAStarPathNode* pNode, CCArray* pList )
{
	CCPoint		tNodePos = pNode->getNode ( )->getPosition ( );
	
	for ( KDuint i = 0; i < pList->count ( ); i++ )
	{
		SSAStarPathNode*	pNode2 = (SSAStarPathNode*) pList->objectAtIndex ( i );
		CCPoint				tNode2Pos = pNode2->getNode ( )->getPosition ( );
		if ( tNodePos.x == tNode2Pos.x && tNodePos.y == tNode2Pos.y )
		{
			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

SSGameWaypoint* SSAStarPathNode::getWaypoint ( KDvoid )
{
	return m_pWaypoint;
}

KDvoid SSAStarPathNode::setWaypoint ( SSGameWaypoint* pWaypoint )
{
	CC_SAFE_RELEASE ( m_pWaypoint );
	m_pWaypoint = pWaypoint;
	CC_SAFE_RETAIN  ( m_pWaypoint );
}

SSAStarNode* SSAStarPathNode::getNode ( KDvoid )
{
	return m_pNode;
}

KDvoid SSAStarPathNode::setNode ( SSAStarNode* pNode )
{
	CC_SAFE_RELEASE ( m_pNode );
	m_pNode = pNode;
	CC_SAFE_RETAIN  ( m_pNode );
}

SSAStarPathNode* SSAStarPathNode::getPrevious ( KDvoid )
{
	return m_pPrevious;
}

KDvoid SSAStarPathNode::setPrevious ( SSAStarPathNode* pPrevious )
{
	CC_SAFE_RELEASE ( m_pPrevious );
	m_pPrevious = pPrevious;
	CC_SAFE_RETAIN  ( m_pPrevious );
}

KDfloat SSAStarPathNode::getCost ( KDvoid )
{
	return m_fCost;
}

KDvoid SSAStarPathNode::setCost ( KDfloat fCost )
{
	m_fCost = fCost;
}