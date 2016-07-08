/* -----------------------------------------------------------------------------------
 *
 *      File            CCTouchDispatcher.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Valentin Milea
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
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouchHandler.h"
#include "touch_dispatcher/CCTouch.h"
#include "cocoa/CCString.h"
#include <algorithm>

NS_CC_BEGIN
    
//
// Used for sort
//
static KDint less ( const CCObject* p1, const CCObject* p2 )
{
	return ( (CCTouchHandler*) p1 )->getPriority ( ) < ( (CCTouchHandler*) p2 )->getPriority ( );
}

KDbool CCTouchDispatcher::isDispatchEvents ( KDvoid )
{
	return m_bDispatchEvents;
}

KDvoid CCTouchDispatcher::setDispatchEvents ( KDbool bDispatchEvents )
{
	m_bDispatchEvents = bDispatchEvents;
}

KDbool CCTouchDispatcher::init ( KDvoid )
{
	m_bDispatchEvents   = KD_TRUE;
 	m_pTargetedHandlers = CCArray::createWithCapacity ( 8 );
    m_pTargetedHandlers->retain ( );

 	m_pStandardHandlers = CCArray::createWithCapacity ( 4 );
    m_pStandardHandlers->retain ( );

 	m_pHandlersToAdd    = CCArray::createWithCapacity ( 8 );
    m_pHandlersToAdd->retain ( );

	m_pHandlersToRemove = ccCArrayNew ( 8 );

	m_bToRemove = KD_FALSE;
	m_bToAdd    = KD_FALSE;
	m_bToQuit   = KD_FALSE;
	m_bLocked   = KD_FALSE;

	m_sHandlerHelperData[ CCTOUCHBEGAN ].m_nType = CCTOUCHBEGAN;
	m_sHandlerHelperData[ CCTOUCHMOVED ].m_nType = CCTOUCHMOVED;
	m_sHandlerHelperData[ CCTOUCHENDED ].m_nType = CCTOUCHENDED;

	return KD_TRUE;
}

CCTouchDispatcher::CCTouchDispatcher ( KDvoid )
{
	m_pTargetedHandlers = KD_NULL;
	m_pStandardHandlers = KD_NULL;
	m_pHandlersToAdd    = KD_NULL;
	m_pHandlersToRemove = KD_NULL;
}

CCTouchDispatcher::~CCTouchDispatcher ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTargetedHandlers );
	CC_SAFE_RELEASE ( m_pStandardHandlers );
	CC_SAFE_RELEASE ( m_pHandlersToAdd );
 
	ccCArrayFree ( m_pHandlersToRemove );
	m_pHandlersToRemove = KD_NULL;
}

//
// handlers management
//
KDvoid CCTouchDispatcher::forceAddHandler ( CCTouchHandler* pHandler, CCArray* pArray )
{
	KDuint  uIndex = 0;

 	CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pArray, pObject )
 	{
		CCTouchHandler*  pTouchHandler = (CCTouchHandler*) pObject;
        if ( pTouchHandler )
        {
			if ( pTouchHandler->getPriority ( ) < pHandler->getPriority ( ) )
 		    {
 			    ++uIndex;
 		    }
 
 		    if ( pTouchHandler->getDelegate ( ) == pHandler->getDelegate ( ) )
 		    {
 			    CCAssert ( 0, "" );
 			    return;
 		    }
		}
 	}

	pArray->insertObject ( pHandler, uIndex );
}

KDvoid CCTouchDispatcher::addStandardDelegate ( CCTouchDelegate* pDelegate, KDint nPriority )
{
	CCTouchHandler*  pHandler = CCStandardTouchHandler::create ( pDelegate, nPriority );
	if ( !m_bLocked )
	{
		forceAddHandler ( pHandler, m_pStandardHandlers );
	}
	else
	{
		/*	If pHandler is contained in m_pHandlersToRemove, if so remove it from m_pHandlersToRemove and retrun.
		 *	Refer issue #752(cocos2d-x)
		 */
		if ( ccCArrayContainsValue ( m_pHandlersToRemove, pDelegate ) )
		{
			ccCArrayRemoveValue ( m_pHandlersToRemove, pDelegate );
			return;
		}

		m_pHandlersToAdd->addObject ( pHandler );
		m_bToAdd = KD_TRUE;
	}
}

