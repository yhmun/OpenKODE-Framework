/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionInterval.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2011 Ricardo Quesada
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
 * ----------------------------------------------------------------------------------- */

#include "2d/actions/CCActionInterval.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/base_nodes/CCNode.h"
#include "2d/actions/CCActionInstant.h"

NS_CC_BEGIN

// Extra action for making a Sequence or Spawn when only adding one action to it.
class ExtraAction : public FiniteTimeAction
{
public:
    static ExtraAction* create();
    virtual ExtraAction* clone() const;
    virtual ExtraAction* reverse(void) const;
    virtual void update(float time);
    virtual void step(float dt);
};

ExtraAction* ExtraAction::create()
{
    ExtraAction* pRet = new ExtraAction();
    if (pRet)
    {
        pRet->autorelease();
    }
    return pRet;
}
ExtraAction* ExtraAction::clone(void) const
{
	// no copy constructor
	auto a = new ExtraAction();
	a->autorelease();
	return a;
}

ExtraAction* ExtraAction::reverse(void) const
{
    return ExtraAction::create();
}

void ExtraAction::update(float time)
{
    CC_UNUSED_PARAM(time);
}

void ExtraAction::step(float dt)
{
    CC_UNUSED_PARAM(dt);
}

//
// IntervalAction
//

bool ActionInterval::initWithDuration(float d)
{
    m_fDuration = d;

    // prevent division by 0
    // This comparison could be in step:, but it might decrease the performance
    // by 3% in heavy based action games.
    if (m_fDuration == 0)
    {
        m_fDuration = FLT_EPSILON;
    }

    m_fElapsed = 0;
    m_bFirstTick = true;

    return true;
}

bool ActionInterval::isDone(void) const
{
    return m_fElapsed >= m_fDuration;
}

void ActionInterval::step(float dt)
{
    if (m_bFirstTick)
    {
        m_bFirstTick = false;
        m_fElapsed = 0;
    }
    else
    {
        m_fElapsed += dt;
    }
    
    this->update(KD_MAX (0,                                  // needed for rewind. elapsed could be negative
                      KD_MIN(1, m_fElapsed /
                          KD_MAX(m_fDuration, FLT_EPSILON)   // division by 0
                          )
                      )
                 );
}

void ActionInterval::setAmplitudeRate(float amp)
{
    CC_UNUSED_PARAM(amp);
    // Abstract class needs implementation
    CCASSERT(0, "");
}

float ActionInterval::getAmplitudeRate(void)
{
    // Abstract class needs implementation
    CCASSERT(0, "");

    return 0;
}

void ActionInterval::startWithTarget(Node *target)
{
    FiniteTimeAction::startWithTarget(target);
    m_fElapsed = 0.0f;
    m_bFirstTick = true;
}

//
// Sequence
//

Sequence* Sequence::createWithTwoActions(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo)
{
    Sequence *pSequence = new Sequence();
    pSequence->initWithTwoActions(pActionOne, pActionTwo);
    pSequence->autorelease();

    return pSequence;
}

Sequence* Sequence::create(FiniteTimeAction *pAction1, ...)
{
    va_list params;
    va_start(params, pAction1);

    Sequence *pRet = Sequence::createWithVariableList(pAction1, params);

    va_end(params);
    
    return pRet;
}

Sequence* Sequence::createWithVariableList(FiniteTimeAction *pAction1, va_list args)
{
    FiniteTimeAction *pNow;
    FiniteTimeAction *pPrev = pAction1;
    bool bOneAction = true;

    while (pAction1)
    {
        pNow = va_arg(args, FiniteTimeAction*);
        if (pNow)
        {
            pPrev = createWithTwoActions(pPrev, pNow);
            bOneAction = false;
        }
        else
        {
            // If only one action is added to Sequence, make up a Sequence by adding a simplest finite time action.
            if (bOneAction)
            {
                pPrev = createWithTwoActions(pPrev, ExtraAction::create());
            }
            break;
        }
    }
    
    return ((Sequence*)pPrev);
}

Sequence* Sequence::create(Array* arrayOfActions)
{
    Sequence* pRet = NULL;
    do 
    {
        long count = arrayOfActions->count();
        CC_BREAK_IF(count == 0);

        FiniteTimeAction* prev = static_cast<FiniteTimeAction*>(arrayOfActions->getObjectAtIndex(0));

        if (count > 1)
        {
            for (long i = 1; i < count; ++i)
            {
                prev = createWithTwoActions(prev, static_cast<FiniteTimeAction*>(arrayOfActions->getObjectAtIndex(i)));
            }
        }
        else
        {
            // If only one action is added to Sequence, make up a Sequence by adding a simplest finite time action.
            prev = createWithTwoActions(prev, ExtraAction::create());
        }
        pRet = static_cast<Sequence*>(prev);
    }while (0);
    return pRet;
}

bool Sequence::initWithTwoActions(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo)
{
    CCASSERT(pActionOne != NULL, "");
    CCASSERT(pActionTwo != NULL, "");

    float d = pActionOne->getDuration() + pActionTwo->getDuration();
    ActionInterval::initWithDuration(d);

    m_aActions[0] = pActionOne;
    pActionOne->retain();

    m_aActions[1] = pActionTwo;
    pActionTwo->retain();

    return true;
}

Sequence* Sequence::clone(void) const
{
	// no copy constructor
	auto a = new Sequence();
    a->initWithTwoActions(m_aActions[0]->clone(), m_aActions[1]->clone() );
	a->autorelease();
	return a;
}

Sequence::~Sequence(void)
{
    CC_SAFE_RELEASE(m_aActions[0]);
    CC_SAFE_RELEASE(m_aActions[1]);
}

void Sequence::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_fSplit = m_aActions[0]->getDuration() / m_fDuration;
    m_nLast = -1;
}

void Sequence::stop(void)
{
    // Issue #1305
    if( m_nLast != - 1)
    {
        m_aActions[m_nLast]->stop();
    }

    ActionInterval::stop();
}

