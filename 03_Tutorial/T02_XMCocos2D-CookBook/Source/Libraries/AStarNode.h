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

#ifndef __AStarNode_h__
#define __AStarNode_h__

class AStarNode : public CCObject
{
	public :
				 AStarNode ( KDvoid );
		virtual ~AStarNode ( KDvoid );

		static AStarNode*		create ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDfloat			costToNode ( AStarNode* pNode );

		static  KDbool			isNode ( AStarNode* pNode, CCArray* pList );

		virtual const CCPoint&	getPosition ( KDvoid );
		virtual KDvoid			setPosition ( const CCPoint& tPosition );

		virtual CCArray*		getNeighbors ( KDvoid );
		virtual KDvoid			setNeighbors ( CCArray* pNeighbors );

		virtual KDbool			 isActive ( KDvoid );
		virtual KDvoid			setActive ( KDbool bActive );

		virtual KDfloat			getCostMultiplier ( KDvoid );
		virtual KDvoid			setCostMultiplier ( KDfloat fCostMultiplier );

	protected :

		CCPoint					m_tPosition;			// The node's position on our map
		CCArray*				m_pNeighbors;			// An array of neighbor AStarNode objects
		KDbool					m_bActive;				// Is this node active?
		KDfloat					m_fCostMultiplier;		// Use this to multiply the normal cost to reach this node.
};

#endif	// __AStarNode_h__
