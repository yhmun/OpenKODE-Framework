/* -----------------------------------------------------------------------------------
 *
 *      File            Animation.cpp
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
#include "extensions/CCSpine/Animation.h"
#include "extensions/CCSpine/Extension.h"

NS_CC_BEGIN


Animation* Animation_create ( const KDchar* name, KDint timelineCount ) 
{
	Animation*	 self = NEW ( Animation );
	MALLOC_STR ( self->name, name );
	self->timelineCount = timelineCount;
	self->timelines = MALLOC ( Timeline*, timelineCount );
	return self;
}

KDvoid Animation_dispose ( Animation* self ) 
{
	for ( KDint i = 0; i < self->timelineCount; ++i )
	{
		Timeline_dispose ( self->timelines[i] );
	}
	FREE ( self->timelines );
	FREE ( self );
}

KDvoid Animation_apply ( const Animation* self, Skeleton* skeleton, KDfloat time, KDint loop ) 
{
	if ( loop && self->duration ) 
	{
		time = kdFmodf ( time, self->duration );
	}

	KDint i, n = self->timelineCount;
	for ( i = 0; i < n; ++i )
	{
		Timeline_apply ( self->timelines[i], skeleton, time, 1 );
	}
}

KDvoid Animation_mix ( const Animation* self, Skeleton* skeleton, KDfloat time, KDint loop, KDfloat alpha ) 
{
	if ( loop && self->duration )
	{
		time = kdFmodf ( time, self->duration );
	}

	KDint i, n = self->timelineCount;
	for ( i = 0; i < n; ++i )
	{
		Timeline_apply ( self->timelines[i], skeleton, time, alpha );
	}
}



typedef struct _TimelineVtable
{
	KDvoid ( *apply )	( const Timeline* self, Skeleton* skeleton, KDfloat time, KDfloat alpha );
	KDvoid ( *dispose ) ( Timeline* self );
} _TimelineVtable;

KDvoid _Timeline_init ( Timeline* self, KDvoid ( *dispose ) ( Timeline* self ), KDvoid ( *apply ) ( const Timeline* self, Skeleton* skeleton, KDfloat time, KDfloat alpha ) )
{
	CONST_CAST ( _TimelineVtable*, self->vtable ) = NEW ( _TimelineVtable );
	VTABLE ( Timeline, self )->dispose = dispose;
	VTABLE ( Timeline, self )->apply = apply;
}

KDvoid _Timeline_deinit ( Timeline* self ) 
{
	FREE ( self->vtable );
}

KDvoid Timeline_dispose ( Timeline* self )
{
	VTABLE ( Timeline, self )->dispose ( self );
}

KDvoid Timeline_apply ( const Timeline* self, Skeleton* skeleton, KDfloat time, KDfloat alpha ) 
{
	VTABLE ( Timeline, self )->apply ( self, skeleton, time, alpha );
}



static const KDfloat	CURVE_LINEAR	= 0;
static const KDfloat	CURVE_STEPPED	= -1;
static const KDint		CURVE_SEGMENTS	= 10;

KDvoid _CurveTimeline_init ( CurveTimeline* self, KDint frameCount, KDvoid ( *dispose ) ( Timeline* self ), KDvoid ( *apply ) ( const Timeline* self, Skeleton* skeleton, KDfloat time, KDfloat alpha ) )
{
	_Timeline_init ( SUPER ( self ), dispose, apply );
	self->curves = CALLOC ( KDfloat, ( frameCount - 1 ) * 6 );
}

KDvoid _CurveTimeline_deinit ( CurveTimeline* self )
{
	_Timeline_deinit ( SUPER ( self ) );
	FREE ( self->curves );
}

KDvoid CurveTimeline_setLinear ( CurveTimeline* self, KDint frameIndex ) 
{
	self->curves[frameIndex * 6] = CURVE_LINEAR;
}

KDvoid CurveTimeline_setStepped ( CurveTimeline* self, KDint frameIndex )
{
	self->curves[frameIndex * 6] = CURVE_STEPPED;
}

KDvoid CurveTimeline_setCurve ( CurveTimeline* self, KDint frameIndex, KDfloat cx1, KDfloat cy1, KDfloat cx2, KDfloat cy2 ) 
{
	KDfloat		subdiv_step = 1.0f / CURVE_SEGMENTS;
	KDfloat		subdiv_step2 = subdiv_step * subdiv_step;
	KDfloat		subdiv_step3 = subdiv_step2 * subdiv_step;
	KDfloat		pre1 = 3 * subdiv_step;
	KDfloat		pre2 = 3 * subdiv_step2;
	KDfloat		pre4 = 6 * subdiv_step2;
	KDfloat		pre5 = 6 * subdiv_step3;
	KDfloat		tmp1x = -cx1 * 2 + cx2;
	KDfloat		tmp1y = -cy1 * 2 + cy2;
	KDfloat		tmp2x = ( cx1 - cx2 ) * 3 + 1;
	KDfloat		tmp2y = ( cy1 - cy2 ) * 3 + 1;
	KDint		i = frameIndex * 6;
	self->curves[i] = cx1 * pre1 + tmp1x * pre2 + tmp2x * subdiv_step3;
	self->curves[i + 1] = cy1 * pre1 + tmp1y * pre2 + tmp2y * subdiv_step3;
	self->curves[i + 2] = tmp1x * pre4 + tmp2x * pre5;
	self->curves[i + 3] = tmp1y * pre4 + tmp2y * pre5;
	self->curves[i + 4] = tmp2x * pre5;
	self->curves[i + 5] = tmp2y * pre5;
}

KDfloat CurveTimeline_getCurvePercent ( const CurveTimeline* self, KDint frameIndex, KDfloat percent ) 
{
	KDint		curveIndex = frameIndex * 6;
	KDfloat		dfx = self->curves[curveIndex];
	
	if ( dfx == CURVE_LINEAR )
	{
		return percent;
	}

	if ( dfx == CURVE_STEPPED )
	{
		return 0;
	}

	KDfloat		dfy = self->curves[curveIndex + 1];
	KDfloat		ddfx = self->curves[curveIndex + 2];
	KDfloat		ddfy = self->curves[curveIndex + 3];
	KDfloat		dddfx = self->curves[curveIndex + 4];
	KDfloat		dddfy = self->curves[curveIndex + 5];
	KDfloat		x = dfx, y = dfy;
	KDint		i = CURVE_SEGMENTS - 2;

	while ( 1 )
	{
		if ( x >= percent ) 
		{
			KDfloat		lastX = x - dfx;
			KDfloat		lastY = y - dfy;
			return lastY + ( y - lastY ) * ( percent - lastX ) / ( x - lastX );
		}
		
		if ( i == 0 ) 
		{
			break;
		}

		i--;
		dfx  += ddfx;
		dfy  += ddfy;
		ddfx += dddfx;
		ddfy += dddfy;
		x += dfx;
		y += dfy;
	}
	return y + ( 1 - y ) * ( percent - x ) / ( 1 - x );		// Last point is 1,1. 
}

// @param target After the first and before the last entry. 
static KDint binarySearch ( KDfloat *values, KDint valuesLength, KDfloat target, KDint step ) 
{
	KDint	low = 0;
	KDint	high = valuesLength / step - 2;
	if ( high == 0 ) 
	{
		return step;
	}

	KDint	current = high >> 1;
	while ( 1 ) 
	{
		if ( values[ ( current + 1 ) * step] <= target )
		{
			low = current + 1;
		}
		else
		{
			high = current;
		}

		if ( low == high )
		{
			return ( low + 1 ) * step;
		}

		current = ( low + high ) >> 1;
	}
	return 0;
}



KDvoid _BaseTimeline_dispose ( Timeline* timeline ) 
{
	struct BaseTimeline*	self = SUB_CAST ( struct BaseTimeline, timeline );
	_CurveTimeline_deinit ( SUPER ( self ) );
	FREE ( self->frames );
	FREE ( self );
}

// Many timelines have structure identical to struct BaseTimeline and extend CurveTimeline. 
struct BaseTimeline* _BaseTimeline_create ( KDint frameCount, KDint frameSize, KDvoid ( *apply ) ( const Timeline* self, Skeleton* skeleton, KDfloat time, KDfloat alpha ) ) 
{

	struct BaseTimeline* self = NEW ( struct BaseTimeline );
	_CurveTimeline_init ( SUPER ( self ), frameCount, _BaseTimeline_dispose, apply );

	CONST_CAST ( int, self->framesLength ) = frameCount * frameSize;
	CONST_CAST ( float*, self->frames ) = CALLOC ( float, self->framesLength );

	return self;
}



static const KDint	ROTATE_LAST_FRAME_TIME = -2;
static const KDint	ROTATE_FRAME_VALUE = 1;

KDvoid _RotateTimeline_apply ( const Timeline* timeline, Skeleton* skeleton, KDfloat time, KDfloat alpha )
{
	RotateTimeline*		self = SUB_CAST ( RotateTimeline, timeline );

	if ( time < self->frames[0] ) 
	{
		return; // Time is before first frame. 
	}

	Bone*	bone = skeleton->bones[self->boneIndex];

	if ( time >= self->frames[self->framesLength - 2] ) 
	{ 
		// Time is after last frame. 
		KDfloat		amount = bone->data->rotation + self->frames[self->framesLength - 1] - bone->rotation;
		while ( amount > 180 )
		{
			amount -= 360;
		}

		while ( amount < -180 )
		{
			amount += 360;
		}

		bone->rotation += amount * alpha;
		return;
	}

	// Interpolate between the last frame and the current frame. 
	KDint		frameIndex = binarySearch ( self->frames, self->framesLength, time, 2 );
	KDfloat		lastFrameValue = self->frames[frameIndex - 1];
	KDfloat		frameTime = self->frames[frameIndex];
	KDfloat		percent = 1 - ( time - frameTime ) / ( self->frames[frameIndex + ROTATE_LAST_FRAME_TIME] - frameTime );
	percent = CurveTimeline_getCurvePercent ( SUPER ( self ), frameIndex / 2 - 1, percent < 0 ? 0 : ( percent > 1 ? 1 : percent ) );

	KDfloat		amount = self->frames[frameIndex + ROTATE_FRAME_VALUE] - lastFrameValue;
	while ( amount > 180 )
	{
		amount -= 360;
	}

	while ( amount < -180 )
	{
		amount += 360;
	}

	amount = bone->data->rotation + ( lastFrameValue + amount * percent ) - bone->rotation;
	while ( amount > 180 )
	{
		amount -= 360;
	}

	while ( amount < -180 )
	{
		amount += 360;
	}
	bone->rotation += amount * alpha;
}

RotateTimeline* RotateTimeline_create ( KDint frameCount )
{
	return _BaseTimeline_create ( frameCount, 2, _RotateTimeline_apply );
}

KDvoid RotateTimeline_setFrame ( RotateTimeline* self, KDint frameIndex, KDfloat time, KDfloat angle ) 
{
	frameIndex *= 2;
	self->frames[frameIndex] = time;
	self->frames[frameIndex + 1] = angle;
}



static const KDint	TRANSLATE_LAST_FRAME_TIME = -3;
static const KDint	TRANSLATE_FRAME_X = 1;
static const KDint	TRANSLATE_FRAME_Y = 2;

KDvoid _TranslateTimeline_apply ( const Timeline* timeline, Skeleton* skeleton, KDfloat time, KDfloat alpha )
{
	TranslateTimeline*	self = SUB_CAST ( TranslateTimeline, timeline );

	if ( time < self->frames[0] ) 
	{
		return; // Time is before first frame. 
	}

	Bone*	bone = skeleton->bones[self->boneIndex];

	if ( time >= self->frames[self->framesLength - 3] ) 
	{ 
		// Time is after last frame. 
		bone->x += ( bone->data->x + self->frames[self->framesLength - 2] - bone->x ) * alpha;
		bone->y += ( bone->data->y + self->frames[self->framesLength - 1] - bone->y ) * alpha;
		return;
	}

	// Interpolate between the last frame and the current frame. 
	KDint		frameIndex = binarySearch ( self->frames, self->framesLength, time, 3 );
	KDfloat		lastFrameX = self->frames[frameIndex - 2];
	KDfloat		lastFrameY = self->frames[frameIndex - 1];
	KDfloat		frameTime  = self->frames[frameIndex];
	KDfloat		percent = 1 - ( time - frameTime ) / ( self->frames[frameIndex + TRANSLATE_LAST_FRAME_TIME] - frameTime );
	percent = CurveTimeline_getCurvePercent ( SUPER ( self ), frameIndex / 3 - 1, percent < 0 ? 0 : ( percent > 1 ? 1 : percent ) );

	bone->x += ( bone->data->x + lastFrameX + ( self->frames[frameIndex + TRANSLATE_FRAME_X] - lastFrameX ) * percent - bone->x ) * alpha;
	bone->y += ( bone->data->y + lastFrameY + ( self->frames[frameIndex + TRANSLATE_FRAME_Y] - lastFrameY ) * percent - bone->y ) * alpha;
}

TranslateTimeline* TranslateTimeline_create ( KDint frameCount ) 
{
	return _BaseTimeline_create ( frameCount, 3, _TranslateTimeline_apply );
}

KDvoid TranslateTimeline_setFrame ( TranslateTimeline* self, KDint frameIndex, KDfloat time, KDfloat x, KDfloat y ) 
{
	frameIndex *= 3;
	self->frames[frameIndex] = time;
	self->frames[frameIndex + 1] = x;
	self->frames[frameIndex + 2] = y;
}



KDvoid _ScaleTimeline_apply ( const Timeline* timeline, Skeleton* skeleton, KDfloat time, KDfloat alpha ) 
{
	ScaleTimeline*	self = SUB_CAST ( ScaleTimeline, timeline );

	if ( time < self->frames[0] ) 
	{
		return;	// Time is before first frame. 
	}

	Bone*	bone = skeleton->bones[self->boneIndex];
	if ( time >= self->frames[self->framesLength - 3] )
	{
		// Time is after last frame. 
		bone->scaleX += ( bone->data->scaleX - 1 + self->frames[self->framesLength - 2] - bone->scaleX ) * alpha;
		bone->scaleY += ( bone->data->scaleY - 1 + self->frames[self->framesLength - 1] - bone->scaleY ) * alpha;
		return;
	}

	// Interpolate between the last frame and the current frame. 
	KDint		frameIndex = binarySearch ( self->frames, self->framesLength, time, 3 );
	KDfloat		lastFrameX = self->frames[frameIndex - 2];
	KDfloat		lastFrameY = self->frames[frameIndex - 1];
	KDfloat		frameTime = self->frames[frameIndex];
	KDfloat		percent = 1 - ( time - frameTime ) / ( self->frames[frameIndex + TRANSLATE_LAST_FRAME_TIME] - frameTime );
	percent = CurveTimeline_getCurvePercent ( SUPER ( self ), frameIndex / 3 - 1, percent < 0 ? 0 : ( percent > 1 ? 1 : percent ) );

	bone->scaleX += ( bone->data->scaleX - 1 + lastFrameX + ( self->frames[frameIndex + TRANSLATE_FRAME_X] - lastFrameX ) * percent - bone->scaleX ) * alpha;
	bone->scaleY += ( bone->data->scaleY - 1 + lastFrameY + ( self->frames[frameIndex + TRANSLATE_FRAME_Y] - lastFrameY ) * percent - bone->scaleY ) * alpha;
}

ScaleTimeline* ScaleTimeline_create ( KDint frameCount ) 
{
	return _BaseTimeline_create ( frameCount, 3, _ScaleTimeline_apply );
}

KDvoid ScaleTimeline_setFrame ( ScaleTimeline* self, KDint frameIndex, KDfloat time, KDfloat x, KDfloat y )
{
	TranslateTimeline_setFrame ( self, frameIndex, time, x, y );
}



static const KDint	COLOR_LAST_FRAME_TIME = -5;
static const KDint	COLOR_FRAME_R = 1;
static const KDint	COLOR_FRAME_G = 2;
static const KDint	COLOR_FRAME_B = 3;
static const KDint	COLOR_FRAME_A = 4;

KDvoid _ColorTimeline_apply ( const Timeline* timeline, Skeleton* skeleton, KDfloat time, KDfloat alpha ) 
{
	ColorTimeline*	self = (ColorTimeline*) timeline;

	if ( time < self->frames[0] ) 
	{
		return;		// Time is before first frame. 
	}

	Slot*	slot = skeleton->slots[self->slotIndex];

	if ( time >= self->frames[self->framesLength - 5] ) 
	{
		// Time is after last frame. 
		KDint i = self->framesLength - 1;
		slot->r = self->frames[i - 3];
		slot->g = self->frames[i - 2];
		slot->b = self->frames[i - 1];
		slot->a = self->frames[i];
		return;
	}

	// Interpolate between the last frame and the current frame. 
	KDint		frameIndex = binarySearch ( self->frames, self->framesLength, time, 5 );
	KDfloat		lastFrameR = self->frames[frameIndex - 4];
	KDfloat		lastFrameG = self->frames[frameIndex - 3];
	KDfloat		lastFrameB = self->frames[frameIndex - 2];
	KDfloat		lastFrameA = self->frames[frameIndex - 1];
	KDfloat		frameTime = self->frames[frameIndex];
	KDfloat		percent = 1 - ( time - frameTime ) / ( self->frames[frameIndex + COLOR_LAST_FRAME_TIME] - frameTime );
	percent = CurveTimeline_getCurvePercent ( SUPER ( self ), frameIndex / 5 - 1, percent < 0 ? 0 : ( percent > 1 ? 1 : percent ) );

	KDfloat		r = lastFrameR + ( self->frames[frameIndex + COLOR_FRAME_R] - lastFrameR ) * percent;
	KDfloat		g = lastFrameG + ( self->frames[frameIndex + COLOR_FRAME_G] - lastFrameG ) * percent;
	KDfloat		b = lastFrameB + ( self->frames[frameIndex + COLOR_FRAME_B] - lastFrameB ) * percent;
	KDfloat		a = lastFrameA + ( self->frames[frameIndex + COLOR_FRAME_A] - lastFrameA ) * percent;
	if ( alpha < 1 )
	{
		slot->r += ( r - slot->r ) * alpha;
		slot->g += ( g - slot->g ) * alpha;
		slot->b += ( b - slot->b ) * alpha;
		slot->a += ( a - slot->a ) * alpha;
	} 
	else 
	{
		slot->r = r;
		slot->g = g;
		slot->b = b;
		slot->a = a;
	}
}

ColorTimeline* ColorTimeline_create ( KDint frameCount ) 
{
	return (ColorTimeline*) _BaseTimeline_create ( frameCount, 5, _ColorTimeline_apply );
}

KDvoid ColorTimeline_setFrame ( ColorTimeline* self, KDint frameIndex, KDfloat time, KDfloat r, KDfloat g, KDfloat b, KDfloat a ) 
{
	frameIndex *= 5;
	self->frames[frameIndex] = time;
	self->frames[frameIndex + 1] = r;
	self->frames[frameIndex + 2] = g;
	self->frames[frameIndex + 3] = b;
	self->frames[frameIndex + 4] = a;
}



KDvoid _AttachmentTimeline_apply ( const Timeline* timeline, Skeleton* skeleton, KDfloat time, KDfloat alpha ) 
{
	AttachmentTimeline*		self = (AttachmentTimeline*) timeline;

	if ( time < self->frames[0] ) 
	{
		return; // Time is before first frame. 
	}

	KDint	frameIndex;
	if ( time >= self->frames[self->framesLength - 1] ) 
	{
		// Time is after last frame. 
		frameIndex = self->framesLength - 1;
	}
	else
	{
		frameIndex = binarySearch ( self->frames, self->framesLength, time, 1 ) - 1;
	}

	const KDchar*	attachmentName = self->attachmentNames[frameIndex];
	Slot_setAttachment ( skeleton->slots[self->slotIndex], attachmentName ? Skeleton_getAttachmentForSlotIndex ( skeleton, self->slotIndex, attachmentName ) : 0 );
}

KDvoid _AttachmentTimeline_dispose ( Timeline* timeline )
{
	_Timeline_deinit ( timeline );
	AttachmentTimeline*		self = (AttachmentTimeline*) timeline;

	for ( KDint i = 0; i < self->framesLength; ++i )
	{
		FREE ( self->attachmentNames[i] );
	}
	FREE ( self->attachmentNames );

	FREE ( self );
}

AttachmentTimeline* AttachmentTimeline_create ( KDint frameCount ) 
{
	AttachmentTimeline*		self = NEW ( AttachmentTimeline );
	_Timeline_init ( SUPER ( self ), _AttachmentTimeline_dispose, _AttachmentTimeline_apply );

	CONST_CAST ( KDchar**, self->attachmentNames ) = CALLOC ( KDchar*, frameCount );
	CONST_CAST ( KDint	 , self->framesLength ) = frameCount;
	CONST_CAST ( KDfloat*, self->frames ) = CALLOC ( KDfloat, frameCount );

	return self;
}

KDvoid AttachmentTimeline_setFrame ( AttachmentTimeline* self, KDint frameIndex, KDfloat time, const KDchar* attachmentName ) 
{
	self->frames[frameIndex] = time;
	FREE ( self->attachmentNames[frameIndex] );
	if ( attachmentName )
	{
		MALLOC_STR ( self->attachmentNames[frameIndex], attachmentName );
	}
	else
	{
		self->attachmentNames[frameIndex] = 0;
	}
}

NS_CC_END
