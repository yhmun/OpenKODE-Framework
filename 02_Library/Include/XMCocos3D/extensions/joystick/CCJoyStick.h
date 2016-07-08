/* --------------------------------------------------------------------------
 *
 *      File            CCJoyStick.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Solo on 13-8-14
 *      More info from  http://www.cocoachina.com/bbs/read.php?tid=122995
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft.  
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

#ifndef __CCJoyStick_h__
#define __CCJoyStick_h__

#include "../ExtensionMacros.h"
#include "../../2d/sprite_nodes/CCSprite.h"
#include "../../2d/layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_EXT_BEGIN

/**
 *  to build a new class for joystick
 *  then you can define joystick function
 */
class JoyStickDelegate 
{
	public :

		/**
		 *  Get joystick info update to use joystick
		 *  @param sender this CCJoyStick
		 *  @param angle  the angle of joystick button
		 *  @param direction the direction of joystick button
		 *  @param power the power of player to hold joystick
		 */
		virtual KDvoid		onJoyStickUpdate ( Node* pSender, KDfloat fAngle, Point tDirection, KDfloat fPower ) = 0;

		/**
		 *  to active joystick
		 */
		virtual KDvoid		onJoyStickActivated ( Node* pSender ) { CC_UNUSED_PARAM ( pSender ); }

		/**
		 *  to deactive joystick
		 */
		virtual KDvoid		onJoyStickDeactivated ( Node* pSender ) { CC_UNUSED_PARAM ( pSender ); }
};

class JoyStick: public Layer 
{
	public :

		/**
		 *	create joystick return CCJoyStick*
		 *	@param ballradius is the radius of joystick
		 *	@param movearearadius is the area of joystick
		 *	@param isfollowtouch true,joystick will change position when touch
		 *	@param iscanvisible true,you can see the joystick
		 *	@param isautohide true,joystick will hide when not using
		 *	@param hasanimation,define if joystick hava animation
		 */
		static JoyStick*	create ( KDint nBallRadius, KDint nMoveAreaRadius,
								     KDbool bIsFollowTouch = true, KDbool bIsCanVisible = true,
								     KDbool bIsAutoHide = false, KDbool bHasAnimation = false );

		KDbool				init ( KDint nBallRadius, KDint nMoveAreaRadius, KDbool bIsFollowTouch, KDbool bIsCanVisible, KDbool bIsAutoHide, KDbool bHasAnimation );
    
		KDvoid				setDelegate ( const JoyStickDelegate* pDelegate )
		{
			m_pDelegate = const_cast<JoyStickDelegate*> ( pDelegate );
		}

		/**
		 * set joystick button texture
		 */
		KDvoid				setBallTexture ( const KDchar* pImageName );

		/**
		 *  set joystick base texture
		 */
		KDvoid				setStickTexture ( const KDchar* pImageName );

		/**
		 *  set joystick background texture
		 */
		KDvoid				setDockTexture ( const KDchar* pImageName );

	public :

				 JoyStick ( KDvoid );
		virtual ~JoyStick ( KDvoid );

	private :

		// joystick info
		Sprite*				m_pBall;
		Sprite*				m_pStick;
		Sprite*				m_pDock;
		KDint				m_nMoveAreaRadius;
		KDint				m_nBallRadius;
		KDbool				m_bIsFollowTouch;
		KDbool				m_bIsCanVisible;
		KDbool				m_bIsAutoHide;
		Rect				m_tActiveRect;
		KDint				m_nActiveRadius;
		Point				m_tCurrentPoint;
		KDbool				m_bIsTouched;
		KDbool				m_bHasAnimation;
		KDfloat				m_fPower;
		KDfloat				m_fAngle;
		Point				m_tDirection;
		JoyStickDelegate*	m_pDelegate;

	private :

		KDvoid				setHitAreaWithRadius ( KDint nRadius );
		KDvoid				setHitAreaWithRect ( Rect tRect );
		KDvoid				startTimer ( KDvoid );
		KDvoid				stopTimer ( KDvoid );
		KDvoid				timerUpdate ( KDvoid );
		KDvoid				timerUpdate ( KDfloat dt );
		KDvoid				touchBegan ( Point tTouchPoint );
		KDvoid				resetTexturePosition ( KDvoid );
		KDbool				containsTouchLocation ( Touch* pTouch );
		KDvoid				updateTouchPoint ( Point tTouchPoint );

	public :

		virtual KDvoid		onEnter ( KDvoid );
		virtual KDvoid		onExit ( KDvoid );

		virtual KDbool		onTouchBegan ( Touch* pTouch, Event* pEvent );
		virtual KDvoid		onTouchMoved ( Touch* pTouch, Event* pEvent );
		virtual KDvoid		onTouchEnded ( Touch* pTouch, Event* pEvent );
};

NS_CC_EXT_END

#endif	// __CCJoyStick_h__

