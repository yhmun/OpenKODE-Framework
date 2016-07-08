/* -----------------------------------------------------------------------------------
 *
 *      File            CCScrollLayer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      SunJiangting on 12-12-06
 * 
 * -----------------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
 *
 *      Cocos2D-iPhone-Extensions v0.2.1
 *      https://github.com/cocos2d/cocos2d-iphone-extensions
 *
 *      Copyright (c) 2010      DK101
 *      http://dk101.net/2010/11/30/implementing-page-scrolling-in-cocos2d/
 *
 *      Copyright (c) 2010      Giv Parvaneh.
 *      http://www.givp.org/blog/2010/12/30/scrolling-menus-in-cocos2d/
 *
 *      Copyright (c) 2011-2012 Stepan Generalov
 *      Copyright (c) 2011      Brian Feller
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
 *      all copies or substantial portions of the Software.
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

#ifndef __CCScrollLayer_h__
#define __CCScrollLayer_h__

#include "../../layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

class CCScrollLayerDelegate;

/** 
 *	Scrolling layer for Menus, like iOS Springboard Screen.
 *
 *	It is a very clean and elegant subclass of CCLayer that lets you pass-in an array
 *	of layers and it will then create a smooth scroller.
 *	Complete with the "snapping" effect. You can create screens with anything that can be added to a CCLayer.
 *
 *	@version 0.2.1
 */
class CCScrollLayer : public CCLayer
{
	public :
				 CCScrollLayer ( KDvoid );
		virtual ~CCScrollLayer ( KDvoid );

		/** 
		 *	Creates new scrollLayer with given pages & width offset.
		 *	@param layers NSArray of CCLayers, that will be used as pages.
		 *	@param widthOffset Length in X-coord, that describes length of possible pages intersection. 
		 */
		static CCScrollLayer*		create ( CCArray* pLayers, KDfloat fWidth, KDint nWidthOffset );

	public :

		/** 
		 *	Inits scrollLayer with given pages & width offset.
		 *	@param layers NSArray of CCLayers, that will be used as pages.
		 *	@param widthOffset Length in X-coord, that describes length of possible pages intersection. 
		 */
		virtual KDbool				initWithLayers ( CCArray* pLayers, KDfloat fWidth, KDint nWidthOffset );

		/// Touches

		/**
		 *	Register with more priority than CCMenu's but don't swallow touches. 
		 */
		virtual KDvoid				registerWithTouchDispatcher ( KDvoid );

		virtual KDbool				ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid				ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid				ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		/// Updates

		/** 
		 *	Updates all pages positions & adds them as children if needed.
		 *	Can be used to update position of pages after screen reshape, or
		 *	for update after dynamic page add/remove.
		 */
		virtual KDvoid				updatePages ( KDvoid );

		/** 
		 *	Moves scrollLayer to page with given number & invokes delegate
		 *	method scrollLayer:scrolledToPageNumber: at the end of CCMoveTo action.
		 *	Does nothing if number >= totalScreens or < 0.
		 */
		virtual KDvoid				moveToPage ( KDint nPage );

		/** 
		 *	Immedeatly moves scrollLayer to page with given number without running CCMoveTo.
		 *	Does nothing if number >= totalScreens or < 0.
		 */
		virtual KDvoid				selectPage ( KDint nPage );

		virtual CCScrollLayerDelegate*	getDelegate ( KDvoid );
		virtual KDvoid					setDelegate ( CCScrollLayerDelegate* pDelegate );

		/// Scroll Config Properties

		virtual KDfloat				getLayerWidth ( KDvoid );

		virtual const CCPoint&		getLastPosition ( KDvoid );

		/** 
		 *	Calibration property. Minimum moving touch length that is enough
		 *	to cancel menu items and start scrolling a layer.
		 */
		virtual KDfloat				getMinimumTouchLengthToSlide ( KDvoid );
		virtual KDvoid				setMinimumTouchLengthToSlide ( KDfloat fValue );

		/** 
		 *	Calibration property. Minimum moving touch length that is enough to change
		 *	the page, without snapping back to the previous selected page.
		 */
		virtual KDfloat				getMinimumTouchLengthToChangePage ( KDvoid );
		virtual KDvoid				setMinimumTouchLengthToChangePage ( KDfloat fValue );

		/**
		 *	Offset that can be used to let user see empty space over first or last page. 
		 */
		virtual KDfloat				getMarginOffset ( KDvoid );
		virtual KDvoid				setMarginOffset ( KDfloat fOffset );

