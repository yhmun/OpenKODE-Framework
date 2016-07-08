/* --------------------------------------------------------------------------
 *
 *      File            M3GWorld.cpp 
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
#include <XMM3G/M3GWorld.h>
#include <XMM3G/M3GBackground.h>
#include <XMM3G/M3GCamera.h>
#include <algorithm>

M3GWorld::M3GWorld ( KDvoid )
: m_pBackground ( KD_NULL )
, m_pActiveCamera ( KD_NULL )
{

}

M3GWorld::~M3GWorld ( KDvoid )
{

}

KDint M3GWorld::animate ( KDint time )
{
	M3GGroup::animate ( time );

	if ( m_pBackground ) 
	{
		m_pBackground->animate ( time );
	}

	return 0;
}

M3GObject3D* M3GWorld::duplicate ( KDvoid ) const
{
	M3GWorld*  pObj = new M3GWorld ( );

	this->_duplicate ( pObj );

	return pObj;
}

KDvoid M3GWorld::_duplicate ( M3GWorld* pObj ) const
{
	M3GGroup::_duplicate ( pObj );

	pObj->m_pBackground   = m_pBackground;
	pObj->m_pActiveCamera = m_pActiveCamera;
}

KDint M3GWorld::getReferences ( M3GObject3D** references ) const
{
	KDint  nNumber = M3GGroup::getReferences ( references );

    if ( m_pBackground )
	{
		if ( references )
		{
			references [ nNumber ] = m_pBackground;
		}

		nNumber++;
	}

    if ( m_pActiveCamera )
	{
		if ( references )
		{
			references [ nNumber ] = m_pActiveCamera;
		}

		nNumber++;
	}

	return nNumber;
}

M3GCamera* M3GWorld::getActiveCamera ( KDvoid ) const
{
	return m_pActiveCamera;
}

M3GBackground* M3GWorld::getBackground ( KDvoid ) const
{
	return m_pBackground;
}

KDvoid M3GWorld::setActiveCamera ( M3GCamera* camera )
{
    if ( camera == KD_NULL )
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Camera is NULL." );
    }

    m_pActiveCamera = camera;
}

KDvoid M3GWorld::setBackground ( M3GBackground* background )
{
    if ( background == KD_NULL ) 
	{
		M3GException ( "NullPointerException", __FUNCTION__, "Background is NULL." );
    }

    m_pBackground = background;
}

KDvoid M3GWorld::render ( M3GRenderState& state ) const
{
    if ( m_pActiveCamera == KD_NULL )
	{
		M3GException ( "IllegalStateException", __FUNCTION__, "Active camera is NULL" );
    }
    
	state.m_pCamera = m_pActiveCamera;

	std::vector<KDint>&  v = state.m_vLayers;

	switch ( state.m_nPass )
	{
		case -1 :
	        
			M3GGroup::render ( state );

			std::sort ( v.begin ( ), v.end ( ) );
			v.erase ( std::unique ( v.begin ( ), v.end ( ) ), v.end ( ) );
			
			break;

		case 0 : 
	       
			if ( m_pBackground )
			{
				m_pBackground->render ( state );
			} 
			else 
			{
				M3GBackground::render ( );
			}

			m_pActiveCamera->render ( state );
			
			break;
    
		case 1 :
	       
			state.m_nLightIndex = 0;

			M3GGroup::render ( state );
			
			break;

		case 2 :
	        
			for ( KDuint  i = 0; i < state.m_vLayers.size ( ); i++ ) 
			{
				state.m_nLayer = state.m_vLayers [ i ];

				M3GGroup::render ( state );
			}

			break;

		default :

			M3GException ( "IllegalStateException", __FUNCTION__, "Render pass is invalid, pass = %d.L", state.m_nPass );
	}
}