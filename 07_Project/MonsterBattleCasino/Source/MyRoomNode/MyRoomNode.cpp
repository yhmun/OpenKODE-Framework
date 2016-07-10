/* -----------------------------------------------------------------------------------
 *
 *      File            MyRoomNode.cpp
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
#include "MyRoomNode.h"
#include "ShopNode/ShopNode.h"
#include "MyData/MyData.h"
#include "Object/Menu/SoundMenuItem.h"
#include "Object/TimeGetMoney/TimeGetMoney.h"
#include "TitleNode/Main/MainNode.h"
#include "CasinoNode/CasinoBattleNode/CasinoBattleNode.h"
#include "Unit/Create/MobDividend.h"
#if 0
#import "OFSocialNotificationService.h"
#endif		// 0

#define _HAVE_MARK_MAX		12

cMachine::cMachine ( Node* pNode, cUnitCreateInfos* pUnitCreateInfo, eCreateInfoFileType eCreateInfoFile, const KDchar* pSprTopName, const KDchar* pSprUnitListName, const KDchar* pSprNotCoin, KDint z, Point tPoint, KDint64 nNeedCoin, eDollarMachineType eDollarMachine, KDdouble bBuyDelay, KDdouble* pBuyLeftTime, KDbool bIsHaveCheckMark )
: m_pNode					( pNode				)
, m_nZoder					( z					)
, m_pUnitCreateInfo			( pUnitCreateInfo	)
, m_eCreateInfoFileType		( eCreateInfoFile	)
, m_pSprBack				( nullptr			)
, m_pSprFront				( nullptr			)
, m_pSprTop					( nullptr			)
, m_pSprHandle				( nullptr			)
, m_pSprSelectL				( nullptr			)
, m_pSprSelectT				( nullptr			)
, m_pSprSelectR				( nullptr			)
, m_pSprSelectB				( nullptr			)
, m_pSprUnitList			( nullptr			)
, m_pLabelNum				( nullptr			)
, m_sSprUnitListName		( ""				)
, m_tPoint					( tPoint			)
, m_nNeedCoin				( nNeedCoin			)
, m_eDollarMachineType		( eDollarMachine	)
, m_dBuyDelay				( bBuyDelay			)
, m_pBuyLeftTime			( pBuyLeftTime		)
, m_bIsHaveCheckMark		( bIsHaveCheckMark	)
, m_bUnitListMoveIn			( false				)
, m_bUnitListMoveOut		( false				)
, m_dUnitListMoveLeftTime	( 0					)
, m_cAlpha					( 255				)
{
	m_pUnitCreateInfo->retain ( );

	// sprite
	m_pSprSelectL	= new cSprite ( m_pNode, "MyRoom/machine_select_left.png"	, z		, Point::ZERO			, Point::ZERO );
	m_pSprSelectT	= new cSprite ( m_pNode, "MyRoom/machine_select_top.png"	, z		, Point::ZERO			, Point::ZERO );
	m_pSprSelectR	= new cSprite ( m_pNode, "MyRoom/machine_select_right.png"	, z		, Point::ZERO			, Point::ZERO );
	m_pSprSelectB	= new cSprite ( m_pNode, "MyRoom/machine_select_bottom.png"	, z		, Point::ZERO			, Point::ZERO );
	m_pSprBack		= new cSprite ( m_pNode, "MyRoom/myroom_machine_back.png"	, z - 2	, Point::ZERO			, Point::ZERO );
	m_pSprFront		= new cSprite ( m_pNode, "MyRoom/myroom_machine_front.png"	, z		, Point::ZERO			, Point::ZERO );
	m_pSprTop		= new cSprite ( m_pNode, pSprTopName						, z		, Point::ZERO			, Point::ZERO );
	m_pSprHandle	= new cSprite ( m_pNode, "MyRoom/myroom_machine_handle.png"	, z		, Point ( 0.5f, 0.5f )	, Point::ZERO );
	m_pSprNotCoin	= new cSprite ( m_pNode, pSprNotCoin						, z		, Point::ZERO			, Point::ZERO );

	// label
	m_pLabelNum  = new cLabelAtlasCustom ( pNode, "MyRoom/myroom_machine_num.png" , 25, z );
	m_pLabelNum->addCharInfo ( '1', 13 );	m_pLabelNum->addCharInfo ( '2', 13 );	m_pLabelNum->addCharInfo ( '3', 13 );
	m_pLabelNum->addCharInfo ( '4', 13 );	m_pLabelNum->addCharInfo ( '5', 13 );	m_pLabelNum->addCharInfo ( '6', 13 );
	m_pLabelNum->addCharInfo ( '7', 13 );	m_pLabelNum->addCharInfo ( '8', 13 );	m_pLabelNum->addCharInfo ( '9', 13 );
	m_pLabelNum->addCharInfo ( '0', 13 );	m_pLabelNum->addCharInfo ( ':',  4 );

	if ( m_pBuyLeftTime != nullptr && *m_pBuyLeftTime > 0 )	m_pLabelNum->setVisible ( true  );
	else													m_pLabelNum->setVisible ( false );

	// create info names
	if ( m_bIsHaveCheckMark == true )
	{
		for ( KDint i = 0; i < m_pUnitCreateInfo->getUnitInfoCount ( ); i++ )
		{
			// init
			cUnitCreateInfo*	pCreateInfo = (cUnitCreateInfo*) m_pUnitCreateInfo->getUnitInfo ( i );
			KDbool				bIsSame = false;

			// compare
			for ( KDuint a = 0; a < m_sCreateInfoNames.size ( ); a++ )
			{
				if ( m_sCreateInfoNames [ a ].compare ( pCreateInfo->sName ) == 0 )
				{
					bIsSame = true;
					break;
				}
			}

			// add
			if ( bIsSame == false )
			{
				m_sCreateInfoNames.push_back ( pCreateInfo->sName );
				m_aIsHaveMarks	  .push_back ( false			  );

				if ( m_sCreateInfoNames.size ( ) == _HAVE_MARK_MAX )
				{
					break;
				}
			}
		}
	}

	// have marks sprite
	for ( KDuint i = 0; i < m_sCreateInfoNames.size ( ); i++ )
	{
		cSprite*	pSprite = new cSprite ( m_pNode, "MyRoom/haveUnitMark.png", m_nZoder + 1, Point::ZERO, Point::ZERO );
		pSprite->get ( )->setVisible ( false );
		m_pSprHaveMarks.push_back ( pSprite );
	}

	// string
	m_sSprUnitListName = pSprUnitListName;

	// visible
	setVisible ( false );
}

cMachine::~cMachine ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pUnitCreateInfo );
	CC_SAFE_RELEASE ( m_pSprFront		);
	CC_SAFE_RELEASE ( m_pSprTop			);
	CC_SAFE_RELEASE ( m_pSprBack		);
	CC_SAFE_RELEASE ( m_pSprHandle		);
	CC_SAFE_RELEASE ( m_pSprSelectL		);
	CC_SAFE_RELEASE ( m_pSprSelectT		);
	CC_SAFE_RELEASE ( m_pSprSelectR		);
	CC_SAFE_RELEASE ( m_pSprSelectB		);
	CC_SAFE_RELEASE ( m_pSprUnitList	);
	CC_SAFE_RELEASE ( m_pLabelNum		);

	SAFE_RELEASE_ARRAYS ( m_pSprHaveMarks );
}

KDvoid cMachine::setVisible ( KDbool bVisible )
{
	m_pSprFront	 ->get ( )->setVisible ( bVisible );
	m_pSprTop	 ->get ( )->setVisible ( bVisible );
	m_pSprBack	 ->get ( )->setVisible ( bVisible );
	m_pSprHandle ->get ( )->setVisible ( bVisible );
	m_pSprNotCoin->get ( )->setVisible ( false );

	m_pLabelNum->setVisible ( bVisible );
	m_pLabelNum->setColor ( 255, 255, 0 );

	setVisibleSelect ( false );
}

KDvoid cMachine::setVisibleSelect ( KDbool bVisible )
{
	m_pSprSelectL->get ( )->setVisible ( bVisible );
	m_pSprSelectT->get ( )->setVisible ( bVisible );
	m_pSprSelectR->get ( )->setVisible ( bVisible );
	m_pSprSelectB->get ( )->setVisible ( bVisible );
}

KDvoid cMachine::setVisibleUnitList ( KDbool bVisible, KDbool bIsMove )
{
	m_bUnitListMoveIn  = false;
	m_bUnitListMoveOut = false;

	if ( bVisible == true )
	{
		// new
		m_pSprUnitList = new cSprite ( m_pNode, m_sSprUnitListName.c_str ( ), m_nZoder, Point::ZERO, Point::ZERO );

		// position
		if ( bIsMove == false )	m_pSprUnitList->get ( )->setPosition ( Point (    0, 24 ) );
		else					m_pSprUnitList->get ( )->setPosition ( Point ( -230, 24 ) );

		// switch & left time
		if ( bIsMove == true )
		{
			m_bUnitListMoveIn	    = true;
			m_dUnitListMoveLeftTime = 0.2f;
		}

		// have mark
		for ( auto pNode : m_pSprHaveMarks )
		{
			pNode->get ( )->setVisible ( false );
		}

		if ( bIsMove == false )
		{
			for ( KDuint i = 0; i < m_pSprHaveMarks.size ( ); i++ )
			{
				if ( m_aIsHaveMarks [ i ] == true )
				{
					m_pSprHaveMarks [ i ]->get ( )->setVisible ( true );
						 if ( i / 3 == 0 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 0				) );
					else if ( i / 3 == 1 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 62				) );
					else if ( i / 3 == 2 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 62 - 61		) );
					else if ( i / 3 == 3 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 62 - 61 - 62	) );
					else					m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - ( i / 3 ) * 62	) );
				}
			}
		}
	}
	else
	{
		// have mark
		for ( auto pNode : m_pSprHaveMarks )
		{
			pNode->get ( )->setVisible ( false );
		}

		if ( bIsMove == false )
		{
			CC_SAFE_RELEASE ( m_pSprUnitList );
		}
		else
		{
			m_bUnitListMoveOut = true;
			m_dUnitListMoveLeftTime = 0.2f;
		}

		for ( auto pNode : m_pSprHaveMarks )
		{
			pNode->get ( )->setVisible ( false );
		}
	}
}

KDvoid cMachine::setVisibleNotCoinTrue ( KDvoid )
{
	m_pSprNotCoin->get ( )->setVisible ( true );
	m_pSprNotCoin->get ( )->setOpacity ( 255 );
}

KDvoid cMachine::setPoint ( Point tPoint )
{
	m_pSprFront		->get ( )->setPosition ( Point ( tPoint.x	  , tPoint.y	   ) );
	m_pSprTop		->get ( )->setPosition ( Point ( tPoint.x	  , tPoint.y + 115 ) );
	m_pSprBack		->get ( )->setPosition ( Point ( tPoint.x +  4, tPoint.y +  19 ) );
	m_pSprHandle	->get ( )->setPosition ( Point ( tPoint.x + 60, tPoint.y +  81 ) );
	m_pSprNotCoin	->get ( )->setPosition ( Point ( tPoint.x + 14, tPoint.y + 125 ) );
	m_pSprSelectL	->get ( )->setPosition ( Point ( tPoint.x -  4, tPoint.y +  11 ) );
	m_pSprSelectT	->get ( )->setPosition ( Point ( tPoint.x -  4, tPoint.y + 190 ) );
	m_pSprSelectR	->get ( )->setPosition ( Point ( tPoint.x + 95, tPoint.y +  11 ) );
	m_pSprSelectB	->get ( )->setPosition ( Point ( tPoint.x -  4, tPoint.y -   3 ) );

	m_pLabelNum->setPosition ( Point ( tPoint.x + 49 - (KDint) m_pLabelNum->getSize ( ).width / 2, tPoint.y + 200 ) );
}

KDvoid cMachine::setHandleRotate ( KDfloat fRotation )
{
	m_pSprHandle->get ( )->setRotation ( fRotation );
}

const cUnitCreateInfo* cMachine::getUnitInfoRand ( KDvoid )
{
	return m_pUnitCreateInfo->getUnitInfoRand ( );
}

KDvoid cMachine::setAlpha ( KDubyte cAlpha )
{
	m_cAlpha = cAlpha;

	m_pSprFront ->get ( )->setOpacity ( cAlpha );
	m_pSprTop   ->get ( )->setOpacity ( cAlpha );
	m_pSprBack  ->get ( )->setOpacity ( cAlpha );
	m_pSprHandle->get ( )->setOpacity ( cAlpha );

	m_pLabelNum->setAlpha ( m_cAlpha );
}

KDvoid cMachine::setIsMark ( std::vector<cUnit*> aUnits )
{
	for ( KDuint i = 0; i < m_aIsHaveMarks.size ( ); i++ )
	{
		m_aIsHaveMarks [ i ] = false;
	}

	for ( auto pUnit : aUnits  )
	{
		for ( KDuint a = 0; a < m_sCreateInfoNames.size ( ); a++ )
		{
			if ( m_sCreateInfoNames [ a ].compare ( pUnit->getName ( ) ) == 0 )
			{
				m_aIsHaveMarks [ a ] = true;
				break;
			}
		}
	}
}

KDvoid cMachine::setColorLabelNum ( KDubyte r, KDubyte g, KDubyte b )
{
	m_pLabelNum->setColor ( r, g, b );
}

KDbool cMachine::getVisibleUnitList ( KDvoid )
{
	if ( m_pSprUnitList == nullptr || m_bUnitListMoveOut == true )
	{
		return false;
	}

	return m_pSprUnitList->get ( )->isVisible ( );
}

KDvoid cMachine::update ( KDdouble dLeftTime )
{
	// buy left time
	if ( m_pBuyLeftTime != nullptr && *m_pBuyLeftTime > 0 )
	{
		if ( *m_pBuyLeftTime > 0 )
		{
			KDint		nTime   = *m_pBuyLeftTime;
			KDint		nHour   = nTime / 3600;
			KDint		nMinute = ( nTime % 3600 ) / 60;
			KDint		nSecond = nTime % 60;

			const KDchar*	szHour	 = ccszf ( "%02d", nHour   );
			const KDchar*	szMinute = ccszf ( "%02d", nMinute );
			const KDchar*	szSecond = ccszf ( "%02d", nSecond );

			m_pLabelNum->setString ( ccszf ( "%s:%s:%s", szHour, szMinute, szSecond ) );
		}
		else
		{
			m_pLabelNum->setString ( "" );
		}
	}

	// label color
	Color3B		tColor = m_pLabelNum->getColor ( );
	if ( tColor.g < 255 ) {	KDint g = tColor.g + 5;	if ( g < 0 ) g = 0;	tColor.g = g; }
	if ( tColor.b >   0 ) {	KDint b = tColor.b - 5;	if ( b < 0 ) b = 0;	tColor.b = b; }
	m_pLabelNum->setColor ( tColor.r, tColor.g, tColor.b );

	// not coin
	if ( m_pSprNotCoin->get ( )->isVisible ( ) == true )
	{
		KDint	nAlpha = m_pSprNotCoin->get ( )->getOpacity ( );
		nAlpha -= 5;
		if ( nAlpha < 0 )
		{
			nAlpha = 0;
		}
		m_pSprNotCoin->get ( )->setOpacity ( nAlpha );
	}

	// set pt
	setPoint ( m_tPoint );

	// unit list
	if ( m_pSprUnitList != nullptr && ( m_bUnitListMoveIn == true || m_bUnitListMoveOut == true ) )
	{
		// left time
		m_dUnitListMoveLeftTime -= dLeftTime;

		// percent
		KDdouble		dPercent = ( 0.2f - m_dUnitListMoveLeftTime ) / 0.2f;		// max == 1

		// position
		Point			tPoint;

		if ( m_bUnitListMoveIn == true )	tPoint = Point ( -230 + 230 * dPercent, 24 );
		else								tPoint = Point ( 0	  - 230 * dPercent, 24 );

		m_pSprUnitList->get ( )->setPosition ( tPoint );

		// time check
		if ( m_dUnitListMoveLeftTime < 0 )
		{
			if ( m_bUnitListMoveOut == true) 	m_pSprUnitList = nullptr;
			else								m_pSprUnitList->get ( )->setPosition ( Point ( 0, 24 ) );

			// have mark
			if ( m_bUnitListMoveIn == true )
			{
				for ( KDuint i = 0; i < m_pSprHaveMarks.size ( ); i++ )
				{
					if ( m_aIsHaveMarks [ i ] == true )
					{
						m_pSprHaveMarks [ i ]->get ( )->setVisible ( true );
							 if ( i / 3 == 0 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 0				) );
						else if ( i / 3 == 1 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 62				) );
						else if ( i / 3 == 2 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 62 - 61		) );
						else if ( i / 3 == 3 )	m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - 62 - 61 - 62	) );
						else					m_pSprHaveMarks [ i ]->get ( )->setPosition ( Point ( 55 + ( i % 3 ) * 75, 251 - ( i / 3 ) * 62	) );
					}
				}
			}

			// switch
			m_bUnitListMoveIn  = false;
			m_bUnitListMoveOut = false;
		}
	}
}

cMyRoomLvlUpEffect::cMyRoomLvlUpEffect ( Node* pNode, KDint z, Point tCenter, Point tSpecial, KDint nSlot, KDint nHp, KDint nAtk, KDint nDef, KDint nAgl, KDint nCrt )
: m_pUILevelUp			( nullptr	)
, m_pUIStatusHp			( nullptr	)
, m_pUIStatusAtk		( nullptr	)
, m_pUIStatusDef		( nullptr	)
, m_pUIStatusAgl		( nullptr	)
, m_pUIStatusCrt		( nullptr	)
, m_nSlot				( nSlot		)
, m_nHp					( nHp		)
, m_nAtk				( nAtk		)
, m_nDef				( nDef		)
, m_nAgl				( nAgl		)
, m_nCrt				( nCrt		)
, m_dLeftTime			( 0			)
, m_dHpStartLeftTime	( 0			)
, m_dAtkStartLeftTime	( 0			)
, m_dDefStartLeftTime	( 0			)
, m_dAglStartLeftTime	( 0			)
, m_dCrtStartLeftTime	( 0			)
, m_dLvLeftTime			( 0			)
, m_dHpLeftTime			( 0			)
, m_dAtkLeftTime		( 0			)
, m_dDefLeftTime		( 0			)
, m_dAglLeftTime		( 0			)
, m_dCrtLeftTime		( 0			)
, m_tPointCenter		( tCenter	)
, m_tPointSpecial		( tSpecial	)
, m_tPointHp			( tSpecial	)
, m_tPointAtk			( tSpecial	)
, m_tPointDef			( tSpecial	)
, m_tPointAgl			( tSpecial	)
, m_tPointCrt			( tSpecial	)
{
	// sprite
	m_pUILevelUp	= new cSprite ( pNode, "CasinoResult/casino_result_lvup.png", z, Point ( 0, 0 ), Point ( m_tPointCenter.x - 50, m_tPointCenter.y - 13 ) );
	m_pUIStatusHp	= new cSprAni ( pNode, "Game/status_effects.png", z, m_tPointHp  );
	m_pUIStatusAtk	= new cSprAni ( pNode, "Game/status_effects.png", z, m_tPointAtk );
	m_pUIStatusDef	= new cSprAni ( pNode, "Game/status_effects.png", z, m_tPointDef );
	m_pUIStatusAgl	= new cSprAni ( pNode, "Game/status_effects.png", z, m_tPointAgl );
	m_pUIStatusCrt	= new cSprAni ( pNode, "Game/status_effects.png", z, m_tPointCrt );

	m_pUIStatusHp ->addFrame ( Rect ( 353,  0, 58, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusAtk->addFrame ( Rect ( 146,  0, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusDef->addFrame ( Rect ( 286,  0, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusAgl->addFrame ( Rect (  73, 30, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );
	m_pUIStatusCrt->addFrame ( Rect ( 213, 30, 67, 30 ), 80, false, false, Point ( 33, 30 ), Rect::ZERO, Rect::ZERO );

	m_pUIStatusHp ->setVisible ( true );	m_pUIStatusHp ->setFrameInit ( 0 );	m_pUIStatusHp ->setLoop ( false );	m_pUIStatusHp ->stop ( );	m_pUIStatusHp ->setAlpha ( 0 );
	m_pUIStatusAtk->setVisible ( true );	m_pUIStatusAtk->setFrameInit ( 0 );	m_pUIStatusAtk->setLoop ( false );	m_pUIStatusAtk->stop ( );	m_pUIStatusAtk->setAlpha ( 0 );
	m_pUIStatusDef->setVisible ( true );	m_pUIStatusDef->setFrameInit ( 0 );	m_pUIStatusDef->setLoop ( false );	m_pUIStatusDef->stop ( );	m_pUIStatusDef->setAlpha ( 0 );
	m_pUIStatusAgl->setVisible ( true );	m_pUIStatusAgl->setFrameInit ( 0 );	m_pUIStatusAgl->setLoop ( false );	m_pUIStatusAgl->stop ( );	m_pUIStatusAgl->setAlpha ( 0 );
	m_pUIStatusCrt->setVisible ( true );	m_pUIStatusCrt->setFrameInit ( 0 );	m_pUIStatusCrt->setLoop ( false );	m_pUIStatusCrt->stop ( );	m_pUIStatusCrt->setAlpha ( 0 );

	m_pUILevelUp->get ( )->setOpacity ( 0 );

	// time
	if ( nHp > 0 )
	{
		m_dLeftTime		  += 0.7f;
		m_dHpStartLeftTime = m_dLeftTime;
		m_dHpLeftTime	   = 1.2f;
	}
	if ( nAtk > 0 )
	{
		m_dLeftTime		   += 0.7f;
		m_dAtkStartLeftTime = m_dLeftTime;
		m_dAtkLeftTime		= 1.2f;
	}
	if ( nDef > 0 )
	{
		m_dLeftTime		   += 0.7f;
		m_dDefStartLeftTime = m_dLeftTime;
		m_dDefLeftTime		= 1.2f;
	}
	if ( nAgl > 0 )
	{
		m_dLeftTime		   += 0.7f;
		m_dAglStartLeftTime = m_dLeftTime;
		m_dAglLeftTime		= 1.2f;
	}
	if ( nCrt > 0 )
	{
		m_dLeftTime		   += 0.7f;
		m_dCrtStartLeftTime = m_dLeftTime;
		m_dCrtLeftTime		= 1.2f;
	}

	m_dLeftTime	  += 2.5f;
	m_dLvLeftTime  = 1.5f;
}

cMyRoomLvlUpEffect::~cMyRoomLvlUpEffect ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pUILevelUp	 );
	CC_SAFE_RELEASE ( m_pUIStatusHp	 );
	CC_SAFE_RELEASE ( m_pUIStatusAtk );
	CC_SAFE_RELEASE ( m_pUIStatusDef );
	CC_SAFE_RELEASE ( m_pUIStatusAgl );
	CC_SAFE_RELEASE ( m_pUIStatusCrt );
}

KDbool cMyRoomLvlUpEffect::update ( KDdouble dLeftTime )
{
	// left time
	m_dLeftTime -= dLeftTime;

	// time check
	if ( m_dLeftTime < 0 )
	{
		return true;
	}

	// level up
	if ( m_dLvLeftTime > 0 )
	{
		KDint		nAlpha = m_pUILevelUp->get ( )->getOpacity ( ) + 10;
		if ( nAlpha > 255 )
		{
			nAlpha = 255;
		}
		m_pUILevelUp->get ( )->setOpacity ( nAlpha );

		m_dLvLeftTime -= dLeftTime;
	}
	else
	{
		KDint		nAlpha = m_pUILevelUp->get ( )->getOpacity ( ) - 10;
		if ( nAlpha < 0 )
		{
			nAlpha = 0;
		}

		m_pUILevelUp->get ( )->setOpacity ( nAlpha );
	}

	// hp
	if ( m_nHp > 0 )
	{
		m_dHpStartLeftTime -= dLeftTime;
		if ( m_dHpStartLeftTime < 0 && m_dHpLeftTime > 0 )
		{
			m_dHpLeftTime -= dLeftTime;

			KDint		nAlpha = 255.0f * m_dHpLeftTime;
			if ( nAlpha <   0 )		nAlpha = 0;
			if ( nAlpha > 255 ) 	nAlpha = 255;

			m_pUIStatusHp->setPoint	( Point ( m_tPointHp.x, m_tPointHp.y + ( 1 - m_dHpLeftTime ) * 42 ) );
			m_pUIStatusHp->setAlpha ( nAlpha );

			if ( m_dHpLeftTime < 0 )
			{
				m_dHpLeftTime = 0;
			}
		}
	}
	// atk
	if ( m_nAtk > 0 )
	{
		m_dAtkStartLeftTime -= dLeftTime;
		if ( m_dAtkStartLeftTime < 0 && m_dAtkLeftTime > 0 )
		{
			m_dAtkLeftTime -= dLeftTime;

			KDint		nAlpha = 255.0f * m_dAtkLeftTime;
			if ( nAlpha <   0 )		nAlpha = 0;
			if ( nAlpha > 255 )		nAlpha = 255;
			
			m_pUIStatusAtk->setPoint ( Point ( m_tPointAtk.x, m_tPointAtk.y + ( 1 - m_dAtkLeftTime ) * 42 ) );
			m_pUIStatusAtk->setAlpha ( nAlpha );

			if ( m_dAtkLeftTime < 0 )
			{
				m_dAtkLeftTime = 0;
			}
		}
	}
	//def
	if ( m_nDef > 0 )
	{
		m_dDefStartLeftTime -= dLeftTime;
		if ( m_dDefStartLeftTime < 0 && m_dDefLeftTime > 0 )
		{
			m_dDefLeftTime -= dLeftTime;

			KDint		nAlpha = 255.0f * m_dDefLeftTime;
			if ( nAlpha <   0 )		nAlpha = 0;
			if ( nAlpha > 255 )		nAlpha = 255;

			m_pUIStatusDef->setPoint ( Point ( m_tPointDef.x, m_tPointDef.y + ( 1 - m_dDefLeftTime ) * 42 ) );
			m_pUIStatusDef->setAlpha ( nAlpha );

			if ( m_dDefLeftTime < 0 )
			{
				m_dDefLeftTime = 0;
			}
		}
	}
	// agl
	if ( m_nAgl > 0 )
	{
		m_dAglStartLeftTime -= dLeftTime;
		if ( m_dAglStartLeftTime < 0 && m_dAglLeftTime > 0 )
		{
			m_dAglLeftTime -= dLeftTime;

			KDint		nAlpha = 255.0f * m_dAglLeftTime;
			if ( nAlpha <   0 )		nAlpha = 0;
			if ( nAlpha > 255 )		nAlpha = 255;

			m_pUIStatusAgl->setPoint ( Point ( m_tPointAgl.x, m_tPointAgl.y + ( 1 - m_dAglLeftTime ) * 42 ) );
			m_pUIStatusAgl->setAlpha ( nAlpha );

			if ( m_dAglLeftTime < 0 )
			{
				m_dAglLeftTime = 0;
			}
		}
	}
	// crt
	if ( m_nCrt > 0 )
	{
		m_dCrtStartLeftTime -= dLeftTime;
		if ( m_dCrtStartLeftTime < 0 && m_dCrtLeftTime > 0 )
		{
			m_dCrtLeftTime -= dLeftTime;

			KDint		nAlpha = 255.0f * m_dCrtLeftTime;
			if ( nAlpha <   0 )		nAlpha = 0;
			if ( nAlpha > 255 )		nAlpha = 255;

			m_pUIStatusCrt->setPoint ( Point ( m_tPointCrt.x, m_tPointCrt.y + ( 1 - m_dCrtLeftTime ) * 42 ) );
			m_pUIStatusCrt->setAlpha ( nAlpha ); 

			if ( m_dCrtLeftTime < 0 )
			{
				m_dCrtLeftTime = 0;
			}
		}
	}
	
	// return
	return false;
}

KDvoid cMyRoomLvlUpEffect::setVisible ( KDbool bVisible )
{
	m_pUILevelUp->get ( )->setVisible ( bVisible );
	m_pUIStatusHp ->setVisible ( bVisible );
	m_pUIStatusAtk->setVisible ( bVisible );
	m_pUIStatusDef->setVisible ( bVisible );
	m_pUIStatusAgl->setVisible ( bVisible );
	m_pUIStatusCrt->setVisible ( bVisible );
}

KDvoid cMyRoomLvlUpEffect::setPoint ( Point tCenter, Point tSpecial )
{
	m_tPointCenter	= tCenter;
	m_tPointSpecial = tSpecial;

	m_pUILevelUp->get ( )->setPosition ( Point ( m_tPointCenter.x - 50, m_tPointCenter.y - 13 ) );

	if ( m_nHp  > 0 && m_dHpStartLeftTime  > 0 )	m_tPointHp  = tSpecial;
	if ( m_nAtk > 0 && m_dAtkStartLeftTime > 0 )	m_tPointAtk = tSpecial;
	if ( m_nDef > 0 && m_dDefStartLeftTime > 0 )	m_tPointDef = tSpecial;
	if ( m_nAgl > 0 && m_dAglStartLeftTime > 0 )	m_tPointAgl = tSpecial;
	if ( m_nCrt > 0 && m_dCrtStartLeftTime > 0 )	m_tPointCrt = tSpecial;
}

cLevelPopup::cLevelPopup ( Node* pNode, KDint z, cLabels* pLabels, const KDchar* pStr, Point tPoint )
: m_pSprBack	( nullptr )
, m_pLabels		( pLabels )
, m_pLabel		( nullptr )
, m_tPoint		( tPoint  )
, m_dLeftTime	( 3		  ) 
{
	m_pSprBack = new cSprite ( pNode, "MyRoom/myroom_lv_popup.png", z, Point::ZERO, Point ( _SCR_W, m_tPoint.y ) );
	m_pLabel   = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 420, m_tPoint.y + 2, 150, 30 ), pStr, z );

	m_pLabels->setColor ( m_pLabel, 255, 255, 0 );
	m_pLabels->setVisible ( m_pLabel, false );
}

cLevelPopup::~cLevelPopup ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprBack );

	m_pLabels->del ( m_pLabel );
}

KDbool cLevelPopup::update ( KDdouble dLeftTime )
{
	m_dLeftTime -= dLeftTime;

	if ( m_dLeftTime > 2.8f )
	{
		KDdouble	dPercent = ( 0.2f - ( m_dLeftTime - 2.8f ) ) / 0.2f;			// max == 1
		Point		tPoint  = Point ( _SCR_W - ( 150 * dPercent ), m_tPoint.y );

		m_pSprBack->get ( )->setPosition ( tPoint );
	}
	else if ( m_dLeftTime < 0.2f )
	{
		m_pLabels->setVisible ( m_pLabel, false );

		KDdouble	dPercent = ( 0.2f - m_dLeftTime ) / 0.2f;						// max == 1
		KDint		nAlpha	= 255 * ( 1 - dPercent );

		if ( nAlpha <	 0 )	nAlpha = 0;
		if ( nAlpha >= 255 )	nAlpha = 255;

		m_pSprBack->get ( )->setOpacity ( nAlpha );
	}
	else
	{
		m_pSprBack->get ( )->setPosition ( Point ( _SCR_W - 150, m_tPoint.y ) );
		m_pLabels->setVisible ( m_pLabel, true );
	}

	if ( m_dLeftTime < 0 )	return true;
	else					return false;
}

cSprProp::cSprProp ( Node* pNode, KDint z, Point tPoint )
: m_pNot		( nullptr )
, m_pCommon		( nullptr )
, m_pUncommon	( nullptr )
, m_pRare		( nullptr )
, m_pUnique		( nullptr )
{
	m_pNot		= new cSprite ( pNode, "MyRoom/myroom_prop_not.png"	    , z, Point::ZERO, tPoint );
	m_pCommon	= new cSprite ( pNode, "MyRoom/myroom_prop_common.png"  , z, Point::ZERO, tPoint );
	m_pUncommon	= new cSprite ( pNode, "MyRoom/myroom_prop_uncommon.png", z, Point::ZERO, tPoint );
	m_pRare		= new cSprite ( pNode, "MyRoom/myroom_prop_rare.png"	, z, Point::ZERO, tPoint );
	m_pUnique	= new cSprite ( pNode, "MyRoom/myroom_prop_unique.png"  , z, Point::ZERO, tPoint );

	m_pNot	   ->get ( )->setVisible ( false );
	m_pCommon  ->get ( )->setVisible ( false );
	m_pUncommon->get ( )->setVisible ( false );
	m_pRare	   ->get ( )->setVisible ( false );
	m_pUnique  ->get ( )->setVisible ( false );
}

cSprProp::~cSprProp ( KDvoid )
{
	m_pNot		= nullptr;
	m_pCommon	= nullptr;
	m_pUncommon	= nullptr;
	m_pRare		= nullptr;
	m_pUnique	= nullptr;
}

KDvoid cSprProp::setSelect ( eUniqueType eType )
{
	m_pNot		->get ( )->setVisible ( false );
	m_pCommon	->get ( )->setVisible ( false );
	m_pUncommon	->get ( )->setVisible ( false );
	m_pRare		->get ( )->setVisible ( false );
	m_pUnique	->get ( )->setVisible ( false );

	switch ( eType )
	{
		case eUniqueType_not		:	m_pNot		->get ( )->setVisible ( true );	break;
		case eUniqueType_common		:	m_pCommon	->get ( )->setVisible ( true );	break;
		case eUniqueType_uncommon	:	m_pUncommon	->get ( )->setVisible ( true );	break;
		case eUniqueType_rare		:	m_pRare		->get ( )->setVisible ( true );	break;
		case eUniqueType_unique		:	m_pUnique	->get ( )->setVisible ( true );	break;
	}
}

KDvoid cSprProp::setVisible ( KDbool bVisible )
{
	m_pNot		->get ( )->setVisible ( false );
	m_pCommon	->get ( )->setVisible ( false );
	m_pUncommon	->get ( )->setVisible ( false );
	m_pRare		->get ( )->setVisible ( false );
	m_pUnique	->get ( )->setVisible ( false );
}

KDvoid cSprProp::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	Color3B		tColor;

	tColor.r = r;
	tColor.g = g;
	tColor.b = b;

	m_pNot		->get ( )->setColor ( tColor );
	m_pCommon	->get ( )->setColor ( tColor );
	m_pUncommon	->get ( )->setColor ( tColor );
	m_pRare		->get ( )->setColor ( tColor );
	m_pUnique	->get ( )->setColor ( tColor );
}

cBtnSprite::cBtnSprite ( Node* pNode, const KDchar* pPathNor, const KDchar* pPathPush, KDint z, Point tPoint, Size tSizeTouch, KDbool bIsSound )
: m_pSprite			( nullptr		)
, m_pSpritePush		( nullptr		)
, m_bIsVisible		( true			)
, m_bIsTouchEnable	( false			)
, m_tPoint			( Point::ZERO	)
, m_tSizeTouch		( tSizeTouch	)
, m_tRectTouch		( Rect::ZERO	)
, m_bIsTouch		( false			)
, m_nTouchID		( 0				)
, m_bIsSound		( bIsSound		)
{
	m_pSprite	  = new cSprite ( pNode, pPathNor , z, Point::ZERO, Point::ZERO );
	m_pSpritePush = new cSprite ( pNode, pPathPush, z, Point::ZERO, Point::ZERO );

	setVisible		 ( false );
	setIsTouchEnable ( false );
	setPoint		 ( tPoint );
}

cBtnSprite::~cBtnSprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprite	);
	CC_SAFE_RELEASE ( m_pSpritePush );
}

KDbool cBtnSprite::touchesBegan ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false )
	{
		return false;
	}

	if ( RectInPoint ( m_tRectTouch, pTouch->getLocation ( ) ) == true )
	{
		if ( m_bIsTouch == false )
		{
			if (m_bIsSound == true && cMyData::getObj ( )->m_bIsOptionSound != false )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "button_select.mp3" );
			}
		}			

		m_pSprite	 ->get ( )->setVisible ( false );
		m_pSpritePush->get ( )->setVisible ( true );

		m_bIsTouch = true;
		m_nTouchID = pTouch->getID ( );

		return true;
	}

	return false;
}

KDbool cBtnSprite::touchesMoved ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false || m_bIsTouch == false || pTouch->getID ( ) != m_nTouchID )
	{
		return false;
	}

	if ( RectInPoint ( m_tRectTouch, pTouch->getLocation ( ) ) == false )
	{
		m_pSprite	 ->get ( )->setVisible ( true );
		m_pSpritePush->get ( )->setVisible ( false );

		m_bIsTouch = false;
		return false;
	}

	return true;
}

KDbool cBtnSprite::touchesEnded  ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false || m_bIsTouch == false || pTouch->getID ( ) != m_nTouchID )
	{
		return false;
	}

	m_pSprite	 ->get ( )->setVisible ( true );
	m_pSpritePush->get ( )->setVisible ( false );

	m_bIsTouch = false;

	if ( RectInPoint ( m_tRectTouch, pTouch->getLocation ( ) ) == true )
	{
		return true;
	}
	else
	{
		return false;
	}
}

KDvoid cBtnSprite::setVisible ( KDbool bIsVisible, KDbool bIsPush )
{
	if ( m_bIsVisible == bIsVisible )
	{
		return;
	}

	m_bIsVisible = bIsVisible;

	if ( bIsPush == false )
	{
		m_pSprite	 ->get ( )->setVisible ( bIsVisible );
		m_pSpritePush->get ( )->setVisible ( false );
	}
	else
	{
		m_pSprite	 ->get ( )->setVisible ( false );
		m_pSpritePush->get ( )->setVisible ( bIsVisible );
	}
}

KDvoid cBtnSprite::setIsTouchEnable ( KDbool bIsTouchEnable )
{
	m_bIsTouchEnable = bIsTouchEnable;
}

KDvoid cBtnSprite::setPoint ( Point tPoint )
{
	m_tPoint	 = tPoint;
	m_tRectTouch = Rect ( m_tPoint.x, m_tPoint.y, m_tSizeTouch.width, m_tSizeTouch.height );

	m_pSprite	 ->get ( )->setPosition ( tPoint );
	m_pSpritePush->get ( )->setPosition ( tPoint );
}

KDvoid cBtnSprite::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	Color3B		tColor;

	tColor.r = r;
	tColor.g = g;
	tColor.b = b;

	m_pSprite	 ->get ( )->setColor ( tColor );
	m_pSpritePush->get ( )->setColor ( tColor );
}


cBtnDragSprite::cBtnDragSprite ( Node* pNode, const KDchar* pPathNor, const KDchar* pPathPush, KDint z, Point tPoint, Size tSizeTouch )
: m_pSprite				( nullptr     )
, m_pSpritePush			( nullptr	  )
, m_bIsVisible			( false		  )
, m_bIsTouchEnable		( false		  )
, m_tPoint				( Point::ZERO )
, m_tSizeTouch			( tSizeTouch  )
, m_tRectTouch			( Rect::ZERO  )
, m_bIsTouch			( false		  )
, m_nTouchID			( 0			  )
, m_dTouchEndedLeftTime	( 0			  )
{
	m_pSprite		= new cSprite ( pNode, pPathNor , z, Point::ZERO, Point::ZERO );
	m_pSpritePush	= new cSprite ( pNode, pPathPush, z, Point::ZERO, Point::ZERO );

	setVisible		 ( false );
	setIsTouchEnable ( false );
	setPoint		 ( tPoint );
}

cBtnDragSprite::~cBtnDragSprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprite	);
	CC_SAFE_RELEASE ( m_pSpritePush );
}

KDbool cBtnDragSprite::touchesMoved ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false || ( m_bIsTouch == true && m_nTouchID != pTouch->getID ( ) ) )
	{
		return false;
	}

	if ( RectInPoint (m_tRectTouch, pTouch->getLocation ( ) ) == true )
	{
		if ( m_bIsTouch == false )
		{
			if ( cMyData::getObj ( )->m_bIsOptionSound != 0 )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "button_select.mp3" );
			}

			m_dTouchEndedLeftTime = 0.5f;
		}

		m_bIsTouch = true;
		m_nTouchID = pTouch->getID ( );

		m_pSprite	 ->get ( )->setVisible ( false );
		m_pSpritePush->get ( )->setVisible ( true );
	}
	else
	{
		m_pSprite	 ->get ( )->setVisible ( true );
		m_pSpritePush->get ( )->setVisible ( false );

		m_bIsTouch = false;
	}

	return m_bIsTouch;
}

KDvoid cBtnDragSprite::setVisible ( KDbool bIsVisible )
{
	m_bIsVisible = bIsVisible;

	m_pSprite 	 ->get ( )->setVisible ( bIsVisible );
	m_pSpritePush->get ( )->setVisible ( false );
}

KDvoid cBtnDragSprite::setIsTouchEnable ( KDbool bIsTouchEnable )
{
	m_bIsTouchEnable = bIsTouchEnable;
	m_bIsTouch		 = false;
}

KDvoid cBtnDragSprite::setPoint ( Point tPoint )
{
	m_tPoint = tPoint;
	m_tRectTouch = Rect ( m_tPoint.x, m_tPoint.y, m_tSizeTouch.width, m_tSizeTouch.height );

	m_pSprite	 ->get ( )->setPosition ( tPoint );
	m_pSpritePush->get ( )->setPosition ( tPoint );
}

KDbool cBtnDragSprite::update ( KDdouble dLeftTime )
{
	if ( m_bIsTouchEnable == true && m_bIsTouch == true )
	{
		m_dTouchEndedLeftTime -= dLeftTime;
		if ( m_dTouchEndedLeftTime < 0 ) 
		{
			m_dTouchEndedLeftTime = 0.5f;
			return true;
		}
	}

	return false;
}

KDvoid cBtnDragSprite::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	Color3B		tColor;

	tColor.r = r;
	tColor.g = g;
	tColor.b = b;

	m_pSprite	 ->get ( )->setColor ( tColor );
	m_pSpritePush->get ( )->setColor ( tColor );
}


cBtnNum::cBtnNum ( Node* pNode, KDint z, Point tPoint, Size tSizeTouch, KDint nNum )
: m_pLabelNum		( nullptr		)
, m_pLabelNumSelect	( nullptr		)
, m_pLabelNumPush	( nullptr		)
, m_nNum			( 0				)
, m_bIsVisible		( false			)
, m_bIsSelect		( false			)
, m_bIsTouchEnable	( false			)
, m_tPoint			( Point::ZERO	)
, m_tSizeTouch		( tSizeTouch	)
, m_tRectTouch		( Rect::ZERO	)
, m_bIsTouch		( false			)
, m_nTouchID		( 0				)
{
	m_pLabelNum		  = new cLabelAtlasCustom ( pNode, "MyRoom/myroom_page_num.png" , 25, z );
	m_pLabelNumSelect = new cLabelAtlasCustom ( pNode, "MyRoom/myroom_page_num3.png", 25, z );
	m_pLabelNumPush	  = new cLabelAtlasCustom ( pNode, "MyRoom/myroom_page_num2.png", 25, z );

	m_pLabelNum->addCharInfo ( '1', 12 );	m_pLabelNum->addCharInfo ( '2', 18 );	m_pLabelNum->addCharInfo ( '3', 18 );
	m_pLabelNum->addCharInfo ( '4', 18 );	m_pLabelNum->addCharInfo ( '5', 18 );	m_pLabelNum->addCharInfo ( '6', 18 );
	m_pLabelNum->addCharInfo ( '7', 18 );	m_pLabelNum->addCharInfo ( '8', 18 );	m_pLabelNum->addCharInfo ( '9', 18 );
	m_pLabelNum->addCharInfo ( '0', 18 );

	m_pLabelNumSelect->addCharInfo ( '1', 12 );	m_pLabelNumSelect->addCharInfo ( '2', 18 );	m_pLabelNumSelect->addCharInfo ( '3', 18 );
	m_pLabelNumSelect->addCharInfo ( '4', 18 );	m_pLabelNumSelect->addCharInfo ( '5', 18 );	m_pLabelNumSelect->addCharInfo ( '6', 18 );
	m_pLabelNumSelect->addCharInfo ( '7', 18 );	m_pLabelNumSelect->addCharInfo ( '8', 18 );	m_pLabelNumSelect->addCharInfo ( '9', 18 );
	m_pLabelNumSelect->addCharInfo ( '0', 18 );

	m_pLabelNumPush->addCharInfo ( '1', 12 );	m_pLabelNumPush->addCharInfo ( '2', 18 );	m_pLabelNumPush->addCharInfo ( '3', 18 );
	m_pLabelNumPush->addCharInfo ( '4', 18 );	m_pLabelNumPush->addCharInfo ( '5', 18 );	m_pLabelNumPush->addCharInfo ( '6', 18 );
	m_pLabelNumPush->addCharInfo ( '7', 18 );	m_pLabelNumPush->addCharInfo ( '8', 18 );	m_pLabelNumPush->addCharInfo ( '9', 18 );
	m_pLabelNumPush->addCharInfo ( '0', 18 );

	setNum			 ( 0	  );
	setVisible		 ( false  );
	setIsSelect		 ( false  );
	setIsTouchEnable ( false  );
	setPoint		 ( tPoint );
}

cBtnNum::~cBtnNum ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLabelNum		);
	CC_SAFE_RELEASE ( m_pLabelNumSelect );
	CC_SAFE_RELEASE ( m_pLabelNumPush	);
}

KDbool cBtnNum::touchesBegan ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false )	
	{
		return false;
	}

	if ( RectInPoint ( m_tRectTouch, pTouch->getLocation ( ) ) == true )
	{
		m_pLabelNum		 ->setVisible ( false );
		m_pLabelNumSelect->setVisible ( false );
		m_pLabelNumPush	 ->setVisible ( true  );

		m_bIsTouch = true;
		m_nTouchID = pTouch->getID ( );

		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "button_select.mp3" );
		}

		return true;
	}

	return false;
}

KDbool cBtnNum::touchesMoved ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false || m_bIsTouch == false || pTouch->getID ( ) != m_nTouchID )
	{
		return false;
	}

	if ( RectInPoint ( m_tRectTouch, pTouch->getLocation ( ) ) == false)
	{
		if ( m_bIsSelect == false )
		{
			m_pLabelNum		 ->setVisible ( true  );
			m_pLabelNumSelect->setVisible ( false );
			m_pLabelNumPush	 ->setVisible ( false );
		}
		else
		{
			m_pLabelNum		 ->setVisible ( false );
			m_pLabelNumSelect->setVisible ( true  );
			m_pLabelNumPush	 ->setVisible ( false );
		}

		m_bIsTouch = false;

		return false;
	}

	return true;
}

KDbool cBtnNum::touchesEnded ( Touch* pTouch )
{
	if ( m_bIsTouchEnable == false || m_bIsTouch == false || pTouch->getID ( ) != m_nTouchID )
	{
		return false;
	}

	if ( m_bIsSelect == false )
	{
		m_pLabelNum		 ->setVisible ( true  );
		m_pLabelNumSelect->setVisible ( false );
		m_pLabelNumPush	 ->setVisible ( false );
	}
	else
	{
		m_pLabelNum		 ->setVisible ( false );
		m_pLabelNumSelect->setVisible ( true  );
		m_pLabelNumPush	 ->setVisible ( false );
	}

	m_bIsTouch = false;

	if ( RectInPoint ( m_tRectTouch, pTouch->getLocation ( ) ) == true )
	{
		return true;
	}
	else
	{
		return false;
	}
}

KDvoid cBtnNum::setNum ( KDint nNum )
{
	m_nNum = nNum;

	m_pLabelNum		 ->setString ( ccszf ( "%d", m_nNum ) );
	m_pLabelNumSelect->setString ( ccszf ( "%d", m_nNum ) );
	m_pLabelNumPush  ->setString ( ccszf ( "%d", m_nNum ) );

	setPoint ( m_tPoint );
}

KDvoid cBtnNum::setVisible ( KDbool bIsVisible )
{
	m_bIsVisible = bIsVisible;

	setIsSelect ( m_bIsSelect );
}

KDvoid cBtnNum::setIsSelect ( KDbool bIsSelect )
{
	m_bIsSelect = bIsSelect;

	if ( m_bIsVisible == false )
	{
		m_pLabelNum		 ->setVisible ( false );
		m_pLabelNumSelect->setVisible ( false );
		m_pLabelNumPush	 ->setVisible ( false );

		return;
	}

	if ( m_bIsSelect == true )
	{
		m_pLabelNum		 ->setVisible ( false );
		m_pLabelNumSelect->setVisible ( true  );
		m_pLabelNumPush	 ->setVisible ( false );
	}
	else
	{
		m_pLabelNum		 ->setVisible ( true  );
		m_pLabelNumSelect->setVisible ( false );
		m_pLabelNumPush	 ->setVisible ( false );
	}
}

KDvoid cBtnNum::setIsTouchEnable ( KDbool bIsTouchEnable )
{
	m_bIsTouchEnable = bIsTouchEnable;
}

KDvoid cBtnNum::setPoint ( Point tPoint )
{
	m_tPoint = tPoint;
	m_tRectTouch = Rect ( m_tPoint.x - m_tSizeTouch.width / 2, m_tPoint.y + (KDint) m_pLabelNum->getSize ( ).height / 2 - m_tSizeTouch.height / 2, m_tSizeTouch.width, m_tSizeTouch.height );

	m_pLabelNum		 ->setPosition ( Point ( tPoint.x - (KDint) m_pLabelNum	     ->getSize ( ).width / 2, tPoint.y ) );
	m_pLabelNumSelect->setPosition ( Point ( tPoint.x - (KDint) m_pLabelNumSelect->getSize ( ).width / 2, tPoint.y ) );
	m_pLabelNumPush	 ->setPosition ( Point ( tPoint.x - (KDint) m_pLabelNumPush  ->getSize ( ).width / 2, tPoint.y ) );
}

KDvoid cBtnNum::setColor ( KDubyte r, KDubyte g, KDubyte b )
{
	m_pLabelNum		 ->setColor ( 255, r, g, b );
	m_pLabelNumSelect->setColor ( 255, r, g, b );
	m_pLabelNumPush	 ->setColor ( 255, r, g, b );
}

MyRoomNode::MyRoomNode ( KDvoid )
{
	m_pSprBack			= nullptr;
	m_pSprBackLeft		= nullptr;
	m_pSprBackTop		= nullptr;
	m_pSprBackTopAdd	= nullptr;
	m_pSprBackBottom	= nullptr;
	m_pSprBack2			= nullptr;
	m_pNodeChangeImg	= nullptr;
}

MyRoomNode::~MyRoomNode ( KDvoid )
{
	this->releaseObject ( );
}

MyRoomNode* MyRoomNode::create ( KDint nSelectUnitID )
{
	MyRoomNode*		pRet = new MyRoomNode ( );

	if ( pRet && pRet->init ( nSelectUnitID ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool MyRoomNode::init ( KDint nSelectUnitID )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// init random
	kdSrand ( kdTime ( 0 ) );

	// switch
	auto	pListener = EventListenerTouchOneByOne::create ( );
	pListener->onTouchBegan = CC_CALLBACK_2 ( MyRoomNode::onTouchBegan, this );
	pListener->onTouchMoved = CC_CALLBACK_2 ( MyRoomNode::onTouchMoved, this );
	pListener->onTouchEnded = CC_CALLBACK_2 ( MyRoomNode::onTouchEnded, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pListener, this );

	// time
	m_dTickCount = 0;
	m_dBeforeTickCount	= 0;

	// unit crate info
	m_pUnitCreateCoin1	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_1.par"	);
	m_pUnitCreateCoin2	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_2.par"	);
	m_pUnitCreateCoin3	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_3.par"	);
	m_pUnitCreateCoin4	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_4.par"	);
	m_pUnitCreateCoin5	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_5.par"	);
	m_pUnitCreateCoin6	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_6.par"	);
	m_pUnitCreateCoin7	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_7.par"	);
	m_pUnitCreateCoin8	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_8.par"	);
	m_pUnitCreateCoin9	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_9.par"	);
	m_pUnitCreateCoin10	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_10.par"	);
	m_pUnitCreateCoin11	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_11.par"	);
	m_pUnitCreateCoin12	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_12.par"	);
	m_pUnitCreateCoin13	  = new cUnitCreateInfos ( "myRoomMobCreateInfos_coin_13.par"	);
	m_pUnitCreatePremium1 = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_1.par" );
	m_pUnitCreatePremium2 = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_2.par" );
	m_pUnitCreatePremium3 = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_3.par" );
	m_pUnitCreatePremium4 = new cUnitCreateInfos ( "myRoomMobCreateInfos_premium_4.par" );

	if ( cMyData::getObj ( )->m_bIsFirstIntoMyRoom == true )
	{
		std::vector<cUnitCreateInfo*>	aCreateInfos;

		for ( KDint i = 0; i < m_pUnitCreateCoin1->getUnitInfoCount ( ); i++ )
		{
			cUnitCreateInfo*	pCreateInfo = (cUnitCreateInfo*) m_pUnitCreateCoin1->getUnitInfo ( i );

			if ( pCreateInfo->nCasinoAdvantageORuniqueType == eUniqueType_rare )
			{
				aCreateInfos.push_back ( pCreateInfo );
			}
		}

		cUnitCreateInfo*		pCreateInfo = aCreateInfos [ kdRand ( ) % aCreateInfos.size ( ) ];

		cMyRoomUnitSaveData		tSaveData;

		tSaveData.nCreateInfoFileType	= eCreateInfoFileType_Coin_1;
		tSaveData.nUnitID				= cMyData::getObj ( )->getNewMyRoomUnitID ( );
		tSaveData.nCreateIndex			= pCreateInfo->nCreateIndex;
		tSaveData.nSlotIndex			= 0;
		tSaveData.aHp  [ 0 ]			= pCreateInfo->aHp  [ 0 ] + kdRand ( ) % ( pCreateInfo->aHp  [ 1 ] - pCreateInfo->aHp  [ 0 ] + 1 );
		tSaveData.aAtk [ 0 ]			= pCreateInfo->aAtk [ 0 ] + kdRand ( ) % ( pCreateInfo->aAtk [ 1 ] - pCreateInfo->aAtk [ 0 ] + 1 );
		tSaveData.aDef [ 0 ]			= pCreateInfo->aDef [ 0 ] + kdRand ( ) % ( pCreateInfo->aDef [ 1 ] - pCreateInfo->aDef [ 0 ] + 1 );
		tSaveData.aAgl [ 0 ]			= pCreateInfo->aAgl [ 0 ] + kdRand ( ) % ( pCreateInfo->aAgl [ 1 ] - pCreateInfo->aAgl [ 0 ] + 1 );
		tSaveData.aCrt [ 0 ]			= pCreateInfo->aCrt [ 0 ] + kdRand ( ) % ( pCreateInfo->aCrt [ 1 ] - pCreateInfo->aCrt [ 0 ] + 1 );
		tSaveData.aHp  [ 1 ]			= pCreateInfo->aLvUpHp  [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpHp  [ 1 ] - pCreateInfo->aLvUpHp  [ 0 ] + 1 );
		tSaveData.aAtk [ 1 ]			= pCreateInfo->aLvUpAtk [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpAtk [ 1 ] - pCreateInfo->aLvUpAtk [ 0 ] + 1 );
		tSaveData.aDef [ 1 ]			= pCreateInfo->aLvUpDef [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpDef [ 1 ] - pCreateInfo->aLvUpDef [ 0 ] + 1 );
		tSaveData.aAgl [ 1 ]			= pCreateInfo->aLvUpAgl [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpAgl [ 1 ] - pCreateInfo->aLvUpAgl [ 0 ] + 1 );
		tSaveData.aCrt [ 1 ]			= pCreateInfo->aLvUpCrt [ 0 ] + kdRand ( ) % ( pCreateInfo->aLvUpCrt [ 1 ] - pCreateInfo->aLvUpCrt [ 0 ] + 1 );
		tSaveData.fExp					= 0;
		tSaveData.fClassExp				= 0;
		tSaveData.fFriendshipExp		= kdRand ( ) % FRIENDSHIP_BEST + 30;
		tSaveData.nKillCount			= 0;

		cMyData::getObj ( )->addMyRoomUnitSaveDatas ( tSaveData );

		cMyData::getObj ( )->m_bIsFirstIntoMyRoom = false;
	}

	// back img
	m_pSprBack			= new cSprite ( this, "MyRoom/myroom_bg.png"		, LAYER_BACK		  , Point::ZERO, Point::ZERO		);
	m_pSprBackLeft		= new cSprite ( this, "MyRoom/myroom_bg_left.png"	, LAYER_MYROOM_UI_1	  , Point::ZERO, Point (   1,  29 ) );
	m_pSprBackTop		= new cSprite ( this, "MyRoom/myroom_bg_top.png"	, LAYER_MYROOM_UI_1	  , Point::ZERO, Point ( 131, 296 ) );
	m_pSprBackTopAdd	= new cSprite ( this, "MyRoom/myroom_bg_top_add.png", LAYER_MYROOM_UI_1	  , Point::ZERO, Point ( 131, 296 ) );
	m_pSprBackBottom	= new cSprite ( this, "MyRoom/myroom_bg_bottom.png"	, LAYER_MYROOM_UI_1	  , Point::ZERO, Point ( 198,	0 ) );
	m_pSprBack2			= new cSprite ( this, "MyRoom/myroom_bg.png"		, LAYER_MYROOM_COVER_1, Point::ZERO, Point::ZERO	    );
	m_pSprBack2->get ( )->setVisible ( false );

	// cover color
	Color4B			tColor;
	Color4B			tColor2;
	tColor .r = 0;	tColor .g = 0;	tColor .b = 0;	tColor .a = 100;
	tColor2.r = 0;	tColor2.g = 0;	tColor2.b = 0;	tColor2.a = 200;

	m_pCoverColor  = LayerColor::create ( tColor,  _SCR_W, _SCR_H );
	m_pCoverColor2 = LayerColor::create ( tColor2, _SCR_W, _SCR_H );
	m_pCoverColor ->setPosition ( Point ( 0, 0 ) );
	m_pCoverColor2->setPosition ( Point ( 0, 0 ) );
	m_pCoverColor ->setVisible ( false );
	m_pCoverColor2->setVisible ( false );

	this->addChild ( m_pCoverColor , LAYER_MYROOM_COVER_3 );
	this->addChild ( m_pCoverColor2, LAYER_MYROOM_COVER_2 );

	// labels
	m_pLabels = new cLabels ( this );

	// page
	m_nPageMax			  = cMyData::getObj ( )->m_nSlotCount / 3;
	m_nViewFirstPageIndex = 0;

	// unit
	for ( auto& rSaveData : cMyData::getObj ( )->m_aMyRoomUnitSaveDatas )
	{
		cUnit*		pUnit = nullptr;
		
		// create
		switch ( rSaveData.nCreateInfoFileType )
		{
			case eCreateInfoFileType_Coin_1		:	pUnit = new cUnit ( this, m_pUnitCreateCoin1	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_1	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin1   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_2		:	pUnit = new cUnit ( this, m_pUnitCreateCoin2	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_2	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin2   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_3		:	pUnit = new cUnit ( this, m_pUnitCreateCoin3	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_3	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin3   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_4		:	pUnit = new cUnit ( this, m_pUnitCreateCoin4	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_4	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin4   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_5		:	pUnit = new cUnit ( this, m_pUnitCreateCoin5	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_5	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin5   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_6		:	pUnit = new cUnit ( this, m_pUnitCreateCoin6	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_6	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin6   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_7		:	pUnit = new cUnit ( this, m_pUnitCreateCoin7	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_7	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin7   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_8		:	pUnit = new cUnit ( this, m_pUnitCreateCoin8	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_8	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin8   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_9		:	pUnit = new cUnit ( this, m_pUnitCreateCoin9	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_9	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin9   ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_10	:	pUnit = new cUnit ( this, m_pUnitCreateCoin10	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_10	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin10  ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_11	:	pUnit = new cUnit ( this, m_pUnitCreateCoin11	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_11	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin11  ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_12	:	pUnit = new cUnit ( this, m_pUnitCreateCoin12	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_12	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin12  ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Coin_13	:	pUnit = new cUnit ( this, m_pUnitCreateCoin13	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Coin_13	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreateCoin13  ->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Premium_1	:	pUnit = new cUnit ( this, m_pUnitCreatePremium1	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Premium_1	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreatePremium1->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Premium_2	:	pUnit = new cUnit ( this, m_pUnitCreatePremium2	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Premium_2	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreatePremium2->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Premium_3	:	pUnit = new cUnit ( this, m_pUnitCreatePremium3	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Premium_3	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreatePremium3->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
			case eCreateInfoFileType_Premium_4	:	pUnit = new cUnit ( this, m_pUnitCreatePremium4	->getUnitInfo ( rSaveData.nCreateIndex ), eCreateInfoFileType_Premium_4	, LAYER_MYROOM_MOB, rSaveData.nUnitID, m_pUnitCreatePremium4->getUnitInfo ( rSaveData.nCreateIndex )->nCreateIndex, 0 );	break;
		}

		// load
		pUnit->load ( (eCreateInfoFileType) rSaveData.nCreateInfoFileType, rSaveData.nSlotIndex, rSaveData.aHp [ 0 ], rSaveData.aAtk [ 0 ], rSaveData.aDef [ 0 ], rSaveData.aAgl [ 0 ], rSaveData.aCrt [ 0 ], rSaveData.aHp [ 1 ], rSaveData.aAtk [ 1 ], rSaveData.aDef [ 1 ], rSaveData.aAgl [ 1 ], rSaveData.aCrt [ 1 ], rSaveData.fExp, rSaveData.fClassExp, rSaveData.fFriendshipExp, rSaveData.nKillCount );

		// arcade clear
		pUnit->setIsArcadeEasyClear ( rSaveData.nIsArcadeEasyClear );
		pUnit->setIsArcadeNorClear  ( rSaveData.nIsArcadeNorClear  );
		pUnit->setIsArcadeHardClear ( rSaveData.nIsArcadeHardClear );

		// point
		pUnit->setPointNameA ( Point ( 13, 318 ) );

		// visible
		pUnit->setVisibleNameA ( false );
		pUnit->setVisible	   ( false );

		// level up
		while ( pUnit->getExp ( ) >= 100 )
		{
			// level up
			this->setUnitLevelUp ( pUnit, false );

			// exp
			pUnit->setExp ( pUnit->getExp ( ) - 100 );
		}

		// push_back
		m_aUnits.push_back ( pUnit );
	}

	// unit save
	cMyData::getObj ( )->updateMyRoomUnitSaveDatas ( m_aUnits );

	// select & drag unit
	m_pUnitSelect		= nullptr;
	m_pUnitDrag			= nullptr;
	m_nChangeSlotIndex	= -1;

	// point
	m_aUnit [ 0 ] = Point ( 191, 138 );
	m_aUnit [ 1 ] = Point ( 302, 138 );
	m_aUnit [ 2 ] = Point ( 413, 138 );

	// rect
	m_aRectChange [ 0 ] = Rect ( 142, 110, 96, 160 );
	m_aRectChange [ 1 ] = Rect ( 253, 110, 96, 160 );
	m_aRectChange [ 2 ] = Rect ( 364, 110, 96, 160 );

	// UI prop
	for ( KDint i = 0; i < _SLOT_IN_PAGE_COUNT; i++)
	{
		m_aUIProps.push_back ( new cSprProp ( this, LAYER_MYROOM_UI_PROP, Point ( m_aUnit [ i ].x - 50, m_aUnit [ i ].y - 28 ) ) );
	}

	//btns
	m_pBtnExit				= Menu::createWithItem ( MenuItemImage::create ( "Bet/bet_exit.png"								, "Bet/bet_exit_push.png"								, CC_CALLBACK_1 ( MyRoomNode::exitCallback				, this ) ) );
	m_pBtnShop				= Menu::createWithItem ( SoundMenuItem::create ( "Bet/bet_shop.png"								, "Bet/bet_shop_push.png"								, CC_CALLBACK_1 ( MyRoomNode::shopCallback				, this ) ) );
	m_pBtnShopExit			= Menu::createWithItem ( SoundMenuItem::create ( "Shop/shop_exit.png"							, "Shop/shop_exit_push.png"								, CC_CALLBACK_1 ( MyRoomNode::shopExitCallback			, this ) ) );
	m_pBtnUnitDeleteYes		= Menu::createWithItem ( MenuItemImage::create ( "MyRoom/myroom_delete_unit_messagebox_yes.png"	, "MyRoom/myroom_delete_unit_messagebox_yes_push.png"	, CC_CALLBACK_1 ( MyRoomNode::unitDeleteYesCallback		, this ) ) );
	m_pBtnUnitDeleteNo		= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_delete_unit_messagebox_no.png"	, "MyRoom/myroom_delete_unit_messagebox_no_push.png"	, CC_CALLBACK_1 ( MyRoomNode::unitDeleteNoCallback		, this ) ) );
	m_pBtnMachineLeft		= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_machine_left.png"				, "MyRoom/myroom_machine_left_push.png"					, CC_CALLBACK_1 ( MyRoomNode::machineLeftCallback		, this ) ) );
	m_pBtnMachineRight		= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_machine_right.png"				, "MyRoom/myroom_machine_right_push.png"				, CC_CALLBACK_1 ( MyRoomNode::machineRightCallback		, this ) ) );
	m_pBtnMachineHandle		= Menu::createWithItem ( MenuItemImage::create ( "MyRoom/myroom_btn_machine_handle.png"			, "MyRoom/myroom_btn_machine_handle.png"				, CC_CALLBACK_1 ( MyRoomNode::machineHandleCallback		, this ) ) );
	m_pBtnGain				= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_gain.png"					, "MyRoom/myroom_btn_gain_push.png"						, CC_CALLBACK_1 ( MyRoomNode::gainCallback				, this ) ) );
	m_pBtnModeExit			= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_exit.png"				, "MyRoom/myroom_btn_mode_exit_push.png"				, CC_CALLBACK_1 ( MyRoomNode::modeExitCallback			, this ) ) );
	m_pBtnModeCasino100		= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_casino_100.png"		, "MyRoom/myroom_btn_mode_casino_100_push.png"			, CC_CALLBACK_1 ( MyRoomNode::modeCasino100Callback		, this ) ) );
	m_pBtnModeCasino1000	= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_casino_1000.png"		, "MyRoom/myroom_btn_mode_casino_1000_push.png"			, CC_CALLBACK_1 ( MyRoomNode::modeCasino1000Callback	, this ) ) );
	m_pBtnModeArcadeEasy	= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_arcade_easy.png"		, "MyRoom/myroom_btn_mode_arcade_easy_push.png"			, CC_CALLBACK_1 ( MyRoomNode::modeArcadeEasyCallback	, this ) ) );
	m_pBtnModeArcadeNormal	= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_arcade_normal.png"		, "MyRoom/myroom_btn_mode_arcade_normal_push.png"		, CC_CALLBACK_1 ( MyRoomNode::modeArcadeNormalCallback	, this ) ) );
	m_pBtnModeArcadeHard	= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_arcade_hard.png"		, "MyRoom/myroom_btn_mode_arcade_hard_push.png"			, CC_CALLBACK_1 ( MyRoomNode::modeArcadeHardCallback	, this ) ) );
	m_pBtnModeSurvival		= Menu::createWithItem ( SoundMenuItem::create ( "MyRoom/myroom_btn_mode_survival.png"			, "MyRoom/myroom_btn_mode_survival_push.png"			, CC_CALLBACK_1 ( MyRoomNode::modeSurvivalCallback		, this ) ) );

	m_pBtnExit				->setPosition ( Point (	 33			,  13.5f		) );
	m_pBtnShop				->setPosition ( Point (	 98			,  13.5f		) );
	m_pBtnShopExit			->setPosition ( Point (	 62			,  96			) );
	m_pBtnUnitDeleteYes		->setPosition ( Point (	  0			,   0			) );
	m_pBtnUnitDeleteNo		->setPosition ( Point (	  0			,   0			) );
	m_pBtnMachineLeft		->setPosition ( Point (	261			, 165			) );
	m_pBtnMachineRight		->setPosition ( Point (	450			, 165			) );
	m_pBtnMachineHandle		->setPosition ( Point (	366			, 120			) );
	m_pBtnGain				->setPosition ( Point (	239.5f		,  45.5f		) );
	m_pBtnModeExit			->setPosition ( Point ( 401 + 18.5f	, 280 + 10		) );
	m_pBtnModeCasino100		->setPosition ( Point (  36 + 41.5f	,  39 + 53.5f	) );
	m_pBtnModeCasino1000	->setPosition ( Point ( 144 + 41.5f	,  39 + 53.5f	) );
	m_pBtnModeArcadeEasy	->setPosition ( Point (  36 + 41.5f	, 157 + 53.5f	) );
	m_pBtnModeArcadeNormal	->setPosition ( Point ( 144 + 41.5f	, 157 + 53.5f	) );
	m_pBtnModeArcadeHard	->setPosition ( Point ( 253 + 41.5f	, 157 + 53.5f	) );
	m_pBtnModeSurvival		->setPosition ( Point ( 361 + 41.5f	, 157 + 53.5f	) );

	m_pBtnExit				->setVisible ( true );
	m_pBtnShop				->setVisible ( true );
	m_pBtnShopExit			->setVisible ( false );
	m_pBtnUnitDeleteYes		->setVisible ( false );
	m_pBtnUnitDeleteNo		->setVisible ( false );
	m_pBtnMachineLeft		->setVisible ( false );
	m_pBtnMachineRight		->setVisible ( false );
	m_pBtnMachineHandle		->setVisible ( false );
	m_pBtnGain				->setVisible ( false );
	m_pBtnModeExit			->setVisible ( false );
	m_pBtnModeCasino100		->setVisible ( false );
	m_pBtnModeCasino1000	->setVisible ( false );
	m_pBtnModeArcadeEasy	->setVisible ( false );
	m_pBtnModeArcadeNormal	->setVisible ( false );
	m_pBtnModeArcadeHard	->setVisible ( false );
	m_pBtnModeSurvival		->setVisible ( false );

	m_pBtnExit				->setEnabled ( false );
	m_pBtnShop				->setEnabled ( false );
	m_pBtnShopExit			->setEnabled ( false );
	m_pBtnUnitDeleteYes		->setEnabled ( false );
	m_pBtnUnitDeleteNo		->setEnabled ( false );
	m_pBtnMachineLeft		->setEnabled ( false );
	m_pBtnMachineRight		->setEnabled ( false );
	m_pBtnMachineHandle		->setEnabled ( false );
	m_pBtnGain				->setEnabled ( false );
	m_pBtnModeExit			->setEnabled ( false );
	m_pBtnModeCasino100		->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	this->addChild ( m_pBtnExit				, LAYER_MYROOM_BTN		);
	this->addChild ( m_pBtnShop				, LAYER_MYROOM_BTN		);
	this->addChild ( m_pBtnShopExit			, LAYER_MYROOM_SHOP_BTN	);
	this->addChild ( m_pBtnUnitDeleteYes	, LAYER_MYROOM_BTN_4	);
	this->addChild ( m_pBtnUnitDeleteNo		, LAYER_MYROOM_BTN_4	);
	this->addChild ( m_pBtnMachineLeft		, LAYER_MYROOM_BTN		);
	this->addChild ( m_pBtnMachineRight		, LAYER_MYROOM_BTN		);
	this->addChild ( m_pBtnMachineHandle	, LAYER_MYROOM_BTN		);
	this->addChild ( m_pBtnGain				, LAYER_MYROOM_BTN_4	);
	this->addChild ( m_pBtnModeExit			, LAYER_MYROOM_MODE_BTN );
	this->addChild ( m_pBtnModeCasino100	, LAYER_MYROOM_MODE_BTN	);
	this->addChild ( m_pBtnModeCasino1000	, LAYER_MYROOM_MODE_BTN	);
	this->addChild ( m_pBtnModeArcadeEasy	, LAYER_MYROOM_MODE_BTN	);
	this->addChild ( m_pBtnModeArcadeNormal , LAYER_MYROOM_MODE_BTN	);
	this->addChild ( m_pBtnModeArcadeHard	, LAYER_MYROOM_MODE_BTN	);
	this->addChild ( m_pBtnModeSurvival		, LAYER_MYROOM_MODE_BTN	);

	m_pBtnBack			= new cBtnSprite	 ( this, "MyRoom/myroom_btn_back.png"		, "MyRoom/myroom_btn_back_push.png"			, LAYER_MYROOM_BTN	, Point (   0,  0 ), Size ( 81, 27 ), true );
	m_pBtnShop2			= new cBtnSprite	 ( this, "MyRoom/myroom_btn_shop.png"		, "MyRoom/myroom_btn_shop_push.png"			, LAYER_MYROOM_BTN	, Point (  80,  0 ), Size ( 73, 27 ), true );
	m_pBtnPageLeft		= new cBtnSprite	 ( this, "MyRoom/myroom_page_left.png"		, "MyRoom/myroom_page_left_push.png"		, LAYER_MYROOM_BTN_2, Point (   0,  0 ), Size ( 39, 31 ), true );
	m_pBtnPageRight		= new cBtnSprite	 ( this, "MyRoom/myroom_page_right.png"		, "MyRoom/myroom_page_right_push.png"		, LAYER_MYROOM_BTN_2, Point (   0,  0 ), Size ( 39, 31 ), true );
	m_pBtnPageDragLeft	= new cBtnDragSprite ( this, "MyRoom/myroom_page_drag_left.png" , "MyRoom/myroom_page_drag_left_push.png"	, LAYER_MYROOM_BTN_3, Point ( 132, 10 ), Size ( 80, 58 )	   );
	m_pBtnPageDragRight	= new cBtnDragSprite ( this, "MyRoom/myroom_page_drag_right.png", "MyRoom/myroom_page_drag_right_push.png"	, LAYER_MYROOM_BTN_3, Point ( 400, 10 ), Size ( 80, 58 )	   );
	m_pBtnSocial		= new cBtnSprite	 ( this, "MyRoom/myroom_btn_social.png"		, "MyRoom/myroom_btn_social_push.png"		, LAYER_MYROOM_BTN	, Point ( 228,  4 ), Size ( 85, 59 ), true );
	m_pBtnCapsule		= new cBtnSprite	 ( this, "MyRoom/myroom_btn_capsule.png"	, "MyRoom/myroom_btn_capsule_push.png"		, LAYER_MYROOM_BTN	, Point ( 322, -4 ), Size ( 63, 67 ), true );
	m_pBtnWastebasket	= new cBtnSprite	 ( this, "MyRoom/myroom_btn_wastebasket.png", "MyRoom/myroom_btn_wastebasket_push.png"	, LAYER_MYROOM_BTN_3, Point (  10, -4 ), Size ( 58, 65 ), true );
	m_pBtnBattle		= new cBtnSprite	 ( this, "MyRoom/myroom_btn_battle.png"		, "MyRoom/myroom_btn_battle_push.png"		, LAYER_MYROOM_BTN	, Point ( 390, -5 ), Size ( 85, 60 ), true );

	m_pBtnBack			->setVisible ( false );
	m_pBtnShop2			->setVisible ( false );
	m_pBtnPageLeft		->setVisible ( false );
	m_pBtnPageRight		->setVisible ( false );
	m_pBtnPageDragLeft	->setVisible ( false );
	m_pBtnPageDragRight	->setVisible ( false );
	m_pBtnSocial		->setVisible ( false );
	m_pBtnCapsule		->setVisible ( true  );
	m_pBtnWastebasket	->setVisible ( false );
	m_pBtnBattle		->setVisible ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );

	for ( KDint i = 0; i < _NUM_IN_PAGE_COUNT; i++ )
	{
		m_aBtnPageNums.push_back ( new cBtnNum ( this, LAYER_MYROOM_BTN_2, Point::ZERO, Size ( 44, 44 ), 0 ) );
	}

	// page
	this->setPage ( cMyData::getObj ( )->m_nMyRoomPage, true );

	// UI money
	m_pUIMoney = new cLabelAtlasCustom ( this, "Bet/coin_nums_s.png", 15, LAYER_MYROOM_UI_1 );
	m_pUIMoney->addCharInfo ( ',',  7 );	m_pUIMoney->addCharInfo ( '0', 15 );	m_pUIMoney->addCharInfo ( '1', 15 );
	m_pUIMoney->addCharInfo ( '2', 15 );	m_pUIMoney->addCharInfo ( '3', 15 );	m_pUIMoney->addCharInfo ( '4', 15 );
	m_pUIMoney->addCharInfo ( '5', 15 );	m_pUIMoney->addCharInfo ( '6', 15 );	m_pUIMoney->addCharInfo ( '7', 15 );
	m_pUIMoney->addCharInfo ( '8', 15 );	m_pUIMoney->addCharInfo ( '9', 15 );

	// UI skill icon
	for ( KDint i = 0; i < 3; i++ )
	{
		cSprAni*	pAniSkillIcon = nullptr;

		switch ( i )
		{
			case 0	:	pAniSkillIcon = new cSprAni ( this, "Skills/skill_icons.png", LAYER_MYROOM_UI_1, Point ( 8, 175 ) );	break;
			case 1	:	pAniSkillIcon = new cSprAni ( this, "Skills/skill_icons.png", LAYER_MYROOM_UI_1, Point ( 8, 154 ) );	break;
			case 2	:	pAniSkillIcon = new cSprAni ( this, "Skills/skill_icons.png", LAYER_MYROOM_UI_1, Point ( 8, 133 ) );	break;
		}

		for ( KDint a = 0; a < 11; a++ )
		{
			pAniSkillIcon->addFrame ( Rect ( a * 20, 0, 20, 20 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
		}

		pAniSkillIcon->setVisible ( false );
		pAniSkillIcon->setLoop ( false );
		pAniSkillIcon->stop ( );

		m_aUISkillIcons.push_back ( pAniSkillIcon );

		switch ( i )
		{
			case 0	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 81, 161, 100, 30 ), "", LAYER_MYROOM_UI_1 ) );	break;
			case 1	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 81, 140, 100, 30 ), "", LAYER_MYROOM_UI_1 ) );	break;
			case 2	:	m_aUISkillStrs.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 81, 119, 100, 30 ), "", LAYER_MYROOM_UI_1 ) );	break;
		}
	}

	// UI achievement
	m_pUIClass		= new cSprAni ( this, "MyRoom/myroom_class.png"	    , LAYER_MYROOM_UI_1, Point ( 51, 92 ) );
	m_pUIFriendship	= new cSprAni ( this, "MyRoom/myroom_friendship.png", LAYER_MYROOM_UI_1, Point ( 74, 70 ) );
	m_pUIKillCount	= m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 89, 36, 100, 30 ), "", LAYER_MYROOM_UI_1 );

	m_pUIClass->addFrame ( Rect (   0, 0, 28, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect (  28, 0, 19, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect (  47, 0, 11, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect (  58, 0, 12, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect (  70, 0, 11, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect (  81, 0, 12, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect (  93, 0, 11, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIClass->addFrame ( Rect ( 104, 0, 10, 12 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );

	m_pUIFriendship->addFrame ( Rect (   0, 0, 51, 10 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIFriendship->addFrame ( Rect (  51, 0, 29, 10 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIFriendship->addFrame ( Rect (  80, 0, 41, 10 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIFriendship->addFrame ( Rect ( 121, 0, 36, 10 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );
	m_pUIFriendship->addFrame ( Rect ( 157, 0, 36, 10 ), 0, false, false, Point::ZERO, Rect::ZERO, Rect::ZERO );

	m_pUIClass	   ->setVisible ( false );
	m_pUIFriendship->setVisible ( false );
	m_pUIClass	   ->setLoop ( false );
	m_pUIFriendship->setLoop ( false );
	m_pUIClass	   ->stop ( );
	m_pUIFriendship->stop ( );

	m_pLabels->setVisible ( m_pUIKillCount, false );

	//UI unique type
	m_aUIUniqueType.push_back ( new cSprite ( this, "MyRoom/myroom_uniqueType_text_commom.png"	, LAYER_MYROOM_UI_1, Point::ZERO, Point ( 46, 272 ) ) );
	m_aUIUniqueType.push_back ( new cSprite ( this, "MyRoom/myroom_uniqueType_text_uncommom.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 46, 272 ) ) );
	m_aUIUniqueType.push_back ( new cSprite ( this, "MyRoom/myroom_uniqueType_text_rare.png"	, LAYER_MYROOM_UI_1, Point::ZERO, Point ( 46, 272 ) ) );
	m_aUIUniqueType.push_back ( new cSprite ( this, "MyRoom/myroom_uniqueType_text_unique.png"	, LAYER_MYROOM_UI_1, Point::ZERO, Point ( 46, 272 ) ) );

	for ( KDuint i = 0; i < m_aUIUniqueType.size ( ); i++ )
	{
		m_aUIUniqueType [ i ]->get ( )->setVisible ( false );
	}

	// UI unit detail gauge
	m_aUIUnitParaGauge.push_back ( new cSprite ( this, "MyRoom/myroom_status_gauge.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 260 ) ) );
	m_aUIUnitParaGauge.push_back ( new cSprite ( this, "MyRoom/myroom_status_gauge.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 246 ) ) );
	m_aUIUnitParaGauge.push_back ( new cSprite ( this, "MyRoom/myroom_status_gauge.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 232 ) ) );
	m_aUIUnitParaGauge.push_back ( new cSprite ( this, "MyRoom/myroom_status_gauge.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 218 ) ) );
	m_aUIUnitParaGauge.push_back ( new cSprite ( this, "MyRoom/myroom_status_gauge.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 204 ) ) );

	for ( KDuint i = 0; i < m_aUIUnitParaGauge.size ( ); i++ )
	{
		m_aUIUnitParaGauge [ i ]->get ( )->setTextureRect ( Rect ( 0, 0, 0, 11 ) );
	}

	m_pUIUnitExpGaugeLarge = new cSprite ( this, "MyRoom/myroom_status_exp_gauge_1.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 190 ) );
	m_pUIUnitExpGaugeSmall = new cSprite ( this, "MyRoom/myroom_status_exp_gauge_2.png", LAYER_MYROOM_UI_1, Point::ZERO, Point ( 37, 198 ) );

	m_pUIUnitExpGaugeLarge->get ( )->setTextureRect ( Rect ( 0, 0, 0, 6 ) );
	m_pUIUnitExpGaugeSmall->get ( )->setTextureRect ( Rect ( 0, 0, 0, 3 ) );

	//UI unit Detail parameter
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 115, 265, 80, 12 ), "", LAYER_MYROOM_UI_1 ) );	// hp
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 115, 251, 80, 12 ), "", LAYER_MYROOM_UI_1 ) );	// atk
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 115, 237, 80, 12 ), "", LAYER_MYROOM_UI_1 ) );	// def
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 115, 223, 80, 12 ), "", LAYER_MYROOM_UI_1 ) );	// agl
	m_aUIMobDetailPara.push_back ( m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::LEFT, Rect ( 115, 209, 80, 12 ), "", LAYER_MYROOM_UI_1 ) );	// crt

	// delete unit message box
	m_pUIUnitDeleteMsgBox = new cSprite ( this, "MyRoom/myroom_delete_unit_messagebox.png", LAYER_MYROOM_UI_3, Point::ZERO, Point ( 37, 177 ) );
	m_pUIUnitDeleteMsg	  = m_pLabels->add ( "gaeul9.ttf", 24, TextHAlignment::CENTER, Rect ( 175, 190, 400, 40 ), "", LAYER_MYROOM_UI_3 );

	m_pUIUnitDeleteMsgBox->get ( )->setVisible ( false );
	m_pLabels->setColor ( m_pUIUnitDeleteMsg, 80, 80, 80 );
	m_pLabels->setVisible ( m_pUIUnitDeleteMsg, false );

	// machine
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin1	  , eCreateInfoFileType_Coin_1	 , "MyRoom/myroom_machine_top_coin_1.png"   , "MyRoom/myroom_machine_monList_coin_1.png"   , "MyRoom/myroom_machine_not_coin_1.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,   100, eDollarMachineType_Not				, _MACHINE_1_BUY_DELAY, &cMyData::getObj ( )->m_dMachine1BuyLeftTime, false ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin2	  , eCreateInfoFileType_Coin_2	 , "MyRoom/myroom_machine_top_coin_2.png"   , "MyRoom/myroom_machine_monList_coin_2.png"   , "MyRoom/myroom_machine_not_coin_2.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,   300, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin3	  , eCreateInfoFileType_Coin_3	 , "MyRoom/myroom_machine_top_coin_3.png"   , "MyRoom/myroom_machine_monList_coin_3.png"   , "MyRoom/myroom_machine_not_coin_3.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,   500, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin4	  , eCreateInfoFileType_Coin_4	 , "MyRoom/myroom_machine_top_coin_4.png"   , "MyRoom/myroom_machine_monList_coin_4.png"   , "MyRoom/myroom_machine_not_coin_4.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,   900, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin5	  , eCreateInfoFileType_Coin_5	 , "MyRoom/myroom_machine_top_coin_5.png"   , "MyRoom/myroom_machine_monList_coin_5.png"   , "MyRoom/myroom_machine_not_coin_5.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,   700, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin6	  , eCreateInfoFileType_Coin_6	 , "MyRoom/myroom_machine_top_coin_6.png"   , "MyRoom/myroom_machine_monList_coin_6.png"   , "MyRoom/myroom_machine_not_coin_6.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,  1200, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin7	  , eCreateInfoFileType_Coin_7	 , "MyRoom/myroom_machine_top_coin_7.png"   , "MyRoom/myroom_machine_monList_coin_7.png"   , "MyRoom/myroom_machine_not_coin_7.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,  1000, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin8	  , eCreateInfoFileType_Coin_8	 , "MyRoom/myroom_machine_top_coin_8.png"   , "MyRoom/myroom_machine_monList_coin_8.png"   , "MyRoom/myroom_machine_not_coin_8.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,  1500, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin9	  , eCreateInfoFileType_Coin_9	 , "MyRoom/myroom_machine_top_coin_9.png"   , "MyRoom/myroom_machine_monList_coin_9.png"   , "MyRoom/myroom_machine_not_coin_9.png"  , LAYER_MYROOM_MACHINE, Point::ZERO,  1800, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin10  , eCreateInfoFileType_Coin_10	 , "MyRoom/myroom_machine_top_coin_10.png"  , "MyRoom/myroom_machine_monList_coin_10.png"  , "MyRoom/myroom_machine_not_coin_10.png" , LAYER_MYROOM_MACHINE, Point::ZERO,  6000, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin11  , eCreateInfoFileType_Coin_11	 , "MyRoom/myroom_machine_top_coin_11.png"  , "MyRoom/myroom_machine_monList_coin_11.png"  , "MyRoom/myroom_machine_not_coin_11.png" , LAYER_MYROOM_MACHINE, Point::ZERO, 46000, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin12  , eCreateInfoFileType_Coin_12	 , "MyRoom/myroom_machine_top_coin_12.png"  , "MyRoom/myroom_machine_monList_coin_12.png"  , "MyRoom/myroom_machine_not_coin_12.png" , LAYER_MYROOM_MACHINE, Point::ZERO,  8500, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreateCoin13  , eCreateInfoFileType_Coin_13	 , "MyRoom/myroom_machine_top_coin_13.png"  , "MyRoom/myroom_machine_monList_coin_13.png"  , "MyRoom/myroom_machine_not_coin_13.png" , LAYER_MYROOM_MACHINE, Point::ZERO, 65000, eDollarMachineType_Not				, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreatePremium1, eCreateInfoFileType_Premium_1, "MyRoom/myroom_machine_top_premium_1.png", "MyRoom/myroom_machine_monList_premium_1.png", "MyRoom/myroom_machine_not_dollor_1.png", LAYER_MYROOM_MACHINE, Point::ZERO,	  0, eDollarMachineType_Unusual			, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreatePremium2, eCreateInfoFileType_Premium_2, "MyRoom/myroom_machine_top_premium_2.png", "MyRoom/myroom_machine_monList_premium_2.png", "MyRoom/myroom_machine_not_dollor_2.png", LAYER_MYROOM_MACHINE, Point::ZERO,	  0, eDollarMachineType_Prettiness		, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreatePremium3, eCreateInfoFileType_Premium_3, "MyRoom/myroom_machine_top_premium_3.png", "MyRoom/myroom_machine_monList_premium_3.png", "MyRoom/myroom_machine_not_dollor_3.png", LAYER_MYROOM_MACHINE, Point::ZERO,	  0, eDollarMachineType_RegendOfHeroRink, 0, nullptr, true  ) );
	m_aMachines.push_back ( new cMachine ( this, m_pUnitCreatePremium4, eCreateInfoFileType_Premium_4, "MyRoom/myroom_machine_top_premium_4.png", "MyRoom/myroom_machine_monList_premium_4.png", "MyRoom/myroom_machine_not_dollor_4.png", LAYER_MYROOM_MACHINE, Point::ZERO,	  0, eDollarMachineType_Warvel_vs_Hell  , 0, nullptr, true  ) );

	m_pMachineDragCursor	  = new cSprite ( this, "MyRoom/myroom_drag_cursor.png"	, LAYER_MYROOM_UI_1		, Point::ZERO, Point( 449, 246 ) );
	m_pMachineTouchMe		  = new cSprite ( this, "MyRoom/myroom_touchMe.png"		, LAYER_MYROOM_UI_2		, Point::ZERO, Point( 262, 101 ) );
	m_pSprCapsule1			  = new cSprite ( this, "MyRoom/myroom_capsule1.png"	, LAYER_MYROOM_MACHINE-1, Point ( 0.5f, 0.0f ), Point::ZERO );
	m_pSprCapsule2			  = new cSprite ( this, "MyRoom/myroom_capsule2.png"	, LAYER_MYROOM_MACHINE-1, Point ( 0.5f, 1.0f ), Point::ZERO );
	m_pSprCapsule3			  = new cSprite ( this, "MyRoom/myroom_capsule1.png"	, LAYER_MYROOM_UI_3		, Point ( 0.5f, 0.0f ), Point::ZERO );
	m_pSprCapsule4			  = new cSprite ( this, "MyRoom/myroom_capsule2.png"	, LAYER_MYROOM_UI_3		, Point ( 0.5f, 1.0f ), Point::ZERO );
	m_pSprCapsuleTouchMeFont  = new cSprite ( this, "MyRoom/capsule_touch_font.png"	, LAYER_MYROOM_UI_3		, Point::ZERO, Point(135, 49)  );
	
	m_pUIMachineCoin		  = new cSprite  ( this, "MyRoom/myroom_machine_coin.png"	, LAYER_MYROOM_UI_3, Point::ZERO, Point::ZERO );
	m_pUIMachineTimeCursor	  = new cSprAnis ( this, "Bet/bet_shop_cursor.spr"			, LAYER_MYROOM_UI_2, Point ( 340, 297 ) );
	m_pUIMachineNotCoinCursor = new cSprAnis ( this, "Bet/bet_shop_cursor.spr"			, LAYER_MYROOM_UI_2, Point ( 325, 215 ) );
	m_pUINotSlotPopup		  = new cSprite  ( this, "MyRoom/myroom_not_enough_slot.png", LAYER_MYROOM_UI_2, Point::ZERO, Point ( (KDint) ( _SCR_W / 2 - 127 ), (KDint) ( _SCR_H / 2 -  26 ) ) );
	m_pUIGettingBack		  = new cSprite  ( this, "MyRoom/myroom_capsule_back.png"	, LAYER_MYROOM_UI_3, Point::ZERO, Point ( 115, 5 ) );
	m_pUIGettingProp		  = new cSprProp ( this, LAYER_MYROOM_UI_PROP, Point ( 190, 104 ) );

	m_pMachineDragCursor	 ->get ( )->setVisible ( false );
	m_pMachineTouchMe		 ->get ( )->setVisible ( false );
	m_pSprCapsule1			 ->get ( )->setVisible ( false );
	m_pSprCapsule2			 ->get ( )->setVisible ( false );
	m_pSprCapsule3			 ->get ( )->setVisible ( false );
	m_pSprCapsule4			 ->get ( )->setVisible ( false );
	m_pSprCapsuleTouchMeFont ->get ( )->setVisible ( false );
	m_pUIMachineCoin		 ->get ( )->setVisible ( false );
	m_pUINotSlotPopup		 ->get ( )->setVisible ( false );
	m_pUIGettingBack		 ->get ( )->setVisible ( false );
	m_pUIGettingProp		 ->setVisible ( false );

	m_pUIMachineTimeCursor	 ->texLoad ( 0 );
	m_pUIMachineNotCoinCursor->texLoad ( 0 );
	m_pUIMachineTimeCursor	 ->stop ( );
	m_pUIMachineNotCoinCursor->stop ( );
	m_pUIMachineTimeCursor	 ->setVisible ( false );
	m_pUIMachineNotCoinCursor->setVisible ( false );

	m_tMachine				= Point ( 20, 40 );
	m_nViewMachineGap		= 132;
	m_fScrollMachine		= 0;
	m_bIsTouchMachine		= false;
	m_bIsTouchMachineDrag	= false;
	m_bIsTouchMachineTouch	= false;

	// particle
	m_pParticleUnitDelect = new cParticle ( this, "unit_delete.plist", LAYER_MYROOM_UNIT_DEL_PARTICLE, Point::ZERO, true , 3 );
	m_pParticleGetUnit	  = new cParticle ( this, "get_unit.plist"	 , LAYER_MYROOM_GET_PARTICLE	 , Point ( 240, 185 ), false, 0 );
	m_pParticleUnitDelect->stop ( );
	m_pParticleGetUnit	 ->stop ( );

	m_bIsDeleteParticleStop = false;
	m_dDeleteParticleStopLeftTime = 0;

	// time get money
	cTimeGetMoney::getObj ( )->changeNode ( this, LAYER_MYROOM_UI_1, Rect ( 150, 300, 50, 15 ) );

	// update money
	this->updateMoney ( );

	// not coin popup
	m_pUINotCoinPopup = new cSprite ( this, "Bet/bet_notcoin_popup.png", LAYER_MYROOM_MODE_BTN, Point ( 0, 0 ), Point ( 65, 134 ) );
	m_pUINotCoinPopup->get ( )->setVisible ( false );

	m_bUIIsNotCoin = false;
	m_dUINotCoinLeftTime = 0;

	//shop cursor
	m_pUIShopCursor = new cSprAnis ( this, "Bet/bet_shop_cursor.spr", LAYER_MYROOM_UI_2 , Point ( 100, 50 ) );

	m_pUIShopCursor->texLoad ( 0 );
	m_pUIShopCursor->setVisible ( false );
	m_pUIShopCursor->stop ( );

	m_bUIIsNotCoinView = false;

	//battle mode
	m_pUIModeBack = new cSprite ( this, "MyRoom/myroom_mode_back.png", LAYER_MYROOM_MODE_BACK, Point::ZERO, Point ( 25, 31 ) );
	m_pUIModeBack->get ( )->setVisible ( false );

	// gray btn
	m_pUIBtnSocialGray = new cSprite ( this, "MyRoom/myroom_btn_social_gray.png", LAYER_MYROOM_BTN, Point::ZERO, Point ( 228, -5 ) ); 
	m_pUIBtnBattleGray = new cSprite ( this, "MyRoom/myroom_btn_battle_gray.png", LAYER_MYROOM_BTN, Point::ZERO, Point ( 390, -5 ) );

	// arcade clear
	m_pUIArcadeEasyClear = new cSprite ( this, "MyRoom/myroom_arcade_clear.png", LAYER_MYROOM_MODE_BTN, Point::ZERO, Point (  81, 233 ) ); 
	m_pUIArcadeNorClear	 = new cSprite ( this, "MyRoom/myroom_arcade_clear.png", LAYER_MYROOM_MODE_BTN, Point::ZERO, Point ( 191, 233 ) ); 
	m_pUIArcadeHardClear = new cSprite ( this, "MyRoom/myroom_arcade_clear.png", LAYER_MYROOM_MODE_BTN, Point::ZERO, Point ( 301, 233 ) ); 

	m_pUIArcadeEasyClear->get ( )->setVisible ( false );
	m_pUIArcadeNorClear	->get ( )->setVisible ( false );
	m_pUIArcadeHardClear->get ( )->setVisible ( false );

	// touch unit
	m_bIsTouchUnit = false;

	// switch
	m_bIsUnitDeleting = false;
	m_bGetCapsuleMode = false;
	m_bIsModePopup	  = false;
	m_bIsBuyingDollarCapsule = false;

	// shop node
	m_pShopNode = ShopNode::createWithMyRoomNode ( this );
	this->addChild ( m_pShopNode , LAYER_MYROOM_SHOP );
	m_pShopNode->setPosition ( Point ( 0, _SCR_H ) );
	m_pShopNode->setVisible	( false );

	// node change
	m_bIsNodeChangeIn	= true;
	m_bIsNodeChangeOut	= false;
	m_bIsGoTitle		= false;
	m_bIsGoCasino100	= false; 
	m_bIsGoCasino1000	= false;
	m_bIsGoArcadeEasy	= false;
	m_bIsGoArcadeNormal	= false;
	m_bIsGoArcadeHard	= false;
	m_bIsGoSurvival		= false;
	m_pNodeChangeImg	= new cNodeChangeImgs ( this, false );

	// schedule
	this->schedule ( schedule_selector ( MyRoomNode::step ) );

	if ( nSelectUnitID > 0 )
	{
		this->selectUnitAndPage ( nSelectUnitID );
	}

	return true;
}

KDvoid MyRoomNode::releaseObject ( KDvoid )
{
	// update save data
	cMyData::getObj ( )->updateMyRoomUnitSaveDatas ( m_aUnits );

	// select unit
	m_pUnitSelect = nullptr;

	// node chage
	CC_SAFE_RELEASE ( m_pNodeChangeImg );

	// shop node
	SAFE_REMOVE_CHILD ( m_pShopNode );

	// unit crate info
	CC_SAFE_RELEASE ( m_pUnitCreateCoin1 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin2 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin3 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin4 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin5 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin6 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin7 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin8 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin9 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin10 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin11 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin12 );
	CC_SAFE_RELEASE ( m_pUnitCreateCoin13 );
	CC_SAFE_RELEASE ( m_pUnitCreatePremium1 );
	CC_SAFE_RELEASE ( m_pUnitCreatePremium2 );
	CC_SAFE_RELEASE ( m_pUnitCreatePremium3 );
	CC_SAFE_RELEASE ( m_pUnitCreatePremium4 );

	// unit
	SAFE_RELEASE_ARRAYS ( m_aUnits );


	// drag unit
	m_pUnitDrag = nullptr;

	// back img
	CC_SAFE_RELEASE ( m_pSprBack		);
	CC_SAFE_RELEASE ( m_pSprBackLeft	);
	CC_SAFE_RELEASE ( m_pSprBackTop		);
	CC_SAFE_RELEASE ( m_pSprBackTopAdd	);
	CC_SAFE_RELEASE ( m_pSprBackBottom	);
	CC_SAFE_RELEASE ( m_pSprBack2		);

	// cover color
	SAFE_REMOVE_CHILD ( m_pCoverColor );
	SAFE_REMOVE_CHILD ( m_pCoverColor2 );


	// btns
	SAFE_REMOVE_CHILD ( m_pBtnExit				);
	SAFE_REMOVE_CHILD ( m_pBtnShop				);
	SAFE_REMOVE_CHILD ( m_pBtnShopExit			);
	SAFE_REMOVE_CHILD ( m_pBtnUnitDeleteYes		);
	SAFE_REMOVE_CHILD ( m_pBtnUnitDeleteNo		);
	SAFE_REMOVE_CHILD ( m_pBtnMachineLeft		);
	SAFE_REMOVE_CHILD ( m_pBtnMachineRight		);
	SAFE_REMOVE_CHILD ( m_pBtnMachineHandle		);
	SAFE_REMOVE_CHILD ( m_pBtnGain				);
	SAFE_REMOVE_CHILD ( m_pBtnModeExit			);
	SAFE_REMOVE_CHILD ( m_pBtnModeCasino100		);
	SAFE_REMOVE_CHILD ( m_pBtnModeCasino1000	);
	SAFE_REMOVE_CHILD ( m_pBtnModeArcadeEasy	);
	SAFE_REMOVE_CHILD ( m_pBtnModeArcadeNormal	);
	SAFE_REMOVE_CHILD ( m_pBtnModeArcadeHard	);
	SAFE_REMOVE_CHILD ( m_pBtnModeSurvival		);

	CC_SAFE_RELEASE ( m_pBtnBack		  );
	CC_SAFE_RELEASE ( m_pBtnShop2         );
	CC_SAFE_RELEASE ( m_pBtnPageLeft	  );
	CC_SAFE_RELEASE ( m_pBtnPageRight	  );
	CC_SAFE_RELEASE ( m_pBtnPageDragLeft  );
	CC_SAFE_RELEASE ( m_pBtnPageDragRight );
	CC_SAFE_RELEASE ( m_pBtnSocial		  );
	CC_SAFE_RELEASE ( m_pBtnCapsule		  );
	CC_SAFE_RELEASE ( m_pBtnWastebasket	  );
	CC_SAFE_RELEASE ( m_pBtnBattle		  );

	SAFE_RELEASE_ARRAYS ( m_aBtnPageNums );

	// particles
	CC_SAFE_RELEASE ( m_pParticleUnitDelect );
	CC_SAFE_RELEASE ( m_pParticleGetUnit	);

	// machine
	SAFE_RELEASE_ARRAYS ( m_aMachines );

	CC_SAFE_RELEASE ( m_pMachineDragCursor		);
	CC_SAFE_RELEASE ( m_pMachineTouchMe			);
	CC_SAFE_RELEASE ( m_pSprCapsule1			);
	CC_SAFE_RELEASE ( m_pSprCapsule2			);
	CC_SAFE_RELEASE ( m_pSprCapsule3			);
	CC_SAFE_RELEASE ( m_pSprCapsule4			);
	CC_SAFE_RELEASE ( m_pSprCapsuleTouchMeFont	);
	CC_SAFE_RELEASE ( m_pUIMachineCoin			);
	CC_SAFE_RELEASE ( m_pUIMachineTimeCursor	);
	CC_SAFE_RELEASE ( m_pUIMachineNotCoinCursor ); 
	CC_SAFE_RELEASE ( m_pUINotSlotPopup			);
	CC_SAFE_RELEASE ( m_pUIGettingBack			);
	CC_SAFE_RELEASE ( m_pUIGettingProp			);
	CC_SAFE_RELEASE ( m_pParticleGetUnit		);

	// labels
	CC_SAFE_RELEASE ( m_pLabels );

	// UI money
	CC_SAFE_RELEASE ( m_pUIMoney );

	// UI prop
	SAFE_RELEASE_ARRAYS ( m_aUIProps );

	// UI unique type
	SAFE_RELEASE_ARRAYS ( m_aUIUniqueType );

	// UI mob Detail parameter
	m_aUIMobDetailPara.clear ( );

	// UI unit detail gauge
	SAFE_RELEASE_ARRAYS ( m_aUIUnitParaGauge );

	CC_SAFE_RELEASE ( m_pUIUnitExpGaugeLarge );
	CC_SAFE_RELEASE ( m_pUIUnitExpGaugeSmall );

	// UI skill
	SAFE_RELEASE_ARRAYS ( m_aUISkillIcons );
	m_aUISkillIcons.clear();
	m_aUISkillStrs .clear();

	// UI achievement
	CC_SAFE_RELEASE ( m_pUIClass	  );
	CC_SAFE_RELEASE ( m_pUIFriendship );
	m_pUIKillCount = nullptr;

	// not coin popup
	CC_SAFE_RELEASE ( m_pUINotCoinPopup );

	// UI not coin
	CC_SAFE_RELEASE ( m_pUIShopCursor );

	// battle mode
	CC_SAFE_RELEASE ( m_pUIModeBack );

	// gray btn
	CC_SAFE_RELEASE ( m_pUIBtnSocialGray );
	CC_SAFE_RELEASE ( m_pUIBtnBattleGray );

	// arcade clear
	CC_SAFE_RELEASE ( m_pUIArcadeEasyClear );
	CC_SAFE_RELEASE ( m_pUIArcadeNorClear  );
	CC_SAFE_RELEASE ( m_pUIArcadeHardClear );

	// delete unit message box
	CC_SAFE_RELEASE ( m_pUIUnitDeleteMsgBox );
	m_pUIUnitDeleteMsg = nullptr;

	// level up
	SAFE_RELEASE_ARRAYS ( m_aLvUpEffects );
	SAFE_RELEASE_ARRAYS ( m_aLvUpPopups  );
}

KDbool MyRoomNode::onTouchBegan ( Touch* pTouch, Event* pEvent ) 
{
	// check shop
	if ( m_pShopNode->getIsShop ( ) == true )
	{
		return true;
	}

	// btn
	m_pBtnBack		 ->touchesBegan ( pTouch );
	m_pBtnShop2		 ->touchesBegan ( pTouch );
	m_pBtnPageLeft	 ->touchesBegan ( pTouch );
	m_pBtnPageRight  ->touchesBegan ( pTouch );
	m_pBtnSocial	 ->touchesBegan ( pTouch );
	m_pBtnCapsule	 ->touchesBegan ( pTouch );
	m_pBtnWastebasket->touchesBegan ( pTouch );
	m_pBtnBattle	 ->touchesBegan ( pTouch );

	//
	if ( m_bGetCapsuleMode == true && m_nCapsuleModeStep == 6 && m_nCapsuleModeStep2 == 5 )
	{
		if ( RectInPoint ( Rect ( 172, 87, 136, 174 ), pTouch->getLocation ( ) ) == true )
		{
			m_bIsCapsuleTouch = true;

			if ( cMyData::getObj ( )->m_bIsOptionSound != false )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "capsuleBomb.mp3" );
			}
		}
	}

	// page num
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->touchesBegan ( pTouch );
	}

	// unit
	if ( m_bGetCapsuleMode == false && m_bIsModePopup == false )
	{
		KDint	nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;
		for ( KDint i = nViewFirstSlotIndex; i < nViewFirstSlotIndex + _SLOT_IN_PAGE_COUNT; i++ )
		{
			// init
			cUnit*	pUnit = this->getUnit ( i );

			// null check
			if ( pUnit == nullptr )
			{
				continue;
			}

			// rect check
			if ( m_bIsUnitDeleting == false && m_pUnitDrag == nullptr && pUnit->isRectInPoint ( pTouch->getLocation ( ) ) == true )
			{
				m_bIsTouchUnit = true;
				m_tUnitTouchStartInfo = *pTouch;

				KDint		nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;
				m_tDragStart = m_aUnit [ pUnit->getSlotIndex ( ) - nViewFirstSlotIndex ];

				this->selectUnit ( pUnit );
			}
		}
	}

	// machine mode
	touchBeganMachineMode ( pTouch );

	return true;
}

KDvoid MyRoomNode::onTouchMoved ( Touch* pTouch, Event* pEvent )
{
	// check shop
	if ( m_pShopNode->getIsShop ( ) == true )
	{
		return;
	}

	// btn
	m_pBtnBack			->touchesMoved ( pTouch );
	m_pBtnShop2			->touchesMoved ( pTouch );
	m_pBtnPageLeft		->touchesMoved ( pTouch );
	m_pBtnPageRight		->touchesMoved ( pTouch );
	m_pBtnPageDragLeft	->touchesMoved ( pTouch );
	m_pBtnPageDragRight	->touchesMoved ( pTouch );
	m_pBtnSocial		->touchesMoved ( pTouch );
	m_pBtnCapsule		->touchesMoved ( pTouch );
	m_pBtnWastebasket	->touchesBegan ( pTouch );
	m_pBtnWastebasket	->touchesMoved ( pTouch );
	m_pBtnBattle		->touchesMoved ( pTouch );

	// page num
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->touchesMoved ( pTouch );
	}

	// unit
	if ( m_pUnitSelect != nullptr && m_bIsTouchUnit == true && pTouch->getID ( ) == m_tUnitTouchStartInfo.getID ( ) )
	{
		if ( pTouch->getLocation ( ).getDistance ( m_tUnitTouchStartInfo.getLocation ( ) ) > 15 )
		{
			m_pUnitDrag	  = m_pUnitSelect;
			m_pUnitSelect = nullptr;

			m_pUnitDrag->changeNode ( this, LAYER_MYROOM_MOB );

			m_pUnitDrag->changeAni(eUnitAniType_Damage);
			m_pUnitDrag->setIsAutoChangeNor ( false );
			m_pUnitDrag->aniStop();
			m_pUnitDrag->setVisibleNameA ( false );

			m_tUnitTouchDragInfo = *pTouch;
			m_dSelectUnitAniLeftTime = 0.2f;

			this->setPageRange ( );
		}
	}

	// drag unit
	if ( m_pUnitDrag != nullptr && m_bIsTouchUnit == true && pTouch->getID ( ) == m_tUnitTouchDragInfo.getID ( ) )
	{
		m_tUnitTouchDragInfo = *pTouch;

		// change unit
		m_nChangeSlotIndex = -1;

		KDint		nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;
		for ( KDint i = 0; i < _SLOT_IN_PAGE_COUNT; i++ )
		{
			// check
			if ( nViewFirstSlotIndex + i == m_pUnitDrag->getSlotIndex ( ) )
			{
				continue;
			}

			if ( RectInPoint ( m_aRectChange [ i ], m_tUnitTouchDragInfo.getLocation ( ) ) == true )
			{
				m_nChangeSlotIndex = nViewFirstSlotIndex + i;
				break;
			}
		}
	}

	if ( m_pUnitSelect == nullptr && m_pUnitDrag == nullptr )
	{
		m_bIsTouchUnit = false;
	}

	// machine mode
	this->touchMovedMachineMode ( pTouch );		
}

KDvoid MyRoomNode::onTouchEnded ( Touch* pTouch, Event* pEvent )
{
	// check shop
	if ( m_pShopNode->getIsShop ( ) == true )
	{
		return;
	}

	// btn
	if ( m_pBtnBack			->touchesEnded ( pTouch ) ) { this->backCallback		( ); }
	if ( m_pBtnShop2		->touchesEnded ( pTouch ) ) { this->shop2Callback		( ); }
	if ( m_pBtnPageLeft		->touchesEnded ( pTouch ) ) { this->pageLeftCallback	( ); }
	if ( m_pBtnPageRight	->touchesEnded ( pTouch ) ) { this->pageRightCallback	( ); }
	if ( m_pBtnSocial		->touchesEnded ( pTouch ) ) { this->socialCallback		( ); }
	if ( m_pBtnCapsule		->touchesEnded ( pTouch ) ) { this->capsuleCallback		( ); }
	if ( m_pBtnWastebasket  ->touchesEnded ( pTouch ) ) { this->wastebasketCallback ( ); }
	if ( m_pBtnBattle		->touchesEnded ( pTouch ) ) { this->battleCallback		( ); }

	// page num
	for ( auto pNode : m_aBtnPageNums )
	{
		if ( pNode->touchesEnded ( pTouch ) )
		{
			this->setPage ( pNode->getNum ( ) - 1, false );
			//return;
		}
	}

	// unit
	if ( m_pUnitSelect != nullptr && m_bIsTouchUnit == true && pTouch->getID ( ) == m_tUnitTouchStartInfo.getID ( ) )
	{
		m_bIsTouchUnit = false;
	}

	if ( m_pUnitDrag != nullptr && m_bIsTouchUnit == true && pTouch->getID ( ) == m_tUnitTouchDragInfo.getID ( ) )
	{
		if ( m_bIsUnitDeleting == false )
		{
			m_pUnitSelect  = m_pUnitDrag;
			m_pUnitSelect->retain ( );
			m_pUnitDrag	   = nullptr;
			m_bIsTouchUnit = false;

			// slot change
			if ( m_nChangeSlotIndex != -1 )
			{
				cUnit*		pChangeUnit = this->getUnit ( m_nChangeSlotIndex );

				if ( pChangeUnit != nullptr )
				{
					KDint	nTempSlot = m_pUnitSelect->getSlotIndex ( );
					m_pUnitSelect->setSlotIndex ( pChangeUnit->getSlotIndex ( ) );
					pChangeUnit	 ->setSlotIndex ( nTempSlot );

					KDint	nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;
					if ( pChangeUnit->getSlotIndex ( ) - nViewFirstSlotIndex >= 0 && pChangeUnit->getSlotIndex ( ) - nViewFirstSlotIndex < _SLOT_IN_PAGE_COUNT )
					{
						pChangeUnit->setPointTime ( m_aUnit [ pChangeUnit->getSlotIndex ( ) - nViewFirstSlotIndex ], 0.1f );
						m_aUIProps [ pChangeUnit->getSlotIndex ( ) - nViewFirstSlotIndex ]->setSelect ( pChangeUnit->getUniqueType ( ) );
					}
					else
					{
						pChangeUnit->setVisible ( false );
					}

					if ( m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex >= 0 && m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex < _SLOT_IN_PAGE_COUNT )
					{
						m_aUIProps [ m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex ]->setSelect ( m_pUnitSelect->getUniqueType ( ) );
					}
				}
				else
				{
					KDint		nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;

					if ( m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex >= 0 && m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex < _SLOT_IN_PAGE_COUNT )
					{
						m_aUIProps [ m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex ]->setSelect ( eUniqueType_not );
					}

					m_pUnitSelect->setSlotIndex ( m_nChangeSlotIndex );

					if ( m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex >= 0 && m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex < _SLOT_IN_PAGE_COUNT )
					{
						m_aUIProps [ m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex ]->setSelect ( m_pUnitSelect->getUniqueType ( ) );
					}
				}

				m_nChangeSlotIndex = -1;
			}

			m_pUnitSelect->setVisibleNameA ( true );
			m_pUnitSelect->changeAni ( eUnitAniType_Normal );
			m_pUnitSelect->aniStart ( );
			m_pUnitSelect->setIsAutoChangeNor ( true );

			m_dSelectUnitAniLeftTime = 5;

			// «?¿Á º±?�√µ» ¿Ø¥÷¿Ã ¿÷¥¬ ?�‰¿Ã¡ˆ∑Œ ¿?�µ�?¿Ãµø µ»¥Ÿ
			this->setPage ( m_pUnitSelect->getSlotIndex ( ) / _SLOT_IN_PAGE_COUNT, false );
			this->setPageRange ( );
		}
	}

	if ( m_pUnitSelect == nullptr && m_pUnitDrag == nullptr )
	{
		m_bIsTouchUnit = false;
	}

	// machine mode
	this->touchEndedMachineMode ( pTouch );
}

KDvoid MyRoomNode::step ( KDfloat fDelta )
{
	// tickCount
	m_dTickCount += fDelta;

	if ( m_dBeforeTickCount == 0 )	
	{
		m_dBeforeTickCount = m_dTickCount;
	}
	KDdouble	dLeftTickCount = m_dTickCount - m_dBeforeTickCount;
	m_dBeforeTickCount = m_dTickCount;

	// my data update
	cMyData::getObj ( )->update ( dLeftTickCount );

	if ( dLeftTickCount > 0.1f )
	{
		dLeftTickCount = 0.1f;
	}

	// node change
	if ( this->updateNodeChage ( dLeftTickCount ) == true )	
	{
		return;
	}

	// shop
	if ( m_pShopNode->getIsShop ( ) == true )
	{
		Point		tPoint = m_pShopNode->getPosition ( );

		if ( tPoint.y > 0 )
		{
			tPoint.y -= dLeftTickCount * 1500;
			if ( tPoint.y < 0 )
			{
				tPoint.y = 0;
			}

			m_pShopNode	  ->setPosition ( tPoint );
			m_pBtnShopExit->setPosition ( Point ( 62, tPoint.y + 96 ) );
		}
	}
	else
	{
		Point		tPoint = m_pShopNode->getPosition ( );

		if ( tPoint.y < _SCR_H )
		{
			tPoint.y += dLeftTickCount * 1500;
			if ( tPoint.y > _SCR_H )
			{
				tPoint.y = _SCR_H;
			}

			m_pShopNode	  ->setPosition ( tPoint );
			m_pBtnShopExit->setPosition ( Point ( 62, tPoint.y + 96 ) );
		}
	}
	// unit deleteing
	if ( m_bIsUnitDeleting == true && m_bIsUnitDeleteYes == true )
	{
		m_dUnitDeleteEffectLeftTime -= dLeftTickCount;
		if ( m_dUnitDeleteEffectLeftTime < 0 )
		{
			m_bIsUnitDeleting  = false;
			m_bIsUnitDeleteYes = false;

			// set page
			this->setPage ( cMyData::getObj ( )->m_nMyRoomPage, true );
			this->setPageRange ( );
		}
	}

	// drag page left & right
	if ( m_pUnitDrag != nullptr )
	{
		if ( m_pBtnPageDragLeft->update ( dLeftTickCount ) == true )
		{
			this->setPage ( cMyData::getObj ( )->m_nMyRoomPage - 1, false );
		}
		if ( m_pBtnPageDragRight->update ( dLeftTickCount ) == true )
		{
			this->setPage ( cMyData::getObj ( )->m_nMyRoomPage + 1, false );
		}
	}

	// capsule
	this->updateCapsuleMode ( dLeftTickCount );

	// unit exp
	this->updateUnitExp ( );

	// unit
	if ( m_bIsUnitDeleting == false )
	{
		this->updateUnit ( dLeftTickCount );
		this->updateUnitStatus ( );
	}

	//not coin
	if ( m_bUIIsNotCoinView == true )
	{
		m_pUIShopCursor->update ( dLeftTickCount );
	}

	// back img & cover 1
	if ( m_bIsUnitDeleting == false && m_bGetCapsuleMode == false && m_bIsModePopup == false )
	{
		if ( m_pUnitDrag != nullptr )
		{
			m_pSprBack		 ->get ( )->setVisible ( false );
			m_pSprBackLeft	 ->get ( )->setVisible ( false );
			m_pSprBackTop	 ->get ( )->setVisible ( false );
			m_pSprBackBottom ->get ( )->setVisible ( false );
			m_pSprBack2		 ->get ( )->setVisible ( true );

			m_pBtnExit		 ->setEnabled ( false );
			m_pBtnShop		 ->setEnabled ( false );
			m_pBtnSocial	 ->setIsTouchEnable ( false );
			m_pBtnCapsule	 ->setIsTouchEnable ( false );
			m_pBtnBattle	 ->setIsTouchEnable ( false );
			m_pBtnWastebasket->setVisible	    ( true );
			m_pBtnWastebasket->setIsTouchEnable ( true );

			m_pBtnPageLeft ->setColor ( 100, 100, 100 );
			m_pBtnPageRight->setColor ( 100, 100, 100 );

			for ( KDuint i = 0; i < m_aBtnPageNums.size ( ); i++ )
			{
				if ( m_aBtnPageNums [ i ]->getNum ( ) - 1 == cMyData::getObj ( )->m_nMyRoomPage )	m_aBtnPageNums [ i ]->setColor ( 255, 255, 255 );
				else																				m_aBtnPageNums [ i ]->setColor ( 100, 100, 100 );
			}
		}
		else
		{
			if ( m_pNodeChangeImg == nullptr && m_pShopNode->getIsShop ( ) == false )
			{
				m_pSprBack		->get ( )->setVisible ( true );
				m_pSprBackLeft	->get ( )->setVisible ( true );
				m_pSprBackTop	->get ( )->setVisible ( true );
				m_pSprBackBottom->get ( )->setVisible ( true );
				m_pSprBack2		->get ( )->setVisible ( false );

				m_pBtnExit		->setEnabled ( true );
				m_pBtnShop		->setEnabled ( true );
				m_pBtnCapsule	->setIsTouchEnable ( true );
				m_pBtnWastebasket->setVisible	   ( false );
				m_pBtnWastebasket->setIsTouchEnable ( false );

				if ( m_pUnitSelect == nullptr )
				{
					m_pUIBtnSocialGray->get ( )->setVisible ( true );
					m_pUIBtnBattleGray->get ( )->setVisible ( true );
					m_pBtnSocial->setVisible ( false );
					m_pBtnBattle->setVisible ( false );
					m_pBtnSocial->setIsTouchEnable ( false );
					m_pBtnBattle->setIsTouchEnable ( false );
				}
				else
				{
					m_pUIBtnSocialGray->get ( )->setVisible ( false );
					m_pUIBtnBattleGray->get ( )->setVisible ( false );
					m_pBtnSocial->setVisible ( true );
					m_pBtnBattle->setVisible ( true );
					m_pBtnSocial->setIsTouchEnable ( true );
					m_pBtnBattle->setIsTouchEnable ( true );
				}

				m_pBtnPageLeft ->setColor ( 255, 255, 255 );
				m_pBtnPageRight->setColor ( 255, 255, 255 );

				for ( auto pNode : m_aBtnPageNums )	
				{
					pNode->setColor ( 255, 255, 255 );
				}
			}
		}
	}		

	// not slot popup
	if ( m_pUINotSlotPopup->get ( )->isVisible ( ) == true )
	{
		m_dNotSlotLeftTime -= dLeftTickCount;
		if ( m_dNotSlotLeftTime < 0 )
		{
			m_pUINotSlotPopup->get ( )->setVisible ( false );
		}
	}

	// time get money
	cTimeGetMoney::getObj ( )->update ( dLeftTickCount, cMyData::getObj ( )->m_nMoney );

	// not coin popup
	if ( m_bUIIsNotCoin == true )
	{
		m_dUINotCoinLeftTime -= dLeftTickCount;

		if ( m_dUINotCoinLeftTime < 0 )
		{
			m_bUIIsNotCoin = false;
			m_pUINotCoinPopup->get ( )->setVisible ( false );
		}
	}

	// buy dollar capsule
	this->updateBuyingCheck ( );

	// update money
	this->updateMoney ( );

	// update level up effect
	this->updateLevelUpEffect ( dLeftTickCount );

	// delete particle stop
	if ( m_bIsDeleteParticleStop == true )
	{
		m_dDeleteParticleStopLeftTime -= dLeftTickCount;

		if ( m_dDeleteParticleStopLeftTime < 0 )
		{
			m_bIsDeleteParticleStop = false;
			m_dDeleteParticleStopLeftTime = 0;

			m_pParticleUnitDelect->stop ( );
		}
	}
}

KDbool MyRoomNode::updateNodeChage ( KDdouble dLeftTIme )
{
	if ( m_pNodeChangeImg != nullptr )
	{
		if ( m_pNodeChangeImg->update ( dLeftTIme ) == true )
		{
			if ( m_bIsNodeChangeIn == true )
			{
				// background music
				if ( cMyData::getObj ( )->m_bIsOptionSound != 0 )
				{
					SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "myroom.mp3", true );
				}

				m_bIsNodeChangeIn = false;
				CC_SAFE_RELEASE ( m_pNodeChangeImg );

				m_pBtnExit			->setEnabled		( true );
				m_pBtnShop			->setEnabled		( true );
				m_pBtnSocial		->setIsTouchEnable	( true );
				m_pBtnCapsule		->setIsTouchEnable	( true );
				m_pBtnWastebasket	->setVisible		( false );
				m_pBtnWastebasket	->setIsTouchEnable	( false );
				m_pBtnBattle		->setIsTouchEnable	( true );
				if ( m_pBtnPageLeft ->getVisible ( ) == true )	m_pBtnPageLeft ->setIsTouchEnable ( true );
				if ( m_pBtnPageRight->getVisible ( ) == true )	m_pBtnPageRight->setIsTouchEnable ( true );
			}
			else if ( m_bIsNodeChangeOut == true )
			{
				if ( m_bIsGoTitle == true )
				{
					// unschedule
					this->unschedule ( schedule_selector ( MyRoomNode::step ) );
			
					// object release
					this->releaseObject ( );

					// cenece change
					Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, MainNode::create ( true ) ) );					
				}
				else if ( m_bIsGoCasino100 == true || m_bIsGoCasino1000 == true )
				{
					// bet money
					KDint64		nBetMoney = 0;
					if ( m_bIsGoCasino100 == true )	nBetMoney = 100LL;
					else							nBetMoney = 1000LL;

					// global money
					cMyData::getObj ( )->m_nMoney		 -= nBetMoney;
					cMyData::getObj ( )->m_nBettingMoney  = nBetMoney;

					// save
					cMyData::getObj ( )->save();

					// unschedule
					this->unschedule ( schedule_selector ( MyRoomNode::step ) );

					// object release
					this->releaseObject ( );

					// mob crate info
					cUnitCreateInfos*		pMobCreate = nullptr;
					eCreateInfoFileType		eCreateInfoFile	= eCreateInfoFileType_Casino_1;
					if ( m_bIsGoCasino100 == true )	{ pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos.par"  ); eCreateInfoFile = eCreateInfoFileType_Casino_1;	}
					else							{ pMobCreate = new cUnitCreateInfos ( "casinoMobCreateInfos3.par" ); eCreateInfoFile = eCreateInfoFileType_Casino_3;	}

					// mobs list add
					std::vector<cUnit*>		aMobs;

					//select mob set
					m_pUnitSelect->setSeatIndex ( 0 );
					m_pUnitSelect->setBetMoney ( nBetMoney );

					m_pUnitSelect->texRelease ( eUnitAniType_Name_a   );
					m_pUnitSelect->texRelease ( eUnitAniType_Name_b   );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Portrait );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Normal   );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Attack   );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Special  );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Damage   );

					m_pUnitSelect->changeAni ( eUnitAniType_Normal );

					aMobs.push_back ( m_pUnitSelect );

					// select mob release
					m_pUnitSelect = nullptr;

					// mobs add
					for ( KDint i = 1; i < 5; i++ )
					{
						//create
						const cUnitCreateInfo*	pCreateInfo = pMobCreate->getUnitInfoRand ( );
						cUnit*					pMob = new cUnit ( this, pCreateInfo, eCreateInfoFile, LAYER_MYROOM_MOB, -1, pCreateInfo->nCreateIndex, i );

						pMob->texLoad ( eUnitAniType_Normal  );
						pMob->texLoad ( eUnitAniType_Attack  );
						pMob->texLoad ( eUnitAniType_Special );
						pMob->texLoad ( eUnitAniType_Damage  );

						// add
						aMobs.push_back ( pMob );
					}

					// mob dividend magnifications, win percent 
					cMobDividend	tMobDividend ( aMobs );
					for ( KDuint i = 0; i < aMobs.size ( ); i++ )
					{
						aMobs [ i ]->setDividendMagnification ( tMobDividend.getMagnification ( i ) );
					}

					// cenece change
					if ( m_bIsGoCasino100 == true )	
					{
						Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, eModeType_MyRoom_Casino100 ) ) );	
					}
					else
					{
						Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, eModeType_MyRoom_Casino1000 ) ) );	
					}
				}
				else if ( m_bIsGoArcadeEasy == true || m_bIsGoArcadeNormal == true || m_bIsGoArcadeHard == true || m_bIsGoSurvival == true )
				{
					// unschedule
					this->unschedule ( schedule_selector ( MyRoomNode::step ) );

					// object release
					this->releaseObject ( );

					// mobs list add
					std::vector<cUnit*>		aMobs;

					// select mob set
					m_pUnitSelect->setSeatIndex ( 0 );

					m_pUnitSelect->texRelease ( eUnitAniType_Name_a   );
					m_pUnitSelect->texRelease ( eUnitAniType_Name_b   );
					m_pUnitSelect->texRelease ( eUnitAniType_Portrait );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Normal   );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Attack   );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Special  );
					m_pUnitSelect->texLoad	  ( eUnitAniType_Damage   );

					m_pUnitSelect->changeAni ( eUnitAniType_Normal );

					aMobs.push_back ( m_pUnitSelect );

					// select mob release
					m_pUnitSelect = nullptr;

					// cenece change
						 if ( m_bIsGoArcadeEasy   == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, eModeType_MyRoom_Arcade_Easy ) ) );
					else if ( m_bIsGoArcadeNormal == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, eModeType_MyRoom_Arcade_Nor  ) ) );
					else if ( m_bIsGoArcadeHard   == true )	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, eModeType_MyRoom_Arcade_Hard ) ) );
					else									Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, CasinoBattleNode::createWithMobs ( aMobs, eModeType_MyRoom_Survival	) ) );
				}
			}

			return true;
		}
	}

	return false;
}

KDvoid MyRoomNode::updateMoney ( KDvoid )
{
	std::string		sStr = ccszf ( "%lld", cMyData::getObj ( )->m_nMoney );

	for ( KDint i = sStr.size ( ) - 3; i > 0; i -= 3 )
	{
		sStr.insert ( i, "," );		
	}

	m_pUIMoney->setString ( sStr.c_str ( ) );
	m_pUIMoney->setPosition ( Point ( 459 - m_pUIMoney->getSize ( ).width, 300 ) );

	// get money
	cTimeGetMoney::getObj ( )->setPoint ( Point ( 432 - m_pUIMoney->getSize ( ).width, 302 ) );
}

KDvoid MyRoomNode::updateUnitExp ( KDvoid )
{
	// switch
	KDbool		bIsSave = false;

	// loop
	for ( KDint i = 0; i < (KDint) m_aUnits.size ( ); i++ )
	{
		KDfloat		fExp = 0;

		for ( KDint a = 0; a < (KDint) cMyData::getObj ( )->m_aMyRoomUnitSaveDatas.size ( ); a++ )
		{
			if ( cMyData::getObj ( )->m_aMyRoomUnitSaveDatas [ a ].nUnitID == m_aUnits [ i ]->getUnitID ( ) )
			{
				fExp = cMyData::getObj ( )->m_aMyRoomUnitSaveDatas [ a ].fExp;
				break;
			}
		}

		// check
		if ( m_aUnits [ i ]->updateExp ( fExp ) == false )
		{
			continue;
		}

		// level up
		this->setUnitLevelUp ( m_aUnits [ i ], true );

		// switch
		bIsSave = true;
	}

	// save
	if ( bIsSave == true )
	{
		cMyData::getObj ( )->updateMyRoomUnitSaveDatas ( m_aUnits );
	}
}

KDvoid MyRoomNode::updateUnit ( KDdouble dLeftTime )
{
	// check
	if ( m_bGetCapsuleMode == true )
	{
		return;
	}

	// init
	KDint	nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;

	// loop
	for ( KDint i = nViewFirstSlotIndex; i < nViewFirstSlotIndex+_SLOT_IN_PAGE_COUNT; i++ )
	{
		// init
		cUnit*		pUnit = this->getUnit ( i );

		// check
		if ( pUnit == nullptr )
		{
			if ( i == m_nChangeSlotIndex )	m_aUIProps [ i - nViewFirstSlotIndex ]->setColor ( 255, 255, 255 );
			else							m_aUIProps [ i - nViewFirstSlotIndex ]->setColor ( 100, 100, 100 );

			continue;
		}

		// update
		if ( pUnit != m_pUnitSelect && pUnit != m_pUnitDrag )
		{
			pUnit->update ( dLeftTime );

			if ( pUnit->getAniType ( ) == eUnitAniType_Normal )
			{
				pUnit->changeAni ( eUnitAniType_Normal );
				pUnit->aniStop ( );
			}
		}

		// color
		if ( i == m_nChangeSlotIndex )
		{
			pUnit->setColor ( 255, 255, 255, 255 );
		}
		else
		{
			if ( pUnit->getAniType ( ) == eUnitAniType_Attack )	pUnit->setColor ( 255, 255, 255, 255 );
			else												pUnit->setColor ( 255, 100, 100, 100 );
		}

		// prop
		if ( pUnit != m_pUnitSelect && pUnit != m_pUnitDrag )
		{
			if ( i == m_nChangeSlotIndex )	m_aUIProps [ i - nViewFirstSlotIndex ]->setColor ( 255, 255, 255 );
			else							m_aUIProps [ i - nViewFirstSlotIndex ]->setColor ( 100, 100, 100 );
		}
	}

	// drag unit
	if ( m_pUnitDrag != nullptr )
	{
		// update
		m_pUnitDrag->update ( dLeftTime );

		// color
		m_pUnitDrag->setColor ( 255, 255, 255, 255 );

		// point
		Point		tPoint = Point ( m_tUnitTouchDragInfo.getLocation ( ).x, m_tUnitTouchDragInfo.getLocation ( ).y + 20 );
		m_pUnitDrag->setPoint ( tPoint );

		// damage ani
		m_dSelectUnitAniLeftTime -= dLeftTime;

		if ( m_dSelectUnitAniLeftTime < 0 )
		{
			m_pUnitDrag->aniStart ( );

			m_dSelectUnitAniLeftTime = 1;
		}

		// prop
		if ( m_pUnitDrag->getSlotIndex ( ) - nViewFirstSlotIndex >= 0 && m_pUnitDrag->getSlotIndex ( ) - nViewFirstSlotIndex < (KDint) m_aUIProps.size ( ) )
		{
			m_aUIProps [ m_pUnitDrag->getSlotIndex ( ) - nViewFirstSlotIndex ]->setColor ( 255, 255, 255 );
		}
	}

	// select unit
	if ( m_pUnitSelect != nullptr )
	{
		if ( m_pUnitSelect->getSlotIndex ( ) >= nViewFirstSlotIndex && m_pUnitSelect->getSlotIndex ( ) < nViewFirstSlotIndex + _SLOT_IN_PAGE_COUNT )
		{
			// update
			m_pUnitSelect->update ( dLeftTime );

			// color
			m_pUnitSelect->setColor ( 255, 255, 255, 255 );

			// attack ani
			if ( m_bIsTouchUnit == false )
			{
				m_dSelectUnitAniLeftTime -= dLeftTime;

				if ( m_dSelectUnitAniLeftTime < 0 )
				{
					m_pUnitSelect->changeAni ( eUnitAniType_Attack );
					m_pUnitSelect->aniStart ( );

					m_dSelectUnitAniLeftTime = 5;
				}
			}

			// point
			Point	tPoint = m_aUnit [ m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex ];
			if ( m_pUnitSelect->getPoint ( ).x != tPoint.x || m_pUnitSelect->getPoint ( ).y != tPoint.y )
			{
				m_pUnitSelect->setPointTime ( tPoint, 0.1f, false );
			}
		}

		// prop
		if ( m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex >= 0 && m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex < (KDint) m_aUIProps.size ( ) )
		{
			m_aUIProps [ m_pUnitSelect->getSlotIndex ( ) - nViewFirstSlotIndex ]->setColor ( 255, 255, 255 );
		}
	}
}

KDvoid MyRoomNode::updateUnitStatus ( KDvoid )
{
	// check
	if ( m_bGetCapsuleMode == true )
	{
		return;
	}

	if ( m_pUnitSelect != nullptr )
	{
		// get parameter
		KDint	nHp		= m_pUnitSelect->getHP  ( );
		KDint	nAtk	= m_pUnitSelect->getATK ( );
		KDint	nDef	= m_pUnitSelect->getDEF ( );
		KDint	nAgl	= m_pUnitSelect->getAGL ( );
		KDint	nCrt	= m_pUnitSelect->getCRT ( );
		KDint	nHpMax	= m_pUnitSelect->getHPMax  ( );
		KDint	nAtkMax = m_pUnitSelect->getATKMax ( );
		KDint	nDefMax = m_pUnitSelect->getDEFMax ( );
		KDint	nAglMax = m_pUnitSelect->getAGLMax ( );
		KDint	nCrtMax = m_pUnitSelect->getCRTMax ( );

		// parameter
		m_pLabels->setStr ( m_aUIMobDetailPara [ 0 ], ccszf ( "%02d", nHp  ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 1 ], ccszf ( "%02d", nAtk ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 2 ], ccszf ( "%02d", nDef ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 3 ], ccszf ( "%02d", nAgl ) );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 4 ], ccszf ( "%02d", nCrt ) );

		// parameter gauge
		if ( nHpMax  > 0 ) m_aUIUnitParaGauge [ 0 ]->get ( )->setTextureRect ( Rect ( 0, 0, 86 * ( (KDfloat) nHp  / (KDfloat) nHpMax  ), 11 ) );
		if ( nAtkMax > 0 ) m_aUIUnitParaGauge [ 1 ]->get ( )->setTextureRect ( Rect ( 0, 0, 86 * ( (KDfloat) nAtk / (KDfloat) nAtkMax ), 11 ) );
		if ( nDefMax > 0 ) m_aUIUnitParaGauge [ 2 ]->get ( )->setTextureRect ( Rect ( 0, 0, 86 * ( (KDfloat) nDef / (KDfloat) nDefMax ), 11 ) );
		if ( nAglMax > 0 ) m_aUIUnitParaGauge [ 3 ]->get ( )->setTextureRect ( Rect ( 0, 0, 86 * ( (KDfloat) nAgl / (KDfloat) nAglMax ), 11 ) );
		if ( nCrtMax > 0 ) m_aUIUnitParaGauge [ 4 ]->get ( )->setTextureRect ( Rect ( 0, 0, 86 * ( (KDfloat) nCrt / (KDfloat) nCrtMax ), 11 ) );

		// exp gauge
		KDfloat		fExp = m_pUnitSelect->getExp ( );
		if ( m_pUnitSelect->getIsParaMax ( ) == true )
		{
			m_pUIUnitExpGaugeLarge->get ( )->setTextureRect ( Rect ( 0, 0, 86, 6 ) );
			m_pUIUnitExpGaugeSmall->get ( )->setTextureRect ( Rect ( 0, 0, 86, 3 ) );
		}
		else
		{
			KDfloat		fWidthLarge = 86 * (KDfloat) ( (KDint) fExp ) / 100.0f;
			KDfloat		fWidthSmall = 86 * ( fExp - (KDint) fExp );
			m_pUIUnitExpGaugeLarge->get ( )->setTextureRect ( Rect ( 0, 0, fWidthLarge, 6 ) );
			m_pUIUnitExpGaugeSmall->get ( )->setTextureRect ( Rect ( 0, 0, fWidthSmall, 3 ) );
		}

		// skill
		for ( KDint i = 0; i < (KDint) m_aUISkillIcons.size ( ); i++ )
		{
			KDint		nSkillIndex = m_pUnitSelect->getSkillType ( i );

			if ( nSkillIndex == -1 )
			{
				m_pLabels->setStr ( m_aUISkillStrs [ i ], "" );
				m_aUISkillIcons [ i ]->setVisible ( false );
				continue;
			}

			m_aUISkillIcons [ i ]->setVisible ( true );
			m_aUISkillIcons [ i ]->setFrameInit ( nSkillIndex );

			std::string		sStr = m_pUnitSelect->getSkillStr ( i );

			if ( sStr.size ( ) > 0 )
			{
				m_pLabels->setStr ( m_aUISkillStrs [ i ], sStr.c_str ( ) );
			}
		}

		// UI achievement
		m_pUIClass	   ->setVisible ( true );
		m_pUIFriendship->setVisible ( true );
		m_pLabels->setVisible ( m_pUIKillCount, true );
		m_pLabels->setStr	  ( m_pUIKillCount, ccszf ( "%d", m_pUnitSelect->getKillCount ( ) ) );

			 if ( m_pUnitSelect->getClassExp ( ) > CLASS_SSS )	m_pUIClass->setFrameInit ( 0 );
		else if ( m_pUnitSelect->getClassExp ( ) > CLASS_SS	 )	m_pUIClass->setFrameInit ( 1 );
		else if ( m_pUnitSelect->getClassExp ( ) > CLASS_S	 )	m_pUIClass->setFrameInit ( 2 );
		else if ( m_pUnitSelect->getClassExp ( ) > CLASS_A	 )	m_pUIClass->setFrameInit ( 3 );
		else if ( m_pUnitSelect->getClassExp ( ) > CLASS_B	 )	m_pUIClass->setFrameInit ( 4 );
		else if ( m_pUnitSelect->getClassExp ( ) > CLASS_C	 )	m_pUIClass->setFrameInit ( 5 );
		else if ( m_pUnitSelect->getClassExp ( ) > CLASS_D	 )	m_pUIClass->setFrameInit ( 6 );
		else													m_pUIClass->setFrameInit ( 7 );

			 if ( m_pUnitSelect->getFriendShipExp ( ) > FRIENDSHIP_LOVE )	m_pUIFriendship->setFrameInit ( 4 );
		else if ( m_pUnitSelect->getFriendShipExp ( ) > FRIENDSHIP_BEST )	m_pUIFriendship->setFrameInit ( 3 );
		else if ( m_pUnitSelect->getFriendShipExp ( ) > FRIENDSHIP_GOOD )	m_pUIFriendship->setFrameInit ( 2 );
		else if ( m_pUnitSelect->getFriendShipExp ( ) > FRIENDSHIP_BAD  )	m_pUIFriendship->setFrameInit ( 1 );
		else																m_pUIFriendship->setFrameInit ( 0 );
	}
	else
	{
		// parameter
		m_pLabels->setStr ( m_aUIMobDetailPara [ 0 ], "" );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 1 ], "" );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 2 ], "" );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 3 ], "" );
		m_pLabels->setStr ( m_aUIMobDetailPara [ 4 ], "" );

		// parameter gauge
		m_aUIUnitParaGauge [ 0 ]->get ( )->setTextureRect ( Rect ( 0, 0, 0, 11 ) );
		m_aUIUnitParaGauge [ 1 ]->get ( )->setTextureRect ( Rect ( 0, 0, 0, 11 ) );
		m_aUIUnitParaGauge [ 2 ]->get ( )->setTextureRect ( Rect ( 0, 0, 0, 11 ) );
		m_aUIUnitParaGauge [ 3 ]->get ( )->setTextureRect ( Rect ( 0, 0, 0, 11 ) );
		m_aUIUnitParaGauge [ 4 ]->get ( )->setTextureRect ( Rect ( 0, 0, 0, 11 ) );
		m_pUIUnitExpGaugeLarge  ->get ( )->setTextureRect ( Rect ( 0, 0, 0,  6 ) );
		m_pUIUnitExpGaugeSmall  ->get ( )->setTextureRect ( Rect ( 0, 0, 0,  3 ) );

		// skill
		for ( KDuint i = 0; i < m_aUISkillIcons.size ( ); i++ )
		{
			m_pLabels->setStr ( m_aUISkillStrs [ i ], "" );
			m_aUISkillIcons [ i ]->setVisible ( false );
		}

		// UI achievement
		m_pUIClass	   ->setVisible ( false );
		m_pUIFriendship->setVisible ( false );
		m_pUIClass	   ->setFrameInit ( 0 );
		m_pUIFriendship->setFrameInit ( 0 );
		m_pLabels->setVisible ( m_pUIKillCount, false );
		m_pLabels->setStr	  ( m_pUIKillCount, ""	  );
	}
}

KDvoid MyRoomNode::updateCapsuleMode ( KDdouble dLeftTime )
{
	if ( m_bGetCapsuleMode == false )
	{
		return;
	}

	// ??�Ω∂ ???�∞¿ª ¥?�∑?�¿�??�ÊøÏø�?ªÃ±???�¢¿∏?��?¡¯¿?�«œ¥�?¥?�∞Ë.
	if ( m_nCapsuleModeStep == 0 )
	{
		// init
		KDdouble	dPercent = ( 0.2f - m_dCapsuleModeLeftTime ) / 0.2f;		// max == 1

		// back
		m_pSprBackLeft	->get ( )->setPosition ( Point ( 1   - ( 128 * dPercent ), 29  ) );
		m_pSprBackTopAdd->get ( )->setPosition ( Point ( 131 - ( 131 * dPercent ), 296 ) );
		m_pSprBackBottom->get ( )->setPosition ( Point ( 198, -( 28 * dPercent ) ) );

		// btn
		m_pBtnBack ->setPoint ( Point (  0, -27 + ( 27 * dPercent ) ) );
		m_pBtnShop2->setPoint ( Point ( 80, -27 + ( 27 * dPercent ) ) );

		// set machine
		KDint		nAlpha = 255 * dPercent;
		if ( nAlpha <	0 )	nAlpha = 0;
		if ( nAlpha > 255 )	nAlpha = 255;
		for ( auto pNode : m_aMachines )
		{
			pNode->setAlpha ( nAlpha );
		}

		// time
		m_dCapsuleModeLeftTime -= dLeftTime;

		// check
		if ( m_dCapsuleModeLeftTime < 0 )
		{
			// back
			m_pSprBackLeft	->get ( )->setPosition ( Point ( 1   - 128, 29	) );
			m_pSprBackTopAdd->get ( )->setPosition ( Point ( 131 - 131, 296	) );
			m_pSprBackBottom->get ( )->setPosition ( Point ( 198	  , -28 ) );

			// btn
			m_pBtnBack ->setPoint ( Point (  0, 0 ) );
			m_pBtnShop2->setPoint ( Point ( 80, 0 ) );

			// set machine
			for ( auto pNode : m_aMachines )
			{
				pNode->setAlpha ( 255 );
			}

			// drag cursor
			m_pMachineDragCursor->get ( )->setVisible ( true );

			// touch
			m_bIsTouchMachineDrag  = false;
			m_bIsTouchMachineTouch = false;

			// select machine
			m_nSelectMachine = -1;

			// ++step
			++m_nCapsuleModeStep;
		}
	}
	// ?�”Ω≈µÈ¿ª ø©?�Ø¥�??�∏ø©¡÷¥¬ ¥?�∞Ë.
	else if ( m_nCapsuleModeStep == 1 )
	{
		
	}
	// º±?�√ ?�”Ω≈¿« º±?�√¿ª ø¨?�‚«œ¥�?¥?�∞Ë.
	else if ( m_nCapsuleModeStep == 2 )
	{
		// left time
		m_dCapsuleModeLeftTime -= dLeftTime;

		// percent
		KDdouble	dPercent = ( 0.2f - m_dCapsuleModeLeftTime ) / 0.2f;	// max == 1

		// scroll
		KDint		nLastScroll = -306 + ( m_tMachine.x + m_nSelectMachine * m_nViewMachineGap );
		m_fScrollMachine = m_fScrollMachineBackup + ( ( nLastScroll - m_fScrollMachineBackup ) * dPercent );

		// alpha
		KDint		nAlpha = 255 * ( 1 - dPercent );
		if ( nAlpha <	 0 )	nAlpha = 0;
		if ( nAlpha >= 255 )	nAlpha = 255;

		for ( KDint i = 0; i < (KDint) m_aMachines.size ( ); i++ )
		{
			if ( i == m_nSelectMachine )
			{
				continue;
			}

			m_aMachines [ i ]->setAlpha ( nAlpha );
		}

		// check time
		if ( m_dCapsuleModeLeftTime < 0 )
		{
			// scroll
			m_fScrollMachine = nLastScroll;

			// alpha
			for ( KDint i = 0; i < (KDint) m_aMachines.size ( ); i++ )
			{
				if ( i == m_nSelectMachine )
				{
					continue;
				}

				m_aMachines [ i ]->setAlpha ( 0 );
			}

			// btn
			if ( m_nSelectMachine > 0 )
			{
				m_pBtnMachineLeft->setVisible ( true );
				m_pBtnMachineLeft->setEnabled ( true );
			}
			if ( m_nSelectMachine + 1 < (KDint) m_aMachines.size ( ) )
			{
				m_pBtnMachineRight->setVisible ( true );
				m_pBtnMachineRight->setEnabled ( true );
			}

			m_pBtnMachineHandle->setVisible ( true );
			m_pBtnMachineHandle->setEnabled ( true );

			//sprite touch me
			m_pMachineTouchMe->get ( )->setVisible ( true );
			m_pMachineTouchMe->get ( )->setOpacity ( 255 );
			m_bCapsuleModeTouchMeAlphaUp = false;

			// step
			m_nCapsuleModeStep	   = 3;
			m_dCapsuleModeLeftTime = 0;
		}
	}
	// º±?�√ ?�”Ω≈ø°º????�Ω∂¿ª ªÃ¥¬ ¥?�∞Ë.
	else if ( m_nCapsuleModeStep == 3 )
	{
		// sprite touchMe alpha
		KDint		nAlpha = m_pMachineTouchMe->get ( )->getOpacity ( );

		if ( m_bCapsuleModeTouchMeAlphaUp == false )	nAlpha -= 1;
		else											nAlpha += 1;

		if ( nAlpha < 180 )	{ nAlpha = 180;	m_bCapsuleModeTouchMeAlphaUp = true;  }
		if ( nAlpha > 255 )	{ nAlpha = 255;	m_bCapsuleModeTouchMeAlphaUp = false; }

		m_pMachineTouchMe->get ( )->setOpacity ( nAlpha );

		// not coin
		m_pUIMachineNotCoinCursor->update ( dLeftTime );

		// time cursor ani
		if ( m_pUIMachineTimeCursor->update ( dLeftTime ) == true )
		{
			m_pUIMachineTimeCursor->stop ( );
			m_pUIMachineTimeCursor->setVisible ( false );
		}
	}
	// ?�”Ω≈ ¿Ãµø ¥?�∞Ë.
	else if ( m_nCapsuleModeStep == 4 )
	{
		// left time
		m_dCapsuleModeLeftTime -= dLeftTime;

		// percent
		KDdouble	dPercent = ( 0.2f - m_dCapsuleModeLeftTime ) / 0.2f;	// max == 1

		// scroll
		KDint		nLastScroll	= -306 + ( m_tMachine.x + m_nSelectMachine * m_nViewMachineGap );
		m_fScrollMachine = m_fScrollMachineBackup + ( ( nLastScroll - m_fScrollMachineBackup ) * dPercent );

		// alpha
		KDint		nBeforeAlpha = 255 * ( 1 - dPercent );
		KDint		nAftereAlpha = 255 * dPercent;
		if ( nBeforeAlpha <	   0 )	nBeforeAlpha = 0;
		if ( nBeforeAlpha >= 255 )	nBeforeAlpha = 255;
		if ( nAftereAlpha <	   0 )	nAftereAlpha = 0;
		if ( nAftereAlpha >= 255 )	nAftereAlpha = 255;

		m_aMachines [ m_nSelectMachineBefore ]->setAlpha ( nBeforeAlpha );
		m_aMachines [ m_nSelectMachine		 ]->setAlpha ( nAftereAlpha );

		// check time
		if ( m_dCapsuleModeLeftTime < 0 )
		{
			// scroll
			m_fScrollMachine = nLastScroll;

			// alpha
			m_aMachines [ m_nSelectMachineBefore ]->setAlpha ( 0 );
			m_aMachines [ m_nSelectMachine		 ]->setAlpha ( 255 );

			// sprite touch me
			m_pMachineTouchMe->get ( )->setVisible ( true );
			m_pMachineTouchMe->get ( )->setOpacity ( 255 );
			m_bCapsuleModeTouchMeAlphaUp = false;

			// btn
			if ( m_nSelectMachine > 0 )
			{
				m_pBtnMachineLeft->setVisible ( true );
				m_pBtnMachineLeft->setEnabled ( true );
			}
			if ( m_nSelectMachine + 1 < (KDint) m_aMachines.size ( ) )
			{
				m_pBtnMachineRight->setVisible ( true );
				m_pBtnMachineRight->setEnabled ( true );
			}

			m_pBtnMachineHandle->setVisible ( true );
			m_pBtnMachineHandle->setEnabled ( true );

			// step
			m_nCapsuleModeStep	   = 3;
			m_dCapsuleModeLeftTime = 0;
		}
	}
	// Back ¥?�∞Ë.
	else if ( m_nCapsuleModeStep == 5 )
	{
		// init
		KDdouble		dPercent = ( 0.2f - m_dCapsuleModeLeftTime ) / 0.2f;	// max == 1

		// back
		if ( m_nCapsuleModeStep2 == 0 )
		{
			m_pSprBackLeft	->get ( )->setPosition ( Point ( -127 + ( 128 * dPercent ), 29  ) );
			m_pSprBackTopAdd->get ( )->setPosition ( Point (		  131 * dPercent  , 296 ) );
			m_pSprBackBottom->get ( )->setPosition ( Point ( 198				      , -28 + ( 28 * dPercent ) ) );

			// btn
			m_pBtnBack ->setPoint ( Point (  0, -( 27 * dPercent ) ) );
			m_pBtnShop2->setPoint ( Point ( 80, -( 27 * dPercent ) ) );

			// set machine
			KDint		nAlpha = 255 - ( 255 * dPercent );
			if ( nAlpha <	0 )	nAlpha = 0;
			if ( nAlpha > 255 )	nAlpha = 255;
			for ( auto pNode : m_aMachines )
			{
				if ( nAlpha < pNode->getAlpha ( ) )
				{
					pNode->setAlpha ( nAlpha );
				}
			}
		}
		else
		{
			// scroll
			KDint	nScrollMax = ( m_aMachines.size ( ) * m_nViewMachineGap ) - _SCR_W;

				 if ( m_fScrollMachineBackup <			0 )	m_fScrollMachine = m_fScrollMachineBackup + ( ( 0		   - m_fScrollMachineBackup ) * dPercent );
			else if ( m_fScrollMachineBackup > nScrollMax )	m_fScrollMachine = m_fScrollMachineBackup + ( ( nScrollMax - m_fScrollMachineBackup ) * dPercent );

			// set machine
			KDint	nAlpha = 255 * dPercent;
			if ( nAlpha <	0 )	nAlpha = 0;
			if ( nAlpha > 255 )	nAlpha = 255;
			for ( auto pNode : m_aMachines  )
			{
				if ( nAlpha > pNode->getAlpha ( ) )	
				{
					pNode->setAlpha ( nAlpha );
				}
			}
		}

		// time
		m_dCapsuleModeLeftTime -= dLeftTime;

		// time check
		if ( m_dCapsuleModeLeftTime < 0 )
		{
			if ( m_nCapsuleModeStep2 == 0 )
			{
				// switch
				m_bGetCapsuleMode  = false;
				m_nCapsuleModeStep = 0;

				// back
				m_pSprBackLeft	->get ( )->setPosition ( Point (   1,  29 ) );
				m_pSprBackTopAdd->get ( )->setPosition ( Point ( 131, 296 ) );
				m_pSprBackBottom->get ( )->setPosition ( Point ( 198,   0 ) );

				// scroll
				KDint	nScrollMax = ( m_aMachines.size ( ) * m_nViewMachineGap ) - _SCR_W;
					 if ( m_fScrollMachineBackup <			0 )	m_fScrollMachine = 0;
				else if ( m_fScrollMachineBackup > nScrollMax )	m_fScrollMachine = nScrollMax;

				// machine
				for ( auto pNode : m_aMachines )
				{
					pNode->setVisible ( false );
				}

				// btn
				m_pBtnBack ->setVisible ( false );
				m_pBtnShop2->setVisible ( false );

				m_pBtnExit->setVisible ( true );
				m_pBtnShop->setVisible ( true );
				m_pBtnExit->setEnabled ( true );
				m_pBtnShop->setEnabled ( true );

				m_pBtnSocial ->setVisible ( true );
				m_pBtnCapsule->setVisible ( true );
				m_pBtnBattle ->setVisible ( true );
				m_pBtnSocial ->setIsTouchEnable ( true );
				m_pBtnCapsule->setIsTouchEnable ( true );
				m_pBtnBattle ->setIsTouchEnable ( true );

				// status
				for ( auto pNode : m_aUIUniqueType	  )	pNode->get ( )->setVisible ( false );
				for ( auto pNode : m_aUIUnitParaGauge )	pNode->get ( )->setVisible ( true );
				for ( auto pNode : m_aUISkillIcons	  )	pNode->setVisible ( true );
				for ( auto pNode : m_aUIMobDetailPara )	m_pLabels->setVisible ( pNode, true );
				for ( auto pNode : m_aUISkillStrs	  )	m_pLabels->setVisible ( pNode, true );
				m_pUIUnitExpGaugeLarge->get ( )->setVisible ( true );
				m_pUIUnitExpGaugeSmall->get ( )->setVisible ( true );

				// page
				this->setPage ( cMyData::getObj ( )->m_nMyRoomPage, true );
				this->setPageRange ( );

				// select unit
				if ( m_pUnitSelect != nullptr )
				{
					this->selectUnit ( m_pUnitSelect );
				}
			}
			else
			{
				// machine
				for ( auto pNode : m_aMachines )	
				{
					pNode->setAlpha ( 255 );
				}

				// step
				m_nCapsuleModeStep = 1;
			}
		}
	}
	// ??�Ω∂¿ª ªÃ¥¬ ¥?�∞Ë.
	else if ( m_nCapsuleModeStep == 6 )
	{
		// left time
		m_dCapsuleModeLeftTime -= dLeftTime;

		// step2
		if ( m_nCapsuleModeStep2 == 0 )
		{
			// percent
			KDdouble	dPercent = ( 0.3f - m_dCapsuleModeLeftTime ) / 0.3f;	// max == 1

			// machine coin
			Point		tCoin = Point ( 403 - dPercent * 22, 130 );
			KDfloat		fTexX = 392 - tCoin.x;
			if ( fTexX < 0 )
			{
				fTexX = 0;
			}
			m_pUIMachineCoin->get ( )->setPosition ( Point ( tCoin.x + fTexX, tCoin.y ) );
			m_pUIMachineCoin->get ( )->setTextureRect ( Rect ( fTexX, 0, 10-fTexX, 14 ) );

			if ( m_dCapsuleModeLeftTime < 0 )
			{
				// machine coin
				m_pUIMachineCoin->get ( )->setVisible ( false );

				// left time
				m_dCapsuleModeLeftTime = 0.5;

				// step
				++m_nCapsuleModeStep2;

				// sound
				if (cMyData::getObj ( )->m_bIsOptionSound != false )
				{
					SimpleAudioEngine::getInstance ( )->playEffect ( ccszf ( "machine_into_coin.mp3" ) );
				}
			}
		}
		else if ( m_nCapsuleModeStep2 == 1 )
		{			
			if ( m_dCapsuleModeLeftTime < 0 )
			{
				// sound
				if ( cMyData::getObj ( )->m_bIsOptionSound != false )
				{
					SimpleAudioEngine::getInstance ( )->playEffect ( ccszf ( "machine_handle_rotate.mp3" ) );
				}

				// left time
				m_dCapsuleModeLeftTime = 0.1;

				// step
				++m_nCapsuleModeStep2;
			}
		}
		else if ( m_nCapsuleModeStep2 == 2 )
		{
			// percent
			KDdouble	dPercent = ( 0.1f - m_dCapsuleModeLeftTime ) / 0.1f;	// max == 1

			// sprite capsule
			if ( m_nMachineHandleRotateCount == 1 )
			{
				m_pSprCapsule1->get ( )->setPosition ( Point ( 331.5f, 140 - 55 * dPercent ) );
				m_pSprCapsule2->get ( )->setPosition ( Point ( 331.5f, 140 - 55 * dPercent ) );
			}

			// handle rotate
			KDint		nRotate = 0;
			switch ( m_nMachineHandleRotateCount )
			{
				case 0	:	nRotate =       50 * dPercent;	break;
				case 1	:	nRotate = 50 + 130 * dPercent;	break;
			}
			m_aMachines [ m_nSelectMachine ]->setHandleRotate ( nRotate );

			// check
			if ( m_dCapsuleModeLeftTime < 0 )
			{
				// sprite capsule
				if ( m_nMachineHandleRotateCount == 1 )
				{
					m_pSprCapsule1->get ( )->setPosition ( Point ( 331.5f, 85 ) );
					m_pSprCapsule2->get ( )->setPosition ( Point ( 331.5f, 85 ) );

					// handle rotate
					m_aMachines [ m_nSelectMachine ]->setHandleRotate ( 0 );

					// left time
					m_dCapsuleModeLeftTime = 0.5f;

					// step
					++m_nCapsuleModeStep2;

					// sound
					if ( cMyData::getObj ( )->m_bIsOptionSound != false )
					{
						SimpleAudioEngine::getInstance ( )->playEffect ( ccszf ( "machine_out_capsule.mp3" ) );
					}
				}
				else
				{
					// handle rotate
					m_aMachines [ m_nSelectMachine ]->setHandleRotate ( 50 );

					// left time
					m_dCapsuleModeLeftTime = 0.4f;

					// step
					++m_nCapsuleModeStep2;

					// sound
					if ( cMyData::getObj ( )->m_bIsOptionSound != false )
					{
						SimpleAudioEngine::getInstance ( )->playEffect ( ccszf ( "machine_capsule_shake.mp3" ) );
					}
				}

				// rotate count
				++m_nMachineHandleRotateCount;
			}
		}
		else if ( m_nCapsuleModeStep2 == 3 )
		{
			if ( m_dCapsuleModeLeftTime < 0 )
			{
				if ( m_nMachineHandleRotateCount < 2 )
				{
					// sound
					if ( cMyData::getObj ( )->m_bIsOptionSound != false )
					{
						SimpleAudioEngine::getInstance ( )->playEffect ( ccszf ( "machine_handle_rotate.mp3" ) );
					}

					// left time
					m_dCapsuleModeLeftTime = 0.1f;

					// step
					--m_nCapsuleModeStep2;
				}
				else
				{
					// sprite capsule
					m_pSprCapsule1->get ( )->setVisible ( false );
					m_pSprCapsule2->get ( )->setVisible ( false );
					m_pSprCapsule3->get ( )->setVisible ( true );
					m_pSprCapsule4->get ( )->setVisible ( true );
					m_pSprCapsule3->get ( )->setScale ( 0.2f );
					m_pSprCapsule4->get ( )->setScale ( 0.2f );
					m_pSprCapsule3->get ( )->setPosition ( Point ( 331.5f, 85 ) );
					m_pSprCapsule4->get ( )->setPosition ( Point ( 331.5f, 85 ) );
					m_pSprCapsule3->get ( )->setOpacity ( 255 );
					m_pSprCapsule4->get ( )->setOpacity ( 255 );
					m_pSprCapsule3->get ( )->setRotation ( 0 );
					m_pSprCapsule4->get ( )->setRotation ( 0 );

					// cover
					m_pCoverColor2->setVisible ( true );

					// left time
					m_dCapsuleModeLeftTime = 0.2f;

					// step
					++m_nCapsuleModeStep2;
				}
			}
		}
		else if ( m_nCapsuleModeStep2 == 4 )
		{
			// percent
			KDdouble	dPercent = ( 0.2f - m_dCapsuleModeLeftTime ) / 0.2f;	// max == 1

			// sprite capsule
			Point		tPoint = Point ( 331.5f - dPercent * ( 331.5f - _SCR_W_HARP ), 85 + dPercent * ( 176 - 85 ) );
			KDfloat		fScale = 0.2f + dPercent * 0.8;
			m_pSprCapsule3->get ( )->setPosition ( tPoint );
			m_pSprCapsule4->get ( )->setPosition ( tPoint );
			m_pSprCapsule3->get ( )->setScale ( fScale );
			m_pSprCapsule4->get ( )->setScale ( fScale );

			// check
			if ( m_dCapsuleModeLeftTime < 0 )
			{
				// sprite capsule
				m_pSprCapsule3->get ( )->setPosition ( Point ( _SCR_W_HARP, 176 ) );
				m_pSprCapsule4->get ( )->setPosition ( Point ( _SCR_W_HARP, 176 ) );
				m_pSprCapsule3->get ( )->setScale ( 1.0f );
				m_pSprCapsule4->get ( )->setScale ( 1.0f );

				m_nCapsuleRotate1 = ( kdRand ( ) % 180 ) - 90;
				m_nCapsuleRotate2 = ( kdRand ( ) % 180 ) - 90;

				// capsule touch me font
				m_pSprCapsuleTouchMeFont->get ( )->setVisible ( true );

				// shake
				m_bIsCapsuleShake = false;
				m_fCapsuleShake	  = 0;

				// left time
				m_dCapsuleModeLeftTime = 0.5f;

				// step
				++m_nCapsuleModeStep2;
			}
		}
		else if ( m_nCapsuleModeStep2 == 5 )
		{
			if ( m_bIsCapsuleShake == true )
			{
				if ( m_fCapsuleShake > -0.5f && m_fCapsuleShake < 0.5f )
				{
					// sprite capsule
					m_pSprCapsule3->get ( )->setPosition ( Point ( _SCR_W_HARP, 176 ) );
					m_pSprCapsule4->get ( )->setPosition ( Point ( _SCR_W_HARP, 176 ) );

					// shake
					m_bIsCapsuleShake = false;
				}
				else
				{
					// shake
					if ( m_fCapsuleShake > 0 )	m_fCapsuleShake = -m_fCapsuleShake + 0.2f;
					else						m_fCapsuleShake = -m_fCapsuleShake - 0.2f;

					// sprite capsule
					m_pSprCapsule3->get ( )->setPosition ( Point ( _SCR_W_HARP + m_fCapsuleShake, 176 ) );
					m_pSprCapsule4->get ( )->setPosition ( Point ( _SCR_W_HARP + m_fCapsuleShake, 176 ) );
				}
			}

			// time check
			if ( m_dCapsuleModeLeftTime < 0 )
			{
				m_bIsCapsuleShake	   = true;
				m_fCapsuleShake		   = 8;
				m_dCapsuleModeLeftTime = 2.5f + kdRand ( ) % 2;
			}

			// check
			if ( m_bIsCapsuleTouch == true )
			{
				// capsule touch me font
				m_pSprCapsuleTouchMeFont->get ( )->setVisible ( false );

				// getting prop
				m_pUIGettingProp->setVisible ( true );
				m_pUIGettingProp->setSelect ( m_pUnitSelect->getUniqueType ( ) );

				// get unit
				m_pUnitSelect->setVisible ( true );
				m_pUnitSelect->changeAni ( eUnitAniType_Damage );
				m_pUnitSelect->aniStart ( );
				m_pUnitSelect->setPoint ( Point ( 240, 132 ) );

				// particle
				m_pParticleGetUnit->start ( );

				// set machine
				for ( KDuint i = 0; i < m_aMachines.size ( ); i++ )
				{
					m_aMachines [ i ]->setIsMark ( m_aUnits );
					if ( m_aMachines [ i ]->getVisibleUnitList ( ) == true )
					{
						m_aMachines [ i ]->setVisibleUnitList ( true, false );
					}
				}

				// left time
				m_dCapsuleModeLeftTime = 0.2f;

				// step
				++m_nCapsuleModeStep2;
			}
		}
		else if ( m_nCapsuleModeStep2 == 6 )
		{
			// percent
			KDdouble		dPercent = ( 0.2f - m_dCapsuleModeLeftTime ) / 0.2f;	// max == 1

			// get unit
			m_pUnitSelect->update ( dLeftTime );

			// sprite capsule
			KDint			nAlpha = 255 - ( dPercent * 255 );
			if ( nAlpha < 0 )	
			{
				nAlpha = 0;
			}

			m_pSprCapsule3->get ( )->setPosition ( Point ( _SCR_W_HARP, 176 + dPercent * 50 ) );
			m_pSprCapsule4->get ( )->setPosition ( Point ( _SCR_W_HARP, 176 - dPercent * 50 ) );
			m_pSprCapsule3->get ( )->setRotation ( m_nCapsuleRotate1 * dPercent );
			m_pSprCapsule4->get ( )->setRotation ( m_nCapsuleRotate2 * dPercent );
			m_pSprCapsule3->get ( )->setOpacity  ( nAlpha );
			m_pSprCapsule4->get ( )->setOpacity  ( nAlpha );

			// damage ani?��??�°≥?�∏È ?�ÿΩ∫?��?Ω?�≈?�¿∏?��?
			if ( m_pUnitSelect->getAniType ( ) == eUnitAniType_Normal )
			{
				// get unit
				m_pUnitSelect->changeAni ( eUnitAniType_Attack );

				// sprite capsule
				m_pSprCapsule3->get ( )->setVisible ( false );
				m_pSprCapsule4->get ( )->setVisible ( false );

				// get back
				m_pUIGettingBack->get ( )->setVisible ( true );
				m_pUIGettingBack->get ( )->setOpacity ( 0 );

				// btn
				m_pBtnGain->setVisible ( true );
				m_pBtnGain->setOpacity ( 0 );

				// left time
				m_dCapsuleModeLeftTime = 2;

				// step
				++m_nCapsuleModeStep2;

				// ±?�¡∏ø° æ¯¥¬ ¿Ø¥÷¿Ã?�Û∏È ªÁøÓµÂ?��??�‚∑¬«ÿ¡ÿ¥Ÿ.
				if ( cMyData::getObj ( )->m_bIsOptionSound != false )
				{
					KDbool		bIsSame = false;
					for ( KDint i = 0; i < (KDint) m_aUnits.size ( ); i++ )
					{
						if ( m_aUnits [ i ]->getUnitID ( ) == m_pUnitSelect->getUnitID ( ) )	continue;
						
						if ( m_aUnits [ i ]->getName ( ).compare( m_pUnitSelect->getName ( ) ) == 0 )
						{
							bIsSame = true;
							break;
						}
					}

					if ( bIsSame == false )
					{
						SimpleAudioEngine::getInstance ( )->playEffect ( "capsule_rare_unique.mp3" );
					}
				}
			}
		}
		else if ( m_nCapsuleModeStep2 == 7 )
		{
			// get unit
			if ( m_dCapsuleModeLeftTime > 0.2f && m_pUnitSelect->getAniType ( ) != eUnitAniType_Attack )
			{
				m_pUnitSelect->changeAni ( eUnitAniType_Attack );
			}
			m_pUnitSelect->update ( dLeftTime );

			// get back, btn
			if ( m_dCapsuleModeLeftTime < 0.5f )
			{
				KDint		nAlpha = ( 1 - m_dCapsuleModeLeftTime * 2 ) * 255;

				m_pUIGettingBack->get ( )->setOpacity ( nAlpha );
				m_pBtnGain			     ->setOpacity ( nAlpha );
			}

			// check
			if ( m_dCapsuleModeLeftTime < 0 )
			{
				// particle
				m_pParticleGetUnit->stop ( );

				// get back
				m_pUIGettingBack->get ( )->setOpacity ( 255 );

				// btn
				m_pBtnGain->setEnabled ( true );
				m_pBtnGain->setOpacity ( 255 );

				// left time
				m_dCapsuleModeLeftTime = 0;

				// step
				++m_nCapsuleModeStep2;
			}
		}
		else if ( m_nCapsuleModeStep2 == 8 )
		{
			// get unit
			m_pUnitSelect->update ( dLeftTime );
		}
	}

	// machine
	for ( KDuint i = 0; i < m_aMachines.size ( ); i++ )
	{
		m_aMachines [ i ]->update ( dLeftTime );
		m_aMachines [ i ]->setPoint ( Point ( m_tMachine.x + m_nViewMachineGap * i - m_fScrollMachine, m_tMachine.y ) );
	}
}

KDvoid MyRoomNode::updateLevelUpEffect ( KDdouble dLeftTime )
{
	// init
	KDint	nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;

	if ( m_bGetCapsuleMode == true )
	{
		//loop
		std::list<cMyRoomLvlUpEffect*>::iterator	pNode = m_aLvUpEffects.begin ( );
		while ( pNode != m_aLvUpEffects.end ( ) )
		{
			// visible
			(*pNode)->setVisible ( false );

			// update
			if ( (*pNode)->update ( dLeftTime ) == true )
			{
				*pNode = nullptr;
				pNode  = m_aLvUpEffects.erase ( pNode );
			}
			else
			{
				++pNode;
			}
		}
	}
	else
	{
		//loop
		std::list<cMyRoomLvlUpEffect*>::iterator	pNode = m_aLvUpEffects.begin ( );
		while ( pNode != m_aLvUpEffects.end ( ) )
		{
			// init
			KDint	nSlot = (*pNode)->getSlotIndex ( );

			// visible
			if ( m_bIsUnitDeleting == true || m_bIsUnitDeleteYes == true || m_bGetCapsuleMode == true )
			{
				(*pNode)->setVisible ( false );
			}
			if ( m_pUnitDrag != nullptr )
			{
				if ( nSlot == m_pUnitDrag->getSlotIndex ( ) )	(*pNode)->setVisible ( true );
				else											(*pNode)->setVisible ( false );
			}
			else
			{
				if ( nSlot < nViewFirstSlotIndex || nSlot >= nViewFirstSlotIndex+_SLOT_IN_PAGE_COUNT )	(*pNode)->setVisible ( false );
				else																					(*pNode)->setVisible ( true );
			}

			// update
			if ( (*pNode)->update ( dLeftTime ) == true )
			{
				*pNode = nullptr;
				pNode = m_aLvUpEffects.erase ( pNode );
			}
			else
			{
				++pNode;
			}
		}

		// effect point
		for ( auto pUnit : m_aUnits )
		{
			for ( auto pNode : m_aLvUpEffects )
			{
				if ( pUnit->getSlotIndex ( ) == pNode->getSlotIndex ( ) )
				{
					Point	tCenter  = pUnit->getPoint ( ) + Point (  0, ( pUnit->getNorFirstSize ( ).height / 2 ) );
					Point	tSpecial = pUnit->getPoint ( ) + pUnit->getPointSpecial ( );
					
					pNode->setPoint ( tCenter, tSpecial );					
					break;
				}
			}
		}
	}

	// level up popup
	std::list<cLevelPopup*>::iterator	pNode = m_aLvUpPopups.begin ( );
	while ( pNode != m_aLvUpPopups.end ( ) )
	{
		if ( (*pNode)->update ( dLeftTime ) == true )	pNode = m_aLvUpPopups.erase ( pNode );
		else											++pNode;
	}
}

KDvoid MyRoomNode::updateBuyingCheck ( KDvoid )
{
	if ( m_pShopNode->getIsShop ( ) == false )
	{
		m_bIsBuyingDollarCapsule = false;
		return;
	}

	if ( m_bIsBuyingDollarCapsule == true )
	{
		// end check
		if ( m_pShopNode->isBuyEnd ( ) == false )	
		{
			return;
		}

		// buy check
		if ( m_pShopNode->isBuyDollarCapsule ( ) == true )
		{
			eDollarMachineType		eDollarMachine = m_pShopNode->getDollarMachineType ( );

			for ( KDuint i = 0; i < m_aMachines.size ( ); i++ )
			{
				if ( m_aMachines [ i ]->getDollarMachineType ( ) == eDollarMachine )
				{
					this->buyCapsule ( i );
				}
			}
		}
		else
		{
			m_pBtnBack ->setIsTouchEnable ( true );
			m_pBtnShop2->setIsTouchEnable ( true );

			if ( m_nCapsuleModeStep == 3 )
			{
				m_pBtnMachineLeft  ->setEnabled ( true );
				m_pBtnMachineRight ->setEnabled ( true );
				m_pBtnMachineHandle->setEnabled ( true );
			}
		}

		m_pCoverColor->setVisible ( false );

		m_pShopNode->close ( );

		if ( cMyData::getObj ( )->m_bIsOptionSound != false && SimpleAudioEngine::getInstance ( )->isBackgroundMusicPlaying ( ) == false )
		{
			SimpleAudioEngine::getInstance ( )->playBackgroundMusic ( "myroom.mp3", true );
		}

		// switch
		m_bIsBuyingDollarCapsule = false;
	}
}

KDvoid MyRoomNode::selectUnitAndPage ( KDint nUnitID )
{
	// select unit
	for ( auto pNode : m_aUnits )
	{
		if ( pNode->getUnitID ( ) == nUnitID )
		{
			this->selectUnit ( pNode );
			break;
		}
	}

	// attack left time
	m_dSelectUnitAniLeftTime = 5;

	// «?¿Á º±?�√µ» ¿Ø¥÷¿Ã ¿÷¥¬ ?�‰¿Ã¡ˆ∑Œ ¿?�µ�?¿Ãµø µ»¥Ÿ.
	this->setPage ( m_pUnitSelect->getSlotIndex ( ) / _SLOT_IN_PAGE_COUNT, false );
	this->setPageRange ( );
}

KDvoid MyRoomNode::selectUnit ( cUnit* pUnit )
{
	// before
	if ( m_pUnitSelect != nullptr )
	{
		KDbool		bIsVisible = m_pUnitSelect->getVisible ( );
		m_pUnitSelect->setVisibleNameA ( false );
		m_pUnitSelect->changeAni ( eUnitAniType_Normal );
		m_pUnitSelect->aniStop ( );
		m_pUnitSelect->setVisible ( bIsVisible );
	}

	// change select unit
	m_pUnitSelect = pUnit;

	if ( m_pUnitSelect != nullptr )
	{
		// now
		m_pUnitSelect->setVisibleNameA ( true );
		m_pUnitSelect->changeAni ( eUnitAniType_Normal );
		m_pUnitSelect->aniStart ( );

		// attack left time
		m_dSelectUnitAniLeftTime = 0;

		// unique type
		for ( auto pNode : m_aUIUniqueType )
		{
			pNode->get ( )->setVisible ( false );
		}

		switch ( (KDint) m_pUnitSelect->getUniqueType ( ) )
		{
			case eUniqueType_common		:	m_aUIUniqueType [ 0 ]->get ( )->setVisible ( true );	break;
			case eUniqueType_uncommon	:	m_aUIUniqueType [ 1 ]->get ( )->setVisible ( true );	break;
			case eUniqueType_rare		:	m_aUIUniqueType [ 2 ]->get ( )->setVisible ( true );	break;
			case eUniqueType_unique		:	m_aUIUniqueType [ 3 ]->get ( )->setVisible ( true );	break;
		}
	}
	else
	{
		// attack left time
		m_dSelectUnitAniLeftTime = 0;

		// unique type
		for ( auto pNode : m_aUIUniqueType )	
		{
			pNode->get ( )->setVisible ( false );
		}
	}
}

KDvoid MyRoomNode::exitCallback ( Object* pSender )
{
	m_bIsNodeChangeOut = true;
	m_bIsGoTitle = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg = new cNodeChangeImgs ( this, true );

	m_pBtnExit			->setEnabled ( false );
	m_pBtnShop			->setEnabled ( false );
	m_pBtnShopExit		->setEnabled ( false );
	m_pBtnMachineLeft	->setEnabled ( false );
	m_pBtnMachineRight	->setEnabled ( false );
	m_pBtnMachineHandle	->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setVisible	   ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::backCallback ( KDvoid )
{
	// check
	if ( m_bGetCapsuleMode == false )
	{
		return;
	}

	if ( m_nCapsuleModeStep == 1 )
	{
		// btn
		m_pBtnBack ->setIsTouchEnable ( false );
		m_pBtnShop2->setIsTouchEnable ( false );

		// sprite
		m_pMachineDragCursor->get ( )->setVisible ( false );
		m_pMachineTouchMe   ->get ( )->setVisible ( false );
		m_pUIMachineTimeCursor	 ->setVisible ( false );
		m_pUIMachineNotCoinCursor->setVisible ( false );

		// shop cursor
		m_pUIShopCursor->stop ( );
		m_pUIShopCursor->setVisible ( false );
		m_bUIIsNotCoinView = false;

		// step
		m_nCapsuleModeStep  = 5;
		m_nCapsuleModeStep2 = 0;

		// time
		m_dCapsuleModeLeftTime = 0.2f;
	}
	else
	{
		// btn
		m_pBtnMachineLeft  ->setVisible ( false );
		m_pBtnMachineRight ->setVisible ( false );
		m_pBtnMachineHandle->setVisible ( false );
		m_pBtnMachineLeft  ->setEnabled ( false );
		m_pBtnMachineRight ->setEnabled ( false );
		m_pBtnMachineHandle->setEnabled ( false );

		// machine unit list
		for ( auto pNode : m_aMachines )
		{
			if ( pNode->getVisibleUnitList ( ) == true )
			{
				pNode->setVisibleUnitList ( false, true );
			}
		}

		// shop cursor
		m_pUIShopCursor->stop ( );
		m_pUIShopCursor->setVisible ( false );
		m_bUIIsNotCoinView = false;

		// sprite
		m_pMachineDragCursor->get ( )->setVisible ( false );
		m_pMachineTouchMe   ->get ( )->setVisible ( false );
		m_pUIMachineTimeCursor	 ->setVisible ( false );
		m_pUIMachineNotCoinCursor->setVisible ( false );

		// scroll
		m_fScrollMachineBackup = m_fScrollMachine;

		// step
		m_nCapsuleModeStep  = 5;
		m_nCapsuleModeStep2 = 1;

		// time
		m_dCapsuleModeLeftTime = 0.2f;
	}
}

KDvoid MyRoomNode::shopCallback ( Object* pSender )
{
	this->shop2Callback ( );
}

KDvoid MyRoomNode::shop2Callback ( KDvoid )
{
	m_pBtnExit			->setEnabled ( false );
	m_pBtnShop			->setEnabled ( false );
	m_pBtnMachineLeft	->setEnabled ( false );
	m_pBtnMachineRight	->setEnabled ( false );
	m_pBtnMachineHandle	->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setVisible	   ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );

	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_pBtnShopExit->setVisible ( true );
	m_pBtnShopExit->setEnabled ( true );

	m_pBtnShopExit->setPosition ( Point ( 62, _SCR_H + 96 ) );

	m_pCoverColor->setVisible ( true );

	m_pShopNode->setVisible ( true );
	m_pShopNode->setPosition ( Point ( 0, _SCR_H ) );

	m_bUIIsNotCoinView = false;
	m_pUIShopCursor->setVisible ( false );
	m_pUIShopCursor->stop ( );

	m_pUIMachineTimeCursor	 ->setVisible ( false );
	m_pUIMachineNotCoinCursor->setVisible ( false );

	//shop cursor
	m_pUIShopCursor->stop ( );
	m_pUIShopCursor->setVisible ( false );
	m_bUIIsNotCoinView = false;

	m_pShopNode->open ( eShopType_Normal, eDollarMachineType_Not );
}

KDvoid MyRoomNode::pageLeftCallback ( KDvoid )
{
	KDint		nPage = m_nViewFirstPageIndex - 1;
	if ( nPage < 0 )	
	{
		nPage = 0;
	}
	this->setPage ( nPage, false );

	if ( cMyData::getObj ( )->m_nMyRoomPage != m_nViewFirstPageIndex + _NUM_IN_PAGE_COUNT - 1 )
	{
		this->setPage ( m_nViewFirstPageIndex + _NUM_IN_PAGE_COUNT - 1, false );
	}
}

KDvoid MyRoomNode::pageRightCallback ( KDvoid )
{
	KDint		nPage = m_nViewFirstPageIndex + _NUM_IN_PAGE_COUNT;
	if ( nPage >= m_nPageMax )
	{
		nPage = m_nPageMax - 1;
	}
	this->setPage ( nPage, false );

	if ( cMyData::getObj ( )->m_nMyRoomPage != m_nViewFirstPageIndex )
	{
		this->setPage ( m_nViewFirstPageIndex, false );
	}
}

KDvoid MyRoomNode::socialCallback ( KDvoid )
{
	if ( m_pUnitSelect == nullptr )
	{
		return;
	}

	const KDchar*	pImgName = m_pUnitSelect->getPortraitSprFileName ( ).c_str ( );
	const KDchar*	pUrl	 = "http://itunes.apple.com/app/battle-monster-casino/id403465401?mt=8";
	std::string		sStr	 = "BATTLE MONSTER CASINO\n";

	// name
	sStr += ccszf ( "Introduction to [ %s ]\n", m_pUnitSelect->getName ( ).c_str ( ) );

	// class
		 if ( m_pUnitSelect->getClassExp ( ) > CLASS_SSS )	sStr += "Class : SSS\n";
	else if ( m_pUnitSelect->getClassExp ( ) > CLASS_SS	 )	sStr += "Class : SS\n";
	else if ( m_pUnitSelect->getClassExp ( ) > CLASS_S	 )	sStr += "Class : S\n";
	else if ( m_pUnitSelect->getClassExp ( ) > CLASS_A	 )	sStr += "Class : A\n";
	else if ( m_pUnitSelect->getClassExp ( ) > CLASS_B	 )	sStr += "Class : B\n";
	else if ( m_pUnitSelect->getClassExp ( ) > CLASS_C	 )	sStr += "Class : C\n";
	else if ( m_pUnitSelect->getClassExp ( ) > CLASS_D	 )	sStr += "Class : D\n";
	else													sStr += "Class : F\n";

	// kill
	sStr += ccszf ( "Kills : %d\n", m_pUnitSelect->getKillCount ( ) );

	// skill
	sStr += "have skill : ";
	for ( KDint i = 0; i < 3; i++ )
	{
		KDint			nSkillIndex = m_pUnitSelect->getSkillType ( i );
		if ( nSkillIndex == -1 )
		{
			continue;
		}

		std::string		sStrSkill = m_pUnitSelect->getSkillStr ( i );
		if ( i == 0 )	sStr += sStrSkill;
		else			sStr += ccszf ( "/ %s", sStrSkill.c_str ( ) );
	}
	sStr += "\n";

#if 0
	// set
	[OFSocialNotificationService sendWithText:str imageNamed:imgName linkedUrl:url);
#endif
}

KDvoid MyRoomNode::capsuleCallback ( KDvoid )
{
	// switch
	m_bGetCapsuleMode = true;

	// step
	m_nCapsuleModeStep = 0;

	// left time
	m_dCapsuleModeLeftTime = 0.2f;

	// units visible
	for ( auto pNode : m_aUnits )
	{
		pNode->setVisibleNameA ( false );
		pNode->aniStop ( );
		pNode->setVisible ( false );
	}

	// unit props
	for ( auto pNode : m_aUIProps )
	{
		pNode->setVisible ( false );
	}

	// back
	m_pSprBackTopAdd->get ( )->setPosition ( Point ( 131, 296 ) );

	// btn
	m_pBtnPageLeft	->setVisible ( false );
	m_pBtnPageRight	->setVisible ( false );
	m_pBtnSocial	->setVisible ( false );
	m_pBtnCapsule	->setVisible ( false );
	m_pBtnBattle	->setVisible ( false );
	m_pBtnExit		->setEnabled ( false );
	m_pBtnShop		->setEnabled ( false );
	m_pBtnPageLeft	->setIsTouchEnable ( false );
	m_pBtnPageRight	->setIsTouchEnable ( false );
	m_pBtnSocial	->setIsTouchEnable ( false );
	m_pBtnCapsule	->setIsTouchEnable ( false );
	m_pBtnBattle	->setIsTouchEnable ( false );
	m_pUIBtnSocialGray->get ( )->setVisible ( false );
	m_pUIBtnBattleGray->get ( )->setVisible ( false );

	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
		pNode->setVisible	    ( false );
	}

	m_pBtnBack ->setVisible ( true );
	m_pBtnShop2->setVisible ( true );
	m_pBtnBack ->setIsTouchEnable ( true );
	m_pBtnShop2->setIsTouchEnable ( true );

	m_pBtnBack ->setPoint ( Point (  0, -27 ) );
	m_pBtnShop2->setPoint ( Point ( 80, -27 ) );

	// status
	for ( auto pNode : m_aUIUniqueType	  )	pNode->get ( )->setVisible ( false );
	for ( auto pNode : m_aUIUnitParaGauge )	pNode->get ( )->setVisible ( false );
	for ( auto pNode : m_aUISkillIcons	  )	pNode->setVisible ( false );
	for ( auto pNode : m_aUIMobDetailPara )	m_pLabels->setVisible ( pNode, false );
	for ( auto pNode : m_aUISkillStrs	  )	m_pLabels->setVisible ( pNode, false );

	m_pUIUnitExpGaugeLarge->get ( )->setVisible ( false );
	m_pUIUnitExpGaugeSmall->get ( )->setVisible ( false );
	m_pUIClass	   ->setVisible ( false );
	m_pUIFriendship->setVisible ( false );
	m_pUIClass	   ->setFrameInit ( 0 );
	m_pUIFriendship->setFrameInit ( 0 );
	m_pLabels->setVisible ( m_pUIKillCount, false );
	m_pLabels->setStr	  ( m_pUIKillCount, ""	  );

	// set machine
	for ( KDuint i = 0; i < m_aMachines.size ( ); i++ )
	{
		m_aMachines [ i ]->setVisible ( true );
		m_aMachines [ i ]->setAlpha ( 0 );
		m_aMachines [ i ]->setIsMark ( m_aUnits );
		m_aMachines [ i ]->setPoint ( Point ( m_fScrollMachine + m_tMachine.x + m_nViewMachineGap * i, m_tMachine.y ) );
	}
}

KDvoid MyRoomNode::wastebasketCallback ( KDvoid )
{
	// check
	if ( m_pUnitDrag == nullptr )
	{
		return;
	}

	// switch
	m_bIsUnitDeleting = true;

	// point
	Point	tPoint = m_pUnitDrag->getPoint ( ) + m_pUnitDrag->getPointSpecial ( );
	tPoint.x += 10;
	if ( tPoint.x < 0 )
	{
		tPoint.x = 0;
	}

	// messageBox
	m_pUIUnitDeleteMsgBox->get ( )->setVisible ( true );
	m_pUIUnitDeleteMsgBox->get ( )->setPosition ( tPoint );

	m_pLabels->setVisible ( m_pUIUnitDeleteMsg, true );
	m_pLabels->setPoint ( m_pUIUnitDeleteMsg, Point ( tPoint.x + 176, tPoint.y + 46 ) );

	if ( m_pUnitDrag->getName ( ).compare ( "Yellow Fighter" ) == 0 )
	{
		m_pLabels->setStr ( m_pUIUnitDeleteMsg, "D'oh!" );
	}
	else if ( m_pUnitDrag->getName ( ).compare ( "Omega Tron" ) == 0 )
	{
		m_pLabels->setStr ( m_pUIUnitDeleteMsg, "I'm omegatron!!!!" );
	}
	else if ( m_pUnitDrag->getName ( ).compare("Zeta Tron") == 0)
	{
		m_pLabels->setStr ( m_pUIUnitDeleteMsg, "I'm zetatron!!!!" );
	}
	else
	{
		switch ( kdRand ( ) % 7 )
		{
			case 0	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "What the...?"			);	break;
			case 1	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "Shut your yap!"		);	break;
			case 2	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "help!!!!"				);	break;
			case 3	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "Oh, No! please..."		);	break;
			case 4	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "You what now!"			);	break;
			case 5	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "no no no, Bad idea."	);	break;
			case 6	:	m_pLabels->setStr ( m_pUIUnitDeleteMsg, "What are you doing?"	);	break;
		}
	}

	m_pBtnUnitDeleteYes->setVisible ( true );
	m_pBtnUnitDeleteNo ->setVisible ( true );
	m_pBtnUnitDeleteYes->setEnabled ( true );
	m_pBtnUnitDeleteNo ->setEnabled ( true );
	m_pBtnUnitDeleteYes->setPosition ( Point ( tPoint.x + 257, tPoint.y + 12 ) );
	m_pBtnUnitDeleteNo ->setPosition ( Point ( tPoint.x + 127, tPoint.y + 12 ) );

	// page btn
	m_pBtnPageDragLeft ->setVisible ( false );
	m_pBtnPageDragRight->setVisible ( false );
	m_pBtnPageLeft	   ->setIsTouchEnable ( false );
	m_pBtnPageRight	   ->setIsTouchEnable ( false );
	m_pBtnPageDragLeft ->setIsTouchEnable ( false );
	m_pBtnPageDragRight->setIsTouchEnable ( false );

	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	// wastebasket
	m_pBtnWastebasket->setIsTouchEnable ( false );
	m_pBtnWastebasket->setVisible ( false, true );
	m_pBtnWastebasket->setVisible ( true , true );
}

KDvoid MyRoomNode::battleCallback ( KDvoid )
{
	if ( m_pUnitSelect == nullptr )
	{
		return;
	}

	m_bIsModePopup = true;

	m_pCoverColor->setVisible ( true );

	m_pUIModeBack->get ( )->setVisible ( true );

	m_pBtnExit->setEnabled ( false );
	m_pBtnShop->setEnabled ( false );

	m_pBtnPageLeft ->setIsTouchEnable ( false );
	m_pBtnPageRight->setIsTouchEnable ( false );
	m_pBtnSocial   ->setIsTouchEnable ( false );
	m_pBtnCapsule  ->setIsTouchEnable ( false );
	m_pBtnBattle   ->setIsTouchEnable ( false );

	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_pBtnModeExit		  ->setVisible ( true );
	m_pBtnModeCasino100	  ->setVisible ( true );
	m_pBtnModeCasino1000  ->setVisible ( true );
	m_pBtnModeArcadeEasy  ->setVisible ( true );
	m_pBtnModeArcadeNormal->setVisible ( true );
	m_pBtnModeArcadeHard  ->setVisible ( true );
	m_pBtnModeSurvival	  ->setVisible ( true );
	m_pBtnModeExit		  ->setEnabled ( true );
	m_pBtnModeCasino100	  ->setEnabled ( true );
	m_pBtnModeCasino1000  ->setEnabled ( true );
	m_pBtnModeArcadeEasy  ->setEnabled ( true );
	m_pBtnModeArcadeNormal->setEnabled ( true );
	m_pBtnModeArcadeHard  ->setEnabled ( true );
	m_pBtnModeSurvival	  ->setEnabled ( true );

	if ( m_pUnitSelect->getIsArcadeEasyClear ( ) == true )	m_pUIArcadeEasyClear->get ( )->setVisible ( true );
	if ( m_pUnitSelect->getIsArcadeNorClear  ( ) == true )	m_pUIArcadeNorClear	->get ( )->setVisible ( true );
	if ( m_pUnitSelect->getIsArcadeHardClear ( ) == true )	m_pUIArcadeHardClear->get ( )->setVisible ( true );
}

KDvoid MyRoomNode::unitDeleteYesCallback ( Object* pSender )
{
	// init
	KDint	nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;

	// switch
	m_bIsUnitDeleteYes		    = true;
	m_dUnitDeleteEffectLeftTime = 0;

	// prop
	if ( m_pUnitDrag->getSlotIndex ( ) >= nViewFirstSlotIndex && m_pUnitDrag->getSlotIndex ( ) < nViewFirstSlotIndex + _SLOT_IN_PAGE_COUNT )
	{
		m_aUIProps [ m_pUnitDrag->getSlotIndex ( ) - nViewFirstSlotIndex ]->setSelect ( eUniqueType_not );
		m_aUIProps [ m_pUnitDrag->getSlotIndex ( ) - nViewFirstSlotIndex ]->setColor ( 100, 100, 100 );
	}

	// particle
	m_pParticleUnitDelect->get ( )->setPosition ( m_pUnitDrag->getPoint ( ) + Point ( 0, m_pUnitDrag->getNorFirstSize ( ).height / 2 ) );
	m_pParticleUnitDelect->start ( );
	m_bIsDeleteParticleStop		  = true;
	m_dDeleteParticleStopLeftTime = 2;

	//delete drag unit
	for ( std::vector<cUnit*>::iterator pNode = m_aUnits.begin ( ); pNode != m_aUnits.end ( ); ++pNode )
	{
		if ( *pNode == m_pUnitDrag )
		{			
			m_aUnits.erase ( pNode );
			CC_SAFE_RELEASE ( *pNode );
			break;
		}
	}

	// money
	switch ( m_pUnitDrag->getUniqueType ( ) )
	{
		case eUniqueType_common		:	cMyData::getObj ( )->m_nMoney += 50;	break;
		case eUniqueType_uncommon	:	cMyData::getObj ( )->m_nMoney += 100;	break;
		case eUniqueType_rare		:	cMyData::getObj ( )->m_nMoney += 300;	break;
		case eUniqueType_unique		:	cMyData::getObj ( )->m_nMoney += 500;	break;
		case eUniqueType_not		:											break;
	}

	// save
	cMyData::getObj ( )->delMyRoomUnitSaveDatas ( m_pUnitDrag );

	// drag init
	m_pUnitDrag = nullptr;

	//set machine
	for ( auto pNode : m_aMachines )
	{
		pNode->setIsMark ( m_aUnits );
	}

	// messageBox
	m_pUIUnitDeleteMsgBox->get ( )->setVisible ( false );
	m_pBtnUnitDeleteYes->setVisible ( false );
	m_pBtnUnitDeleteNo ->setVisible ( false );
	m_pBtnUnitDeleteYes->setEnabled ( false );
	m_pBtnUnitDeleteNo ->setEnabled ( false );
	m_pLabels->setVisible ( m_pUIUnitDeleteMsg, false );

	// select unit
	this->selectUnit ( nullptr );

	// sound
	if ( cMyData::getObj ( )->m_bIsOptionSound != false ) 
	{
		SimpleAudioEngine::getInstance ( )->playEffect ( "unitDelete.mp3" );
	}
}

KDvoid MyRoomNode::unitDeleteNoCallback ( Object* pSender )
{
	// switch
	m_bIsUnitDeleting = false;

	// drag unit
	m_pUnitSelect  = m_pUnitDrag;
	m_pUnitDrag	   = nullptr;
	m_bIsTouchUnit = false;

	m_pUnitSelect->setVisibleNameA ( true );
	m_pUnitSelect->changeAni ( eUnitAniType_Normal );
	m_pUnitSelect->aniStart ( );
	m_pUnitSelect->setIsAutoChangeNor ( true );

	m_dSelectUnitAniLeftTime = 5;

	// messageBox
	m_pUIUnitDeleteMsgBox->get ( )->setVisible ( false );
	m_pBtnUnitDeleteYes->setVisible ( false );
	m_pBtnUnitDeleteNo ->setVisible ( false );
	m_pBtnUnitDeleteYes->setEnabled ( false );
	m_pBtnUnitDeleteNo ->setEnabled ( false );
	m_pLabels->setVisible ( m_pUIUnitDeleteMsg, false );

	// set page
	this->setPage ( m_pUnitSelect->getSlotIndex ( ) / _SLOT_IN_PAGE_COUNT, false );
	this->setPageRange ( );
}

KDvoid MyRoomNode::shopExitCallback ( Object* pSender )
{
	m_pBtnExit			->setEnabled ( true );
	m_pBtnShop			->setEnabled ( true );
	m_pBtnMachineLeft	->setEnabled ( true );
	m_pBtnMachineRight	->setEnabled ( true );
	m_pBtnMachineHandle	->setEnabled ( true );

	m_pBtnBack			->setIsTouchEnable ( true );
	m_pBtnShop2			->setIsTouchEnable ( true );
	m_pBtnPageLeft		->setIsTouchEnable ( true );
	m_pBtnPageRight		->setIsTouchEnable ( true );
	m_pBtnPageDragLeft	->setIsTouchEnable ( true );
	m_pBtnPageDragRight	->setIsTouchEnable ( true );
	m_pBtnSocial		->setIsTouchEnable ( true );
	m_pBtnCapsule		->setIsTouchEnable ( true );
	m_pBtnBattle		->setIsTouchEnable ( true );
	m_pBtnWastebasket	->setVisible	   ( true );
	m_pBtnWastebasket	->setIsTouchEnable ( true );

	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( true );
	}

	m_pBtnShopExit->setVisible ( true );
	m_pBtnShopExit->setEnabled ( true );

//	m_pBtnShopExit->setPosition ( Point ( 62, _SCR_H + 96 ) );

	m_pCoverColor->setVisible ( true );

	m_pShopNode->setVisible ( true );
//	m_pShopNode->setPosition ( Point ( 0, _SCR_H ) );
/*
	m_bUIIsNotCoinView = true;
	m_pUIShopCursor->setVisible ( true );
	m_pUIShopCursor->start ( );

	m_pUIMachineTimeCursor	 ->setVisible ( true );
	m_pUIMachineNotCoinCursor->setVisible ( true );

	// shop cursor
	m_pUIShopCursor->start ( );
	m_pUIShopCursor->setVisible ( true );
	m_bUIIsNotCoinView = true;
*/
	m_pShopNode->close ( );
}	

