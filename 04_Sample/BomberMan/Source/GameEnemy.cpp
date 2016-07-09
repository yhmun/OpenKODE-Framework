/* --------------------------------------------------------------------------
 *
 *      File            GameEnemy.cpp
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
#include "GameEnemy.h"

GameEnemy* GameEnemy::create ( const CCPoint& tPoint, CCLayer* pLayer )
{
	GameEnemy*	pRet = new GameEnemy ( );

	if ( pRet && pRet->initAtPoint ( tPoint, pLayer ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GameEnemy::initAtPoint ( const CCPoint& tPoint, CCLayer* pLayer )
{
	if ( !CCSprite::initWithSpriteFrameName ( "enemy1.png" ) )
	{
		return KD_FALSE;
	}

	this->setPosition ( tPoint );

	m_fSpeed	 = 3;
	m_eAct		 = (EnemyAct) ( kdRand ( ) % 2 + 1 );
	m_pTileArray = KD_NULL;

	m_pMoneyLabel = CCLabelTTF::create ( "100", "Marker Felt.ttf", 16 );
	m_pMoneyLabel->setColor ( ccc3 ( 0, 255, 0 ) );
	m_pMoneyLabel->setVisible ( KD_FALSE );
	pLayer->addChild ( m_pMoneyLabel );
	
	pLayer->addChild ( this );

	CCSpriteFrameCache*		pCache  = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	CCSpriteFrame*			pFrame1 = pCache->spriteFrameByName ( "enemy1.png" );
	CCSpriteFrame*			pFrame2 = pCache->spriteFrameByName ( "enemy2.png" );
	CCArray*				pFrames = CCArray::create ( pFrame1, pFrame2, KD_NULL );

	CCAnimation*			pAnimation = CCAnimation::createWithSpriteFrames ( pFrames, 0.2f );
	this->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );

	this->schedule ( schedule_selector ( GameEnemy::enemyMove ), 1 / 10.f );

	return KD_TRUE;		
}

KDvoid GameEnemy::setTileArray ( CCArray* pTileArray )
{
	m_pTileArray = pTileArray;
}

KDfloat GameEnemy::getSpeed ( KDvoid )
{
	return m_fSpeed;
}

KDvoid GameEnemy::enemyMove ( KDfloat fDelta )
{
    if ( m_eAct == kHorizon ) 
    {
        CCSprite*	pTile;
     
        KDbool		bLeftEnable  = KD_FALSE;
        KDbool		bRightEnable = KD_FALSE;

        if ( this->getPosition ( ).x== 1.5 * TileW ) 
        {
            bLeftEnable = KD_TRUE;
        }
        if ( this->getPosition ( ).x == 13.5 * TileW ) 
        {
            bRightEnable = KD_TRUE;
        }

        for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
        {
            pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
            if ( pTile->getPosition ( ).y == this->getPosition ( ).y && this->getPosition ( ).x - pTile->getPosition ( ).x == TileW ) 
            {
                bLeftEnable = KD_TRUE;
            }
            if ( pTile->getPosition ( ).y == this->getPosition ( ).y && pTile->getPosition ( ).x - this->getPosition ( ).x == TileW ) 
            {
                bRightEnable = KD_TRUE;
            }
        }

        if ( bLeftEnable && bRightEnable ) 
        {
            m_eAct = kVertical;
            return;
        }

        for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
        {
            pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
            if ( ( this->getPosition ( ).x < pTile->getPosition ( ).x ) && 
				   pTile->getPosition ( ).x - this->getPosition ( ).x <= TileW && 
				   kdFabsf ( pTile->getPosition ( ).y - this->getPosition ( ).y ) < TileW && m_fSpeed > 0 ) 
            {
                m_fSpeed = -m_fSpeed;
                break;
            }
            
            if ( this->getPosition ( ).x >= 13.5 * TileW ) 
            {
                this->setPosition ( ccp ( 13.5 * TileW, this->getPosition ( ).y ) );
                m_fSpeed = -m_fSpeed;
                break;
            }
            
            if ( ( this->getPosition ( ).x > pTile->getPosition ( ).x ) &&
				   this->getPosition ( ).x - pTile->getPosition ( ).x <= TileW &&
				   kdFabsf ( pTile->getPosition ( ).y - this->getPosition ( ).y ) < TileW && m_fSpeed < 0 ) 
            {
                m_fSpeed = -m_fSpeed;
                break;
            }
           
            if ( this->getPosition ( ).x <= 1.5 * TileW ) 
            {
                this->setPosition ( ccp ( 1.5 * TileW, this->getPosition ( ).y ) );
                m_fSpeed = -m_fSpeed;
                break;
            }
        }
      
        this->setPositionX ( this->getPositionX ( ) + m_fSpeed );
        m_nStepCount++;

    }

    if ( m_eAct == kVertical )
    {
        CCSprite*	pTile;
     
        KDbool		bUpEnable   = KD_FALSE;
        KDbool		bDownEnable = KD_FALSE;

        if ( this->getPosition ( ).y == 0.5 * TileH ) 
        {
            bDownEnable = KD_TRUE;
        }

        if ( this->getPosition ( ).y == 8.5 * TileH ) 
        {
            bUpEnable = KD_TRUE;
        }

        for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
        {
            pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
            if ( pTile->getPosition ( ).x == this->getPosition ( ).x && this->getPosition ( ).y - pTile->getPosition ( ).y == TileH ) 
            {
                bDownEnable = KD_TRUE;
            }
            if ( pTile->getPosition ( ).x == this->getPosition ( ).x && pTile->getPosition ( ).y - this->getPosition ( ).y == TileW ) 
            {
                bUpEnable = KD_TRUE;
            }
        }

        if ( bDownEnable && bUpEnable ) 
        {
            m_eAct = kHorizon;
            return;
        }
		
        for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
        {
            pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
            if ( ( this->getPosition ( ).y < pTile->getPosition ( ).y ) && 
				   pTile->getPosition ( ).y - this->getPosition ( ).y <= 
				   TileH && kdFabsf ( pTile->getPosition ( ).x - this->getPosition ( ).x ) < TileW ) 
            {
                m_fSpeed = -m_fSpeed;
                break;
            }
           
            if ( this->getPosition ( ).y >= ( 9 - 0.5 ) * TileH ) 
            {
                this->setPosition ( ccp ( this->getPosition ( ).x, ( 9 - 0.5 ) * TileH ) );
                m_fSpeed = -m_fSpeed;
                break;
            }
            if ( ( this->getPosition ( ).y > pTile->getPosition ( ).y ) &&
				  this->getPosition ( ).y - pTile->getPosition ( ).y <= TileH &&
				  kdFabsf ( pTile->getPosition ( ).x - this->getPosition ( ).x ) < TileW ) 
            {
                m_fSpeed = -m_fSpeed;
                break;
            }
            
            if ( this->getPosition ( ).y<=0.5*TileH ) 
            {
                this->setPosition ( ccp ( this->getPosition ( ).x, 0.5 * TileH ) );
                m_fSpeed = -m_fSpeed;
                break;
            }
        }   
        
        this->setPositionY ( this->getPositionY ( ) + m_fSpeed );
        m_nStepCount++;     	
    }

    if ( m_nStepCount >= 64 / Nandu ) 
    {
        if ( (KDint) ( this->getPosition ( ).x + 0.5 * TileW ) % TileW == 0 && m_eAct == kHorizon ) 
        {
            m_eAct = kVertical;
            m_nStepCount = 0;
        }
        else if ( (KDint) ( this->getPosition ( ).y + 0.5 * TileH ) % TileH == 0 && m_eAct == kVertical ) 
        {
            m_eAct = kHorizon;
            m_nStepCount = 0;
        }
    }
}

KDvoid GameEnemy::enemyKill ( KDvoid )
{
	this->runAction ( CCTintTo::create ( 1, 255, 255, 255 ) );

    this->runAction 
	(
		CCSequence::create 
		(
			CCScaleTo::create ( 1, 0 ),
			CCCallFunc::create ( this, callfunc_selector ( GameEnemy::removeEnemy ) ),
			KD_NULL
		) 
	);
        
    m_pMoneyLabel->setVisible ( KD_TRUE );
    m_pMoneyLabel->setPosition ( this->getPosition ( ) );

	m_pMoneyLabel->runAction ( CCScaleTo::create ( 1, 2 ) );
	m_pMoneyLabel->runAction ( CCMoveBy ::create ( 1, ccp ( 0, 32 ) ) );
}

KDvoid GameEnemy::removeEnemy ( KDvoid )
{
    m_pMoneyLabel->removeFromParentAndCleanup ( KD_TRUE );
    this->removeFromParentAndCleanup ( KD_TRUE );
}
