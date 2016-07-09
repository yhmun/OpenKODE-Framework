/* --------------------------------------------------------------------------
 *
 *      File            OverScene.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/13
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

#ifndef __OverScene_h__
#define __OverScene_h__

class GameScene;

class OverScene : public CCLayer 
{
	public :
		
		CREATE_FUNC ( OverScene );

	public :

		virtual	KDbool		init ( KDvoid );

		KDvoid				initTop ( KDvoid );

		KDvoid				setGame ( GameScene* pGame );

		KDvoid				onReStart ( CCObject* pSender );

	private :

		GameScene*			m_pGame;
		CCLabelTTF*			m_pTopScore;
		CCLabelTTF*			m_pScore;
};

/*
@class GameScene;
@interface OverScene : CCLayer {
	CCMenu *menu;
	GameScene *game;
	CCLabel *top;
	CCLabel *score;
}

+(id) scene;
@property (nonatomic, retain) CCMenu *menu;
@property (nonatomic, assign) GameScene *game;
@property (nonatomic, assign) CCLabel *top;
@property (nonatomic, assign) CCLabel *score;
-(void) initTop;
@end
*/

#endif	// __OverScene_h__