/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Impulses.h
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

#ifndef __Ch4_Impulses_h__
#define __Ch4_Impulses_h__

#include "Libraries/GameArea2D.h"

class GameMisc;
class GameSensor;

enum 
{
	// Object type tags
	TYPE_OBJ_BASKETBALL = 0,
	TYPE_OBJ_SHOOTER	= 1,
	TYPE_OBJ_NET_SENSOR = 2,
};

class Ch4_Impulses : public GameArea2D
{
	public :

		SCENE_FUNC ( Ch4_Impulses );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		addLevelBoundaries ( KDvoid );

		virtual KDvoid		handleCollisionWithObjA ( GameObject* pObjectA, GameObject* pObjectB );

		KDvoid				handleCollisionWithSensor ( GameSensor* pSensor, GameMisc* pMisc );		

		KDvoid				addBasketball ( KDvoid );
		
		KDvoid				addBasketballNet ( KDvoid );
		
		KDvoid				addShooter ( KDvoid );


	private :

		GameMisc*			m_pBasketBall;
		CCSprite*			m_pBasketBallNet;
		CCSprite*			m_pBasketBallShooterArm;
};

#endif	// __Ch4_Impulses_h__
