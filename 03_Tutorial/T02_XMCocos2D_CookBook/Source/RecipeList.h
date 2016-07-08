/* --------------------------------------------------------------------------
 *
 *      File            RecipeList.h
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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

#ifndef __RecipeList_h__
#define __RecipeList_h__

class RecipeList : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
	public :
		
		SCENE_FUNC ( RecipeList );

	protected :
		
		virtual KDbool				init ( KDvoid );

		virtual KDuint				numberOfCellsInTableView ( CCTableView* pTable );

		virtual CCSize				cellSizeForTable	 ( CCTableView* pTable );
		virtual CCTableViewCell*	tableCellAtIndex	 ( CCTableView* pTable, KDuint uIndex );
		virtual KDvoid				tableCellTouched	 ( CCTableView* pTable, CCTableViewCell* pCell );
		virtual KDvoid				tableCellHighlight	 ( CCTableView* pTable, CCTableViewCell* pCell );
		virtual KDvoid				tableCellUnhighlight ( CCTableView* pTable, CCTableViewCell* pCell );

		virtual KDvoid				scrollViewDidScroll ( CCScrollView* pView );
		virtual KDvoid				scrollViewDidZoom   ( CCScrollView* pView );	

	public :

		KDvoid						showRecipe ( KDuint uIndex );

		KDvoid						showRecipeList ( CCObject* pSender = KD_NULL );

		const KDchar*				getRecipeName ( KDvoid );

		KDvoid						prevCallback ( CCObject* pSender );
		KDvoid						nextCallback ( CCObject* pSender );

	private :

		KDint						m_nSelected;
		CCTableView*				m_pTableView;
};

extern RecipeList*					g_pRecipeList;

#endif // __TestList_h__
