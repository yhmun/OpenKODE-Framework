/* --------------------------------------------------------------------------
 *
 *      File            Ch8_Tiled.h
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

#ifndef __Ch8_Tiled_h__
#define __Ch8_Tiled_h__

#include "Libraries/GameArea2D.h"
#include "Libraries/GameActor.h"
#include "Libraries/DrawLayer.h"
#include "Libraries/DPad.h"

class Ch8_Tiled : public GameArea2D
{
	public :

		SCENE_FUNC ( Ch8_Tiled );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		step ( KDfloat fDelta );

		virtual KDbool		hudBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDbool		hudMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDbool		hudEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		setGameAreaSize ( KDvoid );

		virtual KDvoid		cameraFollowActor ( KDvoid );

		KDvoid				addDPad ( KDvoid );

		KDvoid				addActor ( KDvoid );

	protected :

		CCTMXTiledMap*		m_pTileMap;
		GameActor*			m_pActor;
		DrawLayer*			m_pDrawLayer;
		DPad*				m_pDPad;
		Vector3D*			m_pSpawnPoint;
};

#endif	// __Ch8_Tiled_h__
