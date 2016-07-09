/* --------------------------------------------------------------------------
 *
 *      File            GameLayer.h
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

#ifndef __GameLayer_h__
#define __GameLayer_h__

#include "Global.h"

class Ship;
class LevelManager;

class GameLayer : public CCLayer
{
	public :

		GameLayer ( KDvoid );
		virtual ~GameLayer ( KDvoid );

		CREATE_FUNC ( GameLayer );

	protected :

		virtual KDbool  init ( KDvoid );

		KDvoid  initBG ( KDvoid );

		virtual KDvoid  registerWithTouchDispatcher ( KDvoid );

		virtual KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid	ccTouchMoved ( CCTouch *pTouch, CCEvent* pEvent );
		virtual KDvoid	ccTouchEnded ( CCTouch *pTouch, CCEvent* pEvent );

		virtual KDvoid  keyReleased ( KDint nID );
		virtual KDvoid  keyPressed  ( KDint nID );

		KDvoid  onUpdate   ( KDfloat fDelta );
		KDvoid  onMovingBG ( KDfloat fDelta );
		KDvoid  onAddEnemy ( KDfloat fDelta );

		KDvoid  onGameOver ( CCNode* pSender );
		KDvoid  onMainMenu ( CCObject* pSender );

		KDvoid  checkIsCollide ( KDvoid );
		KDvoid	checkIsReborn ( KDvoid );

		KDvoid	removeInactiveUnit ( KDfloat fDelta );

		KDvoid  updateUI ( KDvoid );

		KDbool  collide ( Sprite* pSprite1, Sprite* pSprite2 );

	public :

		Ship*   getShip ( KDvoid );

	private :

		LevelManager*	m_pLevelMgr;
		Ship*			m_pShip;

		CCSprite*		m_uiBGSky;
		CCSprite*		m_uiBGSkyRe;
		CCTMXTiledMap*	m_uiBGTile;
		CCTMXTiledMap*	m_uiBGTileRe;
		CCLabelTTF*		m_uiScore;
		CCLabelTTF*		m_uiLife;

		KDbool			m_bIsBGSkyReload;
		KDbool			m_bIsBGTileReload;
		KDfloat			m_fBGSkyHeight;
		KDfloat			m_fBGTileHeight;

		KDint			m_nTempScore;
		CCRect			m_tScreenRect;

		CCTouch			m_tTouch;

};

#endif // __GameLayer_h__