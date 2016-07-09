/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleBatchNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Matt Oswald
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *      Copyright (c) 2011      Marco Tillemans
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

#include "2d/particle_nodes/CCParticleBatchNode.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/textures/CCTextureAtlas.h"
#include "ccConfig.h"
#include "2d/effects/CCGrid.h"
#include "2d/particle_nodes/CCParticleSystem.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "support/base64.h"
#include "support/zip_support/ZipUtils.h"
#include "platform/CCFileUtils.h"
#include "support/CCProfiling.h"
#include "XMKazmath/GL/matrix.h"

NS_CC_BEGIN

ParticleBatchNode::ParticleBatchNode()
: m_pTextureAtlas(NULL)
{

}

ParticleBatchNode::~ParticleBatchNode()
{
    CC_SAFE_RELEASE(m_pTextureAtlas);
}
/*
 * creation with Texture2D
 */

ParticleBatchNode* ParticleBatchNode::createWithTexture(Texture2D *tex, unsigned int capacity/* = kParticleDefaultCapacity*/)
{
    ParticleBatchNode * p = new ParticleBatchNode();
    if( p && p->initWithTexture(tex, capacity))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return NULL;
}

/*
 * creation with File Image
 */

ParticleBatchNode* ParticleBatchNode::create(const char* imageFile, unsigned int capacity/* = kParticleDefaultCapacity*/)
{
    ParticleBatchNode * p = new ParticleBatchNode();
    if( p && p->initWithFile(imageFile, capacity))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return NULL;
}

/*
 * init with Texture2D
 */
bool ParticleBatchNode::initWithTexture(Texture2D *tex, unsigned int capacity)
{
    m_pTextureAtlas = new TextureAtlas();
    m_pTextureAtlas->initWithTexture(tex, capacity);

    // no lazy alloc in this node
    m_pChildren = new Array();
    m_pChildren->initWithCapacity(capacity);

    m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    
    return true;
}

/*
 * init with FileImage
 */
bool ParticleBatchNode::initWithFile(const char* fileImage, unsigned int capacity)
{
    Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(fileImage);
    return initWithTexture(tex, capacity);
}

// ParticleBatchNode - composition

// override visit.
// Don't call visit on it's children
void ParticleBatchNode::visit()
{
    // CAREFUL:
    // This visit is almost identical to Node#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void Sprite#visit, but
    // although this is less maintainable, is faster
    //
    if (!m_bVisible)
    {
        return;
    }

    kmGLPushMatrix();

    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        transformAncestors();
    }

    transform();

    draw();
    
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

// override addChild:
void ParticleBatchNode::addChild(Node * child)
{
    Node::addChild(child);
}

void ParticleBatchNode::addChild(Node * child, int zOrder)
{
    Node::addChild(child, zOrder);
}

void ParticleBatchNode::addChild(Node * aChild, int zOrder, int tag)
{
    CCASSERT( aChild != NULL, "Argument must be non-NULL");
    CCASSERT( dynamic_cast<ParticleSystem*>(aChild) != NULL, "CCParticleBatchNode only supports QuadParticleSystems as children");
    ParticleSystem* child = static_cast<ParticleSystem*>(aChild);
    CCASSERT( child->getTexture()->getName() == m_pTextureAtlas->getTexture()->getName(), "CCParticleSystem is not using the same texture id");
    // If this is the 1st children, then copy blending function
    if( m_pChildren->count() == 0 ) 
    {
        setBlendFunc(child->getBlendFunc());
    }

    CCASSERT( m_tBlendFunc.src  == child->getBlendFunc().src && m_tBlendFunc.dst  == child->getBlendFunc().dst, "Can't add a PaticleSystem that uses a different blending function");

    //no lazy sorting, so don't call super addChild, call helper instead
    unsigned int pos = addChildHelper(child,zOrder,tag);

    //get new atlasIndex
    int atlasIndex = 0;

    if (pos != 0) 
    {
        ParticleSystem* p = (ParticleSystem*)m_pChildren->getObjectAtIndex(pos-1);
        atlasIndex = p->getAtlasIndex() + p->getTotalParticles();

    }
    else
    {
        atlasIndex = 0;
    }

    insertChild(child, atlasIndex);

    // update quad info
    child->setBatchNode(this);
}

// don't use lazy sorting, reordering the particle systems quads afterwards would be too complex
// XXX research whether lazy sorting + freeing current quads and calloc a new block with size of capacity would be faster
// XXX or possibly using vertexZ for reordering, that would be fastest
// this helper is almost equivalent to Node's addChild, but doesn't make use of the lazy sorting
unsigned int ParticleBatchNode::addChildHelper(ParticleSystem* child, int z, int aTag)
{
    CCASSERT( child != NULL, "Argument must be non-nil");
    CCASSERT( child->getParent() == NULL, "child already added. It can't be added again");

    if( ! m_pChildren ) 
    {
        m_pChildren = new Array();
        m_pChildren->initWithCapacity(4);
    }

    //don't use a lazy insert
    unsigned int pos = searchNewPositionInChildrenForZ(z);

    m_pChildren->insertObject(child, pos);

    child->setTag(aTag);
    child->_setZOrder(z);

    child->setParent(this);

    if( m_bRunning ) 
    {
        child->onEnter();
        child->onEnterTransitionDidFinish();
    }
    return pos;
}

