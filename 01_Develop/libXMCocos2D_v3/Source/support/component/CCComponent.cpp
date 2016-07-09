/* -----------------------------------------------------------------------------------
 *
 *      File            CCComponent.cpp
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

#include "support/component/CCComponent.h"

NS_CC_BEGIN

Component::Component(void)
: m_pOwner(NULL)
, m_bEnabled(true)
{
}

Component::~Component(void)
{
}

bool Component::init()
{
    return true;
}

void Component::onEnter()
{
}

void Component::onExit()
{
}

void Component::update(float delta)
{
}

void Component::serialize(void *ar)
{
}

Component* Component::create(void)
{
    Component * pRet = new Component();
    if (pRet != NULL && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

const char* Component::getName() const
{
    return m_sName.c_str();
}

void Component::setName(const char *pName)
{
    m_sName.assign(pName);
}

Node* Component::getOwner() const
{
    return m_pOwner;
}

void Component::setOwner(Node *pOwner)
{
    m_pOwner = pOwner;
}

bool Component::isEnabled() const
{
    return m_bEnabled;
}

void Component::setEnabled(bool b)
{
    m_bEnabled = b;
}

NS_CC_END
