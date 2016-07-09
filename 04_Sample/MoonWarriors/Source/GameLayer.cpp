/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors    
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "GameLayer.h"
#include "SysMenu.h"
#include "GameOver.h"
#include "LevelManager.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Ship.h"

GameLayer::GameLayer ( KDvoid )
{
	m_bIsBGSkyReload  = KD_FALSE;
	m_bIsBGTileReload = KD_FALSE;
	m_nTempScore	  = 0;

	Global::nScore = 0;
	Global::nLife  = 4;

	Global::pEnemyContainer   = new CCArray ( );
	Global::pEBulletContainer = new CCArray ( );
	Global::pSBulletContainer = new CCArray ( );
}

GameLayer::~GameLayer ( KDvoid )
{
	CC_SAFE_DELETE ( Global::pEnemyContainer );
	CC_SAFE_DELETE ( Global::pEBulletContainer );
	CC_SAFE_DELETE ( Global::pSBulletContainer );
}

KDbool GameLayer::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	Explosion::sharedExplosion ( );
	Enemy::sharedEnemy ( );
    m_pLevelMgr = new LevelManager ( this );
    this->initBG ( );
    m_tScreenRect = CCRect ( 0, 0, tWinSize.cx, tWinSize.cy + 10 );

	// score
	m_uiScore = CCLabelTTF::create
	(
		"Score : 0", PATH_FONT_DEFAULT, 14, CCSize ( tWinSize.cx / 2, 50 ), kCCTextAlignmentRight
	);
	m_uiScore->setPosition ( ccp ( tWinSize.cx - 100, tWinSize.cy - m_uiScore->getContentSize ( ).cy / 2 - 15 ) );
	this->addChild ( m_uiScore, 1000 );

	// ship life
	CCSprite*  pLife = CCSprite::create ( PATH_IMAGE_SHIP, CCRect ( 0, 0, 60, 38 ) );
	pLife->setScale ( 0.6f );
	pLife->setPosition ( ccp ( 30, 460 ) );
	this->addChild ( pLife, 1 );

	// ship Life count
	m_uiLife = CCLabelTTF::create ( "0", PATH_FONT_DEFAULT, 20 );
	m_uiLife->setPosition ( ccp ( 60, tWinSize.cy - m_uiLife->getContentSize ( ).cy / 2 - 15 ) );
	m_uiLife->setColor ( ccRED );
	this->addChild ( m_uiLife, 1000 );
	
	// ship
	m_pShip = Ship::create ( );
	this->addChild ( m_pShip, m_pShip->getTag ( ), eTagShip );
			
	CCMenuItemLabel*  pBack = CCMenuItemLabel::create 
	(
		CCLabelTTF::create ( "Main menu", PATH_FONT_DEFAULT, 18 ),
		this, menu_selector ( GameLayer::onMainMenu )
	);
	pBack->setAnchorPoint ( ccp ( 0, 0 ) );
	pBack->setPosition ( ccp ( tWinSize.cx - pBack->getContentSize ( ).cx - 5, 5 ) );
	this->addChild ( CCMenu::createWithItem ( pBack ) );

	this->setTouchEnabled  ( KD_TRUE );
	this->setKeypadEnabled ( KD_TRUE );

	this->schedule ( schedule_selector ( GameLayer::onUpdate ) );
	this->schedule ( schedule_selector ( GameLayer::onAddEnemy ), 1 );

	Global::playBGM ( PATH_MUSIC_BG );

	return KD_TRUE;
}

Ship* GameLayer::getShip ( KDvoid )
{
	return m_pShip;
}

KDvoid GameLayer::initBG ( KDvoid )
{
    // bg
    m_uiBGSky = CCSprite::create ( PATH_IMAGE_BG ); 
    m_uiBGSky->setAnchorPoint ( CCPointZero );
    m_fBGSkyHeight = m_uiBGSky->getContentSize ( ).cy;
    this->addChild ( m_uiBGSky, -10 );

	// tilemap
	m_uiBGTile =  CCTMXTiledMap::create ( PATH_TMX_LEVEL );
	this->addChild ( m_uiBGTile, -9 );
	m_fBGTileHeight = m_uiBGTile->getMapSize ( ).cy * m_uiBGTile->getTileSize ( ).cy;

	m_fBGSkyHeight  -= 48;
	m_fBGTileHeight -= 200;

	m_uiBGSky ->runAction ( CCMoveBy::create ( 3, ccp ( 0, -48 ) ) );
	m_uiBGTile->runAction ( CCMoveBy::create ( 3, ccp ( 0, -200 ) ) );

	this->schedule ( schedule_selector ( GameLayer::onMovingBG ), 3 );
}

