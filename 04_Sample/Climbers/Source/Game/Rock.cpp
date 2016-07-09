/* --------------------------------------------------------------------------
 *
 *      File            Rock.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Sergey Tikhonov. All rights reserved. 
 *
 *      https://www.github.com/haqu/climbers
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
#include "Rock.h"

Rock* Rock::create ( const CCPoint& tPosition )
{
	Rock*	pRet = new Rock ( );

	if ( pRet && pRet->initWithPosition ( tPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool Rock::initWithPosition ( const CCPoint& tPosition )
{
	if ( !CCSprite::initWithSpriteFrameName ( "rock.png" ) )
	{
		return KD_FALSE;
	}

	this->setPosition ( tPosition );

	m_tVelocity	= ccpz;
	m_bFalling	= KD_FALSE;

	return KD_TRUE;
}

#define kGroundY	32.0f

KDvoid Rock::update ( KDfloat fDelta )
{
	if ( this->getPosition ( ).y > kGroundY )
	{
		m_tVelocity = ccpAdd ( m_tVelocity, ccp ( 0, -20.0f * fDelta ) );
		if ( m_tVelocity.y < -10.f )
		{
			m_tVelocity = ccp ( 0, -10 );	// limit
		}
		this->setPosition ( ccpAdd ( this->getPosition ( ), m_tVelocity ) );
	}
	else if ( this->getPosition ( ).y < kGroundY ) 
	{
		this->setPositionY ( kGroundY );

		m_tVelocity	= ccpz;
		m_bFalling	= KD_FALSE;
	}
}

