/* --------------------------------------------------------------------------
 *
 *      File            APSGraphicGroup.h
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

#ifndef __APSGraphicGroup_h__
#define __APSGraphicGroup_h__

#include "APSGraphic.h"

NS_APS_BEGIN

class APSResourceArray;
class APSSpriteSheetHolder;

class APSGraphicGroup : public APSGraphic
{	  
	public :

		APSGraphicGroup ( APSDictionary* pProperties = KD_NULL );

		virtual ~APSGraphicGroup ( KDvoid );    
	    
		APS_CLASS_NAME ( APSGraphicGroup );

	public :

		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		virtual KDbool  initNode ( KDvoid );
	    
		/** Runs an action targeting to this graphic */
		KDvoid  runAction ( APSAction* pAction );
	    
		/** Stops an action targeting to this graphic */
		KDvoid  stopAction ( APSAction* pAction );

		/** 
		 *	Updates accOpacity, accOpacity value propagates to the all descendant
		 *	graphics. Opacities are multiplied from parent to children thoughout 
		 *	graphics hierarchy. For performance reason, updateAccOpacity ( ) should
		 *	be called as minimum times as you can.
		 */
		virtual KDvoid  updateAccOpacity ( KDvoid );
	    
		virtual KDvoid  updateChildSpriteFrames ( KDvoid );

		APSResourceArray*  getChildren ( KDvoid ) const { return m_pChildren; }
	    
		/** Searches and returns a sprite sheet holder of first found among children has */
		APSSpriteSheetHolder*  getFirstSpriteSheetHolder ( KDvoid );
	    
		/**
		 *	Returns a currently running action with a given action type. Except
		 *	audio action, there can be only one running action for each action type in a graphic.
		 */
		APSAction*  getRunningActionForType ( APSActionType uType ) const;
		KDvoid  setRunningAction ( APSAction* pAction, APSActionType uType = kAPSActionTypeNone );
	    
		KDvoid  setEnableActionForType ( KDbool bEnable, APSActionType uType );
		KDbool  isActionEnabledForType ( APSActionType uType ) const;
	    
		KDvoid  setEnableAllActions ( KDbool bEnable );
	    
	protected :
	    
		virtual CCNode*  createNode ( KDvoid );
	    
		APS_PROPERTY_KEY ( ChildIndexes );
	    
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bHasAnimatableChild, HasAnimatableChild );
	    
		APS_SYNTHESIZE_WITH_KEY ( KDuint, m_uDisplayedFrameIndex, DisplayedFrameIndex );
	    
		APS_SYNTHESIZE ( KDuint, m_uRunningFrameIndex, RunningFrameIndex );

	private:

		static inline KDint  getIndexForActionType ( APSActionType uType );
		static APSAction*    getRigidAction ( KDvoid );

	protected :
	    
		APSResourceArray*	m_pChildren;	    
		APSAction*			m_aRunningActions[APS_GRAPHIC_ACTION_TYPES_COUNT];	   
};

NS_APS_END

#endif	// __APSGraphicGroup_h__
