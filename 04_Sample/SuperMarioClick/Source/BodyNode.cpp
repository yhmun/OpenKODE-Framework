/* --------------------------------------------------------------------------
 *
 *      File            BodyNode.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
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
#include "MashRoom.h"
#include "SuperMarioClick.h"

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

	CCSpriteBatchNode*	pBatch = SuperMarioClick::sharedSuperMario ( )->getSpriteBatch ( );
	m_pSprite = CCSprite::createWithSpriteFrameName ( szSpriteName );
	pBatch->addChild ( m_pSprite );

    m_pBody = pWorld->CreateBody ( pBodyDef );
    if ( !kdStrcmp ( szSpriteName, "Mushroom_32.png" ) )
    {
		MashRoom::sharedMashRoom ( )->setBody ( m_pBody );     
    }
	m_pBody->SetUserData ( this );
	
	if ( pFixtureDef != KD_NULL ) 
	{
        b2Fixture*	pTemp = m_pBody->CreateFixture ( pFixtureDef );
        if ( !kdStrcmp ( szSpriteName, "Mushroom_32.png" ) )
        {
			MashRoom::sharedMashRoom ( )->setFixture ( pTemp );
        }
	}
}

KDvoid BodyNode::removeSprite ( KDvoid )
{
	CCSpriteBatchNode*	pBatch = SuperMarioClick::sharedSuperMario ( )->getSpriteBatch ( );
	if ( m_pSprite != KD_NULL && pBatch->getChildren ( )->containsObject ( m_pSprite ) )
	{
		m_pSprite->removeFromParentAndCleanup ( KD_TRUE );
		pBatch->getChildren ( )->removeObject ( m_pSprite );
        m_pSprite = KD_NULL;
	}
}

KDvoid BodyNode::removeBody ( KDvoid )
{
	if ( m_pBody != KD_NULL )
	{
		m_pBody->GetWorld ( )->DestroyBody ( m_pBody );
		m_pBody = KD_NULL;
	}
}

