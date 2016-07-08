/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionFrame.cpp
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

#include "editor-support/cocostudio/action/CCActionFrame.h"
#include "2d/actions/CCActionInterval.h"

using namespace cocos2d;

namespace cocostudio {

ActionFrame::ActionFrame()
: m_nFrameType(0)
, m_nEasingType(0)
, m_nFrameIndex(0)
, m_fTime(0.0f)
{

}
ActionFrame::~ActionFrame()
{

}

void ActionFrame::setFrameIndex(int index)
{
	m_nFrameIndex = index;
}
int ActionFrame::getFrameIndex()
{
	return m_nFrameIndex;
}

void ActionFrame::setFrameTime(float fTime)
{
	m_fTime = fTime;
}
float ActionFrame::getFrameTime()
{
	return m_fTime;
}

void ActionFrame::setFrameType(int frameType)
{
	m_nFrameType = frameType;
}
int ActionFrame::getFrameType()
{
	return m_nFrameType;
}

void ActionFrame::setEasingType(int easingType)
{
	m_nEasingType = easingType;
}
int ActionFrame::getEasingType()
{
	return m_nEasingType;
}

Action* ActionFrame::getAction(float fDuration)
{
	log("Need a definition of <getAction> for ActionFrame");
	return NULL;
}
//////////////////////////////////////////////////////////////////////////

ActionMoveFrame::ActionMoveFrame()
: m_tPosition(Point(0.0f,0.0f))
{
	m_nFrameType = (int)kKeyframeMove;
}
ActionMoveFrame::~ActionMoveFrame()
{

}
void ActionMoveFrame::setPosition(Point pos)
{
	m_tPosition = pos;
}
Point ActionMoveFrame::getPosition()
{
	return m_tPosition;
}
Action* ActionMoveFrame::getAction(float fDuration)
{
	return MoveTo::create(fDuration,m_tPosition);
}
//////////////////////////////////////////////////////////////////////////

ActionScaleFrame::ActionScaleFrame()
: m_fScaleX(1.0f)
, m_fScaleY(1.0f)
{
	m_nFrameType = (int)kKeyframeScale;
}

ActionScaleFrame::~ActionScaleFrame()
{

}

void ActionScaleFrame::setScaleX(float scaleX)
{
	m_fScaleX = scaleX;
}

float ActionScaleFrame::getScaleX()
{
	return m_fScaleX;
}

void ActionScaleFrame::setScaleY(float scaleY)
{
	m_fScaleY = scaleY;
}

float ActionScaleFrame::getScaleY()
{
	return m_fScaleY;
}

Action* ActionScaleFrame::getAction(float fDuration)
{
	return ScaleTo::create(fDuration,m_fScaleX,m_fScaleY);
}

ActionRotationFrame::ActionRotationFrame()
: m_fRotation(0.0f)
{
	m_nFrameType = (int)kKeyframeRotate;
}

ActionRotationFrame::~ActionRotationFrame()
{

}

void ActionRotationFrame::setRotation(float rotation)
{
	m_fRotation = rotation;
}

float ActionRotationFrame::getRotation()
{
	return m_fRotation;
}

Action* ActionRotationFrame::getAction(float fDuration)
{
	return RotateTo::create(fDuration,m_fRotation);
}

ActionFadeFrame::ActionFadeFrame()
: m_fOpacity(255)
{
	m_nFrameType = (int)kKeyframeFade;
}

ActionFadeFrame::~ActionFadeFrame()
{

}

void ActionFadeFrame::setOpacity(int opacity)
{
	m_fOpacity = opacity;
}

int ActionFadeFrame::getOpacity()
{
	return m_fOpacity;
}

Action* ActionFadeFrame::getAction(float fDuration)
{
	return FadeTo::create(fDuration,m_fOpacity);
}


ActionTintFrame::ActionTintFrame()
: m_tColor(Color3B(255,255,255))
{
	m_nFrameType = (int)kKeyframeTint;
}

ActionTintFrame::~ActionTintFrame()
{

}

void ActionTintFrame::setColor(Color3B ccolor)
{
	m_tColor = ccolor;
}

Color3B ActionTintFrame::getColor()
{
	return m_tColor;
}

Action* ActionTintFrame::getAction(float fDuration)
{
	return TintTo::create(fDuration,m_tColor.r,m_tColor.g,m_tColor.b);
}


}