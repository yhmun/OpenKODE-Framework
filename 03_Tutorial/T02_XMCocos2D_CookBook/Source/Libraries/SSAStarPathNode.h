/* --------------------------------------------------------------------------
 *
 *      File            SSAStarPathNode.h
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

#ifndef __SSAStarPathNode_h__
#define __SSAStarPathNode_h__

#include "SSAStarNode.h"

class SSAStarPathNode : public CCObject
{
	public :
				 SSAStarPathNode ( KDvoid );
		virtual ~SSAStarPathNode ( KDvoid );

		static  SSAStarPathNode*	create ( SSAStarNode* pNode );

	public :

		virtual KDbool				initWithAStarNode ( SSAStarNode* pNode );

		static  CCArray*			findPathFrom ( SSAStarNode* pFromNode, SSAStarNode* pToNode );

		static  SSAStarPathNode*	lowestCostNodeInArray ( CCArray* pArray );

		static  KDbool				isPathNode ( SSAStarPathNode* pNode, CCArray* pList );

		virtual SSGameWaypoint*		getWaypoint ( KDvoid );
		virtual KDvoid				setWaypoint ( SSGameWaypoint* pWaypoint );

		virtual SSAStarNode*		getNode ( KDvoid );
		virtual KDvoid				setNode ( SSAStarNode* pNode );

		virtual SSAStarPathNode*	getPrevious ( KDvoid );
		virtual KDvoid				setPrevious ( SSAStarPathNode* pPrevious );

		virtual KDfloat				getCost ( KDvoid );
		virtual KDvoid				setCost ( KDfloat fCost );

	protected :

		SSGameWaypoint*				m_pWaypoint;
		SSAStarNode*				m_pNode;			// The actual node this "path" node points to
		SSAStarPathNode*			m_pPrevious;		// The previous node on our path
		KDfloat						m_fCost;			// The cumulative cost of reaching this node
};

#endif	// __SSAStarPathNode_h__
