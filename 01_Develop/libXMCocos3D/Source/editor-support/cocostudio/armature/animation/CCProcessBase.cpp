/* -----------------------------------------------------------------------------------
 *
 *      File            CCProcessBase.cpp
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

#include "editor-support/cocostudio/armature/animation/CCProcessBase.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"
#include "2d/CCDirector.h"

using namespace cocos2d;

namespace cocostudio {

ProcessBase::ProcessBase(void)
    : m_fProcessScale(1)
    , m_bIsPause(true)
    , m_bIsComplete(true)
    , m_bIsPlaying(false)
    , m_fCurrentPercent(0.0f)
    , m_nRawDuration(0)
    , m_eLoopType(ANIMATION_LOOP_BACK)
    , m_eTweenEasing(Linear)
    , m_nDurationTween(0)
    , m_fCurrentFrame(0)
    , m_nCurFrameIndex(0)
    , m_bIsLoopBack(false)
{
    /*
     *  set _animationInternal defualt value to Director::getInstance()
     *  ->getAnimationInterval(), in line with game update speed
     */
    m_fAnimationInternal = Director::getInstance()->getAnimationInterval();
}


ProcessBase::~ProcessBase(void)
{
}


void ProcessBase::pause()
{
    m_bIsPause = true;
    m_bIsPlaying = false;
}


void ProcessBase::resume()
{
    m_bIsPause = false;
    m_bIsPlaying = true;
}

void ProcessBase::stop()
{
    m_bIsComplete = true;
    m_bIsPlaying = false;
    m_fCurrentFrame = 0;
    m_fCurrentPercent = 0;
}

void ProcessBase::play(int durationTo, int durationTween,  int loop, int tweenEasing)
{
    m_bIsComplete = false;
    m_bIsPause = false;
    m_bIsPlaying = true;
    m_fCurrentFrame = 0;

    /*
     *  Set m_iTotalFrames to durationTo, it is used for change tween between two animation.
     *  When changing end, m_iTotalFrames will be setted to _durationTween
     */
    m_nNextFrameIndex = durationTo;
    m_eTweenEasing = (TweenType)tweenEasing;

}

void ProcessBase::update(float dt)
{

    if (m_bIsComplete || m_bIsPause)
    {
        return;
    }

    /*
     *  Fileter the m_iDuration <=0 and dt >1
     *  If dt>1, generally speaking  the reason is the device is stuck.
     */
    if(m_nRawDuration <= 0 || dt > 1)
    {
        return;
    }

    if (m_nNextFrameIndex <= 0)
    {
        m_fCurrentPercent = 1;
        m_fCurrentFrame = 0;
    }
    else
    {
        /*
        *  update _currentFrame, every update add the frame passed.
        *  dt/_animationInternal determine it is not a frame animation. If frame speed changed, it will not make our
        *  animation speed slower or quicker.
        */
        m_fCurrentFrame += m_fProcessScale * (dt / m_fAnimationInternal);


        m_fCurrentPercent = m_fCurrentFrame / m_nNextFrameIndex;

        /*
        *	if _currentFrame is bigger or equal than m_iTotalFrames, then reduce it util _currentFrame is
        *  smaller than m_iTotalFrames
        */
        m_fCurrentFrame = kdFmodf(m_fCurrentFrame, m_nNextFrameIndex);
    }

    updateHandler();
}



void ProcessBase::gotoFrame(int frameIndex)
{
    if (m_eLoopType == ANIMATION_NO_LOOP)
    {
        m_eLoopType = ANIMATION_MAX;
    }
    else if (m_eLoopType == ANIMATION_TO_LOOP_FRONT)
    {
        m_eLoopType = ANIMATION_LOOP_FRONT;
    }

    m_nCurFrameIndex = frameIndex;
    
    m_nNextFrameIndex = m_nDurationTween;
}

int ProcessBase::getCurrentFrameIndex()
{
    m_nCurFrameIndex = m_nRawDuration * m_fCurrentPercent;
    return m_nCurFrameIndex;
}

}
