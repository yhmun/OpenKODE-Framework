/* --------------------------------------------------------------------------
 *
 *      File            APSRotationAction.h
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

#ifndef __APSRotationAction_h__
#define __APSRotationAction_h__

#include "APSIntervalGraphicAction.h"

NS_APS_BEGIN

class APSRotationAction : public APSIntervalGraphicAction 
{  	    
	public :

		/** Constructor of APSRotationAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSRotationAction ( APSDictionary* pProperties = KD_NULL );    
	    
		APS_CLASS_NAME ( APSRotationAction );

	public :

		/** Initializes APSRotationAction object with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns a type of this action. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeRotation; }

	protected :

		/** Creates and returns an autoreleased cocos2d graphic action object. */
		virtual CCActionInterval*  makeGraphicCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );
	    
		virtual KDvoid  saveNodeState ( KDvoid );
	    
		virtual KDvoid  restoreNodeState ( KDvoid );

		/** Saved rotation of target graphic before rotate action is applied. */
		APS_SYNTHESIZE ( KDfloat, m_fStartRotation, StartRotation );
	    
		/**
		 *	endRotation is the end rotation of target graphic after action is applied.
		 *	If action is relative, endRotation represents relative rotation from target graphic.
		 */
		APS_SYNTHESIZE_WITH_KEY ( KDfloat, m_fEndRotation, EndRotation );
};

NS_APS_END

#endif	// __APSRotationAction_h__
