/* --------------------------------------------------------------------------
 *
 *      File            Enemy.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2011 Robert Blackwood 01/20/11
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

class Game;

class Enemy : public cpCCSprite
{
	public :

		static Enemy*  create ( Game* pGame );
		static Enemy*  create ( Game* pGame, cpShape* pShape );

	public :

		virtual KDbool  initWithGame ( Game* pGame );  
		virtual KDbool  initWithGame ( Game* pGame, cpShape* pShape ); 

		KDvoid  addDamage ( KDint nDamage );

	protected :

		Game*					m_pGame;
		KDint					m_nDamage;
};

#endif  // __Enemy_h__