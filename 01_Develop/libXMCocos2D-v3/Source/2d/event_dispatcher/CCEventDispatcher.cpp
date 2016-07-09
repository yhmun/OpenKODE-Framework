/* -----------------------------------------------------------------------------------
 *
 *      File            CCEventDispatcher.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "2d/event_dispatcher/CCEventDispatcher.h"
#include "2d/event_dispatcher/CCEvent.h"
#include "2d/event_dispatcher/CCEventTouch.h"
#include "2d/event_dispatcher/CCEventCustom.h"
#include "2d/event_dispatcher/CCEventListenerTouch.h"
#include "2d/base_nodes/CCNode.h"
#include "2d/CCDirector.h"
#include <algorithm>


#define DUMP_LISTENER_ITEM_PRIORITY_INFO 0

namespace
{

class DispatchGuard
{
public:
    DispatchGuard(int& count):
            _count(count)
    {
        ++_count;
    }

    ~DispatchGuard()
    {
        --_count;
    }

private:
    int& _count;
};

}

NS_CC_BEGIN

static EventListener::ListenerID getListenerID(Event* event)
{
    EventListener::ListenerID ret;
    switch (event->getType())
    {
        case Event::Type::ACCELERATION:
            ret = static_cast<EventListener::ListenerID>(EventListener::Type::ACCELERATION);
            break;
        case Event::Type::CUSTOM:
            {
                auto customEvent = static_cast<EventCustom*>(event);
                auto listenerID = std::hash<std::string>()(customEvent->getEventName());
                ret = static_cast<EventListener::ListenerID>(listenerID);
            }
            break;
        case Event::Type::KEYBOARD:
            ret = static_cast<EventListener::ListenerID>(EventListener::Type::KEYBOARD);
            break;
        case Event::Type::MOUSE:
            ret = static_cast<EventListener::ListenerID>(EventListener::Type::MOUSE);
            break;
        case Event::Type::TOUCH:
            // Touch listener is very special, it contains two kinds of listeners, EventListenerTouchOneByOne and EventListenerTouchAllAtOnce.
            // return UNKNOW instead.
            ret = static_cast<EventListener::ListenerID>(EventListener::Type::UNKNOWN);
            break;
        default:
            CCASSERT(false, "Invalid type!");
            break;
    }
    
    return ret;
}

EventDispatcher::EventListenerVector::EventListenerVector()
: m_pSceneGraphListeners(nullptr)
, m_pFixedListeners(nullptr)
, m_nGt0Index(0)
{
}

EventDispatcher::EventListenerVector::~EventListenerVector()
{
    CC_SAFE_DELETE(m_pSceneGraphListeners);
    CC_SAFE_DELETE(m_pFixedListeners);
}

KDsize EventDispatcher::EventListenerVector::size ( KDvoid ) const
{
    KDsize	ret = 0;
    if (m_pSceneGraphListeners)
        ret += m_pSceneGraphListeners->size();
    if (m_pFixedListeners)
        ret += m_pFixedListeners->size();
    
    return ret;
}

bool EventDispatcher::EventListenerVector::empty() const
{
    return (m_pSceneGraphListeners == nullptr || m_pSceneGraphListeners->empty())
        && (m_pFixedListeners == nullptr || m_pFixedListeners->empty());
}

void EventDispatcher::EventListenerVector::push_back(EventListener* listener)
{
    if (listener->getFixedPriority() == 0)
    {
        if (m_pSceneGraphListeners == nullptr)
        {
            m_pSceneGraphListeners = new std::vector<EventListener*>();
            m_pSceneGraphListeners->reserve(100);
        }
        
        m_pSceneGraphListeners->push_back(listener);
    }
    else
    {
        if (m_pFixedListeners == nullptr)
        {
            m_pFixedListeners = new std::vector<EventListener*>();
            m_pFixedListeners->reserve(100);
        }
        
        m_pFixedListeners->push_back(listener);
    }
}

void EventDispatcher::EventListenerVector::clearSceneGraphListeners()
{
    if (m_pSceneGraphListeners)
    {
        m_pSceneGraphListeners->clear();
        delete m_pSceneGraphListeners;
        m_pSceneGraphListeners = nullptr;
    }
}

void EventDispatcher::EventListenerVector::clearFixedListeners()
{
    if (m_pFixedListeners)
    {
        m_pFixedListeners->clear();
        delete m_pFixedListeners;
        m_pFixedListeners = nullptr;
    }
}

void EventDispatcher::EventListenerVector::clear()
{
    clearSceneGraphListeners();
    clearFixedListeners();
}


EventDispatcher::EventDispatcher()
: m_nInDispatch(0)
, m_bIsEnabled(true)
, m_nNodePriorityIndex(0)
{
    m_aToAddedListeners.reserve(50);
}

EventDispatcher::~EventDispatcher()
{
    removeAllEventListeners();
}

void EventDispatcher::visitTarget(Node* node)
{    
    int i = 0;
    Array* children = node->getChildren();
    int childrenCount = children ? children->count() : 0;
    
    if(childrenCount > 0)
    {
        
        Node* child = nullptr;
        // visit children zOrder < 0
        for( ; i < childrenCount; i++ )
        {
            child = static_cast<Node*>( children->getObjectAtIndex(i) );
            
            if ( child && child->getZOrder() < 0 )
                visitTarget(child);
            else
                break;
        }
        
        m_aNodePriorityMap.insert(std::make_pair(node, ++m_nNodePriorityIndex));
        
        for( ; i < childrenCount; i++ )
        {
            child = static_cast<Node*>( children->getObjectAtIndex(i) );
            if (child)
                visitTarget(child);
        }
    }
    else
    {
        m_aNodePriorityMap.insert(std::make_pair(node, ++m_nNodePriorityIndex));
    }
}

void EventDispatcher::pauseTarget(Node* node)
{
    auto listenerIter = m_aNodeListenersMap.find(node);
    if (listenerIter != m_aNodeListenersMap.end())
    {
        auto listeners = listenerIter->second;
        for (auto& l : *listeners)
        {
            l->setPaused(true);
        }
    }
}

void EventDispatcher::resumeTarget(Node* node)
{
    auto listenerIter = m_aNodeListenersMap.find(node);
    if (listenerIter != m_aNodeListenersMap.end())
    {
        auto listeners = listenerIter->second;
        for (auto& l : *listeners)
        {
            l->setPaused(false);
        }
    }
    setDirtyForNode(node);
}

void EventDispatcher::cleanTarget(Node* node)
{
    auto listenerIter = m_aNodeListenersMap.find(node);
    if (listenerIter != m_aNodeListenersMap.end())
    {
        auto listeners = listenerIter->second;
        auto listenersCopy = *listeners;
        for (auto& l : listenersCopy)
        {
            removeEventListener(l);
        }
    }
}

void EventDispatcher::associateNodeAndEventListener(Node* node, EventListener* listener)
{
    std::vector<EventListener*>* listeners = nullptr;
    auto found = m_aNodeListenersMap.find(node);
    if (found != m_aNodeListenersMap.end())
    {
        listeners = found->second;
    }
    else
    {
        listeners = new std::vector<EventListener*>();
    }
    
    listeners->push_back(listener);
    
    m_aNodeListenersMap.insert(std::make_pair(node, listeners));
}

void EventDispatcher::dissociateNodeAndEventListener(Node* node, EventListener* listener)
{
    std::vector<EventListener*>* listeners = nullptr;
    auto found = m_aNodeListenersMap.find(node);
    if (found != m_aNodeListenersMap.end())
    {
        listeners = found->second;
        auto iter = std::find(listeners->begin(), listeners->end(), listener);
        if (iter != listeners->end())
        {
            listeners->erase(iter);
        }
        
        if (listeners->empty())
        {
            m_aNodeListenersMap.erase(found);
            delete listeners;
        }
    }
}

void EventDispatcher::addEventListener(EventListener* listener)
{
    if (m_nInDispatch == 0)
    {
        EventListenerVector* listenerList = nullptr;
        
        auto iter = m_aListeners.find(listener->getListenerID());
        if (iter == m_aListeners.end())
        {
            listenerList = new EventListenerVector();
            m_aListeners.insert(std::make_pair(listener->getListenerID(), listenerList));
        }
        else
        {
            listenerList = iter->second;
        }

        listenerList->push_back(listener);
        
        if (listener->getFixedPriority() == 0)
        {
            setDirty(listener->getListenerID(), DirtyFlag::SCENE_GRAPH_PRIORITY);
        }
        else
        {
            setDirty(listener->getListenerID(), DirtyFlag::FIXED_PRITORY);
        }
    }
    else
    {
        m_aToAddedListeners.push_back(listener);
    }
}

void EventDispatcher::addEventListenerWithSceneGraphPriority(EventListener* listener, Node* node)
{
    CCASSERT(listener && node, "Invalid parameters.");
    CCASSERT(!listener->isRegistered(), "The listener has been registered.");
    
    if (!listener->checkAvailable())
        return;
    
    listener->setSceneGraphPriority(node);
    listener->setFixedPriority(0);
    listener->setRegistered(true);
    
    listener->retain();
    addEventListener(listener);

    associateNodeAndEventListener(node, listener);
    
    if (node->isRunning())
    {
        resumeTarget(node);
    }
}

void EventDispatcher::addEventListenerWithFixedPriority(EventListener* listener, int fixedPriority)
{
    CCASSERT(listener, "Invalid parameters.");
    CCASSERT(!listener->isRegistered(), "The listener has been registered.");
    CCASSERT(fixedPriority != 0, "0 priority is forbidden for fixed priority since it's used for scene graph based priority.");
    
    if (!listener->checkAvailable())
        return;
    
    listener->setSceneGraphPriority(nullptr);
    listener->setFixedPriority(fixedPriority);
    listener->setRegistered(true);
    listener->setPaused(false);
    
    listener->retain();

    addEventListener(listener);
}

void EventDispatcher::removeEventListener(EventListener* listener)
{
    if (listener == nullptr)
        return;

    bool isFound = false;

    auto removeListenerInVector = [&](std::vector<EventListener*>* listeners){
        if (listeners == nullptr)
            return;
        
        for (auto iter = listeners->begin(); iter != listeners->end(); ++iter)
        {
            auto l = *iter;
            if (l == listener)
            {
                CC_SAFE_RETAIN(l);
                l->setRegistered(false);
                if (l->getSceneGraphPriority() != nullptr)
                {
                    dissociateNodeAndEventListener(l->getSceneGraphPriority(), l);
                }
                
                if (m_nInDispatch == 0)
                {
                    listeners->erase(iter);
                    CC_SAFE_RELEASE(l);
                }
                
                isFound = true;
                break;
            }
        }
    };
    
    for (auto iter = m_aListeners.begin(); iter != m_aListeners.end();)
    {
        auto listeners = iter->second;
        auto fixedPriorityListeners = listeners->getFixedPriorityListeners();
        auto sceneGraphPriorityListeners = listeners->getSceneGraphPriorityListeners();

        removeListenerInVector(sceneGraphPriorityListeners);
        if (!isFound)
        {
            removeListenerInVector(fixedPriorityListeners);
        }

        if (iter->second->empty())
        {
            m_aPriorityDirtyFlagMap.erase(listener->getListenerID());
            auto list = iter->second;
            iter = m_aListeners.erase(iter);
            CC_SAFE_DELETE(list);
        }
        else
        {
            ++iter;
        }
        
        if (isFound)
            break;
    }

    if (isFound)
    {
        CC_SAFE_RELEASE(listener);
    }
    else
    {
        for(auto iter = m_aToAddedListeners.begin(); iter != m_aToAddedListeners.end(); ++iter)
        {
            if (*iter == listener)
            {
                m_aToAddedListeners.erase(iter);
                break;
            }
        }
    }
}

void EventDispatcher::setPriority(EventListener* listener, int fixedPriority)
{
    if (listener == nullptr)
        return;
    
    for (auto iter = m_aListeners.begin(); iter != m_aListeners.end(); ++iter)
    {
        auto fixedPriorityListeners = iter->second->getFixedPriorityListeners();
        if (fixedPriorityListeners)
        {
            auto found = std::find(fixedPriorityListeners->begin(), fixedPriorityListeners->end(), listener);
            if (found != fixedPriorityListeners->end())
            {
                CCASSERT(listener->getSceneGraphPriority() == nullptr, "Can't set fixed priority with scene graph based listener.");
                
                if (listener->getFixedPriority() != fixedPriority)
                {
                    listener->setFixedPriority(fixedPriority);
                    setDirty(listener->getListenerID(), DirtyFlag::FIXED_PRITORY);
                }
                return;
            }
        }
    }
}

void EventDispatcher::dispatchEventToListeners(EventListenerVector* listeners, std::function<bool(EventListener*)> onEvent)
{
    bool shouldStopPropagation = false;
    auto fixedPriorityListeners = listeners->getFixedPriorityListeners();
    auto sceneGraphPriorityListeners = listeners->getSceneGraphPriorityListeners();
    
    long i = 0;
    // priority < 0
    if (fixedPriorityListeners)
    {
        for (; !fixedPriorityListeners->empty() && i < listeners->getGt0Index(); ++i)
        {
            auto l = fixedPriorityListeners->at(i);
            if (!l->isPaused() && l->isRegistered() && onEvent(l))
            {
                shouldStopPropagation = true;
                break;
            }
        }
    }
    
    if (sceneGraphPriorityListeners)
    {
        if (!shouldStopPropagation)
        {
            // priority == 0, scene graph priority
            for (auto& l : *sceneGraphPriorityListeners)
            {
                if (!l->isPaused() && l->isRegistered() && onEvent(l))
                {
                    shouldStopPropagation = true;
                    break;
                }
            }
        }
    }

    if (fixedPriorityListeners)
    {
        if (!shouldStopPropagation)
        {
            // priority > 0
            for (; i < static_cast<long>(fixedPriorityListeners->size()); ++i)
            {
                auto l = fixedPriorityListeners->at(i);
                
                if (!l->isPaused() && l->isRegistered() && onEvent(fixedPriorityListeners->at(i)))
                {
                    shouldStopPropagation = true;
                    break;
                }
            }
        }
    }
}

void EventDispatcher::dispatchEvent(Event* event)
{
    if (!m_bIsEnabled)
        return;
    
    updateDirtyFlagForSceneGraph();
    
    
    DispatchGuard guard(m_nInDispatch);
    
    if (event->getType() == Event::Type::TOUCH)
    {
        dispatchTouchEvent(static_cast<EventTouch*>(event));
        return;
    }
    
    auto listenerID = getListenerID(event);
    
    sortEventListeners(listenerID);
    
    auto iter = m_aListeners.find(listenerID);
    if (iter != m_aListeners.end())
    {
        auto listeners = iter->second;
        
        auto onEvent = [&event](EventListener* listener) -> bool{
            event->setCurrentTarget(listener->getSceneGraphPriority());
            listener->onEvent(event);
            return event->isStopped();
        };
        
        dispatchEventToListeners(listeners, onEvent);
    }
    
    updateListeners(event);
}

void EventDispatcher::dispatchTouchEvent(EventTouch* event)
{
    auto touchOneByOneID = static_cast<EventListener::ListenerID>(EventListener::Type::TOUCH_ONE_BY_ONE);
    auto touchAllAtOnceID = static_cast<EventListener::ListenerID>(EventListener::Type::TOUCH_ALL_AT_ONCE);
    
    sortEventListeners(touchOneByOneID);
    sortEventListeners(touchAllAtOnceID);
    
    auto oneByOnelisteners = getListeners(touchOneByOneID);
    auto allAtOncelisteners = getListeners(touchAllAtOnceID);
    
    // If there aren't any touch listeners, return directly.
    if (nullptr == oneByOnelisteners && nullptr == allAtOncelisteners)
        return;
    
    bool isNeedsMutableSet = (oneByOnelisteners && allAtOncelisteners);
    
    std::vector<Touch*> orignalTouches = event->getTouches();
    std::vector<Touch*> mutableTouches(orignalTouches.size());
    std::copy(orignalTouches.begin(), orignalTouches.end(), mutableTouches.begin());

    //
    // process the target handlers 1st
    //
    if (oneByOnelisteners)
    {
        auto mutableTouchesIter = mutableTouches.begin();
        auto touchesIter = orignalTouches.begin();
        
        for (; touchesIter != orignalTouches.end(); ++touchesIter)
        {
            bool isSwallowed = false;

            auto onTouchEvent = [&](EventListener* l) -> bool { // Return true to break
                EventListenerTouchOneByOne* listener = static_cast<EventListenerTouchOneByOne*>(l);
                
                // Skip if the listener was removed.
                if (!listener->m_bIsRegistered)
                    return false;
             
                event->setCurrentTarget(listener->m_pNode);
                
                bool isClaimed = false;
                std::vector<Touch*>::iterator removedIter;
                
                EventTouch::EventCode eventCode = event->getEventCode();
                
                if (eventCode == EventTouch::EventCode::BEGAN)
                {
                    if (listener->onTouchBegan)
                    {
                        isClaimed = listener->onTouchBegan(*touchesIter, event);
                        if (isClaimed && listener->m_bIsRegistered)
                        {
                            listener->m_aClaimedTouches.push_back(*touchesIter);
                        }
                    }
                }
                else if (listener->m_aClaimedTouches.size() > 0
                         && ((removedIter = std::find(listener->m_aClaimedTouches.begin(), listener->m_aClaimedTouches.end(), *touchesIter)) != listener->m_aClaimedTouches.end()))
                {
                    isClaimed = true;
                    
                    switch (eventCode)
                    {
                        case EventTouch::EventCode::MOVED:
                            if (listener->onTouchMoved)
                            {
                                listener->onTouchMoved(*touchesIter, event);
                            }
                            break;
                        case EventTouch::EventCode::ENDED:
                            if (listener->onTouchEnded)
                            {
                                listener->onTouchEnded(*touchesIter, event);
                            }
                            if (listener->m_bIsRegistered)
                            {
                                listener->m_aClaimedTouches.erase(removedIter);
                            }
                            break;
                        case EventTouch::EventCode::CANCELLED:
                            if (listener->onTouchCancelled)
                            {
                                listener->onTouchCancelled(*touchesIter, event);
                            }
                            if (listener->m_bIsRegistered)
                            {
                                listener->m_aClaimedTouches.erase(removedIter);
                            }
                            break;
                        default:
                            CCASSERT(false, "The eventcode is invalid.");
                            break;
                    }
                }
                
                // If the event was stopped, return directly.
                if (event->isStopped())
                {
                    updateListeners(event);
                    return true;
                }
                
                CCASSERT((*touchesIter)->getID() == (*mutableTouchesIter)->getID(), "");
                
                if (isClaimed && listener->m_bIsRegistered && listener->m_bNeedSwallow)
                {
                    if (isNeedsMutableSet)
                    {
                        mutableTouchesIter = mutableTouches.erase(mutableTouchesIter);
                        isSwallowed = true;
                    }
                    return true;
                }
                
                return false;
            };
            
            //
            dispatchEventToListeners(oneByOnelisteners, onTouchEvent);
            if (event->isStopped())
            {
                return;
            }
            
            if (!isSwallowed)
                ++mutableTouchesIter;
        }
    }
    
    //
    // process standard handlers 2nd
    //
    if (allAtOncelisteners && mutableTouches.size() > 0)
    {
        
        auto onTouchesEvent = [&](EventListener* l) -> bool{
            EventListenerTouchAllAtOnce* listener = static_cast<EventListenerTouchAllAtOnce*>(l);
            // Skip if the listener was removed.
            if (!listener->m_bIsRegistered)
                return false;
            
            event->setCurrentTarget(listener->m_pNode);
            
            switch (event->getEventCode())
            {
                case EventTouch::EventCode::BEGAN:
                    if (listener->onTouchesBegan)
                    {
                        listener->onTouchesBegan(mutableTouches, event);
                    }
                    break;
                case EventTouch::EventCode::MOVED:
                    if (listener->onTouchesMoved)
                    {
                        listener->onTouchesMoved(mutableTouches, event);
                    }
                    break;
                case EventTouch::EventCode::ENDED:
                    if (listener->onTouchesEnded)
                    {
                        listener->onTouchesEnded(mutableTouches, event);
                    }
                    break;
                case EventTouch::EventCode::CANCELLED:
                    if (listener->onTouchesCancelled)
                    {
                        listener->onTouchesCancelled(mutableTouches, event);
                    }
                    break;
                default:
                    CCASSERT(false, "The eventcode is invalid.");
                    break;
            }
            
            // If the event was stopped, return directly.
            if (event->isStopped())
            {
                updateListeners(event);
                return false;
            }
            
            return false;
        };
        
        dispatchEventToListeners(allAtOncelisteners, onTouchesEvent);
        if (event->isStopped())
        {
            return;
        }
    }
    
    updateListeners(event);
}

void EventDispatcher::updateListeners(Event* event)
{
    auto onUpdateListeners = [this](EventListener::ListenerID listenerID)
    {
        auto listenersIter = m_aListeners.find(listenerID);
        if (listenersIter == m_aListeners.end())
            return;
        
        auto listeners = listenersIter->second;
        auto fixedPriorityListeners = listeners->getFixedPriorityListeners();
        auto sceneGraphPriorityListeners = listeners->getSceneGraphPriorityListeners();
        
        if (sceneGraphPriorityListeners)
        {
            for (auto iter = sceneGraphPriorityListeners->begin(); iter != sceneGraphPriorityListeners->end();)
            {
                auto l = *iter;
                if (!l->isRegistered())
                {
                    iter = sceneGraphPriorityListeners->erase(iter);
                    l->release();
                }
                else
                {
                    ++iter;
                }
            }
        }
        
        if (fixedPriorityListeners)
        {
            for (auto iter = fixedPriorityListeners->begin(); iter != fixedPriorityListeners->end();)
            {
                auto l = *iter;
                if (!l->isRegistered())
                {
                    iter = fixedPriorityListeners->erase(iter);
                    l->release();
                }
                else
                {
                    ++iter;
                }
            }
        }
        
        if (sceneGraphPriorityListeners && sceneGraphPriorityListeners->empty())
        {
            listeners->clearSceneGraphListeners();
        }

        if (fixedPriorityListeners && fixedPriorityListeners->empty())
        {
            listeners->clearFixedListeners();
        }

        if (listenersIter->second->empty())
        {
            m_aPriorityDirtyFlagMap.erase(listenersIter->first);
            delete listenersIter->second;
            listenersIter =  m_aListeners.erase(listenersIter);
        }
        else
        {
            ++listenersIter;
        }
    };
    
    if (event->getType() == Event::Type::TOUCH)
    {
        onUpdateListeners(static_cast<EventListener::ListenerID>(EventListener::Type::TOUCH_ONE_BY_ONE));
        onUpdateListeners(static_cast<EventListener::ListenerID>(EventListener::Type::TOUCH_ALL_AT_ONCE));
    }
    else
    {
        onUpdateListeners(getListenerID(event));
    }
    
    
    if (!m_aToAddedListeners.empty())
    {
        EventListenerVector* listeners = nullptr;
        
        for (auto& listener : m_aToAddedListeners)
        {
            EventListener::ListenerID listenerID = listener->getListenerID();
            auto itr = m_aListeners.find(listenerID);
            if (itr == m_aListeners.end())
            {
                
                listeners = new EventListenerVector();
                m_aListeners.insert(std::make_pair(listenerID, listeners));
            }
            else
            {
                listeners = itr->second;
            }
            
            listeners->push_back(listener);
            
            if (listener->getFixedPriority() == 0)
            {
                setDirty(listenerID, DirtyFlag::SCENE_GRAPH_PRIORITY);
            }
            else
            {
                setDirty(listenerID, DirtyFlag::FIXED_PRITORY);
            }
        }
        m_aToAddedListeners.clear();
    }
}

void EventDispatcher::updateDirtyFlagForSceneGraph()
{
    if (!m_aDirtyNodes.empty())
    {
        for (auto& node : m_aDirtyNodes)
        {
            auto iter = m_aNodeListenersMap.find(node);
            if (iter != m_aNodeListenersMap.end())
            {
                for (auto& l : *iter->second)
                {
                    setDirty(l->getListenerID(), DirtyFlag::SCENE_GRAPH_PRIORITY);
                }
            }
        }
        
        m_aDirtyNodes.clear();
    }
}

void EventDispatcher::sortEventListeners(EventListener::ListenerID listenerID)
{
    DirtyFlag dirtyFlag = DirtyFlag::NONE;
    
    auto dirtyIter = m_aPriorityDirtyFlagMap.find(listenerID);
    if (dirtyIter != m_aPriorityDirtyFlagMap.end())
    {
        dirtyFlag = dirtyIter->second;
    }
    
    if (dirtyFlag != DirtyFlag::NONE)
    {
        if ((int)dirtyFlag & (int)DirtyFlag::FIXED_PRITORY)
        {
            sortEventListenersOfFixedPriority(listenerID);
        }
        
        if ((int)dirtyFlag & (int)DirtyFlag::SCENE_GRAPH_PRIORITY)
        {
            sortEventListenersOfSceneGraphPriority(listenerID);
        }
        
        dirtyIter->second = DirtyFlag::NONE;
    }
}

void EventDispatcher::sortEventListenersOfSceneGraphPriority(EventListener::ListenerID listenerID)
{
    auto listeners = getListeners(listenerID);
    
    if (listeners == nullptr)
        return;
    
    Node* rootNode = (Node*)Director::getInstance()->getRunningScene();
    // Reset priority index
    m_nNodePriorityIndex = 0;
    m_aNodePriorityMap.clear();

    visitTarget(rootNode);
    
    // After sort: priority < 0, > 0
    auto sceneGraphlisteners = listeners->getSceneGraphPriorityListeners();
    std::sort(sceneGraphlisteners->begin(), sceneGraphlisteners->end(), [this](const EventListener* l1, const EventListener* l2) {
        return m_aNodePriorityMap[l1->getSceneGraphPriority()] > m_aNodePriorityMap[l2->getSceneGraphPriority()];
    });
    
#if DUMP_LISTENER_ITEM_PRIORITY_INFO
    log("-----------------------------------");
    for (auto& l : *sceneGraphlisteners)
    {
        log("listener priority: node ([%s]%p), priority (%d)", typeid(*l->_node).name(), l->_node, m_aNodePriorityMap[l->_node]);
    }
#endif
}

void EventDispatcher::sortEventListenersOfFixedPriority(EventListener::ListenerID listenerID)
{
    auto listeners = getListeners(listenerID);

    if (listeners == nullptr)
        return;
    
    // After sort: priority < 0, > 0
    auto fixedlisteners = listeners->getFixedPriorityListeners();
    std::sort(fixedlisteners->begin(), fixedlisteners->end(), [](const EventListener* l1, const EventListener* l2) {
        return l1->getFixedPriority() < l2->getFixedPriority();
    });
    
    // FIXME: Should use binary search
    long index = 0;
    for (auto& listener : *fixedlisteners)
    {
        if (listener->getFixedPriority() >= 0)
            break;
        ++index;
    }
    
    listeners->setGt0Index(index);
    
#if DUMP_LISTENER_ITEM_PRIORITY_INFO
    log("-----------------------------------");
    for (auto& l : *fixedlisteners)
    {
        log("listener priority: node (%p), fixed (%d)", l->_node, l->_fixedPriority);
    }    
#endif
    
}

EventDispatcher::EventListenerVector* EventDispatcher::getListeners(EventListener::ListenerID listenerID)
{
    auto iter = m_aListeners.find(listenerID);
    if (iter != m_aListeners.end())
    {
        return iter->second;
    }
    
    return nullptr;
}

void EventDispatcher::removeEventListenersForListenerID(EventListener::ListenerID listenerID)
{
    auto listenerItemIter = m_aListeners.find(listenerID);
    if (listenerItemIter != m_aListeners.end())
    {
        auto listeners = listenerItemIter->second;
        auto fixedPriorityListeners = listeners->getFixedPriorityListeners();
        auto sceneGraphPriorityListeners = listeners->getSceneGraphPriorityListeners();
        
        auto removeAllListenersInVector = [&](std::vector<EventListener*>* listenerVector){
            if (listenerVector == nullptr)
                return;
            
            for (auto iter = listenerVector->begin(); iter != listenerVector->end();)
            {
                auto l = *iter;
                l->setRegistered(false);
                if (l->getSceneGraphPriority() != nullptr)
                {
                    dissociateNodeAndEventListener(l->getSceneGraphPriority(), l);
                }
                
                if (m_nInDispatch == 0)
                {
                    iter = listenerVector->erase(iter);
                    CC_SAFE_RELEASE(l);
                }
                else
                {
                    ++iter;
                }
            }
        };
        
        removeAllListenersInVector(sceneGraphPriorityListeners);
        removeAllListenersInVector(fixedPriorityListeners);
        
        if (!m_nInDispatch)
        {
            listeners->clear();
            delete listeners;
            m_aListeners.erase(listenerItemIter);
            m_aPriorityDirtyFlagMap.erase(listenerID);
        }
    }
    
    for(auto iter = m_aToAddedListeners.begin(); iter != m_aToAddedListeners.end();)
    {
        if ((*iter)->getListenerID() == listenerID)
        {
            iter = m_aToAddedListeners.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void EventDispatcher::removeEventListeners(EventListener::Type listenerType)
{
    CCASSERT(listenerType != EventListener::Type::CUSTOM, "Not support custom event listener type, please use EventDispatcher::removeCustomEventListeners instead.");
    
    removeEventListenersForListenerID(static_cast<EventListener::ListenerID>(listenerType));
}

void EventDispatcher::removeCustomEventListeners(const std::string& customEventName)
{
    removeEventListenersForListenerID(std::hash<std::string>()(customEventName));
}

void EventDispatcher::removeAllEventListeners()
{
    std::vector<int> types(m_aListeners.size());

    for (auto iter = m_aListeners.begin(); iter != m_aListeners.end(); ++iter)
    {
        types.push_back(iter->first);
    }

    for (auto& type : types)
    {
        removeEventListenersForListenerID(type);
    }
    
    if (!m_nInDispatch)
    {
        m_aListeners.clear();
    }
}

void EventDispatcher::setEnabled(bool isEnabled)
{
    m_bIsEnabled = isEnabled;
}


bool EventDispatcher::isEnabled() const
{
    return m_bIsEnabled;
}

void EventDispatcher::setDirtyForNode(Node* node)
{
    // Mark the node dirty only when there was an eventlistener associates with it. 
    if (m_aNodeListenersMap.find(node) != m_aNodeListenersMap.end())
    {
        m_aDirtyNodes.insert(node);
    }
}

void EventDispatcher::setDirty(EventListener::ListenerID listenerID, DirtyFlag flag)
{    
    auto iter = m_aPriorityDirtyFlagMap.find(listenerID);
    if (iter == m_aPriorityDirtyFlagMap.end())
    {
        m_aPriorityDirtyFlagMap.insert(std::make_pair(listenerID, flag));
    }
    else
    {
        int ret = (int)flag | (int)iter->second;
        iter->second = (DirtyFlag) ret;
    }
}

NS_CC_END
