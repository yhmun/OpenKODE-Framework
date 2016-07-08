/* --------------------------------------------------------------------------
 *
 *      File            GestureLine.h
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

#ifndef __GestureLine_h__
#define __GestureLine_h__

class GestureLine : public CCObject
{
	public :

		static  GestureLine*		create ( const CCPoint& tPoint1, const CCPoint& tPoint2 );

	public :		

		virtual KDbool				initWithPoints ( const CCPoint& tPoint1, const CCPoint& tPoint2 );

		virtual	const CCPoint&		getPoint1 ( KDvoid );
		virtual KDvoid				setPoint1 ( const CCPoint& tPoint );

		virtual	const CCPoint&		getPoint2 ( KDvoid );
		virtual KDvoid				setPoint2 ( const CCPoint& tPoint );

	protected :

		CCPoint					m_tPoint1;
		CCPoint					m_tPoint2;
};

#endif	// __GestureLine_h__
