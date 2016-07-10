/* -----------------------------------------------------------------------------------
 *
 *      File            MobCasinoStatus.cpp
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
#include "MobCasinoStatus.h"
#include "MyData/MyData.h"

cMobCasinoStatus::cMobCasinoStatus ( Node* pNode, const std::vector<cUnit*>& rMobs, eModeType eMode, KDint nBetSeatIndex )
: m_pNode		( pNode					)
, m_pLabels		( new cLabels ( pNode ) )
, m_eModeType	( eMode					)
{
	// loop
	for ( KDint i = 0; i < 5; i++ )
	{
		// label parameters
		sLabelInt	tLabelIntHp;
		sLabelInt	tLabelIntAtk;
		sLabelInt	tLabelIntDef;
		sLabelInt	tLabelIntAgl;
		sLabelInt	tLabelIntCrt;

		if ( rMobs [ i ] == nullptr )
		{
			tLabelIntHp .nSub = 0;
			tLabelIntAtk.nSub = 0;
			tLabelIntDef.nSub = 0;
			tLabelIntAgl.nSub = 0;
			tLabelIntCrt.nSub = 0;
		}
		else
		{
			tLabelIntHp .nSub = rMobs [ i ]->getUseHp  ( );
			tLabelIntAtk.nSub = rMobs [ i ]->getUseAtk ( );
			tLabelIntDef.nSub = rMobs [ i ]->getUseDef ( );
			tLabelIntAgl.nSub = rMobs [ i ]->getUseAgl ( );
			tLabelIntCrt.nSub = rMobs [ i ]->getUseCrt ( );
		}

		if ( rMobs [ i ] == nullptr )
		{
			Rect	tRectHP	 = Rect ( 0, 0, 0, 0 );
			Rect	tRectATK = Rect ( 0, 0, 0, 0 );
			Rect	tRectDEF = Rect ( 0, 0, 0, 0 );
			Rect	tRectAGL = Rect ( 0, 0, 0, 0 );
			Rect	tRectCRT = Rect ( 0, 0, 0, 0 );

			switch ( i )
			{
				case 0	:	tRectHP = Rect (  94, 277, 100, 20 );	tRectATK = Rect (  56, 263, 50, 20 );	tRectDEF = Rect (  99	, 263, 45, 20 );	tRectAGL = Rect ( 56, 248, 50, 20);	tRectCRT = Rect (  99	, 248, 45, 20 );	break;
				case 1	:	tRectHP = Rect ( 189, 277, 100, 20 );	tRectATK = Rect ( 151, 263, 50, 20 );	tRectDEF = Rect ( 194.5f, 263, 45, 20 );	tRectAGL = Rect (151, 248, 50, 20);	tRectCRT = Rect ( 194.5f, 248, 45, 20 );	break;
				case 2	:	tRectHP = Rect ( 284, 277, 100, 20 );	tRectATK = Rect ( 246, 263, 50, 20 );	tRectDEF = Rect ( 289.5f, 263, 45, 20 );	tRectAGL = Rect (246, 248, 50, 20);	tRectCRT = Rect ( 289.5f, 248, 45, 20 );	break;
				case 3	:	tRectHP = Rect ( 379, 277, 100, 20 );	tRectATK = Rect ( 341, 263, 50, 20 );	tRectDEF = Rect ( 384	, 263, 45, 20 );	tRectAGL = Rect (341, 248, 50, 20);	tRectCRT = Rect ( 384	, 248, 45, 20 );	break;
				case 4	:	tRectHP = Rect ( 474, 277, 100, 20 );	tRectATK = Rect ( 436, 263, 50, 20 );	tRectDEF = Rect ( 479	, 263, 45, 20 );	tRectAGL = Rect (436, 248, 50, 20);	tRectCRT = Rect ( 479	, 248, 45, 20 );	break;
			}

			tLabelIntHp .pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectHP , "", LAYER_GAMEVIEW_INFO );
			tLabelIntAtk.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectATK, "", LAYER_GAMEVIEW_INFO );
			tLabelIntDef.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectDEF, "", LAYER_GAMEVIEW_INFO );
			tLabelIntAgl.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectAGL, "", LAYER_GAMEVIEW_INFO );
			tLabelIntCrt.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectCRT, "", LAYER_GAMEVIEW_INFO );

			m_pLabels->setColor ( tLabelIntAtk.pLabel, 255, 255, 255 );
			m_pLabels->setColor ( tLabelIntDef.pLabel, 255, 255, 255 );
			m_pLabels->setColor ( tLabelIntAgl.pLabel, 255, 255, 255 );
			m_pLabels->setColor ( tLabelIntCrt.pLabel, 255, 255, 255 );
		}
		else
		{
			const KDchar*	szHP  = ccszf ( "%02d / %02d", rMobs [ i ]->getUseHp ( ), rMobs [ i ]->getHP ( ) );
			const KDchar*	szATK = ccszf ( "%02d", rMobs [ i ]->getUseAtk ( ) );
			const KDchar*	szDEF = ccszf ( "%02d", rMobs [ i ]->getUseDef ( ) );
			const KDchar*	szAGL = ccszf ( "%02d", rMobs [ i ]->getUseAgl ( ) );
			const KDchar*	szCRT = ccszf ( "%02d", rMobs [ i ]->getUseCrt ( ) );

			Rect	tRectHP  = Rect ( 0, 0, 0, 0 );
			Rect	tRectATK = Rect ( 0, 0, 0, 0 );
			Rect	tRectDEF = Rect ( 0, 0, 0, 0 );
			Rect	tRectAGL = Rect ( 0, 0, 0, 0 );
			Rect	tRectCRT = Rect ( 0, 0, 0, 0 );

			switch ( i )
			{
				case 0	:	tRectHP = Rect (  94, 277, 100, 20 );	tRectATK = Rect ( 56, 263, 50, 20);	tRectDEF = Rect (  99	 , 263, 45, 20 );	tRectAGL = Rect (  56, 248, 50, 20 );	tRectCRT = Rect (  99	, 248, 45, 20 );	break;
				case 1	:	tRectHP = Rect ( 189, 277, 100, 20 );	tRectATK = Rect (151, 263, 50, 20);	tRectDEF = Rect ( 194	 , 263, 45, 20 );	tRectAGL = Rect ( 151, 248, 50, 20 );	tRectCRT = Rect ( 194	, 248, 45, 20 );	break;
				case 2	:	tRectHP = Rect ( 284, 277, 100, 20 );	tRectATK = Rect (246, 263, 50, 20);	tRectDEF = Rect ( 289.5f , 263, 45, 20 );	tRectAGL = Rect ( 246, 248, 50, 20 );	tRectCRT = Rect ( 289.5f, 248, 45, 20 );	break;
				case 3	:	tRectHP = Rect ( 379, 277, 100, 20 );	tRectATK = Rect (341, 263, 50, 20);	tRectDEF = Rect ( 384	 , 263, 45, 20 );	tRectAGL = Rect ( 341, 248, 50, 20 );	tRectCRT = Rect ( 384	, 248, 45, 20 );	break;
				case 4	:	tRectHP = Rect ( 474, 277, 100, 20 );	tRectATK = Rect (436, 263, 50, 20);	tRectDEF = Rect ( 479	 , 263, 45, 20 );	tRectAGL = Rect ( 436, 248, 50, 20 );	tRectCRT = Rect ( 479	, 248, 45, 20 );	break;
			} 

			tLabelIntHp .pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectHP , szHP , LAYER_GAMEVIEW_INFO );
			tLabelIntAtk.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectATK, szATK, LAYER_GAMEVIEW_INFO );
			tLabelIntDef.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectDEF, szDEF, LAYER_GAMEVIEW_INFO );
			tLabelIntAgl.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectAGL, szAGL, LAYER_GAMEVIEW_INFO );
			tLabelIntCrt.pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, tRectCRT, szCRT, LAYER_GAMEVIEW_INFO );

				 if ( rMobs [ i ]->getUseAtk ( ) == rMobs [ i ]->getATK ( ) )	m_pLabels->setColor ( tLabelIntAtk.pLabel, 255, 255, 255 );
			else if ( rMobs [ i ]->getUseAtk ( )  < rMobs [ i ]->getATK ( ) )	m_pLabels->setColor ( tLabelIntAtk.pLabel, 189,  29,  12 );
			else																m_pLabels->setColor ( tLabelIntAtk.pLabel,  12, 187,  37 );

				 if ( rMobs [ i ]->getUseDef ( ) == rMobs [ i ]->getDEF ( ) )	m_pLabels->setColor ( tLabelIntDef.pLabel, 255, 255, 255 );
			else if ( rMobs [ i ]->getUseDef ( )  < rMobs [ i ]->getDEF ( ) )	m_pLabels->setColor ( tLabelIntDef.pLabel, 189,  29,  12 );
			else																m_pLabels->setColor ( tLabelIntDef.pLabel,  12, 187,  37 );

				 if ( rMobs [ i ]->getUseAgl ( ) == rMobs [ i ]->getAGL ( ) )	m_pLabels->setColor ( tLabelIntAgl.pLabel, 255, 255, 255 );
			else if ( rMobs [ i ]->getUseAgl ( )  < rMobs [ i ]->getAGL ( ) )	m_pLabels->setColor ( tLabelIntAgl.pLabel, 189,  29,  12 );
			else																m_pLabels->setColor ( tLabelIntAgl.pLabel,  12, 187,  37 );

				 if ( rMobs [ i ]->getUseCrt ( ) == rMobs [ i ]->getCRT ( ) )	m_pLabels->setColor ( tLabelIntCrt.pLabel, 255, 255, 255 );
			else if ( rMobs [ i ]->getUseCrt ( )  < rMobs [ i ]->getCRT ( ) )	m_pLabels->setColor ( tLabelIntCrt.pLabel, 189,  29,  12 );
			else																m_pLabels->setColor ( tLabelIntCrt.pLabel,  12, 187,  37 );
		}

		m_aLabelHPs .push_back ( tLabelIntHp  );
		m_aLabelATKs.push_back ( tLabelIntAtk );
		m_aLabelDEFs.push_back ( tLabelIntDef );
		m_aLabelAGLs.push_back ( tLabelIntAgl );
		m_aLabelCRTs.push_back ( tLabelIntCrt );

		// status icons
		cStatusIcons*		pStatusIcons = new cStatusIcons ( );
		Point				tPoint = Point::ZERO;
		switch ( i )
		{
			case 0	:	tPoint = Point (   6, 228 );	break;
			case 1	:	tPoint = Point ( 101, 228 );	break;
			case 2	:	tPoint = Point ( 196, 228 );	break;
			case 3	:	tPoint = Point ( 291, 228 );	break;
			case 4	:	tPoint = Point ( 386, 228 );	break;
		}

		for ( KDint a = 0; a < 5; a++ )
		{
			cSprAni*	pAni = new cSprAni ( m_pNode, "Game/status_icons.png", LAYER_GAMEVIEW_INFO, Point ( tPoint.x + a * 18, tPoint.y ) );

			pAni->addFrame ( Rect (  0,  0, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 15,  0, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 30,  0, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 45,  0, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 60,  0, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect (  0, 15, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 15, 15, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 30, 15, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 45, 15, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
			pAni->addFrame ( Rect ( 60, 15, 15, 15), 80, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );

			pAni->setVisible ( false );
			pAni->setFrameInit ( 0 );
			pAni->setLoop ( false );
			pAni->stop ( );

			pStatusIcons->aIcons.push_back ( pAni );
		}

		m_aStatusIcons.push_back ( pStatusIcons );

		if ( m_eModeType == eModeType_Casino || m_eModeType == eModeType_MyRoom_Casino100 || m_eModeType == eModeType_MyRoom_Casino1000 )
		{
			// status cover color layer
			if ( nBetSeatIndex != i )
			{
				Color4B			tColor;

				tColor.r = 0;
				tColor.g = 0;
				tColor.b = 0;
				tColor.a = 255;

				LayerColor*		pColorLayer = LayerColor::create ( tColor, 91, 56 );

				pColorLayer->setVisible ( false );

				switch ( i )
				{
					case 0	:	pColorLayer->setPosition ( Point (   4, 235 ) );	break;
					case 1	:	pColorLayer->setPosition ( Point (  99, 235 ) );	break;
					case 2	:	pColorLayer->setPosition ( Point ( 194, 235 ) );	break;
					case 3	:	pColorLayer->setPosition ( Point ( 289, 235 ) );	break;
					case 4	:	pColorLayer->setPosition ( Point ( 385, 235 ) );	break;
				}

				m_pNode->addChild ( pColorLayer, LAYER_GAMEVIEW_INFO2 );

				m_aColorStatusCovers.push_back ( pColorLayer );

				// portrait
				cSprAnis*		pAnis = nullptr;
				switch ( i )
				{
					case 0	:	pAnis = new cSprAnis ( m_pNode, rMobs [ i ]->getAniFileName ( ), LAYER_GAMEVIEW_INFO2, Point (  17, 288 ) );	break;
					case 1	:	pAnis = new cSprAnis ( m_pNode, rMobs [ i ]->getAniFileName ( ), LAYER_GAMEVIEW_INFO2, Point ( 112, 288 ) );	break;
					case 2	:	pAnis = new cSprAnis ( m_pNode, rMobs [ i ]->getAniFileName ( ), LAYER_GAMEVIEW_INFO2, Point ( 207, 288 ) );	break;
					case 3	:	pAnis = new cSprAnis ( m_pNode, rMobs [ i ]->getAniFileName ( ), LAYER_GAMEVIEW_INFO2, Point ( 302, 288 ) );	break;
					case 4	:	pAnis = new cSprAnis ( m_pNode, rMobs [ i ]->getAniFileName ( ), LAYER_GAMEVIEW_INFO2, Point ( 397, 288 ) );	break;
				}

				pAnis->texLoad ( eUnitAniType_Portrait );
				pAnis->changeAni ( eUnitAniType_Portrait );
				pAnis->setVisible ( false );
				pAnis->stop ( );

				m_aSprPortraits.push_back ( pAnis );

				// UI mob dividend magnifications
				const KDchar*	pStr1	= ccszf ( "%d", (KDint)	    rMobs [ i ]->getDividendMagnification ( ) );
				const KDchar*	pStr2	= ccszf ( "%d", (KDint) ( ( rMobs [ i ]->getDividendMagnification ( ) - (KDint) rMobs [ i ]->getDividendMagnification ( ) ) * 10 ) );
				Point			tPoint	= Point (0, 0);

				cLabelAtlas*	pLabel1 = new cLabelAtlas ( m_pNode, pStr1, "Game/battle_multiple_num.png" , 18, 19, '0', LAYER_GAMEVIEW_INFO2, Point::ZERO );
				cLabelAtlas*	pLabel2 = new cLabelAtlas ( m_pNode, pStr2, "Game/battle_multiple_num2.png", 12, 13, '0', LAYER_GAMEVIEW_INFO2, Point::ZERO );
				cSprite*		pLabel3 = new cSprite	  ( m_pNode, "Game/battle_multiple_num3.png", LAYER_GAMEVIEW_INFO2, Point ( 0, 0 ), Point::ZERO );
				cSprite*		pLabel4 = new cSprite	  ( m_pNode, "Game/battle_multiple.png"	    , LAYER_GAMEVIEW_INFO2, Point ( 0, 0 ), Point::ZERO );

				switch ( i )
				{
					case 0	:	tPoint = Point (  50 - ( 20 + pLabel1->getSize ( ).width + 5 + pLabel2->getSize ( ).width ) / 2, 240 );		break;
					case 1	:	tPoint = Point ( 145 - ( 20 + pLabel1->getSize ( ).width + 5 + pLabel2->getSize ( ).width ) / 2, 240 );		break;
					case 2	:	tPoint = Point ( 240 - ( 20 + pLabel1->getSize ( ).width + 5 + pLabel2->getSize ( ).width ) / 2, 240 );		break;
					case 3	:	tPoint = Point ( 335 - ( 20 + pLabel1->getSize ( ).width + 5 + pLabel2->getSize ( ).width ) / 2, 240 );		break;
					case 4	:	tPoint = Point ( 430 - ( 20 + pLabel1->getSize ( ).width + 5 + pLabel2->getSize ( ).width ) / 2, 240 );		break;
				}

				pLabel1->setPoint ( Point ( tPoint.x + 20								  , tPoint.y ) );
				pLabel2->setPoint ( Point ( tPoint.x + 20 + pLabel1->getSize ( ).width + 5, tPoint.y ) );
				pLabel3->get ( )->setPosition ( Point ( tPoint.x + 20 + pLabel1->getSize ( ).width, tPoint.y ) );
				pLabel4->get ( )->setPosition ( tPoint );

				pLabel1->setVisible ( false );
				pLabel2->setVisible ( false );
				pLabel3->get ( )->setVisible ( false );
				pLabel4->get ( )->setVisible ( false );

				m_pUIMobBarMagnifications .push_back ( pLabel1 );
				m_pUIMobBarMagnifications2.push_back ( pLabel2 );
				m_pUIMobBarMagnifications3.push_back ( pLabel3 );
				m_pUIMobBarMagnifications4.push_back ( pLabel4 );
			}
		}
	}

	// active sprite
	m_aActiveBars.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point (   7, 236 ) ) );
	m_aActiveBars.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 102, 236 ) ) );
	m_aActiveBars.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 197, 236 ) ) );
	m_aActiveBars.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 292, 236 ) ) );
	m_aActiveBars.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 387, 236 ) ) );

	m_aActiveBarEnds.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar_end.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point (   7, 236 ) ) );
	m_aActiveBarEnds.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar_end.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 102, 236 ) ) );
	m_aActiveBarEnds.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar_end.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 197, 236 ) ) );
	m_aActiveBarEnds.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar_end.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 292, 236 ) ) );
	m_aActiveBarEnds.push_back ( new cSprite ( m_pNode, "Game/game_mob_active_bar_end.png", LAYER_GAMEVIEW_ACTIVE, Point ( 0, 0 ), Point ( 387, 236 ) ) );

	for ( auto pNode : m_aActiveBars    )	pNode->get ( )->setTextureRect ( Rect ( 0, 0, 0, 8 ) );
	for ( auto pNode : m_aActiveBarEnds )	pNode->get ( )->setVisible ( false );
	
	// kill
	m_pUIKillFont = new cSprite ( m_pNode, "Game/survival_kill_font.png", LAYER_GAMEVIEW_INFO2, Point ( 0, 0 ), Point ( 423, 13 ) );

	m_pUIKillNum = new cLabelAtlasCustom ( m_pNode, "Game/survival_kill_num.png", 19, LAYER_GAMEVIEW_INFO2 );
	m_pUIKillNum->addCharInfo ( '1', 11 );	m_pUIKillNum->addCharInfo ( '2', 15 );	m_pUIKillNum->addCharInfo ( '3', 15 );
	m_pUIKillNum->addCharInfo ( '4', 15 );	m_pUIKillNum->addCharInfo ( '5', 15 );	m_pUIKillNum->addCharInfo ( '6', 15 );
	m_pUIKillNum->addCharInfo ( '7', 15 );	m_pUIKillNum->addCharInfo ( '8', 15 );
	m_pUIKillNum->addCharInfo ( '9', 15 );	m_pUIKillNum->addCharInfo ( '0', 15 );
	
	m_pUIKillFont->get ( )->setVisible ( false );
	m_pUIKillNum->setVisible ( false );

}

cMobCasinoStatus::~cMobCasinoStatus ( KDvoid )
{
	// sprites
	for ( auto pNode : m_aActiveBars    )	{ CC_SAFE_RELEASE ( pNode ); }	
	for ( auto pNode : m_aActiveBarEnds )	{ CC_SAFE_RELEASE ( pNode ); }	
	m_aActiveBars   .clear ( );
	m_aActiveBarEnds.clear ( );

	// label parameters
	m_aLabelHPs .clear ( );
	m_aLabelATKs.clear ( );
	m_aLabelDEFs.clear ( );
	m_aLabelAGLs.clear ( );
	m_aLabelCRTs.clear ( );

	// status icons
	for ( auto pNode : m_aStatusIcons )		{ CC_SAFE_RELEASE ( pNode ); }	
	m_aStatusIcons.clear ( );

	// labels
	CC_SAFE_RELEASE ( m_pLabels );
	
	// status cover color layer
	for ( auto pNode : m_aColorStatusCovers )
	{
		m_pNode->removeChild ( pNode, true );
		pNode = nullptr;
	}
	m_aColorStatusCovers.clear ( );

	// portrait
	for ( auto pNode : m_aSprPortraits )	{ CC_SAFE_RELEASE ( pNode ); }	
	m_aSprPortraits.clear ( );

	//UI mob dividend magnifications
	for ( auto pNode : m_pUIMobBarMagnifications  )	{ CC_SAFE_RELEASE ( pNode ); }	
	for ( auto pNode : m_pUIMobBarMagnifications2 )	{ CC_SAFE_RELEASE ( pNode ); }	
	for ( auto pNode : m_pUIMobBarMagnifications3 )	{ CC_SAFE_RELEASE ( pNode ); }	
	for ( auto pNode : m_pUIMobBarMagnifications4 )	{ CC_SAFE_RELEASE ( pNode ); }

	m_pUIMobBarMagnifications .clear ( );
	m_pUIMobBarMagnifications2.clear ( );
	m_pUIMobBarMagnifications3.clear ( );
	m_pUIMobBarMagnifications4.clear ( );

	// kill
	CC_SAFE_RELEASE ( m_pUIKillFont );
	CC_SAFE_RELEASE ( m_pUIKillNum  );
}

KDvoid cMobCasinoStatus::update ( const std::vector<cUnit*>& rMobs, const std::vector<KDfloat>& rActiveGages )
{
	// status icons clear---------------------------------------------
	for ( auto pStatusIcons : m_aStatusIcons )
	{
		for ( auto pIcon : pStatusIcons->aIcons )
		{
			pIcon->setVisible ( false );
		}
	}

	// loop
	for ( KDuint i = 0; i < rMobs.size ( ); i++ )
	{
		// get mob
		cUnit*		pMob = rMobs [ i ];

		// check
		if ( pMob == nullptr )
		{
			continue;
		}

		// seat index
		KDint		nSeatIndex = pMob->getSeatIndex ( );

		// parameters-------------------------------------------------
		// hp
		if ( pMob->getDie ( ) == true )
		{
			m_pLabels->setStr ( m_aLabelHPs  [ nSeatIndex ].pLabel, "" );
			m_pLabels->setStr ( m_aLabelATKs [ nSeatIndex ].pLabel, "" );
			m_pLabels->setStr ( m_aLabelDEFs [ nSeatIndex ].pLabel, "" );
			m_pLabels->setStr ( m_aLabelAGLs [ nSeatIndex ].pLabel, "" );
			m_pLabels->setStr ( m_aLabelCRTs [ nSeatIndex ].pLabel, "" );
		}
		else
		{
			if ( m_aLabelHPs [ nSeatIndex ].nSub != pMob->getUseHp ( ) )
			{
				auto	pLabelID = m_aLabelHPs [ nSeatIndex ].pLabel;
				m_aLabelHPs [ nSeatIndex ].nSub = pMob->getUseHp ( );
				m_pLabels->setStr ( pLabelID, ccszf ( "%02d / %02d", pMob->getUseHp ( ), pMob->getHP ( ) ) );
			}

			// atk
			if ( m_aLabelATKs [ nSeatIndex ].nSub != pMob->getUseAtk ( ) )
			{
				m_aLabelATKs [ nSeatIndex ].nSub = pMob->getUseAtk ( );

				m_pLabels->setStr ( m_aLabelATKs [ nSeatIndex ].pLabel, ccszf ( "%02d", m_aLabelATKs [ nSeatIndex ].nSub ) );

					 if ( pMob->getUseAtk ( ) == pMob->getATK ( ) )	m_pLabels->setColor ( m_aLabelATKs [ nSeatIndex ].pLabel, 255, 255, 255 );
				else if ( pMob->getUseAtk ( )  < pMob->getATK ( ) )	m_pLabels->setColor ( m_aLabelATKs [ nSeatIndex ].pLabel, 189,  29,  12 );
				else												m_pLabels->setColor ( m_aLabelATKs [ nSeatIndex ].pLabel,  12, 187,  37 );
			}

			// def
			if( m_aLabelDEFs [ nSeatIndex ].nSub != pMob->getUseDef ( ) )
			{
				m_aLabelDEFs [ nSeatIndex ].nSub = pMob->getUseDef ( );

				m_pLabels->setStr ( m_aLabelDEFs [ nSeatIndex ].pLabel, ccszf ( "%02d", m_aLabelDEFs [ nSeatIndex ].nSub ) );

					 if ( pMob->getUseDef ( ) == pMob->getDEF ( ) )	m_pLabels->setColor ( m_aLabelDEFs [ nSeatIndex ].pLabel, 255, 255, 255 );
				else if ( pMob->getUseDef ( )  < pMob->getDEF ( ) )	m_pLabels->setColor ( m_aLabelDEFs [ nSeatIndex ].pLabel, 189,  29,  12 );
				else												m_pLabels->setColor ( m_aLabelDEFs [ nSeatIndex ].pLabel,  12, 187,  37 );
			}

			// agl
			if ( m_aLabelAGLs [ nSeatIndex ].nSub != pMob->getUseAgl ( ) )
			{
				m_aLabelAGLs [ nSeatIndex ].nSub = pMob->getUseAgl ( );

				m_pLabels->setStr ( m_aLabelAGLs [ nSeatIndex ].pLabel, ccszf ( "%02d", m_aLabelAGLs [ nSeatIndex ].nSub ) );

					 if ( pMob->getUseAgl ( ) == pMob->getAGL ( ) )	m_pLabels->setColor ( m_aLabelAGLs [ nSeatIndex ].pLabel, 255, 255, 255 );
				else if ( pMob->getUseAgl ( )  < pMob->getAGL ( ) )	m_pLabels->setColor ( m_aLabelAGLs [ nSeatIndex ].pLabel, 189,  29,  12 );
				else												m_pLabels->setColor ( m_aLabelAGLs [ nSeatIndex ].pLabel,  12, 187,  37 );
			}

			// crt
			if ( m_aLabelCRTs [ nSeatIndex ].nSub != pMob->getUseCrt ( ) )
			{
				m_aLabelCRTs [ nSeatIndex ].nSub = pMob->getUseCrt ( );

				m_pLabels->setStr ( m_aLabelCRTs [ nSeatIndex ].pLabel, ccszf ( "%02d", m_aLabelCRTs [ nSeatIndex ].nSub ) );

					 if ( pMob->getUseCrt ( ) == pMob->getCRT ( ) )	m_pLabels->setColor ( m_aLabelCRTs [ nSeatIndex ].pLabel, 255, 255, 255 );
				else if ( pMob->getUseCrt ( )  < pMob->getCRT ( ) )	m_pLabels->setColor ( m_aLabelCRTs [ nSeatIndex ].pLabel, 189,  29,  12 );
				else												m_pLabels->setColor ( m_aLabelCRTs [ nSeatIndex ].pLabel,  12, 187,  37 );
			}
		}			

		// status icons set-------------------------------------------
		const std::vector<cParaAdd>&	rParaAdds = pMob->getUseParaAdd ( );
		cStatusIcons*					pStatusIcons = m_aStatusIcons [ nSeatIndex ];

		std::vector<cParaAdd>::const_iterator	pParaAddNode = rParaAdds.begin ( );
		std::vector<cSprAni*>::iterator			pIconNode	 = pStatusIcons->aIcons.begin ( );

		while ( pParaAddNode != rParaAdds.end ( ) && pIconNode != pStatusIcons->aIcons.end ( ) )
		{
			const cParaAdd&		pParaAdd = *pParaAddNode;
			cSprAni*			pSprIcon = *pIconNode;

			pSprIcon->setVisible ( true );

				 if ( pParaAdd.nHp  > 0 )	pSprIcon->setFrameInit ( eStatusIcon_Regene	 );
			else if ( pParaAdd.nHp  < 0 )	pSprIcon->setFrameInit ( eStatusIcon_poison	 );
			else if ( pParaAdd.nAtk > 0 )	pSprIcon->setFrameInit ( eStatusIcon_AtkUp	 );
			else if ( pParaAdd.nAtk < 0 )	pSprIcon->setFrameInit ( eStatusIcon_AtkDown );
			else if ( pParaAdd.nDef > 0 )	pSprIcon->setFrameInit ( eStatusIcon_DefUp	 );
			else if ( pParaAdd.nDef < 0 )	pSprIcon->setFrameInit ( eStatusIcon_DefDown );
			else if ( pParaAdd.nAgl > 0 )	pSprIcon->setFrameInit ( eStatusIcon_AglUp	 );
			else if ( pParaAdd.nAgl < 0 )	pSprIcon->setFrameInit ( eStatusIcon_AglDown );
			else if ( pParaAdd.nCrt > 0 )	pSprIcon->setFrameInit ( eStatusIcon_CrtUp	 );
			else if ( pParaAdd.nCrt < 0 )	pSprIcon->setFrameInit ( eStatusIcon_CrtDown );

			++pParaAddNode;
			++pIconNode;
		}
	}

	// active gages---------------------------------------------------
	for ( KDuint i = 0; i < rActiveGages.size ( ); i++ )
	{
		if ( rActiveGages [ i ] < 100 )	m_aActiveBarEnds [ i ]->get ( )->setVisible ( false );
		else							m_aActiveBarEnds [ i ]->get ( )->setVisible ( true );

		KDfloat		fWidth = ( rActiveGages [ i ] * 86 ) / 100;
		m_aActiveBars [ i ]->get ( )->setTextureRect ( Rect ( 0, 0, fWidth, 8 ) );
	}
	
	// kill-----------------------------------------------------------
	if ( m_eModeType == eModeType_MyRoom_Survival )
	{
		m_pUIKillFont->get ( )->setVisible ( true );
		
		m_pUIKillNum->setString ( ccszf ( "%d", cMyData::getObj ( )->m_nGamingKillCount ) );
		m_pUIKillNum->setPosition ( Point ( 418 - m_pUIKillNum->getSize ( ).width, 13 ) );
		m_pUIKillNum->setVisible ( true );
	}
}

KDvoid cMobCasinoStatus::setParaSub ( cUnit* pMob )
{
	// checi
	if ( pMob->getSeatIndex ( ) < 0 || pMob->getSeatIndex ( ) >= (KDint) m_aLabelHPs.size ( ) )	
	{
		return;
	}

	// init
	KDint	nSeatIndex = pMob->getSeatIndex ( );

	// set sub
	m_aLabelHPs  [ nSeatIndex ].nSub = pMob->getHP  ( );
	m_aLabelATKs [ nSeatIndex ].nSub = pMob->getATK ( );
	m_aLabelDEFs [ nSeatIndex ].nSub = pMob->getDEF ( );
	m_aLabelAGLs [ nSeatIndex ].nSub = pMob->getAGL ( );
	m_aLabelCRTs [ nSeatIndex ].nSub = pMob->getCRT ( );
	 
	// parameters
	// hp
	m_pLabels->setStr   ( m_aLabelHPs [ nSeatIndex ].pLabel, ccszf ( "%02d / %02d", m_aLabelHPs [ nSeatIndex ].nSub, m_aLabelHPs [ nSeatIndex ].nSub ) );
	m_pLabels->setColor ( m_aLabelHPs [ nSeatIndex ].pLabel, 255, 255, 255 );

	// atk
	m_pLabels->setStr   ( m_aLabelATKs [ nSeatIndex ].pLabel, ccszf ( "%02d / %02d", m_aLabelATKs [ nSeatIndex ].nSub, m_aLabelATKs [ nSeatIndex ].nSub ) );
	m_pLabels->setColor ( m_aLabelATKs [ nSeatIndex ].pLabel, 255, 255, 255 );

	// def
	m_pLabels->setStr   ( m_aLabelDEFs [ nSeatIndex ].pLabel, ccszf ( "%02d / %02d", m_aLabelDEFs [ nSeatIndex ].nSub, m_aLabelDEFs [ nSeatIndex ].nSub ) );
	m_pLabels->setColor ( m_aLabelDEFs [ nSeatIndex ].pLabel, 255, 255, 255 );

	// agl
	m_pLabels->setStr   ( m_aLabelAGLs [ nSeatIndex ].pLabel, ccszf ( "%02d / %02d", m_aLabelAGLs [ nSeatIndex ].nSub, m_aLabelAGLs [ nSeatIndex ].nSub ) );
	m_pLabels->setColor ( m_aLabelAGLs [ nSeatIndex ].pLabel, 255, 255, 255 );

	// crt
	m_pLabels->setStr   ( m_aLabelCRTs [ nSeatIndex ].pLabel, ccszf ( "%02d / %02d", m_aLabelCRTs [ nSeatIndex ].nSub, m_aLabelCRTs [ nSeatIndex ].nSub ) );
	m_pLabels->setColor ( m_aLabelCRTs [ nSeatIndex ].pLabel, 255, 255, 255 );
}

KDvoid cMobCasinoStatus::setModeStatus ( KDvoid )
{
	// check
	if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )
	{
		return;
	}

	// cover
	for ( auto pNode : m_aColorStatusCovers )
	{
		pNode->setVisible ( false );
	}

	// portrait
	for ( auto pNode : m_aSprPortraits )
	{
		pNode->setVisible ( false );
	}

	// label magnifications
	for ( auto pNode : m_pUIMobBarMagnifications  )	pNode->setVisible ( false );
	for ( auto pNode : m_pUIMobBarMagnifications2 )	pNode->setVisible ( false );
	for ( auto pNode : m_pUIMobBarMagnifications3 )	pNode->get ( )->setVisible ( false );
	for ( auto pNode : m_pUIMobBarMagnifications4 )	pNode->get ( )->setVisible ( false );
}

KDvoid cMobCasinoStatus::setModeMagnification ( KDvoid )
{
	// check
	if ( m_eModeType != eModeType_Casino && m_eModeType != eModeType_MyRoom_Casino100 && m_eModeType != eModeType_MyRoom_Casino1000 )	
	{
		return;
	}

	// cover
	for ( auto pNode : m_aColorStatusCovers )
	{
		pNode->setVisible ( true );
	}

	// portrait
	for ( auto pNode : m_aSprPortraits )
	{
		pNode->setVisible ( true );
	}

	// label magnifications
	for ( auto pNode : m_pUIMobBarMagnifications  )	pNode->setVisible ( true );
	for ( auto pNode : m_pUIMobBarMagnifications2 )	pNode->setVisible ( true );
	for ( auto pNode : m_pUIMobBarMagnifications3 )	pNode->get ( )->setVisible ( true );
	for ( auto pNode : m_pUIMobBarMagnifications4 )	pNode->get ( )->setVisible ( true );
}
