/* --------------------------------------------------------------------------
 *
 *      File            Ch7_GridPathfinding.h
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

#ifndef __Ch7_GridPathfinding_h__
#define __Ch7_GridPathfinding_h__

#include "Libraries/AStarPathNode.h"
#include "Libraries/DrawLayer.h"

class Ch7_GridPathfinding : public Recipe
{
	public :

		SCENE_FUNC ( Ch7_GridPathfinding );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent ); 
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent ); 

		virtual KDvoid		drawLayer ( KDvoid );

		KDvoid				step ( KDfloat fDelta );
		
		KDvoid				addDrawLayer ( KDvoid );

		KDvoid				switchMode ( CCObject* pSender );
		
		KDvoid				findPath ( CCObject* pSender );
		
		KDvoid				addNeighbor ( AStarNode* pNode, KDint x, KDint y );
		
		KDvoid				addGridArt ( KDvoid );

		KDvoid				flipNodeWithTouchedNode ( const CCPoint& tPoint );

	private :

		CCArray*			m_pGrid;
		CCPoint				m_tGridSize;
		KDfloat				m_fNodeSpace;			// The space between each node
		CCNode*				m_pGridNode;			// Where we draw everything
		CCDictionary*		m_pSprites;				// A way to keep track of our sprites
		CCPoint				m_tTouchedNode;			// Where we touched
		KDbool				m_bTouchedNodeIsNew;
		CCArray*			m_pFoundPath;
		DrawLayer*			m_pDrawLayer;
		CCPoint				m_tStartCoord;
		CCPoint				m_tEndCoord;
		KDbool				m_bAddMode;
};

#endif	// __Ch7_GridPathfinding_h__