// Reorder will be done in this function, no "lazy" reorder to particles
void ParticleBatchNode::reorderChild(Node * aChild, int zOrder)
{
    CCASSERT( aChild != NULL, "Child must be non-NULL");
    CCASSERT( dynamic_cast<ParticleSystem*>(aChild) != NULL, "CCParticleBatchNode only supports QuadParticleSystems as children");
    CCASSERT( m_pChildren->containsObject(aChild), "Child doesn't belong to batch" );

    ParticleSystem* child = static_cast<ParticleSystem*>(aChild);

    if( zOrder == child->getZOrder() ) 
    {
        return;
    }

    // no reordering if only 1 child
    if( m_pChildren->count() > 1)
    {
        unsigned int newIndex = 0, oldIndex = 0;

        getCurrentIndex(&oldIndex, &newIndex, child, zOrder);

        if( oldIndex != newIndex )
        {

            // reorder _children->array
            child->retain();
            m_pChildren->removeObjectAtIndex(oldIndex);
            m_pChildren->insertObject(child, newIndex);
            child->release();

            // save old altasIndex
            int oldAtlasIndex = child->getAtlasIndex();

            // update atlas index
            updateAllAtlasIndexes();

            // Find new AtlasIndex
            int newAtlasIndex = 0;
            for( int i=0;i < m_pChildren->count();i++)
            {
                ParticleSystem* pNode = (ParticleSystem*)m_pChildren->getObjectAtIndex(i);
                if( pNode == child ) 
                {
                    newAtlasIndex = child->getAtlasIndex();
                    break;
                }
            }

            // reorder textureAtlas quads
            m_pTextureAtlas->moveQuadsFromIndex(oldAtlasIndex, child->getTotalParticles(), newAtlasIndex);

            child->updateWithNoTime();
        }
    }

    child->_setZOrder(zOrder);
}

void ParticleBatchNode::getCurrentIndex(unsigned int* oldIndex, unsigned int* newIndex, Node* child, int z)
{
    bool foundCurrentIdx = false;
    bool foundNewIdx = false;

    int  minusOne = 0;
    unsigned int count = m_pChildren->count();

    for( unsigned int i=0; i < count; i++ ) 
    {
        Node* pNode = (Node *)m_pChildren->getObjectAtIndex(i);

        // new index
        if( pNode->getZOrder() > z &&  ! foundNewIdx ) 
        {
            *newIndex = i;
            foundNewIdx = true;

            if( foundCurrentIdx && foundNewIdx )
            {
                break;
            }
        }

        // current index
        if( child == pNode ) 
        {
            *oldIndex = i;
            foundCurrentIdx = true;

            if( ! foundNewIdx )
            {
                minusOne = -1;
            }

            if( foundCurrentIdx && foundNewIdx )
            {
                break;
            }
        }

    }

    if( ! foundNewIdx )
    {
        *newIndex = count;
    }

    *newIndex += minusOne;
}

unsigned int ParticleBatchNode::searchNewPositionInChildrenForZ(int z)
{
    unsigned int count = m_pChildren->count();

    for( unsigned int i=0; i < count; i++ ) 
    {
        Node *child = (Node *)m_pChildren->getObjectAtIndex(i);
        if (child->getZOrder() > z)
        {
            return i;
        }
    }
    return count;
}

// override removeChild:
void  ParticleBatchNode::removeChild(Node* aChild, bool cleanup)
{
    // explicit nil handling
    if (aChild == NULL)
        return;
    
    CCASSERT( dynamic_cast<ParticleSystem*>(aChild) != NULL, "CCParticleBatchNode only supports QuadParticleSystems as children");
    CCASSERT(m_pChildren->containsObject(aChild), "CCParticleBatchNode doesn't contain the sprite. Can't remove it");

    ParticleSystem* child = static_cast<ParticleSystem*>(aChild);
    Node::removeChild(child, cleanup);

    // remove child helper
    m_pTextureAtlas->removeQuadsAtIndex(child->getAtlasIndex(), child->getTotalParticles());

    // after memmove of data, empty the quads at the end of array
    m_pTextureAtlas->fillWithEmptyQuadsFromIndex(m_pTextureAtlas->getTotalQuads(), child->getTotalParticles());

    // particle could be reused for self rendering
    child->setBatchNode(NULL);

    updateAllAtlasIndexes();
}

