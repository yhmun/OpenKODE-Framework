/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleSystemQuad.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada 
 *      Copyright (c) 2009      Leonardo Kasperavičius
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
#include "particle_nodes/CCParticleSystemQuad.h"
#include "particle_nodes/CCParticleBatchNode.h"
#include "sprite_nodes/CCSpriteFrame.h"
#include "textures/CCTextureAtlas.h"
#include "support/CCDirector.h"
#include "support/CCNotificationCenter.h"
#include "support/TransformUtils.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN
    
//
// implementation CCParticleSystemQuad
//

/// overriding the init method
KDbool CCParticleSystemQuad::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	// base initialization
	if ( CCParticleSystem::initWithTotalParticles ( uNumberOfParticles ) ) 
	{
        // allocating data space
        if ( !this->allocMemory ( ) )
		{
            this->release ( );
            return KD_FALSE;
        }

        initIndices ( );
	#if CC_TEXTURE_ATLAS_USE_VAO
        setupVBOandVAO ( );
	#else
        setupVBO ( );
	#endif

        setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );
                
        // Need to listen the event only when not use batchnode, because it will use VBO
		CCNotificationCenter::sharedNotificationCenter ( )->addObserver
		(
			this, callfuncO_selector ( CCParticleSystemQuad::listenBackToForeground ),
			EVENT_COME_TO_FOREGROUND, NULL
		);

		return KD_TRUE;
	}

	return KD_FALSE;
}

CCParticleSystemQuad::CCParticleSystemQuad ( KDvoid )
{
    m_pQuads    = KD_NULL;
    m_pIndices  = KD_NULL;
#if CC_TEXTURE_ATLAS_USE_VAO
    m_uVAOname  = 0;
#endif

	kdMemset ( m_pBuffersVBO, 0, sizeof ( m_pBuffersVBO ) );
}

CCParticleSystemQuad::~CCParticleSystemQuad ( KDvoid )
{
    if ( KD_NULL == m_pBatchNode )
    {
        CC_SAFE_FREE ( m_pQuads );
        CC_SAFE_FREE ( m_pIndices );
        glDeleteBuffers ( 2, &m_pBuffersVBO [ 0 ] );
#if CC_TEXTURE_ATLAS_USE_VAO
        glDeleteVertexArrays ( 1, &m_uVAOname );
#endif
    }
    
	CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, EVENT_COME_TO_FOREGROUND );
}

/// implementation CCParticleSystemQuad

