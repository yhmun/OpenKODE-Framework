/* --------------------------------------------------------------------------
 *
 *      File            M3GAppearance.cpp
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
#include <XMM3G/M3GAppearance.h>
#include <XMM3G/M3GPolygonMode.h>
#include <XMM3G/M3GCompositingMode.h>
#include <XMM3G/M3GMaterial.h>
#include <XMM3G/M3GTexture2D.h>
#include <XMM3G/M3GFog.h>

M3GAppearance::M3GAppearance ( KDvoid )
: m_nLayer ( 0 )
, m_pPolygonMode ( KD_NULL )
, m_pCompositingMode ( KD_NULL )
, m_pMaterial ( KD_NULL )
, m_pFog ( KD_NULL )
{
	for ( KDint  i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
		m_pTextures [ i ] = KD_NULL;
	}
}

M3GAppearance::~M3GAppearance ( KDvoid )
{

}

KDint M3GAppearance::animate ( KDint time )
{
	M3GObject3D::animate ( time );

	if ( m_pPolygonMode ) 
	{
		m_pPolygonMode->animate ( time );
	}

	if ( m_pCompositingMode ) 
	{
		m_pCompositingMode->animate ( time );
	}

	for ( KDint i = 0; i < MAX_TEXTURE_UNITS; i++ ) 
	{
		if ( m_pTextures [ i ] ) 
		{
			m_pTextures [ i ]->animate ( time );
		}
	}

	if ( m_pMaterial ) 
	{
		m_pMaterial->animate ( time );
	}

	if ( m_pFog )
	{
		m_pFog->animate ( time );
	}

	return 0;
}

M3GObject3D* M3GAppearance::duplicate ( KDvoid ) const
{
	M3GAppearance*  pObj = new M3GAppearance ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GAppearance::_duplicate ( M3GAppearance* pObj ) const
{
	M3GObject3D::_duplicate ( pObj );

	pObj->m_nLayer			 = m_nLayer;
	pObj->m_pPolygonMode	 = m_pPolygonMode;
	pObj->m_pCompositingMode = m_pCompositingMode;
	pObj->m_pMaterial		 = m_pMaterial;
	pObj->m_pFog			 = m_pFog;

	for ( KDint  i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
		pObj->m_pTextures [ i ] = m_pTextures [ i ];
	}
}

KDint M3GAppearance::getReferences ( M3GObject3D** references ) const
{
    KDint  nNumber = M3GObject3D::getReferences ( references );
	KDint  i;

    if ( m_pPolygonMode )
	{
		if ( references )
		{
			references [ nNumber ] = m_pPolygonMode;
		}

		nNumber++;
	}

    if ( m_pCompositingMode )
	{
		if ( references )
		{
			references [ nNumber ] = m_pCompositingMode;
		}

		nNumber++;
	}

    if ( m_pMaterial )
	{
		if ( references )
		{
			references [ nNumber ] = m_pMaterial;
		}

		nNumber++;
	}

    for ( i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
        if ( m_pTextures [ i ] )
		{
			if ( references )
			{
				references [ nNumber ] = m_pTextures [ i ];
			}
 
			nNumber++;
		}
    }

    if ( m_pFog )
	{
		if ( references )
		{
			references [ nNumber ] = m_pFog;
		}

		nNumber++;
	}

    return nNumber;
}

M3GCompositingMode* M3GAppearance::getCompositingMode ( KDvoid ) const
{
	return m_pCompositingMode;
}

M3GFog* M3GAppearance::getFog ( KDvoid ) const
{
	return m_pFog;
}

KDint M3GAppearance::getLayer ( KDvoid ) const
{
	return m_nLayer;
}

M3GMaterial* M3GAppearance::getMaterial ( KDvoid ) const
{
	return m_pMaterial;
}

M3GPolygonMode* M3GAppearance::getPolygonMode ( KDvoid ) const
{
	return m_pPolygonMode;
}

M3GTexture2D* M3GAppearance::getTexture ( KDint index ) const
{
    if ( index < 0 || index >= MAX_TEXTURE_UNITS )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Texture index is inalid, index = %d", index );
    }

	return m_pTextures [ index ];
}

KDvoid M3GAppearance::setCompositingMode ( M3GCompositingMode* compositingMode )
{
	m_pCompositingMode = compositingMode;
}

KDvoid M3GAppearance::setFog ( M3GFog* fog )
{
    if ( fog == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Null fog is specified." );
    }

    m_pFog = fog;
}

KDvoid M3GAppearance::setLayer ( KDint layer )
{
    if ( layer < -63 || layer > 64 )
	{
		M3GException ( "IllegalArgumentException", __FUNCTION__, "Lyaer is invalid, layer = %d.", layer );
    }

    m_nLayer = layer;
}

KDvoid M3GAppearance::setMaterial ( M3GMaterial* material )
{
	m_pMaterial = material;
}

KDvoid M3GAppearance::setPolygonMode ( M3GPolygonMode* polygonMode )
{
	m_pPolygonMode = polygonMode;
}

KDvoid M3GAppearance::setTexture ( KDint index, M3GTexture2D* texture )
{
    if ( index < 0 || index >= MAX_TEXTURE_UNITS )
	{
		M3GException ( "IndexOutOfBoundsException", __FUNCTION__, "Texture index is inalid, index = %d in [ 0, %d ]", index, MAX_TEXTURE_UNITS );
    }

    m_pTextures [ index ] = texture;
}

KDvoid M3GAppearance::render ( KDvoid )
{
    M3GPolygonMode    ::render ( );
    M3GCompositingMode::render ( );
    M3GMaterial       ::render ( );
    M3GTexture2D      ::render ( );
    M3GFog            ::render ( );
}

KDvoid M3GAppearance::render ( M3GRenderState& state ) const
{
	if ( state.m_nPass != 2 ) 
	{
		return;
	}

	if ( m_pFog )
	{
		m_pFog->render ( state );
	} 
	else
	{
		M3GFog::render ( );
	}

	if ( m_pMaterial )
	{
		m_pMaterial->render ( state );
	} 
	else 
	{
		M3GMaterial::render ( );
	}

	if ( m_pCompositingMode )
	{
		m_pCompositingMode->render ( state );
	} 
	else 
	{
		M3GCompositingMode::render ( );
	}

	if ( m_pPolygonMode )
	{
		m_pPolygonMode->render ( state );
	} 
	else
	{
		M3GPolygonMode::render ( );
	}

	glMatrixMode ( GL_TEXTURE );

	glEnable ( GL_TEXTURE_2D );

	for ( KDint  i = 0; i < MAX_TEXTURE_UNITS; i++ )
	{
		if ( m_pTextures [ i ] )
		{
			glActiveTexture ( GL_TEXTURE0 + i );             
			glEnable		( GL_TEXTURE_2D );           
			m_pTextures [ i ]->render ( state );
		}
		else 
		{
			glActiveTexture ( GL_TEXTURE0 + i );               
			glDisable		( GL_TEXTURE_2D);            
			M3GTexture2D::render ( );
		}
	}

	glMatrixMode ( GL_MODELVIEW );
}

KDint M3GAppearance::getLayer2 ( KDvoid ) const
{
    KDint  nLayer2 = m_nLayer * 2;

    if ( m_pCompositingMode && m_pCompositingMode->getBlending ( ) != M3GCompositingMode::REPLACE ) 
	{
        nLayer2 += 1;
    }

    return nLayer2;
}