KDvoid MyRoomNode::machineLeftCallback ( Object* pSender )
{
	// check
	if ( m_bGetCapsuleMode == false || m_nCapsuleModeStep != 3 || m_nSelectMachine <= 0 )
	{
		return;
	}

	// before machine
	m_nSelectMachineBefore = m_nSelectMachine;

	// select machine
	--m_nSelectMachine;

	// machine unit list
	m_aMachines [ m_nSelectMachineBefore ]->setVisibleUnitList ( false, false );
	m_aMachines [ m_nSelectMachine		 ]->setVisibleUnitList ( true , false );

	// scroll
	m_fScrollMachineBackup = m_fScrollMachine;

	// btn
	m_pBtnMachineLeft  ->setVisible ( false );
	m_pBtnMachineRight ->setVisible ( false );
	m_pBtnMachineHandle->setVisible ( false );
	m_pBtnMachineLeft  ->setEnabled ( false );
	m_pBtnMachineRight ->setEnabled ( false );
	m_pBtnMachineHandle->setEnabled ( false );

	// sprite touch me
	m_pMachineTouchMe->get ( )->setVisible ( false );
	m_pUIMachineTimeCursor	  ->setVisible ( false );
	m_pUIMachineNotCoinCursor ->setVisible ( false );

	// shop cursor
	m_pUIShopCursor->stop ( );
	m_pUIShopCursor->setVisible ( false );
	m_bUIIsNotCoinView = false;

	// step
	m_nCapsuleModeStep	   = 4;
	m_dCapsuleModeLeftTime = 0.2f;
}

