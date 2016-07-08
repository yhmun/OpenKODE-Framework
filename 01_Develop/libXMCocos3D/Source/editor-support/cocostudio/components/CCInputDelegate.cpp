/* -----------------------------------------------------------------------------------
 *
 *      File            CCInputDelegate.cpp
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

#include "editor-support/cocostudio/components/CCInputDelegate.h"
#include "2d/CCDirector.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"
#include "2d/event_dispatcher/CCEventListenerAcceleration.h"
#include "2d/event_dispatcher/CCEventListenerKeyboard.h"
#include "platform/CCDevice.h"
 
using namespace cocos2d;

namespace cocostudio {

InputDelegate::InputDelegate(void)
: m_bTouchEnabled(false)
, m_pTouchListener(nullptr)
, m_bAccelerometerEnabled(false)
, m_pAccelerometerListener(nullptr)
, m_bKeypadEnabled(false)
, m_pKeyboardListener(nullptr)
, m_nTouchPriority(-1)
, m_eTouchMode(Touch::DispatchMode::ALL_AT_ONCE)
{

}

InputDelegate::~InputDelegate(void)
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(m_pTouchListener);
    dispatcher->removeEventListener(m_pKeyboardListener);
    dispatcher->removeEventListener(m_pAccelerometerListener);
    Device::setAccelerometerEnabled(false);
}

bool InputDelegate::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
    return true;
}

void InputDelegate::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}
    
void InputDelegate::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}

void InputDelegate::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouch);
    CC_UNUSED_PARAM(pEvent);
}    

void InputDelegate::onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void InputDelegate::onTouchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void InputDelegate::onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

void InputDelegate::onTouchesCancelled(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    CC_UNUSED_PARAM(pTouches);
    CC_UNUSED_PARAM(pEvent);
}

bool InputDelegate::isTouchEnabled() const
{
    return m_bTouchEnabled;
}

void InputDelegate::setTouchEnabled(bool enabled)
{
    if (m_bTouchEnabled != enabled)
    {
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        m_bTouchEnabled = enabled;
        if (enabled)
        {            
            if( m_eTouchMode == Touch::DispatchMode::ALL_AT_ONCE ) {
                // Register Touch Event
                auto listener = EventListenerTouchAllAtOnce::create();
                
                listener->onTouchesBegan = CC_CALLBACK_2(InputDelegate::onTouchesBegan, this);
                listener->onTouchesMoved = CC_CALLBACK_2(InputDelegate::onTouchesMoved, this);
                listener->onTouchesEnded = CC_CALLBACK_2(InputDelegate::onTouchesEnded, this);
                listener->onTouchesCancelled = CC_CALLBACK_2(InputDelegate::onTouchesCancelled, this);
                
                dispatcher->addEventListenerWithFixedPriority(listener, m_nTouchPriority);
                m_pTouchListener = listener;
            } else {
                // Register Touch Event
                auto listener = EventListenerTouchOneByOne::create();
                listener->setSwallowTouches(true);
                
                listener->onTouchBegan = CC_CALLBACK_2(InputDelegate::onTouchBegan, this);
                listener->onTouchMoved = CC_CALLBACK_2(InputDelegate::onTouchMoved, this);
                listener->onTouchEnded = CC_CALLBACK_2(InputDelegate::onTouchEnded, this);
                listener->onTouchCancelled = CC_CALLBACK_2(InputDelegate::onTouchCancelled, this);
                
                dispatcher->addEventListenerWithFixedPriority(listener, m_nTouchPriority);
                m_pTouchListener = listener;
            }
        }
        else
        {
            dispatcher->removeEventListener(m_pTouchListener);
        }
    }
}

void InputDelegate::setTouchMode(Touch::DispatchMode mode)
{
    if(m_eTouchMode != mode)
    {
        m_eTouchMode = mode;
        
		if( m_bTouchEnabled)
        {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

void InputDelegate::setTouchPriority(int priority)
{
    if (m_nTouchPriority != priority)
    {
        m_nTouchPriority = priority;
        
		if( m_bTouchEnabled)
        {
			setTouchEnabled(false);
			setTouchEnabled(true);
		}
    }
}

int InputDelegate::getTouchPriority() const
{
    return m_nTouchPriority;
}

Touch::DispatchMode InputDelegate::getTouchMode() const
{
    return m_eTouchMode;
}

bool InputDelegate::isAccelerometerEnabled() const
{
    return m_bAccelerometerEnabled;
}

void InputDelegate::setAccelerometerEnabled(bool enabled)
{
    if (enabled != m_bAccelerometerEnabled)
    {
        m_bAccelerometerEnabled = enabled;

        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListener(m_pAccelerometerListener);
        m_pAccelerometerListener = nullptr;
        
        Device::setAccelerometerEnabled(enabled);
        
        if (enabled)
        {
            auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(InputDelegate::onAcceleration, this));
            dispatcher->addEventListenerWithFixedPriority(listener, -1);
            m_pAccelerometerListener = listener;
        }
    }
}

bool InputDelegate::isKeypadEnabled() const
{
    return m_bKeypadEnabled;
}

void InputDelegate::setKeypadEnabled(bool enabled)
{
    if (enabled != m_bKeypadEnabled)
    {
        m_bKeypadEnabled = enabled;

        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListener(m_pKeyboardListener);
        
        if (enabled)
        {
            auto listener = EventListenerKeyboard::create();
            listener->onKeyPressed = CC_CALLBACK_2(InputDelegate::onKeyPressed, this);
            listener->onKeyReleased = CC_CALLBACK_2(InputDelegate::onKeyReleased, this);
            
            dispatcher->addEventListenerWithFixedPriority(listener, -1);
            m_pKeyboardListener = listener;
        }
    }
}


}
