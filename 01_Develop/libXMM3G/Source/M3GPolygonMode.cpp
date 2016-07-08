/* --------------------------------------------------------------------------
 *
 *      File            M3GPolygonMode.cpp
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
#include <XMM3G/M3GPolygonMode.h>

M3GPolygonMode::M3GPolygonMode ( KDvoid )
: m_nCulling ( CULL_BACK )
, m_nWinding ( WINDING_CCW )
, m_nShading ( SHADE_SMOOTH )
, m_bTwoSidedLighting ( KD_FALSE )
, m_bLocalCameraLighting ( KD_TRUE )
, m_bPerspectiveCorrection ( KD_TRUE )
{

}

M3GPolygonMode::~M3GPolygonMode ( KDvoid )
{

}

M3GObject3D* M3GPolygonMode::duplicate ( KDvoid ) const
{
	M3GPolygonMode*  pObj = new M3GPolygonMode ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GPolygonMode::_duplicate ( M3GPolygonMode* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_nCulling = m_nCulling;
	pObj->m_nWinding = m_nWinding;
	pObj->m_nShading = m_nShading;

	pObj->m_bTwoSidedLighting		= m_bTwoSidedLighting;
	pObj->m_bLocalCameraLighting	= m_bLocalCameraLighting;
	pObj->m_bPerspectiveCorrection	= m_bPerspectiveCorrection;
}

KDint M3GPolygonMode::getCulling ( KDvoid ) const
{
	return m_nCulling;
}

KDint M3GPolygonMode::getShading ( KDvoid ) const
{
	return m_nShading;
}

KDint M3GPolygonMode::getWinding ( KDvoid ) const
{
	return m_nWinding;
}

KDbool M3GPolygonMode::isLocalCameraLightingEnabled ( KDvoid ) const
{
	return m_bLocalCameraLighting;
}

KDbool M3GPolygonMode::isPerspectiveCorrectionEnabled ( KDvoid ) const
{
	return m_bPerspectiveCorrection;
}

KDbool M3GPolygonMode::isTwoSidedLightingEnabled ( KDvoid ) const
{
	return m_bTwoSidedLighting;
}

KDvoid M3GPolygonMode::setCulling ( KDint mode )
{
    if ( mode != CULL_BACK && mode != CULL_FRONT && mode != CULL_NONE ) 
	{
        M3GException ( "IllegalArgumentException", __FUNCTION__, "Mode is invalid, mode = %d.", mode );
    }

    m_nCulling = mode;
}

KDvoid M3GPolygonMode::setLocalCameraLightingEnable ( KDbool enable )
{
	m_bLocalCameraLighting = enable;
}

KDvoid M3GPolygonMode::setPerspectiveCorrectionEnable ( KDbool enable )
{
	m_bPerspectiveCorrection = enable;
}

KDvoid M3GPolygonMode::setShading ( KDint mode )
{
	if ( mode != SHADE_FLAT && mode != SHADE_SMOOTH ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Mode is invalid, mode = %d.", mode );
    }

    m_nShading = mode;
}

KDvoid M3GPolygonMode::setTwoSidedLightingEnable ( KDbool enable )
{
	m_bTwoSidedLighting = enable;
}

KDvoid M3GPolygonMode::setWinding ( KDint mode )
{
	if ( mode != WINDING_CCW && mode != WINDING_CW ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Mode is invalid, mode = %d.", mode );
    }

    m_nWinding = mode;
}

KDvoid M3GPolygonMode::render ( KDvoid )
{
    glFrontFace   ( GL_CCW );
    glCullFace    ( GL_BACK );
    glEnable      ( GL_CULL_FACE );
    glShadeModel  ( GL_SMOOTH );
    glLightModelf ( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glHint        ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	/* Not support OpenGL ES
	glLightModelf ( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
	*/
}

KDvoid M3GPolygonMode::render ( M3GRenderState& state ) const
{
    if ( state.m_nPass != 2 )
	{
        return;
    }

    switch ( m_nWinding )
	{
		case WINDING_CCW : glFrontFace ( GL_CCW ); break;
		case WINDING_CW  : glFrontFace ( GL_CW  ); break;
	
		default : 			
			M3GException ( "InternalException", __FUNCTION__, "Winding mode is invalid, mode = %d.", m_nWinding );
    }

    switch ( m_nCulling )
	{
		case CULL_FRONT : glEnable  ( GL_CULL_FACE ); glCullFace ( GL_FRONT ); break;
		case CULL_BACK  : glEnable  ( GL_CULL_FACE ); glCullFace ( GL_BACK  ); break;
		case CULL_NONE  : glDisable ( GL_CULL_FACE ); break;

		default :
			M3GException ( "InternalException", __FUNCTION__, "Culling mode is invalid, mode = %d.", m_nCulling );
    }

    switch ( m_bTwoSidedLighting )
	{
		case KD_TRUE  : glLightModelf ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE  ); break;
		case KD_FALSE : glLightModelf ( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE ); break;
		
		default :
			M3GException ( "InternalException", __FUNCTION__, "Two sided lighting is invalid, mode = %d.", m_bTwoSidedLighting );
    }

    switch ( m_nShading )
	{
		case SHADE_FLAT   : glShadeModel ( GL_FLAT   ); break;
		case SHADE_SMOOTH : glShadeModel ( GL_SMOOTH ); break;

		default : 
			M3GException ( "InternalException", __FUNCTION__, "Shading mode is invalid, mode = %d.", m_nShading );
    }

	/* Not support OpenGL ES
    switch ( m_bLocalCameraLighting )
	{
		case KD_TRUE  : glLightModelf ( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE  ); break;
		case KD_FALSE : glLightModelf ( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE ); break;

		default : 
			M3GException ( "InternalException", __FUNCTION__, "Local camera lighting is invalid, mode = %d.", m_bLocalCameraLighting );
    }
	*/

    switch ( m_bPerspectiveCorrection )
	{
		case KD_TRUE  : glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST  ); break;
		case KD_FALSE : glHint ( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST ); break;

		default : 
			M3GException ( "InternalException", __FUNCTION__, "Perspective correction is invalid, mode = %d.", m_bPerspectiveCorrection );
    }
}