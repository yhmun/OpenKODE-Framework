/* --------------------------------------------------------------------------
 *
 *      File            RoofCache.cpp
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
#include "RoofCache.h"
#include "RoofSprite.h"
#include "ShipSprite.h"
#include "GameLayer.h"

#define MAX_ROOFS 8

RoofCache* RoofCache::create ( GameLayer* pGameLayer )
{
	RoofCache*	pRet = new RoofCache ( );

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


KDbool RoofCache::init ( GameLayer* pGameLayer )
{
    if ( !CCNode::init ( ) )
    {
        return KD_FALSE;
    }

	m_pGameLayer	= pGameLayer;
    
    CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ( "bullet.png" );
    m_pBatch = CCSpriteBatchNode::createWithTexture ( pFrame->getTexture ( ) );
    this->addChild ( m_pBatch );
    this->initRoofs ( );
    this->scheduleUpdate ( );
    
	return KD_TRUE;
}


KDvoid RoofCache::initRoofs ( KDvoid )
{
    m_pRoofs = CCArray::createWithCapacity ( 2 * MAX_ROOFS );
    m_pRoofs->retain ( );

    CCSize tWinSize = m_pGameLayer->getScreenSize ( );
    for ( KDint i = 0 ; i < MAX_ROOFS; i++ )
    {
        RoofSprite *pRoof1       = RoofSprite::create ( m_pGameLayer );
        KDfloat fRoofHeight        = pRoof1->getContentSize ( ).cy;
        KDfloat fRoofWidth         = pRoof1->getContentSize ( ).cx;
        KDfloat fRandomDeltaHeight = kdRand ( ) % 30;
   
        pRoof1->setPosition ( ccp ( i * ( fRoofWidth - 3 ), 30 - fRoofHeight / 2 + fRandomDeltaHeight ) );
        m_pRoofs->addObject ( pRoof1 );
        pRoof1->showUp ( );
   
        RoofSprite *pRoof2 = RoofSprite::create ( m_pGameLayer );
        pRoof2->setPosition ( ccp ( i * ( fRoofWidth - 3 ), tWinSize.height + fRoofHeight / 8 + fRandomDeltaHeight ) );
        m_pRoofs->addObject ( pRoof2 );
        pRoof2->showUp ( );
    }
}


KDvoid RoofCache::update ( KDfloat fDelta )
{
    this->checkoutForReuse ( );
    this->checkForCollisions ( );
}


KDvoid RoofCache::checkoutForReuse ( KDvoid )
{
    CCObject* pObj = KD_NULL;
    CCARRAY_FOREACH ( m_pRoofs, pObj )
    {
        RoofSprite* pRoof  = ( RoofSprite* ) pObj;
        if ( !pRoof->isVisible ( ) )
        {
            pRoof->setPosition ( ccpAdd ( pRoof->getPosition(), ccp ( ( pRoof->getContentSize ( ).cx - 3 ) * MAX_ROOFS, 0 ) ) );
            pRoof->setVisible ( KD_TRUE );
        }
    }
}


KDvoid RoofCache::checkForCollisions ( KDvoid )
{
    CCObject* pObj = KD_NULL;
    CCARRAY_FOREACH ( m_pRoofs, pObj )
    {
        RoofSprite* pRoof  = ( RoofSprite* ) pObj;
        if ( pRoof->isVisible ( ) )
        {
            ShipSprite* pShip     = m_pGameLayer->getShip ( );
            CCRect      tBlockBox = pRoof->boundingBox ( );
            CCRect      tShipBox  = pShip->boundingBox ( );
            if ( tBlockBox.intersectsRect ( tShipBox ) )
			{
                CCLOG ( "GAME END" );
                m_pGameLayer->doGameOver ( );
            }
        }
    }
}


KDvoid RoofCache::reset ( )
{
    CCObject* pObj = KD_NULL;
    CCARRAY_FOREACH ( m_pRoofs, pObj )
    {
        RoofCache* pRoof = ( RoofCache* ) pObj;
        CCNode*    pNode = pRoof->getChildByTag ( ROOF_TAG_MOVE );
        pNode->unscheduleAllSelectors ( );
        pNode->scheduleUpdate ( );
    }
}
