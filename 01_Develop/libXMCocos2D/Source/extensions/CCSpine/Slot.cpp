/* -----------------------------------------------------------------------------------
 *
 *      File            Slot.cpp
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
 *     ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCSpine/Slot.h"
#include "extensions/CCSpine/Extension.h"
#include "extensions/CCSpine/Skeleton.h"

NS_CC_BEGIN


typedef struct 
{
	Slot		super;
	KDfloat		attachmentTime;
} _Internal;

Slot* Slot_create ( SlotData* data, Skeleton* skeleton, Bone* bone )
{
	Slot*  self = SUPER ( NEW ( _Internal ) );
	CONST_CAST ( SlotData*, self->data ) = data;
	CONST_CAST ( Skeleton*, self->skeleton ) = skeleton;
	CONST_CAST ( Bone*, self->bone ) = bone;
	Slot_setToBindPose ( self );
	return self;
}

KDvoid Slot_dispose ( Slot* self ) 
{
	FREE ( self );
}

KDvoid Slot_setAttachment ( Slot* self, Attachment* attachment ) 
{
	CONST_CAST ( Attachment*, self->attachment ) = attachment;
	SUB_CAST ( _Internal, self )->attachmentTime = self->skeleton->time;
}

KDvoid Slot_setAttachmentTime ( Slot* self, KDfloat time )
{
	SUB_CAST ( _Internal, self )->attachmentTime = self->skeleton->time - time;
}

KDfloat Slot_getAttachmentTime ( const Slot* self ) 
{
	return self->skeleton->time - SUB_CAST ( _Internal, self )->attachmentTime;
}

KDvoid Slot_setToBindPose ( Slot* self ) 
{
	self->r = self->data->r;
	self->g = self->data->g;
	self->b = self->data->b;
	self->a = self->data->a;

	Attachment* attachment = 0;
	if ( self->data->attachmentName )
	{
		// Find slot index. 		
		for ( KDint i = 0; i < self->skeleton->data->slotCount; ++i )
		{
			if ( self->data == self->skeleton->data->slots[i] )
			{
				attachment = Skeleton_getAttachmentForSlotIndex ( self->skeleton, i, self->data->attachmentName );
				break;
			}
		}
	}
	Slot_setAttachment ( self, attachment );
}


NS_CC_END