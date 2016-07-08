/* --------------------------------------------------------------------------
 *
 *      File            SideScrollerRecipe.h
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

#ifndef __SideScrollerRecipe_h__
#define __SideScrollerRecipe_h__

#include "GameArea2D.h"
#include "GameButton.h"
#include "GameSensor.h"
#include "GameMisc.h"
#include "DPad.h"

enum
{
	// Object type tags
	TYPE_OBJ_BOX	= 0
};

enum 
{	
	// Collision bits for filtering
	CB_GUNMAN		= 1 << 0,
	CB_OTHER		= 1 << 2,
	CB_BULLET		= 1 << 4,
	CB_SHELL		= 1 << 8,
};

class SideScrollerRecipe : public GameArea2D
{
	public :		
				 SideScrollerRecipe ( KDvoid );
		virtual ~SideScrollerRecipe ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent ); 
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid			step ( KDfloat fDelta );

		virtual KDvoid			handleCollisionWithObjA   ( GameObject* pObjectA, GameObject* pObjectB );
		virtual KDvoid			handleCollisionWithSensor ( GameSensor* pSensor, GameMisc* pMisc );
		virtual KDvoid			handleCollisionWithMisc   ( GameMisc* pMiscA, GameMisc* pMiscB );

		virtual KDvoid			initGunman ( KDvoid );

		virtual KDvoid			animateGunman ( KDvoid );

		virtual KDvoid			createButtonWithPosition ( const CCPoint& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName );

		virtual KDvoid			processJump ( KDvoid );

		virtual KDvoid			addBoxWithPosition ( const CCPoint& tPoint, const KDchar* szFile, KDfloat fDensity );

	protected :

		DPad*					m_pDPad;
		CCArray*				m_pButtons;		
		GameMisc*				m_pGunman;
		KDint					m_nGunmanDirection;
		KDfloat					m_fLastXSpeed;
		KDfloat					m_fLastYVelocity;
		KDfloat					m_fJumpCounter;
		KDbool					m_bOnGround;
		CCArray*				m_pBoxes;
};

#endif	// __SideScrollerRecipe_h__
