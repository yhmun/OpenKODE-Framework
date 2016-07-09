/* --------------------------------------------------------------------------
 *
 *      File            Ch5_Minimap.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __Ch5_Minimap_h__
#define __Ch5_Minimap_h__

#include "Ch4_TopDownIsometric.h"
#include "Libraries/Minimap.h"

class Ch5_Minimap : public Ch4_TopDownIsometric
{
	public :

		SCENE_FUNC ( Ch5_Minimap );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		step ( KDfloat fDelta );

		virtual KDvoid		addBrickWallFrom ( CCPoint tPoint1, CCPoint tPoint2, KDfloat fHeight );

	private :

		Minimap*			m_pMinimap;
};

#endif	// __Ch5_Minimap_h__
