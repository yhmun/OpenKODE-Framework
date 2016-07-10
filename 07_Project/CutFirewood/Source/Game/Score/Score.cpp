/* -----------------------------------------------------------------------------------
 *
 *      File            Score.cpp
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
#include "Score.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Label/LabelAtlasCostom.h"

cScore::cScore ( JBaseScene* pScene )
: m_pLabel			( nullptr	)
, m_nScore			( 0			)
, m_bIsLookScoreStay	( false		)
{
	// sprite
	m_pBack = new cSprite ( "score_back.png", eLayerGame_Score, Point ( 11, 278 ) );
	
	pScene->addDrawObject ( m_pBack );
	
	// label
	m_pLabel = new cLabelAtlasCostom ( "score_num.png", 17, eLayerGame_Score, Point ( 180, 292 ) );
	m_pLabel->addCharInfo ( '1',   0, 11, 15 );	m_pLabel->addCharInfo ( '2',  11, 15, 15 );	m_pLabel->addCharInfo ( '3',  26, 15, 15 );
	m_pLabel->addCharInfo ( '4',  41, 15, 15 );	m_pLabel->addCharInfo ( '5',  56, 15, 15 );	m_pLabel->addCharInfo ( '6',  71, 15, 15 );
	m_pLabel->addCharInfo ( '7',  86, 15, 15 );	m_pLabel->addCharInfo ( '8', 101, 15, 15 );	m_pLabel->addCharInfo ( '9', 116, 15, 15 );
	m_pLabel->addCharInfo ( '0', 131, 15, 15 );
	
	m_pLabel->setAnchorPoint ( Point ( 1, 0 ) );
	m_pLabel->setString	( String::createWithFormat ( "%d", m_nScore )->getCString ( ) );
	
	pScene->addDrawObject ( m_pLabel );
}

cScore::~cScore ( KDvoid )
{
	// sprite
	m_pBack = nullptr;
	
	// label
	m_pLabel = nullptr;
}

KDint cScore::getLookScore ( KDvoid )
{
	return kdAtoi ( m_pLabel->getString ( ) );
}

KDvoid cScore::update ( KDdouble dLeftTime )
{
	// init	
	KDint	nLookScore = kdAtoi ( m_pLabel->getString ( ) );
	
	// look score edit
	if ( m_bIsLookScoreStay == false && nLookScore < m_nScore )
	{
		if ( m_nScore - nLookScore < 10 )
		{
			++nLookScore;
		}
		else
		{
			nLookScore += ( ( m_nScore - nLookScore ) / 5 ) + 1;
			if ( nLookScore > m_nScore )
			{
				nLookScore = m_nScore;
			}
		}
	}
	
	// set label score
	m_pLabel->setString ( String::createWithFormat ( "%d", nLookScore )->getCString ( ) );
}
