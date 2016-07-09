/* --------------------------------------------------------------------------
 *
 *      File            SneakyButton.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2010/02/10
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sneakyness, llc. All rights reserved.
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

#ifndef __SneakyButton_h__
#define __SneakyButton_h__

NS_CC_BEGIN

class SneakyButton : public CCNode, public CCTargetedTouchDelegate
{
	public :

		static SneakyButton*	create ( const CCRect& tRect );
	
		static SneakyButton*	create ( KDvoid );

	public :

		virtual KDbool		initWithRect ( const CCRect& tRect );

		virtual KDvoid		onEnterTransitionDidFinish ( KDvoid );
		virtual KDvoid		onExit ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid		limiter ( KDfloat fDelta );

		virtual KDfloat		getRadius ( KDvoid );
		virtual KDvoid		setRadius ( KDfloat fRadius );

		virtual KDbool		getStatus ( KDvoid );
		virtual KDvoid		setStatus ( KDbool bStatus );

		virtual KDbool		getValue ( KDvoid );

		virtual KDbool		 isActive ( KDvoid );

		virtual KDbool		 isHoldable ( KDvoid );
		virtual KDvoid		setHoldable ( KDbool bIsHoldable );

		virtual KDbool		 isToggleable ( KDvoid );
		virtual KDvoid		setToggleable ( KDbool bIsToggleable );

		virtual KDfloat		getRateLimit ( KDvoid );
		virtual KDvoid		setRateLimit ( KDfloat fRateLimit );

	protected :

		CCPoint				m_tCenter;
		KDfloat				m_fRadius;
		KDfloat				m_fRadiusSq;
		CCRect				m_tBounds;
		KDbool				m_bStatus;
		KDbool				m_bActive;		
		KDbool				m_bValue;
		KDbool				m_bIsHoldable;
		KDbool				m_bIsToggleable;
		KDfloat				m_fRateLimit;
};

NS_CC_END

#endif	// __SneakyButton_h__