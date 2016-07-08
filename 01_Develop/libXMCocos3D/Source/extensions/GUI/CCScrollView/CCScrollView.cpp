/* -----------------------------------------------------------------------------------
 *
 *      File            CCScrollView.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2010      Sangwoo Im.
 *
 *          http://www.cocos2d-x.org
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "extensions/GUI/CCScrollView/CCScrollView.h"
#include "2d/actions/CCAction.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/actions/CCActionInstant.h"
#include "2d/actions/CCActionTween.h"
#include "2d/effects/CCGrid.h"
#include "platform/CCEGLView.h"
#include "platform/CCDevice.h"
#include "XMKazmath/GL/matrix.h"

#include <algorithm>

NS_CC_EXT_BEGIN

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f
#define MOVE_INCH            7.0f/160.0f

static float convertDistanceFromPointToInch(float pointDis)
{
    float factor = ( EGLView::getInstance()->getScaleX() + EGLView::getInstance()->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}


ScrollView::ScrollView()
: m_fZoomScale(0.0f)
, m_fMinZoomScale(0.0f)
, m_fMaxZoomScale(0.0f)
, m_pDelegate(NULL)
, m_eDirection(Direction::BOTH)
, m_bDragging(false)
, m_pContainer(NULL)
, m_bTouchMoved(false)
, m_bBounceable(false)
, m_bClippingToBounds(false)
, m_fTouchLength(0.0f)
, m_fMinScale(0.0f)
, m_fMaxScale(0.0f)
, m_pTouchListener(nullptr)
{

}

ScrollView::~ScrollView()
{

}

ScrollView* ScrollView::create(Size size, Node* container/* = NULL*/)
{
    ScrollView* pRet = new ScrollView();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

ScrollView* ScrollView::create()
{
    ScrollView* pRet = new ScrollView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


bool ScrollView::initWithViewSize(Size size, Node *container/* = NULL*/)
{
    if (Layer::init())
    {
        m_pContainer = container;
        
        if (!this->m_pContainer)
        {
            m_pContainer = Layer::create();
            this->m_pContainer->ignoreAnchorPointForPosition(false);
            this->m_pContainer->setAnchorPoint(Point(0.0f, 0.0f));
        }

        this->setViewSize(size);

        setTouchEnabled(true);
        
        m_aTouches.reserve(EventTouch::MAX_TOUCHES);
        
        m_pDelegate = NULL;
        m_bBounceable = true;
        m_bClippingToBounds = true;
        //_container->setContentSize(Size::ZERO);
        m_eDirection  = Direction::BOTH;
        m_pContainer->setPosition(Point(0.0f, 0.0f));
        m_fTouchLength = 0.0f;
        
        this->addChild(m_pContainer);
        m_fMinScale = m_fMaxScale = 1.0f;

        
        return true;
    }
    return false;
}

bool ScrollView::init()
{
    return this->initWithViewSize(Size(200, 200), NULL);
}

bool ScrollView::isNodeVisible(Node* node)
{
    const Point offset = this->getContentOffset();
    const Size  size   = this->getViewSize();
    const float   scale  = this->getZoomScale();
    
    Rect viewRect;
    
    viewRect = Rect(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale); 
    
    return viewRect.intersectsRect(node->getBoundingBox());
}

void ScrollView::pause(Object* sender)
{
    m_pContainer->pause();

    Object* pObj = NULL;
    Array* pChildren = m_pContainer->getChildren();

    CCARRAY_FOREACH(pChildren, pObj)
    {
        Node* pChild = static_cast<Node*>(pObj);
        pChild->pause();
    }
}

void ScrollView::resume(Object* sender)
{
    Object* pObj = NULL;
    Array* pChildren = m_pContainer->getChildren();

    CCARRAY_FOREACH(pChildren, pObj)
    {
        Node* pChild = static_cast<Node*>(pObj);
        pChild->resume();
    }

    m_pContainer->resume();
}

bool ScrollView::isTouchEnabled() const
{
	return m_pTouchListener != nullptr;
}

void ScrollView::setTouchEnabled(bool enabled)
{
    m_pEventDispatcher->removeEventListener(m_pTouchListener);
    m_pTouchListener = nullptr;

    if (enabled)
    {
        m_pTouchListener = EventListenerTouchOneByOne::create();
        m_pTouchListener->onTouchBegan = CC_CALLBACK_2(ScrollView::onTouchBegan, this);
        m_pTouchListener->onTouchMoved = CC_CALLBACK_2(ScrollView::onTouchMoved, this);
        m_pTouchListener->onTouchEnded = CC_CALLBACK_2(ScrollView::onTouchEnded, this);
        m_pTouchListener->onTouchCancelled = CC_CALLBACK_2(ScrollView::onTouchCancelled, this);
        
        m_pEventDispatcher->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);
    }
    else
    {
        m_bDragging = false;
        m_bTouchMoved = false;
        m_aTouches.clear();
    }
}

void ScrollView::setContentOffset(Point offset, bool animated/* = false*/)
{
    if (animated)
    { //animate scrolling
        this->setContentOffsetInDuration(offset, BOUNCE_DURATION);
    } 
    else
    { //set the container position directly
        if (!m_bBounceable)
        {
            const Point minOffset = this->minContainerOffset();
            const Point maxOffset = this->maxContainerOffset();
            
            offset.x = KD_MAX(minOffset.x, KD_MIN(maxOffset.x, offset.x));
            offset.y = KD_MAX(minOffset.y, KD_MIN(maxOffset.y, offset.y));
        }

        m_pContainer->setPosition(offset);

        if (m_pDelegate != NULL)
        {
            m_pDelegate->scrollViewDidScroll(this);   
        }
    }
}

void ScrollView::setContentOffsetInDuration(Point offset, float dt)
{
    FiniteTimeAction *scroll, *expire;
    
    scroll = MoveTo::create(dt, offset);
    expire = CallFuncN::create(CC_CALLBACK_1(ScrollView::stoppedAnimatedScroll,this));
    m_pContainer->runAction(Sequence::create(scroll, expire, NULL));
    this->schedule(schedule_selector(ScrollView::performedAnimatedScroll));
}

Point ScrollView::getContentOffset()
{
    return m_pContainer->getPosition();
}

void ScrollView::setZoomScale(float s)
{
    if (m_pContainer->getScale() != s)
    {
        Point oldCenter, newCenter;
        Point center;
        
        if (m_fTouchLength == 0.0f) 
        {
            center = Point(m_tViewSize.width*0.5f, m_tViewSize.height*0.5f);
            center = this->convertToWorldSpace(center);
        }
        else
        {
            center = m_tTouchPoint;
        }
        
        oldCenter = m_pContainer->convertToNodeSpace(center);
        m_pContainer->setScale(KD_MAX(m_fMinScale, KD_MIN(m_fMaxScale, s)));
        newCenter = m_pContainer->convertToWorldSpace(oldCenter);
        
        const Point offset = center - newCenter;
        if (m_pDelegate != NULL)
        {
            m_pDelegate->scrollViewDidZoom(this);
        }
        this->setContentOffset(m_pContainer->getPosition() + offset);
    }
}

float ScrollView::getZoomScale()
{
    return m_pContainer->getScale();
}

void ScrollView::setZoomScale(float s, bool animated)
{
    if (animated)
    {
        this->setZoomScaleInDuration(s, BOUNCE_DURATION);
    }
    else
    {
        this->setZoomScale(s);
    }
}

void ScrollView::setZoomScaleInDuration(float s, float dt)
{
    if (dt > 0)
    {
        if (m_pContainer->getScale() != s)
        {
            ActionTween *scaleAction;
            scaleAction = ActionTween::create(dt, "zoomScale", m_pContainer->getScale(), s);
            this->runAction(scaleAction);
        }
    }
    else
    {
        this->setZoomScale(s);
    }
}

void ScrollView::setViewSize(Size size)
{
    m_tViewSize = size;
    Layer::setContentSize(size);
}

Node * ScrollView::getContainer()
{
    return this->m_pContainer;
}

void ScrollView::setContainer(Node * pContainer)
{
    // Make sure that '_container' has a non-NULL value since there are
    // lots of logic that use '_container'.
    if (NULL == pContainer)
        return;

    this->removeAllChildrenWithCleanup(true);
    this->m_pContainer = pContainer;

    this->m_pContainer->ignoreAnchorPointForPosition(false);
    this->m_pContainer->setAnchorPoint(Point(0.0f, 0.0f));

    this->addChild(this->m_pContainer);

    this->setViewSize(this->m_tViewSize);
}

void ScrollView::relocateContainer(bool animated)
{
    Point oldPoint, min, max;
    float newX, newY;
    
    min = this->minContainerOffset();
    max = this->maxContainerOffset();
    
    oldPoint = m_pContainer->getPosition();

    newX     = oldPoint.x;
    newY     = oldPoint.y;
    if (m_eDirection == Direction::BOTH || m_eDirection == Direction::HORIZONTAL)
    {
        newX     = KD_MAX(newX, min.x);
        newX     = KD_MIN(newX, max.x);
    }

    if (m_eDirection == Direction::BOTH || m_eDirection == Direction::VERTICAL)
    {
        newY     = KD_MIN(newY, max.y);
        newY     = KD_MAX(newY, min.y);
    }

    if (newY != oldPoint.y || newX != oldPoint.x)
    {
        this->setContentOffset(Point(newX, newY), animated);
    }
}

Point ScrollView::maxContainerOffset()
{
    return Point(0.0f, 0.0f);
}

Point ScrollView::minContainerOffset()
{
    return Point(m_tViewSize.width - m_pContainer->getContentSize().width*m_pContainer->getScaleX(), 
               m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY());
}

void ScrollView::deaccelerateScrolling(float dt)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(ScrollView::deaccelerateScrolling));
        return;
    }
    
    float newX, newY;
    Point maxInset, minInset;
    
    m_pContainer->setPosition(m_pContainer->getPosition() + m_tScrollDistance);
    
    if (m_bBounceable)
    {
        maxInset = m_tMaxInset;
        minInset = m_tMinInset;
    }
    else
    {
        maxInset = this->maxContainerOffset();
        minInset = this->minContainerOffset();
    }
    
    //check to see if offset lies within the inset bounds
    newX     = KD_MIN(m_pContainer->getPosition().x, maxInset.x);
    newX     = KD_MAX(newX, minInset.x);
    newY     = KD_MIN(m_pContainer->getPosition().y, maxInset.y);
    newY     = KD_MAX(newY, minInset.y);
    
    newX = m_pContainer->getPosition().x;
    newY = m_pContainer->getPosition().y;
    
    m_tScrollDistance     = m_tScrollDistance - Point(newX - m_pContainer->getPosition().x, newY - m_pContainer->getPosition().y);
    m_tScrollDistance     = m_tScrollDistance * SCROLL_DEACCEL_RATE;
    this->setContentOffset(Point(newX,newY));
    
    if ((kdFabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         kdFabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
        newY > maxInset.y || newY < minInset.y ||
        newX > maxInset.x || newX < minInset.x ||
        newX == maxInset.x || newX == minInset.x ||
        newY == maxInset.y || newY == minInset.y)
    {
        this->unschedule(schedule_selector(ScrollView::deaccelerateScrolling));
        this->relocateContainer(true);
    }
}

