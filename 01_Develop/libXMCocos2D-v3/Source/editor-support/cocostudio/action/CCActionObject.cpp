/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionObject.cpp
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

#include "editor-support/cocostudio/action/CCActionObject.h"
#include "editor-support/cocostudio/json/DictionaryHelper.h"
#include "2d/CCDirector.h"
#include "2d/CCScheduler.h"

using namespace cocos2d;

namespace cocostudio {

ActionObject::ActionObject()
: m_pActionNodeList(NULL)
, m_sName("")
, m_bLoop(false)
, m_bPause(false)
, m_bPlaying(false)
, m_fUnitTime(0.1f)
, m_fCurrentTime(0.0f)
, m_pScheduler(NULL)
{
	m_pActionNodeList = Array::create();
	m_pActionNodeList->retain();
	m_pScheduler = Director::getInstance()->getScheduler();
	CC_SAFE_RETAIN(m_pScheduler);
}

ActionObject::~ActionObject()
{
	m_pActionNodeList->removeAllObjects();
	m_pActionNodeList->release();
	CC_SAFE_RELEASE(m_pScheduler);
}

void ActionObject::setName(const char* name)
{
	m_sName.assign(name);
}
const char* ActionObject::getName()
{
	return m_sName.c_str();
}

void ActionObject::setLoop(bool bLoop)
{
	m_bLoop = bLoop;
}
bool ActionObject::getLoop()
{
	return m_bLoop;
}

void ActionObject::setUnitTime(float fTime)
{
	m_fUnitTime = fTime;
	int nodeNum = m_pActionNodeList->count();
	for ( int i = 0; i < nodeNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)m_pActionNodeList->getObjectAtIndex(i);
		actionNode->setUnitTime(m_fUnitTime);
	}
}
float ActionObject::getUnitTime()
{
	return m_fUnitTime;
}

float ActionObject::getCurrentTime()
{
	return m_fCurrentTime;
}

void ActionObject::setCurrentTime(float fTime)
{
	m_fCurrentTime = fTime;
}

bool ActionObject::isPlaying()
{
	return m_bPlaying;
}

void ActionObject::initWithDictionary(JsonDictionary *dic,Object* root)
{
    setName(DICTOOL->getStringValue_json(dic, "name"));
    setLoop(DICTOOL->getBooleanValue_json(dic, "loop"));
	setUnitTime(DICTOOL->getFloatValue_json(dic, "unittime"));
    int actionNodeCount = DICTOOL->getArrayCount_json(dic, "actionnodelist");
    for (int i=0; i<actionNodeCount; i++) {
        ActionNode* actionNode = new ActionNode();
		actionNode->autorelease();
        JsonDictionary* actionNodeDic = DICTOOL->getDictionaryFromArray_json(dic, "actionnodelist", i);
        actionNode->initWithDictionary(actionNodeDic,root);
		actionNode->setUnitTime(getUnitTime());
        m_pActionNodeList->addObject(actionNode);
		CC_SAFE_DELETE(actionNodeDic);
    }
}

void ActionObject::addActionNode(ActionNode* node)
{
	if (node == NULL)
	{
		return;
	}
	m_pActionNodeList->addObject(node);
	node->setUnitTime(m_fUnitTime);
}
void ActionObject::removeActionNode(ActionNode* node)
{
	if (node == NULL)
	{
		return;
	}
	m_pActionNodeList->removeObject(node);
}

void ActionObject::play()
{
    stop();
	this->updateToFrameByTime(0.0f);
	int frameNum = m_pActionNodeList->count();
	for ( int i = 0; i < frameNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)m_pActionNodeList->getObjectAtIndex(i);
		actionNode->playAction();
	}
	if (m_bLoop)
	{
		m_pScheduler->scheduleSelector(schedule_selector(ActionObject::simulationActionUpdate), this, 0.0f , kRepeatForever, 0.0f, false);
	}
}

void ActionObject::pause()
{
	m_bPause = true;
}

void ActionObject::stop()
{
	int frameNum = m_pActionNodeList->count();

	for ( int i = 0; i < frameNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)m_pActionNodeList->getObjectAtIndex(i);
		actionNode->stopAction();
	}

	m_pScheduler->unscheduleSelector(schedule_selector(ActionObject::simulationActionUpdate), this);
	m_bPause = false;
}

void ActionObject::updateToFrameByTime(float fTime)
{
	m_fCurrentTime = fTime;

	int nodeNum = m_pActionNodeList->count();

	for ( int i = 0; i < nodeNum; i++ )
	{
		ActionNode* actionNode = (ActionNode*)m_pActionNodeList->getObjectAtIndex(i);

		actionNode->updateActionToTimeLine(fTime);
	}
}

void ActionObject::simulationActionUpdate(float dt)
{
	if (m_bLoop)
	{
		bool isEnd = true;
		int nodeNum = m_pActionNodeList->count();

		for ( int i = 0; i < nodeNum; i++ )
		{
			ActionNode* actionNode = (ActionNode*)m_pActionNodeList->getObjectAtIndex(i);

			if (actionNode->isActionDoneOnce() == false)
			{
				isEnd = false;
				break;
			}
		}

		if (isEnd)
		{
			this->play();
		}

		//CCLOG("ActionObject Update");
	}
}
}