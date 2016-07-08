/* --------------------------------------------------------------------------
 *
 *      File            APSGraphicAction.h
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

#ifndef __APSGraphicAction_h__
#define __APSGraphicAction_h__

#include "APSAction.h"

NS_APS_BEGIN

/**
 *	@brief	APSGraphicAction defines general properties and methods of graphic related actions.
 */
class APSGraphicAction : public APSAction 
{
	public :

		/** Constructor of APSGraphicAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSGraphicAction ( APSDictionary* pProperties = KD_NULL );   
	    
		APS_CLASS_NAME ( APSGraphicAction );

	public :

		/** Initializes APSGraphicAction object with given properties. */
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );

		/** Returns true if action is a kind of graphic actions. */
		static  KDbool  isGraphicAction ( APSAction* pAction );
	    
		/** Returns an autoreleased CCAction object that can be applied to none ArtPig projects. */
		virtual CCFiniteTimeAction*  universalCCAction ( KDfloat fScale = 1.0f );
	    
		/** Saves current state of target graphic. */
		virtual KDvoid  saveNodeState ( KDvoid ) { };
	    
		/** Restores saved value to target graphic. */
		virtual KDvoid  restoreNodeState ( KDvoid ) { };

	protected :    

		/** true if the end state of target graphic is relative to beginning state of it. */
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bRelative, Relative );
};

NS_APS_END

#endif	// __APSGraphicAction_h__