void Sequence::update(float t)
{
    int found = 0;
    float new_t = 0.0f;

    if( t < m_fSplit ) {
        // action[0]
        found = 0;
        if( m_fSplit != 0 )
            new_t = t / m_fSplit;
        else
            new_t = 1;

    } else {
        // action[1]
        found = 1;
        if ( m_fSplit == 1 )
            new_t = 1;
        else
            new_t = (t-m_fSplit) / (1 - m_fSplit );
    }

    if ( found==1 ) {

        if( m_nLast == -1 ) {
            // action[0] was skipped, execute it.
            m_aActions[0]->startWithTarget(m_pTarget);
            m_aActions[0]->update(1.0f);
            m_aActions[0]->stop();
        }
        else if( m_nLast == 0 )
        {
            // switching to action 1. stop action 0.
            m_aActions[0]->update(1.0f);
            m_aActions[0]->stop();
        }
    }
	else if(found==0 && m_nLast==1 )
	{
		// Reverse mode ?
		// XXX: Bug. this case doesn't contemplate when _last==-1, found=0 and in "reverse mode"
		// since it will require a hack to know if an action is on reverse mode or not.
		// "step" should be overriden, and the "reverseMode" value propagated to inner Sequences.
		m_aActions[1]->update(0);
		m_aActions[1]->stop();
	}
    // Last action found and it is done.
    if( found == m_nLast && m_aActions[found]->isDone() )
    {
        return;
    }

    // Last action found and it is done
    if( found != m_nLast )
    {
        m_aActions[found]->startWithTarget(m_pTarget);
    }

    m_aActions[found]->update(new_t);
    m_nLast = found;
}

Sequence* Sequence::reverse() const
{
    return Sequence::createWithTwoActions(m_aActions[1]->reverse(), m_aActions[0]->reverse());
}

//
// Repeat
//

Repeat* Repeat::create(FiniteTimeAction *pAction, unsigned int times)
{
    Repeat* pRepeat = new Repeat();
    pRepeat->initWithAction(pAction, times);
    pRepeat->autorelease();

    return pRepeat;
}

bool Repeat::initWithAction(FiniteTimeAction *pAction, unsigned int times)
{
    float d = pAction->getDuration() * times;

    if (ActionInterval::initWithDuration(d))
    {
        m_uTimes = times;
        m_pInnerAction = pAction;
        pAction->retain();

        m_bActionInstant = dynamic_cast<ActionInstant*>(pAction) ? true : false;
        //an instant action needs to be executed one time less in the update method since it uses startWithTarget to execute the action
        if (m_bActionInstant) 
        {
            m_uTimes -=1;
        }
        m_uTotal = 0;

        return true;
    }

    return false;
}

Repeat* Repeat::clone(void) const
{
	// no copy constructor
	auto a = new Repeat();
	a->initWithAction( m_pInnerAction->clone(), m_uTimes );
	a->autorelease();
	return a;
}

Repeat::~Repeat(void)
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

void Repeat::startWithTarget(Node *target)
{
    m_uTotal = 0;
    m_fNextDt = m_pInnerAction->getDuration()/m_fDuration;
    ActionInterval::startWithTarget(target);
    m_pInnerAction->startWithTarget(target);
}

