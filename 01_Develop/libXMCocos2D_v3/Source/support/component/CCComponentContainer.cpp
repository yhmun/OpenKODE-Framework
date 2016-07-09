/* -----------------------------------------------------------------------------------
 *
 *      File            CCComponentContainer.cpp
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

#include "support/component/CCComponentContainer.h"
#include "support/component/CCComponent.h"
#include "2d/CCDirector.h"

NS_CC_BEGIN

ComponentContainer::ComponentContainer(Node *pNode)
: m_pComponents(NULL)
, m_pOwner(pNode)
{
}

ComponentContainer::~ComponentContainer(void)
{
    CC_SAFE_RELEASE(m_pComponents);
}

Component* ComponentContainer::get(const char *pName) const
{
    Component* pRet = NULL;
    CCASSERT(pName != NULL, "Argument must be non-nil");
    do {
        CC_BREAK_IF(NULL == pName);
        CC_BREAK_IF(NULL == m_pComponents);
        pRet = dynamic_cast<Component*>(m_pComponents->objectForKey(pName));
        
    } while (0);
    return pRet;
}

bool ComponentContainer::add(Component *pCom)
{
    bool bRet = false;
    CCASSERT(pCom != NULL, "Argument must be non-nil");
    CCASSERT(pCom->getOwner() == NULL, "Component already added. It can't be added again");
    do
    {
        if (m_pComponents == NULL)
        {
            m_pComponents = Dictionary::create();
            m_pComponents->retain();
            m_pOwner->scheduleUpdate();
        }
        Component *pComponent = dynamic_cast<Component*>(m_pComponents->objectForKey(pCom->getName()));
        
        CCASSERT(pComponent == NULL, "Component already added. It can't be added again");
        CC_BREAK_IF(pComponent);
        pCom->setOwner(m_pOwner);
        m_pComponents->setObject(pCom, pCom->getName());
        pCom->onEnter();
        bRet = true;
    } while(0);
    return bRet;
}

bool ComponentContainer::remove(const char *pName)
{
    bool bRet = false;
    CCASSERT(pName != NULL, "Argument must be non-nil");
    do 
    {        
        CC_BREAK_IF(!m_pComponents);
        Object* pRetObject = NULL;
        DictElement *pElement = NULL;
        HASH_FIND_PTR(m_pComponents->m_pElements, pName, pElement);
        if (pElement != NULL)
        {
           pRetObject = pElement->getObject();
        }
        Component *com = dynamic_cast<Component*>(pRetObject);
        CC_BREAK_IF(!com);
        com->onExit();
        com->setOwner(NULL);
        HASH_DEL(m_pComponents->m_pElements, pElement);
        pElement->getObject()->release();
        CC_SAFE_DELETE(pElement);
        bRet = true;
    } while(0);
    return bRet;
 }

void ComponentContainer::removeAll()
{
    if (m_pComponents != NULL)
    {
        DictElement *pElement, *tmp;
        HASH_ITER(hh, m_pComponents->m_pElements, pElement, tmp)
        {
            HASH_DEL(m_pComponents->m_pElements, pElement);
            ((Component*)pElement->getObject())->onExit();
            ((Component*)pElement->getObject())->setOwner(NULL);
            pElement->getObject()->release();
            CC_SAFE_DELETE(pElement);
        }
        m_pOwner->unscheduleUpdate();
    }
}

void ComponentContainer::alloc(void)
{
    m_pComponents = Dictionary::create();
    m_pComponents->retain();
}

void ComponentContainer::visit(float fDelta)
{
    if (m_pComponents != NULL)
    {
        DictElement *pElement, *tmp;
        HASH_ITER(hh, m_pComponents->m_pElements, pElement, tmp)
        {
            ((Component*)pElement->getObject())->update(fDelta);
        }
    }
}

bool ComponentContainer::isEmpty() const
{
    return (bool)(!(m_pComponents && m_pComponents->count()));
}

NS_CC_END
