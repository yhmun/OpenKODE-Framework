/* --------------------------------------------------------------------------
 *
 *      File            Hero.cpp
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
#include "Hero.h"

Hero* Hero::create ( const CCPoint& tPosition )
{
	Hero*	pRet = new Hero ( );

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

KDbool Hero::initWithPosition ( const CCPoint& tPosition )
{
	if ( !CCSprite::initWithSpriteFrameName ( "hero.png" ) )
	{
		return KD_FALSE;
	}

	this->setPosition ( tPosition );

	m_tVelocity		 = ccpz;
	m_fTopGroundY	 = tPosition.y;
	m_fBottomGroundY = tPosition.y;
	m_eState		 = kHeroStateIdle;

	return KD_TRUE;
}

KDvoid Hero::update ( KDfloat fDelta )
{
	if ( m_eState == kHeroStateFall )
	{
		KDbool		bOverTop = KD_FALSE;

		if ( this->getPosition ( ).y > m_fTopGroundY )
		{
			bOverTop = KD_TRUE;
		}

		if ( this->getPosition ( ).y > m_fBottomGroundY ) 
		{
			m_tVelocity = ccpAdd ( m_tVelocity, ccp ( 0, -20.0f * fDelta ) );
			this->setPosition ( ccpAdd ( this->getPosition ( ), m_tVelocity ) );
		}

		if ( bOverTop && this->getPosition ( ).y <= m_fTopGroundY )
		{
			this->setPositionY ( m_fTopGroundY );
			m_eState = kHeroStateIdle;
		}
		else if ( this->getPosition ( ).y <= m_fBottomGroundY )
		{
			this->setPositionY ( m_fBottomGroundY );
			m_eState = kHeroStateIdle;
		}
	}
}

KDvoid Hero::setState ( HeroState eState )
{
	if ( m_eState == eState )
	{
		return;
	}

	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	switch ( eState )
	{
		case kHeroStateDrag :
		case kHeroStateFall :
			this->setDisplayFrame ( pCache->spriteFrameByName ( "heroDrag.png" ) );
			break;

		case kHeroStateGrab :
			this->setDisplayFrame ( pCache->spriteFrameByName ( "heroGrab.png" ) );
			break;

		default :
			this->setDisplayFrame ( pCache->spriteFrameByName ( "hero.png" ) );
			break;
	}

	m_eState = eState;
	m_tVelocity = ccpz;
}
