/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleBatchNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "particle_nodes/CCParticleBatchNode.h"
#include "particle_nodes/CCParticleSystem.h"
#include "effects/CCGrid.h"
#include "textures/CCTextureCache.h"
#include "textures/CCTextureAtlas.h"
#include "support/zip_support/ZipUtils.h"
#include "support/base64.h"
#include "support/CCImage.h"
#include "support/CCFileUtils.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

CCParticleBatchNode::CCParticleBatchNode ( KDvoid )
{
	m_pTextureAtlas	= KD_NULL;
}

CCParticleBatchNode::~CCParticleBatchNode ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pTextureAtlas );
}

//
// creation with CCTexture2D
//

CCParticleBatchNode* CCParticleBatchNode::createWithTexture ( CCTexture2D* pTexture, KDuint uCapacity )
{
    CCParticleBatchNode*  pRet = new CCParticleBatchNode ( );
    
	if ( pRet && pRet->initWithTexture ( pTexture, uCapacity ) )
    {
        pRet->autorelease ( );
        return pRet;
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

CCParticleBatchNode* CCParticleBatchNode::create ( const KDchar* szFilePath, KDuint uCapacity )
{
    CCParticleBatchNode*  pRet = new CCParticleBatchNode ( );
    
	if ( pRet && pRet->initWithFile ( szFilePath, uCapacity ) )
    {
        pRet->autorelease ( );
        return pRet;
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
KDbool CCParticleBatchNode::initWithTexture ( CCTexture2D* pTexture, KDuint uCapacity )
{
    m_pTextureAtlas = new CCTextureAtlas ( );
    m_pTextureAtlas->initWithTexture ( pTexture, uCapacity );

    // no lazy alloc in this node
    m_pChildren = new CCArray ( );
    m_pChildren->initWithCapacity ( uCapacity );

    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;

    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );
    
    return KD_TRUE;
}

//
// init with FileImage
//
KDbool CCParticleBatchNode::initWithFile ( const KDchar* szFilePath, KDuint uCapacity )
{
    CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );
    return initWithTexture ( pTexture, uCapacity );
}

// CCParticleBatchNode - composition

// override visit.
// Don't call visit on it's children
KDvoid CCParticleBatchNode::visit ( KDvoid )
{
    // CAREFUL:
    // This visit is almost identical to CCNode#visit
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

    transform ( );

    draw ( );

    if ( m_pGrid && m_pGrid->isActive ( ) )
    {
        m_pGrid->afterDraw ( this );
    }

    kmGLPopMatrix ( );
}

// override addChild:
KDvoid CCParticleBatchNode::addChild ( CCNode* pChild )
{
    CCNode::addChild ( pChild );
}

KDvoid CCParticleBatchNode::addChild ( CCNode* pChild, KDint nZOrder )
{
    CCNode::addChild ( pChild, nZOrder );
}

KDvoid CCParticleBatchNode::addChild ( CCNode* _pChild, KDint nZOrder, KDint nTag )
{
    CCAssert ( _pChild != KD_NULL, "Argument must be non-NULL" );
    CCAssert ( dynamic_cast<CCParticleSystem*> ( _pChild ) != KD_NULL, "CCParticleBatchNode only supports CCQuadParticleSystems as children" );
    CCParticleSystem*  pChild = (CCParticleSystem*) _pChild;
    CCAssert ( pChild->getTexture ( )->getName ( ) == m_pTextureAtlas->getTexture ( )->getName ( ), "CCParticleSystem is not using the same texture id" );
    // If this is the 1st children, then copy blending function
    if ( m_pChildren->count ( ) == 0 ) 
    {
        setBlendFunc ( pChild->getBlendFunc ( ) );
    }

    CCAssert ( m_tBlendFunc.src  == pChild->getBlendFunc ( ).src && m_tBlendFunc.dst  == pChild->getBlendFunc ( ).dst, "Can't add a PaticleSystem that uses a different blending function" );

    // no lazy sorting, so don't call super addChild, call helper instead
    KDuint  uPos = addChildHelper ( pChild, nZOrder, nTag );

    // get new atlasIndex
    KDuint  uAtlasIndex = 0;

    if ( uPos != 0 ) 
    {
        CCParticleSystem*  pParticle = (CCParticleSystem*) m_pChildren->objectAtIndex ( uPos - 1 );
        uAtlasIndex = pParticle->getAtlasIndex ( ) + pParticle->getTotalParticles ( );
    }
    else
    {
        uAtlasIndex = 0;
    }

    insertChild ( pChild, uAtlasIndex );

    // update quad info
    pChild->setBatchNode ( this );
}

// don't use lazy sorting, reordering the particle systems quads afterwards would be too complex
// XXX research whether lazy sorting + freeing current quads and calloc a new block with size of capacity would be faster
// XXX or possibly using vertexZ for reordering, that would be fastest
// this helper is almost equivalent to CCNode's addChild, but doesn't make use of the lazy sorting
KDuint CCParticleBatchNode::addChildHelper ( CCParticleSystem* pChild, KDint nZOrder, KDint nTag )
{
    CCAssert ( pChild != KD_NULL, "Argument must be non-nil" );
    CCAssert ( pChild->getParent ( ) == KD_NULL, "child already added. It can't be added again" );

    if ( !m_pChildren ) 
    {
        m_pChildren = new CCArray ( );
        m_pChildren->initWithCapacity ( 4 );
    }

    // don't use a lazy insert
    KDuint  uPos = searchNewPositionInChildrenForZ ( nZOrder );

    m_pChildren->insertObject ( pChild, uPos );

    pChild->setTag ( nTag );
    pChild->_setZOrder ( nZOrder );

    pChild->setParent ( this );

    if ( m_bRunning ) 
    {
        pChild->onEnter ( );
        pChild->onEnterTransitionDidFinish ( );
    }

    return uPos;
}

// Reorder will be done in this function, no "lazy" reorder to particles
KDvoid CCParticleBatchNode::reorderChild ( CCNode* _pChild, KDint nZOrder )
{
    CCAssert ( _pChild != KD_NULL, "Child must be non-NULL" );
    CCAssert ( dynamic_cast<CCParticleSystem*> ( _pChild ) != KD_NULL, "CCParticleBatchNode only supports CCQuadParticleSystems as children" );
    CCAssert ( m_pChildren->containsObject ( _pChild ), "Child doesn't belong to batch" );

    CCParticleSystem*  pChild = (CCParticleSystem*) ( _pChild );

    if ( nZOrder == pChild->getZOrder ( ) ) 
    {
        return;
    }

    // no reordering if only 1 child
    if ( m_pChildren->count ( ) > 1 )
    {
        KDuint  uNewIndex = 0,  uOldIndex = 0;

        getCurrentIndex ( &uOldIndex, &uNewIndex, pChild, nZOrder );

        if ( uOldIndex != uNewIndex )
        {

            // reorder m_pChildren->array
            pChild->retain ( );
            m_pChildren->removeObjectAtIndex ( uOldIndex );
            m_pChildren->insertObject ( pChild, uNewIndex );
            pChild->release ( );

            // save old altasIndex
            KDuint  uOldAtlasIndex = pChild->getAtlasIndex ( );

            // update atlas index
            updateAllAtlasIndexes ( );

            // Find new AtlasIndex
            KDuint  uNewAtlasIndex = 0;
            for ( KDuint i = 0; i < m_pChildren->count ( );i++ )
            {
                CCParticleSystem*  pNode = (CCParticleSystem*) m_pChildren->objectAtIndex ( i );
                if ( pNode == pChild ) 
                {
                    uNewAtlasIndex = pChild->getAtlasIndex ( );
                    break;
                }
            }

            // reorder textureAtlas quads
            m_pTextureAtlas->moveQuadsFromIndex ( uOldAtlasIndex, pChild->getTotalParticles ( ), uNewAtlasIndex );

            pChild->updateWithNoTime ( );
        }
    }

    pChild->_setZOrder ( nZOrder );
}

KDvoid CCParticleBatchNode::getCurrentIndex ( KDuint* pOldIndex, KDuint* pNewIndex, CCNode* _pChild, KDint nZOrder )
{
    KDbool bFoundCurrentIdx = KD_FALSE;
    KDbool bFoundNewIdx     = KD_FALSE;

    KDint  nMinusOne = 0;
    KDuint uCount = m_pChildren->count ( );

    for ( KDuint i = 0; i < uCount; i++ ) 
    {
        CCNode*  pNode = (CCNode*) m_pChildren->objectAtIndex ( i );

        // new index
        if ( pNode->getZOrder ( ) > nZOrder &&  !bFoundNewIdx ) 
        {
            *pNewIndex = i;
            bFoundNewIdx = KD_TRUE;

            if ( bFoundCurrentIdx && bFoundNewIdx )
            {
                break;
            }
        }

        // current index
        if ( _pChild == pNode ) 
        {
            *pOldIndex = i;
            bFoundCurrentIdx = KD_TRUE;

            if ( !bFoundNewIdx )
            {
                nMinusOne = -1;
            }

            if ( bFoundCurrentIdx && bFoundNewIdx )
            {
                break;
            }
        }

    }

    if ( !bFoundNewIdx )
    {
        *pNewIndex = uCount;
    }

    *pNewIndex += nMinusOne;
}

KDuint CCParticleBatchNode::searchNewPositionInChildrenForZ ( KDint nZOrder )
{
    KDuint  uCount = m_pChildren->count ( );

    for ( KDuint i = 0; i < uCount; i++ ) 
    {
        CCNode*  pChild = (CCNode*) m_pChildren->objectAtIndex ( i );
        if ( pChild->getZOrder ( ) > nZOrder )
        {
            return i;
        }
    }

    return uCount;
}

// override removeChild:
KDvoid  CCParticleBatchNode::removeChild ( CCNode* _pChild, KDbool bCleanup )
{
    // explicit nil handling
    if ( _pChild == KD_NULL )
    {
        return;
    }
    
    CCAssert ( dynamic_cast<CCParticleSystem*> ( _pChild ) != KD_NULL, "CCParticleBatchNode only supports CCQuadParticleSystems as children" );
    CCAssert ( m_pChildren->containsObject ( _pChild ), "CCParticleBatchNode doesn't contain the sprite. Can't remove it" );

    CCParticleSystem*  pChild = (CCParticleSystem*) _pChild;
	CCNode::removeChild ( pChild, bCleanup );
    
    // remove child helper
    m_pTextureAtlas->removeQuadsAtIndex ( pChild->getAtlasIndex ( ), pChild->getTotalParticles ( ) );

    // after memmove of data, empty the quads at the end of array
    m_pTextureAtlas->fillWithEmptyQuadsFromIndex ( m_pTextureAtlas->getTotalQuads ( ), pChild->getTotalParticles ( ) );

    // particle could be reused for self rendering
    pChild->setBatchNode ( KD_NULL );

    updateAllAtlasIndexes ( );
}

KDvoid CCParticleBatchNode::removeChildAtIndex ( KDuint uIndex, KDbool bCleanup )
{
    removeChild ( (CCParticleSystem*) m_pChildren->objectAtIndex ( uIndex ), bCleanup );
}

KDvoid CCParticleBatchNode::removeAllChildrenWithCleanup ( KDbool bCleanup )
{
    arrayMakeObjectsPerformSelectorWithObject ( m_pChildren, setBatchNode, KD_NULL, CCParticleSystem* );

    CCNode::removeAllChildrenWithCleanup ( bCleanup );

    m_pTextureAtlas->removeAllQuads ( );
}

KDvoid CCParticleBatchNode::draw ( KDvoid )
{
    CC_PROFILER_STOP ( "CCParticleBatchNode - draw" );

    if ( m_pTextureAtlas->getTotalQuads ( ) == 0 )
    {
        return;
    }

    CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    m_pTextureAtlas->drawQuads ( );

    CC_PROFILER_STOP ( "CCParticleBatchNode - draw" );
}

KDvoid CCParticleBatchNode::increaseAtlasCapacityTo ( KDuint uQuantity )
{
    CCLOG ( "XMCocos2D : CCParticleBatchNode: resizing TextureAtlas capacity from [%lu] to [%lu].",
          (long) m_pTextureAtlas->getCapacity ( ),
          (long) uQuantity );

    if ( !m_pTextureAtlas->resizeCapacity ( uQuantity ) ) 
	{
        // serious problems
        CCLOG ( "XMCocos2D : WARNING: Not enough memory to resize the atlas" );
        CCAssert ( KD_FALSE,"XXX: CCParticleBatchNode #increaseAtlasCapacity SHALL handle this assert" );
    }
}

//sets a 0'd quad into the quads array
KDvoid CCParticleBatchNode::disableParticle ( KDuint uParticleIndex )
{
    ccV3F_C4B_T2F_Quad*  pQuad = &( ( m_pTextureAtlas->getQuads ( ) ) [ uParticleIndex ] );
    pQuad->br.vertices.x = pQuad->br.vertices.y = pQuad->tr.vertices.x = pQuad->tr.vertices.y = pQuad->tl.vertices.x = pQuad->tl.vertices.y = pQuad->bl.vertices.x = pQuad->bl.vertices.y = 0.0f;
}

// CCParticleBatchNode - add / remove / reorder helper methods

// add child helper
KDvoid CCParticleBatchNode::insertChild ( CCParticleSystem* pSystem, KDuint uIndex )
{
    pSystem->setAtlasIndex ( uIndex );

    if ( m_pTextureAtlas->getTotalQuads ( ) + pSystem->getTotalParticles ( ) > m_pTextureAtlas->getCapacity ( ) )
    {
        increaseAtlasCapacityTo ( m_pTextureAtlas->getTotalQuads ( ) + pSystem->getTotalParticles ( ) );

        // after a realloc empty quads of textureAtlas can be filled with gibberish ( realloc doesn't perform calloc ), insert empty quads to prevent it
        m_pTextureAtlas->fillWithEmptyQuadsFromIndex ( m_pTextureAtlas->getCapacity ( ) - pSystem->getTotalParticles ( ), pSystem->getTotalParticles ( ) );
    }

    // make room for quads, not necessary for last child
    if ( pSystem->getAtlasIndex ( ) + pSystem->getTotalParticles ( ) != m_pTextureAtlas->getTotalQuads ( ) )
    {
        m_pTextureAtlas->moveQuadsFromIndex ( uIndex, uIndex + pSystem->getTotalParticles ( ) );
    }

    // increase totalParticles here for new particles, update method of particle-system will fill the quads
    m_pTextureAtlas->increaseTotalQuadsWith ( pSystem->getTotalParticles ( ) );

    updateAllAtlasIndexes ( );
}

//rebuild atlas indexes
KDvoid CCParticleBatchNode::updateAllAtlasIndexes ( KDvoid )
{
    KDuint     uIndex  = 0;
    CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( m_pChildren, pObject )
    {
        CCParticleSystem*  pChild = (CCParticleSystem*) pObject;
        pChild->setAtlasIndex ( uIndex );
        uIndex += pChild->getTotalParticles ( );
    }
}

// CCParticleBatchNode - CocosNodeTexture protocol

KDvoid CCParticleBatchNode::updateBlendFunc ( KDvoid )
{
    if ( !m_pTextureAtlas->getTexture ( )->hasPremultipliedAlpha ( ) ) 
	{
        m_tBlendFunc.src = GL_SRC_ALPHA;
        m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
}

KDvoid CCParticleBatchNode::setTexture ( CCTexture2D* pTexture )
{
    m_pTextureAtlas->setTexture ( pTexture );

    // If the new texture has No premultiplied alpha, AND the blendFunc hasn't been changed, then update it
    if ( pTexture && !pTexture->hasPremultipliedAlpha ( ) && ( m_tBlendFunc.src == CC_BLEND_SRC && m_tBlendFunc.dst == CC_BLEND_DST ) )
    {
        m_tBlendFunc.src = GL_SRC_ALPHA;
        m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
}

CCTexture2D* CCParticleBatchNode::getTexture ( KDvoid )
{
    return m_pTextureAtlas->getTexture ( );
}

KDvoid CCParticleBatchNode::setBlendFunc ( ccBlendFunc tBlendFunc )
{
    m_tBlendFunc = tBlendFunc;
}

// returns the blending function used for the texture
ccBlendFunc CCParticleBatchNode::getBlendFunc ( KDvoid )
{
    return m_tBlendFunc;
}

NS_CC_END
