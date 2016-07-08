/* --------------------------------------------------------------------------
 *
 *      File            TestM3G.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2011 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __TestM3G_h__
#define __TestM3G_h__

#include "../TestBasic.h"

#include <XMM3G/M3G.h>

class TestM3G : public TestBasic
{
	public :

		LAYER_NODE_FUNC ( TestM3G );

	protected :		

		KDbool  init ( KDvoid );

		virtual KDvoid  onExit ( KDvoid );

		virtual KDvoid  draw ( KDvoid );

		virtual KDuint  count ( KDvoid );

	protected :

		std::vector<M3GObject3D*>  m_vObjects;
		M3GWorld*                  m_pWorld;
};

#endif
