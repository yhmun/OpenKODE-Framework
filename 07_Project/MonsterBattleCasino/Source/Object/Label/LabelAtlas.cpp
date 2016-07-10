/* -----------------------------------------------------------------------------------
 *
 *      File            LabelAtlas.cpp
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
#include "LabelAtlas.h"

//-----------------------------------------------------------
// Name : JINE(조진??
// Desc : 
//-----------------------------------------------------------
cLabelAtlas::cLabelAtlas ( Node* pNode, const KDchar* pStr, const KDchar* pCharMapFile, KDint nWidth, KDint nHeight, KDchar cStartChar, KDint z, Point tPoint )
: m_pNode			( pNode			)
, m_pLabel			( nullptr		)
, m_isMove			( false			)
, m_ptMoveBefore	( Point::ZERO	)
, m_ptMove			( Point::ZERO	)
, m_timeMoveFull	( 0				)
, m_timeMoveLeft	( 0				)
{
	m_pLabel = LabelAtlas::create ( pStr, pCharMapFile, nWidth, nHeight, cStartChar );
	m_pLabel->setPosition ( tPoint );
	m_pNode->addChild ( m_pLabel, z );
}

cLabelAtlas::~cLabelAtlas ( KDvoid )
{
	m_pNode->removeChild ( m_pLabel, true );		
	m_pLabel = nullptr;
}

KDvoid cLabelAtlas::setVisible ( KDbool bIsVisible )
{
	m_pLabel->setVisible ( bIsVisible );
}

KDvoid cLabelAtlas::setStr ( const KDchar* pStr )
{
	m_pLabel->setString ( pStr );
}

KDvoid cLabelAtlas::setPoint ( Point tPoint )
{
	m_pLabel->setPosition ( tPoint );
}

KDvoid cLabelAtlas::setPointTime ( Point tPoint, KDdouble dLeftTime )
{
	m_isMove		= true;
	m_ptMove		= tPoint - m_pLabel->getPosition ( );
	m_ptMoveBefore	= m_pLabel->getPosition ( );
	m_timeMoveFull	= dLeftTime;
	m_timeMoveLeft	= dLeftTime;
}

KDvoid cLabelAtlas::update ( KDdouble dLeftTime )
{
	if ( m_isMove == true )
	{
		m_timeMoveLeft -= dLeftTime;

		if ( m_timeMoveLeft < 0 )
		{
			Point	tPoint = Point ( m_ptMoveBefore.x + m_ptMove.x, m_ptMoveBefore.y + m_ptMove.y );
			m_pLabel->setPosition ( tPoint );
			m_isMove = false;
		}
		else
		{
			Point	tPoint = Point ( m_ptMoveBefore.x + ( m_timeMoveFull - m_timeMoveLeft ) * m_ptMove.x / m_timeMoveFull,
									 m_ptMoveBefore.y + ( m_timeMoveFull - m_timeMoveLeft ) * m_ptMove.y / m_timeMoveFull );

			m_pLabel->setPosition ( tPoint );
		}
	}
}
