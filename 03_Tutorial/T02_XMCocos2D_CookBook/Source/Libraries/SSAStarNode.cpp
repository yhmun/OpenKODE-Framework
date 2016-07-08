/* --------------------------------------------------------------------------
 *
 *      File            SSAStarNode.h
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
#include "SSAStarNode.h"

SSAStarNode* SSAStarNode::create ( KDvoid )
{
	SSAStarNode*	pRet = new SSAStarNode ( );

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

SSAStarNode::SSAStarNode ( KDvoid )
{
	m_pActor = KD_NULL;
}

SSAStarNode::~SSAStarNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pActor );
}

KDfloat SSAStarNode::costToNeighbor ( SSNeighborNode* pNeighbor )
{
	SSAStarNode*	pNode = pNeighbor->getNode ( );

	// Here we use jumping/running to determine cost. We could also possibly use a heuristic.
	CCPoint		tSrc = ccp ( this ->getPosition ( ).x / PTM_RATIO, this ->getPosition ( ).y / PTM_RATIO );
	CCPoint		tDst = ccp ( pNode->getPosition ( ).x / PTM_RATIO, pNode->getPosition ( ).y / PTM_RATIO );
	
	KDfloat		fCost;

	if ( pNode->getBody ( ) == this->getBody ( ) )
	{		
		// Compute simple distance
		KDfloat		fRunTime = GameHelper::distanceP1 ( tSrc, tDst ) / m_pActor->getRunSpeed ( );
		fCost = fRunTime * pNode->getCostMultiplier ( );
	}
	else
	{		
		// Compute a jump		
		KDfloat		y = tDst.y - tSrc.y;
		if ( y == 0 )
		{
			y = 0.00001f;		// Prevent divide by zero
		}
		
		CCPoint		tLaunchVector = pNeighbor->getLaunchVector ( );
		KDfloat		fGravity = m_pActor->getBody ( )->GetWorld ( )->GetGravity ( ).y;
				
		Vector3D*	pAirVect = GameHelper::quadraticA ( fGravity * 0.5f, tLaunchVector.y, y * -1 );
		KDfloat		fAirTime;
		
		if ( pAirVect->x > pAirVect->y )
		{
			fAirTime = pAirVect->x;
		}
		else
		{
			fAirTime = pAirVect->y;
		}
		fCost = fAirTime * pNode->getCostMultiplier ( );
	}

	return fCost;
}

KDfloat SSAStarNode::costToNode ( SSAStarNode* pNode )
{
	// This is a simple heuristic
	CCPoint		tSrc = ccp ( this ->getPosition ( ).x / PTM_RATIO, this ->getPosition ( ).y / PTM_RATIO );
	CCPoint		tDst = ccp ( pNode->getPosition ( ).x / PTM_RATIO, pNode->getPosition ( ).y / PTM_RATIO );

	// Compute simple distance
	KDfloat		fRunTime = GameHelper::distanceP1 ( tSrc, tDst ) / m_pActor->getRunSpeed ( );
	KDfloat		fCost = fRunTime * pNode->getCostMultiplier ( );
	return fCost;
}

KDbool SSAStarNode::containsNeighborForNode ( SSAStarNode* pNode )
{
	for ( KDuint i = 0; i < m_pNeighbors->count ( ); i++ )
	{
		SSNeighborNode*		pNeighbor = (SSNeighborNode*) m_pNeighbors->objectAtIndex ( i );
		if ( pNeighbor->getNode ( ) == pNode )
		{
			return KD_TRUE;
		}
	}
	return KD_FALSE;
}

b2Body* SSAStarNode::getBody ( KDvoid )
{
	return m_pBody;
}

KDvoid SSAStarNode::setBody ( b2Body* pBody )
{
	m_pBody = pBody;
}

SSGameActor* SSAStarNode::getActor ( KDvoid )
{
	return m_pActor;
}

KDvoid SSAStarNode::setActor ( SSGameActor* pActor )
{
	CC_SAFE_RELEASE ( m_pActor );
	m_pActor = pActor;
	CC_SAFE_RETAIN  ( m_pActor );
}

