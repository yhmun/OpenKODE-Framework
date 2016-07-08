/* -----------------------------------------------------------------------------------
 *
 *      File            CCControl.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org.
 *      Copyright (c) 2011      Yannick Loriot.
 *
 *          http://www.cocos2d-x.org
 *          http://yannickloriot.com
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

#ifndef __CCControl_h__
#define __CCControl_h__

#include "CCControlUtils.h"
#include "../../../2d/layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_EXT_BEGIN

class Invocation;

/**
 *	@addtogroup GUI
 *	@{
 *	@addtogroup control_extension
 *	@{
 */

/** Number of kinds of control event. */
#define kControlEventTotalNumber		9


/**
 *	@class
 *	Control is inspired by the UIControl API class from the UIKit library of 
 *	CocoaTouch. It provides a base class for control Sprites such as Button 
 *	or Slider that convey user intent to the application.
 *
 *	The goal of Control is to define an interface and base implementation for 
 *	preparing action messages and initially dispatching them to their targets when
 *	certain events occur.
 *
 *	To use the Control you have to subclass it.
 */
class Control : public LayerRGBA
{
	public :

		/** Kinds of possible events for the control objects. */
		enum class EventType
		{
			TOUCH_DOWN           = 1 << 0,		// A touch-down event in the control.
			DRAG_INSIDE          = 1 << 1,		// An event where a finger is dragged inside the bounds of the control.
			DRAG_OUTSIDE         = 1 << 2,		// An event where a finger is dragged just outside the bounds of the control.
			DRAG_ENTER           = 1 << 3,		// An event where a finger is dragged into the bounds of the control.
			DRAG_EXIT            = 1 << 4,		// An event where a finger is dragged from within a control to outside its bounds.
			TOUCH_UP_INSIDE      = 1 << 5,		// A touch-up event in the control where the finger is inside the bounds of the control.
			TOUCH_UP_OUTSIDE     = 1 << 6,		// A touch-up event in the control where the finger is outside the bounds of the control.
			TOUCH_CANCEL         = 1 << 7,		// A system event canceling the current touches for the control.
			VALUE_CHANGED        = 1 << 8		// A touch dragging or otherwise manipulating a control, causing it to emit a series of different values.
		};
    
		typedef KDvoid ( Object::*Handler ) ( Object*, EventType );
    
		/** The possible state for a control.  */
		enum class State
		{
			NORMAL				= 1 << 0,		// The normal, or default state of a control¡ªthat is, enabled but neither selected nor highlighted.
			HIGH_LIGHTED		= 1 << 1,		// Highlighted state of a control. A control enters this state when a touch down, drag inside or drag enter is performed. You can retrieve and set this value through the highlighted property.
			DISABLED			= 1 << 2,		// Disabled state of a control. This state indicates that the control is currently disabled. You can retrieve and set this value through the enabled property.
			SELECTED			= 1 << 3		// Selected state of a control. This state indicates that the control is currently selected. You can retrieve and set this value through the selected property.
		};
    
		static Control*			create ( KDvoid );

		/**
		 *	@js ctor
		 */
		Control ( KDvoid );

		virtual KDbool			init ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Control ( KDvoid );

		/** Tells whether the control is enabled. */
		virtual KDvoid			setEnabled ( KDbool bEnabled );
		virtual KDbool			 isEnabled ( KDvoid ) const;

		/** A Boolean value that determines the control selected state. */
		virtual KDvoid			setSelected ( KDbool bSelected );
		virtual KDbool			 isSelected ( KDvoid ) const;

		/** A Boolean value that determines whether the control is highlighted. */
		virtual KDvoid			setHighlighted ( KDbool bHighlighted );
		virtual KDbool			 isHighlighted ( KDvoid ) const;

		KDbool					hasVisibleParents ( KDvoid ) const;

		/**
		 *	Updates the control layout using its current internal state.
		 */
		virtual KDvoid			needsLayout ( KDvoid );

		/**
		 *	Sends action messages for the given control events.
		 *
		 *	@param controlEvents A bitmask whose set flags specify the control events for
		 *	which action messages are sent. See "CCControlEvent" for bitmask constants.
		 */
		virtual KDvoid			sendActionsForControlEvents ( EventType eControlEvents );

		/**
		 *	Adds a target and action for a particular event (or events) to an internal
		 *	dispatch table.
		 *	The action message may optionnaly include the sender and the event as
		 *	parameters, in that order.
		 *	When you call this method, target is not retained.
		 *
		 *	@param target The target object that is, the object to which the action
		 *	message is sent. It cannot be nil. The target is not retained.
		 *	@param action A selector identifying an action message. It cannot be NULL.
		 *	@param controlEvents A bitmask specifying the control events for which the
		 *	action message is sent. See "CCControlEvent" for bitmask constants.
		 */
		virtual KDvoid			addTargetWithActionForControlEvents ( Object* pTarget, Handler pAction, EventType eControlEvents );