void Repeat::stop(void)
{
    m_pInnerAction->stop();
    ActionInterval::stop();
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any 
// container action like Repeat, Sequence, Ease, etc..
void Repeat::update(float dt)
{
    if (dt >= m_fNextDt)
    {
        while (dt > m_fNextDt && m_uTotal < m_uTimes)
        {

            m_pInnerAction->update(1.0f);
            m_uTotal++;

            m_pInnerAction->stop();
            m_pInnerAction->startWithTarget(m_pTarget);
            m_fNextDt += m_pInnerAction->getDuration()/m_fDuration;
        }

        // fix for issue #1288, incorrect end value of repeat
        if(dt >= 1.0f && m_uTotal < m_uTimes) 
        {
            m_uTotal++;
        }

        // don't set an instant action back or update it, it has no use because it has no duration
        if (!m_bActionInstant)
        {
            if (m_uTotal == m_uTimes)
            {
                m_pInnerAction->update(1);
                m_pInnerAction->stop();
            }
            else
            {
                // issue #390 prevent jerk, use right update
                m_pInnerAction->update(dt - (m_fNextDt - m_pInnerAction->getDuration()/m_fDuration));
            }
        }
    }
    else
    {
        m_pInnerAction->update(kdFmodf(dt * m_uTimes,1.0f));
    }
}

bool Repeat::isDone(void) const
{
    return m_uTotal == m_uTimes;
}

Repeat* Repeat::reverse() const
{
    return Repeat::create(m_pInnerAction->reverse(), m_uTimes);
}

//
// RepeatForever
//
RepeatForever::~RepeatForever()
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

RepeatForever *RepeatForever::create(ActionInterval *pAction)
{
    RepeatForever *pRet = new RepeatForever();
    if (pRet && pRet->initWithAction(pAction))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool RepeatForever::initWithAction(ActionInterval *pAction)
{
    CCASSERT(pAction != NULL, "");
    pAction->retain();
    m_pInnerAction = pAction;
    return true;
}

RepeatForever *RepeatForever::clone(void) const
{
	// no copy constructor	
	auto a = new RepeatForever();
	a->initWithAction(m_pInnerAction->clone());
	a->autorelease();
	return a;
}

void RepeatForever::startWithTarget(Node* target)
{
    ActionInterval::startWithTarget(target);
    m_pInnerAction->startWithTarget(target);
}

void RepeatForever::step(float dt)
{
    m_pInnerAction->step(dt);
    if (m_pInnerAction->isDone())
    {
        float diff = m_pInnerAction->getElapsed() - m_pInnerAction->getDuration();
        m_pInnerAction->startWithTarget(m_pTarget);
        // to prevent jerk. issue #390, 1247
        m_pInnerAction->step(0.0f);
        m_pInnerAction->step(diff);
    }
}

bool RepeatForever::isDone() const
{
    return false;
}

RepeatForever *RepeatForever::reverse() const
{
    return RepeatForever::create(m_pInnerAction->reverse());
}

//
// Spawn
//

Spawn* Spawn::create(FiniteTimeAction *pAction1, ...)
{
    va_list params;
    va_start(params, pAction1);

    Spawn *pRet = Spawn::createWithVariableList(pAction1, params);

    va_end(params);
    
    return pRet;
}

Spawn* Spawn::createWithVariableList(FiniteTimeAction *pAction1, va_list args)
{
    FiniteTimeAction *pNow;
    FiniteTimeAction *pPrev = pAction1;
    bool bOneAction = true;

    while (pAction1)
    {
        pNow = va_arg(args, FiniteTimeAction*);
        if (pNow)
        {
            pPrev = createWithTwoActions(pPrev, pNow);
            bOneAction = false;
        }
        else
        {
            // If only one action is added to Spawn, make up a Spawn by adding a simplest finite time action.
            if (bOneAction)
            {
                pPrev = createWithTwoActions(pPrev, ExtraAction::create());
            }
            break;
        }
    }

    return ((Spawn*)pPrev);
}

Spawn* Spawn::create(Array *arrayOfActions)
{
    Spawn* pRet = NULL;
    do 
    {
        long count = arrayOfActions->count();
        CC_BREAK_IF(count == 0);
        FiniteTimeAction* prev = static_cast<FiniteTimeAction*>(arrayOfActions->getObjectAtIndex(0));
        if (count > 1)
        {
            for (int i = 1; i < arrayOfActions->count(); ++i)
            {
                prev = createWithTwoActions(prev, static_cast<FiniteTimeAction*>(arrayOfActions->getObjectAtIndex(i)));
            }
        }
        else
        {
            // If only one action is added to Spawn, make up a Spawn by adding a simplest finite time action.
            prev = createWithTwoActions(prev, ExtraAction::create());
        }
        pRet = (Spawn*)prev;
    }while (0);

    return pRet;
}

Spawn* Spawn::createWithTwoActions(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2)
{
    Spawn *pSpawn = new Spawn();
    pSpawn->initWithTwoActions(pAction1, pAction2);
    pSpawn->autorelease();

    return pSpawn;
}

bool Spawn:: initWithTwoActions(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2)
{
    CCASSERT(pAction1 != NULL, "");
    CCASSERT(pAction2 != NULL, "");

    bool bRet = false;

    float d1 = pAction1->getDuration();
    float d2 = pAction2->getDuration();

    if (ActionInterval::initWithDuration(KD_MAX(d1, d2)))
    {
        m_pOne = pAction1;
        m_pTwo = pAction2;

        if (d1 > d2)
        {
            m_pTwo = Sequence::createWithTwoActions(pAction2, DelayTime::create(d1 - d2));
        } 
        else if (d1 < d2)
        {
            m_pOne = Sequence::createWithTwoActions(pAction1, DelayTime::create(d2 - d1));
        }

        m_pOne->retain();
        m_pTwo->retain();

        bRet = true;
    }

    
    return bRet;
}

Spawn* Spawn::clone(void) const
{
	// no copy constructor	
	auto a = new Spawn();
    a->initWithTwoActions(m_pOne->clone(), m_pTwo->clone());

	a->autorelease();
	return a;
}

Spawn::~Spawn(void)
{
    CC_SAFE_RELEASE(m_pOne);
    CC_SAFE_RELEASE(m_pTwo);
}

void Spawn::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_pOne->startWithTarget(target);
    m_pTwo->startWithTarget(target);
}

void Spawn::stop(void)
{
    m_pOne->stop();
    m_pTwo->stop();
    ActionInterval::stop();
}

void Spawn::update(float time)
{
    if (m_pOne)
    {
        m_pOne->update(time);
    }
    if (m_pTwo)
    {
        m_pTwo->update(time);
    }
}

Spawn* Spawn::reverse() const
{
    return Spawn::createWithTwoActions(m_pOne->reverse(), m_pTwo->reverse());
}

//
// RotateTo
//

RotateTo* RotateTo::create(float fDuration, float fDeltaAngle)
{
    RotateTo* pRotateTo = new RotateTo();
    pRotateTo->initWithDuration(fDuration, fDeltaAngle);
    pRotateTo->autorelease();

    return pRotateTo;
}

bool RotateTo::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngleX = m_fDstAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

RotateTo* RotateTo::create(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    RotateTo* pRotateTo = new RotateTo();
    pRotateTo->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY);
    pRotateTo->autorelease();
    
    return pRotateTo;
}

bool RotateTo::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngleX = fDeltaAngleX;
        m_fDstAngleY = fDeltaAngleY;
        
        return true;
    }
    
    return false;
}

RotateTo* RotateTo::clone(void) const
{
	// no copy constructor
	auto a = new RotateTo();
	a->initWithDuration(m_fDuration, m_fDstAngleX, m_fDstAngleY);
	a->autorelease();
	return a;
}

void RotateTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    
    // Calculate X
    m_fStartAngleX = target->getRotationX();
    if (m_fStartAngleX > 0)
    {
        m_fStartAngleX = kdFmodf(m_fStartAngleX, 360.0f);
    }
    else
    {
        m_fStartAngleX = kdFmodf(m_fStartAngleX, -360.0f);
    }

    m_fDiffAngleX = m_fDstAngleX - m_fStartAngleX;
    if (m_fDiffAngleX > 180)
    {
        m_fDiffAngleX -= 360;
    }
    if (m_fDiffAngleX < -180)
    {
        m_fDiffAngleX += 360;
    }
    
    //Calculate Y: It's duplicated from calculating X since the rotation wrap should be the same
    m_fStartAngleY = m_pTarget->getRotationY();

    if (m_fStartAngleY > 0)
    {
        m_fStartAngleY = kdFmodf(m_fStartAngleY, 360.0f);
    }
    else
    {
        m_fStartAngleY = kdFmodf(m_fStartAngleY, -360.0f);
    }

    m_fDiffAngleY = m_fDstAngleY - m_fStartAngleY;
    if (m_fDiffAngleY > 180)
    {
        m_fDiffAngleY -= 360;
    }

    if (m_fDiffAngleY < -180)
    {
        m_fDiffAngleY += 360;
    }
}

void RotateTo::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setRotationX(m_fStartAngleX + m_fDiffAngleX * time);
        m_pTarget->setRotationY(m_fStartAngleY + m_fDiffAngleY * time);
    }
}

RotateTo *RotateTo::reverse() const
{
	CCASSERT(false, "RotateTo doesn't support the 'reverse' method");
	return nullptr;
}

//
// RotateBy
//

RotateBy* RotateBy::create(float fDuration, float fDeltaAngle)
{
    RotateBy *pRotateBy = new RotateBy();
    pRotateBy->initWithDuration(fDuration, fDeltaAngle);
    pRotateBy->autorelease();

    return pRotateBy;
}

bool RotateBy::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fAngleX = m_fAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

RotateBy* RotateBy::create(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    RotateBy *pRotateBy = new RotateBy();
    pRotateBy->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY);
    pRotateBy->autorelease();
    
    return pRotateBy;
}

