/* -----------------------------------------------------------------------------------
 *
 *      File            House.cpp
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
#include "House.h"
#include "Game/Commander/Commander.h"
#include "Object/Animation/Animations.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Label/LabelAtlasCostom.h"

cHouse::cHouse ( JBaseGameScene* pScene, KDint nStage, KDint nMaxLevel, Point tPoint )
: m_pScene					( pScene )
, m_nStage					( nStage )
, m_nLevel					( 0		 )
, m_bIsGray					( false	 )
, m_bIsThrobbingEffect		( false	 )
, m_nIsThrobbingScaleStep	( 0		 )
, m_fThrobbingScale			( 0		 )
, m_dThrobbingLeftTime		( 0		 )
, m_nFeetTall				( 0		 )
{
	// sprites
	for ( KDint i = 0; i < nMaxLevel; i++ )
	{
		cSprite*	pSprHouse	  = new cSprite ( ccszf ( "house_%d_%d.png"	    , m_nStage + 1, i + 1 ), eLayerGame_House, Point::ZERO );
		cSprite*	pSprHouseGray = new cSprite ( ccszf ( "house_%d_%d_gray.png", m_nStage + 1, i + 1 ), eLayerGame_House, Point::ZERO );
		
		pSprHouse	 ->setIsVisible ( false );
		pSprHouseGray->setIsVisible ( false );
		pSprHouse	 ->setPoint ( Point ( tPoint.x, tPoint.y ) );
		pSprHouseGray->setPoint ( Point ( tPoint.x, tPoint.y ) );
		pSprHouse	 ->setAnchorPoint ( Point ( 0.5f, 0 ) );
		pSprHouseGray->setAnchorPoint ( Point ( 0.5f, 0 ) );
		
		pScene->addDrawObject ( pSprHouse	  );
		pScene->addDrawObject ( pSprHouseGray );
		
		m_aSprHouses	.push_back ( pSprHouse	   );
		m_aSprHousesGray.push_back ( pSprHouseGray );
	}
	
	// label
	m_pLabelFeetTall = new cLabelAtlasCostom ( "feettall.png", 23, eLayerGame_FeetTall, Point::ZERO );
	m_pLabelFeetTall->addCharInfo ( '1',   0, 16, 16 );	m_pLabelFeetTall->addCharInfo ( '2',  16, 16, 16 );	m_pLabelFeetTall->addCharInfo ( '3',  32, 16, 16 );
	m_pLabelFeetTall->addCharInfo ( '4',  48, 16, 16 );	m_pLabelFeetTall->addCharInfo ( '5',  64, 16, 16 );	m_pLabelFeetTall->addCharInfo ( '6',  80, 16, 16 );
	m_pLabelFeetTall->addCharInfo ( '7',  96, 16, 16 );	m_pLabelFeetTall->addCharInfo ( '8', 112, 16, 16 );	m_pLabelFeetTall->addCharInfo ( '9', 128, 16, 16 );
	m_pLabelFeetTall->addCharInfo ( '0', 144, 16, 16 );	m_pLabelFeetTall->addCharInfo ( 'F', 160, 11, 11 );	m_pLabelFeetTall->addCharInfo ( 'e', 171, 11, 11 );
	m_pLabelFeetTall->addCharInfo ( 'e', 182, 11, 11 );	m_pLabelFeetTall->addCharInfo ( 't', 193, 11, 11 );	m_pLabelFeetTall->addCharInfo ( 'T', 204, 11, 11 );
	m_pLabelFeetTall->addCharInfo ( 'a', 215, 11, 11 );	m_pLabelFeetTall->addCharInfo ( 'l', 226, 11, 11 );	m_pLabelFeetTall->addCharInfo ( 'L', 237, 11, 11 );
	m_pLabelFeetTall->addCharInfo ( ' ', 248, 12, 12 );

	m_pLabelFeetTall->setIsVisible ( false );
	
	m_pScene->addDrawObject ( m_pLabelFeetTall );
	
	// mole
	m_pSprMoleBackup = new cSprite ( "molemotion_2.png", eLayerGame_Mole, Point::ZERO );
}

cHouse::~cHouse ( KDvoid )
{
	// sprite
	m_aSprHouses	.clear ( );
	m_aSprHousesGray.clear ( );
	m_aAniMoles		.clear ( );

	m_pSprMoleBackup = nullptr;
	
	// label
	m_pLabelFeetTall = nullptr;
}

KDvoid cHouse::view ( KDint nLevel, KDbool bIsEffect )
{
	m_nLevel = nLevel;
	
	// error check
	if ( m_nLevel < 0 )
	{
		CCAssert ( false, "" );
	}
	else if ( m_nLevel < getMaxLevel ( ) )
	{
		// sprite house
		for ( auto pSpr : m_aSprHouses		)	pSpr->setIsVisible ( false );
		for ( auto pSpr : m_aSprHousesGray	)	pSpr->setIsVisible ( false );
		if ( m_bIsGray == false )	m_aSprHouses	 [ m_nLevel ]->setIsVisible ( true );
		else						m_aSprHousesGray [ m_nLevel ]->setIsVisible ( true );
		
		// effect
		if ( bIsEffect == true )
		{
			// add cloud create
			for ( KDint y = 0; y < 6; y++ )
			{
				for ( KDint x = 0; x < 13; x++ )
				{
					Point	tPoint	  = Point (  54 + x * 30 - 5 + kdRand ( ) % 11, 130 + y * 30 - 5 + kdRand ( ) % 11 );
					Point	tPointAdd = Point ( -40 + kdRand ( ) % 81, -40 + kdRand ( ) % 81 );
					
					m_pScene->msgCloudAdd ( eLayerGame_HouseCloud, 0, tPoint, tPointAdd, 0.2f + kdRand ( ) % 400 * 0.001f, 1, 0.1f, 0, 50 + kdRand ( ) % 100, false );
				}			
			}
		}
		
		// label
		if ( m_nLevel == getMaxLevel ( ) - 1 )
		{
			m_nFeetTall = 0;
			setAddFeetTall ( );
		}
	}
	else
	{
		// sprite house
		for ( auto pSpr : m_aSprHouses      )	pSpr->setIsVisible ( false );
		for ( auto pSpr : m_aSprHousesGray	)	pSpr->setIsVisible ( false );
		if ( m_bIsGray == false )	m_aSprHouses	.back ( )->setIsVisible ( true );
		else						m_aSprHousesGray.back ( )->setIsVisible ( true );
		
		// effect
		if ( bIsEffect == true )
		{
			m_bIsThrobbingEffect	= true;
			m_nIsThrobbingScaleStep	= 0;
			m_fThrobbingScale		= 1;
			m_dThrobbingLeftTime	= 0.7f;
		}
	}
}

KDvoid cHouse::setGray ( KDbool bIsGray )
{
	m_bIsGray = bIsGray;

	if ( m_nLevel >= getMaxLevel ( ) )
	{
		if ( m_bIsGray == false )
		{
			m_aSprHouses	.back ( )->setIsVisible ( true  );
			m_aSprHousesGray.back ( )->setIsVisible ( false );
		}
		else
		{
			m_aSprHouses	.back ( )->setIsVisible ( false );
			m_aSprHousesGray.back ( )->setIsVisible ( true  );
		}
	}
	else
	{
		if ( m_bIsGray == false )
		{
			m_aSprHouses	 [ m_nLevel ]->setIsVisible ( true	);
			m_aSprHousesGray [ m_nLevel ]->setIsVisible ( false	);
		}
		else
		{
			m_aSprHouses	 [ m_nLevel ]->setIsVisible ( false	);
			m_aSprHousesGray [ m_nLevel ]->setIsVisible ( true	);
		}
	}
}

KDint cHouse::getMaxLevel ( KDvoid )
{
	return (KDint) m_aSprHouses.size ( );
}

KDvoid cHouse::setAddFeetTall ( KDvoid )
{
	// feet tall
	++m_nFeetTall;
	
	// label
	m_pLabelFeetTall->setString ( ccszf ( "%dFeetTalL", m_nFeetTall ) );
	m_pLabelFeetTall->setIsVisible ( true );
	m_pLabelFeetTall->setPoint ( Point ( _SCR_W_HARP - ( m_pLabelFeetTall->getSize ( ).width / 2 ), 250 ) );
}

KDvoid cHouse::update ( KDdouble dLeftTime )
{
	// mole
	for ( auto pAniMole : m_aAniMoles )
	{
		pAniMole->update ( dLeftTime );
	}
	
	// throbbing effect
	if ( m_bIsThrobbingEffect == true )
	{
		if ( m_nIsThrobbingScaleStep == 0 )
		{
			m_fThrobbingScale += dLeftTime / 4;
			if ( m_fThrobbingScale > 1.1f )
			{
				m_fThrobbingScale = 1.1f;
				++m_nIsThrobbingScaleStep;
			}
		}
		else if ( m_nIsThrobbingScaleStep == 1 )
		{
			m_fThrobbingScale -= dLeftTime / 4;
			if ( m_fThrobbingScale < 1 )
			{
				m_fThrobbingScale = 1;
				++m_nIsThrobbingScaleStep;
				setAddFeetTall ( );
			}
		}
		else
		{
			m_dThrobbingLeftTime -= dLeftTime;
			if ( m_dThrobbingLeftTime < 0 )
			{
				m_bIsThrobbingEffect = false;
			}
		}
		
		m_aSprHouses.back ( )->setScaleX ( m_fThrobbingScale );
	}
}
