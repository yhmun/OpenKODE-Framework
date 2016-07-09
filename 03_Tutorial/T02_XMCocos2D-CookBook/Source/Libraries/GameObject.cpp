/* --------------------------------------------------------------------------
 *
 *      File            GameObject.cpp
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
#include "GameObject.h"
#include "GameArea2D.h"

GameObject::GameObject ( KDvoid )
{
	m_pGameArea				= KD_NULL;
	m_pBody					= KD_NULL;
	m_pBodyDef				= KD_NULL;
	m_pFixtureDef			= KD_NULL;
	m_pPolygonShape			= KD_NULL;
	m_pCircleShape			= KD_NULL;
	m_pSprite				= KD_NULL;
	m_nTypeTag				= 0;
	m_bMarkedForDestruction = KD_FALSE;
}

GameObject::~GameObject ( KDvoid )
{
	CC_SAFE_DELETE ( m_pBodyDef );
	CC_SAFE_DELETE ( m_pFixtureDef );

	CC_SAFE_RELEASE ( m_pSprite );
}

KDbool GameObject::init ( KDvoid )
{	
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_bMarkedForDestruction = KD_FALSE;

	this->initBox2D ( );

	return KD_TRUE;
}

KDvoid GameObject::initBox2D ( KDvoid )
{
	m_pBodyDef	  = new b2BodyDef ( );
	m_pFixtureDef = new b2FixtureDef ( );

	// Initial fixture settings
	m_pFixtureDef->density		= 1.0f;
	m_pFixtureDef->friction		= 0.5f;
	m_pFixtureDef->restitution	= 0.3f;

	m_pBodyDef->userData		= this;
}

GameArea2D* GameObject::getGameArea ( KDvoid )
{
	return m_pGameArea;
}

KDvoid GameObject::setGameArea ( GameArea2D* pGameArea )
{
	pGameArea->addChild ( this );

	m_pGameArea = pGameArea;
}

b2Body* GameObject::getBody ( KDvoid )
{
	return m_pBody;
}

KDvoid GameObject::setBody ( b2Body* pBody )
{
	m_pBody = pBody;
}

b2BodyDef* GameObject::getBodyDef ( KDvoid )
{
	return m_pBodyDef;
}

KDvoid GameObject::setBodyDef ( b2BodyDef* pBodyDef )
{
	m_pBodyDef = pBodyDef;
}

b2FixtureDef* GameObject::getFixtureDef ( KDvoid )
{
	return m_pFixtureDef;
}

KDvoid GameObject::setFixtureDef ( b2FixtureDef* pFixtureDef )
{
	m_pFixtureDef = pFixtureDef;
}

b2PolygonShape* GameObject::getPolygonShape ( KDvoid )
{
	return m_pPolygonShape;
}

KDvoid GameObject::setPolygonShape ( b2PolygonShape* pPolygonShape )
{
	m_pPolygonShape = pPolygonShape;
}

b2CircleShape* GameObject::getCircleShape ( KDvoid )
{
	return m_pCircleShape;
}

KDvoid GameObject::setCircleShape ( b2CircleShape* pCircleShape )
{
	m_pCircleShape = pCircleShape;
}

CCSprite* GameObject::getSprite ( KDvoid )
{
	return m_pSprite;
}

KDvoid GameObject::setSprite ( CCSprite* pSprite )
{
	CC_SAFE_RELEASE ( m_pSprite );
	m_pSprite = pSprite;
	CC_SAFE_RETAIN  ( m_pSprite );
}

KDint GameObject::getTypeTag ( KDvoid )
{
	return m_nTypeTag;
}

KDvoid GameObject::setTypeTag ( KDint nTypeTag )
{
	m_nTypeTag = nTypeTag;
}

KDbool GameObject::isMarkedForDestruction ( KDvoid )
{
	return m_bMarkedForDestruction;
}

KDvoid GameObject::setMarkedForDestruction ( KDbool bMarkedForDestruction )
{
	m_bMarkedForDestruction = bMarkedForDestruction;
}	

KDint GameObject::getType ( KDvoid )
{
	return GO_TYPE_NONE;
}