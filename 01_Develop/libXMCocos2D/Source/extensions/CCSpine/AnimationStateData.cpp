/* -----------------------------------------------------------------------------------
 *
 *      File            AnimationStateData.cpp
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
#include "extensions/CCSpine/AnimationStateData.h"
#include "extensions/CCSpine/Extension.h"

NS_CC_BEGIN


typedef struct _ToEntry		_ToEntry;
struct _ToEntry 
{
	Animation*		animation;
	KDfloat			duration;
	_ToEntry*		next;
};

_ToEntry* _ToEntry_create ( Animation* to, KDfloat duration ) 
{
	_ToEntry*	self = NEW ( _ToEntry );
	self->animation = to;
	self->duration = duration;
	return self;
}

KDvoid _ToEntry_dispose ( _ToEntry* self ) 
{
	FREE ( self );
}



typedef struct _FromEntry	_FromEntry;
struct _FromEntry 
{
	Animation*		animation;
	_ToEntry*		toEntries;
	_FromEntry*		next;
};

_FromEntry* _FromEntry_create ( Animation* from ) 
{
	_FromEntry*		self = NEW ( _FromEntry );
	self->animation = from;
	return self;
}

KDvoid _FromEntry_dispose ( _FromEntry* self ) 
{
	FREE ( self );
}



AnimationStateData* AnimationStateData_create ( SkeletonData* skeletonData ) 
{
	AnimationStateData*		self = NEW ( AnimationStateData );
	CONST_CAST ( SkeletonData*, self->skeletonData ) = skeletonData;
	return self;
}

KDvoid AnimationStateData_dispose ( AnimationStateData* self ) 
{
	_FromEntry*		fromEntry = ( _FromEntry* )self->entries;
	while ( fromEntry ) 
	{
		_ToEntry*	toEntry = fromEntry->toEntries;
		while ( toEntry ) 
		{
			_ToEntry*	next = toEntry->next;
			_ToEntry_dispose ( toEntry );
			toEntry = next;
		}
		_FromEntry*		next = fromEntry->next;
		_FromEntry_dispose ( fromEntry );
		fromEntry = next;
	}

	FREE ( self );
}

KDvoid AnimationStateData_setMixByName ( AnimationStateData* self, const KDchar* fromName, const KDchar* toName, KDfloat duration )
{
	Animation*	from = SkeletonData_findAnimation ( self->skeletonData, fromName );
	if ( !from ) 
	{
		return;
	}

	Animation*	to = SkeletonData_findAnimation ( self->skeletonData, toName );
	if ( !to ) 
	{
		return;
	}
	AnimationStateData_setMix ( self, from, to, duration );
}

KDvoid AnimationStateData_setMix ( AnimationStateData* self, Animation* from, Animation* to, KDfloat duration ) 
{
	// Find existing FromEntry. 
	_FromEntry*		fromEntry = (_FromEntry*) self->entries;
	while ( fromEntry ) 
	{
		if ( fromEntry->animation == from )
		{
			// Find existing ToEntry. 
			_ToEntry*	toEntry = fromEntry->toEntries;
			while ( toEntry ) 
			{
				if ( toEntry->animation == to ) 
				{
					toEntry->duration = duration;
					return;
				}
				toEntry = toEntry->next;
			}
			break;	// Add new ToEntry to the existing FromEntry. 
		}
		fromEntry = fromEntry->next;
	}
	if ( !fromEntry )
	{
		fromEntry = _FromEntry_create ( from );
		fromEntry->next = (_FromEntry*) self->entries;
		CONST_CAST ( _FromEntry*, self->entries ) = fromEntry;
	}
	_ToEntry*	toEntry = _ToEntry_create ( to, duration );
	toEntry->next = fromEntry->toEntries;
	fromEntry->toEntries = toEntry;
}

KDfloat AnimationStateData_getMix ( AnimationStateData* self, Animation* from, Animation* to ) 
{
	_FromEntry*		fromEntry = (_FromEntry*) self->entries;
	while ( fromEntry ) 
	{
		if ( fromEntry->animation == from ) 
		{
			_ToEntry*	toEntry = fromEntry->toEntries;
			while ( toEntry ) 
			{
				if ( toEntry->animation == to )
				{
					return toEntry->duration;
				}
				toEntry = toEntry->next;
			}
		}
		fromEntry = fromEntry->next;
	}
	return 0;
}

NS_CC_END
