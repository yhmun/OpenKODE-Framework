/* --------------------------------------------------------------------------
 *
 *      File            TheStack.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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

#ifndef __TheStack_h__
#define __TheStack_h__

class TheStack : public CCNode
{
	public :

		static TheStack*		create ( b2World* pWorld );

	public :

		virtual KDbool			initStackWithWorld ( b2World* pWorld );

	private :

		KDvoid					buildLevel1 ( KDvoid );

		KDvoid					buildLevel2 ( KDvoid );

	protected :

		b2World*				m_pWorld;
        KDint					m_nCurrentLevel;

        KDint					m_nStackLocationX;			// starting location of stack on X
        KDint					m_nStackLocationY;			// starting location of stack on Y

        KDint					m_nStackAdjustmentX;		// use for adjusting on a per level basis
        KDint					m_nStackAdjustmentY;		// use for adjusting on a per level basis
};

#endif	// __TheStack_h__