void ScrollView::stoppedAnimatedScroll(Node * node)
{
    this->unschedule(schedule_selector(ScrollView::performedAnimatedScroll));
    // After the animation stopped, "scrollViewDidScroll" should be invoked, this could fix the bug of lack of tableview cells.
    if (m_pDelegate != NULL)
    {
        m_pDelegate->scrollViewDidScroll(this);
    }
}

void ScrollView::performedAnimatedScroll(float dt)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(ScrollView::performedAnimatedScroll));
        return;
    }

    if (m_pDelegate != NULL)
    {
        m_pDelegate->scrollViewDidScroll(this);
    }
}


const Size& ScrollView::getContentSize() const
{
	return m_pContainer->getContentSize();
}

void ScrollView::setContentSize(const Size & size)
{
    if (this->getContainer() != NULL)
    {
        this->getContainer()->setContentSize(size);
		this->updateInset();
    }
}

void ScrollView::updateInset()
{
	if (this->getContainer() != NULL)
	{
		m_tMaxInset = this->maxContainerOffset();
		m_tMaxInset = Point(m_tMaxInset.x + m_tViewSize.width * INSET_RATIO,
			m_tMaxInset.y + m_tViewSize.height * INSET_RATIO);
		m_tMinInset = this->minContainerOffset();
		m_tMinInset = Point(m_tMinInset.x - m_tViewSize.width * INSET_RATIO,
			m_tMinInset.y - m_tViewSize.height * INSET_RATIO);
	}
}

