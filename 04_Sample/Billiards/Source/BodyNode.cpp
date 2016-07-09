/* --------------------------------------------------------------------------
 *
 *      File            BodyNode.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Quipu on 11/04/11
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
#include "BodyNode.h"

BodyNode::BodyNode ( KDvoid )
{
	m_pBody   = KD_NULL;
	m_pSprite = KD_NULL;
}

BodyNode::~BodyNode ( KDvoid )
{
	this->removeSprite ( );
	this->removeBody   ( );
}

KDvoid BodyNode::createBodyInWorld ( b2World* pWorld, b2BodyDef* pBodyDef, b2FixtureDef* pFixtureDef, const KDchar* szSpriteName )
{
	CCAssert ( pWorld != KD_NULL, "world is null!" );
	CCAssert ( pBodyDef != KD_NULL, "bodyDef is null!" );
	CCAssert ( szSpriteName != KD_NULL, "spriteFrameName is nil!" );

	this->removeSprite ( );
	this->removeBody   ( );

	this->setSprite ( CCSprite::create ( szSpriteName ) );
	this->addChild  ( m_pSprite );
    
    m_pBody = pWorld->CreateBody ( pBodyDef );
    m_pBody->SetUserData ( this );
     
    if ( pFixtureDef != KD_NULL ) 
	{
        m_pBody->CreateFixture ( pFixtureDef );
    }
}

KDvoid BodyNode::removeSprite ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprite );
}

KDvoid BodyNode::removeBody ( KDvoid )
{
	if ( m_pBody != KD_NULL )
	{
		m_pBody->GetWorld ( )->DestroyBody ( m_pBody );
		m_pBody = KD_NULL;
	}
}

