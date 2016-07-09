/* -----------------------------------------------------------------------------------
 *
 *      File            CCSprite.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
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
#include "2d/sprite_nodes/CCAnimation.h"
#include "2d/sprite_nodes/CCAnimationCache.h"
#include "ccConfig.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/sprite_nodes/CCSpriteFrame.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "2d/CCDirector.h"
#include "base/CCGeometry.h"
#include "2d/textures/CCTexture2D.h"
#include "base/CCAffineTransform.h"
#include "support/TransformUtils.h"
#include "support/CCProfiling.h"
// external
#include "XMKazmath/GL/matrix.h"
#include <algorithm>

using namespace std;

NS_CC_BEGIN

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

Sprite* Sprite::createWithTexture(Texture2D *texture)
{
    Sprite *sprite = new Sprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

Sprite* Sprite::createWithTexture(Texture2D *texture, const Rect& rect)
{
    Sprite *sprite = new Sprite();
    if (sprite && sprite->initWithTexture(texture, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

Sprite* Sprite::create(const std::string& filename)
{
    Sprite *sprite = new Sprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

Sprite* Sprite::create(const std::string& filename, const Rect& rect)
{
    Sprite *sprite = new Sprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

Sprite* Sprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    Sprite *sprite = new Sprite();
    if (spriteFrame && sprite && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

Sprite* Sprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
    CCASSERT(frame != NULL, msg);
#endif
    
    return createWithSpriteFrame(frame);
}

Sprite* Sprite::create()
{
    Sprite *sprite = new Sprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

bool Sprite::init(void)
{
    return initWithTexture(NULL, Rect::ZERO);
}

// designated initializer
bool Sprite::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
    if (NodeRGBA::init())
    {
        m_pBatchNode = NULL;
        
        m_bRecursiveDirty = false;
        setDirty(false);
        
        m_bOpacityModifyRGB = true;
        
        m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        
        m_bFlippedX = m_bFlippedY = false;
        
        // default transform anchor: center
        setAnchorPoint(Point(0.5f, 0.5f));
        
        // zwoptex default values
        m_tOffsetPosition = Point::ZERO;
        
        m_bHasChildren = false;
        
        // clean the Quad
        kdMemset ( &m_tQuad, 0, sizeof ( m_tQuad ) );
        
        // Atlas: Color
        m_tQuad.bl.colors = Color4B::WHITE;
        m_tQuad.br.colors = Color4B::WHITE;
        m_tQuad.tl.colors = Color4B::WHITE;
        m_tQuad.tr.colors = Color4B::WHITE;
        
        // shader program
        setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        
        // update texture (calls updateBlendFunc)
        setTexture(texture);
        setTextureRect(rect, rotated, rect.size);
        
        // by default use "Self Render".
        // if the sprite is added to a batchnode, then it will automatically switch to "batchnode Render"
        setBatchNode(NULL);
        
        return true;
    }
    else
    {
        return false;
    }
}

bool Sprite::initWithTexture(Texture2D *texture, const Rect& rect)
{
    return initWithTexture(texture, rect, false);
}

bool Sprite::initWithTexture(Texture2D *texture)
{
    CCASSERT(texture != NULL, "Invalid texture for sprite");

    Rect rect = Rect::ZERO;
    rect.size = texture->getContentSize();
    
    return initWithTexture(texture, rect);
}

bool Sprite::initWithFile(const std::string& filename)
{
    CCASSERT(filename.size()>0, "Invalid filename for sprite");

    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename);
    if (texture)
    {
        Rect rect = Rect::ZERO;
        rect.size = texture->getContentSize();
        return initWithTexture(texture, rect);
    }

    // don't release here.
    // when load texture failed, it's better to get a "transparent" sprite then a crashed program
    // this->release(); 
    return false;
}

bool Sprite::initWithFile(const std::string &filename, const Rect& rect)
{
    CCASSERT(filename.size()>0, "Invalid filename");

    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename);
    if (texture)
    {
        return initWithTexture(texture, rect);
    }

    // don't release here.
    // when load texture failed, it's better to get a "transparent" sprite then a crashed program
    // this->release(); 
    return false;
}

bool Sprite::initWithSpriteFrame(SpriteFrame *spriteFrame)
{
    CCASSERT(spriteFrame != NULL, "");

    bool bRet = initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
    setDisplayFrame(spriteFrame);

    return bRet;
}

bool Sprite::initWithSpriteFrameName(const std::string& spriteFrameName)
{
    CCASSERT(spriteFrameName.size() > 0, "Invalid spriteFrameName");

    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    return initWithSpriteFrame(frame);
}

// XXX: deprecated
/*
Sprite* Sprite::initWithCGImage(CGImageRef pImage)
{
    // todo
    // because it is deprecated, so we do not implement it

    return NULL;
}
*/

