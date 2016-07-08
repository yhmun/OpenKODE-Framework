/* --------------------------------------------------------------------------
 *
 *      File            APSInstantScaleAction.h
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

#ifndef __APSInstantScaleAction_h__
#define __APSInstantScaleAction_h__

#include "APSInstantGraphicAction.h"

NS_APS_BEGIN

class APSInstantScaleAction : public APSInstantGraphicAction 
{	   	    
	public :

		/** Constructor of APSInstantScaleAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSInstantScaleAction ( APSDictionary* pProperties = KD_NULL );	   
	    
		APS_CLASS_NAME ( APSInstantScaleAction );

	public :

		/** Initializes APSInstantScaleAction object with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns a type of this action. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeScale; };
	    
		/**
		 *	Applies instant scale to target graphic. 
		 *	@warning Although instantScale ( ) is declared in public, it should be called solely by CCAction's execution of this action.
		 */
		virtual KDvoid  instantScale ( CCNode* pSender );
	    
	protected :

		/** Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions. */
		virtual CCFiniteTimeAction * makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );

		/** 
		 *	endScale is the end scale of target graphic after action is applied. 
		 *	If action is relative, endScale indicates relative state for target graphic.
		 */
		APS_SYNTHESIZE_WITH_KEY ( CCPoint, m_tEndScale, EndScale );
    
};

NS_APS_END

#endif	// __APSInstantScaleAction_h__
