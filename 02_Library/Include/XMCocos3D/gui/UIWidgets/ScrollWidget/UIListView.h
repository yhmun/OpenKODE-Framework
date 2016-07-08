/* -----------------------------------------------------------------------------------
 *
 *      File            UIListView.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __UIListView_h__
#define __UIListView_h__

#include "UIScrollView.h"

namespace gui {

typedef enum
{
    LISTVIEW_GRAVITY_LEFT,
    LISTVIEW_GRAVITY_RIGHT,
    LISTVIEW_GRAVITY_CENTER_HORIZONTAL,
    
    LISTVIEW_GRAVITY_TOP,
    LISTVIEW_GRAVITY_BOTTOM,
    LISTVIEW_GRAVITY_CENTER_VERTICAL,
} ListViewGravity;
    
typedef enum
{
    LISTVIEW_ONSELECTEDITEM
} ListViewEventType;

typedef KDvoid (cocos2d::Object::*SEL_ListViewEvent) ( cocos2d::Object*, ListViewEventType );
#define listvieweventselector(_SELECTOR) ( SEL_ListViewEvent ) ( &_SELECTOR )

class UIListView : public UIScrollView
{
	public :
    
		/**
		 *	Default constructor
		 */
		UIListView ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UIListView ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UIListView*			create ( KDvoid );
    
		/**
		 *	Sets a item model for listview
		 *
		 *	A model will be cloned for adding default item.
		 *
		 *	@param model  item model for listview
		 */
		KDvoid						setItemModel ( UIWidget* pModel );
    
		/**
		 *	Push back a default item(create by a cloned model) into listview.
		 */
		KDvoid						pushBackDefaultItem ( KDvoid );
    
		/**
		 *	Insert a default item(create by a cloned model) into listview.
		 */
		KDvoid						insertDefaultItem ( KDint nIndex );
    
		/**
		 *	Push back custom item into listview.
		 */
		KDvoid						pushBackCustomItem ( UIWidget* pItem );
    
		/**
		 *	Insert custom item into listview.
		 */
		KDvoid						insertCustomItem ( UIWidget* pItem, KDint nIndex );
    
		/**
		 *  Removes the last item of listview.
		 */
		KDvoid						removeLastItem ( KDvoid );
    
		/**
		 *	Removes a item whose index is same as the parameter.
		 *
		 *	@param index of item.
		 */
		KDvoid						removeItem ( KDint nIndex );
    
		/**
		 *	Returns a item whose index is same as the parameter.
		 *
		 *	@param index of item.
		 *
		 *	@return the item widget.
		 */
		UIWidget*					getItem ( KDuint uIndex );
    
		/**
		 *	Returns the item container.
		 */
		cocos2d::Array*				getItems ( KDvoid );
    
		/**
		 *	Returns the index of item.
		 *
		 *	@param item  the item which need to be checked.
		 *
		 *	@return the index of item.
		 */
		KDuint						getIndex ( UIWidget* pItem ) const;
    
		/**
		 *	Changes the gravity of listview.
		 *	@see ListViewGravity
		 */
		KDvoid						setGravity ( ListViewGravity eGravity );
    
		/**
		 *	Changes the margin between each item.
		 *
		 *	@param margin
		 */
		KDvoid						setItemsMargin ( KDfloat fMargin );
    
		/**
		 *	Refresh the view of list.
		 *
		 *	If you change the data, you need to call this mathod.
		 */
		KDvoid						refreshView ( KDvoid );
    
		KDint						getCurSelectedIndex ( KDvoid ) const;
    
		KDvoid						addEventListenerListView ( cocos2d::Object* pTarget, SEL_ListViewEvent pSelector );
    
		/**
		 *	Changes scroll direction of scrollview.
		 *
		 *	@see SCROLLVIEW_DIR     SCROLLVIEW_DIR_VERTICAL means vertical scroll, SCROLLVIEW_DIR_HORIZONTAL means horizontal scroll
		 *
		 *	@param SCROLLVIEW_DIR
		 */
		virtual KDvoid				setDirection ( SCROLLVIEW_DIR dir) override;
    
		virtual const KDchar*		getDescription ( KDvoid ) const override;
    
	protected :

		virtual KDbool				addChild ( UIWidget* pWidget ) override { return UIScrollView::addChild ( pWidget ); }
		virtual KDbool				removeChild ( UIWidget* pWidget ) override { return UIScrollView::removeChild ( pWidget ); }
		virtual KDvoid				removeAllChildren ( KDvoid ) override{UIScrollView::removeAllChildren ( ); }
		virtual cocos2d::Array*		getChildren ( KDvoid ) override { return UIScrollView::getChildren ( ); }
		virtual KDbool				init ( KDvoid ) override;
		KDvoid						updateInnerContainerSize ( KDvoid );
		KDvoid						remedyLayoutParameter ( UIWidget* pItem );
		virtual KDvoid				onSizeChanged ( KDvoid ) override;
		virtual UIWidget*			createCloneInstance ( KDvoid ) override;
		virtual KDvoid				copySpecialProperties ( UIWidget* pModel ) override;
		virtual KDvoid				copyClonedWidgetChildren ( UIWidget* pModel ) override;
		KDvoid						selectedItemEvent ( KDvoid );
		virtual KDvoid				interceptTouchEvent ( KDint nHandleState, UIWidget* pSender, const cocos2d::Point& tTouchPoint ) override;

	protected :
    
		UIWidget*					m_pModel;
		cocos2d::Array*				m_pItems;
		ListViewGravity				m_eGravity;
		KDfloat						m_fItemsMargin;
		cocos2d::Object*			m_pListViewEventListener;
		SEL_ListViewEvent			m_pListViewEventSelector;
		KDint						m_nCurSelectedIndex;
};

}

#endif	// __UIListView_h__
