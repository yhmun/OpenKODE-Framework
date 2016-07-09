/* --------------------------------------------------------------------------
 *
 *      File            SuperMarioClick.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
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

#ifndef __SuperMarioClick_h__
#define __SuperMarioClick_h__

enum
{
	kTagBatchNode,
};

class ContactListener;

class SuperMarioClick : public CCLayer
{
	public :

		SCENE_FUNC ( SuperMarioClick );

		static SuperMarioClick*		sharedSuperMario ( KDvoid );

		virtual ~SuperMarioClick ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		update ( KDfloat fDelta );		

		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid				initBox2dWorld ( KDvoid );

		KDvoid				enableBox2dDebugDrawing ( KDvoid );

		KDvoid				createMenu ( KDvoid );

		KDvoid				resetGame ( CCObject* pSender );

		CCSpriteBatchNode*	getSpriteBatch ( KDvoid );

		KDvoid				setContactCount ( KDint nCount );

		KDvoid				splashScreen ( KDvoid );

	private :

		CCLabelTTF*			m_pLabel;
		CCLabelTTF*			m_pFighting;
		CCLabelTTF*			m_pMan;

		b2World*			m_pWorld;
		ContactListener*	m_pContactListener;
		b2Body*				m_pContainerBody;
		CCBox2DDebugDraw*	m_pDebugDraw;

		b2Body*				m_pPaddleBody;
		b2Fixture*			m_pPaddleFixture;

		// mouse move joint
		b2MouseJoint*		m_pMouseJoint;

		// contack cound
		KDint				m_nContackCount;
};

#endif	// __SuperMarioClick_h__