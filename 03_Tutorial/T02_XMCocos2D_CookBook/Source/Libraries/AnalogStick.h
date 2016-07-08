/* --------------------------------------------------------------------------
 *
 *      File            AnalogStick.h
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

#ifndef __AnalogStick_h__
#define __AnalogStick_h__

#include "TouchableSprite.h"

enum 
{
	AS_NO_DIRECTION	,
	AS_UP			,
	AS_UP_RIGHT		,
	AS_RIGHT		,
	AS_DOWN_RIGHT	,
	AS_DOWN			,
	AS_DOWN_LEFT	,
	AS_LEFT			,
	AS_UP_LEFT		,
};

class AnalogStick : public TouchableSprite
{
	public :

		static AnalogStick*		create ( KDvoid );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			processTouch ( const CCPoint& tPoint );
		virtual KDvoid			processRelease ( KDvoid );

		virtual KDvoid			repositionNub ( KDvoid );
		
		virtual KDvoid			repositionBarWithPoint ( const CCPoint& tPoint );
		
		virtual KDvoid			resetDirection ( KDvoid );

		virtual KDfloat			magnitude ( KDvoid );

		virtual CCPoint			getPressedVector ( KDvoid );

		virtual KDint			getDirection ( KDvoid );
		virtual KDvoid			setDirection ( KDint nDirection );

		virtual CCSprite*		getNub ( KDvoid );
		virtual KDvoid			setNub ( CCSprite* pNub );

		virtual CCSprite*		getBar ( KDvoid );
		virtual KDvoid			setBar ( CCSprite* pBar );

	protected :

		CCPoint					m_tPressedVector;	
		KDint					m_nDirection;
		CCSprite*				m_pNub;
		CCSprite*				m_pBar;
};

#endif	// __AnalogStick_h__
