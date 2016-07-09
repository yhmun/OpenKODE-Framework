/* -----------------------------------------------------------------------------------
 *
 *      File            CCSkeletonAnimation.h
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

#ifndef __CCSkeletonAnimation_h__
#define __CCSkeletonAnimation_h__

#include "spine.h"
#include "CCSkeleton.h"

namespace spine {

/**
 *	Draws an animated skeleton, providing a simple API for applying one or more animations and queuing animations to be played later.
 */
class CCSkeletonAnimation: public CCSkeleton 
{
	public :

		std::vector<AnimationState*>		m_aStates;

		static CCSkeletonAnimation*			createWithData ( SkeletonData* pSkeletonData );
		static CCSkeletonAnimation*			createWithFile ( const KDchar* pSkeletonDataFile, Atlas* pAtlas, KDfloat fScale = 1 );
		static CCSkeletonAnimation*			createWithFile ( const KDchar* pSkeletonDataFile, const KDchar* pAtlasFile, KDfloat fScale = 1 );

		/**
		 *	@js NA
		 */
		CCSkeletonAnimation ( SkeletonData* pSkeletonData );

		/**
		 *	@js NA
		 */
		CCSkeletonAnimation ( const KDchar* pSkeletonDataFile, Atlas* pAtlas, KDfloat fScale = 1 );

		/**
		 *	@js NA
		 */
		CCSkeletonAnimation ( const KDchar* pSkeletonDataFile, const KDchar* pAtlasFile, KDfloat fScale = 1 );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~CCSkeletonAnimation ( KDvoid );

		virtual KDvoid						update ( KDfloat fDeltaTime );

		KDvoid								addAnimationState ( AnimationStateData* pStateData = 0 );
		KDvoid								setAnimationStateData (AnimationStateData* pStateData, KDint nStateIndex = 0 );
		KDvoid								setMix ( const KDchar* pFromAnimation, const KDchar* toAnimation, KDfloat fDuration, KDint nStateIndex = 0 );
		KDvoid								setAnimation ( const KDchar* pName, KDbool bLoop, KDint nStateIndex = 0 );
		KDvoid								addAnimation ( const KDchar* pName, KDbool bLoop, KDfloat fDelay = 0, KDint nStateIndex = 0 );
		KDvoid								clearAnimation ( KDint nStateIndex = 0);

	protected :

		CCSkeletonAnimation ( KDvoid );

	private :

		typedef CCSkeleton super;

		std::vector<AnimationStateData*>	m_aStateDatas;

		KDvoid								initialize ( KDvoid );
};

} // namespace spine {

#endif	// __CCSkeletonAnimation_h__
