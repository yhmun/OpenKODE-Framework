/* -----------------------------------------------------------------------------------
 *
 *      File            CCScale9Sprite.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Jung Sang-Taik on 12. 3. 16
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2012      Neofect
 *
 *          http://www.cocos2d-x.org
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/sprite_nodes/CCSprite.h"
#include "2d/sprite_nodes/CCSpriteFrameCache.h"

NS_CC_EXT_BEGIN

enum positions
{
    pCentre = 0,
    pTop,
    pLeft,
    pRight,
    pBottom,
    pTopRight,
    pTopLeft,
    pBottomRight,
    pBottomLeft
};

Scale9Sprite::Scale9Sprite()
: m_bSpritesGenerated(false)
, m_bSpriteFrameRotated(false)
, m_bPositionsAreDirty(false)
, m_pScale9Image(NULL)
, m_pTopLeft(NULL)
, m_pTop(NULL)
, m_pTopRight(NULL)
, m_pLeft(NULL)
, m_pCentre(NULL)
, m_pRight(NULL)
, m_pBottomLeft(NULL)
, m_pBottom(NULL)
, m_pBottomRight(NULL)
, m_bOpacityModifyRGB(false)
, m_fInsetLeft(0)
, m_fInsetTop(0)
, m_fInsetRight(0)
, m_fInsetBottom(0)
{

}

Scale9Sprite::~Scale9Sprite()
{
    CC_SAFE_RELEASE(m_pTopLeft);
    CC_SAFE_RELEASE(m_pTop);
    CC_SAFE_RELEASE(m_pTopRight);
    CC_SAFE_RELEASE(m_pLeft);
    CC_SAFE_RELEASE(m_pCentre);
    CC_SAFE_RELEASE(m_pRight);
    CC_SAFE_RELEASE(m_pBottomLeft);
    CC_SAFE_RELEASE(m_pBottom);
    CC_SAFE_RELEASE(m_pBottomRight);
    CC_SAFE_RELEASE(m_pScale9Image);
}

bool Scale9Sprite::init()
{
    return this->initWithBatchNode(NULL, Rect::ZERO, Rect::ZERO);
}

bool Scale9Sprite::initWithBatchNode(SpriteBatchNode* batchnode, const Rect& rect, const Rect& capInsets)
{
    return this->initWithBatchNode(batchnode, rect, false, capInsets);
}

bool Scale9Sprite::initWithBatchNode(SpriteBatchNode* batchnode, const Rect& rect, bool rotated, const Rect& capInsets)
{
    if(batchnode)
    {
        this->updateWithBatchNode(batchnode, rect, rotated, capInsets);
    }
    
    this->setAnchorPoint(Point(0.5f, 0.5f));
    this->m_bPositionsAreDirty = true;
    
    return true;
}

#define    TRANSLATE_X(x, y, xtranslate) \
    x+=xtranslate;                       \

#define    TRANSLATE_Y(x, y, ytranslate) \
    y+=ytranslate;                       \

bool Scale9Sprite::updateWithBatchNode(SpriteBatchNode* batchnode, const Rect& originalRect, bool rotated, const Rect& capInsets)
{
    GLubyte opacity = getOpacity();
    Color3B color = getColor();
    Rect rect(originalRect);

    // Release old sprites
    this->removeAllChildrenWithCleanup(true);

    CC_SAFE_RELEASE(this->m_pCentre);
    CC_SAFE_RELEASE(this->m_pTop);
    CC_SAFE_RELEASE(this->m_pTopLeft);
    CC_SAFE_RELEASE(this->m_pTopRight);
    CC_SAFE_RELEASE(this->m_pLeft);
    CC_SAFE_RELEASE(this->m_pRight);
    CC_SAFE_RELEASE(this->m_pBottomLeft);
    CC_SAFE_RELEASE(this->m_pBottom);
    CC_SAFE_RELEASE(this->m_pBottomRight);

    
    if(this->m_pScale9Image != batchnode)
    {
        CC_SAFE_RELEASE(this->m_pScale9Image);
        m_pScale9Image = batchnode;
        CC_SAFE_RETAIN(m_pScale9Image);
    }
    
    if (!m_pScale9Image)
    {
        return false;
    }

    m_pScale9Image->removeAllChildrenWithCleanup(true);

    m_tCapInsets = capInsets;
    m_bSpriteFrameRotated = rotated;
    
    // If there is no given rect
    if ( rect.equals(Rect::ZERO) )
    {
        // Get the texture size as original
        Size textureSize = m_pScale9Image->getTextureAtlas()->getTexture()->getContentSize();
    
        rect = Rect(0, 0, textureSize.width, textureSize.height);
    }
    
    // Set the given rect's size as original size
    m_tSpriteRect = rect;
    m_tOriginalSize = rect.size;
    m_tPreferredSize = m_tOriginalSize;
    m_tCapInsetsInternal = capInsets;
    
    float w = rect.size.width;
    float h = rect.size.height;

    // If there is no specified center region
    if ( m_tCapInsetsInternal.equals(Rect::ZERO) )
    {
        // log("... cap insets not specified : using default cap insets ...");
        m_tCapInsetsInternal = Rect(w/3, h/3, w/3, h/3);
    }

    float left_w = m_tCapInsetsInternal.origin.x;
    float center_w = m_tCapInsetsInternal.size.width;
    float right_w = rect.size.width - (left_w + center_w);

    float top_h = m_tCapInsetsInternal.origin.y;
    float center_h = m_tCapInsetsInternal.size.height;
    float bottom_h = rect.size.height - (top_h + center_h);

    // calculate rects

    // ... top row
    float x = 0.0;
    float y = 0.0;

    // top left
    Rect lefttopbounds = Rect(x, y, left_w, top_h);

    // top center
    TRANSLATE_X(x, y, left_w);
    Rect centertopbounds = Rect(x, y, center_w, top_h);
        
    // top right
    TRANSLATE_X(x, y, center_w);
    Rect righttopbounds = Rect(x, y, right_w, top_h);

    // ... center row
    x = 0.0;
    y = 0.0;
    TRANSLATE_Y(x, y, top_h);

    // center left
    Rect leftcenterbounds = Rect(x, y, left_w, center_h);

    // center center
    TRANSLATE_X(x, y, left_w);
    Rect centerbounds = Rect(x, y, center_w, center_h);

    // center right
    TRANSLATE_X(x, y, center_w);
    Rect rightcenterbounds = Rect(x, y, right_w, center_h);

    // ... bottom row
    x = 0.0;
    y = 0.0;
    TRANSLATE_Y(x, y, top_h);
    TRANSLATE_Y(x, y, center_h);

    // bottom left
    Rect leftbottombounds = Rect(x, y, left_w, bottom_h);

    // bottom center
    TRANSLATE_X(x, y, left_w);
    Rect centerbottombounds = Rect(x, y, center_w, bottom_h);

    // bottom right
    TRANSLATE_X(x, y, center_w);
    Rect rightbottombounds = Rect(x, y, right_w, bottom_h);

    if (!rotated) {
        // log("!rotated");

        AffineTransform t = AffineTransform::IDENTITY;
        t = AffineTransformTranslate(t, rect.origin.x, rect.origin.y);

        centerbounds = RectApplyAffineTransform(centerbounds, t);
        rightbottombounds = RectApplyAffineTransform(rightbottombounds, t);
        leftbottombounds = RectApplyAffineTransform(leftbottombounds, t);
        righttopbounds = RectApplyAffineTransform(righttopbounds, t);
        lefttopbounds = RectApplyAffineTransform(lefttopbounds, t);
        rightcenterbounds = RectApplyAffineTransform(rightcenterbounds, t);
        leftcenterbounds = RectApplyAffineTransform(leftcenterbounds, t);
        centerbottombounds = RectApplyAffineTransform(centerbottombounds, t);
        centertopbounds = RectApplyAffineTransform(centertopbounds, t);

        // Centre
        m_pCentre = new Sprite();
        m_pCentre->initWithTexture(m_pScale9Image->getTexture(), centerbounds);
        m_pScale9Image->addChild(m_pCentre, 0, pCentre);
        
        // Top
        m_pTop = new Sprite();
        m_pTop->initWithTexture(m_pScale9Image->getTexture(), centertopbounds);
        m_pScale9Image->addChild(m_pTop, 1, pTop);
        
        // Bottom
        m_pBottom = new Sprite();
        m_pBottom->initWithTexture(m_pScale9Image->getTexture(), centerbottombounds);
        m_pScale9Image->addChild(m_pBottom, 1, pBottom);
        
        // Left
        m_pLeft = new Sprite();
        m_pLeft->initWithTexture(m_pScale9Image->getTexture(), leftcenterbounds);
        m_pScale9Image->addChild(m_pLeft, 1, pLeft);
        
        // Right
        m_pRight = new Sprite();
        m_pRight->initWithTexture(m_pScale9Image->getTexture(), rightcenterbounds);
        m_pScale9Image->addChild(m_pRight, 1, pRight);
        
        // Top left
        m_pTopLeft = new Sprite();
        m_pTopLeft->initWithTexture(m_pScale9Image->getTexture(), lefttopbounds);
        m_pScale9Image->addChild(m_pTopLeft, 2, pTopLeft);
        
        // Top right
        m_pTopRight = new Sprite();
        m_pTopRight->initWithTexture(m_pScale9Image->getTexture(), righttopbounds);
        m_pScale9Image->addChild(m_pTopRight, 2, pTopRight);
        
        // Bottom left
        m_pBottomLeft = new Sprite();
        m_pBottomLeft->initWithTexture(m_pScale9Image->getTexture(), leftbottombounds);
        m_pScale9Image->addChild(m_pBottomLeft, 2, pBottomLeft);
        
        // Bottom right
        m_pBottomRight = new Sprite();
        m_pBottomRight->initWithTexture(m_pScale9Image->getTexture(), rightbottombounds);
        m_pScale9Image->addChild(m_pBottomRight, 2, pBottomRight);
    } else {
        // set up transformation of coordinates
        // to handle the case where the sprite is stored rotated
        // in the spritesheet
        // log("rotated");

        AffineTransform t = AffineTransform::IDENTITY;

        Rect rotatedcenterbounds = centerbounds;
        Rect rotatedrightbottombounds = rightbottombounds;
        Rect rotatedleftbottombounds = leftbottombounds;
        Rect rotatedrighttopbounds = righttopbounds;
        Rect rotatedlefttopbounds = lefttopbounds;
        Rect rotatedrightcenterbounds = rightcenterbounds;
        Rect rotatedleftcenterbounds = leftcenterbounds;
        Rect rotatedcenterbottombounds = centerbottombounds;
        Rect rotatedcentertopbounds = centertopbounds;
        
        t = AffineTransformTranslate(t, rect.size.height+rect.origin.x, rect.origin.y);
        t = AffineTransformRotate(t, 1.57079633f);
        
        centerbounds = RectApplyAffineTransform(centerbounds, t);
        rightbottombounds = RectApplyAffineTransform(rightbottombounds, t);
        leftbottombounds = RectApplyAffineTransform(leftbottombounds, t);
        righttopbounds = RectApplyAffineTransform(righttopbounds, t);
        lefttopbounds = RectApplyAffineTransform(lefttopbounds, t);
        rightcenterbounds = RectApplyAffineTransform(rightcenterbounds, t);
        leftcenterbounds = RectApplyAffineTransform(leftcenterbounds, t);
        centerbottombounds = RectApplyAffineTransform(centerbottombounds, t);
        centertopbounds = RectApplyAffineTransform(centertopbounds, t);

        rotatedcenterbounds.origin = centerbounds.origin;
        rotatedrightbottombounds.origin = rightbottombounds.origin;
        rotatedleftbottombounds.origin = leftbottombounds.origin;
        rotatedrighttopbounds.origin = righttopbounds.origin;
        rotatedlefttopbounds.origin = lefttopbounds.origin;
        rotatedrightcenterbounds.origin = rightcenterbounds.origin;
        rotatedleftcenterbounds.origin = leftcenterbounds.origin;
        rotatedcenterbottombounds.origin = centerbottombounds.origin;
        rotatedcentertopbounds.origin = centertopbounds.origin;

        // Centre
        m_pCentre = new Sprite();
        m_pCentre->initWithTexture(m_pScale9Image->getTexture(), rotatedcenterbounds, true);
        m_pScale9Image->addChild(m_pCentre, 0, pCentre);
        
        // Top
        m_pTop = new Sprite();
        m_pTop->initWithTexture(m_pScale9Image->getTexture(), rotatedcentertopbounds, true);
        m_pScale9Image->addChild(m_pTop, 1, pTop);
        
        // Bottom
        m_pBottom = new Sprite();
        m_pBottom->initWithTexture(m_pScale9Image->getTexture(), rotatedcenterbottombounds, true);
        m_pScale9Image->addChild(m_pBottom, 1, pBottom);
        
        // Left
        m_pLeft = new Sprite();
        m_pLeft->initWithTexture(m_pScale9Image->getTexture(), rotatedleftcenterbounds, true);
        m_pScale9Image->addChild(m_pLeft, 1, pLeft);
        
        // Right
        m_pRight = new Sprite();
        m_pRight->initWithTexture(m_pScale9Image->getTexture(), rotatedrightcenterbounds, true);
        m_pScale9Image->addChild(m_pRight, 1, pRight);
        
        // Top left
        m_pTopLeft = new Sprite();
        m_pTopLeft->initWithTexture(m_pScale9Image->getTexture(), rotatedlefttopbounds, true);
        m_pScale9Image->addChild(m_pTopLeft, 2, pTopLeft);
        
        // Top right
        m_pTopRight = new Sprite();
        m_pTopRight->initWithTexture(m_pScale9Image->getTexture(), rotatedrighttopbounds, true);
        m_pScale9Image->addChild(m_pTopRight, 2, pTopRight);
        
        // Bottom left
        m_pBottomLeft = new Sprite();
        m_pBottomLeft->initWithTexture(m_pScale9Image->getTexture(), rotatedleftbottombounds, true);
        m_pScale9Image->addChild(m_pBottomLeft, 2, pBottomLeft);
        
        // Bottom right
        m_pBottomRight = new Sprite();
        m_pBottomRight->initWithTexture(m_pScale9Image->getTexture(), rotatedrightbottombounds, true);
        m_pScale9Image->addChild(m_pBottomRight, 2, pBottomRight);
    }

    this->setContentSize(rect.size);
    this->addChild(m_pScale9Image);
    
    if (m_bSpritesGenerated)
    {
        // Restore color and opacity
        this->setOpacity(opacity);
        this->setColor(color);
    }
    m_bSpritesGenerated = true;

    return true;
}

void Scale9Sprite::setContentSize(const Size &size)
{
    Node::setContentSize(size);
    this->m_bPositionsAreDirty = true;
}

void Scale9Sprite::updatePositions()
{
    // Check that instances are non-NULL
    if(!((m_pTopLeft) &&
         (m_pTopRight) &&
         (m_pBottomRight) &&
         (m_pBottomLeft) &&
         (m_pCentre))) {
        // if any of the above sprites are NULL, return
        return;
    }

    Size size = this->m_tContentSize;

    float sizableWidth = size.width - m_pTopLeft->getContentSize().width - m_pTopRight->getContentSize().width;
    float sizableHeight = size.height - m_pTopLeft->getContentSize().height - m_pBottomRight->getContentSize().height;
    
    float horizontalScale = sizableWidth/m_pCentre->getContentSize().width;
    float verticalScale = sizableHeight/m_pCentre->getContentSize().height;

    m_pCentre->setScaleX(horizontalScale);
    m_pCentre->setScaleY(verticalScale);

    float rescaledWidth = m_pCentre->getContentSize().width * horizontalScale;
    float rescaledHeight = m_pCentre->getContentSize().height * verticalScale;

    float leftWidth = m_pBottomLeft->getContentSize().width;
    float bottomHeight = m_pBottomLeft->getContentSize().height;

    m_pBottomLeft->setAnchorPoint(Point(0,0));
    m_pBottomRight->setAnchorPoint(Point(0,0));
    m_pTopLeft->setAnchorPoint(Point(0,0));
    m_pTopRight->setAnchorPoint(Point(0,0));
    m_pLeft->setAnchorPoint(Point(0,0));
    m_pRight->setAnchorPoint(Point(0,0));
    m_pTop->setAnchorPoint(Point(0,0));
    m_pBottom->setAnchorPoint(Point(0,0));
    m_pCentre->setAnchorPoint(Point(0,0));

    // Position corners
    m_pBottomLeft->setPosition(Point(0,0));
    m_pBottomRight->setPosition(Point(leftWidth+rescaledWidth,0));
    m_pTopLeft->setPosition(Point(0, bottomHeight+rescaledHeight));
    m_pTopRight->setPosition(Point(leftWidth+rescaledWidth, bottomHeight+rescaledHeight));

    // Scale and position borders
    m_pLeft->setPosition(Point(0, bottomHeight));
    m_pLeft->setScaleY(verticalScale);
    m_pRight->setPosition(Point(leftWidth+rescaledWidth,bottomHeight));
    m_pRight->setScaleY(verticalScale);
    m_pBottom->setPosition(Point(leftWidth,0));
    m_pBottom->setScaleX(horizontalScale);
    m_pTop->setPosition(Point(leftWidth,bottomHeight+rescaledHeight));
    m_pTop->setScaleX(horizontalScale);

    // Position centre
    m_pCentre->setPosition(Point(leftWidth, bottomHeight));
}

bool Scale9Sprite::initWithFile(const char* file, const Rect& rect,  const Rect& capInsets)
{
    CCASSERT(file != NULL, "Invalid file for sprite");
    
    SpriteBatchNode *batchnode = SpriteBatchNode::create(file, 9);
    bool pReturn = this->initWithBatchNode(batchnode, rect, capInsets);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::create(const char* file, const Rect& rect,  const Rect& capInsets)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithFile(file, rect, capInsets) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool Scale9Sprite::initWithFile(const char* file, const Rect& rect)
{
    CCASSERT(file != NULL, "Invalid file for sprite");
    bool pReturn = this->initWithFile(file, rect, Rect::ZERO);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::create(const char* file, const Rect& rect)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithFile(file, rect) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}


bool Scale9Sprite::initWithFile(const Rect& capInsets, const char* file)
{
    bool pReturn = this->initWithFile(file, Rect::ZERO, capInsets);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::create(const Rect& capInsets, const char* file)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithFile(capInsets, file) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool Scale9Sprite::initWithFile(const char* file)
{
    bool pReturn = this->initWithFile(file, Rect::ZERO);
    return pReturn;
    
}

Scale9Sprite* Scale9Sprite::create(const char* file)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithFile(file) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool Scale9Sprite::initWithSpriteFrame(SpriteFrame* spriteFrame, const Rect& capInsets)
{
    Texture2D* texture = spriteFrame->getTexture();
    CCASSERT(texture != NULL, "CCTexture must be not nil");

    SpriteBatchNode *batchnode = SpriteBatchNode::createWithTexture(texture, 9);
    CCASSERT(batchnode != NULL, "CCSpriteBatchNode must be not nil");

    bool pReturn = this->initWithBatchNode(batchnode, spriteFrame->getRect(), spriteFrame->isRotated(), capInsets);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::createWithSpriteFrame(SpriteFrame* spriteFrame, const Rect& capInsets)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithSpriteFrame(spriteFrame, capInsets) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}
bool Scale9Sprite::initWithSpriteFrame(SpriteFrame* spriteFrame)
{
    CCASSERT(spriteFrame != NULL, "Invalid spriteFrame for sprite");
    bool pReturn = this->initWithSpriteFrame(spriteFrame, Rect::ZERO);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::createWithSpriteFrame(SpriteFrame* spriteFrame)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithSpriteFrame(spriteFrame) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool Scale9Sprite::initWithSpriteFrameName(const char* spriteFrameName, const Rect& capInsets)
{
    CCASSERT((SpriteFrameCache::getInstance()) != NULL, "SpriteFrameCache::getInstance() must be non-NULL");

    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    CCASSERT(frame != NULL, "CCSpriteFrame must be non-NULL");

    if (NULL == frame) return false;

    bool pReturn = this->initWithSpriteFrame(frame, capInsets);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::createWithSpriteFrameName(const char* spriteFrameName, const Rect& capInsets)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithSpriteFrameName(spriteFrameName, capInsets) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool Scale9Sprite::initWithSpriteFrameName(const char* spriteFrameName)
{
    bool pReturn = this->initWithSpriteFrameName(spriteFrameName, Rect::ZERO);
    return pReturn;
}

Scale9Sprite* Scale9Sprite::createWithSpriteFrameName(const char* spriteFrameName)
{
    CCASSERT(spriteFrameName != NULL, "spriteFrameName must be non-NULL");

    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithSpriteFrameName(spriteFrameName) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);

    log("Could not allocate Scale9Sprite()");
    return NULL;
    
}

Scale9Sprite* Scale9Sprite::resizableSpriteWithCapInsets(const Rect& capInsets)
{
    Scale9Sprite* pReturn = new Scale9Sprite();
    if ( pReturn && pReturn->initWithBatchNode(m_pScale9Image, m_tSpriteRect, capInsets) )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

Scale9Sprite* Scale9Sprite::create()
{ 
    Scale9Sprite *pReturn = new Scale9Sprite();
    if (pReturn && pReturn->init())
    { 
        pReturn->autorelease();   
        return pReturn;
    } 
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

/** sets the opacity.
 @warning If the the texture has premultiplied alpha then, the R, G and B channels will be modifed.
 Values goes from 0 to 255, where 255 means fully opaque.
 */