bool RotateBy::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fAngleX = fDeltaAngleX;
        m_fAngleY = fDeltaAngleY;
        return true;
    }
    
    return false;
}

RotateBy* RotateBy::clone(void) const
{
	// no copy constructor
	auto a = new RotateBy();
    a->initWithDuration(m_fDuration, m_fAngleX, m_fAngleY);
	a->autorelease();
	return a;
}

void RotateBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_fStartAngleX = target->getRotationX();
    m_fStartAngleY = target->getRotationY();
}

void RotateBy::update(float time)
{
    // XXX: shall I add % 360
    if (m_pTarget)
    {
        m_pTarget->setRotationX(m_fStartAngleX + m_fAngleX * time);
        m_pTarget->setRotationY(m_fStartAngleY + m_fAngleY * time);
    }
}

RotateBy* RotateBy::reverse() const
{
    return RotateBy::create(m_fDuration, -m_fAngleX, -m_fAngleY);
}

//
// MoveBy
//

MoveBy* MoveBy::create(float duration, const Point& deltaPosition)
{
    MoveBy *pRet = new MoveBy();
    pRet->initWithDuration(duration, deltaPosition);
    pRet->autorelease();

    return pRet;
}

bool MoveBy::initWithDuration(float duration, const Point& deltaPosition)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_tPositionDelta = deltaPosition;
        return true;
    }

    return false;
}

MoveBy* MoveBy::clone(void) const
{
	// no copy constructor
	auto a = new MoveBy();
    a->initWithDuration(m_fDuration, m_tPositionDelta);
	a->autorelease();
	return a;
}

void MoveBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_tPreviousPosition = m_tStartPosition = target->getPosition();
}

MoveBy* MoveBy::reverse() const
{
    return MoveBy::create(m_fDuration, Point( -m_tPositionDelta.x, -m_tPositionDelta.y));
}


void MoveBy::update(float t)
{
    if (m_pTarget)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        Point currentPos = m_pTarget->getPosition();
        Point diff = currentPos - m_tPreviousPosition;
        m_tStartPosition = m_tStartPosition + diff;
        Point newPos =  m_tStartPosition + (m_tPositionDelta * t);
        m_pTarget->setPosition(newPos);
        m_tPreviousPosition = newPos;
#else
        m_pTarget->setPosition(ccpAdd( m_tStartPosition, ccpMult(m_tPositionDelta, t)));
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}

//
// MoveTo
//

MoveTo* MoveTo::create(float duration, const Point& position)
{
    MoveTo *pRet = new MoveTo();
    pRet->initWithDuration(duration, position);
    pRet->autorelease();

    return pRet;
}

bool MoveTo::initWithDuration(float duration, const Point& position)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_tEndPosition = position;
        return true;
    }

    return false;
}

MoveTo* MoveTo::clone(void) const
{
	// no copy constructor
	auto a = new MoveTo();
    a->initWithDuration(m_fDuration, m_tEndPosition);
	a->autorelease();
	return a;
}

void MoveTo::startWithTarget(Node *target)
{
    MoveBy::startWithTarget(target);
    m_tPositionDelta = m_tEndPosition - target->getPosition();
}


//
// SkewTo
//
SkewTo* SkewTo::create(float t, float sx, float sy)
{
    SkewTo *pSkewTo = new SkewTo();
    if (pSkewTo)
    {
        if (pSkewTo->initWithDuration(t, sx, sy))
        {
            pSkewTo->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pSkewTo);
        }
    }

    return pSkewTo;
}

bool SkewTo::initWithDuration(float t, float sx, float sy)
{
    bool bRet = false;

    if (ActionInterval::initWithDuration(t))
    {
        m_fEndSkewX = sx;
        m_fEndSkewY = sy;

        bRet = true;
    }

    return bRet;
}

SkewTo* SkewTo::clone(void) const
{
	// no copy constructor
	auto a = new SkewTo();
	a->initWithDuration(m_fDuration, m_fEndSkewX, m_fEndSkewY);
	a->autorelease();
	return a;
}

SkewTo* SkewTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in SkewTo");
	return nullptr;
}

void SkewTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    m_fStartSkewX = target->getSkewX();

    if (m_fStartSkewX > 0)
    {
        m_fStartSkewX = kdFmodf(m_fStartSkewX, 180.f);
    }
    else
    {
        m_fStartSkewX = kdFmodf(m_fStartSkewX, -180.f);
    }

    m_fDeltaX = m_fEndSkewX - m_fStartSkewX;

    if (m_fDeltaX > 180)
    {
        m_fDeltaX -= 360;
    }
    if (m_fDeltaX < -180)
    {
        m_fDeltaX += 360;
    }

    m_fStartSkewY = target->getSkewY();

    if (m_fStartSkewY > 0)
    {
        m_fStartSkewY = kdFmodf(m_fStartSkewY, 360.f);
    }
    else
    {
        m_fStartSkewY = kdFmodf(m_fStartSkewY, -360.f);
    }

    m_fDeltaY = m_fEndSkewY - m_fStartSkewY;

    if (m_fDeltaY > 180)
    {
        m_fDeltaY -= 360;
    }
    if (m_fDeltaY < -180)
    {
        m_fDeltaY += 360;
    }
}

void SkewTo::update(float t)
{
    m_pTarget->setSkewX(m_fStartSkewX + m_fDeltaX * t);
    m_pTarget->setSkewY(m_fStartSkewY + m_fDeltaY * t);
}

SkewTo::SkewTo()
: m_fSkewX(0.0)
, m_fSkewY(0.0)
, m_fStartSkewX(0.0)
, m_fStartSkewY(0.0)
, m_fEndSkewX(0.0)
, m_fEndSkewY(0.0)
, m_fDeltaX(0.0)
, m_fDeltaY(0.0)
{
}

//
// SkewBy
//
SkewBy* SkewBy::create(float t, float sx, float sy)
{
    SkewBy *pSkewBy = new SkewBy();
    if (pSkewBy)
    {
        if (pSkewBy->initWithDuration(t, sx, sy))
        {
            pSkewBy->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pSkewBy);
        }
    }

    return pSkewBy;
}

SkewBy * SkewBy::clone() const
{
	// no copy constructor
	auto a = new SkewBy();
	a->initWithDuration(m_fDuration, m_fSkewX, m_fSkewY);
	a->autorelease();
	return a;
}

