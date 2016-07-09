/* --------------------------------------------------------------------------
 *
 *      File            StandardMoveComponent.cpp
 *      Description     
 *      Author          Kyoung-Cheol Kim
 *      Contact         redfreek2c@gmail.com
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
#include "StandardMoveComponent.h"
#include "GameLayer.h"

StandardMoveComponent* StandardMoveComponent::create ( GameLayer* pGameLayer )
{
	StandardMoveComponent*	pRet = new StandardMoveComponent ( );

	if ( pRet && pRet->init ( pGameLayer ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}


KDbool StandardMoveComponent::init ( GameLayer* pGameLayer )
{
    if ( !CCSprite::init ( ) )
    {
        return KD_FALSE;
    }
	m_pGameLayer = pGameLayer;
    
    m_tVelocity = CCPointMake ( -6, 0 );
    this->scheduleUpdate ( );
	
    return KD_TRUE;
}

KDvoid StandardMoveComponent::update ( KDfloat fDelta )
{
	if ( this->getParent ( )->isVisible ( ) )
	{
		CCSprite* pSprite = ( CCSprite* ) this->getParent ( );
        pSprite->setPosition ( ccpAdd(pSprite->getPosition ( ), m_tVelocity ) );

        if ( pSprite->getPosition ( ).x + pSprite->getContentSize ( ).cx < 0 )
        {
            m_pGameLayer->removeChild ( this, KD_TRUE );
            pSprite->setVisible ( KD_FALSE );
        }
	}
}