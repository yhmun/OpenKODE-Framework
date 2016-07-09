/* -----------------------------------------------------------------------------------
 *
 *      File            CCControlColourPicker.cpp
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

#include "extensions/GUI/CCControlExtension/CCControlColourPicker.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"
#include "2d/sprite_nodes/CCSpriteBatchNode.h"

NS_CC_EXT_BEGIN

ControlColourPicker::ControlColourPicker()
: m_pColourPicker(NULL)
, m_pHuePicker(NULL)
, m_pBackground(NULL)
{

}

ControlColourPicker::~ControlColourPicker()
{
    CC_SAFE_RELEASE(m_pBackground);
    CC_SAFE_RELEASE(m_pHuePicker);
    CC_SAFE_RELEASE(m_pColourPicker);
}

bool ControlColourPicker::init()
{
    if (Control::init())
    {
        // Cache the sprites
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("extensions/CCControlColourPickerSpriteSheet.plist");
        
        // Create the sprite batch node
        SpriteBatchNode *spriteSheet  = SpriteBatchNode::create("extensions/CCControlColourPickerSpriteSheet.png");
        addChild(spriteSheet);
        
        // MIPMAP
//        ccTexParams params  = {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		/* Comment next line to avoid something like mosaic in 'ControlExtensionTest',
		   especially the display of 'huePickerBackground.png' when in 800*480 window size with 480*320 design resolution and hd(960*640) resources.
	    */
//        spriteSheet->getTexture()->setAliasTexParameters();
//         spriteSheet->getTexture()->setTexParameters(&params);
//         spriteSheet->getTexture()->generateMipmap();

        // Init default color
        m_tHsv.h = 0;
        m_tHsv.s = 0;
        m_tHsv.v = 0;
        
        // Add image
        m_pBackground=ControlUtils::addSpriteToTargetWithPosAndAnchor("menuColourPanelBackground.png", spriteSheet, Point::ZERO, Point(0.5f, 0.5f));
        CC_SAFE_RETAIN(m_pBackground);
        
        Point backgroundPointZero = m_pBackground->getPosition() - Point(m_pBackground->getContentSize().width / 2, m_pBackground->getContentSize().height / 2);
        
        // Setup panels
        float hueShift                = 8;
        float colourShift             = 28;
        
        m_pHuePicker = new ControlHuePicker();
        m_pHuePicker->initWithTargetAndPos(spriteSheet, Point(backgroundPointZero.x + hueShift, backgroundPointZero.y + hueShift));
        m_pColourPicker = new ControlSaturationBrightnessPicker();
        m_pColourPicker->initWithTargetAndPos(spriteSheet, Point(backgroundPointZero.x + colourShift, backgroundPointZero.y + colourShift));
        
        // Setup events
        m_pHuePicker->addTargetWithActionForControlEvents(this, cccontrol_selector(ControlColourPicker::hueSliderValueChanged), Control::EventType::VALUE_CHANGED);
        m_pColourPicker->addTargetWithActionForControlEvents(this, cccontrol_selector(ControlColourPicker::colourSliderValueChanged), Control::EventType::VALUE_CHANGED);
       
        // Set defaults
        updateHueAndControlPicker();
        addChild(m_pHuePicker);
        addChild(m_pColourPicker);

        // Set content size
        setContentSize(m_pBackground->getContentSize());
        return true;
    }
    else
        return false;
}

ControlColourPicker* ControlColourPicker::create()
{
    ControlColourPicker *pRet = new ControlColourPicker();
    pRet->init();
    pRet->autorelease();
    return pRet;
}


void ControlColourPicker::setColor(const Color3B& color)
{
    // XXX fixed me if not correct
    Control::setColor(color);
    
    RGBA rgba;
    rgba.r      = color.r / 255.0f;
    rgba.g      = color.g / 255.0f;
    rgba.b      = color.b / 255.0f;
    rgba.a      = 1.0f;
    
    m_tHsv=ControlUtils::HSVfromRGB(rgba);
    updateHueAndControlPicker();
}

void ControlColourPicker::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    if (m_pHuePicker != NULL)
    {
        m_pHuePicker->setEnabled(enabled);
    }
    if (m_pColourPicker)
    {
        m_pColourPicker->setEnabled(enabled);
    } 
}


//need two events to prevent an infinite loop! (can't update huePicker when the huePicker triggers the callback due to Control::EventType::VALUE_CHANGED)
void ControlColourPicker::updateControlPicker()
{
    m_pHuePicker->setHue(m_tHsv.h);
    m_pColourPicker->updateWithHSV(m_tHsv);
}

void ControlColourPicker::updateHueAndControlPicker()
{
    m_pHuePicker->setHue(m_tHsv.h);
    m_pColourPicker->updateWithHSV(m_tHsv);
    m_pColourPicker->updateDraggerWithHSV(m_tHsv);
}


void ControlColourPicker::hueSliderValueChanged(Object * sender, Control::EventType controlEvent)
{
    m_tHsv.h      = ((ControlHuePicker*)sender)->getHue();

    // Update the value
    RGBA rgb    = ControlUtils::RGBfromHSV(m_tHsv);
    // XXX fixed me if not correct
    Control::setColor(Color3B((GLubyte)(rgb.r * 255.0f), (GLubyte)(rgb.g * 255.0f), (GLubyte)(rgb.b * 255.0f)));
    
    // Send Control callback
    sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
    updateControlPicker();
}

void ControlColourPicker::colourSliderValueChanged(Object * sender, Control::EventType controlEvent)
{
    m_tHsv.s=((ControlSaturationBrightnessPicker*)sender)->getSaturation();
    m_tHsv.v=((ControlSaturationBrightnessPicker*)sender)->getBrightness();


     // Update the value
    RGBA rgb    = ControlUtils::RGBfromHSV(m_tHsv);
    // XXX fixed me if not correct
    Control::setColor(Color3B((GLubyte)(rgb.r * 255.0f), (GLubyte)(rgb.g * 255.0f), (GLubyte)(rgb.b * 255.0f)));
    
    // Send Control callback
    sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
}

//ignore all touches, handled by children
bool ControlColourPicker::onTouchBegan(Touch* touch, Event* pEvent)
{
    return false;
}

NS_CC_EXT_END
