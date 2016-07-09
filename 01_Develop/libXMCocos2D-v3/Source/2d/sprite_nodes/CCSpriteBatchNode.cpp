/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteBatchNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009-2010 Ricardo Quesada
 *      Copyright (c) 2009      Matt Oswald
 *      Copyright (c) 2011      Zynga Inc.
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

#include "2d/sprite_nodes/CCSpriteBatchNode.h"
#include "ccConfig.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/effects/CCGrid.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "2d/textures/CCTextureCache.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "2d/CCDirector.h"
#include "support/TransformUtils.h"
#include "support/CCProfiling.h"
#include "2d/layers_scenes_transitions_nodes/CCLayer.h"
#include "2d/layers_scenes_transitions_nodes/CCScene.h"
// external
#include "XMKazmath/GL/matrix.h"

#include <algorithm>

NS_CC_BEGIN

/*
* creation with Texture2D
*/

SpriteBatchNode* SpriteBatchNode::createWithTexture(Texture2D* tex, int capacity/* = DEFAULT_CAPACITY*/)
{
    SpriteBatchNode *batchNode = new SpriteBatchNode();
    batchNode->initWithTexture(tex, capacity);
    batchNode->autorelease();

    return batchNode;
}

/*
* creation with File Image
*/

SpriteBatchNode* SpriteBatchNode::create(const char *fileImage, KDint32 capacity/* = DEFAULT_CAPACITY*/)
{
    SpriteBatchNode *batchNode = new SpriteBatchNode();
    batchNode->initWithFile(fileImage, capacity);
    batchNode->autorelease();

    return batchNode;
}

/*
* init with Texture2D
*/
bool SpriteBatchNode::initWithTexture(Texture2D *tex, KDint32 capacity)
{
    CCASSERT(capacity>=0, "Capacity must be >= 0");
    
    m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    m_pTextureAtlas = new TextureAtlas();

    if (capacity == 0)
    {
        capacity = DEFAULT_CAPACITY;
    }
    
    m_pTextureAtlas->initWithTexture(tex, capacity);

    updateBlendFunc();

    // no lazy alloc in this node
    m_pChildren = new Array();
    m_pChildren->initWithCapacity(capacity);

    m_aDescendants.reserve(capacity);

    setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    return true;
}

bool SpriteBatchNode::init()
{
    Texture2D * texture = new Texture2D();
    texture->autorelease();
    return this->initWithTexture(texture, 0);
}

/*
* init with FileImage
*/
bool SpriteBatchNode::initWithFile(const char* fileImage, KDint32 capacity)
{
    Texture2D *texture2D = Director::getInstance()->getTextureCache()->addImage(fileImage);
    return initWithTexture(texture2D, capacity);
}

SpriteBatchNode::SpriteBatchNode()
: m_pTextureAtlas(NULL)
{
}

SpriteBatchNode::~SpriteBatchNode()
{
    CC_SAFE_RELEASE(m_pTextureAtlas);
}

