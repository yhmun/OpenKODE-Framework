/* --------------------------------------------------------------------------
 *
 *      File            TexturedPolygon.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "TexturedPolygon.h"
#include "Vector3D.h"

TexturedPolygon* TexturedPolygon::create ( const KDchar* szFilePath, CCArray* pVerts )
{
	TexturedPolygon*		pRet = new TexturedPolygon ( );

	if ( pRet && pRet->initWithFile ( szFilePath, pVerts ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

TexturedPolygon* TexturedPolygon::create ( const KDchar* szFilePath, CCArray* pVerts, CCArray* pTris )
{
	TexturedPolygon*		pRet = new TexturedPolygon ( );

	if ( pRet && pRet->initWithFile ( szFilePath, pVerts, pTris ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

TexturedPolygon::TexturedPolygon ( KDvoid )
{
	m_pVertices		= KD_NULL;
	m_pTriangles	= KD_NULL;
	m_pCoords		= KD_NULL;
}

TexturedPolygon::~TexturedPolygon ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pVertices );
	CC_SAFE_RELEASE ( m_pTriangles );
	CC_SAFE_FREE ( m_pCoords );
}

KDbool TexturedPolygon::initWithFile ( const KDchar* szFilePath, CCArray* pVerts )
{
	// Create a TexturedPolygon with vertices only. 
	// Perform polygon trianglulation to get triangles. 
	
	// Initialization
	if ( !CCSprite::initWithFile ( szFilePath ) )
	{
		return KD_FALSE;
	}

	this->setAnchorPoint ( ccp ( 0, 0 ) );

	m_pVertices  = CCArray::create ( );
	m_pVertices->retain ( );

	m_pTriangles = CCArray::create ( );
	m_pTriangles->retain ( );
	
	// Polygon Triangulation
	Vector2dVector	a;
	for ( KDuint i = 0; i < pVerts->count ( ); i++ )
	{
		// Add polygon vertices
		m_pVertices->addObject ( pVerts->objectAtIndex ( i ) );
		
		// Add polygon vertices to triangulation container
		CCPoint		tVert = ( (CCPointValue*) pVerts->objectAtIndex ( i ) )->getValue ( ); 
		a.push_back ( Vector2d ( tVert.x, tVert.y ) );
	}

	// Run triangulation algorithm
	Vector2dVector	tResult;
	Triangulate::Process ( a, tResult );

	// Gather all triangles from result container
	KDint	nTriCnt = tResult.size ( ) / 3;
	for ( KDint i = 0; i < nTriCnt; i++ )
	{
		const Vector2d&		tP1 = tResult [ i * 3 + 0 ];
		const Vector2d&		tP2 = tResult [ i * 3 + 1 ];
		const Vector2d&		tP3 = tResult [ i * 3 + 2 ];
		
		CCLOG 
		(
			"Triangle %d => (%f,%f) (%f,%f) (%f,%f)", 
			i + 1, 
			tP1.GetX ( ), tP1.GetY ( ), 
			tP2.GetX ( ), tP2.GetY ( ),
			tP3.GetX ( ), tP3.GetY ( ) 
		);
		
		// Add triangle index
		m_pTriangles->addObject 
		(
			this->getTriangleIndicesFromPoint1
			(
				ccp ( tP1.GetX ( ), tP1.GetY ( ) ),
				ccp ( tP2.GetX ( ), tP2.GetY ( ) ), 
				ccp ( tP3.GetX ( ), tP3.GetY ( ) ) 
			) 
		);
	}	

	// Set texture coordinate information
	this->setCoordInfo ( );

	return KD_TRUE;
}

KDbool TexturedPolygon::initWithFile ( const KDchar* szFilePath, CCArray* pVerts, CCArray* pTris )
{
	// Create a TexturedPolygon with vertices and triangles given. 
	
	// Initialization
	if ( !CCSprite::initWithFile ( szFilePath ) )
	{
		return KD_FALSE;
	}

	this->setAnchorPoint ( ccp ( 0, 0 ) );

	m_pVertices  = CCArray::create ( );
	m_pVertices->retain ( );

	m_pTriangles = CCArray::create ( );
	m_pTriangles->retain ( );
		
	// Set polygon vertices
	for ( KDuint i = 0; i < pVerts->count ( ); i++ )
	{
		m_pVertices->addObject ( pVerts->objectAtIndex ( i ) );
	}

	// Set triangle indices
	for ( KDuint i = 0; i < pTris->count ( ); i++ )
	{
		m_pTriangles->addObject ( pTris->objectAtIndex ( i ) );
	}

	// Set texture coordinate information
	this->setCoordInfo ( );

	return KD_TRUE;
}

KDvoid TexturedPolygon::draw ( KDvoid )
{
	// This is where the magic happens. Texture and draw all triangles. 

	CC_NODE_DRAW_SETUP ( );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosTex );
	
	ccColor3B	tColor = this->getColor ( );	
	ccDrawColor4B ( tColor.r, tColor.g, tColor.b, this->getOpacity () );

	ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

    if ( m_pTexture != KD_NULL )
    {
        ccGLBindTexture2D ( m_pTexture->getName ( ) );
    }
    else
    {
        ccGLBindTexture2D ( 0 );
    }
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, sizeof ( ccV2F_T2F ), (GLvoid*) ( (GLintptr) m_pCoords + offsetof ( ccV2F_T2F, vertices  ) ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof ( ccV2F_T2F ), (GLvoid*) ( (GLintptr) m_pCoords + offsetof ( ccV2F_T2F, texCoords ) ) );

	for ( KDuint i = 0; i < m_pTriangles->count ( ); i++ )
	{
		Vector3D*	pTri = (Vector3D*) m_pTriangles->objectAtIndex ( i );
		KDshort		aIndices [ ] = { (KDshort) pTri->x, (KDshort) pTri->y, (KDshort) pTri->z };
		glDrawElements ( GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, aIndices );
	}
	
	ccDrawColor4B ( 255, 255, 255, 255 );

	CC_INCREMENT_GL_DRAWS ( 1 );
}

Vector3D* TexturedPolygon::getTriangleIndicesFromPoint1 ( const CCPoint& tP1, const CCPoint& tP2, const CCPoint& tP3 )
{
	// Convert three polygon vertices to triangle indices 
	
	Vector3D*	pIndices = Vector3D::create ( -1, -1, -1 );
	
	for ( KDuint i = 0; i < m_pVertices->count ( ); i++ )
	{
		CCPoint		tVert = ( (CCPointValue*) m_pVertices->objectAtIndex ( i ) )->getValue ( ); 

		if ( tP1.x == tVert.x && tP1.y == tVert.y )
		{
			pIndices->x = (KDfloat) i;
		}
		else if ( tP2.x == tVert.x && tP2.y == tVert.y )
		{
			pIndices->y = (KDfloat) i;
		}
		else if ( tP3.x == tVert.x && tP3.y == tVert.y )
		{
			pIndices->z = (KDfloat) i;
		}
	}

	return pIndices;
}

KDvoid TexturedPolygon::addAnimFrameWithFile ( const KDchar* szFilePath, CCArray* pArr )
{
	// For textured polygon animation 
	
	ccTexParams		tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	CCTexture2D*	pFrameTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilePath );
	pFrameTexture->setTexParameters ( tParams );
	CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pFrameTexture, this->getTextureRect ( ) );
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFrame ( pFrame, szFilePath );
	pArr->addObject ( pFrame );
}

KDvoid TexturedPolygon::setCoordInfo ( KDvoid )
{
	// Set texture coordinates for each vertex 

	CC_SAFE_FREE ( m_pCoords );	
	m_pCoords = (ccV2F_T2F*) kdMalloc ( sizeof ( ccV2F_T2F ) * m_pVertices->count ( ) );
	
	KDfloat		fAtlasWidth  = (KDfloat) this->getTexture ( )->getPixelsWide ( );
	KDfloat		fAtlasHeight = (KDfloat) this->getTexture ( )->getPixelsHigh ( );	

	for ( KDuint i = 0; i < m_pVertices->count ( ); i++ )
	{	
		CCPoint		tVert = ( (CCPointValue*) m_pVertices->objectAtIndex ( i ) )->getValue ( ); 

		m_pCoords [ i ].vertices.x = tVert.x;
		m_pCoords [ i ].vertices.y = tVert.y;
				
		m_pCoords [ i ].texCoords.u = ( m_pCoords [ i ].vertices.x + m_tRect.origin.x ) / fAtlasWidth;
		m_pCoords [ i ].texCoords.v = ( m_tContentSize.cy - m_pCoords [ i ].vertices.y + m_tRect.origin.y ) / fAtlasHeight;
	}
}