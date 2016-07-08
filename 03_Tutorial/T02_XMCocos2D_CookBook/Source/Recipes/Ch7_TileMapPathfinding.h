/* --------------------------------------------------------------------------
 *
 *      File            Ch7_TileMapPathfinding.h
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

#ifndef __Ch7_TileMapPathfinding_h__
#define __Ch7_TileMapPathfinding_h__

#include "Ch8_Tiled.h"
#include "Libraries/AStarPathNode.h"

class Ch7_TileMapPathfinding : public Ch8_Tiled
{
	public :

		SCENE_FUNC ( Ch7_TileMapPathfinding );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		step ( KDfloat fDelta );

		virtual KDbool		hudBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDbool		hudMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDbool		hudEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		tapWithPoint ( const CCPoint& tPoint );

		virtual KDvoid		setCameraZoom ( KDfloat fZoom );

		virtual KDvoid		cameraFollowActor ( KDvoid );

		virtual KDvoid		drawLayer ( KDvoid );

		KDvoid				addNeighbor ( AStarNode* pNode, KDint x, KDint y );	
		
		KDvoid				addDrawLayer ( KDvoid );

	private :	

		CCArray*			m_pGrid;
		KDfloat				m_fActorRadius;
};

#endif	// __Ch7_TileMapPathfinding_h__
