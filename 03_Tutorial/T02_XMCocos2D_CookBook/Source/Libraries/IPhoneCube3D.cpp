/* --------------------------------------------------------------------------
 *
 *      File            IPhoneCube3D.cpp
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
#include "IPhoneCube3D.h"

// Vertices for each side of the cube
static const KDfloat	l_w = 0.5f;
static const KDfloat	l_h = 1.0f;
static const KDfloat	l_d = 0.1f;

static const GLfloat	l_aFrontVertices [ ] = 
{
	-l_w, -l_h,  l_d,
     l_w, -l_h,  l_d,
    -l_w,  l_h,  l_d,
     l_w,  l_h,  l_d,
};

static const GLfloat	l_aBackVertices [ ] = 
{
	-l_w, -l_h, -l_d,
    -l_w,  l_h, -l_d,
     l_w, -l_h, -l_d,
     l_w,  l_h, -l_d,
};

static const GLfloat	l_aLeftVertices [ ] =
{
	-l_w, -l_h,  l_d,
    -l_w,  l_h,  l_d,
    -l_w, -l_h, -l_d,
    -l_w,  l_h, -l_d,
};

static const GLfloat	l_aRightVertices [ ] =
{
	l_w, -l_h, -l_d,
    l_w,  l_h, -l_d,
    l_w, -l_h,  l_d,
    l_w,  l_h,  l_d,
};

static const GLfloat	l_aTopVertices [ ] =
{
	-l_w,  l_h,  l_d,
     l_w,  l_h,  l_d,
    -l_w,  l_h, -l_d,
     l_w,  l_h, -l_d,
};

static const GLfloat	l_aBottomVertices [ ] =
{
	-l_w, -l_h,  l_d,
    -l_w, -l_h, -l_d,
     l_w, -l_h,  l_d,
     l_w, -l_h, -l_d,
};

// Coordinates for our texture to map it to a cube side
static const GLfloat	l_aTextureCoordinates [ ] =
{
	0, 0,
    1, 0,
    0, 1,
    1, 1,
};

IPhoneCube3D* IPhoneCube3D::create ( KDvoid )
{
	IPhoneCube3D*	pRet = new IPhoneCube3D ( );

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

KDbool IPhoneCube3D::init ( KDvoid )
{
	if ( !Cube3D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pFront = CCTextureCache::sharedTextureCache ( )->addImage ( "iphone4_front.jpg" );
	m_pBack  = CCTextureCache::sharedTextureCache ( )->addImage ( "iphone4_back.jpg" );
	m_pSide  = CCTextureCache::sharedTextureCache ( )->addImage ( "iphone4_side.jpg" );		

	return KD_TRUE;
}
 
KDvoid IPhoneCube3D::draw ( KDvoid )
{
	CCDirector::sharedDirector()->setDepthTest ( KD_TRUE );  

	kmGLLoadIdentity ( );
	kmGLTranslatef ( m_tTranslation3D.x, m_tTranslation3D.y, m_tTranslation3D.z );
//	kmGLRotatef ( m_fRotation3DAngle, m_tRotation3DAxis.x, m_tRotation3DAxis.y, m_tRotation3DAxis.z );

	kmGLRotatef ( m_tRotation3DAxis.x, 1, 0, 0 );
	kmGLRotatef ( m_tRotation3DAxis.y, 0, 1, 0 );
	kmGLRotatef ( m_tRotation3DAxis.z, 0, 0, 1 );

	CC_NODE_DRAW_SETUP ( );

	// Bind our texture if neccessary
	ccDrawColor4F ( 1.0f, 1.0f, 1.0f, 1.0f );
	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosTex );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, l_aTextureCoordinates );
	        
	ccGLBindTexture2D ( m_pFront->getName ( ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aFrontVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	ccGLBindTexture2D ( m_pBack->getName ( ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aBackVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	ccGLBindTexture2D ( m_pSide->getName ( ) );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aLeftVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aRightVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aTopVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aBottomVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

	CC_INCREMENT_GL_DRAWS ( 1 );

	return;
}

