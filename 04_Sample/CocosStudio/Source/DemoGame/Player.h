/* -----------------------------------------------------------------------------------
 *
 *      File            Player.h
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

#ifndef __Player_h__
#define __Player_h__

#define PLAYER_MAX_HP		100

class PlayerStateMachine
{
	public :

		 PlayerStateMachine ( KDvoid );
		~PlayerStateMachine ( KDvoid );

		KDint					getState ( KDvoid );
		Point					getMovePosition ( KDvoid ) { return m_tMovePosition; }
		KDvoid					reset ( KDvoid ) { m_nPlayerState = 0; }

		KDvoid					updateMoving ( Point tDirection ) { m_tMovePosition = tDirection; }
		KDvoid					updateActiveJoystick ( KDbool bPressJoystick ) { m_bPressJoystick = bPressJoystick; }
		KDvoid					updatePressButton ( KDbool bPressButton ) { m_bPressButton = bPressButton; }

	private :

		KDint					m_nPlayerState;

		KDbool					m_bPressJoystick;
		KDbool					m_bPressButton;
		Point					m_tMovePosition;
};


class Player : public Layer
{
	public :

		static Player*			create ( KDvoid );

		~Player ( KDvoid );

		virtual KDbool			init ( KDvoid );
		virtual KDvoid			update ( KDfloat fDelta );

		KDvoid					beHitted ( KDvoid );

		KDint					getHPPercent ( KDvoid ) { return m_nHitPonts; }
		KDint					getLifeNum ( KDvoid ) { return m_nLifeNum; }

		Armature*				getArmature ( KDvoid ) { return m_pPlayerArmature; }
		PlayerStateMachine*		getStateMachine ( KDvoid ) { return m_pPlayerStateMachine; }
		KDbool					IsGameOver ( KDvoid ) { return m_bIsGameOver; }

	private :

		KDvoid					updatePosition ( KDvoid );
		KDvoid					updateAnimation ( KDvoid );
		KDvoid					initArmature ( KDvoid );
		KDvoid					playerRebirth ( KDvoid );
		KDvoid					animationEvent ( Armature* pArmature, MovementEventType eMovementType, const KDchar* pMovementID );

		Armature*				m_pPlayerArmature;
		PlayerStateMachine*		m_pPlayerStateMachine;

		KDbool					isLock ( KDvoid );

		KDvoid					setGameOver ( KDbool bOver ) { m_bIsGameOver = bOver; }
		KDbool					m_bIsGameOver;

		KDint					m_nLifeNum;
		KDint					m_nHitPonts;
		KDint					m_nPlayerState;
};
#endif