KDvoid MyRoomNode::machineRightCallback ( Object* pSender )
{
	// check
	if ( m_bGetCapsuleMode == false || m_nCapsuleModeStep != 3 || m_nSelectMachine + 1 >= (KDint) m_aMachines.size ( ) )
	{
		return;
	}

	// before machine
	m_nSelectMachineBefore = m_nSelectMachine;

	// select machine
	++m_nSelectMachine;

	// machine unit list
	m_aMachines [ m_nSelectMachineBefore ]->setVisibleUnitList ( false, false );
	m_aMachines [ m_nSelectMachine		 ]->setVisibleUnitList ( true , false );

	// scroll
	m_fScrollMachineBackup = m_fScrollMachine;

	// btn
	m_pBtnMachineLeft  ->setVisible ( false );
	m_pBtnMachineRight ->setVisible ( false );
	m_pBtnMachineHandle->setVisible ( false );
	m_pBtnMachineLeft  ->setEnabled ( false );
	m_pBtnMachineRight ->setEnabled ( false );
	m_pBtnMachineHandle->setEnabled ( false );

	// sprite
	m_pMachineTouchMe->get ( )->setVisible ( false );
	m_pUIMachineTimeCursor	  ->setVisible ( false );
	m_pUIMachineNotCoinCursor ->setVisible ( false );

	// shop cursor
	m_pUIShopCursor->stop ( );
	m_pUIShopCursor->setVisible ( false );
	m_bUIIsNotCoinView = false;

	// step
	m_nCapsuleModeStep	   = 4;
	m_dCapsuleModeLeftTime = 0.2f;
}

