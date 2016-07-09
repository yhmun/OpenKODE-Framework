/* -----------------------------------------------------------------------------------
 *
 *      File            CCTableView.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2010      Sangwoo Im.
 *
 *          http://www.cocos2d-x.org
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCTableView_h__
#define __CCTableView_h__

#include "CCScrollView.h"
#include "CCTableViewCell.h"

#include <set>
#include <vector>

NS_CC_EXT_BEGIN

class TableView;
class ArrayForObjectSorting;

/**
 * Sole purpose of this delegate is to single touch event in this version.
 */
class TableViewDelegate : public ScrollViewDelegate
{
	public :

		/**
		 *	Delegate to respond touch event
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is touched
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			tableCellTouched ( TableView* pTable, TableViewCell* pCell ) = 0;

		/**
		 *	Delegate to respond a table cell press event.
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is pressed
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			tableCellHighlight ( TableView* pTable, TableViewCell* pCell ) { }

		/**
		 *	Delegate to respond a table cell release event
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is pressed
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			tableCellUnhighlight ( TableView* pTable, TableViewCell* pCell ) { }

		/**
		 *	Delegate called when the cell is about to be recycled. Immediately
		 *	after this call the cell will be removed from the scene graph and
		 *	recycled.
		 *
		 *	@param table table contains the given cell
		 *	@param cell  cell that is pressed
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			tableCellWillRecycle ( TableView* pTable, TableViewCell* pCell ) { }

};


/**
 *	Data source that governs table backend data.
 */
class TableViewDataSource
{
	public :

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~TableViewDataSource ( KDvoid ) { }

		/**
		 *	cell size for a given index
		 *
		 *	@param idx the index of a cell to get a size
		 *	@return size of a cell at given index
		 */
		virtual Size			tableCellSizeForIndex ( TableView* pTable, KDint32 nIdx )
		{
			return cellSizeForTable ( pTable );
		};

		/**
		 *	cell height for a given table.
		 *
		 *	@param table table to hold the instances of Class
		 *	@return cell size
		 */
		virtual Size			cellSizeForTable ( TableView* pTable )
		{
			return Size::ZERO;
		};

		/**
		 *	a cell instance at a given index
		 *
		 *	@param idx index to search for a cell
		 *	@return cell found at idx
		 */
		virtual TableViewCell*	tableCellAtIndex ( TableView* pTable, KDint32 nIdx ) = 0;

		/**
		 *	Returns number of cells in a given table view.
		 *
		 *	@return number of cells
		 */
		virtual KDint32			numberOfCellsInTableView ( TableView* pTable ) = 0;

};


/**
 *	UITableView support for cocos2d-x.
 *
 *	This is a very basic, minimal implementation to bring UITableView-like component into cocos2d world.
 */
class TableView : public ScrollView, public ScrollViewDelegate
{
	public :
    
		enum class VerticalFillOrder
		{
			TOP_DOWN,
			BOTTOM_UP
		};

		/**
		 *	An intialized table view object
		 *
		 *	@param dataSource data source
		 *	@param size view size
		 *	@return table view
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js:var create(var jsObject,var size)
		 *	in lua:local create(var size)
		 *	in lua:
		 *	@endcode
		 */
		static TableView*		create ( TableViewDataSource* pDataSource, Size tSize );

		/**
		 *	An initialized table view object
		 *
		 *	@param dataSource data source;
		 *	@param size view size
		 *	@param container parent object for cells
		 *	@return table view
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js:var create(var jsObject,var size,var container)
		 *	in lua:local create(var size, var container)
		 *	in lua:
		 *	@endcode
		 */
		static TableView*		create ( TableViewDataSource* pDataSource, Size tSize, Node* pContainer );

		/**
		 *	@js ctor
		 */
		TableView ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TableView ( KDvoid );

		KDbool					initWithViewSize ( Size tSize, Node* pContainer = KD_NULL );

		/**
		 *	data source
		 *	@js NA
		 *	@lua NA
		 */
		TableViewDataSource*	getDataSource ( KDvoid ) { return m_pDataSource; }
		
