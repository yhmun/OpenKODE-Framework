/* -----------------------------------------------------------------------------------
 *
 *      File            CCArmature.cpp
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

#include "editor-support/cocostudio/armature/CCArmature.h"
#include "editor-support/cocostudio/armature/utils/CCArmatureDataManager.h"
#include "editor-support/cocostudio/armature/utils/CCArmatureDefine.h"
#include "editor-support/cocostudio/armature/utils/CCDataReaderHelper.h"
#include "editor-support/cocostudio/armature/datas/CCDatas.h"
#include "editor-support/cocostudio/armature/display/CCSkin.h"
#include "2d/effects/CCGrid.h"
#include "shaders/CCShaderCache.h"
#include "XMKazmath/GL/matrix.h"

#include "XMBox2D/Box2D.h"
#include "XMChipmunk/chipmunk.h"

using namespace cocos2d;


namespace cocostudio {

Armature *Armature::create()
{
    Armature *armature = new Armature();
    if (armature && armature->init())
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}


Armature *Armature::create(const char *name)
{
    Armature *armature = new Armature();
    if (armature && armature->init(name))
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}

Armature *Armature::create(const char *name, Bone *parentBone)
{
    Armature *armature = new Armature();
    if (armature && armature->init(name, parentBone))
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}

Armature::Armature()
    : m_pArmatureData(nullptr)
    , m_pBatchNode(nullptr)
    , m_pAtlas(nullptr)
    , m_pParentBone(nullptr)
    , m_bArmatureTransformDirty(true)
    , m_pBoneDic(nullptr)
    , m_pTopBoneList(nullptr)
    , m_pAnimation(nullptr)
    , m_pTextureAtlasDic(nullptr)
{
	m_pB2Body = nullptr;
	m_pCPBody = nullptr;	
}


Armature::~Armature(void)
{
    if(nullptr != m_pBoneDic)
    {
        m_pBoneDic->removeAllObjects();
        CC_SAFE_DELETE(m_pBoneDic);
    }
    if (nullptr != m_pTopBoneList)
    {
        m_pTopBoneList->removeAllObjects();
        CC_SAFE_DELETE(m_pTopBoneList);
    }
    CC_SAFE_DELETE(m_pAnimation);
    CC_SAFE_RELEASE(m_pTextureAtlasDic);
}


bool Armature::init()
{
    return init(nullptr);
}


bool Armature::init(const char *name)
{
    bool bRet = false;
    do
    {
        removeAllChildren();

        CC_SAFE_DELETE(m_pAnimation);
        m_pAnimation = new ArmatureAnimation();
        m_pAnimation->init(this);

        CC_SAFE_DELETE(m_pBoneDic);
        m_pBoneDic	= new Dictionary();

        CC_SAFE_DELETE(m_pTopBoneList);
        m_pTopBoneList = new Array();
        m_pTopBoneList->init();

        CC_SAFE_DELETE(m_pTextureAtlasDic);
        m_pTextureAtlasDic = new Dictionary();

        m_tBlendFunc.src = CC_BLEND_SRC;
        m_tBlendFunc.dst = CC_BLEND_DST;


        m_sName = name == nullptr ? "" : name;

        ArmatureDataManager *armatureDataManager = ArmatureDataManager::getInstance();

        if(m_sName.length() != 0)
        {
            m_sName = name;

            AnimationData *animationData = armatureDataManager->getAnimationData(name);
            CCASSERT(animationData, "AnimationData not exist! ");

            m_pAnimation->setAnimationData(animationData);


            ArmatureData *armatureData = armatureDataManager->getArmatureData(name);
            CCASSERT(armatureData, "");

            m_pArmatureData = armatureData;


            DictElement *_element = nullptr;
            Dictionary *boneDataDic = &armatureData->m_tBoneDataDic;
            CCDICT_FOREACH(boneDataDic, _element)
            {
                Bone *bone = createBone(_element->getStrKey());

                //! init bone's  Tween to 1st movement's 1st frame
                do
                {

                    MovementData *movData = animationData->getMovement(animationData->m_aMovementNames.at(0).c_str());
                    CC_BREAK_IF(!movData);

                    MovementBoneData *movBoneData = movData->getMovementBoneData(bone->getName().c_str());
                    CC_BREAK_IF(!movBoneData || movBoneData->m_tFrameList.count() <= 0);

                    FrameData *frameData = movBoneData->getFrameData(0);
                    CC_BREAK_IF(!frameData);

                    bone->getTweenData()->copy(frameData);
                    bone->changeDisplayByIndex(frameData->m_nDisplayIndex, false);
                }
                while (0);
            }

            update(0);
            updateOffsetPoint();
        }
        else
        {
            m_sName = "new_armature";
            m_pArmatureData = ArmatureData::create();
            m_pArmatureData->m_sName = m_sName;

            AnimationData *animationData = AnimationData::create();
            animationData->m_sName = m_sName;

            armatureDataManager->addArmatureData(m_sName.c_str(), m_pArmatureData);
            armatureDataManager->addAnimationData(m_sName.c_str(), animationData);

            m_pAnimation->setAnimationData(animationData);

        }

        setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

        unscheduleUpdate();
        scheduleUpdate();

        setCascadeOpacityEnabled(true);
        setCascadeColorEnabled(true);

        bRet = true;
    }
    while (0);

    return bRet;
}

bool Armature::init(const char *name, Bone *parentBone)
{
    m_pParentBone = parentBone;
    return init(name);
}


Bone *Armature::createBone(const char *boneName)
{
    Bone *existedBone = getBone(boneName);
    if(existedBone != nullptr)
        return existedBone;

    BoneData *boneData = (BoneData *)m_pArmatureData->getBoneData(boneName);
    std::string parentName = boneData->m_sParentName;

    Bone *bone = nullptr;

    if( parentName.length() != 0 )
    {
        createBone(parentName.c_str());
        bone = Bone::create(boneName);
        addBone(bone, parentName.c_str());
    }
    else
    {
        bone = Bone::create(boneName);
        addBone(bone, "");
    }

    bone->setBoneData(boneData);
    bone->getDisplayManager()->changeDisplayByIndex(-1, false);

    return bone;
}


void Armature::addBone(Bone *bone, const char *parentName)
{
    CCASSERT( bone != nullptr, "Argument must be non-nil");
    CCASSERT(m_pBoneDic->objectForKey(bone->getName()) == nullptr, "bone already added. It can't be added again");

    if (nullptr != parentName)
    {
        Bone *boneParent = (Bone *)m_pBoneDic->objectForKey(parentName);
        if (boneParent)
        {
            boneParent->addChildBone(bone);
        }
        else
        {
            m_pTopBoneList->addObject(bone);
        }
    }
    else
    {
        m_pTopBoneList->addObject(bone);
    }

    bone->setArmature(this);

    m_pBoneDic->setObject(bone, bone->getName());
    addChild(bone);
}


void Armature::removeBone(Bone *bone, bool recursion)
{
    CCASSERT(bone != nullptr, "bone must be added to the bone dictionary!");

    bone->setArmature(nullptr);
    bone->removeFromParent(recursion);

    if (m_pTopBoneList->containsObject(bone))
    {
        m_pTopBoneList->removeObject(bone);
    }
    m_pBoneDic->removeObjectForKey(bone->getName());
    removeChild(bone, true);
}


Bone *Armature::getBone(const char *name) const
{
    return (Bone *)m_pBoneDic->objectForKey(name);
}


void Armature::changeBoneParent(Bone *bone, const char *parentName)
{
    CCASSERT(bone != nullptr, "bone must be added to the bone dictionary!");

    if(bone->getParentBone())
    {
        bone->getParentBone()->getChildren()->removeObject(bone);
        bone->setParentBone(nullptr);
    }

    if (parentName != nullptr)
    {
        Bone *boneParent = (Bone *)m_pBoneDic->objectForKey(parentName);

        if (boneParent)
        {
            boneParent->addChildBone(bone);
            if (m_pTopBoneList->containsObject(bone))
            {
                m_pTopBoneList->removeObject(bone);
            }
        }
        else
        {
            m_pTopBoneList->addObject(bone);
        }
    }
}

Dictionary *Armature::getBoneDic() const
{
    return m_pBoneDic;
}

const AffineTransform& Armature::getNodeToParentTransform() const
{
    if (m_bTransformDirty)
    {
        m_bArmatureTransformDirty = true;

        // Translate values
        float x = m_tPosition.x;
        float y = m_tPosition.y;

        if (m_bIgnoreAnchorPointForPosition)
        {
            x += m_tAnchorPointInPoints.x;
            y += m_tAnchorPointInPoints.y;
        }

        // Rotation values
        // Change rotation code to handle X and Y
        // If we skew with the exact same value for both x and y then we're simply just rotating
        float cx = 1, sx = 0, cy = 1, sy = 0;
        if (m_fRotationX || m_fRotationY)
        {
            float radiansX = -CC_DEGREES_TO_RADIANS(m_fRotationX);
            float radiansY = -CC_DEGREES_TO_RADIANS(m_fRotationY);
            cx = kdCosf(radiansX);
            sx = kdSinf(radiansX);
            cy = kdCosf(radiansY);
            sy = kdSinf(radiansY);
        }

        // Add offset point
        x += cy * m_tOffsetPoint.x * m_fScaleX + -sx * m_tOffsetPoint.y * m_fScaleY;
        y += sy * m_tOffsetPoint.x * m_fScaleX + cx * m_tOffsetPoint.y * m_fScaleY;

        bool needsSkewMatrix = ( m_fSkewX || m_fSkewY );

        // optimization:
        // inline anchor point calculation if skew is not needed
        // Adjusted transform calculation for rotational skew
        if (! needsSkewMatrix && !m_tAnchorPointInPoints.equals(Point::ZERO))
        {
            x += cy * -m_tAnchorPointInPoints.x * m_fScaleX + -sx * -m_tAnchorPointInPoints.y * m_fScaleY;
            y += sy * -m_tAnchorPointInPoints.x * m_fScaleX +  cx * -m_tAnchorPointInPoints.y * m_fScaleY;
        }


        // Build Transform Matrix
        // Adjusted transform calculation for rotational skew
        m_tTransform = AffineTransformMake( cy * m_fScaleX,  sy * m_fScaleX,
                                              -sx * m_fScaleY, cx * m_fScaleY,
                                              x, y );

        // XXX: Try to inline skew
        // If skew is needed, apply skew and then anchor point
        if (needsSkewMatrix)
        {
            AffineTransform skewMatrix = AffineTransformMake(1.0f, kdTanf(CC_DEGREES_TO_RADIANS(m_fSkewY)),
                                           kdTanf(CC_DEGREES_TO_RADIANS(m_fSkewX)), 1.0f,
                                           0.0f, 0.0f );
            m_tTransform = AffineTransformConcat(skewMatrix, m_tTransform);

            // adjust anchor point
            if (!m_tAnchorPointInPoints.equals(Point::ZERO))
            {
                m_tTransform = AffineTransformTranslate(m_tTransform, -m_tAnchorPointInPoints.x, -m_tAnchorPointInPoints.y);
            }
        }

        if (m_bAdditionalTransformDirty)
        {
            m_tTransform = AffineTransformConcat(m_tTransform, m_tAdditionalTransform);
            m_bAdditionalTransformDirty = false;
        }

        m_bTransformDirty = false;
    }

    return m_tTransform;
}

void Armature::updateOffsetPoint()
{
    // Set contentsize and Calculate anchor point.
    Rect rect = getBoundingBox();
    setContentSize(rect.size);
    m_tOffsetPoint = Point(-rect.origin.x,  -rect.origin.y);
    if (rect.size.width != 0 && rect.size.height != 0)
    {
        setAnchorPoint(Point(m_tOffsetPoint.x / rect.size.width, m_tOffsetPoint.y / rect.size.height));
    }
}

void Armature::setAnimation(ArmatureAnimation *animation)
{
    m_pAnimation = animation;
}

ArmatureAnimation *Armature::getAnimation() const 
{
    return m_pAnimation;
}

bool Armature::getArmatureTransformDirty() const 
{
    return m_bArmatureTransformDirty;
}

void Armature::update(float dt)
{
    m_pAnimation->update(dt);

    for (auto object : *m_pTopBoneList)
    {
        static_cast<Bone*>(object)->update(dt);
    }

    m_bArmatureTransformDirty = false;
}

void Armature::draw()
{
    if (m_pParentBone == nullptr && m_pBatchNode == nullptr)
    {
        CC_NODE_DRAW_SETUP();
        GL::blendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
    }

    for (auto object : *m_pChildren)
    {
        if (Bone *bone = dynamic_cast<Bone *>(object))
        {
            Node *node = bone->getDisplayRenderNode();

            if (nullptr == node)
                continue;

            switch (bone->getDisplayRenderNodeType())
            {
            case CS_DISPLAY_SPRITE:
            {
                Skin *skin = static_cast<Skin *>(node);

                TextureAtlas *textureAtlas = skin->getTextureAtlas();
                BlendType blendType = bone->getBlendType();
                if(m_pAtlas != textureAtlas || blendType != BLEND_NORMAL)
                {
                    if (m_pAtlas)
                    {
                        m_pAtlas->drawQuads();
                        m_pAtlas->removeAllQuads();
                    }
                }

                m_pAtlas = textureAtlas;
                if (m_pAtlas->getCapacity() == m_pAtlas->getTotalQuads() && !m_pAtlas->resizeCapacity(m_pAtlas->getCapacity() * 2))
                    return;

                skin->updateTransform();

                if (blendType != BLEND_NORMAL)
                {
                    updateBlendType(blendType);
                    m_pAtlas->drawQuads();
                    m_pAtlas->removeAllQuads();
                    GL::blendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
                }
            }
            break;
            case CS_DISPLAY_ARMATURE:
            {
                Armature *armature = static_cast<Armature *>(node);

                TextureAtlas *textureAtlas = armature->getTextureAtlas();
                if(m_pAtlas != textureAtlas)
                {
                    if (m_pAtlas)
                    {
                        m_pAtlas->drawQuads();
                        m_pAtlas->removeAllQuads();
                    }
                }
                armature->draw();
                m_pAtlas = armature->getTextureAtlas();
            }
            break;
            default:
            {
                if (m_pAtlas)
                {
                    m_pAtlas->drawQuads();
                    m_pAtlas->removeAllQuads();
                }
                node->visit();

                CC_NODE_DRAW_SETUP();
                GL::blendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
            }
            break;
            }
        }
        else if(Node *node = dynamic_cast<Node *>(object))
        {
            if (m_pAtlas)
            {
                m_pAtlas->drawQuads();
                m_pAtlas->removeAllQuads();
            }
            node->visit();

            CC_NODE_DRAW_SETUP();
            GL::blendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
        }
    }

    if(m_pAtlas && !m_pBatchNode && m_pParentBone == nullptr)
    {
        m_pAtlas->drawQuads();
        m_pAtlas->removeAllQuads();
    }
}


void Armature::updateBlendType(BlendType blendType)
{
    BlendFunc blendFunc;
    switch (blendType)
    {
    case BLEND_NORMAL:
    {
        blendFunc.src = CC_BLEND_SRC;
        blendFunc.dst = CC_BLEND_DST;
    }
    break;
    case BLEND_ADD:
    {
        blendFunc.src = GL_SRC_ALPHA;
        blendFunc.dst = GL_ONE;
    }
    break;
    case BLEND_MULTIPLY:
    {
        blendFunc.src = GL_DST_COLOR;
        blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
    break;
    case BLEND_SCREEN:
    {
        blendFunc.src = GL_ONE;
        blendFunc.dst = GL_ONE_MINUS_SRC_COLOR;
    }
    break;
    default:
    {
        blendFunc.src = CC_BLEND_SRC;
        blendFunc.dst = CC_BLEND_DST;
    }
    break;
    }
    GL::blendFunc(blendFunc.src, blendFunc.dst);
}



void Armature::visit()
{
    // quick return if not visible. children won't be drawn.
    if (!m_bVisible)
    {
        return;
    }
    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
    }

    transform();
    sortAllChildren();
    draw();

    // reset for next frame
    m_nOrderOfArrival = 0;

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

Rect Armature::getBoundingBox() const
{
    float minx, miny, maxx, maxy = 0;

    bool first = true;

    Rect boundingBox = Rect(0, 0, 0, 0);

    for(auto object : *m_pChildren)
    {
        if (Bone *bone = dynamic_cast<Bone *>(object))
        {
            Rect r = bone->getDisplayManager()->getBoundingBox();

            if(first)
            {
                minx = r.getMinX();
                miny = r.getMinY();
                maxx = r.getMaxX();
                maxy = r.getMaxY();

                first = false;
            }
            else
            {
                minx = r.getMinX() < boundingBox.getMinX() ? r.getMinX() : boundingBox.getMinX();
                miny = r.getMinY() < boundingBox.getMinY() ? r.getMinY() : boundingBox.getMinY();
                maxx = r.getMaxX() > boundingBox.getMaxX() ? r.getMaxX() : boundingBox.getMaxX();
                maxy = r.getMaxY() > boundingBox.getMaxY() ? r.getMaxY() : boundingBox.getMaxY();
            }

            boundingBox.setRect(minx, miny, maxx - minx, maxy - miny);
        }
    }

    return RectApplyAffineTransform(boundingBox, getNodeToParentTransform());
}

Bone *Armature::getBoneAtPoint(float x, float y) const 
{
    int length = m_pChildren->count();
    Bone *bs;

    for(int i = length - 1; i >= 0; i--)
    {
        bs = static_cast<Bone*>( m_pChildren->getObjectAtIndex(i) );
        if(bs->getDisplayManager()->containPoint(x, y))
        {
            return bs;
        }
    }
    return nullptr;
}

TextureAtlas *Armature::getTexureAtlasWithTexture(Texture2D *texture) const
{
    int key = texture->getName();
    
    if (m_pParentBone && m_pParentBone->getArmature())
    {
        return m_pParentBone->getArmature()->getTexureAtlasWithTexture(texture);
    }
    else if (m_pBatchNode)
    {
        m_pBatchNode->getTexureAtlasWithTexture(texture);
    }
    
    TextureAtlas *atlas = static_cast<TextureAtlas *>(m_pTextureAtlasDic->objectForKey(key));
    if (atlas == nullptr)
    {
        atlas = TextureAtlas::createWithTexture(texture, 4);
        m_pTextureAtlasDic->setObject(atlas, key);
    }
    return atlas;
}

void Armature::setParentBone(Bone *parentBone)
{
    m_pParentBone = parentBone;
    
    DictElement *element = nullptr;
    CCDICT_FOREACH(m_pBoneDic, element)
    {
        Bone *bone = static_cast<Bone*>(element->getObject());
        bone->setArmature(this);
    }
}

Bone *Armature::getParentBone() const
{
    return m_pParentBone;
}

void Armature::setColliderFilter(ColliderFilter *filter)
{
    DictElement *element = nullptr;
    CCDICT_FOREACH(m_pBoneDic, element)
    {
        Bone *bone = static_cast<Bone*>(element->getObject());
        bone->setColliderFilter(filter);
    }
}

b2Body* Armature::getB2Body ( KDvoid ) const
{
    return m_pB2Body;
}

KDvoid Armature::setB2Body ( b2Body* pBody )
{
    if ( m_pB2Body == pBody )
    {
        return;
    }

    m_pB2Body = pBody;
    m_pB2Body->SetUserData ( this );

    for ( auto pObject: *m_pChildren )
    {
        if ( Bone* pBone = dynamic_cast<Bone*> ( pObject ) )
        {
            Array*	pDisplayList = pBone->getDisplayManager ( )->getDecorativeDisplayList ( );

            for ( auto pDisplayObject: *pDisplayList )
            {
                ColliderDetector*	pDetector = static_cast<DecorativeDisplay*> ( pDisplayObject )->getColliderDetector ( );
                if ( pDetector != nullptr )
                {
                    pDetector->setB2Body ( m_pB2Body );
                }
            }
        }
    }
}

b2Fixture* Armature::getB2ShapeList ( KDvoid )
{
    if ( m_pB2Body )
    {
        return m_pB2Body->GetFixtureList ( );
    }
    else
    {
        return nullptr;
    }
}

cpBody* Armature::getCPBody ( KDvoid ) const
{
    return m_pCPBody;
}

KDvoid Armature::setCPBody ( cpBody* body )
{
    if ( m_pCPBody == body )
    {
        return;
    }

    m_pCPBody = body;
    m_pCPBody->data = this;

    for ( auto pObject: *m_pChildren )
    {
        if ( Bone* pBone = dynamic_cast<Bone*> ( pObject ) )
        {
            Array*	pDisplayList = pBone->getDisplayManager ( )->getDecorativeDisplayList ( );

            for ( auto pDisplayObject: *pDisplayList )
            {
                ColliderDetector*	pDetector = static_cast<DecorativeDisplay*> ( pDisplayObject )->getColliderDetector ( );
                if ( pDetector != nullptr )
                {
                    pDetector->setCPBody ( m_pCPBody );
                }
            }
        }
    }
}

cpShape* Armature::getCPShapeList ( KDvoid )
{
    if ( m_pCPBody )
    {
        return m_pCPBody->shapeList;
    }
    else
    {
        return nullptr;
    }
}


}
