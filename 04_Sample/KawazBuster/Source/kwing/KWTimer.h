/* --------------------------------------------------------------------------
 *
 *      File            KWTimer.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/03/04
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#ifndef __KWTimer_h__
#define __KWTimer_h__

#undef max

class KWTimer : public CCObject 
{
	public :

		static KWTimer*		create ( KDvoid );

		static KWTimer*		create ( KDint nMax );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDbool		initWithMax ( KDint nMax );

		virtual KDvoid		tick ( KDvoid );

		virtual KWTimer*	play ( KDvoid );

		virtual KWTimer*	stop ( KDvoid );

		virtual KWTimer*	pause ( KDvoid );

		virtual KWTimer*	reset ( KDvoid );

		virtual KDvoid		count ( KDvoid );

		virtual KWTimer*	move ( KDint n );

		virtual KDbool		isOver ( KDvoid );

		virtual KDint		max ( KDvoid );

		virtual KDvoid		set ( KDint nMax );

		virtual KDbool		isLooping ( KDvoid );

		virtual KDvoid		setLooping ( KDbool bLoop );

	private :

		KDint				m_nTime;
		KDint				m_nMax;

		KDbool				m_bFlagLoop;
		KDbool				m_bFlagActive;
};

#endif	// __KWTimer_h__