KDvoid CCTouchDispatcher::addTargetedDelegate ( CCTouchDelegate* pDelegate, KDint nPriority, KDbool bSwallowsTouches )
{
	CCTouchHandler*  pHandler = CCTargetedTouchHandler::create ( pDelegate, nPriority, bSwallowsTouches );
	if ( !m_bLocked )
	{
		forceAddHandler ( pHandler, m_pTargetedHandlers );
	}
	else
	{
		/* If pHandler is contained in m_pHandlersToRemove, if so remove it from m_pHandlersToRemove and retrun.
		 * Refer issue #752(cocos2d-x)
		 */
		if ( ccCArrayContainsValue ( m_pHandlersToRemove, pDelegate ) )
		{
			ccCArrayRemoveValue ( m_pHandlersToRemove, pDelegate );
			return;
		}

		m_pHandlersToAdd->addObject ( pHandler );
		m_bToAdd = KD_TRUE;
	}
}

KDvoid CCTouchDispatcher::forceRemoveDelegate ( CCTouchDelegate* pDelegate )
{
	CCTouchHandler*  pHandler;
	
	// XXX: remove it from both handlers ???

	// remove handler from m_pStandardHandlers
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pStandardHandlers, pObject )
    {
        pHandler = (CCTouchHandler*) pObject;
		if ( pHandler && pHandler->getDelegate ( ) == pDelegate )
		{
			m_pStandardHandlers->removeObject ( pHandler );
			break;
		}
	}

    // remove handler from m_pTargetedHandlers
	CCARRAY_FOREACH ( m_pTargetedHandlers, pObject )
	{
		pHandler = (CCTouchHandler*) pObject;
		if ( pHandler && pHandler->getDelegate ( ) == pDelegate )
		{
			m_pTargetedHandlers->removeObject(pHandler);
			break;
		}
	}
}

KDvoid CCTouchDispatcher::removeDelegate ( CCTouchDelegate* pDelegate )
{
	if ( !pDelegate )
	{
		return;
	}

	if ( !m_bLocked )
	{
		forceRemoveDelegate ( pDelegate );
	}
	else
	{
		/* If pHandler is contained in m_pHandlersToAdd, if so remove it from m_pHandlersToAdd and retrun.
		 * Refer issue #752(cocos2d-x)
		 */
		CCTouchHandler*  pHandler = findHandler ( m_pHandlersToAdd, pDelegate );
		if ( pHandler )
		{
			m_pHandlersToAdd->removeObject ( pHandler );
			return;
		}

		ccCArrayAppendValue ( m_pHandlersToRemove, pDelegate );
		m_bToRemove = KD_TRUE;
	}
}

KDvoid CCTouchDispatcher::forceRemoveAllDelegates ( KDvoid )
{
 	m_pStandardHandlers->removeAllObjects ( );
 	m_pTargetedHandlers->removeAllObjects ( );
}

KDvoid CCTouchDispatcher::removeAllDelegates ( KDvoid )
{
	if ( !m_bLocked )
	{
		forceRemoveAllDelegates ( );
	}
	else
	{
		m_bToQuit = KD_TRUE;
	}
}

CCTouchHandler* CCTouchDispatcher::findHandler ( CCTouchDelegate* pDelegate )
{
    CCObject*		 pObject  = KD_NULL;
	CCTouchHandler*  pHandler = KD_NULL;

    CCARRAY_FOREACH ( m_pTargetedHandlers, pObject )
    {
        pHandler = (CCTouchHandler*) pObject;
        if ( pHandler->getDelegate ( ) == pDelegate )
        {
            return pHandler;
        }
    }

    CCARRAY_FOREACH ( m_pStandardHandlers, pObject )
    {
        pHandler = (CCTouchHandler*) pObject;
        if ( pHandler->getDelegate ( ) == pDelegate )
        {
            return pHandler;
        }
    } 

	return KD_NULL;
}

