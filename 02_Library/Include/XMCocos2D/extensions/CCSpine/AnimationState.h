/* -----------------------------------------------------------------------------------
 *
 *      File            AnimationState.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
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
#include "../../ccMacros.h"

NS_CC_BEGIN


typedef struct 
{
	AnimationStateData* const	data;
	Animation* const			animation;
	KDfloat						time;
	KDint						loop;
} AnimationState;

/* @param data May be 0 for no mixing. */
AnimationState*		AnimationState_create				( AnimationStateData* data );
KDvoid				AnimationState_dispose				( AnimationState* self );

KDvoid				AnimationState_update				( AnimationState* self, KDfloat delta );

KDvoid				AnimationState_apply				( AnimationState* self, struct Skeleton* skeleton );

/* @param animationName May be 0. */
KDvoid				AnimationState_setAnimationByName	( AnimationState* self, const KDchar* animationName, KDint loop );
/* @param animation May be 0. */
KDvoid				AnimationState_setAnimation			( AnimationState* self, Animation* animation, KDint loop );
KDvoid				AnimationState_clearAnimation		( AnimationState* self );

KDint				AnimationState_isComplete			( AnimationState* self );


NS_CC_END

#endif	// __AnimationState_h__