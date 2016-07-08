/* --------------------------------------------------------------------------
 *
 *      File            GameButton.h
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

#ifndef __GameButton_h__
#define __GameButton_h__

#include "TouchableSprite.h"

NS_CC_EXT_BEGIN

class GameButton : public TouchableSprite
{
	public :

		static GameButton*		create ( const KDchar* szUpSpriteFrame, const KDchar* szDownSpriteFrame );

	public :		

		virtual KDbool			initWithSpriteFrames ( const KDchar* szUpSpriteFrame, const KDchar* szDownSpriteFrame );

		virtual KDvoid			processTouch ( const Point& tPoint );
		virtual KDvoid			processRelease ( KDvoid );

		virtual const KDchar*	getUpSpriteFrame ( KDvoid );
		virtual KDvoid			setUpSpriteFrame ( const KDchar* szSpriteFrame );

		virtual const KDchar*	getDownSpriteFrame ( KDvoid );
		virtual KDvoid			setDownSpriteFrame ( const KDchar* szSpriteFrame );

		virtual const KDchar*	getName ( KDvoid );
		virtual KDvoid			setName ( const KDchar* szName );

	protected :

		std::string				m_sUpSpriteFrame;
		std::string				m_sDownSpriteFrame;
		std::string				m_sName;
};

NS_CC_EXT_END

#endif	// __GameButton_h__
