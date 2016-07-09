/* -----------------------------------------------------------------------------------
 *
 *      File            CCBAnimationManager.cpp
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

#include "editor-support/cocosbuilder/CCBAnimationManager.h"
#include "editor-support/cocosbuilder/CCBSequence.h"
#include "editor-support/cocosbuilder/CCBSequenceProperty.h"
#include "editor-support/cocosbuilder/CCBReader.h"
#include "editor-support/cocosbuilder/CCBKeyframe.h"
#include "editor-support/cocosbuilder/CCNode+CCBRelativePositioning.h"
#include <string>
#include <set>
#include <sstream>
#include "audio/SimpleAudioEngine.h"
#include "editor-support/cocosbuilder/CCBSelectorResolver.h"
#include "base/CCInteger.h"
#include "2d/actions/CCActionInstant.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::extension;

namespace cocosbuilder {

// Implementation of CCBAinmationManager

CCBAnimationManager::CCBAnimationManager()
: m_bJsControlled(false)
, m_pOwner(NULL)
, m_pSequences(NULL)
, m_pNodeSequences(NULL)
, m_pBaseValues(NULL)
, m_nAutoPlaySequenceId(0)
, m_pRootNode(NULL)
, m_tRootContainerSize(Size::ZERO)
, m_pDelegate(NULL)
, m_pRunningSequence(NULL)

{
    init();
}

bool CCBAnimationManager::init()
{
    m_pSequences = new Array();
    m_pSequences->init();
    m_pNodeSequences = new Dictionary();
    m_pNodeSequences->init();
    m_pBaseValues = new Dictionary();
    m_pBaseValues->init();
    
    m_pDocumentOutletNames = new Array();
    m_pDocumentOutletNames->init();
    
    m_pDocumentOutletNodes = new Array();
    m_pDocumentOutletNodes->init();
    
    m_pDocumentCallbackNames = new Array();
    m_pDocumentCallbackNames->init();
    
    m_pDocumentCallbackNodes = new Array();
    m_pDocumentCallbackNodes->init();
    
    m_pDocumentCallbackControlEvents = new Array();
    m_pDocumentCallbackControlEvents->init();
    
    m_pKeyframeCallbacks = new Array();
    m_pKeyframeCallbacks->init();
    
    m_pKeyframeCallFuncs = new Dictionary();
    m_pKeyframeCallFuncs->init();

    m_pTarget = NULL;
    m_pAnimationCompleteCallbackFunc = NULL;
    
    return true;
}

CCBAnimationManager::~CCBAnimationManager()
{
//     DictElement *pElement = NULL;
//     CCDICT_FOREACH(_nodeSequences, pElement)
//     {
//         Node *node = (Node*)pElement->getIntKey();
//         node->release();
//     }
//     
//     CCDICT_FOREACH(_baseValues, pElement)
//     {
//         Node *node = (Node*)pElement->getIntKey();
//         node->release();
//     }
    
    m_pNodeSequences->release();
    m_pBaseValues->release();
    m_pSequences->release();
    setRootNode(NULL);
    setDelegate(NULL);

    CC_SAFE_RELEASE(m_pDocumentOutletNames);
    CC_SAFE_RELEASE(m_pDocumentOutletNodes);
    CC_SAFE_RELEASE(m_pDocumentCallbackNames);
    CC_SAFE_RELEASE(m_pDocumentCallbackNodes);
    CC_SAFE_RELEASE(m_pDocumentCallbackControlEvents);
    
    CC_SAFE_RELEASE(m_pKeyframeCallFuncs);
    CC_SAFE_RELEASE(m_pKeyframeCallbacks);
    CC_SAFE_RELEASE(m_pTarget);
}

Array* CCBAnimationManager::getSequences()
{
    return m_pSequences;
}

void CCBAnimationManager::setSequences(Array* seq)
{
    m_pSequences = seq;
}

int CCBAnimationManager::getAutoPlaySequenceId()
{
    return m_nAutoPlaySequenceId;
}

void CCBAnimationManager::setAutoPlaySequenceId(int autoPlaySequenceId)
{
    m_nAutoPlaySequenceId = autoPlaySequenceId;
}

Node* CCBAnimationManager::getRootNode()
{
    return m_pRootNode;
}

void CCBAnimationManager::setRootNode(Node *pRootNode)
{
    m_pRootNode = pRootNode;
}

void CCBAnimationManager::setDocumentControllerName(const std::string &name) {
    m_sDocumentControllerName = name;
}


std::string CCBAnimationManager::getDocumentControllerName() {
    return m_sDocumentControllerName;
}

void CCBAnimationManager::addDocumentCallbackNode(Node *node) {
    m_pDocumentCallbackNodes->addObject(node);
}

void CCBAnimationManager::addDocumentCallbackName(std::string name) {
    String *tmpName = String::create(name);
    m_pDocumentCallbackNames->addObject(tmpName);
}

void CCBAnimationManager::addDocumentCallbackControlEvents(Control::EventType eventType)
{
    m_pDocumentCallbackControlEvents->addObject(Integer::create((int)eventType));
}

Array* CCBAnimationManager::getDocumentCallbackNames() {
    return m_pDocumentCallbackNames;
}

Array* CCBAnimationManager::getDocumentCallbackNodes() {
    return m_pDocumentCallbackNodes;
}

Array* CCBAnimationManager::getDocumentCallbackControlEvents()
{
    return m_pDocumentCallbackControlEvents;
}

void CCBAnimationManager::addDocumentOutletNode(Node *node) {
    m_pDocumentOutletNodes->addObject(node);
}

void CCBAnimationManager::addDocumentOutletName(std::string name) {
    m_pDocumentOutletNames->addObject(String::create(name));
}

Array* CCBAnimationManager::getDocumentOutletNames() {
    return m_pDocumentOutletNames;
}

Array* CCBAnimationManager::getDocumentOutletNodes() {
    return m_pDocumentOutletNodes;
}

std::string CCBAnimationManager::getLastCompletedSequenceName() {
    return m_sLastCompletedSequenceName;
}

Array* CCBAnimationManager::getKeyframeCallbacks() {
    return m_pKeyframeCallbacks;
}

const Size& CCBAnimationManager::getRootContainerSize()
{
    return m_tRootContainerSize;
}

void CCBAnimationManager::setRootContainerSize(const Size &rootContainerSize)
{
    m_tRootContainerSize.setSize(rootContainerSize.width, rootContainerSize.height);
}

CCBAnimationManagerDelegate* CCBAnimationManager::getDelegate()
{
    return m_pDelegate;
}

void CCBAnimationManager::setDelegate(CCBAnimationManagerDelegate *pDelegate)
{
    if ( dynamic_cast<Object*>(m_pDelegate) )
    {
        ( dynamic_cast<Object*>(m_pDelegate) )->release ( );
    }
    m_pDelegate = pDelegate;
    CC_SAFE_RETAIN(dynamic_cast<Object*>(m_pDelegate));
}

const char* CCBAnimationManager::getRunningSequenceName()
{
    if (m_pRunningSequence)
    {
        return m_pRunningSequence->getName();
    }
    return NULL;
}

const Size& CCBAnimationManager::getContainerSize(Node *pNode)
{
    if (pNode)
    {
        return pNode->getContentSize();
    }
    else 
    {
        return m_tRootContainerSize;
    }
}

// refer to CCBReader::readNodeGraph() for data structure of pSeq
void CCBAnimationManager::addNode(Node *pNode, Dictionary *pSeq)
{
    // pNode->retain();
    
    m_pNodeSequences->setObject(pSeq, (KDintptr)pNode);
}

void CCBAnimationManager::setBaseValue(Object *pValue, Node *pNode, const char *propName)
{
    Dictionary *props = (Dictionary*)m_pBaseValues->objectForKey((KDintptr)pNode);
    if (! props)
    {
        props = Dictionary::create();
        m_pBaseValues->setObject(props, (KDintptr)pNode);
        // pNode->retain();
    }
    
    props->setObject(pValue, propName);
}

Object* CCBAnimationManager::getBaseValue(Node *pNode, const char* propName)
{
    Dictionary *props = (Dictionary*)m_pBaseValues->objectForKey((KDintptr)pNode);
    
    return props->objectForKey(propName);
}

int CCBAnimationManager::getSequenceId(const char* pSequenceName)
{
    Object *pElement = NULL;
    string seqName(pSequenceName);
    CCARRAY_FOREACH(m_pSequences, pElement)
    {
        CCBSequence *seq = static_cast<CCBSequence*>(pElement);
        if (seqName.compare(seq->getName()) == 0)
        {
            return seq->getSequenceId();
        }
    }
    return -1;
}

CCBSequence* CCBAnimationManager::getSequence(int nSequenceId)
{
    Object *pElement = NULL;
    CCARRAY_FOREACH(m_pSequences, pElement)
    {
        CCBSequence *seq = static_cast<CCBSequence*>(pElement);
        if (seq->getSequenceId() == nSequenceId)
        {
            return seq;
        }
    }
    return NULL;
}

float CCBAnimationManager::getSequenceDuration(const char *pSequenceName)
{
    int id = getSequenceId(pSequenceName);
    if (id != -1)
        return getSequence(id)->getDuration();
    return 0;
}


void CCBAnimationManager::moveAnimationsFromNode(Node* fromNode, Node* toNode) {

    // Move base values
    Object* baseValue = m_pBaseValues->objectForKey((KDintptr)fromNode);
    if(baseValue) {
        m_pBaseValues->setObject(baseValue, (KDintptr)toNode);
        m_pBaseValues->removeObjectForKey((KDintptr)fromNode);

//         fromNode->release();
//         toNode->retain();
    }
    
    // Move seqs
    Object *seqs = m_pNodeSequences->objectForKey((KDintptr)fromNode);
    if(seqs) {
        m_pNodeSequences->setObject(seqs, (KDintptr)toNode);
        m_pNodeSequences->removeObjectForKey((KDintptr)fromNode);

//         fromNode->release();
//         toNode->retain();
    }
}

// Refer to CCBReader::readKeyframe() for the real type of value
ActionInterval* CCBAnimationManager::getAction(CCBKeyframe *pKeyframe0, CCBKeyframe *pKeyframe1, const char *propName, Node *pNode)
{
    float duration = pKeyframe1->getTime() - (pKeyframe0 ? pKeyframe0->getTime() : 0);
    
    if (kdStrcmp(propName, "rotationX") == 0)
    {
        CCBValue *value = (CCBValue*)pKeyframe1->getValue();
        return CCBRotateXTo::create(duration, value->getFloatValue());
    }
    else if(kdStrcmp(propName, "rotationY") == 0)
    {
        CCBValue *value = (CCBValue*)pKeyframe1->getValue();
        return CCBRotateYTo::create(duration, value->getFloatValue());
    }
    else if (kdStrcmp(propName, "rotation") == 0)
    {
        CCBValue *value = (CCBValue*)pKeyframe1->getValue();
        return CCBRotateTo::create(duration, value->getFloatValue());
    } 
    else if (kdStrcmp(propName, "opacity") == 0)
    {
        CCBValue *value = (CCBValue*)pKeyframe1->getValue();
        return FadeTo::create(duration, value->getByteValue());
    }
    else if (kdStrcmp(propName, "color") == 0)
    {
        Color3BWapper* color = (Color3BWapper*)pKeyframe1->getValue();
        Color3B c = color->getColor();
        
        return TintTo::create(duration, c.r, c.g, c.b);
    }
    else if (kdStrcmp(propName, "visible") == 0)
    {
        CCBValue *value = (CCBValue*)pKeyframe1->getValue();
        if (value->getBoolValue())
        {
            return Sequence::createWithTwoActions(DelayTime::create(duration), Show::create());
        }
        else 
        {
            return Sequence::createWithTwoActions(DelayTime::create(duration), Hide::create());
        }
    }
    else if (kdStrcmp(propName, "displayFrame") == 0)
    {
        return Sequence::createWithTwoActions(DelayTime::create(duration),
                                                CCBSetSpriteFrame::create((SpriteFrame *)pKeyframe1->getValue()));
    }
    else if (kdStrcmp(propName, "position") == 0)
    {
        // Get position type
        Array *array = static_cast<Array*>(getBaseValue(pNode, propName));
        CCBReader::PositionType type = (CCBReader::PositionType)((CCBValue*)array->getObjectAtIndex(2))->getIntValue();
        
        // Get relative position
        Array *value = static_cast<Array*>(pKeyframe1->getValue());
        float x = ((CCBValue*)value->getObjectAtIndex(0))->getFloatValue();
        float y = ((CCBValue*)value->getObjectAtIndex(1))->getFloatValue();
        
        Size containerSize = getContainerSize(pNode->getParent());
        
        Point absPos = getAbsolutePosition(Point(x,y), type, containerSize, propName);
        
        return MoveTo::create(duration, absPos);
    }
    else if (kdStrcmp(propName, "scale") == 0)
    {
        // Get position type
        Array *array = (Array*)getBaseValue(pNode, propName);
        CCBReader::ScaleType type = (CCBReader::ScaleType)((CCBValue*)array->getObjectAtIndex(2))->getIntValue();
        
        // Get relative scale
        Array *value = (Array*)pKeyframe1->getValue();
        float x = ((CCBValue*)value->getObjectAtIndex(0))->getFloatValue();
        float y = ((CCBValue*)value->getObjectAtIndex(1))->getFloatValue();
        
        if (type == CCBReader::ScaleType::MULTIPLY_RESOLUTION)
        {
            float resolutionScale = CCBReader::getResolutionScale();
            x *= resolutionScale;
            y *= resolutionScale;
        }
        
        return ScaleTo::create(duration, x, y);
    }
    else if(kdStrcmp(propName, "skew") == 0) 
    {
        // Get relative skew
        Array *value = (Array*)pKeyframe1->getValue();
        float x = ((CCBValue*)value->getObjectAtIndex(0))->getFloatValue();
        float y = ((CCBValue*)value->getObjectAtIndex(1))->getFloatValue();
        
        return SkewTo::create(duration, x, y);
    }
    else 
    {
        log("CCBReader: Failed to create animation for property: %s", propName);
    }
    
    return NULL;
}

void CCBAnimationManager::setAnimatedProperty(const char *propName, Node *pNode, Object *pValue, float fTweenDuration)
{
    if (fTweenDuration > 0)
    {
        // Create a fake keyframe to generate the action from
        CCBKeyframe *kf1 = new CCBKeyframe();
        kf1->autorelease();
        kf1->setValue(pValue);
        kf1->setTime(fTweenDuration);
        kf1->setEasingType(CCBKeyframe::EasingType::LINEAR);
        
        // Animate
        ActionInterval *tweenAction = getAction(NULL, kf1, propName, pNode);
        pNode->runAction(tweenAction);
    }
    else 
    {
        // Just set the value
        
        if (kdStrcmp(propName, "position") == 0)
        {
            // Get position type
            Array *array = (Array*)getBaseValue(pNode, propName);
            CCBReader::PositionType type = (CCBReader::PositionType)((CCBValue*)array->getObjectAtIndex(2))->getIntValue();
            
            // Get relative position
            Array *value = (Array*)pValue;
            float x = ((CCBValue*)value->getObjectAtIndex(0))->getFloatValue();
            float y = ((CCBValue*)value->getObjectAtIndex(1))->getFloatValue();
            
            pNode->setPosition(getAbsolutePosition(Point(x,y), type, getContainerSize(pNode->getParent()), propName));
        }
        else if (kdStrcmp(propName, "scale") == 0)
        {
            // Get scale type
            Array *array = (Array*)getBaseValue(pNode, propName);
            CCBReader::ScaleType type = (CCBReader::ScaleType)((CCBValue*)array->getObjectAtIndex(2))->getIntValue();
            
            // Get relative scale
            Array *value = (Array*)pValue;
            float x = ((CCBValue*)value->getObjectAtIndex(0))->getFloatValue();
            float y = ((CCBValue*)value->getObjectAtIndex(1))->getFloatValue();
            
            setRelativeScale(pNode, x, y, type, propName);
        }
        else if(kdStrcmp(propName, "skew") == 0)
        {
            // Get relative scale
            Array *value = (Array*)pValue;
            float x = ((CCBValue*)value->getObjectAtIndex(0))->getFloatValue();
            float y = ((CCBValue*)value->getObjectAtIndex(1))->getFloatValue();

            pNode->setSkewX(x);
            pNode->setSkewY(y);
        }
        else 
        {
            // [node setValue:value forKey:name];

            // TODO only handle rotation, opacity, displayFrame, color
            if (kdStrcmp(propName, "rotation") == 0)
            {
                float rotate = ((CCBValue*)pValue)->getFloatValue();
                pNode->setRotation(rotate);
            } else if(kdStrcmp(propName, "rotationX") == 0)
            {
                float rotate = ((CCBValue*)pValue)->getFloatValue();
                pNode->setRotationX(rotate);
            }else if(kdStrcmp(propName, "rotationY") == 0)
            {
                float rotate = ((CCBValue*)pValue)->getFloatValue();
                pNode->setRotationY(rotate);
            }
            else if (kdStrcmp(propName, "opacity") == 0)
            {
                int opacity = ((CCBValue*)pValue)->getByteValue();
                (dynamic_cast<RGBAProtocol*>(pNode))->setOpacity(opacity);
            }
            else if (kdStrcmp(propName, "displayFrame") == 0)
            {
                ((Sprite*)pNode)->setDisplayFrame((SpriteFrame*)pValue);
            }
            else if (kdStrcmp(propName, "color") == 0)
            {
                Color3BWapper *color = (Color3BWapper*)pValue;
                (dynamic_cast<RGBAProtocol*>(pNode))->setColor(color->getColor());
            }
            else if (kdStrcmp(propName, "visible") == 0)
            {
                bool visible = ((CCBValue*)pValue)->getBoolValue();
                pNode->setVisible(visible);
            }
            else
            {
                log("unsupported property name is %s", propName);
                CCASSERT(false, "unsupported property now");
            }
        }
    }
}

void CCBAnimationManager::setFirstFrame(Node *pNode, CCBSequenceProperty *pSeqProp, float fTweenDuration)
{
    Array *keyframes = pSeqProp->getKeyframes();
    
    if (keyframes->count() == 0)
    {
        // Use base value (no animation)
        Object *baseValue = getBaseValue(pNode, pSeqProp->getName());
        CCASSERT(baseValue, "No baseValue found for property");
        setAnimatedProperty(pSeqProp->getName(), pNode, baseValue, fTweenDuration);
    }
    else 
    {
        // Use first keyframe
        CCBKeyframe *keyframe = (CCBKeyframe*)keyframes->getObjectAtIndex(0);
        setAnimatedProperty(pSeqProp->getName(), pNode, keyframe->getValue(), fTweenDuration);
    }
}

ActionInterval* CCBAnimationManager::getEaseAction(ActionInterval *pAction, CCBKeyframe::EasingType easingType, float fEasingOpt)
{
    if (dynamic_cast<Sequence*>(pAction))
    {
        return pAction;
    }
    
    if (easingType == CCBKeyframe::EasingType::LINEAR)
    {
        return pAction;
    }
    else if (easingType == CCBKeyframe::EasingType::INSTANT)
    {
        return CCBEaseInstant::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::CUBIC_IN)
    {
        return EaseIn::create(pAction, fEasingOpt);
    }
    else if (easingType == CCBKeyframe::EasingType::CUBIC_OUT)
    {
        return EaseOut::create(pAction, fEasingOpt);
    }
    else if (easingType == CCBKeyframe::EasingType::CUBIC_INOUT)
    {
        return EaseInOut::create(pAction, fEasingOpt);
    }
    else if (easingType == CCBKeyframe::EasingType::BACK_IN)
    {
        return EaseBackIn::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::BACK_OUT)
    {
        return EaseBackOut::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::BACK_INOUT)
    {
        return EaseBackInOut::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::BOUNCE_IN)
    {
        return EaseBounceIn::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::BOUNCE_OUT)
    {
        return EaseBounceOut::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::BOUNCE_INOUT)
    {
        return EaseBounceInOut::create(pAction);
    }
    else if (easingType == CCBKeyframe::EasingType::ELASTIC_IN)
    {
        return EaseElasticIn::create(pAction, fEasingOpt);
    }
    else if (easingType == CCBKeyframe::EasingType::ELASTIC_OUT)
    {
        return EaseElasticOut::create(pAction, fEasingOpt);
    }
    else if (easingType == CCBKeyframe::EasingType::ELASTIC_INOUT)
    {
        return EaseElasticInOut::create(pAction, fEasingOpt);
    }
    else
    {
        log("CCBReader: Unkown easing type %d", easingType);
        return pAction;
    }
}

Object* CCBAnimationManager::actionForCallbackChannel(CCBSequenceProperty* channel) {
  
    float lastKeyframeTime = 0;
    
    Array *actions = Array::create();
    Array *keyframes = channel->getKeyframes();
    long numKeyframes = keyframes->count();

    for (long i = 0; i < numKeyframes; ++i)
    {

        CCBKeyframe *keyframe = (CCBKeyframe*)keyframes->getObjectAtIndex(i);
        float timeSinceLastKeyframe = keyframe->getTime() - lastKeyframeTime;
        lastKeyframeTime = keyframe->getTime();
        if(timeSinceLastKeyframe > 0) {
            actions->addObject(DelayTime::create(timeSinceLastKeyframe));
        }
	
        Array* keyVal = static_cast<Array *>(keyframe->getValue());
        std::string selectorName = static_cast<String *>(keyVal->getObjectAtIndex(0))->getCString();
        CCBReader::TargetType selectorTarget = (CCBReader::TargetType)kdAtoi(static_cast<String *>(keyVal->getObjectAtIndex(1))->getCString());
	
        if(m_bJsControlled) {
            String* callbackName = String::createWithFormat("%d:%s", selectorTarget, selectorName.c_str());
            Object* callback = m_pKeyframeCallFuncs->objectForKey(callbackName->getCString());
            if (nullptr != callback)
            {
                CallFunc *callbackClone = (static_cast<CallFunc*>(callback))->clone();
    
                if(callbackClone != NULL) {
                    actions->addObject(callbackClone);
                }
            }
        }
        else
        {
            Object* target = NULL;
            
            if(selectorTarget == CCBReader::TargetType::DOCUMENT_ROOT)
                target = m_pRootNode;
            else if (selectorTarget == CCBReader::TargetType::OWNER)
                target = m_pOwner;
            
            if(target != NULL)
            {
                if(selectorName.length() > 0)
                {
                    SEL_CallFuncN selCallFunc = 0;
                    
                    CCBSelectorResolver* targetAsCCBSelectorResolver = dynamic_cast<CCBSelectorResolver *>(target);

                    if(targetAsCCBSelectorResolver != NULL)
                    {
                        selCallFunc = targetAsCCBSelectorResolver->onResolveCCBCCCallFuncSelector(target, selectorName.c_str    ());
                    }
                    
                    if(selCallFunc == 0)
                    {
                        CCLOG("Skipping selector '%s' since no CCBSelectorResolver is present.", selectorName.c_str());
                    }
                    else
                    {
                        // XXX: how to fix this warning?
	//					CallFuncN*	callback = CallFuncN::create ( CC_CALLBACK_1(selCallFunc, target));
  //                      CallFuncN *callback = CallFuncN::create(target, selCallFunc);				
      //                  actions->addObject(callback);
                    }
                }
                else
                {
                    CCLOG("Unexpected empty selector.");
                }
            }
        }
    }
    if(actions->count() < 1) return NULL;
    
    return (Object *) Sequence::create(actions);
}

Object* CCBAnimationManager::actionForSoundChannel(CCBSequenceProperty* channel) {
    
    float lastKeyframeTime = 0;
    
    Array *actions = Array::create();
    Array *keyframes = channel->getKeyframes();
    int numKeyframes = keyframes->count();

    for (int i = 0; i < numKeyframes; ++i) {

        CCBKeyframe *keyframe = (CCBKeyframe*)keyframes->getObjectAtIndex(i);
        float timeSinceLastKeyframe = keyframe->getTime() - lastKeyframeTime;
        lastKeyframeTime = keyframe->getTime();
        if(timeSinceLastKeyframe > 0) {
            actions->addObject(DelayTime::create(timeSinceLastKeyframe));
        }
	
        stringstream ss (stringstream::in | stringstream::out);
        Array* keyVal = (Array*)keyframe->getValue();
        std::string soundFile = ((String *)keyVal->getObjectAtIndex(0))->getCString();
    
        float pitch, pan, gain;
        ss << ((String *)keyVal->getObjectAtIndex(1))->getCString();
        ss >> pitch;
        ss.flush();
    
        ss << ((String *)keyVal->getObjectAtIndex(2))->getCString();
        ss >> pan;
        ss.flush();
        
        ss << ((String *)keyVal->getObjectAtIndex(3))->getCString();
        ss >> gain;
        ss.flush();
        
        actions->addObject(CCBSoundEffect::actionWithSoundFile(soundFile, pitch, pan, gain));
    }

    if(actions->count() < 1) return NULL;
    
    return (Object *) Sequence::create(actions);    
}



void CCBAnimationManager::runAction(Node *pNode, CCBSequenceProperty *pSeqProp, float fTweenDuration)
{
    Array *keyframes = pSeqProp->getKeyframes();
    int numKeyframes = keyframes->count();
    
    if (numKeyframes > 1)
    {
        // Make an animation!
        Array *actions = Array::create();
        
        CCBKeyframe *keyframeFirst = (CCBKeyframe*)keyframes->getObjectAtIndex(0);
        float timeFirst = keyframeFirst->getTime() + fTweenDuration;
        
        if (timeFirst > 0)
        {
            actions->addObject(DelayTime::create(timeFirst));
        }
        
        for (int i = 0; i < numKeyframes - 1; ++i)
        {
            CCBKeyframe *kf0 = (CCBKeyframe*)keyframes->getObjectAtIndex(i);
            CCBKeyframe *kf1 = (CCBKeyframe*)keyframes->getObjectAtIndex(i+1);
            
            ActionInterval *action = getAction(kf0, kf1, pSeqProp->getName(), pNode);
            if (action)
            {
                // Apply easing
                action = getEaseAction(action, kf0->getEasingType(), kf0->getEasingOpt());
                
                actions->addObject(action);
            }
        }
        
        FiniteTimeAction *seq = Sequence::create(actions);
        pNode->runAction(seq);
    }
}

void CCBAnimationManager::runAnimationsForSequenceIdTweenDuration(int nSeqId, float fTweenDuration)
{
    CCASSERT(nSeqId != -1, "Sequence id couldn't be found");
    
    m_pRootNode->stopAllActions();
    
    DictElement* pElement = NULL;
    CCDICT_FOREACH(m_pNodeSequences, pElement)
    {
        Node *node = reinterpret_cast<Node*>(pElement->getIntKey());
        node->stopAllActions();
        
        // Refer to CCBReader::readKeyframe() for the real type of value
        Dictionary *seqs = (Dictionary*)pElement->getObject();
        Dictionary *seqNodeProps = (Dictionary*)seqs->objectForKey(nSeqId);
        
        set<string> seqNodePropNames;
        
        if (seqNodeProps)
        {
            // Reset nodes that have sequence node properties, and run actions on them
            DictElement* pElement1 = NULL;
            CCDICT_FOREACH(seqNodeProps, pElement1)
            {
                const char *propName = pElement1->getStrKey();
                CCBSequenceProperty *seqProp = static_cast<CCBSequenceProperty*>(seqNodeProps->objectForKey(propName));
                seqNodePropNames.insert(propName);
                
                setFirstFrame(node, seqProp, fTweenDuration);
                runAction(node, seqProp, fTweenDuration);
            }
        }
        
        // Reset the nodes that may have been changed by other timelines
        Dictionary *nodeBaseValues = (Dictionary*)m_pBaseValues->objectForKey(pElement->getIntKey());
        if (nodeBaseValues)
        {
            DictElement* pElement2 = NULL;
            CCDICT_FOREACH(nodeBaseValues, pElement2)
            {
                if (seqNodePropNames.find(pElement2->getStrKey()) == seqNodePropNames.end())
                {
                    Object *value = pElement2->getObject();
                    
                    if (value)
                    {
                       setAnimatedProperty(pElement2->getStrKey(), node, value, fTweenDuration);
                    }
                }
            }
        }
    }
    
    // Make callback at end of sequence
    CCBSequence *seq = getSequence(nSeqId);
    Action *completeAction = Sequence::createWithTwoActions(DelayTime::create(seq->getDuration() + fTweenDuration),
                                                                CallFunc::create( CC_CALLBACK_0(CCBAnimationManager::sequenceCompleted,this)));
    m_pRootNode->runAction(completeAction);
    
    // Set the running scene

    if(seq->getCallbackChannel() != NULL) {
        Action* action = (Action *)actionForCallbackChannel(seq->getCallbackChannel());
        if(action != NULL) {
            m_pRootNode->runAction(action);
        }
    } 

    if(seq->getSoundChannel() != NULL) {
        Action* action = (Action *)actionForSoundChannel(seq->getSoundChannel());
        if(action != NULL) {
            m_pRootNode->runAction(action);
        }
    }

    m_pRunningSequence = getSequence(nSeqId);
}

void CCBAnimationManager::runAnimationsForSequenceNamedTweenDuration(const char *pName, float fTweenDuration)
{
    int seqId = getSequenceId(pName);
    runAnimationsForSequenceIdTweenDuration(seqId, fTweenDuration);
}

void CCBAnimationManager::runAnimationsForSequenceNamed(const char *pName)
{
    runAnimationsForSequenceNamedTweenDuration(pName, 0);
}

void CCBAnimationManager::debug()
{
    
}

void CCBAnimationManager::setAnimationCompletedCallback(Object *target, SEL_CallFunc callbackFunc) {
    if (target)
    {
        target->retain();
    }
    
    if (m_pTarget)
    {
        m_pTarget->release();
    }
    
    m_pTarget = target;
    m_pAnimationCompleteCallbackFunc = callbackFunc;
}

void CCBAnimationManager::setCallFunc(CallFunc* callFunc, const std::string &callbackNamed) {
    m_pKeyframeCallFuncs->setObject((Object*)callFunc, callbackNamed);
}

void CCBAnimationManager::sequenceCompleted()
{
    const char *runningSequenceName = m_pRunningSequence->getName();
    int nextSeqId = m_pRunningSequence->getChainedSequenceId();
    m_pRunningSequence = NULL;
    
    if(m_sLastCompletedSequenceName != runningSequenceName) {
        m_sLastCompletedSequenceName = runningSequenceName;
    }
    
    if (nextSeqId != -1)
    {
        runAnimationsForSequenceIdTweenDuration(nextSeqId, 0);
    }
    
    if (m_pDelegate)
    {
        // There may be another runAnimation() call in this delegate method
        // which will assign _runningSequence
        m_pDelegate->completedAnimationSequenceNamed(runningSequenceName);
    }
    
    if (m_pTarget && m_pAnimationCompleteCallbackFunc) {
        (m_pTarget->*m_pAnimationCompleteCallbackFunc)();
    }
}

// Custom actions

/************************************************************
 CCBSetSpriteFrame
 ************************************************************/

