/* --------------------------------------------------------------------------
 *
 *      File            APSBezierMoveAction.h
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

#ifndef __APSBezierMoveAction_h__
#define __APSBezierMoveAction_h__

/**
 *   - Position of the first anchor is used for start position, and it is assigned from graphic's position at the point that the graphic is begin moves.
 */

#include "APSIntervalGraphicAction.h"
#include "APSAnchor.h"

NS_APS_BEGIN

class APSBezierMoveAction : public APSIntervalGraphicAction 
{
	public :

		/** Constructor of APSBezierMoveAction. If 'properties' parameter is not NULL, the instance is initialized by the properties. */
		APSBezierMoveAction ( APSDictionary* pProperties = KD_NULL );    
		virtual ~APSBezierMoveAction ( KDvoid );
	    
		APS_CLASS_NAME ( APSBezierMoveAction );

	public :

		/** Initializes APSBezierMoveAction object with given properties. */
		virtual KDbool				initWithProperties ( APSDictionary* pProperties );
	        
		std::vector<APSAnchor*>*	getAnchors ( KDvoid );
	    
	protected :

		virtual KDvoid				saveNodeState ( KDvoid );
	    
		virtual KDvoid				restoreNodeState ( KDvoid );
	    
		/** Creates and returns an autoreleased cocos2d graphic action object.
		 */
		virtual CCActionInterval*	makeGraphicCCAction ( KDbool bUniversal = KD_FALSE, KDfloat fScale = 1.0f );

		APS_PROPERTY_KEY ( Anchors );

	protected :

		std::vector<APSAnchor*>		m_aAnchors;
};

NS_APS_END

#endif	// __APSBezierMoveAction_h__
