/* -----------------------------------------------------------------------------------
 *
 *      File            UIPageView.cpp
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

#include "gui/UIWidgets/ScrollWidget/UIPageView.h"

namespace gui {

UIPageView::UIPageView():
m_nCurPageIdx(0),
m_pPages(nullptr),
m_tTouchMoveDir(PAGEVIEW_TOUCHLEFT),
m_fTouchStartLocation(0.0f),
m_fTouchMoveStartLocation(0.0f),
m_tMovePagePoint(cocos2d::Point::ZERO),
m_pLeftChild(nullptr),
m_pRightChild(nullptr),
m_fLeftBoundary(0.0f),
m_fRightBoundary(0.0f),
m_bIsAutoScrolling(false),
m_fAutoScrollDistance(0.0f),
m_fAutoScrollSpeed(0.0f),
m_nAutoScrollDir(0),
m_fChildFocusCancelOffset(5.0f),
m_pPageViewEventListener(nullptr),
m_pPageViewEventSelector(nullptr)
{
}

UIPageView::~UIPageView()
{
    m_pPages->removeAllObjects();
    CC_SAFE_RELEASE(m_pPages);
    m_pPageViewEventListener = nullptr;
    m_pPageViewEventSelector = nullptr;
}

UIPageView* UIPageView::create()
{
    UIPageView* widget = new UIPageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool UIPageView::init()
{
    if (UILayout::init())
    {
        m_pPages = cocos2d::Array::create();
        m_pPages->retain();
        setClippingEnabled(true);
        setUpdateEnabled(true);
        setTouchEnabled(true);
        return true;
    }
    return false;
}

void UIPageView::addWidgetToPage(UIWidget *widget, int pageIdx, bool forceCreate)
{
    if (!widget)
    {
        return;
    }
    if (pageIdx < 0)
    {
        return;
    }
    int pageCount = m_pPages->count();
    if (pageIdx < 0 || pageIdx >= pageCount)
    {
        if (forceCreate)
        {
            if (pageIdx > pageCount)
            {
                CCLOG("pageIdx is %d, it will be added as page id [%d]",pageIdx,pageCount);
            }
            UILayout* newPage = createPage();
            newPage->addChild(widget);
            addPage(newPage);
        }
    }
    else
    {
        UILayout * page = dynamic_cast<UILayout*>(m_pPages->getObjectAtIndex(pageIdx));
        if (page)
        {
            page->addChild(widget);
        }
    }
}

UILayout* UIPageView::createPage()
{
    UILayout* newPage = UILayout::create();
    newPage->setSize(getSize());
    return newPage;
}

void UIPageView::addPage(UILayout* page)
{
    if (!page)
    {
        return;
    }
    if (page->getWidgetType() != WidgetTypeContainer)
    {
        return;
    }
    if (m_pPages->containsObject(page))
    {
        return;
    }
    cocos2d::Size pSize = page->getSize();
    cocos2d::Size pvSize = getSize();
    if (!pSize.equals(pvSize))
    {
        CCLOG("page size does not match pageview size, it will be force sized!");
        page->setSize(pvSize);
    }
    page->setPosition(cocos2d::Point(getPositionXByIndex(m_pPages->count()), 0));
    m_pPages->addObject(page);
    addChild(page);
    updateBoundaryPages();
}

void UIPageView::insertPage(UILayout* page, int idx)
{
    if (idx < 0)
    {
        return;
    }
    if (!page)
    {
        return;
    }
    if (page->getWidgetType() != WidgetTypeContainer)
    {
        return;
    }
    if (m_pPages->containsObject(page))
    {
        return;
    }
    
    int pageCount = m_pPages->count();
    if (idx >= pageCount)
    {
        addPage(page);
    }
    else
    {
        m_pPages->insertObject(page, idx);
        page->setPosition(cocos2d::Point(getPositionXByIndex(idx), 0));
        addChild(page);
        cocos2d::Size pSize = page->getSize();
        cocos2d::Size pvSize = getSize();
        if (!pSize.equals(pvSize))
        {
            CCLOG("page size does not match pageview size, it will be force sized!");
            page->setSize(pvSize);
        }
        cocos2d::ccArray* arrayPages = m_pPages->data;
        int length = arrayPages->num;
        for (int i=(idx+1); i<length; i++) {
            UIWidget* behindPage = dynamic_cast<UIWidget*>(arrayPages->arr[i]);
            cocos2d::Point formerPos = behindPage->getPosition();
            behindPage->setPosition(cocos2d::Point(formerPos.x+getSize().width, 0));
        }
        updateBoundaryPages();
    }
}

void UIPageView::removePage(UILayout* page)
{
    if (!page)
    {
        return;
    }
    removeChild(page);
    updateChildrenPosition();
    updateBoundaryPages();
}

void UIPageView::removePageAtIndex(int index)
{
    if (index < 0 || index >= (int)(m_pPages->count()))
    {
        return;
    }
    UILayout* page = dynamic_cast<UILayout*>(m_pPages->getObjectAtIndex(index));
    if (page)
    {
        removePage(page);
    }
}
    
void UIPageView::removeAllPages()
{
    removeAllChildren();
}

void UIPageView::updateBoundaryPages()
{
    if (m_pPages->count() <= 0)
    {
        m_pLeftChild = nullptr;
        m_pRightChild = nullptr;
        return;
    }
    m_pLeftChild = dynamic_cast<UIWidget*>(m_pPages->getObjectAtIndex(0));
    m_pRightChild = dynamic_cast<UIWidget*>(m_pPages->getLastObject());
}

float UIPageView::getPositionXByIndex(int idx)
{
    return (getSize().width*(idx-m_nCurPageIdx));
}

bool UIPageView::addChild(UIWidget* widget)
{
    return UILayout::addChild(widget);
}

bool UIPageView::removeChild(UIWidget* widget)
{
    if (m_pPages->containsObject(widget))
    {
        m_pPages->removeObject(widget);
    }
    return UILayout::removeChild(widget);
}

void UIPageView::onSizeChanged()
{
    UILayout::onSizeChanged();
    m_fRightBoundary = getSize().width;
    updateChildrenSize();
    updateChildrenPosition();
}

void UIPageView::updateChildrenSize()
{
    if (!m_pPages)
    {
        return;
    }
    
    cocos2d::Size selfSize = getSize();
    for (long i=0; i<m_pPages->count(); i++)
    {
        UILayout* page = dynamic_cast<UILayout*>(m_pPages->getObjectAtIndex(i));
        page->setSize(selfSize);
    }
}

void UIPageView::updateChildrenPosition()
{
    if (!m_pPages)
    {
        return;
    }
    
    int pageCount = m_pPages->data->num;
    if (pageCount <= 0)
    {
        m_nCurPageIdx = 0;
        return;
    }
    if (m_nCurPageIdx >= pageCount)
    {
        m_nCurPageIdx = pageCount-1;
    }
    float pageWidth = getSize().width;
    cocos2d::ccArray* arrayPages = m_pPages->data;
    for (int i=0; i<pageCount; i++)
    {
        UILayout* page = dynamic_cast<UILayout*>(arrayPages->arr[i]);
        page->setPosition(cocos2d::Point((i-m_nCurPageIdx)*pageWidth, 0));
    }
}

void UIPageView::removeAllChildren()
{
    m_pPages->removeAllObjects();
    UILayout::removeAllChildren();
}

void UIPageView::scrollToPage(int idx)
{
    if (idx < 0 || idx >= (int)(m_pPages->count()))
    {
        return;
    }
    m_nCurPageIdx = idx;
    UIWidget* curPage = dynamic_cast<UIWidget*>(m_pPages->getObjectAtIndex(idx));
    m_fAutoScrollDistance = -(curPage->getPosition().x);
    m_fAutoScrollSpeed = kdFabsf(m_fAutoScrollDistance)/0.2f;
    m_nAutoScrollDir = m_fAutoScrollDistance > 0 ? 1 : 0;
    m_bIsAutoScrolling = true;
}

void UIPageView::update(float dt)
{
    if (m_bIsAutoScrolling)
    {
        switch (m_nAutoScrollDir)
        {
            case 0:
            {
                float step = m_fAutoScrollSpeed*dt;
                if (m_fAutoScrollDistance + step >= 0.0f)
                {
                    step = -m_fAutoScrollDistance;
                    m_fAutoScrollDistance = 0.0f;
                    m_bIsAutoScrolling = false;
                }
                else
                {
                    m_fAutoScrollDistance += step;
                }
                scrollPages(-step);
                if (!m_bIsAutoScrolling)
                {
                    pageTurningEvent();
                }
                break;
            }
                break;
            case 1:
            {
                float step = m_fAutoScrollSpeed*dt;
                if (m_fAutoScrollDistance - step <= 0.0f)
                {
                    step = m_fAutoScrollDistance;
                    m_fAutoScrollDistance = 0.0f;
                    m_bIsAutoScrolling = false;
                }
                else
                {
                    m_fAutoScrollDistance -= step;
                }
                scrollPages(step);
                if (!m_bIsAutoScrolling)
                {
                    pageTurningEvent();
                }
                break;
            }
            default:
                break;
        }
    }
}

bool UIPageView::onTouchBegan(const cocos2d::Point &touchPoint)
{
    bool pass = UILayout::onTouchBegan(touchPoint);
    handlePressLogic(touchPoint);
    return pass;
}

void UIPageView::onTouchMoved(const cocos2d::Point &touchPoint)
{
    m_tTouchMovePos.x = touchPoint.x;
    m_tTouchMovePos.y = touchPoint.y;
    handleMoveLogic(touchPoint);
    if (m_pWidgetParent)
    {
        m_pWidgetParent->checkChildInfo(1,this,touchPoint);
    }
    moveEvent();
    if (!hitTest(touchPoint))
    {
        setFocused(false);
        onTouchEnded(touchPoint);
    }
}

void UIPageView::onTouchEnded(const cocos2d::Point &touchPoint)
{
    UILayout::onTouchEnded(touchPoint);
    handleReleaseLogic(touchPoint);
}

void UIPageView::movePages(float offset)
{
    cocos2d::ccArray* arrayPages = m_pPages->data;
    int length = arrayPages->num;
    for (int i = 0; i < length; i++)
    {
        UIWidget* child = (UIWidget*)(arrayPages->arr[i]);
        m_tMovePagePoint.x = child->getPosition().x + offset;
        m_tMovePagePoint.y = child->getPosition().y;
        child->setPosition(m_tMovePagePoint);
    }
}

bool UIPageView::scrollPages(float touchOffset)
{
    if (m_pPages->count() <= 0)
    {
        return false;
    }
    
    if (!m_pLeftChild || !m_pRightChild)
    {
        return false;
    }
    
    float realOffset = touchOffset;
    
    switch (m_tTouchMoveDir)
    {
        case PAGEVIEW_TOUCHLEFT: // left
            if (m_pRightChild->getRightInParent() + touchOffset <= m_fRightBoundary)
            {
                realOffset = m_fRightBoundary - m_pRightChild->getRightInParent();
                movePages(realOffset);
                return false;
            }
            break;
            
        case PAGEVIEW_TOUCHRIGHT: // right
            if (m_pLeftChild->getLeftInParent() + touchOffset >= m_fLeftBoundary)
            {
                realOffset = m_fLeftBoundary - m_pLeftChild->getLeftInParent();
                movePages(realOffset);
                return false;
            }
            break;
        default:
            break;
    }
    
    movePages(realOffset);
    return true;
}

void UIPageView::onTouchCancelled(const cocos2d::Point &touchPoint)
{
    UILayout::onTouchCancelled(touchPoint);
}

void UIPageView::handlePressLogic(const cocos2d::Point &touchPoint)
{
    cocos2d::Point nsp = m_pRenderer->convertToNodeSpace(touchPoint);
    m_fTouchMoveStartLocation = nsp.x;
    m_fTouchStartLocation = nsp.x;
}

void UIPageView::handleMoveLogic(const cocos2d::Point &touchPoint)
{
    cocos2d::Point nsp = m_pRenderer->convertToNodeSpace(touchPoint);
    float offset = 0.0;
    float moveX = nsp.x;
    offset = moveX - m_fTouchMoveStartLocation;
    m_fTouchMoveStartLocation = moveX;
    if (offset < 0)
    {
        m_tTouchMoveDir = PAGEVIEW_TOUCHLEFT;
    }
    else if (offset > 0)
    {
        m_tTouchMoveDir = PAGEVIEW_TOUCHRIGHT;
    }
    scrollPages(offset);
}

void UIPageView::handleReleaseLogic(const cocos2d::Point &touchPoint)
{
    if (m_pPages->count() <= 0)
    {
        return;
    }
    UIWidget* curPage = dynamic_cast<UIWidget*>(m_pPages->getObjectAtIndex(m_nCurPageIdx));
    if (curPage)
    {
        cocos2d::Point curPagePos = curPage->getPosition();
        int pageCount = m_pPages->count();
        float curPageLocation = curPagePos.x;
        float pageWidth = getSize().width;
        float boundary = pageWidth/2.0f;
        if (curPageLocation <= -boundary)
        {
            if (m_nCurPageIdx >= pageCount-1)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                scrollToPage(m_nCurPageIdx+1);
            }
        }
        else if (curPageLocation >= boundary)
        {
            if (m_nCurPageIdx <= 0)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                scrollToPage(m_nCurPageIdx-1);
            }
        }
        else
        {
            scrollToPage(m_nCurPageIdx);
        }
    }
}

void UIPageView::checkChildInfo(int handleState,UIWidget* sender, const cocos2d::Point &touchPoint)
{
    interceptTouchEvent(handleState, sender, touchPoint);
}

void UIPageView::interceptTouchEvent(int handleState, UIWidget *sender, const cocos2d::Point &touchPoint)
{
    switch (handleState)
    {
        case 0:
            handlePressLogic(touchPoint);
            break;
        case 1:
        {
            float offset = 0;
            offset = kdFabsf(sender->getTouchStartPos().x - touchPoint.x);
            if (offset > m_fChildFocusCancelOffset)
            {
                sender->setFocused(false);
                handleMoveLogic(touchPoint);
            }
        }
            break;
        case 2:
            handleReleaseLogic(touchPoint);
            break;
            
        case 3:
            break;
    }
}

void UIPageView::pageTurningEvent()
{
    if (m_pPageViewEventListener && m_pPageViewEventSelector)
    {
        (m_pPageViewEventListener->*m_pPageViewEventSelector)(this, PAGEVIEW_EVENT_TURNING);
    }
}

void UIPageView::addEventListenerPageView(cocos2d::Object *target, SEL_PageViewEvent selector)
{
    m_pPageViewEventListener = target;
    m_pPageViewEventSelector = selector;
}

int UIPageView::getCurPageIndex() const
{
    return m_nCurPageIdx;
}

cocos2d::Array* UIPageView::getPages()
{
    return m_pPages;
}
    
UILayout* UIPageView::getPage(int index)
{
    if (index < 0 || index >= (int)(m_pPages->count()))
    {
        return nullptr;
    }
    return (UILayout*)m_pPages->getObjectAtIndex(index);
}

const char* UIPageView::getDescription() const
{
    return "PageView";
}

UIWidget* UIPageView::createCloneInstance()
{
    return UIPageView::create();
}

void UIPageView::copyClonedWidgetChildren(UIWidget* model)
{
    cocos2d::ccArray* arrayPages = dynamic_cast<UIPageView*>(model)->getPages()->data;
    int length = arrayPages->num;
    for (int i=0; i<length; i++)
    {
        UILayout* page = (UILayout*)(arrayPages->arr[i]);
        addPage(dynamic_cast<UILayout*>(page->clone()));
    }
}

void UIPageView::copySpecialProperties(UIWidget *widget)
{
    UIPageView* pageView = dynamic_cast<UIPageView*>(widget);
    if (pageView)
    {
        UILayout::copySpecialProperties(widget);
    }
}

}