/* -----------------------------------------------------------------------------------
 *
 *      File            CCArmatureDataManager.cpp
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

#include "editor-support/cocostudio/armature/utils/CCArmatureDataManager.h"
#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"
#include "editor-support/cocostudio/armature/utils/CCDataReaderHelper.h"
#include "editor-support/cocostudio/armature/utils/CCSpriteFrameCacheHelper.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"

using namespace cocos2d;

namespace cocostudio {

static ArmatureDataManager *s_sharedArmatureDataManager = nullptr;

ArmatureDataManager *ArmatureDataManager::getInstance()
{
    if (s_sharedArmatureDataManager == nullptr)
    {
        s_sharedArmatureDataManager = new ArmatureDataManager();
        if (!s_sharedArmatureDataManager || !s_sharedArmatureDataManager->init())
        {
            CC_SAFE_DELETE(s_sharedArmatureDataManager);
        }
    }
    return s_sharedArmatureDataManager;
}

void ArmatureDataManager::destoryInstance()
{
    SpriteFrameCacheHelper::purge();
    DataReaderHelper::purge();
    CC_SAFE_RELEASE(s_sharedArmatureDataManager);
}

ArmatureDataManager::ArmatureDataManager(void)
{
    m_pArmarureDatas = nullptr;
    m_pAnimationDatas = nullptr;
    m_pTextureDatas = nullptr;
    m_bAutoLoadSpriteFile = false;
}


ArmatureDataManager::~ArmatureDataManager(void)
{
    if( m_pAnimationDatas )
    {
        m_pAnimationDatas->removeAllObjects();
    }
    if( m_pArmarureDatas )
    {
        m_pArmarureDatas->removeAllObjects();
    }

    if( m_pTextureDatas )
    {
        m_pTextureDatas->removeAllObjects();
    }

    m_aRelativeDatas.clear();

    CC_SAFE_DELETE(m_pAnimationDatas);
    CC_SAFE_DELETE(m_pArmarureDatas);
    CC_SAFE_DELETE(m_pTextureDatas);
}


bool ArmatureDataManager::init()
{
    bool bRet = false;
    do
    {
        m_pArmarureDatas = Dictionary::create();
        CCASSERT(m_pArmarureDatas, "create ArmatureDataManager::_armarureDatas fail!");
        m_pArmarureDatas->retain();

        m_pAnimationDatas = Dictionary::create();
        CCASSERT(m_pAnimationDatas, "create ArmatureDataManager::_animationDatas fail!");
        m_pAnimationDatas->retain();

        m_pTextureDatas = Dictionary::create();
        CCASSERT(m_pTextureDatas, "create ArmatureDataManager::_textureDatas fail!");
        m_pTextureDatas->retain();

        bRet = true;
    }
    while (0);

    return bRet;
}

void ArmatureDataManager::removeArmatureFileInfo(const char *configFilePath)
{
    if (RelativeData *data = getRelativeData(configFilePath))
    {
        for (std::string str : data->armatures)
        {
            removeArmatureData(str.c_str());
        }

        for (std::string str : data->animations)
        {
            removeAnimationData(str.c_str());
        }

        for (std::string str : data->textures)
        {
            removeTextureData(str.c_str());
        }

        for (std::string str : data->plistFiles)
        {
            SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(str.c_str());
        }

        m_aRelativeDatas.erase(configFilePath);
        DataReaderHelper::getInstance()->removeConfigFile(configFilePath);
    }
}


void ArmatureDataManager::addArmatureData(const char *id, ArmatureData *armatureData, const char *configFilePath)
{
    if(m_pArmarureDatas)
    {
        if (RelativeData *data = getRelativeData(configFilePath))
        {
            data->armatures.push_back(id);
        }

        m_pArmarureDatas->setObject(armatureData, id);
    }
}

ArmatureData *ArmatureDataManager::getArmatureData(const char *id)
{
    ArmatureData *armatureData = nullptr;
    if (m_pArmarureDatas)
    {
        armatureData = (ArmatureData *)m_pArmarureDatas->objectForKey(id);
    }
    return armatureData;
}

void ArmatureDataManager::removeArmatureData(const char *id)
{
    if (m_pArmarureDatas)
    {
        m_pArmarureDatas->removeObjectForKey(id);
    }
}

void ArmatureDataManager::addAnimationData(const char *id, AnimationData *animationData, const char *configFilePath)
{
    if(m_pAnimationDatas)
    {
        if (RelativeData *data = getRelativeData(configFilePath))
        {
            data->animations.push_back(id);
        }

        m_pAnimationDatas->setObject(animationData, id);
    }
}

AnimationData *ArmatureDataManager::getAnimationData(const char *id)
{
    AnimationData *animationData = nullptr;
    if (m_pAnimationDatas)
    {
        animationData = (AnimationData *)m_pAnimationDatas->objectForKey(id);
    }
    return animationData;
}

void ArmatureDataManager::removeAnimationData(const char *id)
{
    if (m_pAnimationDatas)
    {
        m_pAnimationDatas->removeObjectForKey(id);
    }
}

void ArmatureDataManager::addTextureData(const char *id, TextureData *textureData, const char *configFilePath)
{
    if(m_pTextureDatas)
    {
        if (RelativeData *data = getRelativeData(configFilePath))
        {
            data->textures.push_back(id);
        }

        m_pTextureDatas->setObject(textureData, id);
    }
}


TextureData *ArmatureDataManager::getTextureData(const char *id)
{
    TextureData *textureData = nullptr;
    if (m_pTextureDatas)
    {
        textureData = (TextureData *)m_pTextureDatas->objectForKey(id);
    }
    return textureData;
}


void ArmatureDataManager::removeTextureData(const char *id)
{
    if(m_pTextureDatas)
    {
        m_pTextureDatas->removeObjectForKey(id);
    }
}

void ArmatureDataManager::addArmatureFileInfo(const char *configFilePath)
{
    addRelativeData(configFilePath);

    m_bAutoLoadSpriteFile = true;
    DataReaderHelper::getInstance()->addDataFromFile(configFilePath);
}

void ArmatureDataManager::addArmatureFileInfoAsync(const char *configFilePath, Object *target, SEL_SCHEDULE selector)
{
    addRelativeData(configFilePath);

    m_bAutoLoadSpriteFile = true;
    DataReaderHelper::getInstance()->addDataFromFileAsync("", "", configFilePath, target, selector);
}

void ArmatureDataManager::addArmatureFileInfo(const char *imagePath, const char *plistPath, const char *configFilePath)
{
    addRelativeData(configFilePath);

    m_bAutoLoadSpriteFile = false;
    DataReaderHelper::getInstance()->addDataFromFile(configFilePath);
    addSpriteFrameFromFile(plistPath, imagePath);
}

void ArmatureDataManager::addArmatureFileInfoAsync(const char *imagePath, const char *plistPath, const char *configFilePath, Object *target, SEL_SCHEDULE selector)
{
    addRelativeData(configFilePath);

    m_bAutoLoadSpriteFile = false;
    DataReaderHelper::getInstance()->addDataFromFileAsync(imagePath, plistPath, configFilePath, target, selector);
    addSpriteFrameFromFile(plistPath, imagePath);
}

void ArmatureDataManager::addSpriteFrameFromFile(const char *plistPath, const char *imagePath, const char *configFilePath)
{
    if (RelativeData *data = getRelativeData(configFilePath))
    {
        data->plistFiles.push_back(plistPath);
    }
    SpriteFrameCacheHelper::getInstance()->addSpriteFrameFromFile(plistPath, imagePath);
}


bool ArmatureDataManager::isAutoLoadSpriteFile()
{
    return m_bAutoLoadSpriteFile;
}

Dictionary *ArmatureDataManager::getArmatureDatas() const
{
    return m_pArmarureDatas;
}
Dictionary *ArmatureDataManager::getAnimationDatas() const
{
    return m_pAnimationDatas;
}
Dictionary *ArmatureDataManager::getTextureDatas() const
{
    return m_pTextureDatas;
}

void ArmatureDataManager::addRelativeData(const char *configFilePath)
{
    if (m_aRelativeDatas.find(configFilePath) == m_aRelativeDatas.end())
    {
        m_aRelativeDatas[configFilePath] = RelativeData();
    }
}

RelativeData *ArmatureDataManager::getRelativeData(const char* configFilePath)
{
    return &m_aRelativeDatas[configFilePath];
}

}
