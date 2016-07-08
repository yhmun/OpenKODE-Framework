/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionManager.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2009      Valentin Milea
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
#include "actions/CCActionManager.h"
#include "base_nodes/CCNode.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCString.h"
#include "support/CCScheduler.h"

NS_CC_BEGIN
    
typedef struct _hashElement
{
	struct _ccArray*		pActions;
	CCObject*				pTarget;
	KDuint					uActionIndex;
	CCAction*				pCurrentAction;
	KDbool					bCurrentActionSalvaged;
	KDbool					bPaused;
	UT_hash_handle			hh;
} tHashElement;

CCActionManager::CCActionManager ( KDvoid )
{    
    m_pTargets = KD_NULL;
    m_pCurrentTarget = KD_NULL;
    m_bCurrentTargetSalvaged = KD_FALSE;
}

CCActionManager::~CCActionManager ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCActionManager = %p", this );

	removeAllActions ( );
}

//
// private
//
KDvoid CCActionManager::deleteHashElement ( tHashElement* pElement )
{
	ccArrayFree ( pElement->pActions );
	HASH_DEL ( m_pTargets, pElement );
	pElement->pTarget->release ( );
	kdFree ( pElement );
}

KDvoid CCActionManager::actionAllocWithHashElement ( tHashElement* pElement )
{
	// 4 actions per Node by default
	if ( pElement->pActions == KD_NULL )
	{
		pElement->pActions = ccArrayNew ( 4 );
	}
	else if ( pElement->pActions->num == pElement->pActions->max )
	{
		ccArrayDoubleCapacity ( pElement->pActions );
	}
}

KDvoid CCActionManager::removeActionAtIndex ( KDuint uIndex, tHashElement* pElement )
{
	CCAction*  pAction = (CCAction*) pElement->pActions->arr[ uIndex ];

	if ( pAction == pElement->pCurrentAction && ( !pElement->bCurrentActionSalvaged ) )
	{		
		pElement->pCurrentAction->retain ( );
		pElement->bCurrentActionSalvaged = KD_TRUE;
	}

	ccArrayRemoveObjectAtIndex ( pElement->pActions, uIndex, KD_TRUE );

	// update actionIndex in case we are in tick. looping over the actions
	if ( pElement->uActionIndex >= uIndex )
	{
		pElement->uActionIndex--;
	}

	if ( pElement->pActions->num == 0 )
	{
		if ( m_pCurrentTarget == pElement )
		{
			m_bCurrentTargetSalvaged = KD_TRUE;
		}
		else
		{			
			deleteHashElement ( pElement );
		}
	}
}

//
// pause / resume
//
KDvoid CCActionManager::pauseTarget ( CCObject* pTarget )
{
	tHashElement*  pElement = KD_NULL;

	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );
	if ( pElement )
	{
		pElement->bPaused = KD_TRUE;
	}
}

KDvoid CCActionManager::resumeTarget ( CCObject* pTarget )
{
	tHashElement*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );

	if ( pElement )
	{
		pElement->bPaused = KD_FALSE;
	}
}

CCSet* CCActionManager::pauseAllRunningActions ( KDvoid )
{
    CCSet*  pIdsWithActions = new CCSet ( );
    pIdsWithActions->autorelease ( );
    
    for ( tHashElement*  pElement = m_pTargets; pElement != KD_NULL; pElement = (tHashElement*) pElement->hh.next ) 
    {
        if ( !pElement->bPaused ) 
        {
            pElement->bPaused = KD_TRUE;
            pIdsWithActions->addObject ( pElement->pTarget );
        }
    }    
    
    return pIdsWithActions;
}

KDvoid CCActionManager::resumeTargets ( CCSet* pTargetsToResume )
{    
    CCSetIterator iter;
    for ( iter = pTargetsToResume->begin ( ); iter != pTargetsToResume->end ( ); ++iter )
    {
        resumeTarget ( *iter );
    }
}

//
// run
//
KDvoid CCActionManager::addAction ( CCAction* pAction, CCNode* pTarget, KDbool bPaused )
{
	CCAssert ( pAction != KD_NULL, "" );
	CCAssert ( pTarget != KD_NULL, "" );

	tHashElement*  pElement = KD_NULL;

	// we should convert it to CCObject*, because we save it as CCObject*
	CCObject*  pTemp = pTarget;
	HASH_FIND_INT ( m_pTargets, &pTemp, pElement );
	if ( !pElement )
	{
		pElement = (tHashElement*) kdCalloc ( sizeof ( *pElement ), 1 );
		pElement->bPaused = bPaused;
		pTarget->retain ( );
		pElement->pTarget = pTarget;
		HASH_ADD_INT ( m_pTargets, pTarget, pElement );
	}

 	actionAllocWithHashElement ( pElement );
 
 	CCAssert ( !ccArrayContainsObject ( pElement->pActions, pAction ), "" );
 	ccArrayAppendObject ( pElement->pActions, pAction );
 
 	pAction->startWithTarget ( pTarget );
}

//
// remove
//
KDvoid CCActionManager::removeAllActions ( KDvoid )
{
	for ( tHashElement* pElement = m_pTargets; pElement != KD_NULL; )
	{
		CCObject*  pTarget = pElement->pTarget;
		pElement = (tHashElement*) pElement->hh.next;
		removeAllActionsFromTarget ( pTarget );
	}
}

