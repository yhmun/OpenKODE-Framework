/* -----------------------------------------------------------------------------------
 *
 *      File            UIPageView.h
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

#ifndef __UIPageView_h__
#define __UIPageView_h__

#include "../../Layouts/UILayout.h"
#include "UIScrollInterface.h"

namespace gui {

typedef enum
{
    PAGEVIEW_EVENT_TURNING,
} PageViewEventType;

typedef void (cocos2d::Object::*SEL_PageViewEvent) ( cocos2d::Object*, PageViewEventType );
#define pagevieweventselector(_SELECTOR) (SEL_PageViewEvent) ( &_SELECTOR )

typedef enum 
{
    PAGEVIEW_TOUCHLEFT,
    PAGEVIEW_TOUCHRIGHT
} PVTouchDir;

class UIPageView : public UILayout , public UIScrollInterface
{    
	public :

		/**
		 *	Default constructor
		 */
		UIPageView ( KDvoid );
    
		/**
		 *  Default destructor
		 */
		virtual ~UIPageView ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UIPageView*			create ( KDvoid );
    
		/**
		 *	Add a widget to a page of pageview.
		 *
		 *	@param widget    widget to be added to pageview.
		 *
		 *	@param pageIdx   index of page.
		 *
		 *	@param forceCreate   if force create and there is no page exsit, pageview would create a default page for adding widget.
		 */
		KDvoid						addWidgetToPage ( UIWidget* pWidget, KDint nPageIdx, KDbool bForceCreate );
    
		/**
		 *	Push back a page to pageview.
		 *
		 *	@param page    page to be added to pageview.
		 */
		KDvoid						addPage ( UILayout* pPage );
    
		/**
		 *	Inert a page to pageview.
		 *
		 *	@param page    page to be added to pageview.
		 */
		KDvoid						insertPage ( UILayout* pPage, KDint nIdx );
    
		/**
		 *	Remove a page of pageview.
		 *
		 *	@param page    page which will be removed.
		 */
		KDvoid						removePage ( UILayout* pPage );

		/**
		 *	Remove a page at index of pageview.
		 *
		 *	@param index    index of page.
		 */
		KDvoid						removePageAtIndex ( KDint nIndex );
    
		KDvoid						removeAllPages ( KDvoid );
    
		/**
		 *	scroll pageview to index.
		 *
		 *	@param idx    index of page.
		 */
		KDvoid						scrollToPage ( KDint nIdx );
    
		/**
		 *	Gets current page index.
		 *
		 *	@return current page index.
		 */
		KDint						getCurPageIndex ( KDvoid ) const;
    
		cocos2d::Array*				getPages ( KDvoid );
    
		UILayout*					getPage ( KDint nIndex );
    
		// event
		KDvoid						addEventListenerPageView ( cocos2d::Object* pTarget, SEL_PageViewEvent pSelector );    
    
		// override "onTouchBegan" method of widget.
		virtual KDbool				onTouchBegan ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchMoved" method of widget.
		virtual KDvoid				onTouchMoved ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchEnded" method of widget.
		virtual KDvoid				onTouchEnded ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchCancelled" method of widget.
		virtual KDvoid				onTouchCancelled ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "update" method of widget.
		virtual KDvoid				update ( KDfloat dt ) override;
    
		virtual KDvoid				doLayout ( KDvoid ) override { }
    
		/**
		 *	Sets LayoutType.
		 *
		 *	@see LayoutType
		 *
		 *	@param LayoutType
		 */
		virtual KDvoid				setLayoutType ( LayoutType eType ) override { }
    
		/**
		 *	Gets LayoutType.
		 *
		 *	@see LayoutType
		 *
		 *	@return LayoutType
		 */
		virtual LayoutType			getLayoutType ( KDvoid ) const override { return LAYOUT_ABSOLUTE; }
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*		getDescription ( KDvoid ) const override;
    
	protected :

		virtual KDbool				addChild ( UIWidget* pWidget ) override;
		virtual KDbool				removeChild ( UIWidget* pWidget ) override;
		virtual KDvoid				removeAllChildren ( KDvoid ) override;
		virtual cocos2d::Array*		getChildren ( KDvoid ) override { return UIWidget::getChildren ( ); }
		virtual KDbool				init ( KDvoid ) override;
		UILayout*					createPage ( KDvoid );
		KDfloat						getPositionXByIndex ( KDint nIdx );
		KDvoid						updateBoundaryPages ( KDvoid );
		virtual KDvoid				handlePressLogic ( const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				handleMoveLogic ( const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				handleReleaseLogic ( const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				interceptTouchEvent ( KDint nHandleState, UIWidget* pSender, const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				checkChildInfo ( KDint nHandleState, UIWidget* pSender, const cocos2d::Point& tTouchPoint ) override;
		virtual KDbool				scrollPages ( KDfloat fTouchOffset );
		KDvoid						movePages ( KDfloat fOffset );
		KDvoid						pageTurningEvent ( KDvoid );
		KDvoid						updateChildrenSize ( KDvoid );
		KDvoid						updateChildrenPosition ( KDvoid );
		virtual KDvoid				onSizeChanged ( KDvoid ) override;
		virtual UIWidget*			createCloneInstance ( KDvoid ) override;
		virtual KDvoid				copySpecialProperties ( UIWidget* pModel ) override;
		virtual KDvoid				copyClonedWidgetChildren ( UIWidget* pModel ) override;
		virtual KDvoid				setClippingEnabled ( KDbool bEnabled ) override { UILayout::setClippingEnabled ( bEnabled ); }

	protected :

		KDint						m_nCurPageIdx;
		cocos2d::Array*				m_pPages;
		PVTouchDir					m_tTouchMoveDir;
		KDfloat						m_fTouchStartLocation;
		KDfloat						m_fTouchMoveStartLocation;
		cocos2d::Point				m_tMovePagePoint;
		UIWidget*					m_pLeftChild;
		UIWidget*					m_pRightChild;
		KDfloat						m_fLeftBoundary;
		KDfloat						m_fRightBoundary;
		KDbool						m_bIsAutoScrolling;
		KDfloat						m_fAutoScrollDistance;
		KDfloat						m_fAutoScrollSpeed;
		KDint						m_nAutoScrollDir;
		KDfloat						m_fChildFocusCancelOffset;
		cocos2d::Object*			m_pPageViewEventListener;
		SEL_PageViewEvent			m_pPageViewEventSelector;
};

}

#endif	// __UIPageView_h__
