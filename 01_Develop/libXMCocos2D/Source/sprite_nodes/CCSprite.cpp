/* -----------------------------------------------------------------------------------
 *
 *      File            CCSprite.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "sprite_nodes/CCSprite.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCAnimation.h"
#include "sprite_nodes/CCAnimationCache.h"
#include "sprite_nodes/CCSpriteFrameCache.h"
#include "cocoa/CCAffineTransform.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "textures/CCTextureCache.h"
#include "support/CCDirector.h"
#include "support/CCPointExtension.h"
#include "support/CCImage.h"
#include "shaders/CCShaderCache.h"
#include "ccConfig.h"

NS_CC_BEGIN

#if CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__)	( kdCeilf ( __ARGS__ ) )
#endif

CCSprite* CCSprite::createWithTexture ( CCTexture2D* pTexture )
{
	CCSprite*  pRet = new CCSprite ( );

	if ( pRet && pRet->initWithTexture ( pTexture ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
    CCSprite*  pRet = new CCSprite ( );

	if ( pRet && pRet->initWithTexture ( pTexture, tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::create ( const KDchar* szFilePath )
{
    CCSprite*  pRet = new CCSprite ( );

	if ( pRet && pRet->initWithFile ( szFilePath ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::create ( const KDchar* szFilePath, const CCRect& tRect )
{
    CCSprite*  pRet = new CCSprite ( );

	if ( pRet && pRet->initWithFile ( szFilePath, tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
    CCSprite*  pRet = new CCSprite ( );

	if ( pSpriteFrame && pRet && pRet->initWithSpriteFrame ( pSpriteFrame ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::createWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	CCSpriteFrame*  pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szSpriteFrameName );

#if COCOS2D_DEBUG > 0
	KDchar  szMsg [ 256 ] = { 0 };
    kdSprintf ( szMsg, "Invalid spriteFrameName: %s", szSpriteFrameName );
    CCAssert ( pFrame != KD_NULL, szMsg );
#endif
	
    return createWithSpriteFrame ( pFrame );
}

CCSprite* CCSprite::createWithSpriteBatchNode ( CCSpriteBatchNode* pSpriteBatchNode )
{
    CCSprite*  pRet = new CCSprite ( );

	if ( pSpriteBatchNode && pRet && pRet->initWithTexture ( pSpriteBatchNode->getTexture ( ) ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::createWithSpriteBatchNode ( CCSpriteBatchNode* pSpriteBatchNode, const CCRect& tRect )
{
    CCSprite*  pRet = new CCSprite ( );

	if ( pSpriteBatchNode && pRet && pRet->initWithTexture ( pSpriteBatchNode->getTexture ( ), tRect ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSprite* CCSprite::create ( KDvoid )
{
    CCSprite*  pRet = new CCSprite ( );
    
	if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

KDbool CCSprite::init ( KDvoid )
{
	return initWithTexture ( KD_NULL, CCRectZero );
}

// designated initializer
KDbool CCSprite::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect, KDbool bRotated )
{
    if ( CCNodeRGBA::init ( ) )
    {
		m_pBatchNode = KD_NULL;

		m_bRecursiveDirty = KD_FALSE;
		setDirty ( KD_FALSE );

		m_bOpacityModifyRGB = KD_TRUE;

		m_tBlendFunc.src = CC_BLEND_SRC;
		m_tBlendFunc.dst = CC_BLEND_DST;

		m_bFlipX = m_bFlipY = KD_FALSE;

		// default transform anchor: center
		setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

		// zwoptex default values
		m_tOffsetPosition = CCPointZero;

		m_bHasChildren = KD_FALSE;
	    
		// clean the Quad
		kdMemset ( &m_tQuad, 0, sizeof ( m_tQuad ) );

		// Atlas: Color
		ccColor4B  tTempColor = { 255, 255, 255, 255 };
		m_tQuad.bl.colors = tTempColor;
		m_tQuad.br.colors = tTempColor;
		m_tQuad.tl.colors = tTempColor;
		m_tQuad.tr.colors = tTempColor;

		// shader program
		setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

		// update texture (calls updateBlendFunc)
		setTexture ( pTexture );
		setTextureRect ( tRect, bRotated, tRect.size );

		// by default use "Self Render".
		// if the sprite is added to a batchnode, then it will automatically switch to "batchnode Render"
		setBatchNode ( KD_NULL );

		return KD_TRUE;
	}
	else
	{
		return KD_FALSE;
	}
}

KDbool CCSprite::initWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
	return initWithTexture ( pTexture, tRect, KD_FALSE );
}

KDbool CCSprite::initWithTexture ( CCTexture2D* pTexture )
{
	CCAssert ( pTexture != KD_NULL, "Invalid texture for sprite" );

	return initWithTexture ( pTexture, CCRect ( CCPointZero, pTexture->getContentSize ( ) ) );
}

KDbool CCSprite::initWithFile ( const KDchar* szFilePath )
{
	CCAssert ( szFilePath != KD_NULL, "Invalid filename for sprite" );

	CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );
	if ( pTexture )
	{
		return initWithTexture ( pTexture );
	}

	// don't release here.
	// when load texture failed, it's better to get a "transparent" sprite then a crashed program
	// this->release(); 
    
	return KD_FALSE;
}

KDbool CCSprite::initWithFile ( const KDchar* szFilePath, const CCRect& tRect )
{
	CCAssert ( szFilePath != KD_NULL, "" );

	CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );
	if ( pTexture )
	{
		return initWithTexture ( pTexture, tRect );
	}

	// don't release here.
	// when load texture failed, it's better to get a "transparent" sprite then a crashed program
	// this->release(); 
    
	return KD_FALSE;
}

KDbool CCSprite::initWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
	CCAssert ( pSpriteFrame != KD_NULL, "" );

	KDbool  bRet = initWithTexture ( pSpriteFrame->getTexture ( ), pSpriteFrame->getRect ( ) );
	setDisplayFrame ( pSpriteFrame );

	return bRet;
}

KDbool CCSprite::initWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
	CCAssert ( szSpriteFrameName != KD_NULL, "" );

	CCSpriteFrame*  pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szSpriteFrameName );
	return initWithSpriteFrame ( pFrame );
}

/// XXX: deprecated
CCSprite::CCSprite ( KDvoid )
{
	m_bShouldBeHidden = KD_FALSE;
    m_pTexture = KD_NULL;
}

CCSprite::~CCSprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTexture );
}

KDvoid CCSprite::setTextureRect ( const CCRect& tRect )
{
	setTextureRect ( tRect, KD_FALSE, tRect.size );
}

KDvoid CCSprite::setTextureRect ( const CCRect& tRect, KDbool bRotated, const CCSize& tUntrimmedSize )
{
	m_bRectRotated = bRotated;

	setContentSize   ( tUntrimmedSize );
	setVertexRect    ( tRect );
    setTextureCoords ( tRect );

	CCPoint  tRelativeOffset = m_tUnflippedOffsetPositionFromCenter;

	// issue #732
	if ( m_bFlipX )
	{
		tRelativeOffset.x = -tRelativeOffset.x;
	}

	if ( m_bFlipY )
	{
		tRelativeOffset.y = -tRelativeOffset.y;
	}

	m_tOffsetPosition.x = tRelativeOffset.x + ( m_tContentSize.cx - m_tRect.size.cx ) / 2.0f;
	m_tOffsetPosition.y = tRelativeOffset.y + ( m_tContentSize.cy - m_tRect.size.cy ) / 2.0f;

	// rendering using batch node
	if ( m_pBatchNode )
	{
		// update dirty_, don't update recursiveDirty_
		setDirty ( KD_TRUE );
	}
	else
	{
		// self rendering
		
		// Atlas: Vertex
		KDfloat x1 = m_tOffsetPosition.x;
		KDfloat y1 = m_tOffsetPosition.y;
		KDfloat x2 = x1 + m_tRect.size.cx;
		KDfloat y2 = y1 + m_tRect.size.cy;

		// Don't update Z.
		m_tQuad.bl.vertices = vertex3 ( x1, y1, 0 );
		m_tQuad.br.vertices = vertex3 ( x2, y1, 0 );
		m_tQuad.tl.vertices = vertex3 ( x1, y2, 0 );
		m_tQuad.tr.vertices = vertex3 ( x2, y2, 0 );
	}
}

// override this method to generate "double scale" sprites
KDvoid CCSprite::setVertexRect ( const CCRect& tRect )
{
    m_tRect = tRect;
}

KDvoid CCSprite::setTextureCoords ( CCRect tRect )
{
	tRect = CC_RECT_POINTS_TO_PIXELS ( tRect );

	CCTexture2D*  pTexture = m_pBatchNode ? m_pTextureAtlas->getTexture ( ) : m_pTexture;
	if ( !pTexture )
	{
		return;
	}

	KDfloat  atlasWidth  = (KDfloat) pTexture->getPixelsWide ( );
	KDfloat  atlasHeight = (KDfloat) pTexture->getPixelsHigh ( );

	KDfloat  left, right, top, bottom;

	if ( m_bRectRotated )
	{
		left	= tRect.origin.x / atlasWidth;
		right   = ( tRect.origin.x + tRect.size.cy ) / atlasWidth;
		top		= tRect.origin.y / atlasHeight;
		bottom	= ( tRect.origin.y + tRect.size.cx ) / atlasHeight;

		if ( m_bFlipX )
		{
			CC_SWAP ( top, bottom, KDfloat );
		}

		if ( m_bFlipY )
		{
			CC_SWAP ( left, right, KDfloat );
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
		left	= tRect.origin.x / atlasWidth;
		right	= ( tRect.origin.x + tRect.size.cx ) / atlasWidth;
		top		= tRect.origin.y / atlasHeight;
		bottom	= ( tRect.origin.y + tRect.size.cy ) / atlasHeight; 

		if ( m_bFlipX )
		{
			CC_SWAP ( left, right, KDfloat );
		}

		if ( m_bFlipY )
		{
			CC_SWAP ( top, bottom, KDfloat );
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

KDvoid CCSprite::updateTransform ( KDvoid )
{
	CCAssert ( m_pBatchNode, "updateTransform is only valid when CCSprite is being rendered using an CCSpriteBatchNode" );

	// recalculate matrix only if it is dirty
	if ( isDirty ( ) )
	{
        // If it is not visible, or one of its ancestors is not visible, then do nothing:
        if( !m_bVisible || ( m_pParent && m_pParent != m_pBatchNode && ( (CCSprite*) m_pParent )->m_bShouldBeHidden ) )
        {
            m_tQuad.br.vertices = m_tQuad.tl.vertices = m_tQuad.tr.vertices = m_tQuad.bl.vertices = vertex3 ( 0, 0, 0 );
            m_bShouldBeHidden = KD_TRUE;
        }
        else 
        {
            m_bShouldBeHidden = KD_FALSE;

            if( !m_pParent || m_pParent == m_pBatchNode )
            {
                m_tTransformToBatch = nodeToParentTransform ( );
            }
            else 
            {
                CCAssert ( dynamic_cast<CCSprite*> ( m_pParent ), "Logic error in CCSprite. Parent must be a CCSprite" );
                m_tTransformToBatch = CCAffineTransformConcat ( nodeToParentTransform ( ) , ( (CCSprite*) m_pParent )->m_tTransformToBatch );
            }

            //
            // calculate the Quad based on the Affine Matrix
            //

            CCSize   tSize = m_tRect.size;

            KDfloat  x1  = m_tOffsetPosition.x;
            KDfloat  y1  = m_tOffsetPosition.y;

            KDfloat  x2  = x1 + tSize.cx;
            KDfloat  y2  = y1 + tSize.cy;
            KDfloat  x   = m_tTransformToBatch.tx;
            KDfloat  y   = m_tTransformToBatch.ty;

            KDfloat  cr  = m_tTransformToBatch.a;
            KDfloat  sr  = m_tTransformToBatch.b;
            KDfloat  cr2 =  m_tTransformToBatch.d;
            KDfloat  sr2 = -m_tTransformToBatch.c;
            KDfloat  ax  = x1 * cr - y1 * sr2 + x;
            KDfloat  ay  = x1 * sr + y1 * cr2 + y;

            KDfloat  bx  = x2 * cr - y1 * sr2 + x;
            KDfloat  by  = x2 * sr + y1 * cr2 + y;

            KDfloat  cx  = x2 * cr - y2 * sr2 + x;
            KDfloat  cy  = x2 * sr + y2 * cr2 + y;

            KDfloat  dx  = x1 * cr - y2 * sr2 + x;
            KDfloat	 dy  = x1 * sr + y2 * cr2 + y;

            m_tQuad.bl.vertices = vertex3 ( RENDER_IN_SUBPIXEL ( ax ), RENDER_IN_SUBPIXEL ( ay ), m_fVertexZ );
            m_tQuad.br.vertices = vertex3 ( RENDER_IN_SUBPIXEL ( bx ), RENDER_IN_SUBPIXEL ( by ), m_fVertexZ );
            m_tQuad.tl.vertices = vertex3 ( RENDER_IN_SUBPIXEL ( dx ), RENDER_IN_SUBPIXEL ( dy ), m_fVertexZ );
            m_tQuad.tr.vertices = vertex3 ( RENDER_IN_SUBPIXEL ( cx ), RENDER_IN_SUBPIXEL ( cy ), m_fVertexZ );
        }

		if ( m_pTextureAtlas )
		{
			m_pTextureAtlas->updateQuad ( &m_tQuad, m_uAtlasIndex );
		}

        m_bRecursiveDirty = KD_FALSE;
        setDirty ( KD_FALSE );
    }

	// MARMALADE CHANGED
    // recursively iterate over children
//    if ( m_bHasChildren ) 
//    {
//		  // MARMALADE: CHANGED TO USE CCNode*
//		  // NOTE THAT WE HAVE ALSO DEFINED virtual CCNode::updateTransform ( )
//        arrayMakeObjectsPerformSelector ( m_pChildren, updateTransform, CCSprite* );
//    }
	CCNode::updateTransform ( );

#if CC_SPRITE_DEBUG_DRAW
    // draw bounding box
    CCPoint  aVertices[4] =
	{
        ccp ( m_tQuad.bl.vertices.x, m_tQuad.bl.vertices.y ),
        ccp ( m_tQuad.br.vertices.x, m_tQuad.br.vertices.y ),
        ccp ( m_tQuad.tr.vertices.x, m_tQuad.tr.vertices.y ),
        ccp ( m_tQuad.tl.vertices.x, m_tQuad.tl.vertices.y ),
    };
    ccDrawPoly ( aVertices, 4, KD_TRUE );
#endif // CC_SPRITE_DEBUG_DRAW
}

/// draw
KDvoid CCSprite::draw ( KDvoid )
{
    CC_PROFILER_START_CATEGORY ( kCCProfilerCategorySprite, "CCSprite - draw" );

    CCAssert ( !m_pBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called" );

    CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

	ccGLBindTexture2D ( m_pTexture->getName ( ) );
 
    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );

#define kQuadSize	sizeof ( m_tQuad.bl )
#ifdef EMSCRIPTEN
	GLintptr		nOffset = 0;
	setGLBufferData ( &m_tQuad, 4 * kQuadSize, 0 );
#else
	GLintptr		nOffset = (GLintptr) &m_tQuad;
#endif

	ccGLVertexAttribPointer ( kCCVertexAttrib_Position	, 3, GL_FLOAT        , GL_FALSE, kQuadSize, (GLvoid *) ( nOffset + offsetof ( ccV3F_C4B_T2F, vertices  ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords	, 2, GL_FLOAT        , GL_FALSE, kQuadSize, (GLvoid *) ( nOffset + offsetof ( ccV3F_C4B_T2F, texCoords ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color		, 4, GL_UNSIGNED_BYTE, GL_TRUE , kQuadSize, (GLvoid *) ( nOffset + offsetof ( ccV3F_C4B_T2F, colors    ) ) );
	
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

    CHECK_GL_ERROR_DEBUG ( );

#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    CCPoint	 aVertices [ 4 ] = 
	{
        ccp ( m_tQuad.tl.vertices.x, m_tQuad.tl.vertices.y ),
        ccp ( m_tQuad.bl.vertices.x, m_tQuad.bl.vertices.y ),
        ccp ( m_tQuad.br.vertices.x, m_tQuad.br.vertices.y ),
        ccp ( m_tQuad.tr.vertices.x, m_tQuad.tr.vertices.y ),
    };
    ccDrawPoly ( aVertices, 4, KD_TRUE );
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw texture box
    CCSize   tSize = this->getTextureRect ( ).size;
    CCPoint  tOffset = this->getOffsetPosition ( );
    CCPoint  aVertices [ 4 ] = 
	{
        ccp ( tOffset.x           , tOffset.y            ),
		ccp ( tOffset.x + tSize.cx, tOffset.y            ),
        ccp ( tOffset.x + tSize.cx, tOffset.y + tSize.cy ), 
		ccp ( tOffset.x           , tOffset.y + tSize.cy )
    };
    ccDrawPoly ( aVertices, 4, KD_TRUE );
#endif // CC_SPRITE_DEBUG_DRAW

    CC_INCREMENT_GL_DRAWS ( 1 );

    CC_PROFILER_STOP_CATEGORY ( kCCProfilerCategorySprite, "CCSprite - draw" );
}

/// CCNode overrides
KDvoid CCSprite::addChild ( CCNode* pChild )
{
	CCNode::addChild ( pChild );
}

KDvoid CCSprite::addChild ( CCNode* pChild, KDint nZOrder )
{
	CCNode::addChild ( pChild, nZOrder );
}

KDvoid CCSprite::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{
	CCAssert ( pChild != KD_NULL, "Argument must be non-NULL" );

    if ( m_pBatchNode )
    {
        CCSprite*  pChildSprite = dynamic_cast<CCSprite*> ( pChild );
        CCAssert ( pChildSprite, "CCSprite only supports CCSprites as children when using CCSpriteBatchNode" );
        CCAssert ( pChildSprite->getTexture ( )->getName ( ) == m_pTextureAtlas->getTexture ( )->getName ( ), "" );
        //put it in descendants array of batch node
        m_pBatchNode->appendChild ( pChildSprite );

        if ( !m_bReorderChildDirty )
        {
            setReorderChildDirtyRecursively ( );
        }
    }

    // CCNode already sets isReorderChildDirty_ so this needs to be after batchNode check
    CCNode::addChild ( pChild, nZOrder, nTag );
    m_bHasChildren = KD_TRUE;
}

KDvoid CCSprite::reorderChild ( CCNode* pChild, KDint nZOrder )
{
    CCAssert ( pChild != KD_NULL, "" );
	CCAssert ( m_pChildren->containsObject ( pChild ), "" );

    if ( nZOrder == pChild->getZOrder ( ) )
    {
        return;
    }

    if ( m_pBatchNode && ! m_bReorderChildDirty )
    {
        setReorderChildDirtyRecursively ( );
        m_pBatchNode->reorderBatch ( KD_TRUE );
    }

    CCNode::reorderChild ( pChild, nZOrder );
}

KDvoid CCSprite::removeChild ( CCNode* pChild, KDbool bCleanup )
{
	if ( m_pBatchNode )
	{
		m_pBatchNode->removeSpriteFromAtlas ( (CCSprite *) pChild );
	}

	CCNode::removeChild ( pChild, bCleanup );
}

KDvoid CCSprite::removeAllChildrenWithCleanup ( KDbool bCleanup )
{
	if ( m_pBatchNode )
	{
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pChildren, pObject )
        {
            CCSprite*  pChild = dynamic_cast<CCSprite*> ( pObject );
            if ( pChild )
            {
                m_pBatchNode->removeSpriteFromAtlas ( pChild );
            }
        }
	}

	CCNode::removeAllChildrenWithCleanup ( bCleanup );
	
	m_bHasChildren = KD_FALSE;
}

KDvoid CCSprite::sortAllChildren ( KDvoid )
{
    if ( m_bReorderChildDirty )
    {
        KDint    i = 0, j = 0, nLength = m_pChildren->m_pData->num;
        CCNode** pNodes = (CCNode**) m_pChildren->m_pData->arr;
        CCNode*  pTempItem = KD_NULL;

        // insertion sort
        for ( i = 1; i < nLength; i++ )
        {
            pTempItem = pNodes [ i ];
            j = i - 1;

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but orderOfArrival is smaller
            while ( j >= 0 && ( pTempItem->getZOrder ( )  < pNodes [ j ]->getZOrder ( )
						   || ( pTempItem->getZOrder ( ) == pNodes [ j ]->getZOrder ( )
						   && pTempItem->getOrderOfArrival ( ) < pNodes [ j ]->getOrderOfArrival ( ) ) ) )
            {
                pNodes [ j + 1 ] = pNodes [ j ];
                j = j - 1;
            }

            pNodes [ j + 1 ] = pTempItem;
        }

        if ( m_pBatchNode )
        {
            arrayMakeObjectsPerformSelector ( m_pChildren, sortAllChildren, CCSprite* );
        }

        m_bReorderChildDirty = KD_FALSE;
    }
}

//
// CCNode property overloads
// used only when parent is CCSpriteBatchNode
//

KDvoid CCSprite::setReorderChildDirtyRecursively ( KDvoid )
{
    //only set parents flag the first time
    if ( !m_bReorderChildDirty )
    {
        m_bReorderChildDirty = KD_TRUE;
        CCNode*  pNode = (CCNode*) m_pParent;
        while ( pNode && pNode != m_pBatchNode )
        {
            ( (CCSprite*) pNode )->setReorderChildDirtyRecursively ( );
            pNode = pNode->getParent ( );
        }
    }
}

KDvoid CCSprite::setDirtyRecursively ( KDbool bValue )
{
	m_bRecursiveDirty = bValue;
	setDirty ( bValue );

	// recursively set dirty
	if ( m_bHasChildren )
	{
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pChildren, pObject )
        {
            CCSprite*  pChild = dynamic_cast<CCSprite*> ( pObject );
            if ( pChild )
            {
                pChild->setDirtyRecursively ( KD_TRUE );
            }
        }
	}
}

/// XXX HACK: optimization
#define SET_DIRTY_RECURSIVELY( ) {                      \
    if ( m_pBatchNode && ! m_bRecursiveDirty )			\
    {                                                   \
        m_bRecursiveDirty = KD_TRUE;					\
		setDirty ( KD_TRUE );							\
        if ( m_bHasChildren )                           \
        {                                               \
             setDirtyRecursively ( KD_TRUE );           \
        }                                               \
    }                                                   \
}

KDvoid CCSprite::setPosition ( const CCPoint& tPosition )
{
	CCNode::setPosition ( tPosition );
    SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setRotation ( KDfloat fRotation )
{
	CCNode::setRotation ( fRotation );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setRotationX ( KDfloat fRotationX )
{
	CCNode::setRotationX ( fRotationX );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setRotationY ( KDfloat fRotationY )
{
	CCNode::setRotationY ( fRotationY );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setSkewX ( KDfloat fSkewX )
{
    CCNode::setSkewX ( fSkewX );
    SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setSkewY ( KDfloat fSkewY )
{
    CCNode::setSkewY ( fSkewY );
    SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setScaleX ( KDfloat fScaleX )
{
	CCNode::setScaleX ( fScaleX );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setScaleY ( KDfloat fScaleY )
{
	CCNode::setScaleY ( fScaleY );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setScale ( KDfloat fScale )
{
	CCNode::setScale ( fScale );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setVertexZ ( KDfloat fVertexZ )
{
	CCNode::setVertexZ ( fVertexZ );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setAnchorPoint ( const CCPoint& tAnchorPoint )
{
	CCNode::setAnchorPoint ( tAnchorPoint );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::ignoreAnchorPointForPosition ( KDbool bIgnore )
{
	CCAssert ( !m_pBatchNode, "ignoreAnchorPointForPosition is invalid in CCSprite" );
	CCNode::ignoreAnchorPointForPosition ( bIgnore );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setVisible ( KDbool bVisible )
{
	CCNode::setVisible ( bVisible );
	SET_DIRTY_RECURSIVELY ( );
}

KDvoid CCSprite::setFlipX ( KDbool bFlipX )
{
	if ( m_bFlipX != bFlipX )
	{
		m_bFlipX = bFlipX;
		setTextureRect ( m_tRect, m_bRectRotated, m_tContentSize );
	}
}

KDbool CCSprite::isFlipX ( KDvoid )
{
	return m_bFlipX;
}

KDvoid CCSprite::setFlipY ( KDbool bFlipY )
{
	if ( m_bFlipY != bFlipY )
	{
		m_bFlipY = bFlipY;
		setTextureRect ( m_tRect, m_bRectRotated, m_tContentSize );
	}
}

KDbool CCSprite::isFlipY ( KDvoid )
{
	return m_bFlipY;
}

//
// RGBA protocol
//

KDvoid CCSprite::updateColor ( KDvoid )
{
	ccColor4B  tColor = { m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity };

    // special opacity for premultiplied textures
	if ( m_bOpacityModifyRGB )
    {
		tColor.r = (GLubyte) ( tColor.r * m_cDisplayedOpacity / 255.0f );
		tColor.g = (GLubyte) ( tColor.g * m_cDisplayedOpacity / 255.0f );
		tColor.b = (GLubyte) ( tColor.b * m_cDisplayedOpacity / 255.0f );
    }

	m_tQuad.bl.colors = tColor;
	m_tQuad.br.colors = tColor;
	m_tQuad.tl.colors = tColor;
	m_tQuad.tr.colors = tColor;

	// renders using batch node
	if ( m_pBatchNode )
	{
		if ( m_uAtlasIndex != CCSpriteIndexNotInitialized )
		{
			m_pTextureAtlas->updateQuad ( &m_tQuad, m_uAtlasIndex );
		}
		else
		{
			// no need to set it recursively
			// update dirty_, don't update recursiveDirty_
			setDirty ( KD_TRUE );
		}
	}

	// self render
	// do nothing
}

KDvoid CCSprite::setOpacity ( GLubyte cOpacity )
{
	CCNodeRGBA::setOpacity ( cOpacity );

	updateColor ( );
}

KDvoid CCSprite::setColor ( const ccColor3B& tColor )
{
    CCNodeRGBA::setColor ( tColor );

	updateColor ( );
}

KDvoid CCSprite::setOpacityModifyRGB ( KDbool bModify )
{
	if ( m_bOpacityModifyRGB != bModify )
	{
		m_bOpacityModifyRGB = bModify;
		updateColor ( );
	}
}

KDbool CCSprite::isOpacityModifyRGB ( KDvoid )
{
	return m_bOpacityModifyRGB;
}

KDvoid CCSprite::updateDisplayedColor ( const ccColor3B& tParentColor )
{
    CCNodeRGBA::updateDisplayedColor ( tParentColor );
    
    updateColor ( );
}

KDvoid CCSprite::updateDisplayedOpacity ( GLubyte cOpacity )
{
    CCNodeRGBA::updateDisplayedOpacity ( cOpacity );
    
    updateColor ( );
}

/// Frames
KDvoid CCSprite::setDisplayFrame ( CCSpriteFrame* pNewFrame )
{
	m_tUnflippedOffsetPositionFromCenter = pNewFrame->getOffset ( );

	CCTexture2D*  pNewTexture = pNewFrame->getTexture ( );
	// update texture before updating texture rect
	if ( pNewTexture != m_pTexture )
	{
        setTexture ( pNewTexture );
	}

	// update rect
	m_bRectRotated = pNewFrame->isRotated ( );
	setTextureRect ( pNewFrame->getRect ( ), m_bRectRotated, pNewFrame->getOriginalSize ( ) );
}

KDvoid CCSprite::setDisplayFrameWithAnimationName ( const KDchar* szAnimationName, KDint nFrameIndex )
{
	CCAssert ( szAnimationName, "CCSprite#setDisplayFrameWithAnimationName. animationName must not be NULL" );

	CCAnimation*  pAnimation = CCAnimationCache::sharedAnimationCache ( )->animationByName ( szAnimationName );
	CCAssert ( pAnimation, "CCSprite#setDisplayFrameWithAnimationName: Frame not found" );

	CCAnimationFrame*  pFrame = (CCAnimationFrame*) pAnimation->getFrames ( )->objectAtIndex ( nFrameIndex );
	CCAssert ( pFrame, "CCSprite#setDisplayFrame. Invalid frame" );

	setDisplayFrame ( pFrame->getSpriteFrame ( ) );
}

KDbool CCSprite::isFrameDisplayed ( CCSpriteFrame* pFrame )
{
	CCRect  tRect = pFrame->getRect ( );

	return ( tRect.equals ( m_tRect ) 
		     && pFrame->getTexture ( )->getName ( ) == m_pTexture->getName ( ) 
			 && pFrame->getOffset ( ).equals ( m_tUnflippedOffsetPositionFromCenter ) );
}

CCSpriteFrame* CCSprite::displayFrame ( KDvoid )
{
	return  CCSpriteFrame::createWithTexture 
	(
		m_pTexture, 
		CC_RECT_POINTS_TO_PIXELS  ( m_tRect ),
		m_bRectRotated,
		CC_POINT_POINTS_TO_PIXELS ( m_tUnflippedOffsetPositionFromCenter ), 
		CC_SIZE_POINTS_TO_PIXELS  ( m_tContentSize )
	);
}

CCSpriteBatchNode* CCSprite::getBatchNode ( KDvoid )
{
    return m_pBatchNode;
}

KDvoid CCSprite::setBatchNode ( CCSpriteBatchNode* pSpriteBatchNode )
{
    m_pBatchNode = pSpriteBatchNode; // weak reference

    // self render
    if ( !m_pBatchNode )
	{
        m_uAtlasIndex = CCSpriteIndexNotInitialized;
        setTextureAtlas ( KD_NULL );
        m_bRecursiveDirty = KD_FALSE;
        setDirty ( KD_FALSE );

        KDfloat  x1 = m_tOffsetPosition.x;
        KDfloat  y1 = m_tOffsetPosition.y;
        KDfloat  x2 = x1 + m_tRect.size.cx;
        KDfloat  y2 = y1 + m_tRect.size.cy;
        m_tQuad.bl.vertices = vertex3 ( x1, y1, 0 );
        m_tQuad.br.vertices = vertex3 ( x2, y1, 0 );
        m_tQuad.tl.vertices = vertex3 ( x1, y2, 0 );
        m_tQuad.tr.vertices = vertex3 ( x2, y2, 0 );
    }
	else
	{
        // using batch
        m_tTransformToBatch = CCAffineTransformIdentity;
        setTextureAtlas ( m_pBatchNode->getTextureAtlas ( ) ); // weak ref
    }
}

/// Texture protocol
KDvoid CCSprite::updateBlendFunc ( KDvoid )
{
	CCAssert ( !m_pBatchNode, "CCSprite: updateBlendFunc doesn't work when the sprite is rendered using a CCSpriteBatchNode" );

	// it's possible to have an untextured sprite
	if ( !m_pTexture || !m_pTexture->hasPremultipliedAlpha ( ) )
	{
		m_tBlendFunc.src = GL_SRC_ALPHA;
		m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		setOpacityModifyRGB ( KD_FALSE );
	}
	else
	{
		m_tBlendFunc.src = CC_BLEND_SRC;
		m_tBlendFunc.dst = CC_BLEND_DST;		
		setOpacityModifyRGB ( KD_TRUE );
	}
}

/**
 *	This array is the data of a white image with 2 by 2 dimension.
 *	It's used for creating a default texture when sprite's texture is set to NULL.
 *	Supposing codes as follows:
 *
 *		CCSprite* sp = new CCSprite();
 *		sp->init();  // Texture was set to NULL, in order to make opacity and color to work correctly, we need to create a 2x2 white texture.
 *
 *	The test is in "TestCpp/SpriteTest/Sprite without texture".
 */
