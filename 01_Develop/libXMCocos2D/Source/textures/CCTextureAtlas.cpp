/* -----------------------------------------------------------------------------------
 *
 *      File            CCTextureAtlas.cpp
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
#include "textures/CCTextureAtlas.h"
#include "textures/CCTextureCache.h"
#include "cocoa/CCString.h"
#include "shaders/CCGLProgram.h"
#include "support/CCNotificationCenter.h"
#include "shaders/ccGLStateCache.h"

NS_CC_BEGIN
    
/// According to some tests GL_TRIANGLE_STRIP is slower, MUCH slower. Probably I'm doing something very wrong

//
// implementation CCTextureAtlas
//
CCTextureAtlas::CCTextureAtlas ( KDvoid )
{
	m_pIndices	= KD_NULL;
    m_pTexture	= KD_NULL;
    m_pQuads	= KD_NULL;
	m_bDirty	= KD_FALSE;

	m_pBuffersVBO [ 0 ] = 0;
	m_pBuffersVBO [ 1 ] = 0;
}

CCTextureAtlas::~CCTextureAtlas ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCTextureAtlas = %p.", this );

	CC_SAFE_FREE ( m_pQuads );
	CC_SAFE_FREE ( m_pIndices );

	glDeleteBuffers ( 2, m_pBuffersVBO ); 

#if CC_TEXTURE_ATLAS_USE_VAO
    glDeleteVertexArrays ( 1, &m_uVAOname );
	ccGLBindVAO ( 0 );
#endif

	CC_SAFE_RELEASE ( m_pTexture );

#if CC_ENABLE_CACHE_TEXTURE_DATA
	CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, EVENT_COME_TO_FOREGROUND );
#endif
}

KDuint CCTextureAtlas::getTotalQuads ( KDvoid )
{
	return m_uTotalQuads;
}

KDuint CCTextureAtlas::getCapacity ( KDvoid )
{
	return m_uCapacity;
}

CCTexture2D* CCTextureAtlas::getTexture ( KDvoid )
{
	return m_pTexture;
}

KDvoid CCTextureAtlas::setTexture ( CCTexture2D* pTexture )
{
	CC_SAFE_RETAIN  ( pTexture );
	CC_SAFE_RELEASE ( m_pTexture );
	m_pTexture = pTexture;
}

ccV3F_C4B_T2F_Quad* CCTextureAtlas::getQuads ( KDvoid )
{
    //if someone accesses the quads directly, presume that changes will be made
    m_bDirty = KD_TRUE;
	return m_pQuads;
}

KDvoid CCTextureAtlas::setQuads ( ccV3F_C4B_T2F_Quad* pQuads )
{
	m_pQuads = pQuads;
}

/// TextureAtlas - alloc & init

CCTextureAtlas* CCTextureAtlas::create ( const KDchar* szFilePath, KDuint uCapacity )
{
	CCTextureAtlas*  pRet = new CCTextureAtlas ( );

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

CCTextureAtlas* CCTextureAtlas::createWithTexture ( CCTexture2D* pTexture, KDuint uCapacity )
{
	CCTextureAtlas*  pRet = new CCTextureAtlas ( );

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

KDbool CCTextureAtlas::initWithFile ( const KDchar* szFilePath, KDuint uCapacity )
{
	// retained in property
	CCTexture2D*  pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );

	if ( pTexture )
	{
        return initWithTexture ( pTexture, uCapacity );
	}
	else
	{
		CCLOG ( "XMCocos2D : Could not open file: %s", szFilePath );
		return KD_FALSE;
	}
}

KDbool CCTextureAtlas::initWithTexture ( CCTexture2D* pTexture, KDuint uCapacity )
{
//	CCAssert ( pTexture != KD_NULL, "texture should not be null" );
    
	m_uCapacity   = uCapacity;
	m_uTotalQuads = 0;

	// retained in property
	m_pTexture = pTexture;
	CC_SAFE_RETAIN ( m_pTexture );

	// Re-initialization is not allowed
	CCAssert ( m_pQuads == KD_NULL && m_pIndices == KD_NULL, "" );

	m_pQuads   = (ccV3F_C4B_T2F_Quad *) kdCalloc ( 1, sizeof ( ccV3F_C4B_T2F_Quad ) * m_uCapacity );
	m_pIndices = (GLushort *) kdCalloc ( 1, sizeof ( GLushort ) * m_uCapacity * 6 );
	if ( !( m_pQuads && m_pIndices ) && m_uCapacity > 0 ) 
	{
		//CCLOG ( "XMCocos2D : CCTextureAtlas: not enough memory" );

		CC_SAFE_FREE ( m_pQuads );
		CC_SAFE_FREE ( m_pIndices );

		// release texture, should set it to null, because the destruction will
		// release it too. see cocos2d-x issue #484
		CC_SAFE_RELEASE ( m_pTexture );

		return KD_FALSE;
	}

#if CC_ENABLE_CACHE_TEXTURE_DATA
    // listen the event when app go to background
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver 
	(
		this, callfuncO_selector ( CCTextureAtlas::listenBackToForeground ),
		EVENT_COME_TO_FOREGROUND, KD_NULL 
	);
#endif

	this->setupIndices ( );

#if CC_TEXTURE_ATLAS_USE_VAO
    setupVBOandVAO ( );    
#else    
    setupVBO ( );
#endif

	m_bDirty = KD_TRUE;	

	return KD_TRUE;
}

KDvoid CCTextureAtlas::listenBackToForeground ( CCObject* pObject )
{  
#if CC_TEXTURE_ATLAS_USE_VAO
    setupVBOandVAO ( );    
#else    
    setupVBO ( );
#endif
    
    // set m_bDirty to true to force it rebinding buffer
    m_bDirty = KD_TRUE;
}

const KDchar* CCTextureAtlas::description ( KDvoid )
{
	return CCString::createWithFormat ( "<CCTextureAtlas | totalQuads = %u>", m_uTotalQuads )->getCString ( );
}

KDvoid CCTextureAtlas::setupIndices ( KDvoid )
{
	if ( m_uCapacity == 0 )
	{
		return;
	}

	for ( KDuint i = 0; i < m_uCapacity; i++ )
	{
		KDuint  i6 = i * 6;
		KDuint  i4 = i * 4;

		m_pIndices [ i6 + 0 ] = i4 + 0;
		m_pIndices [ i6 + 1 ] = i4 + 1;
		m_pIndices [ i6 + 2 ] = i4 + 2;

		// inverted index. issue #179
		m_pIndices [ i6 + 3 ] = i4 + 3;
		m_pIndices [ i6 + 4 ] = i4 + 2;
		m_pIndices [ i6 + 5 ] = i4 + 1;		
	}
}

//TextureAtlas - VAO / VBO specific

#if CC_TEXTURE_ATLAS_USE_VAO
void CCTextureAtlas::setupVBOandVAO ( KDvoid )
{
    glGenVertexArrays ( 1, &m_uVAOname );
    ccGLBindVAO ( m_uVAOname );

#define kQuadSize sizeof(m_pQuads[0].bl)

    glGenBuffers ( 2, &m_pBuffersVBO[0] );

    glBindBuffer ( GL_ARRAY_BUFFER, m_pBuffersVBO[0] );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * m_uCapacity, m_pQuads, GL_DYNAMIC_DRAW );

    // vertices
    glEnableVertexAttribArray ( kCCVertexAttrib_Position );
    glVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, vertices ) );

    // colors
    glEnableVertexAttribArray ( kCCVertexAttrib_Color );
    glVertexAttribPointer ( kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, colors ) );

    // tex coords
    glEnableVertexAttribArray ( kCCVertexAttrib_TexCoords );
    glVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( ccV3F_C4B_T2F, texCoords ) );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1] );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( m_pIndices[0] ) * m_uCapacity * 6, m_pIndices, GL_STATIC_DRAW );

    ccGLBindVAO  ( 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    CHECK_GL_ERROR_DEBUG ( );
}
#else // CC_TEXTURE_ATLAS_USE_VAO
KDvoid CCTextureAtlas::setupVBO ( KDvoid )
{
	if ( m_pBuffersVBO[0] )
	{
		glDeleteBuffers ( 2, &m_pBuffersVBO[0] );
		kdMemset ( m_pBuffersVBO, 0, sizeof ( m_pBuffersVBO ) );
	}

    glGenBuffers ( 2, &m_pBuffersVBO[0] );

    mapBuffers ( );
}
#endif // CC_TEXTURE_ATLAS_USE_VAO

KDvoid CCTextureAtlas::mapBuffers ( KDvoid )
{
	ccGLBindVAO  ( 0 );

    glBindBuffer ( GL_ARRAY_BUFFER, m_pBuffersVBO [ 0 ] );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads [ 0 ] ) * m_uCapacity, m_pQuads, GL_DYNAMIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO [ 1 ]);
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( m_pIndices [ 0 ] ) * m_uCapacity * 6, m_pIndices, GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

    CHECK_GL_ERROR_DEBUG ( );
}

/// TextureAtlas - Update, Insert, Move & Remove
KDvoid CCTextureAtlas::updateQuad ( ccV3F_C4B_T2F_Quad* pQuad, KDuint uIndex )
{
	CCAssert ( uIndex >= 0 && uIndex < m_uCapacity, "updateQuadWithTexture: Invalid index" );

	m_uTotalQuads = KD_MAX ( uIndex + 1, m_uTotalQuads );

	m_pQuads [ uIndex ] = *pQuad;	

	m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::insertQuad ( ccV3F_C4B_T2F_Quad* pQuad, KDuint uIndex )
{
	CCAssert ( uIndex < m_uCapacity, "insertQuadWithTexture: Invalid index" );

	m_uTotalQuads++;
	CCAssert ( m_uTotalQuads <= m_uCapacity, "invalid totalQuads" );

	// issue #575. index can be > totalQuads
	KDuint  uRemaining = ( m_uTotalQuads - 1 ) - uIndex;

	// last object doesn't need to be moved
	if ( uRemaining > 0 )
	{
		// texture coordinates
		kdMemmove ( &m_pQuads [ uIndex + 1 ], &m_pQuads [ uIndex ], sizeof ( m_pQuads [ 0 ] ) * uRemaining );		
	}

	m_pQuads [ uIndex ] = *pQuad;

	m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::insertQuads ( ccV3F_C4B_T2F_Quad* pQuads, KDuint uIndex, KDuint uAmount )
{
    CCAssert ( uIndex + uAmount <= m_uCapacity, "insertQuadWithTexture: Invalid index + amount" );

    m_uTotalQuads += uAmount;

    CCAssert ( m_uTotalQuads <= m_uCapacity, "invalid totalQuads");

    // issue #575. index can be > totalQuads
    KDint  nRemaining = ( m_uTotalQuads - 1 ) - uIndex - uAmount;

    // last object doesn't need to be moved
    if ( nRemaining > 0 )
    {
        // tex coordinates
        kdMemmove ( &m_pQuads [ uIndex + uAmount ], &m_pQuads [ uIndex ], sizeof ( m_pQuads [ 0 ] ) * nRemaining );
    }

    KDuint  uMax = uIndex + uAmount;
    KDuint  j = 0;
    for ( KDuint  i = uIndex; i < uMax ; i++ )
    {
        m_pQuads [ uIndex ] = pQuads [ j ];
        uIndex++;
        j++;
    }

    m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::insertQuadFromIndex ( KDuint uOldIndex, KDuint uNewIndex )
{
	CCAssert ( uNewIndex >= 0 && uNewIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index" );
	CCAssert ( uOldIndex >= 0 && uOldIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index" );

	if ( uOldIndex == uNewIndex )
	{
		return;
	}

	// because it is ambiguous in iphone, so we implement abs ourselves
	// unsigned int howMany = abs( oldIndex - newIndex);
	KDuint  uHowMany = ( uOldIndex - uNewIndex ) > 0 ? ( uOldIndex - uNewIndex ) : ( uNewIndex - uOldIndex );
	KDuint  uDst = uOldIndex;
	KDuint  uSrc = uOldIndex + 1;

	if ( uOldIndex > uNewIndex )
	{
		uDst = uNewIndex + 1;
		uSrc = uNewIndex;
	}

	// texture coordinates
	ccV3F_C4B_T2F_Quad  tQuadsBackup = m_pQuads [ uOldIndex ];
	kdMemmove ( &m_pQuads [ uDst ], &m_pQuads [ uSrc ], sizeof ( m_pQuads [ 0 ] ) * uHowMany );
	m_pQuads [ uNewIndex ] = tQuadsBackup;

	m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::removeQuadAtIndex ( KDuint uIndex )
{
	CCAssert ( uIndex < m_uTotalQuads, "removeQuadAtIndex: Invalid index" );

	KDuint  uRemaining = ( m_uTotalQuads - 1 ) - uIndex;

	// last object doesn't need to be moved
	if ( uRemaining )
	{
		// texture coordinates
		kdMemmove ( &m_pQuads [ uIndex ], &m_pQuads [ uIndex + 1 ], sizeof ( m_pQuads [ 0 ] ) * uRemaining );
	}

	m_uTotalQuads--;

	m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::removeQuadsAtIndex ( KDuint uIndex, KDuint uAmount )
{
	CCAssert ( uIndex + uAmount <= m_uTotalQuads, "removeQuadAtIndex: index + amount out of bounds" );

	KDuint  uRemaining = (m_uTotalQuads) - ( uIndex + uAmount );

	m_uTotalQuads -= uAmount;

	if ( uRemaining )
	{
		kdMemmove ( &m_pQuads [ uIndex ], &m_pQuads [ uIndex + uAmount ], sizeof ( m_pQuads [ 0 ] ) * uRemaining );
	}

	m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::removeAllQuads ( KDvoid )
{
	m_uTotalQuads = 0;
}

// TextureAtlas - Resize
KDbool CCTextureAtlas::resizeCapacity ( KDuint uNewCapacity )
{
	if ( uNewCapacity == m_uCapacity )
	{
		return KD_TRUE;
	}

	KDuint  uOldCapacity = m_uCapacity;

	// update capacity and totolQuads
	m_uTotalQuads = KD_MIN ( m_uTotalQuads, uNewCapacity );
	m_uCapacity = uNewCapacity;

	ccV3F_C4B_T2F_Quad*  pTempQuads   = KD_NULL;
	GLushort*            pTempIndices = KD_NULL;
	
	// when calling initWithTexture(fileName, 0) on bada device, calloc(0, 1) will fail and return NULL,
	// so here must judge whether m_pQuads and m_pIndices is NULL.
	if ( m_pQuads == KD_NULL )
	{
		pTempQuads = (ccV3F_C4B_T2F_Quad*) kdCalloc ( 1, m_uCapacity * sizeof ( m_pQuads[0] ) );
	}
	else
	{
		pTempQuads = (ccV3F_C4B_T2F_Quad*) kdRealloc ( m_pQuads, sizeof ( m_pQuads[0] ) * m_uCapacity );
        if ( pTempQuads != KD_NULL && m_uCapacity > uOldCapacity )
        {
            kdMemset ( pTempQuads + uOldCapacity, 0, ( m_uCapacity - uOldCapacity ) * sizeof ( m_pQuads[0] ) );
        }
	}

	if ( m_pIndices == KD_NULL )
	{
		pTempIndices = (GLushort*) kdCalloc ( 1, m_uCapacity * 6 * sizeof ( m_pIndices[0] ) );
	}
	else
	{
		pTempIndices = (GLushort*) kdRealloc ( m_pIndices, sizeof ( m_pIndices[0] ) * m_uCapacity * 6 );
        if ( pTempIndices != KD_NULL && m_uCapacity > uOldCapacity )
        {
            kdMemset ( pTempIndices + uOldCapacity, 0, ( m_uCapacity - uOldCapacity ) * 6 * sizeof ( m_pIndices[0] ) );
        }
	}

	if ( !( pTempQuads && pTempIndices ) )
	{
		CCLOG ( "XMCocos2D : CCTextureAtlas - not enough memory");
		CC_SAFE_FREE ( pTempQuads );
		CC_SAFE_FREE ( pTempIndices );
		CC_SAFE_FREE ( m_pQuads );
		CC_SAFE_FREE ( m_pIndices );
		m_uCapacity = m_uTotalQuads = 0;
		return KD_FALSE;
	}

	m_pQuads   = pTempQuads;
	m_pIndices = pTempIndices;

	setupIndices ( );
    mapBuffers   ( );

	m_bDirty = KD_TRUE;

	return KD_TRUE;
}

KDvoid CCTextureAtlas::increaseTotalQuadsWith ( KDuint uAmount )
{
    m_uTotalQuads += uAmount;
}

KDvoid CCTextureAtlas::moveQuadsFromIndex ( KDuint uOldIndex, KDuint uAmount, KDuint uNewIndex )
{
    CCAssert ( uNewIndex + uAmount <= m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index" );
    CCAssert ( uOldIndex < m_uTotalQuads, "insertQuadFromIndex:atIndex: Invalid index" );

    if ( uOldIndex == uNewIndex )
    {
        return;
    }

    //create buffer
    KDsize   nQuadSize = sizeof ( ccV3F_C4B_T2F_Quad );
    ccV3F_C4B_T2F_Quad*  pTempQuads = (ccV3F_C4B_T2F_Quad*) kdMalloc ( nQuadSize * uAmount );
    kdMemcpy ( pTempQuads, &m_pQuads [ uOldIndex ], nQuadSize * uAmount );

    if ( uNewIndex < uOldIndex )
    {
        // move quads from newIndex to newIndex + amount to make room for buffer
        kdMemmove ( &m_pQuads [ uNewIndex ], &m_pQuads [ uNewIndex + uAmount ], ( uOldIndex - uNewIndex ) * nQuadSize );
    }
    else
    {
        // move quads above back
        kdMemmove ( &m_pQuads [ uOldIndex ], &m_pQuads [ uOldIndex + uAmount ], ( uNewIndex - uOldIndex ) * nQuadSize );
    }
    kdMemcpy ( &m_pQuads [ uNewIndex ], pTempQuads, uAmount * nQuadSize );

    kdFree ( pTempQuads );

    m_bDirty = KD_TRUE;
}

KDvoid CCTextureAtlas::moveQuadsFromIndex ( KDuint uIndex, KDuint uNewIndex )
{
    CCAssert ( uNewIndex + ( m_uTotalQuads - uIndex ) <= m_uCapacity, "moveQuadsFromIndex move is out of bounds" );

    kdMemmove ( m_pQuads + uNewIndex, m_pQuads + uIndex, ( m_uTotalQuads - uIndex ) * sizeof ( m_pQuads[0] ) );
}

KDvoid CCTextureAtlas::fillWithEmptyQuadsFromIndex ( KDuint uIndex, KDuint uAmount )
{
    ccV3F_C4B_T2F_Quad  tQuad;
    kdMemset ( &tQuad, 0, sizeof ( tQuad ) );

    KDuint  uTo = uIndex + uAmount;
    for ( KDuint i = uIndex; i < uTo ; i++ )
    {
        m_pQuads [ i ] = tQuad;
    }
}

/// TextureAtlas - Drawing
KDvoid CCTextureAtlas::drawQuads ( KDvoid )
{
	this->drawNumberOfQuads ( m_uTotalQuads, 0 );
}

KDvoid CCTextureAtlas::drawNumberOfQuads( KDuint uNumber )
{
	this->drawNumberOfQuads ( uNumber, 0 );
}

KDvoid CCTextureAtlas::drawNumberOfQuads ( KDuint uNumber, KDuint uStart )
{
	if ( 0 == uNumber )
	{
		return;
	}

    ccGLBindTexture2D ( m_pTexture->getName ( ) );

#if CC_TEXTURE_ATLAS_USE_VAO

    //
    // Using VBO and VAO
    //

    // XXX: update is done in draw... perhaps it should be done in a timer
    if ( m_bDirty ) 
    {
        glBindBuffer    ( GL_ARRAY_BUFFER, m_pBuffersVBO[0] );
		
		// option 1: subdata
        // glBufferSubData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * uStart, sizeof ( m_pQuads[0] ) * uNumber , &m_pQuads [ uStart ] );

		// option 2: data
        // glBufferData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * ( n - uStart ), &m_pQuads [ uStart ], GL_DYNAMIC_DRAW );
		
		// option 3: orphaning + glMapBuffer
		glBufferData    ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * ( n - uStart ), KD_NULL, GL_DYNAMIC_DRAW );
		KDvoid*  pBuff = glMapBuffer ( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
		kdMemcpy ( pBuff, m_pQuads, sizeof ( m_pQuads [ 0 ] ) * ( n - uStart ) );
		glUnmapBuffer ( GL_ARRAY_BUFFER );

        glBindBuffer    ( GL_ARRAY_BUFFER, 0 );

        m_bDirty = KD_FALSE;
    }

    ccGLBindVAO ( m_uVAOname );

#if CC_REBIND_INDICES_BUFFER
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1] );
#endif

    glDrawElements ( GL_TRIANGLES, (GLsizei) uNumber * 6, GL_UNSIGNED_SHORT, (GLvoid*) ( uStart * 6 * sizeof ( m_pIndices[0] ) ) );

#if CC_REBIND_INDICES_BUFFER
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
#endif

//    glBindVertexArray ( 0 );

#else // CC_TEXTURE_ATLAS_USE_VAO

    //
    // Using VBO without VAO
    //

#define kQuadSize sizeof(m_pQuads[0].bl)
    glBindBuffer ( GL_ARRAY_BUFFER, m_pBuffersVBO[0] );

    // XXX: update is done in draw... perhaps it should be done in a timer
    if ( m_bDirty ) 
    {
		glBufferSubData ( GL_ARRAY_BUFFER, sizeof ( m_pQuads[0] ) * uStart, sizeof ( m_pQuads[0] ) * uNumber , &m_pQuads[ uStart ] );
        m_bDirty = KD_FALSE;
    }

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );

    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 3, GL_FLOAT        , GL_FALSE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, vertices  ) );
    ccGLVertexAttribPointer ( kCCVertexAttrib_Color    , 4, GL_UNSIGNED_BYTE, GL_TRUE , kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, colors    ) );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT        , GL_FALSE, kQuadSize, (GLvoid*) offsetof ( ccV3F_C4B_T2F, texCoords ) );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pBuffersVBO[1] );
    glDrawElements ( GL_TRIANGLES, (GLsizei) uNumber * 6, GL_UNSIGNED_SHORT, (GLvoid*) ( uStart * 6 * sizeof ( m_pIndices[0] ) ) );

    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

#endif // CC_TEXTURE_ATLAS_USE_VAO

    CC_INCREMENT_GL_DRAWS ( 1 );
    CHECK_GL_ERROR_DEBUG ( );
}


NS_CC_END