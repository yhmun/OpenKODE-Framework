/* --------------------------------------------------------------------------
 *
 *      File            GameHero.h
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

#ifndef __GameHero_h__
#define __GameHero_h__

enum HeroAct
{
	kUp		= 1,
	kLeft	= 2,
	kRight	= 3,
	kDown	= 4,
	kFire	= 5,
	kStay	= 6,
};

enum ToOrg 
{
    ku		= 1,
    kl		= 2,
    kr		= 3,
    kd		= 4,    
};


class GameHero : public CCSprite 
{
	public :

		static GameHero*	create ( const CCPoint& tPoint, CCLayer* pLayer );

	public :

		virtual KDbool		initAtPoint ( const CCPoint& tPoint, CCLayer* pLayer );

		KDvoid				setTileArray ( CCArray* pTileArray );

		HeroAct				getAct ( KDvoid );
		KDvoid				setAct ( HeroAct eAct );

		KDvoid				moveLeft ( KDvoid );

		KDvoid				moveRight ( KDvoid );		

		KDvoid				moveUp ( KDvoid );

		KDvoid				moveDown ( KDvoid );

		KDvoid				onFire ( CCSprite* pBomb );

		KDvoid				onStay ( KDvoid );

		KDvoid				changWithSpriteFile ( const KDchar* szFilename );

	private :

		KDfloat				m_fSpeed;
		HeroAct				m_eAct;
		ToOrg				m_eOrg;
		CCArray*			m_pTileArray;
		CCPoint				m_tTouchPos;	  
//		NSTimer*			m_pTimer;
};

/*
@property(assign) HeroAct kact;
@property(retain,nonatomic) CCSprite *heroSprite;
@property(retain,nonatomic) NSMutableArray *tileArray;
@property(assign)CGPoint touchPos;
-(id) initAtPoint:(CGPoint) point InLayer:(CCLayer *)layer;
-(void) changWithSpriteFile:(NSString *)fileName;
- (void) MoveLeft;
- (void) MoveRight;
- (void) MoveUp;
- (void) MoveDown;
- (void) OnFire:(CCSprite *)bomb;
- (void) OnStay;
@end

*/
#endif	// __GameHero_h__