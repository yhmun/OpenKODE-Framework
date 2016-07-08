/* --------------------------------------------------------------------------
 *
 *      File            TouchableSprite.h
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

#ifndef __TouchableSprite_h__
#define __TouchableSprite_h__

class TouchableSprite : public CCSprite
{
	public :

		static TouchableSprite*		create ( KDvoid );

	public :		

		virtual KDbool		init ( KDvoid );
		
 		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent ); 
 		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
 		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDbool		checkTouchWithPoint ( const CCPoint& tPoint );
		
		virtual CCRect		rect ( KDvoid );
		
		virtual KDvoid		processTouch ( const CCPoint& tPoint );
		
		virtual KDvoid		processRelease ( KDvoid );

		virtual KDbool		 isPressed ( KDvoid );
		virtual KDvoid		setPressed ( KDbool bPressed );

		virtual KDint		getTouchHash ( KDvoid );
		virtual KDvoid		setTouchHash ( KDint nTouchHash );

	protected :

		KDbool				m_bPressed;			// Is this sprite pressed
		KDuint				m_nTouchHash;		// Used to identify individual touches
};

#endif	// __TouchableSprite_h__
