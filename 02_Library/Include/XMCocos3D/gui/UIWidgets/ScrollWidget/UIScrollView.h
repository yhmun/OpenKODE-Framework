/* -----------------------------------------------------------------------------------
 *
 *      File            UIScrollView.h
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

#ifndef __UIScrollView_h__
#define __UIScrollView_h__

#include "../../Layouts/UILayout.h"
#include "UIScrollInterface.h"

namespace gui {

enum SCROLLVIEW_DIR
{
    SCROLLVIEW_DIR_NONE,
    SCROLLVIEW_DIR_VERTICAL,
    SCROLLVIEW_DIR_HORIZONTAL,
    SCROLLVIEW_DIR_BOTH
};

typedef enum
{
    SCROLLVIEW_EVENT_SCROLL_TO_TOP,
    SCROLLVIEW_EVENT_SCROLL_TO_BOTTOM,
    SCROLLVIEW_EVENT_SCROLL_TO_LEFT,
    SCROLLVIEW_EVENT_SCROLL_TO_RIGHT,
    SCROLLVIEW_EVENT_SCROLLING,
    SCROLLVIEW_EVENT_BOUNCE_TOP,
    SCROLLVIEW_EVENT_BOUNCE_BOTTOM,
    SCROLLVIEW_EVENT_BOUNCE_LEFT,
    SCROLLVIEW_EVENT_BOUNCE_RIGHT
} ScrollviewEventType;

typedef KDvoid ( cocos2d::Object::*SEL_ScrollViewEvent ) ( cocos2d::Object*, ScrollviewEventType );
#define scrollvieweventselector(_SELECTOR) (SEL_ScrollViewEvent) ( &_SELECTOR )

class UIScrollView : public UILayout , public UIScrollInterface
{
	public :

		/**
		 *	Default constructor
		 */
		UIScrollView ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~UIScrollView ( KDvoid );
    
		/**
		 *	Allocates and initializes.
		 */
		static UIScrollView*		create ( KDvoid );
    
		/**
		 *	Changes scroll direction of scrollview.
		 *
		 *	@see SCROLLVIEW_DIR      SCROLLVIEW_DIR_VERTICAL means vertical scroll, SCROLLVIEW_DIR_HORIZONTAL means horizontal scroll
		 *
		 *	@param SCROLLVIEW_DIR
		 */
		virtual KDvoid				setDirection ( SCROLLVIEW_DIR eDir );
    
		/**
		 *	Gets scroll direction of scrollview.
		 *
		 *	@see SCROLLVIEW_DIR      SCROLLVIEW_DIR_VERTICAL means vertical scroll, SCROLLVIEW_DIR_HORIZONTAL means horizontal scroll
		 *
		 *	@return SCROLLVIEW_DIR
		 */
		SCROLLVIEW_DIR				getDirection ( KDvoid );
    
		/**
		 *	Gets inner container of scrollview.
		 *
		 *	Inner container is the container of scrollview's children.
		 *
		 *	@return inner container.
		 */
		UILayout*					getInnerContainer ( KDvoid );
    
		/**
		 *	Scroll inner container to bottom boundary of scrollview.
		 */
		KDvoid						scrollToBottom ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to top boundary of scrollview.
		 */
		KDvoid						scrollToTop ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to left boundary of scrollview.
		 */
		KDvoid						scrollToLeft ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to right boundary of scrollview.
		 */
		KDvoid						scrollToRight ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to top and left boundary of scrollview.
		 */
		KDvoid						scrollToTopLeft ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to top and right boundary of scrollview.
		 */
		KDvoid						scrollToTopRight ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to bottom and left boundary of scrollview.
		 */
		KDvoid						scrollToBottomLeft ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to bottom and right boundary of scrollview.
		 */
		KDvoid						scrollToBottomRight ( KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to vertical percent position of scrollview.
		 */
		KDvoid						scrollToPercentVertical ( KDfloat fPercent, KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to horizontal percent position of scrollview.
		 */
		KDvoid						scrollToPercentHorizontal ( KDfloat fPercent, KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Scroll inner container to both direction percent position of scrollview.
		 */
		KDvoid						scrollToPercentBothDirection ( const cocos2d::Point& tPercent, KDfloat fTime, KDbool bAttenuated );
    
		/**
		 *	Move inner container to bottom boundary of scrollview.
		 */
		KDvoid						jumpToBottom ( KDvoid );
    
		/**
		 *	Move inner container to top boundary of scrollview.
		 */
		KDvoid						jumpToTop ( KDvoid );
    
		/**
		 *	Move inner container to left boundary of scrollview.
		 */
		KDvoid						jumpToLeft ( KDvoid );
    
		/**
		 *	Move inner container to right boundary of scrollview.
		 */
		KDvoid						jumpToRight ( KDvoid );
    
		/**
		 *	Move inner container to top and left boundary of scrollview.
		 */
		KDvoid						jumpToTopLeft ( KDvoid );
    
		/**
		 *	Move inner container to top and right boundary of scrollview.
		 */
		KDvoid						jumpToTopRight ( KDvoid );
    
		/**
		 *	Move inner container to bottom and left boundary of scrollview.
		 */
		KDvoid						jumpToBottomLeft ( KDvoid );
    
		/**
		 *	Move inner container to bottom and right boundary of scrollview.
		 */
		KDvoid						jumpToBottomRight ( KDvoid );
    
		/**
		 *	Move inner container to vertical percent position of scrollview.
		 */
		KDvoid						jumpToPercentVertical ( KDfloat fPercent );
    
		/**
		 *	Move inner container to horizontal percent position of scrollview.
		 */
		KDvoid						jumpToPercentHorizontal ( KDfloat fPercent );
    
		/**
		 *	Move inner container to both direction percent position of scrollview.
		 */
		KDvoid						jumpToPercentBothDirection ( const cocos2d::Point& tPercent );
    
		/**
		 *	Changes inner container size of scrollview.
		 *
		 *	Inner container size must be larger than or equal scrollview's size.
		 *
		 *	@param inner container size.
		 */
		KDvoid						setInnerContainerSize ( const cocos2d::Size& tSize );
    
		/**
		 *	Gets inner container size of scrollview.
		 *
		 *	Inner container size must be larger than or equal scrollview's size.
		 *
		 *	@return inner container size.
		 */
		const cocos2d::Size&		getInnerContainerSize ( KDvoid ) const;
    
		/**
		 *	Add call back function called scrollview event triggered
		 */
		KDvoid						addEventListenerScrollView ( cocos2d::Object* pTarget, SEL_ScrollViewEvent pSelector );
        
		// override "addChild" method of widget.
		virtual KDbool				addChild ( UIWidget* pWidget ) override;
    
		// override "removeAllChildrenAndCleanUp" method of widget.
		virtual KDvoid				removeAllChildren ( KDvoid ) override;
    
		// override "removeChild" method of widget.
		virtual KDbool				removeChild ( UIWidget* pChild ) override;
    
		// override "getChildren" method of widget.
		virtual cocos2d::Array*		getChildren ( KDvoid ) override;
    
		// override "onTouchBegan" method of widget.
		virtual KDbool				onTouchBegan ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchMoved" method of widget.
		virtual KDvoid				onTouchMoved ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchEnded" method of widget.
		virtual KDvoid				onTouchEnded ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchCancelled" method of widget.
		virtual KDvoid				onTouchCancelled ( const cocos2d::Point& tTouchPoint ) override;
    
		// override "onTouchLongClicked" method of widget.
		virtual KDvoid				onTouchLongClicked ( const cocos2d::Point& tTouchPoint ) override;
    
		virtual KDvoid				update ( KDfloat dt ) override;
    
		KDvoid						setBounceEnabled ( KDbool bEnabled );
    
		KDbool						isBounceEnabled ( KDvoid ) const;
    
		KDvoid						setInertiaScrollEnabled ( KDbool bEnabled );
    
		KDbool						isInertiaScrollEnabled ( KDvoid ) const;
    
		/**
		 *	Sets LayoutType.
		 *
		 *	@see LayoutType
		 *
		 *	@param LayoutType
		 */
		virtual KDvoid				setLayoutType ( LayoutType eType ) override;
    
		/**
		 *	Gets LayoutType.
		 *
		 *	@see LayoutType
		 *
		 *	@return LayoutType
		 */
		virtual LayoutType			getLayoutType ( KDvoid ) const override;
    
		virtual KDvoid				doLayout ( KDvoid ) override;
    
		/**
		 *	Returns the "class name" of widget.
		 */
		virtual const KDchar*		getDescription ( KDvoid ) const override;

	protected :

		virtual KDbool				init ( KDvoid ) override;
		virtual KDvoid				initRenderer ( KDvoid ) override;
		KDvoid						moveChildren ( KDfloat fOffsetX, KDfloat fOffsetY );
		KDvoid						autoScrollChildren ( KDfloat dt );
		KDvoid						bounceChildren ( KDfloat dt );
		KDvoid						checkBounceBoundary ( KDvoid );
		KDbool						checkNeedBounce ( KDvoid );
		KDvoid						startAutoScrollChildrenWithOriginalSpeed (const cocos2d::Point& tDir, KDfloat v, KDbool bAttenuated, KDfloat fAcceleration );
		KDvoid						startAutoScrollChildrenWithDestination ( const cocos2d::Point& tDes, KDfloat fTime, KDbool bAttenuated );
		KDvoid						jumpToDestination ( const cocos2d::Point& tDes );
		KDvoid						stopAutoScrollChildren ( KDvoid );
		KDvoid						startBounceChildren ( KDfloat v);
		KDvoid						stopBounceChildren ( KDvoid );
		KDbool						checkCustomScrollDestination ( KDfloat* pTouchOffsetX, KDfloat* pTouchOffsetY );
		virtual KDbool				scrollChildren ( KDfloat fTouchOffsetX, KDfloat fTouchOffsetY );
		KDbool						bounceScrollChildren ( KDfloat fTouchOffsetX, KDfloat fTouchOffsetY );
		KDvoid						startRecordSlidAction ( KDvoid );
		virtual KDvoid				endRecordSlidAction ( KDvoid );
		virtual KDvoid				handlePressLogic ( const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				handleMoveLogic ( const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				handleReleaseLogic ( const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				interceptTouchEvent ( KDint nHandleState, UIWidget* pSender, const cocos2d::Point& tTouchPoint ) override;
		virtual KDvoid				checkChildInfo ( KDint nHandleState, UIWidget* pSender, const cocos2d::Point& tTouchPoint ) override;
		KDvoid						recordSlidTime ( KDfloat dt );
		KDvoid						scrollToTopEvent ( KDvoid );
		KDvoid						scrollToBottomEvent ( KDvoid );
		KDvoid						scrollToLeftEvent ( KDvoid );
		KDvoid						scrollToRightEvent ( KDvoid );
		KDvoid						scrollingEvent ( KDvoid );
		KDvoid						bounceTopEvent ( KDvoid );
		KDvoid						bounceBottomEvent ( KDvoid );
		KDvoid						bounceLeftEvent ( KDvoid );
		KDvoid						bounceRightEvent ( KDvoid );
		virtual KDvoid				onSizeChanged ( KDvoid ) override;
		virtual UIWidget*			createCloneInstance ( KDvoid ) override;
		virtual KDvoid				copySpecialProperties ( UIWidget* pModel ) override;
		virtual KDvoid				copyClonedWidgetChildren ( UIWidget* pModel ) override;
		virtual KDvoid				setClippingEnabled ( KDbool bAble ) override { UILayout::setClippingEnabled ( bAble ); }
	
	protected :

		UILayout*					m_pInnerContainer;
    
		SCROLLVIEW_DIR				m_eDirection;

		cocos2d::Point				m_tTouchBeganPoint;
		cocos2d::Point				m_tTouchMovedPoint;
		cocos2d::Point				m_tTouchEndedPoint;
		cocos2d::Point				m_tTouchMovingPoint;
		cocos2d::Point				m_tAutoScrollDir;
    
		KDfloat						m_fTopBoundary;
		KDfloat						m_fBottomBoundary;
		KDfloat						m_fLeftBoundary;
		KDfloat						m_fRightBoundary;
    
		KDfloat						m_fBounceTopBoundary;
		KDfloat						m_fBounceBottomBoundary;
		KDfloat						m_fBounceLeftBoundary;
		KDfloat						m_fBounceRightBoundary;

		KDbool						m_bAutoScroll;
		KDfloat						m_fAutoScrollAddUpTime;
    
		KDfloat						m_fAutoScrollOriginalSpeed;
		KDfloat						m_fAutoScrollAcceleration;
		KDbool						m_bIsAutoScrollSpeedAttenuated;
		KDbool						m_bNeedCheckAutoScrollDestination;
		cocos2d::Point				m_tAutoScrollDestination;
    
		KDbool						m_bBePressed;
		KDfloat						m_fSlidTime;
		cocos2d::Point				m_tMoveChildPoint;
		KDfloat						m_fChildFocusCancelOffset;
    
		KDbool						m_bLeftBounceNeeded;
		KDbool						m_bTopBounceNeeded;
		KDbool						m_bRightBounceNeeded;
		KDbool						m_bBottomBounceNeeded;
    
		KDbool						m_bBounceEnabled;
		KDbool						m_bBouncing;
		cocos2d::Point				m_tBounceDir;
		KDfloat						m_fBounceOriginalSpeed;
		KDbool						m_bInertiaScrollEnabled;
    
		cocos2d::Object*			m_pScrollViewEventListener;
		SEL_ScrollViewEvent			m_pScrollViewEventSelector;
};

}

#endif	// __UIScrollView_h__