bool SkewBy::initWithDuration(float t, float deltaSkewX, float deltaSkewY)
{
    bool bRet = false;

    if (SkewTo::initWithDuration(t, deltaSkewX, deltaSkewY))
    {
        m_fSkewX = deltaSkewX;
        m_fSkewY = deltaSkewY;

        bRet = true;
    }

    return bRet;
}

void SkewBy::startWithTarget(Node *target)
{
    SkewTo::startWithTarget(target);
    m_fDeltaX = m_fSkewX;
    m_fDeltaY = m_fSkewY;
    m_fEndSkewX = m_fStartSkewX + m_fDeltaX;
    m_fEndSkewY = m_fStartSkewY + m_fDeltaY;
}

SkewBy* SkewBy::reverse() const
{
    return SkewBy::create(m_fDuration, -m_fSkewX, -m_fSkewY);
}

//
// JumpBy
//

JumpBy* JumpBy::create(float duration, const Point& position, float height, int jumps)
{
    JumpBy *pJumpBy = new JumpBy();
    pJumpBy->initWithDuration(duration, position, height, jumps);
    pJumpBy->autorelease();

    return pJumpBy;
}

bool JumpBy::initWithDuration(float duration, const Point& position, float height, int jumps)
{
    CCASSERT(jumps>=0, "Number of jumps must be >= 0");
    
    if (ActionInterval::initWithDuration(duration) && jumps>=0)
    {
        m_tDelta = position;
        m_fHeight = height;
        m_nJumps = jumps;

        return true;
    }

    return false;
}

JumpBy* JumpBy::clone(void) const
{
	// no copy constructor
	auto a = new JumpBy();
	a->initWithDuration(m_fDuration, m_tDelta, m_fHeight, m_nJumps);
	a->autorelease();
	return a;
}

void JumpBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_tPreviousPos = m_tStartPosition = target->getPosition();
}

void JumpBy::update(float t)
{
    // parabolic jump (since v0.8.2)
    if (m_pTarget)
    {
        float frac = kdFmodf( t * m_nJumps, 1.0f );
        float y = m_fHeight * 4 * frac * (1 - frac);
        y += m_tDelta.y * t;

        float x = m_tDelta.x * t;
#if CC_ENABLE_STACKABLE_ACTIONS
        Point currentPos = m_pTarget->getPosition();

        Point diff = currentPos - m_tPreviousPos;
        m_tStartPosition = diff + m_tStartPosition;

        Point newPos = m_tStartPosition + Point(x,y);
        m_pTarget->setPosition(newPos);

        m_tPreviousPos = newPos;
#else
        m_pTarget->setPosition(ccpAdd( m_tStartPosition, Point(x,y)));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

JumpBy* JumpBy::reverse() const
{
    return JumpBy::create(m_fDuration, Point(-m_tDelta.x, -m_tDelta.y),
        m_fHeight, m_nJumps);
}

//
// JumpTo
//

JumpTo* JumpTo::create(float duration, const Point& position, float height, int jumps)
{
    JumpTo *pJumpTo = new JumpTo();
    pJumpTo->initWithDuration(duration, position, height, jumps);
    pJumpTo->autorelease();

    return pJumpTo;
}

JumpTo* JumpTo::clone(void) const
{
	// no copy constructor
	auto a = new JumpTo();
    a->initWithDuration(m_fDuration, m_tDelta, m_fHeight, m_nJumps);
	a->autorelease();
	return a;
}

JumpTo* JumpTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in JumpTo");
	return nullptr;
}

void JumpTo::startWithTarget(Node *target)
{
    JumpBy::startWithTarget(target);
    m_tDelta = Point(m_tDelta.x - m_tStartPosition.x, m_tDelta.y - m_tStartPosition.y);
}

// Bezier cubic formula:
//    ((1 - t) + t)3 = 1 
// Expands to ...
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (kdPowf(1-t,3) * a + 
            3*t*(kdPowf(1-t,2))*b + 
            3*kdPowf(t,2)*(1-t)*c +
            kdPowf(t,3)*d );
}

//
// BezierBy
//

BezierBy* BezierBy::create(float t, const ccBezierConfig& c)
{
    BezierBy *pBezierBy = new BezierBy();
    pBezierBy->initWithDuration(t, c);
    pBezierBy->autorelease();

    return pBezierBy;
}

bool BezierBy::initWithDuration(float t, const ccBezierConfig& c)
{
    if (ActionInterval::initWithDuration(t))
    {
        m_tConfig = c;
        return true;
    }

    return false;
}

void BezierBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_tPreviousPosition = m_tStartPosition = target->getPosition();
}

BezierBy* BezierBy::clone(void) const
{
	// no copy constructor
	auto a = new BezierBy();
	a->initWithDuration(m_fDuration, m_tConfig);
	a->autorelease();
	return a;
}

