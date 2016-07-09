/* --------------------------------------------------------------------------
 *
 *      File            IPhoneCube3D.h
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

#ifndef __IPhoneCube3D_h__
#define __IPhoneCube3D_h__

#include "Cube3D.h"

class IPhoneCube3D : public Cube3D
{
	public :

		static IPhoneCube3D*		create ( KDvoid );

	public :		

		virtual KDbool				init ( KDvoid );

		virtual KDvoid				draw ( KDvoid );

	protected :

		CCTexture2D*				m_pFront;
		CCTexture2D*				m_pBack;
		CCTexture2D*				m_pSide;
};

#endif	// __IPhoneCube3D_h__
