/* --------------------------------------------------------------------------
 *
 *      File            GameSprite.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 03/05/2011
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

#ifndef __GameSprite_h__
#define __GameSprite_h__

class GameSprite : public CCSprite 
{
	public :
				 GameSprite ( KDvoid );
		virtual ~GameSprite ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		virtual CCPoint		getLocation ( KDvoid );

		virtual KDvoid		changeSpriteAnimation ( const KDchar* szDirection );
		
		virtual KDvoid		moveSpritePosition ( const CCPoint& tTargetPosition, CCObject* pSender );
		
		virtual KDvoid		updateVertexZ ( const CCPoint& tTilePos, CCTMXTiledMap* pTileMap );
		
		virtual KDbool		checkIfPointIsInSight ( const CCPoint& tTargetPos, GameSprite* pEnemy );
		
		virtual KDvoid		deathSequence ( KDvoid );
		
		virtual KDvoid		cacheFrames ( KDvoid );

		virtual KDvoid		spriteMoveFinished ( CCNode* pSender );

	private :

		KDvoid				deathSpasm ( KDfloat fDelta );

		KDvoid				getPointsOnLine ( KDint nX0, KDint nY0, KDint nX1, KDint nY1, CCArray* pPointsArray );

	public :

		CC_SYNTHESIZE_BOOL ( m_bIsMoving, Moving );
		CC_SYNTHESIZE_BOOL ( m_bAlive	, Alive  );

		CC_SYNTHESIZE ( KDint	,	m_nDeathTurns	, DeathTurns	);
		CC_SYNTHESIZE ( KDint	,	m_nOrderOffsetZ	, OrderOffsetZ	);
		CC_SYNTHESIZE ( KDfloat	,	m_fVelocity		, Velocity		);

		CC_SYNTHESIZE_RETAIN ( CCAnimation*		 , m_pAnimation		 , Animation		);
		CC_SYNTHESIZE_RETAIN ( CCSequence*		 , m_pSpriteRunAction, SpriteRunAction	);
		CC_SYNTHESIZE_RETAIN ( CCSpriteBatchNode*, m_pSpriteSheet	 , SpriteSheet		);

		CC_SYNTHESIZE ( std::string, m_sSpritesheetBaseFilename, SpritesheetBaseFilename );
};

#endif	// __GameSprite_h__