/*
Sprite* Sprite::initWithCGImage(CGImageRef pImage, const char *pszKey)
{
    CCASSERT(pImage != NULL);

    // XXX: possible bug. See issue #349. New API should be added
    Texture2D *texture = Director::getInstance()->getTextureCache()->addCGImage(pImage, pszKey);

    const Size& size = texture->getContentSize();
    Rect rect = Rect(0 ,0, size.width, size.height);

    return initWithTexture(texture, rect);
}
*/

Sprite::Sprite(void)
: m_bShouldBeHidden(false)
, m_pTexture(nullptr)
{
}

Sprite::~Sprite(void)
{
    CC_SAFE_RELEASE(m_pTexture);
}

void Sprite::setTextureRect(const Rect& rect)
{
    setTextureRect(rect, false, rect.size);
}


void Sprite::setTextureRect(const Rect& rect, bool rotated, const Size& untrimmedSize)
{
    m_bRectRotated = rotated;

    setContentSize(untrimmedSize);
    setVertexRect(rect);
    setTextureCoords(rect);

    Point relativeOffset = m_tUnflippedOffsetPositionFromCenter;

    // issue #732
    if (m_bFlippedX)
    {
        relativeOffset.x = -relativeOffset.x;
    }
    if (m_bFlippedY)
    {
        relativeOffset.y = -relativeOffset.y;
    }

    m_tOffsetPosition.x = relativeOffset.x + (m_tContentSize.width - m_tRect.size.width) / 2;
    m_tOffsetPosition.y = relativeOffset.y + (m_tContentSize.height - m_tRect.size.height) / 2;

    // rendering using batch node
    if (m_pBatchNode)
    {
        // update dirty_, don't update recursiveDirty_
        setDirty(true);
    }
    else
    {
        // self rendering
        
        // Atlas: Vertex
        float x1 = 0 + m_tOffsetPosition.x;
        float y1 = 0 + m_tOffsetPosition.y;
        float x2 = x1 + m_tRect.size.width;
        float y2 = y1 + m_tRect.size.height;

        // Don't update Z.
        m_tQuad.bl.vertices = Vertex3F(x1, y1, 0);
        m_tQuad.br.vertices = Vertex3F(x2, y1, 0);
        m_tQuad.tl.vertices = Vertex3F(x1, y2, 0);
        m_tQuad.tr.vertices = Vertex3F(x2, y2, 0);
    }
}

// override this method to generate "double scale" sprites
void Sprite::setVertexRect(const Rect& rect)
{
    m_tRect = rect;
}

