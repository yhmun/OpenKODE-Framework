/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlHuePicker.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://creativewax.co.uk
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

#include "extensions/GUI/CCControlExtension/CCControlHuePicker.h"

NS_CC_EXT_BEGIN

ControlHuePicker::ControlHuePicker()
: m_fHue(0.0f)
, m_fHuePercentage(0.0f)
, m_pBackground(NULL)
, m_pSlider(NULL)
{

}

ControlHuePicker::~ControlHuePicker()
{
    removeAllChildrenWithCleanup(true);
    CC_SAFE_RELEASE(m_pBackground);
    CC_SAFE_RELEASE(m_pSlider);
}

ControlHuePicker* ControlHuePicker::create(Node* target, Point pos)
{
    ControlHuePicker *pRet = new ControlHuePicker();
    pRet->initWithTargetAndPos(target, pos);
    pRet->autorelease();
    return pRet;
}


bool ControlHuePicker::initWithTargetAndPos(Node* target, Point pos)
{
    if (Control::init())
    {
        // Add background and slider sprites
        this->setBackground(ControlUtils::addSpriteToTargetWithPosAndAnchor("huePickerBackground.png", target, pos, Point(0.0f, 0.0f)));
        this->setSlider(ControlUtils::addSpriteToTargetWithPosAndAnchor("colourPicker.png", target, pos, Point(0.5f, 0.5f)));
        
        m_pSlider->setPosition(Point(pos.x, pos.y + m_pBackground->getBoundingBox().size.height * 0.5f));
        m_tStartPos=pos;

        // Sets the default value
        m_fHue=0.0f;
        m_fHuePercentage=0.0f;
        return true;
    }
    else
    {
        return false;
    }
}

void ControlHuePicker::setHue(float hueValue)
{
    m_fHue=hueValue;
    // Set the position of the slider to the correct hue
    // We need to divide it by 360 as its taken as an angle in degrees
    float huePercentage	= hueValue / 360.0f;
    setHuePercentage(huePercentage);
}

void ControlHuePicker::setHuePercentage(float hueValueInPercent)
{
    m_fHuePercentage=hueValueInPercent;
    m_fHue=m_fHuePercentage*360.0f;

    // Clamp the position of the icon within the circle
    Rect backgroundBox=m_pBackground->getBoundingBox();

    // Get the center point of the background image
    float centerX           = m_tStartPos.x + backgroundBox.size.width * 0.5f;
    float centerY           = m_tStartPos.y + backgroundBox.size.height * 0.5f;
    
    // Work out the limit to the distance of the picker when moving around the hue bar
    float limit             = backgroundBox.size.width * 0.5f - 15.0f;
    
    // Update angle
    float angleDeg          = m_fHuePercentage * 360.0f - 180.0f;
    float angle             = CC_DEGREES_TO_RADIANS(angleDeg);
    
    // Set new position of the slider
    float x                 = centerX + limit * kdCosf(angle);
    float y                 = centerY + limit * kdSinf(angle);
    m_pSlider->setPosition(Point(x, y));

}

void ControlHuePicker::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    if (m_pSlider != NULL)
    {
        m_pSlider->setOpacity(enabled ? 255 : 128);
    }
}

void ControlHuePicker::updateSliderPosition(Point location)
{

    // Clamp the position of the icon within the circle
    Rect backgroundBox=m_pBackground->getBoundingBox();
    
    // Get the center point of the background image
    float centerX           = m_tStartPos.x + backgroundBox.size.width * 0.5f;
    float centerY           = m_tStartPos.y + backgroundBox.size.height * 0.5f;

    // Work out the distance difference between the location and center
    float dx                = location.x - centerX;
    float dy                = location.y - centerY;
    
    // Update angle by using the direction of the location
    float angle             = kdAtan2f(dy, dx);
    float angleDeg          = CC_RADIANS_TO_DEGREES(angle) + 180.0f;
    
    // use the position / slider width to determin the percentage the dragger is at
    setHue(angleDeg);
    
    // send Control callback
    sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
}

bool ControlHuePicker::checkSliderPosition(Point location)
{
    // compute the distance between the current location and the center
    double distance = kdSqrtf(kdPowf (location.x + 10, 2) + kdPowf(location.y, 2));

    // check that the touch location is within the circle
    if (80 > distance && distance > 59)
    {
        updateSliderPosition(location);
        return true;
    }
    return false;
}

bool ControlHuePicker::onTouchBegan(Touch* touch, Event* event)
{
    if (!isEnabled() || !isVisible())
    {
        return false;
    }
    
    // Get the touch location
    Point touchLocation=getTouchLocation(touch);

    // Check the touch position on the slider
    return checkSliderPosition(touchLocation);
}


void ControlHuePicker::onTouchMoved(Touch* touch, Event* event)
{
    // Get the touch location
    Point touchLocation=getTouchLocation(touch);

    //small modification: this allows changing of the colour, even if the touch leaves the bounding area
//     updateSliderPosition(touchLocation);
//     sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
    // Check the touch position on the slider
    checkSliderPosition(touchLocation);
}

NS_CC_EXT_END
