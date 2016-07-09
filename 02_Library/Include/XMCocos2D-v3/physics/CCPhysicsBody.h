/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsBody.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCPhysicsBody_h__
#define __CCPhysicsBody_h__

#include "CCPhysicsSetting.h"
#ifdef CC_USE_PHYSICS

#include "../base/CCObject.h"
#include "../base/CCGeometry.h"
#include "../base/CCArray.h"

#include "CCPhysicsShape.h"

#include <vector>

NS_CC_BEGIN
class Sprite;
class PhysicsWorld;
class PhysicsJoint;

class PhysicsBodyInfo;


const PhysicsMaterial PHYSICSBODY_MATERIAL_DEFAULT ( 0.1f, 0.5f, 0.5f );

/**
 *	A body affect by physics.
 *	it can attach one or more shapes.
 *	if you create body with createXXX, it will automatically compute mass and moment with density your specified(which is PHYSICSBODY_MATERIAL_DEFAULT by default, and the density value is 0.1f), and it based on the formular: mass = density * area.
 *	if you create body with createEdgeXXX, the mass and moment will be INFINITY by default. and it's a static body.
 *	you can change mass and moment with setMass() and setMoment(). and you can change the body to be dynamic or static by use function setDynamic().
 */
class PhysicsBody : public Object
{
	public :
		/** create a body with defult mass and moment. */
		static PhysicsBody*				create ( KDvoid );

		/** create a body with mass and defult moment. */
		static PhysicsBody*				create ( KDfloat fMass );

		/** create a body with mass and moment. */
		static PhysicsBody*				create ( KDfloat fMass, KDfloat fMoment );

		/** Create a body contains a circle shape. */
		static PhysicsBody*				createCircle ( KDfloat fRadius, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );

		/** Create a body contains a box shape. */
		static PhysicsBody*				createBox ( const Size& tSize, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );

