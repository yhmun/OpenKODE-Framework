/* --------------------------------------------------------------------------
 *
 *      File            TestChipmunk.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2011 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org     
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __TestChipmunk_h__
#define __TestChipmunk_h__

#include "../TestBasic.h"

struct cpBody;
struct cpSpace;
struct cpShape;

class TestChipmunk : public TestBasic
{
	public :

		CREATE_FUNC ( TestChipmunk );

	protected :		

		virtual	KDvoid			onEnter	 ( KDvoid );
		virtual	KDvoid			onExit   ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual	KDuint			count	 ( KDvoid );

		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			didAccelerate  ( CCAcceleration* pAccelerationValue );

	protected :		

		KDvoid					initPhysics ( KDvoid );

		KDvoid					addNewSpriteAtPosition ( CCPoint tPoint );

		KDvoid					update ( KDfloat fDelta );

		KDvoid					toggleDebugCallback ( CCObject* pSender );

	private :

		CCTexture2D*			m_pSpriteTexture;	// weak ref
		CCPhysicsDebugNode*		m_pDebugLayer;		// weak ref
		cpSpace*				m_pSpace;			// strong ref
		cpShape*				m_pWalls[4];
};

#endif	// __TestChipmunk_h__