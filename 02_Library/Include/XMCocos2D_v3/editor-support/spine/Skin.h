/* -----------------------------------------------------------------------------------
 *
 *      File            Skin.h
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

#ifndef __Skin_h__
#define __Skin_h__

#include "Attachment.h"

namespace spine {

struct Skeleton;

typedef struct 
{
	const KDchar* const		name;
} Skin;

Skin*			Skin_create  ( const KDchar* pName );
KDvoid			Skin_dispose ( Skin* pSelf);

/* The Skin owns the attachment. */
KDvoid			Skin_addAttachment ( Skin* pSelf, KDint nSlotIndex, const KDchar* pName, Attachment* pAttachment );
/* Returns 0 if the attachment was not found. */
Attachment*		Skin_getAttachment ( const Skin* pSelf, KDint nSlotIndex, const KDchar* pName );

/* Returns 0 if the slot or attachment was not found. */
const KDchar*	Skin_getAttachmentName ( const Skin* pSelf, KDint nSlotIndex, KDint nAttachmentIndex );

/** Attach each attachment in this skin if the corresponding attachment in oldSkin is currently attached. */
KDvoid			Skin_attachAll ( const Skin* pSelf, struct Skeleton* pSkeleton, const Skin* pOldSkin );

} // namespace spine {

#endif	// __Skin_h__
