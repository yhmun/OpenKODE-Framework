/* -----------------------------------------------------------------------------------
 *
 *      File            GameTime.cpp
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
#include "GameTime.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Label/LabelAtlasCostom.h"

cGameTime::cGameTime ( JBaseScene* pScene, KDdouble dTime )
: m_pScene				( pScene		)
, m_pSprBack			( nullptr		)
, m_pSprBackGray		( nullptr		)
, m_pSprIce				( nullptr		)
, m_pSprNumAdd			( nullptr		)
, m_pLabel				( nullptr		)
, m_dTime				( dTime			)
, m_nLookTime			( (KDint) dTime )
, m_bIsPalpitate		( false			)
, m_dPalpitateLeftTime	( 0				)
, m_bIsTimeStop			( false			)
{
	// sprite
	m_pSprBack		= new cSprite ( "time_back .png"	 , eLayerGame_Time	  , Point ( 350, 278 ) );
	m_pSprBackGray	= new cSprite ( "time_back_gray .png", eLayerGame_Time	  , Point ( 350, 278 ) );
	m_pSprIce		= new cSprite ( "time_mode_ice.png"	 , eLayerGame_Time_Ice, Point ( 335, 263 ) );
	m_pSprNumAdd	= new cSprite ( "score_add_num.png"	 , eLayerGame_Time_Add, Point (   0,   0 ) );
	
	m_pSprBackGray->setIsVisible ( false );
	m_pSprIce	  ->setIsVisible ( false );
	m_pSprNumAdd  ->setIsVisible ( false );
	
	pScene->addDrawObject ( m_pSprBack		);
	pScene->addDrawObject ( m_pSprBackGray	);
	pScene->addDrawObject ( m_pSprIce		);
	
	// label
	m_pLabel = new cLabelAtlasCostom ( "time_num.png", 24, eLayerGame_Time, Point ( 442, 303 ) );
	m_pLabel->addCharInfo ( '1',   0, 15, 15 );	m_pLabel->addCharInfo ( '2',  17, 20, 20 );	m_pLabel->addCharInfo ( '3',  39, 20, 20 );
	m_pLabel->addCharInfo ( '4',  61, 20, 20 );	m_pLabel->addCharInfo ( '5',  83, 20, 20 );	m_pLabel->addCharInfo ( '6', 105, 20, 20 );
	m_pLabel->addCharInfo ( '7', 127, 20, 20 );	m_pLabel->addCharInfo ( '8', 149, 20, 20 );	m_pLabel->addCharInfo ( '9', 171, 20, 20 );
	m_pLabel->addCharInfo ( '0', 193, 20, 20 );	m_pLabel->addCharInfo ( '.', 215, 20, 20 );
	
	m_pLabel->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_pLabel->setString	( String::createWithFormat ( "%d", (KDint) m_dTime )->getCString ( ) );
	
	pScene->addDrawObject ( m_pLabel );
}

cGameTime::~cGameTime ( KDvoid )
{
	// sprite
	m_pSprBack		= nullptr;
	m_pSprBackGray	= nullptr;
	m_pSprIce		= nullptr;
	m_pSprNumAdd	= nullptr;
	
	// label
	m_pLabel = nullptr;
	m_aLabelsAdd.clear ( );
}

KDvoid cGameTime::setIceVersion ( KDbool bIsIce )
{
	if ( bIsIce == true )
	{
		m_pSprBack	  ->setIsVisible ( false );
		m_pSprBackGray->setIsVisible ( true );
		m_pSprIce	  ->setIsVisible ( true );
		
		m_bIsTimeStop = true;
	}
	else
	{
		m_pSprBack	  ->setIsVisible ( true );
		m_pSprBackGray->setIsVisible ( false );
		m_pSprIce	  ->setIsVisible ( false );

		m_bIsTimeStop = false;
	}
}

KDvoid cGameTime::setTime ( KDdouble dTime )
{
	m_dTime = dTime;
	m_pLabel->setString ( String::createWithFormat ( "%d", (KDint) m_dTime )->getCString ( ) );
}

KDvoid cGameTime::setAddTime ( KDdouble dTime )
{
	// num add
	cLabelAtlasCostom*		pLabel = new cLabelAtlasCostom ( "score_add_num.png", 13, eLayerGame_Time_Add, Point ( 470, 300 ) );
	pLabel->addCharInfo ( '1',  0,  8,  9 );	pLabel->addCharInfo ( '2',  8, 10, 11 );	pLabel->addCharInfo ( '3', 18, 10, 11 );
	pLabel->addCharInfo ( '4', 28, 10, 11 );	pLabel->addCharInfo ( '5', 38, 10, 11 );	pLabel->addCharInfo ( '6', 48, 10, 11 );
	pLabel->addCharInfo ( '7', 58, 10, 11 );	pLabel->addCharInfo ( '8', 68, 11, 11 );	pLabel->addCharInfo ( '9', 79, 10, 11 );
	pLabel->addCharInfo ( '0', 89, 10, 11 );	pLabel->addCharInfo ( '+', 99, 10, 11 );
	
	pLabel->setAnchorPoint ( Point ( 1, 0 ) );
	pLabel->setString ( String::createWithFormat ( "+%d", (KDint) dTime )->getCString ( ) );
	
	m_pScene->addDrawObject ( pLabel );
	
	m_aLabelsAdd.push_back ( pLabel );
	
	// num
	m_dTime += dTime;
	m_pLabel->setString (  String::createWithFormat ( "%d", (KDint) m_dTime )->getCString ( ) );
}

KDvoid cGameTime::update ( KDdouble dLeftTime )
{
	// look time check
	if ( m_nLookTime != (KDint) m_dTime )
	{
		// look time
		m_nLookTime = (KDint) m_dTime;
		
		// palpitate
		if ( m_nLookTime < 10 )
		{
			m_bIsPalpitate = true;
			m_dPalpitateLeftTime = 0.5f;
		}
	}
	
	// left time
	if ( m_bIsTimeStop == false )
	{
		m_dTime -= dLeftTime;
		if ( m_dTime < 0 )
		{
			m_dTime = 0;
		}
	}
	
	// label string
	m_pLabel->setString ( String::createWithFormat ( "%d", (KDint) m_dTime )->getCString ( ) );

	// percent
	if ( m_bIsPalpitate == true )
	{
		// percent
		KDfloat		fPercent = ( 0.5f - m_dPalpitateLeftTime ) * 2;

		// scale
		if ( fPercent < 0.5f )	m_pLabel->setScale ( 1 + ( fPercent * 2 ) * 0.2f );
		else					m_pLabel->setScale ( 1.2f - ( ( fPercent - 0.5f ) * 2 ) * 0.2f );
		
		// left time
		m_dPalpitateLeftTime -= dLeftTime;
		
		// check
		if ( m_dPalpitateLeftTime < 0 )
		{
			// scale
			m_pLabel->setScale ( 1 );
			
			// switch
			m_bIsPalpitate = false;
		}
	}
	
	// add time
	updateAddTime ( dLeftTime );
}

KDvoid cGameTime::updateAddTime ( KDdouble dLeftTime )
{
	// add time
	std::vector<cLabelAtlasCostom*>::iterator	pNode = m_aLabelsAdd.begin ( );
	while ( pNode != m_aLabelsAdd.end ( ) )
	{
		// init
		cLabelAtlasCostom*	pLabel = *pNode;
		
		// point
		Point		tPoint = pLabel->getPoint ( );
		tPoint.y += dLeftTime * 10;
		pLabel->setPoint ( tPoint );
		
		// erase
		if(  tPoint.y > 305 )
		{
			pNode = m_aLabelsAdd.erase ( pNode );
			continue;
		}
		
		// opacity
		KDfloat		fOpacity = 255 * ( ( 305 - tPoint.y ) / 5 );
		pLabel->setOpacity ( fOpacity );
		
		// next
		++pNode;
	}
}