KDvoid MyRoomNode::machineHandleCallback ( Object* pSender )
{
	// check
	if ( m_bGetCapsuleMode == false || m_nCapsuleModeStep != 3 || m_nSelectMachine < 0 || m_nSelectMachine >= (KDint) m_aMachines.size ( ) )
	{
		return;
	}

	// shop cursor
	m_pUIShopCursor->stop ( );
	m_pUIShopCursor->setVisible ( false );
	m_bUIIsNotCoinView = false;

	// time check
	if ( m_aMachines [ m_nSelectMachine ]->buyLeftTime ( ) != nullptr && *( m_aMachines [ m_nSelectMachine ]->buyLeftTime ( ) ) > 0 )
	{
		m_aMachines [ m_nSelectMachine ]->setColorLabelNum ( 255, 0, 0 );
		m_pUIMachineTimeCursor->setVisible ( true );
		m_pUIMachineTimeCursor->start ( );

		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "capsule_buy_not.mp3" );
		}

		return;
	}

	// slot check
	if ( cMyData::getObj ( )->m_nSlotCount == (KDint) m_aUnits.size ( ) )
	{
		m_pUINotSlotPopup->get ( )->setVisible ( true );
		m_dNotSlotLeftTime = 1.0f;

		m_bUIIsNotCoinView = true;
		m_pUIShopCursor->setVisible ( true );
		m_pUIShopCursor->start ( );

		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "capsule_buy_not.mp3" );
		}

		return;
	}

	// coin check
	if ( cMyData::getObj ( )->m_nMoney < m_aMachines [ m_nSelectMachine ]->getNeedCoin ( ) )
	{
		m_aMachines [ m_nSelectMachine ]->setVisibleNotCoinTrue ( );
		m_pUIMachineNotCoinCursor->setVisible ( true );
		m_pUIMachineNotCoinCursor->start ( );

		m_bUIIsNotCoinView = true;
		m_pUIShopCursor->setVisible ( true );
		m_pUIShopCursor->start ( );

		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "capsule_buy_not.mp3" );
		}

		return;
	}

	// premium check
	if ( m_aMachines [ m_nSelectMachine ]->getDollarMachineType ( ) > eDollarMachineType_Not )
	{
		m_pBtnExit		   ->setEnabled ( false );
		m_pBtnShop		   ->setEnabled ( false );
		m_pBtnMachineLeft  ->setEnabled ( false );
		m_pBtnMachineRight ->setEnabled ( false );
		m_pBtnMachineHandle->setEnabled ( false );

		m_pBtnBack		   ->setIsTouchEnable ( false );
		m_pBtnShop2		   ->setIsTouchEnable ( false );
		m_pBtnPageLeft	   ->setIsTouchEnable ( false );
		m_pBtnPageRight	   ->setIsTouchEnable ( false );
		m_pBtnPageDragLeft ->setIsTouchEnable ( false );
		m_pBtnPageDragRight->setIsTouchEnable ( false );
		m_pBtnSocial	   ->setIsTouchEnable ( false );
		m_pBtnCapsule	   ->setIsTouchEnable ( false );
		m_pBtnBattle	   ->setIsTouchEnable ( false );
		m_pBtnWastebasket  ->setVisible		  ( false );
		m_pBtnWastebasket  ->setIsTouchEnable ( false );

		for ( auto pNode : m_aBtnPageNums )
		{
			pNode->setIsTouchEnable ( false );
		}

		m_pCoverColor->setVisible ( true );

		m_pShopNode->setVisible	( true );
		m_pShopNode->setPosition ( Point ( 0, 0 ) );

		m_bIsBuyingDollarCapsule = true;
		 
		m_pShopNode->open ( eShopType_Machine, m_aMachines [ m_nSelectMachine ]->getDollarMachineType ( ) );

		return;
	}

	// machine ¿« Ω?�∞£ ?� ±‚»≠.
	if ( m_aMachines [ m_nSelectMachine ]->buyLeftTime ( ) != nullptr )
	{
		*( m_aMachines [ m_nSelectMachine ]->buyLeftTime ( ) ) = m_aMachines [ m_nSelectMachine ]->buyDelayTime ( );
	}

	// µ?�¿�?¡Ÿø©¡ÿ¥Ÿ.
	cMyData::getObj ( )->m_nMoney -= m_aMachines [ m_nSelectMachine ]->getNeedCoin ( );

	// buy
	this->buyCapsule ( m_nSelectMachine );
}