void Sprite::setTextureCoords(Rect rect)
{
    rect = CC_RECT_POINTS_TO_PIXELS(rect);

    Texture2D *tex = m_pBatchNode ? m_pTextureAtlas->getTexture() : m_pTexture;
    if (! tex)
    {
        return;
    }

    float atlasWidth = (float)tex->getPixelsWide();
    float atlasHeight = (float)tex->getPixelsHigh();

    float left, right, top, bottom;

    if (m_bRectRotated)
    {
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        left    = (2*rect.origin.x+1)/(2*atlasWidth);
        right   = left+(rect.size.height*2-2)/(2*atlasWidth);
        top     = (2*rect.origin.y+1)/(2*atlasHeight);
        bottom  = top+(rect.size.width*2-2)/(2*atlasHeight);
#else
        left    = rect.origin.x/atlasWidth;
        right   = (rect.origin.x+rect.size.height) / atlasWidth;
        top     = rect.origin.y/atlasHeight;
        bottom  = (rect.origin.y+rect.size.width) / atlasHeight;
#endif // CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL

        if (m_bFlippedX)
        {
            CC_SWAP(top, bottom, float);
        }

        if (m_bFlippedY)
        {
            CC_SWAP(left, right, float);
        }

        m_tQuad.bl.texCoords.u = left;
        m_tQuad.bl.texCoords.v = top;
        m_tQuad.br.texCoords.u = left;
        m_tQuad.br.texCoords.v = bottom;
        m_tQuad.tl.texCoords.u = right;
        m_tQuad.tl.texCoords.v = top;
        m_tQuad.tr.texCoords.u = right;
        m_tQuad.tr.texCoords.v = bottom;
    }
    else
    {
#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
        left    = (2*rect.origin.x+1)/(2*atlasWidth);
        right    = left + (rect.size.width*2-2)/(2*atlasWidth);
        top        = (2*rect.origin.y+1)/(2*atlasHeight);
        bottom    = top + (rect.size.height*2-2)/(2*atlasHeight);
#else
        left    = rect.origin.x/atlasWidth;
        right    = (rect.origin.x + rect.size.width) / atlasWidth;
        top        = rect.origin.y/atlasHeight;
        bottom    = (rect.origin.y + rect.size.height) / atlasHeight;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL

        if(m_bFlippedX)
        {
            CC_SWAP(left,right,float);
        }

        if(m_bFlippedY)
        {
            CC_SWAP(top,bottom,float);
        }

        m_tQuad.bl.texCoords.u = left;
        m_tQuad.bl.texCoords.v = bottom;
        m_tQuad.br.texCoords.u = right;
        m_tQuad.br.texCoords.v = bottom;
        m_tQuad.tl.texCoords.u = left;
        m_tQuad.tl.texCoords.v = top;
        m_tQuad.tr.texCoords.u = right;
        m_tQuad.tr.texCoords.v = top;
    }
}

void Sprite::updateTransform(void)
{
    CCASSERT(m_pBatchNode, "updateTransform is only valid when Sprite is being rendered using an SpriteBatchNode");
    
#ifdef CC_USE_PHYSICS
    updatePhysicsTransform();
    setDirty(true);
#endif
    
    // recalculate matrix only if it is dirty
    if( isDirty() ) {

        // If it is not visible, or one of its ancestors is not visible, then do nothing:
        if( !m_bVisible || ( m_pParent && m_pParent != m_pBatchNode && static_cast<Sprite*>(m_pParent)->m_bShouldBeHidden) )
        {
            m_tQuad.br.vertices = m_tQuad.tl.vertices = m_tQuad.tr.vertices = m_tQuad.bl.vertices = Vertex3F(0,0,0);
            m_bShouldBeHidden = true;
        }
        else 
        {
            m_bShouldBeHidden = false;

            if( ! m_pParent || m_pParent == m_pBatchNode )
            {
                m_tTransformToBatch = getNodeToParentTransform();
            }
            else 
            {
                CCASSERT( dynamic_cast<Sprite*>(m_pParent), "Logic error in Sprite. Parent must be a Sprite");
                m_tTransformToBatch = AffineTransformConcat( getNodeToParentTransform() , static_cast<Sprite*>(m_pParent)->m_tTransformToBatch );
            }

            //
            // calculate the Quad based on the Affine Matrix
            //

            Size size = m_tRect.size;

            float x1 = m_tOffsetPosition.x;
            float y1 = m_tOffsetPosition.y;

            float x2 = x1 + size.width;
            float y2 = y1 + size.height;
            float x = m_tTransformToBatch.tx;
            float y = m_tTransformToBatch.ty;

            float cr = m_tTransformToBatch.a;
            float sr = m_tTransformToBatch.b;
            float cr2 = m_tTransformToBatch.d;
            float sr2 = -m_tTransformToBatch.c;
            float ax = x1 * cr - y1 * sr2 + x;
            float ay = x1 * sr + y1 * cr2 + y;

            float bx = x2 * cr - y1 * sr2 + x;
            float by = x2 * sr + y1 * cr2 + y;

            float cx = x2 * cr - y2 * sr2 + x;
            float cy = x2 * sr + y2 * cr2 + y;

            float dx = x1 * cr - y2 * sr2 + x;
            float dy = x1 * sr + y2 * cr2 + y;

            m_tQuad.bl.vertices = Vertex3F( RENDER_IN_SUBPIXEL(ax), RENDER_IN_SUBPIXEL(ay), m_fVertexZ );
            m_tQuad.br.vertices = Vertex3F( RENDER_IN_SUBPIXEL(bx), RENDER_IN_SUBPIXEL(by), m_fVertexZ );
            m_tQuad.tl.vertices = Vertex3F( RENDER_IN_SUBPIXEL(dx), RENDER_IN_SUBPIXEL(dy), m_fVertexZ );
            m_tQuad.tr.vertices = Vertex3F( RENDER_IN_SUBPIXEL(cx), RENDER_IN_SUBPIXEL(cy), m_fVertexZ );
        }

        // MARMALADE CHANGE: ADDED CHECK FOR NULL, TO PERMIT SPRITES WITH NO BATCH NODE / TEXTURE ATLAS
        if (m_pTextureAtlas)
		{
            m_pTextureAtlas->updateQuad(&m_tQuad, m_nAtlasIndex);
        }
		
        m_bRecursiveDirty = false;
        setDirty(false);
    }

    // MARMALADE CHANGED
    // recursively iterate over children
/*    if( _hasChildren ) 
    {
        // MARMALADE: CHANGED TO USE Node*
        // NOTE THAT WE HAVE ALSO DEFINED virtual Node::updateTransform()
        arrayMakeObjectsPerformSelector(_children, updateTransform, Sprite*);
    }*/
    Node::updateTransform();

#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    Point vertices[4] = {
        Point( m_tQuad.bl.vertices.x, m_tQuad.bl.vertices.y ),
        Point( m_tQuad.br.vertices.x, m_tQuad.br.vertices.y ),
        Point( m_tQuad.tr.vertices.x, m_tQuad.tr.vertices.y ),
        Point( m_tQuad.tl.vertices.x, m_tQuad.tl.vertices.y ),
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
}

