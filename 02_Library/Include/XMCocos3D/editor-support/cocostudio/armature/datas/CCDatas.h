/* -----------------------------------------------------------------------------------
 *
 *      File            CCDatas.h
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

#ifndef __CCDatas_h__
#define __CCDatas_h__

#include "../utils/CCArmatureDefine.h"
#include "../utils/CCTweenFunction.h"
#include "../../../../ccTypes.h"
#include "../../../../base/CCArray.h"
#include "../../../../base/CCDictionary.h"
#include "../../../../base/CCAffineTransform.h"

#include <vector>


#define CC_CREATE_NO_PARAM_NO_INIT(varType)				\
public :												\
	static inline varType* create ( KDvoid ) {			\
	varType* var = new varType ( );						\
	if ( var )											\
	{													\
		var->autorelease ( );							\
		return var;										\
	}													\
	CC_SAFE_DELETE ( var );								\
	return nullptr;										\
}

#define CC_CREATE_NO_PARAM(varType)						\
public :												\
	static inline varType* create ( KDvoid ) {			\
	varType* var = new varType ( );						\
	if ( var && var->init ( ) )							\
	{													\
		var->autorelease ( );							\
		return var;										\
	}													\
	CC_SAFE_DELETE ( var );								\
	return nullptr;										\
}

namespace cocostudio {

/**
 *	The base node include a lot of attributes.
 */
class  BaseData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( BaseData )

	public :

		/**
		 *	@js ctor
		 */
		BaseData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~BaseData ( KDvoid );

		/**
		 *	Copy data from node
		 *	@param  node A BaseData to copy data
		 */
		virtual KDvoid				copy ( const BaseData* pNode );

		/**
		 *	Calculate two BaseData's between value(to - from) and set to self
		 *
		 *	@param  from   from BaseData
		 *	@param  to     to BaseData
		 */
		virtual KDvoid				subtract ( BaseData* pFrom, BaseData* pTo, KDbool bLimit );

		virtual KDvoid				setColor ( const cocos2d::Color4B& tColor );
		virtual cocos2d::Color4B	getColor ( KDvoid );

	public :

		KDfloat						x;						//! position x attribute
		KDfloat						y;						//! position y attribute
		KDint						m_zOrder;				//! zorder attribute, used to order the Bone's depth order

		/**
		 *	x y skewX skewY scaleX scaleY used to calculate transform matrix
		 *	skewX, skewY can have rotation effect
		 *	To get more matrix information, you can have a look at this pape : http://www.senocular.com/flash/tutorials/transformmatrix/
		 */
		KDfloat						m_fSkewX;
		KDfloat						m_fSkewY;
		KDfloat						m_fScaleX;
		KDfloat						m_fScaleY;

		KDfloat						m_fTweenRotate;			//! SkewX, SkewY, and TweenRotate effect the rotation

		KDbool						m_bIsUseColorInfo;		//! Whether or not this frame have the color changed Info
		KDint						a, r, g, b;

};

/**
 *	DisplayType distinguish which type your display is.
 */
enum DisplayType
{
    CS_DISPLAY_SPRITE,					//! display is a single Sprite
    CS_DISPLAY_ARMATURE,				//! display is a Armature
    CS_DISPLAY_PARTICLE,				//! display is a CCParticle.

    CS_DISPLAY_MAX
};

class  DisplayData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( DisplayData )

		static const KDchar*		changeDisplayToTexture ( const KDchar* pDisplayName );

	public :

		/**
		 *	@js ctor
		 */
		DisplayData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~DisplayData ( KDvoid );

		DisplayType					m_eDisplayType;	//! mark which type your display is
};

class  SpriteDisplayData : public DisplayData
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( SpriteDisplayData )

	public :

		/**
		 *	@js ctor
		 */
		SpriteDisplayData ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~SpriteDisplayData ( KDvoid );

		KDvoid						setParam ( const KDchar* pszDisplayName ) { this->m_sDisplayName = pszDisplayName; }
    
		KDvoid						copy ( SpriteDisplayData* pDisplayData );

	public :

		/**
		 *	If DisplayType is CS_DISPLAY_SPRITE, then Bone will use this image name to create a Sprite from CCSpriteFrameCache.
		 *	It should note that when use this name to create Sprite from CCSpriteFrameCache, you should use _displayName + ".png", because when use Texture Packer to pack single image file, the name have ".png".
		 *
		 *	If DisplayType is CS_DISPLAY_ARMATURE, the name is the Armature's name. When Bone init display and type is CS_DISPLAY_ARMATURE,
		 *	then Bone will create a Armature.
		 */
		std::string					m_sDisplayName;

		BaseData					m_tSkinData;
};