		/**
		 *	Removes a target and action for a particular event (or events) from an
		 *	internal dispatch table.
		 *
		 *	@param target The target object—that is, the object to which the action
		 *	message is sent. Pass nil to remove all targets paired with action and the
		 *	specified control events.
		 *	@param action A selector identifying an action message. Pass NULL to remove
		 *	all action messages paired with target.
		 *	@param controlEvents A bitmask specifying the control events associated with
		 *	target and action. See "CCControlEvent" for bitmask constants.
		 */
		virtual KDvoid			removeTargetWithActionForControlEvents ( Object* pTarget, Handler pAction, EventType eControlEvents );

		/**
		 *	Returns a point corresponding to the touh location converted into the
		 *	control space coordinates.
		 *	@param touch A Touch object that represents a touch.
		 */
		virtual Point			getTouchLocation ( Touch* pTouch );

		virtual KDbool			onTouchBegan ( Touch* pTouch, Event* pEvent ) { return false; }
		virtual KDvoid			onTouchMoved ( Touch* pTouch, Event* pEvent ) { }
		virtual KDvoid			onTouchEnded ( Touch* pTouch, Event* pEvent ) { }
		virtual KDvoid			onTouchCancelled ( Touch* pTouch, Event* pEvent ) { }
    
		/**
		 *	Returns a boolean value that indicates whether a touch is inside the bounds
		 *	of the receiver. The given touch must be relative to the world.
		 *
		 *	@param touch A Touch object that represents a touch.
		 *
		 *	@return Whether a touch is inside the receiver's rect.
		 */
		virtual KDbool			isTouchInside ( Touch* pTouch );

		// Overrides
		virtual KDbool			 isOpacityModifyRGB ( KDvoid ) const override;
		virtual KDvoid			setOpacityModifyRGB ( KDbool bOpacityModifyRGB ) override;

	protected :

		/**
		 *	Returns an Invocation object able to construct messages using a given 
		 *	target-action pair. (The invocation may optionnaly include the sender and
		 *	the event as parameters, in that order)
		 *
		 *	@param target The target object.
		 *	@param action A selector identifying an action message.
		 *	@param controlEvent A control events for which the action message is sent.
		 *	See "CCControlEvent" for constants.
		 *
		 *	@return an Invocation object able to construct messages using a given 
		 *	target-action pair.
		 */
		Invocation*				invocationWithTargetAndActionForControlEvent ( Object* pTarget, Handler pAction, EventType eControlEvent );

		/**
		 *	Returns the Invocation list for the given control event. If the list does
		 *	not exist, it'll create an empty array before returning it.
		 *
		 *	@param controlEvent A control events for which the action message is sent.
		 *	See "CCControlEvent" for constants.
		 *
		 *	@return the Invocation list for the given control event.
		 */
		//<Invocation*>
		Array*					dispatchListforControlEvent ( EventType eControlEvent );

		/**
		 *	Adds a target and action for a particular event to an internal dispatch 
		 *	table.
		 *	The action message may optionnaly include the sender and the event as 
		 *	parameters, in that order.
		 *	When you call this method, target is not retained.
		 *
		 *	@param target The target object¡ªthat is, the object to which the action 
		 *	message is sent. It cannot be nil. The target is not retained.
		 *	@param action A selector identifying an action message. It cannot be NULL.
		 *	@param controlEvent A control event for which the action message is sent.
		 *	See "CCControlEvent" for constants.
		 */
		KDvoid					addTargetWithActionForControlEvent ( Object* pTarget, Handler pAction, EventType eControlEvent );
    
		/**
		 *	Removes a target and action for a particular event from an internal dispatch
		 *	table.
		 *
		 *	@param target The target object¡ªthat is, the object to which the action 
		 *	message is sent. Pass nil to remove all targets paired with action and the
		 *	specified control events.
		 *	@param action A selector identifying an action message. Pass NULL to remove
		 *	all action messages paired with target.
		 *	@param controlEvent A control event for which the action message is sent.
		 *	See "CCControlEvent" for constants.
		 */
		KDvoid					removeTargetWithActionForControlEvent ( Object* pTarget, Handler pAction, EventType eControlEvent );

	protected :

		KDbool					m_bEnabled;
		KDbool					m_bSelected;
		KDbool					m_bHighlighted;

		/** True if all of the controls parents are visible */
		KDbool					m_bHasVisibleParents;

		/**
		 *	Table of connection between the ControlEvents and their associated
		 *	target-actions pairs. For each ButtonEvents a list of NSInvocation
		 *	(which contains the target-action pair) is linked.
		 */
		Dictionary*				m_pDispatchTable;

		// CCRGBAProtocol
		KDbool					m_bIsOpacityModifyRGB;

		/** The current control state constant. */
		CC_SYNTHESIZE_READONLY ( State, m_eState, State );
};

Control::EventType operator| ( Control::EventType a, Control::EventType b );

//	end of GUI group
/// @}
/// @}

NS_CC_EXT_END

#endif	// __CCControl_h__
