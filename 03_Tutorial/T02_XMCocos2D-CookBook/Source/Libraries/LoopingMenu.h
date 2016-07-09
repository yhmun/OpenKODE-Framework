/* --------------------------------------------------------------------------
 *
 *      File            LoopingMenu.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      João Caxaria on 5/29/09
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2009      Imaginary Factory. All rights reserved.
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

#ifndef __LoopingMenu_h__
#define __LoopingMenu_h__

class LoopingMenu : public CCMenu
{
	public :

		static  LoopingMenu*	create ( KDvoid );

	public :		

		virtual KDvoid			registerWithTouchDispatcher ( KDvoid );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );		

		KDvoid					alignItemsHorizontallyWithPadding ( KDfloat fPadding );

	private :

		KDvoid					updateAnimation ( KDvoid );

		KDvoid					moveItemsLeftBy  ( KDfloat fOffset );
		KDvoid					moveItemsRightBy ( KDfloat fOffset );

	private :

		KDfloat			m_fPaddingH;
		KDfloat			m_fLowerBound;
		KDfloat			m_fOffsetY;
		KDbool			m_bMoving;
		KDbool			m_bTouchDown;
		KDfloat			m_fAccelerometerVelocity;
};

#endif	// __LoopingMenu_h__