class ArmatureDisplayData  : public DisplayData
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( ArmatureDisplayData )

	public :

		/**
		 *	@js ctor
		 */
		ArmatureDisplayData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ArmatureDisplayData ( KDvoid );

		KDvoid						setParam ( const KDchar* pszDisplayName ) { this->m_sDisplayName = pszDisplayName; }
		KDvoid						copy ( ArmatureDisplayData* pDisplayData );

	public :

		/**
		 *	If DisplayType is CS_DISPLAY_SPRITE, then Bone will use this image name to create a Sprite from CCSpriteFrameCache.
		 *	It should note that when use this name to create Sprite from CCSpriteFrameCache, you should use _displayName + ".png", because when use Texture Packer to pack single image file, the name have ".png".
		 *
		 *	If DisplayType is CS_DISPLAY_ARMATURE, the name is the Armature's name. When Bone init display and type is CS_DISPLAY_ARMATURE,
		 *	then Bone will create a Armature.
		 */
		std::string					m_sDisplayName;
};


class  ParticleDisplayData : public DisplayData
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( ParticleDisplayData )

	public :

		/**
		 *	@js ctor
		 */
		ParticleDisplayData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleDisplayData ( KDvoid ) { }

		KDvoid						setParam ( const KDchar* pszPlistName ) { this->m_sPlist = pszPlistName; }

		KDvoid						copy ( ParticleDisplayData* pDisplayData );

	public :

		std::string					m_sPlist;
};

/**
 *	BoneData used to init a Bone.
 *	BoneData keeps a DisplayData list, a Bone can have many display to change.
 *	The display information saved in the DisplayData
 */
class  BoneData : public BaseData
{
	public :

		CC_CREATE_NO_PARAM ( BoneData )

	public :

		/**
		 *	@js ctor
		 */
		BoneData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~BoneData ( KDvoid );

		virtual KDbool				init ( KDvoid );

		KDvoid						addDisplayData ( DisplayData* pDisplayData );
		DisplayData*				getDisplayData ( KDint nIndex );

	public :

		std::string					m_sName;					//! the bone's name
		std::string					m_sParentName;				//! the bone parent's name
		cocos2d::Array				m_tDisplayDataList;		//! save DisplayData informations for the Bone
		cocos2d::AffineTransform	m_tBoneDataTransform;
};

/**
 *	ArmatureData saved the Armature name and Bonedata needed for the CCBones in this Armature
 *	When we create a Armature, we need to get each Bone's BoneData as it's init information.
 *	So we can get a BoneData from the Dictionary saved in the ArmatureData.
 */
class  ArmatureData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM ( ArmatureData )

	public :

		/**
		 *	@js ctor
		 */
		ArmatureData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ArmatureData ( KDvoid );

		KDbool						init ( KDvoid );

		KDvoid						addBoneData ( BoneData* pBoneData );

		BoneData*					getBoneData ( const KDchar* pBoneName );

	public :

		std::string					m_sName;
		cocos2d::Dictionary			m_tBoneDataDic;
		KDfloat						m_fDataVersion;
};

enum BlendType
{
    BLEND_NORMAL,
    BLEND_LAYER,
    BLEND_DARKEN,
    BLEND_MULTIPLY,
    BLEND_LIGHTEN,
    BLEND_SCREEN,
    BLEND_OVERLAY,
    BLEND_HARD_LIGHT,
    BLEND_ADD,
    BLEND_SUBSTRACT,
    BLEND_DIFFERENCE,
    BLEND_INVERT,
    BLEND_ALPHA,
    BLEND_ERASE
};

class  FrameData : public BaseData
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( FrameData )

	public :

		/**
		 *	@js ctor
		 */
		FrameData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~FrameData ( KDvoid );

		virtual KDvoid				copy ( const BaseData* pBaseData );

	public :

		KDint						m_nFrameID;
		KDint						m_nDuration;				//! The frame will last duration frames
		TweenType					m_eTweenEasing;				//! Every frame's tween easing effect
		KDbool						m_bIsTween;					//! Whether it's a tween key frame

		/**
		 *	The current display index when change to this frame.
		 *	If value is -1, then display will not be shown.
		 */
		KDint						m_nDisplayIndex;

		BlendType					m_eBlendType;

		std::string					m_sEvent;
		
		/**
		 *	strMovement, strEvent, strSound, strSoundEffect do not support yet
		 */
		std::string					m_sMovement;
		std::string					m_sSound;
		std::string					m_sSoundEffect;
};


