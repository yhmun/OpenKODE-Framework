/* --------------------------------------------------------------------------
 *
 *      File            CCTableView.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sangwoo Im
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

#ifndef __CCTableView_h__
#define __CCTableView_h__

#include "CCScrollView.h"
#include "CCTableViewCell.h"

#include <set>
#include <vector>

NS_CC_BEGIN

class CCTableView;
class CCArrayForObjectSorting;

typedef enum 
{
    kCCTableViewFillTopDown ,
    kCCTableViewFillBottomUp,
} CCTableViewVerticalFillOrder;

/**
 *	Sole purpose of this delegate is to single touch event in this version.
 */
class CCTableViewDelegate : public CCScrollViewDelegate
{
	public :

		/**
		 *	Delegate to respond touch event
		 *
		 *	@param pTable table contains the given cell
		 *	@param pCell  cell that is touched
		 */
		virtual KDvoid		tableCellTouched ( CCTableView* pTable, CCTableViewCell* pCell ) = 0;

		/**
		 *	Delegate to respond a table cell press event.
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is pressed
		 */
		virtual KDvoid		tableCellHighlight ( CCTableView* pTable, CCTableViewCell* pCell ) { };

		/**
		 *	Delegate to respond a table cell release event
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is pressed
		 */
		virtual KDvoid		tableCellUnhighlight ( CCTableView* pTable, CCTableViewCell* pCell ) { };
	    
		/**
		 *	Delegate called when the cell is about to be recycled. Immediately
		 *	after this call the cell will be removed from the scene graph and
		 *	recycled.
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is pressed
		 */
		virtual KDvoid		tableCellWillRecycle ( CCTableView* pTable, CCTableViewCell* pCell ) { };  
};

/**
 *	Data source that governs table backend data.
 */
class CCTableViewDataSource
{
	public :

		virtual ~CCTableViewDataSource ( ) { }

		/**
		 *	cell size for a given index
		 *
		 *	@param idx the index of a cell to get a size
		 *	@return size of a cell at given index
		 */
		virtual CCSize				tableCellSizeForIndex ( CCTableView* pTable, KDuint uIndex )
		{
			return cellSizeForTable ( pTable );
		};

		/**
		 *	cell height for a given table.
		 *
		 *	@param  pTable table to hold the instances of Class
		 *	@return cell size
		 */
		virtual CCSize				cellSizeForTable ( CCTableView* pTable ) 
		{
			return CCSizeZero;
		}

		/**
		 *	a cell instance at a given index
		 *
		 *	@param  uIndex index to search for a cell
		 *	@return cell found at idx
		 */
		virtual CCTableViewCell*	tableCellAtIndex ( CCTableView* pTable, KDuint uIndex ) = 0;

		/**
		 *	Returns number of cells in a given table view.
		 * 
		 *	@return number of cells
		 */
		virtual KDuint				numberOfCellsInTableView ( CCTableView* pTable ) = 0; 
};

/**
 *	UITableView counterpart for Cocos2D for iphone.
 *
 *	this is a very basic, minimal implementation to bring UITableView-like component into Cocos2D world.
 * 
 */
class CCTableView : public CCScrollView, public CCScrollViewDelegate
{
	public :
				 CCTableView ( KDvoid );
		virtual ~CCTableView ( KDvoid );

		/**
		 *	An intialized table view object
		 *
		 *	@param  pDataSource data source
		 *	@param  tSize view size
		 *	@return table view
		 */
		static  CCTableView*			create ( CCTableViewDataSource* pDataSource, const CCSize& tSize );

		/**
		 *	An initialized table view object
		 *
		 *	@param  pDataSource data source;
		 *	@param  tSize view size
		 *	@param  pContainer parent object for cells
		 *	@return table view
		 */
		static  CCTableView*			create ( CCTableViewDataSource* pDataSource, const CCSize& tSize, CCNode* pContainer );
	    
	public :

		virtual KDbool					initWithViewSize ( const CCSize& tSize, CCNode* pContainer = KD_NULL );

