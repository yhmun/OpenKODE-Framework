/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionTween.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      lhunath (Maarten Billemont)
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

#include "2d/actions/CCActionTween.h"

NS_CC_BEGIN

ActionTween* ActionTween::create(float aDuration, const char* key, float from, float to)
{
    ActionTween* pRet = new ActionTween();
    if (pRet && pRet->initWithDuration(aDuration, key, from, to))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool ActionTween::initWithDuration(float aDuration, const char* key, float from, float to)
{
    if (ActionInterval::initWithDuration(aDuration)) 
    {
        m_sKey    = key;
        m_fTo       = to;
        m_fFrom     = from;
        return true;
    }

    return false;
}

ActionTween *ActionTween::clone() const
{
	// no copy constructor	
	auto a = new ActionTween();
	a->initWithDuration(m_fDuration, m_sKey.c_str(), m_fFrom, m_fTo);
	a->autorelease();
	return a;
}

void ActionTween::startWithTarget(Node *target)
{
    CCASSERT(dynamic_cast<ActionTweenDelegate*>(target), "target must implement ActionTweenDelegate");
    ActionInterval::startWithTarget(target);
    m_fDelta = m_fTo - m_fFrom;
}

void ActionTween::update(float dt)
{
    dynamic_cast<ActionTweenDelegate*>(m_pTarget)->updateTweenAction(m_fTo  - m_fDelta * (1 - dt), m_sKey.c_str());
}

ActionTween* ActionTween::reverse() const
{
    return ActionTween::create(m_fDuration, m_sKey.c_str(), m_fTo, m_fFrom);
}


NS_CC_END
