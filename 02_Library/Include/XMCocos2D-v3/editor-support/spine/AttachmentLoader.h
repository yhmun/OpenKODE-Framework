/* -----------------------------------------------------------------------------------
 *
 *      File            AttachmentLoader.h
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

#ifndef __AttachmentLoader_h__
#define __AttachmentLoader_h__

#include "Attachment.h"
#include "Skin.h"

namespace spine {

typedef struct AttachmentLoader AttachmentLoader;
struct AttachmentLoader 
{
	const KDchar*			error1;
	const KDchar*			error2;

	const KDvoid* const		vtable;
#ifdef __cplusplus
	AttachmentLoader ( KDvoid ) : error1 ( 0 ), error2 ( 0 ), vtable ( 0 ) { }
#endif
};

KDvoid			AttachmentLoader_dispose ( AttachmentLoader* pSelf );

/* Returns 0 to not load an attachment. If 0 is returned and AttachmentLoader.error1 is set, an error occurred. */
Attachment*		AttachmentLoader_newAttachment ( AttachmentLoader* pSelf, Skin* pSkin, AttachmentType eType, const KDchar* pName );

} // namespace spine {

#endif	// __AttachmentLoader_h__
