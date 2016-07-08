/* -----------------------------------------------------------------------------------
 *
 *      File            CCBone.h
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

#ifndef __CCBone_h__
#define __CCBone_h__

#include "utils/CCArmatureDefine.h"
#include "datas/CCDatas.h"
#include "animation/CCTween.h"
#include "display/CCDecorativeDisplay.h"
#include "display/CCDisplayManager.h"

namespace cocostudio {


class Armature;

class Bone : public cocos2d::NodeRGBA
{
	public :

		/**
		 *	Allocates and initializes a bone.
		 *	@return A initialized bone which is marked as "autorelease".
		 */
		static Bone*						create ( KDvoid );

		/**
		 *	Allocates and initializes a bone.
		 *
		 *	@param  name If name is not null, then set name to the bone's name
		 *	@return A initialized bone which is marked as "autorelease".
		 */
		static Bone*						create ( const KDchar* pName );

	public :

		Bone ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Bone ( KDvoid );

		/**
		 *	Initializes an empty Bone with nothing init.
		 */
		virtual KDbool						init ( KDvoid ) override;

		/**
		 *	Initializes a Bone with the specified name
		 *	@param name Bone's name.
		 */
		virtual KDbool						init ( const KDchar* pName );

		/**
		 *	Add display and use displayData to init the display.
		 *	If index already have a display, then replace it.
		 *	If index is current display index, then also change display to _index
		 *
		 *	@param displayData it include the display information, like DisplayType.
		 *           If you want to create a sprite display, then create a SpriteDisplayData param
		 *
		 *	@param index the index of the display you want to replace or add to
		 *           -1 : append display from back
		 */
		KDvoid								addDisplay ( DisplayData* pDisplayData, KDint nIndex );

		KDvoid								addDisplay ( cocos2d::Node* pDisplay, KDint nIndex );

		KDvoid								removeDisplay ( KDint nIndex );

		KDvoid								changeDisplayByIndex ( KDint nIndex, KDbool bForce );

		/**
		 * Add a child to this bone, and it will let this child call setParent(Bone *parent) function to set self to it's parent
		 * @param 	child  the child you want to add
		 */
		KDvoid								addChildBone ( Bone* pChild );

		/**
		 *  Set parent bone.
		 *  If parent is NUll, then also remove this bone from armature.
		 *  It will not set the Armature, if you want to add the bone to a Armature, you should use Armature::addBone(Bone *bone, const char* parentName).
		 *
		 *	@param parent  the parent bone.
		 *           nullptr : remove this bone from armature
		 */
		KDvoid								setParentBone ( Bone* pParent );

		/**
		 *	Get parent bone
		 *	@return parent bone
		 */
		Bone*								getParentBone ( KDvoid );

		using Node::removeFromParent;

		/**
		 *	Remove itself from its parent.
		 *	@param recursion    whether or not to remove childBone's display
		 */
		KDvoid								removeFromParent ( KDbool bRecursion );

		/**
		 *	Removes a child Bone
		 *	@param 	bone   the bone you want to remove
		 */
		KDvoid								removeChildBone ( Bone* pBone, KDbool bRecursion );

		KDvoid								update ( KDfloat fDelta ) override;

		KDvoid								updateDisplayedColor ( const cocos2d::Color3B& tParentColor ) override;
		KDvoid								updateDisplayedOpacity ( GLubyte cParentOpacity ) override;

		virtual KDvoid						setColor ( const cocos2d::Color3B& tColor ) override;
		virtual KDvoid						setOpacity ( GLubyte cOpacity ) override;

		//! Update color to render display
		KDvoid								updateColor ( KDvoid );

		//! Update zorder
		KDvoid								updateZOrder ( KDvoid );

		virtual KDvoid						setZOrder ( KDint zOrder ) override;

		Tween*								getTween ( KDvoid );

		/**
		 *	Whether or not the bone's transform property changed. if true, the bone will update the transform.
		 */
		virtual KDvoid						setTransformDirty ( KDbool bDirty ) { m_bBoneTransformDirty = bDirty; }
		virtual KDbool						 isTransformDirty ( KDvoid ) { return m_bBoneTransformDirty; }

		virtual cocos2d::AffineTransform	getNodeToArmatureTransform ( KDvoid ) const;
		virtual cocos2d::AffineTransform	getNodeToWorldTransform ( KDvoid ) const override;

		Node*								getDisplayRenderNode ( KDvoid );
		DisplayType							getDisplayRenderNodeType ( KDvoid );

		/**
		 *	Get the ColliderBody list in this bone. The object in the Array is ColliderBody.
		 */
		virtual cocos2d::Array*				getColliderBodyList ( KDvoid );

		virtual KDvoid						setColliderFilter ( ColliderFilter* pFilter );
		virtual ColliderFilter*				getColliderFilter ( KDvoid );

		virtual KDvoid						setBoneData ( BoneData* pBoneData );
		virtual BoneData*					getBoneData ( KDvoid ) const;

		virtual KDvoid						setArmature ( Armature* pArmature );
		virtual Armature*					getArmature ( KDvoid ) const;

		virtual KDvoid						setChildArmature ( Armature* pChildArmature );
		virtual Armature*					getChildArmature ( KDvoid ) const;

		virtual DisplayManager*				getDisplayManager ( KDvoid ) const { return m_pDisplayManager; }

		virtual KDvoid						setIgnoreMovementBoneData ( KDbool bIgnore ) { m_bIgnoreMovementBoneData = bIgnore; }
		virtual KDbool						 isIgnoreMovementBoneData ( KDvoid ) const { return m_bIgnoreMovementBoneData; }

		virtual KDvoid						setBlendType ( BlendType eType ) { m_eBlendType = eType; }
		virtual BlendType					getBlendType ( KDvoid ) const { return m_eBlendType; }

		virtual FrameData*					getTweenData ( KDvoid ) const { return m_pTweenData; }

		virtual KDvoid						setName ( const std::string& sName ) { m_sName = sName; }
		virtual const std::string			getName ( KDvoid ) const { return m_sName; }

		virtual BaseData*					getWorldInfo ( KDvoid ) const { return m_pWorldInfo; }

	protected :

		KDvoid								applyParentTransform ( Bone* pParent );

		/**
		 *  The origin state of the Bone. Display's state is effected by _boneData, m_pNode, _tweenData
		 *  when call setData function, it will copy from the BoneData.
		 */
		BoneData*							m_pBoneData;

		//! A weak reference to the Armature
		Armature*							m_pArmature;

		//! A weak reference to the child Armature
		Armature*							m_pChildArmature;

		DisplayManager*						m_pDisplayManager;

		/**
		 *	When Armature play an animation, if there is not a MovementBoneData of this bone in this MovementData, this bone will be hidden.
		 *	Set IgnoreMovementBoneData to true, then this bone will also be shown.
		 */
		KDbool								m_bIgnoreMovementBoneData;

		BlendType							m_eBlendType;

		Tween*								m_pTween;							//! Calculate tween effect

		//! Used for making tween effect in every frame
		FrameData*							m_pTweenData;

		std::string							m_sName;

		Bone*								m_pParentBone;						//! A weak reference to its parent
		KDbool								m_bBoneTransformDirty;				//! Whether or not transform dirty

		//! self Transform, use this to change display's state
		cocos2d::AffineTransform			m_tWorldTransform;

		BaseData*							m_pWorldInfo;
    
		//! Armature's parent bone
		Bone*								m_pArmatureParentBone;
    
		//! Data version
		KDfloat								m_fDataVersion;
};

}

#endif	// __CCBone_h__
