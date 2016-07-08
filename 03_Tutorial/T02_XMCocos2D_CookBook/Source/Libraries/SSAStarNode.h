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

#ifndef __SSAStarNode_h__
#define __SSAStarNode_h__

#include "AStarNode.h"
#include "SSNeighborNode.h"
#include "SSGameActor.h"

class SSAStarNode : public AStarNode
{
	public :
				 SSAStarNode ( KDvoid );
		virtual ~SSAStarNode ( KDvoid );

		static SSAStarNode*		create ( KDvoid );

	public :

		virtual KDfloat			costToNeighbor ( SSNeighborNode* pNode );

		virtual KDfloat			costToNode ( SSAStarNode* pNode );

		virtual KDbool			containsNeighborForNode ( SSAStarNode* pNode );

		virtual b2Body*			getBody ( KDvoid );
		virtual KDvoid			setBody ( b2Body* pBody );

		virtual SSGameActor*	getActor ( KDvoid );
		virtual KDvoid			setActor ( SSGameActor* pActor );

	protected :

		b2Body*					m_pBody;		// The static body this node is associated with
		SSGameActor*			m_pActor;		// The actor who will be using these nodes
};

#endif	// __SSAStarNode_h__
