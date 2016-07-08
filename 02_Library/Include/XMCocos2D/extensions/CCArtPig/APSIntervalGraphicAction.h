/* --------------------------------------------------------------------------
 *
 *      File            APSIntervalGraphicAction.h
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

#ifndef __APSIntervalGraphicActionh__
#define __APSIntervalGraphicActionh__

#include "APSGraphicAction.h"

NS_APS_BEGIN

/*
 *	@brief	APSIntervalGraphicAction class defines common properties of actions that have duration.
 *
 *	- In Cocos2d, CCRepeat does not work with non-relative graphic actions ( eg. CCMoveTo, CCFadeTo, CCScaleTo... ). However, ArtPig Editor assumes that repeating non-relative graphic actions bring target graphic's state to original state and proceeds next round.
 */
class APSIntervalGraphicAction : public APSGraphicAction
{
	public :

		/** Constructor of APSIntervalGraphicAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSIntervalGraphicAction ( APSDictionary* pProperties = KD_NULL );  	   

		APS_ABSTRACT_CLASS_NAME ( APSIntervalGraphicAction );

	public :

		/** Initializes APSIntervalGraphicAction object with a given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns startTime + duration. */
		virtual KDfloat  getEndTime ( KDvoid );                  

		/** Returns true if action restores to original position for each iteration. */
		KDbool  isRestoringAction ( KDvoid ) const;
	    
		/** Returns an autoreleased CCAction object that can be applied to none ArtPig projects.  
		 */
		virtual CCFiniteTimeAction*  universalCCAction ( KDfloat fScale = 1.0f );

		/** Saves current state of target graphic. */
		virtual KDvoid  saveNodeState ( KDvoid ) = 0;
	    
		/** Restores saved value to target graphic. */
		virtual KDvoid  restoreNodeState ( KDvoid ) = 0;

		/** 
		 *	startFunc ( ) is supposed to be called at the beggining of CCAction's execution 
		 *	@warning startFunc ( ) should be only called by CCAction's execution.
		 */
		virtual KDvoid  startFunc ( KDvoid );        
	    
		/**
		 *	stopFunc ( ) is supposed to be called at the end of CCAction's execution
		 *	@warning stopFunc ( ) should be only called by CCAction's execution.
		 */
		virtual KDvoid  stopFunc ( KDvoid );

	protected :
	    
		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions. */
		virtual CCFiniteTimeAction*  makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );
	    
		/** Creates and returns an autoreleased cocos2d graphic action object. */
		virtual CCActionInterval*  makeGraphicCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f ) = 0;
	        
		virtual CCActionInterval*  attachRepeat ( CCActionInterval* pAction );
	    
		virtual CCActionInterval*  attachTimingMode ( CCActionInterval* pAction );
	    
		/** If autoReverse is true, a reversed action is performed after finishing the original action. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bAutoReverse, AutoReverse );
	    
		/** Delay time before reverse action starts. */
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fAutoReverseDelay, AutoReverseDelay );
	    
		/** Action repeats as many times as size of repeat property. */
		APS_SYNTHESIZE_WITH_KEY ( KDuint, m_uRepeat, Repeat );
	    
		/** Pacing of action, such as linear, ease in, ease out, and so on. */
		APS_SYNTHESIZE_WITH_KEY ( KDint, m_nTimingMode, TimingMode );
	    
		/** Parameter for timingMode. timingFactor is used for ease in/out or ease elastic in/out. */
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fTimingFactor, TimingFactor );
	    
		/** If finishRecovery is true, graphic returns to the original state instantly at the end of action. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bFinishRecovery, FinishRecovery );

	protected :

		CCActionInterval*	m_pGraphicCCAction;
};

NS_APS_END

#endif	// __APSIntervalGraphicActionh__
