/* --------------------------------------------------------------------------
 *
 *      File            Knight.h
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

#ifndef __Knight_h__
#define __Knight_h__

#include "GameSprite.h"

//extern KDfloat*	const	g_pVelocity;

class Knight : public GameSprite 
{
	public :

		CREATE_FUNC ( Knight );	

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			spriteMoveFinished ( CCNode* pSender );

		KDvoid					moveInRandomDirection ( KDvoid );

	public :

		CC_SYNTHESIZE ( CCPoint, m_tLastPosition, LastPosition );
};

#endif	// __Knight_h__