// draw

void Sprite::draw(void)
{
    CC_PROFILER_START_CATEGORY(kProfilerCategorySprite, "CCSprite - draw");

    CCASSERT(!m_pBatchNode, "If Sprite is being rendered by SpriteBatchNode, Sprite#draw SHOULD NOT be called");

    CC_NODE_DRAW_SETUP();
    
    GL::blendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    GL::bindTexture2D( m_pTexture->getName() );
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );

#define kQuadSize sizeof(m_tQuad.bl)
#ifdef EMSCRIPTEN
    long offset = 0;
    setGLBufferData(&m_tQuad, 4 * kQuadSize, 0);
#else
    long offset = (long)&m_tQuad;
#endif // EMSCRIPTEN

    // vertex
    int diff = offsetof( V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

    // texCoods
    diff = offsetof( V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    Point vertices[4]={
        Point(m_tQuad.tl.vertices.x,m_tQuad.tl.vertices.y),
        Point(m_tQuad.bl.vertices.x,m_tQuad.bl.vertices.y),
        Point(m_tQuad.br.vertices.x,m_tQuad.br.vertices.y),
        Point(m_tQuad.tr.vertices.x,m_tQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw texture box
    Size s = this->getTextureRect().size;
    Point offsetPix = this->getOffsetPosition();
    Point vertices[4] = {
        Point(offsetPix.x,offsetPix.y), Point(offsetPix.x+s.width,offsetPix.y),
        Point(offsetPix.x+s.width,offsetPix.y+s.height), Point(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

    CC_INCREMENT_GL_DRAWS(1);

    CC_PROFILER_STOP_CATEGORY(kProfilerCategorySprite, "CCSprite - draw");
}

// Node overrides

void Sprite::addChild(Node* child)
{
    Node::addChild(child);
}

void Sprite::addChild(Node *child, int zOrder)
{
    Node::addChild(child, zOrder);
}

void Sprite::addChild(Node *child, int zOrder, int tag)
{
    CCASSERT(child != NULL, "Argument must be non-NULL");

    if (m_pBatchNode)
    {
        Sprite* childSprite = dynamic_cast<Sprite*>(child);
        CCASSERT( childSprite, "CCSprite only supports Sprites as children when using SpriteBatchNode");
        CCASSERT(childSprite->getTexture()->getName() == m_pTextureAtlas->getTexture()->getName(), "");
        //put it in descendants array of batch node
        m_pBatchNode->appendChild(childSprite);

        if (!m_bReorderChildDirty)
        {
            setReorderChildDirtyRecursively();
        }
    }
    //CCNode already sets isReorderChildDirty_ so this needs to be after batchNode check
    Node::addChild(child, zOrder, tag);
    m_bHasChildren = true;
}

void Sprite::reorderChild(Node *child, int zOrder)
{
    CCASSERT(child != NULL, "");
    CCASSERT(m_pChildren->containsObject(child), "");

    if (zOrder == child->getZOrder())
    {
        return;
    }

    if( m_pBatchNode && ! m_bReorderChildDirty)
    {
        setReorderChildDirtyRecursively();
        m_pBatchNode->reorderBatch(true);
    }

    Node::reorderChild(child, zOrder);
}

void Sprite::removeChild(Node *child, bool cleanup)
{
    if (m_pBatchNode)
    {
        m_pBatchNode->removeSpriteFromAtlas((Sprite*)(child));
    }

    Node::removeChild(child, cleanup);
    
}

void Sprite::removeAllChildrenWithCleanup(bool cleanup)
{
    if (m_pBatchNode)
    {
        Object* object = NULL;
        CCARRAY_FOREACH(m_pChildren, object)
        {
            Sprite* child = dynamic_cast<Sprite*>(object);
            if (child)
            {
                m_pBatchNode->removeSpriteFromAtlas(child);
            }
        }
    }

    Node::removeAllChildrenWithCleanup(cleanup);
    
    m_bHasChildren = false;
}

void Sprite::sortAllChildren()
{
    if (m_bReorderChildDirty)
    {
#if 0
        int i = 0, j = 0, length = _children->count();

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

        if ( m_pBatchNode)
        {
            arrayMakeObjectsPerformSelector(m_pChildren, sortAllChildren, Sprite*);
        }

        m_bReorderChildDirty = false;
    }
}

//
// Node property overloads
// used only when parent is SpriteBatchNode
//

void Sprite::setReorderChildDirtyRecursively(void)
{
    //only set parents flag the first time
    if ( ! m_bReorderChildDirty )
    {
        m_bReorderChildDirty = true;
        Node* node = static_cast<Node*>(m_pParent);
        while (node && node != m_pBatchNode)
        {
            static_cast<Sprite*>(node)->setReorderChildDirtyRecursively();
            node=node->getParent();
        }
    }
}


void Sprite::setDirtyRecursively(bool bValue)
{
    m_bRecursiveDirty = bValue;
    setDirty(bValue);
    // recursively set dirty
    if (m_bHasChildren)
    {
        Object* object = NULL;
        CCARRAY_FOREACH(m_pChildren, object)
        {
            Sprite* child = dynamic_cast<Sprite*>(object);
            if (child)
            {
                child->setDirtyRecursively(true);
            }
        }
    }
}

// XXX HACK: optimization
#define SET_DIRTY_RECURSIVELY() {						\
	if ( m_pBatchNode && ! m_bRecursiveDirty ) {		\
		m_bRecursiveDirty = true;						\
		setDirty(true);									\
		if ( m_bHasChildren )							\
			setDirtyRecursively(true);					\
		}												\
	}

void Sprite::setPosition(const Point& pos)
{
    Node::setPosition(pos);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setRotation(float rotation)
{
    Node::setRotation(rotation);
    
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setRotationX(float fRotationX)
{
    Node::setRotationX(fRotationX);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setRotationY(float fRotationY)
{
    Node::setRotationY(fRotationY);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setSkewX(float sx)
{
    Node::setSkewX(sx);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setSkewY(float sy)
{
    Node::setSkewY(sy);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScaleX(float scaleX)
{
    Node::setScaleX(scaleX);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScaleY(float scaleY)
{
    Node::setScaleY(scaleY);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScale(float fScale)
{
    Node::setScale(fScale);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setScale(float scaleX, float scaleY)
{
    Node::setScale(scaleX, scaleY);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setVertexZ(float fVertexZ)
{
    Node::setVertexZ(fVertexZ);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setAnchorPoint(const Point& anchor)
{
    Node::setAnchorPoint(anchor);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::ignoreAnchorPointForPosition(bool value)
{
    CCASSERT(! m_pBatchNode, "ignoreAnchorPointForPosition is invalid in Sprite");
    Node::ignoreAnchorPointForPosition(value);
}

void Sprite::setVisible(bool bVisible)
{
    Node::setVisible(bVisible);
    SET_DIRTY_RECURSIVELY();
}

void Sprite::setFlippedX(bool flippedX)
{
    if (m_bFlippedX != flippedX)
    {
        m_bFlippedX = flippedX;
        setTextureRect(m_tRect, m_bRectRotated, m_tContentSize);
    }
}

bool Sprite::isFlippedX(void) const
{
    return m_bFlippedX;
}

void Sprite::setFlippedY(bool flippedY)
{
    if (m_bFlippedY != flippedY)
    {
        m_bFlippedY = flippedY;
        setTextureRect(m_tRect, m_bRectRotated, m_tContentSize);
    }
}

bool Sprite::isFlippedY(void) const
{
    return m_bFlippedY;
}

//
// RGBA protocol
//

void Sprite::updateColor(void)
{
    Color4B color4( m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity );
    
    // special opacity for premultiplied textures
	if (m_bOpacityModifyRGB)
    {
		color4.r *= m_cDisplayedOpacity/255.0f;
		color4.g *= m_cDisplayedOpacity/255.0f;
		color4.b *= m_cDisplayedOpacity/255.0f;
    }

    m_tQuad.bl.colors = color4;
    m_tQuad.br.colors = color4;
    m_tQuad.tl.colors = color4;
    m_tQuad.tr.colors = color4;

    // renders using batch node
    if (m_pBatchNode)
    {
        if (m_nAtlasIndex != INDEX_NOT_INITIALIZED)
        {
            m_pTextureAtlas->updateQuad(&m_tQuad, m_nAtlasIndex);
        }
        else
        {
            // no need to set it recursively
            // update dirty_, don't update recursiveDirty_
            setDirty(true);
        }
    }

    // self render
    // do nothing
}

void Sprite::setOpacity(GLubyte opacity)
{
    NodeRGBA::setOpacity(opacity);

    updateColor();
}

void Sprite::setColor(const Color3B& color3)
{
    NodeRGBA::setColor(color3);

    updateColor();
}

void Sprite::setOpacityModifyRGB(bool modify)
{
    if (m_bOpacityModifyRGB != modify)
    {
        m_bOpacityModifyRGB = modify;
        updateColor();
    }
}

bool Sprite::isOpacityModifyRGB(void) const
{
    return m_bOpacityModifyRGB;
}

void Sprite::updateDisplayedColor(const Color3B& parentColor)
{
    NodeRGBA::updateDisplayedColor(parentColor);
    
    updateColor();
}

void Sprite::updateDisplayedOpacity(GLubyte opacity)
{
    NodeRGBA::updateDisplayedOpacity(opacity);
    
    updateColor();
}

// Frames

void Sprite::setDisplayFrame(SpriteFrame *pNewFrame)
{
    m_tUnflippedOffsetPositionFromCenter = pNewFrame->getOffset();

    Texture2D *pNewTexture = pNewFrame->getTexture();
    // update texture before updating texture rect
    if (pNewTexture != m_pTexture)
    {
        setTexture(pNewTexture);
    }

    // update rect
    m_bRectRotated = pNewFrame->isRotated();
    setTextureRect(pNewFrame->getRect(), m_bRectRotated, pNewFrame->getOriginalSize());
}

KDvoid Sprite::setDisplayFrameName ( const KDchar* szNewFrameName )
{
	SpriteFrame*	pFrame = SpriteFrameCache::getInstance ( )->getSpriteFrameByName ( szNewFrameName );

	if ( pFrame )
	{
		this->setDisplayFrame ( pFrame );
	}
}

void Sprite::setDisplayFrameWithAnimationName(const std::string& animationName, int frameIndex)
{
    CCASSERT(animationName.size()>0, "CCSprite#setDisplayFrameWithAnimationName. animationName must not be NULL");

    Animation *a = AnimationCache::getInstance()->getAnimation(animationName);

    CCASSERT(a, "CCSprite#setDisplayFrameWithAnimationName: Frame not found");

    AnimationFrame* frame = static_cast<AnimationFrame*>( a->getFrames()->getObjectAtIndex(frameIndex) );

    CCASSERT(frame, "CCSprite#setDisplayFrame. Invalid frame");

    setDisplayFrame(frame->getSpriteFrame());
}

bool Sprite::isFrameDisplayed(SpriteFrame *frame) const
{
    Rect r = frame->getRect();

    return (r.equals(m_tRect) &&
            frame->getTexture()->getName() == m_pTexture->getName() &&
            frame->getOffset().equals(m_tUnflippedOffsetPositionFromCenter));
}

SpriteFrame* Sprite::getDisplayFrame()
{
    return SpriteFrame::createWithTexture(m_pTexture,
                                           CC_RECT_POINTS_TO_PIXELS(m_tRect),
                                           m_bRectRotated,
                                           CC_POINT_POINTS_TO_PIXELS(m_tUnflippedOffsetPositionFromCenter),
                                           CC_SIZE_POINTS_TO_PIXELS(m_tContentSize));
}

SpriteBatchNode* Sprite::getBatchNode()
{
    return m_pBatchNode;
}

void Sprite::setBatchNode(SpriteBatchNode *spriteBatchNode)
{
    m_pBatchNode = spriteBatchNode; // weak reference

    // self render
    if( ! m_pBatchNode ) {
        m_nAtlasIndex = INDEX_NOT_INITIALIZED;
        setTextureAtlas(NULL);
        m_bRecursiveDirty = false;
        setDirty(false);

        float x1 = m_tOffsetPosition.x;
        float y1 = m_tOffsetPosition.y;
        float x2 = x1 + m_tRect.size.width;
        float y2 = y1 + m_tRect.size.height;
        m_tQuad.bl.vertices = Vertex3F( x1, y1, 0 );
        m_tQuad.br.vertices = Vertex3F( x2, y1, 0 );
        m_tQuad.tl.vertices = Vertex3F( x1, y2, 0 );
        m_tQuad.tr.vertices = Vertex3F( x2, y2, 0 );

    } else {

        // using batch
        m_tTransformToBatch = AffineTransformIdentity;
        setTextureAtlas(m_pBatchNode->getTextureAtlas()); // weak ref
    }
}

// Texture protocol

void Sprite::updateBlendFunc(void)
{
    CCASSERT(! m_pBatchNode, "CCSprite: updateBlendFunc doesn't work when the sprite is rendered using a SpriteBatchNode");

    // it is possible to have an untextured sprite
    if (! m_pTexture || ! m_pTexture->hasPremultipliedAlpha())
    {
        m_tBlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
        setOpacityModifyRGB(false);
    }
    else
    {
        m_tBlendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        setOpacityModifyRGB(true);
    }
}

/*
 * This array is the data of a white image with 2 by 2 dimension.
 * It's used for creating a default texture when sprite's texture is set to NULL.
 * Supposing codes as follows:
 *
 *   auto sp = new Sprite();
 *   sp->init();  // Texture was set to NULL, in order to make opacity and color to work correctly, we need to create a 2x2 white texture.
 *
 * The test is in "TestCpp/SpriteTest/Sprite without texture".
 */
static unsigned char cc_2x2_white_image[] = {
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define CC_2x2_WHITE_IMAGE_KEY  "/cc_2x2_white_image"

void Sprite::setTexture(Texture2D *texture)
{
    // If batchnode, then texture id should be the same
	if (texture)
	{
		CCASSERT(!m_pBatchNode || texture->getName() == m_pBatchNode->getTexture()->getName(), "CCSprite: Batched sprites should use the same texture as the batchnode");
	}
    // accept texture==nil as argument
    CCASSERT( !texture || dynamic_cast<Texture2D*>(texture), "setTexture expects a Texture2D. Invalid argument");
    
    if (NULL == texture)
    {
        // Gets the texture by key firstly.
        texture = Director::getInstance()->getTextureCache()->getTextureForKey(CC_2x2_WHITE_IMAGE_KEY);
        
        // If texture wasn't in cache, create it from RAW data.
        if (NULL == texture)
        {
            Image* image = new Image();
            bool isOK = image->initWithRawData(cc_2x2_white_image, sizeof(cc_2x2_white_image), 2, 2, 8);
            CCASSERT(isOK, "The 2x2 empty texture was created unsuccessfully.");

            texture = Director::getInstance()->getTextureCache()->addImage(image, CC_2x2_WHITE_IMAGE_KEY);
            CC_SAFE_RELEASE(image);
        }
    }
    
    if (!m_pBatchNode && m_pTexture != texture)
    {
        CC_SAFE_RETAIN(texture);
        CC_SAFE_RELEASE(m_pTexture);
        m_pTexture = texture;
        updateBlendFunc();
    }
}

Texture2D* Sprite::getTexture(void) const
{
    return m_pTexture;
}

NS_CC_END
