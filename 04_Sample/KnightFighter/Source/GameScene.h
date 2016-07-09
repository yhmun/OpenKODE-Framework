/* --------------------------------------------------------------------------
 *
 *      File            GameScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 27/04/2011
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

#ifndef __GameScene_h__
#define __GameScene_h__

#include "HudLayer.h"
#include "Player.h"
#include "Knight.h"
#include "Attacker.h"
#include "Door.h"
#include "Bullet.h"

enum
{
	TileMapNode		= 0,
};

class KnightFight : public CCLayer 
{
	public :

				 KnightFight ( KDvoid );
		virtual ~KnightFight ( KDvoid );

		static  CCScene*	scene ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid				setViewpointCenter ( const CCPoint& tPosition );
		
		KDvoid				setUpHouseContents ( KDvoid );
		
		KDvoid				resetGame ( KDvoid );
		
		KDvoid				loseLife ( KDvoid );
		
		KDvoid				removeSpeedUp ( KDfloat fDelta );
		
		KDvoid				removeTripleShots ( KDfloat fDelta );

		CCPoint				getPlayerSpawn ( KDvoid );

		Player*				getPlayer ( KDvoid );

		KDvoid				spawnAttackers ( KDvoid );

		KDvoid				checkPlayerCollisionWithEnvironment ( KDvoid );

		KDvoid				checkCollisions ( KDvoid );

		KDvoid				checkLinesOfSight ( KDvoid );

		KDvoid				updateAstarPaths ( KDvoid );

		KDvoid				setHudLayer ( HudLayer* pLayer );

		const KDchar*		getHouseContentsAtTilePos ( const CCPoint& tTilePos );

		KDvoid				fireBullet ( const CCPoint& tTarget );

		KDvoid				destroyBullet ( Bullet* pBullet );

		KDvoid				startShootOut ( KDfloat fDelta );		

	private :

		CCTMXLayer*			m_pMeta;
		Player*				m_pPlayer;
		Knight*				m_pKnight;
		CCArray*			m_pAttackers;
		CCArray*			m_pBullets;
		CCArray*			m_pDoors;
		KDfloat				m_fCollisionsTick;
		KDfloat				m_fLinesOfSightTick;
		KDfloat				m_fPositionZTick;
		KDfloat				m_fStarUpdateTick;
		KDfloat				m_fCollisionPlayerEnvironTick;
		KDfloat				m_fTouchTick;
		HudLayer*			m_pHudLayer;
		CCSprite*			m_pPowerUpAlert;
};

/*

#import "cocos2d.h"
#import "Player.h"
#import "Attacker.h"
#import "Bullet.h"
#import "Door.h"
#import "SimpleAudioEngine.h"
#import "Knight.h"
#import "HudLayer.h"




// HelloWorld Layer
@interface KnightFight : CCLayer
{

}

// returns a Scene that contains the HelloWorld as the only child
+(id) scene;


@property (nonatomic, retain) CCTMXLayer *meta;
@property (nonatomic, retain) Player *player;
@property (nonatomic, retain) Knight *knight;
@property (nonatomic, retain) NSMutableArray *attackers;
@property (nonatomic, retain) NSMutableArray *bullets;
@property (nonatomic, retain) NSMutableArray *doors;
@property (nonatomic) float collisionsTick;
@property (nonatomic) float aStarUpdateTick;
@property (nonatomic) float linesOfSightTick;
@property (nonatomic) float zPositionTick;
@property (nonatomic) float collisionPlayerEnvironTick;
@property (nonatomic, retain) CCLayer *hudLayer;
@property (nonatomic, retain) CCSprite *powerUpAlert;
	

@end
*/

#endif	// __GameScene_h__