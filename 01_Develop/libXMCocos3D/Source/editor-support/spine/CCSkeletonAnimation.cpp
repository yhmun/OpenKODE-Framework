/* -----------------------------------------------------------------------------------
 *
 *      File            CCSkeletonAnimation.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      Esoteric Software. All rights reserved.
 *
 *         http://esotericsoftware.com/      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "editor-support/spine/CCSkeletonAnimation.h"
#include "editor-support/spine/extension.h"
#include "editor-support/spine/spine-cocos2dx.h"
#include <algorithm>

USING_NS_CC;
using std::min;
using std::max;
using std::vector;

namespace spine {

CCSkeletonAnimation* CCSkeletonAnimation::createWithData (SkeletonData* skeletonData) {
	CCSkeletonAnimation* node = new CCSkeletonAnimation(skeletonData);
	node->autorelease();
	return node;
}

CCSkeletonAnimation* CCSkeletonAnimation::createWithFile (const char* skeletonDataFile, Atlas* atlas, float scale) {
	CCSkeletonAnimation* node = new CCSkeletonAnimation(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

CCSkeletonAnimation* CCSkeletonAnimation::createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale) {
	CCSkeletonAnimation* node = new CCSkeletonAnimation(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

CCSkeletonAnimation::CCSkeletonAnimation (SkeletonData *skeletonData)
		: CCSkeleton(skeletonData) {
	addAnimationState();
}

CCSkeletonAnimation::CCSkeletonAnimation (const char* skeletonDataFile, Atlas* atlas, float scale)
		: CCSkeleton(skeletonDataFile, atlas, scale) {
	addAnimationState();
}

CCSkeletonAnimation::CCSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale)
		: CCSkeleton(skeletonDataFile, atlasFile, scale) {
	addAnimationState();
}

CCSkeletonAnimation::~CCSkeletonAnimation () {
	for (std::vector<AnimationStateData*>::iterator iter = m_aStateDatas.begin(); iter != m_aStateDatas.end(); ++iter)
		AnimationStateData_dispose(*iter);

	for (std::vector<AnimationState*>::iterator iter = m_aStates.begin(); iter != m_aStates.end(); ++iter)
		AnimationState_dispose(*iter);
}

void CCSkeletonAnimation::update (float deltaTime) {
	super::update(deltaTime);

	deltaTime *= m_fTimeScale;
	for (std::vector<AnimationState*>::iterator iter = m_aStates.begin(); iter != m_aStates.end(); ++iter) {
		AnimationState_update(*iter, deltaTime);
		AnimationState_apply(*iter, m_pSkeleton);
	}
	Skeleton_updateWorldTransform(m_pSkeleton);
}

void CCSkeletonAnimation::addAnimationState (AnimationStateData* stateData) {
	if (!stateData) {
		stateData = AnimationStateData_create(m_pSkeleton->data);
		m_aStateDatas.push_back(stateData);
	}
	AnimationState* state = AnimationState_create(stateData);
	m_aStates.push_back(state);
}

void CCSkeletonAnimation::setAnimationStateData (AnimationStateData* stateData, int stateIndex) {
	CCASSERT(stateIndex >= 0 && stateIndex < (int)m_aStates.size(), "stateIndex out of range.");
	CCASSERT(stateData, "stateData cannot be null.");

	AnimationState* state = m_aStates[stateIndex];
	for (std::vector<AnimationStateData*>::iterator iter = m_aStateDatas.begin(); iter != m_aStateDatas.end(); ++iter) {
		if (state->data == *iter) {
			AnimationStateData_dispose(state->data);
			m_aStateDatas.erase(iter);
			break;
		}
	}
	for (std::vector<AnimationState*>::iterator iter = m_aStates.begin(); iter != m_aStates.end(); ++iter) {
		if (state == *iter) {
			m_aStates.erase(iter);
			break;
		}
	}
	AnimationState_dispose(state);

	state = AnimationState_create(stateData);
	m_aStates[stateIndex] = state;
}

void CCSkeletonAnimation::setMix (const char* fromAnimation, const char* toAnimation, float duration, int stateIndex) {
	CCASSERT(stateIndex >= 0 && stateIndex < (int)m_aStates.size(), "stateIndex out of range.");
	AnimationStateData_setMixByName(m_aStates[stateIndex]->data, fromAnimation, toAnimation, duration);
}

void CCSkeletonAnimation::setAnimation (const char* name, bool loop, int stateIndex) {
	CCASSERT(stateIndex >= 0 && stateIndex < (int)m_aStates.size(), "stateIndex out of range.");
	AnimationState_setAnimationByName(m_aStates[stateIndex], name, loop);
}

void CCSkeletonAnimation::addAnimation (const char* name, bool loop, float delay, int stateIndex) {
	CCASSERT(stateIndex >= 0 && stateIndex < (int)m_aStates.size(), "stateIndex out of range.");
	AnimationState_addAnimationByName(m_aStates[stateIndex], name, loop, delay);
}

void CCSkeletonAnimation::clearAnimation (int stateIndex) {
	CCASSERT(stateIndex >= 0 && stateIndex < (int)m_aStates.size(), "stateIndex out of range.");
	AnimationState_clearAnimation(m_aStates[stateIndex]);
}

} // namespace spine {