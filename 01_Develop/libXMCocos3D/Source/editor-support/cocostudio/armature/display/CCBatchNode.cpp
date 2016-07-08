/* -----------------------------------------------------------------------------------
 *
 *      File            CCBatchNode.cpp
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

#include "editor-support/cocostudio/armature/display/CCBatchNode.h"
#include "editor-support/cocostudio/armature/utils/CCArmatureDefine.h"
#include "editor-support/cocostudio/armature/CCArmature.h"
#include "editor-support/cocostudio/armature/display/CCSkin.h"
#include "2d/effects/CCGrid.h"
#include "2d/textures/CCTextureAtlas.h"
#include "shaders/CCShaderCache.h"
#include "XMKazmath/GL/matrix.h"

using namespace cocos2d;

namespace cocostudio {

BatchNode *BatchNode::create()
{
    BatchNode *batchNode = new BatchNode();
    if (batchNode && batchNode->init())
    {
        batchNode->autorelease();
        return batchNode;
    }
    CC_SAFE_DELETE(batchNode);
    return nullptr;
}

BatchNode::BatchNode()
    : m_pAtlas(nullptr)
    , m_pTextureAtlasDic(nullptr)
{
}

BatchNode::~BatchNode()
{
    CC_SAFE_RELEASE(m_pTextureAtlasDic);
}

bool BatchNode::init()
{
    bool ret = Node::init();
    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

    CC_SAFE_DELETE(m_pTextureAtlasDic);
    m_pTextureAtlasDic = new Dictionary();

    return ret;
}

void BatchNode::addChild(Node *pChild)
{
    Node::addChild(pChild);
}

void BatchNode::addChild(Node *child, int zOrder)
{
    Node::addChild(child, zOrder);
}

void BatchNode::addChild(Node *child, int zOrder, int tag)
{
    Node::addChild(child, zOrder, tag);
    Armature *armature = dynamic_cast<Armature *>(child);
    if (armature != nullptr)
    {
        armature->setBatchNode(this);

        const Dictionary *dict = armature->getBoneDic();
        DictElement *element = nullptr;
        CCDICT_FOREACH(dict, element)
        {
            Bone *bone = static_cast<Bone*>(element->getObject());

            Array *displayList = bone->getDisplayManager()->getDecorativeDisplayList();
            for(auto object : *displayList)
            {
                DecorativeDisplay *display = static_cast<DecorativeDisplay*>(object);
                                
                if (Skin *skin = dynamic_cast<Skin*>(display->getDisplay()))
                {
                    skin->setTextureAtlas(getTexureAtlasWithTexture(skin->getTexture()));
                }
            }
        }
    }
}

void BatchNode::removeChild(Node* child, bool cleanup)
{
    Armature *armature = dynamic_cast<Armature *>(child);
    if (armature != nullptr)
    {
        armature->setBatchNode(nullptr);
        
        const Dictionary *dict = armature->getBoneDic();
        DictElement *element = nullptr;
        CCDICT_FOREACH(dict, element)
        {
            Bone *bone = static_cast<Bone*>(element->getObject());
            
            Array *displayList = bone->getDisplayManager()->getDecorativeDisplayList();
            for(auto object : *displayList)
            {
                DecorativeDisplay *display = static_cast<DecorativeDisplay*>(object);
                
                if (Skin *skin = dynamic_cast<Skin*>(display->getDisplay()))
                {
                    skin->setTextureAtlas(armature->getTexureAtlasWithTexture(skin->getTexture()));
                }
            }
        }
    }

    Node::removeChild(child, cleanup);
}

void BatchNode::visit()
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

void BatchNode::draw()
{
    if (m_pChildren == nullptr)
    {
        return;
    }

    CC_NODE_DRAW_SETUP();

    for(auto object : *m_pChildren)
    {
        Armature *armature = dynamic_cast<Armature *>(object);
        if (armature)
        {
            armature->visit();
            m_pAtlas = armature->getTextureAtlas();
        }
        else
        {
            ((Node *)object)->visit();
        }
    }

    if (m_pAtlas)
    {
        m_pAtlas->drawQuads();
        m_pAtlas->removeAllQuads();
    }
}

TextureAtlas *BatchNode::getTexureAtlasWithTexture(Texture2D *texture) const
{
    int key = texture->getName();
    
    TextureAtlas *atlas = static_cast<TextureAtlas *>(m_pTextureAtlasDic->objectForKey(key));
    if (atlas == nullptr)
    {
        atlas = TextureAtlas::createWithTexture(texture, 4);
        m_pTextureAtlasDic->setObject(atlas, key);
    }
    return atlas;
}

}
