/* -----------------------------------------------------------------------------------
 *
 *      File            CCDatas.cpp
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

#include "editor-support/cocostudio/armature/datas/CCDatas.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"
#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"

using namespace cocos2d;

namespace cocostudio {


BaseData::BaseData()
    : x(0.0f)
    , y(0.0f)
    , m_zOrder(0)

    , m_fSkewX(0.0f)
    , m_fSkewY(0.0f)
    , m_fScaleX(1.0f)
    , m_fScaleY(1.0f)

    , m_fTweenRotate(0.0f)

    , m_bIsUseColorInfo(false)
    , a(255)
    , r(255)
    , g(255)
    , b(255)
{
}

BaseData::~BaseData()
{
}

void BaseData::copy(const BaseData *node )
{
    x = node->x;
    y = node->y;
    m_zOrder = node->m_zOrder;

    m_fScaleX = node->m_fScaleX;
    m_fScaleY = node->m_fScaleY;
    m_fSkewX = node->m_fSkewX;
    m_fSkewY = node->m_fSkewY;

    m_fTweenRotate = node->m_fTweenRotate;

    m_bIsUseColorInfo = node->m_bIsUseColorInfo;
    r = node->r;
    g = node->g;
    b = node->b;
    a = node->a;
}


void BaseData::subtract(BaseData *from, BaseData *to, bool limit)
{
    x = to->x - from->x;
    y = to->y - from->y;
    m_fScaleX = to->m_fScaleX - from->m_fScaleX;
    m_fScaleY = to->m_fScaleY - from->m_fScaleY;
    m_fSkewX = to->m_fSkewX - from->m_fSkewX;
    m_fSkewY = to->m_fSkewY - from->m_fSkewY;

    if(m_bIsUseColorInfo || from->m_bIsUseColorInfo || to->m_bIsUseColorInfo)
    {
        a = to->a - from->a;
        r = to->r - from->r;
        g = to->g - from->g;
        b = to->b - from->b;

        m_bIsUseColorInfo = true;
    }
    else
    {
        a = r = g = b = 0;
        m_bIsUseColorInfo = false;
    }

	if (limit)
	{
        if (m_fSkewX > KD_PI_F)
        {
            m_fSkewX -= (float)CC_DOUBLE_PI;
        }
		if (m_fSkewX < -KD_PI_F)
        {
            m_fSkewX += (float)CC_DOUBLE_PI;
        }

		if (m_fSkewY > KD_PI_F)
        {
            m_fSkewY -= (float)CC_DOUBLE_PI;
        }
		if (m_fSkewY < -KD_PI_F)
        {
            m_fSkewY += (float)CC_DOUBLE_PI;
        }
    }

    if (to->m_fTweenRotate)
    {
        m_fSkewX += to->m_fTweenRotate;
        m_fSkewY -= to->m_fTweenRotate;
    }
	
}

void BaseData::setColor(const Color4B &color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    a = color.a;
}

Color4B BaseData::getColor()
{
    return Color4B(r, g, b, a);
}

const char *DisplayData::changeDisplayToTexture(const char *displayName)
{
    // remove .xxx
    std::string textureName = displayName;
    size_t startPos = textureName.find_last_of(".");

    if(startPos != std::string::npos)
    {
        textureName = textureName.erase(startPos);
    }

    return textureName.c_str();
}

DisplayData::DisplayData(void)
    : m_eDisplayType(CS_DISPLAY_MAX)
{
}

DisplayData::~DisplayData(void)
{
}

SpriteDisplayData::SpriteDisplayData(void)
    : m_sDisplayName("")
{
    m_eDisplayType = CS_DISPLAY_SPRITE;
}

SpriteDisplayData::~SpriteDisplayData()
{
}

void SpriteDisplayData::copy(SpriteDisplayData *displayData)
{
    m_sDisplayName = displayData->m_sDisplayName;
    m_eDisplayType = displayData->m_eDisplayType;

    m_tSkinData = displayData->m_tSkinData;
}

ArmatureDisplayData::ArmatureDisplayData(void)
    : m_sDisplayName("")
{
    m_eDisplayType = CS_DISPLAY_ARMATURE;
}

ArmatureDisplayData::~ArmatureDisplayData()
{
}

void ArmatureDisplayData::copy(ArmatureDisplayData *displayData)
{
    m_sDisplayName = displayData->m_sDisplayName;
    m_eDisplayType = displayData->m_eDisplayType;
}

ParticleDisplayData::ParticleDisplayData(void)
    : m_sPlist("")
{
    m_eDisplayType = CS_DISPLAY_PARTICLE;
}

void ParticleDisplayData::copy(ParticleDisplayData *displayData)
{
    m_sPlist = displayData->m_sPlist;
    m_eDisplayType = displayData->m_eDisplayType;
}


BoneData::BoneData(void)
    : m_sName("")
    , m_sParentName("")
{
}

BoneData::~BoneData(void)
{
}

bool BoneData::init()
{
    m_tDisplayDataList.init();
    return true;
}

void BoneData::addDisplayData(DisplayData *displayData)
{
    m_tDisplayDataList.addObject(displayData);
}

DisplayData *BoneData::getDisplayData(int index)
{
    return static_cast<DisplayData *>(m_tDisplayDataList.getObjectAtIndex(index));
}


ArmatureData::ArmatureData()
    : m_fDataVersion(0.1f)
{
}

ArmatureData::~ArmatureData()
{
}

bool ArmatureData::init()
{
    return true;
}

void ArmatureData::addBoneData(BoneData *boneData)
{
    m_tBoneDataDic.setObject(boneData, boneData->m_sName);
}

BoneData *ArmatureData::getBoneData(const char *boneName)
{
    return static_cast<BoneData*>(m_tBoneDataDic.objectForKey(boneName));
}

FrameData::FrameData(void)
    : m_nFrameID(0)
    , m_nDuration(1)
    , m_eTweenEasing(Linear)
    , m_bIsTween(true)
    , m_nDisplayIndex(0)
    , m_eBlendType(BLEND_NORMAL)

    , m_sEvent("")
    , m_sMovement("")
    , m_sSound("")
    , m_sSoundEffect("")
{
}

FrameData::~FrameData(void)
{
}

void FrameData::copy(const BaseData *baseData)
{
    BaseData::copy(baseData);
    
    if (const FrameData *frameData = dynamic_cast<const FrameData*>(baseData))
    {
        m_nDuration = frameData->m_nDuration;
        m_nDisplayIndex = frameData->m_nDisplayIndex;
        m_eTweenEasing = frameData->m_eTweenEasing;
        m_eBlendType = frameData->m_eBlendType;
    }
}

MovementBoneData::MovementBoneData()
    : m_fDelay(0.0f)
    , m_fScale(1.0f)
    , m_fDuration(0)
    , m_sName("")
{
}

MovementBoneData::~MovementBoneData(void)
{
}

bool MovementBoneData::init()
{
    return m_tFrameList.init();
}

void MovementBoneData::addFrameData(FrameData *frameData)
{
    m_tFrameList.addObject(frameData);
}

FrameData *MovementBoneData::getFrameData(int index)
{
    return static_cast<FrameData*>(m_tFrameList.getObjectAtIndex(index));
}



MovementData::MovementData(void)
    : m_sName("")
    , m_nDuration(0)
    , m_fScale(1.0f)
    , m_nDurationTo(0)
    , m_nDurationTween(0)
    , m_bLoop(true)
    , m_eTweenEasing(Linear)
{
}

MovementData::~MovementData(void)
{
}

void MovementData::addMovementBoneData(MovementBoneData *movBoneData)
{
    m_tMovBoneDataDic.setObject(movBoneData, movBoneData->m_sName);
}

MovementBoneData *MovementData::getMovementBoneData(const char *boneName)
{
    return static_cast<MovementBoneData *>(m_tMovBoneDataDic.objectForKey(boneName));
}



AnimationData::AnimationData(void)
{
}

AnimationData::~AnimationData(void)
{
}

void AnimationData::addMovement(MovementData *movData)
{
    m_tMovementDataDic.setObject(movData, movData->m_sName);
    m_aMovementNames.push_back(movData->m_sName);
}

MovementData *AnimationData::getMovement(const char *movementName)
{
    return static_cast<MovementData *>(m_tMovementDataDic.objectForKey(movementName));
}

int AnimationData::getMovementCount()
{
    return m_tMovementDataDic.count();
}



ContourData::ContourData()
{
}

ContourData::~ContourData()
{
}

bool ContourData::init()
{
    return m_tVertexList.init();
}

void ContourData::addVertex(Point *vertex)
{
    ContourVertex2 *vertex2 = new ContourVertex2(vertex->x, vertex->y);
    vertex2->autorelease();

    m_tVertexList.addObject(vertex2);
}

TextureData::TextureData()
    : m_fHeight(0.0f)
    , m_fWidth(0.0f)
    , m_fPivotX(0.5f)
    , m_fPivotY(0.5f)
    , m_sName("")
{
}

TextureData::~TextureData()
{
}

bool TextureData::init()
{
    return m_tContourDataList.init();
}

void TextureData::addContourData(ContourData *contourData)
{
    m_tContourDataList.addObject(contourData);
}

ContourData *TextureData::getContourData(int index)
{
    return static_cast<ContourData *>(m_tContourDataList.getObjectAtIndex(index));
}


}
