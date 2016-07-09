/* --------------------------------------------------------------------------
 *
 *      File            BodyNode.cpp    
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "BodyNode.h"

BodyNode::BodyNode ( KDvoid )
{
	m_pBody   = KD_NULL;
	m_pSprite = KD_NULL;
}

b2Body* BodyNode::getBody ( KDvoid )
{
	return m_pBody;
}

CCSprite* BodyNode::getSprite ( KDvoid )
{
	return m_pSprite;
}

KDvoid BodyNode::fadeThenRemove ( KDfloat fDelta )
{
	m_pSprite->runAction 
	(
		CCSequence::create 
		(	
			CCFadeTo::create ( 1.f, 0 ),
			CCCallFunc::create ( this, callfunc_selector ( BodyNode::removeSpriteAndBody ) ),
			KD_NULL 
		) 
	);
}

KDvoid BodyNode::makeBodyStatic ( KDvoid ) 
{
	m_pBody->SetType ( b2_staticBody );
}

KDvoid BodyNode::removeBody ( KDvoid ) 
{
	if ( m_pBody != KD_NULL )
	{
		m_pBody->GetWorld ( )->DestroyBody ( m_pBody );
		m_pBody = KD_NULL;
	}
}

KDvoid BodyNode::removeSprite ( KDvoid )  
{
	if ( m_pSprite != KD_NULL ) 
	{
		this->removeFromParentAndCleanup ( KD_TRUE );
		m_pSprite = KD_NULL;
	}
}

KDvoid BodyNode::removeSpriteAndBody ( KDvoid )
{	
	CCLOG ( "removing sprite and body" );

	this->removeSprite ( );

	this->removeBody ( );

	this->removeFromParentAndCleanup ( KD_TRUE );
}

KDvoid BodyNode::createBodyWithSpriteAndFixture ( b2World* pWorld, b2BodyDef* pBodyDef, b2FixtureDef* pFixtureDef, const KDchar* szSpriteName )
{
	// this is the meat of our class, it creates (OR recreates) the body in the world with the body definition, fixture definition and sprite name

	removeBody ( );		// if remove the body if it already exists

	removeSprite ( );	// if remove the sprite if it already exists

	m_pSprite = CCSprite::create ( szSpriteName );
	this->addChild ( m_pSprite );

	m_pBody = pWorld->CreateBody ( pBodyDef );
	m_pBody->SetUserData ( this ); 

	if ( pFixtureDef != KD_NULL ) 
	{		
		m_pBody->CreateFixture ( pFixtureDef );
	}
}