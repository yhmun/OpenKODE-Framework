/* --------------------------------------------------------------------------
 *
 *      File            MainScene.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/27 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#ifndef __MainScene_h__
#define __MainScene_h__

class MainScene : public KWScene 
{
	public :

		SCENE_FUNC ( MainScene );

	public :

				 MainScene ( KDvoid );
		virtual ~MainScene ( KDvoid );

	protected :

		virtual KDbool		init ( KDvoid );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid		update	( KDfloat fDelta );

		KDvoid				playMusic ( KDvoid );

		KDvoid				startGame ( KDvoid );

		KDvoid				hurryUp ( KDvoid );

		KDvoid				endGame ( KDvoid );

		KDvoid				showResult ( KDvoid );

		KDvoid				popTarget ( KDvoid );

		KDvoid				shakeScreen ( KDvoid );

		KDvoid				getScore ( KDint nScore );

		KDvoid				pressRetryButton ( CCObject* pSender );

		KDvoid				pressReturnButton ( CCObject* pSender );

	private :

		KDvoid				onGameOver ( KDvoid );

		KDvoid				onShowResult ( KDfloat fDelta );

	private :

		KDint				m_nScore;
		KDint				m_nHighScore;
		KDbool				m_bActive;
		KDbool				m_bHurryUp;
		CCArray*			m_pTargets;
		CCLabelTTF*			m_pScoreLabel;
		CCLabelTTF*			m_pHighScoreLabel;
		KWTimerLabel*		m_pTimerLabel;
};

#endif	// __MainScene_h__