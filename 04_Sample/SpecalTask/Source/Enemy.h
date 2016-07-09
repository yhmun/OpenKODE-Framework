/* --------------------------------------------------------------------------
 *
 *      File            Enemy.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-11-03
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

#ifndef __Enemy_h__
#define __Enemy_h__

#include "EnemyBase.h"

class Enemy : public EnemyBase 
{
	public :

		CREATE_FUNC ( Enemy );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			active ( KDvoid );

		virtual KDbool			beenAttack ( KDint nType );

		virtual KDvoid			fire ( KDvoid );

		KDvoid					runDeadAnimation ( KDvoid );

		KDvoid					checkUpdate ( KDfloat fDelta );

	private :

		CCFiniteTimeAction*		m_pActIdle;
};

#endif	// __Enemy_h__
