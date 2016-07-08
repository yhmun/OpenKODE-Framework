/* --------------------------------------------------------------------------
 *
 *      File            GameDPad.h
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

#ifndef __GameDPad_h__
#define __GameDPad_h__

#include "TouchableSprite.h"

NS_CC_EXT_BEGIN

typedef enum 
{
	NO_DIRECTION		= 0,
	LEFT				= 1,
	UP_LEFT				= 2,
	UP					= 3,
	UP_RIGHT			= 4,
	RIGHT				= 5,
	DOWN_RIGHT			= 6,
	DOWN				= 7,
	DOWN_LEFT			= 8,
} DirectionType;

class GameDPad : public TouchableSprite
{
	public :

		static GameDPad*		create ( const KDchar* szNormalSpriteFrame, const KDchar* szHorizontalSpriteFrame, const KDchar* szDiagonalSpriteFrame );

	public :		

		virtual KDbool			initWithSpriteFrames ( const KDchar* szNormalSpriteFrame, const KDchar* szHorizontalSpriteFrame, const KDchar* szDiagonalSpriteFrame );

		virtual KDvoid			processTouch ( const Point& tPoint );
		virtual KDvoid			processRelease ( KDvoid );

		virtual const Point&	getPressedVector ( KDvoid );
		virtual KDvoid			setPressedVector ( const Point& tPressedVector );

		virtual KDint			getDirection ( KDvoid );
		virtual KDvoid			setDirection ( KDint nDirection );

	protected :

		Point					m_tPressedVector;
		KDint					m_nDirection;
		std::string				m_sNormalSpriteFrame;
		std::string				m_sHorizontalSpriteFrame;
		std::string				m_sDiagonalSpriteFrame;
};

NS_CC_EXT_END

#endif	// __GameDPad_h__
