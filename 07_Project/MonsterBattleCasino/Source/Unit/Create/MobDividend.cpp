/* -----------------------------------------------------------------------------------
 *
 *      File            MobDividend.cpp
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
#include "MobDividend.h"

cMobDividend::cMobDividend ( const std::vector<cUnit*>& aMobs )
{
	KDfloat					fAverage		= 0;
	KDfloat					fTotalParaSum	= 0;
	std::vector<KDfloat>	aTotalParas;

	// total paras & average
	for ( auto pMob : aMobs )
	{
		KDfloat		fTotalPara = 0;

		fTotalPara += pMob->getHP  ( ) * _CAPABLE_POINT_HP_MULT;
		fTotalPara += pMob->getATK ( ) * _CAPABLE_POINT_ATK_MULT;
		fTotalPara += pMob->getDEF ( ) * _CAPABLE_POINT_DEF_MULT;
		fTotalPara += pMob->getAGL ( ) * _CAPABLE_POINT_AGL_MULT;
		fTotalPara += pMob->getCRT ( ) * _CAPABLE_POINT_CRT_MULT;
		fTotalPara += pMob->getCasinoAdvantage ( );

		fAverage += fTotalPara;

		aTotalParas.push_back ( fTotalPara );
	}

	fAverage /= aMobs.size ( );

	// dividend magnifications
	for ( auto fTotalParas : aTotalParas )
	{
		KDint		nAverageGap = fTotalParas - fAverage;
		KDfloat		fMagnifications = 4;

		if ( nAverageGap < 0 )
		{
			for ( KDint i = nAverageGap; i < 0; i++ )
			{
					 if ( fMagnifications < 5 )		fMagnifications += 0.1f;
				else if ( fMagnifications < 6 )		fMagnifications += 0.2f;
				else								fMagnifications += 0.3f;
			}
		}
		else
		{
			for ( KDint i = nAverageGap; i > 0; i-- )
			{
					 if ( fMagnifications >= 3 )	fMagnifications -= 0.1f;
				else if ( fMagnifications >= 2 )	fMagnifications -= 0.05f;
				else								fMagnifications -= 0.03f;
			}
		}

		if ( fMagnifications < 1.1f )
		{
			fMagnifications = 1.1f;
		}

		m_aDividendMagnifications.push_back ( fMagnifications );
	}

	// win percent
	for ( auto fTotalParas : aTotalParas )
	{
		KDint	nWinPercent = ( fTotalParas * 100 ) / fTotalParaSum;

		m_aWinPercents.push_back ( nWinPercent );
	}
}

cMobDividend::~cMobDividend ( KDvoid )
{
	m_aDividendMagnifications.clear ( );
	m_aWinPercents			 .clear ( );
}

KDfloat cMobDividend::getMagnification ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aDividendMagnifications.size ( ) )
	{
		return -1;
	}

	return m_aDividendMagnifications [ nIndex ];
}

KDint cMobDividend::getWinPercent ( KDint nIndex )
{
	if ( nIndex < 0 || nIndex >= (KDint) m_aWinPercents.size ( ) )
	{
		return -1;
	}
	
	return m_aWinPercents [ nIndex ];
}
