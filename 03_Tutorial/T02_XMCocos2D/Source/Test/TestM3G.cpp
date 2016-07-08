/* --------------------------------------------------------------------------
 *
 *      File            TestM3G.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2011 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
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

#include "TestM3G.h"
#include "../TestRes.h"

static const KDchar*  l_szPath [ ] =
{
	"/res/m3g/BEFighter1.m3g", 
	"/res/m3g/nokia_on_ice.m3g", 
	"/res/m3g/pond_vilkutus2.m3g", 
	"/res/m3g/bcd.m3g", 
	"/res/m3g/abc09.m3g"
};

KDbool TestM3G::init ( KDvoid )
{	
	if ( TestBasic::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	m_vObjects = M3GLoader::load ( l_szPath [ s_nSubTestIndex ] );

	for ( KDuint  i = 0; i < m_vObjects.size ( ); i++ ) 
	{
		m_pWorld = dynamic_cast<M3GWorld*> ( m_vObjects [ i ] );
		if ( m_pWorld ) 
		{
			break;
		}
	}

	return KD_TRUE;
}

KDvoid TestM3G::onExit ( KDvoid )
{
	while ( !m_vObjects.empty ( ) )
    {
    	M3GObject3D*  pObject = m_vObjects.back ( );

    	delete pObject;

    	m_vObjects.pop_back ( );
    }

	m_pWorld = KD_NULL;

	TestBasic::onExit ( );
}

KDvoid TestM3G::draw ( KDvoid )
{
	CCDirector*  pDirector = CCDirector::sharedDirector ( );
	CCSize  tSize = pDirector->getWinSize ( );

	pDirector->setDepthTest ( KD_TRUE );

	if ( m_pWorld )
	{
		M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );

		KDfloat     fParams [ 4 ];
		M3GCamera*  pCamera = m_pWorld->getActiveCamera ( );
	    
		pCamera->getProjection ( fParams );
		pCamera->setPerspective ( fParams [ 0 ], tSize.cx / tSize.cy, fParams [ 2 ], fParams [ 3 ] );

		m_pWorld->animate ( KD_GET_UST2MSEC );
		pG3D->render ( m_pWorld );
	}

	pDirector->setGLDefaultValues ( );
}

KDuint TestM3G::count ( KDvoid )
{
	return 5;
}
