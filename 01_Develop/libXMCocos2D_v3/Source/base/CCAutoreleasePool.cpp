/* -----------------------------------------------------------------------------------
 *
 *      File            CCAutoReleasePool.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#include "base/CCAutoreleasePool.h"
#include "ccMacros.h"

NS_CC_BEGIN

static PoolManager* s_pPoolManager = NULL;

AutoreleasePool::AutoreleasePool()
{
    m_pManagedObjectArray = new Array();
    m_pManagedObjectArray->initWithCapacity(150);
}

AutoreleasePool::~AutoreleasePool()
{
    CCLOGINFO("deallocing AutoreleasePool: %p", this);
    CC_SAFE_DELETE(m_pManagedObjectArray);
}

void AutoreleasePool::addObject(Object* object)
{
    m_pManagedObjectArray->addObject(object);

    CCASSERT(object->m_uReference > 1, "reference count should be greater than 1");
    ++(object->m_uAutoReleaseCount);
    object->release(); // no ref count, in this case autorelease pool added.
}

void AutoreleasePool::removeObject(Object* object)
{
    for (unsigned int i = 0; i < object->m_uAutoReleaseCount; ++i)
    {
        m_pManagedObjectArray->removeObject(object, false);
    }
}

void AutoreleasePool::clear()
{
    if(m_pManagedObjectArray->count() > 0)
    {
        //CCAutoreleasePool* pReleasePool;
#ifdef _DEBUG
        int nIndex = m_pManagedObjectArray->count() - 1;
#endif

        Object* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(m_pManagedObjectArray, pObj)
        {
            if(!pObj)
                break;

            --(pObj->m_uAutoReleaseCount);
            //(*it)->release();
            //delete (*it);
#ifdef _DEBUG
            nIndex--;
#endif
        }

        m_pManagedObjectArray->removeAllObjects();
    }
}


//--------------------------------------------------------------------
//
// PoolManager
//
//--------------------------------------------------------------------

PoolManager* PoolManager::sharedPoolManager()
{
    if (s_pPoolManager == NULL)
    {
        s_pPoolManager = new PoolManager();
    }
    return s_pPoolManager;
}

void PoolManager::purgePoolManager()
{
    CC_SAFE_DELETE(s_pPoolManager);
}

PoolManager::PoolManager()
{
    m_pReleasePoolStack = new Array();    
    m_pReleasePoolStack->initWithCapacity(150);
    m_pCurReleasePool = 0;
}

PoolManager::~PoolManager()
{
    CCLOGINFO("deallocing PoolManager: %p", this);
    finalize();
 
     // we only release the last autorelease pool here 
    m_pCurReleasePool = 0;
    m_pReleasePoolStack->removeObjectAtIndex(0);
 
    CC_SAFE_DELETE(m_pReleasePoolStack);
}

void PoolManager::finalize()
{
    if(m_pReleasePoolStack->count() > 0)
    {
        //CCAutoreleasePool* pReleasePool;
        Object* pObj = NULL;
        CCARRAY_FOREACH(m_pReleasePoolStack, pObj)
        {
            if(!pObj)
                break;
            AutoreleasePool* pPool = static_cast<AutoreleasePool*>(pObj);
            pPool->clear();
        }
    }
}

void PoolManager::push()
{
    AutoreleasePool* pPool = new AutoreleasePool();       //ref = 1
    m_pCurReleasePool = pPool;

    m_pReleasePoolStack->addObject(pPool);                   //ref = 2

    pPool->release();                                       //ref = 1
}

void PoolManager::pop()
{
    if (! m_pCurReleasePool)
    {
        return;
    }

    int nCount = m_pReleasePoolStack->count();

    m_pCurReleasePool->clear();
 
    if (nCount > 1)
    {
        m_pReleasePoolStack->removeObjectAtIndex(nCount-1);

//         if(nCount > 1)
//         {
//             _curReleasePool = _releasePoolStack->getObjectAtIndex(nCount - 2);
//             return;
//         }
        m_pCurReleasePool = (AutoreleasePool*)m_pReleasePoolStack->getObjectAtIndex(nCount - 2);
    }

    /*_curReleasePool = NULL;*/
}

void PoolManager::removeObject(Object* object)
{
    CCASSERT(m_pCurReleasePool, "current auto release pool should not be null");

    m_pCurReleasePool->removeObject(object);
}

void PoolManager::addObject(Object* object)
{
    getCurReleasePool()->addObject(object);
}


AutoreleasePool* PoolManager::getCurReleasePool()
{
    if(!m_pCurReleasePool)
    {
        push();
    }

    CCASSERT(m_pCurReleasePool, "current auto release pool should not be null");

    return m_pCurReleasePool;
}

NS_CC_END
