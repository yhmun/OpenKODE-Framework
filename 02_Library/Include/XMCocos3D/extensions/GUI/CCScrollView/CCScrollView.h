/* -----------------------------------------------------------------------------------
 *
 *      File            CCScrollView.h
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

#ifndef __CCScrollView_h__
#define __CCScrollView_h__

#include "../../ExtensionMacros.h"
#include "../../../2d/layers_scenes_transitions_nodes/CCLayer.h"
#include "../../../2d/event_dispatcher/CCEventListenerTouch.h"

NS_CC_EXT_BEGIN

/**
 *	@addtogroup GUI
 *	@{
 */

class ScrollView;

class ScrollViewDelegate
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ScrollViewDelegate ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				scrollViewDidScroll ( ScrollView* pView ) = 0;
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				scrollViewDidZoom ( ScrollView* pView ) = 0;
};


/**
 *	ScrollView support for cocos2d-x.
 *	It provides scroll view functionalities to cocos2d projects natively.
 */
class ScrollView : public Layer
{
	public :

		enum class Direction
		{
			NONE = -1,
			HORIZONTAL = 0,
			VERTICAL,
			BOTH
		};

		/**
		 *	Returns an autoreleased scroll view object.
		 *
		 *	@param size view size
		 *	@param container parent object
		 *	@return autoreleased scroll view object
		 */
		static ScrollView*			create ( Size tSize, Node* pContainer = KD_NULL );

		/**
		 *	Returns an autoreleased scroll view object.
		 *
		 *	@return autoreleased scroll view object
		 */
		static ScrollView*			create ( KDvoid );

		/**
		 *	@js ctor
		 */
		ScrollView ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ScrollView ( KDvoid );

		KDbool						init ( KDvoid );

		/**
		 *	Returns a scroll view object
		 *
		 *	@param size view size
		 *	@param container parent object
		 *	@return scroll view object
		 */
		KDbool						initWithViewSize ( Size tSize, Node* pContainer = KD_NULL );

		/**
		 *	Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
		 *
		 *	@param offset    The new offset.
		 *	@param animated  If true, the view will scroll to the new offset.
		 */
		KDvoid						setContentOffset ( Point tOffset, KDbool bAnimated = false );
		Point						getContentOffset ( KDvoid );
		
		/**
		 *	Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
		 *	You can override the animation duration with this method.
		 *
		 *	@param offset    The new offset.
		 *	@param dt        The animation duration.
		 */
		KDvoid						setContentOffsetInDuration ( Point tOffset, KDfloat dt ); 

		KDvoid						setZoomScale ( KDfloat s );

		/**
		 *	Sets a new scale and does that for a predefined duration.
		 *
		 *	@param s         The new scale vale
		 *	@param animated  If true, scaling is animated
		 */
		KDvoid						setZoomScale ( KDfloat s, KDbool bAnimated );

		KDfloat						getZoomScale ( KDvoid );

		/**
		 *	Sets a new scale for container in a given duration.
		 *
		 *	@param s     The new scale value
		 *	@param dt    The animation duration
		 */
		KDvoid						setZoomScaleInDuration ( KDfloat s, KDfloat dt );

		/**
		 *	Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
		 */
		Point						minContainerOffset ( KDvoid );

		/**
		 *	Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
		 */
		Point						maxContainerOffset ( KDvoid ); 

		/**
		 *	Determines if a given node's bounding box is in visible bounds
		 *
		 *	@returns true if it is in visible bounds
		 */
		KDbool						isNodeVisible ( Node* pNode );

		/**
		 *	Provided to make scroll view compatible with SWLayer's pause method
		 */
		KDvoid						pause ( Object* pSender );

		/**
		 *	Provided to make scroll view compatible with SWLayer's resume method
		 */
		KDvoid						resume ( Object* pSender );

		KDvoid						setTouchEnabled ( KDbool bEnabled );
		KDbool						 isTouchEnabled ( KDvoid ) const;

		KDbool						isDragging ( KDvoid ) const {return m_bDragging; }
		KDbool						isTouchMoved ( KDvoid ) const { return m_bTouchMoved; }
		KDbool						isBounceable ( KDvoid ) const { return m_bBounceable; }
		KDvoid						setBounceable ( KDbool bBounceable ) { m_bBounceable = bBounceable; }

		/**
		 *	size to clip. Node boundingBox uses contentSize directly.
		 *	It's semantically different what it actually means to common scroll views.
		 *	Hence, this scroll view will use a separate size property.
		 */
		Size						getViewSize ( KDvoid ) const { return m_tViewSize; }
		KDvoid						setViewSize ( Size tSize );

		Node*						getContainer ( KDvoid );
		KDvoid						setContainer ( Node* pContainer );

		/**
		 *	direction allowed to scroll. ScrollViewDirectionBoth by default.
		 */
		Direction					getDirection ( KDvoid ) const { return m_eDirection; }
		virtual KDvoid				setDirection ( Direction eDirection ) { m_eDirection = eDirection; }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		ScrollViewDelegate*			getDelegate ( KDvoid ) { return m_pDelegate; }

