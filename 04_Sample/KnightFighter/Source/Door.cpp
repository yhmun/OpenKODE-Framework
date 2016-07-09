/* --------------------------------------------------------------------------
 *
 *      File            Door.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 09/05/2011
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
#include "Door.h"

KDbool Door::init ( KDvoid )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	// Allocate house contents randomly from an array
	const KDchar*	aHouseContents [ ] = { "ExtraLife", "TripleShots", "GhostRespawn", "SpeedUp" };
	KDint			nRandomNumber = kdRand ( ) % 4;

	m_sContents = aHouseContents [ nRandomNumber ];

	return KD_TRUE;
}

const KDchar* Door::getContents ( KDvoid )
{
	return m_sContents.c_str ( );
}

KDvoid Door::setContents ( const KDchar* szContents )
{
	m_sContents = szContents;
}