// override visit
// don't call visit on it's children
void SpriteBatchNode::visit(void)
{
    CC_PROFILER_START_CATEGORY(kProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");

    // CAREFUL:
    // This visit is almost identical to CocosNode#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void Sprite#visit, but
    // although this is less maintainable, is faster
    //
    if (! m_bVisible)
    {
        return;
    }

    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        transformAncestors();
    }

    sortAllChildren();
    transform();

    draw();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
    setOrderOfArrival(0);

    CC_PROFILER_STOP_CATEGORY(kProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");
}

void SpriteBatchNode::addChild(Node *child, int zOrder, int tag)
{
    CCASSERT(child != NULL, "child should not be null");
    CCASSERT(dynamic_cast<Sprite*>(child) != NULL, "CCSpriteBatchNode only supports Sprites as children");
    Sprite *sprite = static_cast<Sprite*>(child);
    // check Sprite is using the same texture id
    CCASSERT(sprite->getTexture()->getName() == m_pTextureAtlas->getTexture()->getName(), "CCSprite is not using the same texture id");

    Node::addChild(child, zOrder, tag);

    appendChild(sprite);
}

// override reorderChild
void SpriteBatchNode::reorderChild(Node *child, int zOrder)
{
    CCASSERT(child != NULL, "the child should not be null");
    CCASSERT(m_pChildren->containsObject(child), "Child doesn't belong to Sprite");

    if (zOrder == child->getZOrder())
    {
        return;
    }

    //set the z-order and sort later
    Node::reorderChild(child, zOrder);
}

// override remove child
void SpriteBatchNode::removeChild(Node *child, bool cleanup)
{
    Sprite *sprite = static_cast<Sprite*>(child);

    // explicit null handling
    if (sprite == NULL)
    {
        return;
    }

    CCASSERT(m_pChildren->containsObject(sprite), "sprite batch node should contain the child");

    // cleanup before removing
    removeSpriteFromAtlas(sprite);

    Node::removeChild(sprite, cleanup);
}

void SpriteBatchNode::removeChildAtIndex(int index, bool doCleanup)
{
    CCASSERT(index>=0 && index < m_pChildren->count(), "Invalid index");
    removeChild( static_cast<Sprite*>(m_pChildren->getObjectAtIndex(index)), doCleanup);
}

void SpriteBatchNode::removeAllChildrenWithCleanup(bool doCleanup)
{
    // Invalidate atlas index. issue #569
    // useSelfRender should be performed on all descendants. issue #1216
    std::for_each(m_aDescendants.begin(), m_aDescendants.end(), [](Sprite* sprite) {
        sprite->setBatchNode(nullptr);
    });

    Node::removeAllChildrenWithCleanup(doCleanup);

    m_aDescendants.clear();
    m_pTextureAtlas->removeAllQuads();
}

//override sortAllChildren
void SpriteBatchNode::sortAllChildren()
{
    if (m_bReorderChildDirty)
    {
#if 0
        int i = 0,j = 0,length = _children->count();

        // insertion sort
        for(i=1; i<length; i++)
        {
            j = i-1;
            auto tempI = static_cast<Node*>( _children->getObjectAtIndex(i) );
            auto tempJ = static_cast<Node*>( _children->getObjectAtIndex(j) );

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but mutatedIndex is smaller
            while(j>=0 && ( tempI->getZOrder() < tempJ->getZOrder() ||
                           ( tempI->getZOrder() == tempJ->getZOrder() &&
                            tempI->getOrderOfArrival() < tempJ->getOrderOfArrival() ) ) )
            {
                _children->fastSetObject( tempJ, j+1 );
                j = j-1;
                if(j>=0)
                    tempJ = static_cast<Node*>( _children->getObjectAtIndex(j) );
            }
            _children->fastSetObject(tempI, j+1);
        }
#else
        std::sort(std::begin(*m_pChildren), std::end(*m_pChildren), nodeComparisonLess);
#endif

        //sorted now check all children
        if (m_pChildren->count() > 0)
        {
            //first sort all children recursively based on zOrder
            arrayMakeObjectsPerformSelector(m_pChildren, sortAllChildren, Sprite*);

            int index=0;

            Object* obj = NULL;
            //fast dispatch, give every child a new atlasIndex based on their relative zOrder (keep parent -> child relations intact)
            // and at the same time reorder descendants and the quads to the right index
            CCARRAY_FOREACH(m_pChildren, obj)
            {
                Sprite* child = static_cast<Sprite*>(obj);
                updateAtlasIndex(child, &index);
            }
        }

        m_bReorderChildDirty=false;
    }
}

void SpriteBatchNode::updateAtlasIndex(Sprite* sprite, int* curIndex)
{
    int count = 0;
    Array* array = sprite->getChildren();
    if (array != NULL)
    {
        count = array->count();
    }
    
    int oldIndex = 0;

    if( count == 0 )
    {
        oldIndex = sprite->getAtlasIndex();
        sprite->setAtlasIndex(*curIndex);
        sprite->setOrderOfArrival(0);
        if (oldIndex != *curIndex){
            swap(oldIndex, *curIndex);
        }
        (*curIndex)++;
    }
    else
    {
        bool needNewIndex=true;

        if (static_cast<Sprite*>(array->getObjectAtIndex(0) )->getZOrder() >= 0)
        {
            //all children are in front of the parent
            oldIndex = sprite->getAtlasIndex();
            sprite->setAtlasIndex(*curIndex);
            sprite->setOrderOfArrival(0);
            if (oldIndex != *curIndex)
            {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;

            needNewIndex = false;
        }

        Object* obj = NULL;
        CCARRAY_FOREACH(array,obj)
        {
            Sprite* child = static_cast<Sprite*>(obj);
            if (needNewIndex && child->getZOrder() >= 0)
            {
                oldIndex = sprite->getAtlasIndex();
                sprite->setAtlasIndex(*curIndex);
                sprite->setOrderOfArrival(0);
                if (oldIndex != *curIndex) {
                    this->swap(oldIndex, *curIndex);
                }
                (*curIndex)++;
                needNewIndex = false;

            }

            updateAtlasIndex(child, curIndex);
        }

        if (needNewIndex)
        {//all children have a zOrder < 0)
            oldIndex = sprite->getAtlasIndex();
            sprite->setAtlasIndex(*curIndex);
            sprite->setOrderOfArrival(0);
            if (oldIndex != *curIndex) {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;
        }
    }
}

void SpriteBatchNode::swap(int oldIndex, int newIndex)
{
    CCASSERT(oldIndex>=0 && oldIndex < (int)m_aDescendants.size() && newIndex >=0 && newIndex < (int)m_aDescendants.size(), "Invalid index");

    V3F_C4B_T2F_Quad* quads = m_pTextureAtlas->getQuads();
    std::swap( quads[oldIndex], quads[newIndex] );

    //update the index of other swapped item

    auto oldIt = std::next( m_aDescendants.begin(), oldIndex );
    auto newIt = std::next( m_aDescendants.begin(), newIndex );

    (*newIt)->setAtlasIndex(oldIndex);
//    (*oldIt)->setAtlasIndex(newIndex);

    std::swap( *oldIt, *newIt );
}

void SpriteBatchNode::reorderBatch(bool reorder)
{
    m_bReorderChildDirty=reorder;
}

// draw
void SpriteBatchNode::draw(void)
{
    CC_PROFILER_START("CCSpriteBatchNode - draw");

    // Optimization: Fast Dispatch
    if( m_pTextureAtlas->getTotalQuads() == 0 )
    {
        return;
    }

    CC_NODE_DRAW_SETUP();

    arrayMakeObjectsPerformSelector(m_pChildren, updateTransform, Sprite*);

    GL::blendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    m_pTextureAtlas->drawQuads();

    CC_PROFILER_STOP("CCSpriteBatchNode - draw");
}

void SpriteBatchNode::increaseAtlasCapacity(void)
{
    // if we're going beyond the current TextureAtlas's capacity,
    // all the previously initialized sprites will need to redo their texture coords
    // this is likely computationally expensive
    int quantity = (m_pTextureAtlas->getCapacity() + 1) * 4 / 3;

    CCLOG("cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [%d] to [%u].",
        m_pTextureAtlas->getCapacity(),
        quantity);

    if (! m_pTextureAtlas->resizeCapacity(quantity))
    {
        // serious problems
        CCLOGWARN("cocos2d: WARNING: Not enough memory to resize the atlas");
        CCASSERT(false, "Not enough memory to resize the atlas");
    }
}

int SpriteBatchNode::rebuildIndexInOrder(Sprite *parent, int index)
{
    CCASSERT(index>=0 && index < m_pChildren->count(), "Invalid index");

    Array *children = parent->getChildren();

    if (children && children->count() > 0)
    {
        Object* object = NULL;
        CCARRAY_FOREACH(children, object)
        {
            Sprite* child = static_cast<Sprite*>(object);
            if (child && (child->getZOrder() < 0))
            {
                index = rebuildIndexInOrder(child, index);
            }
        }
    }    

    // ignore self (batch node)
    if (! parent->isEqual(this))
    {
        parent->setAtlasIndex(index);
        index++;
    }

    if (children && children->count() > 0)
    {
        Object* object = NULL;
        CCARRAY_FOREACH(children, object)
        {
            Sprite* child = static_cast<Sprite*>(object);
            if (child && (child->getZOrder() >= 0))
            {
                index = rebuildIndexInOrder(child, index);
            }
        }
    }

    return index;
}

int SpriteBatchNode::highestAtlasIndexInChild(Sprite *sprite)
{
    Array *children = sprite->getChildren();

    if (! children || children->count() == 0)
    {
        return sprite->getAtlasIndex();
    }
    else
    {
        return highestAtlasIndexInChild( static_cast<Sprite*>(children->getLastObject()));
    }
}

int SpriteBatchNode::lowestAtlasIndexInChild(Sprite *sprite)
{
    Array *children = sprite->getChildren();

    if (! children || children->count() == 0)
    {
        return sprite->getAtlasIndex();
    }
    else
    {
        return lowestAtlasIndexInChild(static_cast<Sprite*>(children->getObjectAtIndex(0)));
    }
}

int SpriteBatchNode::atlasIndexForChild(Sprite *sprite, int nZ)
{
    Array *siblings = sprite->getParent()->getChildren();
    int childIndex = siblings->getIndexOfObject(sprite);

    // ignore parent Z if parent is spriteSheet
    bool ignoreParent = (SpriteBatchNode*)(sprite->getParent()) == this;
    Sprite *prev = NULL;
    if (childIndex > 0 && childIndex != -1)
    {
        prev = static_cast<Sprite*>(siblings->getObjectAtIndex(childIndex - 1));
    }

    // first child of the sprite sheet
    if (ignoreParent)
    {
        if (childIndex == 0)
        {
            return 0;
        }

        return highestAtlasIndexInChild(prev) + 1;
    }

    // parent is a Sprite, so, it must be taken into account

    // first child of an Sprite ?
    if (childIndex == 0)
    {
        Sprite *p = static_cast<Sprite*>(sprite->getParent());

        // less than parent and brothers
        if (nZ < 0)
        {
            return p->getAtlasIndex();
        }
        else
        {
            return p->getAtlasIndex() + 1;
        }
    }
    else
    {
        // previous & sprite belong to the same branch
        if ((prev->getZOrder() < 0 && nZ < 0) || (prev->getZOrder() >= 0 && nZ >= 0))
        {
            return highestAtlasIndexInChild(prev) + 1;
        }

        // else (previous < 0 and sprite >= 0 )
        Sprite *p = static_cast<Sprite*>(sprite->getParent());
        return p->getAtlasIndex() + 1;
    }

    // Should not happen. Error calculating Z on SpriteSheet
    CCASSERT(0, "should not run here");
    return 0;
}

// addChild helper, faster than insertChild
void SpriteBatchNode::appendChild(Sprite* sprite)
{
    m_bReorderChildDirty=true;
    sprite->setBatchNode(this);
    sprite->setDirty(true);

    if(m_pTextureAtlas->getTotalQuads() == m_pTextureAtlas->getCapacity()) {
        increaseAtlasCapacity();
    }

    m_aDescendants.push_back(sprite);
    int index = m_aDescendants.size()-1;

    sprite->setAtlasIndex(index);

    V3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pTextureAtlas->insertQuad(&quad, index);

    // add children recursively
    
    Object* obj = nullptr;
    CCARRAY_FOREACH(sprite->getChildren(), obj)
    {
        Sprite* child = static_cast<Sprite*>(obj);
        appendChild(child);
    }
}

void SpriteBatchNode::removeSpriteFromAtlas(Sprite *sprite)
{
    // remove from TextureAtlas
    m_pTextureAtlas->removeQuadAtIndex(sprite->getAtlasIndex());

    // Cleanup sprite. It might be reused (issue #569)
    sprite->setBatchNode(NULL);

    auto it = std::find(m_aDescendants.begin(), m_aDescendants.end(), sprite );
    if( it != m_aDescendants.end() )
    {
        auto next = std::next(it);

        std::for_each(next, m_aDescendants.end(), [](Sprite *spr) {
            spr->setAtlasIndex( spr->getAtlasIndex() - 1 );
        });

        m_aDescendants.erase(it);
    }

    // remove children recursively
    Array *children = sprite->getChildren();
    if (children && children->count() > 0)
    {
        Object* object = NULL;
        CCARRAY_FOREACH(children, object)
        {
            Sprite* child = static_cast<Sprite*>(object);
            if (child)
            {
                removeSpriteFromAtlas(child);
            }
        }
    }
}

void SpriteBatchNode::updateBlendFunc(void)
{
    if (! m_pTextureAtlas->getTexture()->hasPremultipliedAlpha())
        m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
}

// CocosNodeTexture protocol
void SpriteBatchNode::setBlendFunc(const BlendFunc &blendFunc)
{
    m_tBlendFunc = blendFunc;
}

const BlendFunc& SpriteBatchNode::getBlendFunc(void) const
{
    return m_tBlendFunc;
}

Texture2D* SpriteBatchNode::getTexture(void) const
{
    return m_pTextureAtlas->getTexture();
}

void SpriteBatchNode::setTexture(Texture2D *texture)
{
    m_pTextureAtlas->setTexture(texture);
    updateBlendFunc();
}


// SpriteSheet Extension
//implementation SpriteSheet (TMXTiledMapExtension)

void SpriteBatchNode::insertQuadFromSprite(Sprite *sprite, int index)
{
    CCASSERT( sprite != NULL, "Argument must be non-NULL");
    CCASSERT( dynamic_cast<Sprite*>(sprite), "CCSpriteBatchNode only supports Sprites as children");

    // make needed room
    while(index >= m_pTextureAtlas->getCapacity() || m_pTextureAtlas->getCapacity() == m_pTextureAtlas->getTotalQuads())
    {
        this->increaseAtlasCapacity();
    }
    //
    // update the quad directly. Don't add the sprite to the scene graph
    //
    sprite->setBatchNode(this);
    sprite->setAtlasIndex(index);

    V3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pTextureAtlas->insertQuad(&quad, index);

    // XXX: updateTransform will update the textureAtlas too, using updateQuad.
    // XXX: so, it should be AFTER the insertQuad
    sprite->setDirty(true);
    sprite->updateTransform();
}

void SpriteBatchNode::updateQuadFromSprite(Sprite *sprite, int index)
{
    CCASSERT(sprite != NULL, "Argument must be non-nil");
    CCASSERT(dynamic_cast<Sprite*>(sprite) != NULL, "CCSpriteBatchNode only supports Sprites as children");
    
	// make needed room
	while (index >= m_pTextureAtlas->getCapacity() || m_pTextureAtlas->getCapacity() == m_pTextureAtlas->getTotalQuads())
    {
		this->increaseAtlasCapacity();
    }
    
	//
	// update the quad directly. Don't add the sprite to the scene graph
	//
	sprite->setBatchNode(this);
    sprite->setAtlasIndex(index);
    
	sprite->setDirty(true);
	
	// UpdateTransform updates the textureAtlas quad
	sprite->updateTransform();
}

SpriteBatchNode * SpriteBatchNode::addSpriteWithoutQuad(Sprite*child, int z, int aTag)
{
    CCASSERT( child != NULL, "Argument must be non-NULL");
    CCASSERT( dynamic_cast<Sprite*>(child), "CCSpriteBatchNode only supports Sprites as children");

    // quad index is Z
    child->setAtlasIndex(z);

    // XXX: optimize with a binary search
    auto it = std::begin(m_aDescendants);
    std::for_each(m_aDescendants.begin(), m_aDescendants.end(), [&](Sprite *sprite) {
        if(sprite->getAtlasIndex() >= z)
            std::next(it);
    });

    m_aDescendants.insert(it, child);

    // IMPORTANT: Call super, and not self. Avoid adding it to the texture atlas array
    Node::addChild(child, z, aTag);

    //#issue 1262 don't use lazy sorting, tiles are added as quads not as sprites, so sprites need to be added in order
    reorderBatch(false);

    return this;
}

NS_CC_END