		/**
		 *	If YES - when starting scrolling CCScrollLayer will claim touches, that are
		 *	already claimed by others targetedTouchDelegates by calling CCTouchDispatcher#touchesCancelled
		 *	Usefull to have ability to scroll with touch above menus in pages.
		 *	If NO - scrolling will start, but no touches will be cancelled.
		 *	Default is YES.
		 */
		virtual KDbool				 isStealTouches ( KDvoid );
		virtual KDvoid				setStealTouches ( KDbool bIsSteal );

		/// Pages Indicator Properties

		/** 
		 *	Whenever show or not white/grey dots under the scroll layer.
		 *	If yes - dots will be rendered in parents transform (rendered after scroller visit).
		 */
		virtual KDbool				 isShowPagesIndicator ( KDvoid );
		virtual KDvoid				setShowPagesIndicator( KDbool bShow );

		/**
		 *	Position of dots center in parent coordinates.
		 *	(Default value is screenWidth/2, screenHeight/4)
		 */		
		virtual const CCPoint&		getPagesIndicatorPosition ( KDvoid );
		virtual KDvoid				setPagesIndicatorPosition ( const CCPoint& tPosition );

		/**
		 *	Color of dot, that represents current selected page(only one dot). 
		 */
		virtual const ccColor4B&	getPagesIndicatorSelectedColor ( KDvoid );
		virtual KDvoid				setPagesIndicatorSelectedColor ( const ccColor4B& tColor );

		/**
		 *	Color of dots, that represents other pages. 
		 */
		virtual const ccColor4B&	getPagesIndicatorNormalColor ( KDvoid );
		virtual KDvoid				setPagesIndicatorNormalColor ( const ccColor4B& tColor );


		/// Pages Control Properties

		/**
		 *	Total pages available in scrollLayer. 
		 */
		virtual KDint				getTotalScreens ( KDvoid );

		/**
		 *	Current page number, that is shown. Belongs to the [0, totalScreen] interval. 
		 */
		virtual KDint				getCurrentScreen ( KDvoid );

		/**
		 *	Offset, that can be used to let user see next/previous page. 
		 */
		virtual KDfloat				getPagesWidthOffset ( KDvoid );
		virtual KDvoid				setPagesWidthOffset ( KDfloat fOffset );

		/**
		 *	Returns array of pages CCLayer's  
		 */
		virtual CCArray*			getPages ( KDvoid );

	private :

		KDint						pageNumberAtOrigin ( const CCPoint& tOrigin );

		CCPoint						originFromPageNumber ( KDint nPage );

		/// Moving To / Selecting Pages

		KDvoid						moveToPageEnded ( KDvoid );

		/**
		 *	Hackish stuff - stole touches from other CCTouchDispatcher targeted delegates.
		 *	Used to claim touch without receiving ccTouchBegan. 
		 */
		KDvoid						claimTouch ( CCTouch* pTouch );

	protected :

		CCScrollLayerDelegate*		m_pDelegate;
    
		KDfloat						m_fLayerWidth;
		CCPoint						m_tLastPosition;

		// Holds the current page being displayed.
		KDint						m_nCurrentScreen;
	    
		// Number of previous page being displayed.
		KDint						m_nPrevScreen;
    
		// The x coord of initial point the user starts their swipe.
		KDfloat						m_fStartSwipe;
    
		// For what distance user must slide finger to start scrolling menu.
		KDfloat						m_fMinimumTouchLengthToSlide;
    
		// For what distance user must slide finger to change the page.
		KDfloat						m_fMinimumTouchLengthToChangePage;
	    
		// Whenever show or not gray/white dots under scrolling content.
		KDbool						m_bShowPagesIndicator;
		CCPoint						m_tPagesIndicatorPosition;
		ccColor4B					m_tPagesIndicatorSelectedColor;
		ccColor4B					m_tPagesIndicatorNormalColor;
	    
		// Internal state of scrollLayer (scrolling or idle).
		KDint						m_nState;
	    
		KDbool						m_bStealTouches;
	
		// Holds the touch that started the scroll
		CCTouch*					m_pScrollTouch;
    
		// Holds pages.
		CCArray*					m_pLayers;
	    
		// Holds current pages width offset.
		KDfloat						m_fPagesWidthOffset;
	    
		// Holds current margin offset
		KDfloat						m_fMarginOffset;
};

class CCScrollLayerDelegate 
{
	public :

		/**
		 *	Called when scroll layer begins scrolling.
		 *	Usefull to cancel CCTouchDispatcher standardDelegates.
		 */
		virtual KDvoid		scrollLayerScrollingStarted ( CCScrollLayer* pSender ) { };

		/** 
		 *	Called at the end of moveToPage:
		 *	Doesn't get called in selectPage:
		 */
		virtual KDvoid		scrollLayer ( CCScrollLayer* pSender, KDint nPage ) { }
};

NS_CC_END

#endif	// __CCScrollLayer_h__