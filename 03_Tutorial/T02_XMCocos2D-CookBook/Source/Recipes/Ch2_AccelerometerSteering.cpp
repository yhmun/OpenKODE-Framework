/* --------------------------------------------------------------------------
 *
 *      File            Ch2_AccelerometerSteering.cpp
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
#include "Ch2_AccelerometerSteering.h"

KDbool Ch2_AccelerometerSteering::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Enable the accelerometer 
	this->setAccelerometerEnabled ( KD_TRUE );

	// Init car background
	CCSprite*	pBG = CCSprite::create ( "car_dash.jpg" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setOpacity ( 200 );
	this->addChild ( pBG, 0 );

	// Init steeringWheel sprite
	m_pSteeringWheel = CCSprite::create ( "car_steering_wheel.png" );
	m_pSteeringWheel->setPosition ( ccp ( 230, 170 ) );
	this->addChild ( m_pSteeringWheel, 1 );

	return KD_TRUE;
}

KDvoid Ch2_AccelerometerSteering::didAccelerate ( CCAcceleration* pAccelerationValue )
{
	// Set steeringWheel rotation based on Y plane rotation
	m_pSteeringWheel->setRotation ( pAccelerationValue->x * 180 );
}
