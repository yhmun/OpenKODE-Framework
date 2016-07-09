/* -----------------------------------------------------------------------------------
 *
 *      File            CCSkin.cpp
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

#include "editor-support/cocostudio/armature/display/CCSkin.h"
#include "editor-support/cocostudio/armature/utils/CCTransformHelp.h"
#include "editor-support/cocostudio/armature/utils/CCSpriteFrameCacheHelper.h"
#include "editor-support/cocostudio/armature/CCArmature.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"

using namespace cocos2d;

namespace cocostudio {

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

#define SET_VERTEX3F(_v_, _x_, _y_, _z_) (_v_).x = (_x_); (_v_).y = (_y_); (_v_).z = (_z_);

Skin *Skin::create()
{
    Skin *skin = new Skin();
    if(skin && skin->init())
    {
        skin->autorelease();
        return skin;
    }
    CC_SAFE_DELETE(skin);
    return nullptr;
}

Skin *Skin::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    Skin *skin = new Skin();
    if(skin && skin->initWithSpriteFrameName(pszSpriteFrameName))
    {
        skin->autorelease();
        return skin;
    }
    CC_SAFE_DELETE(skin);
    return nullptr;
}

Skin *Skin::create(const char *pszFileName)
{
    Skin *skin = new Skin();
    if(skin && skin->initWithFile(pszFileName))
    {
        skin->autorelease();
        return skin;
    }
    CC_SAFE_DELETE(skin);
    return nullptr;
}

Skin::Skin()
    : m_pBone(nullptr)
    , m_pArmature(nullptr)
    , m_sDisplayName("")
{
    m_tSkinTransform = AffineTransformIdentity;
}

bool Skin::initWithSpriteFrameName(const std::string& spriteFrameName)
{
    CCAssert(spriteFrameName != "", "");

    SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    bool ret = true;

    if (pFrame != nullptr)
    {
        ret = initWithSpriteFrame(pFrame);
    }
    else
    {
        CCLOG("Cann't find CCSpriteFrame with %s. Please check your .plist file", spriteFrameName.c_str());
        ret = false;
    }

    m_sDisplayName = spriteFrameName;

    return ret;
}

bool Skin::initWithFile(const std::string& filename)
{
    bool ret = Sprite::initWithFile(filename);

    m_sDisplayName = filename;

    return ret;
}

void Skin::setSkinData(const BaseData &var)
{
    m_tSkinData = var;

    setScaleX(m_tSkinData.m_fScaleX);
    setScaleY(m_tSkinData.m_fScaleY);
    setRotationX(CC_RADIANS_TO_DEGREES(m_tSkinData.m_fSkewX));
    setRotationY(CC_RADIANS_TO_DEGREES(-m_tSkinData.m_fSkewY));
    setPosition(Point(m_tSkinData.x, m_tSkinData.y));

    m_tSkinTransform = getNodeToParentTransform();
    updateArmatureTransform();
}

const BaseData &Skin::getSkinData() const
{
    return m_tSkinData;
}

void Skin::updateArmatureTransform()
{
    m_tTransform = AffineTransformConcat(m_tSkinTransform, m_pBone->getNodeToArmatureTransform());
    if(m_pArmature && m_pArmature->getBatchNode())
    {
        m_tTransform = AffineTransformConcat(m_tTransform, m_pArmature->getNodeToParentTransform());
    }
}

void Skin::updateTransform()
{
    // If it is not visible, or one of its ancestors is not visible, then do nothing:
    if( !m_bVisible)
    {
        m_tQuad.br.vertices = m_tQuad.tl.vertices = m_tQuad.tr.vertices = m_tQuad.bl.vertices = Vertex3F(0, 0, 0);
    }
    else
    {
        //
        // calculate the Quad based on the Affine Matrix
        //

        Size &size = m_tRect.size;

        float x1 = m_tOffsetPosition.x;
        float y1 = m_tOffsetPosition.y;

        float x2 = x1 + size.width;
        float y2 = y1 + size.height;

        float x = m_tTransform.tx;
        float y = m_tTransform.ty;

        float cr = m_tTransform.a;
        float sr = m_tTransform.b;
        float cr2 = m_tTransform.d;
        float sr2 = -m_tTransform.c;
        float ax = x1 * cr - y1 * sr2 + x;
        float ay = x1 * sr + y1 * cr2 + y;

        float bx = x2 * cr - y1 * sr2 + x;
        float by = x2 * sr + y1 * cr2 + y;

        float cx = x2 * cr - y2 * sr2 + x;
        float cy = x2 * sr + y2 * cr2 + y;

        float dx = x1 * cr - y2 * sr2 + x;
        float dy = x1 * sr + y2 * cr2 + y;

        SET_VERTEX3F( m_tQuad.bl.vertices, RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), m_fVertexZ );
        SET_VERTEX3F( m_tQuad.br.vertices, RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), m_fVertexZ );
        SET_VERTEX3F( m_tQuad.tl.vertices, RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), m_fVertexZ );
        SET_VERTEX3F( m_tQuad.tr.vertices, RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), m_fVertexZ );
    }

    // MARMALADE CHANGE: ADDED CHECK FOR nullptr, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
    if (m_pTextureAtlas)
    {
        m_pTextureAtlas->updateQuad(&m_tQuad, m_pTextureAtlas->getTotalQuads());
    }
}

AffineTransform Skin::getNodeToWorldTransform() const
{
    return AffineTransformConcat(m_tTransform, m_pBone->getArmature()->getNodeToWorldTransform());
}

AffineTransform Skin::getNodeToWorldTransformAR() const
{
    AffineTransform displayTransform = m_tTransform;
    Point anchorPoint =  m_tAnchorPointInPoints;

    anchorPoint = PointApplyAffineTransform(anchorPoint, displayTransform);

    displayTransform.tx = anchorPoint.x;
    displayTransform.ty = anchorPoint.y;

    return AffineTransformConcat(displayTransform, m_pBone->getArmature()->getNodeToWorldTransform());
}

void Skin::setBone(Bone *bone)
{
    m_pBone = bone;
    if(Armature *armature = m_pBone->getArmature())
    {
        m_pArmature = armature;
        TextureAtlas *atlas = armature->getTexureAtlasWithTexture(m_pTexture);
        setTextureAtlas(atlas);
    }
}

Bone *Skin::getBone() const
{
    return m_pBone;
}

}
