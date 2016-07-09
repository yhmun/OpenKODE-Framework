/* --------------------------------------------------------------------------
 *
 *      File            Attacker.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 *
 *      Created By      Loz Archer on 05/05/2011
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

#ifndef __Attacker_h__
#define __Attacker_h__

#include "GameSprite.h"

//extern float *const velocity;

class Attacker : public GameSprite 
{
	public :
				 Attacker ( KDvoid );
		virtual ~Attacker ( KDvoid );

		CREATE_FUNC ( Attacker );	

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			spriteMoveFinished ( CCNode* pSender );

		KDvoid					createPathToPlayer ( KDvoid );

		KDvoid					chasePlayer ( GameSprite* pPlayer );

		KDvoid					updateAStarPath ( KDfloat fDelta );

		KDvoid					followPath ( KDvoid );

		KDvoid					getPath ( CCArray* pTilePositions );

		KDvoid					setPathToAttacker ( CCArray* pAttackerPath );

	public :

		CC_SYNTHESIZE_BOOL ( m_bChasingPlayer, ChasingPlayer );
		CC_SYNTHESIZE_BOOL ( m_bFollowingPath, FollowingPath );

		CC_SYNTHESIZE ( CCPoint, m_tLastPosition, LastPosition );

		CC_SYNTHESIZE_RETAIN ( CCArray*, m_pPath, Path );

	/*
	NSThread *thread;
	*/
};

#endif	// __Attacker_h__