CCBSetSpriteFrame* CCBSetSpriteFrame::create(SpriteFrame *pSpriteFrame)
{
    CCBSetSpriteFrame *ret = new CCBSetSpriteFrame();
    if (ret)
    {
        if (ret->initWithSpriteFrame(pSpriteFrame))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_DELETE(ret);
        }
    }
    
    return ret;
}

bool CCBSetSpriteFrame::initWithSpriteFrame(SpriteFrame *pSpriteFrame)
{
    m_pSpriteFrame = pSpriteFrame;
    CC_SAFE_RETAIN(m_pSpriteFrame);
    
    return true;
}

CCBSetSpriteFrame::~CCBSetSpriteFrame()
{
    CC_SAFE_RELEASE(m_pSpriteFrame);
}

CCBSetSpriteFrame* CCBSetSpriteFrame::clone() const
{
	// no copy constructor
	auto a = new CCBSetSpriteFrame();
    a->initWithSpriteFrame(m_pSpriteFrame);
	a->autorelease();
	return a;
}

CCBSetSpriteFrame* CCBSetSpriteFrame::reverse() const
{
	// returns a copy of itself
	return this->clone();
}

void CCBSetSpriteFrame::update(float time)
{
    ((Sprite*)m_pTarget)->setDisplayFrame(m_pSpriteFrame);
}