static KDubyte	cc_2x2_white_image [ ] =
{
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define CC_2x2_WHITE_IMAGE_KEY  "cc_2x2_white_image"

KDvoid CCSprite::setTexture ( CCTexture2D* pTexture )
{
	// If batchnode, then texture id should be the same
	CCAssert ( !m_pBatchNode || pTexture->getName ( ) == m_pBatchNode->getTexture ( )->getName ( ), "CCSprite: Batched sprites should use the same texture as the batchnode" );

	if ( KD_NULL == pTexture )
	{
		// Gets the texture by key firstly.
		pTexture = CCTextureCache::sharedTextureCache ( )->textureForKey ( CC_2x2_WHITE_IMAGE_KEY );

		// If texture wasn't in cache, create it from RAW data.
		if ( KD_NULL == pTexture )
		{
			CCImage*	pImage = CCImage::create ( );
			KDbool		bIsOK  = pImage->initWithRawData ( cc_2x2_white_image, sizeof ( cc_2x2_white_image ), 2, 2, 32 );
			CCAssert ( bIsOK, "The 2x2 empty texture was created unsuccessfully." );

			pTexture = CCTextureCache::sharedTextureCache ( )->addCCImage ( pImage, CC_2x2_WHITE_IMAGE_KEY );			
		}
	}

    if ( !m_pBatchNode && m_pTexture != pTexture )
    {
        CC_SAFE_RETAIN ( pTexture );
        CC_SAFE_RELEASE ( m_pTexture );
        m_pTexture = pTexture;
        updateBlendFunc ( );
    }
}

CCTexture2D* CCSprite::getTexture ( KDvoid )
{
	return m_pTexture;
}


NS_CC_END