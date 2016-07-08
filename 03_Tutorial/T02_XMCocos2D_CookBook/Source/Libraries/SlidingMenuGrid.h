/* --------------------------------------------------------------------------
 *
 *      File            SlidingMenuGrid.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Brandon Reynolds on 1/9/11.
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

#ifndef __SlidingMenuGrid_h__
#define __SlidingMenuGrid_h__

class SlidingMenuGrid : public CCLayer
{
	public :

		static SlidingMenuGrid*		create ( CCArray* pItems, KDint nCols, KDint nRows, const CCPoint& tPosition, const CCPoint& tPadding );
		static SlidingMenuGrid*		create ( CCArray* pItems, KDint nCols, KDint nRows, const CCPoint& tPosition, const CCPoint& tPadding, KDbool bIsVertical );

	public :

		virtual KDbool				initWithArray ( CCArray* pItems, KDint nCols, KDint nRows, const CCPoint& tPosition, const CCPoint& tPadding, KDbool bIsVertical );

		virtual KDvoid				registerWithTouchDispatcher ( KDvoid );

		virtual KDbool				ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid				ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid				ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );		

		virtual KDvoid				buildGrid ( KDint nCols, KDint nRows );
		virtual KDvoid				buildGridVertical ( KDint nCols, KDint nRows );

		virtual CCMenuItem*			getItemWithinTouch ( CCTouch* pTouch );

		virtual CCPoint				getPositionOfCurrentPageWithOffset ( KDfloat fOffset );
		virtual CCPoint				getPositionOfCurrentPage ( KDvoid );

		virtual KDfloat				getSwipeDeadZone ( KDvoid );
		virtual KDvoid				setSwipeDeadZone ( KDfloat fValue );

		virtual const CCPoint&		getPadding ( KDvoid );
		virtual KDvoid				setPadding ( const CCPoint& tPadding );

		virtual const CCPoint&		getMenuOrigin ( KDvoid );
		virtual KDvoid				setMenuOrigin ( const CCPoint& tMenuOrigin );

		virtual const CCPoint&		getTouchOrigin ( KDvoid );
		virtual KDvoid				setTouchOrigin ( const CCPoint& tTouchOrigin );

		virtual const CCPoint&		getTouchStop ( KDvoid );
		virtual KDvoid				setTouchStop ( const CCPoint& tTouchStop );

		virtual KDint				getPageCount ( KDvoid );
		virtual KDvoid				setPageCount ( KDint nPageCount );

		virtual KDint				getCurrentPage ( KDvoid );
		virtual KDvoid				setCurrentPage ( KDint nCurrentPage );

		virtual KDbool				 isMoving ( KDvoid );
		virtual KDvoid				setMoving ( KDbool bIsMoving );

		virtual KDbool				 isVerticalPaging ( KDvoid );
		virtual KDvoid				setVerticalPaging ( KDbool bValue );

		virtual KDfloat				getMoveDelta ( KDvoid );
		virtual KDvoid				setMoveDelta ( KDfloat fMoveDelta );

		virtual KDfloat				getMoveDeadZone ( KDvoid );
		virtual KDvoid				setMoveDeadZone ( KDfloat fMoveDeadZone );

		virtual KDfloat				getAnimSpeed ( KDvoid );
		virtual KDvoid				setAnimSpeed ( KDfloat fAnimSpeed );

	private :

		KDvoid						moveToCurrentPage ( KDvoid );

	protected :

		tCCMenuState				m_eState;						// State of our menu grid. (Eg. waiting, tracking touch, cancelled, etc)
		CCMenuItem*					m_pSelectedItem;				// Menu item that was selected/active.
		
		CCPoint						m_tPadding;						// Padding in between menu items. 
		CCPoint						m_tMenuOrigin;					// Origin position of the entire menu grid.
		CCPoint						m_tTouchOrigin;					// Where the touch action began.
		CCPoint						m_tTouchStop;					// Where the touch action stopped.
		
		KDint						m_nPageCount;					// Number of pages in this grid.
		KDint						m_nCurrentPage;					// Current page of menu items being viewed.
		
		KDbool						m_bMoving;						// Is the grid currently moving?
		KDbool						m_bVerticalPaging;				// Disabled by default. Allows for pages to be scrolled vertically instead of horizontal.

		KDfloat						m_fMoveDelta;					// Distance from origin of touch and current frame.
		KDfloat						m_fMoveDeadZone;				// Amount they need to slide the grid in order to move to a new page.
		KDfloat						m_fAnimSpeed;					// 0.0-1.0 value determining how slow/fast to animate the paging.
};

#endif	// __SlidingMenuGrid_h__