/************************************************************
 CCBSoundEffect
 ************************************************************/

CCBSoundEffect* CCBSoundEffect::actionWithSoundFile(const std::string &filename, float pitch, float pan, float gain) {
  CCBSoundEffect* pRet = new CCBSoundEffect();
  if (pRet != NULL && pRet->initWithSoundFile(filename, pitch, pan, gain))
    {
      pRet->autorelease();
    }
  else
    {
      CC_SAFE_DELETE(pRet);
    }
  return pRet;
}


CCBSoundEffect::~CCBSoundEffect()
{
}

bool CCBSoundEffect::initWithSoundFile(const std::string &filename, float pitch, float pan, float gain) {
    m_sSoundFile = filename;
    m_fPitch = pitch;
    m_fPan = pan;
    m_fGain = gain;
    return true;
}

CCBSoundEffect* CCBSoundEffect::clone() const
{
	// no copy constructor
	auto a = new CCBSoundEffect();
    a->initWithSoundFile(m_sSoundFile, m_fPitch, m_fPan, m_fGain);
	a->autorelease();
	return a;
}

CCBSoundEffect* CCBSoundEffect::reverse() const
{
	// returns a copy of itself
	return this->clone();
}

void CCBSoundEffect::update(float time)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(m_sSoundFile.c_str());
}