CCTouchHandler* CCTouchDispatcher::findHandler ( CCArray* pArray, CCTouchDelegate* pDelegate )
{
	CCAssert ( pArray != KD_NULL && pDelegate != KD_NULL, "" );

	CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pArray, pObject )
    {
        CCTouchHandler*  pHandle = (CCTouchHandler*) pObject;
        if ( pHandle->getDelegate ( ) == pDelegate )
        {
            return pHandle;
        }
    }

	return KD_NULL;
}

KDvoid CCTouchDispatcher::rearrangeHandlers ( CCArray* pArray )
{
	std::sort ( pArray->m_pData->arr, pArray->m_pData->arr + pArray->m_pData->num, less );
}

KDvoid CCTouchDispatcher::setPriority ( KDint nPriority, CCTouchDelegate* pDelegate )
{
	CCAssert ( pDelegate, "" );

	CCTouchHandler*  pHandler = this->findHandler ( pDelegate );

	if ( pHandler && pHandler->getPriority ( ) != nPriority )
    {
        pHandler->setPriority ( nPriority );

        this->rearrangeHandlers ( m_pTargetedHandlers );
        this->rearrangeHandlers ( m_pStandardHandlers );
    }
}

KDvoid CCTouchDispatcher::setTargetedSwallow ( KDbool bSwallow, CCTouchDelegate* pDelegate )
{
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pTargetedHandlers, pObject )
    {
        CCTargetedTouchHandler*  pHandler = (CCTargetedTouchHandler*) pObject;
        if ( pHandler->getDelegate ( ) == pDelegate )
        {
			pHandler->setSwallowsTouches ( bSwallow );
			break;
        }
    }
}