void BezierBy::update(float time)
{
    if (m_pTarget)
    {
        float xa = 0;
        float xb = m_tConfig.tControlPoint1.x;
        float xc = m_tConfig.tControlPoint2.x;
        float xd = m_tConfig.tEndPosition.x;

        float ya = 0;
        float yb = m_tConfig.tControlPoint1.y;
        float yc = m_tConfig.tControlPoint2.y;
        float yd = m_tConfig.tEndPosition.y;

        float x = bezierat(xa, xb, xc, xd, time);
        float y = bezierat(ya, yb, yc, yd, time);

#if CC_ENABLE_STACKABLE_ACTIONS
        Point currentPos = m_pTarget->getPosition();
        Point diff = currentPos - m_tPreviousPosition;
        m_tStartPosition = m_tStartPosition + diff;

        Point newPos = m_tStartPosition + Point(x,y);
        m_pTarget->setPosition(newPos);

        m_tPreviousPosition = newPos;
#else
        m_pTarget->setPosition(ccpAdd( m_tStartPosition, Point(x,y)));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

BezierBy* BezierBy::reverse(void) const
{
    ccBezierConfig r;

    r.tEndPosition = -m_tConfig.tEndPosition;
    r.tControlPoint1 = m_tConfig.tControlPoint2 + (-m_tConfig.tEndPosition);
    r.tControlPoint2 = m_tConfig.tControlPoint1 + (-m_tConfig.tEndPosition);

    BezierBy *pAction = BezierBy::create(m_fDuration, r);
    return pAction;
}

//
// BezierTo
//

BezierTo* BezierTo::create(float t, const ccBezierConfig& c)
{
    BezierTo *pBezierTo = new BezierTo();
    pBezierTo->initWithDuration(t, c);
    pBezierTo->autorelease();

    return pBezierTo;
}

bool BezierTo::initWithDuration(float t, const ccBezierConfig &c)
{
    if (ActionInterval::initWithDuration(t))
    {
        m_tToConfig = c;
        return true;
    }
    
    return false;
}

BezierTo* BezierTo::clone(void) const
{
	// no copy constructor
	auto a = new BezierTo();
	a->initWithDuration(m_fDuration, m_tToConfig);
	a->autorelease();
	return a;
}

void BezierTo::startWithTarget(Node *target)
{
    BezierBy::startWithTarget(target);
    m_tConfig.tControlPoint1 = m_tToConfig.tControlPoint1 - m_tStartPosition;
    m_tConfig.tControlPoint2 = m_tToConfig.tControlPoint2 - m_tStartPosition;
    m_tConfig.tEndPosition = m_tToConfig.tEndPosition - m_tStartPosition;
}

BezierTo* BezierTo::reverse() const
{
	CCASSERT(false, "CCBezierTo doesn't support the 'reverse' method");
	return nullptr;
}


//
// ScaleTo
//
ScaleTo* ScaleTo::create(float duration, float s)
{
    ScaleTo *pScaleTo = new ScaleTo();
    pScaleTo->initWithDuration(duration, s);
    pScaleTo->autorelease();

    return pScaleTo;
}

ScaleTo* ScaleTo::create(float duration, float sx, float sy)
{
    ScaleTo *pScaleTo = new ScaleTo();
    pScaleTo->initWithDuration(duration, sx, sy);
    pScaleTo->autorelease();

    return pScaleTo;
}

bool ScaleTo::initWithDuration(float duration, float s)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_fEndScaleX = s;
        m_fEndScaleY = s;

        return true;
    }

    return false;
}

bool ScaleTo::initWithDuration(float duration, float sx, float sy)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_fEndScaleX = sx;
        m_fEndScaleY = sy;

        return true;
    }

    return false;
}

ScaleTo* ScaleTo::clone(void) const
{
	// no copy constructor
	auto a = new ScaleTo();
	a->initWithDuration(m_fDuration, m_fEndScaleX, m_fEndScaleY);
	a->autorelease();
	return a;
}

ScaleTo* ScaleTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in ScaleTo");
	return nullptr;
}


void ScaleTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_fStartScaleX = target->getScaleX();
    m_fStartScaleY = target->getScaleY();
    m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
    m_fDeltaY = m_fEndScaleY - m_fStartScaleY;
}

void ScaleTo::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setScaleX(m_fStartScaleX + m_fDeltaX * time);
        m_pTarget->setScaleY(m_fStartScaleY + m_fDeltaY * time);
    }
}

//
// ScaleBy
//

ScaleBy* ScaleBy::create(float duration, float s)
{
    ScaleBy *pScaleBy = new ScaleBy();
    pScaleBy->initWithDuration(duration, s);
    pScaleBy->autorelease();

    return pScaleBy;
}

ScaleBy* ScaleBy::create(float duration, float sx, float sy)
{
    ScaleBy *pScaleBy = new ScaleBy();
    pScaleBy->initWithDuration(duration, sx, sy);
    pScaleBy->autorelease();

    return pScaleBy;
}

ScaleBy* ScaleBy::clone(void) const
{
	// no copy constructor
	auto a = new ScaleBy();
    a->initWithDuration(m_fDuration, m_fEndScaleX, m_fEndScaleY);
	a->autorelease();
	return a;
}

void ScaleBy::startWithTarget(Node *target)
{
    ScaleTo::startWithTarget(target);
    m_fDeltaX = m_fStartScaleX * m_fEndScaleX - m_fStartScaleX;
    m_fDeltaY = m_fStartScaleY * m_fEndScaleY - m_fStartScaleY;
}

ScaleBy* ScaleBy::reverse() const
{
    return ScaleBy::create(m_fDuration, 1 / m_fEndScaleX, 1 / m_fEndScaleY);
}

//
// Blink
//

Blink* Blink::create(float duration, int blinks)
{
    Blink *pBlink = new Blink();
    pBlink->initWithDuration(duration, blinks);
    pBlink->autorelease();

    return pBlink;
}

bool Blink::initWithDuration(float duration, int blinks)
{
    CCASSERT(blinks>=0, "blinks should be >= 0");
    
    if (ActionInterval::initWithDuration(duration) && blinks>=0)
    {
        m_nTimes = blinks;
        return true;
    }

    return false;
}

void Blink::stop()
{
    m_pTarget->setVisible(m_bOriginalState);
    ActionInterval::stop();
}

void Blink::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_bOriginalState = target->isVisible();
}

Blink* Blink::clone(void) const
{
	// no copy constructor
	auto a = new Blink();
	a->initWithDuration(m_fDuration, m_nTimes);
	a->autorelease();
	return a;
}

void Blink::update(float time)
{
    if (m_pTarget && ! isDone())
    {
        float slice = 1.0f / m_nTimes;
        float m = kdFmodf(time, slice);
        m_pTarget->setVisible(m > slice / 2 ? true : false);
    }
}

Blink* Blink::reverse() const
{
    return Blink::create(m_fDuration, m_nTimes);
}

//
// FadeIn
//

FadeIn* FadeIn::create(float d)
{
    FadeIn* pAction = new FadeIn();

    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

FadeIn* FadeIn::clone() const
{
	// no copy constructor
	auto a = new FadeIn();
    a->initWithDuration(m_fDuration);
	a->autorelease();
	return a;
}

void FadeIn::update(float time)
{
    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setOpacity((GLubyte)(255 * time));
    }
    /*_target->setOpacity((GLubyte)(255 * time));*/
}

ActionInterval* FadeIn::reverse() const
{
    return FadeOut::create(m_fDuration);
}

//
// FadeOut
//

FadeOut* FadeOut::create(float d)
{
    FadeOut* pAction = new FadeOut();

    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

FadeOut* FadeOut::clone() const
{
	// no copy constructor
	auto a = new FadeOut();
    a->initWithDuration(m_fDuration);
	a->autorelease();
	return a;
}

void FadeOut::update(float time)
{
    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setOpacity(GLubyte(255 * (1 - time)));
    }
    /*_target->setOpacity(GLubyte(255 * (1 - time)));*/    
}

ActionInterval* FadeOut::reverse() const
{
    return FadeIn::create(m_fDuration);
}

