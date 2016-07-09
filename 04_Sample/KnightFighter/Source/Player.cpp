/* --------------------------------------------------------------------------
 *
 *      File            Player.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 30/04/2011
 *
 * --------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "Player.h"

KDbool Player::init ( KDvoid )
{
	if ( !GameSprite::init ( ) )
	{
		return KD_FALSE;
	}
	
	m_nOrderOffsetZ				= 0;
	m_fVelocitySpeedUp			= 270.f;
	m_fVelocityOrdinary			= 180.f;
	m_fVelocity					= m_fVelocityOrdinary;
	m_bAlive					= KD_TRUE;
	m_bIsMoving					= KD_FALSE;
	m_bSpeedUpActive			= KD_FALSE;
	m_bTripleShotsActive		= KD_FALSE;
	m_sSpritesheetBaseFilename	= "walking";

	this->cacheFrames ( );

	return KD_TRUE;
}
