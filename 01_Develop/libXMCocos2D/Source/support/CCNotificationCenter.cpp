/* -----------------------------------------------------------------------------------
 *
 *      File            CCNotificationCenter.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Erawppa
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
#include "support/CCNotificationCenter.h"
#include "script_support/CCScriptSupport.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN

static CCNotificationCenter*	l_pSharedNotifCenter = KD_NULL;

CCNotificationCenter::CCNotificationCenter ( KDvoid )
{
	m_nScriptHandler = 0;
    m_pObservers = CCArray::createWithCapacity ( 3 );
    m_pObservers->retain ( );
}

CCNotificationCenter::~CCNotificationCenter ( KDvoid )
{	
    m_pObservers->release ( );
}

CCNotificationCenter* CCNotificationCenter::sharedNotificationCenter ( KDvoid )
{
    if ( !l_pSharedNotifCenter )
    {
        l_pSharedNotifCenter = new CCNotificationCenter;
    }

    return l_pSharedNotifCenter;
}

KDvoid CCNotificationCenter::purgeNotificationCenter ( KDvoid )
{
    CC_SAFE_RELEASE ( l_pSharedNotifCenter );
}

//
// internal functions
//
KDbool CCNotificationCenter::observerExisted ( CCObject* pTarget, const KDchar* szName )
{
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pObservers, pObject )
    {
        CCNotificationObserver*  pObserver = (CCNotificationObserver*) pObject;

        if ( !pObserver )
		{
            continue;
		}
        
        if ( !kdStrcmp ( pObserver->getName ( ), szName ) && pObserver->getTarget ( ) == pTarget )
		{
            return KD_TRUE;
		}
    }
    return KD_FALSE;
}

//
// observer functions
//
KDvoid CCNotificationCenter::addObserver ( CCObject* pTarget, SEL_CallFuncO pSelector, const KDchar* szName, CCObject* pObject )
{
    if ( this->observerExisted ( pTarget, szName ) )
	{
        return;
	}
    
    CCNotificationObserver*  pObserver = new CCNotificationObserver ( pTarget, pSelector, szName, pObject );
    if ( !pObserver )
	{
        return;
	}
    
    pObserver->autorelease ( );
    m_pObservers->addObject ( pObserver );
}

KDvoid CCNotificationCenter::removeObserver ( CCObject* pTarget, const KDchar* szName )
{
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pObservers, pObject )
    {
        CCNotificationObserver*  pObserver = (CCNotificationObserver*) pObject;

        if ( !pObserver )
		{
            continue;
		}
        
        if ( !kdStrcmp ( pObserver->getName ( ), szName ) && pObserver->getTarget ( ) == pTarget )
        {
            m_pObservers->removeObject ( pObserver );
            return;
        }
    }
}

KDint CCNotificationCenter::removeAllObservers ( CCObject* pTarget )
{
    CCObject*  pObj = KD_NULL;
    CCArray*   pToRemove = CCArray::create ( );

    CCARRAY_FOREACH ( m_pObservers, pObj )
    {
        CCNotificationObserver*  pObserver = (CCNotificationObserver*) pObj;
        if ( !pObserver )
		{
            continue;
		}

        if ( pObserver->getTarget ( ) == pTarget )
        {
            pToRemove->addObject ( pObserver );
        }
    }

    m_pObservers->removeObjectsInArray ( pToRemove );
    return pToRemove->count ( );
}

KDvoid CCNotificationCenter::registerScriptObserver ( CCObject* pTarget, KDint nHandler, const KDchar* szName )
{
	if ( this->observerExisted ( pTarget, szName ) )
	{
		return;
	}

	CCNotificationObserver*		pObserver = new CCNotificationObserver ( pTarget, KD_NULL, szName, KD_NULL );
	if ( !pObserver )
	{
		return;
	}

	pObserver->setHandler ( nHandler );
	pObserver->autorelease ( );
	m_pObservers->addObject ( pObserver );
}

KDvoid CCNotificationCenter::unregisterScriptObserver ( CCObject* pTarget, const KDchar* szName )
{
	CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( m_pObservers, pObject )
	{
		CCNotificationObserver*		pObserver = (CCNotificationObserver*) pObject;
		if ( !pObserver )
		{
			continue;
		}

		if ( !kdStrcmp ( pObserver->getName ( ), szName ) && pObserver->getTarget ( ) == pTarget )
		{
			m_pObservers->removeObject ( pObserver );
		}
	}
}

KDvoid CCNotificationCenter::postNotification ( const KDchar* szName, CCObject* pTarget )
{
    CCArray*   pObserversCopy = CCArray::createWithCapacity ( m_pObservers->count ( ) );
    pObserversCopy->addObjectsFromArray ( m_pObservers );

    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pObserversCopy, pObject )
    {
        CCNotificationObserver*  pObserver = (CCNotificationObserver*) pObject;

        if ( !pObserver )
		{
            continue;
		}
        
		if ( !kdStrcmp ( szName, pObserver->getName ( ) ) && ( pObserver->getObject ( ) == pObject || pObserver->getObject ( ) == KD_NULL || pObject == KD_NULL ) )
		{
			if ( 0 != pObserver->getHandler ( ) )
			{
				CCScriptEngineProtocol*		pEngine = CCScriptEngineManager::sharedManager ( )->getScriptEngine ( );
				pEngine->executeNotificationEvent ( this, szName );
			}
			else
			{
				pObserver->performSelector ( pTarget );
			}
		}
    }
}

KDvoid CCNotificationCenter::postNotification ( const KDchar* szName )
{
    this->postNotification ( szName, KD_NULL );
}

KDint CCNotificationCenter::getObserverHandlerByName ( const KDchar* szName )
{
	if ( KD_NULL == szName || kdStrlen ( szName ) == 0 )
	{
		return -1;
	}

	CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( m_pObservers, pObject )
	{
		CCNotificationObserver*		pObserver = (CCNotificationObserver*) pObject;
		if ( KD_NULL == pObserver )
		{
			continue;
		}

		if ( 0 == kdStrcmp ( pObserver->getName ( ),szName ) )
		{
			return pObserver->getHandler ( );
			break;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
///
/// CCNotificationObserver
///
////////////////////////////////////////////////////////////////////////////////
CCNotificationObserver::CCNotificationObserver ( CCObject* pTarget, SEL_CallFuncO pSelector, const KDchar* szName, CCObject* pObject )
{
    m_pTarget   = pTarget;
    m_pSelector = pSelector;
    m_pObject   = pObject;
	m_nHandler	= 0;

	KDsize  uLen = kdStrlen ( szName );

    m_szName = new KDchar [ uLen + 1 ];
    kdMemset  ( m_szName, 0, uLen + 1 );
	kdStrncpy ( m_szName, szName, uLen );
}

CCNotificationObserver::~CCNotificationObserver ( KDvoid )
{
	CC_SAFE_DELETE_ARRAY ( m_szName );
}

KDvoid CCNotificationObserver::performSelector ( CCObject* pObject )
{
    if ( m_pTarget )
    {
		if ( pObject ) 
		{
			( m_pTarget->*m_pSelector ) ( pObject );
		}
		else 
		{
			( m_pTarget->*m_pSelector ) ( m_pObject );
		}
    }
}

CCObject* CCNotificationObserver::getTarget ( KDvoid )
{
    return m_pTarget;
}

SEL_CallFuncO CCNotificationObserver::getSelector ( KDvoid )
{
    return m_pSelector;
}

KDchar* CCNotificationObserver::getName ( KDvoid )
{
    return m_szName;
}

CCObject* CCNotificationObserver::getObject ( KDvoid )
{
    return m_pObject;
}

KDint CCNotificationObserver::getHandler ( KDvoid )
{
	return m_nHandler;
}

KDvoid CCNotificationObserver::setHandler ( KDint nVar )
{
	m_nHandler = nVar;
}

NS_CC_END
