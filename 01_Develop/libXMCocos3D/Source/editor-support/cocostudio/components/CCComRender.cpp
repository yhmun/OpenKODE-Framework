/* -----------------------------------------------------------------------------------
 *
 *      File            CCComRender.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#include "editor-support/cocostudio/components/CCComRender.h"
#include "2d/base_nodes/CCNode.h"

namespace cocostudio {

ComRender::ComRender(void)
: m_pRender(nullptr)
{
  
}


ComRender::ComRender(cocos2d::Node *node, const char *comName)
{
    m_pRender = node;
    m_sName.assign(comName);
}

ComRender::~ComRender(void)
{
    m_pRender = NULL;
}

void ComRender::onEnter()
{
    if (m_pOwner != NULL)
    {
        m_pOwner->addChild(m_pRender);
    }
}

void ComRender::onExit()
{
    m_pRender = NULL;
}

cocos2d::Node* ComRender::getNode()
{
    return m_pRender;
}

ComRender* ComRender::create(cocos2d::Node *pNode, const char *comName)
{
    ComRender * pRet = new ComRender(pNode, comName);
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

}
