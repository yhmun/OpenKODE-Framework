/* --------------------------------------------------------------------------
 *
 *      File            Ch4_TopDownIsometric.h
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

#ifndef __Ch4_TopDownIsometric_h__
#define __Ch4_TopDownIsometric_h__

#include "Libraries/GameArea2D.h"
#include "Libraries/GameIsoObject.h"
#include "Libraries/DPad.h"

#define PERSPECTIVE_RATIO	0.5f		// This is our viewing angle. 1 would be perfectly top-down. 0 would be ground level.
#define GRAVITY				0.3f		// Our new Z gravity constant

// NOTE: We modified the Box2D engine for this. I'll need to make a note of that.
enum 
{
	// Object type tags
	TYPE_OBJ_GUNMAN = 0,
	TYPE_OBJ_BALL	= 1,
	TYPE_OBJ_TREE	= 2,
};

class Ch4_TopDownIsometric : public GameArea2D
{
	public :

		SCENE_FUNC ( Ch4_TopDownIsometric );

	protected :		

		virtual KDbool		init ( KDvoid );
	
		virtual KDvoid		onExit ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		handleCollisionWithGroundWithObj ( GameObject* pObject );

		virtual KDvoid		handleZMissWithObjA ( GameObject* pObjectA, GameObject* pObjectB );

		virtual KDvoid		step ( KDfloat fDelta );
	
		virtual KDbool		checkCameraBoundsWithFailPosition ( CCPoint* pFailPosition );
		
		virtual KDvoid		setCameraZoom ( KDfloat fZoom );

		KDvoid				addGroundTexture ( KDvoid );
		
		KDvoid				initGunman ( KDvoid );

		KDvoid				animateGunman ( KDvoid );

		KDvoid				createButtonWithPosition ( const CCPoint& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName );

		virtual KDvoid		fireBall ( KDvoid );
		
		virtual KDvoid		processJump ( KDvoid );

		CCPoint				getWorldPosition ( GameIsoObject* pGameIsoObject );
		
		KDvoid				setZOrderByBodyPosition ( GameIsoObject* pGameIsoObject );

		virtual KDvoid		addBrickWallFrom ( CCPoint tPoint1, CCPoint tPoint2, KDfloat fHeight );
		
		KDvoid				addTreeAtPoint ( const CCPoint& tTreePosition );

		GameIsoObject*		addBallAtPoint ( const CCPoint& tBallPosition );

	protected :

		DPad*				m_pDPad;
		CCArray*			m_pButtons;
		
		GameIsoObject*		m_pGunman;
		KDint				m_nGunmanDirection;
		CCPoint				m_tLastVelocity;
		KDfloat				m_fAnimCount;
		KDfloat				m_fFireCount;
		
		CCArray*			m_pBalls;
		CCArray*			m_pTrees;
		
		KDfloat				m_tLastMultiTouchZoomDistance;	// How far apart your fingers were last time
		CCPoint				m_tLastPressedVector;
};

#endif	// __Ch4_TopDownIsometric_h__
