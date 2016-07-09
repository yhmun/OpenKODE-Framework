/* --------------------------------------------------------------------------
 *
 *      File            AStarPathNode.cpp
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
#include "AStarPathNode.h"

AStarPathNode* AStarPathNode::create ( AStarNode* pNode )
{
	AStarPathNode*	pRet = new AStarPathNode ( );

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

AStarPathNode::AStarPathNode ( KDvoid )
{
	m_pNode		= KD_NULL;
	m_pPrevious = KD_NULL;
	m_fCost		= 0.0f;
}

AStarPathNode::~AStarPathNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNode );
	CC_SAFE_RELEASE ( m_pPrevious );
}

KDbool AStarPathNode::initWithAStarNode ( AStarNode* pNode )
{
	m_fCost = 0.0f;

	this->setNode ( pNode );

	return KD_TRUE;
}

/// Our implementation of the A* search algorithm 
CCArray* AStarPathNode::findPathFrom ( AStarNode* pFromNode, AStarNode* pToNode )
{
	CCArray*		pFoundPath = CCArray::create ( );

	CCPoint			tFromPos = pFromNode->getPosition ( );
	CCPoint			tToPos   = pToNode  ->getPosition ( );

	if ( tFromPos.x == tToPos.x && tFromPos.y == tToPos.y )
	{
		return KD_NULL;
	 } 

	CCArray*		pOpenList   = CCArray::create ( );
	CCArray*		pClosedList = CCArray::create ( );
	
	AStarPathNode*	pCurrentNode = KD_NULL;
	AStarPathNode*	pNode = KD_NULL;
			
	AStarPathNode*	pStartNode = AStarPathNode::create ( pFromNode );
	AStarPathNode*	pEndNode   = AStarPathNode::create ( pToNode );
	pOpenList->addObject ( pStartNode );
		
	while ( pOpenList->count ( ) > 0 )
	{		
		pCurrentNode = AStarPathNode::lowestCostNodeInArray ( pOpenList );
		
		CCPoint		tCurrentPos = pCurrentNode->getNode ( )->getPosition ( );
		CCPoint		tEndPos		= pEndNode	  ->getNode ( )->getPosition ( );

		if ( tCurrentPos.x == tEndPos.x && tCurrentPos.y == tEndPos.y )
		{	
			// Path Found!
			pNode = pCurrentNode;
			while ( pNode->getPrevious ( ) != KD_NULL )
			{
				// Mark path
				pFoundPath->addObject ( CCPointValue::create ( pNode->getNode ( )->getPosition ( ) ) );
				pNode = pNode->getPrevious ( );
			}
			pFoundPath->addObject ( CCPointValue::create ( pNode->getNode ( )->getPosition ( ) ) );
			return pFoundPath;		
		}		
		else
		{			
			// Still searching
			pClosedList->addObject ( pCurrentNode );
			pOpenList->removeObject ( pCurrentNode );
						
			for ( KDuint i = 0; i < pCurrentNode->getNode ( )->getNeighbors ( )->count ( ); i++ )
			{				
				AStarPathNode*	pNode = AStarPathNode::create ( (AStarNode*) pCurrentNode->getNode ( )->getNeighbors ( )->objectAtIndex ( i ) );
				
				pNode->setCost ( pCurrentNode->getCost ( ) + pCurrentNode->getNode ( )->costToNode ( pNode->getNode ( ) ) + pNode->getNode ( )->costToNode ( pEndNode->getNode ( ) ) );				
				pNode->setPrevious ( pCurrentNode );
				
				if ( pNode->getNode ( )->isActive ( ) && !AStarPathNode::isPathNode ( pNode, pOpenList ) && !AStarPathNode::isPathNode ( pNode, pClosedList ) )
				{
					pOpenList->addObject ( pNode );
				}				
			}			
		}
	}
	
	// No Path Found
	return KD_NULL;
}

/// Helper method: Find the lowest cost node in an array 
AStarPathNode* AStarPathNode::lowestCostNodeInArray ( CCArray* pArray )
{
	AStarPathNode*			pLowest = KD_NULL;

	for ( KDuint i = 0; i < pArray->count ( ); i++ )
	{
		AStarPathNode*		pNode = (AStarPathNode*) pArray->objectAtIndex ( i );
		if ( !pLowest || pNode->getCost ( ) < pLowest->getCost ( ) )
		{
			pLowest = pNode;
		}
	}

	return pLowest;
}

/// Helper method: Is a path node in a given list? 
KDbool AStarPathNode::isPathNode ( AStarPathNode* pNode, CCArray* pList )
{
	CCPoint		tNodePos = pNode->getNode ( )->getPosition ( );
	
	for ( KDuint i = 0; i < pList->count ( ); i++ )
	{
		AStarPathNode*		pNode2 = (AStarPathNode*) pList->objectAtIndex ( i );
		CCPoint				tNode2Pos = pNode2->getNode ( )->getPosition ( );
		if ( tNodePos.x == tNode2Pos.x && tNodePos.y == tNode2Pos.y )
		{
			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

AStarNode* AStarPathNode::getNode ( KDvoid )
{
	return m_pNode;
}

KDvoid AStarPathNode::setNode ( AStarNode* pNode )
{
	CC_SAFE_RELEASE ( m_pNode );
	m_pNode = pNode;
	CC_SAFE_RETAIN  ( m_pNode );
}

AStarPathNode* AStarPathNode::getPrevious ( KDvoid )
{
	return m_pPrevious;
}

KDvoid AStarPathNode::setPrevious ( AStarPathNode* pPrevious )
{
	CC_SAFE_RELEASE ( m_pPrevious );
	m_pPrevious = pPrevious;
	CC_SAFE_RETAIN  ( m_pPrevious );
}

KDfloat AStarPathNode::getCost ( KDvoid )
{
	return m_fCost;
}

KDvoid AStarPathNode::setCost ( KDfloat fCost )
{
	m_fCost = fCost;
}