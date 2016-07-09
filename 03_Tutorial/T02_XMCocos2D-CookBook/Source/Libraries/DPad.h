/* --------------------------------------------------------------------------
 *
 *      File            DPad.h
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

#ifndef __DPad_h__
#define __DPad_h__

#include "TouchableSprite.h"

class DPad : public TouchableSprite
{
	public :

		static DPad*			create ( KDvoid );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			processTouch ( const CCPoint& tPoint );
		virtual KDvoid			processRelease ( KDvoid );

		virtual const CCPoint&	getPressedVector ( KDvoid );
		virtual KDvoid			setPressedVector ( const CCPoint& tPressedVector );

		virtual KDint			getDirection ( KDvoid );
		virtual KDvoid			setDirection ( KDint nDirection );

	protected :

		CCPoint					m_tPressedVector;
		KDint					m_nDirection;
};

#endif	// __DPad_h__
