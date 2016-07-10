/* -----------------------------------------------------------------------------------
 *
 *      File            TimeGetMoney.cpp
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
#include "TimeGetMoney.h"

static cTimeGetMoney*	l_pTimeGetMoney = nullptr;

cTimeGetMoney::cTimeGetMoney ( KDvoid )
: m_pLabels		( nullptr	)
, m_pLabel		( nullptr	)	
, m_dLeftTime	( 21		)
{
	l_pTimeGetMoney = this;
}

cTimeGetMoney::~cTimeGetMoney ( KDvoid )
{
	clear ( );
}

cTimeGetMoney* cTimeGetMoney::getObj ( KDvoid )
{
	if ( !l_pTimeGetMoney )
	{
		l_pTimeGetMoney = new cTimeGetMoney ( );
	}

	return l_pTimeGetMoney;
}

KDvoid cTimeGetMoney::clear ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pLabels );
	m_pLabel = nullptr;
}

KDvoid cTimeGetMoney::changeNode ( Node* pNode, KDint z, Rect tRectDest )
{
	CC_SAFE_RELEASE ( m_pLabels );

	KDint	nTime = m_dLeftTime;
	m_pLabels = new cLabels ( pNode );
	m_pLabel = m_pLabels->add ( "gaeul9.ttf", 12, TextHAlignment::RIGHT, tRectDest, ccszf ( "%d", nTime ), z );
}

KDbool cTimeGetMoney::update ( KDdouble dLeftTime, KDint64& rMoney )
{
	if ( m_pLabels == nullptr )
	{
		return false;
	}

	if ( rMoney >= 100 )
	{
		clear ( );
		return false;
	}

	m_dLeftTime -= dLeftTime;
	if ( m_dLeftTime < 0 )
	{
		m_dLeftTime = 21;

		KDint	nTime = m_dLeftTime;
		m_pLabels->setStr ( m_pLabel, ccszf ( "%d", nTime ) );

		if ( rMoney < 100 )
		{
			rMoney += (KDint64) 1;

			if ( rMoney == 100 )
			{
				clear ( );
			}

			return true;
		}
	}

	KDint	nTime = m_dLeftTime;
	m_pLabels->setStr ( m_pLabel, ccszf ( "%d", nTime ) );

	return false;
}

KDvoid cTimeGetMoney::setPoint ( Point tPoint )
{
	if ( m_pLabels == nullptr )
	{
		return;
	}

	m_pLabels->setPoint ( m_pLabel, tPoint );
}