//
// FadeTo
//

FadeTo* FadeTo::create(float duration, GLubyte opacity)
{
    FadeTo *pFadeTo = new FadeTo();
    pFadeTo->initWithDuration(duration, opacity);
    pFadeTo->autorelease();

    return pFadeTo;
}

bool FadeTo::initWithDuration(float duration, GLubyte opacity)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_cToOpacity = opacity;
        return true;
    }

    return false;
}

FadeTo* FadeTo::clone() const
{
	// no copy constructor
	auto a = new FadeTo();
	a->initWithDuration(m_fDuration, m_cToOpacity);
	a->autorelease();
	return a;
}

FadeTo* FadeTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in FadeTo");
	return nullptr;
}

void FadeTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(target);
    if (pRGBAProtocol)
    {
        m_cFromOpacity = pRGBAProtocol->getOpacity();
    }
    /*_fromOpacity = target->getOpacity();*/
}

void FadeTo::update(float time)
{
    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setOpacity((GLubyte)(m_cFromOpacity + (m_cToOpacity - m_cFromOpacity) * time));
    }
    /*_target->setOpacity((GLubyte)(_fromOpacity + (_toOpacity - _fromOpacity) * time));*/
}

//
// TintTo
//
TintTo* TintTo::create(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    TintTo *pTintTo = new TintTo();
    pTintTo->initWithDuration(duration, red, green, blue);
    pTintTo->autorelease();

    return pTintTo;
}

bool TintTo::initWithDuration(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_tTo = Color3B(red, green, blue);
        return true;
    }

    return false;
}

TintTo* TintTo::clone() const
{
	// no copy constructor
	auto a = new TintTo();
	a->initWithDuration(m_fDuration, m_tTo.r, m_tTo.g, m_tTo.b);
	a->autorelease();
	return a;
}

TintTo* TintTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in TintTo");
	return nullptr;
}

void TintTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        m_tFrom = pRGBAProtocol->getColor();
    }
    /*_from = target->getColor();*/
}

void TintTo::update(float time)
{
    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setColor(Color3B(GLubyte(m_tFrom.r + (m_tTo.r - m_tFrom.r) * time), 
            (GLubyte)(m_tFrom.g + (m_tTo.g - m_tFrom.g) * time),
            (GLubyte)(m_tFrom.b + (m_tTo.b - m_tFrom.b) * time)));
    }    
}

//
// TintBy
//

TintBy* TintBy::create(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
    TintBy *pTintBy = new TintBy();
    pTintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue);
    pTintBy->autorelease();

    return pTintBy;
}

bool TintBy::initWithDuration(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_nDeltaR = deltaRed;
        m_nDeltaG = deltaGreen;
        m_nDeltaB = deltaBlue;

        return true;
    }

    return false;
}

TintBy* TintBy::clone() const
{
	// no copy constructor
	auto a = new TintBy();
	a->initWithDuration(m_fDuration, (GLubyte)m_nDeltaR, (GLubyte)m_nDeltaG, (GLubyte)m_nDeltaB);
	a->autorelease();
	return a;
}

void TintBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(target);
    if (pRGBAProtocol)
    {
        Color3B color = pRGBAProtocol->getColor();
        m_nFromR = color.r;
        m_nFromG = color.g;
        m_nFromB = color.b;
    }    
}

void TintBy::update(float time)
{
    RGBAProtocol *pRGBAProtocol = dynamic_cast<RGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setColor(Color3B((GLubyte)(m_nFromR + m_nDeltaR * time),
            (GLubyte)(m_nFromG + m_nDeltaG * time),
            (GLubyte)(m_nFromB + m_nDeltaB * time)));
    }    
}

TintBy* TintBy::reverse() const
{
    return TintBy::create(m_fDuration, -m_nDeltaR, -m_nDeltaG, -m_nDeltaB);
}

//
// DelayTime
//
DelayTime* DelayTime::create(float d)
{
    DelayTime* pAction = new DelayTime();

    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

DelayTime* DelayTime::clone() const
{
	// no copy constructor
	auto a = new DelayTime();
    a->initWithDuration(m_fDuration);
	a->autorelease();
	return a;
}

void DelayTime::update(float time)
{
    CC_UNUSED_PARAM(time);
    return;
}

DelayTime* DelayTime::reverse() const
{
    return DelayTime::create(m_fDuration);
}

//
// ReverseTime
//

ReverseTime* ReverseTime::create(FiniteTimeAction *pAction)
{
    // casting to prevent warnings
    ReverseTime *pReverseTime = new ReverseTime();
    pReverseTime->initWithAction( pAction->clone() );
    pReverseTime->autorelease();

    return pReverseTime;
}

bool ReverseTime::initWithAction(FiniteTimeAction *pAction)
{
    CCASSERT(pAction != NULL, "");
    CCASSERT(pAction != m_pOther, "");

    if (ActionInterval::initWithDuration(pAction->getDuration()))
    {
        // Don't leak if action is reused
        CC_SAFE_RELEASE(m_pOther);

        m_pOther = pAction;
        pAction->retain();

        return true;
    }

    return false;
}

ReverseTime* ReverseTime::clone() const
{
	// no copy constructor
	auto a = new ReverseTime();
	a->initWithAction( m_pOther->clone() );
	a->autorelease();
	return a;
}

ReverseTime::ReverseTime() : m_pOther(NULL) 
{

}

ReverseTime::~ReverseTime(void)
{
    CC_SAFE_RELEASE(m_pOther);
}

void ReverseTime::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_pOther->startWithTarget(target);
}

void ReverseTime::stop(void)
{
    m_pOther->stop();
    ActionInterval::stop();
}

void ReverseTime::update(float time)
{
    if (m_pOther)
    {
        m_pOther->update(1 - time);
    }
}

ReverseTime* ReverseTime::reverse() const
{
    // XXX: This looks like a bug
    return (ReverseTime*)m_pOther->clone();
}

//
// Animate
//
Animate* Animate::create(Animation *pAnimation)
{
    Animate *pAnimate = new Animate();
    pAnimate->initWithAnimation(pAnimation);
    pAnimate->autorelease();

    return pAnimate;
}

Animate::Animate()
: m_pSplitTimes(new std::vector<float>)
, m_nNextFrame(0)
, m_pOrigFrame(NULL)
, m_uExecutedLoops(0)
, m_pAnimation(NULL)
{

}

