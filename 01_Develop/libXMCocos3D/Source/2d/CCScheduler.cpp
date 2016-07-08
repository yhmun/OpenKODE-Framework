/* -----------------------------------------------------------------------------------
 *
 *      File            CCScheduler.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#include "2d/CCScheduler.h"
#include "2d/CCDirector.h"
#include "support/data_support/utlist.h"
#include "support/data_support/ccCArray.h"
#include "base/CCArray.h"
#include "2d/script_support/CCScriptSupport.h"

using namespace std;

NS_CC_BEGIN

// data structures

// A list double-linked list used for "updates with priority"
typedef struct _listEntry
{
    struct _listEntry   *prev, *next;
    Object            *target;        // not retained (retained by hashUpdateEntry)
    int                 priority;
    bool                paused;
    bool                markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
} tListEntry;

typedef struct _hashUpdateEntry
{
    tListEntry          **list;        // Which list does it belong to ?
    tListEntry          *entry;        // entry in the list
    Object            *target;        // hash key (retained)
    UT_hash_handle      hh;
} tHashUpdateEntry;

// Hash Element used for "selectors with interval"
typedef struct _hashSelectorEntry
{
    ccArray             *timers;
    Object            *target;    // hash key (retained)
    int        timerIndex;
    Timer             *currentTimer;
    bool                currentTimerSalvaged;
    bool                paused;
    UT_hash_handle      hh;
} tHashTimerEntry;

// implementation Timer

Timer::Timer()
: m_pTarget(nullptr)
, m_fElapsed(-1)
, m_bRunForever(false)
, m_bUseDelay(false)
, m_uTimesExecuted(0)
, m_uRepeat(0)
, m_fDelay(0.0f)
, m_fInterval(0.0f)
, m_pSelector(nullptr)
, m_nScriptHandler(0)
{
}

Timer* Timer::create(Object *target, SEL_SCHEDULE selector)
{
    Timer *timer = new Timer();

    timer->initWithTarget(target, selector, 0.0f, kRepeatForever, 0.0f);
    timer->autorelease();

    return timer;
}

Timer* Timer::create(Object *target, SEL_SCHEDULE selector, float seconds)
{
    Timer *timer = new Timer();

    timer->initWithTarget(target, selector, seconds, kRepeatForever, 0.0f);
    timer->autorelease();

    return timer;
}

Timer* Timer::createWithScriptHandler(int handler, float seconds)
{
    Timer *timer = new Timer();

    timer->initWithScriptHandler(handler, seconds);
    timer->autorelease();

    return timer;
}

bool Timer::initWithScriptHandler(int handler, float seconds)
{
    m_nScriptHandler = handler;
    m_fElapsed = -1;
    m_fInterval = seconds;

    return true;
}

bool Timer::initWithTarget(Object *target, SEL_SCHEDULE selector)
{
    return initWithTarget(target, selector, 0, kRepeatForever, 0.0f);
}

bool Timer::initWithTarget(Object *target, SEL_SCHEDULE selector, float seconds, unsigned int repeat, float delay)
{
    m_pTarget = target;
    m_pSelector = selector;
    m_fElapsed = -1;
    m_fInterval = seconds;
    m_fDelay = delay;
    m_bUseDelay = (delay > 0.0f) ? true : false;
    m_uRepeat = repeat;
    m_bRunForever = (repeat == kRepeatForever) ? true : false;
    return true;
}

void Timer::update(float dt)
{
    if (m_fElapsed == -1)
    {
        m_fElapsed = 0;
        m_uTimesExecuted = 0;
    }
    else
    {
        if (m_bRunForever && !m_bUseDelay)
        {//standard timer usage
            m_fElapsed += dt;
            if (m_fElapsed >= m_fInterval)
            {
                if (m_pTarget && m_pSelector)
                {
                    (m_pTarget->*m_pSelector)(m_fElapsed);
                }

                if (0 != m_nScriptHandler)
                {
                    SchedulerScriptData data(m_nScriptHandler,m_fElapsed);
                    ScriptEvent event(kScheduleEvent,&data);
                    ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                }
                m_fElapsed = 0;
            }
        }    
        else
        {//advanced usage
            m_fElapsed += dt;
            if (m_bUseDelay)
            {
                if( m_fElapsed >= m_fDelay )
                {
                    if (m_pTarget && m_pSelector)
                    {
                        (m_pTarget->*m_pSelector)(m_fElapsed);
                    }

                    if (0 != m_nScriptHandler)
                    {
                        SchedulerScriptData data(m_nScriptHandler,m_fElapsed);
                        ScriptEvent event(kScheduleEvent,&data);
                        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                    }

                    m_fElapsed = m_fElapsed - m_fDelay;
                    m_uTimesExecuted += 1;
                    m_bUseDelay = false;
                }
            }
            else
            {
                if (m_fElapsed >= m_fInterval)
                {
                    if (m_pTarget && m_pSelector)
                    {
                        (m_pTarget->*m_pSelector)(m_fElapsed);
                    }

                    if (0 != m_nScriptHandler)
                    {
                        SchedulerScriptData data(m_nScriptHandler,m_fElapsed);
                        ScriptEvent event(kScheduleEvent,&data);
                        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                    }

                    m_fElapsed = 0;
                    m_uTimesExecuted += 1;

                }
            }

            if (!m_bRunForever && m_uTimesExecuted > m_uRepeat)
            {    //unschedule timer
                Director::getInstance()->getScheduler()->unscheduleSelector(m_pSelector, m_pTarget);
            }
        }
    }
}

float Timer::getInterval() const
{
    return m_fInterval;
}

void Timer::setInterval(float interval)
{
    m_fInterval = interval;
}

SEL_SCHEDULE Timer::getSelector() const
{
    return m_pSelector;
}

// implementation of Scheduler

// Priority level reserved for system services.
const int Scheduler::PRIORITY_SYSTEM = INT_MIN;

// Minimum priority level for user scheduling.
const int Scheduler::PRIORITY_NON_SYSTEM_MIN = PRIORITY_SYSTEM + 1;

Scheduler::Scheduler(void)
: m_fTimeScale(1.0f)
, m_pUpdatesNegList(nullptr)
, m_pUpdates0List(nullptr)
, m_pUpdatesPosList(nullptr)
, m_pHashForUpdates(nullptr)
, m_pHashForTimers(nullptr)
, m_pCurrentTarget(nullptr)
, m_bCurrentTargetSalvaged(false)
, m_bUpdateHashLocked(false)
, m_pScriptHandlerEntries(nullptr)
{

}

Scheduler::~Scheduler(void)
{
    unscheduleAll();
    CC_SAFE_RELEASE(m_pScriptHandlerEntries);
}

void Scheduler::removeHashElement(_hashSelectorEntry *element)
{

	cocos2d::Object *target = element->target;

    ccArrayFree(element->timers);
    HASH_DEL(m_pHashForTimers, element);
    free(element);

    // make sure the target is released after we have removed the hash element
    // otherwise we access invalid memory when the release call deletes the target
    // and the target calls removeAllSelectors() during its destructor
    target->release();

}

void Scheduler::scheduleSelector(SEL_SCHEDULE selector, Object *target, float interval, bool paused)
{
    this->scheduleSelector(selector, target, interval, kRepeatForever, 0.0f, paused);
}

void Scheduler::scheduleSelector(SEL_SCHEDULE selector, Object *target, float interval, unsigned int repeat, float delay, bool paused)
{
    CCASSERT(selector, "Argument selector must be non-nullptr");
    CCASSERT(target, "Argument target must be non-nullptr");

    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);

    if (! element)
    {
        element = (tHashTimerEntry *)calloc(sizeof(*element), 1);
        element->target = target;
        if (target)
        {
            target->retain();
        }
        HASH_ADD_PTR(m_pHashForTimers, target, element);

        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        element->paused = paused;
    }
    else
    {
        CCASSERT(element->paused == paused, "");
    }

    if (element->timers == nullptr)
    {
        element->timers = ccArrayNew(10);
    }
    else 
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            Timer *timer = (Timer*)element->timers->arr[i];

            if (selector == timer->getSelector())
            {
                CCLOG("CCScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
                timer->setInterval(interval);
                return;
            }        
        }
        ccArrayEnsureExtraCapacity(element->timers, 1);
    }

    Timer *pTimer = new Timer();
    pTimer->initWithTarget(target, selector, interval, repeat, delay);
    ccArrayAppendObject(element->timers, pTimer);
    pTimer->release();    
}

void Scheduler::unscheduleSelector(SEL_SCHEDULE selector, Object *target)
{
    // explicity handle nil arguments when removing an object
    if (target == 0 || selector == 0)
    {
        return;
    }

    //CCASSERT(target);
    //CCASSERT(selector);

    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);

    if (element)
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            Timer *timer = static_cast<Timer*>(element->timers->arr[i]);

            if (selector == timer->getSelector())
            {
                if (timer == element->currentTimer && (! element->currentTimerSalvaged))
                {
                    element->currentTimer->retain();
                    element->currentTimerSalvaged = true;
                }

                ccArrayRemoveObjectAtIndex(element->timers, i, true);

                // update timerIndex in case we are in tick:, looping over the actions
                if (element->timerIndex >= i)
                {
                    element->timerIndex--;
                }

                if (element->timers->num == 0)
                {
                    if (m_pCurrentTarget == element)
                    {
                        m_bCurrentTargetSalvaged = true;
                    }
                    else
                    {
                        removeHashElement(element);
                    }
                }

                return;
            }
        }
    }
}

void Scheduler::priorityIn(tListEntry **list, Object *target, int priority, bool paused)
{
    tListEntry *listElement = (tListEntry *)malloc(sizeof(*listElement));

    listElement->target = target;
    listElement->priority = priority;
    listElement->paused = paused;
    listElement->next = listElement->prev = nullptr;
    listElement->markedForDeletion = false;

    // empty list ?
    if (! *list)
    {
        DL_APPEND(*list, listElement);
    }
    else
    {
        bool added = false;

        for (tListEntry *element = *list; element; element = element->next)
        {
            if (priority < element->priority)
            {
                if (element == *list)
                {
                    DL_PREPEND(*list, listElement);
                }
                else
                {
                    listElement->next = element;
                    listElement->prev = element->prev;

                    element->prev->next = listElement;
                    element->prev = listElement;
                }

                added = true;
                break;
            }
        }

        // Not added? priority has the higher value. Append it.
        if (! added)
        {
            DL_APPEND(*list, listElement);
        }
    }

    // update hash entry for quick access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
    hashElement->target = target;
    target->retain();
    hashElement->list = list;
    hashElement->entry = listElement;
    HASH_ADD_PTR(m_pHashForUpdates, target, hashElement);
}

void Scheduler::appendIn(_listEntry **list, Object *target, bool paused)
{
    tListEntry *listElement = (tListEntry *)malloc(sizeof(*listElement));

    listElement->target = target;
    listElement->paused = paused;
    listElement->markedForDeletion = false;

    DL_APPEND(*list, listElement);

    // update hash entry for quicker access
    tHashUpdateEntry *pHashElement = (tHashUpdateEntry *)calloc(sizeof(*pHashElement), 1);
    pHashElement->target = target;
    target->retain();
    pHashElement->list = list;
    pHashElement->entry = listElement;
    HASH_ADD_PTR(m_pHashForUpdates, target, pHashElement);
}

void Scheduler::scheduleUpdateForTarget(Object *target, int priority, bool paused)
{

    tHashUpdateEntry *hashElement = nullptr;
    HASH_FIND_PTR(m_pHashForUpdates, &target, hashElement);
    if (hashElement)
    {
#if COCOS2D_DEBUG >= 1
        CCASSERT(hashElement->entry->markedForDeletion,"");
#endif
        // TODO: check if priority has changed!

        hashElement->entry->markedForDeletion = false;
        return;
    }

    // most of the updates are going to be 0, that's way there
    // is an special list for updates with priority 0
    if (priority == 0)
    {
        appendIn(&m_pUpdates0List, target, paused);
    }
    else if (priority < 0)
    {
        priorityIn(&m_pUpdatesNegList, target, priority, paused);
    }
    else
    {
        // priority > 0
        priorityIn(&m_pUpdatesPosList, target, priority, paused);
    }
}

bool Scheduler::isScheduledForTarget(SEL_SCHEDULE selector, Object *target)
{
    CCASSERT(selector, "Argument selector must be non-nullptr");
    CCASSERT(target, "Argument target must be non-nullptr");
    
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);
    
    if (!element)
    {
        return false;
    }
    
    if (element->timers == nullptr)
    {
        return false;
    }else
    {
        for (int i = 0; i < element->timers->num; ++i)
        {
            Timer *timer = (Timer*)element->timers->arr[i];
            
            if (selector == timer->getSelector())
            {
                return true;
            }
        }
        
        return false;
    }
    
    return false;  // should never get here
}

void Scheduler::removeUpdateFromHash(struct _listEntry *entry)
{
    tHashUpdateEntry *element = nullptr;

    HASH_FIND_PTR(m_pHashForUpdates, &entry->target, element);
    if (element)
    {
        // list entry
        DL_DELETE(*element->list, element->entry);
        free(element->entry);

        // hash entry
        Object* target = element->target;
        HASH_DEL(m_pHashForUpdates, element);
        free(element);

        // target#release should be the last one to prevent
        // a possible double-free. eg: If the [target dealloc] might want to remove it itself from there
        target->release();
    }
}

void Scheduler::unscheduleUpdateForTarget(const Object *target)
{
    if (target == nullptr)
    {
        return;
    }

    tHashUpdateEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForUpdates, &target, element);
    if (element)
    {
        if (m_bUpdateHashLocked)
        {
            element->entry->markedForDeletion = true;
        }
        else
        {
            this->removeUpdateFromHash(element->entry);
        }
    }
}

void Scheduler::unscheduleAll(void)
{
    unscheduleAllWithMinPriority(PRIORITY_SYSTEM);
}

void Scheduler::unscheduleAllWithMinPriority(int minPriority)
{
    // Custom Selectors
    tHashTimerEntry *element = nullptr;
    tHashTimerEntry *nextElement = nullptr;
    for (element = m_pHashForTimers; element != nullptr;)
    {
        // element may be removed in unscheduleAllSelectorsForTarget
        nextElement = (tHashTimerEntry *)element->hh.next;
        unscheduleAllForTarget(element->target);

        element = nextElement;
    }

    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE(m_pUpdatesNegList, entry, tmp)
        {
            if(entry->priority >= minPriority)
            {
                unscheduleUpdateForTarget(entry->target);
            }
        }
    }

    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE(m_pUpdates0List, entry, tmp)
        {
            unscheduleUpdateForTarget(entry->target);
        }
    }

    DL_FOREACH_SAFE(m_pUpdatesPosList, entry, tmp)
    {
        if(entry->priority >= minPriority)
        {
            unscheduleUpdateForTarget(entry->target);
        }
    }

    if (m_pScriptHandlerEntries)
    {
        m_pScriptHandlerEntries->removeAllObjects();
    }
}

void Scheduler::unscheduleAllForTarget(Object *target)
{
    // explicit nullptr handling
    if (target == nullptr)
    {
        return;
    }

    // Custom Selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);

    if (element)
    {
        if (ccArrayContainsObject(element->timers, element->currentTimer)
            && (! element->currentTimerSalvaged))
        {
            element->currentTimer->retain();
            element->currentTimerSalvaged = true;
        }
        ccArrayRemoveAllObjects(element->timers);

        if (m_pCurrentTarget == element)
        {
            m_bCurrentTargetSalvaged = true;
        }
        else
        {
            removeHashElement(element);
        }
    }

    // update selector
    unscheduleUpdateForTarget(target);
}

unsigned int Scheduler::scheduleScriptFunc(unsigned int handler, float interval, bool paused)
{
    SchedulerScriptHandlerEntry* entry = SchedulerScriptHandlerEntry::create(handler, interval, paused);
    if (!m_pScriptHandlerEntries)
    {
        m_pScriptHandlerEntries = Array::createWithCapacity(20);
        m_pScriptHandlerEntries->retain();
    }
    m_pScriptHandlerEntries->addObject(entry);
    return entry->getEntryId();
}

void Scheduler::unscheduleScriptEntry(unsigned int scheduleScriptEntryID)
{
    for (int i = m_pScriptHandlerEntries->count() - 1; i >= 0; i--)
    {
        SchedulerScriptHandlerEntry* entry = static_cast<SchedulerScriptHandlerEntry*>(m_pScriptHandlerEntries->getObjectAtIndex(i));
        if (entry->getEntryId() == (int)scheduleScriptEntryID)
        {
            entry->markedForDeletion();
            break;
        }
    }
}

void Scheduler::resumeTarget(Object *target)
{
    CCASSERT(target != nullptr, "");

    // custom selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);
    if (element)
    {
        element->paused = false;
    }

    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(m_pHashForUpdates, &target, elementUpdate);
    if (elementUpdate)
    {
        CCASSERT(elementUpdate->entry != nullptr, "");
        elementUpdate->entry->paused = false;
    }
}

void Scheduler::pauseTarget(Object *target)
{
    CCASSERT(target != nullptr, "");

    // custom selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);
    if (element)
    {
        element->paused = true;
    }

    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(m_pHashForUpdates, &target, elementUpdate);
    if (elementUpdate)
    {
        CCASSERT(elementUpdate->entry != nullptr, "");
        elementUpdate->entry->paused = true;
    }
}

bool Scheduler::isTargetPaused(Object *target)
{
    CCASSERT( target != nullptr, "target must be non nil" );

    // Custom selectors
    tHashTimerEntry *element = nullptr;
    HASH_FIND_PTR(m_pHashForTimers, &target, element);
    if( element )
    {
        return element->paused;
    }
    
    // We should check update selectors if target does not have custom selectors
	tHashUpdateEntry *elementUpdate = nullptr;
	HASH_FIND_PTR(m_pHashForUpdates, &target, elementUpdate);
	if ( elementUpdate )
    {
		return elementUpdate->entry->paused;
    }
    
    return false;  // should never get here
}

Set* Scheduler::pauseAllTargets()
{
    return pauseAllTargetsWithMinPriority(PRIORITY_SYSTEM);
}

Set* Scheduler::pauseAllTargetsWithMinPriority(int minPriority)
{
    Set* idsWithSelectors = new Set();// setWithCapacity:50];
    idsWithSelectors->autorelease();

    // Custom Selectors
    for(tHashTimerEntry *element = m_pHashForTimers; element != nullptr;
        element = (tHashTimerEntry*)element->hh.next)
    {
        element->paused = true;
        idsWithSelectors->addObject(element->target);
    }

    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE( m_pUpdatesNegList, entry, tmp ) 
        {
            if(entry->priority >= minPriority)
            {
                entry->paused = true;
                idsWithSelectors->addObject(entry->target);
            }
        }
    }

    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE( m_pUpdates0List, entry, tmp )
        {
            entry->paused = true;
            idsWithSelectors->addObject(entry->target);
        }
    }

    DL_FOREACH_SAFE( m_pUpdatesPosList, entry, tmp ) 
    {
        if(entry->priority >= minPriority) 
        {
            entry->paused = true;
            idsWithSelectors->addObject(entry->target);
        }
    }

    return idsWithSelectors;
}

void Scheduler::resumeTargets(Set* targetsToResume)
{
    SetIterator iter;
    for (iter = targetsToResume->begin(); iter != targetsToResume->end(); ++iter)
    {
        resumeTarget(*iter);
    }
}

// main loop
void Scheduler::update(float dt)
{
    m_bUpdateHashLocked = true;

    if (m_fTimeScale != 1.0f)
    {
        dt *= m_fTimeScale;
    }

    // Iterate over all the Updates' selectors
    tListEntry *entry, *tmp;

    // updates with priority < 0
    DL_FOREACH_SAFE(m_pUpdatesNegList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->target->update(dt);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(m_pUpdates0List, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->target->update(dt);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(m_pUpdatesPosList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->target->update(dt);
        }
    }

    // Iterate over all the custom selectors
    for (tHashTimerEntry *elt = m_pHashForTimers; elt != nullptr; )
    {
        m_pCurrentTarget = elt;
        m_bCurrentTargetSalvaged = false;

        if (! m_pCurrentTarget->paused)
        {
            // The 'timers' array may change while inside this loop
            for (elt->timerIndex = 0; elt->timerIndex < elt->timers->num; ++(elt->timerIndex))
            {
                elt->currentTimer = (Timer*)(elt->timers->arr[elt->timerIndex]);
                elt->currentTimerSalvaged = false;

                elt->currentTimer->update(dt);

                if (elt->currentTimerSalvaged)
                {
                    // The currentTimer told the remove itself. To prevent the timer from
                    // accidentally deallocating itself before finishing its step, we retained
                    // it. Now that step is done, it's safe to release it.
                    elt->currentTimer->release();
                }

                elt->currentTimer = nullptr;
            }
        }

        // elt, at this moment, is still valid
        // so it is safe to ask this here (issue #490)
        elt = (tHashTimerEntry *)elt->hh.next;

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (m_bCurrentTargetSalvaged && m_pCurrentTarget->timers->num == 0)
        {
            removeHashElement(m_pCurrentTarget);
        }
    }

    // Iterate over all the script callbacks
    if (m_pScriptHandlerEntries)
    {
        for (int i = m_pScriptHandlerEntries->count() - 1; i >= 0; i--)
        {
            SchedulerScriptHandlerEntry* eachEntry = static_cast<SchedulerScriptHandlerEntry*>(m_pScriptHandlerEntries->getObjectAtIndex(i));
            if (eachEntry->isMarkedForDeletion())
            {
                m_pScriptHandlerEntries->removeObjectAtIndex(i);
            }
            else if (!eachEntry->isPaused())
            {
                eachEntry->getTimer()->update(dt);
            }
        }
    }

    // delete all updates that are marked for deletion
    // updates with priority < 0
    DL_FOREACH_SAFE(m_pUpdatesNegList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(m_pUpdates0List, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(m_pUpdatesPosList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    m_bUpdateHashLocked = false;

    m_pCurrentTarget = nullptr;
}


NS_CC_END
