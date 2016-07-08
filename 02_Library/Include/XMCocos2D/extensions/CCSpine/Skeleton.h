/* -----------------------------------------------------------------------------------
 *
 *      File            Skeleton.h
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

#ifndef __Skeleton_h__
#define __Skeleton_h__

#include "SkeletonData.h"
#include "Slot.h"
#include "Skin.h"
#include "../../ccMacros.h"

NS_CC_BEGIN


typedef struct Skeleton		Skeleton;

struct Skeleton 
{
	SkeletonData* const		data;

	KDint					boneCount;
	Bone**					bones;
	Bone* const				root;

	KDint					slotCount;
	Slot**					slots;
	Slot**					drawOrder;

	Skin* const				skin;
	KDfloat					r, g, b, a;
	KDfloat					time;
	KDint					flipX;
	KDint					flipY;
};

Skeleton*	Skeleton_create						( SkeletonData* data );
KDvoid		Skeleton_dispose					( Skeleton* self );

KDvoid		Skeleton_updateWorldTransform		( const Skeleton* self );

KDvoid		Skeleton_setToBindPose				( const Skeleton* self );
KDvoid		Skeleton_setBonesToBindPose			( const Skeleton* self );
KDvoid		Skeleton_setSlotsToBindPose			( const Skeleton* self );

/* Returns 0 if the bone was not found. */
Bone*		Skeleton_findBone					( const Skeleton* self, const KDchar* boneName );
/* Returns -1 if the bone was not found. */
KDint		Skeleton_findBoneIndex				( const Skeleton* self, const KDchar* boneName );

/* Returns 0 if the slot was not found. */
Slot*		Skeleton_findSlot					( const Skeleton* self, const KDchar* slotName );
/* Returns -1 if the slot was not found. */
KDint		Skeleton_findSlotIndex				( const Skeleton* self, const KDchar* slotName );

/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
 * attached if the corresponding attachment from the old skin was attached.
 * @param skin May be 0.*/
KDvoid		Skeleton_setSkin					( Skeleton* self, Skin* skin );

/* Returns 0 if the skin was not found. See Skeleton_setSkin.
 * @param skinName May be 0. */
KDint		Skeleton_setSkinByName				( Skeleton* self, const KDchar* skinName );

/* Returns 0 if the slot or attachment was not found. */
Attachment* Skeleton_getAttachmentForSlotName	( const Skeleton* self, const KDchar* slotName, const KDchar* attachmentName );
/* Returns 0 if the slot or attachment was not found. */
Attachment* Skeleton_getAttachmentForSlotIndex	( const Skeleton* self, KDint slotIndex, const KDchar* attachmentName );
/* Returns 0 if the slot or attachment was not found. */
KDint		Skeleton_setAttachment				( Skeleton* self, const KDchar* slotName, const KDchar* attachmentName );

KDvoid		Skeleton_update						( Skeleton* self, KDfloat deltaTime );


NS_CC_END

#endif	// __Skeleton_h__