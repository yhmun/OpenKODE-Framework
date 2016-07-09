/* --------------------------------------------------------------------------
 *
 *      File            Game.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-10-25
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
#include "Game.h"

#define GAMESCALE	1.5f

static KDfloat		l_aMapShape [ 4200 ] = { 0 };

static KDfloat		l_aEnemiesPos [ 20 ] =
{
     500,  510,  525,  660,  920, 1030, 1090, 1180, 1315, 1380,
    1570, 1590, 1900, 2150, 2285, 2420, 2610, 2770, 2870, 2970
};

Game::~Game ( KDvoid )
{
	m_pGameTouches->release ( );
	m_pEnemies->release ( );
}

KDbool Game::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	this->setTag ( 2 );

    if ( l_aMapShape [ 0 ] == 0 )
    {
        for ( KDint i = 0; i < 4200; i++ )
		{
			if      ( i >   50 && i <  120 )	{ l_aMapShape [ i ] =  90.f + kdFabsf ( 70.f - i ) / 2.5f; }
			else if ( i >  550 && i <  660 )	{ l_aMapShape [ i ] =  90.f + 34; }
			else if ( i > 1225 && i < 1296 )	{ l_aMapShape [ i ] =  90.f + 64; }
			else if ( i > 1295 && i < 1350 )	{ l_aMapShape [ i ] =  90.f + 130 + i - 1320; }
			else if ( i > 1350 && i < 1701 )	{ l_aMapShape [ i ] =  90.f + 160; }
			else if ( i > 1700 && i < 1761 )	{ l_aMapShape [ i ] =  90.f + 200; }
			else if ( i > 1760 && i < 1851 )	{ l_aMapShape [ i ] =  90.f + 240; }
			else if ( i > 1850 && i < 1951 )	{ l_aMapShape [ i ] =  90.f + 300 + ( i - 1910 ) / 2.f; }
			else if ( i > 1950 && i < 2191 )	{ l_aMapShape [ i ] = 460.f + ( i - 2075 ) / 15.f; }
			else if ( i > 2190 && i < 2271 )	{ l_aMapShape [ i ] = 470.f + 50; }
			else if ( i > 2270 && i < 2501 )	{ l_aMapShape [ i ] = 450.f + ( 2345 - i ) / 4.f; }
			else if ( i > 2500 && i < 3221 )	{ l_aMapShape [ i ] = 414.f + ( 2500 - i ) / 2.222222f; }
			else								{ l_aMapShape [ i ] =  90.f; }
        }
    }

	CCSize		tSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDint		nIndex = 0;
	KDfloat		fOffsetX = 0;	

	m_pGameBG = CCLayer::create ( );
	m_pGameBG->setTag ( 3 );
	this->addChild ( m_pGameBG );	

	for ( KDint i = 347; i < 358; i++ )
	{
		if ( i == 349 || i == 371 )
		{
			i++;
		}
		else if ( i == 361 )
		{
			i += 2;
		}

		CCSprite*	pMapcell = CCSprite::create ( ccszf ( "Image %d at frame 10.jpg", i )  );
		pMapcell->setScale ( GAMESCALE );
		pMapcell->setTag ( 11169 );
		pMapcell->getTexture ( )->setAliasTexParameters ( );

		CCSize		tSizeCell = pMapcell->getTextureRect ( ).size;
		fOffsetX += tSizeCell.cx / 2 * GAMESCALE;
		KDfloat		y = tSize.cy / 2;
		if ( tSizeCell.cy != 214.5 )
		{
			y = tSize.cy / 2 + ( tSizeCell.cy - 214.5f ) / 2 * GAMESCALE;
			if ( nIndex == 4 ) 
			{
				y += 50 * GAMESCALE;
			}
		}
	   
		pMapcell->setPosition ( ccp ( fOffsetX, y ) );
		pMapcell->setTag ( nIndex );
		nIndex++;
		m_pGameBG->addChild ( pMapcell );
		fOffsetX += tSizeCell.cx / 2 * GAMESCALE;
	}

	m_pEnemies = CCArray::create ( );
	m_pEnemies->retain ( );

	for ( KDint i = 0; i < 20; i++ )
	{
		Enemy*		pEnemy = Enemy::create ( );
		m_pGameBG->addChild ( pEnemy );
		m_pEnemies->addObject ( pEnemy );
		
		pEnemy->setPosition ( ccp ( l_aEnemiesPos [ i ], l_aMapShape [ (KDint) l_aEnemiesPos [ i ] ] ) );
		pEnemy->setTag ( i + 10 );		
	}

	Tank*	pTank1 = Tank::create ( );
	pTank1->setPosition ( ccp ( 3600, l_aMapShape [ 3600 ] ) );
	m_pGameBG->addChild ( pTank1 );
	m_pEnemies->addObject ( pTank1 );

	Tank*	pTank2 = Tank::create ( );
	pTank2->setPosition ( ccp ( 3600, l_aMapShape [ 4020 ] ) );
	m_pGameBG->addChild ( pTank2 );
	m_pEnemies->addObject ( pTank2 );

	m_pHero = Hero::getInstance ( );
	m_pHero->setPosition ( ccp ( 80, 120 ) );
	m_pHero->setOpacity ( 0 );
	m_pHero->setScale ( GAMESCALE );
	m_pGameBG->addChild ( m_pHero, 1 );

	m_pTruck = Truck::create ( );
	m_pTruck->setScale ( GAMESCALE );
	m_pTruck->setPosition ( ccp ( -150, 170 ) );
	m_pGameBG->addChild ( m_pTruck, 2 );

	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );

	pSE->preloadBackgroundMusic ( "Sound 13 at frame 10.mp3" );
	pSE->preloadEffect ( "Sound 1 at frame 10.mp3" );	
	pSE->preloadEffect ( "Sound 20 at frame 10.mp3" );
	pSE->preloadEffect ( "Sound 21 at frame 10.mp3" );
	pSE->preloadEffect ( "Sound 32 at frame 10.mp3" );

	m_pGameTouches = CCArray::create ( );
	m_pGameTouches->retain ( );

	m_nIndextruck	= 0;	
	m_fAccellX		= 0;
	m_fMapOffset	= 0;

	return KD_TRUE;
}

KDvoid Game::onEnterTransitionDidFinish ( KDvoid )
{
	CCLayer::onEnterTransitionDidFinish ( );

	m_pTruck->runAction 
	(
		CCSequence::create
		(
			CCEaseIn	::create ( CCMoveTo::create ( 1, ccp ( 190, 160 ) ), 1 ),
			CCDelayTime	::create ( 0.6f ),
			CCDelayTime	::create ( 0.6f ),
			CCDelayTime	::create ( 0.6f ),
			CCEaseOut	::create ( CCMoveTo::create ( 1, ccp ( 700, 160 ) ), 1 ),
			CCCallFunc	::create ( this, callfunc_selector ( Game::truckAnimationDone ) ),
			KD_NULL 
		)
	);

	this->runAction 
	(	
		CCSequence::create
		(
			CCCallFunc	::create ( this, callfunc_selector ( Game::truckAnimationDone ) ),
			CCDelayTime	::create ( 0.6f ),
			CCCallFunc	::create ( this, callfunc_selector ( Game::truckAnimationDone ) ),
			CCDelayTime	::create ( 0.6f ),
			CCCallFunc	::create ( this, callfunc_selector ( Game::jumpOutAnimationDone ) ),
			CCDelayTime	::create ( 0.6f ),
			CCDelayTime	::create ( 0.6f ),
			CCDelayTime	::create ( 0.6f ),
			CCCallFunc	::create ( this, callfunc_selector ( Game::truckAnimationDone ) ),
			KD_NULL
		)		
	);

	this->scheduleUpdate ( );
}

KDvoid Game::update ( KDfloat fDelta )
{
	CCPoint		tPos = m_pHero->getPosition ( );

	this->ajustmap ( tPos );
	
	if ( m_fAccellX > 0.03f )
	{
		m_pHero->setScaleX ( -GAMESCALE );
		if ( m_fAccellX > 0.08f )
		{
			if ( tPos.x < m_fMapOffset + 465 && l_aMapShape [ (KDint) tPos.x + 5 ] - tPos.y < 20 )
			{
				m_pHero->setPosition ( ccp ( tPos.x + 3, tPos.y ) );
			}
			m_pHero->run ( );
		}
	}
	else if ( m_fAccellX < -0.03f )
	{
		m_pHero->setScaleX ( GAMESCALE );
		if ( m_fAccellX < -0.08 )
		{
			if ( tPos.x > m_fMapOffset + 15 && l_aMapShape [ (KDint) tPos.x - 5 ] - tPos.y < 20 )
			{
				m_pHero->setPosition ( ccp ( tPos.x - 3, tPos.y ) );
			}
			m_pHero->run ( );
		}
	}

	if ( m_fAccellX > -0.08f && m_fAccellX < 0.08f )
	{
		m_pHero->stop ( );
	}
	
	KDfloat		fOffsetX = 0;
	tPos = m_pHero->getPosition ( );
	if ( tPos.x - m_fMapOffset > 120 && tPos.x < 3830 )
	{
		fOffsetX = tPos.x- m_fMapOffset - 120;
		m_pGameBG->setPositionX ( m_pGameBG->getPositionX ( ) - fOffsetX );
		m_fMapOffset += fOffsetX;
	}

	tPos = m_pGameBG->getPosition ( );

	CCObject*	pObject;
	CCARRAY_FOREACH ( this->getChildren ( ), pObject )
	{
		CCSprite*	pSprite = (CCSprite*) pObject;

		if ( pSprite->getTag ( ) == 851137 )
		{
			CCPoint		tSPos = pSprite->getPosition ( );

			if ( tSPos.y + 15 < l_aMapShape [ (KDint) ( tSPos.x - tPos.x ) ] + tPos.y ) 
			{			
				pSprite->removeFromParentAndCleanup ( KD_TRUE );
			} 
			else if ( tSPos.x < 0 || tSPos.x > 480 ) 
			{
				pSprite->removeFromParentAndCleanup ( KD_TRUE );
			} 
		}
	}
	
	CCARRAY_FOREACH ( m_pEnemies, pObject )
	{
		Enemy*		pEnemy = (Enemy*) pObject;
		KDfloat		fAbsoluteX = pEnemy->getPositionX ( ) + tPos.x;

		if ( fAbsoluteX < -20 )
		{
			pEnemy->setTag ( -1 );
			pEnemy->removeFromParentAndCleanup ( KD_TRUE );
		}

		if ( fAbsoluteX > 0 && fAbsoluteX < 500 )
		{
			pEnemy->active ( );
		}
	}
}

KDvoid Game::didAccelerate ( CCAcceleration* pAccelerationValue )
{
    m_fAccellX = pAccelerationValue->x;
}

KDvoid Game::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{	
    if ( pTouches->count ( ) == 1 )
	{
		m_pGameTouches->addObject ( pTouches->anyObject ( ) );

		if ( m_pGameTouches->count ( ) == 2 )
		{
			this->jump ( );
			m_pGameTouches->removeAllObjects ( );
		}
    }
	else
    {
		this->jump ( );
    }
}

KDvoid Game::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( m_pGameTouches->count ( ) == 1 )
	{
		CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

		this->fire ( pTouch->getLocation ( ) );
	}

	m_pGameTouches->removeAllObjects ( );
}

KDvoid Game::truckAnimationDone ( KDvoid )
{
	SimpleAudioEngine*	pSE = SimpleAudioEngine::sharedEngine ( );

	switch ( m_nIndextruck )
	{
		case 0 :
			pSE->playEffect ( "Sound 32 at frame 10.mp3" );
			break;

		case 1 :
			pSE->playEffect ( "Sound 21 at frame 10.mp3" );
			break;

		case 2 :
			pSE->playEffect ( "Sound 32 at frame 10.mp3" );
			break;

		case 3 :
			pSE->playBackgroundMusic ( "Sound 13 at frame 10.mp3" );
			m_pTruck->removeFromParentAndCleanup ( KD_TRUE );
			this->setTouchEnabled ( KD_TRUE );
			this->setAccelerometerEnabled ( KD_TRUE );
			break;
	}

	m_nIndextruck++;
}

KDvoid Game::jumpOutAnimationDone ( KDvoid )
{
	m_pHero->setOpacity ( 255 );

	m_pHero->runAction 
	(
		CCSequence::create 
		(
			CCEaseIn	::create ( CCMoveTo::create ( 0.2f, ccp ( 55, 140 ) ), 0.2f ),
			CCEaseOut	::create ( CCMoveTo::create ( 0.2f, ccp ( 35, 90 ) ), 0.2f ),
			CCCallFunc	::create ( m_pHero, callfunc_selector ( Hero::jumpDownFromTruck ) ),
			KD_NULL 
		)
	);
}

KDvoid Game::ajustmap ( CCPoint& tPos )
{
	if ( !m_pHero->isJumping ( ) )
	{
		tPos.y = l_aMapShape [ (KDint) tPos.x ];
	}

	if ( tPos.y > 180 )
	{
		m_pGameBG->setPositionY ( 90 - tPos.y + 90 );
	}
	else if ( tPos.y > 90 )
	{
		if ( tPos.y + m_pGameBG->getPositionY ( ) < 90 )
		{
			m_pGameBG->setPositionY ( 0 );
		}
	}
}

KDvoid Game::jump ( KDvoid )
{
	m_pHero->jump ( l_aMapShape );
}

KDvoid Game::fire ( const CCPoint& tPos )
{
	m_pHero->fire ( );

	CCSprite*	pSpriteBullet = CCSprite::create ( "Image 779 at frame 10.png" );
    CCPoint		pHeroPos = m_pHero->getPosition ( );
	pSpriteBullet->setPosition ( ccp ( pHeroPos.x - m_fMapOffset - 30 * m_pHero->getScaleX ( ), pHeroPos.y + m_pGameBG->getPosition ( ).y + 15 ) );
    pSpriteBullet->setScale ( -m_pHero->getScaleX ( ) );
    this->addChild ( pSpriteBullet, 2 );
	pSpriteBullet->setTag ( 851137 );
	pSpriteBullet->runAction ( CCMoveBy::create ( 0.8f, ccp ( m_pHero->getScaleX ( ) > 0 ? -600 : 600, 0 ) ) );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sound 1 at frame 10.mp3" );
}