CCParticleSystemQuad* CCParticleSystemQuad::create ( const KDchar* szPlistPath )
{
    CCParticleSystemQuad*  pRet = new CCParticleSystemQuad ( );

    if ( pRet && pRet->initWithFile ( szPlistPath ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
	return pRet;
}

CCParticleSystemQuad* CCParticleSystemQuad::createWithTotalParticles ( KDuint uNumberOfParticles ) 
{
	CCParticleSystemQuad*  pRet = new CCParticleSystemQuad ( );

    if ( pRet && pRet->initWithTotalParticles ( uNumberOfParticles ) )
    {
        pRet->autorelease ( );
    }
	else
	{
		CC_SAFE_DELETE ( pRet );
	}
	
    return pRet;
}

/// pointRect should be in Texture coordinates, not pixel coordinates
KDvoid CCParticleSystemQuad::initTexCoordsWithRect ( const CCRect& tPointRect )
{
	// convert to Tex coords

    CCRect   tRect = CCRectMake (
        tPointRect.origin.x * CC_CONTENT_SCALE_FACTOR ( ),
        tPointRect.origin.y * CC_CONTENT_SCALE_FACTOR ( ),
        tPointRect.size.cx  * CC_CONTENT_SCALE_FACTOR ( ),
        tPointRect.size.cy  * CC_CONTENT_SCALE_FACTOR ( ) );

	GLfloat  fWide = tPointRect.size.cx;
	GLfloat  fHigh = tPointRect.size.cy;

    if ( m_pTexture )
    {
        fWide = (GLfloat) m_pTexture->getPixelsWide ( );
        fHigh = (GLfloat) m_pTexture->getPixelsHigh ( );
    }
    
    GLfloat   left = tRect.origin.x / fWide;
    GLfloat bottom = tRect.origin.y / fHigh;
    GLfloat  right =   left + tRect.size.cx / fWide;
    GLfloat    top = bottom + tRect.size.cy / fHigh; 

	// Important. Texture in cocos2d are inverted, so the Y component should be inverted
	CC_SWAP ( top, bottom, KDfloat );

    ccV3F_C4B_T2F_Quad*  pQuads = KD_NULL;
    KDuint  uStart = 0, uEnd = 0;
    if ( m_pBatchNode )
    {
        pQuads = m_pBatchNode->getTextureAtlas ( )->getQuads ( );
        uStart = m_uAtlasIndex;
        uEnd   = m_uAtlasIndex + m_uTotalParticles;
    }
    else
    {
        pQuads = m_pQuads;
        uStart = 0;
        uEnd   = m_uTotalParticles;
    }

	for ( KDuint i = 0; i < m_uTotalParticles; i++ ) 
	{
		// bottom-left vertex:
		pQuads[i].bl.texCoords.u = left;
		pQuads[i].bl.texCoords.v = bottom;
        
		// bottom-right vertex:
		pQuads[i].br.texCoords.u = right;
		pQuads[i].br.texCoords.v = bottom;
        
		// top-left vertex:
		pQuads[i].tl.texCoords.u = left;
		pQuads[i].tl.texCoords.v = top;
        
		// top-right vertex:
		pQuads[i].tr.texCoords.u = right;
		pQuads[i].tr.texCoords.v = top;
	}
}

KDvoid CCParticleSystemQuad::setTextureWithRect ( CCTexture2D* pTexture, const CCRect& tRect )
{
	// Only update the texture if is different from the current one
	if( !m_pTexture || pTexture->getName ( ) != m_pTexture->getName ( ) )
	{
		CCParticleSystem::setTexture ( pTexture );
	}

	this->initTexCoordsWithRect ( tRect );
}

KDvoid CCParticleSystemQuad::setTexture ( CCTexture2D* pTexture )
{
	this->setTextureWithRect ( pTexture, CCRect ( CCPointZero, pTexture->getContentSize ( ) ) );
}

KDvoid CCParticleSystemQuad::setDisplayFrame ( CCSpriteFrame* pSpriteFrame )
{
	CCAssert ( pSpriteFrame->getOffset ( ).equals ( CCPointZero ), "QuadParticle only supports SpriteFrames with no offsets" );

	// update texture before updating texture rect
	if ( !m_pTexture || pSpriteFrame->getTexture ( )->getName ( ) != m_pTexture->getName ( ) )
	{
		this->setTexture ( pSpriteFrame->getTexture ( ) );
	}
}

KDvoid CCParticleSystemQuad::initIndices ( KDvoid )
{
	for ( KDuint i = 0; i < m_uTotalParticles; ++i)
	{
        KDuint i6 = i * 6;
        KDuint i4 = i * 4;
        
		m_pIndices [ i6 + 0 ] = i4 + 0;
		m_pIndices [ i6 + 1 ] = i4 + 1;
		m_pIndices [ i6 + 2 ] = i4 + 2;

		m_pIndices [ i6 + 5 ] = i4 + 1;
		m_pIndices [ i6 + 4 ] = i4 + 2;
		m_pIndices [ i6 + 3 ] = i4 + 3;
	}
}

KDvoid CCParticleSystemQuad::updateQuadWithParticle ( tCCParticle* pParticle, const CCPoint& tNewPosition )
{
	// colors
	ccV3F_C4B_T2F_Quad*  pQuad;

	if ( m_pBatchNode )
    {
        ccV3F_C4B_T2F_Quad*  pBatchQuads = m_pBatchNode->getTextureAtlas ( )->getQuads ( );
        pQuad = &( pBatchQuads [ m_uAtlasIndex + pParticle->uAtlasIndex ] );
    }
    else
    {
        pQuad = &( m_pQuads [ m_uParticleIdx ] );
    }

    ccColor4B  tColor = ( m_bOpacityModifyRGB )
        ? ccc4 ( (GLubyte) ( pParticle->tColor.r * pParticle->tColor.a * 255 ), (GLubyte) ( pParticle->tColor.g * pParticle->tColor.a * 255 ), (GLubyte) ( pParticle->tColor.b * pParticle->tColor.a * 255 ), (GLubyte) ( pParticle->tColor.a * 255 ) )
        : ccc4 ( (GLubyte) ( pParticle->tColor.r * 255					     ), (GLubyte) ( pParticle->tColor.g * 255					    ), (GLubyte) ( pParticle->tColor.b * 255				       ), (GLubyte) ( pParticle->tColor.a * 255 ) );
    
	pQuad->bl.colors = tColor;
	pQuad->br.colors = tColor;
	pQuad->tl.colors = tColor;
	pQuad->tr.colors = tColor;

	// vertices
	GLfloat  fSize2 = pParticle->fSize / 2;
	if ( pParticle->fRotation ) 
	{
		GLfloat  x1 = -fSize2;
		GLfloat  y1 = -fSize2;

		GLfloat  x2 = fSize2;
		GLfloat  y2 = fSize2;
		GLfloat  x = tNewPosition.x;
		GLfloat  y = tNewPosition.y;

		GLfloat  r = (GLfloat) -CC_DEGREES_TO_RADIANS ( pParticle->fRotation );
		GLfloat  cr = kdCosf ( r );
		GLfloat  sr = kdSinf ( r );
		GLfloat  ax = x1 * cr - y1 * sr + x;
		GLfloat  ay = x1 * sr + y1 * cr + y;
		GLfloat  bx = x2 * cr - y1 * sr + x;
		GLfloat  by = x2 * sr + y1 * cr + y;
		GLfloat  cx = x2 * cr - y2 * sr + x;
		GLfloat  cy = x2 * sr + y2 * cr + y;
		GLfloat  dx = x1 * cr - y2 * sr + x;
		GLfloat  dy = x1 * sr + y2 * cr + y;

		// bottom-left
		pQuad->bl.vertices.x = ax;
		pQuad->bl.vertices.y = ay;

		// bottom-right vertex:
		pQuad->br.vertices.x = bx;
		pQuad->br.vertices.y = by;

		// top-left vertex:
		pQuad->tl.vertices.x = dx;
		pQuad->tl.vertices.y = dy;

		// top-right vertex:
		pQuad->tr.vertices.x = cx;
		pQuad->tr.vertices.y = cy;
	}
	else
	{
		// bottom-left vertex:
		pQuad->bl.vertices.x = tNewPosition.x - fSize2;
		pQuad->bl.vertices.y = tNewPosition.y - fSize2;

		// bottom-right vertex:
		pQuad->br.vertices.x = tNewPosition.x + fSize2;
		pQuad->br.vertices.y = tNewPosition.y - fSize2;

		// top-left vertex:
		pQuad->tl.vertices.x = tNewPosition.x - fSize2;
		pQuad->tl.vertices.y = tNewPosition.y + fSize2;

		// top-right vertex:
		pQuad->tr.vertices.x = tNewPosition.x + fSize2;
		pQuad->tr.vertices.y = tNewPosition.y + fSize2;		
	}
}

KDvoid CCParticleSystemQuad::postStep ( KDvoid )
{
	glBindBuffer    ( GL_ARRAY_BUFFER, m_pBuffersVBO [ 0 ] );

	// Option 1: Sub Data
	glBufferSubData ( GL_ARRAY_BUFFER, 0, sizeof ( m_pQuads[0] ) * m_uTotalParticles, m_pQuads );

	// Option 2: Data
	// glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * m_uParticleCount, m_pQuads, GL_DYNAMIC_DRAW );

	// Option 3: Orphaning + glMapBuffer
	// glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * m_uTotalParticles, KD_NULL, GL_STREAM_DRAW );
	// KDvoid*  pBuff = glMapBuffer ( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	// kdMemcpy ( pBuff, m_pQuads, sizeof ( m_pQuads[0] ) * m_uTotalParticles );
	// glUnmapBuffer ( GL_ARRAY_BUFFER );

	glBindBuffer    ( GL_ARRAY_BUFFER, 0 );

	CHECK_GL_ERROR_DEBUG ( );
}

// overriding draw method
KDvoid CCParticleSystemQuad::draw ( KDvoid )
{	
    CCAssert ( !m_pBatchNode, "draw should not be called when added to a particleBatchNode" );

    CC_NODE_DRAW_SETUP ( );

    ccGLBindTexture2D ( m_pTexture->getName ( ) );
    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    CCAssert ( m_uParticleIdx == m_uParticleCount, "Abnormal error in particle quad" );

#if CC_TEXTURE_ATLAS_USE_VAO
    //
    // Using VBO and VAO
    //
    ccGLBindVAO ( m_uVAOname );

#if CC_REBIND_INDICES_BUFFER
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO [1] );
#endif

    glDrawElements ( GL_TRIANGLES, (GLsizei) m_uParticleIdx * 6, GL_UNSIGNED_SHORT, 0 );

#if CC_REBIND_INDICES_BUFFER
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
#endif

    ccGLBindVAO ( 0 );

#else
    //
    // Using VBO without VAO
    //

    #define kQuadSize   sizeof ( m_pQuads [ 0 ].bl )

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );

    glBindBuffer ( GL_ARRAY_BUFFER, m_pBuffersVBO [ 0 ] );
  
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 3, GL_FLOAT        , GL_FALSE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, vertices  ) );
    ccGLVertexAttribPointer ( kCCVertexAttrib_Color    , 4, GL_UNSIGNED_BYTE, GL_TRUE , kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, colors    ) );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT        , GL_FALSE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, texCoords ) );
    
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO [ 1 ] );

    glDrawElements ( GL_TRIANGLES, (GLsizei) m_uParticleIdx * 6, GL_UNSIGNED_SHORT, 0 );

    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