void Scale9Sprite::setPreferredSize(Size preferedSize)
{
    this->setContentSize(preferedSize);
    this->m_tPreferredSize = preferedSize;
}

Size Scale9Sprite::getPreferredSize()
{
    return this->m_tPreferredSize;
}

void Scale9Sprite::setCapInsets(Rect capInsets)
{
    Size contentSize = this->m_tContentSize;
    this->updateWithBatchNode(this->m_pScale9Image, this->m_tSpriteRect, m_bSpriteFrameRotated, capInsets);
    this->setContentSize(contentSize);
}

Rect Scale9Sprite::getCapInsets()
{
    return m_tCapInsets;
}

void Scale9Sprite::updateCapInset()
{
    Rect insets;
    if (this->m_fInsetLeft == 0 && this->m_fInsetTop == 0 && this->m_fInsetRight == 0 && this->m_fInsetBottom == 0)
    {
        insets = Rect::ZERO;
    }
    else
    {
        insets = Rect(m_fInsetLeft,
            m_fInsetTop,
            m_tSpriteRect.size.width-m_fInsetLeft-m_fInsetRight,
            m_tSpriteRect.size.height-m_fInsetTop-m_fInsetBottom);
    }
    this->setCapInsets(insets);
}

void Scale9Sprite::setOpacityModifyRGB(bool var)
{
    if (!m_pScale9Image)
    {
        return;
    }
    m_bOpacityModifyRGB = var;
    Object* child;
    Array* children = m_pScale9Image->getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);
        if (pNode)
        {
            pNode->setOpacityModifyRGB(m_bOpacityModifyRGB);
        }
    }
}
bool Scale9Sprite::isOpacityModifyRGB() const
{
    return m_bOpacityModifyRGB;
}

