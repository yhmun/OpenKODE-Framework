/* --------------------------------------------------------------------------
 *
 *      File            CCScrollView.h
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

#ifndef __CCScrollView_h__
#define __CCScrollView_h__

#include "../../layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 */

typedef enum 
{
	kCCScrollViewDirectionNone		 = -1,
    kCCScrollViewDirectionHorizontal =  0,
    kCCScrollViewDirectionVertical		 ,
    kCCScrollViewDirectionBoth			 ,
} CCScrollViewDirection;

class CCScrollView;

class CCScrollViewDelegate
{
	public :

		virtual ~CCScrollViewDelegate ( KDvoid ) { }

		virtual KDvoid		scrollViewDidScroll ( CCScrollView* pView ) = 0;
		virtual KDvoid		scrollViewDidZoom   ( CCScrollView* pView ) = 0;
};

/**
 * ScrollView support for cocos2d for iphone.
 * It provides scroll view functionalities to cocos2d projects natively.
 */
class CCScrollView : public CCLayer
{
	public :
				 CCScrollView ( KDvoid );
		virtual ~CCScrollView ( KDvoid );

		/**
		 *	Returns an autoreleased scroll view object.
		 *
		 *	@param size view size
		 *	@param container parent object
		 *	@return autoreleased scroll view object
		 */
		static CCScrollView*	create ( const CCSize& tSize, CCNode* pContainer = KD_NULL );

		/**
		 *	Returns an autoreleased scroll view object.
		 *
		 *	@param size view size
		 *	@param container parent object
		 *	@return autoreleased scroll view object
		 */
		static CCScrollView*	create ( KDvoid );

	public :

		/**
		 *	Returns a scroll view object
		 *
		 *	@param size view size
		 *	@param container parent object
		 *	@return scroll view object
		 */
		virtual KDbool			initWithViewSize ( const CCSize& tSize, CCNode* pContainer = KD_NULL );

		virtual KDbool			init ( KDvoid );

		/**
		 *	Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
		 *
		 *	@param offset new offset
		 *	@param If YES, the view scrolls to the new offset
		 */
		KDvoid					setContentOffset ( CCPoint tOffset, KDbool bAnimated = KD_FALSE );
		CCPoint					getContentOffset ( KDvoid );

		/**
		 *	Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
		 *	You can override the animation duration with this method.
		 *
		 *	@param offset new offset
		 *	@param animation duration
		 */
		KDvoid					setContentOffsetInDuration ( CCPoint tOffset, KDfloat fDelta ); 

		KDvoid					setZoomScale ( KDfloat fScale );

		/**
		 *	Sets a new scale and does that for a predefined duration.
		 *
		 *	@param s a new scale vale
		 *	@param animated if YES, scaling is animated
		 */
		KDvoid					setZoomScale ( KDfloat fScale, KDbool bAnimated );

		KDfloat					getZoomScale ( KDvoid );

		/**
		 *	Sets a new scale for container in a given duration.
		 *
		 *	@param s a new scale value
		 *	@param animation duration
		 */
		KDvoid					setZoomScaleInDuration ( KDfloat fScale, KDfloat fDelta );

		/**
		 *	Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
		 */
		CCPoint					minContainerOffset ( KDvoid );
		/**
		 * Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
		 */
		CCPoint					maxContainerOffset ( KDvoid ); 
	
		/**
		 *	Determines if a given node's bounding box is in visible bounds
		 *
		 *	@return YES if it is in visible bounds
		 */
		KDbool					isNodeVisible ( CCNode* pNode );

		/**
		 *	Provided to make scroll view compatible with SWLayer's pause method
		 */
		KDvoid					pause ( CCObject* pSender );

		/**
		 *	Provided to make scroll view compatible with SWLayer's resume method
		 */
		KDvoid					resume ( CCObject* pSender );

		inline	KDbool			 isDragging   ( KDvoid ) { return m_bDragging; }
		inline	KDbool			 isTouchMoved ( KDvoid ) { return m_bTouchMoved; }
		inline	KDbool			 isBounceable ( KDvoid ) { return m_bBounceable; }
		inline	KDvoid			setBounceable ( KDbool bBounceable ) { m_bBounceable = bBounceable; }

		/**
		 *	size to clip. CCNode boundingBox uses contentSize directly.
		 *	It's semantically different what it actually means to common scroll views.
		 *	Hence, this scroll view will use a separate size property.
		 */
		const CCSize&			getViewSize ( KDvoid ) { return m_tViewSize; } 
		KDvoid					setViewSize ( const CCSize& tSize );

		CCNode*					getContainer ( KDvoid );
		KDvoid					setContainer ( CCNode* pContainer );

		/**
		 *	direction allowed to scroll. CCScrollViewDirectionBoth by default.
		 */
		CCScrollViewDirection	getDirection ( KDvoid ) { return m_eDirection; }
		virtual KDvoid			setDirection ( CCScrollViewDirection eDirection ) { m_eDirection = eDirection; }

		CCScrollViewDelegate*	getDelegate ( KDvoid ) { return m_pDelegate; }
		KDvoid					setDelegate ( CCScrollViewDelegate* pDelegate ) { m_pDelegate = pDelegate; }

		/** override functions */
		// optional
		virtual KDbool			ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid			ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

