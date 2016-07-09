/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsDebugNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org
 *      Copyright (c) 2012      Scott Lembcke and Howling Moon Software
 *
 *         http://www.cocos2d-x.org      
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

#ifndef __CCPhysicsDebugNode_h__
#define __CCPhysicsDebugNode_h__

#include "../ExtensionMacros.h"
#include "../../2d/draw_nodes/CCDrawNode.h"
#include "XMChipmunk/chipmunk.h"

NS_CC_EXT_BEGIN

/**
 *	A BaseData that draws the components of a physics engine.
 *
 *	Supported physics engines:
 *		- Chipmunk
 *		- Objective-Chipmunk
 *
 *	@since v2.1
 */

class PhysicsDebugNode : public DrawNode
{
	public :

		/** Create a debug node for a regular Chipmunk space. */
		static PhysicsDebugNode*	create ( cpSpace* pSpace );

		/**
		 *	@js ctor
		 */
		PhysicsDebugNode ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~PhysicsDebugNode ( KDvoid );
    

		cpSpace*					getSpace ( KDvoid ) const;
		KDvoid						setSpace ( cpSpace* pSpace );
    
		// Overrides
		virtual KDvoid				draw ( KDvoid ) override;

	protected :

		cpSpace*					m_pSpacePtr;
};

NS_CC_EXT_END

#endif	// __CCPhysicsDebugNode_h__
