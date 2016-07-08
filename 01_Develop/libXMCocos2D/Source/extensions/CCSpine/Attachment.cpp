/* -----------------------------------------------------------------------------------
 *
 *      File            Attachment.cpp
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

#include "Precompiled.h"
#include "extensions/CCSpine/Attachment.h"
#include "extensions/CCSpine/Extension.h"
#include "extensions/CCSpine/Slot.h"

NS_CC_BEGIN


typedef struct _AttachmentVtable
{
	KDvoid ( *dispose ) ( Attachment* self );
} _AttachmentVtable;

KDvoid _Attachment_init ( Attachment* self, const KDchar* name, AttachmentType type, KDvoid ( *dispose ) ( Attachment* self ) ) 
{
	CONST_CAST ( _AttachmentVtable*, self->vtable ) = NEW ( _AttachmentVtable );
	VTABLE ( Attachment, self )->dispose = dispose;

	MALLOC_STR ( self->name, name );
	self->type = type;
}

KDvoid _Attachment_deinit ( Attachment* self ) 
{
	FREE ( self->vtable );
	FREE ( self->name );
}

KDvoid Attachment_dispose ( Attachment* self )
{
	VTABLE ( Attachment, self )->dispose ( self );
}


NS_CC_END