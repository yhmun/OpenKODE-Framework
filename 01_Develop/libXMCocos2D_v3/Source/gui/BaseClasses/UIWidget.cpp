/* -----------------------------------------------------------------------------------
 *
 *      File            UIWidget.cpp
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

#include "gui/BaseClasses/UIWidget.h"
#include "gui/System/UILayer.h"
#include "gui/Layouts/UILayout.h"
#include "gui/System/UIHelper.h"
#include "base/CCDictionary.h"
#include "2d/CCDirector.h"

namespace gui {

#define DYNAMIC_CAST_CCBLENDPROTOCOL dynamic_cast<cocos2d::BlendProtocol*> ( m_pRenderer )

#define DYNAMIC_CAST_CCRGBAPROTOCOL  dynamic_cast<cocos2d::RGBAProtocol*> ( m_pRenderer )

#define DYNAMIC_CAST_CCNODERGBA      dynamic_cast<GUIRenderer*> ( m_pRenderer )
    
UIWidget::UIWidget():
m_bEnabled(true),
m_bVisible(true),
m_bBright(true),
m_bTouchEnabled(false),
m_bTouchPassedEnabled(false),
m_bFocus(false),
m_nWidgetZOrder(0),
m_tAnchorPoint(cocos2d::Point(0.5f, 0.5f)),
m_pWidgetParent(nullptr),
m_eBrightStyle(BRIGHT_NONE),
m_bUpdateEnabled(false),
m_pRenderer(nullptr),
m_tTouchStartPos(cocos2d::Point::ZERO),
m_tTouchMovePos(cocos2d::Point::ZERO),
m_tTouchEndPos(cocos2d::Point::ZERO),
m_pTouchEventListener(nullptr),
m_pTouchEventSelector(nullptr),
m_nWidgetTag(-1),
m_sName("default"),
m_eWidgetType(WidgetTypeWidget),
m_nActionTag(0),
m_tSize(cocos2d::Size::ZERO),
m_tCustomSize(cocos2d::Size::ZERO),
m_pLayoutParameterDictionary(nullptr),
m_bIgnoreSize(false),
m_pChildren(nullptr),
m_bAffectByClipping(false),
m_pScheduler(nullptr),
m_eSizeType(SIZE_ABSOLUTE),
m_tSizePercent(cocos2d::Point::ZERO),
m_ePositionType(POSITION_ABSOLUTE),
m_tPositionPercent(cocos2d::Point::ZERO),
m_bIsRunning(false),
m_pUserObject(nullptr)
{    
	// Added By XMSoft
	m_pPushListener		= nullptr;
	m_pPushSelector		= nullptr;
	m_pMoveListener		= nullptr;
	m_pMoveSelector		= nullptr;
	m_pReleaseListener	= nullptr;
	m_pReleaseSelector	= nullptr;
	m_pCancelListener	= nullptr;
	m_pCancelSelector	= nullptr;
}

UIWidget::~UIWidget()
{
	// Added By XMSoft
	m_pPushListener		= nullptr;
	m_pPushSelector		= nullptr;
	m_pMoveListener		= nullptr;
	m_pMoveSelector		= nullptr;
	m_pReleaseListener	= nullptr;
	m_pReleaseSelector	= nullptr;
	m_pCancelListener	= nullptr;
	m_pCancelSelector	= nullptr;

    m_pTouchEventListener = nullptr;
    m_pTouchEventSelector = nullptr;
    removeAllChildren();
    m_pChildren->release();
    m_pRenderer->removeAllChildrenWithCleanup(true);
    m_pRenderer->removeFromParentAndCleanup(true);
    m_pRenderer->release();
    setParent(nullptr);
    m_pLayoutParameterDictionary->removeAllObjects();
    CC_SAFE_RELEASE(m_pLayoutParameterDictionary);
    CC_SAFE_RELEASE(m_pScheduler);
    CC_SAFE_RELEASE(m_pUserObject);
}

UIWidget* UIWidget::create()
{
    UIWidget* widget = new UIWidget();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool UIWidget::init()
{
    m_pChildren = cocos2d::Array::create();
    m_pChildren->retain();
    m_pLayoutParameterDictionary = cocos2d::Dictionary::create();
    CC_SAFE_RETAIN(m_pLayoutParameterDictionary);
    initRenderer();
    m_pRenderer->retain();
    m_pRenderer->setZOrder(m_nWidgetZOrder);
    cocos2d::RGBAProtocol* renderRGBA = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (renderRGBA)
    {
        renderRGBA->setCascadeColorEnabled(true);
        renderRGBA->setCascadeOpacityEnabled(true);
    }
    setBright(true);
    ignoreContentAdaptWithSize(true);
    m_pScheduler = cocos2d::Director::getInstance()->getScheduler();
    CC_SAFE_RETAIN(m_pScheduler);
    return true;
}

void UIWidget::onEnter()
{
    arrayMakeObjectsPerformSelector(m_pChildren, onEnter, UIWidget*);
    m_bIsRunning = true;
    updateSizeAndPosition();
}

void UIWidget::onExit()
{
    m_bIsRunning = false;
    arrayMakeObjectsPerformSelector(m_pChildren, onExit, UIWidget*);
}
    
void UIWidget::setUserObject(cocos2d::Object *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

bool UIWidget::addChild(UIWidget *child)
{
    if (!child)
    {
        return false;
    }
    if (m_pChildren->containsObject(child))
    {
        return false;
    }
    child->setParent(this);
    int childrenCount = m_pChildren->data->num;
    if (childrenCount <= 0)
    {
        m_pChildren->addObject(child);
    }
    else
    {
        bool seekSucceed = false;
        cocos2d::ccArray* arrayChildren = m_pChildren->data;
        for (int i=childrenCount-1; i>=0; --i)
        {
            UIWidget* widget = (UIWidget*)(arrayChildren->arr[i]);
            if (child->getZOrder() >= widget->getZOrder())
            {
                if (i == childrenCount-1)
                {
                    m_pChildren->addObject(child);
                    seekSucceed = true;
                    break;
                }
                else
                {
                    m_pChildren->insertObject(child, i+1);
                    seekSucceed = true;
                    break;
                }
            }
        }
        if (!seekSucceed)
        {
            m_pChildren->insertObject(child,0);
        }
    }
    child->getRenderer()->setZOrder(child->getZOrder());
    m_pRenderer->addChild(child->getRenderer());
    if (m_bIsRunning)
    {
        child->onEnter();
    }
    return true;
}

bool UIWidget::removeChild(UIWidget *child)
{
    if (!child)
    {
        return false;
    }
    if (m_pChildren->containsObject(child))
    {
        if (m_bIsRunning)
        {
            child->onExit();    
        }
        child->setUpdateEnabled(false);
        child->setParent(nullptr);
        m_pRenderer->removeChild(child->getRenderer());
        m_pChildren->removeObject(child);
        return true;
    }
    return false;
}

void UIWidget::removeFromParent()
{
    if (m_pWidgetParent)
    {
        m_pWidgetParent->removeChild(this);
    }
}

void UIWidget::removeAllChildren()
{
    if (!m_pChildren || m_pChildren->count() <= 0)
    {
        return;
    }
    int times = m_pChildren->data->num;
    for (int i=0; i<times; ++i)
    {
        UIWidget* lastChild = (UIWidget*)(m_pChildren->getLastObject());
        removeChild(lastChild);
    }
}

void UIWidget::reorderChild(UIWidget* child)
{
    CC_SAFE_RETAIN(child);
    m_pChildren->removeObject(child);
    int childrenCount = m_pChildren->data->num;
    if (childrenCount <= 0)
    {
        m_pChildren->addObject(child);
    }
    else
    {
        bool seekSucceed = false;
        cocos2d::ccArray* arrayChildren = m_pChildren->data;
        for (int i=childrenCount-1; i>=0; --i)
        {
            UIWidget* widget = (UIWidget*)(arrayChildren->arr[i]);
            if (child->getZOrder() >= widget->getZOrder())
            {
                if (i == childrenCount-1)
                {
                    m_pChildren->addObject(child);
                    seekSucceed = true;
                    break;
                }
                else
                {
                    m_pChildren->insertObject(child, i+1);
                    seekSucceed = true;
                    break;
                }
            }
        }
        if (!seekSucceed)
        {
            m_pChildren->insertObject(child,0);
        }
    }
    CC_SAFE_RELEASE(child);
}

void UIWidget::setEnabled(bool enabled)
{
    m_bEnabled = enabled;
    GUIRenderer* renderer = DYNAMIC_CAST_CCNODERGBA;
    if (renderer)
    {
        renderer->setEnabled(enabled);
    }
    else
    {
        dynamic_cast<UIRectClippingNode*>(m_pRenderer)->setEnabled(enabled);
    }
    cocos2d::ccArray* arrayChildren = m_pChildren->data;
    int childrenCount = arrayChildren->num;
    for (int i = 0; i < childrenCount; i++)
    {
        UIWidget* child = dynamic_cast<UIWidget*>(arrayChildren->arr[i]);
        child->setEnabled(enabled);
    }
}

UIWidget* UIWidget::getChildByName(const char *name)
{
    return UIHelper::seekWidgetByName(this, name);
}

UIWidget* UIWidget::getChildByTag(int tag)
{
    return UIHelper::seekWidgetByTag(this, tag);
}

cocos2d::Array* UIWidget::getChildren()
{
    return m_pChildren;
}

void UIWidget::initRenderer()
{
    m_pRenderer = GUIRenderer::create();
}

void UIWidget::setSize(const cocos2d::Size &size)
{
    m_tCustomSize = size;
    if (m_bIgnoreSize)
    {
        m_tSize = getContentSize();
    }
    else
    {
        m_tSize = size;
    }
    if (m_bIsRunning && m_pWidgetParent)
    {
        cocos2d::Size pSize = m_pWidgetParent->getSize();
        float spx = 0.0f;
        float spy = 0.0f;
        if (pSize.width > 0.0f)
        {
            spx = m_tCustomSize.width / pSize.width;
        }
        if (pSize.height > 0.0f)
        {
            spy = m_tCustomSize.height / pSize.height;
        }
        m_tSizePercent = cocos2d::Point(spx, spy);
    }
    
    onSizeChanged();
}

void UIWidget::setSizePercent(const cocos2d::Point &percent)
{
    m_tSizePercent = percent;
    cocos2d::Size cSize = m_tCustomSize;
    if (m_bIsRunning)
    {
        cSize = (m_pWidgetParent == nullptr) ? cocos2d::Size::ZERO : cocos2d::Size(m_pWidgetParent->getSize().width * percent.x , m_pWidgetParent->getSize().height * percent.y);
    }
    if (m_bIgnoreSize)
    {
        m_tSize = getContentSize();
    }
    else
    {
        m_tSize = cSize;
    }
    m_tCustomSize = cSize;
    onSizeChanged();
}

void UIWidget::updateSizeAndPosition()
{
    switch (m_eSizeType)
    {
        case SIZE_ABSOLUTE:
        {
            if (m_bIgnoreSize)
            {
                m_tSize = getContentSize();
            }
            else
            {
                m_tSize = m_tCustomSize;
            }
            if (m_pWidgetParent)
            {
                cocos2d::Size pSize = m_pWidgetParent->getSize();
                float spx = 0.0f;
                float spy = 0.0f;
                if (pSize.width > 0.0f)
                {
                    spx = m_tCustomSize.width / pSize.width;
                }
                if (pSize.height > 0.0f)
                {
                    spy = m_tCustomSize.height / pSize.height;
                }
                m_tSizePercent = cocos2d::Point(spx, spy);
            }
            break;
        }
        case SIZE_PERCENT:
        {
            cocos2d::Size cSize = (m_pWidgetParent == NULL) ? cocos2d::Size::ZERO : cocos2d::Size(m_pWidgetParent->getSize().width * m_tSizePercent.x , m_pWidgetParent->getSize().height * m_tSizePercent.y);
            if (m_bIgnoreSize)
            {
                m_tSize = getContentSize();
            }
            else
            {
                m_tSize = cSize;
            }
            m_tCustomSize = cSize;
        }
            break;
        default:
            break;
    }
    onSizeChanged();
    cocos2d::Point absPos = getPosition();
    switch (m_ePositionType)
    {
        case POSITION_ABSOLUTE:
        {
            if (m_pWidgetParent)
            {
                cocos2d::Size pSize = m_pWidgetParent->getSize();
                if (pSize.width <= 0.0f || pSize.height <= 0.0f)
                {
                    m_tPositionPercent = cocos2d::Point::ZERO;
                }
                else
                {
                    m_tPositionPercent = cocos2d::Point(absPos.x / pSize.width, absPos.y / pSize.height);
                }
            }
            else
            {
                m_tPositionPercent = cocos2d::Point::ZERO;
            }
            break;
        }
        case POSITION_PERCENT:
        {
            if (m_pWidgetParent)
            {
                cocos2d::Size parentSize = m_pWidgetParent->getSize();
                absPos = cocos2d::Point(parentSize.width * m_tPositionPercent.x, parentSize.height * m_tPositionPercent.y);
            }
            else
            {
                absPos = cocos2d::Point::ZERO;
            }
            break;
        }
        default:
            break;
    }
    m_pRenderer->setPosition(absPos);
}

void UIWidget::setSizeType(SizeType type)
{
    m_eSizeType = type;
}

SizeType UIWidget::getSizeType() const
{
    return m_eSizeType;
}

void UIWidget::ignoreContentAdaptWithSize(bool ignore)
{
    m_bIgnoreSize = ignore;
    if (m_bIgnoreSize)
    {
        cocos2d::Size s = getContentSize();
        m_tSize = s;
    }
    else
    {
        m_tSize = m_tCustomSize;
    }
    onSizeChanged();
}

bool UIWidget::isIgnoreContentAdaptWithSize() const
{
    return m_bIgnoreSize;
}

const cocos2d::Size& UIWidget::getSize() const
{
    return m_tSize;
}

const cocos2d::Point& UIWidget::getSizePercent() const
{
    return m_tSizePercent;
}

cocos2d::Point UIWidget::getWorldPosition()
{
    return m_pRenderer->convertToWorldSpace(cocos2d::Point::ZERO);
}

cocos2d::Point UIWidget::convertToWorldSpace(const cocos2d::Point& pt)
{
    return m_pRenderer->convertToWorldSpace(pt);
}

cocos2d::Node* UIWidget::getVirtualRenderer()
{
    return m_pRenderer;
}

void UIWidget::onSizeChanged()
{

}

const cocos2d::Size& UIWidget::getContentSize() const
{
    return m_tSize;
}

void UIWidget::setZOrder(int z)
{
    m_nWidgetZOrder = z;
    m_pRenderer->setZOrder(z);
    if (m_pWidgetParent)
    {
        m_pWidgetParent->reorderChild(this);
    }
}

int UIWidget::getZOrder()
{
    return m_nWidgetZOrder;
}

void UIWidget::setTouchEnabled(bool enable)
{
    m_bTouchEnabled = enable;
}

bool UIWidget::isTouchEnabled() const
{
    return m_bTouchEnabled;
}

void UIWidget::setUpdateEnabled(bool enable)
{
    if (enable == m_bUpdateEnabled)
    {
        return;
    }
    m_bUpdateEnabled = enable;
    if (enable)
    {
        if (m_pScheduler)
        {
            m_pScheduler->scheduleUpdateForTarget(this, 0, false);
        }
    }
    else
    {
        if (m_pScheduler)
        {
            m_pScheduler->unscheduleUpdateForTarget(this);
        }
    }
}

bool UIWidget::isUpdateEnabled()
{
    return m_bUpdateEnabled;
}

bool UIWidget::isFocused() const
{
    return m_bFocus;
}

void UIWidget::setFocused(bool fucos)
{
    if (fucos == m_bFocus)
    {
        return;
    }
    m_bFocus = fucos;
    if (m_bBright)
    {
        if (m_bFocus)
        {
            setBrightStyle(BRIGHT_HIGHLIGHT);
        }
        else
        {
            setBrightStyle(BRIGHT_NORMAL);
        }
    }
    else
    {
        onPressStateChangedToDisabled();
    }
}

KDvoid UIWidget::setBright ( KDbool bBright )
{
    m_bBright = bBright;
    if ( m_bBright )
    {
        m_eBrightStyle = BRIGHT_NONE;
        setBrightStyle ( BRIGHT_NORMAL );
    }
    else
    {
        onPressStateChangedToDisabled ( );
    }
}

void UIWidget::setBrightStyle(BrightStyle style)
{
    if (m_eBrightStyle == style)
    {
        return;
    }
    m_eBrightStyle = style;
    switch (m_eBrightStyle)
    {
        case BRIGHT_NORMAL:
            onPressStateChangedToNormal();
            break;
        case BRIGHT_HIGHLIGHT:
            onPressStateChangedToPressed();
            break;
        default:
            break;
    }
}

void UIWidget::onPressStateChangedToNormal()
{
    
}

void UIWidget::onPressStateChangedToPressed()
{
    
}

void UIWidget::onPressStateChangedToDisabled()
{
    
}

void UIWidget::didNotSelectSelf()
{
    
}

bool UIWidget::onTouchBegan(const cocos2d::Point &touchPoint)
{
    setFocused(true);
    m_tTouchStartPos.x = touchPoint.x;
    m_tTouchStartPos.y = touchPoint.y;
    if (m_pWidgetParent)
    {
        m_pWidgetParent->checkChildInfo(0,this,touchPoint);
    }
    pushDownEvent();
    return m_bTouchPassedEnabled;
}

void UIWidget::onTouchMoved(const cocos2d::Point &touchPoint)
{
    m_tTouchMovePos.x = touchPoint.x;
    m_tTouchMovePos.y = touchPoint.y;
    setFocused(hitTest(touchPoint));
    if (m_pWidgetParent)
    {
        m_pWidgetParent->checkChildInfo(1,this,touchPoint);
    }
    moveEvent();
}

void UIWidget::onTouchEnded(const cocos2d::Point &touchPoint)
{
    m_tTouchEndPos.x = touchPoint.x;
    m_tTouchEndPos.y = touchPoint.y;
    bool focus = m_bFocus;
    setFocused(false);
    if (m_pWidgetParent)
    {
        m_pWidgetParent->checkChildInfo(2,this,touchPoint);
    }
    if (focus)
    {
        releaseUpEvent();
    }
    else
    {
        cancelUpEvent();
    }
}

void UIWidget::onTouchCancelled(const cocos2d::Point &touchPoint)
{
    setFocused(false);
    cancelUpEvent();
}

void UIWidget::onTouchLongClicked(const cocos2d::Point &touchPoint)
{
    longClickEvent();
}

KDvoid UIWidget::pushDownEvent ( KDvoid )
{
    if ( m_pTouchEventListener && m_pTouchEventSelector )
    {
        ( m_pTouchEventListener->*m_pTouchEventSelector ) ( this, TOUCH_EVENT_BEGAN );
    }

	// Added By XMSoft
    if ( m_pPushListener && m_pPushSelector )
    {
        ( m_pPushListener->*m_pPushSelector ) ( this );
    }
}

KDvoid UIWidget::moveEvent ( KDvoid )
{
    if ( m_pTouchEventListener && m_pTouchEventSelector )
    {
        ( m_pTouchEventListener->*m_pTouchEventSelector ) ( this, TOUCH_EVENT_MOVED );
    }

	// Added By XMSoft
    if ( m_pMoveListener && m_pMoveSelector )
    {
        ( m_pMoveListener->*m_pMoveSelector ) ( this );
    }
}

KDvoid UIWidget::releaseUpEvent ( KDvoid )
{
    if ( m_pTouchEventListener && m_pTouchEventSelector )
    {
        ( m_pTouchEventListener->*m_pTouchEventSelector ) ( this,TOUCH_EVENT_ENDED );
    }

	// Added By XMSoft
    if ( m_pReleaseListener && m_pReleaseSelector )
    {
        ( m_pReleaseListener->*m_pReleaseSelector ) ( this );
    }
}

KDvoid UIWidget::cancelUpEvent ( KDvoid )
{
    if ( m_pTouchEventListener && m_pTouchEventSelector )
    {
        ( m_pTouchEventListener->*m_pTouchEventSelector ) ( this,TOUCH_EVENT_CANCELED );
    }

	// Added By XMSoft
    if ( m_pCancelListener && m_pCancelSelector )
    {
        ( m_pCancelListener->*m_pCancelSelector ) ( this );
    }
}

void UIWidget::longClickEvent()
{
    
}

KDvoid UIWidget::addTouchEventListener ( cocos2d::Object* pTarget, SEL_TouchEvent pSelector )
{
    m_pTouchEventListener = pTarget;
    m_pTouchEventSelector = pSelector;
}
 
// Added By XMSoft
KDvoid UIWidget::addPushDownEvent ( cocos2d::Object* pTarget, SEL_PushEvent pSelector )
{
	m_pPushListener = pTarget;
	m_pPushSelector = pSelector;
}

KDvoid UIWidget::addMoveEvent ( cocos2d::Object* pTarget, SEL_MoveEvent pSelector )
{
	m_pMoveListener = pTarget;
	m_pMoveSelector = pSelector;
}

KDvoid UIWidget::addReleaseEvent ( cocos2d::Object* pTarget, SEL_ReleaseEvent pSelector )
{
	m_pReleaseListener = pTarget;
	m_pReleaseSelector = pSelector;
}

KDvoid UIWidget::addCancelEvent ( cocos2d::Object* pTarget, SEL_CancelEvent pSelector )
{
	m_pCancelListener = pTarget;
	m_pCancelSelector = pSelector;
}

cocos2d::Node* UIWidget::getRenderer()
{
    return m_pRenderer;
}

void UIWidget::addRenderer(cocos2d::Node* renderer, int zOrder)
{
    m_pRenderer->addChild(renderer, zOrder);
}

void UIWidget::removeRenderer(cocos2d::Node* renderer, bool cleanup)
{
    m_pRenderer->removeChild(renderer,cleanup);
}

bool UIWidget::hitTest(const cocos2d::Point &pt)
{
    cocos2d::Point nsp = m_pRenderer->convertToNodeSpace(pt);
    cocos2d::Rect bb = cocos2d::Rect(-m_tSize.width * m_tAnchorPoint.x, -m_tSize.height * m_tAnchorPoint.y, m_tSize.width, m_tSize.height);
    if (nsp.x >= bb.origin.x && nsp.x <= bb.origin.x + bb.size.width && nsp.y >= bb.origin.y && nsp.y <= bb.origin.y + bb.size.height)
    {
        return true;
    }
    return false;
}

bool UIWidget::clippingParentAreaContainPoint(const cocos2d::Point &pt)
{
    m_bAffectByClipping = false;
    UIWidget* parent = getParent();
    UIWidget* clippingParent = nullptr;
    while (parent)
    {
        UILayout* layoutParent = dynamic_cast<UILayout*>(parent);
        if (layoutParent)
        {
            if (layoutParent->isClippingEnabled())
            {
                m_bAffectByClipping = true;
                clippingParent = layoutParent;
                break;
            }
        }
        parent = parent->getParent();
    }
    
    if (!m_bAffectByClipping)
    {
        return true;
    }
    
    
    if (clippingParent)
    {
        bool bRet = false;
        if (clippingParent->hitTest(pt))
        {
            bRet = true;
        }
        if (bRet)
        {
            return clippingParent->clippingParentAreaContainPoint(pt);
        }
        return false;
    }
    return true;
}

void UIWidget::checkChildInfo(int handleState, UIWidget *sender, const cocos2d::Point &touchPoint)
{
    if (m_pWidgetParent)
    {
        m_pWidgetParent->checkChildInfo(handleState,sender,touchPoint);
    }
}

void UIWidget::setPosition(const cocos2d::Point &pos)
{
    if (m_bIsRunning && m_pWidgetParent)
    {
        cocos2d::Size pSize = m_pWidgetParent->getSize();
        if (pSize.width <= 0.0f || pSize.height <= 0.0f)
        {
            m_tPositionPercent = cocos2d::Point::ZERO;
        }
        else
        {
            m_tPositionPercent = (m_pWidgetParent == NULL) ? cocos2d::Point::ZERO : cocos2d::Point(pos.x / pSize.width, pos.y / pSize.height);
        }
    }
    m_pRenderer->setPosition(pos);
}

void UIWidget::setPositionPercent(const cocos2d::Point &percent)
{
    m_tPositionPercent = percent;
    if (m_bIsRunning && m_pWidgetParent)
    {
        cocos2d::Size parentSize = m_pWidgetParent->getSize();
        cocos2d::Point absPos = cocos2d::Point(parentSize.width * m_tPositionPercent.x, parentSize.height * m_tPositionPercent.y);
        m_pRenderer->setPosition(absPos);
    }
}

void UIWidget::setAnchorPoint(const cocos2d::Point &pt)
{
    m_tAnchorPoint = pt;
    m_pRenderer->setAnchorPoint(pt);
}

void UIWidget::updateAnchorPoint()
{
    setAnchorPoint(m_tAnchorPoint);
}

const cocos2d::Point& UIWidget::getPosition()
{
    return m_pRenderer->getPosition();
}

const cocos2d::Point& UIWidget::getPositionPercent()
{
    return m_tPositionPercent;
}

void UIWidget::setPositionType(PositionType type)
{
    m_ePositionType = type;
}

PositionType UIWidget::getPositionType() const
{
    return m_ePositionType;
}

const cocos2d::Point& UIWidget::getAnchorPoint()
{
    return m_tAnchorPoint;
}

void UIWidget::setScale(float scale)
{
    m_pRenderer->setScale(scale);
}

float UIWidget::getScale()
{
    return m_pRenderer->getScale();
}

void UIWidget::setScaleX(float scaleX)
{
    m_pRenderer->setScaleX(scaleX);
}

float UIWidget::getScaleX()
{
    return m_pRenderer->getScaleX();
}

void UIWidget::setScaleY(float scaleY)
{
    m_pRenderer->setScaleY(scaleY);
}

float UIWidget::getScaleY()
{
    return m_pRenderer->getScaleY();
}

void UIWidget::setRotation(float rotation)
{
    m_pRenderer->setRotation(rotation);
}

float UIWidget::getRotation()
{
    return m_pRenderer->getRotation();
}

void UIWidget::setRotationX(float rotationX)
{
    m_pRenderer->setRotationX(rotationX);
}

float UIWidget::getRotationX()
{
    return m_pRenderer->getRotationX();
}

void UIWidget::setRotationY(float rotationY)
{
    m_pRenderer->setRotationY(rotationY);
}

float UIWidget::getRotationY()
{
    return m_pRenderer->getRotationY();
}

void UIWidget::setVisible(bool visible)
{
    m_bVisible = visible;
    m_pRenderer->setVisible(visible);
}

bool UIWidget::isVisible() const
{
    return m_bVisible;
}

bool UIWidget::isBright() const
{
    return m_bBright;
}

bool UIWidget::isEnabled() const
{
    return m_bEnabled;
}

float UIWidget::getLeftInParent()
{
    return getPosition().x - getAnchorPoint().x * m_tSize.width;;
}

float UIWidget::getBottomInParent()
{
    return getPosition().y - getAnchorPoint().y * m_tSize.height;;
}

float UIWidget::getRightInParent()
{
    return getLeftInParent() + m_tSize.width;
}

float UIWidget::getTopInParent()
{
    return getBottomInParent() + m_tSize.height;
}

UIWidget* UIWidget::getParent()
{
    return m_pWidgetParent;
}

void UIWidget::setParent(UIWidget* parent)
{
	m_pWidgetParent = parent;
}

cocos2d::Action* UIWidget::runAction(cocos2d::Action *action)
{
    return m_pRenderer->runAction(action);
}

void UIWidget::setActionManager(cocos2d::ActionManager *actionManager)
{
    m_pRenderer->setActionManager(actionManager);
}

cocos2d::ActionManager* UIWidget::getActionManager()
{
    return m_pRenderer->getActionManager();
}

void UIWidget::stopAllActions()
{
    m_pRenderer->stopAllActions();
}

void UIWidget::stopAction(cocos2d::Action *action)
{
    m_pRenderer->stopAction(action);
}

void UIWidget::stopActionByTag(int tag)
{
    m_pRenderer->stopActionByTag(tag);
}

cocos2d::Action* UIWidget::getActionByTag(int tag)
{
    return m_pRenderer->getActionByTag(tag);
}  

void UIWidget::setColor(const cocos2d::Color3B &color)
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        rgbap->setColor(color);
    }
}

const cocos2d::Color3B& UIWidget::getColor()
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        return rgbap->getColor();
    }
    return cocos2d::Color3B::WHITE;
}

void UIWidget::setOpacity(int opacity)
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        rgbap->setOpacity(opacity);
    }
}

int UIWidget::getOpacity()
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        return rgbap->getOpacity();
    }
    return 255;
}

bool UIWidget::isCascadeOpacityEnabled()
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        return rgbap->isCascadeOpacityEnabled();
    }
    return false;
}

void UIWidget::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        rgbap->setCascadeOpacityEnabled(cascadeOpacityEnabled);
    }
}

bool UIWidget::isCascadeColorEnabled()
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        return rgbap->isCascadeColorEnabled();
    }
    return false;
}

void UIWidget::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    cocos2d::RGBAProtocol* rgbap = DYNAMIC_CAST_CCRGBAPROTOCOL;
    if (rgbap)
    {
        rgbap->setCascadeColorEnabled(cascadeColorEnabled);
    }
}

void UIWidget::setBlendFunc(cocos2d::BlendFunc blendFunc)
{
    cocos2d::BlendProtocol * blendNode = DYNAMIC_CAST_CCBLENDPROTOCOL;
    if (blendNode)
    {
        blendNode->setBlendFunc(blendFunc);
    }
}

const cocos2d::Point& UIWidget::getTouchStartPos()
{
    return m_tTouchStartPos;
}

const cocos2d::Point& UIWidget::getTouchMovePos()
{
    return m_tTouchMovePos;
}

const cocos2d::Point& UIWidget::getTouchEndPos()
{
    return m_tTouchEndPos;
}

void UIWidget::setTag(int tag)
{
    m_nWidgetTag = tag;
}

int UIWidget::getTag() const
{
    return m_nWidgetTag;
}

void UIWidget::setName(const char* name)
{
    m_sName = name;
}

const char* UIWidget::getName() const
{
    return m_sName.c_str();
}

WidgetType UIWidget::getWidgetType() const
{
    return m_eWidgetType;
}

void UIWidget::setLayoutParameter(UILayoutParameter *parameter)
{
    if (!parameter)
    {
        return;
    }
    m_pLayoutParameterDictionary->setObject(parameter, parameter->getLayoutType());
}

UILayoutParameter* UIWidget::getLayoutParameter(LayoutParameterType type)
{
    return dynamic_cast<UILayoutParameter*>(m_pLayoutParameterDictionary->objectForKey(type));
}

const char* UIWidget::getDescription() const
{
    return "Widget";
}

UIWidget* UIWidget::clone()
{
    UIWidget* clonedWidget = createCloneInstance();
    clonedWidget->copyProperties(this);
    clonedWidget->copyClonedWidgetChildren(this);
    return clonedWidget;
}

UIWidget* UIWidget::createCloneInstance()
{
    return UIWidget::create();
}

void UIWidget::copyClonedWidgetChildren(UIWidget* model)
{
    cocos2d::ccArray* arrayWidgetChildren = model->getChildren()->data;
    int length = arrayWidgetChildren->num;
    for (int i=0; i<length; i++)
    {
        UIWidget* child = (UIWidget*)(arrayWidgetChildren->arr[i]);
        addChild(child->clone());
    }
}

void UIWidget::copySpecialProperties(UIWidget* model)
{
    
}

void UIWidget::copyProperties(UIWidget *widget)
{
    setEnabled(widget->isEnabled());
    setVisible(widget->isVisible());
    setBright(widget->isBright());
    setTouchEnabled(widget->isTouchEnabled());
    m_bTouchPassedEnabled = false;
    setZOrder(widget->getZOrder());
    setUpdateEnabled(widget->isUpdateEnabled());
    setTag(widget->getTag());
    setName(widget->getName());
    setActionTag(widget->getActionTag());
    m_bIgnoreSize = widget->m_bIgnoreSize;
    m_tSize = widget->m_tSize;
    m_tCustomSize = widget->m_tCustomSize;
    copySpecialProperties(widget);
    m_eSizeType = widget->getSizeType();
    m_tSizePercent = widget->m_tSizePercent;
    m_ePositionType = widget->m_ePositionType;
    m_tPositionPercent = widget->m_tPositionPercent;
    setPosition(widget->getPosition());
    setAnchorPoint(widget->getAnchorPoint());
    setScaleX(widget->getScaleX());
    setScaleY(widget->getScaleY());
    setRotation(widget->getRotation());
    setRotationX(widget->getRotationX());
    setRotationY(widget->getRotationY());
    setFlipX(widget->isFlipX());
    setFlipY(widget->isFlipY());
    setColor(widget->getColor());
    setOpacity(widget->getOpacity());
    setCascadeOpacityEnabled(widget->isCascadeOpacityEnabled());
    setCascadeColorEnabled(widget->isCascadeColorEnabled());
    onSizeChanged();
}

/*temp action*/
void UIWidget::setActionTag(int tag)
{
	m_nActionTag = tag;
}

int UIWidget::getActionTag()
{
	return m_nActionTag;
}

KDvoid UIWidget::setActive ( KDbool bActive )
{
	setBright ( bActive );
	setTouchEnabled ( bActive );
}

KDbool UIWidget::isActive ( KDvoid )
{
	return isBright ( );
}

GUIRenderer::GUIRenderer():
m_bEnabled(true)
{
    
}

GUIRenderer::~GUIRenderer()
{
    
}

GUIRenderer* GUIRenderer::create()
{
    GUIRenderer* renderer = new GUIRenderer();
    if (renderer && renderer->init())
    {
        renderer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(renderer);
    }
    return renderer;
}

void GUIRenderer::setEnabled(bool enabled)
{
    m_bEnabled = enabled;
}

bool GUIRenderer::isEnabled() const
{
    return m_bEnabled;
}

void GUIRenderer::visit()
{
    if (!m_bEnabled)
    {
        return;
    }
    cocos2d::NodeRGBA::visit();
}

}
