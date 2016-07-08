/* --------------------------------------------------------------------------
 *
 *      File            APSFadeAction.h
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

#ifndef __APSFadeAction_h__
#define __APSFadeAction_h__

#include "APSIntervalGraphicAction.h"

NS_APS_BEGIN

class APSFadeTo;

/**
 *	@brief	APSFadeAction defines fade action with some duration.
 *	APSFadeAction cannot be relative.
 */
class APSFadeAction : public APSIntervalGraphicAction 
{		
	public :

		/** Constructor of APSFadeAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSFadeAction ( APSDictionary* pProperties = KD_NULL );

		APS_CLASS_NAME ( APSFadeAction );
		
	public :

		/** Initializes APSFadeAction object with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** Returns a type of this action. */
		virtual APSActionType  getActionType ( KDvoid ) const { return kAPSActionTypeFade; };
	    
	protected :

		/** Creates and returns an autoreleased cocos2d graphic action object. */
		virtual CCActionInterval*  makeGraphicCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );
	    
		virtual KDvoid  saveNodeState ( KDvoid );
	    
		virtual KDvoid  restoreNodeState ( KDvoid );

		/** Saved opacity of target graphic before fade is applied. */
		APS_SYNTHESIZE ( KDuint, m_uStartOpacity, StartOpacity );
	    
		/** endOpacity is the end opacity of target graphic after action is applied. */
		APS_SYNTHESIZE_WITH_KEY ( KDuint, m_uEndOpacity, EndOpacity );
};

NS_APS_END

#endif	// __APSFadeAction_h__
