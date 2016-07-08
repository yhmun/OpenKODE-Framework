/* -----------------------------------------------------------------------------------
 *
 *      File            Animation.h
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

#ifndef __Animation_h__
#define __Animation_h__

#include "../../ccMacros.h"

NS_CC_BEGIN


typedef struct Timeline		Timeline;

struct Skeleton;

typedef struct 
{
	const KDchar* const		name;
	KDfloat					duration;
	KDint					timelineCount;
	Timeline**				timelines;
} Animation;

Animation*	Animation_create	( const KDchar* name, KDint timelineCount );
KDvoid		Animation_dispose	( Animation* self);

KDvoid		Animation_apply		( const Animation* self, struct Skeleton* skeleton, KDfloat time, KDint loop );
KDvoid		Animation_mix		( const Animation* self, struct Skeleton* skeleton, KDfloat time, KDint loop, KDfloat alpha );



struct Timeline 
{
	const KDvoid* const		vtable;
};

KDvoid		Timeline_dispose	( Timeline* self );
KDvoid		Timeline_apply		( const Timeline* self, struct Skeleton* skeleton, KDfloat time, KDfloat alpha );



typedef struct 
{
	Timeline				super;
	KDfloat*				curves;		// dfx, dfy, ddfx, ddfy, dddfx, dddfy, ... 
} CurveTimeline;

KDvoid		CurveTimeline_setLinear  ( CurveTimeline* self, KDint frameIndex );
KDvoid		CurveTimeline_setStepped ( CurveTimeline* self, KDint frameIndex );

/**
 *	Sets the control handle positions for an interpolation bezier curve used to transition from this keyframe to the next.
 *	cx1 and cx2 are from 0 to 1, representing the percent of time between the two keyframes. cy1 and cy2 are the percent of
 *	the difference between the keyframe's values. 
 */
KDvoid		CurveTimeline_setCurve			( CurveTimeline* self, KDint frameIndex, KDfloat cx1, KDfloat cy1, KDfloat cx2, KDfloat cy2 );
KDfloat		CurveTimeline_getCurvePercent	( const CurveTimeline* self, KDint frameIndex, KDfloat percent );



typedef struct BaseTimeline 
{
	CurveTimeline			super;
	KDint const				framesLength;
	KDfloat* const			frames;		// time, angle, ... for rotate. time, x, y, ... for translate and scale. 
	KDint					boneIndex;
} RotateTimeline;

RotateTimeline*		RotateTimeline_create	( KDint frameCount );

KDvoid				RotateTimeline_setFrame ( RotateTimeline* self, KDint frameIndex, KDfloat time, KDfloat angle );





typedef struct BaseTimeline		TranslateTimeline;

TranslateTimeline*	TranslateTimeline_create	( KDint frameCount );

KDvoid				TranslateTimeline_setFrame	( TranslateTimeline* self, KDint frameIndex, KDfloat time, KDfloat x, KDfloat y );




typedef struct BaseTimeline		ScaleTimeline;

ScaleTimeline*	ScaleTimeline_create	( KDint frameCount );

KDvoid			ScaleTimeline_setFrame	( ScaleTimeline* self, KDint frameIndex, KDfloat time, KDfloat x, KDfloat y );





typedef struct 
{
	CurveTimeline		super;
	KDint const			framesLength;
	KDfloat* const		frames;			// time, r, g, b, a, ... 
	KDint				slotIndex;
} ColorTimeline;

ColorTimeline*	ColorTimeline_create	( KDint frameCount );

KDvoid			ColorTimeline_setFrame	( ColorTimeline* self, KDint frameIndex, KDfloat time, KDfloat r, KDfloat g, KDfloat b, KDfloat a );





typedef struct
{
	Timeline			super;
	KDint const			framesLength;
	KDfloat* const		frames;			// time, ... 
	KDint				slotIndex;
	const KDchar** const attachmentNames;
} AttachmentTimeline;

AttachmentTimeline*		AttachmentTimeline_create	( KDint frameCount );

/* @param attachmentName May be 0. */
KDvoid					AttachmentTimeline_setFrame ( AttachmentTimeline* self, KDint frameIndex, KDfloat time, const KDchar* attachmentName );


NS_CC_END

#endif	// __Animation_h__
