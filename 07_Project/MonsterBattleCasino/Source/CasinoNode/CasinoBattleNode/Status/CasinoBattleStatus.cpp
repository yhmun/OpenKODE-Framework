/* -----------------------------------------------------------------------------------
 *
 *      File            CasinoBattleStatus.cpp
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

#include "Precompiled.h"
#include "CasinoBattleStatus.h"
#include "MyData/MyData.h"
#include "Object/TimeGetMoney/TimeGetMoney.h"

cCasinoBattleStatus::cCasinoBattleStatus ( Node* pNode, cUnit* pBetMob, eModeType eMode )
: m_pNode						( pNode							)
, m_pSprMobNum					( new cSprite ( pNode )			)
, m_sName						( ""							)
, m_sMagnification				( ""							)
, m_sWinPercent					( ""							)
, m_sBetMoney					( ""							)
, m_sDividendMoney				( ""							)
, m_pLabelMoney					( nullptr						)
, m_pLabels						( new cLabels ( pNode )			)
, m_pLabelBetMobName			( nullptr						)
, m_pLabelBetMobMagnification	( nullptr						)
, m_pLabelBetMobBetMoney		( nullptr						)
, m_pLabelBetMobDividendMoney	( nullptr						)
, m_nMoneyBackup				( cMyData::getObj ( )->m_nMoney )
, m_eModeType					( eMode							)
{
	if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
	{
		// sprite
		switch ( pBetMob->getSeatIndex ( ) )
		{
			case 0	:	m_pSprMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_GAMEVIEW_INFO, Point ( 28, 11 ) );	m_pSprMobNum->addFrame ( Rect (  0, 0, 21, 21 ) );	break;
			case 1	:	m_pSprMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_GAMEVIEW_INFO, Point ( 28, 11 ) );	m_pSprMobNum->addFrame ( Rect ( 21, 0, 21, 21 ) );	break;
			case 2	:	m_pSprMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_GAMEVIEW_INFO, Point ( 28, 11 ) );	m_pSprMobNum->addFrame ( Rect ( 42, 0, 21, 21 ) );	break;
			case 3	:	m_pSprMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_GAMEVIEW_INFO, Point ( 28, 11 ) );	m_pSprMobNum->addFrame ( Rect ( 63, 0, 21, 21 ) );	break;
			case 4	:	m_pSprMobNum->initTexture ( "Bet/bet_mob_num.png", LAYER_GAMEVIEW_INFO, Point ( 28, 11 ) );	m_pSprMobNum->addFrame ( Rect ( 84, 0, 21, 21 ) );	break;
		}

		m_pSprMobNum->setFrame ( 0 );
		m_pSprMobNum->get ( )->setVisible ( true );

		// bet mob name
		Rect	tRectName = Rect ( 187, 41, 150, 20 );
		m_sName = pBetMob->getName ( ).c_str ( );
		m_pLabelBetMobName = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::CENTER, tRectName, m_sName.c_str ( ), LAYER_GAMEVIEW_INFO );

		// dividend Magnification
		Rect	tRectMagnification = Rect ( 307, 41, 50, 20 );
		KDint	nMagnification1 = pBetMob->getDividendMagnification();
		KDint	nMagnification2 = (KDint) ( pBetMob->getDividendMagnification ( ) * 10 ) % 10;
		m_sMagnification = ccszf ( "x  %d.%d", nMagnification1, nMagnification2 );
		m_pLabelBetMobMagnification	= m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::CENTER, tRectMagnification, m_sMagnification.c_str ( ), LAYER_GAMEVIEW_INFO );

		// betting money
		m_sBetMoney = ccszf ( "%lld", pBetMob->getBetMoney ( ) );
		for ( KDint i = m_sBetMoney.size ( ) - 3; i > 0; i -= 3 )	m_sBetMoney.insert ( i, "," );					
		m_sBetMoney += " C";
		m_pLabelBetMobBetMoney = new cLabelAtlasCustom ( m_pNode, "Game/game_money_num.png", 11, LAYER_GAMEVIEW_INFO );
		m_pLabelBetMobBetMoney->addCharInfo ( ',', 3 );	m_pLabelBetMobBetMoney->addCharInfo ( '0', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( '1', 4 );
		m_pLabelBetMobBetMoney->addCharInfo ( '2', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( '3', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( '4', 6 );
		m_pLabelBetMobBetMoney->addCharInfo ( '5', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( '6', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( '7', 6 );
		m_pLabelBetMobBetMoney->addCharInfo ( '8', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( '9', 6 );	m_pLabelBetMobBetMoney->addCharInfo ( ' ', 4 );
		m_pLabelBetMobBetMoney->addCharInfo ( 'C', 8 );
		m_pLabelBetMobBetMoney->setString ( m_sBetMoney.c_str ( ) );
		m_pLabelBetMobBetMoney->setPosition ( Point ( 304 - (KDint) ( m_pLabelBetMobBetMoney->getSize ( ).width / 2 ), 24 ) );

		// betting dividend money
		KDfloat		fMagnification = pBetMob->getDividendMagnification ( ) - 1;
		KDdouble	dPlusMoney = pBetMob->getBetMoney ( ) * fMagnification;

		if ( dPlusMoney >= (KDdouble) ( (KDint64) dPlusMoney ) + 0.1f )	dPlusMoney = (KDint64) dPlusMoney + 1;
		else															dPlusMoney = (KDint64) dPlusMoney;

		KDint64		dDividendMoney	= pBetMob->getBetMoney ( ) + dPlusMoney;
		m_sDividendMoney = ccszf ( "%lld", dDividendMoney );
		for ( KDint i = m_sDividendMoney.size ( ) - 3; i > 0; i -= 3 )	m_sDividendMoney.insert ( i, "," );	
		m_sDividendMoney += " C";

		m_pLabelBetMobDividendMoney = new cLabelAtlasCustom ( m_pNode, "Game/game_money_num.png", 11, LAYER_GAMEVIEW_INFO );
		m_pLabelBetMobDividendMoney->addCharInfo ( ',', 3 );	m_pLabelBetMobDividendMoney->addCharInfo ( '0', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( '1', 4 );
		m_pLabelBetMobDividendMoney->addCharInfo ( '2', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( '3', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( '4', 6 );
		m_pLabelBetMobDividendMoney->addCharInfo ( '5', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( '6', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( '7', 6 );
		m_pLabelBetMobDividendMoney->addCharInfo ( '8', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( '9', 6 );	m_pLabelBetMobDividendMoney->addCharInfo ( ' ', 4 );
		m_pLabelBetMobDividendMoney->addCharInfo ( 'C', 8 );
		m_pLabelBetMobDividendMoney->setString ( m_sDividendMoney.c_str ( ) );
		m_pLabelBetMobDividendMoney->setPosition ( Point ( 304 - (KDint) ( m_pLabelBetMobDividendMoney->getSize ( ).width / 2 ), 8 ) );
	}		

	// label money
	m_pLabelMoney = new cLabelAtlasCustom ( m_pNode, "Bet/coin_nums_s.png", 15, LAYER_GAMEVIEW_INFO );
	m_pLabelMoney->addCharInfo ( ',',  7 );	m_pLabelMoney->addCharInfo ( '0', 15 );	m_pLabelMoney->addCharInfo ( '1', 15 );
	m_pLabelMoney->addCharInfo ( '2', 15 );	m_pLabelMoney->addCharInfo ( '3', 15 );	m_pLabelMoney->addCharInfo ( '4', 15 );
	m_pLabelMoney->addCharInfo ( '5', 15 );	m_pLabelMoney->addCharInfo ( '6', 15 );	m_pLabelMoney->addCharInfo ( '7', 15 );
	m_pLabelMoney->addCharInfo ( '8', 15 );	m_pLabelMoney->addCharInfo ( '9', 15 );

	std::string		sStr = ccszf ( "%lld", cMyData::getObj ( )->m_nMoney + pBetMob->getBetMoney ( ) );
	for ( KDint i = sStr.size ( ) - 3; i > 0; i -= 3 )	sStr.insert ( i, "," );	

	m_pLabelMoney->setString ( sStr.c_str ( ) );
	m_pLabelMoney->setPosition ( Point ( 459 - m_pLabelMoney->getSize ( ).width, 300 ) );

	// time get money
	cTimeGetMoney::getObj ( )->changeNode ( m_pNode, LAYER_GAMEVIEW_INFO, Rect ( 432 - m_pLabelMoney->getSize ( ).width, 302, 50, 15 ) );
}

cCasinoBattleStatus::~cCasinoBattleStatus ( KDvoid )
{
	// sprite
	m_pSprMobNum = nullptr;

	// labels
	CC_SAFE_RELEASE ( m_pLabelMoney	);
	CC_SAFE_RELEASE ( m_pLabels );

	m_pLabelBetMobName			= nullptr;
	m_pLabelBetMobMagnification = nullptr;

	CC_SAFE_RELEASE ( m_pLabelBetMobBetMoney );
	CC_SAFE_RELEASE ( m_pLabelBetMobDividendMoney );
}

KDvoid cCasinoBattleStatus::update ( cUnit* pBetMob, KDdouble dLeftTime )
{
	// time get money
	KDint64		nMoney = cMyData::getObj ( )->m_nMoney + pBetMob->getBetMoney ( );
	cTimeGetMoney::getObj ( )->update ( dLeftTime, nMoney );
	cMyData::getObj ( )->m_nMoney = nMoney - pBetMob->getBetMoney ( );

	// money
	if ( m_nMoneyBackup != cMyData::getObj ( )->m_nMoney )
	{
		std::string		sStr = ccszf ( "%lld", cMyData::getObj ( )->m_nMoney + pBetMob->getBetMoney ( ) );
		for ( KDint i = sStr.size ( ) - 3; i > 0; i -= 3 )	sStr.insert ( i, "," );	

		m_pLabelMoney->setString ( sStr.c_str ( ) );
		m_pLabelMoney->setPosition ( Point ( 459 - m_pLabelMoney->getSize ( ).width, 300 ) );

		m_nMoneyBackup = cMyData::getObj ( )->m_nMoney;

		// get money
		cTimeGetMoney::getObj ( )->setPoint ( Point ( 432 - m_pLabelMoney->getSize ( ).width, 302 ) );
	}
}