/* -----------------------------------------------------------------------------------
 *
 *      File            Bone.cpp
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
#include "extensions/CCSpine/Bone.h"
#include "extensions/CCSpine/Extension.h"

NS_CC_BEGIN


static KDint	yDown;

KDvoid Bone_setYDown ( KDint value ) 
{
	yDown = value;
}

Bone* Bone_create ( BoneData* data, Bone* parent ) 
{
	Bone*	self = NEW ( Bone );
	CONST_CAST ( BoneData*, self->data ) = data;
	CONST_CAST ( Bone*, self->parent ) = parent;
	Bone_setToBindPose ( self );
	return self;
}

KDvoid Bone_dispose ( Bone* self ) 
{
	FREE ( self );
}

KDvoid Bone_setToBindPose ( Bone* self )
{
	self->x = self->data->x;
	self->y = self->data->y;
	self->rotation = self->data->rotation;
	self->scaleX = self->data->scaleX;
	self->scaleY = self->data->scaleY;
}

KDvoid Bone_updateWorldTransform ( Bone* self, KDint flipX, KDint flipY ) 
{
	if ( self->parent )
	{
		CONST_CAST ( KDfloat, self->worldX ) = self->x * self->parent->m00 + self->y * self->parent->m01 + self->parent->worldX;
		CONST_CAST ( KDfloat, self->worldY ) = self->x * self->parent->m10 + self->y * self->parent->m11 + self->parent->worldY;
		CONST_CAST ( KDfloat, self->worldScaleX ) = self->parent->worldScaleX * self->scaleX;
		CONST_CAST ( KDfloat, self->worldScaleY ) = self->parent->worldScaleY * self->scaleY;
		CONST_CAST ( KDfloat, self->worldRotation ) = self->parent->worldRotation + self->rotation;
	} 
	else 
	{
		CONST_CAST ( KDfloat, self->worldX ) = self->x;
		CONST_CAST ( KDfloat, self->worldY ) = self->y;
		CONST_CAST ( KDfloat, self->worldScaleX ) = self->scaleX;
		CONST_CAST ( KDfloat, self->worldScaleY ) = self->scaleY;
		CONST_CAST ( KDfloat, self->worldRotation ) = self->rotation;
	}
	KDfloat		radians = (KDfloat) ( self->worldRotation * 3.1415926535897932385 / 180 );
	KDfloat		cosine	= kdCosf ( radians );
	KDfloat		sine	= kdSinf ( radians );
	CONST_CAST ( KDfloat, self->m00 ) = cosine * self->worldScaleX;
	CONST_CAST ( KDfloat, self->m10 ) =   sine * self->worldScaleX;
	CONST_CAST ( KDfloat, self->m01 ) =  -sine * self->worldScaleY;
	CONST_CAST ( KDfloat, self->m11 ) = cosine * self->worldScaleY;
	if ( flipX ) 
	{
		CONST_CAST ( KDfloat, self->m00 ) = -self->m00;
		CONST_CAST ( KDfloat, self->m01 ) = -self->m01;
	}
	if ( flipY ) 
	{
		CONST_CAST ( KDfloat, self->m10 ) = -self->m10;
		CONST_CAST ( KDfloat, self->m11 ) = -self->m11;
	}
	if ( yDown ) 
	{
		CONST_CAST ( KDfloat, self->m10 ) = -self->m10;
		CONST_CAST ( KDfloat, self->m11 ) = -self->m11;
	}
}


NS_CC_END