KDvoid MyRoomNode::gainCallback ( Object* pSender )
{
	// cover color
	m_pCoverColor2->setVisible ( false );

	// btn
	m_pBtnBack ->setIsTouchEnable ( true );
	m_pBtnShop2->setIsTouchEnable ( true );

	if ( m_nSelectMachine > 0 )
	{
		m_pBtnMachineLeft->setVisible ( true );
		m_pBtnMachineLeft->setEnabled ( true );
	}
	if ( m_nSelectMachine + 1 < (KDint) m_aMachines.size ( ) )
	{
		m_pBtnMachineRight->setVisible ( true );
		m_pBtnMachineRight->setEnabled ( true );
	}

	m_pBtnMachineHandle->setVisible ( true  );
	m_pBtnGain		   ->setVisible ( false );
	m_pBtnMachineHandle->setEnabled ( true  );
	m_pBtnGain		   ->setEnabled ( false );

	// sprite touch me
	m_pMachineTouchMe->get ( )->setVisible ( true );
	m_pMachineTouchMe->get ( )->setOpacity ( 255 );
	m_bCapsuleModeTouchMeAlphaUp = false;

	// select unit
	m_pUnitSelect->setVisible ( false );
	m_pUnitSelect->setPoint ( m_aUnit [ m_pUnitSelect->getSlotIndex ( ) % _SLOT_IN_PAGE_COUNT ] );

	// sprite back
	m_pUIGettingBack->get ( )->setVisible ( false );

	// prop
	m_pUIGettingProp->setVisible ( false );

	// select machine handle rotate
	m_aMachines [ m_nSelectMachine ]->setHandleRotate ( 0 );

	// step
	m_nCapsuleModeStep	   = 3;
	m_dCapsuleModeLeftTime = 0;
}

