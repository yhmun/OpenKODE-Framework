/* --------------------------------------------------------------------------
 *
 *      File            GameObstacle.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/10
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

#ifndef __GameObstacle_h__
#define __GameObstacle_h__

#include "Mira.h"

class GameObstacle : public GameCharacter
{
	public :
				 GameObstacle ( KDvoid );
		virtual ~GameObstacle ( KDvoid );

		static GameObstacle*	create ( GameObjectType eGameObjectType );

	public :

   
    
		virtual KDbool		init ( GameObjectType eType );

		virtual KDvoid		update ( KDfloat fDelta );

		virtual KDvoid		changeState ( CharacterState eNewState );

		virtual CCRect		adjustedBoundingBox ( KDvoid );
	    
		virtual KDvoid		checkAndClampSpritePosition ( KDvoid );

		virtual KDvoid		changeStateToIdle ( KDvoid );
		
		virtual KDvoid		changeStateToSpawning ( KDvoid );
		
		virtual KDvoid		changeStateToFlying ( KDvoid );
		
		virtual KDvoid		changeStateToHit ( KDvoid );
		
		virtual KDvoid		changeStateToDead ( KDvoid );
	    
		KDvoid				initAnimations ( KDvoid );
	    
		KDvoid				playSpawningSound ( KDvoid );
	    
		KDvoid				playHitSound ( KDvoid );
	    
		KDvoid				playDeadSound ( KDvoid );    

	public :

		CC_SYNTHESIZE	( KDfloat  , m_fAdjustedVelocity, AdjustedVelocity );
		CC_SYNTHESIZE	( CCAction*, m_pSpawningAnimationAction, SpawningAnimationAction );
		CC_SYNTHESIZE	( CCAction*, m_pFlyingAnimationAction, FlyingAnimationAction );
		CC_SYNTHESIZE	( CCAction*, m_pHitAnimationAction, HitAnimationAction );

		CC_SYNTHESIZE_PASS_BY_REF	( Mira, miraPlayer, MiraPlayer);    
};

#endif	// __GameObstacle_h__
