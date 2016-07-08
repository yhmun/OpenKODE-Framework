/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteFrame.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2011 Ricardo Quesada
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
 * ----------------------------------------------------------------------------------- */

#include "2d/sprite_nodes/CCSpriteFrame.h"
#include "2d/textures/CCTextureCache.h"
#include "2d/CCDirector.h"

NS_CC_BEGIN

// implementation of SpriteFrame

SpriteFrame* SpriteFrame::create(const std::string& filename, const Rect& rect)
{
    SpriteFrame *pSpriteFrame = new SpriteFrame();
    pSpriteFrame->initWithTextureFilename(filename, rect);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

SpriteFrame* SpriteFrame::createWithTexture(Texture2D *texture, const Rect& rect)
{
    SpriteFrame *pSpriteFrame = new SpriteFrame();
    pSpriteFrame->initWithTexture(texture, rect);
    pSpriteFrame->autorelease();
    
    return pSpriteFrame;
}

SpriteFrame* SpriteFrame::createWithTexture(Texture2D* texture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    SpriteFrame *pSpriteFrame = new SpriteFrame();
    pSpriteFrame->initWithTexture(texture, rect, rotated, offset, originalSize);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

SpriteFrame* SpriteFrame::create(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    SpriteFrame *pSpriteFrame = new SpriteFrame();
    pSpriteFrame->initWithTextureFilename(filename, rect, rotated, offset, originalSize);
    pSpriteFrame->autorelease();

    return pSpriteFrame;
}

bool SpriteFrame::initWithTexture(Texture2D* texture, const Rect& rect)
{
    Rect rectInPixels = CC_RECT_POINTS_TO_PIXELS(rect);
    return initWithTexture(texture, rectInPixels, false, Point::ZERO, rectInPixels.size);
}

bool SpriteFrame::initWithTextureFilename(const std::string& filename, const Rect& rect)
{
    Rect rectInPixels = CC_RECT_POINTS_TO_PIXELS( rect );
    return initWithTextureFilename(filename, rectInPixels, false, Point::ZERO, rectInPixels.size);
}

bool SpriteFrame::initWithTexture(Texture2D* texture, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    m_pTexture = texture;

    if (texture)
    {
        texture->retain();
    }

    m_tRectInPixels = rect;
    m_tRect = CC_RECT_PIXELS_TO_POINTS(rect);
    m_tOffsetInPixels = offset;
    m_tOffset = CC_POINT_PIXELS_TO_POINTS( m_tOffsetInPixels );
    m_tOriginalSizeInPixels = originalSize;
    m_tOriginalSize = CC_SIZE_PIXELS_TO_POINTS( m_tOriginalSizeInPixels );
    m_bRotated = rotated;

    return true;
}

bool SpriteFrame::initWithTextureFilename(const std::string& filename, const Rect& rect, bool rotated, const Point& offset, const Size& originalSize)
{
    m_pTexture = NULL;
    m_sTextureFilename = filename;
    m_tRectInPixels = rect;
    m_tRect = CC_RECT_PIXELS_TO_POINTS( rect );
    m_tOffsetInPixels = offset;
    m_tOffset = CC_POINT_PIXELS_TO_POINTS( m_tOffsetInPixels );
    m_tOriginalSizeInPixels = originalSize;
    m_tOriginalSize = CC_SIZE_PIXELS_TO_POINTS( m_tOriginalSizeInPixels );
    m_bRotated = rotated;

    return true;
}

SpriteFrame::~SpriteFrame(void)
{
    CCLOGINFO("deallocing SpriteFrame: %p", this);
    CC_SAFE_RELEASE(m_pTexture);
}

SpriteFrame* SpriteFrame::clone() const
{
	// no copy constructor	
    SpriteFrame *copy = new SpriteFrame();
    copy->initWithTextureFilename(m_sTextureFilename.c_str(), m_tRectInPixels, m_bRotated, m_tOffsetInPixels, m_tOriginalSizeInPixels);
    copy->setTexture(m_pTexture);
    copy->autorelease();
    return copy;
}

void SpriteFrame::setRect(const Rect& rect)
{
    m_tRect = rect;
    m_tRectInPixels = CC_RECT_POINTS_TO_PIXELS(m_tRect);
}

void SpriteFrame::setRectInPixels(const Rect& rectInPixels)
{
    m_tRectInPixels = rectInPixels;
    m_tRect = CC_RECT_PIXELS_TO_POINTS(rectInPixels);
}

const Point& SpriteFrame::getOffset() const
{
    return m_tOffset;
}

void SpriteFrame::setOffset(const Point& offsets)
{
    m_tOffset = offsets;
    m_tOffsetInPixels = CC_POINT_POINTS_TO_PIXELS( m_tOffset );
}

const Point& SpriteFrame::getOffsetInPixels() const
{
    return m_tOffsetInPixels;
}

void SpriteFrame::setOffsetInPixels(const Point& offsetInPixels)
{
    m_tOffsetInPixels = offsetInPixels;
    m_tOffset = CC_POINT_PIXELS_TO_POINTS( m_tOffsetInPixels );
}

void SpriteFrame::setTexture(Texture2D * texture)
{
    if( m_pTexture != texture ) {
        CC_SAFE_RELEASE(m_pTexture);
        CC_SAFE_RETAIN(texture);
        m_pTexture = texture;
    }
}

Texture2D* SpriteFrame::getTexture(void)
{
    if( m_pTexture ) {
        return m_pTexture;
    }

    if( m_sTextureFilename.length() > 0 ) {
        return Director::getInstance()->getTextureCache()->addImage(m_sTextureFilename.c_str());
    }
    // no texture or texture filename
    return NULL;
}

NS_CC_END

