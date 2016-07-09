/* --------------------------------------------------------------------------
 *
 *      File            AStarPathNode.h
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

#ifndef __AStarPathNode_h__
#define __AStarPathNode_h__

#include "AStarNode.h"

class AStarPathNode : public CCObject
{
	public :
				 AStarPathNode ( KDvoid );
		virtual ~AStarPathNode ( KDvoid );

		static AStarPathNode*		create ( AStarNode* pNode );

	public :

		virtual KDbool				initWithAStarNode ( AStarNode* pNode );

		static  CCArray*			findPathFrom ( AStarNode* pFromNode, AStarNode* pToNode );

		static  AStarPathNode*		lowestCostNodeInArray ( CCArray* pArray );

		static  KDbool				isPathNode ( AStarPathNode* pNode, CCArray* pList );

		virtual AStarNode*			getNode ( KDvoid );
		virtual KDvoid				setNode ( AStarNode* pNode );

		virtual AStarPathNode*		getPrevious ( KDvoid );
		virtual KDvoid				setPrevious ( AStarPathNode* pPrevious );

		virtual KDfloat				getCost ( KDvoid );
		virtual KDvoid				setCost ( KDfloat fCost );

	protected :

		AStarNode*					m_pNode;			// The actual node this "path" node points to
		AStarPathNode*				m_pPrevious;		// The previous node on our path
		KDfloat						m_fCost;			// The cumulative cost of reaching this node
};

#endif	// __AStarPathNode_h__
