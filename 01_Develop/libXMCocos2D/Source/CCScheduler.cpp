/* -----------------------------------------------------------------------------------
 *
 *      File            CCScheduler.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/CCScheduler.h"
#include "support/CCDirector.h"
#include "support/data_support/utlist.h"
#include "script_support/CCScriptSupport.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN
    
//
// data structures
//

/// A list double-linked list used for "updates with priority"
typedef struct _listEntry
{
	struct _listEntry*	prev;
	struct _listEntry*	next;
	CCObject*			target;				// not retained (retained by hashUpdateEntry)
	KDint				priority;
	KDbool				paused;
	KDbool				markedForDeletion;	// selector will no longer be called and entry will be removed at end of the next tick
} tListEntry;

typedef struct _hashUpdateEntry
{
	tListEntry**        list;				// Which list does it belong to ?
	tListEntry*         entry;				// entry in the list
	CCObject*			target;				// hash key (retained)
	UT_hash_handle      hh;
} tHashUpdateEntry;

/// Hash Element used for "selectors with interval"
typedef struct _hashSelectorEntry
{
	ccArray*            timers;
	CCObject*			target;				// hash key (retained)
	KDuint              timerIndex;
	CCTimer*            currentTimer;
	KDbool              currentTimerSalvaged;
	KDbool              paused;
	UT_hash_handle      hh;
} tHashTimerEntry;

//
// implementation CCTimer
//
CCTimer::CCTimer ( KDvoid )
{
    m_pSelector			= KD_NULL;
    m_fInterval			= 0.0f;
    m_pTarget			= KD_NULL;
    m_fElapsed			= -1;
	m_bRunForever		= KD_FALSE;
	m_bUseDelay			= KD_FALSE;
	m_uTimesExecuted	= 0;
	m_uRepeat			= 0;
	m_fDelay			= 0.0f;
	m_nScriptHandler	= 0;
}

CCTimer* CCTimer::createWithTarget ( CCObject* pTarget, SEL_SCHEDULE pSelector )
{
	CCTimer*  pRet = new CCTimer ( );

    if ( pRet && pRet->initWithTarget ( pTarget, pSelector, 0.0f, kCCRepeatForever, 0.0f ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCTimer* CCTimer::createWithTarget ( CCObject* pTarget, SEL_SCHEDULE pSelector, KDfloat fSeconds )
{
	CCTimer*  pRet = new CCTimer ( );

    if ( pRet && pRet->initWithTarget ( pTarget, pSelector, fSeconds, kCCRepeatForever, 0.0f ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCTimer* CCTimer::createWithScriptHandler ( KDint nHandler, KDfloat fSeconds )
{
	CCTimer*  pRet = new CCTimer ( );

    if ( pRet && pRet->initWithScriptHandler ( nHandler, fSeconds ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCTimer::initWithScriptHandler ( KDint nHandler, KDfloat fSeconds )
{
	m_nScriptHandler = nHandler;
	m_fInterval		 = fSeconds;
	m_fElapsed		 = -1;

	return KD_TRUE;
}

KDbool CCTimer::initWithTarget ( CCObject* pTarget, SEL_SCHEDULE pSelector )
{
    return initWithTarget ( pTarget, pSelector, 0, kCCRepeatForever, 0.0f );
}

KDbool CCTimer::initWithTarget ( CCObject* pTarget, SEL_SCHEDULE pSelector, KDfloat fSeconds, KDuint uRepeat, KDfloat fDelay )
{
	m_pTarget		= pTarget;
	m_pSelector		= pSelector;
	m_fElapsed		= -1;
	m_fInterval		= fSeconds;
	m_fDelay		= fDelay;
    m_bUseDelay		= ( fDelay > 0.0f ) ? KD_TRUE : KD_FALSE;
    m_uRepeat		= uRepeat;
    m_bRunForever	= ( uRepeat == kCCRepeatForever ) ? KD_TRUE : KD_FALSE;

	return KD_TRUE;
}

KDvoid CCTimer::update ( KDfloat fElapsed )
{
	if ( m_fElapsed == -1 )
	{
		m_fElapsed = 0;
		m_uTimesExecuted = 0;
	}
	else
    {
        if ( m_bRunForever && !m_bUseDelay )
        {
			// standard timer usage
            m_fElapsed += fElapsed;
            if ( m_fElapsed >= m_fInterval )
            {
                if ( m_pTarget && m_pSelector )
                {
                    ( m_pTarget->*m_pSelector ) ( m_fElapsed );
                }

                if ( m_nScriptHandler )
                {
					CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeSchedule ( m_nScriptHandler, m_fElapsed );
	            }
                m_fElapsed = 0;
            }
        }    
        else
        {
			// advanced usage
            m_fElapsed += fElapsed;
            if ( m_bUseDelay )
            {
                if ( m_fElapsed >= m_fDelay )
                {
                    if ( m_pTarget && m_pSelector )
                    {
                        ( m_pTarget->*m_pSelector ) ( m_fElapsed );
                    }

                    if ( m_nScriptHandler )
                    {
                        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeSchedule ( m_nScriptHandler, m_fElapsed ); 
                    }
                    m_fElapsed = m_fElapsed - m_fDelay;
                    m_uTimesExecuted += 1;
                    m_bUseDelay = KD_FALSE;
                }
            }
            else
            {
                if ( m_fElapsed >= m_fInterval )
                {
                    if ( m_pTarget && m_pSelector )
                    {
                        ( m_pTarget->*m_pSelector ) ( m_fElapsed );
                    }

                    if ( m_nScriptHandler )
                    {
                        CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeSchedule ( m_nScriptHandler, m_fElapsed );
                    }
                    m_fElapsed = 0;
                    m_uTimesExecuted += 1;
                }
            }

            if ( !m_bRunForever && m_uTimesExecuted > m_uRepeat )
            {   
				// unschedule timer
                CCDirector::sharedDirector ( )->getScheduler ( )->unscheduleSelector ( m_pSelector, m_pTarget );
            }
        }
    }
}

KDfloat CCTimer::getInterval ( KDvoid ) const
{
    return m_fInterval;
}

KDvoid CCTimer::setInterval ( KDfloat fInterval )
{
    m_fInterval = fInterval;
}

SEL_SCHEDULE CCTimer::getSelector ( KDvoid ) const
{
    return m_pSelector;
}

//
// implementation of CCScheduler
//

CCScheduler::CCScheduler ( KDvoid )
{
    m_fTimeScale			 = 1.0f;
    m_pUpdatesNegList		 = KD_NULL; 
    m_pUpdates0List			 = KD_NULL; 
    m_pUpdatesPosList		 = KD_NULL; 
    m_pHashForUpdates		 = KD_NULL; 
    m_pHashForTimers		 = KD_NULL; 
    m_pCurrentTarget		 = KD_NULL; 
    m_bCurrentTargetSalvaged = KD_FALSE;
    m_pScriptHandlerEntries	 = KD_NULL; 
	m_bUpdateHashLocked		 = KD_FALSE;
}

CCScheduler::~CCScheduler ( KDvoid )
{
	unscheduleAll ( );
	CC_SAFE_RELEASE ( m_pScriptHandlerEntries );
}

KDvoid CCScheduler::removeHashElement ( _hashSelectorEntry* pElement )
{
	CCObject*  pTarget = pElement->target;

	ccArrayFree ( pElement->timers );

	HASH_DEL ( m_pHashForTimers, pElement );
	kdFree ( pElement );

	// make sure the target is released after we have removed the hash element
    // otherwise we access invalid memory when the release call deletes the target
    // and the target calls removeAllSelectors() during its destructor
    pTarget->release ( );
}

KDvoid CCScheduler::scheduleSelector ( SEL_SCHEDULE pSelector, CCObject* pTarget, KDfloat fInterval, KDbool bPaused )
{
	this->scheduleSelector ( pSelector, pTarget, fInterval, kCCRepeatForever, 0.0f, bPaused );
}

KDvoid CCScheduler::scheduleSelector ( SEL_SCHEDULE pSelector, CCObject* pTarget, KDfloat fInterval, KDuint uRepeat, KDfloat fDelay, KDbool bPaused )
{
	CCAssert ( pSelector, "Argument selector must be non-NULL" );
    CCAssert ( pTarget  , "Argument target must be non-NULL" );
  
    tHashTimerEntry*  pElement = KD_NULL;
    HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );

    if ( !pElement )
    {
        pElement = (tHashTimerEntry*) kdCalloc ( 1, sizeof (*pElement) );
        pElement->target = pTarget;
        if ( pTarget )
        {
            pTarget->retain ( );
        }
        HASH_ADD_INT ( m_pHashForTimers, target, pElement );

        // Is this the 1st element ? Then set the pause level to all the selectors of this target
        pElement->paused = bPaused;
    }
    else
    {
        CCAssert ( pElement->paused == bPaused, "" );
    }

    if ( pElement->timers == KD_NULL )
    {
        pElement->timers = ccArrayNew ( 10 );
    }
    else 
    {
        for ( KDuint i = 0; i < pElement->timers->num; ++i )
        {
            CCTimer*  pTimer = (CCTimer*) pElement->timers->arr [ i ];

			if ( pSelector == pTimer->getSelector ( ) )
            {
				CCLOG ( "CCScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", pTimer->getInterval ( ), fInterval );          
				pTimer->setInterval ( fInterval );
                return;
            }        
        }
        ccArrayEnsureExtraCapacity ( pElement->timers, 1 );
    }

    CCTimer*  pTimer = new CCTimer ( );
    pTimer->initWithTarget ( pTarget, pSelector, fInterval, uRepeat, fDelay );
    ccArrayAppendObject ( pElement->timers, pTimer );
    pTimer->release ( );   
}

KDvoid CCScheduler::unscheduleSelector ( SEL_SCHEDULE pSelector, CCObject* pTarget )
{
	// explicity handle nil arguments when removing an object
	if ( pTarget == 0 || pSelector == 0 )
	{
		return;
	}

	tHashTimerEntry*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );

	if ( pElement )
	{
		for ( KDuint i = 0; i < pElement->timers->num; ++i )
		{
			CCTimer*  pTimer = (CCTimer*) ( pElement->timers->arr[ i ] );

			if ( pSelector == pTimer->getSelector ( ) )
			{
				if ( pTimer == pElement->currentTimer && ( !pElement->currentTimerSalvaged ) )
				{
					pElement->currentTimer->retain ( );
					pElement->currentTimerSalvaged = KD_TRUE;
				}

				ccArrayRemoveObjectAtIndex ( pElement->timers, i, KD_TRUE );

				// update timerIndex in case we are in tick:, looping over the actions
				if ( pElement->timerIndex >= i )
				{
					pElement->timerIndex--;
				}

				if ( pElement->timers->num == 0 )
				{
					if ( m_pCurrentTarget == pElement )
					{
						m_bCurrentTargetSalvaged = KD_TRUE;
					}
					else
					{
						removeHashElement ( pElement );
					}
				}

				return;
			}
		}
	}
}

KDvoid CCScheduler::priorityIn ( tListEntry** ppList, CCObject* pTarget, KDint nPriority, KDbool bPaused )
{
	tListEntry*  pListElement = (tListEntry *) kdMalloc ( sizeof ( *pListElement ) );

	pListElement->target   = pTarget;
	pListElement->priority = nPriority;
	pListElement->paused   = bPaused;
	pListElement->next     = pListElement->prev = KD_NULL;
	pListElement->markedForDeletion = KD_FALSE;

	// empty list ?
	if ( !*ppList )
	{
		DL_APPEND ( *ppList, pListElement );
	}
	else
	{
		KDbool  bAdded = KD_FALSE;

		for ( tListEntry* pElement = *ppList; pElement; pElement = pElement->next )
		{
			if ( nPriority < pElement->priority )
			{
				if ( pElement == *ppList )
				{
					DL_PREPEND ( *ppList, pListElement );
				}
				else
				{
					pListElement->next = pElement;
					pListElement->prev = pElement->prev;

					pElement->prev->next = pListElement;
					pElement->prev = pListElement;
				}

				bAdded = KD_TRUE;
				break;
			}
		}

		// Not added? priority has the higher value. Append it.
		if ( !bAdded )
		{
			DL_APPEND ( *ppList, pListElement );
		}
	}

	// update hash entry for quick access
	tHashUpdateEntry*  pHashElement = (tHashUpdateEntry *) kdCalloc ( 1, sizeof ( *pHashElement ) );
	pHashElement->target = pTarget;
	pTarget->retain ( );
	pHashElement->list = ppList;
	pHashElement->entry = pListElement;
	HASH_ADD_INT ( m_pHashForUpdates, target, pHashElement );
}

KDvoid CCScheduler::appendIn ( _listEntry** ppList, CCObject* pTarget, KDbool bPaused )
{
	tListEntry*  pListElement = (tListEntry *) kdMalloc ( sizeof ( *pListElement ) );

	pListElement->target = pTarget;
	pListElement->paused = bPaused;
	pListElement->markedForDeletion = KD_FALSE;

	DL_APPEND ( *ppList, pListElement );

	// update hash entry for quicker access
	tHashUpdateEntry* pHashElement = (tHashUpdateEntry *) kdCalloc ( 1, sizeof ( *pHashElement ) );
	pHashElement->target = pTarget;
	pTarget->retain ( );
	pHashElement->list = ppList;
	pHashElement->entry = pListElement;
	HASH_ADD_INT ( m_pHashForUpdates, target, pHashElement );
}

KDvoid CCScheduler::scheduleUpdateForTarget ( CCObject* pTarget, KDint nPriority, KDbool bPaused )
{
	tHashUpdateEntry*  pHashElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForUpdates, &pTarget, pHashElement );
	if ( pHashElement )
	{
#if COCOS2D_DEBUG >= 1
		CCAssert ( pHashElement->entry->markedForDeletion, "" );
#endif
		// TODO: check if priority has changed!

		pHashElement->entry->markedForDeletion = KD_FALSE;
		return;
	}

	// most of the updates are going to be 0, that's way there
	// is an special list for updates with priority 0
	if ( nPriority == 0 )
	{
		appendIn ( &m_pUpdates0List, pTarget, bPaused );
	}
	else if ( nPriority < 0 )
	{
		priorityIn ( &m_pUpdatesNegList, pTarget, nPriority, bPaused );
	}
	else
	{
		// priority > 0
		priorityIn ( &m_pUpdatesPosList, pTarget, nPriority, bPaused );
	}
}

KDvoid CCScheduler::removeUpdateFromHash ( struct _listEntry *entry )
{
	tHashUpdateEntry*  pElement = KD_NULL;

	HASH_FIND_INT ( m_pHashForUpdates, &entry->target, pElement );
	if ( pElement )
	{
		// list entry
		DL_DELETE ( *pElement->list, pElement->entry );
		kdFree ( pElement->entry );

		// hash entry
		CCObject*  pTarget = pElement->target;
		HASH_DEL ( m_pHashForUpdates, pElement );
		kdFree ( pElement );

		// target#release should be the last one to prevent
        // a possible double-free. eg: If the [target dealloc] might want to remove it itself from there
        pTarget->release ( );
	}
}

KDvoid CCScheduler::unscheduleUpdateForTarget ( const CCObject* pTarget )
{
	if ( pTarget == KD_NULL )
	{
		return;
	}

	tHashUpdateEntry*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForUpdates, &pTarget, pElement );
	if ( pElement )
	{
		if ( m_bUpdateHashLocked )
		{
			pElement->entry->markedForDeletion = KD_TRUE;
		}
		else
		{
			this->removeUpdateFromHash ( pElement->entry );
		}
	}
}

KDvoid CCScheduler::unscheduleAll ( KDvoid )
{
	unscheduleAllWithMinPriority ( kCCPrioritySystem );
}

KDvoid CCScheduler::unscheduleAllWithMinPriority ( KDint nMinPriority )
{
	// Custom Selectors
    tHashTimerEntry*  pElement = KD_NULL;
	tHashTimerEntry*  pNextElement = KD_NULL;
	for ( pElement = m_pHashForTimers; pElement != KD_NULL; )
	{
		// pElement may be removed in unscheduleAllSelectorsForTarget
		pNextElement = (tHashTimerEntry *) pElement->hh.next;
		unscheduleAllForTarget ( pElement->target );

        pElement = pNextElement;
	}

	// Updates selectors
	tListEntry*  pEntry;
	tListEntry*  pTemp;

    if ( nMinPriority < 0 ) 
    {
        DL_FOREACH_SAFE ( m_pUpdatesNegList, pEntry, pTemp )
        {
            if ( pEntry->priority >= nMinPriority ) 
            {
                unscheduleUpdateForTarget ( pEntry->target );
            }
        }
    }

    if ( nMinPriority <= 0 ) 
    {
        DL_FOREACH_SAFE ( m_pUpdates0List, pEntry, pTemp )
        {
            unscheduleUpdateForTarget ( pEntry->target );
        }
    }

    DL_FOREACH_SAFE ( m_pUpdatesPosList, pEntry, pTemp )
    {
        if ( pEntry->priority >= nMinPriority ) 
        {
            unscheduleUpdateForTarget ( pEntry->target );
        }
    }

	if ( m_pScriptHandlerEntries )
    {
        m_pScriptHandlerEntries->removeAllObjects ( );
    }
}

KDvoid CCScheduler::unscheduleAllForTarget ( CCObject* pTarget )
{
	// explicit NULL handling
	if ( pTarget == KD_NULL )
	{
		return;
	}

	// Custom Selectors
	tHashTimerEntry*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );

	if ( pElement )
	{
		if ( ccArrayContainsObject ( pElement->timers, pElement->currentTimer ) 
			 && ( !pElement->currentTimerSalvaged ) )
		{
			pElement->currentTimer->retain ( );
			pElement->currentTimerSalvaged = KD_TRUE;
		}
		ccArrayRemoveAllObjects ( pElement->timers );

		if ( m_pCurrentTarget == pElement )
		{
			m_bCurrentTargetSalvaged = KD_TRUE;
		}
		else
		{
			removeHashElement ( pElement );
		}
	}

	// update selector
	unscheduleUpdateForTarget ( pTarget );
}

KDuint CCScheduler::scheduleScriptFunc ( KDuint uHandler, KDfloat fInterval, KDbool bPaused )
{
    CCSchedulerScriptHandlerEntry*  pEntry = CCSchedulerScriptHandlerEntry::create ( uHandler, fInterval, bPaused );
    if ( !m_pScriptHandlerEntries )
    {
        m_pScriptHandlerEntries = CCArray::createWithCapacity ( 20 );
        m_pScriptHandlerEntries->retain ( );
    }
    m_pScriptHandlerEntries->addObject ( pEntry );
    return pEntry->getEntryId ( );
}

KDvoid CCScheduler::unscheduleScriptEntry ( KDuint uScheduleScriptEntryID )
{
    for ( KDint i = m_pScriptHandlerEntries->count ( ) - 1; i >= 0; i-- )
    {
        CCSchedulerScriptHandlerEntry*  pEntry = static_cast<CCSchedulerScriptHandlerEntry*> ( m_pScriptHandlerEntries->objectAtIndex ( i ) );
        if ( pEntry->getEntryId ( ) == uScheduleScriptEntryID )
        {
            pEntry->markedForDeletion ( );
            break;
        }
    }
}

KDbool CCScheduler::isScheduled ( SEL_SCHEDULE pSelector, CCObject* pTarget )
{
	// explicity handle nil arguments when removing an object
	if ( pTarget == 0 || pSelector == 0 )
	{
		return KD_FALSE;
	}

	tHashTimerEntry*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );

	if ( pElement )
	{
		for ( KDuint i = 0; i < pElement->timers->num; ++i )
		{
			CCTimer*  pTimer = (CCTimer*) ( pElement->timers->arr[ i ] );

			if ( pSelector == pTimer->getSelector ( ) )
			{			
				return KD_TRUE;
			}
		}
	}

	return KD_FALSE;
}

KDvoid CCScheduler::resumeTarget ( CCObject* pTarget )
{
	CCAssert ( pTarget != KD_NULL, "" );

	// custom selectors
	tHashTimerEntry*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );
	if ( pElement )
	{
		pElement->paused = KD_FALSE;
	}

	// update selector
	tHashUpdateEntry*  pElementUpdate = KD_NULL;
	HASH_FIND_INT ( m_pHashForUpdates, &pTarget, pElementUpdate );
	if ( pElementUpdate )
	{
		CCAssert ( pElementUpdate->entry != KD_NULL, "" );
		pElementUpdate->entry->paused = KD_FALSE;
	}
}

KDvoid CCScheduler::pauseTarget ( CCObject* pTarget )
{
	CCAssert ( pTarget != KD_NULL, "" );

	// custom selectors
	tHashTimerEntry*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );
	if ( pElement )
	{
		pElement->paused = KD_TRUE;
	}

	// update selector
	tHashUpdateEntry*  pElementUpdate = KD_NULL;
	HASH_FIND_INT ( m_pHashForUpdates, &pTarget, pElementUpdate );
	if ( pElementUpdate )
	{
		CCAssert ( pElementUpdate->entry != KD_NULL, "" );
		pElementUpdate->entry->paused = KD_TRUE;
	}
}

KDbool CCScheduler::isTargetPaused ( CCObject* pTarget )
{
    CCAssert ( pTarget != KD_NULL, "target must be non nil" );

    // Custom selectors
    tHashTimerEntry*  pElement = KD_NULL;
    HASH_FIND_INT ( m_pHashForTimers, &pTarget, pElement );
    if ( pElement )
    {
        return pElement->paused;
    }
    
	// We should check update selectors if target does not have custom selectors
	tHashUpdateEntry*  pElementUpdate = KD_NULL;
	HASH_FIND_INT ( m_pHashForUpdates, &pTarget, pElementUpdate );
	if ( pElementUpdate )
    {
		return pElementUpdate->entry->paused;
    }

    return KD_FALSE;  // should never get here
}

CCSet* CCScheduler::pauseAllTargets ( KDvoid )
{
    return pauseAllTargetsWithMinPriority ( kCCPrioritySystem );
}

CCSet* CCScheduler::pauseAllTargetsWithMinPriority ( KDint nMinPriority )
{
    CCSet*  pIdsWithSelectors = new CCSet ( );	// setWithCapacity:50];
    pIdsWithSelectors->autorelease ( );

    // Custom Selectors
    for ( tHashTimerEntry* pElement = m_pHashForTimers; pElement != KD_NULL; pElement = (tHashTimerEntry*) pElement->hh.next ) 
    {
        pElement->paused = KD_TRUE;
        pIdsWithSelectors->addObject ( pElement->target );
    }

    // Updates selectors
    tListEntry*  pEntry;
	tListEntry*  pTemp;

    if ( nMinPriority < 0 ) 
    {
        DL_FOREACH_SAFE ( m_pUpdatesNegList, pEntry, pTemp ) 
        {
            if ( pEntry->priority >= nMinPriority ) 
            {
                pEntry->paused = KD_TRUE;
                pIdsWithSelectors->addObject ( pEntry->target );
            }
        }
    }

    if ( nMinPriority <= 0 ) 
    {
        DL_FOREACH_SAFE ( m_pUpdates0List, pEntry, pTemp )
        {
            pEntry->paused = KD_TRUE;
            pIdsWithSelectors->addObject ( pEntry->target );
        }
    }

    DL_FOREACH_SAFE ( m_pUpdatesPosList, pEntry, pTemp ) 
    {
        if ( pEntry->priority >= nMinPriority ) 
        {
            pEntry->paused = KD_TRUE;
            pIdsWithSelectors->addObject ( pEntry->target );
        }
    }

    return pIdsWithSelectors;
}

KDvoid CCScheduler::resumeTargets ( CCSet* pTargetsToResume )
{
    CCSetIterator  iter;
    for ( iter = pTargetsToResume->begin ( ); iter != pTargetsToResume->end ( ); ++iter )
    {
        resumeTarget ( *iter );
    }
}

/// main loop
KDvoid CCScheduler::update ( KDfloat fDelta )
{
	m_bUpdateHashLocked = KD_TRUE;

	if ( m_fTimeScale != 1.0f )
	{
		fDelta *= m_fTimeScale;
	}

	// Iterate over the Updates' selectors
	tListEntry*  pEntry;
	tListEntry*  pTemp;

	// updates with priority < 0
	DL_FOREACH_SAFE ( m_pUpdatesNegList, pEntry, pTemp )
	{
		if ( (!pEntry->paused) && (!pEntry->markedForDeletion) )
		{
			CCScriptEngineProtocol*  pEngine = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
            if ( pEngine != KD_NULL && kScriptTypeJavascript == pEngine->getScriptType ( ) )
            {
                CCScriptEngineManager::sharedManager ( )->getScriptEngine()->executeSchedule ( KD_NULL, fDelta, (CCNode*) pEntry->target );
            }

			pEntry->target->update ( fDelta );
		}
	}

	// updates with priority == 0
	DL_FOREACH_SAFE ( m_pUpdates0List, pEntry, pTemp )
	{
		if ( (!pEntry->paused) && (!pEntry->markedForDeletion) )
		{
			CCScriptEngineProtocol*  pEngine = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
            if ( pEngine != KD_NULL && kScriptTypeJavascript == pEngine->getScriptType ( ) )
            {
                pEngine->executeSchedule ( KD_NULL, fDelta, (CCNode*) pEntry->target );
            }

			pEntry->target->update ( fDelta );
		}
	}

	// updates with priority > 0
	DL_FOREACH_SAFE ( m_pUpdatesPosList, pEntry, pTemp )
	{
		if ( ( !pEntry->paused ) && ( !pEntry->markedForDeletion ) )
		{
			CCScriptEngineProtocol*  pEngine = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
            if ( pEngine != KD_NULL && kScriptTypeJavascript == pEngine->getScriptType ( ) )
            {
                CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeSchedule ( KD_NULL, fDelta, (CCNode*) pEntry->target );
            }

			pEntry->target->update ( fDelta );
		}
	}

	// Interate over the custom selectors
	for ( tHashTimerEntry* pElt = m_pHashForTimers; pElt != KD_NULL; )
	{
		m_pCurrentTarget = pElt;
		m_bCurrentTargetSalvaged = KD_FALSE;

		if ( !m_pCurrentTarget->paused )
		{
			// The 'timers' array may change while inside this loop
			for ( pElt->timerIndex = 0; pElt->timerIndex < pElt->timers->num; ++(pElt->timerIndex) )
			{
				pElt->currentTimer = (CCTimer*) ( pElt->timers->arr[ pElt->timerIndex ] );
				pElt->currentTimerSalvaged = KD_FALSE;

				pElt->currentTimer->update ( fDelta );

				if ( pElt->currentTimerSalvaged )
				{
					// The currentTimer told the remove itself. To prevent the timer from
					// accidentally deallocating itself before finishing its step, we retained
					// it. Now that step is done, it's safe to release it.
					pElt->currentTimer->release ( );
				}

				pElt->currentTimer = KD_NULL;
			}
		}

		// elt, at this moment, is still valid
		// so it is safe to ask this here (issue #490)
		pElt = (tHashTimerEntry *) pElt->hh.next;

		// only delete currentTarget if no actions were scheduled during the cycle (issue #481)
		if ( m_bCurrentTargetSalvaged && m_pCurrentTarget->timers->num == 0 )
		{
			removeHashElement ( m_pCurrentTarget );
		}
	}

	// Interate over the script callbacks
    if ( m_pScriptHandlerEntries )
    {
        for ( KDint  i = m_pScriptHandlerEntries->count ( ) - 1; i >= 0; i-- )
        {
            CCSchedulerScriptHandlerEntry*  pEntry = static_cast<CCSchedulerScriptHandlerEntry*> ( m_pScriptHandlerEntries->objectAtIndex ( i ) );
            if ( pEntry->isMarkedForDeletion ( ) )
            {
                m_pScriptHandlerEntries->removeObjectAtIndex ( i );
            }
            else if ( !pEntry->isPaused ( ) )
            {
                pEntry->getTimer ( )->update ( fDelta );
            }
        }
    }

	// delete all updates that are marked for deletion
	// updates with priority < 0
	DL_FOREACH_SAFE ( m_pUpdatesNegList, pEntry, pTemp )
	{
		if ( pEntry->markedForDeletion )
		{
			this->removeUpdateFromHash ( pEntry );
		}
	}

	// updates with priority == 0
	DL_FOREACH_SAFE ( m_pUpdates0List, pEntry, pTemp )
	{
		if ( pEntry->markedForDeletion )
		{
			this->removeUpdateFromHash ( pEntry );
		}
	}

	// updates with priority > 0
	DL_FOREACH_SAFE ( m_pUpdatesPosList, pEntry, pTemp )
	{
		if ( pEntry->markedForDeletion )
		{
			this->removeUpdateFromHash ( pEntry );
		}
	}

	m_bUpdateHashLocked = KD_FALSE;

	m_pCurrentTarget = KD_NULL;
}

    
NS_CC_END
