/* --------------------------------------------------------------------------
 *
 *      File            APSVisibilityAction.h
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

#ifndef __APSVisibilityAction_h__
#define __APSVisibilityAction_h__

#include "APSInstantGraphicAction.h"

NS_APS_BEGIN

class APSVisibilityAction : public APSInstantGraphicAction
{
	public :

		/** Constructor of APSVisibilityAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSVisibilityAction ( APSDictionary* pProperties = KD_NULL );  	
	    
		APS_CLASS_NAME ( APSVisibilityAction );

	public :

		/** Initializes APSVisibilityAction object with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns a type of this action. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeVisibility; }

		CCFiniteTimeAction*  universalCCAction ( KDfloat fScale = 1.0f );
	    
	protected :
	    
		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions. */
		virtual CCFiniteTimeAction*  makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );     

		/** endVisibility is the end visibility state of target graphic after action is applied. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bEndVisibility, EndVisibility );
};

NS_APS_END

#endif	// __APSVisibilityAction_h__