		virtual KDvoid			setContentSize ( const CCSize& tSize );
		virtual const CCSize&	getContentSize ( );

		KDvoid					updateInset ( KDvoid );

		/**
		 *	Determines whether it clips its children or not.
		 */
		KDbool					 isClippingToBounds ( KDvoid ) { return m_bClippingToBounds; }
		KDvoid					setClippingToBounds ( KDbool bClippingToBounds ) { m_bClippingToBounds = bClippingToBounds; }

		virtual KDvoid			visit ( KDvoid );

		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder, KDint nTag );
		virtual KDvoid			addChild ( CCNode* pChild, KDint nZOrder );
		virtual KDvoid			addChild ( CCNode* pChild );
		
		KDvoid					setTouchEnabled ( KDbool bEnable );

		// If smaller content size than view size, ignore scroll
		KDbool					isIgnoreScroll ( KDvoid );
		KDvoid					 ignoreScroll ( KDbool bIsIgnoreScroll );

		CC_SYNTHESIZE ( KDfloat, m_fDeAccelRate		, DeAccelRate      );	// Default 0.95f
		CC_SYNTHESIZE ( KDfloat, m_fDeAccelDist		, DeAccelDist      );	// Default 1.00f
		CC_SYNTHESIZE ( KDfloat, m_fBounceDuration	, BounceDuration   );	// Default 0.15f
		CC_SYNTHESIZE ( KDfloat, m_fInsetRatio		, InsetRatio       );	// Default 0.20f
		CC_SYNTHESIZE ( KDfloat, m_fDetectTouchMoved, DetectTouchMoved );	// Default 10.0f

	private :

		/**
		 *	Relocates the container at the proper offset, in bounds of max/min offsets.
		 *
		 *	@param animated If YES, relocation is animated
		 */

		KDvoid					relocateContainer ( KDbool bAnimated );

		/**
		 *	implements auto-scrolling behavior. change SCROLL_DEACCEL_RATE as needed to choose
		 *	deacceleration speed. it must be less than 1.0f.
		 *
		 *	@param dt delta
		 */
		KDvoid					deaccelerateScrolling ( KDfloat fDelta );

		/**
		 *	This method makes sure auto scrolling causes delegate to invoke its method
		 */
		KDvoid					performedAnimatedScroll ( KDfloat fDelta );

		/**
		 *	Expire animated scroll delegate calls
		 */
		KDvoid					stoppedAnimatedScroll ( CCNode* pNode );

		/**
		 *	clip this view so that outside of the visible bounds can be hidden.
		 */
		KDvoid					beforeDraw ( KDvoid );

		/**
		 *	retract what's done in beforeDraw so that there's no side effect to
		 *	other nodes.
		 */
		KDvoid					afterDraw ( KDvoid );

		/**
		 *	Zoom handling
		 */
		KDvoid					handleZoom ( KDvoid );

	protected :

		CCRect					getViewRect ( KDvoid );

	protected :

		/**
		 *	current zoom scale
		 */
		KDfloat					m_fZoomScale;
		
		/**
		 *	min zoom scale
		 */
		KDfloat					m_fMinZoomScale;

		/**
		 *	max zoom scale
		 */
		KDfloat					m_fMaxZoomScale;
		
		/**
		 *	scroll view delegate
		 */
		CCScrollViewDelegate*	m_pDelegate;

		CCScrollViewDirection	m_eDirection;
		
		/**
		 *	If YES, the view is being dragged.
		 */
		KDbool					m_bDragging;

		/**
		 *	Content offset. Note that left-bottom point is the origin
		 */
		CCPoint					m_tContentOffset;

		/**
		 *	Container holds scroll view contents, Sets the scrollable container object of the scroll view
		 */
		CCNode*					m_pContainer;

		/**
		 *	Determiens whether user touch is moved after begin phase.
		 */
		KDbool					m_bTouchMoved;

		/**
		 *	max inset point to limit scrolling by touch
		 */
		CCPoint					m_fMaxInset;

		/**
		 *	min inset point to limit scrolling by touch
		 */
		CCPoint					m_fMinInset;

		/**
		 *	Determines whether the scroll view is allowed to bounce or not.
		 */
		KDbool					m_bBounceable;

		KDbool					m_bClippingToBounds;

		/**
		 *	scroll speed
		 */
		CCPoint					m_tScrollDistance;

		/**
		 *	Touch point
		 */
		CCPoint					m_tTouchPoint;

		/**
		 *	length between two fingers
		 */
		KDfloat					m_fTouchLength;

		/**
		 *	UITouch objects to detect multitouch
		 */
		CCArray*				m_pTouches;

		/**
		 *	size to clip. CCNode boundingBox uses contentSize directly.
		 *	It's semantically different what it actually means to common scroll views.
		 *	Hence, this scroll view will use a separate size property.
		 */
		CCSize					m_tViewSize;

		/**
		 *	max and min scale
		 */
		KDfloat					m_fMinScale;
		KDfloat					m_fMaxScale;

		/**
		 *	scissor rect for parent, just for restoring GL_SCISSOR_BOX
		 */
		CCRect					m_tParentScissorRect;
		KDbool					m_bScissorRestored;

		KDbool					m_bIgnoreScroll;		
};

// end of GUI group
/// @}

NS_CC_END

#endif // __CCScrollView_h__ 
