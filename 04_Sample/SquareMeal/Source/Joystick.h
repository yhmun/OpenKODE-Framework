/* --------------------------------------------------------------------------
 *
 *      File            Joystick.h
 *
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
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __Joystick_h__
#define __Joystick_h__


class Joystick :public CCLayer
{
	public :

		/**
		 *	center:   Center of the joystick 
		 *	radius:   Radius of the joystick 
		 *	joystick: Sprite to be displayed as the controlling item
		 *	stickBg:  Background of the joystick
		 */
		static Joystick*	createWithCenter ( const CCPoint& tCenter, KDfloat fRadius, const KDchar* szJoystick, const KDchar* szStickBg, KDbool bIsFollowRole );

	public :

		/**
		 *	Enable the joystick
		 */
		KDvoid				active ( KDvoid );

		/**
		 *	Disable the joystick
		 */
		KDvoid				inactive ( KDvoid );

		KDvoid				setJostickOpacity ( GLubyte cOpacity );

		KDvoid				setJoystickBackgroundOpacity ( GLubyte cOpacity );

		CCPoint				getDirection ( KDvoid );

		KDfloat				getVelocity ( KDvoid );

		KDfloat				getRadius ( KDvoid );

	private :

		KDbool				initWithCenter ( const CCPoint& tCenter, KDfloat fRadius, const KDchar* szJoystick, const KDchar* szStickBg, KDbool bIsFollowRole );

		KDvoid				updatePos ( KDfloat fDelta );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

	private :

		CCPoint				m_tCenterPoint;
		CCPoint				m_tCurrentPoint;
		KDbool				m_bActive;
		KDfloat				m_fRadius;
		CCSprite*			m_pSprite;
		CCSprite*			m_pBgSprite;
		KDbool				m_bIsFollowRole;				
};

#endif	// __Joystick_h__
