/* -----------------------------------------------------------------------------------
 *
 *      File            MyController.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __MyController_h__
#define __MyController_h__

#include "Player.h"
#include "PlayerHeadUI.h"

class MyController : public Layer
{
	public :

		public:
		
		/**
		 *  create a joy stick and two buttons
		 */
		static MyController*	create ( KDvoid );

		virtual KDbool			init ( KDvoid );

		~MyController ( KDvoid ) { }

		KDvoid					bindPlayer ( Player* pPlayer );

	private :

		JoyStick*				createJoyStick ( KDvoid );
		UILayer*				createButton ( KDvoid );

		/**
		 *  call back for button
		 */
		KDvoid					touchEvent ( Object* pSender, TouchEventType eType );
		KDvoid					bindPlayerStateMachine ( KDvoid );

		Player*					m_pPlayer;
		PlayerStateMachine*		m_pPlayerStateMachine;
		JoyStick*				m_pJoyStick;
		PlayerHeadUI*			m_pHeadUI;
};

class MyJoyStickDelegate : public JoyStickDelegate
{
	public : 

		/**
		 *  Get joystick info update to use joystick
		 *  @param sender this JoyStick
		 *  @param angle  the angle of joystick button
		 *  @param direction the direction of joystick button
		 *  @param power the power of player to hold joystick
		 */
		virtual KDvoid			onJoyStickUpdate ( Node* pSender, KDfloat fAngle, Point tDirection, KDfloat fPower );

		/**
		 *  to active joystick
		 */
		virtual KDvoid			onJoyStickActivated ( Node* pSender );

		/**
		 *  to deactive joystick
		 */
		virtual KDvoid			onJoyStickDeactivated ( Node* pSender );
	
		KDvoid					bindPlayerStateMachine ( PlayerStateMachine* pMachine ) { m_pPlayerStateMachine = pMachine; }

	private :

		PlayerStateMachine*		m_pPlayerStateMachine;
};

#endif	// __MyController_h__