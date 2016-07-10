/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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

#ifndef __GameLayer_h__
#define __GameLayer_h__

class GameLayer : public CCLayer
{
    public :

		CREATE_FUNC ( GameLayer );

		         GameLayer ( KDvoid );
	    virtual ~GameLayer ( KDvoid );

	private :
	    
		virtual KDbool		init			( KDvoid );
		virtual KDvoid		ccTouchesBegan	( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved	( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded	( CCSet* pTouches, CCEvent* pEvent );

	private :

		KDvoid	    createBackground		( KDvoid );
		KDvoid      createStartTitle		( KDvoid );
		KDvoid      createPlayer			( KDvoid );
		KDvoid      createPlayerAnimation	( KDvoid );
		KDvoid      createLabel				( KDvoid );
		KDvoid		createBtn				( KDvoid );
		KDvoid		createShield			( KDvoid );
		KDvoid      createCrater			( KDvoid );
	

		KDvoid      onMovingPlayer			( KDfloat fDelta );
		KDvoid		onGameLogic				( KDfloat fDelta );
		KDvoid		onCollision				( KDfloat fDelta );
		KDvoid      onUpdateTimeLabel		( KDfloat fDelta );

		KDvoid		addDrops				( KDint nTagCrater );
		KDvoid		addShield				( KDvoid );

		KDvoid		removeDrops				( CCNode* pSender );
		KDvoid		removeShield			( CCNode* pSender );

		KDvoid		shakeScreen				( KDvoid );
		KDvoid		gameOver				( KDvoid );
		
		KDvoid		onRetry					( CCObject* pObject );
		KDvoid		onMenu					( CCObject* pObject );
		KDvoid      replaceScene			( CCLayer* pLayer );
		
		KDvoid		playPauseMenuCallback	( CCObject* pSenser );
		KDvoid		playStopGame			( KDbool bGamePause );

		KDvoid		soundOnOffMenuCallback	( CCObject* pSenser );

	private :

		CCSize	            m_tLyrSize;
		CCPoint			    m_tPlayerVel;

		KDint			    m_nTouchID;

		CCSprite*		    m_pPlayer;
		CCAnimate*		    m_pPlayerRunAnimate;		
		CCAnimate*		    m_pPlayerDieAnimate;

		CCParticleSystem*   m_pCraterLeft;
		CCParticleSystem*	m_pCraterCenter;
		CCParticleSystem*   m_pCraterRight;

		CCLabelTTF*		    m_pScoreLabel;
		CCLabelTTF*		    m_pHighScoreLabel;
		CCLabelTTF*		    m_pTimeLabel;
		
		KDint			    m_nScore;
		KDint			    m_nTime;
		KDint               m_nDropPercent;
		KDint				m_nLevel;
		KDbool              m_bGameOver;

		CCArray*		    m_pDrops;

		CCArray*		    m_pShields;
		CCSprite*		    m_pShield;
		KDbool			    m_bShieldOn;

		CCMenuItemImage*	m_pPlayMenuItem;
		CCMenuItemImage*	m_pPauseMenuItem;

		CCMenuItemImage*	m_pSoundOnMenuItem;
		CCMenuItemImage*	m_pSoundOffMenuItem;
};

#endif // __GameLayer_h__
