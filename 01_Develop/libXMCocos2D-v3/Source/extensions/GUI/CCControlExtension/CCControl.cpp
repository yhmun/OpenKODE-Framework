/* -----------------------------------------------------------------------------------
 *
 *      File            CCControl.cpp
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

#include "extensions/GUI/CCControlExtension/CCControl.h"
#include "2d/CCDirector.h"
#include "2d/menu_nodes/CCMenu.h"
#include "2d/event_dispatcher/CCTouch.h"
#include "extensions/GUI/CCControlExtension/CCInvocation.h"
#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"

NS_CC_EXT_BEGIN

Control::Control()
: m_bEnabled(false)
, m_bSelected(false)
, m_bHighlighted(false)
, m_bHasVisibleParents(false)
, m_pDispatchTable(NULL)
, m_bIsOpacityModifyRGB(false)
, m_eState(State::NORMAL)
{

}

Control* Control::create()
{
    Control* pRet = new Control();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool Control::init()
{
    if (Layer::init())
    {
        // Initialise instance variables
        m_eState=Control::State::NORMAL;
        setEnabled(true);
        setSelected(false);
        setHighlighted(false);

        // Initialise the tables
        m_pDispatchTable = new Dictionary();
        m_pDispatchTable->init();
        
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(Control::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(Control::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(Control::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(Control::onTouchCancelled, this);
        
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        return true;
    }
    else
    {
        return false;
    }
}

Control::~Control()
{
    CC_SAFE_RELEASE(m_pDispatchTable);
}

void Control::sendActionsForControlEvents(EventType controlEvents)
{
    // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the curent event
        if (((int)controlEvents & (1 << i)))
        {
            // Call invocations
            // <Invocation*>
            Array* invocationList = this->dispatchListforControlEvent((Control::EventType)(1<<i));
            Object* pObj = NULL;
            CCARRAY_FOREACH(invocationList, pObj)
            {
                Invocation* invocation = static_cast<Invocation*>(pObj);
                invocation->invoke(this);
            }
            //Call ScriptFunc
            if (kScriptTypeLua == m_eScriptType)
            {
                cocos2d::BasicScriptData data(this,(void*)&controlEvents);
                cocos2d::ScriptEvent event(cocos2d::kControlEvent,(void*)&data);
                cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
            }
        }
    }
}
void Control::addTargetWithActionForControlEvents(Object* target, Handler action, EventType controlEvents)
{
    // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the curent event
        if (((int)controlEvents & (1 << i)))
        {
            this->addTargetWithActionForControlEvent(target, action, (EventType)(1<<i));
        }
    }
}



/**
 * Adds a target and action for a particular event to an internal dispatch 
 * table.
 * The action message may optionnaly include the sender and the event as 
 * parameters, in that order.
 * When you call this method, target is not retained.
 *
 * @param target The target object that is, the object to which the action 
 * message is sent. It cannot be nil. The target is not retained.
 * @param action A selector identifying an action message. It cannot be NULL.
 * @param controlEvent A control event for which the action message is sent.
 * See "CCControlEvent" for constants.
 */
void Control::addTargetWithActionForControlEvent(Object* target, Handler action, EventType controlEvent)
{    
    // Create the invocation object
    Invocation *invocation = Invocation::create(target, action, controlEvent);

    // Add the invocation into the dispatch list for the given control event
    Array* eventInvocationList = this->dispatchListforControlEvent(controlEvent);
    eventInvocationList->addObject(invocation);    
}

void Control::removeTargetWithActionForControlEvents(Object* target, Handler action, EventType controlEvents)
{
     // For each control events
    for (int i = 0; i < kControlEventTotalNumber; i++)
    {
        // If the given controlEvents bitmask contains the curent event
        if (((int)controlEvents & (1 << i)))
        {
            this->removeTargetWithActionForControlEvent(target, action, (EventType)(1 << i));
        }
    }
}

void Control::removeTargetWithActionForControlEvent(Object* target, Handler action, EventType controlEvent)
{
    // Retrieve all invocations for the given control event
    //<Invocation*>
    Array *eventInvocationList = this->dispatchListforControlEvent(controlEvent);
    
    //remove all invocations if the target and action are null
    //TODO: should the invocations be deleted, or just removed from the array? Won't that cause issues if you add a single invocation for multiple events?
    bool bDeleteObjects=true;
    if (!target && !action)
    {
        //remove objects
        eventInvocationList->removeAllObjects();
    } 
    else
    {
            //normally we would use a predicate, but this won't work here. Have to do it manually
            Object* pObj = NULL;
            CCARRAY_FOREACH(eventInvocationList, pObj)
            {
                Invocation *invocation = static_cast<Invocation*>(pObj);
                bool shouldBeRemoved=true;
                if (target)
                {
                    shouldBeRemoved=(target==invocation->getTarget());
                }
                if (action)
                {
                    shouldBeRemoved=(shouldBeRemoved && (action==invocation->getAction()));
                }
                // Remove the corresponding invocation object
                if (shouldBeRemoved)
                {
                    eventInvocationList->removeObject(invocation, bDeleteObjects);
                }
            }
    }
}


//CRGBA protocol
void Control::setOpacityModifyRGB(bool bOpacityModifyRGB)
{
    m_bIsOpacityModifyRGB=bOpacityModifyRGB;
    Object* child;
    Array* children=getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);        
        if (pNode)
        {
            pNode->setOpacityModifyRGB(bOpacityModifyRGB);
        }
    }
}

bool Control::isOpacityModifyRGB() const
{
    return m_bIsOpacityModifyRGB;
}


Point Control::getTouchLocation(Touch* touch)
{
    Point touchLocation = touch->getLocation();            // Get the touch position
    touchLocation = this->convertToNodeSpace(touchLocation);  // Convert to the node space of this class
    
    return touchLocation;
}

bool Control::isTouchInside(Touch* touch)
{
    Point touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    Rect bBox = getBoundingBox();
    return bBox.containsPoint(touchLocation);
}

Array* Control::dispatchListforControlEvent(EventType controlEvent)
{
    Array* invocationList = static_cast<Array*>(m_pDispatchTable->objectForKey((int)controlEvent));

    // If the invocation list does not exist for the  dispatch table, we create it
    if (invocationList == NULL)
    {
        invocationList = Array::createWithCapacity(1);
        m_pDispatchTable->setObject(invocationList, (int)controlEvent);
    }    
    return invocationList;
}

void Control::needsLayout()
{
}

void Control::setEnabled(bool bEnabled)
{
    m_bEnabled = bEnabled;
    if(m_bEnabled) {
        m_eState = Control::State::NORMAL;
    } else {
        m_eState = Control::State::DISABLED;
    }

    this->needsLayout();
}

bool Control::isEnabled() const
{
    return m_bEnabled;
}

void Control::setSelected(bool bSelected)
{
    m_bSelected = bSelected;
    this->needsLayout();
}

bool Control::isSelected() const
{
    return m_bSelected;
}

void Control::setHighlighted(bool bHighlighted)
{
    m_bHighlighted = bHighlighted;
    this->needsLayout();
}

bool Control::isHighlighted() const
{
    return m_bHighlighted;
}

bool Control::hasVisibleParents() const
{
    auto parent = this->getParent();
    for( auto c = parent; c != NULL; c = c->getParent() )
    {
        if( !c->isVisible() )
        {
            return false;
        }
    }
    return true;
}

Control::EventType operator|(Control::EventType a, Control::EventType b) {
    return static_cast<Control::EventType>(static_cast<int>(a) | static_cast<int>(b));
}

NS_CC_EXT_END
