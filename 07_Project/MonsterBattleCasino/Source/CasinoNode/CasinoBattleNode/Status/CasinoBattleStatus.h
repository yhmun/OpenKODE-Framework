/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoBattleStatus.h
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

#ifndef __CasinoBattleStatus_h__
#define __CasinoBattleStatus_h__

#include "Object/Label/Labels.h"
#include "Object/Label/LabelAtlasCustom.h"
#include "Object/Sprite/Sprite.h"
#include "Unit/Unit/Unit.h"

class cCasinoBattleStatus : public Object
{
	public :
				 cCasinoBattleStatus ( Node* pNode, cUnit* pBetMob, eModeType eModeType );
		virtual ~cCasinoBattleStatus ( KDvoid );

		KDvoid					update ( cUnit* pBetMob, KDdouble dLeftTime );

	private :

		// node
		Node*					m_pNode;

		// sprite
		cSprite*				m_pSprMobNum;

		// strings
		std::string				m_sName;
		std::string				m_sMagnification;
		std::string				m_sWinPercent;
		std::string				m_sBetMoney;
		std::string				m_sDividendMoney;

		//labels
		cLabelAtlasCustom*		m_pLabelMoney;
		cLabels*				m_pLabels;
		LabelTTF*				m_pLabelBetMobName;
		LabelTTF*				m_pLabelBetMobMagnification;
		cLabelAtlasCustom*		m_pLabelBetMobBetMoney;
		cLabelAtlasCustom*		m_pLabelBetMobDividendMoney;

		// backup
		KDint64					m_nMoneyBackup;

		// mode type
		eModeType				m_eModeType;
};

#endif	// __CasinoBattleStatus_h__
