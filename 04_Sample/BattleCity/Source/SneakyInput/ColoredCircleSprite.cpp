/* --------------------------------------------------------------------------
 *
 *      File            ColoredCircleSprite.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      CJ Hanson on 2010/02/17
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Hanson Interactive. All rights reserved.
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
#include "ColoredCircleSprite.h"

NS_CC_BEGIN

ColoredCircleSprite* ColoredCircleSprite::create ( const ccColor4B& tColor, KDfloat fRadius )
{
	ColoredCircleSprite*	pRet = new ColoredCircleSprite ( );

	if ( pRet && pRet->initWithColor ( tColor, fRadius ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

ColoredCircleSprite::ColoredCircleSprite ( KDvoid )
{
	m_pCircleVertices = KD_NULL;
}

ColoredCircleSprite::~ColoredCircleSprite ( KDvoid )
{
	CC_SAFE_FREE ( m_pCircleVertices );
}

KDbool ColoredCircleSprite::init ( KDvoid )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_uNumberOfSegments	= 36;
		
	// default blend function
	m_tBlendFunc.src = CC_BLEND_SRC;
	m_tBlendFunc.dst = CC_BLEND_DST;
		
	m_tColor.r = 0;
	m_tColor.g = 0;
	m_tColor.b = 0;
	m_cOpacity = 255;
		
	m_pCircleVertices = (GLfloat*) kdCalloc ( 1, sizeof ( GLfloat ) * 2 * ( m_uNumberOfSegments ) );
	if ( !m_pCircleVertices )
	{
		return KD_FALSE;
	}
	
	this->setRadius ( 10.f );

	setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionColor ) );

	return KD_TRUE;
}

KDbool ColoredCircleSprite::initWithColor ( const ccColor4B& tColor, KDfloat fRadius )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_uNumberOfSegments	= 36;
		
	// default blend function
	m_tBlendFunc.src = CC_BLEND_SRC;
	m_tBlendFunc.dst = CC_BLEND_DST;	
		
	m_tColor.r = tColor.r;
	m_tColor.g = tColor.g;
	m_tColor.b = tColor.b;
	m_cOpacity = tColor.a;

	m_pCircleVertices = (GLfloat*) kdCalloc ( 1, sizeof ( GLfloat ) * 2 * ( m_uNumberOfSegments ) );
	if ( !m_pCircleVertices )
	{
		return KD_FALSE;
	}

	this->setRadius ( fRadius );

	setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

	return KD_TRUE;
}

KDvoid ColoredCircleSprite::setContentSize ( const CCSize& tContentSize )
{
	CCNode::setContentSize ( tContentSize );

	this->setRadius	( tContentSize.cx / 2 );
}

KDbool ColoredCircleSprite::containsPoint ( const CCPoint& tPoint )
{
	KDfloat		fSq = tPoint.x * tPoint.x + tPoint.y * tPoint.y;
	KDfloat		rSq = m_fRadius * m_fRadius;
	return ( fSq <= rSq );
}

KDfloat ColoredCircleSprite::getRadius ( KDvoid )
{
	return m_fRadius;
}

KDvoid ColoredCircleSprite::setRadius ( KDfloat fRadius )
{
	const KDfloat fThetaInc	= 2.0f * 3.14159265359f / m_uNumberOfSegments;
	KDfloat		  fTheta = 0.0f;
	
	m_fRadius = fRadius;

	for ( KDuint i = 0; i < m_uNumberOfSegments; i++ )
	{
		KDfloat		j = fRadius * kdCosf ( fTheta ) + m_tPosition.x;
		KDfloat		k = fRadius * kdSinf ( fTheta ) + m_tPosition.y;
		
		m_pCircleVertices [ i * 2     ]	= j;
		m_pCircleVertices [ i * 2 + 1 ]	= k;
		
		fTheta += fThetaInc;
	}
	
	this->updateContentSize ( );
}

KDbool ColoredCircleSprite::isOpacityModifyRGB ( KDvoid )
{
	return KD_FALSE;
}

KDvoid ColoredCircleSprite::setOpacityModifyRGB ( KDbool bIsOpacityModifyRGB )
{
	
}

GLubyte ColoredCircleSprite::getOpacity ( KDvoid )
{
	return m_cOpacity;
}

KDvoid ColoredCircleSprite::setOpacity ( GLubyte cOpacity )
{
	m_cOpacity = cOpacity;
	this->updateColor ( );
}

GLubyte ColoredCircleSprite::getDisplayedOpacity ( KDvoid )
{
	return m_cOpacity;
}

KDvoid ColoredCircleSprite::updateDisplayedOpacity ( GLubyte cParentOpacity )
{

}

KDbool ColoredCircleSprite::isCascadeOpacityEnabled ( KDvoid )
{
	return KD_FALSE;
}

KDvoid ColoredCircleSprite::setCascadeOpacityEnabled ( KDbool bCascadeOpacityEnabled )
{

}

const ccColor3B& ColoredCircleSprite::getColor ( KDvoid )
{
	return m_tColor;
}

const ccColor3B& ColoredCircleSprite::getDisplayedColor ( KDvoid )
{
	return m_tColor;
}		

KDvoid ColoredCircleSprite::setColor ( const ccColor3B& tColor )
{
	m_tColor = tColor;
}

KDvoid ColoredCircleSprite::updateDisplayedColor ( const ccColor3B& tParentColor )
{

}

KDbool ColoredCircleSprite::isCascadeColorEnabled ( KDvoid )
{
	return KD_FALSE;
}

KDvoid ColoredCircleSprite::setCascadeColorEnabled ( KDbool bCascadeColorEnabled )
{

}

KDvoid ColoredCircleSprite::setBlendFunc ( ccBlendFunc tBlendFunc )
{
	
}

ccBlendFunc ColoredCircleSprite::getBlendFunc ( KDvoid )
{
	return m_tBlendFunc;
}

KDvoid ColoredCircleSprite::updateContentSize ( KDvoid )
{
	this->setContentSize ( ccs ( m_fRadius * 2 , m_fRadius * 2 ) );
}

KDvoid ColoredCircleSprite::updateColor ( KDvoid )
{

}

const KDchar* ColoredCircleSprite::description ( KDvoid )
{
	return ccszf ( "<ColoredCircleSprite = %08X | Tag = %i | Color = %02X%02X%02X%02X | Radius = %1.2f>", this, m_nTag, m_tColor.r, m_tColor.g, m_tColor.b, m_cOpacity, m_fRadius );
}

KDvoid ColoredCircleSprite::draw ( KDvoid )
{
	CC_NODE_DRAW_SETUP ( );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

	ccGLBlendFunc ( m_tBlendFunc.src, m_tBlendFunc.dst );

	ccDrawColor4F ( m_tColor.r / 255.f, m_tColor.g / 255.f, m_tColor.b / 255.f, m_cOpacity / 255.f );
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pCircleVertices );
    
	glDrawArrays ( GL_TRIANGLE_FAN, 0, (GLsizei) m_uNumberOfSegments );

	CC_INCREMENT_GL_DRAWS ( 1 );
}

/*
#define GL_COLOR_ARRAY                    0x8076
#define GL_TEXTURE_COORD_ARRAY            0x8078
- (void)draw
{		
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_VERTEX_ARRAY
	// Unneeded states: GL_COLOR_ARRAY, GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
	
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
    glDisable(GL_TEXTURE_2D);
	
	glVertexPointer(2, GL_FLOAT, 0, circleVertices_);
	glColor4f(color_.r/255.0f, color_.g/255.0f, color_.b/255.0f, opacity_/255.0f);
	
	BOOL newBlend = NO;
	if( blendFunc_.src != CC_BLEND_SRC || blendFunc_.dst != CC_BLEND_DST ) {
		newBlend = YES;
		glBlendFunc(blendFunc_.src, blendFunc_.dst);
	}else if( opacity_ == 255 ) {
		newBlend = YES;
		glBlendFunc(GL_ONE, GL_ZERO);
	}else{
		newBlend = YES;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfSegments);
	
	if( newBlend )
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	
	// restore default GL state
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
    glEnable(GL_TEXTURE_2D);
}
*/

NS_CC_END