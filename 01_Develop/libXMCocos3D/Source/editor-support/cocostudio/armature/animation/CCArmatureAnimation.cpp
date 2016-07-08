/* -----------------------------------------------------------------------------------
 *
 *      File            CCArmatureAnimation.cpp
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

#include "editor-support/cocostudio/armature/animation/CCArmatureAnimation.h"
#include "editor-support/cocostudio/armature/CCArmature.h"
#include "editor-support/cocostudio/armature/CCBone.h"
#include "editor-support/cocostudio/armature/utils/CCArmatureDefine.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"
#include "editor-support/cocostudio/armature/datas/CCDatas.h"

using namespace cocos2d;

namespace cocostudio {

ArmatureAnimation *ArmatureAnimation::create(Armature *armature)
{
    ArmatureAnimation *pArmatureAnimation = new ArmatureAnimation();
    if (pArmatureAnimation && pArmatureAnimation->init(armature))
    {
        pArmatureAnimation->autorelease();
        return pArmatureAnimation;
    }
    CC_SAFE_DELETE(pArmatureAnimation);
    return nullptr;
}


ArmatureAnimation::ArmatureAnimation()
    : m_pAnimationData(nullptr)
    , m_fSpeedScale(1)
    , m_pMovementData(nullptr)
    , m_pArmature(nullptr)
    , m_sMovementID("")
    , m_nToIndex(0)
    , m_pTweenList(nullptr)
    , m_bIgnoreFrameEvent(false)
    , m_pUserObject(nullptr)

    , m_pMovementEventCallFunc(nullptr)
    , m_pFrameEventCallFunc(nullptr)
    , m_pMovementEventTarget(nullptr)
    , m_pFrameEventTarget(nullptr)
{

}

ArmatureAnimation::~ArmatureAnimation(void)
{
    CC_SAFE_RELEASE(m_pTweenList);
    CC_SAFE_RELEASE(m_pAnimationData);

    CC_SAFE_RELEASE(m_pUserObject);
}

bool ArmatureAnimation::init(Armature *armature)
{
    bool bRet = false;
    do
    {
        m_pArmature = armature;

        m_pTweenList = new Array();
        m_pTweenList->init();

        bRet = true;
    }
    while (0);

    return bRet;
}


void ArmatureAnimation:: pause()
{
    for(auto object : *m_pTweenList)
    {
		static_cast<Tween*>(object)->pause();
    }
    ProcessBase::pause();
}

void ArmatureAnimation::resume()
{
    for(auto object : *m_pTweenList)
    {
        static_cast<Tween*>(object)->resume();
    }
    ProcessBase::resume();
}

void ArmatureAnimation::stop()
{
    for(auto object : *m_pTweenList)
    {
        static_cast<Tween*>(object)->stop();
    }
    m_pTweenList->removeAllObjects();
    ProcessBase::stop();
}

void ArmatureAnimation::setSpeedScale(float speedScale)
{
    if(speedScale == m_fSpeedScale)
    {
        return;
    }

    m_fSpeedScale = speedScale;

    m_fProcessScale = !m_pMovementData ? m_fSpeedScale : m_fSpeedScale * m_pMovementData->m_fScale;

    DictElement *element = nullptr;
    const Dictionary *dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(dict, element)
    {
        Bone *bone = static_cast<Bone*>(element->getObject());

        bone->getTween()->setProcessScale(m_fProcessScale);
        if (bone->getChildArmature())
        {
            bone->getChildArmature()->getAnimation()->setProcessScale(m_fProcessScale);
        }
    }
}

float ArmatureAnimation::getSpeedScale() const
{
    return m_fSpeedScale;
}

void ArmatureAnimation::setAnimationInternal(float animationInternal)
{
    if(animationInternal == m_fAnimationInternal)
    {
        return;
    }

    m_fAnimationInternal = animationInternal;

    DictElement *element = nullptr;
    const Dictionary *dict = m_pArmature->getBoneDic();
    CCDICT_FOREACH(dict, element)
    {
        Bone *bone = static_cast<Bone*>(element->getObject());
        bone->getTween()->setAnimationInternal(m_fAnimationInternal);
        if (bone->getChildArmature())
        {
            bone->getChildArmature()->getAnimation()->setAnimationInternal(m_fAnimationInternal);
        }
    }
}


void ArmatureAnimation::play(const char *animationName, int durationTo, int durationTween,  int loop, int tweenEasing)
{
    CCASSERT(m_pAnimationData, "_animationData can not be null");

    m_pMovementData = m_pAnimationData->getMovement(animationName);
    CCASSERT(m_pMovementData, "_movementData can not be null");

    //! Get key frame count
    m_nRawDuration = m_pMovementData->m_nDuration;

    m_sMovementID = animationName;

    m_fProcessScale = m_fSpeedScale * m_pMovementData->m_fScale;

    //! Further processing parameters
    durationTo = (durationTo == -1) ? m_pMovementData->m_nDurationTo : durationTo;

    durationTween = (durationTween == -1) ? m_pMovementData->m_nDurationTween : durationTween;
    durationTween = (durationTween == 0) ? m_pMovementData->m_nDuration : durationTween;

    tweenEasing	= (tweenEasing == TWEEN_EASING_MAX) ? m_pMovementData->m_eTweenEasing : tweenEasing;
    loop = (loop < 0) ? m_pMovementData->m_bLoop : loop;


    ProcessBase::play(durationTo, durationTween, loop, tweenEasing);


    if (m_nRawDuration == 0)
    {
        m_eLoopType = SINGLE_FRAME;
    }
    else
    {
        if (loop)
        {
            m_eLoopType = ANIMATION_TO_LOOP_FRONT;
        }
        else
        {
            m_eLoopType = ANIMATION_NO_LOOP;
            m_nRawDuration --;
        }
        m_nDurationTween = durationTween;
    }

    MovementBoneData *movementBoneData = nullptr;
    m_pTweenList->removeAllObjects();

    DictElement *element = nullptr;
    const Dictionary *dict = m_pArmature->getBoneDic();

    CCDICT_FOREACH(dict, element)
    {
        Bone *bone = static_cast<Bone*>(element->getObject());
        movementBoneData = static_cast<MovementBoneData *>(m_pMovementData->m_tMovBoneDataDic.objectForKey(bone->getName()));

        Tween *tween = bone->getTween();
        if(movementBoneData && movementBoneData->m_tFrameList.count() > 0)
        {
            m_pTweenList->addObject(tween);
            movementBoneData->m_fDuration = m_pMovementData->m_nDuration;
            tween->play(movementBoneData, durationTo, durationTween, loop, tweenEasing);

            tween->setProcessScale(m_fProcessScale);
            tween->setAnimationInternal(m_fAnimationInternal);

            if (bone->getChildArmature())
            {
                bone->getChildArmature()->getAnimation()->setProcessScale(m_fProcessScale);
                bone->getChildArmature()->getAnimation()->setAnimationInternal(m_fAnimationInternal);
            }
        }
        else
        {
            if(!bone->isIgnoreMovementBoneData())
            {
                //! this bone is not include in this movement, so hide it
                bone->getDisplayManager()->changeDisplayByIndex(-1, false);
                tween->stop();
            }

        }
    }

    m_pArmature->update(0);
}


void ArmatureAnimation::playByIndex(int animationIndex, int durationTo, int durationTween,  int loop, int tweenEasing)
{
    std::vector<std::string> &movName = m_pAnimationData->m_aMovementNames;
    CC_ASSERT((animationIndex > -1) && ((unsigned int)animationIndex < movName.size()));

    std::string animationName = movName.at(animationIndex);
    play(animationName.c_str(), durationTo, durationTween, loop, tweenEasing);
}

void ArmatureAnimation::gotoAndPlay(int frameIndex)
{
    if (!m_pMovementData || frameIndex < 0 || frameIndex >= m_pMovementData->m_nDuration)
    {
        CCLOG("Please ensure you have played a movement, and the frameIndex is in the range.");
        return;
    }

    bool ignoreFrameEvent = m_bIgnoreFrameEvent;
    m_bIgnoreFrameEvent = true;

    m_bIsPlaying = true;
    m_bIsComplete = m_bIsPause = false;

    ProcessBase::gotoFrame(frameIndex);
    m_fCurrentPercent = (float)m_nCurFrameIndex / (float)m_pMovementData->m_nDuration;
    m_fCurrentFrame = m_nNextFrameIndex * m_fCurrentPercent;

    for(auto object : *m_pTweenList)
    {
        static_cast<Tween *>(object)->gotoAndPlay(frameIndex);
    }

    m_pArmature->update(0);

    m_bIgnoreFrameEvent = ignoreFrameEvent;
}

void ArmatureAnimation::gotoAndPause(int frameIndex)
{
    gotoAndPlay(frameIndex);
    pause();
}

int ArmatureAnimation::getMovementCount() const
{
    return m_pAnimationData->getMovementCount();
}

void ArmatureAnimation::update(float dt)
{
    ProcessBase::update(dt);
    
    for(auto object : *m_pTweenList)
    {
		static_cast<Tween *>(object)->update(dt);
    }

    while (m_aFrameEventQueue.size() > 0)
    {
        FrameEvent *frameEvent = m_aFrameEventQueue.front();
        m_aFrameEventQueue.pop();

        m_bIgnoreFrameEvent = true;
        (m_pFrameEventTarget->*m_pFrameEventCallFunc)(frameEvent->bone, frameEvent->frameEventName, frameEvent->originFrameIndex, frameEvent->currentFrameIndex);
        m_bIgnoreFrameEvent = false;

        CC_SAFE_DELETE(frameEvent);
    }
}

void ArmatureAnimation::updateHandler()
{
    if (m_fCurrentPercent >= 1)
    {
        switch(m_eLoopType)
        {
        case ANIMATION_NO_LOOP:
        {
            m_eLoopType = ANIMATION_MAX;
            m_fCurrentFrame = (m_fCurrentPercent - 1) * m_nNextFrameIndex;
            m_fCurrentPercent = m_fCurrentFrame / m_nDurationTween;

            if (m_fCurrentPercent >= 1.0f)
            {
            }
            else
            {
                m_nNextFrameIndex = m_nDurationTween;

                if (m_pMovementEventTarget && m_pMovementEventCallFunc)
                {
                    (m_pMovementEventTarget->*m_pMovementEventCallFunc)(m_pArmature, START, m_sMovementID.c_str());
                }

                break;
            }
        }
        break;
        case ANIMATION_MAX:
        case SINGLE_FRAME:
        {
            m_fCurrentPercent = 1;
            m_bIsComplete = true;
            m_bIsPlaying = false;

            if (m_pMovementEventTarget && m_pMovementEventCallFunc)
            {
                (m_pMovementEventTarget->*m_pMovementEventCallFunc)(m_pArmature, COMPLETE, m_sMovementID.c_str());
            }
        }
        break;
        case ANIMATION_TO_LOOP_FRONT:
        {
            m_eLoopType = ANIMATION_LOOP_FRONT;
            m_fCurrentPercent = kdFmodf(m_fCurrentPercent, 1);
            m_fCurrentFrame = m_nNextFrameIndex == 0 ? 0 : kdFmodf(m_fCurrentFrame, m_nNextFrameIndex);
            m_nNextFrameIndex = m_nDurationTween > 0 ? m_nDurationTween : 1;

            if (m_pMovementEventTarget && m_pMovementEventCallFunc)
            {
                (m_pMovementEventTarget->*m_pMovementEventCallFunc)(m_pArmature, START, m_sMovementID.c_str());
            }
        }
        break;
        default:
        {
            //_currentPercent = kdFmodf(_currentPercent, 1);
            m_fCurrentFrame = kdFmodf(m_fCurrentFrame, m_nNextFrameIndex);
            m_nToIndex = 0;

            if (m_pMovementEventTarget && m_pMovementEventCallFunc)
            {
                (m_pMovementEventTarget->*m_pMovementEventCallFunc)(m_pArmature, LOOP_COMPLETE, m_sMovementID.c_str());
            }
        }
        break;
        }
    }
}

std::string ArmatureAnimation::getCurrentMovementID() const
{
    if (m_bIsComplete)
    {
        return "";
    }
    return m_sMovementID;
}

void ArmatureAnimation::setMovementEventCallFunc(Object *target, SEL_MovementEventCallFunc callFunc)
{
    m_pMovementEventTarget = target;
    m_pMovementEventCallFunc = callFunc;
}

void ArmatureAnimation::setFrameEventCallFunc(Object *target, SEL_FrameEventCallFunc callFunc)
{
    m_pFrameEventTarget = target;
    m_pFrameEventCallFunc = callFunc;
}

void ArmatureAnimation::setUserObject(Object *pUserObject)
{
    CC_SAFE_RETAIN(pUserObject);
    CC_SAFE_RELEASE(m_pUserObject);
    m_pUserObject = pUserObject;
}

void ArmatureAnimation::frameEvent(Bone *bone, const char *frameEventName, int originFrameIndex, int currentFrameIndex)
{
    if (m_pFrameEventTarget && m_pFrameEventCallFunc)
    {
        FrameEvent *frameEvent = new FrameEvent();
        frameEvent->bone = bone;
        frameEvent->frameEventName = frameEventName;
        frameEvent->originFrameIndex = originFrameIndex;
        frameEvent->currentFrameIndex = currentFrameIndex;

        m_aFrameEventQueue.push(frameEvent);
    }
}
}