void ParticleBatchNode::removeChildAtIndex(unsigned int index, bool doCleanup)
{
    removeChild((ParticleSystem *)m_pChildren->getObjectAtIndex(index),doCleanup);
}

void ParticleBatchNode::removeAllChildrenWithCleanup(bool doCleanup)
{
    arrayMakeObjectsPerformSelectorWithObject(m_pChildren, setBatchNode, NULL, ParticleSystem*);

    Node::removeAllChildrenWithCleanup(doCleanup);

    m_pTextureAtlas->removeAllQuads();
}

void ParticleBatchNode::draw(void)
{
    CC_PROFILER_START("CCParticleBatchNode - draw");

    if( m_pTextureAtlas->getTotalQuads() == 0 )
    {
        return;
    }

    CC_NODE_DRAW_SETUP();

    GL::blendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    m_pTextureAtlas->drawQuads();

    CC_PROFILER_STOP("CCParticleBatchNode - draw");
}



void ParticleBatchNode::increaseAtlasCapacityTo(unsigned int quantity)
{
    CCLOG("cocos2d: ParticleBatchNode: resizing TextureAtlas capacity from [%d] to [%u].",
          m_pTextureAtlas->getCapacity(),
          quantity);

    if( ! m_pTextureAtlas->resizeCapacity(quantity) ) {
        // serious problems
        CCLOGWARN("cocos2d: WARNING: Not enough memory to resize the atlas");
        CCASSERT(false,"XXX: ParticleBatchNode #increaseAtlasCapacity SHALL handle this assert");
    }
}

//sets a 0'd quad into the quads array
void ParticleBatchNode::disableParticle(unsigned int particleIndex)
{
    V3F_C4B_T2F_Quad* quad = &((m_pTextureAtlas->getQuads())[particleIndex]);
    quad->br.vertices.x = quad->br.vertices.y = quad->tr.vertices.x = quad->tr.vertices.y = quad->tl.vertices.x = quad->tl.vertices.y = quad->bl.vertices.x = quad->bl.vertices.y = 0.0f;
}

// ParticleBatchNode - add / remove / reorder helper methods

// add child helper
void ParticleBatchNode::insertChild(ParticleSystem* system, int index)
{
    system->setAtlasIndex(index);

    if(m_pTextureAtlas->getTotalQuads() + system->getTotalParticles() > m_pTextureAtlas->getCapacity())
    {
        increaseAtlasCapacityTo(m_pTextureAtlas->getTotalQuads() + system->getTotalParticles());

        // after a realloc empty quads of textureAtlas can be filled with gibberish (realloc doesn't perform calloc), insert empty quads to prevent it
        m_pTextureAtlas->fillWithEmptyQuadsFromIndex(m_pTextureAtlas->getCapacity() - system->getTotalParticles(), system->getTotalParticles());
    }

    // make room for quads, not necessary for last child
    if (system->getAtlasIndex() + system->getTotalParticles() != m_pTextureAtlas->getTotalQuads())
    {
        m_pTextureAtlas->moveQuadsFromIndex(index, index+system->getTotalParticles());
    }

    // increase totalParticles here for new particles, update method of particle-system will fill the quads
    m_pTextureAtlas->increaseTotalQuadsWith(system->getTotalParticles());

    updateAllAtlasIndexes();
}

//rebuild atlas indexes
void ParticleBatchNode::updateAllAtlasIndexes()
{
    Object *pObj = NULL;
    unsigned int index = 0;

    CCARRAY_FOREACH(m_pChildren,pObj)
    {
        ParticleSystem* child = static_cast<ParticleSystem*>(pObj);
        child->setAtlasIndex(index);
        index += child->getTotalParticles();
    }
}

// ParticleBatchNode - CocosNodeTexture protocol

void ParticleBatchNode::updateBlendFunc(void)
{
    if( ! m_pTextureAtlas->getTexture()->hasPremultipliedAlpha())
        m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
}

void ParticleBatchNode::setTexture(Texture2D* texture)
{
    m_pTextureAtlas->setTexture(texture);

    // If the new texture has No premultiplied alpha, AND the blendFunc hasn't been changed, then update it
    if( texture && ! texture->hasPremultipliedAlpha() && ( m_tBlendFunc.src == CC_BLEND_SRC && m_tBlendFunc.dst == CC_BLEND_DST ) )
    {
        m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
    }
}

Texture2D* ParticleBatchNode::getTexture(void) const
{
    return m_pTextureAtlas->getTexture();
}

void ParticleBatchNode::setBlendFunc(const BlendFunc &blendFunc)
{
    m_tBlendFunc = blendFunc;
}
// returns the blending function used for the texture
const BlendFunc& ParticleBatchNode::getBlendFunc(void) const
{
    return m_tBlendFunc;
}

NS_CC_END
