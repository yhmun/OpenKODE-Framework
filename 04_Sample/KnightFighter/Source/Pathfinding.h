/* --------------------------------------------------------------------------
 *
 *      File            Pathfinding.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 10/05/2011
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

#ifndef __Pathfinding_h__
#define __Pathfinding_h__

#include "PathfindingNode.h"

class Pathfinding : public CCNode 
{
	public :
				 Pathfinding ( KDvoid );
		virtual ~Pathfinding ( KDvoid );

		CREATE_FUNC ( Pathfinding );	

	public :

		virtual KDbool			init ( KDvoid );

		CCArray*				search ( const CCPoint& tStartTile, const CCPoint& tTargetTile );

		KDvoid					searchLowestCostNodeInOpenList ( const CCPoint& tTargetTile );

		PathfindingNode*		isOnList ( const CCPoint& tTilePos, CCArray* pList );

		CCArray*				reachableTiles ( PathfindingNode* pFromTile, const CCPoint& tTargetTile );

		KDbool					isReachableSquare ( const CCPoint& tTilePos );

		KDint					manhattanDistance ( const CCPoint& tFromTile, const CCPoint& tToTile );

	public :

		CC_SYNTHESIZE ( CCArray*, m_pOpenList  , OpenList   );
		CC_SYNTHESIZE ( CCArray*, m_pClosedList, ClosedList );
};

#endif	// __Pathfinding_h__