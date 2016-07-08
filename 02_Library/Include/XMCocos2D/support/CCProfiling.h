/* -----------------------------------------------------------------------------------
 *
 *      File            CCProfiling.h
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

#ifndef __CCProfiling_h__
#define __CCProfiling_h__
    
#include "../cocoa/CCDictionary.h"
#include "../support/CCTime.h"

NS_CC_BEGIN
    
/**
 * @addtogroup global
 * @{
 */

class CCProfilingTimer;

/** 
 *	CCProfiler
 *	cocos2d builtin profiler.
 *
 *	To use it, enable set the CC_ENABLE_PROFILERS=1 in the ccConfig.h file
 *	@js NA
 *	@lua NA
 */

class CCProfiler : public CCObject
{
	public :

		virtual ~CCProfiler ( KDvoid );

		static CCProfiler*		sharedProfiler ( KDvoid );
		
	public :

		virtual KDbool			init ( KDvoid );

		/** display the timers */
		KDvoid					displayTimers ( KDvoid );
		
		/** Creates and adds a new timer */
		CCProfilingTimer*		createAndAddTimerWithName ( const KDchar* szTimerName );
		
		/** releases a timer */
		KDvoid					releaseTimer ( const KDchar* szTimerName );
		
		/** releases all timers */
		KDvoid					releaseAllTimers ( KDvoid );

	public :

		CCDictionary*			m_pActiveTimers;
};

/**
 *	@js NA
 *	@lua NA
 */
class CCProfilingTimer : public CCObject
{
	public :
				 CCProfilingTimer ( KDvoid );
		virtual ~CCProfilingTimer ( KDvoid );

	public :

		virtual KDbool			initWithName ( const KDchar* szTimerName );

		const KDchar*			description ( KDvoid );

		struct KDTimeval*		getStartTime ( KDvoid ) { return &m_tStartTime; };

		/** resets the timer properties */
		KDvoid					reset ( KDvoid );

	public :

		std::string				m_sNameStr;
		KDint					m_nNumberOfCalls;
		KDint					m_nAverageTime1;
		KDint					m_nAverageTime2;
		KDint64					m_nTotalTime;
		KDint					m_nMinTime;
		KDint					m_nMaxTime;
		struct KDTimeval		m_tStartTime;
};

extern KDvoid	CCProfilingBeginTimingBlock	( const KDchar* szTimerName );
extern KDvoid	CCProfilingEndTimingBlock	( const KDchar* szTimerName );
extern KDvoid	CCProfilingResetTimingBlock	( const KDchar* szTimerName );

/**
 *	cocos2d profiling categories
 *	used to enable / disable profilers with granularity
 */

extern KDbool	kCCProfilerCategorySprite;
extern KDbool	kCCProfilerCategoryBatchSprite;
extern KDbool	kCCProfilerCategoryParticles;

// end of global group
/// @}

NS_CC_END

#endif // __CCProfiling_h__
