/* --------------------------------------------------------------------------
 *
 *      File            SneakyJoystick.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2009/02/15
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010	Sneakyness, llc. All rights reserved.
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

#ifndef __SneakyJoystick_h__
#define __SneakyJoystick_h__

NS_CC_BEGIN

class SneakyJoystick : public CCNode, public CCTargetedTouchDelegate 
{
	public :

		static SneakyJoystick*	create ( const CCRect& tRect );

	public :

		virtual KDbool			initWithRect ( const CCRect& tRect );

		virtual KDvoid			onEnterTransitionDidFinish ( KDvoid );
		virtual KDvoid			onExit ( KDvoid );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		virtual const CCPoint&	getStickPosition ( KDvoid );

		virtual KDfloat			getDegrees ( KDvoid );

		virtual const CCPoint&	getVelocity ( KDvoid );

		virtual KDbool			 isAutoCenter ( KDvoid );
		virtual KDvoid			setAutoCenter ( KDbool bAutoCenter );

		virtual KDbool			 isDPad ( KDvoid );
		virtual KDvoid			setDPad ( KDbool bIsDPad );

		virtual KDbool			hasDeadZone ( KDvoid );
		virtual KDvoid			setDeadZone ( KDbool bHasDeadZone );

		virtual KDuint			getNumberOfDirections ( KDvoid );
		virtual KDvoid			setNumberOfDirections ( KDuint uNumberOfDirections );

		virtual KDfloat			getJoystickRadius ( KDvoid );
		virtual KDvoid			setJoystickRadius ( KDfloat fRadius );
		
		virtual KDfloat			getThumbRadius ( KDvoid );
		virtual KDvoid			setThumbRadius ( KDfloat fRadius );

		virtual KDfloat			getDeadRadius ( KDvoid );
		virtual KDvoid			setDeadRadius ( KDfloat fRadius );

	private :

		KDvoid					updateVelocity ( const CCPoint& tPoint );

	protected :

		CCPoint					m_tStickPosition;
		KDfloat					m_fDegrees;
		CCPoint					m_tVelocity;
		KDbool					m_bAutoCenter;
		KDbool					m_bIsDPad;
		KDbool					m_bHasDeadzone;
		KDuint					m_uNumberOfDirections;
		KDfloat					m_fJoystickRadius;			
		KDfloat					m_fJoystickRadiusSq;
		KDfloat					m_fThumbRadius;
		KDfloat					m_fThumbRadiusSq;
		KDfloat					m_fDeadRadius;
		KDfloat					m_fDeadRadiusSq;
};

NS_CC_END

#endif	// __SneakyJoystick_h__
