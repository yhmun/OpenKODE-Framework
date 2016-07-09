/* --------------------------------------------------------------------------
 *
 *      File            BilliardsTable.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Matsumae Kentaro on 12/03/20
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

#ifndef __BilliardsTable_h__
#define __BilliardsTable_h__

class ContactListener;
class Board;

class BilliardsTable : public CCLayer
{
	public :

		SCENE_FUNC ( BilliardsTable );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			draw ( KDvoid );

		virtual KDvoid			update ( KDfloat fDelta );

		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		KDvoid					initBox2dWorld ( KDvoid );	

		KDvoid					enableBox2dDebugDrawing ( KDvoid );

	protected :

		b2World*				m_pWorld;
		CCBox2DDebugDraw*		m_pDebugDraw;
		ContactListener*		m_pContactListener;
		Board*					m_pBoard;
};

#endif	// __BilliardsTable_h__