class  MovementBoneData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM ( MovementBoneData )

	public :

		/**
		 *	@js ctor
		 */
		MovementBoneData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~MovementBoneData ( KDvoid );

		virtual KDbool				init ( KDvoid );

		KDvoid						addFrameData ( FrameData* pFrameData );
		FrameData*					getFrameData ( KDint nIndex );

	public :

		KDfloat						m_fDelay;				//! movement delay percent, this value can produce a delay effect
		KDfloat						m_fScale;				//! scale this movement
		KDfloat						m_fDuration;			//! this Bone in this movement will last m_iDuration frames
		std::string					m_sName;				//! bone name

		cocos2d::Array				m_tFrameList;
};

class  MovementData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( MovementData )

	public :

		/**
		 *	@js ctor
		 */
		MovementData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~MovementData ( KDvoid );

		KDvoid						addMovementBoneData ( MovementBoneData* pMovBoneData );
		MovementBoneData*			getMovementBoneData ( const KDchar* pBoneName );

	public :

		std::string					m_sName;
		KDint						m_nDuration;			//! the frames this movement will last
		KDfloat						m_fScale;				//! scale this movement

		/**
		 *	Change to this movement will last durationTo frames. Use this effect can avoid too suddenly changing.
		 *
		 *	Example : current movement is "stand", we want to change to "run", then we fill durationTo frames before
		 *	change to "run" instead of changing to "run" directly.
		 */
		KDint						m_nDurationTo;

		/**
		 *	This is different from duration, durationTween contain tween effect.
		 *	duration is the raw time that the animation will last, it's the same with the time you edit in the Action Editor.
		 *	durationTween is the actual time you want this animation last.
	 	 *	Example : If we edit 10 frames in the flash, then duration is 10. When we set durationTween to 50, the movement will last 50 frames, the extra 40 frames will auto filled with tween effect
		 */
		KDint						m_nDurationTween;

		KDbool						m_bLoop;           //! whether the movement was looped

		/**
		 *	Which tween easing effect the movement use
		 *	TWEEN_EASING_MAX : use the value from MovementData get from flash design panel
		 */
		TweenType					m_eTweenEasing;

		/**
		 *	@brief	save movment bone data
		 *	@key	const char *
		 *	@value	MovementBoneData *
		 */
		cocos2d::Dictionary			m_tMovBoneDataDic;
};

/**
 *  AnimationData include all movement infomation for the Armature
 *  The struct is AnimationData -> MovementData -> MovementBoneData -> FrameData
 *                                              -> MovementFrameData
 */
class  AnimationData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM_NO_INIT ( AnimationData )

	public :

		/**
		 *	@js ctor
		 */
		AnimationData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~AnimationData ( KDvoid );

		KDvoid						addMovement ( MovementData* pMovData );
		MovementData*				getMovement ( const KDchar* pMovementName );
		KDint						getMovementCount ( KDvoid );

	public :

		std::string					m_sName;
		cocos2d::Dictionary			m_tMovementDataDic;
		std::vector<std::string>	m_aMovementNames;
};


struct ContourVertex2 : public cocos2d::Object
{
    ContourVertex2 ( KDfloat xx, KDfloat yy )
    {
        this->x = xx;
        this->y = yy;
    }

    KDfloat							x;
    KDfloat							y;
};

/**
 *	ContourData include a contour vertex information
 */
class  ContourData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM ( ContourData )

	public :

		/**
		 *	@js ctor
		 */
		ContourData ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ContourData ( KDvoid );

		virtual KDbool				init ( KDvoid );
		virtual KDvoid				addVertex ( cocos2d::Point* pVertex );

	public :

		cocos2d::Array				m_tVertexList;				//! Save contour vertex info, vertex saved in a Point
};

/**
 *	TextureData include a texture's information
 */
class  TextureData : public cocos2d::Object
{
	public :

		CC_CREATE_NO_PARAM ( TextureData )

	public :

		/**
		 *	@js ctor
		 */
		TextureData ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		~TextureData ( KDvoid );

		virtual KDbool				init ( KDvoid );

		KDvoid						addContourData ( ContourData* pContourData );
		ContourData*				getContourData ( KDint nIndex );

	public :

		KDfloat						m_fHeight;		//! The texture's width, height
		KDfloat						m_fWidth;

		KDfloat						m_fPivotX;		//! The texture's anchor point
		KDfloat						m_fPivotY;

		std::string					m_sName;		//! The texture's name

		cocos2d::Array				m_tContourDataList;
};


}

#endif	// __CCDatas_h__
