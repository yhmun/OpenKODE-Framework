/* -----------------------------------------------------------------------------------
 *
 *      File            CCAction.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Zynga Inc.
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

#include "2d/actions/CCAction.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/base_nodes/CCNode.h"
#include "2d/CCDirector.h"

NS_CC_BEGIN
//
// Action Base Class
//

Action::Action()
:m_pOriginalTarget(nullptr)
,m_pTarget(nullptr)
,m_nTag(Action::INVALID_TAG)
{
}

Action::~Action()
{
    CCLOGINFO("deallocing Action: %p - tag: %i", this, m_nTag);
}

const char* Action::description() const
{
    return String::createWithFormat("<Action | Tag = %d>", m_nTag)->getCString();
}

void Action::startWithTarget(Node *aTarget)
{
    m_pOriginalTarget = m_pTarget = aTarget;
}

void Action::stop()
{
    m_pTarget = nullptr;
}

bool Action::isDone() const
{
    return true;
}

void Action::step(float dt)
{
    CC_UNUSED_PARAM(dt);
    CCLOG("[Action step]. override me");
}

void Action::update(float time)
{
    CC_UNUSED_PARAM(time);
    CCLOG("[Action update]. override me");
}

//
// Speed
//
Speed::Speed()
: m_fSpeed(0.0)
, m_pInnerAction(nullptr)
{
}

Speed::~Speed()
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

Speed* Speed::create(ActionInterval* action, float speed)
{
    Speed *ret = new Speed();
    if (ret && ret->initWithAction(action, speed))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Speed::initWithAction(ActionInterval *action, float speed)
{
    CCASSERT(action != nullptr, "");
    action->retain();
    m_pInnerAction = action;
    m_fSpeed = speed;
    return true;
}

Speed *Speed::clone() const
{
	// no copy constructor
	auto a = new Speed();
	a->initWithAction(m_pInnerAction->clone(), m_fSpeed);
	a->autorelease();
	return  a;
}

void Speed::startWithTarget(Node* target)
{
    Action::startWithTarget(target);
    m_pInnerAction->startWithTarget(target);
}

void Speed::stop()
{
    m_pInnerAction->stop();
    Action::stop();
}

void Speed::step(float dt)
{
    m_pInnerAction->step(dt * m_fSpeed);
}

bool Speed::isDone() const
{
    return m_pInnerAction->isDone();
}

Speed *Speed::reverse() const
{

	return Speed::create(m_pInnerAction->reverse(), m_fSpeed);
}

void Speed::setInnerAction(ActionInterval *action)
{
    if (m_pInnerAction != action)
    {
        CC_SAFE_RELEASE(m_pInnerAction);
        m_pInnerAction = action;
        CC_SAFE_RETAIN(m_pInnerAction);
    }
}

//
// Follow
//
Follow::~Follow()
{
    CC_SAFE_RELEASE(m_pFollowedNode);
}

Follow* Follow::create(Node *followedNode, const Rect& rect/* = Rect::ZERO*/)
{
    Follow *follow = new Follow();
    if (follow && follow->initWithTarget(followedNode, rect))
    {
        follow->autorelease();
        return follow;
    }
    CC_SAFE_DELETE(follow);
    return nullptr;
}

Follow* Follow::clone() const
{
	// no copy constructor
	auto a = new Follow();
	a->initWithTarget(m_pFollowedNode, m_tWorldRect);
	a->autorelease();
	return a;
}

Follow* Follow::reverse() const
{
    return clone();
}

bool Follow::initWithTarget(Node *followedNode, const Rect& rect/* = Rect::ZERO*/)
{
    CCASSERT(followedNode != nullptr, "");
 
    followedNode->retain();
    m_pFollowedNode = followedNode;
	m_tWorldRect = rect;
    if (rect.equals(Rect::ZERO))
    {
        m_bBoundarySet = false;
    }
    else
    {
        m_bBoundarySet = true;
    }
    
    m_bBoundaryFullyCovered = false;

    Size winSize = Director::getInstance()->getWinSize();
    m_tFullScreenSize = Point(winSize.width, winSize.height);
    m_tHalfScreenSize = m_tFullScreenSize * 0.5f;

    if (m_bBoundarySet)
    {
        m_fLeftBoundary = -((rect.origin.x+rect.size.width) - m_tFullScreenSize.x);
        m_fRightBoundary = -rect.origin.x ;
        m_fTopBoundary = -rect.origin.y;
        m_fBottomBoundary = -((rect.origin.y+rect.size.height) - m_tFullScreenSize.y);

        if(m_fRightBoundary < m_fLeftBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fRightBoundary = m_fLeftBoundary = (m_fLeftBoundary + m_fRightBoundary) / 2;
        }
        if(m_fTopBoundary < m_fBottomBoundary)
        {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fTopBoundary = m_fBottomBoundary = (m_fTopBoundary + m_fBottomBoundary) / 2;
        }

        if( (m_fTopBoundary == m_fBottomBoundary) && (m_fLeftBoundary == m_fRightBoundary) )
        {
            m_bBoundaryFullyCovered = true;
        }
    }
    
    return true;
}

void Follow::step(float dt)
{
    CC_UNUSED_PARAM(dt);

    if(m_bBoundarySet)
    {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if(m_bBoundaryFullyCovered)
            return;

        Point tempPos = m_tHalfScreenSize - m_pFollowedNode->getPosition();

        m_pTarget->setPosition(Point(clampf(tempPos.x, m_fLeftBoundary, m_fRightBoundary),
                                   clampf(tempPos.y, m_fBottomBoundary, m_fTopBoundary)));
    }
    else
    {
        m_pTarget->setPosition(m_tHalfScreenSize - m_pFollowedNode->getPosition());
    }
}

bool Follow::isDone() const
{
    return ( !m_pFollowedNode->isRunning() );
}

void Follow::stop()
{
    m_pTarget = nullptr;
    Action::stop();
}

NS_CC_END


