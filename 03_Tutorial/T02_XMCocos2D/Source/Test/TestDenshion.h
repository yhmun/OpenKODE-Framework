/* --------------------------------------------------------------------------
 *
 *      File            TestDenshion.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __TestDenshion_h__
#define __TestDenshion_h__

#include "../TestBasic.h"

class TestDenshion : public TestBasic, public CCTableViewDataSource, public CCTableViewDelegate
{
	public :

		static TestDenshion*		create ( KDvoid );

	protected :		

		virtual KDbool				init  ( KDvoid );
		virtual KDuint				count ( KDvoid );	

		virtual CCSize				cellSizeForTable	 ( CCTableView* pTable );
		virtual CCTableViewCell*	tableCellAtIndex	 ( CCTableView* pTable, KDuint uIndex );
		virtual KDvoid				tableCellTouched	 ( CCTableView* pTable, CCTableViewCell* pCell );
		virtual KDvoid				tableCellHighlight	 ( CCTableView* pTable, CCTableViewCell* pCell );
		virtual KDvoid				tableCellUnhighlight ( CCTableView* pTable, CCTableViewCell* pCell );

		virtual KDvoid				scrollViewDidScroll ( CCScrollView* pView );
		virtual KDvoid				scrollViewDidZoom   ( CCScrollView* pView );

		virtual KDuint				numberOfCellsInTableView ( CCTableView* pTable );

		virtual const KDchar*		getName ( KDuint uIndex );
		virtual KDvoid				action  ( KDuint uIndex );
};

class TestSimpleSound : public TestDenshion
{
	public :

		virtual KDvoid				onEnter	 ( KDvoid );
		virtual KDvoid				onExit   ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDuint				numberOfCellsInTableView ( CCTableView* pTable );

		virtual const KDchar*		getName ( KDuint uIndex );
		virtual KDvoid				action  ( KDuint uIndex );

	protected :

		KDuint						m_uSoundID;
};

class TestSound : public TestDenshion
{
	public :

		virtual KDvoid				onEnter	 ( KDvoid );
		virtual KDvoid				onExit   ( KDvoid );
		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDuint				numberOfCellsInTableView ( CCTableView* pTable );

		virtual const KDchar*		getName ( KDuint uIndex );
		virtual KDvoid				action  ( KDuint uIndex );

	protected :

		KDvoid						onUpdate ( KDfloat fDelta );
		KDvoid						onSound  ( KDuint uID, KDint uParam );
    
	protected :

		CCLabelTTF*					m_pLabel;
		KDuint						m_aSoundID [ 6 ];
        KDuint						m_nIdxNotify;
        KDuint						m_nIdxSpeed;
};

#endif	// TestDenshion