		virtual KDvoid					onEnter ( KDvoid );

		/**	data source */
		inline CCTableViewDataSource*	getDataSource ( KDvoid ) { return m_pDataSource; }
		inline KDvoid					setDataSource ( CCTableViewDataSource* pSource ) { m_pDataSource = pSource; }

		/**	delegate */
		inline CCTableViewDelegate*		getDelegate ( KDvoid ) { return m_pTableViewDelegate; } 
		inline KDvoid					setDelegate ( CCTableViewDelegate* pDelegate ) { m_pTableViewDelegate = pDelegate; }

		/**	determines how cell is ordered and filled in the view. */
		KDvoid							setVerticalFillOrder ( CCTableViewVerticalFillOrder eOrder );
		CCTableViewVerticalFillOrder	getVerticalFillOrder ( KDvoid );		

		/**
		 *	Updates the content of the cell at a given index.
		 *
		 *	@param uIdx index to find a cell
		 */
		KDvoid				updateCellAtIndex ( KDuint uIndex );

		/**
		 *	Inserts a new cell at a given index
		 *
		 *	@param uIdx location to insert
		 */
		KDvoid				insertCellAtIndex ( KDuint uIndex );

		/**
		 *	Removes a cell at a given index
		 *
		 *	@param uIdx index to find a cell
		 */
		KDvoid				removeCellAtIndex ( KDuint uIndex );

		/**
		 *  reloads data from data source.  the view will be refreshed.
		 */
		KDvoid				reloadData ( KDvoid );

		/**
		 *  Dequeues a free cell if available. nil if not.
		 *
		 *  @return free cell
		 */
		CCTableViewCell*	dequeueCell ( KDvoid );

		/**
		 *  Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
		 *
		 *  @param  uIdx index
		 *  @return a cell at a given index
		 */
		CCTableViewCell*	cellAtIndex ( KDuint uIndex );

		KDbool				isDidEnter ( KDvoid );

		virtual KDvoid		scrollViewDidScroll ( CCScrollView* pView );
		virtual KDvoid		scrollViewDidZoom   ( CCScrollView* pView );

		virtual KDbool		ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid		ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

	protected :

		KDint				__indexFromOffset ( CCPoint tOffset );
		KDuint				 _indexFromOffset ( CCPoint tOffset );

		CCPoint				__offsetFromIndex ( KDuint uIndex );
		CCPoint				 _offsetFromIndex ( KDuint uIndex );

		KDvoid				_moveCellOutOfSight ( CCTableViewCell* pCell );
		KDvoid				_setIndexForCell ( KDuint uIndex, CCTableViewCell* pCell );
		KDvoid				_addCellIfNecessary ( CCTableViewCell* pCell );

		CCTableViewCell*	getHitCell ( CCTouch* pTouch );

		KDvoid				_updateCellPositions ( KDvoid );
	public :

		KDvoid				_updateContentSize ( KDvoid );

	protected :
	    
		CCTableViewCell*				m_pTouchedCell;
		/** 
		 *	vertical direction of cell filling 
		 */
		CCTableViewVerticalFillOrder	m_eVordering;	 
		/**
		 *	index set to query the indexes of the cells used. 
		 */
		std::set<KDuint>*				m_pIndices;
		/**
		 *	vector with all cell positions
		 */
		std::vector<KDfloat>			m_aCellsPositions;
		/**
		 *	cells that are currently in the table 
		 */
		CCArrayForObjectSorting*		m_pCellsUsed;
		/**
		 *	free list of cells 
		 */
		CCArrayForObjectSorting*		m_pCellsFreed;
		/**
		 *	weak link to the data source object 
		 */
		CCTableViewDataSource*			m_pDataSource;
		/**	
		 *	weak link to the delegate object 
		 */
		CCTableViewDelegate*			m_pTableViewDelegate;
		CCScrollViewDirection			m_eOldDirection;

		KDuint							m_uCellCount;
};

NS_CC_END

#endif	// __CCTableView_h__

