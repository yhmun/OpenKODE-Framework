/* --------------------------------------------------------------------------
 *
 *      File            SampleLayer.h
 *      Description     SampleLayer
 *      Author          J.S. Park
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

#ifndef __SampleLayer_h__
#define __SampleLayer_h__

class SampleLayer : public Layer
{
	public :

		CREATE_FUNC ( SampleLayer );

		KDint				m_nMaxJump;

	protected :

		virtual KDbool		init ( KDvoid );

		KDvoid				initBox2DWorld ( KDvoid );
		KDvoid				initItem ( KDvoid );

		KDvoid				draw ( KDvoid );

		KDvoid				addLevelBoundaries ( KDvoid );
		KDvoid				addCharacter ( KDvoid );
		KDvoid				addBackground ( KDvoid );
		KDvoid				addFloor ( KDvoid );
		

		KDvoid				step ( KDfloat fDelta );

		b2World*			m_pWorld;
		Box2DDebugDraw*		m_pDebugDraw;

		virtual KDbool		onTouchBegan ( Touch* pTouch, Event* pUnusedEvent );
		
		KDvoid				moveBackground ( KDfloat fDelta );

		KDvoid				setItem ( KDvoid );
		KDvoid				makeItem ( KDfloat fDelta );
		KDvoid				removeItem ( Node* pSender );
		KDvoid				eatItem ( Node* pSender );
		KDvoid				collisionCheck ( KDfloat fDelta );
		KDint				m_nItemCount;
		KDint				m_nItemY;


		b2Body*				m_pBody;

		Dictionary*			m_pJellyDictionary;
		Array*				m_pItemArray;
		Array*				m_pTempArray;
		
		
};

#endif  // __SampleLayer_h__