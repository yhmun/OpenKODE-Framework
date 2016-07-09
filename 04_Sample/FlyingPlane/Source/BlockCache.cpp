/* --------------------------------------------------------------------------
 *
 *      File            BlocksCache.cpp
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
#include "BlockCache.h"
#include "BlockSprite.h"
#include "ShipSprite.h"
#include "GameLayer.h"

#define BLOCK_FREQUENCY_FAST   60
#define BLOCK_FREQUENCY_MID    90
#define BLOCK_FREQUENCY_SLOW  120
#define MAX_BLOCKS             20


BlockCache* BlockCache::create ( GameLayer* pGameLayer )
{
	BlockCache*	pRet = new BlockCache ( );

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


KDbool BlockCache::init ( GameLayer* pGameLayer  )
{
    if ( !CCNode::init ( ) )
    {
        return KD_FALSE;
    }

	m_pGameLayer = pGameLayer;
    m_nBlockFrequency     = BLOCK_FREQUENCY_SLOW;

    CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "monster-a.png" );
    m_pBatch              = CCSpriteBatchNode::createWithTexture ( pFrame->getTexture ( ) );
    this->addChild ( m_pBatch );

    this->initBlocks ( );
    this->scheduleUpdate ( );
	
	return KD_TRUE;
}


KDvoid BlockCache::initBlocks ( KDvoid )
{
    m_pBlocks = CCArray::createWithCapacity ( MAX_BLOCKS );
    m_pBlocks->retain();
    
    for ( KDint i = 0 ; i < MAX_BLOCKS; i++ )
    {
        m_pBlocks->addObject ( BlockSprite::create ( m_pGameLayer ) );
    }
}


KDvoid BlockCache::update ( KDfloat fDelta )
{
	m_nUpdateCount++;
    if ( m_nUpdateCount % m_nBlockFrequency == 0 )
    {
        this->addBlock ( );
    }
	this->checkForCollisions ( );
}


KDvoid BlockCache::addBlock ( KDvoid )
{
    CCObject*        pObj   = KD_NULL;
	CCARRAY_FOREACH ( m_pBlocks, pObj )
	{
        BlockSprite* pBlock = ( BlockSprite* ) pObj;
        
		// find the first free enemy and respawn it
		if ( !pBlock->isVisible ( ) )
		{
			pBlock->showUp ( );
			break;
		}
	}
}


KDvoid BlockCache::checkForCollisions ( KDvoid )
{
    CCObject*           pObj      = KD_NULL;
	CCARRAY_FOREACH ( m_pBlocks, pObj )
	{
        BlockSprite*    pBlock    = ( BlockSprite* ) pObj;
		if ( pBlock->isVisible ( ) )
		{
			ShipSprite* pShip     = m_pGameLayer->getShip ( );
            CCRect      tBlockBox = pBlock->boundingBox ( );
            CCRect      tShipBox  = pShip->boundingBox ( );
            if ( tBlockBox.intersectsRect ( tShipBox ) )
			{
                CCLOG ( "GAME OVER" );
                m_pGameLayer->doGameOver ( );
			}
		}
	}
}


KDvoid BlockCache::reset ( KDvoid )
{
    CCObject*         pObj   = KD_NULL;
    CCARRAY_FOREACH ( m_pBlocks, pObj )
    {
        BlockSprite*  pBlock = ( BlockSprite* ) pObj;
        pBlock->setVisible ( KD_FALSE );
        CCNode*       pNode  = pBlock->getChildByTag ( BLOCK_TAG_MOVE );
        pNode->unscheduleAllSelectors ( );
        pNode->scheduleUpdate ( );
    }
}