void Scale9Sprite::setSpriteFrame(SpriteFrame * spriteFrame)
{
    SpriteBatchNode * batchnode = SpriteBatchNode::createWithTexture(spriteFrame->getTexture(), 9);
    this->updateWithBatchNode(batchnode, spriteFrame->getRect(), spriteFrame->isRotated(), Rect::ZERO);

    // Reset insets
    this->m_fInsetLeft = 0;
    this->m_fInsetTop = 0;
    this->m_fInsetRight = 0;
    this->m_fInsetBottom = 0;
}

float Scale9Sprite::getInsetLeft()
{
    return this->m_fInsetLeft;
}

float Scale9Sprite::getInsetTop()
{
    return this->m_fInsetTop;
}

float Scale9Sprite::getInsetRight()
{
    return this->m_fInsetRight;
}

float Scale9Sprite::getInsetBottom()
{
    return this->m_fInsetBottom;
}

void Scale9Sprite::setInsetLeft(float insetLeft)
{
    this->m_fInsetLeft = insetLeft;
    this->updateCapInset();
}

void Scale9Sprite::setInsetTop(float insetTop)
{
    this->m_fInsetTop = insetTop;
    this->updateCapInset();
}

void Scale9Sprite::setInsetRight(float insetRight)
{
    this->m_fInsetRight = insetRight;
    this->updateCapInset();
}

