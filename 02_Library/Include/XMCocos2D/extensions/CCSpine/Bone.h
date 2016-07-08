/* -----------------------------------------------------------------------------------
 *
 *      File            Bone.h
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

#ifndef __Bone_h__
#define __Bone_h__

#include "BoneData.h"
#include "../../ccMacros.h"

NS_CC_BEGIN


typedef struct Bone		Bone;

struct Bone 
{
	BoneData* const		data;
	Bone* const			parent;
	KDfloat				x, y;
	KDfloat				rotation;
	KDfloat				scaleX, scaleY;

	KDfloat const		m00, m01, worldX;	// a b x 
	KDfloat const		m10, m11, worldY;	// c d y 
	KDfloat const		worldRotation;
	KDfloat const		worldScaleX, worldScaleY;
};

KDvoid		Bone_setYDown				( KDint yDown );

/* @param parent May be 0. */
Bone*		Bone_create					( BoneData* data, Bone* parent );
KDvoid		Bone_dispose				( Bone* self);

KDvoid		Bone_setToBindPose			( Bone* self );

KDvoid		Bone_updateWorldTransform	( Bone* self, KDint flipX, KDint flipY );


NS_CC_END

#endif	// __Bone_h__
