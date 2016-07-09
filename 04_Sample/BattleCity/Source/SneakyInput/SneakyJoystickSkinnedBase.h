/* --------------------------------------------------------------------------
 *
 *      File            SneakyJoystickSkinnedBase.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2010/02/18
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010	    Sneakyness, llc. All rights reserved.
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

#ifndef __SneakyJoystickSkinnedBase_h__
#define __SneakyJoystickSkinnedBase_h__

NS_CC_BEGIN

class SneakyJoystick;

class SneakyJoystickSkinnedBase : public CCSprite
{
	public :

		static SneakyJoystickSkinnedBase*	create ( KDvoid );

	public :

		virtual KDbool				init ( KDvoid );

		virtual KDvoid				setContentSize ( const CCSize& tContentSize );

		virtual KDvoid				updatePositions ( KDfloat fDelta );

		virtual CCSprite*			getBackgroundSprite ( KDvoid );
		virtual KDvoid				setBackgroundSprite ( CCSprite* pSprite );
		
		virtual CCSprite*			getThumbSprite ( KDvoid );
		virtual KDvoid				setThumbSprite ( CCSprite* pSprite );

		virtual SneakyJoystick*		getJoystick ( KDvoid );
		virtual KDvoid				setJoystick ( SneakyJoystick* pJoystick );

	protected :

		CCSprite*					m_pBackgroundSprite;
		CCSprite*					m_pThumbSprite;
		SneakyJoystick*				m_pJoystick;
};

NS_CC_END

#endif	// __SneakyJoystickSkinnedBase_h__