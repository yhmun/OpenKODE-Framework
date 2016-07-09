/* --------------------------------------------------------------------------
 *
 *      File            GameplayLayer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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

#ifndef __GameplayLayer_h__
#define __GameplayLayer_h__

#include "Mira.h"

class GameplayLayer : public CCLayer, public GameplayLayerDelegate
{
	public :

		CREATE_FUNC ( GameplayLayer );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid		createObjectOfType ( GameObjectType eObjectType, const CCPoint& tSpawnLocation, KDint zValue );

		KDvoid				addObstacle ( KDfloat fDelta );

		KDvoid				startNewGame ( KDvoid );

		KDvoid				launchCollectableItem ( KDfloat fDelta );

		KDvoid				launchGameObstacle ( KDfloat fDelta = 0 );

		KDvoid				initHud ( KDvoid );

		KDvoid				initMiraPlayer ( KDvoid );

		KDvoid				initGameplay ( KDvoid );

		KDvoid				checkCollisions ( KDvoid );

		KDvoid				updateObstacles ( KDvoid );

		KDvoid				updateCollectables ( KDvoid );

		KDvoid				cameraShakeAction ( KDvoid );

		KDvoid				cameraShake ( KDvoid );

		KDvoid				performCleanup ( KDvoid );

		KDvoid				normalVelocity ( KDvoid );	

		KDvoid				changePattern ( KDvoid );

		KDvoid				removeChildSpecified ( CCNode* pSender );

		static KDvoid		drawPoly ( const CCRect& tRect );

		KDvoid				translateMiraPosition ( const CCPoint& tTranslation );

		KDvoid				resetSugarRush ( KDvoid );
		
		KDvoid				activateSugarRush ( KDvoid );

		KDvoid				resetMagnet ( KDvoid );

		KDvoid				activateMagnet ( KDvoid );

	public :

		CC_SYNTHESIZE	( Mira*		, m_pMiraPlayer		 , MiraPlayer		);
		CC_SYNTHESIZE	( CCLayer*	, m_pObstaclesLayer	 , ObstaclesLayer	);
		CC_SYNTHESIZE	( CCLayer*	, m_pItemsLayer		 , ItemsLayer		);
		CC_SYNTHESIZE	( KDint		, m_nDistanceTick	 , DistanceTick		);
	    
		CC_SYNTHESIZE	( KDbool	, m_bSugarRushActive , SugarRushActive	);
		CC_SYNTHESIZE	( KDint		, m_nSugarRushTick	 , SugarRushTick	);
	    
		CC_SYNTHESIZE	( KDbool	, m_bMagnetActive	 , MagnetActive		);
		CC_SYNTHESIZE	( KDint		, m_nMagnetTick		 , MagnetTick		);
	    
		CC_SYNTHESIZE	( CCParticleMeteor*, m_pMiraEmitter, MiraEmitter	);

		CC_SYNTHESIZE	( KDint		, m_nLastYPos		 , LastYPos			);
		CC_SYNTHESIZE	( KDint		, m_nPatternCount	 , PatternCount		);
		CC_SYNTHESIZE	( KDint		, m_nPatternDirection, PatternDirection	);
		CC_SYNTHESIZE	( KDint		, m_nPattern		 , Pattern			);
};

#endif	// __GameplayLayer_h__
