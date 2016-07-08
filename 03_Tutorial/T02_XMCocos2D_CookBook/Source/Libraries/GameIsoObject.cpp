/* --------------------------------------------------------------------------
 *
 *      File            GameIsoObject.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "GameIsoObject.h"
#include "GameArea2D.h"

GameIsoObject* GameIsoObject::create ( KDvoid )
{
	GameIsoObject*	pRet = new GameIsoObject ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

GameIsoObject::GameIsoObject ( KDvoid )
{
	m_pSpriteShadow			= KD_NULL;
	m_pBounceCoefficient	= KD_NULL;
	m_pRollCoefficient		= KD_NULL;
}

GameIsoObject::~GameIsoObject ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSpriteShadow );
	CC_SAFE_RELEASE ( m_pBounceCoefficient );
	CC_SAFE_RELEASE ( m_pRollCoefficient );
}

KDbool GameIsoObject::init ( KDvoid )
{	
	if ( !GameObject::init ( ) )
	{
		return KD_FALSE;
	}

	m_fModifierZ = 0;

	this->setBounceCoefficient ( Vector3D::create ( 0.2f, 0.2f, 0.75f ) );
	this->setRollCoefficient   ( Vector3D::create ( 0.01f, 0.01f, 0.8f ) );

	return KD_TRUE;
}

KDint GameIsoObject::getType ( KDvoid )
{
	return GO_TYPE_MISC;
}

KDfloat GameIsoObject::getModifierY ( KDvoid )
{
	return m_fModifierY;
}

KDvoid GameIsoObject::setModifierY ( KDfloat fModifierY )
{
	m_fModifierY = fModifierY;
}

KDfloat GameIsoObject::getActualImageSize ( KDvoid )
{
	return m_fActualImageSize;
}

KDvoid GameIsoObject::setActualImageSize ( KDfloat fActualImageSize )
{
	m_fActualImageSize = fActualImageSize;
}

KDfloat GameIsoObject::getInGameSize ( KDvoid )
{	
	return m_fInGameSize;
}

KDvoid GameIsoObject::setInGameSize ( KDfloat fInGameSize )
{
	m_fInGameSize = fInGameSize;
}

KDfloat GameIsoObject::getModifierZ ( KDvoid )
{
	return m_fModifierZ;
}

KDvoid GameIsoObject::setModifierZ ( KDfloat fModifierZ )
{
	m_fModifierZ = fModifierZ;
}

Vector3D* GameIsoObject::getBounceCoefficient ( KDvoid )
{
	return m_pBounceCoefficient;
}

KDvoid GameIsoObject::setBounceCoefficient ( Vector3D* pBounceCoefficient )
{
	CC_SAFE_RELEASE ( m_pBounceCoefficient );
	m_pBounceCoefficient = pBounceCoefficient;
	CC_SAFE_RETAIN  ( m_pBounceCoefficient );
}

Vector3D* GameIsoObject::getRollCoefficient ( KDvoid )
{
	return m_pRollCoefficient;
}

KDvoid GameIsoObject::setRollCoefficient ( Vector3D* pRollCoefficient )
{
	CC_SAFE_RELEASE ( m_pRollCoefficient );
	m_pRollCoefficient = pRollCoefficient;
	CC_SAFE_RETAIN  ( m_pRollCoefficient );
}

CCSprite* GameIsoObject::getSpriteShadow ( KDvoid )
{
	return m_pSpriteShadow;
}

KDvoid GameIsoObject::setSpriteShadow ( CCSprite* pSpriteShadow )
{
	CC_SAFE_RELEASE ( m_pSpriteShadow );
	m_pSpriteShadow = pSpriteShadow;
	CC_SAFE_RETAIN  ( m_pSpriteShadow );
}
