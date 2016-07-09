/* --------------------------------------------------------------------------
 *
 *      File            Game.h
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

#ifndef __Game_h__
#define __Game_h__

#include "Hero.h"
#include "Truck.h"
#include "Tank.h"
#include "Enemy.h"

class Game : public CCLayer
{
	public :

		virtual ~Game ( KDvoid );

		SCENE_FUNC ( Game );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		onEnterTransitionDidFinish ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );

		virtual KDvoid		didAccelerate ( CCAcceleration* pAccelerationValue );

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				truckAnimationDone ( KDvoid );

		KDvoid				jumpOutAnimationDone ( KDvoid );

		KDvoid				ajustmap ( CCPoint& tPos );

		KDvoid				jump ( KDvoid );

		KDvoid				fire ( const CCPoint& tPos );

	private :

		CCLayer*			m_pGameBG;
		Hero*				m_pHero;
		Truck*				m_pTruck;		

		KDint				m_nIndextruck;

		KDfloat				m_fMapOffset;
		KDfloat				m_fAccellX;

		CCArray*			m_pGameTouches;
		CCArray*			m_pEnemies;
};

/*

#import <Foundation/Foundation.h>
#import "cocos2d.h"
#import "Hero.h"
#import "Truck.h"
#import "Bullet.h"
#import "Enemy.h"
#import "EnemyBase.h"
#import "Tank.h"

@interface Game : CCLayer
{
    CCLayer *_gameBG;
    Hero *hero;
    Truck *truck;
    int indextruck;
    float accellY;
    
    
    CCSpriteFrame *bulletFrame;
    
    
    NSMutableArray *enemys;
    
}

// returns a CCScene that contains the HelloWorldLayer as the only child
@property (nonatomic, retain)CCLayer *gameBG;
+(CCScene *) scene;


-(void) truckAnimationDone;
-(void) jumpOutAnimationDone;

- (void)update:(float)dt;
- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration;

- (void)ajustmap:(CGPoint *)p;

- (void)jump;
- (void)fire:(CGPoint)p;


@end
*/

#endif	// __Game_h__