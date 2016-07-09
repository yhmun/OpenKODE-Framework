/* --------------------------------------------------------------------------
 *
 *      File            GameScene.cpp
 *
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "GameScene.h"
#include "ControlLayer.h"
#include "PlayActor.h"
#include "Joystick.h"
#include "Monster.h"
#include "Enemy.h"
#include "Global.h"

CCScene* GameScene::createWithLevel ( KDint nLevel )
{
	CCScene*		pScene = CCScene::create ( );
	GameScene*		pGameLayer = new GameScene ( );
	ControlLayer*	pControlLayer = ControlLayer::create ( );
	
	if ( pGameLayer->initWithLevel ( nLevel ) )
	{
		pGameLayer->autorelease ( );

		pScene->addChild ( pGameLayer );		
		pScene->addChild ( pControlLayer, 200 );
	}

	return pScene;
}

KDbool GameScene::initWithLevel ( KDint nLevel )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	m_nLevel = nLevel;

	m_pMap = CCTMXTiledMap::create ( ccszf ( s_MapFormat, m_nLevel ) );
	m_tMapSize = m_pMap->getContentSize ( );
	m_pMap->setAnchorPoint ( ccp ( 0.0f, 1.0f ) );
	m_pMap->setPosition ( 0, m_tMapSize.cy - 20.0f );
	this->addChild ( m_pMap, 0 );

	this->setContentSize ( CCSizeMake ( m_tMapSize.cx, m_tMapSize.cy + 20.0f ) );

	CCSprite*		pBackground = CCSprite::create ( s_MapBg );
	pBackground->setAnchorPoint ( ccp ( 0.0f, 1.0f ) );
	pBackground->setPosition ( ccp ( 0, m_tMapSize.cy - 20.0f ) );
	this->addChild ( pBackground, -1 );

	m_pMetaLayer = m_pMap->layerNamed ( s_MapMetaLayer );
	m_pMetaLayer->setVisible ( KD_FALSE );

	CCTMXObjectGroup*	pObjects = m_pMap->objectGroupNamed ( s_MapObjectLayer );
	CCDictionary*		pSpawnDict = pObjects->objectNamed ( s_MapSpawnPoint );
	CCPoint				tSpawnPoint = ccp ( pSpawnDict->valueForKey ( "x" )->intValue ( ), pSpawnDict->valueForKey ( "y" )->intValue ( ) );
	CCLOG ( "mapSize: ( %f, %f )", m_tMapSize.cx, m_tMapSize.cy );
	CCLOG ( "spawnPos: ( %f, %f )", tSpawnPoint.x, tSpawnPoint.y );

	//Monster* mon = PlayActor::sharedPlayActor ( )->getMonster ( );
	CCTMXLayer*			pTmxLayer = m_pMap->layerNamed ( "map_level_1" );
	CCSprite*			pMonster  = pTmxLayer->tileAt ( tileCoordForPosition ( tSpawnPoint ) );
	//pMonster->initWithFile ( "map_tile.png", CCRectMake ( 164, 326, 75, 72 ) );

	//CCSize contentSize = pMonster->getContentSize ( );
	//CCSize tileSize = m_pMap->getTileSize ( );
	//pMonster->setAnchorPoint ( ccp ( 
	//	0.5f - tileSize.cx / contentSize.cx / 2,
	//	tileSize.cy / 2 / contentSize.cy ) );
	//pMonster->setPosition ( spawnPoint );
//	this->addChild ( pMonster, 0 );

	this->setViewpointCenter ( pMonster->getPosition ( ) );
	this->schedule ( schedule_selector ( GameScene::updatePostion ) );

	return KD_TRUE;
}

KDvoid GameScene::updatePostion ( KDfloat fDelta )
{
	Monster*	pMonster  = PlayActor::sharedPlayActor ( )->getMonster ( );
	Joystick*	pJoystick = PlayActor::sharedPlayActor ( )->getJoyStick ( );
	CCPoint		tDeltaPos = pJoystick->getDirection ( );
	KDint		nVelocity = (KDint) pJoystick->getVelocity ( );

	if ( nVelocity >= pJoystick->getRadius ( ) / 2 )
	{
		// MoveDirection direction;
		tDeltaPos = ccpSub ( CCPointZero, tDeltaPos );

		CCPoint		tNewPos = ccpAdd ( pMonster->getPosition ( ), tDeltaPos );
		pMonster->setPosition ( tNewPos );
		this->setViewpointCenter ( tNewPos );
	}
}

KDvoid GameScene::setTileAntiAltas ( KDvoid )
{
	CCArray*			pChildrenArray = m_pMap->getChildren ( );
	CCSpriteBatchNode*	pChild  = KD_NULL;
	CCObject*			pObject = KD_NULL;
	CCARRAY_FOREACH ( pChildrenArray, pObject )
	{
		pChild = (CCSpriteBatchNode*) pObject;
		if ( !pChild )
		{
			break;
		}

		pChild->getTexture ( )->setAntiAliasTexParameters ( );
	}
}

KDvoid GameScene::setViewpointCenter ( const CCPoint& tPos )
{
	CCSize	tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	KDint	x = (KDint) KD_MAX ( tPos.x, tWinSize.cx / 2 );
	KDint	y = (KDint) KD_MAX ( tPos.y, tWinSize.cy / 2 );

	x = (KDint) KD_MIN ( x, m_tMapSize.cx - tWinSize.cx / 2 );
	y = (KDint) KD_MIN ( y, m_tMapSize.cy - tWinSize.cy / 2 );

	CCPoint		tActualPos    = ccp ( x, y );
	CCPoint		tCenterOfView = ccp ( tWinSize.cx / 2, tWinSize.cy / 2 );
	CCPoint		tViewPoint    = ccpSub ( tCenterOfView, tActualPos );

	this->setPosition ( tViewPoint );
}

CCPoint GameScene::tileCoordForPosition ( const CCPoint& tPos )
{
	KDint	x = (KDint) ( tPos.x / m_pMap->getTileSize ( ).cx );
	KDint	y = (KDint) ( ( m_tMapSize.cy - tPos.y ) / m_pMap->getTileSize ( ).cy );

	return ccp ( x, y );
}

KDvoid GameScene::setJoystickEnable ( KDbool bEnable )
{
	if ( bEnable )
	{
		PlayActor::sharedPlayActor ( )->getJoyStick ( )->active ( );
	}
	else
	{
		PlayActor::sharedPlayActor ( )->getJoyStick ( )->inactive ( );
	}
}