/* --------------------------------------------------------------------------
 *
 *      File            KWVector.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 10/12/08
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010-2013 Kawaz. All rights reserved.                             
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

#ifndef __KWVector_h__
#define __KWVector_h__

#undef max
#undef min

class KWVector : public CCObject
{
	public :

		static	KWVector*		create ( KDvoid );
		static  KWVector*		create ( const CCPoint& tPoint );

	public :
	
		KDbool			init ( KDvoid );

		KDbool			initWithPoint ( const CCPoint& tPoint );

		const KDchar*	description ( KDvoid );

		KWVector*		set ( const CCPoint& tPoint );

		KWVector*		clone ( KDvoid );

		KWVector*		add ( KWVector* v );

		KWVector*		sub ( KWVector* v );
		
		KDfloat			scalar ( KWVector* v );
		
		KDfloat			cross ( KWVector* v );
		
		KWVector*		scale ( KDfloat n );

		KDfloat			length ( KDvoid );		

		KWVector*		normalize ( KDvoid );
		
		KWVector*		resize ( KDfloat n );

		KDfloat			angle ( KDvoid );

		KWVector*		rotate ( KDfloat deg );
		
		KWVector*		reverse ( KDvoid );
		
		KWVector*		zero ( KDvoid );

		KWVector*		max ( KDfloat max );

		KWVector*		min ( KDfloat min );

		KDbool			isEqual ( KWVector* v );

	public :

		KDfloat			x;
		KDfloat			y;
};


#endif	// __KWVector_h__