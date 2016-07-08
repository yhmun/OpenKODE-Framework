/* -----------------------------------------------------------------------------------
 *
 *      File            SpineCocos2dx.h
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

#ifndef __SpineCocos2dx_h__
#define __SpineCocos2dx_h__

#include "Spine.h"
#include "../../base_nodes/CCNode.h"

NS_CC_BEGIN


class CCSkeleton : public CCNodeRGBA, public CCBlendProtocol 
{
	public :
				 CCSkeleton ( SkeletonData* skeletonData, AnimationStateData* stateData = 0 );
		virtual ~CCSkeleton ( KDvoid );

		static	 CCSkeleton*	create ( const KDchar* skeletonDataFile, Atlas* atlas, KDfloat scale = 1 );
		static	 CCSkeleton*	create ( const KDchar* skeletonDataFile, const KDchar* atlasFile, KDfloat scale = 1 );
		static	 CCSkeleton*	create ( SkeletonData* skeletonData, AnimationStateData* stateData = 0 );

	public :

		virtual KDvoid			update ( KDfloat deltaTime );

		virtual KDvoid			draw ( KDvoid );

		virtual CCRect			boundingBox ( KDvoid );

		// CCBlendProtocol
		CC_PROPERTY ( ccBlendFunc, m_tBlendFunc, BlendFunc );

	public :

		Skeleton* const			skeleton;
		AnimationState* const	state;
		KDfloat					timeScale;
		KDbool					debugSlots;
		KDbool					debugBones;

	private :

		KDbool					ownsSkeleton;
		KDbool					ownsStateData;
		Atlas*					atlas;
};

KDvoid	RegionAttachment_updateQuad ( RegionAttachment* self, Slot* slot, ccV3F_C4B_T2F_Quad* quad );


NS_CC_END

#endif	// __SpineCocos2dx_h__