/************************************************************
 CCBRotateTo
 ************************************************************/

CCBRotateTo* CCBRotateTo::create(float fDuration, float fAngle)
{
    CCBRotateTo *ret = new CCBRotateTo();
    if (ret)
    {
        if (ret->initWithDuration(fDuration, fAngle))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_DELETE(ret);
        }
    }
    
    return ret;
}

bool CCBRotateTo::initWithDuration(float fDuration, float fAngle)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngle = fAngle;
        
        return true;
    }
    else 
    {
        return false;
    }
}

CCBRotateTo* CCBRotateTo::clone() const
{
	// no copy constructor	
	auto a = new CCBRotateTo();
    a->initWithDuration(m_fDuration, m_fDstAngle);
	a->autorelease();
	return a;
}

CCBRotateTo* CCBRotateTo::reverse() const
{
	CCASSERT(false, "reverse() is not supported in CCBRotateTo");
	return nullptr;
}

void CCBRotateTo::startWithTarget(Node *pNode)
{
    ActionInterval::startWithTarget(pNode);
    m_fStartAngle = m_pTarget->getRotation();
    m_fDiffAngle = m_fDstAngle - m_fStartAngle;
}

void CCBRotateTo::update(float time)
{
    m_pTarget->setRotation(m_fStartAngle + (m_fDiffAngle * time))
    ;
}