KDvoid MyRoomNode::modeExitCallback ( Object* pSender )
{
	m_pUINotCoinPopup->get ( )->setVisible ( false );
	m_bUIIsNotCoin		 = false;
	m_dUINotCoinLeftTime = 1.5f;

	m_bIsModePopup = false;

	m_pCoverColor->setVisible ( false );

	m_pUIModeBack->get ( )->setVisible ( false );

	m_pBtnExit->setEnabled ( true );
	m_pBtnShop->setEnabled ( true );

	m_pBtnSocial ->setIsTouchEnable ( true );
	m_pBtnCapsule->setIsTouchEnable ( true );
	m_pBtnBattle ->setIsTouchEnable ( true );

	m_pBtnModeExit			->setVisible ( false );
	m_pBtnModeCasino100		->setVisible ( false );
	m_pBtnModeCasino1000	->setVisible ( false );
	m_pBtnModeArcadeEasy	->setVisible ( false );
	m_pBtnModeArcadeNormal	->setVisible ( false );
	m_pBtnModeArcadeHard	->setVisible ( false );
	m_pBtnModeSurvival		->setVisible ( false );
	m_pBtnModeExit			->setEnabled ( false );
	m_pBtnModeCasino100		->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pUIArcadeEasyClear->get ( )->setVisible ( false );
	m_pUIArcadeNorClear	->get ( )->setVisible ( false );
	m_pUIArcadeHardClear->get ( )->setVisible ( false );

	this->setPage ( cMyData::getObj ( )->m_nMyRoomPage, true );
	this->setPageRange ( );
}

