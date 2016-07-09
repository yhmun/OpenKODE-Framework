/* --------------------------------------------------------------------------
 *
 *      File            TankEnemySprite.h
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

#ifndef __TankEnemySprite_h__
#define __TankEnemySprite_h__

#include "Tank90Sprite.h"

class TankEnemySprite : public Tank90Sprite 
{
	public :

		static TankEnemySprite*		create ( KDint nKind );

	public :

		virtual KDbool				initWithKind ( KDint nKind );

		KDvoid						setTank ( Tank90Sprite* pTank );

		virtual KDvoid				checkBang ( KDfloat fDelta );

		KDvoid						stopTankAction ( KDvoid );

		KDvoid						removeSelfFromMap ( KDfloat fDelta );

		virtual KDbool				checkHome ( CCSprite* pButtle );

		KDbool						checkTank ( CCSprite* pButtle );

		virtual KDvoid				onFire ( KDfloat fDelta = 0 );

		virtual KDbool				checkTankPosition ( KDvoid );		

		KDvoid						initAction ( KDfloat fDelta );

	private :		
		
		KDvoid						doRandomAction ( KDfloat fDelta );

		KDvoid						keepMove ( KDfloat fDelta );
		
		KDvoid						rodmoFire ( KDfloat fDelta );
		
	private :

		KDint						m_nScore;
		Tank90Sprite*				m_pTank;
};

#endif	// __TankEnemySprite_h__
