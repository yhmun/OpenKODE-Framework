/* -----------------------------------------------------------------------------------
 *
 *      File            Commander.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __Commander_h__
#define __Commander_h__

#include "Scene/JBase/JBaseGameScene.h"
#include "Game/Character/Char.h"

class cLabelAtlasCostom;
class cCharHelper;
class cFirewoodMgr;
class cCombo;
class cGameMode;
class cCommander : public Object
{
	public :

		struct sPlayerInitInfo
		{
			// char
			cChar::eType		eCharType;
			eLookAtType			eLookAt;
			Point				tCharPoint;
			Point				tCharHelper;
		
			// firewood
			Point				tFirewoodIntro;
			Point				tFirewoodPreview;
			Point				tFirewoodStandBy;
			Point				tFirewoodProduct;
		};

	private :

		struct sPlayer
		{
			KDint				nId;
			cChar*				pChar;
			cCharHelper*		pCharHelper;
			cFirewoodMgr*		pFirewoodMgr;

			KDbool				bIsSplitting;
			KDdouble			dSplittingDelay;
			KDdouble			dFirewoodDelay;
		};
	
	public :

		// init
		cCommander ( JBaseGameScene* pScene, KDint nStage, KDint nHouseLevel );
		virtual ~cCommander ( KDvoid );

		// player
		KDvoid					addPlayer ( const sPlayerInitInfo& tInfo );
		KDint					getPlayerNewID ( KDvoid );
		KDvoid					getPlayer ( KDint nId, sPlayer& tPlayer );

		// play
		KDvoid					playInit  ( KDvoid );
		KDvoid					playStart ( KDvoid );
		KDvoid					playStop  ( KDvoid );
		KDvoid					playEnd   ( KDvoid );
	
		// pose
		KDvoid					setPose ( KDbool bIsPose );

		// time mode
		KDbool					getIsTimeStopMode ( KDvoid );
	
		// result data
		inline KDbool			getIsPerfectSuccess	 ( KDvoid )	{ return m_bIsPerfectSuccess; }
		inline KDbool			getIsBerserkSuccess	 ( KDvoid )	{ return m_bIsBerserkSuccess; }
		inline KDbool			getIsTimeStopSuccess ( KDvoid )	{ return m_bIsTimeStopSuccess; }
		inline KDint			getMaxPowerCount	 ( KDvoid )	{ return m_nMaxPowerCount; }
	
		// message
		KDvoid					msgFirewoodPreviewStandby ( KDint nPlayerID );
		KDvoid					msgFirewoodProduct ( KDint nPlayerID, KDint nLevel );
		KDvoid					msgIsComboSuccess ( KDint nPlayerID, KDbool bIsSuccess );
		KDvoid					msgCloudAdd ( KDint z, KDdouble dStartDelay, Point tPoint, Point tAdd, KDdouble dLeftTime, KDfloat fScale, KDfloat fScaleAdd, KDfloat fRotation, KDfloat fRotationAdd, KDbool bIsFlipX );
		KDvoid					msgTimeModeIntro ( KDbool bIsTimeModeIntro );
		KDvoid					msgBerserkerModeIntro ( KDbool bIsBerserkerModeIntro );
		KDvoid					msgSprBackGray ( KDbool bIsGray );
		KDvoid					msgCharAngry ( KDbool bIsAngry );

		// touch
		KDvoid					touchBegan ( KDvoid );

		// update
		KDvoid					update ( KDdouble dLeftTime );

	private :

		// update
		KDvoid					updateIntro	( KDdouble dLeftTime );
		KDvoid					updateTimeModeIntro ( KDdouble dLeftTime );
		KDvoid					updateGame ( KDdouble dLeftTime );
	
		// scene
		JBaseGameScene*			m_pScene;
	
		// character
		std::list<sPlayer>		m_aPlayers;
	
		// gauge
		cGameMode*				m_pGameMode;

		// combo
		cCombo*					m_pCombo;

		// touch
		KDbool					m_bIsTouch;
	
		// play
		KDbool					m_bIsPlay;
	
		// pose
		KDbool					m_bIsPose;
	
		// time mode intro
		KDbool					m_bIsTimeModeIntro;
		KDbool					m_bIsBerserkerModeIntro;
	
		// result data
		KDbool					m_bIsPerfectSuccess;
		KDbool					m_bIsBerserkSuccess;
		KDbool					m_bIsTimeStopSuccess;
		KDint					m_nMaxPowerCount;
};

#endif	// __Commander_h__