/************************************************************
 CCBRotateXTO
 ************************************************************/


CCBRotateXTo* CCBRotateXTo::create(float fDuration, float fAngle)
{
    CCBRotateXTo *ret = new CCBRotateXTo();
    if (ret)
    {
        if (ret->initWithDuration(fDuration, fAngle))
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
    }
    
    return ret;
}

bool CCBRotateXTo::initWithDuration(float fDuration, float fAngle)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngle = fAngle;
        
        return true;
    }
    else
    {
        return false;
    }
}


void CCBRotateXTo::startWithTarget(Node *pNode)
{
    //CCActionInterval::startWithTarget(pNode);
    m_pOriginalTarget = pNode;
    m_pTarget = pNode;
    m_fElapsed = 0.0f;
    m_bFirstTick = true;
    m_fStartAngle = m_pTarget->getRotationX();
    m_fDiffAngle = m_fDstAngle - m_fStartAngle;
}

CCBRotateXTo* CCBRotateXTo::clone() const
{
	// no copy constructor
	auto a = new CCBRotateXTo();
    a->initWithDuration(m_fDuration, m_fDstAngle);
	a->autorelease();
	return a;
}

CCBRotateXTo* CCBRotateXTo::reverse() const
{
	CCASSERT(false, "reverse() is not supported in CCBRotateXTo");
	return nullptr;
}