/**
 * make sure all children go to the container
 */
void ScrollView::addChild(Node * child, int zOrder, int tag)
{
    if (m_pContainer != child) {
        m_pContainer->addChild(child, zOrder, tag);
    } else {
        Layer::addChild(child, zOrder, tag);
    }
}

void ScrollView::addChild(Node * child, int zOrder)
{
    this->addChild(child, zOrder, child->getTag());
}

void ScrollView::addChild(Node * child)
{
    this->addChild(child, child->getZOrder(), child->getTag());
}

/**
 * clip this view so that outside of the visible bounds can be hidden.
 */
void ScrollView::beforeDraw()
{
    if (m_bClippingToBounds)
    {
		m_bScissorRestored = false;
        Rect frame = getViewRect();
        if (EGLView::getInstance()->isScissorEnabled()) {
            m_bScissorRestored = true;
            m_tParentScissorRect = EGLView::getInstance()->getScissorRect();
            //set the intersection of _parentScissorRect and frame as the new scissor rect
            if (frame.intersectsRect(m_tParentScissorRect)) {
                float x = KD_MAX(frame.origin.x, m_tParentScissorRect.origin.x);
                float y = KD_MAX(frame.origin.y, m_tParentScissorRect.origin.y);
                float xx = KD_MIN(frame.origin.x+frame.size.width, m_tParentScissorRect.origin.x+m_tParentScissorRect.size.width);
                float yy = KD_MIN(frame.origin.y+frame.size.height, m_tParentScissorRect.origin.y+m_tParentScissorRect.size.height);
                EGLView::getInstance()->setScissorInPoints(x, y, xx-x, yy-y);
            }
        }
        else {
            glEnable(GL_SCISSOR_TEST);
            EGLView::getInstance()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
        }
    }
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void ScrollView::afterDraw()
{
    if (m_bClippingToBounds)
    {
        if (m_bScissorRestored) {//restore the parent's scissor rect
            EGLView::getInstance()->setScissorInPoints(m_tParentScissorRect.origin.x, m_tParentScissorRect.origin.y, m_tParentScissorRect.size.width, m_tParentScissorRect.size.height);
        }
        else {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}

void ScrollView::visit()
{
	// quick return if not visible
	if (!isVisible())
    {
		return;
    }

	kmGLPushMatrix();
	
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }

	this->transform();
    this->beforeDraw();

	if(m_pChildren)
    {
		int i=0;
		
		// draw children zOrder < 0
		for( ; i < m_pChildren->count(); i++ )
        {
			Node *child = static_cast<Node*>( m_pChildren->getObjectAtIndex(i) );
			if ( child->getZOrder() < 0 )
            {
				child->visit();
			}
            else
            {
				break;
            }
		}
		
		// this draw
		this->draw();
        
		// draw children zOrder >= 0
		for( ; i < m_pChildren->count(); i++ )
        {
			Node *child = static_cast<Node*>( m_pChildren->getObjectAtIndex(i) );
			child->visit();
		}
        
	}
    else
    {
		this->draw();
    }

    this->afterDraw();
	if ( m_pGrid && m_pGrid->isActive())
    {
		m_pGrid->afterDraw(this);
    }

	kmGLPopMatrix();
}

bool ScrollView::onTouchBegan(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    Rect frame = getViewRect();

    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (m_aTouches.size() > 2 ||
        m_bTouchMoved          ||
        !frame.containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(touch))))
    {
        return false;
    }

    if (std::find(m_aTouches.begin(), m_aTouches.end(), touch) == m_aTouches.end())
    {
        m_aTouches.push_back(touch);
    }

    if (m_aTouches.size() == 1)
    { // scrolling
        m_tTouchPoint     = this->convertTouchToNodeSpace(touch);
        m_bTouchMoved     = false;
        m_bDragging     = true; //dragging started
        m_tScrollDistance = Point(0.0f, 0.0f);
        m_fTouchLength    = 0.0f;
    }
    else if (m_aTouches.size() == 2)
    {
        m_tTouchPoint = (this->convertTouchToNodeSpace(m_aTouches[0]).getMidpoint(
                        this->convertTouchToNodeSpace(m_aTouches[1])));
        
        m_fTouchLength = m_pContainer->convertTouchToNodeSpace(m_aTouches[0]).getDistance(
                       m_pContainer->convertTouchToNodeSpace(m_aTouches[1]));
        
        m_bDragging  = false;
    } 
    return true;
}

