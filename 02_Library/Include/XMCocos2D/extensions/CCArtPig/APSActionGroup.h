/* --------------------------------------------------------------------------
 *
 *      File            APSActionGroup.h
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

#ifndef __APSActionGroup_h__
#define __APSActionGroup_h__

#include "APSAction.h"
#include "APSResourceArray.h"

NS_APS_BEGIN

class APSActionGroupSelector;
class APSActionGroup;

/**
 *	@brief	APSActionGroup contains unit-actions ( move, rotate, scale, fade, and so on ) and manages to run them.
 */
class APSActionGroup : public APSAction 
{
	public :

		/** Constructor of APSActionGroup. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSActionGroup ( APSDictionary* pProperties = KD_NULL );
	    
		virtual ~APSActionGroup ( KDvoid );
	    
		APS_CLASS_NAME ( APSActionGroup );

	public :

		/** Initializes APSActionGroup with given properties. */
		virtual KDbool				initWithProperties ( APSDictionary* pProperties );
	        
		/** Decreases runningActionCount by 1. stopFunc ( ) is called when the count becomes zero. */
		KDvoid						decreaseRunningActionCount ( KDvoid );

		virtual KDvoid				_trigger ( KDvoid );
	    
		/** Stops all actions in this action group. */
		virtual KDvoid				_interrupt ( KDvoid );

		/** Returns the type of action. */
		virtual APSActionType		getActionType ( KDvoid ) const { return kAPSActionTypeGroup; };

		/** Returns an array of actions in this action group. The array is sorted by startTime property of APSAction. */
		APSResourceArray*			getActions ( KDvoid ) const { return m_pActions; }

		/** Fills up actions parameter so that it contains all actions targeting to targetGraphic. */
		KDvoid						getActionsForTargetGraphic ( std::vector<APSAction*>* pActions, const APSGraphicGroup* pTargetGraphic ) const;
	    
		/** 
		 *	startFunc ( ) is supposed to be called at the beggining of CCAction's execution 
		 *	@warning startFunc ( ) should be only called by CCAction's execution.
		 */
		virtual KDvoid				startFunc ( KDvoid );

		/**
		 *	Notifies parent symbol to trigger targetAction.
		 *	@warning Although notifyTrigger ( ) is declared in public, it should be called solely by CCAction's execution of this action group.
		 */
		KDvoid						notifyTrigger ( CCNode* pSender, KDvoid* pTargetAction );
	        
	protected :

		/** 
		 *	Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions.
		 */
		virtual CCFiniteTimeAction*		makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );

		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run at the end. */
		virtual CCFiniteTimeAction*		makePostCCAction ( KDvoid );
	    
	private :

		CCFiniteTimeAction*				spawnWithActions ( std::vector<std::pair<APSAction*, CCFiniteTimeAction*> >& aActionPairs );
		std::pair<APSAction*, CCFiniteTimeAction*>		getActionPair ( KDint nActionIndex );

	protected :

		APS_PROPERTY_KEY ( ActionIndexes );

		/** true if action group is supposed to run first among all action groups */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bPrologue, Prologue );
	    
		/** 
		 *	If intact is true, whole action group is interrupted when any action in 
		 *	the group cannot run or is interrupted. 
		 */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bIntact, Intact );

	protected :

		APSResourceArray*		m_pActions;	    
		KDuint					m_uRunningActionCount;     
};

NS_APS_END

#endif	// __APSActionGroup_h__
