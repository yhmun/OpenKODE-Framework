/* -----------------------------------------------------------------------------------
 *
 *      File            CCDataReaderHelper.h
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

#ifndef __CCDataReaderHelper_h__
#define __CCDataReaderHelper_h__

#include "CCArmatureDefine.h"
#include "../datas/CCDatas.h"
#include "../CCArmature.h"
#include "../../json/CSContentJsonDictionary.h"

#include <string>
#include <queue>
#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace tinyxml2
{
    class XMLElement;
}

namespace cocostudio {


class  DataReaderHelper : cocos2d::Object
{
	protected :

		enum ConfigType
		{
			DragonBone_XML,
			CocoStudio_JSON
		};

		typedef struct _AsyncStruct
		{
			std::string						filename;
			std::string						fileContent;
			ConfigType						configType;
			std::string						baseFilePath;
			cocos2d::Object*				target;
			cocos2d::SEL_SCHEDULE			selector;
			KDbool					        autoLoadSpriteFile;

			std::string						imagePath;
			std::string						plistPath;
		} AsyncStruct;

		typedef struct _DataInfo
		{
			AsyncStruct*					asyncStruct;
			std::queue<std::string>			configFileQueue;
			KDfloat							contentScale;
			std::string						filename;
			std::string						baseFilePath;
			KDfloat							flashToolVersion;
			KDfloat							cocoStudioVersion;
		} DataInfo;

	public :

		static DataReaderHelper*			getInstance ( KDvoid );

		/**
		 *	Scale the position data, used for multiresolution adapter
		 *	It won't effect the data already read.
		 */
		static KDvoid						setPositionReadScale ( KDfloat fScale );
		static KDfloat						getPositionReadScale ( KDvoid );

		static KDvoid						purge ( KDvoid );

	public :

		/**
		 *	@js ctor
		 */
		DataReaderHelper ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~DataReaderHelper ( KDvoid );

		KDvoid								addDataFromFile ( const KDchar* pFilePath );
		KDvoid								addDataFromFileAsync ( const KDchar* pImagePath, const KDchar* pPlistPath, const KDchar* pFilePath, cocos2d::Object* pTarget, cocos2d::SEL_SCHEDULE pSelector );

		KDvoid								addDataAsyncCallBack ( KDfloat dt );

		KDvoid								removeConfigFile ( const KDchar* pConfigFile );

	public :

		/**
		 *	Translate XML export from Dragon Bone flash tool to datas, and save them.
		 *	When you add a new xml, the data already saved will be keeped.
		 *
		 *	@param xmlPath The cache of the xml
		 */
		static KDvoid						addDataFromCache ( const KDchar* pFileContent, DataInfo* pDataInfo = nullptr );

		/**
		 *	Decode Armature Datas from xml export from Dragon Bone flash tool
		 */
		static ArmatureData*				decodeArmature ( tinyxml2::XMLElement* pArmatureXML, DataInfo* pDataInfo );
		static BoneData*					decodeBone ( tinyxml2::XMLElement* pBoneXML, tinyxml2::XMLElement* pParentXML, DataInfo* pDataInfo );
		static DisplayData*					decodeBoneDisplay ( tinyxml2::XMLElement* pDisplayXML, DataInfo* pDataInfo );

		/**
		 *	Decode ArmatureAnimation Datas from xml export from Dragon Bone flash tool
		 */
		static AnimationData*				decodeAnimation ( tinyxml2::XMLElement* pAnimationXML, DataInfo* pDataInfo );
		static MovementData*				decodeMovement ( tinyxml2::XMLElement* pMovementXML, ArmatureData* pArmatureData, DataInfo* pDataInfo );
		static MovementBoneData*			decodeMovementBone ( tinyxml2::XMLElement* pMovBoneXml, tinyxml2::XMLElement* pParentXml, BoneData* pBoneData, DataInfo* pDataInfo );
		static FrameData*					decodeFrame ( tinyxml2::XMLElement* pFrameXML, tinyxml2::XMLElement* pParentFrameXml, BoneData* pBoneData, DataInfo* pDataInfo );


		/**
		 *	Decode Texture Datas from xml export from Dragon Bone flash tool
		 */
		static TextureData*					decodeTexture ( tinyxml2::XMLElement* pTextureXML, DataInfo* pDataInfo );

		/**
		 *	Decode Contour Datas from xml export from Dragon Bone flash tool
		 */
		static ContourData*					decodeContour ( tinyxml2::XMLElement* pContourXML, DataInfo* pDataInfo );

	public :

		static KDvoid						addDataFromJsonCache ( const KDchar* pFileContent, DataInfo* pDataInfo = nullptr );

		static ArmatureData*				decodeArmature ( JsonDictionary& tJson, DataInfo* pDataInfo );
		static BoneData*					decodeBone ( JsonDictionary& tJson, DataInfo* pDataInfo );
		static DisplayData*					decodeBoneDisplay ( JsonDictionary& tJson, DataInfo* pDataInfo );

		static AnimationData*				decodeAnimation ( JsonDictionary& tJson, DataInfo* pDataInfo );
		static MovementData*				decodeMovement ( JsonDictionary& tJson, DataInfo* pDataInfo );
		static MovementBoneData*			decodeMovementBone ( JsonDictionary& tJson, DataInfo* pDataInfo );
		static FrameData*					decodeFrame ( JsonDictionary& tJson, DataInfo* pDataInfo );

		static TextureData*					decodeTexture ( JsonDictionary& tJson );

		static ContourData*					decodeContour ( JsonDictionary& tJson );

		static KDvoid						decodeNode ( BaseData* pNode, JsonDictionary& tJson, DataInfo* pDataInfo );

	protected :

		KDvoid								loadData ( KDvoid );

		std::condition_variable				m_tSleepCondition;

		std::thread*						m_pLoadingThread;

		std::mutex							m_tSleepMutex;

		std::mutex							m_tAsyncStructQueueMutex;
		std::mutex							m_tDataInfoMutex;

		std::mutex							m_tAddDataMutex;

		std::mutex							m_tGetFileMutex;

	  
		KDuint32							m_uAsyncRefCount;
		KDuint32							m_uAsyncRefTotalCount;

		KDbool								m_bNeedQuit;

		std::queue<AsyncStruct*>*			m_pAsyncStructQueue;
		std::queue<DataInfo*>*				m_pDataQueue;

		static std::vector<std::string>		m_sConfigFileList;

		static DataReaderHelper*			m_pDataReaderHelper;
};

}

#endif	// __CCDataReaderHelper_h__