		/**
		 *	@brief Create a body contains a polygon shape.
		 *	points is an array of Point structs defining a convex hull with a clockwise winding.
		 */
		static PhysicsBody*				createPolygon ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );
    
		/** Create a body contains a EdgeSegment shape. */
		static PhysicsBody*				createEdgeSegment ( const Point& a, const Point& b, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, KDfloat fBorder = 1 );

		/** Create a body contains a EdgeBox shape. */
		static PhysicsBody*				createEdgeBox ( const Size& tSize, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, KDfloat border = 1, const Point& tOffset = Point::ZERO );
		
		/** Create a body contains a EdgePolygon shape. */
		static PhysicsBody*				createEdgePolygon ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, KDfloat fBorder = 1 );
		
		/** Create a body contains a EdgeChain shape. */
		static PhysicsBody*				createEdgeChain ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSBODY_MATERIAL_DEFAULT, KDfloat fBorder = 1 );
    
		/**
		 *	@brief add a shape to body
		 *	@param shape the shape to be added
		 *	@param addMassAndMoment if this is true, the shape's mass and moment will be added to body. the default is true
		 */
		virtual PhysicsShape*			addShape ( PhysicsShape* pShape, KDbool bAddMassAndMoment = true );
		/**
		 *	@brief remove a shape from body
		 *	@param shape the shape to be removed
		 *	@param reduceMassAndMoment if this is true, the body mass and moment will be reduced by shape. the default is true
		 */
		KDvoid							removeShape ( PhysicsShape* pShape, KDbool bReduceMassAndMoment = true );

		/**
		 *	@brief remove a shape from body
		 *	@param tag the tag of the shape to be removed
		 *	@param reduceMassAndMoment if this is true, the body mass and moment will be reduced by shape. the default is true
		 */
		KDvoid							removeShape ( KDint nTag, KDbool bReduceMassAndMoment = true );

		/* remove all shapes */
		KDvoid							removeAllShapes ( KDbool bReduceMassAndMoment = true );

		/* get the body shapes. */
		inline Array*					getShapes ( KDvoid ) const { return m_pShapes; }

		/* get the first shape of the body shapes. */
		inline PhysicsShape*			getFirstShape ( KDvoid ) const { return m_pShapes->count ( ) >= 1 ? dynamic_cast<PhysicsShape*> ( m_pShapes->getObjectAtIndex ( 0 ) ) : nullptr; }

		/* get the shape of the body. */
		PhysicsShape*					getShape ( KDint nTag ) const;
    
		/** Applies a immediate force to body. */
		virtual KDvoid					applyForce ( const Vect& tForce );
		
		/** Applies a immediate force to body. */
		virtual KDvoid					applyForce ( const Vect& tForce, const Point& tOffset );
		
		/** reset all the force applied to body. */
		virtual KDvoid					resetForces ( KDvoid );
		
		/** Applies a continuous force to body. */
		virtual KDvoid					applyImpulse ( const Vect& tImpulse );
		
		/** Applies a continuous force to body. */
		virtual KDvoid					applyImpulse ( const Vect& tImpulse, const Point& tOffset );
		
		/** Applies a torque force to body. */
		virtual KDvoid					applyTorque ( KDfloat fTorque );
    
		/** set the velocity of a body */
		virtual KDvoid					setVelocity ( const Vect& tVelocity );

		/** get the velocity of a body */
		virtual Point					getVelocity ( KDvoid );

		/** set the angular velocity of a body */
		virtual KDvoid					setAngularVelocity ( KDfloat fVelocity );
		
		/** get the angular velocity of a body at a local point */
		virtual Point					getVelocityAtLocalPoint ( const Point& tPoint );

		/** get the angular velocity of a body at a world point */
		virtual Point					getVelocityAtWorldPoint ( const Point& tPoint );
		
		/** get the angular velocity of a body */
		virtual KDfloat					getAngularVelocity ( KDvoid );
		
		/** set the max of velocity */
		virtual KDvoid					setVelocityLimit ( KDfloat fLimit );

		/** get the max of velocity */
		virtual KDfloat					getVelocityLimit ( KDvoid );

		/** set the max of angular velocity */
		virtual KDvoid					setAngularVelocityLimit ( KDfloat fLimit );
		
		/** get the max of angular velocity */
		virtual KDfloat					getAngularVelocityLimit ( KDvoid );
    
		/** remove the body from the world it added to */
		KDvoid							removeFromWorld ( KDvoid );
    
		/** get the world body added to. */
		inline PhysicsWorld*			getWorld ( KDvoid ) const { return m_pWorld; }

		/** get all joints the body have */
		inline const std::vector<PhysicsJoint*>&	getJoints ( KDvoid ) const { return m_aJoints; }
    
		/** get the sprite the body set to. */
		inline Node*					getNode ( KDvoid ) const { return m_pNode; }
    
		/**
		 *	A mask that defines which categories this physics body belongs to.
		 *	Every physics body in a scene can be assigned to up to 32 different categories, each corresponding to a bit in the bit mask. You define the mask values used in your game. In conjunction with the collisionBitMask and contactTestBitMask properties, you define which physics bodies interact with each other and when your game is notified of these interactions.
		 *	The default value is 0xFFFFFFFF (all bits set).
		 */
		KDvoid							setCategoryBitmask ( KDint nBitmask );

		/** 
		 *	A mask that defines which categories of bodies cause intersection notifications with this physics body.
		 *	When two bodies share the same space, each body’s category mask is tested against the other body’s contact mask by performing a logical AND operation. If either comparison results in a non-zero value, an PhysicsContact object is created and passed to the physics world’s delegate. For best performance, only set bits in the contacts mask for interactions you are interested in.
		 *	The default value is 0x00000000 (all bits cleared).
		 */
		KDvoid							setContactTestBitmask ( KDint nBitmask );

		/**
		 *	A mask that defines which categories of physics bodies can collide with this physics body.
		 *	When two physics bodies contact each other, a collision may occur. This body’s collision mask is compared to the other body’s category mask by performing a logical AND operation. If the result is a non-zero value, then this body is affected by the collision. Each body independently chooses whether it wants to be affected by the other body. For example, you might use this to avoid collision calculations that would make negligible changes to a body’s velocity.
		 *	The default value is 0xFFFFFFFF (all bits set).
		 */
		KDvoid							setCollisionBitmask ( KDint nBitmask );

		/** get the category bit mask */
		inline KDint					getCategoryBitmask ( KDvoid ) const { return m_nCategoryBitmask; }

		/** get the contact test bit mask */
		inline KDint					getContactTestBitmask ( KDvoid ) const { return m_nContactTestBitmask; }

		/** get the collision bit mask */
		inline KDint					getCollisionBitmask ( KDvoid ) const { return m_nCollisionBitmask; }
    
		/** 
		 *	set the group of body
		 *	Collision groups let you specify an integral group index. You can have all fixtures with the same group index always collide (positive index) or never collide (negative index)
		 *	it have high priority than bit masks
		 */
		KDvoid							setGroup ( KDint nGroup );

		/** get the group of body */
		inline KDint					getGroup ( KDvoid ) const { return m_nGroup; }
    
		/** get the body position. */
		Point							getPosition ( KDvoid ) const;

		/** get the body rotation. */
		KDfloat							getRotation ( KDvoid ) const;
    
		/**
		 *	@brief test the body is dynamic or not.
		 *	a dynamic body will effect with gravity.
		 */
		inline KDbool					isDynamic ( KDvoid ) const { return m_bDynamic; }

		/**
		 *	@brief set dynamic to body.
		 *	a dynamic body will effect with gravity.
		 */
		KDvoid							setDynamic ( KDbool bDynamic );
    
		/**
		 *	@brief set the body mass.
		 *	@note if you need add/subtract mass to body, don't use setMass(getMass() +/- mass), because the mass of body may be equal to PHYSICS_INFINITY, it will cause some unexpected result, please use addMass() instead.
		 */
		KDvoid							setMass ( KDfloat fMass );

		/** get the body mass. */
		inline KDfloat					getMass ( KDvoid ) const { return m_fMass; }

		/**
		 *	@brief add mass to body.
		 *	if _mass(mass of the body) == PHYSICS_INFINITY, it remains.
		 *	if mass == PHYSICS_INFINITY, _mass will be PHYSICS_INFINITY.
		 *	if mass == -PHYSICS_INFINITY, _mass will not change.
		 *	if mass + _mass <= 0, _mass will equal to MASS_DEFAULT(1.0)
		 *	other wise, mass = mass + _mass;
		 */
		KDvoid							addMass ( KDfloat fMass );
    
		/**
		 *	@brief set the body moment of inertia.
		 *	@note if you need add/subtract moment to body, don't use setMoment(getMoment() +/- moment), because the moment of body may be equal to PHYSICS_INFINITY, it will cause some unexpected result, please use addMoment() instead.
		 */
		KDvoid							setMoment ( KDfloat fMoment );

		/** get the body moment of inertia. */
		inline KDfloat					getMoment ( KDvoid ) const { return m_fMoment; }

		/**
		 *	@brief add moment of inertia to body.
		 *	if _moment(moment of the body) == PHYSICS_INFINITY, it remains.
		 *	if moment == PHYSICS_INFINITY, _moment will be PHYSICS_INFINITY.
		 *	if moment == -PHYSICS_INFINITY, _moment will not change.
		 *	if moment + _moment <= 0, _moment will equal to MASS_DEFAULT(1.0)
		 *	other wise, moment = moment + _moment;
		 */
		KDvoid							addMoment ( KDfloat fMoment );
		
		/** get linear damping. */
		inline KDfloat					getLinearDamping ( KDvoid ) const { return m_fLinearDamping; }

		/** 
		 *	set linear damping.
		 *	it is used to simulate fluid or air friction forces on the body. 
		 *	the value is 0.0f to 1.0f. 
		 */
		inline KDvoid					setLinearDamping ( KDfloat fDamping ) { m_fLinearDamping = fDamping; }

		/** get angular damping. */
		inline KDfloat					getAngularDamping ( KDvoid ) const { return m_fAngularDamping; }

		/**
		 *	set angular damping.
		 *	it is used to simulate fluid or air friction forces on the body.
		 *	the value is 0.0f to 1.0f.
		 */
		inline KDvoid					setAngularDamping ( KDfloat fDamping ) { m_fAngularDamping = fDamping; }
    
		/** whether the body is at rest */
		KDbool							isResting ( KDvoid ) const;

		/** 
		 *	whether the body is enabled
		 *	if the body it isn't enabled, it will not has simulation by world
		 */
		inline KDbool					isEnabled ( KDvoid ) const { return m_bEnable; }

		/**
		 *	set the enable value.
		 *	if the body it isn't enabled, it will not has simulation by world
		 */
		KDvoid							setEnable ( KDbool bEnable );
    
		/** whether the body can rotation */
		inline KDbool					isRotationEnabled ( KDvoid ) const { return m_bRotationEnable; }
		
		/** set the body is allow rotation or not */
		KDvoid							setRotationEnable ( KDbool bEnable );
    
		/** whether this physics body is affected by the physics world’s gravitational force. */
		inline KDbool					isGravityEnabled ( KDvoid ) const { return m_bGravityEnable; }

		/** set the body is affected by the physics world's gravitational force or not. */
		KDvoid							setGravityEnable ( KDbool bEnable );
    
		/** get the body's tag */
		inline KDint					getTag ( KDvoid ) const { return m_nTag; }

		/** set the body's tag */
		inline KDvoid					setTag ( KDint nTag ) { m_nTag = nTag; }
    
		/** convert the world point to local */
		Point							world2Local ( const Point& tPoint );
		
		/** convert the local point to world */
		Point							local2World ( const Point& tPoint );
    
	protected :
    
		KDbool							init ( KDvoid );
    
		virtual KDvoid					setPosition ( Point tPosition );
		virtual KDvoid					setRotation ( KDfloat fRotation );
    
		virtual KDvoid					update ( KDfloat fDelta ) override;
    
		KDvoid							removeJoint ( PhysicsJoint* pJoint );
    
	protected :

				 PhysicsBody ( KDvoid );
		virtual ~PhysicsBody ( KDvoid );
    
	protected :

		Node*							m_pNode;
		std::vector<PhysicsJoint*>		m_aJoints;
		Array*							m_pShapes;
		PhysicsWorld*					m_pWorld;
		PhysicsBodyInfo*				m_pInfo;
		KDbool							m_bDynamic;
		KDbool							m_bEnable;
		KDbool							m_bRotationEnable;
		KDbool							m_bGravityEnable;
		KDbool							m_bMassDefault;
		KDbool							m_bMomentDefault;
		KDfloat							m_fMass;
		KDfloat							m_fArea;
		KDfloat							m_fDensity;
		KDfloat							m_fMoment;
		KDfloat							m_fLinearDamping;
		KDfloat							m_fAngularDamping;
		KDint							m_nTag;
    
		KDint							m_nCategoryBitmask;
		KDint							m_nCollisionBitmask;
		KDint							m_nContactTestBitmask;
		KDint							m_nGroup;
    
		friend class PhysicsWorld;
		friend class PhysicsShape;
		friend class PhysicsJoint;
		friend class Node;
};

NS_CC_END

#endif	// CC_USE_PHYSICS
#endif	// __CCPhysicsBody_h__
