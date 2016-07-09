/* --------------------------------------------------------------------------
 *
 *      File            Ch7_Box2DPathfinding.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __Ch7_Box2DPathfinding_h__
#define __Ch7_Box2DPathfinding_h__

#include "Libraries/GameArea2D.h"
#include "Libraries/GameActor.h"
#include "Libraries/DrawLayer.h"
#include "Libraries/AStarPathNode.h"

class Ch7_Box2DPathfinding : public GameArea2D
{
	public :

		SCENE_FUNC ( Ch7_Box2DPathfinding );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		step ( KDfloat fDelta );

		virtual KDvoid		drawLayer ( KDvoid );

		virtual KDvoid		tapWithPoint ( const CCPoint& tPoint );

		KDvoid				addNeighbor ( AStarNode* pNode, KDint x, KDint y );

		KDvoid				addActor ( KDvoid );

		KDvoid				addDrawLayer ( KDvoid );

	private :

		GameActor*			m_pActor;
		KDfloat				m_fActorRadius;
		DrawLayer*			m_pDrawLayer;
		
		CCArray*			m_pGrid;
		KDfloat				m_fNodeSpace;				// The space between each node, increase this to increase A* efficiency at the cost of accuracy
		KDint				m_nGridSizeX;
		KDint				m_nGridSizeY;
};

#endif	// __Ch7_Box2DPathfinding_h__