Animate::~Animate()
{
    CC_SAFE_RELEASE(m_pAnimation);
    CC_SAFE_RELEASE(m_pOrigFrame);
    CC_SAFE_DELETE(m_pSplitTimes);
}

bool Animate::initWithAnimation(Animation *pAnimation)
{
    CCASSERT( pAnimation!=NULL, "Animate: argument Animation must be non-NULL");

    float singleDuration = pAnimation->getDuration();

    if ( ActionInterval::initWithDuration(singleDuration * pAnimation->getLoops() ) ) 
    {
        m_nNextFrame = 0;
        setAnimation(pAnimation);
        m_pOrigFrame = NULL;
        m_uExecutedLoops = 0;

        m_pSplitTimes->reserve(pAnimation->getFrames()->count());

        float accumUnitsOfTime = 0;
        float newUnitOfTimeValue = singleDuration / pAnimation->getTotalDelayUnits();

        Array* pFrames = pAnimation->getFrames();
        CCARRAY_VERIFY_TYPE(pFrames, AnimationFrame*);

        Object* pObj = NULL;
        CCARRAY_FOREACH(pFrames, pObj)
        {
            AnimationFrame* frame = static_cast<AnimationFrame*>(pObj);
            float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
            accumUnitsOfTime += frame->getDelayUnits();
            m_pSplitTimes->push_back(value);
        }    
        return true;
    }
    return false;
}

void Animate::setAnimation(cocos2d::Animation *animation)
{
    if (m_pAnimation != animation)
    {
        CC_SAFE_RETAIN(animation);
        CC_SAFE_RELEASE(m_pAnimation);
        m_pAnimation = animation;
    }
}

Animate* Animate::clone() const
{
	// no copy constructor
	auto a = new Animate();
	a->initWithAnimation(m_pAnimation->clone());
	a->autorelease();
	return a;
}

void Animate::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    Sprite *pSprite = static_cast<Sprite*>(target);

    CC_SAFE_RELEASE(m_pOrigFrame);

    if (m_pAnimation->getRestoreOriginalFrame())
    {
        m_pOrigFrame = pSprite->getDisplayFrame();
        m_pOrigFrame->retain();
    }
    m_nNextFrame = 0;
    m_uExecutedLoops = 0;
}

void Animate::stop(void)
{
    if (m_pAnimation->getRestoreOriginalFrame() && m_pTarget)
    {
        static_cast<Sprite*>(m_pTarget)->setDisplayFrame(m_pOrigFrame);
    }

    ActionInterval::stop();
}

void Animate::update(float t)
{
    // if t==1, ignore. Animation should finish with t==1
    if( t < 1.0f ) {
        t *= m_pAnimation->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if( loopNumber > m_uExecutedLoops ) {
            m_nNextFrame = 0;
            m_uExecutedLoops++;
        }

        // new t for animations
        t = kdFmodf(t, 1.0f);
    }

    Array* frames = m_pAnimation->getFrames();
    long numberOfFrames = frames->count();
    SpriteFrame *frameToDisplay = NULL;

    for( int i=m_nNextFrame; i < numberOfFrames; i++ ) {
        float splitTime = m_pSplitTimes->at(i);

        if( splitTime <= t ) {
            AnimationFrame* frame = static_cast<AnimationFrame*>(frames->getObjectAtIndex(i));
            frameToDisplay = frame->getSpriteFrame();
            static_cast<Sprite*>(m_pTarget)->setDisplayFrame(frameToDisplay);

            Dictionary* dict = frame->getUserInfo();
            if( dict )
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:AnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }
            m_nNextFrame = i+1;
        }
        // Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
        else {
            break;
        }
    }
}

Animate* Animate::reverse() const
{
    Array* pOldArray = m_pAnimation->getFrames();
    Array* pNewArray = Array::createWithCapacity(pOldArray->count());
   
    CCARRAY_VERIFY_TYPE(pOldArray, AnimationFrame*);

    if (pOldArray->count() > 0)
    {
        Object* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(pOldArray, pObj)
        {
            AnimationFrame* pElement = static_cast<AnimationFrame*>(pObj);
            if (! pElement)
            {
                break;
            }

            pNewArray->addObject(pElement->clone());
        }
    }

    Animation *newAnim = Animation::create(pNewArray, m_pAnimation->getDelayPerUnit(), m_pAnimation->getLoops());
    newAnim->setRestoreOriginalFrame(m_pAnimation->getRestoreOriginalFrame());
    return Animate::create(newAnim);
}

// TargetedAction

TargetedAction::TargetedAction()
: m_pAction(NULL)
, m_pForcedTarget(NULL)
{

}

TargetedAction::~TargetedAction()
{
    CC_SAFE_RELEASE(m_pForcedTarget);
    CC_SAFE_RELEASE(m_pAction);
}

TargetedAction* TargetedAction::create(Node* target, FiniteTimeAction* pAction)
{
    TargetedAction* p = new TargetedAction();
    p->initWithTarget(target, pAction);
    p->autorelease();
    return p;
}


bool TargetedAction::initWithTarget(Node* target, FiniteTimeAction* pAction)
{
    if(ActionInterval::initWithDuration(pAction->getDuration()))
    {
        CC_SAFE_RETAIN(target);
        m_pForcedTarget = target;
        CC_SAFE_RETAIN(pAction);
        m_pAction = pAction;
        return true;
    }
    return false;
}

TargetedAction* TargetedAction::clone() const
{
	// no copy constructor	
	auto a = new TargetedAction();
    // win32 : use the _other's copy object.
	a->initWithTarget(m_pForcedTarget, m_pAction->clone());
	a->autorelease();
	return a;
}

TargetedAction* TargetedAction::reverse(void) const
{
	// just reverse the internal action
	auto a = new TargetedAction();
	a->initWithTarget(m_pForcedTarget, m_pAction->reverse());
	a->autorelease();
	return a;
}

void TargetedAction::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_pAction->startWithTarget(m_pForcedTarget);
}

void TargetedAction::stop(void)
{
    m_pAction->stop();
}

void TargetedAction::update(float time)
{
    m_pAction->update(time);
}

void TargetedAction::setForcedTarget(Node* forcedTarget)
{
    if( m_pForcedTarget != forcedTarget ) {
        CC_SAFE_RETAIN(forcedTarget);
        CC_SAFE_RELEASE(m_pForcedTarget);
        m_pForcedTarget = forcedTarget;
    }
}

NS_CC_END
