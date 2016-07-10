/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoResultNode.h
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

#ifndef __CasinoResultNode_h__
#define __CasinoResultNode_h__

#include "Object/Label/LabelAtlasCustom.h"
#include "Object/Label/LabelAtlas.h"
#include "Object/Label/Labels.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/NodeChageImgs/NodeChangeImgs.h"
#include "Unit/Unit/Unit.h"

class cResultLevelUp : public Object
{
	public :
				 cResultLevelUp ( Node* pNode, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt );
		virtual ~cResultLevelUp ( KDvoid );

		KDvoid							start ( KDvoid );
		inline KDbool					getIsStart ( KDvoid ) { return m_bIsStart; }

		KDint							update ( KDdouble dLeftTime );		// return 1 == hp, return 2 == atk, return 3 == def, return 4 == agl, return 5 == crt ë°”ë€???

		inline KDint					getHp  ( KDvoid ) { return m_nHp;  }
		inline KDint					getAtk ( KDvoid ) { return m_nAtk; }
		inline KDint					getDef ( KDvoid ) { return m_nDef; }
		inline KDint					getAgl ( KDvoid ) { return m_nAgl; }
		inline KDint					getCrt ( KDvoid ) { return m_nCrt; }

	private :

		cSprite*						m_pUILevelUp;
		cSprAni*						m_pUIStatusHp;
		cSprAni*						m_pUIStatusAtk;
		cSprAni*						m_pUIStatusDef;
		cSprAni*						m_pUIStatusAgl;
		cSprAni*						m_pUIStatusCrt;
		KDbool							m_bIsStart;
		KDbool							m_bIsLevelAlphaAdd;
		KDdouble						m_dLevelUpLeftTime;
		KDdouble						m_dStatusLeftTime;
		KDint							m_nStatusStep;
		KDint							m_nHp;
		KDint							m_nAtk;
		KDint							m_nDef;
		KDint							m_nAgl;
		KDint							m_nCrt;
};

class CasinoResultNode : public Scene
{
	public :
				 CasinoResultNode ( KDvoid );
		virtual ~CasinoResultNode ( KDvoid );

		static CasinoResultNode*		createWithBetMob ( cUnit* pBetMob, eModeType eType );
		static CasinoResultNode*		createWithFileLoad ( eIntoType eIntro );

		static KDvoid					saveData ( KDvoid );

	public :

		KDbool							init ( KDvoid );
		KDbool							initWithBetMob ( cUnit* pBetMob, eModeType eType );
		KDbool							initWithFileLoad ( eIntoType eIntro );

		KDvoid							step ( KDfloat fDelta );

		// set
		KDvoid							setBetMob ( cUnit* pBetMob, eModeType eMode );
		KDvoid							setFileLoad ( eIntoType eInto );

		// init
		KDvoid							initUI ( KDvoid );
		KDvoid							initFinish ( KDvoid );

		KDvoid							releaseObject ( KDvoid );

		// openfeint
		KDvoid							updateOpenFeint ( KDvoid );

		// exp
		KDvoid							plusExp ( KDfloat fPlusExp );

		// level
		KDvoid							levelUp ( KDvoid );

		// my data
		KDvoid							updateMyData ( KDvoid );
		KDvoid							updateMyRoomData ( KDvoid );

		// change node
		KDvoid							sceneGoBattle ( cUnit* pBetMob );

		// save
		KDvoid							save ( KDvoid );

		// update
		KDbool							updateNodeChage ( KDdouble dLeftTIme );
		KDvoid							updateExp	    ( KDdouble dLeftTIme );
		KDvoid							updateLevelUp   ( KDdouble dLeftTIme );
		KDvoid							updateMoney	    ( KDdouble dLeftTIme );
		
		KDvoid							exitCallback	 ( Object* pSender );
		KDvoid							retryCallback	 ( Object* pSender );
		KDvoid							faceBookCallback ( Object* pSender );		
		KDvoid							twitterCallback  ( Object* pSender );		
		KDvoid							rankCallback     ( Object* pSender );		
		
	private :

		// touch
		KDint							m_nTidCount;
		std::list<sTouch>				m_aListTouch;

		// time
		KDdouble						m_dTickCount;
		KDdouble						m_dBeforeTickCount;

		// node change
		KDbool							m_bIsNodeChangeIn;
		KDbool							m_bIsNodeChangeOut;
		KDbool							m_bIsGoBattle;
		cNodeChangeImgs*				m_pNodeChangeImg;

		// back
		Sprite*							m_pSprBack;

		// back decorations
		std::vector<cSprAni*>			m_aBackDecos;

		// mobs
		cUnit*							m_pBetMob;

		// labels
		cLabels*						m_pLabels;

		// result back
		cSprite*						m_pSprResultBack;

		// top money
		cLabelAtlasCustom*				m_pLabelTopMoney;

		// result text
		cSprite*						m_pSprResultText;

		//result money
		cLabelAtlasCustom*				m_pLabelEarnMoney;
		cLabelAtlasCustom*				m_pLabelResultMoney;

		// bar UI
		cSprite*						m_pUIMobBar;
		cLabelAtlas*					m_pUIMobBarMagnification;
		cLabelAtlas*					m_pUIMobBarMagnification2;
		cSprite*						m_pUIMobBarMagnification3;
		LabelTTF*						m_pUIMobBarHP;
		LabelTTF*						m_pUIMobBarATK;
		LabelTTF*						m_pUIMobBarDEF;

		// bar exp UI
		cSprite*						m_pUIBarExpGauge1;
		cSprite*						m_pUIBarExpGauge2;
		cSprite*						m_pUIBarExpFont1;
		cLabelAtlasCustom*				m_pUIBarExpFont2;

		// bar kill UI
		cLabelAtlasCustom*				m_pUIBarKill;

		// detail UI
		cSprite*						m_pUIMobNum;
		std::vector<LabelTTF*>			m_aUIMobDetailPara;		// [0] == hp, [1] == atk, [2] == def, [3] == agl, [4] == crt
		std::vector<cSprAni*>			m_aUISkillIcons;
		std::vector<LabelTTF*>			m_aUISkillStrs;
		cSprite*						m_pUIExpBack;
		cSprite*						m_pUIExpGauge;
		cSprite*						m_pUIExpFont;

		// level up UI
		std::vector<cResultLevelUp*>	m_aUILevels;

		// successive UI
		cLabelAtlasCustom*				m_pUISuccessiveNum;

		// btns
		Menu*							m_pBtnExit;
		Menu*							m_pBtnRetry;
		Menu*							m_pBtnRank;

		// money
		KDint64							m_nMoney;
		KDint64							m_nMoneyBackup;
		KDint64							m_nMoneyBackup2;
		KDint64							m_nMoneyEarn;
		KDint64							m_nMoneyEarnBackup;
		KDdouble						m_dMoneyWaitLeftTime;
		KDdouble						m_dMoneyLeftTimeBackup;
		KDdouble						m_dMoneyLeftTime;
		KDdouble						m_dMoneySoundLeftTime;
		KDbool							m_bIsMoneySoundFinish;

		// ani
		KDdouble						m_dAniStartLeftTime;

		// mode type
		eModeType						m_eModeType;

		// exp
		KDfloat							m_fStartExp;
		KDfloat							m_fNowExp;
		KDfloat							m_fAfterExp;

		// kill
		KDint							m_nStartKillCount;
		KDint							m_nAfterKillCount;
};

#endif	// __CasinoResultNode_h__