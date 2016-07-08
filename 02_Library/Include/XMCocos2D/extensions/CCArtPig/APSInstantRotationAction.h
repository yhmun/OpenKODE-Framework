/* --------------------------------------------------------------------------
 *
 *      File            APSInstantRotationAction.h
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

#ifndef __APSInstantRotationAction_h__
#define __APSInstantRotationAction_h__

#include "APSInstantGraphicAction.h"

NS_APS_BEGIN

/**
 *	@brief	APSInstantRotationAction
 */
class APSInstantRotationAction : public APSInstantGraphicAction 
{	    
	public :

		/** Constructor of APSInstantRotationAction. If 'properties' parameter is not NULL, the instance is initialized by the properties.*/
		APSInstantRotationAction ( APSDictionary* pProperties = KD_NULL );  	   
	    
		APS_CLASS_NAME ( APSInstantRotationAction );

	public :

		/** Initializes APSInstantRotationAction object with given properties. */
		virtual KDbool initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns a type of this action. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeRotation; }

		/** 
		 *	Applies instant rotation to target graphic. 
		 *	@warning Although instantRotation ( ) is declared in public, it should be called solely by CCAction's execution of this action.
		 */
		KDvoid  instantRotation ( CCNode* pSender );
	    
	protected :

		/** 
		 *	Creates and returns an autoreleased CCFiniteTimeAction object that is supposed to run between pre and post actions.
		 */
		virtual CCFiniteTimeAction*  makeMidCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );
    
		/** 
		 *	endRotation is the end rotation of target graphic after action is applied. 
		 *	If action is relative, endRotation indicates relative state for target graphic.
		 */
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fEndRotation, EndRotation )
};

NS_APS_END

#endif	// __APSInstantRotationAction_h__