void ScrollView::onTouchMoved(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }

    if (std::find(m_aTouches.begin(), m_aTouches.end(), touch) != m_aTouches.end())
    {
        if (m_aTouches.size() == 1 && m_bDragging)
        { // scrolling
            Point moveDistance, newPoint, maxInset, minInset;
            Rect  frame;
            float newX, newY;
            
            frame = getViewRect();

            newPoint     = this->convertTouchToNodeSpace(m_aTouches[0]);
            moveDistance = newPoint - m_tTouchPoint;
            
            float dis = 0.0f;
            if (m_eDirection == Direction::VERTICAL)
            {
                dis = moveDistance.y;
            }
            else if (m_eDirection == Direction::HORIZONTAL)
            {
                dis = moveDistance.x;
            }
            else
            {
                dis = kdSqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);
            }

            if (!m_bTouchMoved && kdFabsf(convertDistanceFromPointToInch(dis)) < MOVE_INCH )
            {
                //CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y);
                return;
            }
            
            if (!m_bTouchMoved)
            {
                moveDistance = Point::ZERO;
            }
            
            m_tTouchPoint = newPoint;
            m_bTouchMoved = true;
            
            if (frame.containsPoint(this->convertToWorldSpace(newPoint)))
            {
                switch (m_eDirection)
                {
                    case Direction::VERTICAL:
                        moveDistance = Point(0.0f, moveDistance.y);
                        break;
                    case Direction::HORIZONTAL:
                        moveDistance = Point(moveDistance.x, 0.0f);
                        break;
                    default:
                        break;
                }
                
                maxInset = m_tMaxInset;
                minInset = m_tMinInset;

                newX     = m_pContainer->getPosition().x + moveDistance.x;
                newY     = m_pContainer->getPosition().y + moveDistance.y;

                m_tScrollDistance = moveDistance;
                this->setContentOffset(Point(newX, newY));
            }
        }
        else if (m_aTouches.size() == 2 && !m_bDragging)
        {
            const float len = m_pContainer->convertTouchToNodeSpace(m_aTouches[0]).getDistance(
                                            m_pContainer->convertTouchToNodeSpace(m_aTouches[1]));
            this->setZoomScale(this->getZoomScale()*len/m_fTouchLength);
        }
    }
}

