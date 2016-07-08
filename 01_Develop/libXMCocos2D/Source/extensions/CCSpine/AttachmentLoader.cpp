/* -----------------------------------------------------------------------------------
 *
 *      File            AttachmentLoader.cpp
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
#include "extensions/CCSpine/AttachmentLoader.h"
#include "extensions/CCSpine/Extension.h"

NS_CC_BEGIN


typedef struct _AttachmentLoaderVtable 
{
	Attachment* ( *newAttachment )	( AttachmentLoader* self, Skin* skin, AttachmentType type, const KDchar* name );
	KDvoid		( *dispose )		( AttachmentLoader* self );
} _AttachmentLoaderVtable;

KDvoid _AttachmentLoader_init ( AttachmentLoader* self, KDvoid ( *dispose ) ( AttachmentLoader* self ), Attachment* ( *newAttachment ) ( AttachmentLoader* self, Skin* skin, AttachmentType type, const KDchar* name ) )
{
	CONST_CAST ( _AttachmentLoaderVtable*, self->vtable ) = NEW ( _AttachmentLoaderVtable );
	VTABLE ( AttachmentLoader, self )->dispose = dispose;
	VTABLE ( AttachmentLoader, self )->newAttachment = newAttachment;
}

KDvoid _AttachmentLoader_deinit ( AttachmentLoader* self ) 
{
	FREE ( self->vtable );
	FREE ( self->error1 );
	FREE ( self->error2 );
}

KDvoid AttachmentLoader_dispose ( AttachmentLoader* self )
{
	VTABLE ( AttachmentLoader, self )->dispose ( self );
}

Attachment* AttachmentLoader_newAttachment ( AttachmentLoader* self, Skin* skin, AttachmentType type, const KDchar* name )
{
	FREE ( self->error1 );
	FREE ( self->error2 );
	self->error1 = 0;
	self->error2 = 0;
	return VTABLE ( AttachmentLoader, self )->newAttachment ( self, skin, type, name );
}

KDvoid _AttachmentLoader_setError ( AttachmentLoader* self, const KDchar* error1, const KDchar* error2 ) 
{
	FREE ( self->error1 );
	FREE ( self->error2 );
	MALLOC_STR ( self->error1, error1 );
	MALLOC_STR ( self->error2, error2 );
}

KDvoid _AttachmentLoader_setUnknownTypeError ( AttachmentLoader* self, AttachmentType type )
{
	KDchar	buffer[16];
	kdSprintf ( buffer, "%d", type );
	_AttachmentLoader_setError ( self, "Unknown attachment type: ", buffer );
}


NS_CC_END
