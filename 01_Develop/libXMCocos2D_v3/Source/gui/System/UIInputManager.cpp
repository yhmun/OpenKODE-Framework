/* -----------------------------------------------------------------------------------
 *
 *      File            UIInputManager.cpp
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

#include "gui/System/UIInputManager.h"
#include "gui/System/UIHelper.h"

 using namespace cocos2d;

namespace gui {

UIInputManager::UIInputManager():
m_pManageredWidget(nullptr),
m_bTouchDown(false),
m_fLongClickTime(0.0),
m_fLongClickRecordTime(0.0),
m_pCheckedDoubleClickWidget(nullptr),
m_pRootWidget(nullptr)
{
    m_pManageredWidget = Array::create();
    m_pManageredWidget->retain();
    m_pCheckedDoubleClickWidget = Array::create();
    m_pCheckedDoubleClickWidget->retain();
    m_pSelectedWidgets = Array::create();
    m_pSelectedWidgets->retain();
}

UIInputManager::~UIInputManager()
{
    m_pManageredWidget->removeAllObjects();
    CC_SAFE_RELEASE(m_pManageredWidget);
    m_pCheckedDoubleClickWidget->removeAllObjects();
    CC_SAFE_RELEASE(m_pCheckedDoubleClickWidget);
    m_pSelectedWidgets->removeAllObjects();
    CC_SAFE_RELEASE(m_pSelectedWidgets);
}

void UIInputManager::registWidget(UIWidget* widget)
{
    if (!widget)
    {
        return;
    }
    if (m_pManageredWidget->containsObject(widget))
    {
        return;
    }
    m_pManageredWidget->addObject(widget);
}

bool UIInputManager::checkTouchEvent(UIWidget *root, const Point &touchPoint)
{
    ccArray* arrayRootChildren = root->getChildren()->data;
    int length = arrayRootChildren->num;
    for (int i=length-1; i >= 0; i--)
    {
        UIWidget* widget = (UIWidget*)(arrayRootChildren->arr[i]);
        if (checkTouchEvent(widget, touchPoint))
        {
            return true;
        }
    }
    if (root->isEnabled() && root->isTouchEnabled() && root->hitTest(touchPoint) && root->clippingParentAreaContainPoint(touchPoint))
    {
        m_pSelectedWidgets->addObject(root);
        root->onTouchBegan(touchPoint);
        return true;
    }
    return false;
}

void UIInputManager::removeManageredWidget(UIWidget* widget)
{
    if (!widget)
    {
        return;
    }
    if (!m_pManageredWidget->containsObject(widget))
    {
        return;
    }
    m_pManageredWidget->removeObject(widget);
}

bool UIInputManager::checkEventWidget(const Point &touchPoint)
{
    checkTouchEvent(m_pRootWidget,touchPoint);
    return (m_pSelectedWidgets->count() > 0);
}

void UIInputManager::addCheckedDoubleClickWidget(UIWidget* widget)
{
    if (m_pCheckedDoubleClickWidget->containsObject(widget))
    {
        return;
    }
    m_pCheckedDoubleClickWidget->addObject(widget);
}

void UIInputManager::update(float dt)
{
    if (m_bTouchDown)
    {
        m_fLongClickRecordTime += dt;
        if (m_fLongClickRecordTime >= m_fLongClickTime)
        {
            m_fLongClickRecordTime = 0;
            m_bTouchDown = false;
//            m_pCurSelectedWidget->onTouchLongClicked(touchBeganedPoint);
        }
    }
    ccArray* arrayWidget = m_pCheckedDoubleClickWidget->data;
    int widgetCount = arrayWidget->num;
    for (int i=0;i<widgetCount;i++)
    {
        UIWidget* widget = (UIWidget*)(arrayWidget->arr[i]);
        if (!widget->isVisible())
        {
            continue;
        }
    }
}

bool UIInputManager::onTouchBegan(Touch* touch)
{
    m_tTouchBeganedPoint.x = touch->getLocation().x;
    m_tTouchBeganedPoint.y = touch->getLocation().y;
    m_bTouchDown = true;
    return checkEventWidget(m_tTouchBeganedPoint);
}

void UIInputManager::onTouchMoved(Touch* touch)
{
    m_tTouchMovedPoint.x = touch->getLocation().x;
    m_tTouchMovedPoint.y = touch->getLocation().y;
    ccArray* selectedWidgetArray = m_pSelectedWidgets->data;
    int length = selectedWidgetArray->num;
    for (int i=0; i<length; ++i)
    {
        UIWidget* hitWidget = (UIWidget*)(selectedWidgetArray->arr[i]);
        hitWidget->onTouchMoved(m_tTouchMovedPoint);
    }
    if (m_bTouchDown)
    {
        m_fLongClickRecordTime = 0;
        m_bTouchDown = false;
    }
}

void UIInputManager::onTouchEnd(Touch* touch)
{
    m_bTouchDown = false;
    m_tTouchEndedPoint.x = touch->getLocation().x;
    m_tTouchEndedPoint.y = touch->getLocation().y;
    ccArray* selectedWidgetArray = m_pSelectedWidgets->data;
    int length = selectedWidgetArray->num;
    for (int i=0; i<length; ++i)
    {
        UIWidget* hitWidget = (UIWidget*)(selectedWidgetArray->arr[0]);
        m_pSelectedWidgets->removeObject(hitWidget);
        hitWidget->onTouchEnded(m_tTouchEndedPoint);
    }
}

void UIInputManager::onTouchCancelled(Touch* touch)
{
    m_bTouchDown = false;
    m_tTouchEndedPoint.x = touch->getLocation().x;
    m_tTouchEndedPoint.y = touch->getLocation().y;
    ccArray* selectedWidgetArray = m_pSelectedWidgets->data;
    int length = selectedWidgetArray->num;
    for (int i=0; i<length; ++i)
    {
        UIWidget* hitWidget = (UIWidget*)(selectedWidgetArray->arr[0]);
        m_pSelectedWidgets->removeObject(hitWidget);
        hitWidget->onTouchCancelled(m_tTouchEndedPoint);
    }
}

void UIInputManager::setRootWidget(UIWidget *root)
{
    m_pRootWidget = root;
}

UIWidget* UIInputManager::getRootWidget()
{
    return m_pRootWidget;
}

}