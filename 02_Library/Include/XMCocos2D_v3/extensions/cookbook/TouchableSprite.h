/* --------------------------------------------------------------------------
 *
 *      File            TouchableSprite.h
 *      Ported By       Young-Hwan Mun, Jin-Su Park
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. 
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

#ifndef __TouchableSprite_h__
#define __TouchableSprite_h__

#include "../ExtensionMacros.h"
#include "../../2d/sprite_nodes/CCSprite.h"

NS_CC_EXT_BEGIN

class TouchableSprite : public Sprite
{
	public :

		static TouchableSprite*		create ( KDvoid );

	public :		

		virtual KDbool				init ( KDvoid );
		
		virtual KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		virtual KDbool				checkTouchWithPoint ( const Point& tPoint );
		
		virtual Rect				rect ( KDvoid );
		
		virtual KDvoid				processTouch ( const Point& tPoint );
		
		virtual KDvoid				processRelease ( KDvoid );

		virtual KDbool				 isPressed ( KDvoid );
		virtual KDvoid				setPressed ( KDbool bPressed );

		virtual KDint				getTouchHash ( KDvoid );
		virtual KDvoid				setTouchHash ( KDint nTouchHash );

		virtual KDfloat				getScaleMod ( KDvoid );
		virtual KDvoid				setScaleMod ( KDfloat fScaleMod );

		virtual const Color3B&		getTouchColor ( KDvoid );
		virtual KDvoid				setTouchColor ( const Color3B& tColor );


	protected :

		KDbool						m_bPressed;			// Is this sprite pressed
		KDuint						m_nTouchHash;		// Used to identify individual touches
		KDfloat						m_fScaleMod;
		Color3B						m_tTouchColor;
};

NS_CC_EXT_END

#endif	// __TouchableSprite_h__
