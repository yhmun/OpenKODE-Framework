/* -----------------------------------------------------------------------------------
 *
 *      File            UILayer.cpp
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

#include "gui/System/UILayer.h"
#include "gui/System/UIHelper.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"

using namespace cocos2d;

namespace gui {

UILayer::UILayer():
m_pRootWidget(nullptr),
m_pInputManager(nullptr)
{
    
}

UILayer::~UILayer()
{
    m_pRootWidget->release();
    CC_SAFE_DELETE(m_pInputManager);
}

bool UILayer::init()
{
    if (Layer::init())
    {
        m_pRootWidget = UIRootWidget::create();
        m_pRootWidget->retain();
        addChild(m_pRootWidget->getRenderer());
        m_pInputManager = new UIInputManager();
        m_pInputManager->setRootWidget(m_pRootWidget);
        return true;
    }
    return false;
}

UILayer* UILayer::create(void)
{
    UILayer *pRet = new UILayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
}

void UILayer::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(UILayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(UILayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(UILayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(UILayer::onTouchCancelled, this);
    
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    m_pRootWidget->onEnter();
}

void UILayer::onExit()
{
    m_pRootWidget->onExit();
    Layer::onExit();
}

void UILayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void UILayer::addWidget(UIWidget* widget)
{
    m_pRootWidget->addChild(widget);
}

void UILayer::removeWidget(UIWidget* widget)
{
    m_pRootWidget->removeChild(widget);
}

void UILayer::setVisible(bool visible)
{
    Layer::setVisible(visible);
    m_pRootWidget->setVisible(visible);
}

UIWidget* UILayer::getWidgetByTag(int tag)
{
    if (!m_pRootWidget)
    {
        return nullptr;
    }
    return UIHelper::seekWidgetByTag(m_pRootWidget, tag);
}

UIWidget* UILayer::getWidgetByName(const char* name)
{
    if (!m_pRootWidget)
    {
        return nullptr;
    }
    return UIHelper::seekWidgetByName(m_pRootWidget, name);
}

UIRootWidget* UILayer::getRootWidget()
{
    return m_pRootWidget;
}

UIInputManager* UILayer::getInputManager()
{
    return m_pInputManager;
}

void UILayer::clear()
{
    m_pRootWidget->removeAllChildren();
}

bool UILayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (m_pInputManager && m_pInputManager->onTouchBegan(pTouch))
    {
        return true;
    }
    return false;
}

void UILayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    m_pInputManager->onTouchMoved(pTouch);
}

void UILayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pInputManager->onTouchEnd(pTouch);
}

void UILayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    m_pInputManager->onTouchCancelled(pTouch);
}

}
