/* -----------------------------------------------------------------------------------
 *
 *      File            CCDrawNode.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Scott Lembcke and Howling Moon Software
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
#include "draw_nodes/CCDrawNode.h"
#include "shaders/CCShaderCache.h"
#include "support/CCNotificationCenter.h"

NS_CC_BEGIN
   
// ccVertex2F == CGPoint in 32-bits, but not in 64-bits (OS X)
// that's why the "v2f" functions are needed
static ccVertex2F  v2fzero = { 0.0f, 0.0f };

static inline ccVertex2F v2f ( KDfloat x, KDfloat y )
{
    ccVertex2F  tRet = { x, y };

	return tRet;
}

static inline ccVertex2F v2fadd ( const ccVertex2F& v0, const ccVertex2F& v1 )
{
	return v2f ( v0.x + v1.x, v0.y + v1.y );
}

static inline ccVertex2F v2fsub ( const ccVertex2F& v0, const ccVertex2F& v1 )
{
	return v2f ( v0.x - v1.x, v0.y - v1.y );
}

static inline ccVertex2F v2fmult ( const ccVertex2F& v, KDfloat s )
{
	return v2f ( v.x * s, v.y * s );
}

static inline ccVertex2F v2fperp ( const ccVertex2F& p0 )
{
	return v2f ( -p0.y, p0.x );
}

static inline ccVertex2F v2fneg ( const ccVertex2F& p0 )
{
	return v2f ( -p0.x, -p0.y );
}

static inline KDfloat v2fdot ( const ccVertex2F& p0, const ccVertex2F& p1 )
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline ccVertex2F v2fforangle ( KDfloat a )
{
	return v2f ( kdCosf ( a ), kdSinf ( a ) );
}

static inline ccVertex2F v2fnormalize ( const ccVertex2F& p )
{
	CCPoint  r = ccpNormalize ( ccp ( p.x, p.y ) );
	return v2f ( r.x, r.y );
}

static inline ccVertex2F __v2f ( const CCPoint& v )
{
	return v2f ( v.x, v.y );
}

static inline ccTex2F __t ( const ccVertex2F& v )
{
	return *(ccTex2F*) &v;
}

//
// implementation of CCDrawNode
//

CCDrawNode::CCDrawNode ( KDvoid )
{
	m_uVao				= 0;
	m_uVbo				= 0;
	m_uBufferCapacity	= 0;
	m_nBufferCount		= 0;
	m_pBuffer			= KD_NULL;
	m_bDirty			= KD_FALSE;

    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;
}

CCDrawNode::~CCDrawNode ( KDvoid )
{
	CC_SAFE_FREE ( m_pBuffer );
    
    glDeleteBuffers ( 1, &m_uVbo );
    m_uVbo = 0;
    
#if CC_TEXTURE_ATLAS_USE_VAO      
    glDeleteVertexArrays ( 1, &m_uVao );
	ccGLBindVAO ( 0 );
    m_uVao = 0;
#endif

#if CC_ENABLE_CACHE_TEXTURE_DATA
	CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, EVENT_COME_TO_FOREGROUND );
#endif    
}

CCDrawNode* CCDrawNode::create ( KDvoid )
{
    CCDrawNode*  pRet = new CCDrawNode ( );
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

KDvoid CCDrawNode::ensureCapacity ( KDuint uCount )
{
    if ( m_nBufferCount + uCount > m_uBufferCapacity )
    {
		m_uBufferCapacity += KD_MAX ( m_uBufferCapacity, uCount );
		m_pBuffer = (ccV2F_C4B_T2F*) kdRealloc ( m_pBuffer, m_uBufferCapacity * sizeof ( ccV2F_C4B_T2F ) );
	}
}

KDbool CCDrawNode::init ( KDvoid )
{
    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;

    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionLengthTexureColor ) );
    
    ensureCapacity ( 512 );
    
#if CC_TEXTURE_ATLAS_USE_VAO    
    glGenVertexArrays ( 1, &m_uVao );
    ccGLBindVAO ( m_uVao );
#endif
    
    glGenBuffers ( 1, &m_uVbo );
    glBindBuffer ( GL_ARRAY_BUFFER, m_uVbo );
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( ccV2F_C4B_T2F ) * m_uBufferCapacity, m_pBuffer, GL_DYNAMIC_DRAW );    
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
    
#if CC_TEXTURE_ATLAS_USE_VAO 
    ccGLBindVAO ( 0 );
#endif
    
    CHECK_GL_ERROR_DEBUG ( );
    
    m_bDirty = KD_TRUE;

#if CC_ENABLE_CACHE_TEXTURE_DATA
	// Need to listen the event only when not use batchnode, because it will use VBO
	CCNotificationCenter::sharedNotificationCenter ( )->addObserver
	(
		this,
		callfuncO_selector ( CCDrawNode::listenBackToForeground ),
		EVENT_COME_TO_FOREGROUND,
		KD_NULL 
	);
#endif

    return KD_TRUE;
}

KDvoid CCDrawNode::render ( KDvoid )
{
	if ( m_bDirty )
	{
		glBindBuffer ( GL_ARRAY_BUFFER, m_uVbo );
		glBufferData ( GL_ARRAY_BUFFER, sizeof ( ccV2F_C4B_T2F ) * m_uBufferCapacity, m_pBuffer, GL_DYNAMIC_DRAW );
		m_bDirty = KD_FALSE;
	}
#if CC_TEXTURE_ATLAS_USE_VAO     
	ccGLBindVAO ( m_uVao );
#else
	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColor );
	glBindBuffer ( GL_ARRAY_BUFFER, m_uVbo );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT		, GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid*) offsetof ( ccV2F_C4B_T2F, vertices  ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color	   , 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof ( ccV2F_C4B_T2F ), (GLvoid*) offsetof ( ccV2F_C4B_T2F, colors    ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT        , GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid*) offsetof ( ccV2F_C4B_T2F, texCoords ) );
#endif

	glDrawArrays ( GL_TRIANGLES, 0, m_nBufferCount );
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );	

	CC_INCREMENT_GL_DRAWS ( 1 );
	CHECK_GL_ERROR_DEBUG  ( );
}

KDvoid CCDrawNode::draw ( KDvoid )
{
	CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );
    
    render ( );
}

KDvoid CCDrawNode::drawDot ( const CCPoint& tPos, KDfloat fRadius, const ccColor4F& tColor )
{
    KDuint  uVertexCount = 2 * 3;
    ensureCapacity ( uVertexCount );
	
	ccV2F_C4B_T2F  a = { { tPos.x - fRadius, tPos.y - fRadius }, ccc4BFromccc4F ( tColor ), { -1.0, -1.0 } };
	ccV2F_C4B_T2F  b = { { tPos.x - fRadius, tPos.y + fRadius }, ccc4BFromccc4F ( tColor ), { -1.0,  1.0 } };
	ccV2F_C4B_T2F  c = { { tPos.x + fRadius, tPos.y + fRadius }, ccc4BFromccc4F ( tColor ), {  1.0,  1.0 } };
	ccV2F_C4B_T2F  d = { { tPos.x + fRadius, tPos.y - fRadius }, ccc4BFromccc4F ( tColor ), {  1.0, -1.0 } };
	
	ccV2F_C4B_T2F_Triangle*  pTriangles = ( ccV2F_C4B_T2F_Triangle*) ( m_pBuffer + m_nBufferCount );
    ccV2F_C4B_T2F_Triangle   tTriangle0 = { a, b, c };
    ccV2F_C4B_T2F_Triangle   tTriangle1 = { a, c, d };

	pTriangles [ 0 ] = tTriangle0;
	pTriangles [ 1 ] = tTriangle1;
	
	m_nBufferCount += uVertexCount;
	
	m_bDirty = KD_TRUE;
}

KDvoid CCDrawNode::drawSegment ( const CCPoint& tFrom, const CCPoint& tTo, KDfloat fRadius, const ccColor4F& tColor )
{
    KDuint  uVertexCount = 6 * 3;
    ensureCapacity ( uVertexCount );
	
	ccVertex2F  a = __v2f ( tFrom );
	ccVertex2F  b = __v2f ( tTo );
	
	ccVertex2F  n = v2fnormalize ( v2fperp ( v2fsub ( b, a ) ) );
	ccVertex2F  t = v2fperp ( n );
	
	ccVertex2F  nw = v2fmult ( n, fRadius );
	ccVertex2F  tw = v2fmult ( t, fRadius );
	ccVertex2F  v0 = v2fsub ( b, v2fadd ( nw, tw ) );
	ccVertex2F  v1 = v2fadd ( b, v2fsub ( nw, tw ) );
	ccVertex2F  v2 = v2fsub ( b, nw );
	ccVertex2F  v3 = v2fadd ( b, nw );
	ccVertex2F  v4 = v2fsub ( a, nw );
	ccVertex2F  v5 = v2fadd ( a, nw );
	ccVertex2F  v6 = v2fsub ( a, v2fsub ( nw, tw ) );
	ccVertex2F  v7 = v2fadd ( a, v2fadd ( nw, tw ) );
	
	ccV2F_C4B_T2F_Triangle*  pTriangles = (ccV2F_C4B_T2F_Triangle *) ( m_pBuffer + m_nBufferCount );
	
    ccV2F_C4B_T2F_Triangle   tTriangles0 = 
	{
        { v0, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( v2fadd ( n, t ) ) ) },
        { v1, ccc4BFromccc4F ( tColor ), __t ( v2fsub ( n, t ) ) },
        { v2, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( n ) ) },
    }; 
	pTriangles [ 0 ] = tTriangles0;
	
    ccV2F_C4B_T2F_Triangle  tTriangles1 =
	{
        { v3, ccc4BFromccc4F ( tColor ), __t ( n ) },
        { v1, ccc4BFromccc4F ( tColor ), __t ( v2fsub ( n, t ) ) },
        { v2, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( n ) ) },
    };
	pTriangles [ 1 ] = tTriangles1;
	
    ccV2F_C4B_T2F_Triangle  tTriangles2 =
	{
        { v3, ccc4BFromccc4F ( tColor ), __t ( n ) },
        { v4, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( n ) ) },
        { v2, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( n ) ) },
    };
	pTriangles [ 2 ] = tTriangles2;

    ccV2F_C4B_T2F_Triangle  tTriangles3 =
	{
        { v3, ccc4BFromccc4F ( tColor ), __t ( n ) },
        { v4, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( n ) ) },
        { v5, ccc4BFromccc4F ( tColor ), __t ( n ) },
    };
    pTriangles [ 3 ] = tTriangles3;

    ccV2F_C4B_T2F_Triangle  tTriangles4 =
	{
        { v6, ccc4BFromccc4F ( tColor ), __t ( v2fsub ( t, n ) ) },
        { v4, ccc4BFromccc4F ( tColor ), __t ( v2fneg ( n ) ) },
        { v5, ccc4BFromccc4F ( tColor ), __t ( n ) },
    };
	pTriangles [ 4 ] = tTriangles4;

    ccV2F_C4B_T2F_Triangle  tTriangles5 = 
	{
        { v6, ccc4BFromccc4F ( tColor ), __t ( v2fsub ( t, n ) ) },
        { v7, ccc4BFromccc4F ( tColor ), __t ( v2fadd ( n, t ) ) },
        { v5, ccc4BFromccc4F ( tColor ), __t ( n ) },
    };
	pTriangles [ 5 ] = tTriangles5;
	
	m_nBufferCount += uVertexCount;
	
	m_bDirty = KD_TRUE;
}

KDvoid CCDrawNode::drawPolygon ( CCPoint* pVerts, KDuint uCount, const ccColor4F& tFillColor, KDfloat fBorderWidth, const ccColor4F& tBorderColor )
{
    struct ExtrudeVerts 
	{
		ccVertex2F	offset;
		ccVertex2F	n;
	};

	struct ExtrudeVerts*  pExtrude = (struct ExtrudeVerts*) kdMalloc ( sizeof ( struct ExtrudeVerts ) * uCount );
	kdMemset ( pExtrude, 0, sizeof ( struct ExtrudeVerts ) * uCount );
	
	for ( KDuint i = 0; i < uCount; i++ )
    {
		ccVertex2F  v0 = __v2f ( pVerts [ ( i - 1 + uCount ) % uCount ] );
		ccVertex2F  v1 = __v2f ( pVerts [ i ] );
		ccVertex2F  v2 = __v2f ( pVerts [ ( i + 1 ) % uCount ] );
        
		ccVertex2F  n1 = v2fnormalize ( v2fperp ( v2fsub ( v1, v0 ) ) );
		ccVertex2F  n2 = v2fnormalize ( v2fperp ( v2fsub ( v2, v1 ) ) );
		
		ccVertex2F  offset = v2fmult ( v2fadd ( n1, n2 ), 1.0f / ( v2fdot ( n1, n2 ) + 1.0f ) );
        struct ExtrudeVerts  tTemp = { offset, n2 };
		pExtrude [ i ] = tTemp;
	}
	
	KDbool  bOutline = ( tBorderColor.a > 0.0f && fBorderWidth > 0.0f );
	
	KDuint  uTriangleCount = 3 * uCount - 2;
	KDuint  uVertexCount   = 3 * uTriangleCount;
    ensureCapacity ( uVertexCount );
	
	ccV2F_C4B_T2F_Triangle*  pTriangles = (ccV2F_C4B_T2F_Triangle*) ( m_pBuffer + m_nBufferCount );
	ccV2F_C4B_T2F_Triangle*  pCursor = pTriangles;
	
	KDfloat  fInset = ( bOutline ? 0.5f : 0.0f );
	for ( KDuint i = 0; i < uCount - 2; i++ )
    {
		ccVertex2F  v0 = v2fsub ( __v2f ( pVerts [ 0     ] ), v2fmult ( pExtrude [ 0     ].offset, fInset ) );
		ccVertex2F  v1 = v2fsub ( __v2f ( pVerts [ i + 1 ] ), v2fmult ( pExtrude [ i + 1 ].offset, fInset ) );
		ccVertex2F  v2 = v2fsub ( __v2f ( pVerts [ i + 2 ] ), v2fmult ( pExtrude [ i + 2 ].offset, fInset ) );
		
        ccV2F_C4B_T2F_Triangle  tTemp =
		{
            { v0, ccc4BFromccc4F ( tFillColor ), __t ( v2fzero ) },
            { v1, ccc4BFromccc4F ( tFillColor ), __t ( v2fzero ) },
            { v2, ccc4BFromccc4F ( tFillColor ), __t ( v2fzero ) },
        };

		*pCursor++ = tTemp;
	}
	
	for ( KDuint i = 0; i < uCount; i++ )
    {
		KDint  j = ( i + 1 ) % uCount;

		ccVertex2F  v0 = __v2f ( pVerts [ i ] );
		ccVertex2F  v1 = __v2f ( pVerts [ j ] );
		
		ccVertex2F  n0 = pExtrude [ i ].n;
		
		ccVertex2F  tOffset0 = pExtrude [ i ].offset;
		ccVertex2F  tOffset1 = pExtrude [ j ].offset;
		
		if ( bOutline )
        {
			ccVertex2F  tInner0 = v2fsub ( v0, v2fmult ( tOffset0, fBorderWidth ) );
			ccVertex2F  tInner1 = v2fsub ( v1, v2fmult ( tOffset1, fBorderWidth ) );
			ccVertex2F  tOuter0 = v2fadd ( v0, v2fmult ( tOffset0, fBorderWidth ) );
			ccVertex2F  tOuter1 = v2fadd ( v1, v2fmult ( tOffset1, fBorderWidth ) );
			
            ccV2F_C4B_T2F_Triangle  tTemp1 = 
			{
                { tInner0, ccc4BFromccc4F ( tBorderColor ), __t ( v2fneg ( n0 ) ) },
                { tInner1, ccc4BFromccc4F ( tBorderColor ), __t ( v2fneg ( n0 ) ) },
                { tOuter1, ccc4BFromccc4F ( tBorderColor ), __t ( n0 ) }
            };
			*pCursor++ = tTemp1;
 
            ccV2F_C4B_T2F_Triangle  tTemp2 =
			{
                { tInner0, ccc4BFromccc4F ( tBorderColor ), __t ( v2fneg ( n0 ) ) },
                { tOuter0, ccc4BFromccc4F ( tBorderColor ), __t ( n0 ) },
                { tOuter1, ccc4BFromccc4F ( tBorderColor ), __t ( n0 ) }
            };
			*pCursor++ = tTemp2;
		}
        else 
		{
			ccVertex2F  tInner0 = v2fsub ( v0, v2fmult ( tOffset0, 0.5f ) );
			ccVertex2F  tInner1 = v2fsub ( v1, v2fmult ( tOffset1, 0.5f ) );
			ccVertex2F  tOuter0 = v2fadd ( v0, v2fmult ( tOffset0, 0.5f ) );
			ccVertex2F  tOuter1 = v2fadd ( v1, v2fmult ( tOffset1, 0.5f ) );
			
            ccV2F_C4B_T2F_Triangle  tTemp1 =
			{
                { tInner0, ccc4BFromccc4F ( tFillColor ), __t ( v2fzero ) },
                { tInner1, ccc4BFromccc4F ( tFillColor ), __t ( v2fzero ) },
                { tOuter1, ccc4BFromccc4F ( tFillColor ), __t ( n0 ) }
            };
			*pCursor++ = tTemp1;

            ccV2F_C4B_T2F_Triangle  tTemp2 =
			{
                { tInner0, ccc4BFromccc4F ( tFillColor ), __t ( v2fzero ) },
                { tOuter0, ccc4BFromccc4F ( tFillColor ), __t ( n0 ) },
                { tOuter1, ccc4BFromccc4F ( tFillColor ), __t ( n0 ) }
            };
			*pCursor++ = tTemp2;
		}
	}
	
	m_nBufferCount += uVertexCount;
	
	m_bDirty = KD_TRUE;

    kdFree ( pExtrude );
}

KDvoid CCDrawNode::clear ( KDvoid )
{
    m_nBufferCount = 0;
    m_bDirty = KD_TRUE;
}

ccBlendFunc CCDrawNode::getBlendFunc ( KDvoid ) const
{
    return m_tBlendFunc;
}

KDvoid CCDrawNode::setBlendFunc ( const ccBlendFunc& tBlendFunc )
{
    m_tBlendFunc = tBlendFunc;
}

/** 
 *	listen the event that coming to foreground on Android
 */
KDvoid CCDrawNode::listenBackToForeground ( CCObject* pObject )
{
    init ( );
}

NS_CC_END