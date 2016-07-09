/* --------------------------------------------------------------------------
 *
 *      File            Mole.h
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

#ifndef __Mole_h__
#define __Mole_h__

#include "TouchableSprite.h"

enum 
{
	MOLE_MOVING_UP		,
	MOLE_UP				,
	MOLE_MOVING_DOWN	,
	MOLE_DOWN			,
	MOLE_HIT			,
};

class MoleDelgate 
{
	public :

		virtual KDvoid			processMoleHit ( KDvoid ) = 0;
};

class Mole : public TouchableSprite
{
	public :
				 Mole ( KDvoid );
		virtual ~Mole ( KDvoid );

		static  Mole*			createWithSpriteFrameName ( const KDchar* szSpriteFrameName );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent ); 

		virtual CCRect			rect ( KDvoid );

		virtual KDvoid			processTouch ( const CCPoint& tPoint );

		virtual KDvoid			processRelease ( KDvoid );

		virtual KDvoid			setDownPosition ( const CCPoint& tPoint );

		virtual KDvoid			startPopUp ( KDvoid );
		
		virtual KDvoid			finishPopUp ( KDvoid );
		
		virtual KDvoid			startHideDown ( KDvoid );
		
		virtual KDvoid			finishHideDown ( KDvoid );

		virtual KDvoid			gotHit ( KDvoid );

		virtual KDint			getState ( KDvoid );

		virtual KDvoid			setDelegate ( MoleDelgate* pDelegate );

	protected :

		KDint					m_nState;
		CCAnimation*			m_pNormalAnim;
		CCAnimation*			m_pHitAnim;
		CCPoint					m_tDownPosition;
		MoleDelgate*			m_pDelegate;
};

#endif	// __Mole_h__