KDvoid GameLayer::registerWithTouchDispatcher ( KDvoid )
{
	CCDirector::sharedDirector ( )->getTouchDispatcher ( )->addTargetedDelegate ( this, 0, KD_TRUE );
}

KDbool GameLayer::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_tTouch.getID ( ) == -1 )
	{
		m_tTouch = *pTouch;
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid GameLayer::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_tTouch.getID ( ) == pTouch->getID ( ) )
	{
		if ( m_pShip && m_pShip->boundingBox ( ).intersectsRect ( m_tScreenRect ) )
		{
			CCPoint  tDiff = pTouch->getDelta ( );

            m_pShip->setPosition ( ccpAdd ( m_pShip->getPosition ( ), tDiff ) );
        }
	}
}

KDvoid GameLayer::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( m_tTouch.getID ( ) == pTouch->getID ( ) )
	{
		m_tTouch.clear ( );
	}
}

KDvoid GameLayer::keyReleased ( KDint nID )
{
	Global::bKeys [ nID - KD_INPUT_GAMEKEYS_UP ] = KD_FALSE;
}

KDvoid GameLayer::keyPressed  ( KDint nID )
{
	Global::bKeys [ nID - KD_INPUT_GAMEKEYS_UP ] = KD_TRUE;
}

KDvoid GameLayer::onUpdate ( KDfloat fDelta )
{
	this->checkIsCollide ( );
	this->removeInactiveUnit ( fDelta );
	this->updateUI ( );
}

KDvoid GameLayer::onMovingBG ( KDfloat fDelta )
{
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	m_uiBGSky ->runAction ( CCMoveBy::create ( 3, ccp ( 0, -48 ) ) );
	m_uiBGTile->runAction ( CCMoveBy::create ( 3, ccp ( 0, -200 ) ) );

	m_fBGSkyHeight  -= 48;
	m_fBGTileHeight -= 200;

    if ( m_fBGSkyHeight <= tWinSize.cy ) 
	{
        if ( !m_bIsBGSkyReload ) 
		{
			m_uiBGSkyRe = CCSprite::create ( PATH_IMAGE_BG ); 
			m_uiBGSkyRe->setAnchorPoint ( CCPointZero );
			m_uiBGSkyRe->setPosition ( ccp ( 0, tWinSize.cy ) );
			this->addChild ( m_uiBGSkyRe, -10 );
			m_bIsBGSkyReload = KD_TRUE;
        }

		m_uiBGSkyRe->runAction ( CCMoveBy::create ( 3, ccp ( 0, -48 ) ) );
    }

    if ( m_fBGSkyHeight <= 0 ) 
	{
		m_fBGSkyHeight = m_uiBGSky->getContentSize ( ).cy;
		this->removeChild ( m_uiBGSky, KD_TRUE );
		m_uiBGSky   = m_uiBGSkyRe;
		m_uiBGSkyRe = KD_NULL;
		m_bIsBGSkyReload = KD_FALSE;
    }

    if ( m_fBGTileHeight <= tWinSize.cy ) 
	{
        if ( !m_bIsBGTileReload )
		{
			m_uiBGTileRe =  CCTMXTiledMap::create ( PATH_TMX_LEVEL );
			m_uiBGTileRe->setPosition ( ccp ( 0, tWinSize.cy ) );
			this->addChild ( m_uiBGTileRe, -9 );
            m_bIsBGTileReload = KD_TRUE;
        }
        m_uiBGTileRe->runAction ( CCMoveBy::create ( 3, ccp ( 0, -200 ) ) );
    }

    if ( m_fBGTileHeight <= 0 ) 
	{
        m_fBGTileHeight = m_uiBGTileRe->getMapSize ( ).cy * m_uiBGTileRe->getTileSize ( ).cy;
        this->removeChild ( m_uiBGTile, KD_TRUE );
        m_uiBGTile   = m_uiBGTileRe;
        m_uiBGTileRe = KD_NULL;
        m_bIsBGTileReload = KD_FALSE;
    }
}

