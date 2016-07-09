/* --------------------------------------------------------------------------
 *
 *      File            AStarNode.h
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
#include "AStarNode.h"

AStarNode* AStarNode::create ( KDvoid )
{
	AStarNode*	pRet = new AStarNode ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

AStarNode::AStarNode ( KDvoid )
{
	m_pNeighbors = KD_NULL;
}

AStarNode::~AStarNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNeighbors );
}

KDbool AStarNode::init ( KDvoid )
{
	m_bActive = KD_TRUE;
	m_fCostMultiplier = 1.0f;

	this->setNeighbors ( CCArray::create ( ) );

	return KD_TRUE;
}

/// Cost to node heuristic 
KDfloat AStarNode::costToNode ( AStarNode* pNode )
{
	CCPoint		tSrc = this->getPosition ( );
	CCPoint		tDst = pNode->getPosition ( );
	KDfloat		fCost = GameHelper::distanceP1 ( tSrc, tDst ) * pNode->getCostMultiplier ( );
	return fCost;
}

/// Helper method: Is a node in a given list? 
KDbool AStarNode::isNode ( AStarNode* pNode, CCArray* pList )
{
	CCPoint		tNodePos = pNode->getPosition ( );

	for ( KDuint i = 0; i < pList->count ( ); i++ )
	{
		AStarNode*		pNode2 = (AStarNode*) pList->objectAtIndex ( i );
		CCPoint			pNode2Pos = pNode2->getPosition ( );
		if ( tNodePos.x == pNode2Pos.x && tNodePos.y == pNode2Pos.y )
		{
			return KD_TRUE;
		}
	}
	return KD_FALSE;
}

const CCPoint& AStarNode::getPosition ( KDvoid )
{
	return m_tPosition;
}

KDvoid AStarNode::setPosition ( const CCPoint& tPosition )
{
	m_tPosition = tPosition;
}

CCArray* AStarNode::getNeighbors ( KDvoid )
{
	return m_pNeighbors;
}

KDvoid AStarNode::setNeighbors ( CCArray* pNeighbors )
{
	CC_SAFE_RELEASE ( m_pNeighbors );
	m_pNeighbors = pNeighbors;
	CC_SAFE_RETAIN  ( m_pNeighbors );
}

KDbool AStarNode::isActive ( KDvoid )
{
	return m_bActive;
}

KDvoid AStarNode::setActive ( KDbool bActive )
{		
	m_bActive = bActive;
}

KDfloat AStarNode::getCostMultiplier ( KDvoid )
{
	return m_fCostMultiplier;
}

KDvoid AStarNode::setCostMultiplier ( KDfloat fCostMultiplier )
{
	m_fCostMultiplier = fCostMultiplier;
}