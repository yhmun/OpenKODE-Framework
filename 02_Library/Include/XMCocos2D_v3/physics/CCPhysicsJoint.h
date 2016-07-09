/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsJoint.h
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

#ifndef __CCPhysicsJoint_h__
#define __CCPhysicsJoint_h__

#include "CCPhysicsSetting.h"
#ifdef CC_USE_PHYSICS

#include "../base/CCObject.h"
#include "../base/CCGeometry.h"

NS_CC_BEGIN

class PhysicsBody;
class PhysicsWorld;
class PhysicsJointInfo;
class PhysicsBodyInfo;

/*
 * @brief An PhysicsJoint object connects two physics bodies together.
 */
class PhysicsJoint
{
	protected :

				 PhysicsJoint ( KDvoid );
		virtual ~PhysicsJoint ( KDvoid ) = 0;

	public :

		inline PhysicsBody*				getBodyA ( KDvoid ) const { return m_pBodyA; }
		inline PhysicsBody*				getBodyB ( KDvoid ) const { return m_pBodyB; }
		inline PhysicsWorld*			getWorld ( KDvoid ) const { return m_pWorld; }
		inline KDint					getTag ( KDvoid ) const { return m_nTag; }
		inline KDvoid					setTag ( KDint nTag ) { m_nTag = nTag; }		
		inline KDbool					isEnabled ( KDvoid ) const { return m_bEnable; }
		KDvoid							setEnable ( KDbool bEnable );
		inline KDbool					isCollisionEnabled ( KDvoid ) const { return m_bCollisionEnable; }
		KDvoid							setCollisionEnable ( KDbool bEnable );
		KDvoid							removeFormWorld ( KDvoid );
		static KDvoid					destroy ( PhysicsJoint* pJoint );
    
	protected :

		KDbool							init ( PhysicsBody* a, PhysicsBody* b );
    
		/**
		 *	PhysicsShape is PhysicsBody's friend class, but all the subclasses isn't. so this method is use for subclasses to catch the bodyInfo from PhysicsBody.
		 */
		PhysicsBodyInfo*				getBodyInfo ( PhysicsBody* pBody ) const;
		Node*							getBodyNode ( PhysicsBody* pBody ) const;
    
	protected :

		PhysicsBody*					m_pBodyA;
		PhysicsBody*					m_pBodyB;
		PhysicsWorld*					m_pWorld;
		PhysicsJointInfo*				m_pInfo;
		KDbool							m_bEnable;
		KDbool							m_bCollisionEnable;
		KDbool							m_bDestoryMark;
		KDint							m_nTag;
    
		friend class PhysicsBody;
		friend class PhysicsWorld;
		friend class PhysicsDebugDraw;
};

/**
 *	@brief A fixed joint fuses the two bodies together at a reference point. Fixed joints are useful for creating complex shapes that can be broken apart later.
 */
class PhysicsJointFixed : public PhysicsJoint
{
	public :

		static PhysicsJointFixed*		construct ( PhysicsBody* a, PhysicsBody* b, const Point& anchr );
    
	protected :

		KDbool							init ( PhysicsBody* a, PhysicsBody* b, const Point& anchr );
    
	protected :
				 PhysicsJointFixed ( KDvoid );
		virtual ~PhysicsJointFixed ( KDvoid );
};

/**
 *	@brief A limit joint imposes a maximum distance between the two bodies, as if they were connected by a rope.
 */
class PhysicsJointLimit : public PhysicsJoint
{
	public :

		static PhysicsJointLimit*		construct ( PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2 );
    
		KDfloat							getMin ( KDvoid ) const;
		KDvoid							setMin ( KDfloat fMin );
		KDfloat							getMax ( KDvoid ) const;
		KDvoid							setMax ( KDfloat fMax );
    
	protected :

		KDbool							init ( PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2 );
    
	protected :

				 PhysicsJointLimit ( KDvoid );
		virtual ~PhysicsJointLimit ( KDvoid );
};

/**
 *	@brief A pin joint allows the two bodies to independently rotate around the anchor point as if pinned together.
 */
class PhysicsJointPin : public PhysicsJoint
{
	public :

		static PhysicsJointPin*			construct ( PhysicsBody* a, PhysicsBody* b, const Point& anchr );
    
		KDvoid							setMaxForce(float force);
		KDfloat							getMaxForce() const;
    
	protected :

		KDbool							init ( PhysicsBody* a, PhysicsBody* b, const Point& anchr );
    
	protected :

				 PhysicsJointPin ( KDvoid );
		virtual ~PhysicsJointPin ( KDvoid );
};

class PhysicsJointDistance : public PhysicsJoint
{
	public :

		static PhysicsJointDistance*	construct ( PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2 );
    
	protected :

		KDbool							init ( PhysicsBody* a, PhysicsBody* b, const Point& anchr1, const Point& anchr2 );
    
	protected :

				 PhysicsJointDistance ( KDvoid );
		virtual ~PhysicsJointDistance ( KDvoid );
};

NS_CC_END

#endif	// CC_USE_PHYSICS
#endif	// __CCPhysicsJoint_h__
