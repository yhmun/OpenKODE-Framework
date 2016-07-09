/* --------------------------------------------------------------------------
 *
 *      File            Ch2_AccelerometerRotation.cpp
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
#include "Ch2_AccelerometerRotation.h"

KDbool Ch2_AccelerometerRotation::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Enable the accelerometer and set its updateInterval
	this->setAccelerometerEnabled ( KD_TRUE );

	// Init our textured box
	m_pIPhoneCube = IPhoneCube3D::create ( );
	m_pIPhoneCube->setTranslation3D ( Vector3D ( 0, 0, -2 ) ); 
	m_pIPhoneCube->setRotation3DAxis ( Vector3D ( 0, 0, -90 ) ); 
		
	this->addChild ( m_pIPhoneCube, 3, 0 );

	return KD_TRUE;
}

/// Handle accelerometer input 
KDvoid Ch2_AccelerometerRotation::didAccelerate ( CCAcceleration* pAccelerationValue )
{
	Vector3D	tVector;

	tVector.x = pAccelerationValue->x * 360.f;
	tVector.y = pAccelerationValue->y * 360.f;
	tVector.z = pAccelerationValue->z * 360.f + 270.f;

	m_pIPhoneCube->setRotation3DAxis ( tVector );
}