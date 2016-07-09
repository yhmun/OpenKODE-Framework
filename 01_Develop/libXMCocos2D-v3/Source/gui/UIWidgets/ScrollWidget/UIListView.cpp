/* -----------------------------------------------------------------------------------
 *
 *      File            UIListView.cpp
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

#include "gui/UIWidgets/ScrollWidget/UIListView.h"
#include "gui/System/UIHelper.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"

namespace gui {

UIListView::UIListView():
m_pModel(nullptr),
m_pItems(nullptr),
m_eGravity(LISTVIEW_GRAVITY_CENTER_HORIZONTAL),
m_fItemsMargin(0.0f),
m_pListViewEventListener(nullptr),
m_pListViewEventSelector(nullptr),
m_nCurSelectedIndex(0)
{
    
}

UIListView::~UIListView()
{
    m_pItems->removeAllObjects();
    CC_SAFE_RELEASE(m_pItems);
    m_pListViewEventListener = nullptr;
    m_pListViewEventSelector = nullptr;
}

UIListView* UIListView::create()
{
    UIListView* widget = new UIListView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool UIListView::init()
{
    if (UIScrollView::init())
    {
        m_pItems = cocos2d::Array::create();
        CC_SAFE_RETAIN(m_pItems);
        setLayoutType(LAYOUT_LINEAR_VERTICAL);
        return true;
    }
    return false;
}

void UIListView::setItemModel(UIWidget *model)
{
    if (!model)
    {
        return;
    }
    CC_SAFE_RELEASE(m_pModel);
    m_pModel = model;
    CC_SAFE_RETAIN(m_pModel);
}

void UIListView::updateInnerContainerSize()
{
    if (!m_pModel)
    {
        return;
    }
    switch (m_eDirection) {
        case SCROLLVIEW_DIR_VERTICAL:
        {
            int childrenCount = m_pItems->count();
            float totalHeight = m_pModel->getSize().height * childrenCount + (childrenCount - 1) * m_fItemsMargin;
            float finalWidth = m_tSize.width;
            float finalHeight = totalHeight;
            setInnerContainerSize(cocos2d::Size(finalWidth, finalHeight));
            break;
        }
        case SCROLLVIEW_DIR_HORIZONTAL:
        {
            int childrenCount = m_pItems->count();
            float totalWidth = m_pModel->getSize().width * childrenCount + (childrenCount - 1) * m_fItemsMargin;
            float finalWidth = totalWidth;
            float finalHeight = m_tSize.height;
            setInnerContainerSize(cocos2d::Size(finalWidth, finalHeight));
            break;
        }
        default:
            break;
    }
}

void UIListView::remedyLayoutParameter(UIWidget *item)
{
    if (!item)
    {
        return;
    }
    switch (m_eDirection) {
        case SCROLLVIEW_DIR_VERTICAL:
        {
            UILinearLayoutParameter* llp = (UILinearLayoutParameter*)(item->getLayoutParameter(LAYOUT_PARAMETER_LINEAR));
            if (!llp)
            {
                UILinearLayoutParameter* defaultLp = UILinearLayoutParameter::create();
                switch (m_eGravity) {
                    case LISTVIEW_GRAVITY_LEFT:
                        defaultLp->setGravity(LINEAR_GRAVITY_LEFT);
                        break;
                    case LISTVIEW_GRAVITY_RIGHT:
                        defaultLp->setGravity(LINEAR_GRAVITY_RIGHT);
                        break;
                    case LISTVIEW_GRAVITY_CENTER_HORIZONTAL:
                        defaultLp->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
                        break;
                    default:
                        break;
                }
                if (getIndex(item) == 0)
                {
                    defaultLp->setMargin(UIMarginZero);
                }
                else
                {
                    defaultLp->setMargin(UIMargin(0.0f, m_fItemsMargin, 0.0f, 0.0f));
                }
                item->setLayoutParameter(defaultLp);
            }
            else
            {
                if (getIndex(item) == 0)
                {
                    llp->setMargin(UIMarginZero);
                }
                else
                {
                    llp->setMargin(UIMargin(0.0f, m_fItemsMargin, 0.0f, 0.0f));
                }
                switch (m_eGravity) {
                    case LISTVIEW_GRAVITY_LEFT:
                        llp->setGravity(LINEAR_GRAVITY_LEFT);
                        break;
                    case LISTVIEW_GRAVITY_RIGHT:
                        llp->setGravity(LINEAR_GRAVITY_RIGHT);
                        break;
                    case LISTVIEW_GRAVITY_CENTER_HORIZONTAL:
                        llp->setGravity(LINEAR_GRAVITY_CENTER_HORIZONTAL);
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        case SCROLLVIEW_DIR_HORIZONTAL:
        {
            UILinearLayoutParameter* llp = (UILinearLayoutParameter*)(item->getLayoutParameter(LAYOUT_PARAMETER_LINEAR));
            if (!llp)
            {
                UILinearLayoutParameter* defaultLp = UILinearLayoutParameter::create();
                switch (m_eGravity) {
                    case LISTVIEW_GRAVITY_TOP:
                        defaultLp->setGravity(LINEAR_GRAVITY_TOP);
                        break;
                    case LISTVIEW_GRAVITY_BOTTOM:
                        defaultLp->setGravity(LINEAR_GRAVITY_BOTTOM);
                        break;
                    case LISTVIEW_GRAVITY_CENTER_VERTICAL:
                        defaultLp->setGravity(LINEAR_GRAVITY_CENTER_VERTICAL);
                        break;
                    default:
                        break;
                }
                if (getIndex(item) == 0)
                {
                    defaultLp->setMargin(UIMarginZero);
                }
                else
                {
                    defaultLp->setMargin(UIMargin(m_fItemsMargin, 0.0f, 0.0f, 0.0f));
                }
                item->setLayoutParameter(defaultLp);
            }
            else
            {
                if (getIndex(item) == 0)
                {
                    llp->setMargin(UIMarginZero);
                }
                else
                {
                    llp->setMargin(UIMargin(m_fItemsMargin, 0.0f, 0.0f, 0.0f));
                }
                switch (m_eGravity) {
                    case LISTVIEW_GRAVITY_TOP:
                        llp->setGravity(LINEAR_GRAVITY_TOP);
                        break;
                    case LISTVIEW_GRAVITY_BOTTOM:
                        llp->setGravity(LINEAR_GRAVITY_BOTTOM);
                        break;
                    case LISTVIEW_GRAVITY_CENTER_VERTICAL:
                        llp->setGravity(LINEAR_GRAVITY_CENTER_VERTICAL);
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }
    
}

void UIListView::pushBackDefaultItem()
{
    if (!m_pModel)
    {
        return;
    }
    UIWidget* newItem = m_pModel->clone();
    m_pItems->addObject(newItem);
    remedyLayoutParameter(newItem);
    addChild(newItem);
}

void UIListView::insertDefaultItem(int index)
{
    if (!m_pItems)
    {
        return;
    }
    if (!m_pModel)
    {
        return;
    }
    UIWidget* newItem = m_pModel->clone();
    m_pItems->insertObject(newItem, index);
    remedyLayoutParameter(newItem);
    addChild(newItem);
}

void UIListView::pushBackCustomItem(UIWidget* item)
{
    m_pItems->addObject(item);
    remedyLayoutParameter(item);
    addChild(item);
}

void UIListView::insertCustomItem(UIWidget* item, int index)
{
    m_pItems->insertObject(item, index);
    remedyLayoutParameter(item);
    addChild(item);
}

void UIListView::removeItem(int index)
{
    if (!m_pItems)
    {
        return;
    }
    UIWidget* item = getItem(index);
    if (!item)
    {
        return;
        
    }
    m_pItems->removeObject(item);
    removeChild(item);
}

void UIListView::removeLastItem()
{
    removeItem(m_pItems->count() -1);
}

UIWidget* UIListView::getItem(unsigned int index)
{
    if ((int)index < 0 || (int)index >= m_pItems->count())
    {
        return nullptr;
    }
    return (UIWidget*)(m_pItems->data->arr[index]);
}

cocos2d::Array* UIListView::getItems()
{
    return m_pItems;
}

unsigned int UIListView::getIndex(UIWidget *item) const
{
    if (!m_pItems)
    {
        return -1;
    }
    if (!item)
    {
        return -1;
    }
    return m_pItems->getIndexOfObject(item);
}

void UIListView::setGravity(ListViewGravity gravity)
{
    if (m_eGravity == gravity)
    {
        return;
    }
    m_eGravity = gravity;
    refreshView();
}

void UIListView::setItemsMargin(float margin)
{
    if (m_fItemsMargin == margin)
    {
        return;
    }
    m_fItemsMargin = margin;
    refreshView();
}

void UIListView::setDirection(SCROLLVIEW_DIR dir)
{
    switch (dir)
    {
        case SCROLLVIEW_DIR_VERTICAL:
            setLayoutType(LAYOUT_LINEAR_VERTICAL);
            break;
        case SCROLLVIEW_DIR_HORIZONTAL:
            setLayoutType(LAYOUT_LINEAR_HORIZONTAL);
            break;
        case SCROLLVIEW_DIR_BOTH:
            return;
        default:
            return;
            break;
    }
    UIScrollView::setDirection(dir);
    
}

void UIListView::refreshView()
{
    if (!m_pItems)
    {
        return;
    }
    cocos2d::ccArray* arrayItems = m_pItems->data;
    int length = arrayItems->num;
    for (int i=0; i<length; i++)
    {
        UIWidget* item = (UIWidget*)(arrayItems->arr[i]);
        item->setZOrder(i);
        remedyLayoutParameter(item);
    }
    updateInnerContainerSize();
}
    
void UIListView::addEventListenerListView(cocos2d::Object *target, SEL_ListViewEvent selector)
{
    m_pListViewEventListener = target;
    m_pListViewEventSelector = selector;
}
    
void UIListView::selectedItemEvent()
{
    if (m_pListViewEventListener && m_pListViewEventSelector)
    {
        (m_pListViewEventListener->*m_pListViewEventSelector)(this, LISTVIEW_ONSELECTEDITEM);
    }
}
    
void UIListView::interceptTouchEvent(int handleState, gui::UIWidget *sender, const cocos2d::Point &touchPoint)
{
    UIScrollView::interceptTouchEvent(handleState, sender, touchPoint);
    if (handleState != 1)
    {
        UIWidget* parent = sender;
        while (parent)
        {
            if (parent && parent->getParent() == m_pInnerContainer)
            {
                m_nCurSelectedIndex = getIndex(parent);
                break;
            }
            parent = parent->getParent();
        }
        selectedItemEvent();
    }
}
    
int UIListView::getCurSelectedIndex() const
{
    return m_nCurSelectedIndex;
}

void UIListView::onSizeChanged()
{
    UIScrollView::onSizeChanged();
    refreshView();
}

const char* UIListView::getDescription() const
{
    return "ListViewEx";
}

UIWidget* UIListView::createCloneInstance()
{
    return UIListView::create();
}

void UIListView::copyClonedWidgetChildren(UIWidget* model)
{
    cocos2d::ccArray* arrayItems = dynamic_cast<UIListView*>(model)->getItems()->data;
    int length = arrayItems->num;
    for (int i=0; i<length; i++)
    {
        UIWidget* item = (UIWidget*)(arrayItems->arr[i]);
        pushBackCustomItem(item->clone());
    }
}

void UIListView::copySpecialProperties(UIWidget *widget)
{
    UIListView* listViewEx = dynamic_cast<UIListView*>(widget);
    if (listViewEx)
    {
        UIScrollView::copySpecialProperties(widget);
        setItemModel(listViewEx->m_pModel);
        setItemsMargin(listViewEx->m_fItemsMargin);
        setGravity(listViewEx->m_eGravity);
    }
}

}
