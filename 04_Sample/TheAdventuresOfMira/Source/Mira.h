/* --------------------------------------------------------------------------
 *
 *      File            Mira.h
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

#ifndef __Mira_h__
#define __Mira_h__

#include "GameCharacter.h"

class Mira : public GameCharacter
{
	public :
				 Mira ( KDvoid );
		virtual ~Mira ( KDvoid );

		static  Mira*		create ( KDvoid );

	public :
    
		virtual KDbool		init ( KDvoid );

		virtual CCRect		adjustedBoundingBox ( KDvoid );

		virtual KDvoid		changeState ( CharacterState eNewState );
	    
		virtual KDvoid		checkAndClampSpritePosition ( KDvoid );

		virtual KDvoid		changeStateToIdle ( KDvoid );
		
		virtual KDvoid		changeStateToSpawning ( KDvoid );
		
		virtual KDvoid		changeStateToFlying ( KDvoid );
		
		virtual KDvoid		changeStateToHit ( KDvoid );
		
		virtual KDvoid		changeStateToDead ( KDvoid );
	    
		KDvoid				changeToVulnerable ( KDvoid );
	    
		KDvoid 				changeToInvulnerable ( KDvoid );
	    
		KDvoid 				maxVelocity ( KDvoid );
	    
		KDvoid 				normalVelocity ( KDvoid );
	    
		KDvoid 				minVelocity ( KDvoid );
	    
		KDvoid 				initAnimations ( KDvoid );
	    			    
		KDvoid 				playSpawningSound ( KDvoid );
	    
		KDvoid 				playHitSound ( KDvoid );
	    
		KDvoid 				playDeadSound ( KDvoid );

	public :
 
		CC_SYNTHESIZE	( CCAction* , m_pFlyingAnimationAction	, FlyingAnimationAction );
		CC_SYNTHESIZE	( CCAction* , m_pHitAnimationAction		, HitAnimationAction	);
		CC_SYNTHESIZE	( CCAction* , m_pDeathAnimationAction	, DeathAnimationAction	);
		CC_SYNTHESIZE	( CCAction* , m_pCurrentAction			, CurrentAction			);
		CC_SYNTHESIZE	( KDbool	, m_bVulnerable				, Vulnerable			);
};

#endif	// __Mira_h__
