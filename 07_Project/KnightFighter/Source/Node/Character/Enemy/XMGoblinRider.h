/* --------------------------------------------------------------------------
 *
 *      File            XMGoblinRider.h
 *      Description     XMGoblinRider
 *      Author          Pill-Gwan Jeon
 *      Contact         jpg3087@gmail.com
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

#ifndef __XMGoblinRider_h__
#define __XMGoblinRider_h__

#include "XMEnemy.h"

class XMGoblinRider : public XMEnemy
{
	public :

				 XMGoblinRider ( b2World* pWorld, String* sType = nullptr );
		virtual ~XMGoblinRider ( KDvoid );

		static XMGoblinRider*		create ( b2World* pWorld, String* sType = nullptr );

		virtual	KDvoid				playAI ( XMPlayer* pPlayer );

	protected :

		virtual KDbool				init ( KDvoid );
		virtual KDvoid				doAI ( KDfloat fDelta );

		virtual KDbool				shoot ( KDvoid );
		virtual	KDbool				slash ( KDvoid );


		KDvoid						ShootLinear   ( KDvoid );
		KDvoid						ShootDelay    ( KDvoid );
		KDvoid						ShootDiagonalFromTopToBottom		( KDvoid );
		KDvoid						ShootDiagonalFromTopToBottomDelay	( KDvoid );
		KDvoid						ShootDiagonalFromBottomToTop		( KDvoid );
		KDvoid						ShootDiagonalFromBottomToTopDelay	( KDvoid );

		KDvoid						slashWithNotMove		 ( KDfloat fTime = 1.0f );
		KDvoid						slashWithLinearMove		 ( KDfloat fTime = 1.0f );
		KDvoid						slashWithMvoeAndThreehit ( KDfloat fTime = 1.0f );
};


#endif  // __XMGoblinRider_h__