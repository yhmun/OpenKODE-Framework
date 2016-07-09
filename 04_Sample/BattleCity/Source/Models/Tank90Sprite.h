/* --------------------------------------------------------------------------
 *
 *      File            Tank90Sprite.h
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

#ifndef __Tank90Sprite_h__
#define __Tank90Sprite_h__

#include "ToolSprite.h"

typedef enum 
{
    eBorn			= 1	,
    ePlusStarOne		,
    ePlusStarTwo		,
    ePlusStarThree		,
} Tank90Kind;

typedef enum 
{    
    eUp				= 1 ,
    eDown				,
    eLeft				,
    eRight				,
    eFire				,
    eStay				,
    ePause				,    
} Tank90Action;

typedef enum 
{    
    eP1				= 0 ,
    eSlow				,
    eQuike				,
    eStrong				,
    eStrongYellow		,
    eStrongRed			,
    eStrongRedLife3		,
    eStrongGreen		,
    eQuikeR				,
    eSlowR				,
} TankKind;

class Tank90SpriteDelegate;

class Tank90Sprite : public CCSprite
{
	public :

		static Tank90Sprite*	create ( Tank90SpriteDelegate* pDelegate, KDint nLife, Tank90Kind eKind, const CCSize& tSize );

	public :

		virtual KDbool			initWithDelegate ( Tank90SpriteDelegate* pDelegate, KDint nLife, Tank90Kind eKind, const CCSize& tSize );

		KDvoid					playMoveVideo ( KDvoid );

		KDvoid					animationBorn ( KDvoid );

		KDvoid					animationBang ( KDvoid );

		KDvoid					animationShield ( KDfloat fDelta );

		KDvoid					check2Tool ( KDvoid );

		KDvoid					checkLife ( KDfloat fDelta );

		virtual KDvoid			checkBang ( KDfloat fDelta );

		virtual KDbool			checkHome ( CCSprite* pButtle );

		KDvoid					onButtle ( KDvoid );

		KDvoid					checkTool ( KDvoid );

		virtual KDvoid			onFire ( KDfloat fDelta = 0 );

		KDvoid					fire ( CCSprite* pButtle, Tank90Action eButtleOrientation );

		KDvoid					changeKind ( ToolSprite* pTool );

		KDbool					checkWall ( CCSprite* pButtle );

		KDbool					checkBound ( CCSprite* pButtle );

		KDbool					checkStrongWall ( CCSprite* pButtle );

		KDbool					checkLayer2 ( CCSprite* pButtle );	

		KDvoid					removeShield ( KDfloat fDelta );

		KDvoid					removeShieldWall ( KDvoid );

		KDvoid					topButtle ( KDvoid );

		KDvoid					moveUp ( KDvoid );
		
		KDvoid					moveDown ( KDvoid );

		KDvoid					moveLeft ( KDvoid );
		
		KDvoid					moveRight ( KDvoid );

		virtual KDbool			checkTankPosition ( KDvoid );

		KDbool					checkPoint ( const CCPoint& tPosition );

		KDvoid					setDelegate ( Tank90SpriteDelegate* pDelegate );

		KDvoid					setMapSize	( const CCSize& tSize );

		KDvoid					setBornPosition ( const CCPoint& tPosition );

		KDvoid					setHomeRect ( const CCRect& tRect );

		KDbool					isHomeProtect ( KDvoid );

		KDbool					isProtect ( KDvoid );

		KDint					getKind ( KDvoid );

		KDvoid					setKind ( KDint nKind );

		KDvoid					setSpeed ( KDfloat fSpeed );

		KDint					getLife ( KDvoid );

		KDvoid					setLife ( KDint nLife );

		KDint					getEnemyKindForScore ( KDvoid );

		CCSprite*				getButtleSprite ( KDvoid );

		KDvoid					setButtleDone ( KDbool bDone );

		KDbool					isRead ( KDvoid );
		
		KDvoid					setRead ( KDbool bIsRead );

	protected :		

		KDvoid					makeCanFire ( KDvoid );	

		KDvoid					canFire2 ( KDfloat fDelta );

	private :

		KDvoid					bronInformation ( KDvoid );

		KDvoid					changeTankStatus ( ToolSprite* pTool );

		KDvoid					checkEnemyKind ( Tank90Sprite* pTank );

		KDvoid					removeHomeProtect ( KDfloat fDelta );

	protected :

		Tank90SpriteDelegate*	m_pDelegate;

		CCArray*				m_pEnemyArray;         
		CCSize					m_tMapSize;
		CCPoint					m_tBornPosition;
		
		KDbool					m_bIsButtle1OnFire;
		KDbool					m_bIsButtle2OnFire;

		KDint					m_nKind;
		KDfloat					m_fSpeed;
		KDint					m_nLife;
		KDint					m_nButtleCount;
		Tank90Action			m_eAction;
		KDbool					m_bIsCanFire;
		KDint					m_nButtleOrientation;
		CCSprite*				m_pButtleSprite;

		Tank90Sprite*			m_pTmpTank;

		KDbool					m_bIsTankDone;
		KDbool					m_bIsHomeDone;
		KDbool					m_bIsHomeProtect;                 
		KDbool					m_bIsRead;
		KDbool					m_bIsProtect;  

		ToolSprite*				m_pTmpTool;
		KDbool					m_bIsButtleDone;
		KDint					m_nEnemyKindForScore;	
		CCRect					m_tHomeRect;
};

class Tank90SpriteDelegate 
{
	public :

		virtual KDvoid			initButtlesDidFinish ( Tank90Sprite* pTank, CCSprite* pButtle1, CCSprite* pButtle2 ) = 0;

		virtual KDvoid			initButtleDidFinish  ( Tank90Sprite* pTank, CCSprite* pButtle ) = 0;

		virtual KDint			tileIDFromPosition ( const CCPoint& tPos, Tank90Sprite* pTank ) = 0;

		virtual KDint			tileIDFromPositionLayer2 ( const CCPoint& tPos, Tank90Sprite* pTank ) = 0;

		virtual CCArray*		enemyArray ( Tank90Sprite* pTank ) = 0;

		virtual KDvoid			destroyTile ( const CCPoint& tPos, Tank90Sprite* pTank ) = 0;

		virtual KDvoid			removeSprite ( Tank90Sprite* pTank ) = 0;

		virtual CCSprite*		home ( KDvoid ) = 0;

		virtual KDvoid			gameOver ( Tank90Sprite* pTank ) = 0;

		virtual KDvoid			createTool ( KDvoid ) = 0;

		virtual CCArray*		toolsArray ( Tank90Sprite* pTank ) = 0;

		virtual KDvoid			removeTool ( ToolSprite* pTool ) = 0;

		virtual KDvoid			homeProtect ( KDbool bIsProtect, Tank90Sprite* pTank ) = 0;

		virtual KDvoid			plusBoon ( Tank90Sprite* pTank ) = 0;

		virtual KDvoid			changeLife ( Tank90Sprite* pTank ) = 0;

		virtual KDvoid			plusPass ( Tank90Sprite* pTank ) = 0;

		virtual Tank90Sprite*	tankFromMap ( Tank90Sprite* pTank ) = 0;
};

#endif	// __Tank90Sprite_h__