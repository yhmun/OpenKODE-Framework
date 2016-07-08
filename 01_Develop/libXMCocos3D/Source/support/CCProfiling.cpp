/* -----------------------------------------------------------------------------------
 *
 *      File            CCProfiling.cpp
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

#include "support/CCProfiling.h"

#include <chrono>

using namespace std;

NS_CC_BEGIN

//#pragma mark - Profiling Categories
/* set to false the categories that you don't want to profile */
bool kProfilerCategorySprite = false;
bool kProfilerCategoryBatchSprite = false;
bool kProfilerCategoryParticles = false;

static Profiler* g_sSharedProfiler = NULL;

Profiler* Profiler::getInstance()
{
    if (! g_sSharedProfiler)
    {
        g_sSharedProfiler = new Profiler();
        g_sSharedProfiler->init();
    }

    return g_sSharedProfiler;
}

ProfilingTimer* Profiler::createAndAddTimerWithName(const char* timerName)
{
    ProfilingTimer *t = new ProfilingTimer();
    t->initWithName(timerName);
    m_pActiveTimers->setObject(t, timerName);
    t->release();

    return t;
}

void Profiler::releaseTimer(const char* timerName)
{
    m_pActiveTimers->removeObjectForKey(timerName);
}

void Profiler::releaseAllTimers()
{
    m_pActiveTimers->removeAllObjects();
}

bool Profiler::init()
{
    m_pActiveTimers = new Dictionary();
    m_pActiveTimers->init();
    return true;
}

Profiler::~Profiler(void)
{
    CC_SAFE_RELEASE(m_pActiveTimers);
}

void Profiler::displayTimers()
{
    DictElement* pElement = NULL;
    CCDICT_FOREACH(m_pActiveTimers, pElement)
    {
        ProfilingTimer* timer = static_cast<ProfilingTimer*>(pElement->getObject());
        log("%s", timer->description());
    }
}

// implementation of ProfilingTimer

ProfilingTimer::ProfilingTimer()
: m_nAverageTime1(0)
, m_nAverageTime2(0)
, m_nMinTime(100000000)
, m_nMaxTime(0)
, m_nTotalTime(0)
, m_nNumberOfCalls(0)
{
}

bool ProfilingTimer::initWithName(const char* timerName)
{
    m_sNameStr = timerName;
    return true;
}

ProfilingTimer::~ProfilingTimer(void)
{
    
}

const char* ProfilingTimer::description() const
{
    static char s_desciption[512] = {0};

    kdSprintf(s_desciption, "%s ::\tavg1: %dµ,\tavg2: %dµ,\tmin: %dµ,\tmax: %dµ,\ttotal: %.2fs,\tnr calls: %d", m_sNameStr.c_str(), m_nAverageTime1, m_nAverageTime2, m_nMinTime, m_nMaxTime, m_nTotalTime/1000000., m_nNumberOfCalls);
    return s_desciption;
}

void ProfilingTimer::reset()
{
    m_nNumberOfCalls = 0;
    m_nAverageTime1 = 0;
    m_nAverageTime2 = 0;
    m_nTotalTime = 0;
    m_nMinTime = 100000000;
    m_nMaxTime = 0;
    m_pStartTime = chrono::high_resolution_clock::now();
}

void ProfilingBeginTimingBlock(const char *timerName)
{
    Profiler* p = Profiler::getInstance();
    ProfilingTimer* timer = static_cast<ProfilingTimer*>( p->m_pActiveTimers->objectForKey(timerName) );
    if( ! timer )
    {
        timer = p->createAndAddTimerWithName(timerName);
    }

    timer->m_nNumberOfCalls++;

    // should be the last instruction in order to be more reliable
    timer->m_pStartTime = chrono::high_resolution_clock::now();
}

void ProfilingEndTimingBlock(const char *timerName)
{
    // should be the 1st instruction in order to be more reliable
    auto now = chrono::high_resolution_clock::now();

    Profiler* p = Profiler::getInstance();
    ProfilingTimer* timer = (ProfilingTimer*)p->m_pActiveTimers->objectForKey(timerName);

    CCASSERT(timer, "CCProfilingTimer  not found");


    long duration = static_cast<long>(chrono::duration_cast<chrono::microseconds>(now - timer->m_pStartTime).count());

    timer->m_nTotalTime += duration;
    timer->m_nAverageTime1 = (timer->m_nAverageTime1 + duration) / 2.0f;
    timer->m_nAverageTime2 = timer->m_nTotalTime / timer->m_nNumberOfCalls;
    timer->m_nMaxTime = KD_MAX( timer->m_nMaxTime, duration);
    timer->m_nMinTime = KD_MIN( timer->m_nMinTime, duration);
}

void ProfilingResetTimingBlock(const char *timerName)
{
    Profiler* p = Profiler::getInstance();
    ProfilingTimer *timer = (ProfilingTimer*)p->m_pActiveTimers->objectForKey(timerName);

    CCASSERT(timer, "CCProfilingTimer not found");

    timer->reset();
}

NS_CC_END

