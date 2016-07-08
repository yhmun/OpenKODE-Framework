/* -----------------------------------------------------------------------------------
 *
 *      File            CCSkeleton.h
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

#ifndef __CCSkeleton_h__
#define __CCSkeleton_h__

#include "spine.h"
#include "RegionAttachment.h"
#include "../../2d/base_nodes/CCNode.h"
#include "../../2d/textures/CCTextureAtlas.h"

namespace spine {

/**
 *	Draws a skeleton.
 */
class CCSkeleton: public cocos2d::NodeRGBA, public cocos2d::BlendProtocol
{
	public :

		static CCSkeleton*		createWithData ( SkeletonData* pSkeletonData, KDbool bOwnsSkeletonData = false );
		static CCSkeleton*		createWithFile ( const KDchar* pSkeletonDataFile, Atlas* pAtlas, KDfloat fScale = 1 );
		static CCSkeleton*		createWithFile ( const KDchar* pSkeletonDataFile, const KDchar* pAtlasFile, KDfloat fScale = 1 );
		
		/**
		 *	@js NA
		 */
		CCSkeleton ( SkeletonData* pSkeletonData, KDbool bOwnsSkeletonData = false );
		
		/**
		 *	@js NA
		 */
		CCSkeleton ( const KDchar* pSkeletonDataFile, Atlas* pAtlas, KDfloat fScale = 1 );

		/**
		 *	@js NA
		 */
		CCSkeleton ( const KDchar* pSkeletonDataFile, const KDchar* pAtlasFile, KDfloat fScale = 1 );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~CCSkeleton ( KDvoid );

		// --- Convenience methods for common Skeleton_* functions.
		KDvoid								updateWorldTransform ( KDvoid );

		KDvoid								setToSetupPose ( KDvoid );
		KDvoid								setBonesToSetupPose ( KDvoid );
		KDvoid								setSlotsToSetupPose ( KDvoid );

		/* Returns 0 if the bone was not found. */
		Bone*								findBone ( const KDchar* pBoneName ) const;
		/* Returns 0 if the slot was not found. */
		Slot*								findSlot ( const KDchar* pSlotName ) const;
	
		/* Sets the skin used to look up attachments not found in the SkeletonData defaultSkin. Attachments from the new skin are
		 * attached if the corresponding attachment from the old skin was attached. Returns false if the skin was not found.
		 * @param skin May be 0.*/
		KDbool								setSkin ( const KDchar* pSkinName );
	
		/* Returns 0 if the slot or attachment was not found. */
		Attachment*							getAttachment ( const KDchar* pSlotName, const KDchar* pAttachmentName ) const;
		/* Returns false if the slot or attachment was not found. */
		KDbool								setAttachment ( const KDchar* pSlotName, const KDchar* pAttachmentName );

		// Overrides
		virtual KDvoid						update ( KDfloat fDeltaTime ) override;
		virtual KDvoid						draw ( KDvoid ) override;
		virtual cocos2d::Rect				getBoundingBox ( KDvoid ) const override;

		virtual KDvoid						setOpacityModifyRGB ( KDbool bValue ) override;
		virtual KDbool						 isOpacityModifyRGB ( KDvoid ) const override;

		virtual KDvoid						setBlendFunc ( const cocos2d::BlendFunc& tFunc ) override;
		virtual const cocos2d::BlendFunc&	getBlendFunc ( KDvoid ) const override;

		Skeleton*							m_pSkeleton;
		Bone*								m_pRootBone;
		KDfloat								m_fTimeScale;
		KDbool								m_bDebugSlots;
		KDbool								m_bDebugBones;
		KDbool								m_bPremultipliedAlpha;
		cocos2d::BlendFunc					m_tBlendFunc;

	protected :

		CCSkeleton ( KDvoid );

		KDvoid								setSkeletonData ( SkeletonData* pSkeletonData, KDbool bOwnsSkeletonData );
		cocos2d::TextureAtlas*				getTextureAtlas ( RegionAttachment* pRegionAttachment ) const;

	private :

		KDbool								m_bOwnsSkeletonData;
		Atlas*								m_pAtlas;

		KDvoid								initialize ( KDvoid );
};

} // namespace spine {

#endif	// __CCSkeleton_h__
