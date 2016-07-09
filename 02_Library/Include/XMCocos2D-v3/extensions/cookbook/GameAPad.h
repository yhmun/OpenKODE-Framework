/* --------------------------------------------------------------------------
 *
 *      File            GameDPad.h
 *      Ported By       Young-Hwan Mun
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

#ifndef __GameAPad_h__
#define __GameAPad_h__

#include "GameDPad.h"

NS_CC_EXT_BEGIN

class GameAPad : public TouchableSprite
{
	public :

		static GameAPad*		create ( const KDchar* szPadSpriteFrame, const KDchar* szBarSpriteFrame, const KDchar* szNubSpriteFrame );

	public :		

		virtual KDbool			initWithSpriteFrames ( const KDchar* szPadSpriteFrame, const KDchar* szBarSpriteFrame, const KDchar* szNubSpriteFrame );

		virtual KDvoid			processTouch ( const Point& tPoint );
		virtual KDvoid			processRelease ( KDvoid );

		virtual KDvoid			repositionNub ( KDvoid );
		
		virtual KDvoid			repositionBarWithPoint ( const Point& tPoint );
		
		virtual KDvoid			resetDirection ( KDvoid );

		virtual KDfloat			magnitude ( KDvoid );

		virtual Point			getPressedVector ( KDvoid );

		virtual KDint			getDirection ( KDvoid );
		virtual KDvoid			setDirection ( KDint nDirection );

		virtual Sprite*			getNub ( KDvoid );
		virtual KDvoid			setNub ( Sprite* pNub );

		virtual Sprite*			getBar ( KDvoid );
		virtual KDvoid			setBar ( Sprite* pBar );

	protected :

		Point					m_tPressedVector;	
		KDint					m_nDirection;
		Sprite*					m_pNub;
		Sprite*					m_pBar;
		std::string				m_sPadSpriteFrame;
		std::string				m_sBarSpriteFrame;
		std::string				m_sNubSpriteFrame;
};

NS_CC_EXT_END

#endif	// __GameAPad_h__
