/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlStepper.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://yannickloriot.com
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

#include "extensions/GUI/CCControlExtension/CCControlStepper.h"

NS_CC_EXT_BEGIN

#define ControlStepperLabelColorEnabled   Color3B(55, 55, 55)
#define ControlStepperLabelColorDisabled  Color3B(147, 147, 147)

#define ControlStepperLabelFont           "CourierNewPSMT"


#define kAutorepeatDeltaTime                0.15f
#define kAutorepeatIncreaseTimeIncrement    12

ControlStepper::ControlStepper()
: m_dValue(0.0)
, m_bContinuous(false)
, m_bAutorepeat(false)
, m_bWraps(false)
, m_dMinimumValue(0.0)
, m_dMaximumValue(0.0)
, m_dStepValue(0.0)
, m_bTouchInsideFlag(false)
, m_tTouchedPart(Part::NONE)
, m_nAutorepeatCount(0)
, m_pMinusSprite(NULL)
, m_pPlusSprite(NULL)
, m_pMinusLabel(NULL)
, m_pPlusLabel(NULL)
{

}

ControlStepper::~ControlStepper()
{
    unscheduleAllSelectors();
    
    CC_SAFE_RELEASE(m_pMinusSprite);
    CC_SAFE_RELEASE(m_pPlusSprite);
    CC_SAFE_RELEASE(m_pMinusLabel);
    CC_SAFE_RELEASE(m_pPlusLabel);
}

bool ControlStepper::initWithMinusSpriteAndPlusSprite(Sprite *minusSprite, Sprite *plusSprite)
{
    if (Control::init())
    {
        CCASSERT(minusSprite,   "Minus sprite must be not nil");
        CCASSERT(plusSprite,    "Plus sprite must be not nil");
        
        // Set the default values
        m_bAutorepeat                         = true;
        m_bContinuous                         = true;
        m_dMinimumValue                       = 0;
        m_dMaximumValue                       = 100;
        m_dValue                              = 0;
        m_dStepValue                          = 1;
        m_bWraps                              = false;
        this->ignoreAnchorPointForPosition( false );
    
        // Add the minus components
        this->setMinusSprite(minusSprite);
		m_pMinusSprite->setPosition( Point(minusSprite->getContentSize().width / 2, minusSprite->getContentSize().height / 2) );
		this->addChild(m_pMinusSprite);
        
        this->setMinusLabel( LabelTTF::create("-", ControlStepperLabelFont, 40));
        m_pMinusLabel->setColor(ControlStepperLabelColorDisabled);
        m_pMinusLabel->setPosition(Point(m_pMinusSprite->getContentSize().width / 2, m_pMinusSprite->getContentSize().height / 2) );
        m_pMinusSprite->addChild(m_pMinusLabel);
        
        // Add the plus components 
        this->setPlusSprite( plusSprite );
		m_pPlusSprite->setPosition( Point(minusSprite->getContentSize().width + plusSprite->getContentSize().width / 2, 
                                                  minusSprite->getContentSize().height / 2) );
		this->addChild(m_pPlusSprite);
        
        this->setPlusLabel( LabelTTF::create("+", ControlStepperLabelFont, 40 ));
        m_pPlusLabel->setColor( ControlStepperLabelColorEnabled );
        m_pPlusLabel->setPosition( Point(m_pPlusSprite->getContentSize().width / 2, m_pPlusSprite->getContentSize().height / 2) );
        m_pPlusSprite->addChild(m_pPlusLabel);
        
        // Defines the content size
        Rect maxRect = ControlUtils::RectUnion(m_pMinusSprite->getBoundingBox(), m_pPlusSprite->getBoundingBox());
        this->setContentSize( Size(m_pMinusSprite->getContentSize().width + m_pPlusSprite->getContentSize().height, maxRect.size.height) );
        return true;
    }
    return false;
}

