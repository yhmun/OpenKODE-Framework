/* --------------------------------------------------------------------------
 *
 *      File            GameArea2D.h
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

#ifndef __GameArea2D_h__
#define __GameArea2D_h__

#include "GameObject.h"
#include "DebugDrawNode.h"

enum 
{
	GO_TAG_WALL				= 0,
	GO_TAG_ACTOR			= 1,
};

class GameArea2D : public Recipe
{
	public :
				 GameArea2D ( KDvoid );
		virtual ~GameArea2D ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			step ( KDfloat fDelta );
		
		virtual KDvoid			handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB );

		virtual KDvoid			handleZMissWithObjA ( GameObject* pObjectA, GameObject* pObjectB );
		
		virtual KDvoid			showDebugDraw ( KDvoid );
		
		virtual KDvoid			showMinimalDebugDraw ( KDvoid );
		
		virtual KDvoid			hideDebugDraw ( KDvoid );
		
		virtual KDvoid			initDebugDraw ( KDvoid );
		
		virtual KDvoid			swapDebugDraw ( CCObject* pSender = KD_NULL );
		
		virtual KDvoid			addDebugButton ( KDvoid );

		virtual KDvoid			addLevelBoundaries ( KDvoid );

		virtual KDvoid			addRandomPolygons ( KDint nNum );
		
		virtual KDvoid			addRandomBoxes ( KDint nNum );
		
		virtual KDvoid			addPolygonAtPoint ( CCPoint tPoint );

		virtual KDvoid			addBoxAtPoint ( CCPoint tPoint, const CCSize& tSize );

		virtual KDvoid			markBodyForDestruction ( GameObject* pObject );
		
		virtual KDvoid			destroyBodies ( KDvoid );
		
		virtual KDvoid			markBodyForCreation ( GameObject* pObject );
		
		virtual KDvoid			createBodies ( KDvoid );

		virtual KDvoid			runQueuedActions ( KDvoid );

		// Drawing
		virtual KDvoid			drawLayer ( KDvoid );

		// Camera
		virtual KDbool			checkCameraBoundsWithFailPosition ( CCPoint* pFailPosition );
		
		virtual KDbool			checkCameraBounds ( KDvoid );
		
		virtual KDvoid			setCameraPosition ( const CCPoint& tPosition );
		
		virtual CCPoint			convertCamera ( const CCPoint& tPosition );
		
		virtual KDvoid			setCameraZoom ( KDfloat fZoom );
		
		virtual KDvoid			zoomTo ( KDfloat fZoom, KDfloat fSpeed );
		
		virtual KDvoid			processZoomStep ( KDvoid );
		
		virtual CCPoint			convertTouchCoord ( const CCPoint& tTouchPoint );
		
		virtual CCPoint			convertGameCoord ( const CCPoint& tGamePoint );

		virtual KDvoid			centerCameraOnGameCoord ( const CCPoint& tGamePoint );

		// Touches
		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDbool			hudBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDbool			hudMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDbool			hudEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid			tapWithPoint ( const CCPoint& tPoint );

		// Misc
		virtual KDfloat			distanceP1 ( const CCPoint& tFirstTouch, const CCPoint& tSecondTouch );

	protected :

		b2World*				m_pWorld;
		CCBox2DDebugDraw*		m_pDebugDraw;
		KDbool					m_bDebugDraw;
		CCNode*					m_pGameNode;
		DebugDrawNode*			m_pDebugDrawNode;
		GameObject*				m_pLevelBoundary;

		CCArray*				m_pBodiesToDestroy;
		CCArray*				m_pPostDestructionCallbacks;
		CCArray*				m_pBodiesToCreate;
		CCArray*				m_pQueuedActions;

		CCSize					m_tGameAreaSize;
		KDfloat					m_fCameraZoom;					// Starts at 1.0f. Smaller means more zoomed out.
		KDfloat					m_fCameraZoomTo;
		KDfloat					m_fCameraZoomSpeed;				// Must be greater than 0 but less than 1
		KDfloat					m_fLastMultiTouchZoomDistance;	// How far apart your fingers were last time
		CCPoint					m_tDraggedToPoint;				// How far we dragged from initial touch
		CCPoint					m_tLastTouchedPoint;			// Where we last touched
		KDint					m_nCameraState;					// What is the camera currently doing?

		CCDictionary*			m_pAllTouches;
};

#endif	// __GameArea2D_h__
