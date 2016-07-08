/* -----------------------------------------------------------------------------------
 *
 *      File            CCSet.cpp
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

#include "base/CCSet.h"

using namespace std;

NS_CC_BEGIN

Set::Set(void)
{
    m_pSet = new set<Object *>;
}

Set::Set(const Set &rSetObject)
{
    m_pSet = new set<Object *>(*rSetObject.m_pSet);

    // call retain of members
    SetIterator iter;
    for (iter = m_pSet->begin(); iter != m_pSet->end(); ++iter)
    {
        if (! (*iter))
        {
            break;
        }

        (*iter)->retain();
    }
}

Set::~Set(void)
{
    removeAllObjects();
    CC_SAFE_DELETE(m_pSet);
}

void Set::acceptVisitor(DataVisitor &visitor)
{
    visitor.visit(this);
}

Set * Set::create()
{
    Set * pRet = new Set();
    
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    
    return pRet;
}

Set* Set::copy(void)
{
    Set *pSet = new Set(*this);

    return pSet;
}

Set* Set::mutableCopy(void)
{
    return copy();
}

int Set::count(void)
{
    return (int)m_pSet->size();
}

void Set::addObject(Object *pObject)
{
    if (m_pSet->count(pObject) == 0)
    {
        CC_SAFE_RETAIN(pObject);
        m_pSet->insert(pObject);
    }
}

void Set::removeObject(Object *pObject)
{
    if (m_pSet->erase(pObject) > 0)
    {
        CC_SAFE_RELEASE(pObject);
    }
}

void Set::removeAllObjects()
{
    SetIterator it = m_pSet->begin();
    SetIterator tmp;

    while (it != m_pSet->end())
    {
        if (!(*it))
        {
            break;
        }
        
        tmp = it;
        ++tmp;
        Object * obj = *it;
        m_pSet->erase(it);
        CC_SAFE_RELEASE(obj);
        it = tmp;
    }
}

bool Set::containsObject(Object *pObject)
{
    return m_pSet->find(pObject) != m_pSet->end();
}

SetIterator Set::begin(void)
{
    return m_pSet->begin();
}

SetIterator Set::end(void)
{
    return m_pSet->end();
}

Object* Set::anyObject()
{
    if (!m_pSet || m_pSet->empty())
    {
        return 0;
    }
    
    SetIterator it;

    for( it = m_pSet->begin(); it != m_pSet->end(); ++it)
    {
        if (*it)
        {
            return (*it);
        }
    }

    return 0;
}

NS_CC_END