void CCBRotateXTo::update(float time)
{
    m_pTarget->setRotationX(m_fStartAngle + (m_fDiffAngle * time))
    ;
}



/************************************************************
 CCBRotateYTO
 ************************************************************/



CCBRotateYTo* CCBRotateYTo::create(float fDuration, float fAngle)
{
    CCBRotateYTo *ret = new CCBRotateYTo();
    if (ret)
    {
        if (ret->initWithDuration(fDuration, fAngle))
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
    }
    
    return ret;
}

bool CCBRotateYTo::initWithDuration(float fDuration, float fAngle)
{
    if (ActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngle = fAngle;
        
        return true;
    }
    else
    {
        return false;
    }
}

CCBRotateYTo* CCBRotateYTo::clone() const
{
	// no copy constructor
	auto a = new CCBRotateYTo();
    a->initWithDuration(m_fDuration, m_fDstAngle);
	a->autorelease();
	return a;
}

CCBRotateYTo* CCBRotateYTo::reverse() const
{
	CCASSERT(false, "reverse() is not supported in CCBRotateXTo");
	return nullptr;
}


void CCBRotateYTo::startWithTarget(Node *pNode)
{
 //   ActionInterval::startWithTarget(pNode);
    m_pOriginalTarget = pNode;
    m_pTarget = pNode;
    m_fElapsed = 0.0f;
    m_bFirstTick = true;
    m_fStartAngle = m_pTarget->getRotationY();
    m_fDiffAngle = m_fDstAngle - m_fStartAngle;
}

void CCBRotateYTo::update(float time)
{
    m_pTarget->setRotationY(m_fStartAngle + (m_fDiffAngle * time))
    ;
}



/************************************************************
 CCBEaseInstant
 ************************************************************/
CCBEaseInstant* CCBEaseInstant::create(ActionInterval *pAction)
{
    CCBEaseInstant *pRet = new CCBEaseInstant();
    if (pRet && pRet->initWithAction(pAction))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE(pRet);
    }
    
    return pRet;
}

CCBEaseInstant* CCBEaseInstant::clone() const
{
	// no copy constructor	
	auto a = new CCBEaseInstant();
    a->initWithAction(m_pInner);
	a->autorelease();
	return a;
}

CCBEaseInstant* CCBEaseInstant::reverse() const
{
	return CCBEaseInstant::create(m_pInner->reverse());
}

void CCBEaseInstant::update(float dt)
{
    if (dt < 0)
    {
        m_pInner->update(0);
    }
    else
    {
        m_pInner->update(1);
    }
}


}
