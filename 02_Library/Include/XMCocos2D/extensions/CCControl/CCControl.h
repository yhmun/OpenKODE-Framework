/* --------------------------------------------------------------------------
 *
 *      File            CCControl.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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

#ifndef __CCControl_h__
#define __CCControl_h__

#include "CCInvocation.h"
#include "CCControlUtils.h"
#include "../../cocoa/CCDictionary.h"
#include "../../layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

/** Number of kinds of control event. */
#define kCCControlEventTotalNumber 9

/** Kinds of possible events for the control objects. */
enum 
{
    kCCControlEventTouchDown           = 1 << 0,    // A touch-down event in the control.
    kCCControlEventTouchDragInside     = 1 << 1,    // An event where a finger is dragged inside the bounds of the control.
    kCCControlEventTouchDragOutside    = 1 << 2,    // An event where a finger is dragged just outside the bounds of the control. 
    kCCControlEventTouchDragEnter      = 1 << 3,    // An event where a finger is dragged into the bounds of the control.
    kCCControlEventTouchDragExit       = 1 << 4,    // An event where a finger is dragged from within a control to outside its bounds.
    kCCControlEventTouchUpInside       = 1 << 5,    // A touch-up event in the control where the finger is inside the bounds of the control. 
    kCCControlEventTouchUpOutside      = 1 << 6,    // A touch-up event in the control where the finger is outside the bounds of the control.
    kCCControlEventValueChanged        = 1 << 8,    // A touch dragging or otherwise manipulating a control, causing it to emit a series of different values.
};
typedef KDuint CCControlEvent;

/** The possible state for a control.  */
enum 
{
    kCCControlStateNormal				= 1 << 0,	// The normal, or default state of a control-that is, enabled but neither selected nor highlighted.
    kCCControlStateHighlighted			= 1 << 1,	// Highlighted state of a control. A control enters this state when a touch down, drag inside or drag enter is performed. You can retrieve and set this value through the highlighted property.
    kCCControlStateDisabled				= 1 << 2,	// Disabled state of a control. This state indicates that the control is currently disabled. You can retrieve and set this value through the enabled property.
    kCCControlStateSelected				= 1 << 3,	// Selected state of a control. This state indicates that the control is currently selected. You can retrieve and set this value through the selected property.
};
typedef KDuint CCControlState;

/*
 * @class
 * CCControl is inspired by the UIControl API class from the UIKit library of 
 * CocoaTouch. It provides a base class for control CCSprites such as CCButton 
 * or CCSlider that convey user intent to the application.
 *
 * The goal of CCControl is to define an interface and base implementation for 
 * preparing action messages and initially dispatching them to their targets when
 * certain events occur.
 *
 * To use the CCControl you have to subclass it.
 */
class CCControl : public CCLayerRGBA
{
	public :
				 CCControl ( KDvoid );
		virtual ~CCControl ( KDvoid );		

		static   CCControl*		create ( KDvoid );

	public :

		virtual KDbool		init ( KDvoid );
		
		virtual KDvoid		onEnter ( KDvoid );
		virtual KDvoid		onExit  ( KDvoid );

		virtual KDvoid		registerWithTouchDispatcher ( KDvoid );

		/**
		 *	Sends action messages for the given control events.
		 *
		 *	@param controlEvents A bitmask whose set flags specify the control events for
		 *	which action messages are sent. See "CCControlEvent" for bitmask constants.
		 */
		virtual KDvoid		sendActionsForControlEvents ( CCControlEvent uControlEvents );

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
		virtual KDvoid		addTargetWithActionForControlEvents ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvents );

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
		virtual KDvoid		removeTargetWithActionForControlEvents ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvents );

		/**
		* Returns a point corresponding to the touh location converted into the 
		* control space coordinates.
		* @param touch A CCTouch object that represents a touch.
		*/
		virtual CCPoint		getTouchLocation ( CCTouch* pTouch );

		/**
		 *	Returns a boolean value that indicates whether a touch is inside the bounds
		 *	of the receiver. The given touch must be relative to the world.
		 *
		 *	@param touch A CCTouch object that represents a touch.
		 *
		 *	@return YES whether a touch is inside the receiver's rect.
		 */
		virtual KDbool		isTouchInside ( CCTouch* pTouch );

	protected :

		/**
		 *	Returns an CCInvocation object able to construct messages using a given 
		 *	target-action pair. (The invocation may optionnaly include the sender and
		 *	the event as parameters, in that order)
		 *
		 *	@param target The target object.
		 *	@param action A selector identifying an action message.
		 *	@param controlEvent A control events for which the action message is sent.
		 *	 See "CCControlEvent" for constants.
		 *
		 *	@return an CCInvocation object able to construct messages using a given 
		 *	target-action pair.
		 */
		CCInvocation*		invocationWithTargetAndActionForControlEvent ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent );

		/**
		 *	Returns the CCInvocation list for the given control event. If the list does
		 *	not exist, it'll create an empty array before returning it.
		 *
		 *	@param controlEvent A control events for which the action message is sent.
		 *	See "CCControlEvent" for constants.
		 *
		 *	@return the CCInvocation list for the given control event.
		 */
		CCArray*			dispatchListforControlEvent ( CCControlEvent uControlEvent );

	public :

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
		KDvoid				addTargetWithActionForControlEvent ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent);

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
		KDvoid				removeTargetWithActionForControlEvent ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent);

		/** Tells whether the control is enabled. */
		virtual KDvoid		setEnabled ( KDbool bEnabled );
		virtual KDbool		 isEnabled ( KDvoid );

		/** A Boolean value that determines the control selected state. */
		virtual KDvoid		setSelected ( KDbool bSelected );
		virtual KDbool		 isSelected ( KDvoid );

		/** A Boolean value that determines whether the control is highlighted. */
		virtual KDvoid		setHighlighted ( KDbool bHighlighted );
		virtual KDbool		 isHighlighted ( KDvoid );

		KDbool				hasVisibleParents ( KDvoid );

		/**	Updates the control layout using its current internal state. */
		virtual KDvoid		needsLayout ( KDvoid );

		virtual KDbool		 isOpacityModifyRGB ( KDvoid );
		virtual KDvoid		setOpacityModifyRGB ( KDbool bOpacityModifyRGB );

	public :

		/** The current control state constant. */
		CC_SYNTHESIZE_READONLY ( CCControlState, m_eState, State );
	    
	protected :

		/** True if all of the controls parents are visible */
		KDbool				m_bHasVisibleParents;
		KDbool				m_bIsOpacityModifyRGB;

		KDbool				m_bEnabled;
		KDbool				m_bSelected;
		KDbool				m_bHighlighted;

		/** 
		 *	Table of connection between the CCControlEvents and their associated
		 *	target-actions pairs. For each CCButtonEvents a list of NSInvocation
		 *	(which contains the target-action pair) is linked.
		 */
		CCDictionary*		m_pDispatchTable;

};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControl_h__
