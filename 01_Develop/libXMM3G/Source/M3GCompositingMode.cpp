/* --------------------------------------------------------------------------
 *
 *      File            M3GCompositingMode.cpp
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
#include <XMM3G/M3GCompositingMode.h>

M3GCompositingMode::M3GCompositingMode ( KDvoid )
: m_nBlendingMode ( REPLACE )
, m_fAlphaThreshold ( 0 )
, m_fOffsetFactor ( 0 )
, m_fOffsetUnits ( 0 )
, m_bDepthTest ( KD_TRUE )
, m_bDepthWrite ( KD_TRUE )
, m_bColorWrite ( KD_TRUE )
, m_bAlphaWrite ( KD_TRUE )
{

}

M3GCompositingMode::~M3GCompositingMode ( KDvoid )
{

}

M3GObject3D* M3GCompositingMode::duplicate ( KDvoid ) const
{
    M3GCompositingMode*  pObj = new M3GCompositingMode ( );
	
	this->_duplicate ( pObj );

    return pObj;
}

KDvoid M3GCompositingMode::_duplicate ( M3GCompositingMode* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_nBlendingMode	= m_nBlendingMode;
	pObj->m_fAlphaThreshold = m_fAlphaThreshold;
	pObj->m_fOffsetFactor	= m_fOffsetFactor;
	pObj->m_fOffsetUnits	= m_fOffsetUnits;
	pObj->m_bDepthTest		= m_bDepthTest;
	pObj->m_bDepthWrite		= m_bDepthWrite;
	pObj->m_bColorWrite		= m_bColorWrite;
	pObj->m_bAlphaWrite		= m_bAlphaWrite;
}

KDfloat M3GCompositingMode::getAlphaThreshold ( KDvoid ) const
{
	return m_fAlphaThreshold;
}

KDint M3GCompositingMode::getBlending ( KDvoid ) const
{
	return m_nBlendingMode;
}

KDfloat M3GCompositingMode::getDepthOffsetFactor ( KDvoid ) const
{
	return m_fOffsetFactor;
}

KDfloat M3GCompositingMode::getDepthOffsetUnits ( KDvoid ) const
{
	return m_fOffsetUnits;
}

KDbool M3GCompositingMode::isAlphaWriteEnabled ( KDvoid ) const
{
	return m_bAlphaWrite;
}

KDbool M3GCompositingMode::isColorWriteEnabled ( KDvoid ) const
{
	return m_bColorWrite;
}

KDbool M3GCompositingMode::isDepthTestEnabled ( KDvoid ) const
{
	return m_bDepthTest;
}

KDbool M3GCompositingMode::isDepthWriteEnabled ( KDvoid ) const
{
	return m_bDepthWrite;
}

KDvoid M3GCompositingMode::setAlphaThreshold ( KDfloat threshold )
{
    if ( threshold < 0 || threshold > 1 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Alpha threashold is invalid, threshold = %f.", threshold );
    }

    m_fAlphaThreshold = threshold;
}

KDvoid M3GCompositingMode::setAlphaWriteEnable ( KDbool enable )
{
	m_bAlphaWrite = enable;
}

KDvoid M3GCompositingMode::setBlending ( KDint mode )
{
	if ( mode < ALPHA || mode > REPLACE ) 
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Blending mode is invalid, mode = %d.", mode );
    }

    m_nBlendingMode = mode;
}

KDvoid M3GCompositingMode::setColorWriteEnable ( KDbool enable )
{
	m_bColorWrite = enable;
}

KDvoid M3GCompositingMode::setDepthOffset ( KDfloat factor, KDfloat units )
{
	m_fOffsetFactor = factor;
	m_fOffsetUnits  = units;
}

KDvoid M3GCompositingMode::setDepthTestEnable ( KDbool enable )
{
	m_bDepthTest = enable;
}

KDvoid M3GCompositingMode::setDepthWriteEnable ( KDbool enable )
{
	m_bDepthWrite = enable;
}

KDvoid M3GCompositingMode::render ( KDvoid )
{
    glDepthFunc ( GL_LESS );
    glDepthMask ( GL_TRUE );
    glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glBlendFunc ( GL_ONE, GL_ZERO );
    glDisable   ( GL_ALPHA_TEST );
    glDisable   ( GL_POLYGON_OFFSET_FILL );
}

KDvoid M3GCompositingMode::render ( M3GRenderState& state ) const
{
    if ( state.m_nPass != 2 )
	{
        return;
    }

    if ( this == KD_NULL )
	{
        render ( );
        return;
    }

    switch ( m_bDepthTest )
	{
		case KD_TRUE  : glDepthFunc ( GL_LESS );	break;
		case KD_FALSE : glDepthFunc ( GL_ALWAYS );	break;
    }

    glDepthMask ( m_bDepthWrite );
    glColorMask ( m_bColorWrite, m_bColorWrite, m_bColorWrite, m_bAlphaWrite );

    glEnable ( GL_BLEND );
    switch ( m_nBlendingMode )
	{
		case REPLACE     : glBlendFunc ( GL_ONE		 , GL_ZERO				 ); break;
		case ALPHA_ADD   : glBlendFunc ( GL_SRC_ALPHA, GL_ONE				 ); break;
		case ALPHA       : glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
		case MODULATE    : glBlendFunc ( GL_DST_COLOR, GL_ZERO				 ); break;
		case MODULATE_X2 : glBlendFunc ( GL_DST_COLOR, GL_SRC_COLOR			 ); break;

		default : 

			M3GException ( "InternalException", __FUNCTION__, "Blending mode is invalid, mode = %d.", m_nBlendingMode );
    }

    if ( m_fAlphaThreshold > 0 )
	{
        glEnable    ( GL_ALPHA_TEST );
        glAlphaFunc ( GL_GEQUAL, m_fAlphaThreshold );      
    }
	else 
	{
        glDisable ( GL_ALPHA_TEST );
    }

    if ( m_fOffsetFactor || m_fOffsetUnits )
	{
        glEnable ( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset ( m_fOffsetFactor, m_fOffsetUnits );
    } 
	else 
	{
        glDisable ( GL_POLYGON_OFFSET_FILL );
    }
}