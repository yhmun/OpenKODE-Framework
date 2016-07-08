/* -----------------------------------------------------------------------------------
 *
 *      File            CCArmature.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCArmature_h__
#define __CCArmature_h__

#include "utils/CCArmatureDefine.h"
#include "CCBone.h"
#include "display/CCBatchNode.h"
#include "animation/CCArmatureAnimation.h"
#include "utils/CCSpriteFrameCacheHelper.h"
#include "utils/CCArmatureDataManager.h"

class b2Body;
class b2Fixture;
struct cpBody;

namespace cocostudio {

class Armature : public cocos2d::NodeRGBA, public cocos2d::BlendProtocol
{
	public :

		/**
		 *	Allocates and initializes an armature.
		 *	@return An initialized armature which is marked as "autorelease".
		 */
		static Armature*						create ( KDvoid );

		/**
		 *	Allocates an armature, and use the ArmatureData named name in ArmatureDataManager to initializes the armature.
		 *
		 *	@param  name Armature will use the name to find the ArmatureData to initializes it.
		 *	@return A initialized armature which is marked as "autorelease".
		 */
		static Armature*						create ( const KDchar* pName );

		static Armature*						create ( const KDchar* pName, Bone* pParentBone );

	public :

		Armature ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Armature ( KDvoid );

		/**
		 *	Init the empty armature
		 */
		virtual KDbool							init ( KDvoid ) override;

		/**
		 *	Init an armature with specified name
		 *	@param name Armature name
		 */
		virtual KDbool							init ( const KDchar* pName );

		virtual KDbool							init ( const KDchar* pName, Bone* pParentBone );

		/**
		 *	Add a Bone to this Armature,
		 *
		 *	@param bone  The Bone you want to add to Armature
		 *	@param parentName   The parent Bone's name you want to add to . If it's  nullptr, then set Armature to its parent
		 */
		virtual KDvoid							addBone ( Bone* pBone, const KDchar* pParentName );

		/**
		 *	Get a bone with the specified name
		 *
		 *	@param name The bone's name you want to get
		 */
		virtual Bone*							getBone ( const KDchar* pName ) const;

		/**
		 *	Change a bone's parent with the specified parent name.
		 *
		 *	@param bone The bone you want to change parent
		 *	@param parentName The new parent's name.
		 */
		virtual KDvoid							changeBoneParent ( Bone* pBone, const KDchar* pParentName );

		/**
		 *	Remove a bone with the specified name. If recursion it will also remove child Bone recursionly.
		 *
		 *	@param bone The bone you want to remove
		 *	@param recursion Determine whether remove the bone's child  recursion.
		 */
		virtual KDvoid							removeBone ( Bone* pBone, KDbool bRecursion );

		/**
		 *	Get Armature's bone dictionary
		 *	@return Armature's bone dictionary
		 */
		cocos2d::Dictionary*					getBoneDic ( KDvoid ) const;

		/**
		 *	This boundingBox will calculate all bones' boundingBox every time
		 */
		virtual cocos2d::Rect					getBoundingBox ( KDvoid ) const override;

		Bone*									getBoneAtPoint ( KDfloat x, KDfloat y ) const;

		// overrides

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid							visit ( KDvoid ) override;
		virtual KDvoid							update ( KDfloat dt ) override;
		virtual KDvoid							draw ( KDvoid ) override;

		virtual const cocos2d::AffineTransform&	getNodeToParentTransform ( KDvoid ) const override;

		inline KDvoid							setBlendFunc ( const cocos2d::BlendFunc& tBlendFunc ) override { m_tBlendFunc = tBlendFunc; }
		inline const cocos2d::BlendFunc&		getBlendFunc ( KDvoid ) const override { return m_tBlendFunc; }
	

		/**
		 *	Set contentsize and Calculate anchor point.
		 */
		virtual KDvoid							updateOffsetPoint ( KDvoid );

		virtual KDvoid							setAnimation ( ArmatureAnimation* pAnimation );
		virtual ArmatureAnimation*				getAnimation ( KDvoid ) const;
    
		virtual KDbool							getArmatureTransformDirty ( KDvoid ) const;

		virtual cocos2d::TextureAtlas*			getTexureAtlasWithTexture ( cocos2d::Texture2D* pTexture ) const;

		virtual KDvoid							setColliderFilter ( ColliderFilter* pFilter );


		virtual KDvoid							setArmatureData ( ArmatureData* pArmatureData ) { m_pArmatureData = pArmatureData; }
		virtual ArmatureData*					getArmatureData ( KDvoid ) const { return m_pArmatureData; }

		virtual KDvoid							setBatchNode ( BatchNode* pBatchNode ) { m_pBatchNode = pBatchNode; }
		virtual BatchNode*						getBatchNode ( KDvoid ) const { return m_pBatchNode; }

		virtual KDvoid							setName ( const std::string& sName ) { m_sName = sName; }
		virtual const std::string&				getName ( KDvoid ) const { return m_sName; } 

		virtual KDvoid							setTextureAtlas ( cocos2d::TextureAtlas* pAtlas ) { m_pAtlas = pAtlas; }
		virtual cocos2d::TextureAtlas*			getTextureAtlas ( KDvoid ) const { return m_pAtlas; }

		virtual KDvoid							setParentBone ( Bone* pParentBone );
		virtual Bone*							getParentBone ( KDvoid ) const;

		virtual KDvoid							setVersion ( KDfloat fVersion ) { m_fVersion = fVersion; }
		virtual KDfloat							getVersion ( KDvoid ) const { return m_fVersion; }

		virtual b2Fixture*						getB2ShapeList ( KDvoid );

		virtual KDvoid							setB2Body ( b2Body* pBody );
		virtual b2Body*							getB2Body ( KDvoid ) const;

		virtual cpShape*						getCPShapeList ( KDvoid );

		virtual KDvoid							setCPBody ( cpBody* pBody );
		virtual cpBody*							getCPBody ( KDvoid ) const;

	protected :

		/**
		 *	Used to create Bone internal
		 */
		Bone*									createBone ( const KDchar* pBoneName );

		//! Update blend function
		KDvoid									updateBlendType ( BlendType eBlendType );

	protected :

		ArmatureData*							m_pArmatureData;
		BatchNode*								m_pBatchNode;
		std::string								m_sName;
		cocos2d::TextureAtlas*					m_pAtlas;
		Bone*									m_pParentBone;
		KDfloat									m_fVersion;

		mutable KDbool							m_bArmatureTransformDirty;

		cocos2d::Dictionary*					m_pBoneDic;							//! The dictionary of the bones, include all bones in the armature, no matter it is the direct bone or the indirect bone. It is different from m_pChindren.

		cocos2d::Array*							m_pTopBoneList;

		cocos2d::BlendFunc						m_tBlendFunc;						//! It's required for CCTextureProtocol inheritance

		cocos2d::Point							m_tOffsetPoint;

		ArmatureAnimation*						m_pAnimation;

		cocos2d::Dictionary*					m_pTextureAtlasDic;

		b2Body*									m_pB2Body;
		cpBody*									m_pCPBody;
};

}

#endif	// __CCArmature_h__
