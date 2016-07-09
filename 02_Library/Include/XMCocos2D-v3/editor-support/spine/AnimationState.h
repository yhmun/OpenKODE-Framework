/* -----------------------------------------------------------------------------------
 *
 *      File            AnimationState.h
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

#ifndef __AnimationState_h__
#define __AnimationState_h__

#include "AnimationStateData.h"

namespace spine {

typedef struct 
{
	AnimationStateData* const	data;
	Animation* const			animation;
	KDfloat						time;
	KDint						loop;
} AnimationState;

/* @param data May be 0 for no mixing. */
AnimationState* AnimationState_create  ( AnimationStateData* pData );
KDvoid			AnimationState_dispose ( AnimationState* pSelf);

KDvoid			AnimationState_update ( AnimationState* pSelf, KDfloat fDelta );

KDvoid			AnimationState_apply ( AnimationState* pSelf, struct Skeleton* pSkeleton );

/* @param animationName May be 0. */
KDvoid			AnimationState_setAnimationByName ( AnimationState* pSelf, const KDchar* pAnimationName, KDint nLoop );
/* @param animation May be 0. */
KDvoid			AnimationState_setAnimation ( AnimationState* pSelf, Animation* pAnimation, KDint nLoop );

/** @param animationName May be 0.
 * @param delay May be <= 0 to use duration of previous animation minus any mix duration plus the negative delay. */
KDvoid			AnimationState_addAnimationByName ( AnimationState* pSelf, const KDchar* pAnimationName, KDint nLoop, KDfloat fDelay );
/** @param animation May be 0.
 * @param delay May be <= 0 to use duration of previous animation minus any mix duration plus the negative delay. */
KDvoid			AnimationState_addAnimation ( AnimationState* pSelf, Animation* pAnimation, KDint nLoop, KDfloat fDelay );

KDvoid			AnimationState_clearAnimation ( AnimationState* pSelf );

KDint			AnimationState_isComplete ( AnimationState* pSelf );

} // namespace spine {

#endif	// __AnimationState_h__
