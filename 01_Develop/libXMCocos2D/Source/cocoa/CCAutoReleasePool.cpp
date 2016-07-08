/* -----------------------------------------------------------------------------------
 *
 *      File            CCAutoReleasePool.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "cocoa/CCAutoreleasePool.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN

static CCPoolManager*	l_pPoolManager = KD_NULL;

CCAutoReleasePool::CCAutoReleasePool ( KDvoid )
{
	m_pManagedObjectArray = new CCArray ( );
	m_pManagedObjectArray->init ( );
}

CCAutoReleasePool::~CCAutoReleasePool ( KDvoid )
{
	CC_SAFE_DELETE ( m_pManagedObjectArray );
}

KDvoid CCAutoReleasePool::addObject( CCObject* pObject )
{
	m_pManagedObjectArray->addObject ( pObject );

	CCAssert ( pObject->m_uReference > 1, "reference count should be greater than 1" );

	++( pObject->m_uAutoReleaseCount );

	pObject->release ( );                       // no ref count, in this case autorelease pool added.
}

KDvoid CCAutoReleasePool::removeObject ( CCObject* pObject )
{
	for ( KDuint i = 0; i < pObject->m_uAutoReleaseCount; ++i )
	{
		m_pManagedObjectArray->removeObject ( pObject, KD_FALSE );
	}
}

KDvoid CCAutoReleasePool::clear ( KDvoid )
{
	if ( m_pManagedObjectArray->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH_REVERSE ( m_pManagedObjectArray, pObject )
		{
			if ( !pObject )
			{
				break;
			}

			--( pObject->m_uAutoReleaseCount );
		}

		m_pManagedObjectArray->removeAllObjects ( );
	}
}

//--------------------------------------------------------------------
//
// CCPoolManager
//
//--------------------------------------------------------------------

CCPoolManager* CCPoolManager::sharedPoolManager ( KDvoid )
{
	if ( !l_pPoolManager )
	{
		l_pPoolManager = new CCPoolManager ( );
	}

	return l_pPoolManager;
}

KDvoid CCPoolManager::purgePoolManager ( KDvoid )
{
	CC_SAFE_DELETE ( l_pPoolManager );
}

CCPoolManager::CCPoolManager ( KDvoid )
{
	m_pReleasePoolStack = new CCArray ( );	
	m_pReleasePoolStack->init ( );
	m_pCurReleasePool = 0;
}

CCPoolManager::~CCPoolManager ( KDvoid )
{	
	finalize ( );

	// we only release the last autorelease pool here 
	m_pCurReleasePool = KD_NULL;
	m_pReleasePoolStack->removeObjectAtIndex ( 0 );

	CC_SAFE_DELETE ( m_pReleasePoolStack );
}

KDvoid CCPoolManager::finalize ( KDvoid )
{
	if ( m_pReleasePoolStack->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pReleasePoolStack, pObject )
		{
			if ( !pObject )
			{
				break;
			}

			( (CCAutoReleasePool*) pObject )->clear ( );
		}
	}
}

KDvoid CCPoolManager::push ( KDvoid )
{
	CCAutoReleasePool*  pPool = new CCAutoReleasePool ( );      //ref = 1
	
    m_pCurReleasePool = pPool;
	m_pReleasePoolStack->addObject ( pPool );					//ref = 2

	pPool->release ( );											//ref = 1
}

KDvoid CCPoolManager::pop ( KDvoid )
{
    if ( !m_pCurReleasePool )
    {
        return;
    }

 	KDint  nCount = m_pReleasePoolStack->count ( );

	m_pCurReleasePool->clear ( );
 
  	if ( nCount > 1 )
  	{
		m_pReleasePoolStack->removeObjectAtIndex ( nCount - 1 );

		m_pCurReleasePool = (CCAutoReleasePool*) m_pReleasePoolStack->objectAtIndex ( nCount - 2 );
	}
}

KDvoid CCPoolManager::removeObject ( CCObject* pObject )
{
	CCAssert ( m_pCurReleasePool, "current auto release pool should not be null" );

	m_pCurReleasePool->removeObject ( pObject );
}

KDvoid CCPoolManager::addObject ( CCObject* pObject )
{
	getCurReleasePool ( )->addObject ( pObject );
}

CCAutoReleasePool* CCPoolManager::getCurReleasePool ( KDvoid )
{
	if ( !m_pCurReleasePool )
	{
		push ( );
	}

	CCAssert ( m_pCurReleasePool, "current auto release pool should not be null" );

	return m_pCurReleasePool;
}

NS_CC_END