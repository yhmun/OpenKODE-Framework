/* --------------------------------------------------------------------------
 *
 *      File            Ch1_3DCubes.cpp
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
#include "Ch1_3DCubes.h"
#include "Libraries/Cube3D.h"

KDbool Ch1_3DCubes::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Load a textured cube and set initial variables
	m_pCube3d1 = Cube3D::create ( "crate.jpg" );
	m_pCube3d1->setPosition ( ccp ( 100, 100 ) );
	m_pCube3d1->setTranslation3D ( Vector3D ( 2.0f, 0.0f, -4.0f ) );
	m_pCube3d1->setRotation3DAxis ( Vector3D ( 2.0f, 2.0f, 4.0f ) );
	m_pCube3d1->setRotation3DAngle ( 0.0f );
	m_pCube3d1->setDrawTextured ( KD_TRUE );
	this->addChild ( m_pCube3d1, 3, 0 );
	
	// Load a colored cube and set initial variables
	m_pCube3d2 = Cube3D::create ( "blank.png" ); 
	m_pCube3d2->setTranslation3D ( Vector3D ( -2.0f, 0.0f, -4.0f ) );
	m_pCube3d2->setRotation3DAxis ( Vector3D ( 2.0f, 2.0f, 4.0f ) );
	m_pCube3d2->setRotation3DAngle ( 0.0f );
	m_pCube3d2->setDrawTextured ( KD_FALSE );
	this->addChild ( m_pCube3d2, 1, 1 );
	
	// Schedule cube rotation
	this->schedule ( schedule_selector ( Ch1_3DCubes::step ) );

	return KD_TRUE;
}

KDvoid Ch1_3DCubes::step ( KDfloat fDelta )
{
	m_pCube3d1->setRotation3DAngle ( m_pCube3d1->getRotation3DAngle ( ) + 0.5f );
	m_pCube3d2->setRotation3DAngle ( m_pCube3d2->getRotation3DAngle ( ) - 0.5f );
}