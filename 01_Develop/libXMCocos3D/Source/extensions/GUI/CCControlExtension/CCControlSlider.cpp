/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlSlider.cpp
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

#include "extensions/GUI/CCControlExtension/CCControlSlider.h"
#include "2d/event_dispatcher/CCTouch.h"
#include "2d/CCDirector.h"

NS_CC_EXT_BEGIN

ControlSlider::ControlSlider()
: m_fValue(0.0f)
, m_fMinimumValue(0.0f)
, m_fMaximumValue(0.0f)
, m_fMinimumAllowedValue(0.0f)
, m_fMaximumAllowedValue(0.0f)
, m_pThumbSprite(NULL)
, m_pProgressSprite(NULL)
, m_pBackgroundSprite(NULL)
{

}

ControlSlider::~ControlSlider()
{
    CC_SAFE_RELEASE(m_pThumbSprite);
    CC_SAFE_RELEASE(m_pProgressSprite);
    CC_SAFE_RELEASE(m_pBackgroundSprite);
}

ControlSlider* ControlSlider::create(const char* bgFile, const char* progressFile, const char* thumbFile)
{
    // Prepare background for slider
    Sprite *backgroundSprite      = Sprite::create(bgFile);
    
    // Prepare progress for slider
    Sprite *progressSprite        = Sprite::create(progressFile);
    
    // Prepare thumb (menuItem) for slider
    Sprite *thumbSprite           = Sprite::create(thumbFile);
    
    return ControlSlider::create(backgroundSprite, progressSprite, thumbSprite);
}

ControlSlider* ControlSlider::create(Sprite * backgroundSprite, Sprite* pogressSprite, Sprite* thumbSprite)
{
    ControlSlider *pRet = new ControlSlider();
    pRet->initWithSprites(backgroundSprite, pogressSprite, thumbSprite);
    pRet->autorelease();
    return pRet;
}

 bool ControlSlider::initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite, Sprite* thumbSprite)
 {
     if (Control::init())
     {
        CCASSERT(backgroundSprite,  "Background sprite must be not nil");
        CCASSERT(progressSprite,    "Progress sprite must be not nil");
        CCASSERT(thumbSprite,       "Thumb sprite must be not nil");

        ignoreAnchorPointForPosition(false);

        this->setBackgroundSprite(backgroundSprite);
        this->setProgressSprite(progressSprite);
        this->setThumbSprite(thumbSprite);

        // Defines the content size
        Rect maxRect   = ControlUtils::RectUnion(backgroundSprite->getBoundingBox(), thumbSprite->getBoundingBox());

        setContentSize(Size(maxRect.size.width, maxRect.size.height));
        
        // Add the slider background
        m_pBackgroundSprite->setAnchorPoint(Point(0.5f, 0.5f));
        m_pBackgroundSprite->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
        addChild(m_pBackgroundSprite);

        // Add the progress bar
        m_pProgressSprite->setAnchorPoint(Point(0.0f, 0.5f));
        m_pProgressSprite->setPosition(Point(0.0f, this->getContentSize().height / 2));
        addChild(m_pProgressSprite);

        // Add the slider thumb  
        m_pThumbSprite->setPosition(Point(0.0f, this->getContentSize().height / 2));
        addChild(m_pThumbSprite);
        
        // Init default values
        m_fMinimumValue                   = 0.0f;
        m_fMaximumValue                   = 1.0f;

        setValue(m_fMinimumValue);
        return true;
     }
     else
     {
         return false;
     }
 }


