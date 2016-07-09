/* --------------------------------------------------------------------------
 *
 *      File            RoofSprite.cpp
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
#include "RoofSprite.h"
#include "StandardMoveComponent.h"
#include "GameLayer.h"


RoofSprite* RoofSprite::create ( GameLayer* pGameLayer )
{
	RoofSprite*	pRet = new RoofSprite ( );

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


KDbool RoofSprite::init ( GameLayer* pGameLayer )
{
    if ( !CCSprite::initWithFile ( "Images/bg.png" ) )
    {
        return KD_FALSE;
    }
    
	m_pGameLayer	= pGameLayer;

    this->addChild ( StandardMoveComponent::create ( m_pGameLayer ), 0, ROOF_TAG_MOVE );
    
    return KD_TRUE;
}


KDvoid RoofSprite::showUp ( KDvoid )
{
    this->setVisible ( KD_TRUE );
    m_pGameLayer->addChild ( this, 1 );
}