#endif

    CC_INCREMENT_GL_DRAWS ( 1 );
    CHECK_GL_ERROR_DEBUG ( );
}

KDvoid CCParticleSystemQuad::setTotalParticles ( KDuint uTotal )
{
    // If we are setting the total number of particles to a number higher
    // than what is allocated, we need to allocate new arrays
    if ( uTotal > m_uAllocatedParticles )
    {
        // Allocate new memory
        KDsize  nParticlesSize	= uTotal * sizeof ( tCCParticle );
        KDsize  nQuadsSize		= sizeof ( m_pQuads   [ 0 ] ) * uTotal * 1;
        KDsize  nIndicesSize	= sizeof ( m_pIndices [ 0 ] ) * uTotal * 6 * 1;

        tCCParticle*		 pParticlesNew	= (tCCParticle*)		kdRealloc ( m_pParticles, nParticlesSize );
        ccV3F_C4B_T2F_Quad*  pQuadsNew		= (ccV3F_C4B_T2F_Quad*) kdRealloc ( m_pQuads	, nQuadsSize	 );
        GLushort*			 pIndicesNew	= (GLushort*)			kdRealloc ( m_pIndices	, nIndicesSize   );

        if ( pParticlesNew && pQuadsNew && pIndicesNew )
        {
            // Assign pointers
            m_pParticles = pParticlesNew;
            m_pQuads	 = pQuadsNew;
            m_pIndices	 = pIndicesNew;

            // Clear the memory
			// XXX: Bug? If the quads are cleared, then drawing doesn't work... WHY??? XXX
            kdMemset ( m_pParticles , 0, nParticlesSize	);
            kdMemset ( m_pQuads		, 0, nQuadsSize		);
            kdMemset ( m_pIndices	, 0, nIndicesSize	);

            m_uAllocatedParticles = uTotal;
        }
        else
        {
            // Out of memory, failed to resize some array
            if ( pParticlesNew ) m_pParticles = pParticlesNew;
            if ( pQuadsNew     ) m_pQuads	  = pQuadsNew;
            if ( pIndicesNew   ) m_pIndices   = pIndicesNew;

            CCLOG ( "Particle system: out of memory" );
            return;
        }

        m_uTotalParticles = uTotal;

        // Init particles
        if ( m_pBatchNode )
        {
            for ( KDuint i = 0; i < m_uTotalParticles; i++ )
            {
                m_pParticles [ i ].uAtlasIndex = i;
            }
        }

        initIndices ( );
#if CC_TEXTURE_ATLAS_USE_VAO
        setupVBOandVAO ( );
#else
        setupVBO ( );
#endif
    }
    else
    {
        m_uTotalParticles = uTotal;
    }

	resetSystem ( );
}