KDvoid CCActionManager::removeAllActionsFromTarget ( CCObject* pTarget )
{
	// explicit null handling
	if ( pTarget == KD_NULL )
	{
		return;
	}

	tHashElement*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );
	if ( pElement )
	{
		if ( ccArrayContainsObject ( pElement->pActions, pElement->pCurrentAction ) && ( !pElement->bCurrentActionSalvaged ) )
		{
			pElement->pCurrentAction->retain ( );
			pElement->bCurrentActionSalvaged = KD_TRUE;
		}

		ccArrayRemoveAllObjects ( pElement->pActions );
		if ( m_pCurrentTarget == pElement )
		{
			m_bCurrentTargetSalvaged = KD_TRUE;
		}
		else
		{
			deleteHashElement ( pElement );
		}
	}
	else
	{
//		CCLOG ( "XMCocos2D : removeAllActionsFromTarget - Target not found" );
	}
}

KDvoid CCActionManager::removeAction ( CCAction* pAction )
{
	// explicit null handling
	if ( pAction == KD_NULL )
	{
		return;
	}

	tHashElement*  pElement = KD_NULL;
	CCObject*      pTarget  = pAction->getOriginalTarget ( );
	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );
	if ( pElement )
	{
		KDuint  i = ccArrayGetIndexOfObject ( pElement->pActions, pAction );
		if ( KDUINT_MAX != i )
		{
			removeActionAtIndex ( i, pElement );
		}
	}
	else
	{
		CCLOG ( "XMCocos2D : removeAction - Target not found" );
	}
}

KDvoid CCActionManager::removeActionByTag ( KDuint uTag, CCObject* pTarget )
{
	CCAssert ( (KDint) uTag != kCCActionTagInvalid, "" );
	CCAssert ( pTarget != KD_NULL, "" );

	tHashElement*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );
	if ( pElement )
	{
		KDuint  uLimit = pElement->pActions->num;
		for ( KDuint i = 0; i < uLimit; ++i )
		{
			CCAction*  pAction = (CCAction*) pElement->pActions->arr [ i ];
			if ( pAction->getTag ( ) == (KDint) uTag && pAction->getOriginalTarget ( ) == pTarget )
			{
				removeActionAtIndex ( i, pElement );
				break;
			}
		}
	}
}

//
// get
//
CCAction* CCActionManager::getActionByTag ( KDuint uTag, CCObject* pTarget )
{
	CCAssert ( (KDint) uTag != kCCActionTagInvalid, "" );

	tHashElement*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );
	if ( pElement )
	{
		if ( pElement->pActions != KD_NULL )
		{
			KDuint  uLimit = pElement->pActions->num;
			for ( KDuint i = 0; i < uLimit; ++i )
			{
				CCAction*  pAction = (CCAction*) pElement->pActions->arr [ i ];
				if ( pAction->getTag ( ) == (KDint) uTag )
				{
					return pAction;
				}
			}
		}
        
		CCLOG ( "XMCocos2D : getActionByTag(tag = %d): Action not found", uTag );
	}
	else
	{
//		CCLOG ( "XMCocos2D : getActionByTag - Target not found" );
	}

	return KD_NULL;
}

KDuint CCActionManager::numberOfRunningActionsInTarget ( CCObject* pTarget )
{
	tHashElement*  pElement = KD_NULL;
	HASH_FIND_INT ( m_pTargets, &pTarget, pElement );
	if ( pElement )
	{
		return pElement->pActions ? pElement->pActions->num : 0;
	}

	return 0;
}

//
// main loop
//
KDvoid CCActionManager::update ( KDfloat fDelta )
{
	for ( tHashElement*  pElement = m_pTargets; pElement != KD_NULL; )
	{
		m_pCurrentTarget = pElement;
		m_bCurrentTargetSalvaged = KD_FALSE;

		if ( !m_pCurrentTarget->bPaused )
		{
			// The 'actions' CCMutableArray may change while inside this loop.
			for ( m_pCurrentTarget->uActionIndex = 0; m_pCurrentTarget->uActionIndex < m_pCurrentTarget->pActions->num; m_pCurrentTarget->uActionIndex++ )
			{
				m_pCurrentTarget->pCurrentAction = (CCAction*) m_pCurrentTarget->pActions->arr [ m_pCurrentTarget->uActionIndex ];
				if ( m_pCurrentTarget->pCurrentAction == KD_NULL )
				{
					continue;
				}

				m_pCurrentTarget->bCurrentActionSalvaged = KD_FALSE;

				m_pCurrentTarget->pCurrentAction->step ( fDelta );

				if ( m_pCurrentTarget->bCurrentActionSalvaged )
				{
					// The currentAction told the node to remove it. To prevent the action from
					// accidentally deallocating itself before finishing its step, we retained
					// it. Now that step is done, it's safe to release it.
					m_pCurrentTarget->pCurrentAction->release ( );
				} 
				else if ( m_pCurrentTarget->pCurrentAction->isDone ( ) )
				{
					m_pCurrentTarget->pCurrentAction->stop ( );

					CCAction*  pAction = m_pCurrentTarget->pCurrentAction;

					// Make currentAction nil to prevent removeAction from salvaging it.
					m_pCurrentTarget->pCurrentAction = KD_NULL;
					removeAction ( pAction );
				}

				m_pCurrentTarget->pCurrentAction = KD_NULL;
			}
		}

		// elt, at this moment, is still valid
		// so it is safe to ask this here (issue #490)
		pElement = (tHashElement*) ( pElement->hh.next );

		// only delete currentTarget if no actions were scheduled during the cycle (issue #481)
		if ( m_bCurrentTargetSalvaged && m_pCurrentTarget->pActions->num == 0 )
		{
			deleteHashElement ( m_pCurrentTarget );
		}
	}

	// issue #635
	m_pCurrentTarget = KD_NULL;
}


NS_CC_END