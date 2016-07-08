/* --------------------------------------------------------------------------
 *
 *      File            CCMoveByCustom.cpp
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
#include "CCMoveByCustom.h"

CCMoveByCustom* CCMoveByCustom::create ( KDfloat fDuration, const CCPoint& tDeltaPosition )
{
	CCMoveByCustom*		pRet = new CCMoveByCustom ( );

	if ( pRet && pRet->initWithDuration ( fDuration, tDeltaPosition ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid CCMoveByCustom::update ( KDfloat fDelta )
{	
	// Here we neglect to change something with a zero delta.
	if ( m_tPositionDelta.x == 0 )
	{
		m_pTarget->setPosition ( ccp ( m_pTarget->getPosition ( ).x, m_tStartPosition.y +  m_tPositionDelta.y * fDelta ) );
	}
	else if ( m_tPositionDelta.y == 0 )
	{
		m_pTarget->setPosition ( ccp ( m_tStartPosition.x + m_tPositionDelta.x * fDelta, m_pTarget->getPosition ( ).y ) );
	}
	else
	{
		m_pTarget->setPosition ( ccp ( m_tStartPosition.x + m_tPositionDelta.x * fDelta, m_tStartPosition.y + m_tPositionDelta.y * fDelta ) );	
	}
}