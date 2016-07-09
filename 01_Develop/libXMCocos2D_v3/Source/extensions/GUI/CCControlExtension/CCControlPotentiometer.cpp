/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlPotentiometer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2011      Yannick Loriot.
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

#include "extensions/GUI/CCControlExtension/CCControlPotentiometer.h"

NS_CC_EXT_BEGIN

ControlPotentiometer::ControlPotentiometer()
: m_fValue(0.0f)
, m_fMinimumValue(0.0f)
, m_fMaximumValue(0.0f)
, m_pThumbSprite(NULL)
, m_pProgressTimer(NULL)
{

}

ControlPotentiometer::~ControlPotentiometer()
{
    CC_SAFE_RELEASE(m_pThumbSprite);
    CC_SAFE_RELEASE(m_pProgressTimer);
}

ControlPotentiometer* ControlPotentiometer::create(const char* backgroundFile, const char* progressFile, const char* thumbFile)
{
    ControlPotentiometer* pRet = new ControlPotentiometer();
    if (pRet != NULL)
    {
        // Prepare track for potentiometer
        Sprite *backgroundSprite      = Sprite::create(backgroundFile);

        // Prepare thumb for potentiometer
        Sprite *thumbSprite           = Sprite::create(thumbFile);

        // Prepare progress for potentiometer
        ProgressTimer *progressTimer  = ProgressTimer::create(Sprite::create(progressFile));
        //progressTimer.type              = ProgressTimer::RADIALCW;
        if (pRet->initWithTrackSprite_ProgressTimer_ThumbSprite(backgroundSprite, progressTimer, thumbSprite))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pRet);
        }
    }
    return pRet;
}

bool ControlPotentiometer::initWithTrackSprite_ProgressTimer_ThumbSprite(Sprite* trackSprite, ProgressTimer* progressTimer, Sprite* thumbSprite)
{
    if (Control::init())
    {
        setProgressTimer(progressTimer);
        setThumbSprite(thumbSprite);
        thumbSprite->setPosition(progressTimer->getPosition());
        
        addChild(thumbSprite, 2);
        addChild(progressTimer, 1);
        addChild(trackSprite);
        
        setContentSize(trackSprite->getContentSize());
        
        // Init default values
        m_fMinimumValue           = 0.0f;
        m_fMaximumValue           = 1.0f;
        setValue(m_fMinimumValue);
        return true;
    }
    return false;
}

void ControlPotentiometer::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    if (m_pThumbSprite != NULL)
    {
        m_pThumbSprite->setOpacity((enabled) ? 255 : 128);
    }
}

void ControlPotentiometer::setValue(float value)
{
    // set new value with sentinel
    if (value < m_fMinimumValue)
    {
        value                   = m_fMinimumValue;
    }
	
    if (value > m_fMaximumValue) 
    {
        value                   = m_fMaximumValue;
    }
    
    m_fValue                      = value;
    
    // Update thumb and progress position for new value
    float percent               = (value - m_fMinimumValue) / (m_fMaximumValue - m_fMinimumValue);
    m_pProgressTimer->setPercentage(percent * 100.0f);
    m_pThumbSprite->setRotation(percent * 360.0f);
    
    sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);    
}

float ControlPotentiometer::getValue()
{
    return m_fValue;
}

void ControlPotentiometer::setMinimumValue(float minimumValue)
{
    m_fMinimumValue       = minimumValue;
    
    if (m_fMinimumValue >= m_fMaximumValue)
    {
        m_fMaximumValue   = m_fMinimumValue + 1.0f;
    }
    
    setValue(m_fMaximumValue);
}

float ControlPotentiometer::getMinimumValue()
{
    return m_fMinimumValue;
}

void ControlPotentiometer::setMaximumValue(float maximumValue)
{
    m_fMaximumValue       = maximumValue;
    
    if (m_fMaximumValue <= m_fMinimumValue)
    {
        m_fMinimumValue   = m_fMaximumValue - 1.0f;
    }
    
    setValue(m_fMinimumValue);
}

float ControlPotentiometer::getMaximumValue()
{
    return m_fMaximumValue;
}

bool ControlPotentiometer::isTouchInside(Touch * touch)
{
    Point touchLocation   = this->getTouchLocation(touch);
    
    float distance          = this->distanceBetweenPointAndPoint(m_pProgressTimer->getPosition(), touchLocation);

    return distance < KD_MIN(getContentSize().width / 2, getContentSize().height / 2);
}

bool ControlPotentiometer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isTouchInside(pTouch) || !this->isEnabled() || !isVisible())
    {
        return false;
    }
    
    m_tPreviousLocation    = this->getTouchLocation(pTouch);
    
    this->potentiometerBegan(m_tPreviousLocation);
    
    return true;
}

void ControlPotentiometer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point location    = this->getTouchLocation(pTouch);

    this->potentiometerMoved(location);
}

void ControlPotentiometer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    this->potentiometerEnded(Point::ZERO);
}

float ControlPotentiometer::distanceBetweenPointAndPoint(Point point1, Point point2)
{
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    return kdSqrtf(dx*dx + dy*dy);
}

float ControlPotentiometer::angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(
    Point beginLineA, 
    Point endLineA,
    Point beginLineB,
    Point endLineB)
{
    float a = endLineA.x - beginLineA.x;
    float b = endLineA.y - beginLineA.y;
    float c = endLineB.x - beginLineB.x;
    float d = endLineB.y - beginLineB.y;
    
    float atanA = kdAtan2f(a, b);
    float atanB = kdAtan2f(c, d);
    
    // convert radiants to degrees
    return (atanA - atanB) * 180 / KD_PI_F;
}

void ControlPotentiometer::potentiometerBegan(Point location)
{
    setSelected(true);
    getThumbSprite()->setColor(Color3B::GRAY);
}

void ControlPotentiometer::potentiometerMoved(Point location)
{
    float angle       = this->angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(
        m_pProgressTimer->getPosition(),
        location,
        m_pProgressTimer->getPosition(),
        m_tPreviousLocation);
    
    // fix value, if the 12 o'clock position is between location and previousLocation
    if (angle > 180)
    {
        angle -= 360;
    }
    else if (angle < -180)
    {
        angle += 360;
    }

    setValue(m_fValue + angle / 360.0f * (m_fMaximumValue - m_fMinimumValue));
    
    m_tPreviousLocation    = location;
}

void ControlPotentiometer::potentiometerEnded(Point location)
{
    getThumbSprite()->setColor(Color3B::WHITE);
    setSelected(false);
}

NS_CC_EXT_END