		/**
		 *	when this function bound to js or lua,the input params are changed
		 *	in js:var setDataSource(var jsSource)
		 *	in lua:local setDataSource()
		 *	@endcode
		 */
		KDvoid					setDataSource ( TableViewDataSource* pSource ) { m_pDataSource = pSource; }
		
		/**
		 *	delegate
		 *	@js NA
		 *	@lua NA
		 */
		TableViewDelegate*		getDelegate ( KDvoid ) { return m_pTableViewDelegate; }

		/**
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js:var setDelegate(var jsDelegate)
		 *	in lua:local setDelegate()
		 *	@endcode
		 */
		KDvoid					setDelegate ( TableViewDelegate* pDelegate ) { m_pTableViewDelegate = pDelegate; }

		/**
		 *	determines how cell is ordered and filled in the view.
		 */
		KDvoid					setVerticalFillOrder ( VerticalFillOrder eOrder );
		VerticalFillOrder		getVerticalFillOrder ( KDvoid );

		/**
		 *	Updates the content of the cell at a given index.
		 *
		 *	@param idx index to find a cell
		 */
		KDvoid					updateCellAtIndex ( KDint32 nIdx );

		/**
		 *	Inserts a new cell at a given index
		 *
		 *	@param idx location to insert
		 */
		KDvoid					insertCellAtIndex ( KDint32 nIdx );

		/**
		 *	Removes a cell at a given index
		 *
		 *	@param idx index to find a cell
		 */
		KDvoid					removeCellAtIndex ( KDint32 nIdx );

		/**
		 *	reloads data from data source.  the view will be refreshed.
		 */
		KDvoid					reloadData ( KDvoid );

		/**
		 *	Dequeues a free cell if available. nil if not.
		 *
		 *	@return free cell
		 */
		TableViewCell*			dequeueCell ( KDvoid );

		/**
		 *	Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
		 *
		 *	@param idx index
		 *	@return a cell at a given index
		 */
		TableViewCell*			cellAtIndex ( KDint32 nIdx );

		// Overrides
		virtual KDvoid			scrollViewDidScroll ( ScrollView* pView ) override;
		virtual KDvoid			scrollViewDidZoom ( ScrollView* pView )  override { }
		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent ) override;
		virtual KDvoid			onTouchCancelled ( Touch* pTouch, Event* pEvent ) override;

	protected :

		TableViewCell*			m_pTouchedCell;
		/**
		 *	vertical direction of cell filling
		 */
		VerticalFillOrder		m_eVordering;

		/**
		 *	index set to query the indexes of the cells used.
		 */
		std::set<KDint32>*		m_pIndices;

		/**
		 *	vector with all cell positions
		 */
		std::vector<KDfloat>	m_aCellsPositions;
		/**
		 *	cells that are currently in the table
		 */
		ArrayForObjectSorting*	m_pCellsUsed;
		/**
		 *	free list of cells
		 */
		ArrayForObjectSorting*	m_pCellsFreed;
		/**
		 *	weak link to the data source object
		 */
		TableViewDataSource*	m_pDataSource;
		/**
		 *	weak link to the delegate object
		 */
		TableViewDelegate*		m_pTableViewDelegate;

		Direction				m_eOldDirection;

		KDint32					__indexFromOffset ( Point tOffset );
		KDint32					_indexFromOffset  ( Point tOffset );
		Point					__offsetFromIndex ( KDint32 nIndex );
		Point					_offsetFromIndex  ( KDint32 nIndex );

		KDvoid					_moveCellOutOfSight ( TableViewCell* pCell );
		KDvoid					_setIndexForCell ( KDint32 nIndex, TableViewCell* pCell );
		KDvoid					_addCellIfNecessary ( TableViewCell* pCell );

		KDvoid					_updateCellPositions ( KDvoid );

	public :

		KDvoid					_updateContentSize ( KDvoid );

};


NS_CC_EXT_END

#endif	// __CCTableView_h__
