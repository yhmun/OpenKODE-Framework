/* -----------------------------------------------------------------------------------
 *
 *      File            CCArmatureDataManager.h
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

#ifndef __CCArmatureDataManager_h__
#define __CCArmatureDataManager_h__

#include "CCArmatureDefine.h"
#include "../datas/CCDatas.h"
#include <map>

namespace cocostudio {

struct RelativeData
{
    std::vector<std::string>	plistFiles;
    std::vector<std::string>	armatures;
    std::vector<std::string>	animations;
    std::vector<std::string>	textures;
};

/**
 *	@brief	format and manage armature configuration and armature animation
 */
class  ArmatureDataManager : public cocos2d::Object
{
	public :
    
		static ArmatureDataManager*			getInstance ( KDvoid );

		static KDvoid						destoryInstance ( KDvoid );
    
	private :

		/**
		 *	@js ctor
		 */
		ArmatureDataManager ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ArmatureDataManager ( KDvoid );

	public :

		/**
		 *	Init ArmatureDataManager
		 */
		virtual KDbool						init ( KDvoid );

		/**
		 *	Add armature data
		 *	@param id The id of the armature data
		 *	@param armatureData ArmatureData *
		 */
		KDvoid								addArmatureData ( const KDchar* pId, ArmatureData* pArmatureData, const KDchar* pConfigFilePath = "" );

		/**
		 *	@brief	get armature data
		 *	@param	id the id of the armature data you want to get
		 *  @return	ArmatureData *
		 */
		ArmatureData*						getArmatureData ( const KDchar* pId );

		/**
		 *	@brief	remove armature data
		 *	@param	id the id of the armature data you want to get
		 */
		KDvoid								removeArmatureData ( const KDchar* pId );

		/**
		 *	@brief	add animation data
		 *	@param 	id the id of the animation data
		 *  @return AnimationData *
		 */
		KDvoid								addAnimationData ( const KDchar* pId, AnimationData* pAnimationData, const KDchar* pConfigFilePath = "" );

		/**
		 *	@brief	get animation data from _animationDatas(Dictionary)
		 *	@param 	id the id of the animation data you want to get
		 *  @return AnimationData *
		 */
		AnimationData*						getAnimationData ( const KDchar* pId );

		/**
		 *	@brief	remove animation data
		 *	@param 	id the id of the animation data
		 */
		KDvoid								removeAnimationData ( const KDchar* pId );

		/**
		 *	@brief	add texture data
		 *	@param 	id the id of the texture data
		 *  @return TextureData *
		 */
		KDvoid								addTextureData ( const KDchar* pId, TextureData* pTextureData, const KDchar* pConfigFilePath = "" );

		/**
		 *	@brief	get texture data
		 *	@param 	id the id of the texture data you want to get
		 *  @return TextureData *
		 */
		TextureData*						getTextureData ( const KDchar* pId );

		/**
		 *	@brief	remove texture data
		 *	@param 	id the id of the texture data you want to get
		 */
		KDvoid								removeTextureData ( const KDchar* pId );

		/**
		 *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
		 */
		KDvoid								addArmatureFileInfo ( const KDchar* pConfigFilePath );

		/**
		 *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
		 *			It will load data in a new thread
		 */
		KDvoid								addArmatureFileInfoAsync ( const KDchar* pConfigFilePath, cocos2d::Object* pTarget, cocos2d::SEL_SCHEDULE pSelector );

		/**
		 *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
		 */
		KDvoid								addArmatureFileInfo ( const KDchar* pImagePath, const KDchar* pPlistPath, const KDchar* pConfigFilePath );

		/**
		 *	@brief	Add ArmatureFileInfo, it is managed by ArmatureDataManager.
		 *			It will load data in a new thread
		 */
		KDvoid								addArmatureFileInfoAsync ( const KDchar* pImagePath, const KDchar* pPlistPath, const KDchar* pConfigFilePath, cocos2d::Object* pTarget, cocos2d::SEL_SCHEDULE pSelector );

		/**
		 *	@brief	Add sprite frame to CCSpriteFrameCache, it will save display name and it's relative image name
		 */
		KDvoid								addSpriteFrameFromFile ( const KDchar* pPlistPath, const KDchar* pImagePath, const KDchar* pConfigFilePath = "" );

		virtual KDvoid						removeArmatureFileInfo ( const KDchar* pConfigFilePath );

		/**
		 *	@brief	Juge whether or not need auto load sprite file
		 */
		KDbool								isAutoLoadSpriteFile ( KDvoid );

		cocos2d::Dictionary*				getArmatureDatas ( KDvoid ) const;
		cocos2d::Dictionary*				getAnimationDatas ( KDvoid ) const;
		cocos2d::Dictionary*				getTextureDatas ( KDvoid ) const;

	protected :
		
		KDvoid								addRelativeData ( const KDchar* pConfigFilePath );
		RelativeData*						getRelativeData ( const KDchar* pConfigFilePath );

	private :

		/**
		 *	@brief	save amature datas
		 *  @key	std::string
		 *  @value	ArmatureData *
		 */
		cocos2d::Dictionary*				m_pArmarureDatas;

		/**
		 *	@brief	save animation datas
		 *  @key	std::string
		 *  @value	AnimationData *
		 */
		cocos2d::Dictionary*				m_pAnimationDatas;

		/**
		 *	@brief	save texture datas
		 *  @key	std::string
		 *  @value	TextureData *
		 */
		cocos2d::Dictionary*				m_pTextureDatas;

		KDbool								m_bAutoLoadSpriteFile;

		std::map<std::string, RelativeData> m_aRelativeDatas;
};


}

#endif	// __CCArmatureDataManager_h__
