/* --------------------------------------------------------------------------
 *
 *      File            cpCCNode.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2009 Robert Blackwood 02/22/2009
 *
 *         Mobile Bros. All rights reserved.
 *
 *         http://www.mobile-bros.com  
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
 *      version 2 of the License, or (at your option) any later version.
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

#ifndef __cpCCNode_h__
#define __cpCCNode_h__

#include "cpCCNodeImpl.h"
#include "../../../base_nodes/CCNode.h"

NS_CC_BEGIN

class cpCCNode : public CCNodeRGBA, public cpCCNodeProtocol
{
	public :
				 cpCCNode ( KDvoid );
		virtual ~cpCCNode ( KDvoid );

		static cpCCNode*			create ( KDvoid );

		/** perform a self alloc with the given shape */
		static cpCCNode*			create ( cpShape* pShape );

		/** perform a self alloc with the given body */
		static cpCCNode*			create ( cpBody* pBody );

	public :

		virtual KDbool				init ( KDvoid );

		/** init with the given shape */
		virtual KDbool				initWithShape ( cpShape* pShape );

		/** init with the given body */
		virtual KDbool				initWithBody ( cpBody* pBody );

		virtual CCAffineTransform	nodeToParentTransform ( KDvoid );

	public :

		virtual KDvoid				setRotation ( KDfloat fRotation );

		virtual	KDvoid				setPosition ( const CCPoint& tPosition );

		virtual KDvoid				applyImpulse ( cpVect tImpulse, cpVect tOffset = cpvzero );

		virtual KDvoid				applyForce ( cpVect tForce, cpVect tOffset = cpvzero );

		virtual KDvoid				resetForces ( KDvoid );

		virtual KDbool				 isIgnoreRotation ( KDvoid );
		virtual KDvoid				setIgnoreRotation ( KDbool bIgnore );

		virtual KDbool				 isAutoFreeShapeAndBody ( KDvoid );
		virtual KDvoid				setAutoFreeShapeAndBody ( KDbool bAuto );

		virtual cpFloat				getIntegrationDelta ( KDvoid );
		virtual KDvoid				setIntegrationDelta ( cpFloat fDelta );

		virtual cpShape*			getShape ( KDvoid );
		virtual KDvoid				setShape ( cpShape* pShape );

		virtual CCArray*			getShapes ( KDvoid );

		virtual cpBody*				getBody ( KDvoid );
		virtual KDvoid				setBody ( cpBody* pBody );

		virtual CCSpaceManager*		getSpaceManager ( KDvoid );
		virtual KDvoid				setSpaceManager ( CCSpaceManager* pManager );

	protected :

		cpCCNodeImpl*	m_pImplementation;
}; 

NS_CC_END

#endif	// __cpCCNode_h__