//
// dispatch events
//
KDvoid CCTouchDispatcher::touches ( CCSet* pTouches, CCEvent* pEvent, KDuint uIndex )
{
	CCAssert ( uIndex >= 0 && uIndex < 4, "" );

	CCSet*  pMutableTouches;
	m_bLocked = KD_TRUE;

	// optimization to prevent a mutable copy when it is not necessary
 	KDuint  uTargetedHandlersCount = m_pTargetedHandlers->count ( );
 	KDuint  uStandardHandlersCount = m_pStandardHandlers->count ( );
	KDbool  bNeedsMutableSet = ( uTargetedHandlersCount && uStandardHandlersCount );

	pMutableTouches = ( bNeedsMutableSet ? pTouches->mutableCopy ( ) : pTouches );

	struct ccTouchHandlerHelperData  tHelper = m_sHandlerHelperData [ uIndex ];
	
	//
	// process the target handlers 1st
	//
	if ( uTargetedHandlersCount > 0 )
	{
        CCTouch*  pTouch;
		CCSetIterator  setIter;
		for ( setIter = pTouches->begin ( ); setIter != pTouches->end ( ); ++setIter )
		{
			pTouch = (CCTouch*) ( *setIter );

			CCTargetedTouchHandler*  pHandler = KD_NULL;
            CCObject*  pObject = KD_NULL;
            CCARRAY_FOREACH ( m_pTargetedHandlers, pObject )
            {
                pHandler = (CCTargetedTouchHandler*) pObject;
				if ( !pHandler )
				{
				   break;
				}

				KDbool  bClaimed = KD_FALSE;
				if ( uIndex == CCTOUCHBEGAN )
				{
					bClaimed = pHandler->getDelegate ( )->ccTouchBegan ( pTouch, pEvent );
					if ( bClaimed )
					{
						pHandler->getClaimedTouches ( )->addObject ( pTouch );
					}
				}
				else if ( pHandler->getClaimedTouches ( )->containsObject ( pTouch ) )
				{
					// moved ended 
					bClaimed = KD_TRUE;

					switch ( tHelper.m_nType )
					{
						case CCTOUCHMOVED :
							pHandler->getDelegate ( )->ccTouchMoved ( pTouch, pEvent );
							break;

						case CCTOUCHENDED :
							pHandler->getDelegate ( )->ccTouchEnded ( pTouch, pEvent );
							pHandler->getClaimedTouches ( )->removeObject ( pTouch );
							break;
					}
				}

				if ( bClaimed && pHandler->isSwallowsTouches ( ) )
				{
					if ( bNeedsMutableSet )
					{
						pMutableTouches->removeObject ( pTouch );
					}

					break;
				}
			}
		}
	}

	//
	// process standard handlers 2nd
	//
	if ( uStandardHandlersCount > 0 && pMutableTouches->count ( ) > 0 )
	{
		CCStandardTouchHandler*  pHandler = KD_NULL;
		CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pStandardHandlers, pObject )
        {
            pHandler = (CCStandardTouchHandler*) pObject;
            if ( !pHandler )
            {
			    break;
            }

			switch ( tHelper.m_nType )
			{
				case CCTOUCHBEGAN : pHandler->getDelegate ( )->ccTouchesBegan ( pMutableTouches, pEvent ); break;
				case CCTOUCHMOVED : pHandler->getDelegate ( )->ccTouchesMoved ( pMutableTouches, pEvent ); break;
				case CCTOUCHENDED : pHandler->getDelegate ( )->ccTouchesEnded ( pMutableTouches, pEvent ); break;
			}
		}
	}

	if ( bNeedsMutableSet )
	{
		pMutableTouches->release ( );
	}

	//
	// Optimization. To prevent a [handlers copy] which is expensive
	// the add/removes/quit is done after the iterations
	//
	m_bLocked = KD_FALSE;
	if ( m_bToRemove )
	{
		m_bToRemove = KD_FALSE;
		for ( KDuint i = 0; i < m_pHandlersToRemove->num; ++i )
		{
			forceRemoveDelegate ( (CCTouchDelegate*) m_pHandlersToRemove->arr [ i ] );
		}
		ccCArrayRemoveAllValues ( m_pHandlersToRemove );
	}

	if ( m_bToAdd )
	{
		m_bToAdd = KD_FALSE;
		CCTouchHandler* pHandler = KD_NULL;
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pHandlersToAdd, pObject )
		{
			pHandler = (CCTouchHandler*) pObject;
            if ( !pHandler )
            {
                break;
            }

			if ( dynamic_cast<CCTargetedTouchHandler*> ( pHandler ) != KD_NULL )
			{				
				forceAddHandler ( pHandler, m_pTargetedHandlers );
			}
			else
			{
				forceAddHandler ( pHandler, m_pStandardHandlers );
			}
 		}
 
 		m_pHandlersToAdd->removeAllObjects ( );	
	}

	if ( m_bToQuit )
	{
		m_bToQuit = KD_FALSE;
		forceRemoveAllDelegates ( );
	}
}

KDvoid CCTouchDispatcher::touchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_bDispatchEvents )
	{
		this->touches ( pTouches, pEvent, CCTOUCHBEGAN );
	}
}

KDvoid CCTouchDispatcher::touchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( m_bDispatchEvents )
	{
		this->touches ( pTouches, pEvent, CCTOUCHMOVED );
	}
}

KDvoid CCTouchDispatcher::touchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
    if ( m_bDispatchEvents )
	{
		this->touches ( pTouches, pEvent, CCTOUCHENDED );
	}
}

CCArray* CCTouchDispatcher::getTargetedHandlers ( KDvoid )
{
	return m_pTargetedHandlers;
}

CCArray* CCTouchDispatcher::getStandardHandlers ( KDvoid )
{
	return m_pStandardHandlers;
}

NS_CC_END
