/* -----------------------------------------------------------------------------------
 *
 *      File            CCBone.cpp
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

#include "editor-support/cocostudio/armature/CCBone.h"
#include "editor-support/cocostudio/armature/CCArmature.h"
#include "editor-support/cocostudio/armature/utils/CCUtilMath.h"
#include "editor-support/cocostudio/armature/utils/CCArmatureDataManager.h"
#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"
#include "editor-support/cocostudio/armature/display/CCDisplayManager.h"

using namespace cocos2d;

namespace cocostudio {

Bone *Bone::create()
{

    Bone *pBone = new Bone();
    if (pBone && pBone->init())
    {
        pBone->autorelease();
        return pBone;
    }
    CC_SAFE_DELETE(pBone);
    return nullptr;

}

Bone *Bone::create(const char *name)
{

    Bone *pBone = new Bone();
    if (pBone && pBone->init(name))
    {
        pBone->autorelease();
        return pBone;
    }
    CC_SAFE_DELETE(pBone);
    return nullptr;
}

Bone::Bone()
{
    m_pTweenData = nullptr;
    m_pParentBone = nullptr;
    m_pArmature = nullptr;
    m_pChildArmature = nullptr;
    m_pBoneData = nullptr;
    m_pTween = nullptr;
    m_pTween = nullptr;
    m_pChildren = nullptr;
    m_pDisplayManager = nullptr;
    m_bIgnoreMovementBoneData = false;
    m_tWorldTransform = AffineTransformMake(1, 0, 0, 1, 0, 0);
    m_bBoneTransformDirty = true;
    m_eBlendType = BLEND_NORMAL;
    m_pWorldInfo = nullptr;

    m_pArmatureParentBone = nullptr;
    m_fDataVersion = 0;
}


Bone::~Bone(void)
{
    CC_SAFE_DELETE(m_pTweenData);
    CC_SAFE_DELETE(m_pChildren);
    CC_SAFE_DELETE(m_pTween);
    CC_SAFE_DELETE(m_pDisplayManager);
    CC_SAFE_DELETE(m_pWorldInfo);

    CC_SAFE_RELEASE(m_pBoneData);

    CC_SAFE_RELEASE(m_pChildArmature);
}

bool Bone::init()
{
    return Bone::init(nullptr);
}


bool Bone::init(const char *name)
{
    bool bRet = false;
    do
    {

        if(nullptr != name)
        {
            m_sName = name;
        }

        CC_SAFE_DELETE(m_pTweenData);
        m_pTweenData = new FrameData();

        CC_SAFE_DELETE(m_pTween);
        m_pTween = new Tween();
        m_pTween->init(this);

        CC_SAFE_DELETE(m_pDisplayManager);
        m_pDisplayManager = new DisplayManager();
        m_pDisplayManager->init(this);

        CC_SAFE_DELETE(m_pWorldInfo);
        m_pWorldInfo = new BaseData();

        CC_SAFE_DELETE(m_pBoneData);
        m_pBoneData  = new BoneData();

        bRet = true;
    }
    while (0);

    return bRet;
}

void Bone::setBoneData(BoneData *boneData)
{
    CCASSERT(nullptr != boneData, "_boneData must not be nullptr");

    if (m_pBoneData != boneData)
    {
        CC_SAFE_RETAIN(boneData);
        CC_SAFE_RELEASE(m_pBoneData);
        m_pBoneData = boneData;
    }

    m_sName = m_pBoneData->m_sName;
    m_nZOrder = m_pBoneData->m_zOrder;

    m_pDisplayManager->initDisplayList(boneData);
}

BoneData *Bone::getBoneData() const
{
    return m_pBoneData;
}

void Bone::setArmature(Armature *armature)
{
    m_pArmature = armature;
    if (m_pArmature)
    {
        m_pTween->setAnimation(m_pArmature->getAnimation());
        m_fDataVersion = m_pArmature->getArmatureData()->m_fDataVersion;
        m_pArmatureParentBone = m_pArmature->getParentBone();
    }
    else
    {
        m_pArmatureParentBone = nullptr;
    }
}


Armature *Bone::getArmature() const
{
    return m_pArmature;
}

void Bone::update(float delta)
{
    if (m_pParentBone)
        m_bBoneTransformDirty = m_bBoneTransformDirty || m_pParentBone->isTransformDirty();

    if (m_pArmatureParentBone && !m_bBoneTransformDirty)
    {
        m_bBoneTransformDirty = m_pArmatureParentBone->isTransformDirty();
    }

    if (m_bBoneTransformDirty)
    {
        if (m_fDataVersion >= VERSION_COMBINED)
        {
            TransformHelp::nodeConcat(*m_pTweenData, *m_pBoneData);
            m_pTweenData->m_fScaleX -= 1;
            m_pTweenData->m_fScaleY -= 1;
        }

        m_pWorldInfo->copy(m_pTweenData);

        m_pWorldInfo->x = m_pTweenData->x + m_tPosition.x;
        m_pWorldInfo->y = m_pTweenData->y + m_tPosition.y;
        m_pWorldInfo->m_fScaleX = m_pTweenData->m_fScaleX * m_fScaleX;
        m_pWorldInfo->m_fScaleY = m_pTweenData->m_fScaleY * m_fScaleY;
        m_pWorldInfo->m_fSkewX = m_pTweenData->m_fSkewX + m_fSkewX + m_fRotationX;
        m_pWorldInfo->m_fSkewY = m_pTweenData->m_fSkewY + m_fSkewY - m_fRotationY;

        if(m_pParentBone)
        {
            applyParentTransform(m_pParentBone);
        }
        else
        {
            if (m_pArmatureParentBone)
            {
                applyParentTransform(m_pArmatureParentBone);
            }
        }

        TransformHelp::nodeToMatrix(*m_pWorldInfo, m_tWorldTransform);

        if (m_pArmatureParentBone)
        {
            m_tWorldTransform = AffineTransformConcat(m_tWorldTransform, m_pArmature->getNodeToParentTransform());
        }
    }

    DisplayFactory::updateDisplay(this, delta, m_bBoneTransformDirty || m_pArmature->getArmatureTransformDirty());

    if (m_pChildren)
    {
        for(auto object : *m_pChildren)
        {
            Bone *childBone = (Bone *)object;
            childBone->update(delta);
        }
    }

    m_bBoneTransformDirty = false;
}

void Bone::applyParentTransform(Bone *parent) 
{
    float x = m_pWorldInfo->x;
    float y = m_pWorldInfo->y;
    m_pWorldInfo->x = x * parent->m_tWorldTransform.a + y * parent->m_tWorldTransform.c + parent->m_pWorldInfo->x;
    m_pWorldInfo->y = x * parent->m_tWorldTransform.b + y * parent->m_tWorldTransform.d + parent->m_pWorldInfo->y;
    m_pWorldInfo->m_fScaleX = m_pWorldInfo->m_fScaleX * parent->m_pWorldInfo->m_fScaleX;
    m_pWorldInfo->m_fScaleY = m_pWorldInfo->m_fScaleY * parent->m_pWorldInfo->m_fScaleY;
    m_pWorldInfo->m_fSkewX = m_pWorldInfo->m_fSkewX + parent->m_pWorldInfo->m_fSkewX;
    m_pWorldInfo->m_fSkewY = m_pWorldInfo->m_fSkewY + parent->m_pWorldInfo->m_fSkewY;
}


void Bone::updateDisplayedColor(const Color3B &parentColor)
{
    m_tRealColor = Color3B(255, 255, 255);
    NodeRGBA::updateDisplayedColor(parentColor);
    updateColor();
}

void Bone::updateDisplayedOpacity(GLubyte parentOpacity)
{
    m_cRealOpacity = 255;
    NodeRGBA::updateDisplayedOpacity(parentOpacity);
    updateColor();
}

void Bone::setColor(const Color3B& color)
{
    NodeRGBA::setColor(color);
    updateColor();
}

void Bone::setOpacity(GLubyte opacity)
{
    NodeRGBA::setOpacity(opacity);
    updateColor();
}

void Bone::updateColor()
{
    Node *display = m_pDisplayManager->getDisplayRenderNode();
    RGBAProtocol *protocol = dynamic_cast<RGBAProtocol *>(display);
    if(protocol != nullptr)
    {
        protocol->setColor(Color3B(m_tDisplayedColor.r * m_pTweenData->r / 255, m_tDisplayedColor.g * m_pTweenData->g / 255, m_tDisplayedColor.b * m_pTweenData->b / 255));
        protocol->setOpacity(m_cDisplayedOpacity * m_pTweenData->a / 255);
    }
}

void Bone::updateZOrder()
{
    if (m_fDataVersion >= VERSION_COMBINED)
    {
        int zorder = m_pTweenData->m_zOrder + m_pBoneData->m_zOrder;
        setZOrder(zorder);
    }
    else
    {
        setZOrder(m_pTweenData->m_zOrder);
    }
}

void Bone::addChildBone(Bone *child)
{
    CCASSERT( nullptr != child, "Argument must be non-nil");
    CCASSERT( nullptr == child->m_pParentBone, "child already added. It can't be added again");

    if(!m_pChildren)
    {
        m_pChildren = Array::createWithCapacity(4);
        m_pChildren->retain();
    }

    if (m_pChildren->getIndexOfObject(child) == CC_INVALID_INDEX)
    {
        m_pChildren->addObject(child);
        child->setParentBone(this);
    }
}

void Bone::removeChildBone(Bone *bone, bool recursion)
{
    if (m_pChildren && m_pChildren->getIndexOfObject(bone) != CC_INVALID_INDEX )
    {
        if(recursion)
        {
            Array *ccbones = bone->m_pChildren;
            
            for(auto object : *ccbones)
            {
                Bone *ccBone = (Bone *)object;
                bone->removeChildBone(ccBone, recursion);
            }
        }

        bone->setParentBone(nullptr);

        bone->getDisplayManager()->setCurrentDecorativeDisplay(nullptr);

        m_pChildren->removeObject(bone);
    }
}

void Bone::removeFromParent(bool recursion)
{
    if (nullptr != m_pParentBone)
    {
        m_pParentBone->removeChildBone(this, recursion);
    }
}

void Bone::setParentBone(Bone *parent)
{
    m_pParentBone = parent;
}

Bone *Bone::getParentBone()
{
    return m_pParentBone;
}

void Bone::setChildArmature(Armature *armature)
{
    if (m_pChildArmature != armature)
    {
        if (armature == nullptr && m_pChildArmature)
        {
            m_pChildArmature->setParentBone(nullptr);
        }

        CC_SAFE_RETAIN(armature);
        CC_SAFE_RELEASE(m_pChildArmature);
        m_pChildArmature = armature;
    }
}

Armature *Bone::getChildArmature() const
{
    return m_pChildArmature;
}

Tween *Bone::getTween()
{
    return m_pTween;
}

void Bone::setZOrder(int zOrder)
{
    if (m_nZOrder != zOrder)
        Node::setZOrder(zOrder);
}

AffineTransform Bone::getNodeToArmatureTransform() const
{
    return m_tWorldTransform;
}

AffineTransform Bone::getNodeToWorldTransform() const
{
    return AffineTransformConcat(m_tWorldTransform, m_pArmature->getNodeToWorldTransform());
}

Node *Bone::getDisplayRenderNode()
{
    return m_pDisplayManager->getDisplayRenderNode();
}

DisplayType Bone::getDisplayRenderNodeType()
{
    return m_pDisplayManager->getDisplayRenderNodeType();
}


void Bone::addDisplay(DisplayData *displayData, int index)
{
    m_pDisplayManager->addDisplay(displayData, index);
}

void Bone::addDisplay(Node *display, int index)
{
    m_pDisplayManager->addDisplay(display, index);
}

void Bone::removeDisplay(int index)
{
    m_pDisplayManager->removeDisplay(index);
}

void Bone::changeDisplayByIndex(int index, bool force)
{
    m_pDisplayManager->changeDisplayByIndex(index, force);
}

Array *Bone::getColliderBodyList()
{
    if (DecorativeDisplay *decoDisplay = m_pDisplayManager->getCurrentDecorativeDisplay())
    {
        if (ColliderDetector *detector = decoDisplay->getColliderDetector())
        {
            return detector->getColliderBodyList();
        }
    }
    return nullptr;
}



void Bone::setColliderFilter(ColliderFilter *filter)
{
    Array *array = m_pDisplayManager->getDecorativeDisplayList();

    for(auto object : *array)
    {
        DecorativeDisplay *decoDisplay = static_cast<DecorativeDisplay *>(object);
        if (ColliderDetector *detector = decoDisplay->getColliderDetector())
        {
            detector->setColliderFilter(filter);
        }
    }
}
ColliderFilter *Bone::getColliderFilter()
{
    if (DecorativeDisplay *decoDisplay = m_pDisplayManager->getCurrentDecorativeDisplay())
    {
        if (ColliderDetector *detector = decoDisplay->getColliderDetector())
        {
            return detector->getColliderFilter();
        }
    }
    return nullptr;
}


}
