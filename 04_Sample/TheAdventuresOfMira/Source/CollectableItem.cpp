/* --------------------------------------------------------------------------
 *
 *      File            CollectableItem.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/15
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
#include "CollectableItem.h"

CollectableItem *CollectableItem::create ( GameObjectType eGameObjectType )
{
    CollectableItem*	pRet = new CollectableItem ( );
    
	if ( pRet && pRet->init ( eGameObjectType ) )
    {
        pRet->autorelease ( );
        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CollectableItem::init ( GameObjectType eType )
{
	if ( !GameObject::init ( ) )
	{
		return KD_FALSE;
	}

	this->setGameObjectType ( eType );
	
	this->changeState ( kCharacterStateIdle );

	this->initSprite ( );

    return KD_TRUE;
}

KDvoid CollectableItem::changeState ( CharacterState eNewState )
{
    
}

CCRect CollectableItem::adjustedBoundingBox ( KDvoid )
{
    CCRect	tOriginalBoundingBox = this->boundingBox ( );

    return CCRect ( tOriginalBoundingBox.origin.x + ( tOriginalBoundingBox.size.cx / 8 ),
					tOriginalBoundingBox.origin.y + ( tOriginalBoundingBox.size.cy / 4 ),
					tOriginalBoundingBox.size.cx * 0.7f,
					tOriginalBoundingBox.size.cy * 0.4f );
}

KDvoid CollectableItem::checkAndClampSpritePosition ( KDvoid )
{
    
}

KDvoid CollectableItem::initSprite ( KDvoid )
{
	const KDchar*	szFrameName;

    switch ( this->getGameObjectType ( ) )
	{
        case kGameObjectTypeCollectableFlower1	:	szFrameName = "item_1.png";		break;
		case kGameObjectTypeCollectableFlower2	:	szFrameName = "item_2.png";		break;
		case kGameObjectTypeCollectableFlower3	:	szFrameName = "item_3.png";		break;
		case kGameObjectTypeCollectableFlower4	:	szFrameName = "item_4.png";		break;
		case kGameObjectTypeCollectableFlower5	:	szFrameName = "item_5.png";		break;
		case kGameObjectTypePowerUpCandy		:	szFrameName = "item_6.png";		break;
		case kGameObjectTypePowerUpMagnet		:	szFrameName = "item_7.png";		break;
            
        default :
			return;
    }

	this->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szFrameName ) );
}