void ControlSlider::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    if (m_pThumbSprite != NULL) 
    {
        m_pThumbSprite->setOpacity((enabled) ? 255 : 128);
    }
}

 void ControlSlider::setValue(float value)
 {
     // set new value with sentinel
     if (value < m_fMinimumValue)
     {
         value = m_fMinimumValue;
     }

     if (value > m_fMaximumValue) 
     {
         value = m_fMaximumValue;
     }

     m_fValue = value;

     this->needsLayout();

     this->sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
 }

 void ControlSlider::setMinimumValue(float minimumValue)
 {
     m_fMinimumValue=minimumValue;
     m_fMinimumAllowedValue = minimumValue;
     if (m_fMinimumValue >= m_fMaximumValue)    
     {
        m_fMaximumValue   = m_fMinimumValue + 1.0f;
     }
     setValue(m_fValue);
 }

 void ControlSlider::setMaximumValue(float maximumValue)
 {
     m_fMaximumValue=maximumValue;
     m_fMaximumAllowedValue = maximumValue;
     if (m_fMaximumValue <= m_fMinimumValue)   
     {
        m_fMinimumValue   = m_fMaximumValue - 1.0f;
     }
     setValue(m_fValue);
 }

bool ControlSlider::isTouchInside(Touch * touch)
{
  Point touchLocation   = touch->getLocation();
  touchLocation           = this->getParent()->convertToNodeSpace(touchLocation);

  Rect rect             = this->getBoundingBox();
  rect.size.width         += m_pThumbSprite->getContentSize().width;
  rect.origin.x           -= m_pThumbSprite->getContentSize().width / 2;

  return rect.containsPoint(touchLocation);
}

Point ControlSlider::locationFromTouch(Touch* touch)
{
  Point touchLocation   = touch->getLocation();                      // Get the touch position
  touchLocation           = this->convertToNodeSpace(touchLocation);                  // Convert to the node space of this class

  if (touchLocation.x < 0)
  {
      touchLocation.x     = 0;
  } else if (touchLocation.x > m_pBackgroundSprite->getContentSize().width)
  {
      touchLocation.x     = m_pBackgroundSprite->getContentSize().width;
  }

  return touchLocation;
}


bool ControlSlider::onTouchBegan(Touch* touch, Event* pEvent)
{
    if (!isTouchInside(touch) || !isEnabled() || !isVisible())
    {
        return false;
    }

    Point location = locationFromTouch(touch);
    sliderBegan(location);
    return true;
}

void ControlSlider::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point location = locationFromTouch(pTouch);
    sliderMoved(location);
}

void ControlSlider::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    sliderEnded(Point::ZERO);
}

void ControlSlider::needsLayout()
{
    if (NULL == m_pThumbSprite || NULL == m_pBackgroundSprite || NULL == m_pProgressSprite)
    {
        return;
    }
    // Update thumb position for new value
    float percent               = (m_fValue - m_fMinimumValue) / (m_fMaximumValue - m_fMinimumValue);

    Point pos                 = m_pThumbSprite->getPosition();
    pos.x                       = percent * m_pBackgroundSprite->getContentSize().width;
    m_pThumbSprite->setPosition(pos);

    // Stretches content proportional to newLevel
    Rect textureRect          = m_pProgressSprite->getTextureRect();
    textureRect                 = Rect(textureRect.origin.x, textureRect.origin.y, pos.x, textureRect.size.height);
    m_pProgressSprite->setTextureRect(textureRect, m_pProgressSprite->isTextureRectRotated(), textureRect.size);
}

void ControlSlider::sliderBegan(Point location)
{
    this->setSelected(true);
    this->getThumbSprite()->setColor(Color3B::GRAY);
    setValue(valueForLocation(location));
}

void ControlSlider::sliderMoved(Point location)
{
    setValue(valueForLocation(location));
}

void ControlSlider::sliderEnded(Point location)
{
    if (this->isSelected())
    {
        setValue(valueForLocation(m_pThumbSprite->getPosition()));
    }
    this->getThumbSprite()->setColor(Color3B::WHITE);
    this->setSelected(false);
}

float ControlSlider::valueForLocation(Point location)
{
    float percent = location.x/ m_pBackgroundSprite->getContentSize().width;
    return KD_MAX(KD_MIN(m_fMinimumValue + percent * (m_fMaximumValue - m_fMinimumValue), m_fMaximumAllowedValue), m_fMinimumAllowedValue);
}

NS_CC_EXT_END
