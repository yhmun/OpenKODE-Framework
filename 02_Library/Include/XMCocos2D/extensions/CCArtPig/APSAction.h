/* --------------------------------------------------------------------------
 *
 *      File            APSAction.h
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

#ifndef __APSAction_h__
#define __APSAction_h__

#include "APSResource.h"
#include "../../actions/CCAction.h"

NS_APS_BEGIN

class APSGraphicGroup;
class APSSymbol;
class APSActionGroup;
class APSActionSelector;

/**
 *	@brief	APSAction defines base control flow of creating and running cocos2d actions.
 *
 *	1. When parent symbol ( APSSymbol ) receives trigger message for this action, the symbol checks if there is a running action with the same actionType property and the same target graphic. If there is, symbol compares priority properties of the two actions, and one of them is interrupted.
 *	2. To trigger this action, parent symbol calls run ( ) method.
 *	3. If CCAction object is not created yet, it is created by getCCAction ( ) method.
 *	4. run ( ) method calls runAction ( ) method of target graphic, and CCAction begins execution.
 *	5. At the beginning of CCAction's execution, startFunc ( ) method of APSAction is called.
 *	6. startFunc ( ) calls notifyStart ( ) of parent symbol ( APSSymbol ).
 *	7. APSAction object is added to runningActions set in symbol.
 *	8. At the end of CCAction's execution, stopFunc ( ) method of APSAction is called.
 *	9. stopFunc ( ) calls notifyStop ( ) of parent symbol.
 *	10. APSAction object is removed from runningActions set in symbol.
 *
 *	- The all APSAction objects have targetGraphic no matter they are graphic action or not. Generally none graphic action's target is parentSymbol. 
 */
class  APSAction : public APSResource
{
	public :

		/** Constructor of APSAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSAction ( APSDictionary* pProperties = KD_NULL );
		virtual ~APSAction ( KDvoid );

		APS_CLASS_NAME ( APSAction );
	    
	public :

		/** Initializes APSAction with given properties. */
		virtual KDbool				initWithProperties ( APSDictionary* pProperties );

		/** Returns the type of action. */
		virtual APSActionType		getActionType ( KDvoid ) const { return kAPSActionTypeNone; } 
	        
		/** Returns the end time of action. */
		virtual KDfloat				getEndTime ( KDvoid );                         
	    
		/** Releases current CCAction object and creates new CCAction. If action is currently running, it is interrupted by this method. */
		virtual KDvoid				resetCCAction ( KDvoid );

		/** Returns a CCAction object corresponded by this action. When getCCAction ( ) is called for the first time, CCAction object is created, and APSAction retains the object. If getCCAction ( ) is called again, the retained CCAction object is returned without creation. */
		CCAction*					getCCAction ( KDvoid );
	    
		virtual KDvoid				preloadData ( KDvoid );
	    
		/** Minimizes memory usage. Espectially, retained CCAction object is released if the action is not running. */
		virtual KDvoid				clearCachedData ( KDvoid );           

		/** Returns target graphic. This must be called after targetGraphicIndex is properly set. */
		virtual APSGraphicGroup*	getTargetGraphic ( KDvoid );
	    
		/** Returns APSActionGroup object contains this action. This must be called after actionGroupIndex is properly set. */
		virtual APSActionGroup*		getActionGroup ( KDvoid );

		/**
		 *	startFunc ( ) is supposed to be called at the beggining of CCAction's execution 
		 *	@warning startFunc ( ) should be only called by CCAction's execution.
		 */
		virtual KDvoid				startFunc ( KDvoid );
	    
		/**
		 *	stopFunc ( ) is supposed to be called at the end of CCAction's execution
		 *	@warning stopFunc ( ) should be only called by CCAction's execution.
		 */
		virtual KDvoid				stopFunc ( KDvoid );                      
	    
		/** Makes APSAction to run. */
		virtual KDvoid				trigger ( KDvoid );
	    
		/** Forces APSAction to stop. */
		virtual KDvoid				interrupt ( KDvoid );

		virtual KDvoid				_trigger  ( KDvoid );
	    
		virtual KDvoid				_interrupt ( KDvoid );

	protected :

		/** Creates and returns an autoreleased CCAction object. */
		virtual CCFiniteTimeAction*		makeCCAction ( KDvoid );
	    
		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run at the beginning. 
		 */
		virtual CCFiniteTimeAction*		makePreCCAction ( KDvoid );
	    
		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions.
		 */
		virtual CCFiniteTimeAction*		makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f ) { return NULL; }
	    
		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run at the end. 
		 */
		virtual CCFiniteTimeAction*		makePostCCAction ( KDvoid );
	    
		KDvoid							setRunning ( KDbool bRunning ) { this->m_bRunning = bRunning; }
	    
		/** Index of target graphic. If action is not a graphic action, target is parent APSSymbol object. */
		APS_SYNTHESIZE_WITH_KEY ( APSIndex, m_uTargetGraphicIndex, TargetGraphicIndex ); 
	    
		/** Index of action group containing this action. */
		APS_SYNTHESIZE_WITH_KEY ( APSIndex, m_uActionGroupIndex, ActionGroupIndex );    
	    
		/** Priority of action. */
		APS_SYNTHESIZE_WITH_KEY ( KDint, m_nPriority, Priority );
	    
		/** This action can be triggered only if 'enabled' property is true. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bEnabled, Enabled );
	    
		/** If this action is APSActionGroup, startTime indicates starting time from page loading. If this action is an action within APSActionGroup, startTime indicates starting time from ActionGroup began. */
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fStartTime, StartTime );
	    
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fDuration, Duration );
	    
		/** running property is true if action is currently running */
		APS_SYNTHESIZE_READONLY_WITH_KEY ( KDbool, m_bRunning, Running );

	protected :

		CCAction*						m_pCCAction;
		APSGraphicGroup*				m_pTargetGraphic;
		APSActionGroup*					m_pActionGroup;
};

NS_APS_END

#endif	// __APSAction_h__
