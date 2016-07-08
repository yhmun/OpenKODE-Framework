/* --------------------------------------------------------------------------
 *
 *      File            M3GGraphics3D.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com      
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

#include "M3GInternal.h"
#include <XMM3G/M3GGraphics3D.h>
#include <XMM3G/M3GWorld.h>
#include <XMM3G/M3GCamera.h>

static M3GGraphics3D*	l_pGraphics3D = KD_NULL;

M3GGraphics3D::M3GGraphics3D ( KDvoid )
{
	glEnable ( GL_DEPTH_TEST );
}

M3GGraphics3D::~M3GGraphics3D ( KDvoid )
{

}

M3GGraphics3D* M3GGraphics3D::getInstance ( KDvoid )
{
	if ( !l_pGraphics3D )
	{
		l_pGraphics3D = new M3GGraphics3D ( );

		kdAssert ( l_pGraphics3D );
	}

	return l_pGraphics3D;
}

KDint M3GGraphics3D::addLight ( M3GLight* light, const M3GTransform& transform )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
	return 0;
}

KDvoid M3GGraphics3D::bindTarget ( KDvoid* target )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, bindTarget ( ) is not implemented." );
}

KDvoid M3GGraphics3D::bindTarget ( KDvoid* target, KDbool depthBuffer, KDint hints )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, bindTarget ( ) is not implemented." );
}

KDvoid M3GGraphics3D::clear ( M3GBackground* background )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

M3GCamera* M3GGraphics3D::getCamera ( M3GTransform* transform )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
	return KD_NULL;
}

KDfloat M3GGraphics3D::getDepthRangeFar ( KDvoid ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
	return 0;
}

KDfloat	M3GGraphics3D::getDepthRangeNear ( KDvoid ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
	return 0;
}

KDint M3GGraphics3D::getHints ( KDvoid ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, getHints ( ) is not implemented." );
	return 0;
}

M3GLight* M3GGraphics3D::getLight ( KDint index, M3GTransform* transform ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
	return KD_NULL;
}

KDint M3GGraphics3D::getLightCount ( KDvoid ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
	return 0;
}

std::map<const KDchar*, KDint> M3GGraphics3D::getProperties ( KDvoid ) const
{
	return m_mProperties;
}

KDvoid*	M3GGraphics3D::getTarget ( KDvoid ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, getTarget ( ) is not implemented." );
	return KD_NULL;
}

KDint* M3GGraphics3D::getViewport ( KDvoid ) const
{
	static GLint  nViewport [ 4 ];

	glGetIntegerv ( GL_VIEWPORT, nViewport );

	return nViewport;
}

KDint M3GGraphics3D::getViewportX ( KDvoid ) const
{
	return this->getViewport ( ) [ 0 ];
}

KDint M3GGraphics3D::getViewportY ( KDvoid ) const
{
	return this->getViewport ( ) [ 1 ];
}

KDint M3GGraphics3D::getViewportWidth ( KDvoid ) const
{
	return this->getViewport ( ) [ 2 ];
}

KDint M3GGraphics3D::getViewportHeight ( KDvoid ) const
{
	return this->getViewport ( ) [ 3 ];
}

KDbool M3GGraphics3D::isDepthBufferEnabled ( KDvoid ) const
{
	GLboolean  bEnabled;

	glGetBooleanv ( GL_DEPTH_TEST, &bEnabled );

	return bEnabled;
}

KDvoid M3GGraphics3D::releaseTarget ( KDvoid )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Sorry, releaseTarget ( ) is not implemented." );
}

KDvoid M3GGraphics3D::render ( const M3GNode* node, const M3GTransform* transform ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::render ( const M3GVertexBuffer* vertices, const M3GIndexBuffer* triangles, const M3GAppearance* apperance, const M3GTransform* transform ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::render ( const M3GVertexBuffer* vertices, const M3GIndexBuffer* triangles, const M3GAppearance* apperance, const M3GTransform* transform, KDint scope ) const
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::render ( const M3GWorld* world ) const
{
    if ( world == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "World is NULL." );
    }

    M3GCamera*  pCamera = world->getActiveCamera ( );
    if ( pCamera == KD_NULL )
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "Active camera is NULL." );
    }

    M3GNode*  pParent = pCamera->getGlobalParent ( );
    if ( pParent != world )
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "Activated camera must be under the World." );
    }

    glEnable ( GL_NORMALIZE );

    glMatrixMode ( GL_TEXTURE );
    glLoadIdentity ( );
    
	glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ( );

    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ( );

	M3GRenderState  tState;

    // pass -1 for set up valid layers.
    // pass 0 for Background, Camera.
    // pass 1 for Lights.
    // pass 2 for Scene nodes.
    for ( KDint  i = -1; i < 3; i++ )
	{
		tState.m_nPass = i;
		world->render ( tState );
    }
}

KDvoid M3GGraphics3D::resetLights ( KDvoid )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::setCamera ( M3GCamera* camera, const M3GTransform& transform )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::setDepthRange ( KDfloat near, KDfloat far )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::setLight ( KDint index, M3GLight* light, const M3GTransform& transform )
{
	M3GException ( "NotImplementedException", __FUNCTION__, "Immediate mode is obsoleted, Please use retained mode." );
}

KDvoid M3GGraphics3D::setViewport ( KDint x, KDint y, KDint width, KDint height )
{
	if ( width <= 0 && height <= 0 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Size is invalid, w = %d, h = %d.", width, height );
	}

	glViewport ( x, y, width, height );
}