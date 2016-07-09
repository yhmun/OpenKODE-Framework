/* -----------------------------------------------------------------------------------
 *
 *      File            Slot.h
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

#ifndef __Slot_h__
#define __Slot_h__

#include "Bone.h"
#include "Attachment.h"
#include "SlotData.h"

namespace spine {

struct Skeleton;

typedef struct Slot
{
	SlotData* const				data;
	struct Skeleton* const		skeleton;
	Bone* const					bone;
	KDfloat						r, g, b, a;
	Attachment* const			attachment;
} Slot;

Slot*		Slot_create  ( SlotData* pData, struct Skeleton* pSkeleton, Bone* pBone );
KDvoid		Slot_dispose ( Slot* pSelf );

/* @param attachment May be 0 to clear the attachment for the slot. */
KDvoid		Slot_setAttachment ( Slot* pSelf, Attachment* pAttachment );

KDvoid		Slot_setAttachmentTime ( Slot* pSelf, KDfloat fTime );
KDfloat		Slot_getAttachmentTime ( const Slot* pSelf );

KDvoid		Slot_setToSetupPose ( Slot* pSelf );

} // namespace spine {

#endif	// __Slot_h__
