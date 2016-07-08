/* -----------------------------------------------------------------------------------
 *
 *      File            CCDiamoneTiledLayer.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "tilemap_parallax_nodes/CCDiamondTiledLayer.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "support/CCFileUtils.h"
#include "shaders/CCShaderCache.h"
#include "support/ccUtils.h"

NS_CC_BEGIN
    
CCDiamondTiledLayer::CCDiamondTiledLayer ( KDvoid ) 
{
    m_tGridSize     = ccg ( 0, 0 );
    m_tFrameSize    = ccg ( 0, 0 );
    m_uFrameCount   = 0;
    m_pIndices      = KD_NULL;
    m_pTexture      = KD_NULL;
    m_pFrameIndices = KD_NULL;
    m_pVertices     = KD_NULL;
    m_pPixels       = KD_NULL;
	m_aGIDs [ 0 ]   = 0;
	m_aGIDs [ 1 ]   = 0;
}

CCDiamondTiledLayer::~CCDiamondTiledLayer ( KDvoid )
{
	purge ( );
}

KDvoid CCDiamondTiledLayer::purge ( KDvoid )
{
	if ( m_aGIDs [ 0 ] )
	{
		glDeleteBuffers ( 2, m_aGIDs );
	}

	CC_SAFE_RELEASE ( m_pTexture );
	CC_SAFE_FREE ( m_pIndices );
	CC_SAFE_FREE ( m_pVertices );
	CC_SAFE_FREE ( m_pFrameIndices );
	CC_SAFE_FREE ( m_pPixels );
}

CCDiamondTiledLayer* CCDiamondTiledLayer::create ( const ccGridSize& tGridSize, const ccGridSize& tFrameSize, KDuint uFrameCount )
{
	CCDiamondTiledLayer*  pRet = new CCDiamondTiledLayer ( );

	if ( pRet && pRet->initWithInfo ( tGridSize, tFrameSize, uFrameCount ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCDiamondTiledLayer::initWithInfo ( const ccGridSize& tGridSize, const ccGridSize& tFrameSize, KDuint uFrameCount )
{
	if ( tGridSize.x < 0 || tFrameSize.x < 0 ||
		 tGridSize.y < 0 || tFrameSize.y < 0 )
	{
		return KD_FALSE;
	}

	purge ( );

	m_pIndices = (GLushort *) kdMalloc ( sizeof ( GLushort ) * tGridSize.x * tGridSize.y * 6 );
	m_pVertices = (ccV2F_C4B_T2F *) kdMalloc ( sizeof ( ccV2F_C4B_T2F ) * tGridSize.x * tGridSize.y * 4 );
	m_pFrameIndices = (KDuint *) kdCalloc ( tGridSize.x * tGridSize.y, sizeof ( KDuint ) );
	
	CCAssert ( m_pIndices, "Memory allocation failed." );
	CCAssert ( m_pVertices, "Memory allocation failed." );
	CCAssert ( m_pFrameIndices, "Memory allocation failed." );

	CCPoint  tVertex;       // for Vertex
	CCPoint  tTexCoord;     // for Texture coordinate
	CCPoint  tFrameHalf;    // for Frame half size

	KDuint   uWide = ccNextPOT ( tFrameSize.x * uFrameCount );
	KDuint   uHigh = ccNextPOT ( tFrameSize.y );

	KDuint   uOffset = 0;
	KDuint   uCount  = 0;

	tTexCoord  = ccp ( (KDfloat) tFrameSize.x / uWide, (KDfloat) tFrameSize.y / uHigh );
	tFrameHalf = ccp ( tFrameSize.x / 2.f, tFrameSize.y / 2.f );

	this->setContentSize ( tGridSize.x > tGridSize.y ? CCSizeMake ( tGridSize.x * tFrameSize.x, tGridSize.x * tFrameSize.y ) 
												     : CCSizeMake ( tGridSize.y * tFrameSize.x, tGridSize.y * tFrameSize.y ) );

    const CCSize&  tSize = this->getContentSize ( );

	for ( KDint nRow = 0; nRow < tGridSize.y; nRow++ )
	{
		for ( KDint nCol = 0; nCol < tGridSize.x; nCol++ )
		{			
			uOffset = ( nRow * tGridSize.x + nCol ) * 4;

			tVertex = ccp ( tSize.cx / 2.f + nCol * tFrameHalf.x - nRow * tFrameHalf.x, tSize.cy - ( nRow + nCol ) * tFrameHalf.y - tFrameHalf.y );
			
			m_pVertices [ uOffset + 0 ].vertices = vertex2 ( tVertex.x - tFrameHalf.x, tVertex.y + tFrameHalf.y ); 		
			m_pVertices [ uOffset + 1 ].vertices = vertex2 ( tVertex.x - tFrameHalf.x, tVertex.y - tFrameHalf.y ); 
			m_pVertices [ uOffset + 2 ].vertices = vertex2 ( tVertex.x + tFrameHalf.x, tVertex.y - tFrameHalf.y );
			m_pVertices [ uOffset + 3 ].vertices = vertex2 ( tVertex.x + tFrameHalf.x, tVertex.y + tFrameHalf.y ); 
			
			m_pVertices [ uOffset + 0 ].texCoords = tex2 (           0, tTexCoord.y );
			m_pVertices [ uOffset + 1 ].texCoords = tex2 (           0,           0 ); 
			m_pVertices [ uOffset + 2 ].texCoords = tex2 ( tTexCoord.x,           0 );
			m_pVertices [ uOffset + 3 ].texCoords = tex2 ( tTexCoord.x, tTexCoord.y );	
			
			m_pVertices [ uOffset + 0 ].colors = ccc4 ( ccWHITE );
			m_pVertices [ uOffset + 1 ].colors = ccc4 ( ccWHITE );
			m_pVertices [ uOffset + 2 ].colors = ccc4 ( ccWHITE );
			m_pVertices [ uOffset + 3 ].colors = ccc4 ( ccWHITE );

			m_pIndices [ uCount++ ] = uOffset + 0;
			m_pIndices [ uCount++ ] = uOffset + 1;
			m_pIndices [ uCount++ ] = uOffset + 2;
			m_pIndices [ uCount++ ] = uOffset + 0;
			m_pIndices [ uCount++ ] = uOffset + 2;
			m_pIndices [ uCount++ ] = uOffset + 3;
		}
	}

#if CC_USES_VBO
	
	glGenBuffers ( 2, m_aGIDs );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_aGIDs [ 0 ] );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( GLushort ) * tGridSize.x * tGridSize.y * 6, m_pIndices, GL_STATIC_DRAW ); 
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glBindBuffer ( GL_ARRAY_BUFFER, m_aGIDs [ 1 ] );
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( ccV2F_C4B_T2F ) * tGridSize.x * tGridSize.y * 4, m_pVertices, GL_STATIC_DRAW ); 
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	
#endif

	m_pTexture = new CCTexture2D ( );
	CCAssert ( m_pTexture, "Memory allocation failed." );
		
	m_pTexture->retain ( );
	m_pTexture->initWithData ( KD_NULL, kCCTexture2DPixelFormat_RGBA8888, CCSizeMake ( tFrameSize.x * uFrameCount, tFrameSize.y ) );			
	
#if CC_ENABLE_CACHE_TEXTURE_DATA

	m_pPixels = (KDubyte *) kdCalloc ( tFrameSize.x * uFrameCount * tFrameSize.y, 4 );
	CCAssert ( m_pPixels, "Memory allocation failed." );

#endif

	m_tGridSize = tGridSize;
	m_tFrameSize = tFrameSize;
	m_uFrameCount = uFrameCount;

	// shader program
    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

	return KD_TRUE;
}

KDvoid CCDiamondTiledLayer::draw ( KDvoid )
{
	CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  );
	ccGLBindTexture2D ( m_pTexture->getName ( ) );
	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );

#if CC_USES_VBO
    
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_aGIDs [ 0 ] );
	glBindBuffer ( GL_ARRAY_BUFFER, m_aGIDs [ 1 ] );
    
	GLintptr  pVBOs = 0; 
    GLintptr  pIndices = 0;
#else
    
	GLintptr  pVBOs = (GLintptr) m_pVertices;
    GLintptr  pIndices = (GLintptr) m_pIndices;
    
#endif

    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT        , GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid *) ( pVBOs + offsetof ( ccV2F_C4B_T2F, vertices  ) ) );	
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT        , GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid *) ( pVBOs + offsetof ( ccV2F_C4B_T2F, texCoords ) ) );	
    ccGLVertexAttribPointer ( kCCVertexAttrib_Color    , 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof ( ccV2F_C4B_T2F ), (GLvoid *) ( pVBOs + offsetof ( ccV2F_C4B_T2F, colors    ) ) );

	glDrawElements ( GL_TRIANGLES, 6 * m_tGridSize.y * m_tGridSize.x, GL_UNSIGNED_SHORT, (GLvoid *) ( pIndices ) );

#if CC_USES_VBO
    
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	
#endif
    
    ccGLBindTexture2D ( 0 );

	CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid CCDiamondTiledLayer::loadFrame ( KDuint uFrameIndex, const KDchar* szFilePath )
{
	KDImageATX	pImage  = 0;
	KDubyte*    pPixels = 0;
	
	CCAssert ( uFrameIndex < m_uFrameCount, "Frame index must be less than Frame number." );

	pImage = kdGetImageATX ( CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath ).c_str ( ), KD_IMAGE_FORMAT_RGBA8888_ATX, KD_IMAGE_FLAG_FLIP_Y_ATX );		
	CCAssert ( pImage, "Image file loading failed." );

	CCAssert ( kdGetImageIntATX ( pImage, KD_IMAGE_WIDTH_ATX ) == m_tFrameSize.x && kdGetImageIntATX ( pImage, KD_IMAGE_HEIGHT_ATX ) == m_tFrameSize.y, "Image size must be same as Frame size." );

	pPixels = (KDubyte*) kdGetImagePointerATX ( pImage, KD_IMAGE_POINTER_BUFFER_ATX );
	CCAssert ( pPixels, "Image pixels are invaild." );

#if CC_ENABLE_CACHE_TEXTURE_DATA

	for ( KDint nRow = 0; nRow < m_tFrameSize.y; nRow++ )
	{		
		kdMemcpy ( &m_pPixels [ ( nRow * m_tFrameSize.x * m_uFrameCount + m_tFrameSize.x * uFrameIndex ) * 4 ], &pPixels [ nRow * m_tFrameSize.x * 4 ], m_tFrameSize.x * 4 );
		VolatileTexture::addDataTexture ( m_pTexture, m_pPixels, kCCTexture2DPixelFormat_RGBA8888, CCSizeMake ( m_tFrameSize.x * m_uFrameCount, m_tFrameSize.y ) );
	}	

#endif

	ccGLBindTexture2D ( m_pTexture->getName ( ) );
	glTexSubImage2D ( GL_TEXTURE_2D, 0, m_tFrameSize.x * uFrameIndex, 0, m_tFrameSize.x, m_tFrameSize.y, GL_RGBA, GL_UNSIGNED_BYTE, pPixels );	 							
	ccGLBindTexture2D ( 0 );

	kdFreeImageATX ( pImage );
}

KDvoid CCDiamondTiledLayer::setFrame ( const ccGridSize& tGID, KDuint uFrameIndex )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y ||
		 uFrameIndex >= m_uFrameCount )
	{
		return;
	}

	m_pFrameIndices [ tGID.y * m_tGridSize.x + tGID.x ] = uFrameIndex;	
	
	KDuint  uWide = ccNextPOT ( m_tFrameSize.x * m_uFrameCount );	
	KDuint  uOffset = ( tGID.y * m_tGridSize.x + tGID.x ) * 4;

	m_pVertices [ uOffset + 1 ].texCoords.u = m_pVertices [ uOffset + 0 ].texCoords.u = (KDfloat) m_tFrameSize.x / uWide * uFrameIndex;
	m_pVertices [ uOffset + 3 ].texCoords.u = m_pVertices [ uOffset + 2 ].texCoords.u = m_pVertices [ uOffset + 0 ].texCoords.u + (KDfloat) m_tFrameSize.x / uWide;

#if CC_USES_VBO

	glBindBuffer ( GL_ARRAY_BUFFER, m_aGIDs [ 1 ] );

	for ( KDuint it = 0; it < 4; it++ )
	{		
		glBufferSubData ( GL_ARRAY_BUFFER, 
                          sizeof ( ccV2F_C4B_T2F ) * ( uOffset + it ) + offsetof ( ccV2F_C4B_T2F, texCoords.u ),
                          sizeof ( KDfloat ), 
                          &m_pVertices [ uOffset + it ].texCoords.u ); 
	}

	glBindBuffer ( GL_ARRAY_BUFFER, 0 );

#endif
}

KDuint CCDiamondTiledLayer::getFrame ( const ccGridSize& tGID )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y )
	{
		return KDUINT_MAX;
	}

	return m_pFrameIndices [ tGID.y * m_tGridSize.x + tGID.x ];
}

ccColor3B CCDiamondTiledLayer::getColor ( const ccGridSize& tGID )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y )
	{
		return ccBLACK;
	}
	
	KDuint     uOffset = ( tGID.y * m_tGridSize.x + tGID.x ) * 4;
	ccColor3B  tColor = { m_pVertices [ uOffset ].colors.r, m_pVertices [ uOffset ].colors.g, m_pVertices [ uOffset ].colors.b };

	return  tColor;
}

KDvoid CCDiamondTiledLayer::setColor ( const ccGridSize& tGID, const ccColor3B& tColor )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y )
	{
		return;
	}

	KDuint  uOffset = ( tGID.y * m_tGridSize.x + tGID.x ) * 4;
	for ( KDuint it = 0; it < 4; it++ )
	{
		m_pVertices [ uOffset + it ].colors.r = tColor.r;
		m_pVertices [ uOffset + it ].colors.g = tColor.g;
		m_pVertices [ uOffset + it ].colors.b = tColor.b;

#if CC_USES_VBO		

		glBindBuffer    ( GL_ARRAY_BUFFER, m_aGIDs [ 1 ] );				

		glBufferSubData ( GL_ARRAY_BUFFER,
                          sizeof ( ccV2F_C4B_T2F ) * ( uOffset + it ) + offsetof ( ccV2F_C4B_T2F, colors ), 
                          sizeof ( ccColor4B ), 
                          &m_pVertices [ uOffset + it ].colors ); 		

		glBindBuffer    ( GL_ARRAY_BUFFER, 0 );

#endif

	}
}

KDvoid CCDiamondTiledLayer::setOpacity ( const ccGridSize& tGID, KDubyte cOpacity )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y )
	{
		return;
	}

	KDuint  uOffset = ( tGID.y * m_tGridSize.x + tGID.x ) * 4;	
	for ( KDuint it = 0; it < 4; it++ )
	{
		m_pVertices [ uOffset + it ].colors.a = cOpacity;

#if CC_USES_VBO		

		glBindBuffer    ( GL_ARRAY_BUFFER, m_aGIDs [ 1 ] );				

		glBufferSubData ( GL_ARRAY_BUFFER, 
                          sizeof ( ccV2F_C4B_T2F ) * ( uOffset + it ) + offsetof ( ccV2F_C4B_T2F, colors ),
                          sizeof ( ccColor4B ), 
                          &m_pVertices [ uOffset + it ].colors ); 		

		glBindBuffer    ( GL_ARRAY_BUFFER, 0 );

#endif

	}
}

KDubyte CCDiamondTiledLayer::getOpacity ( const ccGridSize& tGID )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y )
	{
		return 0;
	}
	
	return m_pVertices [ ( tGID.y * m_tGridSize.x + tGID.x ) * 4 ].colors.a;
}

CCPoint CCDiamondTiledLayer::getPosition ( const ccGridSize& tGID )
{
	if ( tGID.x < 0 || tGID.x >= m_tGridSize.x ||
		 tGID.y < 0 || tGID.y >= m_tGridSize.y )
	{
		return ccp ( -1, -1 );
	}

	const CCSize&  tSize = this->getContentSize ( );
	CCPoint  tHalf = ccp ( m_tFrameSize.x / 2.f, m_tFrameSize.y / 2.f );

	return ccp ( tSize.cx / 2.f + tGID.x * tHalf.x - tGID.y * tHalf.x, tSize.cy - ( tGID.y + tGID.x ) * tHalf.y - tHalf.y );
}

ccGridSize CCDiamondTiledLayer::getIndex ( const CCPoint& tPosition )
{
	const CCSize&  tSize = this->getContentSize ( );
	      CCSize   tHalf = CCSize ( tSize.cx / 2.f, tSize.cy / 2.f );

	CCPoint  tVertex [ 4 ];

	tVertex [ 0 ] = ccp (        0, tHalf.cy );
	tVertex [ 1 ] = ccp ( tHalf.cx, tSize.cy );
	tVertex [ 2 ] = ccp ( tSize.cx, tHalf.cy );
	tVertex [ 3 ] = ccp ( tHalf.cx,        0 );

	if ( tPosition.containsPolygon ( tVertex, 4 ) )
	{	
		CCPoint  tFrameHalf = ccp ( m_tFrameSize.x / 2.f, m_tFrameSize.y / 2.f );
		CCPoint  tCenter;

		for ( KDint nRow = 0; nRow < m_tGridSize.y; nRow++ )
		{
			for ( KDint nCol = 0; nCol < m_tGridSize.x; nCol++ )
			{
				tCenter = ccp ( tHalf.cx + nCol * tFrameHalf.x - nRow * tFrameHalf.x, tSize.cy - ( nRow + nCol ) * tFrameHalf.y - tFrameHalf.y );

				tVertex [ 0 ] = ccp ( tCenter.x - tFrameHalf.x, tCenter.y                );
				tVertex [ 1 ] = ccp ( tCenter.x               , tCenter.y + tFrameHalf.y );
				tVertex [ 2 ] = ccp ( tCenter.x + tFrameHalf.x, tCenter.y                );
				tVertex [ 3 ] = ccp ( tCenter.x               , tCenter.y - tFrameHalf.y );

				if ( tPosition.containsPolygon ( tVertex, 4 ) )
				{
					return ccg ( nCol, nRow );
				}
			}
		}
	}

	return ccg ( -1, -1 );
}


NS_CC_END