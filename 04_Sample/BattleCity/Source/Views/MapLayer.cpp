/* --------------------------------------------------------------------------
 *
 *      File            MapLayer.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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
#include "MapLayer.h"
#include "Models/Tank90Sprite.h"
#include "Models/TankEnemySprite.h"
#include "StartScene.h"
#include "ScoreScene.h"

MapLayer* MapLayer::create ( KDint nLevel, KDint nStatus, KDint nLife )
{	
	MapLayer*	pRet = new MapLayer ( );

	if ( pRet && pRet->initWithMap ( nLevel, nStatus, nLife ) )
	{
		pRet->autorelease ( );	
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

MapLayer::MapLayer ( KDvoid )
{
	m_nSlow			= 0;
	m_nQuike		= 0;
	m_nStrong		= 0;
	m_nStrongYe		= 0;
	m_nStrongG		= 0;
	m_nBornNum		= 0;
	m_pAiDict		= KD_NULL;
	m_pEnemyArray	= KD_NULL;
	m_pPointArray	= KD_NULL;
	m_pPropArray	= KD_NULL;
	m_bIsGameOver	= KD_FALSE;
}

MapLayer::~MapLayer ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAiDict );
	CC_SAFE_RELEASE ( m_pEnemyArray );
	CC_SAFE_RELEASE ( m_pPointArray );
	CC_SAFE_RELEASE ( m_pPropArray );
}

KDbool MapLayer::initWithMap ( KDint nLevel, KDint nKind, KDint nLife )
{
	if ( !CCLayerColor::initWithColor ( ccc4 ( 0, 0, 0, 255) ) )
	{
		return KD_FALSE;
	}	

	const CCSize&	tLyrSize = CCEGLView::sharedOpenGLView ( )->getVisibleSize ( );	

	m_nLevel = nLevel;
	m_bIsGameOver = KD_FALSE;

	m_pMap = CCTMXTiledMap::create ( ccszf ( "Images/map%d.tmx", nLevel ) );

	KDfloat		fBig = tLyrSize.cy / m_pMap->getContentSize ( ).cy;
	m_pMap->setScale ( fBig );

	m_pBGLayer1 = m_pMap->layerNamed ( "bg1" );
	m_pBGLayer2 = m_pMap->layerNamed ( "bg2" );
	m_pBGLayer2->setVisible ( KD_FALSE );

	m_pObjects = m_pMap->objectGroupNamed ( "objects" );
	
	m_pHome	= CCSprite::create ( "Images/home.png" );

	CCPoint		tHomePoint = this->objectPosition ( m_pObjects, "home" );
	CCSize		tHomeSize = m_pHome->getContentSize ( );
	
	m_pHome->setPosition ( ccp ( tHomePoint.x + tHomeSize.cx / 2, tHomePoint.y + tHomeSize.cy / 2 ) );
	m_tHomeRect = CCRect ( tHomePoint.x - tHomeSize.cx / 2, 
						   tHomePoint.y - tHomeSize.cy / 2,
						   tHomeSize.cx, 
						   tHomeSize.cy );

	m_pMap->addChild ( m_pHome, -1 );

	this->changeWidthAndHeight ( tLyrSize.cy, tLyrSize.cy );
	this->setPosition ( ccp ( ( tLyrSize.cx - tLyrSize.cy ) / 2, 0 ) );
	this->addChild ( m_pMap );

	m_pTank = Tank90Sprite::create ( this, nLife, (Tank90Kind) nKind, m_pBGLayer1->getContentSize ( ) );                       

	CCPoint		tTPition = this->objectPosition ( m_pObjects, "pl1" );
	CCSize		tTankSize = m_pTank->boundingBox ( ).size;
	m_pTank->setPosition ( ccp ( tTPition.x + tTankSize.cx / 2, tTPition.y + tTankSize.cy / 2 ) );
	m_pTank->setBornPosition ( m_pTank->getPosition ( ) );
	m_pTank->setHomeRect ( m_tHomeRect );
	m_pMap->addChild ( m_pTank, -1, 100 );

	m_nEnemyNum = 1;
	m_pEnemyArray = CCArray::create ( );
	m_pEnemyArray->retain ( );

	this->initAIPlistFile ( );
	this->schedule ( schedule_selector ( MapLayer::initEnemys ), 2 );

	m_pPointArray = CCArray::create ( );
	m_pPointArray->retain ( );

	m_pPropArray  = CCArray::create ( );
	m_pPropArray->retain ( );

	m_nRodamPoint = 10;
    
    for ( KDint i = 1; i <= m_nRodamPoint; i++ )
	{
        CCPoint		tPoint = this->objectPosition ( m_pObjects, ccszf ( "t%d", i ) );
		m_pPointArray->addObject ( CCValue<CCPoint>::create ( tPoint ) );
    }

	return KD_TRUE;
}

KDvoid MapLayer::initEnemys ( KDfloat fDelta )
{
	if ( m_nEnemyNum > 20 && m_pEnemyArray->count ( ) == 0 )
	{
		this->unschedule   ( schedule_selector ( MapLayer::initEnemys ) );
		this->scheduleOnce ( schedule_selector ( MapLayer::gotoScoreScene ), 3.f );		
	}

	if ( m_nEnemyNum > 20 )
	{
		return;
	}

	if ( m_pEnemyArray->count ( ) >= 4 )
	{
		return;
	}
    
	CCString*	pTankKind =	(CCString*) m_pAiDict->objectForKey ( ccszf ( "%d", m_nEnemyNum++ ) );

	TankEnemySprite*	pEnemy = TankEnemySprite::create ( pTankKind->intValue ( ) );
	pEnemy->setMapSize ( m_pBGLayer1->getContentSize ( ) );
	pEnemy->setDelegate ( this );
	pEnemy->setTank ( m_pTank );
	pEnemy->setHomeRect ( m_tHomeRect );

	if ( m_nBornNum == 3 )
	{
		m_nBornNum = 0;
	}
	
	KDint		nRandom = m_nBornNum;
	m_nBornNum++;

	CCPoint		tPoint;

	switch ( nRandom )
	{
		case  0 : tPoint = this->objectPosition ( m_pObjects, "en1" ); break;
		case  1 : tPoint = this->objectPosition ( m_pObjects, "en2" ); break;
		case  2 : 
		default : tPoint = this->objectPosition ( m_pObjects, "en3" ); break;
	}

	CCSize		tSize = pEnemy->boundingBox ( ).size;
	pEnemy->setPosition ( ccp ( tPoint.x + tSize.cx / 2, tPoint.y + tSize.cy / 2 ) );

	m_pMap->addChild ( pEnemy, -1 );
	m_pEnemyArray->addObject ( pEnemy );

	m_pDelegate->bornEnmey ( );
}

KDvoid MapLayer::setDelegate ( MapLayerDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

Tank90Sprite* MapLayer::getTank ( KDvoid )
{
	return m_pTank;
}

KDvoid MapLayer::initAIPlistFile ( KDvoid )
{
	CCDictionary*	pDict = CCDictionary::createWithContentsOfFile ( "AI/AI.plist" );

	m_pAiDict = (CCDictionary*) pDict->objectForKey ( ccszf ( "leve%d", m_nLevel ) );
	m_pAiDict->retain ( );
}

KDvoid MapLayer::upQutoRemoveTool ( KDvoid )
{
	this->unschedule ( schedule_selector ( MapLayer::autoRemoveTool ) );
	this->scheduleOnce ( schedule_selector ( MapLayer::autoRemoveTool ), 10 );
}

KDvoid MapLayer::autoRemoveTool ( KDfloat fDelta )
{
	ToolSprite*		pSprite = (ToolSprite*) m_pPropArray->lastObject ( );
	pSprite->removeFromParent ( );
	m_pPropArray->removeAllObjects ( );
}

CCPoint MapLayer::objectPosition ( CCTMXObjectGroup* pGroup, const KDchar* szObject )
{
	CCDictionary*	pDict = pGroup->objectNamed ( szObject );

	return ccp ( pDict->valueForKey ( "x" )->intValue ( ), 
				 pDict->valueForKey ( "y" )->intValue ( ) );
}

KDvoid MapLayer::gotoScoreScene ( KDfloat fDelta )
{
	CCScene*	pScoreScene = ScoreScene::scene ( m_nSlow, m_nQuike, m_nStrong, m_nStrongYe, m_nStrongG, m_nLevel, m_pTank->getKind ( ), m_pTank->getLife ( ) );
	CCDirector::sharedDirector ( )->replaceScene ( pScoreScene );
}

KDvoid MapLayer::returnMainScene ( KDfloat fDelta )
{
	CCDirector::sharedDirector ( )->replaceScene ( StartScene::scene ( ) );
}

CCPoint MapLayer::tileCoordinateFromPosition ( const CCPoint& tPos )
{
    KDint			nCOX, nCOY;
    const CCSize&	tLayerSize = m_pBGLayer1->getLayerSize ( );
	const CCSize&	tTileSize  = m_pMap->getTileSize ( );
    
    nCOX = tPos.x / tTileSize.cx;
    nCOY = tLayerSize.cy - tPos.y / tTileSize.cy;
    if ( ( nCOX >= 0 ) && ( nCOY < tLayerSize.cx ) && ( nCOY >= 0 ) && ( nCOY < tLayerSize.cy ) )
	{
        return  ccp ( nCOX, nCOY );
    }
	else 
	{
        return ccp ( -1, -1 );
    }
}

KDuint MapLayer::tileIDFromPosition ( const CCPoint& tPos )
{
    CCPoint		tCPT = this->tileCoordinateFromPosition ( tPos );

	if ( tCPT.x < 0 ) return -1;
	if ( tCPT.y < 0 ) return -1;

	if ( tCPT.x >= m_pBGLayer1->getLayerSize ( ).cx ) return -1;
	if ( tCPT.y >= m_pBGLayer1->getLayerSize ( ).cy ) return -1;
    
    return m_pBGLayer1->tileGIDAt ( tCPT );
}

KDuint MapLayer::tileIDFromPositionLayer2 ( const CCPoint& tPos )
{
    if ( !m_pBGLayer2->isVisible ( ) )
	{
		return 0;
	}

	CCPoint		tCPT = this->tileCoordinateFromPosition ( tPos );

	if ( tCPT.x < 0 ) return -1;
	if ( tCPT.y < 0 ) return -1;

	if ( tCPT.x >= m_pBGLayer2->getLayerSize ( ).cx ) return -1;
	if ( tCPT.y >= m_pBGLayer2->getLayerSize ( ).cy ) return -1;

    return m_pBGLayer2->tileGIDAt ( tCPT );
}

KDvoid MapLayer::destroyTile ( const CCPoint& tPos )
{
	CCPoint		tCPT = this->tileCoordinateFromPosition ( tPos );

	m_pBGLayer1->setTileGID ( 0, tCPT );
}

KDvoid MapLayer::initButtlesDidFinish ( Tank90Sprite* pTank, CCSprite* pButtle1, CCSprite* pButtle2 )
{
	m_pMap->addChild ( pButtle1 );
	m_pMap->addChild ( pButtle2 );
}

KDvoid MapLayer::initButtleDidFinish ( Tank90Sprite* pTank, CCSprite* pButtle )
{
	m_pMap->addChild ( pButtle );
}

KDint MapLayer::tileIDFromPosition ( const CCPoint& tPos, Tank90Sprite* pTank )
{
	return this->tileIDFromPosition ( tPos );
}

KDint MapLayer::tileIDFromPositionLayer2 ( const CCPoint& tPos, Tank90Sprite* pTank )
{
	return this->tileIDFromPositionLayer2 ( tPos );
}

CCArray* MapLayer::enemyArray ( Tank90Sprite* pTank )
{
	return m_pEnemyArray;
}

KDvoid MapLayer::gameOver ( Tank90Sprite* pTank )
{
	this->unschedule ( schedule_selector ( MapLayer::initEnemys ) );
    
    if ( m_bIsGameOver )
	{
		return;
	}

	CCSize			tLyrSIze = m_pBGLayer1->getContentSize ( );
  
	CCTexture2D*	pNewTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "Images/home2.png" );
	m_pHome->setTexture ( pNewTexture );

	CCSprite*		pGameSprite = CCSprite::create ( "Images/gamedone.png" );
	pGameSprite->setScale ( 8 );
	pGameSprite->setPosition ( ccp ( tLyrSIze.cx / 2, -10 ) );
	pGameSprite->runAction ( CCMoveTo::create ( 4, ccpMid ( tLyrSIze ) ) );
	m_pMap->addChild ( pGameSprite, 2 );

	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/gameover.wav" );

	this->schedule ( schedule_selector ( MapLayer::returnMainScene ), 5 );

	m_bIsGameOver = KD_TRUE;
}

KDvoid MapLayer::createTool ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/sound3.wav" );

	KDint	nRodam = kdRand ( ) % 6 + 1;

	ToolSprite*		pPropSprite;

	if ( m_pPropArray->count ( ) != 0 )
	{
		pPropSprite = (ToolSprite*) m_pPropArray->lastObject ( );

		pPropSprite->removeFromParent ( );

		m_pPropArray->removeAllObjects ( );
	}

	pPropSprite = ToolSprite::create ( (ToolSpriteKind) nRodam );

	CCValue<CCPoint>*	pPoint = (CCValue<CCPoint>*) m_pPointArray->lastObject ( );
	CCPoint				tPoint = pPoint->getValue ( );
	CCSize				tSize  = pPropSprite->getContentSize ( );
	pPropSprite->setPosition ( ccp ( tPoint.x + tSize.cx / 2, tPoint.y + tSize.cy / 2 ) );
	
	m_pMap->addChild ( pPropSprite, -1 );
	m_pPointArray->removeLastObject ( );
	m_pPropArray->addObject ( pPropSprite );

	this->upQutoRemoveTool ( );
}

CCArray* MapLayer::toolsArray ( Tank90Sprite* pTank )
{
	return m_pPropArray;
}

KDvoid MapLayer::removeTool ( ToolSprite* pTool )
{
	m_pMap->removeChild ( pTool );
}

KDvoid MapLayer::destroyTile ( const CCPoint& tPos, Tank90Sprite* pTank )
{
	this->destroyTile ( tPos );	
}

KDvoid MapLayer::plusBoon ( Tank90Sprite* pTank )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pEnemyArray, pObject )
	{
		TankEnemySprite*	pTankSprite = (TankEnemySprite*) pObject;

		pTankSprite->animationBang ( );
		pTankSprite->scheduleOnce ( schedule_selector ( TankEnemySprite::removeSelfFromMap ), 0.3f );
	}

	m_pEnemyArray->removeAllObjects ( );
}

KDvoid MapLayer::changeLife ( Tank90Sprite* pTank )
{
	m_pDelegate->changeTankLife ( pTank->getLife ( ) );
}

KDvoid MapLayer::plusPass ( Tank90Sprite* pTank )
{
	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pEnemyArray, pObject )
	{
		TankEnemySprite*	pTankSprite = (TankEnemySprite*) pObject;

		pTankSprite->stopTankAction ( );
		pTankSprite->scheduleOnce ( schedule_selector ( TankEnemySprite::initAction ), 5 );
	}
}

Tank90Sprite* MapLayer::tankFromMap ( Tank90Sprite* pTank )
{
	return m_pTank;
}

KDvoid MapLayer::removeSprite ( Tank90Sprite* pTank_ )
{
    TankEnemySprite*	pTank = (TankEnemySprite*) pTank_;   

	switch ( pTank->getEnemyKindForScore ( ) )
	{
		case eSlow :
		case eSlowR :
			m_nSlow++;
			break;

		case eQuike :
		case eQuikeR :
			m_nQuike++;
			break;

		case eStrong :
		case eStrongRed :
			m_nStrong++;
			break;

		case eStrongYellow :
			m_nStrongYe++;
			break;

		case eStrongRedLife3 :
		case eStrongGreen :
			m_nStrongG++;
			break;
	}
    
    m_pEnemyArray->removeObject ( pTank );
    pTank->stopTankAction ( );
	pTank->scheduleOnce ( schedule_selector ( TankEnemySprite::removeSelfFromMap ), 0.3f );
}

CCSprite* MapLayer::home ( KDvoid )
{
	return m_pHome;
}

KDvoid MapLayer::homeProtect ( KDbool bIsProtect, Tank90Sprite* pTank )
{
	m_pBGLayer2->setVisible ( bIsProtect );
}

