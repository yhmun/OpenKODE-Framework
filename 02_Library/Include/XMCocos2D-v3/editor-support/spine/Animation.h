/* -----------------------------------------------------------------------------------
 *
 *      File            Animation.h
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __Animation_h__
#define __Animation_h__

#include <kode.h>

namespace spine {

typedef struct Timeline		Timeline;
struct Skeleton;

typedef struct 
{
	const KDchar* const		name;
	KDfloat					duration;

	KDint					timelineCount;
	Timeline**				timelines;
} Animation;

Animation*			Animation_create  ( const KDchar* pName, KDint nTimelineCount );
KDvoid				Animation_dispose ( Animation* pSelf);

KDvoid				Animation_apply ( const Animation* pSelf, struct Skeleton* pSkeleton, KDfloat fTime, KDint nLoop );
KDvoid				Animation_mix   ( const Animation* pSelf, struct Skeleton* pSkeleton, KDfloat fTime, KDint nLoop, KDfloat fAlpha );

/**/

struct Timeline 
{
	const KDvoid* const		vtable;
};

KDvoid				Timeline_dispose ( Timeline* pSelf );
KDvoid				Timeline_apply   ( const Timeline* pSelf, struct Skeleton* pSkeleton, KDfloat fTime, KDfloat fAlpha );

/**/

typedef struct
{
	Timeline				super;
	KDfloat*				curves; /* dfx, dfy, ddfx, ddfy, dddfx, dddfy, ... */
} CurveTimeline;

KDvoid				CurveTimeline_setLinear  ( CurveTimeline* pSelf, KDint nFrameIndex );
KDvoid				CurveTimeline_setStepped ( CurveTimeline* pSelf, KDint nFrameIndex );

/* Sets the control handle positions for an interpolation bezier curve used to transition from this keyframe to the next.
 * cx1 and cx2 are from 0 to 1, representing the percent of time between the two keyframes. cy1 and cy2 are the percent of
 * the difference between the keyframe's values. */
KDvoid				CurveTimeline_setCurve ( CurveTimeline* pSelf, KDint nFrameIndex, KDfloat cx1, KDfloat cy1, KDfloat cx2, KDfloat cy2 );
KDfloat				CurveTimeline_getCurvePercent ( const CurveTimeline* pSelf, KDint nFrameIndex, KDfloat fPercent );

/**/

typedef struct BaseTimeline 
{
	CurveTimeline			super;
	KDint const				framesLength;
	KDfloat* const			frames;				/* time, angle, ... for rotate. time, x, y, ... for translate and scale. */
	KDint					boneIndex;
} RotateTimeline;

RotateTimeline*		RotateTimeline_create ( KDint nFrameCount );

KDvoid				RotateTimeline_setFrame ( RotateTimeline* pSelf, KDint nFrameIndex, KDfloat fTime, KDfloat fAngle );

/**/

typedef struct BaseTimeline		TranslateTimeline;

TranslateTimeline*	TranslateTimeline_create ( KDint nFrameCount );

KDvoid				TranslateTimeline_setFrame ( TranslateTimeline* pSelf, KDint nFrameIndex, KDfloat fTime, KDfloat x, KDfloat y );

/**/

typedef struct BaseTimeline		ScaleTimeline;

ScaleTimeline*		ScaleTimeline_create ( KDint nFrameCount );

KDvoid				ScaleTimeline_setFrame ( ScaleTimeline* pSelf, KDint nFrameIndex, KDfloat fTime, KDfloat x, KDfloat y );

/**/

typedef struct 
{
	CurveTimeline			super;
	KDint const				framesLength;
	KDfloat* const			frames;				/* time, r, g, b, a, ... */
	KDint					slotIndex;
} ColorTimeline;

ColorTimeline*		ColorTimeline_create ( KDint nFrameCount );

KDvoid				ColorTimeline_setFrame (ColorTimeline* pSelf, KDint nFrameIndex, KDfloat fTime, KDfloat r, KDfloat g, KDfloat b, KDfloat a );

/**/

typedef struct 
{
	Timeline				super;
	KDint const				framesLength;
	KDfloat* const			frames; /* time, ... */
	KDint					slotIndex;
	const KDchar** const	attachmentNames;
} AttachmentTimeline;

AttachmentTimeline*	AttachmentTimeline_create ( KDint nFrameCount );

/* @param attachmentName May be 0. */
KDvoid				AttachmentTimeline_setFrame ( AttachmentTimeline* pSelf, KDint nFrameIndex, KDfloat fTime, const KDchar* pAttachmentName );

}

#endif	// __Animation_h__
