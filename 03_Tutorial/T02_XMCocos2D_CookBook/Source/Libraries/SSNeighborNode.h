/* --------------------------------------------------------------------------
 *
 *      File            SSNeighborNode.h
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

#ifndef __SSNeighborNode_h__
#define __SSNeighborNode_h__

class SSAStarNode;

class SSNeighborNode : public CCObject
{
	public :
				 SSNeighborNode ( KDvoid );
		virtual ~SSNeighborNode ( KDvoid );

		static SSNeighborNode*	create ( KDvoid );

	public :

		virtual KDfloat			getCost ( KDvoid );
		virtual KDvoid			setCost ( KDfloat fCost );

		virtual SSAStarNode*	getNode ( KDvoid );
		virtual KDvoid			setNode ( SSAStarNode* pNode );

		virtual KDint			getMoveType ( KDvoid );
		virtual KDvoid			setMoveType ( KDint nMoveType );

		virtual const CCPoint&	getLaunchVector ( KDvoid );
		virtual KDvoid			setLaunchVector ( const CCPoint& tLaunchVector );

	protected :

		KDfloat					m_fCost;				// The cost to this node
		SSAStarNode*			m_pNode;				// The node
		KDint					m_nMoveType;			// How we should move to this node
		CCPoint					m_tLaunchVector;		// If we need to JUMP, the launch vector
};

#endif	// __SSNeighborNode_h__
