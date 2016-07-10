/* --------------------------------------------------------------------------
 *
 *      File            XMKnightBullet.h
 *      Description     XMKnightBullet
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

#ifndef __XMKnightBullet_h__
#define __XMKnightBullet_h__

#include "XMPlayerBullet.h"

class XMKnightBullet : public XMPlayerBullet
{
	public :

				 XMKnightBullet ( b2World* pWorld );
		virtual ~XMKnightBullet ( KDvoid );

		static XMKnightBullet*	create ( b2World* pWorld );

		virtual KDvoid			setVisible ( KDbool bVisible );

	protected :

		virtual KDbool		init ( KDvoid );
		virtual KDvoid		initVariable ( KDvoid );

		virtual KDvoid		destroy ( KDvoid );
};

#endif  // __XMKnightBullet_h__