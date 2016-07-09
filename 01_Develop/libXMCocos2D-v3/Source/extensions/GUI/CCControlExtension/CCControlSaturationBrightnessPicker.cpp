/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlSaturationBrightnessPicker.cpp
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

#include "extensions/GUI/CCControlExtension/CCControlSaturationBrightnessPicker.h"

NS_CC_EXT_BEGIN

ControlSaturationBrightnessPicker::ControlSaturationBrightnessPicker()
: m_fSaturation(0.0f)
, m_fBrightness(0.0f)
, m_pBackground(NULL)
, m_pOverlay(NULL)
, m_pShadow(NULL)
, m_pSlider(NULL)
, m_nBoxPos(0)
, m_nBoxSize(0)
{

}

ControlSaturationBrightnessPicker::~ControlSaturationBrightnessPicker()
{
    removeAllChildrenWithCleanup(true);

    m_pBackground = NULL;
    m_pOverlay    = NULL;
    m_pShadow     = NULL;
    m_pSlider     = NULL;
}
    
bool ControlSaturationBrightnessPicker::initWithTargetAndPos(Node* target, Point pos)
{
    if (Control::init())
    {
        // Add background and slider sprites
        m_pBackground=ControlUtils::addSpriteToTargetWithPosAndAnchor("colourPickerBackground.png", target, pos, Point(0.0f, 0.0f));
        m_pOverlay=ControlUtils::addSpriteToTargetWithPosAndAnchor("colourPickerOverlay.png", target, pos, Point(0.0f, 0.0f));
        m_pShadow=ControlUtils::addSpriteToTargetWithPosAndAnchor("colourPickerShadow.png", target, pos, Point(0.0f, 0.0f));
        m_pSlider=ControlUtils::addSpriteToTargetWithPosAndAnchor("colourPicker.png", target, pos, Point(0.5f, 0.5f));
                
        m_tStartPos=pos; // starting position of the colour picker        
        m_nBoxPos          = 35;    // starting position of the virtual box area for picking a colour
        m_nBoxSize         = m_pBackground->getContentSize().width / 2;;    // the size (width and height) of the virtual box for picking a colour from
        return true;
    }
    else
    {
        return false;
    }
}

ControlSaturationBrightnessPicker* ControlSaturationBrightnessPicker::create(Node* target, Point pos)
{
    ControlSaturationBrightnessPicker *pRet = new ControlSaturationBrightnessPicker();
    pRet->initWithTargetAndPos(target, pos);
    pRet->autorelease();
    return pRet;
}

void ControlSaturationBrightnessPicker::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    if (m_pSlider != NULL)
    {
        m_pSlider->setOpacity(enabled ? 255 : 128);
    }
}

void ControlSaturationBrightnessPicker::updateWithHSV(HSV hsv)
{
    HSV hsvTemp;
    hsvTemp.s = 1;
    hsvTemp.h = hsv.h;
    hsvTemp.v = 1;
    
    RGBA rgb = ControlUtils::RGBfromHSV(hsvTemp);
    m_pBackground->setColor(Color3B((GLubyte)(rgb.r * 255.0f), (GLubyte)(rgb.g * 255.0f), (GLubyte)(rgb.b * 255.0f)));
}

void ControlSaturationBrightnessPicker::updateDraggerWithHSV(HSV hsv)
{
    // Set the position of the slider to the correct saturation and brightness
    Point pos = Point(m_tStartPos.x + m_nBoxPos + (m_nBoxSize*(1 - hsv.s)),
                              m_tStartPos.y + m_nBoxPos + (m_nBoxSize*hsv.v));
    
    // update
    updateSliderPosition(pos);
}

void ControlSaturationBrightnessPicker::updateSliderPosition(Point sliderPosition)
{
    // Clamp the position of the icon within the circle
    
    // Get the center point of the bkgd image
    float centerX           = m_tStartPos.x + m_pBackground->getBoundingBox().size.width*0.5f;
    float centerY           = m_tStartPos.y + m_pBackground->getBoundingBox().size.height*0.5f;
    
    // Work out the distance difference between the location and center
    float dx                = sliderPosition.x - centerX;
    float dy                = sliderPosition.y - centerY;
    float dist              = kdSqrtf(dx * dx + dy * dy);
    
    // Update angle by using the direction of the location
    float angle             = kdAtan2f(dy, dx);
    
    // Set the limit to the slider movement within the colour picker
    float limit             = m_pBackground->getBoundingBox().size.width*0.5f;
    
    // Check distance doesn't exceed the bounds of the circle
    if (dist > limit)
    {
        sliderPosition.x    = centerX + limit * kdCosf(angle);
        sliderPosition.y    = centerY + limit * kdSinf(angle);
    }
    
    // Set the position of the dragger
    m_pSlider->setPosition(sliderPosition);
    
    
    // Clamp the position within the virtual box for colour selection
    if (sliderPosition.x < m_tStartPos.x + m_nBoxPos)                        sliderPosition.x = m_tStartPos.x + m_nBoxPos;
    else if (sliderPosition.x > m_tStartPos.x + m_nBoxPos + m_nBoxSize - 1)    sliderPosition.x = m_tStartPos.x + m_nBoxPos + m_nBoxSize - 1;
    if (sliderPosition.y < m_tStartPos.y + m_nBoxPos)                        sliderPosition.y = m_tStartPos.y + m_nBoxPos;
    else if (sliderPosition.y > m_tStartPos.y + m_nBoxPos + m_nBoxSize)        sliderPosition.y = m_tStartPos.y + m_nBoxPos + m_nBoxSize;
    
    // Use the position / slider width to determin the percentage the dragger is at
    m_fSaturation = 1.0f - kdFabsf((m_tStartPos.x + (float)m_nBoxPos - sliderPosition.x)/(float)m_nBoxSize);
    m_fBrightness = kdFabsf((m_tStartPos.y + (float)m_nBoxPos - sliderPosition.y)/(float)m_nBoxSize);
}

bool ControlSaturationBrightnessPicker::checkSliderPosition(Point location)
{
    // Clamp the position of the icon within the circle
    
    // get the center point of the bkgd image
    float centerX           = m_tStartPos.x + m_pBackground->getBoundingBox().size.width*0.5f;
    float centerY           = m_tStartPos.y + m_pBackground->getBoundingBox().size.height*0.5f;
    
    // work out the distance difference between the location and center
    float dx                = location.x - centerX;
    float dy                = location.y - centerY;
    float dist              = kdSqrtf(dx*dx+dy*dy);
    
    // check that the touch location is within the bounding rectangle before sending updates
    if (dist <= m_pBackground->getBoundingBox().size.width*0.5f)
    {
        updateSliderPosition(location);
        sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
        return true;
    }
    return false;
}


bool ControlSaturationBrightnessPicker::onTouchBegan(Touch* touch, Event* event)
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


void ControlSaturationBrightnessPicker::onTouchMoved(Touch* touch, Event* event)
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
