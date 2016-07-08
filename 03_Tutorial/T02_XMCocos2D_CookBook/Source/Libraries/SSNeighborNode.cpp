/* --------------------------------------------------------------------------
 *
 *      File            SSNeighborNode.cpp
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
#include "SSNeighborNode.h"
#include "SSAStarNode.h"

SSNeighborNode* SSNeighborNode::create ( KDvoid )
{
	SSNeighborNode*		pRet = new SSNeighborNode ( );

	if ( pRet )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

SSNeighborNode::SSNeighborNode ( KDvoid )
{
	m_fCost		= 0;				
	m_pNode		= KD_NULL;		
	m_nMoveType	= 0;		
}

SSNeighborNode::~SSNeighborNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNode );
}

KDfloat SSNeighborNode::getCost ( KDvoid )
{
	return m_fCost;
}

KDvoid SSNeighborNode::setCost ( KDfloat fCost )
{
	m_fCost = fCost;
}

SSAStarNode* SSNeighborNode::getNode ( KDvoid )
{
	return m_pNode;
}

KDvoid SSNeighborNode::setNode ( SSAStarNode* pNode )
{
	CC_SAFE_RELEASE ( m_pNode );
	m_pNode = pNode;
	CC_SAFE_RETAIN  ( m_pNode );
}

KDint SSNeighborNode::getMoveType ( KDvoid )
{
	return m_nMoveType;
}

KDvoid SSNeighborNode::setMoveType ( KDint nMoveType )
{
	m_nMoveType = nMoveType;
}

const CCPoint& SSNeighborNode::getLaunchVector ( KDvoid )
{
	return m_tLaunchVector;
}

KDvoid SSNeighborNode::setLaunchVector ( const CCPoint& tLaunchVector )
{
	m_tLaunchVector = tLaunchVector;
}