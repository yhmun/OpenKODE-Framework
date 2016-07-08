/* -----------------------------------------------------------------------------------
 *
 *      File            CCSkeleton.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      Esoteric Software. All rights reserved.
 *
 *         http://esotericsoftware.com/      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "editor-support/spine/CCSkeleton.h"
#include "editor-support/spine/spine-cocos2dx.h"
#include "shaders/CCShaderCache.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include <algorithm>

USING_NS_CC;
using std::min;
using std::max;

namespace spine {

CCSkeleton* CCSkeleton::createWithData (SkeletonData* skeletonData, bool ownsSkeletonData) {
    CCSkeleton* node = new CCSkeleton(skeletonData, ownsSkeletonData);
    node->autorelease();
    return node;
}

CCSkeleton* CCSkeleton::createWithFile (const char* skeletonDataFile, Atlas* atlas, float scale) {
    CCSkeleton* node = new CCSkeleton(skeletonDataFile, atlas, scale);
    node->autorelease();
    return node;
}

CCSkeleton* CCSkeleton::createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale) {
    CCSkeleton* node = new CCSkeleton(skeletonDataFile, atlasFile, scale);
    node->autorelease();
    return node;
}

void CCSkeleton::initialize () {
    m_pAtlas = 0;
    m_bDebugSlots = false;
    m_bDebugBones = false;
    m_fTimeScale = 1;

    m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    setOpacityModifyRGB(true);

    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    scheduleUpdate();
}

void CCSkeleton::setSkeletonData (SkeletonData *skeletonData, bool isOwnsSkeletonData) {
    m_pSkeleton = Skeleton_create(skeletonData);
    m_pRootBone = m_pSkeleton->bones[0];
    this->m_bOwnsSkeletonData = isOwnsSkeletonData;
}

CCSkeleton::CCSkeleton () {
    initialize();
}

CCSkeleton::CCSkeleton (SkeletonData *skeletonData, bool isOwnsSkeletonData) {
    initialize();

    setSkeletonData(skeletonData, isOwnsSkeletonData);
}

CCSkeleton::CCSkeleton (const char* skeletonDataFile, Atlas* aAtlas, float scale) {
    initialize();

    SkeletonJson* json = SkeletonJson_create(aAtlas);
    json->scale = scale;
    SkeletonData* skeletonData = SkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
    CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data.");
    SkeletonJson_dispose(json);

    setSkeletonData(skeletonData, true);
}

CCSkeleton::CCSkeleton (const char* skeletonDataFile, const char* atlasFile, float scale) {
    initialize();

    m_pAtlas = Atlas_readAtlasFile(atlasFile);
    CCASSERT(m_pAtlas, "Error reading atlas file.");

    SkeletonJson* json = SkeletonJson_create(m_pAtlas);
    json->scale = scale;
    SkeletonData* skeletonData = SkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
    CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
    SkeletonJson_dispose(json);

    setSkeletonData(skeletonData, true);
}

CCSkeleton::~CCSkeleton () {
    if (m_bOwnsSkeletonData) SkeletonData_dispose(m_pSkeleton->data);
    if (m_pAtlas) Atlas_dispose(m_pAtlas);
    Skeleton_dispose(m_pSkeleton);
}

void CCSkeleton::update (float deltaTime) {
    Skeleton_update(m_pSkeleton, deltaTime * m_fTimeScale);
}

void CCSkeleton::draw () 
{
    CC_NODE_DRAW_SETUP();
    GL::blendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
    Color3B color = getColor();
    m_pSkeleton->r = color.r / (float)255;
    m_pSkeleton->g = color.g / (float)255;
    m_pSkeleton->b = color.b / (float)255;
    m_pSkeleton->a = getOpacity() / (float)255;
    if (m_bPremultipliedAlpha) 
    {
        m_pSkeleton->r *= m_pSkeleton->a;
        m_pSkeleton->g *= m_pSkeleton->a;
        m_pSkeleton->b *= m_pSkeleton->a;
    }

    TextureAtlas* textureAtlas = 0;
    V3F_C4B_T2F_Quad quad;
    quad.tl.vertices.z = 0;
    quad.tr.vertices.z = 0;
    quad.bl.vertices.z = 0;
    quad.br.vertices.z = 0;
    for (int i = 0, n = m_pSkeleton->slotCount; i < n; i++) 
    {
        Slot* slot = m_pSkeleton->slots[i];
        if (!slot->attachment || slot->attachment->type != ATTACHMENT_REGION) continue;
        RegionAttachment* attachment = (RegionAttachment*)slot->attachment;
        TextureAtlas* regionTextureAtlas = getTextureAtlas(attachment);
        if (regionTextureAtlas != textureAtlas) 
        {
            if (textureAtlas) 
            {
                if(textureAtlas->getTexture() && textureAtlas->getTexture()->hasPremultipliedAlpha())
                {
                    GL::blendFunc(BlendFunc::ALPHA_PREMULTIPLIED.src, BlendFunc::ALPHA_PREMULTIPLIED.dst);
                }
                else
                {
                    GL::blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED.src, BlendFunc::ALPHA_NON_PREMULTIPLIED.dst);
                }
                textureAtlas->drawQuads();
                textureAtlas->removeAllQuads();
            }
        }
        textureAtlas = regionTextureAtlas;
        if (textureAtlas->getCapacity() == textureAtlas->getTotalQuads() &&
            !textureAtlas->resizeCapacity(textureAtlas->getCapacity() * 2)) return;
        RegionAttachment_updateQuad(attachment, slot, &quad, m_bPremultipliedAlpha);
        textureAtlas->updateQuad(&quad, textureAtlas->getTotalQuads());
    }
    if (textureAtlas) 
    {
        if(textureAtlas->getTexture() && textureAtlas->getTexture()->hasPremultipliedAlpha())
        {
            GL::blendFunc(BlendFunc::ALPHA_PREMULTIPLIED.src, BlendFunc::ALPHA_PREMULTIPLIED.dst);
        }
        else
        {
            GL::blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED.src, BlendFunc::ALPHA_NON_PREMULTIPLIED.dst);
        }
        textureAtlas->drawQuads();
        textureAtlas->removeAllQuads();
    }

    if (m_bDebugSlots) 
    {
        // Slots.
        DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
        glLineWidth(1);
        Point points[4];
        V3F_C4B_T2F_Quad tmpQuad;
        for (int i = 0, n = m_pSkeleton->slotCount; i < n; i++) 
        {
            Slot* slot = m_pSkeleton->slots[i];
            if (!slot->attachment || slot->attachment->type != ATTACHMENT_REGION) continue;
            RegionAttachment* attachment = (RegionAttachment*)slot->attachment;
            RegionAttachment_updateQuad(attachment, slot, &tmpQuad);
            points[0] = Point(tmpQuad.bl.vertices.x, tmpQuad.bl.vertices.y);
            points[1] = Point(tmpQuad.br.vertices.x, tmpQuad.br.vertices.y);
            points[2] = Point(tmpQuad.tr.vertices.x, tmpQuad.tr.vertices.y);
            points[3] = Point(tmpQuad.tl.vertices.x, tmpQuad.tl.vertices.y);
            DrawPrimitives::drawPoly(points, 4, true);
        }
    }
    if (m_bDebugBones) 
    {
        // Bone lengths.
        glLineWidth(2);
        DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
        for (int i = 0, n = m_pSkeleton->boneCount; i < n; i++) 
        {
            Bone *bone = m_pSkeleton->bones[i];
            float x = bone->data->length * bone->m00 + bone->worldX;
            float y = bone->data->length * bone->m10 + bone->worldY;
            DrawPrimitives::drawLine(Point(bone->worldX, bone->worldY), Point(x, y));
        }
        // Bone origins.
        DrawPrimitives::setPointSize(4);
        DrawPrimitives::setDrawColor4B(0, 0, 255, 255); // Root bone is blue.
        for (int i = 0, n = m_pSkeleton->boneCount; i < n; i++) 
        {
            Bone *bone = m_pSkeleton->bones[i];
            DrawPrimitives::drawPoint(Point(bone->worldX, bone->worldY));
            if (i == 0) DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
        }
    }
}

TextureAtlas* CCSkeleton::getTextureAtlas (RegionAttachment* regionAttachment) const {
    return (TextureAtlas*)((AtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
}

Rect CCSkeleton::getBoundingBox() const {
    float minX = KD_FLT_MAX, minY = KD_FLT_MAX, maxX = KD_FLT_MIN, maxY = KD_FLT_MIN;
    float scaleX = getScaleX();
    float scaleY = getScaleY();
    float vertices[8];
    for (int i = 0; i < m_pSkeleton->slotCount; ++i) {
        Slot* slot = m_pSkeleton->slots[i];
        if (!slot->attachment || slot->attachment->type != ATTACHMENT_REGION) continue;
        RegionAttachment* attachment = (RegionAttachment*)slot->attachment;
        RegionAttachment_computeVertices(attachment, slot->skeleton->x, slot->skeleton->y, slot->bone, vertices);
        minX = min(minX, vertices[VERTEX_X1] * scaleX);
        minY = min(minY, vertices[VERTEX_Y1] * scaleY);
        maxX = max(maxX, vertices[VERTEX_X1] * scaleX);
        maxY = max(maxY, vertices[VERTEX_Y1] * scaleY);
        minX = min(minX, vertices[VERTEX_X4] * scaleX);
        minY = min(minY, vertices[VERTEX_Y4] * scaleY);
        maxX = max(maxX, vertices[VERTEX_X4] * scaleX);
        maxY = max(maxY, vertices[VERTEX_Y4] * scaleY);
        minX = min(minX, vertices[VERTEX_X2] * scaleX);
        minY = min(minY, vertices[VERTEX_Y2] * scaleY);
        maxX = max(maxX, vertices[VERTEX_X2] * scaleX);
        maxY = max(maxY, vertices[VERTEX_Y2] * scaleY);
        minX = min(minX, vertices[VERTEX_X3] * scaleX);
        minY = min(minY, vertices[VERTEX_Y3] * scaleY);
        maxX = max(maxX, vertices[VERTEX_X3] * scaleX);
        maxY = max(maxY, vertices[VERTEX_Y3] * scaleY);
    }
    Point position = getPosition();
    return Rect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
}

// --- Convenience methods for Skeleton_* functions.

void CCSkeleton::updateWorldTransform () {
    Skeleton_updateWorldTransform(m_pSkeleton);
}

void CCSkeleton::setToSetupPose () {
    Skeleton_setToSetupPose(m_pSkeleton);
}
void CCSkeleton::setBonesToSetupPose () {
    Skeleton_setBonesToSetupPose(m_pSkeleton);
}
void CCSkeleton::setSlotsToSetupPose () {
    Skeleton_setSlotsToSetupPose(m_pSkeleton);
}

Bone* CCSkeleton::findBone (const char* boneName) const {
    return Skeleton_findBone(m_pSkeleton, boneName);
}

Slot* CCSkeleton::findSlot (const char* slotName) const {
    return Skeleton_findSlot(m_pSkeleton, slotName);
}

bool CCSkeleton::setSkin (const char* skinName) {
    return Skeleton_setSkinByName(m_pSkeleton, skinName) ? true : false;
}

Attachment* CCSkeleton::getAttachment (const char* slotName, const char* attachmentName) const {
    return Skeleton_getAttachmentForSlotName(m_pSkeleton, slotName, attachmentName);
}
bool CCSkeleton::setAttachment (const char* slotName, const char* attachmentName) {
    return Skeleton_setAttachment(m_pSkeleton, slotName, attachmentName) ? true : false;
}

// --- BlendProtocol

const BlendFunc& CCSkeleton::getBlendFunc() const
{
    return m_tBlendFunc;
}

void CCSkeleton::setBlendFunc( const BlendFunc &aBlendFunc) {
    this->m_tBlendFunc = aBlendFunc;
}

void CCSkeleton::setOpacityModifyRGB (bool value) {
    m_bPremultipliedAlpha = value;
}

bool CCSkeleton::isOpacityModifyRGB () const {
    return m_bPremultipliedAlpha;
}

} // namespace spine {
