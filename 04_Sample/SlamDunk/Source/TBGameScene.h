/* --------------------------------------------------------------------------
 *
 *      File            TBGameScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Rana Hammad Hussain on 04/22/11
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Azosh & Co. All rights reserved.
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

#ifndef __TBGameScene_h__
#define __TBGameScene_h__

class TBGameScene : public CCLayer
{
	public :

		SCENE_FUNC ( TBGameScene );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		didAccelerate ( CCAcceleration* pAccelerationValue );

		KDvoid				initGamePlayVariables ( KDvoid );

		KDvoid				addBall ( KDvoid );

		KDvoid				addBaskets ( KDvoid );

		KDvoid				addCharacters ( KDvoid );

		KDvoid				addGameObjects ( KDvoid );

		KDvoid				showInstructions ( KDvoid );

		KDvoid				showGoLabel ( KDvoid );

		KDvoid				selectCharacter ( KDvoid );

		KDvoid				menuClicked ( CCObject* pSender );
		
		KDvoid				instructionsEnded ( KDvoid );

		KDvoid				blueCharacterClicked ( CCObject* pSender );

		KDvoid				orangeCharacterClicked ( CCObject* pSender );

		KDvoid				yellowCharacterClicked ( CCObject* pSender );

		KDvoid				characterSelected ( CCObject* pSender );

		KDvoid				characterSelectedEnded ( KDvoid );	

		KDvoid				pause ( KDvoid );

		KDvoid				gameOver ( KDvoid );

		KDvoid				leftClicked ( CCObject* pSender );

		KDvoid				rightClicked ( CCObject* pSender );	

		KDvoid				updateGoLabel ( KDvoid );

		KDvoid				startClicked ( KDvoid );

		KDvoid				backClicked ( CCObject* pSender );

		KDvoid				nextClicked ( CCObject* pSender );

		KDvoid				updateTimer ( KDfloat fDelta );

		KDvoid				updateComputerMovements ( KDvoid );

		// game specific
		KDint				detectOverBasket ( KDvoid );
		
		KDbool				detectBallBoundaryCheck ( KDvoid );

		KDbool				detectBallPlayerCollision ( CCSprite* pPlayer );
		
		KDvoid				resetBallPosition ( KDvoid );
		
		KDvoid				updateBallPosition ( KDvoid );

		KDvoid				playerLanded ( KDvoid );

		KDvoid				computerLanded ( KDvoid );

		KDvoid				resumeClicked ( CCObject* pSender );

		KDvoid				restartClicked ( CCObject* pSender );

		KDvoid				shareClicked ( CCObject* pSender );

		CCLayer*			addScores ( const CCPoint& tCenter );
		
	private :

		KDbool				m_bPause;
		KDbool				m_bBallOverBasket;
		KDbool				m_bGameOver;
		CCPoint				m_tBallMovementDelta;
		KDfloat				m_fBallRadius;
		
		CCMenuItem*			m_pLeftControl;
		CCMenuItem*			m_pRightControl;
		CCMenu*				m_pBackMenu;
		
		CCSprite*			m_pGoLabel3;
		CCSprite*			m_pGoLabel2;
		CCSprite*			m_pGoLabel1;	
		CCSprite*			m_pGoLabelGO;
		
		KDint				m_nStartTime;
		KDint				m_nCharacterIdx;
		KDint				m_nComputerIdx;

		KDbool				m_bShowingGoLabel;
		KDbool				m_bComputerJumping;
		KDbool				m_bPlayerJumping;
		KDbool				m_bMoveLeft;
		KDbool				m_bMoveRight;
		KDint				m_nPlayerScore;
		KDint				m_nComputerScore;
		KDint				m_nTimer;
		
		CCLabelTTF*			m_pPlayerScoreLabel;
		CCLabelTTF*			m_pComputerScoreLabel;
		CCLabelTTF*			m_pTimerLabel;
		
		CCSprite*			m_pBall;
		CCSprite*			m_pPlayer;
		CCSprite*			m_pComputer;
		CCSprite*			m_pLeftBasket;
		CCSprite*			m_pRightBasket;
		
		// instructions
		CCSprite*			m_pInstructionsBg;
		CCSprite*			m_pCharactersBg;
		CCSprite*			m_pPlayerSelectionBg;
		CCMenu*				m_pMenuInstructions;
		CCMenu*				m_pMenuPlayerSelection;
			
		CCSprite*			m_pGameOverBg;
		CCSprite*			m_pPauseBg;
		CCSprite*			m_pSelectedCharacterBg;
};

#endif	// __TBGameScene_h__
