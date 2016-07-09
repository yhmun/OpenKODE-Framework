/* --------------------------------------------------------------------------
 *
 *      File            Common.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/10
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

#ifndef __Common_h__
#define __Common_h__

enum BubbleType 
{
	NoType		= 0xFFF00000,
	BlueType	= 0xFFF10000,
	GreenType	= 0xFFF20000,
	RedType		= 0xFFF30000,
	OrangeType	= 0xFFF40000,
	PurpleType	= 0xFFF50000,
};

class Common 
{
	public :

		static Common*		sharedCommon ( KDvoid );

	protected :		

		KDvoid				init ( KDvoid );

	public :

		KDvoid				playOverSound  ( KDvoid );

		KDvoid				playRightSound ( KDvoid );

		KDvoid				playWrongSound ( KDvoid );

		KDvoid				setPlay ( KDbool bPlay );

	public :
	
		KDbool				m_bIsPlay;
};

#endif	// __Common_h__