KDvoid GameLayer::onAddEnemy ( KDfloat fDelta )
{
	m_pLevelMgr->loadLevelResource ( fDelta );
}

KDvoid GameLayer::checkIsCollide ( KDvoid )
{
	Sprite    *pBullet , *pChild;
    CCObject  *pObject1, *pObject2;

	CCARRAY_FOREACH ( Global::pEnemyContainer, pObject1 )
    {
		pChild = (Sprite*) pObject1;
		CCARRAY_FOREACH ( Global::pSBulletContainer, pObject2 )
		{
			pBullet = (Sprite*) pObject2;

			if ( this->collide ( pBullet, pChild ) )
			{
				pBullet->hurt ( );
				pChild ->hurt ( );
			}

			if ( !m_tScreenRect.intersectsRect ( pBullet->boundingBoxToWorld ( ) ) )
			{
				pBullet->destroy ( );
				break;
			}
		}

        if ( m_pShip && this->collide ( pChild, m_pShip ) )
		{
            if ( m_pShip->active ( ) )
			{
                pChild ->hurt ( );
                m_pShip->hurt ( );
            }
        }

		if ( !m_tScreenRect.intersectsRect ( pChild->boundingBoxToWorld ( ) ) )
		{
			pChild->destroy ( );
			break;
		}
	}

	CCARRAY_FOREACH ( Global::pEBulletContainer, pObject1 )
    {
		pChild = (Sprite*) pObject1;

        if ( m_pShip && this->collide ( pChild, m_pShip ) )
		{
            if ( m_pShip->active ( ) )
			{
                pChild ->hurt ( );
                m_pShip->hurt ( );
            }
        }

		if ( !m_tScreenRect.intersectsRect ( pChild->boundingBoxToWorld ( ) ) )
		{
			pChild->destroy ( );
			break;
		}
	}
}

KDvoid GameLayer::checkIsReborn ( KDvoid )
{
	m_pShip = KD_NULL;

	if ( Global::nLife > 0 )
	{
        // ship
		m_pShip = Ship::create ( );
		this->addChild ( m_pShip, m_pShip->getTag ( ), eTagShip );
    }
    else 
	{
		this->runAction ( CCSequence::createWithTwoActions 
		(
			CCDelayTime::create ( 3 ),
			CCCallFuncN::create ( this, callfuncN_selector ( GameLayer::onGameOver ) )
		) );
    }
}

KDvoid GameLayer::removeInactiveUnit ( KDfloat fDelta )
{
	CCArray*    pArray = this->getChildren ( );
	CCObject*  pObject = KD_NULL;
	CCARRAY_FOREACH ( pArray, pObject )
	{
		Sprite*  pChild = dynamic_cast<Sprite*> ( pObject );
		if ( pChild )
		{
			pChild->update ( fDelta );
			if ( !pChild->active ( ) )
			{
				pChild->destroy ( );

				if ( pChild == m_pShip )
				{
					checkIsReborn ( );
				}

				break;
			}
		}
	}
}

KDvoid GameLayer::updateUI ( KDvoid )
{
	if ( m_nTempScore < Global::nScore )
	{
		m_nTempScore += 5;
	}

	m_uiLife ->setString ( CCString::createWithFormat ( "%d", Global::nLife )->getCString ( ) );
	m_uiScore->setString ( CCString::createWithFormat ( "Score : %d", m_nTempScore )->getCString ( ) );
}

KDbool GameLayer::collide ( Sprite* pSprite1, Sprite* pSprite2 )
{
	CCRect  tRect1 = pSprite1->collideRect ( );
	CCRect  tRect2 = pSprite2->collideRect ( );
	return tRect1.intersectsRect ( tRect2 );
}

KDvoid GameLayer::onGameOver ( CCNode* pSender )
{
	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( GameOver::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}

KDvoid GameLayer::onMainMenu ( CCObject* pSender )
{
	CCScene*  pScene = CCScene::create ( );
	pScene->addChild ( SysMenu::create ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( CCTransitionFade::create ( 1.2f, pScene ) );
}
