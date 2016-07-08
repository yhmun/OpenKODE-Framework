/* -----------------------------------------------------------------------------------
 *
 *      File            PRFilledPolygon.cpp
 *      Description     This class fills a polygon as described by an array of
 *                      NSValue-encapsulated points with a texture.
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Damiano Mazzella ( Translated in C++ for Cocos2d-X on 19/03/2012 )
 *      Copyright (c) 2011      Precognitive Research, LLC. All rights reserved. ( Created by Andy Sinesio on 6/25/10 )
 *
 *         http://precognitiveresearch.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCPRKit/PRFilledPolygon.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN


PRFilledPolygon* PRFilledPolygon::create ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture )
{    
    PRFilledPolygon*	pRet = new PRFilledPolygon ( );
    
	if ( pRet && pRet->initWithPoints ( aPolygonPoints, pFillTexture ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;    
}

PRFilledPolygon* PRFilledPolygon::create ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture, PRRatcliffTriangulator* pPolygonTriangulator )
{    
    PRFilledPolygon*	pRet = new PRFilledPolygon ( );
    
	if ( pRet && pRet->initWithPoints ( aPolygonPoints, pFillTexture, pPolygonTriangulator ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;  
}

PRFilledPolygon::PRFilledPolygon ( KDvoid )
{
	m_nAreaTrianglePointCount	= 0;
	m_pTexture					= KD_NULL;
	m_pAreaTrianglePoints		= KD_NULL;
	m_pTextureCoordinates		= KD_NULL;
	m_pTriangulator				= KD_NULL;
}

PRFilledPolygon::~PRFilledPolygon ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pTexture );

	CC_SAFE_FREE ( m_pTriangulator );
    CC_SAFE_FREE ( m_pAreaTrianglePoints );
    CC_SAFE_FREE ( m_pTextureCoordinates );
}

KDbool PRFilledPolygon::initWithPoints ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture )
{        
    PRRatcliffTriangulator*		pPolygonTriangulator = new PRRatcliffTriangulator ( );

    return this->initWithPoints ( aPolygonPoints, pFillTexture, pPolygonTriangulator );
}

KDbool PRFilledPolygon::initWithPoints ( Vector2dVector& aPolygonPoints, CCTexture2D* pFillTexture, PRRatcliffTriangulator* pPolygonTriangulator ) 
{   
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

    m_pTriangulator = pPolygonTriangulator;
    
    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTexture ) );
    
    this->setTexture ( pFillTexture );

    this->setPoints ( aPolygonPoints );
    
	return KD_TRUE;
}

KDvoid PRFilledPolygon::setPoints ( Vector2dVector& aPoints )
{    
    CC_SAFE_FREE ( m_pAreaTrianglePoints );
    CC_SAFE_FREE ( m_pTextureCoordinates );
    
    Vector2dVector	aTriangulatedPoints = PRRatcliffTriangulator::triangulateVertices ( aPoints );
    
    m_nAreaTrianglePointCount = aTriangulatedPoints.size ( );

    m_pAreaTrianglePoints = (ccVertex2F*) kdMalloc ( sizeof ( ccVertex2F ) * m_nAreaTrianglePointCount );
    m_pTextureCoordinates = (ccVertex2F*) kdMalloc ( sizeof ( ccVertex2F ) * m_nAreaTrianglePointCount );
    
    for ( KDint i = 0; i < m_nAreaTrianglePointCount; i++ )
	{
        Vector2d	tVert = (Vector2d) aTriangulatedPoints.at ( i );

		m_pAreaTrianglePoints [ i ].x = tVert.GetX ( );
		m_pAreaTrianglePoints [ i ].y = tVert.GetY ( );
    }
    
    calculateTextureCoordinates ( );
}

KDvoid PRFilledPolygon::calculateTextureCoordinates ( KDvoid  )
{
	GLfloat		fScale = 1.0f / m_pTexture->getPixelsWide ( ) * CC_CONTENT_SCALE_FACTOR ( );

    for ( KDint j = 0; j < m_nAreaTrianglePointCount; j++ )
	{		
        m_pTextureCoordinates [ j ].x = m_pAreaTrianglePoints [ j ].x * fScale;
		m_pTextureCoordinates [ j ].y = 1.0f - m_pAreaTrianglePoints [ j ].y * fScale;
    }
}

KDvoid PRFilledPolygon::draw ( KDvoid )
{    
	CC_NODE_DRAW_SETUP ( );   
    
    ccGLBindTexture2D ( m_pTexture->getName ( ) );
    
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );
    
    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
        
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, 0, m_pAreaTrianglePoints );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pTextureCoordinates );

	glDrawArrays ( GL_TRIANGLES, 0, (GLsizei) m_nAreaTrianglePointCount );
    
    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid PRFilledPolygon::updateBlendFunc ( KDvoid )
{
    // it's possible to have an untextured sprite
    if ( !m_pTexture || !m_pTexture->hasPremultipliedAlpha ( ) )
	{
        m_tBlendFunc.src = GL_SRC_ALPHA;
        m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    } 
	else 
	{
        m_tBlendFunc.src = CC_BLEND_SRC;
        m_tBlendFunc.dst = CC_BLEND_DST;
    }
}

KDvoid PRFilledPolygon::setBlendFunc ( ccBlendFunc tBlendFunc ) 
{
    m_tBlendFunc = tBlendFunc;
}

KDvoid PRFilledPolygon::setTexture ( CCTexture2D* pTexture )
{        
    CCAssert ( pTexture, "setTexture expects a CCTexture2D. Invalid argument" );

    CC_SAFE_RELEASE ( m_pTexture );	
    m_pTexture = pTexture;
    CC_SAFE_RETAIN ( m_pTexture );
    
	ccTexParams		aTexParams = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
	pTexture->setTexParameters ( aTexParams );
    
    this->updateBlendFunc ( );

	this->calculateTextureCoordinates ( );    
}

CCTexture2D* PRFilledPolygon::getTexture ( KDvoid )
{
    return m_pTexture;
}
 
NS_CC_END