/* --------------------------------------------------------------------------
 *
 *      File            XMBackground.h
 *      Description     XMBackground
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

#ifndef __XMBackground_h__
#define __XMBackground_h__

class XMBackground : public ParallaxScrollNode
{
	public :
		
		static XMBackground*	create ( KDint nStage );

	protected :

		virtual KDbool			init				( KDint nStage			);

		KDvoid					moveBackground		( KDfloat fDelta		);
		Point					getObjectPosX		( KDint i , KDint nCmp	);
};

#endif  // __XMBackground_h__