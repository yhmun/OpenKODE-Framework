/* --------------------------------------------------------------------------
 *
 *      File            TestChipmunk2.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2012 XMSoft.
 *      Copyright (C) 2010-2013 cocos2d-x.org. All rights reserved.
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __TestChipmunk2_h__
#define __TestChipmunk2_h__

#include "../TestBasic.h"
#include "TestChipmunk/DrawSpace.h"

typedef cpSpace* (*demoInitFunc)	( KDvoid );
typedef KDvoid	 (*demoUpdateFunc)	( KDint  );
typedef KDvoid	 (*demoDestroyFunc)	( KDvoid );

typedef struct  
{
	const KDchar*       name;
	drawSpaceOptions*   drawOptions;
	demoInitFunc        initFunc;
	demoUpdateFunc      updateFunc;
	demoDestroyFunc     destroyFunc;
} chipmunkDemo;

static inline cpFloat frand ( KDvoid )
{
	return (cpFloat) kdRand ( ) / (cpFloat) KD_RAND_MAX;
}

#define GRABABLE_MASK_BIT		( 1 << 31 )
#define NOT_GRABABLE_MASK		( ~GRABABLE_MASK_BIT )

extern cpVect					arrowDirection;
extern KDchar					messageString[1024];


class TestChipmunk2 : public TestBasic
{
	public :

		CREATE_FUNC ( TestChipmunk2 );

	protected :		

		virtual KDuint			count	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			draw	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

		KDvoid					step	 ( KDfloat fDelta );		

	private :

		chipmunkDemo*			m_pCurDemo;
		cpSpace*				m_pSpace;
};

#endif	// __TestChipmunk2_h__