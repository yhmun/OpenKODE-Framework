/* -----------------------------------------------------------------------------------
 *
 *      File            CCProfiling.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCProfiling_h__
#define __CCProfiling_h__

#include <string>
#include <chrono>
#include "../ccConfig.h"
#include "../base/CCObject.h"
#include "../base/CCDictionary.h"

NS_CC_BEGIN

/**
 *	@addtogroup global
 *	@{
 */

class ProfilingTimer;

/** 
 *	Profiler
 *	cocos2d builtin profiler.
 *
 *	To use it, enable set the CC_ENABLE_PROFILERS=1 in the ccConfig.h file
 */

class CC_DLL Profiler : public Object
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~Profiler ( KDvoid );

		/**
		 *	display the timers
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				displayTimers ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				init ( KDvoid );

	public :

		/**
		 *	returns the singleton 
		 *	@js NA
		 *	@lua NA
		 */
		static Profiler*	getInstance ( KDvoid );

		/**
		 *	Creates and adds a new timer 
		 *	@js NA
		 *	@lua NA
		 */
		ProfilingTimer*		createAndAddTimerWithName ( const KDchar* pTimerName );

		/**
		 *	releases a timer 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				releaseTimer ( const KDchar* pTimerName );

		/** 
		 *	releases all timers 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				releaseAllTimers ( KDvoid );

		Dictionary*			m_pActiveTimers;
};

class ProfilingTimer : public Object
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		ProfilingTimer ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ProfilingTimer ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool				initWithName ( const KDchar* pTimerName );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		const KDchar*		description ( KDvoid ) const;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		inline const std::chrono::high_resolution_clock::time_point&	getStartTime ( KDvoid ) { return m_pStartTime; }

		/** 
		 *	resets the timer properties
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				reset ( KDvoid );

		std::string										m_sNameStr;
		std::chrono::high_resolution_clock::time_point	m_pStartTime;
		KDint32											m_nAverageTime1;
		KDint32											m_nAverageTime2;
		KDint32											m_nMinTime;
		KDint32											m_nMaxTime;
		KDint32											m_nTotalTime;
		KDint32											m_nNumberOfCalls;
};

extern KDvoid	ProfilingBeginTimingBlock	( const KDchar* pTimerName );
extern KDvoid	ProfilingEndTimingBlock		( const KDchar* pTimerName );
extern KDvoid	ProfilingResetTimingBlock	( const KDchar* pTimerName );

/**
 *	cocos2d profiling categories
 *	used to enable / disable profilers with granularity
 */

extern KDbool	kProfilerCategorySprite;
extern KDbool	kProfilerCategoryBatchSprite;
extern KDbool	kProfilerCategoryParticles;

//	end of global group
/// @}

NS_CC_END

#endif	// __CCProfiling_h__
