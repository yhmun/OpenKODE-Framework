/* -----------------------------------------------------------------------------------
 *
 *      File            UIScrollView.cpp
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

#include "gui/UIWidgets/ScrollWidget/UIScrollView.h"

namespace gui {

#define AUTOSCROLLMAXSPEED 1000.0f

const cocos2d::Point SCROLLDIR_UP = cocos2d::Point(0.0f, 1.0f);
const cocos2d::Point SCROLLDIR_DOWN = cocos2d::Point(0.0f, -1.0f);
const cocos2d::Point SCROLLDIR_LEFT = cocos2d::Point(-1.0f, 0.0f);
const cocos2d::Point SCROLLDIR_RIGHT = cocos2d::Point(1.0f, 0.0f);

UIScrollView::UIScrollView():
m_pInnerContainer(nullptr),
m_eDirection(SCROLLVIEW_DIR_VERTICAL),
m_tTouchBeganPoint(cocos2d::Point::ZERO),
m_tTouchMovedPoint(cocos2d::Point::ZERO),
m_tTouchEndedPoint(cocos2d::Point::ZERO),
m_tTouchMovingPoint(cocos2d::Point::ZERO),
m_tAutoScrollDir(cocos2d::Point::ZERO),
m_fTopBoundary(0.0f),
m_fBottomBoundary(0.0f),
m_fLeftBoundary(0.0f),
m_fRightBoundary(0.0f),
m_fBounceTopBoundary(0.0f),
m_fBounceBottomBoundary(0.0f),
m_fBounceLeftBoundary(0.0f),
m_fBounceRightBoundary(0.0f),
m_bAutoScroll(false),
m_fAutoScrollAddUpTime(0.0f),
m_fAutoScrollOriginalSpeed(0.0f),
m_fAutoScrollAcceleration(-1000.0f),
m_bIsAutoScrollSpeedAttenuated(false),
m_bNeedCheckAutoScrollDestination(false),
m_tAutoScrollDestination(cocos2d::Point::ZERO),
m_bBePressed(false),
m_fSlidTime(0.0f),
m_tMoveChildPoint(cocos2d::Point::ZERO),
m_fChildFocusCancelOffset(5.0f),
m_bLeftBounceNeeded(false),
m_bTopBounceNeeded(false),
m_bRightBounceNeeded(false),
m_bBottomBounceNeeded(false),
m_bBounceEnabled(false),
m_bBouncing(false),
m_tBounceDir(cocos2d::Point::ZERO),
m_fBounceOriginalSpeed(0.0f),
m_bInertiaScrollEnabled(true),
m_pScrollViewEventListener(nullptr),
m_pScrollViewEventSelector(nullptr)
{
}

UIScrollView::~UIScrollView()
{
    m_pScrollViewEventListener = nullptr;
    m_pScrollViewEventSelector = nullptr;
}

UIScrollView* UIScrollView::create()
{
    UIScrollView* widget = new UIScrollView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool UIScrollView::init()
{
    if (UILayout::init())
    {
        setUpdateEnabled(true);
        setTouchEnabled(true);
        setClippingEnabled(true);
        m_pInnerContainer->setTouchEnabled(false);
        return true;
    }
    return false;
}

void UIScrollView::initRenderer()
{
    UILayout::initRenderer();
    m_pInnerContainer = UILayout::create();
    UILayout::addChild(m_pInnerContainer);
}

void UIScrollView::onSizeChanged()
{
    UILayout::onSizeChanged();
    m_fTopBoundary = m_tSize.height;
    m_fRightBoundary = m_tSize.width;
    float bounceBoundaryParameterX = m_tSize.width / 3.0f;
    float bounceBoundaryParameterY = m_tSize.height / 3.0f;
    m_fBounceTopBoundary = m_tSize.height - bounceBoundaryParameterY;
    m_fBounceBottomBoundary = bounceBoundaryParameterY;
    m_fBounceLeftBoundary = bounceBoundaryParameterX;
    m_fBounceRightBoundary = m_tSize.width - bounceBoundaryParameterX;
    cocos2d::Size innerSize = m_pInnerContainer->getSize();
    float orginInnerSizeWidth = innerSize.width;
    float orginInnerSizeHeight = innerSize.height;
    float innerSizeWidth = KD_MAX(orginInnerSizeWidth, m_tSize.width);
    float innerSizeHeight = KD_MAX(orginInnerSizeHeight, m_tSize.height);
    m_pInnerContainer->setSize(cocos2d::Size(innerSizeWidth, innerSizeHeight));
    m_pInnerContainer->setPosition(cocos2d::Point(0, m_tSize.height - m_pInnerContainer->getSize().height));
}

void UIScrollView::setInnerContainerSize(const cocos2d::Size &size)
{
    float innerSizeWidth = m_tSize.width;
    float innerSizeHeight = m_tSize.height;
    cocos2d::Size originalInnerSize = m_pInnerContainer->getSize();
    if (size.width < m_tSize.width)
    {
        CCLOG("Inner width <= scrollview width, it will be force sized!");
    }
    else
    {
        innerSizeWidth = size.width;
    }
    if (size.height < m_tSize.height)
    {
        CCLOG("Inner height <= scrollview height, it will be force sized!");
    }
    else
    {
        innerSizeHeight = size.height;
    }
    m_pInnerContainer->setSize(cocos2d::Size(innerSizeWidth, innerSizeHeight));

    switch (m_eDirection)
    {
        case SCROLLVIEW_DIR_VERTICAL:
        {
            cocos2d::Size newInnerSize = m_pInnerContainer->getSize();
            float offset = originalInnerSize.height - newInnerSize.height;
            scrollChildren(0.0f, offset);
            break;
        }
        case SCROLLVIEW_DIR_HORIZONTAL:
        {
            if (m_pInnerContainer->getRightInParent() <= m_tSize.width)
            {
                cocos2d::Size newInnerSize = m_pInnerContainer->getSize();
                float offset = originalInnerSize.width - newInnerSize.width;
                scrollChildren(offset, 0.0f);
            }
            break;
        }
        case SCROLLVIEW_DIR_BOTH:
        {
            cocos2d::Size newInnerSize = m_pInnerContainer->getSize();
            float offsetY = originalInnerSize.height - newInnerSize.height;
            float offsetX = 0.0f;
            if (m_pInnerContainer->getRightInParent() <= m_tSize.width)
            {
                offsetX = originalInnerSize.width - newInnerSize.width;
            }
            scrollChildren(offsetX, offsetY);
            break;
        }
        default:
            break;
    }
}

const cocos2d::Size& UIScrollView::getInnerContainerSize() const
{
	return m_pInnerContainer->getSize();
}

bool UIScrollView::addChild(UIWidget* widget)
{
    return m_pInnerContainer->addChild(widget);
}

void UIScrollView::removeAllChildren()
{
    m_pInnerContainer->removeAllChildren();
}

bool UIScrollView::removeChild(UIWidget* child)
{
	return m_pInnerContainer->removeChild(child);
}

cocos2d::Array* UIScrollView::getChildren()
{
    return m_pInnerContainer->getChildren();
}

void UIScrollView::moveChildren(float offsetX, float offsetY)
{
    m_tMoveChildPoint = m_pInnerContainer->getPosition() + cocos2d::Point(offsetX, offsetY);
    m_pInnerContainer->setPosition(m_tMoveChildPoint);
}

void UIScrollView::autoScrollChildren(float dt)
{
    float lastTime = m_fAutoScrollAddUpTime;
    m_fAutoScrollAddUpTime += dt;
    if (m_bIsAutoScrollSpeedAttenuated)
    {
        float nowSpeed = m_fAutoScrollOriginalSpeed + m_fAutoScrollAcceleration * m_fAutoScrollAddUpTime;
        if (nowSpeed <= 0.0f)
        {
            stopAutoScrollChildren();
            checkNeedBounce();
        }
        else
        {
            float timeParam = lastTime * 2 + dt;
            float offset = (m_fAutoScrollOriginalSpeed + m_fAutoScrollAcceleration * timeParam * 0.5f) * dt;
            float offsetX = offset * m_tAutoScrollDir.x;
            float offsetY = offset * m_tAutoScrollDir.y;
            if (!scrollChildren(offsetX, offsetY))
            {
                stopAutoScrollChildren();
                checkNeedBounce();
            }
        }
    }
    else
    {
        if (m_bNeedCheckAutoScrollDestination)
        {
            float xOffset = m_tAutoScrollDir.x * dt * m_fAutoScrollOriginalSpeed;
            float yOffset = m_tAutoScrollDir.y * dt * m_fAutoScrollOriginalSpeed;
            bool notDone = checkCustomScrollDestination(&xOffset, &yOffset);
            bool scrollCheck = scrollChildren(xOffset, yOffset);
            if (!notDone || !scrollCheck)
            {
                stopAutoScrollChildren();
                checkNeedBounce();
            }
        }
        else
        {
            if (!scrollChildren(m_tAutoScrollDir.x * dt * m_fAutoScrollOriginalSpeed, m_tAutoScrollDir.y * dt * m_fAutoScrollOriginalSpeed))
            {
                stopAutoScrollChildren();
                checkNeedBounce();
            }
        }
    }
}

void UIScrollView::bounceChildren(float dt)
{
    if (m_fBounceOriginalSpeed <= 0.0f)
    {
        stopBounceChildren();
    }
    if (!bounceScrollChildren(m_tBounceDir.x * dt * m_fBounceOriginalSpeed, m_tBounceDir.y * dt * m_fBounceOriginalSpeed))
    {
        stopBounceChildren();
    }
}

bool UIScrollView::checkNeedBounce()
{
    if (!m_bBounceEnabled)
    {
        return false;
    }
    checkBounceBoundary();
    if (m_bTopBounceNeeded || m_bBottomBounceNeeded || m_bLeftBounceNeeded || m_bRightBounceNeeded)
    {
        if (m_bTopBounceNeeded && m_bLeftBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point(0.0f, m_tSize.height) - cocos2d::Point(m_pInnerContainer->getLeftInParent(), m_pInnerContainer->getTopInParent());
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bTopBounceNeeded && m_bRightBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point(m_tSize.width, m_tSize.height) - cocos2d::Point(m_pInnerContainer->getRightInParent(), m_pInnerContainer->getTopInParent());
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bBottomBounceNeeded && m_bLeftBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point::ZERO - cocos2d::Point(m_pInnerContainer->getLeftInParent(), m_pInnerContainer->getBottomInParent());
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bBottomBounceNeeded && m_bRightBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point(m_tSize.width, 0.0f) - cocos2d::Point(m_pInnerContainer->getRightInParent(), m_pInnerContainer->getBottomInParent());
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bTopBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point(0.0f, m_tSize.height) - cocos2d::Point(0.0f, m_pInnerContainer->getTopInParent());
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bBottomBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point::ZERO - cocos2d::Point(0.0f, m_pInnerContainer->getBottomInParent());
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bLeftBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point::ZERO - cocos2d::Point(m_pInnerContainer->getLeftInParent(), 0.0f);
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        else if (m_bRightBounceNeeded)
        {
            cocos2d::Point scrollVector = cocos2d::Point(m_tSize.width, 0.0f) - cocos2d::Point(m_pInnerContainer->getRightInParent(), 0.0f);
            float orSpeed = scrollVector.getLength()/(0.2f);
            m_tBounceDir = scrollVector.normalize();
            startBounceChildren(orSpeed);
        }
        return true;
    }
    return false;
}

void UIScrollView::checkBounceBoundary()
{
    float icBottomPos = m_pInnerContainer->getBottomInParent();
    if (icBottomPos > m_fBottomBoundary)
    {
        scrollToBottomEvent();
        m_bBottomBounceNeeded = true;
    }
    else
    {
        m_bBottomBounceNeeded = false;
    }
    float icTopPos = m_pInnerContainer->getTopInParent();
    if (icTopPos < m_fTopBoundary)
    {
        scrollToTopEvent();
        m_bTopBounceNeeded = true;
    }
    else
    {
        m_bTopBounceNeeded = false;
    }
    float icRightPos = m_pInnerContainer->getRightInParent();
    if (icRightPos < m_fRightBoundary)
    {
        scrollToRightEvent();
        m_bRightBounceNeeded = true;
    }
    else
    {
        m_bRightBounceNeeded = false;
    }
    float icLeftPos = m_pInnerContainer->getLeftInParent();
    if (icLeftPos > m_fLeftBoundary)
    {
        scrollToLeftEvent();
        m_bLeftBounceNeeded = true;
    }
    else
    {
        m_bLeftBounceNeeded = false;
    }
}

void UIScrollView::startBounceChildren(float v)
{
    m_fBounceOriginalSpeed = v;
    m_bBouncing = true;
}

void UIScrollView::stopBounceChildren()
{
    m_bBouncing = false;
    m_fBounceOriginalSpeed = 0.0f;
    m_bLeftBounceNeeded = false;
    m_bRightBounceNeeded = false;
    m_bTopBounceNeeded = false;
    m_bBottomBounceNeeded = false;
}

void UIScrollView::startAutoScrollChildrenWithOriginalSpeed(const cocos2d::Point& dir, float v, bool attenuated, float acceleration)
{
    stopAutoScrollChildren();
    m_tAutoScrollDir = dir;
    m_bIsAutoScrollSpeedAttenuated = attenuated;
    m_fAutoScrollOriginalSpeed = v;
    m_bAutoScroll = true;
    m_fAutoScrollAcceleration = acceleration;
}

void UIScrollView::startAutoScrollChildrenWithDestination(const cocos2d::Point& des, float time, bool attenuated)
{
    m_bNeedCheckAutoScrollDestination = false;
    m_tAutoScrollDestination = des;
    cocos2d::Point dis = des - m_pInnerContainer->getPosition();
    cocos2d::Point dir = dis.normalize();
    float orSpeed = 0.0f;
    float acceleration = -1000.0f;
    if (attenuated)
    {
        acceleration = (-(2 * dis.getLength())) / (time * time);
        orSpeed =  2 * dis.getLength() / time;
    }
    else
    {
        m_bNeedCheckAutoScrollDestination = true;
        orSpeed = dis.getLength() / time;
    }
    startAutoScrollChildrenWithOriginalSpeed(dir, orSpeed, attenuated, acceleration);
}

void UIScrollView::jumpToDestination(const cocos2d::Point &des)
{
    float finalOffsetX = des.x;
    float finalOffsetY = des.y;
    switch (m_eDirection)
    {
        case SCROLLVIEW_DIR_VERTICAL:
            if (des.y <= 0)
            {
                finalOffsetY = KD_MAX(des.y, m_tSize.height - m_pInnerContainer->getSize().height);
            }
            break;
        case SCROLLVIEW_DIR_HORIZONTAL:
            if (des.x <= 0)
            {
                finalOffsetX = KD_MAX(des.x, m_tSize.width - m_pInnerContainer->getSize().width);
            }
            break;
        case SCROLLVIEW_DIR_BOTH:
            if (des.y <= 0)
            {
                finalOffsetY = KD_MAX(des.y, m_tSize.height - m_pInnerContainer->getSize().height);
            }
            if (des.x <= 0)
            {
                finalOffsetX = KD_MAX(des.x, m_tSize.width - m_pInnerContainer->getSize().width);
            }
            break;
        default:
            break;
    }
    m_pInnerContainer->setPosition(cocos2d::Point(finalOffsetX, finalOffsetY));
}

void UIScrollView::stopAutoScrollChildren()
{
    m_bAutoScroll = false;
    m_fAutoScrollOriginalSpeed = 0.0f;
    m_fAutoScrollAddUpTime = 0.0f;
}

bool UIScrollView::bounceScrollChildren(float touchOffsetX, float touchOffsetY)
{
    bool scrollenabled = true;
    if (touchOffsetX > 0.0f && touchOffsetY > 0.0f) //first quadrant //bounce to top-right
    {
        float realOffsetX = touchOffsetX;
        float realOffsetY = touchOffsetY;
        float icRightPos = m_pInnerContainer->getRightInParent();
        if (icRightPos + realOffsetX >= m_fRightBoundary)
        {
            realOffsetX = m_fRightBoundary - icRightPos;
            bounceRightEvent();
            scrollenabled = false;
        }
        float icTopPos = m_pInnerContainer->getTopInParent();
        if (icTopPos + touchOffsetY >= m_fTopBoundary)
        {
            realOffsetY = m_fTopBoundary - icTopPos;
            bounceTopEvent();
            scrollenabled = false;
        }
        moveChildren(realOffsetX, realOffsetY);
    }
    else if(touchOffsetX < 0.0f && touchOffsetY > 0.0f) //second quadrant //bounce to top-left
    {
        float realOffsetX = touchOffsetX;
        float realOffsetY = touchOffsetY;
        float icLefrPos = m_pInnerContainer->getLeftInParent();
        if (icLefrPos + realOffsetX <= m_fLeftBoundary)
        {
            realOffsetX = m_fLeftBoundary - icLefrPos;
            bounceLeftEvent();
            scrollenabled = false;
        }
        float icTopPos = m_pInnerContainer->getTopInParent();
        if (icTopPos + touchOffsetY >= m_fTopBoundary)
        {
            realOffsetY = m_fTopBoundary - icTopPos;
            bounceTopEvent();
            scrollenabled = false;
        }
        moveChildren(realOffsetX, realOffsetY);
    }
    else if (touchOffsetX < 0.0f && touchOffsetY < 0.0f) //third quadrant //bounce to bottom-left
    {
        float realOffsetX = touchOffsetX;
        float realOffsetY = touchOffsetY;
        float icLefrPos = m_pInnerContainer->getLeftInParent();
        if (icLefrPos + realOffsetX <= m_fLeftBoundary)
        {
            realOffsetX = m_fLeftBoundary - icLefrPos;
            bounceLeftEvent();
            scrollenabled = false;
        }
        float icBottomPos = m_pInnerContainer->getBottomInParent();
        if (icBottomPos + touchOffsetY <= m_fBottomBoundary)
        {
            realOffsetY = m_fBottomBoundary - icBottomPos;
            bounceBottomEvent();
            scrollenabled = false;
        }
        moveChildren(realOffsetX, realOffsetY);
    }
    else if (touchOffsetX > 0.0f && touchOffsetY < 0.0f) //forth quadrant //bounce to bottom-right
    {
        float realOffsetX = touchOffsetX;
        float realOffsetY = touchOffsetY;
        float icRightPos = m_pInnerContainer->getRightInParent();
        if (icRightPos + realOffsetX >= m_fRightBoundary)
        {
            realOffsetX = m_fRightBoundary - icRightPos;
            bounceRightEvent();
            scrollenabled = false;
        }
        float icBottomPos = m_pInnerContainer->getBottomInParent();
        if (icBottomPos + touchOffsetY <= m_fBottomBoundary)
        {
            realOffsetY = m_fBottomBoundary - icBottomPos;
            bounceBottomEvent();
            scrollenabled = false;
        }
        moveChildren(realOffsetX, realOffsetY);
    }
    else if (touchOffsetX == 0.0f && touchOffsetY > 0.0f) // bounce to top
    {
        float realOffsetY = touchOffsetY;
        float icTopPos = m_pInnerContainer->getTopInParent();
        if (icTopPos + touchOffsetY >= m_fTopBoundary)
        {
            realOffsetY = m_fTopBoundary - icTopPos;
            bounceTopEvent();
            scrollenabled = false;
        }
        moveChildren(0.0f, realOffsetY);
    }
    else if (touchOffsetX == 0.0f && touchOffsetY < 0.0f) //bounce to bottom
    {
        float realOffsetY = touchOffsetY;
        float icBottomPos = m_pInnerContainer->getBottomInParent();
        if (icBottomPos + touchOffsetY <= m_fBottomBoundary)
        {
            realOffsetY = m_fBottomBoundary - icBottomPos;
            bounceBottomEvent();
            scrollenabled = false;
        }
        moveChildren(0.0f, realOffsetY);
    }
    else if (touchOffsetX > 0.0f && touchOffsetY == 0.0f) //bounce to right
    {
        float realOffsetX = touchOffsetX;
        float icRightPos = m_pInnerContainer->getRightInParent();
        if (icRightPos + realOffsetX >= m_fRightBoundary)
        {
            realOffsetX = m_fRightBoundary - icRightPos;
            bounceRightEvent();
            scrollenabled = false;
        }
        moveChildren(realOffsetX, 0.0f);
    }
    else if (touchOffsetX < 0.0f && touchOffsetY == 0.0f) //bounce to left
    {
        float realOffsetX = touchOffsetX;
        float icLeftPos = m_pInnerContainer->getLeftInParent();
        if (icLeftPos + realOffsetX <= m_fLeftBoundary)
        {
            realOffsetX = m_fLeftBoundary - icLeftPos;
            bounceLeftEvent();
            scrollenabled = false;
        }
        moveChildren(realOffsetX, 0.0f);
    }
    return scrollenabled;
}

bool UIScrollView::checkCustomScrollDestination(float* touchOffsetX, float* touchOffsetY)
{
    bool scrollenabled = true;
    switch (m_eDirection)
    {
        case SCROLLVIEW_DIR_VERTICAL: // vertical
        {
            if (m_tAutoScrollDir.y > 0)
            {
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + *touchOffsetY >= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icBottomPos;
                    scrollenabled = false;
                }
            }
            else
            {
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + *touchOffsetY <= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icBottomPos;
                    scrollenabled = false;
                }
            }
            break;
        }
        case SCROLLVIEW_DIR_HORIZONTAL: // horizontal
        {
            if (m_tAutoScrollDir.x > 0)
            {
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + *touchOffsetX >= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icLeftPos;
                    scrollenabled = false;
                }
            }
            else
            {
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + *touchOffsetX <= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icLeftPos;
                    scrollenabled = false;
                }
            }
            break;
        }
        case SCROLLVIEW_DIR_BOTH:
        {
            if (*touchOffsetX > 0.0f && *touchOffsetY > 0.0f) // up right
            {
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + *touchOffsetX >= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icLeftPos;
                    scrollenabled = false;
                }
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + *touchOffsetY >= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icBottomPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX < 0.0f && *touchOffsetY > 0.0f) // up left
            {
                float icRightPos = m_pInnerContainer->getRightInParent();
                if (icRightPos + *touchOffsetX <= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icRightPos;
                    scrollenabled = false;
                }
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + *touchOffsetY >= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icBottomPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX < 0.0f && *touchOffsetY < 0.0f) // down left
            {
                float icRightPos = m_pInnerContainer->getRightInParent();
                if (icRightPos + *touchOffsetX <= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icRightPos;
                    scrollenabled = false;
                }
                float icTopPos = m_pInnerContainer->getTopInParent();
                if (icTopPos + *touchOffsetY <= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icTopPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX > 0.0f && *touchOffsetY < 0.0f) // down right
            {
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + *touchOffsetX >= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icLeftPos;
                    scrollenabled = false;
                }
                float icTopPos = m_pInnerContainer->getTopInParent();
                if (icTopPos + *touchOffsetY <= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icTopPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX == 0.0f && *touchOffsetY > 0.0f) // up
            {
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + *touchOffsetY >= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icBottomPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX < 0.0f && *touchOffsetY == 0.0f) // left
            {
                float icRightPos = m_pInnerContainer->getRightInParent();
                if (icRightPos + *touchOffsetX <= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icRightPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX == 0.0f && *touchOffsetY < 0.0f) // down
            {
                float icTopPos = m_pInnerContainer->getTopInParent();
                if (icTopPos + *touchOffsetY <= m_tAutoScrollDestination.y)
                {
                    *touchOffsetY = m_tAutoScrollDestination.y - icTopPos;
                    scrollenabled = false;
                }
            }
            else if (*touchOffsetX > 0.0f && *touchOffsetY == 0.0f) // right
            {
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + *touchOffsetX >= m_tAutoScrollDestination.x)
                {
                    *touchOffsetX = m_tAutoScrollDestination.x - icLeftPos;
                    scrollenabled = false;
                }
            }
            break;
        }
        default:
            break;
    }
    return scrollenabled;
}

bool UIScrollView::scrollChildren(float touchOffsetX, float touchOffsetY)
{
    bool scrollenabled = true;
    scrollingEvent();
    switch (m_eDirection)
    {
        case SCROLLVIEW_DIR_VERTICAL: // vertical
        {
            float realOffset = touchOffsetY;
            if (m_bBounceEnabled)
            {
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + touchOffsetY >= m_fBounceBottomBoundary)
                {
                    realOffset = m_fBounceBottomBoundary - icBottomPos;
                    scrollToBottomEvent();
                    scrollenabled = false;
                }
                float icTopPos = m_pInnerContainer->getTopInParent();
                if (icTopPos + touchOffsetY <= m_fBounceTopBoundary)
                {
                    realOffset = m_fBounceTopBoundary - icTopPos;
                    scrollToTopEvent();
                    scrollenabled = false;
                }
            }
            else
            {
                float icBottomPos = m_pInnerContainer->getBottomInParent();
                if (icBottomPos + touchOffsetY >= m_fBottomBoundary)
                {
                    realOffset = m_fBottomBoundary - icBottomPos;
                    scrollToBottomEvent();
                    scrollenabled = false;
                }
                float icTopPos = m_pInnerContainer->getTopInParent();
                if (icTopPos + touchOffsetY <= m_fTopBoundary)
                {
                    realOffset = m_fTopBoundary - icTopPos;
                    scrollToTopEvent();
                    scrollenabled = false;
                }
            }
            moveChildren(0.0f, realOffset);
            break;
        }
        case SCROLLVIEW_DIR_HORIZONTAL: // horizontal
        {
            float realOffset = touchOffsetX;
            if (m_bBounceEnabled)
            {
                float icRightPos = m_pInnerContainer->getRightInParent();
                if (icRightPos + touchOffsetX <= m_fBounceRightBoundary)
                {
                    realOffset = m_fBounceRightBoundary - icRightPos;
                    scrollToRightEvent();
                    scrollenabled = false;
                }
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + touchOffsetX >= m_fBounceLeftBoundary)
                {
                    realOffset = m_fBounceLeftBoundary - icLeftPos;
                    scrollToLeftEvent();
                    scrollenabled = false;
                }
            }
            else
            {
                float icRightPos = m_pInnerContainer->getRightInParent();
                if (icRightPos + touchOffsetX <= m_fRightBoundary)
                {
                    realOffset = m_fRightBoundary - icRightPos;
                    scrollToRightEvent();
                    scrollenabled = false;
                }
                float icLeftPos = m_pInnerContainer->getLeftInParent();
                if (icLeftPos + touchOffsetX >= m_fLeftBoundary)
                {
                    realOffset = m_fLeftBoundary - icLeftPos;
                    scrollToLeftEvent();
                    scrollenabled = false;
                }
            }
            moveChildren(realOffset, 0.0f);
            break;
        }
        case SCROLLVIEW_DIR_BOTH:
        {
            float realOffsetX = touchOffsetX;
            float realOffsetY = touchOffsetY;
            if (m_bBounceEnabled)
            {
                if (touchOffsetX > 0.0f && touchOffsetY > 0.0f) // up right
                {
                    float icLeftPos = m_pInnerContainer->getLeftInParent();
                    if (icLeftPos + touchOffsetX >= m_fBounceLeftBoundary)
                    {
                        realOffsetX = m_fBounceLeftBoundary - icLeftPos;
                        scrollToLeftEvent();
                        scrollenabled = false;
                    }
                    float icBottomPos = m_pInnerContainer->getBottomInParent();
                    if (icBottomPos + touchOffsetY >= m_fBounceBottomBoundary)
                    {
                        realOffsetY = m_fBounceBottomBoundary - icBottomPos;
                        scrollToBottomEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX < 0.0f && touchOffsetY > 0.0f) // up left
                {
                    float icRightPos = m_pInnerContainer->getRightInParent();
                    if (icRightPos + touchOffsetX <= m_fBounceRightBoundary)
                    {
                        realOffsetX = m_fBounceRightBoundary - icRightPos;
                        scrollToRightEvent();
                        scrollenabled = false;
                    }
                    float icBottomPos = m_pInnerContainer->getBottomInParent();
                    if (icBottomPos + touchOffsetY >= m_fBounceBottomBoundary)
                    {
                        realOffsetY = m_fBounceBottomBoundary - icBottomPos;
                        scrollToBottomEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX < 0.0f && touchOffsetY < 0.0f) // down left
                {
                    float icRightPos = m_pInnerContainer->getRightInParent();
                    if (icRightPos + touchOffsetX <= m_fBounceRightBoundary)
                    {
                        realOffsetX = m_fBounceRightBoundary - icRightPos;
                        scrollToRightEvent();
                        scrollenabled = false;
                    }
                    float icTopPos = m_pInnerContainer->getTopInParent();
                    if (icTopPos + touchOffsetY <= m_fBounceTopBoundary)
                    {
                        realOffsetY = m_fBounceTopBoundary - icTopPos;
                        scrollToTopEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX > 0.0f && touchOffsetY < 0.0f) // down right
                {
                    float icLeftPos = m_pInnerContainer->getLeftInParent();
                    if (icLeftPos + touchOffsetX >= m_fBounceLeftBoundary)
                    {
                        realOffsetX = m_fBounceLeftBoundary - icLeftPos;
                        scrollToLeftEvent();
                        scrollenabled = false;
                    }
                    float icTopPos = m_pInnerContainer->getTopInParent();
                    if (icTopPos + touchOffsetY <= m_fBounceTopBoundary)
                    {
                        realOffsetY = m_fBounceTopBoundary - icTopPos;
                        scrollToTopEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX == 0.0f && touchOffsetY > 0.0f) // up
                {
                    float icBottomPos = m_pInnerContainer->getBottomInParent();
                    if (icBottomPos + touchOffsetY >= m_fBounceBottomBoundary)
                    {
                        realOffsetY = m_fBounceBottomBoundary - icBottomPos;
                        scrollToBottomEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX < 0.0f && touchOffsetY == 0.0f) // left
                {
                    float icRightPos = m_pInnerContainer->getRightInParent();
                    if (icRightPos + touchOffsetX <= m_fBounceRightBoundary)
                    {
                        realOffsetX = m_fBounceRightBoundary - icRightPos;
                        scrollToRightEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX == 0.0f && touchOffsetY < 0.0f) // down
                {
                    float icTopPos = m_pInnerContainer->getTopInParent();
                    if (icTopPos + touchOffsetY <= m_fBounceTopBoundary)
                    {
                        realOffsetY = m_fBounceTopBoundary - icTopPos;
                        scrollToTopEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX > 0.0f && touchOffsetY == 0.0f) // right
                {
                    float icLeftPos = m_pInnerContainer->getLeftInParent();
                    if (icLeftPos + touchOffsetX >= m_fBounceLeftBoundary)
                    {
                        realOffsetX = m_fBounceLeftBoundary - icLeftPos;
                        scrollToLeftEvent();
                        scrollenabled = false;
                    }
                }
            }
            else
            {
                if (touchOffsetX > 0.0f && touchOffsetY > 0.0f) // up right
                {
                    float icLeftPos = m_pInnerContainer->getLeftInParent();
                    if (icLeftPos + touchOffsetX >= m_fLeftBoundary)
                    {
                        realOffsetX = m_fLeftBoundary - icLeftPos;
                        scrollToLeftEvent();
                        scrollenabled = false;
                    }
                    float icBottomPos = m_pInnerContainer->getBottomInParent();
                    if (icBottomPos + touchOffsetY >= m_fBottomBoundary)
                    {
                        realOffsetY = m_fBottomBoundary - icBottomPos;
                        scrollToBottomEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX < 0.0f && touchOffsetY > 0.0f) // up left
                {
                    float icRightPos = m_pInnerContainer->getRightInParent();
                    if (icRightPos + touchOffsetX <= m_fRightBoundary)
                    {
                        realOffsetX = m_fRightBoundary - icRightPos;
                        scrollToRightEvent();
                        scrollenabled = false;
                    }
                    float icBottomPos = m_pInnerContainer->getBottomInParent();
                    if (icBottomPos + touchOffsetY >= m_fBottomBoundary)
                    {
                        realOffsetY = m_fBottomBoundary - icBottomPos;
                        scrollToBottomEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX < 0.0f && touchOffsetY < 0.0f) // down left
                {
                    float icRightPos = m_pInnerContainer->getRightInParent();
                    if (icRightPos + touchOffsetX <= m_fRightBoundary)
                    {
                        realOffsetX = m_fRightBoundary - icRightPos;
                        scrollToRightEvent();
                        scrollenabled = false;
                    }
                    float icTopPos = m_pInnerContainer->getTopInParent();
                    if (icTopPos + touchOffsetY <= m_fTopBoundary)
                    {
                        realOffsetY = m_fTopBoundary - icTopPos;
                        scrollToTopEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX > 0.0f && touchOffsetY < 0.0f) // down right
                {
                    float icLeftPos = m_pInnerContainer->getLeftInParent();
                    if (icLeftPos + touchOffsetX >= m_fLeftBoundary)
                    {
                        realOffsetX = m_fLeftBoundary - icLeftPos;
                        scrollToLeftEvent();
                        scrollenabled = false;
                    }
                    float icTopPos = m_pInnerContainer->getTopInParent();
                    if (icTopPos + touchOffsetY <= m_fTopBoundary)
                    {
                        realOffsetY = m_fTopBoundary - icTopPos;
                        scrollToTopEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX == 0.0f && touchOffsetY > 0.0f) // up
                {
                    float icBottomPos = m_pInnerContainer->getBottomInParent();
                    if (icBottomPos + touchOffsetY >= m_fBottomBoundary)
                    {
                        realOffsetY = m_fBottomBoundary - icBottomPos;
                        scrollToBottomEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX < 0.0f && touchOffsetY == 0.0f) // left
                {
                    float icRightPos = m_pInnerContainer->getRightInParent();
                    if (icRightPos + touchOffsetX <= m_fRightBoundary)
                    {
                        realOffsetX = m_fRightBoundary - icRightPos;
                        scrollToRightEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX == 0.0f && touchOffsetY < 0.0f) // down
                {
                    float icTopPos = m_pInnerContainer->getTopInParent();
                    if (icTopPos + touchOffsetY <= m_fTopBoundary)
                    {
                        realOffsetY = m_fTopBoundary - icTopPos;
                        scrollToTopEvent();
                        scrollenabled = false;
                    }
                }
                else if (touchOffsetX > 0.0f && touchOffsetY == 0.0f) // right
                {
                    float icLeftPos = m_pInnerContainer->getLeftInParent();
                    if (icLeftPos + touchOffsetX >= m_fLeftBoundary)
                    {
                        realOffsetX = m_fLeftBoundary - icLeftPos;
                        scrollToLeftEvent();
                        scrollenabled = false;
                    }
                }
            }
            moveChildren(realOffsetX, realOffsetY);
            break;
        }
        default:
            break;
    }
    return scrollenabled;
}

void UIScrollView::scrollToBottom(float time, bool attenuated)
{
    startAutoScrollChildrenWithDestination(cocos2d::Point(m_pInnerContainer->getPosition().x, 0.0f), time, attenuated);
}

void UIScrollView::scrollToTop(float time, bool attenuated)
{
    startAutoScrollChildrenWithDestination(cocos2d::Point(m_pInnerContainer->getPosition().x, m_tSize.height - m_pInnerContainer->getSize().height), time, attenuated);
}

void UIScrollView::scrollToLeft(float time, bool attenuated)
{
    startAutoScrollChildrenWithDestination(cocos2d::Point(0.0f, m_pInnerContainer->getPosition().y), time, attenuated);
}

void UIScrollView::scrollToRight(float time, bool attenuated)
{
    startAutoScrollChildrenWithDestination(cocos2d::Point(m_tSize.width - m_pInnerContainer->getSize().width, m_pInnerContainer->getPosition().y), time, attenuated);
}

void UIScrollView::scrollToTopLeft(float time, bool attenuated)
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    startAutoScrollChildrenWithDestination(cocos2d::Point(0.0f, m_tSize.height - m_pInnerContainer->getSize().height), time, attenuated);
}

void UIScrollView::scrollToTopRight(float time, bool attenuated)
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    startAutoScrollChildrenWithDestination(cocos2d::Point(m_tSize.width - m_pInnerContainer->getSize().width, m_tSize.height - m_pInnerContainer->getSize().height), time, attenuated);
}

void UIScrollView::scrollToBottomLeft(float time, bool attenuated)
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    startAutoScrollChildrenWithDestination(cocos2d::Point::ZERO, time, attenuated);
}

void UIScrollView::scrollToBottomRight(float time, bool attenuated)
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    startAutoScrollChildrenWithDestination(cocos2d::Point(m_tSize.width - m_pInnerContainer->getSize().width, 0.0f), time, attenuated);
}

void UIScrollView::scrollToPercentVertical(float percent, float time, bool attenuated)
{
    float minY = m_tSize.height - m_pInnerContainer->getSize().height;
    float h = - minY;
    startAutoScrollChildrenWithDestination(cocos2d::Point(m_pInnerContainer->getPosition().x, minY + percent * h / 100.0f), time, attenuated);
}

void UIScrollView::scrollToPercentHorizontal(float percent, float time, bool attenuated)
{
    float w = m_pInnerContainer->getSize().width - m_tSize.width;
    startAutoScrollChildrenWithDestination(cocos2d::Point(-(percent * w / 100.0f), m_pInnerContainer->getPosition().y), time, attenuated);
}

void UIScrollView::scrollToPercentBothDirection(const cocos2d::Point& percent, float time, bool attenuated)
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        return;
    }
    float minY = m_tSize.height - m_pInnerContainer->getSize().height;
    float h = - minY;
    float w = m_pInnerContainer->getSize().width - m_tSize.width;
    startAutoScrollChildrenWithDestination(cocos2d::Point(-(percent.x * w / 100.0f), minY + percent.y * h / 100.0f), time, attenuated);
}

void UIScrollView::jumpToBottom()
{
    jumpToDestination(cocos2d::Point(m_pInnerContainer->getPosition().x, 0.0f));
}

void UIScrollView::jumpToTop()
{
    jumpToDestination(cocos2d::Point(m_pInnerContainer->getPosition().x, m_tSize.height - m_pInnerContainer->getSize().height));
}

void UIScrollView::jumpToLeft()
{
    jumpToDestination(cocos2d::Point(0.0f, m_pInnerContainer->getPosition().y));
}

void UIScrollView::jumpToRight()
{
    jumpToDestination(cocos2d::Point(m_tSize.width - m_pInnerContainer->getSize().width, m_pInnerContainer->getPosition().y));
}

void UIScrollView::jumpToTopLeft()
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    jumpToDestination(cocos2d::Point(0.0f, m_tSize.height - m_pInnerContainer->getSize().height));
}

void UIScrollView::jumpToTopRight()
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    jumpToDestination(cocos2d::Point(m_tSize.width - m_pInnerContainer->getSize().width, m_tSize.height - m_pInnerContainer->getSize().height));
}

void UIScrollView::jumpToBottomLeft()
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    jumpToDestination(cocos2d::Point::ZERO);
}

void UIScrollView::jumpToBottomRight()
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        CCLOG("Scroll diretion is not both!");
        return;
    }
    jumpToDestination(cocos2d::Point(m_tSize.width - m_pInnerContainer->getSize().width, 0.0f));
}

void UIScrollView::jumpToPercentVertical(float percent)
{
    float minY = m_tSize.height - m_pInnerContainer->getSize().height;
    float h = - minY;
    jumpToDestination(cocos2d::Point(m_pInnerContainer->getPosition().x, minY + percent * h / 100.0f));
}

void UIScrollView::jumpToPercentHorizontal(float percent)
{
    float w = m_pInnerContainer->getSize().width - m_tSize.width;
    jumpToDestination(cocos2d::Point(-(percent * w / 100.0f), m_pInnerContainer->getPosition().y));
}

void UIScrollView::jumpToPercentBothDirection(const cocos2d::Point& percent)
{
    if (m_eDirection != SCROLLVIEW_DIR_BOTH)
    {
        return;
    }
    float minY = m_tSize.height - m_pInnerContainer->getSize().height;
    float h = - minY;
    float w = m_pInnerContainer->getSize().width - m_tSize.width;
    jumpToDestination(cocos2d::Point(-(percent.x * w / 100.0f), minY + percent.y * h / 100.0f));
}

void UIScrollView::startRecordSlidAction()
{
    if (m_bAutoScroll)
    {
        stopAutoScrollChildren();
    }
    if (m_bBouncing)
    {
        stopBounceChildren();
    }
    m_fSlidTime = 0.0f;
}

void UIScrollView::endRecordSlidAction()
{
    if (!checkNeedBounce() && m_bInertiaScrollEnabled)
    {
        if (m_fSlidTime <= 0.016f)
        {
            return;
        }
        float totalDis = 0.0f;
        cocos2d::Point dir;
        switch (m_eDirection)
        {
            case SCROLLVIEW_DIR_VERTICAL:
                totalDis = m_tTouchEndedPoint.y - m_tTouchBeganPoint.y;
                if (totalDis < 0.0f)
                {
                    dir = SCROLLDIR_DOWN;
                }
                else
                {
                    dir = SCROLLDIR_UP;
                }
                break;
            case SCROLLVIEW_DIR_HORIZONTAL:
                totalDis = m_tTouchEndedPoint.x - m_tTouchBeganPoint.x;
                if (totalDis < 0.0f)
                {
                    dir = SCROLLDIR_LEFT;
                }
                else
                {
                    dir = SCROLLDIR_RIGHT;
                }
                break;
            case SCROLLVIEW_DIR_BOTH:
            {
                cocos2d::Point subVector = m_tTouchEndedPoint - m_tTouchBeganPoint;
                totalDis = subVector.getLength();
                dir = subVector.normalize();
                break;
            }
            default:
                break;
        }
        float orSpeed = KD_MIN(kdFabsf(totalDis)/(m_fSlidTime), AUTOSCROLLMAXSPEED);
        startAutoScrollChildrenWithOriginalSpeed(dir, orSpeed, true, -1000);
        m_fSlidTime = 0.0f;
    }
}

void UIScrollView::handlePressLogic(const cocos2d::Point &touchPoint)
{        
    m_tTouchBeganPoint = m_pRenderer->convertToNodeSpace(touchPoint);
    m_tTouchMovingPoint = m_tTouchBeganPoint;    
    startRecordSlidAction();
    m_bBePressed = true;
}

void UIScrollView::handleMoveLogic(const cocos2d::Point &touchPoint)
{
    m_tTouchMovedPoint = m_pRenderer->convertToNodeSpace(touchPoint);
    cocos2d::Point delta = m_tTouchMovedPoint - m_tTouchMovingPoint;
    m_tTouchMovingPoint = m_tTouchMovedPoint;
    switch (m_eDirection)
    {
        case SCROLLVIEW_DIR_VERTICAL: // vertical
        {
            scrollChildren(0.0f, delta.y);
            break;
        }
        case SCROLLVIEW_DIR_HORIZONTAL: // horizontal
        {
            scrollChildren(delta.x, 0.0f);
            break;
        }
        case SCROLLVIEW_DIR_BOTH: // both
        {
            scrollChildren(delta.x, delta.y);
            break;
        }
        default:
            break;
    }
}

void UIScrollView::handleReleaseLogic(const cocos2d::Point &touchPoint)
{
    m_tTouchEndedPoint = m_pRenderer->convertToNodeSpace(touchPoint);
    endRecordSlidAction();
    m_bBePressed = false;
}    

bool UIScrollView::onTouchBegan(const cocos2d::Point &touchPoint)
{
    bool pass = UILayout::onTouchBegan(touchPoint);
    handlePressLogic(touchPoint);
    return pass;
}

void UIScrollView::onTouchMoved(const cocos2d::Point &touchPoint)
{
    UILayout::onTouchMoved(touchPoint);
    handleMoveLogic(touchPoint);
}

void UIScrollView::onTouchEnded(const cocos2d::Point &touchPoint)
{
    UILayout::onTouchEnded(touchPoint);
    handleReleaseLogic(touchPoint);
}

void UIScrollView::onTouchCancelled(const cocos2d::Point &touchPoint)
{
    UILayout::onTouchCancelled(touchPoint);
    handleReleaseLogic(touchPoint);
}

void UIScrollView::onTouchLongClicked(const cocos2d::Point &touchPoint)
{
    
}

void UIScrollView::update(float dt)
{
    if (m_bAutoScroll)
    {
        autoScrollChildren(dt);
    }
    if (m_bBouncing)
    {
        bounceChildren(dt);
    }
    recordSlidTime(dt);
}

void UIScrollView::recordSlidTime(float dt)
{
    if (m_bBePressed)
    {
        m_fSlidTime += dt;
    }
}

void UIScrollView::interceptTouchEvent(int handleState, UIWidget *sender, const cocos2d::Point &touchPoint)
{
    switch (handleState)
    {
        case 0:
            handlePressLogic(touchPoint);
            break;
            
        case 1:
        {
            float offset = (sender->getTouchStartPos() - touchPoint).getLength();
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
            handleReleaseLogic(touchPoint);
            break;
    }
}

void UIScrollView::checkChildInfo(int handleState,UIWidget* sender,const cocos2d::Point &touchPoint)
{
    interceptTouchEvent(handleState, sender, touchPoint);
}

void UIScrollView::scrollToTopEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_SCROLL_TO_TOP);
    }
}

void UIScrollView::scrollToBottomEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_SCROLL_TO_BOTTOM);
    }
}

void UIScrollView::scrollToLeftEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_SCROLL_TO_LEFT);
    }
}

void UIScrollView::scrollToRightEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_SCROLL_TO_RIGHT);
    }
}

void UIScrollView::scrollingEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_SCROLLING);
    }
}

void UIScrollView::bounceTopEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_BOUNCE_TOP);
    }
}

void UIScrollView::bounceBottomEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_BOUNCE_BOTTOM);
    }
}

void UIScrollView::bounceLeftEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_BOUNCE_LEFT);
    }
}

void UIScrollView::bounceRightEvent()
{
    if (m_pScrollViewEventListener && m_pScrollViewEventSelector)
    {
        (m_pScrollViewEventListener->*m_pScrollViewEventSelector)(this, SCROLLVIEW_EVENT_BOUNCE_RIGHT);
    }
}

void UIScrollView::addEventListenerScrollView(cocos2d::Object *target, SEL_ScrollViewEvent selector)
{
    m_pScrollViewEventListener = target;
    m_pScrollViewEventSelector = selector;
}

void UIScrollView::setDirection(SCROLLVIEW_DIR dir)
{
    m_eDirection = dir;
}

SCROLLVIEW_DIR UIScrollView::getDirection()
{
    return m_eDirection;
}

void UIScrollView::setBounceEnabled(bool enabled)
{
    m_bBounceEnabled = enabled;
}

bool UIScrollView::isBounceEnabled() const
{
    return m_bBounceEnabled;
}

void UIScrollView::setInertiaScrollEnabled(bool enabled)
{
    m_bInertiaScrollEnabled = enabled;
}

bool UIScrollView::isInertiaScrollEnabled() const
{
    return m_bInertiaScrollEnabled;
}

UILayout* UIScrollView::getInnerContainer()
{
    return m_pInnerContainer;
}

void UIScrollView::setLayoutType(LayoutType type)
{
    m_pInnerContainer->setLayoutType(type);
}

LayoutType UIScrollView::getLayoutType() const
{
    return m_pInnerContainer->getLayoutType();
}

void UIScrollView::doLayout()
{
    m_pInnerContainer->doLayout();
}

const char* UIScrollView::getDescription() const
{
    return "ScrollView";
}

UIWidget* UIScrollView::createCloneInstance()
{
    return UIScrollView::create();
}

void UIScrollView::copyClonedWidgetChildren(UIWidget* model)
{
    UILayout::copyClonedWidgetChildren(model);
}

void UIScrollView::copySpecialProperties(UIWidget *widget)
{
    UIScrollView* scrollView = dynamic_cast<UIScrollView*>(widget);
    if (scrollView)
    {
        UILayout::copySpecialProperties(widget);
        setInnerContainerSize(scrollView->getInnerContainerSize());
        setDirection(scrollView->m_eDirection);
        setBounceEnabled(scrollView->m_bBounceEnabled);
        setInertiaScrollEnabled(scrollView->m_bInertiaScrollEnabled);
    }
}

}