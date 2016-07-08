/* -----------------------------------------------------------------------------------
 *
 *      File            CCProgress.cpp
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
#include "misc_nodes/CCProgress.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "support/CCFileUtils.h"
#include "support/CCImage.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN

static CCDictionary  l_tImageCache;

class CCImageEx : public CCImage
{
	public :

		CCSize			m_pSizes [ 9 ];
};

CCProgress::CCProgress ( KDvoid )
{
	m_eProgressType = kCCProgressTypeToRight;
	m_fPercentage = 100.f;	

	m_bDirty = KD_TRUE;	

	m_pGIDs [ 0 ] = 0;
	m_pGIDs [ 1 ] = 0;

	m_pIndices  = KD_NULL;
	m_pVBOs     = KD_NULL;
	m_pTexture  = KD_NULL;	

	setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	ignoreAnchorPointForPosition ( KD_FALSE );
}

CCProgress::~CCProgress ( KDvoid )
{
	purge ( );	
}

KDvoid CCProgress::purge ( KDvoid )
{
	if ( m_pGIDs [ 0 ] )
	{
		glDeleteBuffers ( 2, m_pGIDs );
	}
	
	CC_SAFE_FREE ( m_pIndices );
	CC_SAFE_FREE ( m_pVBOs );
    
    if ( m_sFilePath.size ( ) )
    {
        l_tImageCache.removeObjectForKey ( m_sFilePath );
        m_sFilePath.clear ( );
    }

	CC_SAFE_RELEASE ( m_pTexture );
}

CCProgress* CCProgress::create ( const KDchar* szFilePath, const CCSize& tSize )
{
	CCProgress*  ret = new CCProgress ( );

	if ( ret && ret->initWithFile ( szFilePath, tSize ) )
	{
		ret->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( ret );
	}

	return ret;
}

KDbool CCProgress::initWithFile ( const KDchar* szFilePath, const CCSize& tSize )
{
    purge ( );
    
	std::string  sFullPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFilePath );
    CCImageEx*   pImage  = (CCImageEx*) l_tImageCache.objectForKey ( sFullPath );
    KDint        off;
	KDint        icx;
	KDint        icy;

    if ( !pImage )
    {
        pImage = new CCImageEx ( );
        if ( pImage && pImage->initWithFile ( sFullPath.c_str ( ), kCCTexture2DPixelFormat_RGBA8888 ) )
        {
            pImage->autorelease ( );
            l_tImageCache.setObject ( pImage, sFullPath );

			KDubyte*  pixel = pImage->getData ( );

			icx = pImage->getWidth  ( );
			icy = pImage->getHeight ( );
		        
			CCAssert ( icx > 2 && icy > 2, "Invalid image. dimension must be more than 2px." );
		    
			KDfloat  val[3];
		    
			KDint  scan;
			KDint  scan_max;
		    
			KDint  start;
			KDint  end;
			
		    
			for ( KDuint dir = 0; dir < 2; dir++ )
			{
				scan_max = ( dir == 0 ? icx : icy ) - 1;
				for ( scan = 1, start = 0, end = 0; scan < scan_max; scan++ )
				{
					off = ( dir == 0 ? scan : scan * icx ) * 4;
					if ( pixel [ off + 3 ] > 0 )
					{
						if ( start == 0 )
						{
							start = scan;
						}
						
						end = scan;
					}
					else
					{
						if ( start != 0 )
						{
							break;
						}
					}
				}
		        
				if ( start == 0 )
				{
					val [ 2 ] = val [ 0 ] = 0;
					val [ 1 ] = scan_max - 1.f;
				}
				else
				{
					val [ 0 ] = start - 1.f;
					val [ 1 ] = end - start + 1.f;
					val [ 2 ] = scan_max - 1.f - val [ 0 ] - val [ 1 ];
				}
		        
				switch ( dir )
				{
					case 0 :
						pImage->m_pSizes [ 6 ].cx = pImage->m_pSizes [ 3 ].cx = pImage->m_pSizes [ 0 ].cx = val [ 0 ];
						pImage->m_pSizes [ 7 ].cx = pImage->m_pSizes [ 4 ].cx = pImage->m_pSizes [ 1 ].cx = val [ 1 ];
						pImage->m_pSizes [ 8 ].cx = pImage->m_pSizes [ 5 ].cx = pImage->m_pSizes [ 2 ].cx = val [ 2 ];
						break;
		                
					case 1 :
						pImage->m_pSizes [ 2 ].cy = pImage->m_pSizes [ 1 ].cy = pImage->m_pSizes [ 0 ].cy = val [ 0 ];
						pImage->m_pSizes [ 5 ].cy = pImage->m_pSizes [ 4 ].cy = pImage->m_pSizes [ 3 ].cy = val [ 1 ];
						pImage->m_pSizes [ 8 ].cy = pImage->m_pSizes [ 7 ].cy = pImage->m_pSizes [ 6 ].cy = val [ 2 ];
						break;
				}
			}

			for ( KDint y = 0; y < icy; y++ )
			{
				if ( y == 0 || y == icy - 1 )
				{
					for ( KDint x = 0; x < icx; x++ )
					{
						off = ( y * icx + x ) * 4;
						
						pixel [ off + 0 ] = 0;
						pixel [ off + 1 ] = 0;
						pixel [ off + 2 ] = 0;
						pixel [ off + 3 ] = 0;
					}
				}
				else
				{
					off = ( y * icx + 0 ) * 4;
					
					pixel [ off + 0 ] = 0;
					pixel [ off + 1 ] = 0;
					pixel [ off + 2 ] = 0;	
					pixel [ off + 3 ] = 0;	

					off = ( y * icx + icx - 1 ) * 4;
					
					pixel [ off + 0 ] = 0;
					pixel [ off + 1 ] = 0;
					pixel [ off + 2 ] = 0;	
					pixel [ off + 3 ] = 0;
				}
			}
        }
        else
        {
            CC_SAFE_DELETE ( pImage );
            return KD_FALSE;
        }
    }
	
	icx = pImage->getWidth  ( );
	icy = pImage->getHeight ( );
	kdMemcpy ( m_pSizes, pImage->m_pSizes, sizeof ( m_pSizes ) );

	m_pIndices = (GLushort*) kdMalloc ( sizeof ( GLushort ) * 9 * 6 );
	m_pVBOs = (ccV2F_C4B_T2F*) kdMalloc ( sizeof ( ccV2F_C4B_T2F ) * 9 * 4 );

	for ( KDuint it = 0, cnt = 0; it < 9; it++ )
	{		
		off = it * 4;

		m_pVBOs [ off + 0 ].colors = ccc4 ( ccWHITE );
		m_pVBOs [ off + 1 ].colors = ccc4 ( ccWHITE );
		m_pVBOs [ off + 2 ].colors = ccc4 ( ccWHITE );
		m_pVBOs [ off + 3 ].colors = ccc4 ( ccWHITE );

		m_pIndices [ cnt++ ] = off + 0;
		m_pIndices [ cnt++ ] = off + 1;
		m_pIndices [ cnt++ ] = off + 2;
		m_pIndices [ cnt++ ] = off + 0;
		m_pIndices [ cnt++ ] = off + 2;
		m_pIndices [ cnt++ ] = off + 3;				
	}

#if CC_USES_VBO

	glGenBuffers ( 2, m_pGIDs );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pGIDs [ 0 ] );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( GLushort ) * 9 * 6, m_pIndices, GL_STATIC_DRAW ); 
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

#endif

	m_pTexture = CCTextureCache::sharedTextureCache ( )->addCCImage ( pImage, sFullPath.c_str ( ) );
	if ( !m_pTexture )
	{
		return KD_FALSE;
	}

	m_pTexture->retain ( );
	this->setContentSize ( ccp ( tSize.cx == 0 ? icx - 2.f : tSize.cx, tSize.cy == 0 ? icy - 2.f : tSize.cy ) );
	this->setPercentage ( 100.f );

	// shader program
    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

    m_sFilePath = sFullPath;
 
	return KD_TRUE;
}

KDvoid CCProgress::draw ( KDvoid )
{
	CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( CC_BLEND_SRC, CC_BLEND_DST );
	ccGLBindTexture2D ( m_pTexture->getName ( ) );
	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );

#if CC_USES_VBO

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_pGIDs [ 0 ] );
	glBindBuffer ( GL_ARRAY_BUFFER, m_pGIDs [ 1 ] );
    
    GLintptr  pIndices = 0; 
	GLintptr  pVBOs = 0; 

#else

    GLintptr  pIndices = (GLintptr) m_pIndices;
	GLintptr  pVBOs = (GLintptr) m_pVBOs;

#endif		

	if ( m_bDirty )
	{
		updateVBO ( );		
	}

    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT        , GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid *) ( pVBOs + offsetof ( ccV2F_C4B_T2F, vertices  ) ) );	
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT        , GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid *) ( pVBOs + offsetof ( ccV2F_C4B_T2F, texCoords ) ) );	
    ccGLVertexAttribPointer ( kCCVertexAttrib_Color    , 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof ( ccV2F_C4B_T2F ), (GLvoid *) ( pVBOs + offsetof ( ccV2F_C4B_T2F, colors    ) ) );

    glDrawElements ( GL_TRIANGLES, 6 * 9, GL_UNSIGNED_SHORT, (GLvoid *) ( pIndices ) );

#if CC_USES_VBO
    
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );

#endif			
    
    ccGLBindTexture2D ( 0 );

	CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid CCProgress::setType ( CCProgressType eProgressType )
{
	m_eProgressType = eProgressType;
	m_bDirty = KD_TRUE;
}

CCProgressType CCProgress::getType ( KDvoid )
{
	return m_eProgressType;
}

KDvoid CCProgress::setContentSize ( const CCSize& tSize )
{
	CCSize  tImgSize = m_pTexture->getContentSize ( );
	CCSize  tContentSize = CCSize ( KD_MAX ( tImgSize.cx - m_pSizes [ 4 ].cx - 2, tSize.cx ), KD_MAX ( tImgSize.cy - m_pSizes [ 4 ].cy - 2, tSize.cy ) );

	CCNode::setContentSize ( tContentSize );

	m_bDirty = KD_TRUE;
}

KDvoid CCProgress::setPercentage ( KDfloat fPercentage )
{
	m_fPercentage = fPercentage < 0 ? 0 : fPercentage > 100 ? 100 : fPercentage;	
	m_bDirty = KD_TRUE;
}

KDfloat CCProgress::getPercentage ( KDvoid )
{
	return m_fPercentage;
}

KDvoid CCProgress::updateVBO ( KDvoid )
{
	const CCSize&  tSize = getContentSize ( );	
    const CCSize&  tTexSize = m_pTexture->getContentSize ( );
	      CCSize   tDstSize = CCSize ( m_eProgressType & 0x0f ? tSize.cx * m_fPercentage / 100.f : tSize.cx, m_eProgressType & 0xf0 ? tSize.cy * m_fPercentage / 100.f : tSize.cy );

	ccColor4B  tColor = { m_tDisplayedColor.r, m_tDisplayedColor.g, m_tDisplayedColor.b, m_cDisplayedOpacity };
	KDfloat    fWide  = (KDfloat) m_pTexture->getPixelsWide ( );
	KDfloat    fHigh  = (KDfloat) m_pTexture->getPixelsHigh ( );
	KDuint     uOffset;	
	
	if ( isOpacityModifyRGB ( ) )
    {
		tColor.r = (GLubyte) ( tColor.r * m_cDisplayedOpacity / 255.0f );
		tColor.g = (GLubyte) ( tColor.g * m_cDisplayedOpacity / 255.0f );
		tColor.b = (GLubyte) ( tColor.b * m_cDisplayedOpacity / 255.0f );
    }

	for ( KDuint uRow = 0; uRow < 3; uRow++ )
	{			
		KDuint   pIndices [ 4 ];
		KDfloat  fVertex;
		KDfloat  fCoord;

		if ( m_eProgressType & kCCProgressTypeToLeft )
		{
			pIndices [ 0 ] = 2; pIndices [ 1 ] = 3; pIndices [ 2 ] = 0; pIndices [ 3 ] = 1;

			fVertex = tSize.cx;
			fCoord  = ( tTexSize.cx - 1.f ) / fWide;
		}
		else
		{
			pIndices [ 0 ] = 0; pIndices [ 1 ] = 1; pIndices [ 2 ] = 2; pIndices [ 3 ] = 3;

			fVertex = 0;
			fCoord  = 1.f / fWide;
		}
		
		for ( KDuint uCol = 0; uCol < 3; uCol++ )
		{
			KDuint  uCol2 = m_eProgressType & kCCProgressTypeToLeft ? 2 - uCol : uCol;

			uOffset = ( uRow * 3 + uCol2 ) * 4;	

			m_pVBOs [ uOffset + pIndices [ 0 ] ].vertices.x  = m_pVBOs [ uOffset + pIndices [ 1 ] ].vertices.x  = fVertex;	
			m_pVBOs [ uOffset + pIndices [ 0 ] ].texCoords.u = m_pVBOs [ uOffset + pIndices [ 1 ] ].texCoords.u = fCoord;	
			m_pVBOs [ uOffset + pIndices [ 0 ] ].colors      = m_pVBOs [ uOffset + pIndices [ 1 ] ].colors      = tColor;	

			KDfloat  fVertex2 = 0;
			KDfloat  fCoord2  = 0;

			switch ( uCol )
			{
				case 0 :

					if ( tDstSize.cx < m_pSizes [ uRow * 3 + uCol2 ].cx )
					{	
						fVertex2 = tDstSize.cx;
						fCoord2  = tDstSize.cx / fWide;						
					}
					else
					{
						fVertex2 = m_pSizes [ uRow * 3 + uCol2 ].cx;
						fCoord2  = m_pSizes [ uRow * 3 + uCol2 ].cx / fWide;
					}

					break;

				case 1 :

					if ( tDstSize.cx > m_pSizes [ uRow * 3 + ( m_eProgressType & kCCProgressTypeToLeft ? 2 : 0 ) ].cx )
					{
						if ( tDstSize.cx < ( tSize.cx - m_pSizes [ uRow * 3 + ( m_eProgressType & kCCProgressTypeToLeft ? 0 : 2 ) ].cx ) )
						{
							fVertex2 = tDstSize.cx - m_pSizes [ uRow * 3 + ( m_eProgressType & kCCProgressTypeToLeft ? 2 : 0 ) ].cx;
						}
						else
						{
							fVertex2 = tSize.cx - m_pSizes [ uRow * 3 + 0 ].cx - m_pSizes [ uRow * 3 + 2 ].cx;
						}

						fCoord2 = m_pSizes [ uRow * 3 + uCol2 ].cx / fWide;
					}

					break;

				case 2 :

					if ( tDstSize.cx > ( tSize.cx - m_pSizes [ uRow * 3 + uCol2 ].cx ) )
					{						
						fVertex2 = m_pSizes [ uRow * 3 + uCol2 ].cx - ( tSize.cx - tDstSize.cx );
						fCoord2  = ( m_pSizes [ uRow * 3 + uCol2 ].cx - ( tSize.cx - tDstSize.cx ) ) / fWide;			
					}

					break;
			}

			if ( m_eProgressType & kCCProgressTypeToLeft )
			{
				fVertex -= fVertex2;
				fCoord  -= fCoord2;
			}
			else
			{
				fVertex += fVertex2;
				fCoord  += fCoord2;
			}

			m_pVBOs [ uOffset + pIndices [ 2 ] ].vertices.x  = m_pVBOs [ uOffset + pIndices [ 3 ] ].vertices.x  = fVertex;
			m_pVBOs [ uOffset + pIndices [ 2 ] ].texCoords.u = m_pVBOs [ uOffset + pIndices [ 3 ] ].texCoords.u = fCoord;
			m_pVBOs [ uOffset + pIndices [ 2 ] ].colors      = m_pVBOs [ uOffset + pIndices [ 3 ] ].colors      = tColor;
		}
	}		

	for ( KDuint uCol = 0; uCol < 3; uCol++ )
	{			
		KDuint   pIndices [ 4 ];
		KDfloat  fVertex;
		KDfloat  fCoord;

		if ( m_eProgressType & kCCProgressTypeToBottom )
		{
			pIndices [ 0 ] = 2; pIndices [ 1 ] = 3; pIndices [ 2 ] = 0; pIndices [ 3 ] = 1;

			fVertex = tSize.cy;
			fCoord  = 1.f / fHigh;			
		}
		else
		{
			pIndices [ 0 ] = 0; pIndices [ 1 ] = 1; pIndices [ 2 ] = 2; pIndices [ 3 ] = 3;

			fVertex = 0;
			fCoord  = ( tTexSize.cy - 1.f ) / fHigh;			
		}
		
		for ( KDuint uRow = 0; uRow < 3; uRow++ )
		{
			KDuint  uRow2 = m_eProgressType & kCCProgressTypeToBottom ? uRow : 2 - uRow;

			uOffset = ( uRow2 * 3 + uCol ) * 4;	

			m_pVBOs [ uOffset + pIndices [ 1 ] ].vertices.y  = m_pVBOs [ uOffset + pIndices [ 2 ] ].vertices.y  = fVertex;	
			m_pVBOs [ uOffset + pIndices [ 1 ] ].texCoords.v = m_pVBOs [ uOffset + pIndices [ 2 ] ].texCoords.v = fCoord;	
			m_pVBOs [ uOffset + pIndices [ 1 ] ].colors      = m_pVBOs [ uOffset + pIndices [ 2 ] ].colors      = tColor;

			KDfloat  fVertex2 = 0;
			KDfloat  fCoord2  = 0;

			switch ( uRow )
			{
				case 0 :

					if ( tDstSize.cy < m_pSizes [ uRow2 * 3 + uCol ].cy )
					{	
						fVertex2 = tDstSize.cy;
						fCoord2  = tDstSize.cy / fHigh;						
					}
					else
					{
						fVertex2 = m_pSizes [ uRow2 * 3 + uCol ].cy;
						fCoord2  = m_pSizes [ uRow2 * 3 + uCol ].cy / fHigh;
					}

					break;

				case 1 :

					if ( tDstSize.cy > m_pSizes [ ( m_eProgressType & kCCProgressTypeToBottom ? 0 : 2 ) * 3 + uCol ].cy )
					{						
						if ( tDstSize.cy < ( tSize.cy - m_pSizes [ ( m_eProgressType & kCCProgressTypeToBottom ? 0 : 2 ) * 3 + uCol ].cy ) )
						{
							fVertex2 = tDstSize.cy - m_pSizes [ ( m_eProgressType & kCCProgressTypeToBottom ? 0 : 2 ) * 3 + uCol ].cy;
						}
						else
						{
							fVertex2 = tSize.cy - m_pSizes [ 0 * 3 + uCol ].cy - m_pSizes [ 2 * 3 + uCol ].cy;
						}						

						fCoord2 = m_pSizes [ uRow2 * 3 + uCol ].cy / fHigh;
					}

					break;

				case 2 :

					if ( tDstSize.cy > ( tSize.cy - m_pSizes [ uRow2 * 3 + uCol ].cy ) )
					{						
						fVertex2 = m_pSizes [ uRow2 * 3 + uCol ].cy - ( tSize.cy - tDstSize.cy );
						fCoord2  = ( m_pSizes [ uRow2 * 3 + uCol ].cy - ( tSize.cy - tDstSize.cy ) ) / fHigh;			
					}

					break;
			}

			if ( m_eProgressType & kCCProgressTypeToBottom )
			{
				fVertex -= fVertex2;
				fCoord  += fCoord2;
			}
			else
			{
				fVertex += fVertex2;
				fCoord  -= fCoord2;
			}

			m_pVBOs [ uOffset + pIndices [ 0 ] ].vertices.y  = m_pVBOs [ uOffset + pIndices [ 3 ] ].vertices.y  = fVertex;
			m_pVBOs [ uOffset + pIndices [ 0 ] ].texCoords.v = m_pVBOs [ uOffset + pIndices [ 3 ] ].texCoords.v = fCoord;
			m_pVBOs [ uOffset + pIndices [ 0 ] ].colors      = m_pVBOs [ uOffset + pIndices [ 3 ] ].colors      = tColor;
		}
	}		

#if CC_USES_VBO
	glBufferData ( GL_ARRAY_BUFFER, sizeof ( ccV2F_C4B_T2F ) * 9 * 4, m_pVBOs, GL_STATIC_DRAW ); 
#endif

	m_bDirty = KD_FALSE;
}

KDvoid CCProgress::setColor ( const ccColor3B& tColor )
{
	m_pSprite->setColor ( tColor );
	updateColor ( );
}

const ccColor3B& CCProgress::getColor ( KDvoid ) const
{
	return m_pSprite->getColor ( );
}

KDvoid CCProgress::setOpacity ( GLubyte cOpacity )
{
	m_pSprite->setOpacity ( cOpacity );
	updateColor ( );
}

GLubyte CCProgress::getOpacity ( KDvoid ) const
{
	return m_pSprite->getOpacity ( );
}

CCTexture2D* CCProgress::getTexture ( KDvoid )
{
	return m_pTexture;
}

NS_CC_END