		/**
		 *	@code
		 *	when this function bound to js or lua,the input param are changed
		 *	in js: var setDelegate(var jsObject)
		 *	in lua: local setDelegate()
		 *	@endcode
		 */
		KDvoid						setDelegate ( ScrollViewDelegate* pDelegate ) { m_pDelegate = pDelegate; }

		KDvoid						updateInset ( KDvoid );

		/**
		 *	Determines whether it clips its children or not.
		 */
		KDbool						 isClippingToBounds ( KDvoid ) { return m_bClippingToBounds; }
		KDvoid						setClippingToBounds ( KDbool bClippingToBounds ) { m_bClippingToBounds = bClippingToBounds; }

		virtual KDbool				onTouchBegan ( Touch* pTouch, Event* pEvent );
		virtual KDvoid				onTouchMoved ( Touch* pTouch, Event* pEvent );
		virtual KDvoid				onTouchEnded ( Touch* pTouch, Event* pEvent );
		virtual KDvoid				onTouchCancelled ( Touch* pTouch, Event* pEvent );
    
		// Overrides
		virtual KDvoid				setContentSize ( const Size& tSize ) override;
		virtual const Size&			getContentSize ( KDvoid ) const override;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid				visit ( KDvoid ) override;
		virtual KDvoid				addChild ( Node* pChild, KDint zOrder, KDint nTag ) override;
		virtual KDvoid				addChild ( Node* pChild, KDint zOrder ) override;
		virtual KDvoid				addChild ( Node* pChild ) override;

	protected :

		/**
		 *	Relocates the container at the proper offset, in bounds of max/min offsets.
		 *
		 *	@param animated If true, relocation is animated
		 */
		KDvoid						relocateContainer ( KDbool bAnimated );

		/**
		 *	implements auto-scrolling behavior. change SCROLL_DEACCEL_RATE as needed to choose
		 *	deacceleration speed. it must be less than 1.0f.
		 *
		 *	@param dt delta
		 */
		KDvoid						deaccelerateScrolling ( KDfloat dt );

		/**
		 *	This method makes sure auto scrolling causes delegate to invoke its method
		 */
		KDvoid						performedAnimatedScroll ( KDfloat dt );

		/**
		 *	Expire animated scroll delegate calls
		 */
		KDvoid						stoppedAnimatedScroll ( Node* pNode );

		/**
		 *	clip this view so that outside of the visible bounds can be hidden.
		 */
		KDvoid						beforeDraw ( KDvoid );

		/**
		 *	retract what's done in beforeDraw so that there's no side effect to
		 *	other nodes.
		 */
		KDvoid						afterDraw ( KDvoid );

		/**
		 *	Zoom handling
		 */
		KDvoid						handleZoom ( KDvoid );

		Rect						getViewRect ( KDvoid );
    
		/**
		 *	current zoom scale
		 */
		KDfloat						m_fZoomScale;
		/**
		 *	min zoom scale
		 */
		KDfloat						m_fMinZoomScale;
		/**
		 *	max zoom scale
		 */
		KDfloat						m_fMaxZoomScale;
		/**
		 *	scroll view delegate
		 */
		ScrollViewDelegate*			m_pDelegate;

		Direction					m_eDirection;
		/**
		 *	If YES, the view is being dragged.
		 */
		KDbool						m_bDragging;

		/**
		 *	Content offset. Note that left-bottom point is the origin
		 */
		Point						m_tContentOffset;

		/**
		 *	Container holds scroll view contents, Sets the scrollable container object of the scroll view
		 */
		Node*						m_pContainer;
		/**
		 *	Determiens whether user touch is moved after begin phase.
		 */
		KDbool						m_bTouchMoved;
		/**
		 *	max inset point to limit scrolling by touch
		 */
		Point						m_tMaxInset;
		/**
		 *	min inset point to limit scrolling by touch
		 */
		Point						m_tMinInset;
		/**
		 *	Determines whether the scroll view is allowed to bounce or not.
		 */
		KDbool						m_bBounceable;

		KDbool						m_bClippingToBounds;

		/**
		 *	scroll speed
		 */
		Point						m_tScrollDistance;
		/**
		 *	Touch point
		 */
		Point						m_tTouchPoint;
		/**
		 *	length between two fingers
		 */
		KDfloat						m_fTouchLength;
		/**
		 *	Touch objects to detect multitouch
		 */
		std::vector<Touch*>			m_aTouches;
		/**
		 *	size to clip. Node boundingBox uses contentSize directly.
		 *	It's semantically different what it actually means to common scroll views.
		 *	Hence, this scroll view will use a separate size property.
		 */
		Size						m_tViewSize;
		/**
		 *	max and min scale
		 */
		KDfloat						m_fMinScale, m_fMaxScale;
		/**
		 *	scissor rect for parent, just for restoring GL_SCISSOR_BOX
		 */
		Rect						m_tParentScissorRect;
		KDbool						m_bScissorRestored;
    
		/** Touch listener */
		EventListenerTouchOneByOne* m_pTouchListener;
};

//	end of GUI group
/// @}

NS_CC_EXT_END

#endif	// __CCScrollView_h__
