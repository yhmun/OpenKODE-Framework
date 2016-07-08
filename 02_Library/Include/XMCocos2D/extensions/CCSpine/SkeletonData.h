/* -----------------------------------------------------------------------------------
 *
 *      File            SkeletonData.h
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

#ifndef __SkeletonData_h__
#define __SkeletonData_h__

#include "BoneData.h"
#include "SlotData.h"
#include "Skin.h"
#include "Animation.h"
#include "../../ccMacros.h"

NS_CC_BEGIN


typedef struct 
{
	KDint			boneCount;
	BoneData**		bones;

	KDint			slotCount;
	SlotData**		slots;

	KDint			skinCount;
	Skin**			skins;
	Skin*			defaultSkin;

	KDint			animationCount;
	Animation**		animations;
} SkeletonData;

SkeletonData*	SkeletonData_create			( KDvoid );
KDvoid			SkeletonData_dispose		( SkeletonData* self );

BoneData*		SkeletonData_findBone		( const SkeletonData* self, const KDchar* boneName );
KDint			SkeletonData_findBoneIndex	( const SkeletonData* self, const KDchar* boneName );

SlotData*		SkeletonData_findSlot		( const SkeletonData* self, const KDchar* slotName );
KDint			SkeletonData_findSlotIndex	( const SkeletonData* self, const KDchar* slotName );

Skin*			SkeletonData_findSkin		( const SkeletonData* self, const KDchar* skinName );

Animation*		SkeletonData_findAnimation	( const SkeletonData* self, const KDchar* animationName );


NS_CC_END

#endif	// __SkeletonData_h__
