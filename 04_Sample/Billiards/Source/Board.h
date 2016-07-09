/* --------------------------------------------------------------------------
 *
 *      File            Board.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/22
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

#ifndef __Board_h__
#define __Board_h__

#define HALL_COUNT			6

class Board : public CCNode 
{
	public :

		static Board*		create ( b2World* pWorld );

	public :

		virtual KDbool		initWithWorld ( b2World* pWorld );

		virtual KDbool		isInHall ( const CCPoint& tPosition );

		virtual KDvoid		resetBalls ( KDvoid );

	private :

		KDvoid				setup ( KDvoid );

		KDvoid				setupAll ( KDbool bSetupAll );

		KDvoid				createStaticBodyWithVertices ( b2Vec2 aVertices [ ], KDint nNumVertices );

	protected :

		b2World*			m_pWorld;
		CCPoint				m_aHalls [ 6 ];
};

#endif	// __Board_h__