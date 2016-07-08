/* --------------------------------------------------------------------------
 *
 *      File            TestAccelerometer.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestAccelerometer.h"

#define FIX_POS( _pos, _min, _max )    if ( _pos < _min ) { _pos = _min; } else if ( _pos > _max ) { _pos = _max; }

//------------------------------------------------------------------
//
// AccelerometerTest
//
//------------------------------------------------------------------
KDbool TestAccelerometer::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  s = this->getContentSize ( );
	
    m_pBall = CCSprite::create ( "Images/ball.png" );	
	m_pBall->setPosition ( ccpMid ( s ) );
	this->addChild ( m_pBall );   

	m_tPos = m_pBall->getPosition ( );

	this->setAccelerometerEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid TestAccelerometer::didAccelerate ( CCAcceleration* pAccelerationValue )
{
	const CCSize&  s = this->getContentSize ( );
	const CCSize&  t = m_pBall->getContentSize ( );

	m_tPos.x += pAccelerationValue->x * 9.8f;
	m_tPos.y += pAccelerationValue->y * 9.8f;

	FIX_POS ( m_tPos.x, ( t.cx / 2.0f ), ( s.cx - t.cx / 2.0f ) );
	FIX_POS ( m_tPos.y, ( t.cy / 2.0f ), ( s.cy - t.cy / 2.0f ) );

	m_pBall->runAction ( CCMoveTo::create ( 1, m_tPos ) );

}