#if CC_TEXTURE_ATLAS_USE_VAO
KDvoid CCParticleSystemQuad::setupVBOandVAO ( KDvoid )
{
    // clean VAO
    glDeleteBuffers ( 2, &m_pBuffersVBO[0] );
    ccGLDeleteVertexArrays ( 1, &m_uVAOname );

    glGenVertexArrays ( 1, &m_uVAOname );
    ccGLBindVAO ( m_uVAOname );

	#define kQuadSize sizeof ( m_pQuads[0].bl )

    glGenBuffers ( 2, &m_pBuffersVBO[0] );

    glBindBuffer ( GL_ARRAY_BUFFER, m_pBuffersVBO[0] );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * m_uTotalParticles, m_pQuads, GL_DYNAMIC_DRAW );

    // vertices
    glEnableVertexAttribArray ( kCCVertexAttrib_Position );
    glVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, vertices ) );

    // colors
    glEnableVertexAttribArray ( kCCVertexAttrib_Color );
    glVertexAttribPointer ( kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, colors ) );

    // tex coords
    glEnableVertexAttribArray ( kCCVertexAttrib_TexCoords );
    glVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, texCoords ) );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1] );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( m_pIndices[0] ) * m_uTotalParticles * 6, m_pIndices, GL_STATIC_DRAW );

	// Must unbind the VAO before changing the element buffer.
    ccGLBindVAO ( 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    CHECK_GL_ERROR_DEBUG ( );
}
#else

