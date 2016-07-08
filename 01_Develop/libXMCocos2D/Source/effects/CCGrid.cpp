/* -----------------------------------------------------------------------------------
 *
 *      File            CCGrid.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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
#include "effects/CCGrid.h"
#include "effects/CCGrabber.h"
#include "support/ccUtils.h"
#include "support/CCPointExtension.h"
#include "support/TransformUtils.h"
#include "support/CCEGLView.h"
#include "support/CCNotificationCenter.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "kazmath/kazmath.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN
    
//
// implementation of CCGridBase
//

CCGridBase* CCGridBase::create ( const CCSize& tGridSize )
{
    CCGridBase*  pRet = new CCGridBase ( );

    if ( pRet && pRet->initWithSize ( tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCGridBase* CCGridBase::create ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped )
{
	CCGridBase*  pRet = new CCGridBase ( );

    if ( pRet && pRet->initWithSize ( tGridSize, pTexture, bFlipped ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCGridBase::initWithSize ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped )
{
	KDbool  bRet = KD_TRUE;

	m_bActive    = KD_FALSE;
	m_nReuseGrid = 0;
	m_tGridSize  = tGridSize;
	m_pTexture   = pTexture;
	CC_SAFE_RETAIN ( m_pTexture );
	m_bIsTextureFlipped = bFlipped;

	const CCSize&  tSize = m_pTexture->getContentSize ( );

	m_tStep.x = tSize.cx / m_tGridSize.cx;
	m_tStep.y = tSize.cy / m_tGridSize.cy;

	m_pGrabber = new CCGrabber ( );
	if ( m_pGrabber )
	{
		m_pGrabber->grab ( m_pTexture );
	}
	else
	{
        bRet = KD_FALSE;
	}
	
	m_pShaderProgram = CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTexture );
	calculateVertexPoints ( );

	return bRet;
}

KDbool CCGridBase::initWithSize ( const CCSize& tGridSize )
{
    KDbool  bRet = KD_TRUE;
    
    const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSizeInPixels ( );        

	CCTexture2D*  pTexture = new CCTexture2D ( );
    if ( pTexture )
    {
        pTexture->initWithData ( KD_NULL, kCCTexture2DPixelFormat_RGBA8888, tSize );
        
       	bRet = initWithSize ( tGridSize, pTexture, KD_FALSE );
        
        pTexture->release ( ); 
    }
    else
	{
		CCLOG ( "XMCocos2D : CCGrid - error creating texture" );
		bRet = KD_FALSE;
	}

	return bRet;
}

KDvoid CCGridBase::listenBackToForeground ( CCObject* pObject )
{
	CC_SAFE_RELEASE ( m_pGrabber );
	initWithSize ( m_tGridSize, m_pTexture, m_bIsTextureFlipped );
}

CCGridBase::CCGridBase ( KDvoid )
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // listen the event when app go to background
    CCNotificationCenter::sharedNotificationCenter ( )->addObserver 
	(
		this, callfuncO_selector ( CCGridBase::listenBackToForeground ),
		EVENT_COME_TO_FOREGROUND, KD_NULL 
	);
#endif
}

CCGridBase::~CCGridBase ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCGridBase = %p", this );

	//TODO: ? why 2.0 comments this line	setActive ( KD_FALSE );

	CC_SAFE_RELEASE ( m_pTexture );
	CC_SAFE_RELEASE ( m_pGrabber );

#if CC_ENABLE_CACHE_TEXTURE_DATA
	CCNotificationCenter::sharedNotificationCenter ( )->removeObserver ( this, EVENT_COME_TO_FOREGROUND );
#endif
}

// properties
KDvoid CCGridBase::setActive ( KDbool bActive )
{
	m_bActive = bActive;

	if ( !bActive )
	{
		CCDirector*           pDirector   = CCDirector::sharedDirector ( );
		ccDirectorProjection  eProjection = pDirector->getProjection ( );
		pDirector->setProjection ( eProjection );
	}
}

KDvoid CCGridBase::setTextureFlipped ( KDbool bFlipped )
{
	if ( m_bIsTextureFlipped != bFlipped )
	{
		m_bIsTextureFlipped = bFlipped;
		calculateVertexPoints ( );
	}
}

KDvoid CCGridBase::set2DProjection ( KDvoid )
{
	const CCSize&  tSize  = CCDirector::sharedDirector ( )->getWinSizeInPixels ( );

	glViewport ( 0, 0, (GLsizei) ( tSize.cx ), (GLsizei) ( tSize.cy ) );

	kmGLMatrixMode ( KM_GL_PROJECTION );
	kmGLLoadIdentity ( );

	kmMat4  tOrthoMatrix;
	kmMat4OrthographicProjection ( &tOrthoMatrix, 0, tSize.cx, 0, tSize.cy, -1, 1 );
	kmGLMultMatrix ( &tOrthoMatrix );

	kmGLMatrixMode ( KM_GL_MODELVIEW );
	kmGLLoadIdentity ( );

    ccSetProjectionMatrixDirty ( );
}

KDvoid CCGridBase::beforeDraw ( CCNode* pTarget )
{
	// save projection
	CCDirector::sharedDirector ( )->GLToClipTransform ( );
	m_eDirectorProjection = CCDirector::sharedDirector ( )->getProjection ( );

	// 2d projection
	set2DProjection ( );

	if ( pTarget )
	{
//		CCPoint tPoint = pTarget->convertToWorldSpace ( CCPointZero );
//		kmGLTranslatef ( tPoint.x, tPoint.y, 0 );
	}

	m_pGrabber->beforeRender ( m_pTexture );
}

KDvoid CCGridBase::afterDraw ( CCNode* pTarget )
{
	m_pGrabber->afterRender ( m_pTexture );

	// restore projection
	
	CCDirector::sharedDirector ( )->setProjection ( m_eDirectorProjection );	
	CCDirector::sharedDirector ( )->ClipToGLTransform ( );

	if ( pTarget->getCamera ( )->isDirty ( ) )
	{
		const CCPoint&  tOffset = pTarget->getAnchorPointInPoints ( );

		//
		// XXX: Camera should be applied in the AnchorPoint
		//
		kmGLTranslatef (  tOffset.x,  tOffset.y, 0 );
        
		pTarget->getCamera ( )->locate ( );

        kmGLTranslatef ( -tOffset.x, -tOffset.y, 0 );
	}

	ccGLBindTexture2D ( m_pTexture->getName ( ) );

    // restore projection for default FBO
	blit ( );
}

KDvoid CCGridBase::blit ( KDvoid )
{
	CCAssert ( 0, "" );
}

KDvoid CCGridBase::reuse ( KDvoid )
{
	CCAssert ( 0, "" );
}

KDvoid CCGridBase::calculateVertexPoints ( KDvoid )
{
	CCAssert ( 0, "" );
}

//
// implementation of CCGrid3D
//

CCGrid3D* CCGrid3D::create ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped )
{
	CCGrid3D*  pRet = new CCGrid3D ( );

    if ( pRet && pRet->initWithSize ( tGridSize, pTexture, bFlipped ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCGrid3D* CCGrid3D::create ( const CCSize& tGridSize )
{
	CCGrid3D*  pRet= new CCGrid3D ( );

    if ( pRet && pRet->initWithSize ( tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCGrid3D::CCGrid3D ( KDvoid )
{
	m_pTexCoordinates	= KD_NULL;
	m_pVertices			= KD_NULL;
	m_pIndices			= KD_NULL;
	m_pOriginalVertices	= KD_NULL;
}

CCGrid3D::~CCGrid3D ( KDvoid )
{
	CC_SAFE_FREE ( m_pTexCoordinates );
    CC_SAFE_FREE ( m_pVertices );
	CC_SAFE_FREE ( m_pIndices );
	CC_SAFE_FREE ( m_pOriginalVertices );
}

KDvoid CCGrid3D::blit ( KDvoid )
{
	KDint		n = (KDint) ( m_tGridSize.width * m_tGridSize.height );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

	m_pShaderProgram->use ( );
	m_pShaderProgram->setUniformsForBuiltins ( );

	//
	// Attributes
	//
#ifdef EMSCRIPTEN
	// Size calculations from calculateVertexPoints().
	KDuint		uNumOfPoints = ( m_tGridSize.width + 1 ) * ( m_tGridSize.height + 1 );

	// position
	setGLBufferData ( m_pVertices, uNumOfPoints * sizeof ( ccVertex3F ), 0 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// texCoords
	setGLBufferData ( m_pTexCoordinates, uNumOfPoints * sizeof ( ccVertex2F ), 1 );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	setGLIndexData ( m_pIndices, n * 12, 0 );
	glDrawElements ( GL_TRIANGLES, (GLsizei) n * 6, GL_UNSIGNED_SHORT, 0 );
#else
	// position
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, m_pVertices );

	// texCoords
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pTexCoordinates );

	glDrawElements ( GL_TRIANGLES, (GLsizei) n * 6, GL_UNSIGNED_SHORT, m_pIndices );
#endif // EMSCRIPTEN
	CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid CCGrid3D::calculateVertexPoints ( KDvoid )
{
	KDfloat  fWide  = (KDfloat) m_pTexture->getPixelsWide ( );
	KDfloat  fHigh  = (KDfloat) m_pTexture->getPixelsHigh ( );
	KDfloat  fImgH  =  m_pTexture->getContentSizeInPixels ( ).cy;
	KDint    nGridX = (KDint) m_tGridSize.cx;
	KDint	 nGridY = (KDint) m_tGridSize.cy;

    CC_SAFE_FREE ( m_pVertices );
    CC_SAFE_FREE ( m_pOriginalVertices );
    CC_SAFE_FREE ( m_pTexCoordinates );
    CC_SAFE_FREE ( m_pIndices );

	KDuint  uNumOfPoints = ( nGridX + 1 ) * ( nGridY + 1 );

	m_pVertices            = kdMalloc ( uNumOfPoints * sizeof ( ccVertex3F ) );
	m_pOriginalVertices    = kdMalloc ( uNumOfPoints * sizeof ( ccVertex3F ) );
	m_pTexCoordinates      = kdMalloc ( uNumOfPoints * sizeof ( CCPoint ) );
	m_pIndices = (GLushort*) kdMalloc ( nGridX * nGridY * sizeof ( GLushort ) * 6 );

	GLfloat*  pVertices  = (KDfloat*) m_pVertices;
	GLfloat*  pTexCoords = (KDfloat*) m_pTexCoordinates;
	GLushort* pIndices   = m_pIndices;

	for ( KDint x = 0; x < nGridX; ++x )
	{
		for ( KDint y = 0; y < nGridY; ++y )
		{
			KDint  idx = ( y * nGridX ) + x;

			KDfloat  x1 =  x * m_tStep.x;
			KDfloat  x2 = x1 + m_tStep.x;
			KDfloat  y1 =  y * m_tStep.y;
			KDfloat  y2 = y1 + m_tStep.y;

			GLushort  a = (GLushort) ( x * ( m_tGridSize.cy + 1 ) + y );
			GLushort  b = (GLushort) ( ( x + 1 ) * ( m_tGridSize.cy + 1 ) + y );
			GLushort  c = (GLushort) ( ( x + 1 ) * ( m_tGridSize.cy + 1 ) + ( y + 1 ) );
			GLushort  d = (GLushort) ( x * ( m_tGridSize.cy + 1 ) + ( y + 1 ) );

			GLushort  tempidx[6] = { a, b, d, b, c, d };

			kdMemcpy ( &pIndices [ 6 * idx ], tempidx, 6 * sizeof ( GLushort ) );

			KDint      l1[4] = { a * 3, b * 3, c * 3, d * 3 };
			ccVertex3F     e = { x1, y1, 0 };
			ccVertex3F     f = { x2, y1, 0 };
			ccVertex3F     g = { x2, y2, 0 };
			ccVertex3F     h = { x1, y2, 0 };

			ccVertex3F l2[4] = { e, f, g, h };

			KDint    tex1[4] = { a * 2, b * 2, c * 2, d * 2 };
			CCPoint  tex2[4] = { ccp ( x1, y1 ), ccp ( x2, y1 ), ccp ( x2, y2 ), ccp ( x1, y2 ) };

			for ( KDint i = 0; i < 4; ++i )
			{
				pVertices [ l1[ i ] + 0 ] = l2[ i ].x;
				pVertices [ l1[ i ] + 1 ] = l2[ i ].y;
				pVertices [ l1[ i ] + 2 ] = l2[ i ].z;

				pTexCoords [ tex1[ i ] ] = tex2[ i ].x / fWide;
				if ( m_bIsTextureFlipped )
				{
					pTexCoords [ tex1[ i ] + 1 ] = ( fImgH - tex2[ i ].y ) / fHigh;
				}
				else
				{
					pTexCoords [ tex1[ i ] + 1 ] = tex2[ i ].y / fHigh;
				}
			}
		}
	}

	kdMemcpy ( m_pOriginalVertices, m_pVertices, uNumOfPoints * sizeof ( ccVertex3F ) );
}

ccVertex3F CCGrid3D::vertex ( const CCPoint& tPos )
{
	CCAssert ( tPos.x == (KDuint) tPos.x && tPos.y == (KDuint) tPos.y , "Numbers must be integers" );

	KDint       nIndex    = (KDint) ( tPos.x * ( m_tGridSize.cy + 1 ) + tPos.y ) * 3;
    KDfloat*    pVertices = (KDfloat *) m_pVertices;
	ccVertex3F  tVertex   = { pVertices [ nIndex ], pVertices [ nIndex + 1 ], pVertices [ nIndex + 2 ] };

	return tVertex;
}

ccVertex3F CCGrid3D::originalVertex ( const CCPoint& tPos )
{
	CCAssert ( tPos.x == (KDuint) tPos.x && tPos.y == (KDuint) tPos.y , "Numbers must be integers" );

	KDint       nIndex    = (KDint) ( tPos.x * ( m_tGridSize.cy + 1 ) + tPos.y ) * 3;
	KDfloat*    pVertices = (KDfloat *) m_pOriginalVertices;
	ccVertex3F  tVertex   = { pVertices [ nIndex ], pVertices [ nIndex + 1 ], pVertices [ nIndex + 2 ] };

	return tVertex;
}

KDvoid CCGrid3D::setVertex ( const CCPoint& tPos, const ccVertex3F& tVertex )
{
	CCAssert ( tPos.x == (KDuint) tPos.x && tPos.y == (KDuint) tPos.y , "Numbers must be integers" );

	KDint     nIndex    = (KDint) ( tPos.x * ( m_tGridSize.cy + 1 ) + tPos.y ) * 3;
	KDfloat*  pVertices = (KDfloat *) m_pVertices;

	pVertices [ nIndex + 0 ] = tVertex.x;
	pVertices [ nIndex + 1 ] = tVertex.y;
	pVertices [ nIndex + 2 ] = tVertex.z;
}

KDvoid CCGrid3D::reuse ( KDvoid )
{
	if ( m_nReuseGrid > 0 )
	{
		kdMemcpy ( m_pOriginalVertices, m_pVertices, ( (KDint) m_tGridSize.cx + 1 ) * ( (KDint) m_tGridSize.cy + 1 ) * sizeof ( ccVertex3F ) );

		--m_nReuseGrid;
	}
}

//
// implementation of CCTiledGrid3D
//

CCTiledGrid3D::CCTiledGrid3D ( KDvoid )
{
	 m_pTexCoordinates   = KD_NULL;
	 m_pVertices		 = KD_NULL;
	 m_pOriginalVertices = KD_NULL;
	 m_pIndices			 = KD_NULL;
}

CCTiledGrid3D::~CCTiledGrid3D ( KDvoid )
{
	CC_SAFE_FREE ( m_pTexCoordinates );
	CC_SAFE_FREE ( m_pVertices );
	CC_SAFE_FREE ( m_pOriginalVertices );
	CC_SAFE_FREE ( m_pIndices );
}

CCTiledGrid3D* CCTiledGrid3D::create ( const CCSize& tGridSize, CCTexture2D* pTexture, KDbool bFlipped )
{
	CCTiledGrid3D*  pRet= new CCTiledGrid3D ( );

    if ( pRet && pRet->initWithSize ( tGridSize, pTexture, bFlipped ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCTiledGrid3D* CCTiledGrid3D::create ( const CCSize& tGridSize )
{
	CCTiledGrid3D*  pRet= new CCTiledGrid3D ( );

    if ( pRet && pRet->initWithSize ( tGridSize ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDvoid CCTiledGrid3D::blit ( KDvoid )
{
	KDint	n = (KDint) ( m_tGridSize.cx * m_tGridSize.cy );

    m_pShaderProgram->use ( );
    m_pShaderProgram->setUniformsForBuiltins ( );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

#ifdef EMSCRIPTEN
	KDint	nNumQuads = (KDint) ( m_tGridSize.cx * m_tGridSize.cy );

	// position
	setGLBufferData ( m_pVertices, ( nNumQuads * 4 * sizeof ( ccVertex3F ) ), 0 );
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// texCoords
	setGLBufferData ( m_pTexCoordinates, ( nNumQuads * 4 * sizeof ( ccVertex2F ) ), 1 );
	glVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	setGLIndexData ( m_pIndices, n * 12, 0 );
	glDrawElements ( GL_TRIANGLES, (GLsizei) n * 6, GL_UNSIGNED_SHORT, 0 );
#else
	// position
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, m_pVertices );

	// texCoords
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pTexCoordinates );

	glDrawElements ( GL_TRIANGLES, n * 6, GL_UNSIGNED_SHORT, m_pIndices );
#endif // EMSCRIPTEN

	CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid CCTiledGrid3D::calculateVertexPoints ( KDvoid )
{
	KDfloat  fWide = (KDfloat) m_pTexture->getPixelsWide ( );
	KDfloat  fHigh = (KDfloat) m_pTexture->getPixelsHigh ( );
	KDfloat  fImgH =  m_pTexture->getContentSizeInPixels ( ).cy;
	
	KDint  nNumQuads = (KDint) ( m_tGridSize.cx * m_tGridSize.cy );
	
    CC_SAFE_FREE ( m_pVertices );
    CC_SAFE_FREE ( m_pOriginalVertices );
    CC_SAFE_FREE ( m_pTexCoordinates );
    CC_SAFE_FREE ( m_pIndices );

	m_pVertices             = kdMalloc ( nNumQuads * 4 * sizeof ( ccVertex3F ) );
	m_pOriginalVertices     = kdMalloc ( nNumQuads * 4 * sizeof ( ccVertex3F ) );
	m_pTexCoordinates       = kdMalloc ( nNumQuads * 4 * sizeof ( ccVertex2F ) );
	m_pIndices = (GLushort *) kdMalloc ( nNumQuads * 6 * sizeof ( GLushort ) );
	
	GLfloat*   pVertices = (GLfloat *) m_pVertices;
	GLfloat*  pTexCoords = (GLfloat *) m_pTexCoordinates;
	GLushort*   pIndices = m_pIndices;
	
	for ( KDint x = 0; x < (KDint) m_tGridSize.cx; x++ )
	{
		for ( KDint y = 0; y < (KDint) m_tGridSize.cy; y++ )
		{
			KDfloat  x1 =  x * m_tStep.x;
			KDfloat  x2 = x1 + m_tStep.x;
			KDfloat  y1 =  y * m_tStep.y;
			KDfloat  y2 = y1 + m_tStep.y;
			
			*pVertices++ = x1;
			*pVertices++ = y1;
			*pVertices++ =  0;
			*pVertices++ = x2;
			*pVertices++ = y1;
			*pVertices++ =  0;
			*pVertices++ = x1;
			*pVertices++ = y2;
			*pVertices++ =  0;
			*pVertices++ = x2;
			*pVertices++ = y2;
			*pVertices++ =  0;
			
			KDfloat  newY1 = y1;
			KDfloat  newY2 = y2;
			
			if ( m_bIsTextureFlipped ) 
			{
				newY1 = fImgH - y1;
				newY2 = fImgH - y2;
			}

			*pTexCoords++ =    x1 / fWide;
			*pTexCoords++ = newY1 / fHigh;
			*pTexCoords++ =    x2 / fWide;
			*pTexCoords++ = newY1 / fHigh;
			*pTexCoords++ =    x1 / fWide;
			*pTexCoords++ = newY2 / fHigh;
			*pTexCoords++ =    x2 / fWide;
			*pTexCoords++ = newY2 / fHigh;
		}
	}
	
	for ( KDint x = 0; x < nNumQuads; x++ )
	{
		GLushort  x6 = (GLushort) ( x * 6 );
		GLushort  x4 = (GLushort) ( x * 4 );

		pIndices [ x6 + 0 ] = x4 + 0;
		pIndices [ x6 + 1 ] = x4 + 1;
		pIndices [ x6 + 2 ] = x4 + 2;
		
		pIndices [ x6 + 3 ] = x4 + 1;
		pIndices [ x6 + 4 ] = x4 + 2;
		pIndices [ x6 + 5 ] = x4 + 3;
	}
	
	kdMemcpy ( m_pOriginalVertices, m_pVertices, nNumQuads * 12 * sizeof ( GLfloat ) );
}

KDvoid CCTiledGrid3D::setTile ( const CCPoint& tPos, const ccQuad3& tCoords )
{
	CCAssert ( tPos.x == (KDuint) tPos.x && tPos.y == (KDuint) tPos.y , "Numbers must be integers" );

	KDint     nIndex    = (KDint) ( m_tGridSize.cy * tPos.x + tPos.y ) * 4 * 3;
	KDfloat*  pVertices = (KDfloat *) m_pVertices;

	kdMemcpy ( &pVertices [ nIndex ], &tCoords, sizeof ( ccQuad3 ) );
}

ccQuad3 CCTiledGrid3D::originalTile ( const CCPoint& tPos )
{
	CCAssert ( tPos.x == (KDuint) tPos.x && tPos.y == (KDuint) tPos.y , "Numbers must be integers" );

	KDint     nIndex    = (KDint) ( m_tGridSize.cy * tPos.x + tPos.y ) * 4 * 3;
	KDfloat*  pVertices = (KDfloat *) m_pOriginalVertices;

	ccQuad3   tRet;
	kdMemcpy ( &tRet, &pVertices [ nIndex ], sizeof ( ccQuad3 ) );

	return tRet;
}

ccQuad3 CCTiledGrid3D::tile ( const CCPoint& tPos )
{
	CCAssert ( tPos.x == (KDuint) tPos.x && tPos.y == (KDuint) tPos.y , "Numbers must be integers" );

	KDint     nIndex = (KDint) ( m_tGridSize.cy * tPos.x + tPos.y ) * 4 * 3;
	KDfloat*  pVertices = (KDfloat *) m_pVertices;

	ccQuad3   tRet;
	kdMemcpy ( &tRet, &pVertices [ nIndex ], sizeof ( ccQuad3 ) );

	return tRet;
}

KDvoid CCTiledGrid3D::reuse ( KDvoid )
{
	if ( m_nReuseGrid > 0 )
	{
		KDint nNumQuads = (KDint) m_tGridSize.cx * (KDint) m_tGridSize.cy;
		kdMemcpy ( m_pOriginalVertices, m_pVertices, nNumQuads * 12 * sizeof ( GLfloat ) );

		--m_nReuseGrid;
	}
}

NS_CC_END