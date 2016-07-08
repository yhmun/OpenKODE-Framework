/* -----------------------------------------------------------------------------------
 *
 *      File            CCProfiling.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Stuart Carnie
 *
 *         http://www.cocos2d-x.org      
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
#include "support/CCProfiling.h"

NS_CC_BEGIN

/* set to NO the categories that you don't want to profile */
KDbool kCCProfilerCategorySprite		= KD_FALSE;
KDbool kCCProfilerCategoryBatchSprite	= KD_FALSE;
KDbool kCCProfilerCategoryParticles		= KD_FALSE;


static CCProfiler*	l_pSharedProfiler = KD_NULL;

CCProfiler* CCProfiler::sharedProfiler ( KDvoid )
{
	if ( !l_pSharedProfiler )
	{
		l_pSharedProfiler = new CCProfiler ( );
		l_pSharedProfiler->init();
	}

	return l_pSharedProfiler;
}

CCProfilingTimer* CCProfiler::createAndAddTimerWithName ( const KDchar* szTimerName )
{
	CCProfilingTimer*	pRet = new CCProfilingTimer ( );
	pRet->initWithName ( szTimerName );
	m_pActiveTimers->setObject ( pRet, szTimerName );
	pRet->release ( );

	return pRet;
}

KDvoid CCProfiler::releaseTimer ( const KDchar* szTimerName )
{
	m_pActiveTimers->removeObjectForKey ( szTimerName );
}

KDvoid CCProfiler::releaseAllTimers ( KDvoid )
{
	m_pActiveTimers->removeAllObjects ( );
}

KDbool CCProfiler::init ( KDvoid )
{
	m_pActiveTimers = new CCDictionary();
	return KD_TRUE;
}

CCProfiler::~CCProfiler ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pActiveTimers );
}

KDvoid CCProfiler::displayTimers ( KDvoid )
{
	CCDictElement*	pElement = KD_NULL;
	CCDICT_FOREACH ( m_pActiveTimers, pElement )
	{
		CCProfilingTimer*	pTimer = (CCProfilingTimer*) pElement->getObject ( );
		CCLOG ( "%s", pTimer->description ( ) );
	}
}

// implementation of CCProfilingTimer

KDbool CCProfilingTimer::initWithName ( const KDchar* szTimerName )
{
	m_sNameStr = szTimerName;
	return KD_TRUE;
}

CCProfilingTimer::CCProfilingTimer ( KDvoid )
{
	m_nNumberOfCalls	= 0;
	m_nAverageTime1		= 0;
	m_nAverageTime2		= 0;
	m_nTotalTime		= 0;
	m_nMinTime			= 100000000;
	m_nMaxTime			= 0;
}

CCProfilingTimer::~CCProfilingTimer ( KDvoid )
{

}

const KDchar* CCProfilingTimer::description ( KDvoid )
{
	static KDchar	s_szDesciption [ 512 ] = { 0 };

	kdSprintf ( s_szDesciption, "%s ::\tavg1: %d,\tavg2: %d,\tmin: %d,\tmax: %d,\ttotal: %.2fs,\tnr calls: %d", m_sNameStr.c_str(), m_nAverageTime1, m_nAverageTime2, m_nMinTime, m_nMaxTime, m_nTotalTime / 1000000., m_nNumberOfCalls );
	return s_szDesciption;
}

KDvoid CCProfilingTimer::reset ( KDvoid )
{
	m_nNumberOfCalls	= 0;
	m_nAverageTime1		= 0;
	m_nAverageTime2		= 0;
	m_nTotalTime		= 0;
	m_nMinTime			= 100000000;
	m_nMaxTime			= 0;
	kdGettimeofday ( (struct KDTimeval*) &m_tStartTime, KD_NULL );
}

KDvoid CCProfilingBeginTimingBlock ( const KDchar* szTimerName )
{
	CCProfiler*			pProfiler = CCProfiler::sharedProfiler ( );
	CCProfilingTimer*	pTimer = (CCProfilingTimer*) pProfiler->m_pActiveTimers->objectForKey ( szTimerName );
	if ( !pTimer )
	{
		pTimer = pProfiler->createAndAddTimerWithName ( szTimerName );
	}

	// must the be last thing to execute
	pTimer->m_nNumberOfCalls++;
	kdGettimeofday ( (struct KDTimeval*) &pTimer->m_tStartTime, KD_NULL );
}

KDvoid CCProfilingEndTimingBlock ( const KDchar* szTimerName )
{
	// must the be 1st thing to execute
	struct KDTimeval	tNow;
	kdGettimeofday ( &tNow, KD_NULL );

	CCProfiler*			pProfiler = CCProfiler::sharedProfiler ( );
	CCProfilingTimer*	pTimer = (CCProfilingTimer*) pProfiler->m_pActiveTimers->objectForKey ( szTimerName );

	CCAssert ( pTimer, "CCProfilingTimer  not found" );

	KDint	nDuration = 1000000 * ( tNow.tv_sec - pTimer->m_tStartTime.tv_sec ) + ( tNow.tv_usec - pTimer->m_tStartTime.tv_usec );

	pTimer->m_nTotalTime += nDuration;
	pTimer->m_nAverageTime1 = ( pTimer->m_nAverageTime1 + nDuration ) / 2;
	pTimer->m_nAverageTime2 = (KDint) pTimer->m_nTotalTime / pTimer->m_nNumberOfCalls;
	pTimer->m_nMaxTime = KD_MAX ( pTimer->m_nMaxTime, nDuration );
	pTimer->m_nMinTime = KD_MIN ( pTimer->m_nMinTime, nDuration );
}

KDvoid CCProfilingResetTimingBlock ( const KDchar* szTimerName )
{
	CCProfiler*			pProfiler = CCProfiler::sharedProfiler ( );
	CCProfilingTimer*	pTimer = (CCProfilingTimer*) pProfiler->m_pActiveTimers->objectForKey ( szTimerName );

	CCAssert ( pTimer, "CCProfilingTimer not found" );

	pTimer->reset();
}


NS_CC_END
