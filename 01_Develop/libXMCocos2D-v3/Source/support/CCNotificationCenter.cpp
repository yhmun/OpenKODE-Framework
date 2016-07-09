/* -----------------------------------------------------------------------------------
 *
 *      File            CCNotificationCenter.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2011      Erawppa
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

#include "support/CCNotificationCenter.h"
#include "base/CCArray.h"
#include "2d/script_support/CCScriptSupport.h"
#include <string>

using namespace std;

NS_CC_BEGIN

static NotificationCenter *s_sharedNotifCenter = NULL;

NotificationCenter::NotificationCenter()
: m_nScriptHandler(0)
{
    m_pObservers = Array::createWithCapacity(3);
    m_pObservers->retain();
}

NotificationCenter::~NotificationCenter()
{
    m_pObservers->release();
}

NotificationCenter *NotificationCenter::getInstance()
{
    if (!s_sharedNotifCenter)
    {
        s_sharedNotifCenter = new NotificationCenter;
    }
    return s_sharedNotifCenter;
}

void NotificationCenter::destroyInstance()
{
    CC_SAFE_RELEASE(s_sharedNotifCenter);
}

//
// internal functions
//
bool NotificationCenter::observerExisted(Object *target,const char *name, Object *sender)
{
    Object* obj = NULL;
    CCARRAY_FOREACH(m_pObservers, obj)
    {
        NotificationObserver* observer = (NotificationObserver*) obj;
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target && observer->getSender() == sender)
            return true;
    }
    return false;
}

//
// observer functions
//
void NotificationCenter::addObserver(Object *target, 
                                       SEL_CallFuncO selector,
                                       const char *name,
                                       Object *sender)
{
    if (this->observerExisted(target, name, sender))
        return;
    
    NotificationObserver *observer = new NotificationObserver(target, selector, name, sender);
    if (!observer)
        return;
    
    observer->autorelease();
    m_pObservers->addObject(observer);
}

void NotificationCenter::removeObserver(Object *target,const char *name)
{
    Object* obj = NULL;
    CCARRAY_FOREACH(m_pObservers, obj)
    {
        NotificationObserver* observer = static_cast<NotificationObserver*>(obj);
        if (!observer)
            continue;
        
        if (!strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_pObservers->removeObject(observer);
            return;
        }
    }
}

int NotificationCenter::removeAllObservers(Object *target)
{
    Object *obj = NULL;
    Array *toRemove = Array::create();

    CCARRAY_FOREACH(m_pObservers, obj)
    {
        NotificationObserver *observer = static_cast<NotificationObserver *>(obj);
        if (!observer)
            continue;

        if (observer->getTarget() == target)
        {
            toRemove->addObject(observer);
        }
    }

    m_pObservers->removeObjectsInArray(toRemove);
    return toRemove->count();
}

void NotificationCenter::registerScriptObserver( Object *target, int handler,const char* name)
{
    
    if (this->observerExisted(target, name, NULL))
        return;
    
    NotificationObserver *observer = new NotificationObserver(target, NULL, name, NULL);
    if (!observer)
        return;
    
    observer->setHandler(handler);
    observer->autorelease();
    m_pObservers->addObject(observer);
}

void NotificationCenter::unregisterScriptObserver(Object *target,const char* name)
{        
    Object* obj = NULL;
    CCARRAY_FOREACH(m_pObservers, obj)
    {
        NotificationObserver* observer = static_cast<NotificationObserver*>(obj);
        if (!observer)
            continue;
            
        if ( !strcmp(observer->getName(),name) && observer->getTarget() == target)
        {
            m_pObservers->removeObject(observer);
        }
    }
}

void NotificationCenter::postNotification(const char *name, Object *sender)
{
    Array* ObserversCopy = Array::createWithCapacity(m_pObservers->count());
    ObserversCopy->addObjectsFromArray(m_pObservers);
    Object* obj = NULL;
    CCARRAY_FOREACH(ObserversCopy, obj)
    {
        NotificationObserver* observer = static_cast<NotificationObserver*>(obj);
        if (!observer)
            continue;
        
        if (!strcmp(name,observer->getName()) && (observer->getSender() == sender || observer->getSender() == NULL || sender == NULL))
        {
            if (0 != observer->getHandler())
            {
                BasicScriptData data(this, (void*)name);
                ScriptEvent scriptEvent(kNotificationEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
            }
            else
            {
                observer->performSelector(sender);
            }
        }
    }
}

void NotificationCenter::postNotification(const char *name)
{
    this->postNotification(name,NULL);
}

int NotificationCenter::getObserverHandlerByName(const char* name)
{
    if (NULL == name || strlen(name) == 0)
    {
        return 0;
    }
    
    Object* obj = NULL;
    CCARRAY_FOREACH(m_pObservers, obj)
    {
        NotificationObserver* observer = static_cast<NotificationObserver*>(obj);
        if (NULL == observer)
            continue;
        
        if ( 0 == strcmp(observer->getName(),name) )
        {
            return observer->getHandler();
            break;
        }
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///
/// NotificationObserver
///
////////////////////////////////////////////////////////////////////////////////
NotificationObserver::NotificationObserver(Object *target, 
                                               SEL_CallFuncO selector,
                                               const char *name,
                                               Object *sender)
{
    m_pTarget = target;
    m_pSelector = selector;
    m_pSender = sender;
    
    m_sName = name;
    m_nHandler = 0;
}

NotificationObserver::~NotificationObserver()
{

}

void NotificationObserver::performSelector(Object *sender)
{
    if (m_pTarget)
    {
		if (sender) {
			(m_pTarget->*m_pSelector)(sender);
		} else {
			(m_pTarget->*m_pSelector)(m_pSender);
		}
    }
}

Object *NotificationObserver::getTarget() const
{
    return m_pTarget;
}

SEL_CallFuncO NotificationObserver::getSelector() const
{
    return m_pSelector;
}

const char* NotificationObserver::getName() const
{
    return m_sName.c_str();
}

Object* NotificationObserver::getSender() const
{
    return m_pSender;
}

int NotificationObserver::getHandler() const
{
    return m_nHandler;
}

void NotificationObserver::setHandler(int var)
{
    m_nHandler = var;
}

NS_CC_END
