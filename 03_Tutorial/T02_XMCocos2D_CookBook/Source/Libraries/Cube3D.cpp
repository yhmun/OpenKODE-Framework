/* --------------------------------------------------------------------------
 *
 *      File            Cube3D.cpp
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
#include "Cube3D.h"

// Vertices for each side of the cube
static const GLfloat	l_aFrontVertices [ ] = 
{
	-0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
};

static const GLfloat	l_aBackVertices [ ] = 
{
	-0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
};

static const GLfloat	l_aLeftVertices [ ] =
{
	-0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
};

static const GLfloat	l_aRightVertices [ ] =
{
	0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
};

static const GLfloat	l_aTopVertices [ ] =
{
	-0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
};

static const GLfloat	l_aBottomVertices [ ] =
{
	-0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
};

// Coordinates for our texture to map it to a cube side
static const GLfloat	l_aTextureCoordinates [ ] =
{
	0, 0,
    1, 0,
    0, 1,
    1, 1,
};

Cube3D* Cube3D::create ( const KDchar* szFilePath )
{
	Cube3D*		pRet = new Cube3D ( );

	if ( pRet && pRet->initWithFile ( szFilePath ) )
	{
		pRet->autorelease ( );		
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}
 
Cube3D::Cube3D ( KDvoid )
{
	m_fRotation3DAngle = 0;
}

KDvoid Cube3D::draw ( KDvoid )
{
	CCDirector::sharedDirector()->setDepthTest ( KD_TRUE );  

	kmGLLoadIdentity ( );
	kmGLTranslatef ( m_tTranslation3D.x, m_tTranslation3D.y, m_tTranslation3D.z );
	kmGLRotatef ( m_fRotation3DAngle, m_tRotation3DAxis.x, m_tRotation3DAxis.y, m_tRotation3DAxis.z );

	CC_NODE_DRAW_SETUP ( );

	// Bind our texture if neccessary
	KDbool	bDrawTexture = m_pTexture != KD_NULL && m_bDrawTextured;
    if ( bDrawTexture )
    {
		ccDrawColor4F ( 1.0f, 1.0f, 1.0f, 1.0f );
        ccGLBindTexture2D ( m_pTexture->getName ( ) );
		ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosTex );
		ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, l_aTextureCoordinates );
    }
    else
    {
        ccGLBindTexture2D ( 0 );
		ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );
    }	

	if ( !bDrawTexture )
	{
		ccDrawColor4F ( 1.0f, 0.0f, 0.0f, 1.0f );
	}
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aFrontVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	if ( !bDrawTexture )
	{
		ccDrawColor4F ( 1.0f, 1.0f, 0.0f, 1.0f );
	}
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aBackVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	if ( !bDrawTexture )
	{
		ccDrawColor4F ( 1.0f, 0.0f, 0.0f, 1.0f );
	}
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aLeftVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

	if ( !bDrawTexture )
	{
		ccDrawColor4F ( 0.0f, 1.0f, 1.0f, 1.0f );
	}
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aRightVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	if ( !bDrawTexture )
	{
		ccDrawColor4F ( 0.0f, 1.0f, 0.0f, 1.0f );
	}
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aTopVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
	
	if ( !bDrawTexture )
	{
		ccDrawColor4F ( 0.0f, 0.0f, 1.0f, 1.0f );
	}
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, l_aBottomVertices );
	glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

	CC_INCREMENT_GL_DRAWS ( 1 );

	return;
}

const Vector3D& Cube3D::getTranslation3D ( KDvoid )
{
	return m_tTranslation3D;
}

KDvoid Cube3D::setTranslation3D ( const Vector3D& tVector )
{
	m_tTranslation3D = tVector;
}

const Vector3D& Cube3D::getRotation3DAxis ( KDvoid )
{
	return m_tRotation3DAxis;
}

KDvoid Cube3D::setRotation3DAxis ( const Vector3D& tVector )
{
	m_tRotation3DAxis = tVector;
}

KDfloat Cube3D::getRotation3DAngle ( KDvoid )
{
	return m_fRotation3DAngle;
}

KDvoid Cube3D::setRotation3DAngle ( KDfloat fAngle )
{
	m_fRotation3DAngle = fAngle;
}

KDbool Cube3D::isDrawTextured ( KDvoid )
{
	return m_bDrawTextured;
}

KDvoid Cube3D::setDrawTextured ( KDbool bDraw )
{
	m_bDrawTextured = bDraw;
}