void ScrollView::onTouchEnded(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    
    auto touchIter = std::find(m_aTouches.begin(), m_aTouches.end(), touch);
    
    if (touchIter != m_aTouches.end())
    {
        if (m_aTouches.size() == 1 && m_bTouchMoved)
        {
            this->schedule(schedule_selector(ScrollView::deaccelerateScrolling));
        }
        m_aTouches.erase(touchIter);
    } 

    if (m_aTouches.size() == 0)
    {
        m_bDragging = false;    
        m_bTouchMoved = false;
    }
}

void ScrollView::onTouchCancelled(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    
    auto touchIter = std::find(m_aTouches.begin(), m_aTouches.end(), touch);
    m_aTouches.erase(touchIter);
    
    if (m_aTouches.size() == 0)
    {
        m_bDragging = false;    
        m_bTouchMoved = false;
    }
}

Rect ScrollView::getViewRect()
{
    Point screenPos = this->convertToWorldSpace(Point::ZERO);
    
    float scaleX = this->getScaleX();
    float scaleY = this->getScaleY();
    
    for (Node *p = m_pParent; p != NULL; p = p->getParent()) {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }

    // Support negative scaling. Not doing so causes intersectsRect calls
    // (eg: to check if the touch was within the bounds) to return false.
    // Note, Node::getScale will assert if X and Y scales are different.
    if(scaleX<0.f) {
        screenPos.x += m_tViewSize.width*scaleX;
        scaleX = -scaleX;
    }
    if(scaleY<0.f) {
        screenPos.y += m_tViewSize.height*scaleY;
        scaleY = -scaleY;
    }

    return Rect(screenPos.x, screenPos.y, m_tViewSize.width*scaleX, m_tViewSize.height*scaleY);
}
NS_CC_EXT_END
