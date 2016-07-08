/* -----------------------------------------------------------------------------------
 *
 *      File            CCSpriteBatchNode.cpp 
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "effects/CCGrid.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "shaders/CCShaderCache.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

//
// creation with CCTexture2D
//

CCSpriteBatchNode* CCSpriteBatchNode::createWithTexture ( CCTexture2D* pTexture, KDuint uCapacity )
{
	CCSpriteBatchNode*  pRet = new CCSpriteBatchNode ( );

    if ( pRet && pRet->initWithTexture ( pTexture, uCapacity ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

//
// creation with File Image
//

CCSpriteBatchNode* CCSpriteBatchNode::create ( const KDchar* szFilePath, KDuint uCapacity )
{
	CCSpriteBatchNode*  pRet = new CCSpriteBatchNode ( );
    
    if ( pRet && pRet->initWithFile ( szFilePath, uCapacity ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

//
// init with CCTexture2D
//
KDbool CCSpriteBatchNode::initWithTexture ( CCTexture2D* pTexture, KDuint uCapacity )
{
	m_tBlendFunc.src = CC_BLEND_SRC;
	m_tBlendFunc.dst = CC_BLEND_DST;
	m_pTextureAtlas  = new CCTextureAtlas ( );

	if ( 0 == uCapacity )
    {
        uCapacity = kDefaultSpriteBatchCapacity;
    }

	m_pTextureAtlas->initWithTexture ( pTexture, uCapacity );

	updateBlendFunc ( );

	// no lazy alloc in this node
	m_pChildren = new CCArray ( );
	m_pChildren->initWithCapacity ( uCapacity );

	m_pDescendants = new CCArray ( );
	m_pDescendants->initWithCapacity ( uCapacity );

	setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

	return KD_TRUE;
}

KDbool CCSpriteBatchNode::init ( KDvoid )
{
    CCTexture2D*  pTexture = new CCTexture2D ( );
    pTexture->autorelease ( );
    return this->initWithTexture ( pTexture, 0 );
}

//
// init with FileImage
//
KDbool CCSpriteBatchNode::initWithFile ( const KDchar* szFilePath, KDuint uCapacity )
{
	CCTexture2D*  pTexture2D = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );

	return initWithTexture ( pTexture2D, uCapacity );
}

CCSpriteBatchNode::CCSpriteBatchNode ( KDvoid )
{
    m_pTextureAtlas = KD_NULL;
    m_pDescendants  = KD_NULL;
}

CCSpriteBatchNode::~CCSpriteBatchNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pTextureAtlas );
	CC_SAFE_RELEASE ( m_pDescendants );
}

// override visit
// don't call visit on it's children
KDvoid CCSpriteBatchNode::visit ( KDvoid )
{
	CC_PROFILER_START_CATEGORY ( kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit" );

	// CAREFUL:
	// This visit is almost identical to CocosNode#visit
	// with the exception that it doesn't call visit on it's children
	//
	// The alternative is to have a void CCSprite#visit, but
	// although this is less maintainable, is faster
	//
	if ( !m_bVisible )
	{
		return;
	}

	kmGLPushMatrix ( );

	if ( m_pGrid && m_pGrid->isActive ( ) )
	{
		m_pGrid->beforeDraw ( );
		transformAncestors ( );
	}

	sortAllChildren ( );
	transform ( );

	draw ( );

	if ( m_pGrid && m_pGrid->isActive ( ) )
	{
		m_pGrid->afterDraw ( this );
	}

	kmGLPopMatrix ( );
	setOrderOfArrival ( 0 );

	CC_PROFILER_STOP_CATEGORY ( kCCProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit" );
}

KDvoid CCSpriteBatchNode::addChild ( CCNode* pChild, KDint nZOrder, KDint nTag )
{
	CCAssert ( pChild != KD_NULL, "child should not be null" );
    CCAssert ( dynamic_cast<CCSprite*> ( pChild ) != KD_NULL, "CCSpriteBatchNode only supports CCSprites as children" );

    CCSprite*  pSprite = (CCSprite *) pChild;
    // check CCSprite is using the same texture id
    CCAssert ( pSprite->getTexture ( )->getName ( ) == m_pTextureAtlas->getTexture ( )->getName ( ), "CCSprite is not using the same texture id" );

    CCNode::addChild ( pChild, nZOrder, nTag );

    appendChild ( pSprite );
}

KDvoid CCSpriteBatchNode::addChild ( CCNode* pChild )
{
	CCNode::addChild ( pChild );
}

KDvoid CCSpriteBatchNode::addChild ( CCNode* pChild, KDint nZOrder )
{
	CCNode::addChild ( pChild, nZOrder );
}

/// override reorderChild
KDvoid CCSpriteBatchNode::reorderChild ( CCNode* pChild, KDint nZOrder )
{
	CCAssert ( pChild != KD_NULL, "the child should not be null" );
	CCAssert ( m_pChildren->containsObject ( pChild ), "Child doesn't belong to Sprite" );

	if ( nZOrder == pChild->getZOrder ( ) )
	{
		return;
	}

	// set the z-order and sort later
    CCNode::reorderChild ( pChild, nZOrder );
}

/// override remove child
KDvoid CCSpriteBatchNode::removeChild ( CCNode* pChild, KDbool bCleanup )
{
	CCSprite*  pSprite = (CCSprite *) ( pChild );

	// explicit null handling
	if ( pSprite == KD_NULL )
	{
		return;
	}

	CCAssert ( m_pChildren->containsObject ( pSprite ), "sprite batch node should contain the child" );

	// cleanup before removing
	removeSpriteFromAtlas ( pSprite );

	CCNode::removeChild ( pSprite, bCleanup );
}

KDvoid CCSpriteBatchNode::removeChildAtIndex ( KDuint uIndex, KDbool bCleanup )
{
	removeChild ( (CCSprite *) m_pChildren->objectAtIndex ( uIndex ), bCleanup );
}

KDvoid CCSpriteBatchNode::removeAllChildrenWithCleanup ( KDbool bCleanup )
{
	// Invalidate atlas index. issue #569
    // useSelfRender should be performed on all descendants. issue #1216
    arrayMakeObjectsPerformSelectorWithObject ( m_pDescendants, setBatchNode, KD_NULL, CCSprite* );

    CCNode::removeAllChildrenWithCleanup ( bCleanup );

    m_pDescendants->removeAllObjects ( );
    m_pTextureAtlas->removeAllQuads ( );
}

//override sortAllChildren
KDvoid CCSpriteBatchNode::sortAllChildren ( KDvoid )
{
    if ( m_bReorderChildDirty )
    {
        KDint     i = 0, j = 0, nLength = m_pChildren->m_pData->num;
        CCNode**  pNodes = (CCNode**) m_pChildren->m_pData->arr;
        CCNode*   pTempItem = KD_NULL;

        //insertion sort
        for ( i = 1; i < nLength; i++ )
        {
            pTempItem = pNodes [ i ];
            j = i - 1;

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but orderOfArrival is smaller
            while ( j >= 0 && ( pTempItem->getZOrder ( ) < pNodes [ j ]->getZOrder ( ) || ( pTempItem->getZOrder ( ) == pNodes [ j ]->getZOrder ( ) && pTempItem->getOrderOfArrival ( ) < pNodes [ j ]->getOrderOfArrival ( ) ) ) )
            {
                pNodes [ j + 1 ] = pNodes [ j ];
                j--;
            }

            pNodes [ j +  1 ] = pTempItem;
        }

        //sorted now check all children
        if ( m_pChildren->count ( ) > 0 )
        {
            // first sort all children recursively based on zOrder
            arrayMakeObjectsPerformSelector ( m_pChildren, sortAllChildren, CCSprite* );

            KDint  nIndex = 0;

            CCObject*  pObject = KD_NULL;
            // fast dispatch, give every child a new atlasIndex based on their relative zOrder (keep parent -> child relations intact)
            // and at the same time reorder descendants and the quads to the right index
            CCARRAY_FOREACH ( m_pChildren, pObject )
            {
                CCSprite*  pChild = (CCSprite*) pObject;
                updateAtlasIndex ( pChild, &nIndex );
            }
        }

        m_bReorderChildDirty = KD_FALSE;
    }
}

KDvoid CCSpriteBatchNode::updateAtlasIndex ( CCSprite* pSprite, KDint* pCurIndex )
{
    KDuint    uCount = 0;
    CCArray*  pArray = pSprite->getChildren ( );

    if ( pArray != KD_NULL )
    {
        uCount = pArray->count ( );
    }
    
    KDint   nOldIndex = 0;

    if ( uCount == 0 )
    {
        nOldIndex = pSprite->getAtlasIndex ( );
        pSprite->setAtlasIndex ( *pCurIndex );
        pSprite->setOrderOfArrival ( 0 );

        if ( nOldIndex != *pCurIndex ) 
		{
            swap ( nOldIndex, *pCurIndex );
        }

        ( *pCurIndex )++;
    }
    else
    {
        KDbool  bNeedNewIndex = KD_TRUE;

        if ( ( (CCSprite*) pArray->m_pData->arr [ 0 ] )->getZOrder ( ) >= 0 )
        {
            // all children are in front of the parent
            nOldIndex = pSprite->getAtlasIndex ( );
            pSprite->setAtlasIndex ( *pCurIndex );
            pSprite->setOrderOfArrival ( 0 );

            if ( nOldIndex != *pCurIndex )
            {
                swap ( nOldIndex, *pCurIndex );
            }

            ( *pCurIndex )++;

            bNeedNewIndex = KD_FALSE;
        }

        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pArray, pObject )
        {
            CCSprite*  pChild = (CCSprite*) pObject;

            if ( bNeedNewIndex && pChild->getZOrder ( ) >= 0 )
            {
                nOldIndex = pSprite->getAtlasIndex ( );
                pSprite->setAtlasIndex ( *pCurIndex );
                pSprite->setOrderOfArrival ( 0 );

                if ( nOldIndex != *pCurIndex ) 
				{
                    this->swap ( nOldIndex, *pCurIndex );
                }

                ( *pCurIndex )++;

                bNeedNewIndex = KD_FALSE;
            }

            updateAtlasIndex ( pChild, pCurIndex );
        }

        if ( bNeedNewIndex )
        {
			// all children have a zOrder < 0)
            nOldIndex = pSprite->getAtlasIndex ( );
            pSprite->setAtlasIndex ( *pCurIndex );
            pSprite->setOrderOfArrival ( 0 );

            if ( nOldIndex != *pCurIndex ) 
			{
                swap ( nOldIndex, *pCurIndex );
            }

            ( *pCurIndex )++;
        }
    }
}

KDvoid CCSpriteBatchNode::swap ( KDint nOldIndex, KDint nNewIndex )
{
    CCObject**			 pObjects = m_pDescendants->m_pData->arr;
    ccV3F_C4B_T2F_Quad*  pQuads   = m_pTextureAtlas->getQuads ( );

    CCObject*			 pTempItem   = pObjects [ nOldIndex ];
    ccV3F_C4B_T2F_Quad	 tTempItemQuad = pQuads [ nOldIndex ];

    // update the index of other swapped item
    ( (CCSprite*) pObjects [ nNewIndex ] )->setAtlasIndex ( nOldIndex );

    pObjects [ nOldIndex ] = pObjects [ nNewIndex ];
    pQuads   [ nOldIndex ] = pQuads   [ nNewIndex ];
    pObjects [ nNewIndex ] = pTempItem;
    pQuads   [ nNewIndex ] = tTempItemQuad;
}

KDvoid CCSpriteBatchNode::reorderBatch ( KDbool bReorder )
{
    m_bReorderChildDirty = bReorder;
}

/// draw
KDvoid CCSpriteBatchNode::draw ( KDvoid )
{
	CC_PROFILER_START ( "CCSpriteBatchNode - draw" );

    // Optimization: Fast Dispatch
    if ( m_pTextureAtlas->getTotalQuads ( ) == 0 )
    {
        return;
    }

    CC_NODE_DRAW_SETUP ( );

    arrayMakeObjectsPerformSelector ( m_pChildren, updateTransform, CCSprite* );

    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    m_pTextureAtlas->drawQuads ( );

    CC_PROFILER_STOP ( "CCSpriteBatchNode - draw" );
}

KDvoid CCSpriteBatchNode::increaseAtlasCapacity ( KDvoid )
{
    // if we're going beyond the current TextureAtlas's capacity,
    // all the previously initialized sprites will need to redo their texture coords
    // this is likely computationally expensive
    KDuint  uQuantity = ( m_pTextureAtlas->getCapacity ( ) + 1 ) * 4 / 3;

    //CCLOG ( "XMCocos2D : CCSpriteBatchNode: resizing TextureAtlas capacity from [%lu] to [%lu].",
    //    (long) m_pTextureAtlas->getCapacity ( ),
    //    (long) uQuantity );

    if ( !m_pTextureAtlas->resizeCapacity ( uQuantity ) )
    {
        // serious problems
        CCLOG ( "XMCocos2D : WARNING: Not enough memory to resize the atlas" );
        CCAssert ( KD_FALSE, "Not enough memory to resize the atlas" );
    }
}

KDuint CCSpriteBatchNode::rebuildIndexInOrder ( CCSprite* pParent, KDuint uIndex )
{
	CCArray*  pChildren = pParent->getChildren ( );

	if ( pChildren && pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( pChildren, pObject )
		{
			CCSprite*  pChild = (CCSprite *) pObject;
			if ( pChild && ( pChild->getZOrder() < 0 ) )
			{
				uIndex = rebuildIndexInOrder ( pChild, uIndex );
			}
		}
	}	

	// ignore self (batch node)
	if ( !pParent->isEqual ( this ) )
	{
		pParent->setAtlasIndex ( uIndex );
		uIndex++;
	}

	if ( pChildren && pChildren->count ( ) > 0 )
	{
        CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( pChildren, pObject )
        {
            CCSprite*  pChild = (CCSprite *) pObject;
            if ( pChild && ( pChild->getZOrder ( ) >= 0 ) )
            {
                uIndex = rebuildIndexInOrder ( pChild, uIndex );
            }
        }
	}

	return uIndex;
}

KDuint CCSpriteBatchNode::highestAtlasIndexInChild ( CCSprite* pSprite )
{
	CCArray*  pChildren = pSprite->getChildren ( );

	if ( !pChildren || pChildren->count ( ) == 0 )
	{
		return pSprite->getAtlasIndex ( );
	}
	else
	{
		return highestAtlasIndexInChild ( (CCSprite *) pChildren->lastObject ( ) );
	}
}

KDuint CCSpriteBatchNode::lowestAtlasIndexInChild ( CCSprite* pSprite )
{
	CCArray*  pChildren = pSprite->getChildren ( );

	if ( !pChildren || pChildren->count ( ) == 0 )
	{
		return pSprite->getAtlasIndex ( );
	}
	else
	{
		return lowestAtlasIndexInChild ( (CCSprite *) pChildren->objectAtIndex ( 0 ) );
	}
}

KDuint CCSpriteBatchNode::atlasIndexForChild ( CCSprite* pSprite, KDint nZOrder )
{
	CCArray*  pBrothers = pSprite->getParent ( )->getChildren ( );
	KDuint  uChildIndex = pBrothers->indexOfObject ( pSprite );

	// ignore parent Z if parent is spriteSheet
	KDbool  bIgnoreParent = (CCSpriteBatchNode *) ( pSprite->getParent ( ) ) == this;
	CCSprite*   pPrevious = KD_NULL;
	if ( uChildIndex > 0 && uChildIndex < KDUINT_MAX )
	{
		pPrevious = (CCSprite *) pBrothers->objectAtIndex ( uChildIndex - 1 );
	}

	// first child of the sprite sheet
	if ( bIgnoreParent )
	{
		if ( uChildIndex == 0 )
		{
			return 0;
		}

		return highestAtlasIndexInChild ( pPrevious ) + 1;
	}

	// parent is a CCSprite, so, it must be taken into account

	// first child of an CCSprite ?
	if ( uChildIndex == 0 )
	{
		CCSprite*  pParent = (CCSprite *) pSprite->getParent ( );

		// less than parent and brothers
		if ( nZOrder < 0 )
		{
			return pParent->getAtlasIndex ( );
		}
		else
		{
			return pParent->getAtlasIndex ( ) + 1;
		}
	}
	else
	{
		// previous & sprite belong to the same branch
		if ( ( pPrevious->getZOrder ( ) < 0 && nZOrder < 0 ) || ( pPrevious->getZOrder ( ) >= 0 && nZOrder >= 0 ) )
		{
			return highestAtlasIndexInChild ( pPrevious ) + 1;
		}

		// else (previous < 0 and sprite >= 0 )
		CCSprite*  pParent = (CCSprite*) pSprite->getParent ( );
		return pParent->getAtlasIndex ( ) + 1;
	}

	// Should not happen. Error calculating Z on SpriteSheet
	CCAssert ( 0, "should not run here" );
	return 0;
}

// add child helper
KDvoid CCSpriteBatchNode::insertChild ( CCSprite* pSprite, KDuint uIndex )
{
	pSprite->setBatchNode ( this );
	pSprite->setAtlasIndex ( uIndex );
	pSprite->setDirty ( KD_TRUE );

	if ( m_pTextureAtlas->getTotalQuads ( ) == m_pTextureAtlas->getCapacity ( ) )
	{
		increaseAtlasCapacity ( );
	}

	ccV3F_C4B_T2F_Quad  tQuad = pSprite->getQuad ( );
	m_pTextureAtlas->insertQuad ( &tQuad, uIndex );

	ccArray*  pDescendantsData = m_pDescendants->m_pData;
    ccArrayInsertObjectAtIndex ( pDescendantsData, pSprite, uIndex );

    // update indices
    CCSprite*  pChild = NULL;
    for ( KDuint  i = uIndex + 1; i < pDescendantsData->num; i++ ) 
	{
        pChild = (CCSprite*) pDescendantsData->arr [ i ];
        pChild->setAtlasIndex ( pChild->getAtlasIndex ( ) + 1 );
    }

    // add children recursively
    CCObject*  pObject = NULL;
    CCARRAY_FOREACH ( pSprite->getChildren ( ), pObject )
    {
        pChild = (CCSprite*) pObject;
        KDuint uIdx = atlasIndexForChild ( pChild, pChild->getZOrder ( ) );
        insertChild ( pChild, uIdx );
    }
}

// addChild helper, faster than insertChild
KDvoid CCSpriteBatchNode::appendChild ( CCSprite* pSprite )
{
    m_bReorderChildDirty = KD_TRUE;
    pSprite->setBatchNode ( this );
    pSprite->setDirty ( KD_TRUE );

    if ( m_pTextureAtlas->getTotalQuads ( ) == m_pTextureAtlas->getCapacity ( ) )
	{
        increaseAtlasCapacity ( );
    }

    ccArray*  pDescendantsData = m_pDescendants->m_pData;

    ccArrayAppendObjectWithResize ( pDescendantsData, pSprite );

    KDuint  uIndex = pDescendantsData->num - 1;

    pSprite->setAtlasIndex ( uIndex );

    ccV3F_C4B_T2F_Quad  tQuad = pSprite->getQuad ( );
    m_pTextureAtlas->insertQuad ( &tQuad, uIndex );

    // add children recursively
    
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pSprite->getChildren ( ), pObject )
    {
        CCSprite*  pChild = (CCSprite*) pObject;
        appendChild ( pChild );
    }
}

KDvoid CCSpriteBatchNode::removeSpriteFromAtlas ( CCSprite* pSprite )
{
	// remove from TextureAtlas
	m_pTextureAtlas->removeQuadAtIndex ( pSprite->getAtlasIndex ( ) );

	// Cleanup sprite. It might be reused (issue #569)
	pSprite->setBatchNode ( KD_NULL );

	KDuint  uIndex = m_pDescendants->indexOfObject ( pSprite );
	if ( uIndex != KDUINT_MAX )
	{
		m_pDescendants->removeObjectAtIndex ( uIndex );

		// update all sprites beyond this one
		KDuint  uCount = m_pDescendants->count ( );

		for ( ; uIndex < uCount; ++uIndex )
		{
			CCSprite*  pDescendant = (CCSprite *) m_pDescendants->objectAtIndex ( uIndex );
			pDescendant->setAtlasIndex ( pDescendant->getAtlasIndex ( ) - 1 );
		}
	}

	// remove children recursively
	CCArray*  pChildren = pSprite->getChildren ( );
	if ( pChildren && pChildren->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
		CCARRAY_FOREACH ( pChildren, pObject )
		{
			CCSprite*  pChild = (CCSprite *) pObject;
			if ( pChild )
			{
				removeSpriteFromAtlas ( pChild );
			}
		}
	}
}

KDvoid CCSpriteBatchNode::updateBlendFunc ( KDvoid )
{
	if ( !m_pTextureAtlas->getTexture ( )->hasPremultipliedAlpha ( ) )
	{
		m_tBlendFunc.src = GL_SRC_ALPHA;
		m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	}
}

/// CocosNodeTexture protocol
KDvoid CCSpriteBatchNode::setBlendFunc ( ccBlendFunc tBlendFunc )
{
	m_tBlendFunc = tBlendFunc;
}

ccBlendFunc CCSpriteBatchNode::getBlendFunc ( KDvoid )
{
	return m_tBlendFunc;
}

CCTexture2D* CCSpriteBatchNode::getTexture ( KDvoid )
{
	return m_pTextureAtlas->getTexture ( );
}

KDvoid CCSpriteBatchNode::setTexture ( CCTexture2D* pTexture )
{
	m_pTextureAtlas->setTexture ( pTexture );
	updateBlendFunc ( );
}

// CCSpriteSheet Extension
//implementation CCSpriteSheet (TMXTiledMapExtension)

KDvoid CCSpriteBatchNode::insertQuadFromSprite ( CCSprite* pSprite, KDuint uIndex )
{
    CCAssert ( pSprite != KD_NULL, "Argument must be non-nil" );
	CCAssert ( dynamic_cast<CCSprite*> ( pSprite ), "CCSpriteBatchNode only supports CCSprites as children" );

	// make needed room
    while ( uIndex >= m_pTextureAtlas->getCapacity ( ) || m_pTextureAtlas->getCapacity ( ) == m_pTextureAtlas->getTotalQuads ( ) )
    {
        this->increaseAtlasCapacity ( );
    }
    //
    // update the quad directly. Don't add the sprite to the scene graph
    //
    pSprite->setBatchNode ( this );
    pSprite->setAtlasIndex ( uIndex );

    ccV3F_C4B_T2F_Quad  tQuad = pSprite->getQuad ( );
    m_pTextureAtlas->insertQuad ( &tQuad, uIndex );

    // XXX: updateTransform will update the textureAtlas too, using updateQuad.
    // XXX: so, it should be AFTER the insertQuad
    pSprite->setDirty ( KD_TRUE );
    pSprite->updateTransform ( );
}

KDvoid CCSpriteBatchNode::updateQuadFromSprite ( CCSprite* pSprite, KDuint uIndex )
{
    CCAssert ( pSprite != KD_NULL, "Argument must be non-nil" );
    CCAssert ( dynamic_cast<CCSprite*> ( pSprite ) != KD_NULL, "CCSpriteBatchNode only supports CCSprites as children" );
    
	// make needed room
	while ( uIndex >= m_pTextureAtlas->getCapacity ( ) || m_pTextureAtlas->getCapacity ( ) == m_pTextureAtlas->getTotalQuads ( ) )
    {
		this->increaseAtlasCapacity ( );
    }
    
	//
	// update the quad directly. Don't add the sprite to the scene graph
	//
	pSprite->setBatchNode  ( this );
    pSprite->setAtlasIndex ( uIndex );
    
	pSprite->setDirty ( KD_TRUE );
	
	// UpdateTransform updates the textureAtlas quad
	pSprite->updateTransform ( );
}

CCSpriteBatchNode* CCSpriteBatchNode::addSpriteWithoutQuad ( CCSprite* pChild, KDuint uZOrder, KDint nTag )
{
    CCAssert ( pChild != KD_NULL, "Argument must be non-nil" );
	CCAssert ( dynamic_cast<CCSprite*> ( pChild ), "CCSpriteBatchNode only supports CCSprites as children" );

    // quad index is Z
    pChild->setAtlasIndex ( uZOrder );

    // XXX: optimize with a binary search
    KDint  i = 0;
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pDescendants, pObject )
    {
        CCSprite*  pChild = (CCSprite*) pObject;
        if ( pChild && ( pChild->getAtlasIndex ( ) >= uZOrder ) )
        {
            ++i;
        }
    }

	m_pDescendants->insertObject ( pChild, i );

    // IMPORTANT: Call super, and not self. Avoid adding it to the texture atlas array
    CCNode::addChild ( pChild, uZOrder, nTag );

	//#issue 1262 don't use lazy sorting, tiles are added as quads not as sprites, so sprites need to be added in order
    reorderBatch ( KD_FALSE );

    return this;
}


NS_CC_END