ControlStepper* ControlStepper::create(Sprite *minusSprite, Sprite *plusSprite)
{
    ControlStepper* pRet = new ControlStepper();
    if (pRet != NULL && pRet->initWithMinusSpriteAndPlusSprite(minusSprite, plusSprite))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

//#pragma mark Properties

void ControlStepper::setWraps(bool wraps)
{
    m_bWraps = wraps;
    
    if (m_bWraps)
    {
        m_pMinusLabel->setColor( ControlStepperLabelColorEnabled );
        m_pPlusLabel->setColor(ControlStepperLabelColorEnabled );
    }
    
    this->setValue( m_dValue );
}

void ControlStepper::setMinimumValue(double minimumValue)
{
    if (minimumValue >= m_dMaximumValue)
    {
        CCASSERT(0, "Must be numerically less than maximumValue.");
    }
    
    m_dMinimumValue   = minimumValue;
    this->setValue( m_dValue );
}

void ControlStepper::setMaximumValue(double maximumValue)
{
    if (maximumValue <= m_dMinimumValue)
    {
        CCASSERT(0, "Must be numerically greater than minimumValue.");
    }
    
    m_dMaximumValue   = maximumValue;
    this->setValue(m_dValue);
}

void ControlStepper::setValue(double value)
{
    this->setValueWithSendingEvent(value, true);
}

double ControlStepper::getValue() const
{
    return m_dValue;
}

void ControlStepper::setStepValue(double stepValue)
{
    if (stepValue <= 0)
    {
        CCASSERT(0,"Must be numerically greater than 0.");
    }

    m_dStepValue  = stepValue;
}

bool ControlStepper::isContinuous() const
{
    return m_bContinuous;
}
//#pragma mark -
//#pragma mark ControlStepper Public Methods

void ControlStepper::setValueWithSendingEvent(double value, bool send)
{
    if (value < m_dMinimumValue)
    {
        value = m_bWraps ? m_dMaximumValue : m_dMinimumValue;
    } else if (value > m_dMaximumValue)
    {
        value = m_bWraps ? m_dMinimumValue : m_dMaximumValue;
    }
    
    m_dValue = value;
    
    if (!m_bWraps)
    {
        m_pMinusLabel->setColor((value == m_dMinimumValue) ? ControlStepperLabelColorDisabled : ControlStepperLabelColorEnabled);
        m_pPlusLabel->setColor((value == m_dMaximumValue) ? ControlStepperLabelColorDisabled : ControlStepperLabelColorEnabled);
    }
    
    if (send)
    {
        this->sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
    }
}

void ControlStepper::startAutorepeat()
{
    m_nAutorepeatCount    = -1;
    
    this->schedule(schedule_selector(ControlStepper::update), kAutorepeatDeltaTime, kRepeatForever, kAutorepeatDeltaTime * 3);
}

/** Stop the autorepeat. */
void ControlStepper::stopAutorepeat()
{
    this->unschedule(schedule_selector(ControlStepper::update));
}

void ControlStepper::update(float dt)
{
    m_nAutorepeatCount++;
    
    if ((m_nAutorepeatCount < kAutorepeatIncreaseTimeIncrement) && (m_nAutorepeatCount % 3) != 0)
        return;
    
    if (m_tTouchedPart == Part::MINUS)
    {
        this->setValueWithSendingEvent(m_dValue - m_dStepValue, m_bContinuous);
    } else if (m_tTouchedPart == Part::PLUS)
    {
        this->setValueWithSendingEvent(m_dValue + m_dStepValue, m_bContinuous);
    }
}

//#pragma mark ControlStepper Private Methods

void ControlStepper::updateLayoutUsingTouchLocation(Point location)
{
    if (location.x < m_pMinusSprite->getContentSize().width
        && m_dValue > m_dMinimumValue)
    {
        m_tTouchedPart        = Part::MINUS;
        
        m_pMinusSprite->setColor(Color3B::GRAY);
        m_pPlusSprite->setColor(Color3B::WHITE);
    } else if (location.x >= m_pMinusSprite->getContentSize().width
               && m_dValue < m_dMaximumValue)
    {
        m_tTouchedPart        = Part::PLUS;
        
        m_pMinusSprite->setColor(Color3B::WHITE);
        m_pPlusSprite->setColor(Color3B::GRAY);
    } else
    {
        m_tTouchedPart        = Part::NONE;
        
        m_pMinusSprite->setColor(Color3B::WHITE);
        m_pPlusSprite->setColor(Color3B::WHITE);
    }
}


bool ControlStepper::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!isTouchInside(pTouch) || !isEnabled() || !isVisible())
    {
        return false;
    }
    
    Point location    = this->getTouchLocation(pTouch);
    this->updateLayoutUsingTouchLocation(location);
    
    m_bTouchInsideFlag = true;
    
    if (m_bAutorepeat)
    {
        this->startAutorepeat();
    }
    
    return true;
}

void ControlStepper::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (this->isTouchInside(pTouch))
    {
        Point location    = this->getTouchLocation(pTouch);
        this->updateLayoutUsingTouchLocation(location);
        
        if (!m_bTouchInsideFlag)
        {
            m_bTouchInsideFlag    = true;
            
            if (m_bAutorepeat)
            {
                this->startAutorepeat();
            }
        }
    }
    else
    {
        m_bTouchInsideFlag    = false;
        
        m_tTouchedPart        = Part::NONE;
        
        m_pMinusSprite->setColor(Color3B::WHITE);
        m_pPlusSprite->setColor(Color3B::WHITE);
        
        if (m_bAutorepeat)
        {
            this->stopAutorepeat();
        }
    }
}

void ControlStepper::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    m_pMinusSprite->setColor(Color3B::WHITE);
    m_pPlusSprite->setColor(Color3B::WHITE);
    
    if (m_bAutorepeat)
    {
        this->stopAutorepeat();
    }
    
    if (this->isTouchInside(pTouch))
    {
        Point location    = this->getTouchLocation(pTouch);
        
        this->setValue(m_dValue + ((location.x < m_pMinusSprite->getContentSize().width) ? (0.0-m_dStepValue) : m_dStepValue));
    }
}

NS_CC_EXT_END