KDvoid MyRoomNode::modeCasino100Callback ( Object* pSender )
{
	if ( cMyData::getObj ( )->m_nMoney < 100 )
	{
		m_pUINotCoinPopup->get ( )->setVisible ( true );
		m_bUIIsNotCoin		 = true;
		m_dUINotCoinLeftTime = 1.5f;

		return;
	}

	m_pBtnExit				->setEnabled ( false );
	m_pBtnShop				->setEnabled ( false );
	m_pBtnShopExit			->setEnabled ( false );
	m_pBtnUnitDeleteYes		->setEnabled ( false );
	m_pBtnUnitDeleteNo		->setEnabled ( false );
	m_pBtnMachineLeft		->setEnabled ( false );
	m_pBtnMachineRight		->setEnabled ( false );
	m_pBtnMachineHandle		->setEnabled ( false );
	m_pBtnGain				->setEnabled ( false );
	m_pBtnModeExit			->setEnabled ( false );
	m_pBtnModeCasino100		->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		  ->setIsTouchEnable ( false );
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_bIsNodeChangeOut	= true;
	m_bIsGoCasino100	= true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg	= new cNodeChangeImgs ( this, true );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::modeCasino1000Callback ( Object* pSender )
{
	if (cMyData::getObj ( )->m_nMoney < 1000)
	{
		m_pUINotCoinPopup->get ( )->setVisible ( true );
		m_bUIIsNotCoin		 = true;
		m_dUINotCoinLeftTime = 1.5f;
		return;
	}

	m_pBtnExit		   		->setEnabled ( false );
	m_pBtnShop		   		->setEnabled ( false );
	m_pBtnShopExit	   		->setEnabled ( false );
	m_pBtnUnitDeleteYes 	->setEnabled ( false );
	m_pBtnUnitDeleteNo  	->setEnabled ( false );
	m_pBtnMachineLeft   	->setEnabled ( false );
	m_pBtnMachineRight  	->setEnabled ( false );
	m_pBtnMachineHandle 	->setEnabled ( false );
	m_pBtnGain		   		->setEnabled ( false );
	m_pBtnModeExit	   		->setEnabled ( false );
	m_pBtnModeCasino100 	->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_bIsNodeChangeOut = true;
	m_bIsGoCasino1000  = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::modeArcadeEasyCallback ( Object* pSender )
{
	m_pBtnExit		   		->setEnabled ( false );
	m_pBtnShop		   		->setEnabled ( false );
	m_pBtnShopExit	   		->setEnabled ( false );
	m_pBtnUnitDeleteYes 	->setEnabled ( false );
	m_pBtnUnitDeleteNo  	->setEnabled ( false );
	m_pBtnMachineLeft   	->setEnabled ( false );
	m_pBtnMachineRight  	->setEnabled ( false );
	m_pBtnMachineHandle 	->setEnabled ( false );
	m_pBtnGain		   		->setEnabled ( false );
	m_pBtnModeExit	   		->setEnabled ( false );
	m_pBtnModeCasino100 	->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_bIsNodeChangeOut = true;
	m_bIsGoArcadeEasy  = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::modeArcadeNormalCallback ( Object* pSender )
{
	m_pBtnExit		   		->setEnabled ( false );
	m_pBtnShop		   		->setEnabled ( false );
	m_pBtnShopExit	   		->setEnabled ( false );
	m_pBtnUnitDeleteYes		->setEnabled ( false );
	m_pBtnUnitDeleteNo		->setEnabled ( false );
	m_pBtnMachineLeft		->setEnabled ( false );
	m_pBtnMachineRight		->setEnabled ( false );
	m_pBtnMachineHandle		->setEnabled ( false );
	m_pBtnGain		   		->setEnabled ( false );
	m_pBtnModeExit	   		->setEnabled ( false );
	m_pBtnModeCasino100		->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_bIsNodeChangeOut  = true;
	m_bIsGoArcadeNormal = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg    = new cNodeChangeImgs ( this, true );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::modeArcadeHardCallback ( Object* pSender )
{
	m_pBtnExit		   		 ->setEnabled ( false );
	m_pBtnShop		   		 ->setEnabled ( false );
	m_pBtnShopExit	   		->setEnabled ( false );
	m_pBtnUnitDeleteYes		->setEnabled ( false );
	m_pBtnUnitDeleteNo		->setEnabled ( false );
	m_pBtnMachineLeft		->setEnabled ( false );
	m_pBtnMachineRight		->setEnabled ( false );
	m_pBtnMachineHandle		->setEnabled ( false );
	m_pBtnGain				->setEnabled ( false );
	m_pBtnModeExit	   		->setEnabled ( false );
	m_pBtnModeCasino100		->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_bIsNodeChangeOut = true;
	m_bIsGoArcadeHard  = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg   = new cNodeChangeImgs ( this, true );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::modeSurvivalCallback ( Object* pSender )
{
	m_pBtnExit		   		->setEnabled ( false );
	m_pBtnShop		   		->setEnabled ( false );
	m_pBtnShopExit	   		->setEnabled ( false );
	m_pBtnUnitDeleteYes		->setEnabled ( false );
	m_pBtnUnitDeleteNo		->setEnabled ( false );
	m_pBtnMachineLeft		->setEnabled ( false );
	m_pBtnMachineRight		->setEnabled ( false );
	m_pBtnMachineHandle		->setEnabled ( false );
	m_pBtnGain				->setEnabled ( false );
	m_pBtnModeExit			->setEnabled ( false );
	m_pBtnModeCasino100		->setEnabled ( false );
	m_pBtnModeCasino1000	->setEnabled ( false );
	m_pBtnModeArcadeEasy	->setEnabled ( false );
	m_pBtnModeArcadeNormal	->setEnabled ( false );
	m_pBtnModeArcadeHard	->setEnabled ( false );
	m_pBtnModeSurvival		->setEnabled ( false );

	m_pBtnBack			->setIsTouchEnable ( false );
	m_pBtnShop2			->setIsTouchEnable ( false );
	m_pBtnPageLeft		->setIsTouchEnable ( false );
	m_pBtnPageRight		->setIsTouchEnable ( false );
	m_pBtnPageDragLeft	->setIsTouchEnable ( false );
	m_pBtnPageDragRight	->setIsTouchEnable ( false );
	m_pBtnSocial		->setIsTouchEnable ( false );
	m_pBtnCapsule		->setIsTouchEnable ( false );
	m_pBtnWastebasket	->setIsTouchEnable ( false );
	m_pBtnBattle		->setIsTouchEnable ( false );
	for ( auto pNode : m_aBtnPageNums )
	{
		pNode->setIsTouchEnable ( false );
	}

	m_bIsNodeChangeOut = true;
	m_bIsGoSurvival	   = true;
	CC_SAFE_RELEASE ( m_pNodeChangeImg );
	m_pNodeChangeImg  = new cNodeChangeImgs ( this, true );

	// stop background music
	SimpleAudioEngine::getInstance ( )->stopBackgroundMusic ( );
}

KDvoid MyRoomNode::setExitTouchEnabled ( KDbool bIsEnabled )
{
	m_pBtnShopExit->setEnabled ( bIsEnabled );
}

cUnit* MyRoomNode::getUnit ( KDint nSlotIndex )
{
	for ( auto pNode : m_aUnits )
	{
		if ( pNode->getSlotIndex ( ) == nSlotIndex )
		{
			return pNode;
		}
	}

	return nullptr;
}

KDvoid MyRoomNode::setPage ( KDint nPage, KDbool bIsForcibly )
{
	// check
	if ( nPage < 0 || nPage >= m_nPageMax || ( bIsForcibly == false && nPage == cMyData::getObj ( )->m_nMyRoomPage ) )
	{
		return;
	}

	// set page
	cMyData::getObj ( )->m_nMyRoomPage = nPage;

	// init
	KDint		nViewFirstSlotIndex = cMyData::getObj ( )->m_nMyRoomPage * _SLOT_IN_PAGE_COUNT;

	// unit texture & visible clear
	for ( KDuint i = 0; i < m_aUnits.size ( ); i++ )
	{
		if ( m_aUnits [ i ] != m_pUnitDrag )
		{
			m_aUnits [ i ]->texRelease ( eUnitAniType_Name_a );
			m_aUnits [ i ]->texRelease ( eUnitAniType_Normal );
			m_aUnits [ i ]->texRelease ( eUnitAniType_Attack );
			m_aUnits [ i ]->texRelease ( eUnitAniType_Damage );

			m_aUnits [ i ]->setVisible ( false );
		}
	}

	// prop
	for ( KDuint i = 0; i < m_aUIProps.size ( ); i++ )
	{
		m_aUIProps [ i ]->setSelect ( eUniqueType_not );
	}

	// unit visible & ani start
	for ( KDint i = nViewFirstSlotIndex; i < nViewFirstSlotIndex + _SLOT_IN_PAGE_COUNT; i++ )
	{
		// init
		cUnit*	pUnit = this->getUnit ( i );

		// check
		if ( pUnit == nullptr )
		{
			continue;
		}

		// texture
		if ( pUnit != m_pUnitDrag )
		{
			pUnit->texLoad ( eUnitAniType_Name_a );
			pUnit->texLoad ( eUnitAniType_Normal );
			pUnit->texLoad ( eUnitAniType_Attack );
			pUnit->texLoad ( eUnitAniType_Damage );

			pUnit->changeAni ( eUnitAniType_Normal );

			// visible
			pUnit->setVisible ( true );

			// point
			if ( pUnit != m_pUnitSelect )
			{
				pUnit->setPoint ( m_aUnit [ i - nViewFirstSlotIndex ] );
			}
		}

		// select µ» ¿Ø¥÷¿Ã?�Û∏È ¥ŸΩ??æ÷¥œ?�ﬁ¿Ãº«¿ª Ω?�¿€«ÿ¡ÿ¥Ÿ.
		if ( pUnit == m_pUnitSelect )
		{
			pUnit->aniStart ( );
			m_dSelectUnitAniLeftTime = 5;
		}
		// drag ¡ﬂ¿�?¿Ø¥÷¿Ã æ?�¥œ∂Û?��?ani stop
		else if ( pUnit != m_pUnitDrag )
		{
			pUnit->aniStop ( );
		}

		// prop
		m_aUIProps [ i - nViewFirstSlotIndex ]->setSelect ( pUnit->getUniqueType ( ) );
	}

	// drag¡ﬂ¿�?¿Ø¥÷¿« ?�π¿ÃæÓ?��?¥ŸΩ???�÷ªÛ¿∏?��?«?�¥�?
	if ( m_pUnitDrag != nullptr )
	{
		m_pUnitDrag->changeNode ( this, LAYER_MYROOM_MOB );
	}

	this->setPageRange ( );
}

KDvoid MyRoomNode::setPageRange ( KDvoid )
{
	// view first page
	for ( KDint i = 0; i < 2000; i++ )
	{
		if ( m_nViewFirstPageIndex > cMyData::getObj ( )->m_nMyRoomPage )
		{
			m_nViewFirstPageIndex -= _NUM_IN_PAGE_COUNT;
			continue;
		}

		if ( m_nViewFirstPageIndex <= cMyData::getObj ( )->m_nMyRoomPage -_NUM_IN_PAGE_COUNT )
		{
			m_nViewFirstPageIndex += _NUM_IN_PAGE_COUNT;
			continue;
		}
		break;
	}

	// init
	Point		tStartNum			 = Point ( 0, 82 );
	Point		tLastNum			 = Point ( 0, 82 );
	const KDint nPageNumWidthGap	 = 44;
	const KDint nPageNumWidthGapHarp = 22;

	// ?�∏¡§.
	if ( m_nViewFirstPageIndex >= m_nPageMax - _NUM_IN_PAGE_COUNT )	m_nViewFirstPageIndex = m_nPageMax - _NUM_IN_PAGE_COUNT;
	if ( m_nViewFirstPageIndex <  0								  )	m_nViewFirstPageIndex = 0;

	// first point
	switch ( m_nPageMax )
	{
	case 1	:	tStartNum.x = 304;								tLastNum.x = 304;	break;
	case 2	:	tStartNum.x = 304 - nPageNumWidthGapHarp; 		tLastNum.x = 304 + nPageNumWidthGapHarp;		break;
	case 3	:	tStartNum.x = 304 - nPageNumWidthGapHarp * 2;	tLastNum.x = 304 + nPageNumWidthGapHarp * 2;	break;
	case 4	:	tStartNum.x = 304 - nPageNumWidthGapHarp * 3;	tLastNum.x = 304 + nPageNumWidthGapHarp * 3;	break;
	default	:	tStartNum.x = 304 - nPageNumWidthGapHarp * 4;	tLastNum.x = 304 + nPageNumWidthGapHarp * 4;	break;
	}

	// set num btn
	for ( KDint i = 0; i < (KDint) m_aBtnPageNums.size ( ); i++ )
	{
		if ( m_nViewFirstPageIndex + i >= m_nPageMax )
		{
			m_aBtnPageNums [ i ]->setVisible	   ( false );
			m_aBtnPageNums [ i ]->setIsTouchEnable ( false );
			continue;
		}

		m_aBtnPageNums [ i ]->setVisible ( true );
		m_aBtnPageNums [ i ]->setPoint( Point ( tStartNum.x + i * nPageNumWidthGap, tStartNum.y ) );
		m_aBtnPageNums [ i ]->setNum ( m_nViewFirstPageIndex + i + 1 );

		if ( m_nViewFirstPageIndex+i == cMyData::getObj ( )->m_nMyRoomPage )
		{
			m_aBtnPageNums [ i ]->setIsSelect	   ( true  );
			m_aBtnPageNums [ i ]->setIsTouchEnable ( false );
		}
		else
		{
			m_aBtnPageNums [ i ]->setIsSelect	   ( false );
			m_aBtnPageNums [ i ]->setIsTouchEnable ( true  );
		}
	}

	// page left & right
	m_pBtnPageLeft ->setPoint ( Point ( tStartNum.x - 67, tStartNum.y - 5 ) );
	m_pBtnPageRight->setPoint ( Point ( tLastNum .x + 36, tLastNum .y - 5 ) );

	if ( m_nViewFirstPageIndex > 0 )	{ m_pBtnPageLeft->setVisible ( true  );	m_pBtnPageLeft->setIsTouchEnable ( true  ); }
	else								{ m_pBtnPageLeft->setVisible ( false );	m_pBtnPageLeft->setIsTouchEnable ( false );	}

	if ( m_nViewFirstPageIndex + (KDint) m_aBtnPageNums.size ( ) < m_nPageMax )	{ m_pBtnPageRight->setVisible ( true  ); m_pBtnPageRight->setIsTouchEnable ( true  ); }
	else																		{ m_pBtnPageRight->setVisible ( false ); m_pBtnPageRight->setIsTouchEnable ( false ); }

	// drag page left & right
	if ( m_pUnitDrag == nullptr )
	{
		m_pBtnPageDragLeft ->setVisible ( false );
		m_pBtnPageDragRight->setVisible ( false );
		m_pBtnPageDragLeft ->setIsTouchEnable ( false );
		m_pBtnPageDragRight->setIsTouchEnable ( false );
	}
	else
	{
		if ( cMyData::getObj ( )->m_nMyRoomPage > 0 )
		{
			if ( m_pBtnPageDragLeft->getVisible ( ) == false )
			{
				m_pBtnPageDragLeft->setVisible	     ( true );
				m_pBtnPageDragLeft->setIsTouchEnable ( true );
			}
		}
		else
		{
			m_pBtnPageDragLeft->setVisible	     ( false );
			m_pBtnPageDragLeft->setIsTouchEnable ( false );
		}
		if (cMyData::getObj ( )->m_nMyRoomPage + 1 < m_nPageMax )
		{
			if ( m_pBtnPageDragRight->getVisible ( ) == false )
			{
				m_pBtnPageDragRight->setVisible		  ( true );
				m_pBtnPageDragRight->setIsTouchEnable ( true );
			}
		}
		else
		{
			m_pBtnPageDragRight->setVisible		  ( false );
			m_pBtnPageDragRight->setIsTouchEnable ( false );
		}
	}
}

KDvoid MyRoomNode::setUnitLevelUp ( cUnit* pUnit, KDbool bIsEffect )
{
	// check
	if ( pUnit == nullptr || pUnit->getIsParaMax ( ) == true )
	{
		return;
	}

	// para
	KDint	nHp	 = _MYROOM_LEVELUP_POINT_HP_MIN  + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_HP_MAX -_MYROOM_LEVELUP_POINT_HP_MIN  );
	KDint	nAtk = _MYROOM_LEVELUP_POINT_ATK_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_ATK_MAX-_MYROOM_LEVELUP_POINT_ATK_MIN );
	KDint	nDef = _MYROOM_LEVELUP_POINT_DEF_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_DEF_MAX-_MYROOM_LEVELUP_POINT_DEF_MIN );
	KDint	nAgl = _MYROOM_LEVELUP_POINT_AGL_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_AGL_MAX-_MYROOM_LEVELUP_POINT_AGL_MIN );
	KDint	nCrt = _MYROOM_LEVELUP_POINT_CRT_MIN + kdRand ( ) % ( _MYROOM_LEVELUP_POINT_CRT_MAX-_MYROOM_LEVELUP_POINT_CRT_MIN );
	pUnit->paraUp ( nHp, nAtk, nDef, nAgl, nCrt );

	if ( bIsEffect == true )
	{
		// point
		Point	tCenter  = pUnit->getPoint ( ) + Point ( 0, ( pUnit->getNorFirstSize ( ).height / 2 ) );
		Point	tSpecial = pUnit->getPoint ( ) + pUnit->getPointSpecial ( );

		Point	tPopup = Point ( _SCR_W - 150, 270 );

		for ( KDint i = 0; i < 200; i++ )
		{
			KDbool		bSame = false;

			for ( auto pNode : m_aLvUpPopups )
			{
				if ( pNode->getPoint ( ).y == tPopup.y )
				{
					bSame = true;
					break;
				}
			}
			
			if ( bSame == false )
			{
				break;
			}

			tPopup.y -= 26;
		}

		const KDchar*	pStr = ccszf ( "%s LEVEL UP", pUnit->getName ( ).c_str ( ) );
		m_aLvUpPopups.push_back ( new cLevelPopup ( this, LAYER_MYROOM_LEVEL_UP_POPUP, m_pLabels, pStr, tPopup ) );

		m_aLvUpEffects.push_back ( new cMyRoomLvlUpEffect ( this, LAYER_MYROOM_UI_3, tCenter, tSpecial, pUnit->getSlotIndex ( ), nHp, nAtk, nDef, nAgl, nCrt ) );

		if ( pUnit == m_pUnitSelect )
		{
			if ( pUnit->getAniType ( ) == eUnitAniType_Normal )
			{
				KDbool		bVisible = pUnit->getVisible ( );
				 
				pUnit->changeAni ( eUnitAniType_Attack );
				pUnit->aniStart ( );
				pUnit->setVisible ( bVisible );
				m_dSelectUnitAniLeftTime = 5;
			}
		}
		else
		{
			KDbool			bVisible = pUnit->getVisible ( );
			
			pUnit->changeAni ( eUnitAniType_Attack );
			pUnit->aniStart ( );
			pUnit->setVisible ( bVisible );
		}
	}
}

KDvoid MyRoomNode::touchBeganMachineMode ( Touch* pTouch )
{
	// check
	if ( m_nCapsuleModeStep == false || m_nCapsuleModeStep != 1 || m_bIsTouchMachineTouch == true )
	{
		return;
	}

	// init
	Rect	tRectCheck = Rect ( m_tMachine.x - m_fScrollMachine, m_tMachine.y, 99, 200 );
	m_nSelectMachine = -1;

	// loop
	for ( KDuint i = 0; i < m_aMachines.size ( ); i++ )
	{
		//check
		if ( RectInPoint ( tRectCheck, pTouch->getLocation ( ) ) == true )
		{
			m_aMachines [ i ]->setVisibleSelect ( true );
			m_nSelectMachine = i;
			break;
		}

		// point
		tRectCheck.origin.x += m_nViewMachineGap;
	}

	// set drag touch info
	m_bIsTouchMachineTouch	 = true;
	m_bIsTouchMachineDrag	 = false;
	m_tMachineTouchStartInfo = *pTouch;
}

KDvoid MyRoomNode::touchMovedMachineMode ( Touch* pTouch )
{
	// touch check
	if ( m_bIsTouchMachineTouch == false || (m_bIsTouchMachineTouch == true && pTouch->getID ( ) != m_tMachineTouchStartInfo.getID ( ) ) )
	{
		return;
	}

	// mode check
	if ( m_nCapsuleModeStep == false || m_nCapsuleModeStep != 1 )
	{
		m_bIsTouchMachineTouch = false;
		m_bIsTouchMachineDrag  = false;
		return;
	}

	if ( m_bIsTouchMachineDrag == true )
	{
		// scroll
		this->setMachineScroll ( m_fScrollMachine - ( pTouch->getLocation ( ).x - m_tMachineTouchStartInfo.getLocation ( ).x ) );

		// machine drag cursor
		if ( m_fScrollMachine > 0 )	
		{
			m_pMachineDragCursor->get ( )->setOpacity ( 0 );
		}

		// touch info
		m_tMachineTouchStartInfo = *pTouch;
	}
	else
	{
		if ( pTouch->getLocation ( ).x - m_tMachineTouchStartInfo.getLocation ( ).x > 15 || pTouch->getLocation ( ).x - m_tMachineTouchStartInfo.getLocation ( ).x < -15 )
		{
			// scroll
			this->setMachineScroll ( m_fScrollMachine - ( pTouch->getLocation ( ).x - m_tMachineTouchStartInfo.getLocation ( ).x ) );

			// machine drag cursor
			if ( m_fScrollMachine > 0 )	
			{
				m_pMachineDragCursor->get ( )->setOpacity ( 0 );
			}

			// touch info
			m_bIsTouchMachineDrag	 = true;
			m_tMachineTouchStartInfo = *pTouch;

			// select machine
			if ( m_nSelectMachine >= 0 && m_nSelectMachine < (KDint) m_aMachines.size ( ) )
			{
				m_aMachines [ m_nSelectMachine ]->setVisibleSelect ( false );
			}
			m_nSelectMachine = -1;
		}
	}
}

KDvoid MyRoomNode::touchEndedMachineMode ( Touch* pTouch )
{
	// touch check
	if ( m_bIsTouchMachineTouch == false || (m_bIsTouchMachineTouch == true && pTouch->getID ( ) != m_tMachineTouchStartInfo.getID ( ) ) )
	{
		return;
	}

	// mode check
	if ( m_nCapsuleModeStep == false || m_nCapsuleModeStep != 1 )
	{
		m_bIsTouchMachineTouch = false;
		m_bIsTouchMachineDrag  = false;
		return;
	}

	// switch
	m_bIsTouchMachineTouch = false;
	m_bIsTouchMachineDrag  = false;

	// select machine
	if ( m_nSelectMachine >= 0 && m_nSelectMachine < (KDint) m_aMachines.size ( ) )
	{
		// machine drag cursor
		m_pMachineDragCursor->get ( )->setVisible ( false );

		// machine
		m_aMachines [ m_nSelectMachine ]->setVisibleSelect   ( false );
		m_aMachines [ m_nSelectMachine ]->setVisibleUnitList ( true, true );

		// scroll
		m_fScrollMachineBackup = m_fScrollMachine;

		// sprite touch me
		m_pMachineTouchMe->get ( )->setVisible ( false );

		// step
		m_nCapsuleModeStep	   = 2;
		m_dCapsuleModeLeftTime = 0.2f;

		// sound
		if ( cMyData::getObj ( )->m_bIsOptionSound != false )
		{
			SimpleAudioEngine::getInstance ( )->playEffect ( "button_select.mp3" );
		}
	}
	else
	{
		m_nSelectMachine = -1;
	}
}

KDvoid MyRoomNode::setMachineScroll ( KDfloat fScroll )
{
	// init
	KDint		nScrollMax = ( m_aMachines.size ( ) * m_nViewMachineGap ) - _SCR_W;

	// check
	if ( fScroll <			0 )		fScroll = 0;
	if ( fScroll > nScrollMax )		fScroll = nScrollMax;

	// set
	m_fScrollMachine = fScroll;
}

KDvoid MyRoomNode::buyCapsule ( KDint nSelectMachine )
{
	m_nSelectMachine = nSelectMachine;

	// step
	m_nCapsuleModeStep			= 6;
	m_nCapsuleModeStep2			= 0;
	m_dCapsuleModeLeftTime		= 0.3f;
	m_nMachineHandleRotateCount	= 0;
	m_bIsCapsuleTouch			= false;

	// machine coin
	m_pUIMachineCoin->get ( )->setVisible ( true );
	m_pUIMachineCoin->get ( )->setPosition ( Point ( 403, 130 ) );
	m_pUIMachineCoin->get ( )->setTextureRect ( Rect ( 0, 0, 10, 14 ) );

	// sprite capsule
	m_pSprCapsule1->get ( )->setVisible ( true );
	m_pSprCapsule2->get ( )->setVisible ( true );
	m_pSprCapsule1->get ( )->setScale ( 0.2f );
	m_pSprCapsule2->get ( )->setScale ( 0.2f );
	m_pSprCapsule1->get ( )->setPosition ( Point ( 331.5f, 140 ) );
	m_pSprCapsule2->get ( )->setPosition ( Point ( 331.5f, 140 ) );
	m_pSprCapsule3->get ( )->setVisible ( false );
	m_pSprCapsule4->get ( )->setVisible ( false );

	// btn
	m_pBtnBack ->setIsTouchEnable ( false );
	m_pBtnShop2->setIsTouchEnable ( false );
	m_pBtnMachineLeft  ->setEnabled ( false );
	m_pBtnMachineRight ->setEnabled ( false );
	m_pBtnMachineHandle->setEnabled ( false );

	// ?��?ΩΩ?�‘¿�??�£¥�?��?
	KDint	nSlot = 0;
	for ( nSlot = 0; nSlot < 2000; nSlot++ )
	{
		KDbool	bIsSame = false;

		for ( auto pNode : m_aUnits )
		{
			if ( pNode->getSlotIndex ( ) == nSlot )
			{
				bIsSame = true;
				break;
			}
		}

		if ( bIsSame == false )
		{
			break;
		}
	}

	// ¿Ø¥÷¿ª ªÃ¥¬¥Ÿ.
	const cUnitCreateInfo*		pCreateInfo = m_aMachines [ m_nSelectMachine ]->getUnitInfoRand ( );

	// ¿Ø¥÷ ¿ŒΩ?�≈œ?��?ª?º??
	cUnit*	pUnit = new cUnit ( this, pCreateInfo, m_aMachines [ m_nSelectMachine ]->getCreateInfoFileType ( ), LAYER_MYROOM_MOB, cMyData::getObj ( )->getNewMyRoomUnitID ( ), pCreateInfo->nCreateIndex, -1 );

	// ¿?¿Â«?�¥�?
	cMyRoomUnitSaveData			tSaveData;

	tSaveData.nCreateInfoFileType	= m_aMachines [ m_nSelectMachine ]->getCreateInfoFileType ( );
	tSaveData.nUnitID				= pUnit->getUnitID ( );
	tSaveData.nCreateIndex			= pUnit->getCreateIndex ( );
	tSaveData.nSlotIndex			= nSlot;
	tSaveData.aHp  [ 0 ]			= pUnit->getHP	   ( );
	tSaveData.aHp  [ 1 ]			= pUnit->getHPMax  ( );
	tSaveData.aAtk [ 0 ]			= pUnit->getATK	   ( );
	tSaveData.aAtk [ 1 ]			= pUnit->getATKMax ( );
	tSaveData.aDef [ 0 ]			= pUnit->getDEF	   ( );
	tSaveData.aDef [ 1 ]			= pUnit->getDEFMax ( );
	tSaveData.aAgl [ 0 ]			= pUnit->getAGL	   ( );
	tSaveData.aAgl [ 1 ]			= pUnit->getAGLMax ( );
	tSaveData.aCrt [ 0 ]			= pUnit->getCRT	   ( );
	tSaveData.aCrt [ 1 ]			= pUnit->getCRTMax ( );
	tSaveData.fExp					= 0;
	tSaveData.fClassExp				= 0;
	tSaveData.fFriendshipExp		= kdRand ( ) % FRIENDSHIP_BEST + 30;
	tSaveData.nKillCount			= 0;

	// unit load
	pUnit->load ( (eCreateInfoFileType) tSaveData.nCreateInfoFileType, tSaveData.nSlotIndex, tSaveData.aHp [ 0 ], tSaveData.aAtk [ 0 ], tSaveData.aDef [ 0 ], tSaveData.aAgl [ 0 ], tSaveData.aCrt [ 0 ], tSaveData.aHp [ 1 ], tSaveData.aAtk [ 1 ], tSaveData.aDef [ 1 ], tSaveData.aAgl [ 1 ], tSaveData.aCrt [ 1 ], tSaveData.fExp, tSaveData.fClassExp, tSaveData.fFriendshipExp, tSaveData.nKillCount );

	// texture load
	pUnit->texLoad ( eUnitAniType_Name_a );
	pUnit->texLoad ( eUnitAniType_Normal );
	pUnit->texLoad ( eUnitAniType_Attack );
	pUnit->texLoad ( eUnitAniType_Damage );

	// change ani
	pUnit->changeAni ( eUnitAniType_Normal );

	// point
	pUnit->setPointNameA ( Point ( 13, 318 ) );

	// stop & visible
	pUnit->aniStop ( );
	pUnit->setVisible ( false );
	pUnit->setVisibleNameA ( false );

	// save
	cMyData::getObj ( )->addMyRoomUnitSaveDatas ( tSaveData );

	// unit push_back
	m_aUnits.push_back ( pUnit );

	// select unit
	m_pUnitSelect = pUnit;
}

