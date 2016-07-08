/* --------------------------------------------------------------------------
 *
 *      File            cpCCNodeImpl.h
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

#ifndef __cpCCNodeImpl_h__
#define __cpCCNodeImpl_h__

#include "../../../cocoa/CCArray.h"
#include "../../../cocoa/CCGeometry.h"
#include "XMChipmunk/chipmunk.h"

NS_CC_BEGIN

class CCSpaceManager;

/*****
 Unfortunately we can't use multiple inheritance so we must
 use a pattern similar to strategy or envelope/letter, basically
 we've just added an instance of cpCCNode to whatever class
 we wish its functionality to be in. Then we create the same
 interface functions/properties and have then delegate to 
 our instance of cpCCNode, macros are defined below to help
 with this.
 
 -rkb
 *****/
 

/** 
 *	Our protocol that our CocosNode objects follow, these include:
 *	-cpSprite
 *	-cpShapeNode
 *	-cpAtlasSprite
 */
class cpCCNodeProtocol 
{
	public :

		/** Set this if you don't want this object to sync with the body's rotation */
		virtual KDbool				 isIgnoreRotation ( KDvoid ) = 0;
		virtual KDvoid				setIgnoreRotation ( KDbool bIgnore ) = 0;

		/** Set this to true if you want the shape and/or body to be free'd when we're released */
		virtual KDbool				 isAutoFreeShapeAndBody ( KDvoid ) = 0;
		virtual KDvoid				setAutoFreeShapeAndBody ( KDbool bAuto ) = 0;

		/** If this is anything other than zero, a position change will update the shapes velocity using integrationDt to calculate it */
		virtual cpFloat				getIntegrationDelta ( KDvoid )  = 0;
		virtual KDvoid				setIntegrationDelta ( cpFloat fDelta ) = 0;

		/** The shape, if any, that we're connected to. Setting this will also set the body */
		virtual cpShape*			getShape ( KDvoid ) = 0;
		virtual KDvoid				setShape ( cpShape* pShape ) = 0;

		/*! The shapes, each wrapped in an NSValue, that our body is connected to */
		virtual CCArray*			getShapes ( KDvoid ) = 0;

		/** The body, if any, that we're connected to. You may have a body with no shape set */
		virtual cpBody*				getBody ( KDvoid )  = 0;
		virtual KDvoid				setBody ( cpBody* pBody ) = 0;

		/** If autoFreeShapeAndBody is set, then this must be set too */
		virtual CCSpaceManager*		getSpaceManager ( KDvoid ) = 0;
		virtual KDvoid				setSpaceManager ( CCSpaceManager* pManager ) = 0;

	public :

		virtual KDvoid				applyImpulse ( cpVect tImpulse, cpVect tOffset = cpvzero ) { };

		virtual KDvoid				applyForce ( cpVect tForce, cpVect tOffset = cpvzero ) { };

		virtual KDvoid				resetForces ( KDvoid ) { };
};

/** 
 *	Since we can not extend functionality from multiple
 *	class definitions, any class wishing to include this
 *	functionality must serve as a proxy and forward requests
 *	to a (member) object of this type, there are macros in this
 *	file aimed at helping achieve this
 */
class cpCCNodeImpl : public CCObject, public cpCCNodeProtocol 
{
	public :
				 cpCCNodeImpl ( KDvoid );
		virtual ~cpCCNodeImpl ( KDvoid );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDbool			initWithShape ( cpShape* pShape );

		virtual KDbool			initWithBody ( cpBody* pBody );

	public :

		virtual cpShape*		getShape ( KDvoid );
		virtual KDvoid			setShape ( cpShape* pShape );

		virtual CCArray*		getShapes ( KDvoid );

		CC_SYNTHESIZE_BOOL ( m_bIgnoreRotation, IgnoreRotation );

		CC_SYNTHESIZE_BOOL ( m_bAutoFreeShapeAndBody, AutoFreeShapeAndBody );

		CC_SYNTHESIZE ( cpFloat, m_fIntegrationDelta, IntegrationDelta );

		CC_PROPERTY ( cpBody*, m_pBody, Body );

		CC_SYNTHESIZE ( CCSpaceManager*, m_pSpaceManager, SpaceManager );

	public :

		virtual KDbool			setRotation ( KDfloat fRotation );

		virtual	KDvoid			setPosition ( const CCPoint& tPosition );

		virtual KDvoid			applyImpulse ( cpVect tImpulse, cpVect tOffset = cpvzero );

		virtual KDvoid			applyForce ( cpVect tForce, cpVect tOffset = cpvzero );

		virtual KDvoid			resetForces ( KDvoid );
};

/* Macros for attempt at multiple inheritance */

// create our instance
#define CPCCNODE_MEM_VARS_BODY_INIT( _BODY_ )											\
	m_pImplementation = new cpCCNodeImpl ( );											\
	m_pImplementation->initWithBody ( _BODY_ );											\
	if ( _BODY_ ) { _BODY_->data = this; }	

#define CPCCNODE_SYNC_POS_ROT( _BODY_ )													\
	if ( _BODY_ )																		\
	{																					\
		this->setPosition ( cpBodyGetPos ( _BODY_ ) );									\
		this->setRotation ( -CC_RADIANS_TO_DEGREES ( cpBodyGetAngle ( _BODY_ ) ) );		\
	}

NS_CC_END

#endif	// __cpCCNodeImpl_h__