/* -----------------------------------------------------------------------------------
 *
 *      File            CCTween.cpp
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

#include "editor-support/cocostudio/armature/animation/CCTween.h"
#include "editor-support/cocostudio/armature/animation/CCArmatureAnimation.h"
#include "editor-support/cocostudio/armature/CCBone.h"
#include "editor-support/cocostudio/armature/CCArmature.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"
#include "editor-support/cocostudio/armature/utils/CCTweenFunction.h"
#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"

namespace cocostudio {

Tween *Tween::create(Bone *bone)
{
    Tween *pTween = new Tween();
    if (pTween && pTween->init(bone))
    {
        pTween->autorelease();
        return pTween;
    }
    CC_SAFE_DELETE(pTween);
    return nullptr;

}



Tween::Tween()
    : m_pMovementBoneData(nullptr)
    , m_pTweenData(nullptr)
    , m_pFrom(nullptr)
    , m_pTo(nullptr)
    , m_pBetween(nullptr)
    , m_pBone(nullptr)

    , m_eFrameTweenEasing(Linear)
    , m_nFromIndex(0)
    , m_nToIndex(0)
    , m_pAnimation(nullptr)
    , m_bPassLastFrame(false)
{

}


Tween::~Tween(void)
{
    CC_SAFE_DELETE( m_pFrom );
    CC_SAFE_DELETE( m_pBetween );
}


bool Tween::init(Bone *bone)
{
    bool bRet = false;
    do
    {
        m_pFrom = new FrameData();
        m_pBetween = new FrameData();

        m_pBone = bone;
        m_pTweenData = m_pBone->getTweenData();
        m_pTweenData->m_nDisplayIndex = -1;

        m_pAnimation = m_pBone->getArmature() != nullptr ? m_pBone->getArmature()->getAnimation() : nullptr;

        bRet = true;
    }
    while (0);

    return bRet;
}


void Tween::play(MovementBoneData *movementBoneData, int durationTo, int durationTween,  int loop, int tweenEasing)
{
    ProcessBase::play(durationTo, durationTween, loop, tweenEasing);

    if (loop)
    {
        m_eLoopType = ANIMATION_TO_LOOP_FRONT;
    }
    else
    {
        m_eLoopType = ANIMATION_NO_LOOP;
    }

    m_nTotalDuration = 0;
    m_nBetweenDuration = 0;
    m_nFromIndex = m_nToIndex = 0;

    bool difMovement = movementBoneData != m_pMovementBoneData;

    setMovementBoneData(movementBoneData);
    m_nRawDuration = m_pMovementBoneData->m_fDuration;

    FrameData *nextKeyFrame = m_pMovementBoneData->getFrameData(0);
    m_pTweenData->m_nDisplayIndex = nextKeyFrame->m_nDisplayIndex;

    if (m_pBone->getArmature()->getArmatureData()->m_fDataVersion >= VERSION_COMBINED)
    {
        TransformHelp::nodeSub(*m_pTweenData, *m_pBone->getBoneData());
        m_pTweenData->m_fScaleX += 1;
        m_pTweenData->m_fScaleY += 1;
    }

    if (m_nRawDuration == 0 )
    {
        m_eLoopType = SINGLE_FRAME;
        if(durationTo == 0)
        {
            setBetween(nextKeyFrame, nextKeyFrame);
        }
        else
        {
            setBetween(m_pTweenData, nextKeyFrame);
        }
        m_eFrameTweenEasing = Linear;
    }
    else if (m_pMovementBoneData->m_tFrameList.count() > 1)
    {
        m_nDurationTween = durationTween * m_pMovementBoneData->m_fScale;

        if (loop && m_pMovementBoneData->m_fDelay != 0)
        {
            setBetween(m_pTweenData, tweenNodeTo(updateFrameData(1 - m_pMovementBoneData->m_fDelay), m_pBetween));
        }
        else
        {
            if (!difMovement || durationTo == 0)
            {
                setBetween(nextKeyFrame, nextKeyFrame);
            }
            else
            {
                setBetween(m_pTweenData, nextKeyFrame);
            }
        }
    }

    tweenNodeTo(0);
}

void Tween::gotoAndPlay(int frameIndex)
{
    ProcessBase::gotoFrame(frameIndex);

    m_nTotalDuration = 0;
    m_nBetweenDuration = 0;
    m_nFromIndex = m_nToIndex = 0;

    m_bIsPlaying = true;
    m_bIsComplete = m_bIsPause = false;

    m_fCurrentPercent = (float)m_nCurFrameIndex / (float)m_nRawDuration;
    m_fCurrentFrame = m_nNextFrameIndex * m_fCurrentPercent;
}

void Tween::gotoAndPause(int frameIndex)
{
    gotoAndPlay(frameIndex);
    pause();
}

void Tween::updateHandler()
{
    if (m_fCurrentPercent >= 1)
    {
        switch(m_eLoopType)
        {
        case SINGLE_FRAME:
        {
            m_fCurrentPercent = 1;
            m_bIsComplete = true;
            m_bIsPlaying = false;
        }
        break;
        case ANIMATION_NO_LOOP:
        {
            m_eLoopType = ANIMATION_MAX;


            if (m_nDurationTween <= 0)
            {
                m_fCurrentPercent = 1;
            }
            else
            {
                m_fCurrentPercent = (m_fCurrentPercent - 1) * m_nNextFrameIndex / m_nDurationTween;
            }

            if (m_fCurrentPercent >= 1)
            {
                m_fCurrentPercent = 1;
                m_bIsComplete = true;
                m_bIsPlaying = false;
                break;
            }
            else
            {
                m_nNextFrameIndex = m_nDurationTween;
                m_fCurrentFrame = m_fCurrentPercent * m_nNextFrameIndex;
                m_nTotalDuration = 0;
                m_nBetweenDuration = 0;
                m_nFromIndex = m_nToIndex = 0;
                break;
            }
        }
        break;
        case ANIMATION_TO_LOOP_FRONT:
        {
            m_eLoopType = ANIMATION_LOOP_FRONT;

            m_nNextFrameIndex = m_nDurationTween > 0 ? m_nDurationTween : 1;

            if (m_pMovementBoneData->m_fDelay != 0)
            {
                //
                m_fCurrentFrame = (1 - m_pMovementBoneData->m_fDelay) * (float)m_nNextFrameIndex;
                m_fCurrentPercent = m_fCurrentFrame / m_nNextFrameIndex;
            }
            else
            {
                m_fCurrentPercent = 0;
                m_fCurrentFrame = 0;
            }

            m_nTotalDuration = 0;
            m_nBetweenDuration = 0;
            m_nFromIndex = m_nToIndex = 0;
        }
        break;
        case ANIMATION_MAX:
        {
            m_fCurrentPercent = 1;
            m_bIsComplete = true;
            m_bIsPlaying = false;
        }
        break;
        default:
        {
            m_fCurrentFrame = kdFmodf(m_fCurrentFrame, m_nNextFrameIndex);
        }
        break;
        }
    }

    if (m_fCurrentPercent < 1 && m_eLoopType <= ANIMATION_TO_LOOP_BACK)
    {
        m_fCurrentPercent = kdSinf(m_fCurrentPercent * CC_HALF_PI);
    }

    float percent  = m_fCurrentPercent;

    if (m_eLoopType > ANIMATION_TO_LOOP_BACK)
    {
        percent = updateFrameData(percent);
    }

    if(m_eFrameTweenEasing != TWEEN_EASING_MAX)
    {
        tweenNodeTo(percent);
    }
}

void Tween::setBetween(FrameData *from, FrameData *to, bool limit)
{
    do
    {
        if(from->m_nDisplayIndex < 0 && to->m_nDisplayIndex >= 0)
        {
            m_pFrom->copy(to);
            m_pBetween->subtract(to, to, limit);
            break;
        }
        else if(to->m_nDisplayIndex < 0 && from->m_nDisplayIndex >= 0)
        {
            m_pFrom->copy(from);
            m_pBetween->subtract(to, to, limit);
            break;
        }

        m_pFrom->copy(from);
        m_pBetween->subtract(from, to, limit);
    }
    while (0);

    if (!from->m_bIsTween)
    {
        m_pTweenData->copy(from);
        m_pTweenData->m_bIsTween = true;
    }

    arriveKeyFrame(from);
}


void Tween::arriveKeyFrame(FrameData *keyFrameData)
{
    if(keyFrameData)
    {
        DisplayManager *displayManager = m_pBone->getDisplayManager();

        //! Change bone's display
        int displayIndex = keyFrameData->m_nDisplayIndex;

        if (!displayManager->isForceChangeDisplay())
        {
            displayManager->changeDisplayByIndex(displayIndex, false);
        }

        //! Update bone zorder, bone's zorder is determined by frame zorder and bone zorder
        m_pTweenData->m_zOrder = keyFrameData->m_zOrder;
        m_pBone->updateZOrder();

        //! Update blend type
        m_pBone->setBlendType(keyFrameData->m_eBlendType);

        //! Update child armature's movement
        Armature *childAramture = m_pBone->getChildArmature();
        if(childAramture)
        {
            if(keyFrameData->m_sMovement.length() != 0)
            {
                childAramture->getAnimation()->play(keyFrameData->m_sMovement.c_str());
            }
        }
    }
}

FrameData *Tween::tweenNodeTo(float percent, FrameData *node)
{
    node = node == nullptr ? m_pTweenData : node;

    if (!m_pFrom->m_bIsTween)
    {
        percent = 0;
    }

    node->x = m_pFrom->x + percent * m_pBetween->x;
    node->y = m_pFrom->y + percent * m_pBetween->y;
    node->m_fScaleX = m_pFrom->m_fScaleX + percent * m_pBetween->m_fScaleX;
    node->m_fScaleY = m_pFrom->m_fScaleY + percent * m_pBetween->m_fScaleY;
    node->m_fSkewX = m_pFrom->m_fSkewX + percent * m_pBetween->m_fSkewX;
    node->m_fSkewY = m_pFrom->m_fSkewY + percent * m_pBetween->m_fSkewY;

    m_pBone->setTransformDirty(true);

    if (node && m_pBetween->m_bIsUseColorInfo)
    {
        tweenColorTo(percent, node);
    }

    return node;
}

void Tween::tweenColorTo(float percent, FrameData *node)
{
    node->a = m_pFrom->a + percent * m_pBetween->a;
    node->r = m_pFrom->r + percent * m_pBetween->r;
    node->g = m_pFrom->g + percent * m_pBetween->g;
    node->b = m_pFrom->b + percent * m_pBetween->b;
    m_pBone->updateColor();
}

float Tween::updateFrameData(float currentPercent)
{
    if (currentPercent > 1 && m_pMovementBoneData->m_fDelay != 0)
    {
        currentPercent = kdFmodf(currentPercent, 1);
    }

    float playedTime = (float)m_nRawDuration * currentPercent;


    //! If play to current frame's front or back, then find current frame again
    if (playedTime < m_nTotalDuration || playedTime >= m_nTotalDuration + m_nBetweenDuration)
    {
        /*
         *  Get frame length, if _toIndex >= _length, then set _toIndex to 0, start anew.
         *  _toIndex is next index will play
         */
        int length = m_pMovementBoneData->m_tFrameList.count();
        FrameData **frames = (FrameData **)m_pMovementBoneData->m_tFrameList.data->arr;

        FrameData *from = nullptr;
        FrameData *to = nullptr;

        if (playedTime < frames[0]->m_nFrameID)
        {
            from = to = frames[0];
            setBetween(from, to);
            return m_fCurrentPercent;
        }
        
        if(playedTime >= frames[length - 1]->m_nFrameID)
        {
            // If _passLastFrame is true and playedTime >= frames[length - 1]->frameID, then do not need to go on. 
            if (m_bPassLastFrame)
            {
                from = to = frames[length - 1];
                setBetween(from, to);
                return m_fCurrentPercent;
            }
            m_bPassLastFrame = true;
        }
        else
        {
            m_bPassLastFrame = false;
        }


        do
        {
            m_nFromIndex = m_nToIndex;
            from = frames[m_nFromIndex];
            m_nTotalDuration  = from->m_nFrameID;

            m_nToIndex = m_nFromIndex + 1;
            if (m_nToIndex >= length)
            {
                m_nToIndex = 0;
            }

            to = frames[m_nToIndex];

            //! Guaranteed to trigger frame event
            if(from->m_sEvent.length() != 0 && !m_pAnimation->isIgnoreFrameEvent())
            {
                m_pAnimation->frameEvent(m_pBone, from->m_sEvent.c_str(), from->m_nFrameID, playedTime);
            }

            if (playedTime == from->m_nFrameID || (m_bPassLastFrame && m_nFromIndex == length-1))
            {
                break;
            }
        }
        while (playedTime < from->m_nFrameID || playedTime >= to->m_nFrameID);

        m_nBetweenDuration = to->m_nFrameID - from->m_nFrameID;

        m_eFrameTweenEasing = from->m_eTweenEasing;

        setBetween(from, to, false);

    }
    currentPercent = m_nBetweenDuration == 0 ? 0 : (playedTime - m_nTotalDuration) / (float)m_nBetweenDuration;


    /*
     *  If frame tween easing equal to TWEEN_EASING_MAX, then it will not do tween.
     */
    TweenType tweenType = (m_eFrameTweenEasing != Linear) ? m_eFrameTweenEasing : m_eTweenEasing;
    if (tweenType != TWEEN_EASING_MAX && tweenType != Linear && !m_bPassLastFrame)
    {
        currentPercent = TweenFunction::tweenTo(0, 1, currentPercent, 1, tweenType);
    }

    return currentPercent;
}

}