KDvoid CCParticleSystemQuad::setupVBO ( KDvoid )
{
	glDeleteBuffers ( 2, &m_pBuffersVBO [ 0 ]);		
    glGenBuffers ( 2, &m_pBuffersVBO [ 0 ] );

    glBindBuffer ( GL_ARRAY_BUFFER, m_pBuffersVBO [ 0 ] );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads [ 0 ] ) * m_uTotalParticles, m_pQuads, GL_DYNAMIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO [ 1 ] );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( m_pIndices [ 0 ] ) * m_uTotalParticles * 6, m_pIndices, GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

    CHECK_GL_ERROR_DEBUG ( );
}

#endif

KDvoid CCParticleSystemQuad::listenBackToForeground ( CCObject* pObject )
{
#if CC_TEXTURE_ATLAS_USE_VAO
        setupVBOandVAO ( );
#else
        setupVBO ( );
#endif
}

KDbool CCParticleSystemQuad::allocMemory ( KDvoid )
{
    CCAssert ( ( !m_pQuads && !m_pIndices), "Memory already alloced" );
    CCAssert ( !m_pBatchNode, "Memory should not be alloced when not using batchNode" );

    CC_SAFE_FREE ( m_pQuads );
    CC_SAFE_FREE ( m_pIndices );

    m_pQuads   = (ccV3F_C4B_T2F_Quad*) kdCalloc ( 1, m_uTotalParticles * sizeof ( ccV3F_C4B_T2F_Quad ) );
    m_pIndices = (GLushort*) kdCalloc ( 1, m_uTotalParticles * 6 * sizeof ( GLushort ) );
    
    if ( !m_pQuads || !m_pIndices ) 
    {
        CCLOG ( "XMCocos2D : Particle system: not enough memory" );
        CC_SAFE_FREE ( m_pQuads );
        CC_SAFE_FREE ( m_pIndices );

        return KD_FALSE;
    }

    return KD_TRUE;
}

KDvoid CCParticleSystemQuad::setBatchNode ( CCParticleBatchNode* pBatchNode )
{
    if ( m_pBatchNode != pBatchNode ) 
    {
        CCParticleBatchNode*  pOldBatch = m_pBatchNode;

        CCParticleSystem::setBatchNode ( pBatchNode );

        // NEW: is self render ?
        if ( !pBatchNode ) 
        {
            allocMemory ( );
            initIndices ( );
            setTexture ( pOldBatch->getTexture ( ) );
		#if CC_TEXTURE_ATLAS_USE_VAO
            setupVBOandVAO ( );
		#else
            setupVBO ( );
		#endif
        }
        // OLD: was it self render ? cleanup
        else if ( !pOldBatch )
        {
            // copy current state to batch
            ccV3F_C4B_T2F_Quad*  pBatchQuads = m_pBatchNode->getTextureAtlas ( )->getQuads ( );
            ccV3F_C4B_T2F_Quad*  pQuad = &( pBatchQuads [ m_uAtlasIndex ] );
            kdMemcpy ( pQuad, m_pQuads, m_uTotalParticles * sizeof ( m_pQuads [0] ) );

            CC_SAFE_FREE ( m_pQuads );
            CC_SAFE_FREE ( m_pIndices );

            glDeleteBuffers ( 2, &m_pBuffersVBO [ 0 ] );
		#if CC_TEXTURE_ATLAS_USE_VAO
            glDeleteVertexArrays ( 1, &m_uVAOname );
		#endif
        }
    }
}

CCParticleSystemQuad* CCParticleSystemQuad::create ( KDvoid )
{
    CCParticleSystemQuad*  pRet = new CCParticleSystemQuad ( );
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

NS_CC_END
