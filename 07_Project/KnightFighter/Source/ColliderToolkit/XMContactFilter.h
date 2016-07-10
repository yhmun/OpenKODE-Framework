/* --------------------------------------------------------------------------
 *
 *      File            XMContactFilter.h
 *      Description     XMContactFilter
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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

#ifndef __XMContactFilter_h__
#define __XMContactFilter_h__

class XMContactFilter : public b2ContactFilter
{
	public :

		virtual KDbool			ShouldCollide ( b2Fixture* pFixtureA, b2Fixture* pFixtureB );

	protected :

		b2Fixture*				m_pFixtureA;
		b2Fixture*				m_pFixtureB;

		b2Filter				m_tFilterA;
		b2Filter				m_tFilterB;
};

#endif  // __XMContactFilter_h__