void Scale9Sprite::setInsetBottom(float insetBottom)
{
    this->m_fInsetBottom = insetBottom;
    this->updateCapInset();
}

void Scale9Sprite::visit()
{
    if(this->m_bPositionsAreDirty)
    {
        this->updatePositions();
        this->m_bPositionsAreDirty = false;
    }
    Node::visit();
}

void Scale9Sprite::setColor(const Color3B& color)
{
    if (!m_pScale9Image)
    {
        return;
    }
    
    NodeRGBA::setColor(color);
    Object* child;
    Array* children = m_pScale9Image->getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);
        if (pNode)
        {
            pNode->setColor(color);
        }
    }
}

const Color3B& Scale9Sprite::getColor() const
{
	return m_tRealColor;
}

void Scale9Sprite::setOpacity(GLubyte opacity)
{
    if (!m_pScale9Image)
    {
        return;
    }
    NodeRGBA::setOpacity(opacity);
    Object* child;
    Array* children = m_pScale9Image->getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);
        if (pNode)
        {
            pNode->setOpacity(opacity);
        }
    }
}

GLubyte Scale9Sprite::getOpacity() const
{
	return m_cRealOpacity;
}

void Scale9Sprite::updateDisplayedColor(const cocos2d::Color3B &parentColor)
{
    if (!m_pScale9Image)
    {
        return;
    }
    NodeRGBA::updateDisplayedColor(parentColor);
    Object* child;
    Array* children = m_pScale9Image->getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);
        if (pNode)
        {
            pNode->updateDisplayedColor(parentColor);
        }
    }
}

void Scale9Sprite::updateDisplayedOpacity(GLubyte parentOpacity)
{
    if (!m_pScale9Image)
    {
        return;
    }
    NodeRGBA::updateDisplayedOpacity(parentOpacity);
    Object* child;
    Array* children = m_pScale9Image->getChildren();
    CCARRAY_FOREACH(children, child)
    {
        RGBAProtocol* pNode = dynamic_cast<RGBAProtocol*>(child);
        if (pNode)
        {
            pNode->updateDisplayedOpacity(parentOpacity);
        }
    }
}

NS_CC_EXT_END
