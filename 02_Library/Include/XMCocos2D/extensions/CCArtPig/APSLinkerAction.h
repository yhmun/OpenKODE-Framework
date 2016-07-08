/* --------------------------------------------------------------------------
 *
 *      File            APSLinkerAction.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSLinkerAction_h__
#define __APSLinkerAction_h__

#include "APSAction.h"


NS_APS_BEGIN

/**
 *	@brief	APSLinkerAction defines properties and methods to manage link messages between action groups.
 *	There are kinds of link messages ( trigger, interrupt, interrupt all, enable, disable ).
 *	If APSLinkerAction object's type is trigger, target action is triggered to start.
 *	If APSLinkerAction object's type is interrupt, target action is interrupted by linker.
 *	If APSLinkerAction object's type is interrupt all, all actions in parent symbol are interrupted.
 *	If APSLinkerAction object's type is disable, target action is disabled, and the disabled action cannot be triggered to start.
 *	If APSLinkerAction object's type is enable, target action is enabled so that it can be triggered.
 * 
 *	Although APSLinkerAction's targetAction does not have to be APSActionGroup object, linker action is generally used to send link messages between action groups.
 */
class APSLinkerAction : public APSAction
{
	public :

		/** Constructor of APSLinkerAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSLinkerAction ( APSDictionary* pProperties = KD_NULL );    	   	

		APS_CLASS_NAME ( APSLinkerAction );

	public :

		/** Initializes APSLinkerAction with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Sends link message. */
		virtual KDvoid  sendLinkMessage ( KDvoid );

		/** Returns the type of APSLinkerAction. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeLinker; }
	    
		/** Returns target action linked by this action. */
		virtual APSAction*  getTargetAction ( KDvoid );
	    
		virtual KDvoid  _trigger ( KDvoid );
	    
	protected :
	    
		KDvoid  _sendLinkMessage ( APSSymbol* pSymbol );
	    
		/** Creates and returns an autoreleased CCAction object. */
		virtual CCFiniteTimeAction*  makeCCAction ( KDvoid );

		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions.
		 */
		virtual CCFiniteTimeAction*  makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );
	    
		/** Index of target action. */
		APS_SYNTHESIZE_WITH_KEY ( APSIndex, m_uTargetActionIndex, TargetActionIndex );
	    
		APS_SYNTHESIZE_WITH_KEY ( APSLinkerType, m_uType, Type );
	    
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bNoTriggerRunningTarget, NoTriggerRunningTarget );

	protected :

		APSAction*		m_pTargetAction;
};

NS_APS_END

#endif	// __APSLinkerAction_h__
