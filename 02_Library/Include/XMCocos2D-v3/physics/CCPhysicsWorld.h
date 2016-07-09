/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsWorld.h
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

#ifndef __CCPhysicsWorld_h__
#define __CCPhysicsWorld_h__

#include "CCPhysicsSetting.h"
#ifdef CC_USE_PHYSICS

#include <list>
#include <vector>

#include "../base/CCObject.h"
#include "../base/CCGeometry.h"

NS_CC_BEGIN

class PhysicsBody;
class PhysicsJoint;
class PhysicsWorldInfo;
class PhysicsShape;
class PhysicsContact;
class Array;

class Sprite;
class Scene;
class DrawNode;
class PhysicsDebugDraw;

class PhysicsWorld;

typedef struct PhysicsRayCastInfo
{
    PhysicsShape*	shape;
    Point			start;
    Point			end;
    Point			contact;
    Vect			normal;
    KDfloat			fraction;
    KDvoid*			data;
} PhysicsRayCastInfo;

/**
 *	@brief Called for each fixture found in the query. You control how the ray cast
 *	proceeds by returning a float:
 *	return true: continue
 *	return false: terminate the ray cast
 *	@param fixture the fixture hit by the ray
 *	@param point the point of initial intersection
 *	@param normal the normal vector at the point of intersection
 *	@return true to continue, false to terminate
 */
typedef std::function<KDbool(PhysicsWorld& tWorld, const PhysicsRayCastInfo& tInfo, KDvoid* pData)> PhysicsRayCastCallbackFunc;
typedef std::function<KDbool(PhysicsWorld&, PhysicsShape&, KDvoid*)> PhysicsRectQueryCallbackFunc;
typedef PhysicsRectQueryCallbackFunc PhysicsPointQueryCallbackFunc;

/**
 *	@brief An PhysicsWorld object simulates collisions and other physical properties. You do not create PhysicsWorld objects directly; instead, you can get it from an Scene object.
 */
class PhysicsWorld
{
	public :

		static const KDint32		DEBUGDRAW_NONE		= 0x00;
		static const KDint32		DEBUGDRAW_SHAPE		= 0x01;
		static const KDint32		DEBUGDRAW_JOINT		= 0x02;
		static const KDint32		DEBUGDRAW_CONTACT	= 0x04;
		static const KDint32		DEBUGDRAW_ALL		= DEBUGDRAW_SHAPE | DEBUGDRAW_JOINT | DEBUGDRAW_CONTACT;
    
	public :

		/** Adds a joint to the physics world.*/
		virtual KDvoid				addJoint ( PhysicsJoint* pJoint );
		/** Removes a joint from the physics world.*/
		virtual KDvoid				removeJoint ( PhysicsJoint* pJoint, KDbool bDestroy );
		/** Remove all joints from the physics world.*/
		virtual KDvoid				removeAllJoints ( KDbool bDestroy );
    
		virtual KDvoid				removeBody ( PhysicsBody* pBody );
		virtual KDvoid				removeBody ( KDint nTag );
		virtual KDvoid				removeAllBodies ( KDvoid );
    
		KDvoid						rayCast ( PhysicsRayCastCallbackFunc pFunc, const Point& tPoint1, const Point& tPoint2, KDvoid* pData );
		KDvoid						queryRect ( PhysicsRectQueryCallbackFunc pFunc, const Rect& tRect, KDvoid* pData );
		KDvoid						queryPoint ( PhysicsPointQueryCallbackFunc pFunc, const Point& tPoint, KDvoid* pData );
		Array*						getShapes ( const Point& tPoint ) const;
		PhysicsShape*				getShape ( const Point& tPoint ) const;
		Array*						getAllBodies ( KDvoid ) const;
		PhysicsBody*				getBody ( KDint nTag ) const;   
    
		inline Scene&				getScene ( KDvoid ) const { return *m_pScene; }
		/** get the gravity value */
		inline Vect					getGravity ( KDvoid ) const { return m_tGravity; }
		/** set the gravity value */
		KDvoid						setGravity ( const Vect& tGravity );
    
		/** set the debug draw */
		KDvoid						setDebugDrawMask ( KDint nMask );
		inline KDint				getDebugDrawMask ( KDvoid ) { return m_nDebugDrawMask; }
    
	protected :

		static PhysicsWorld*		construct ( Scene& tScene );
		KDbool						init ( Scene& tScene );
    
		virtual KDvoid				addBody ( PhysicsBody* pBody );
		virtual KDvoid				addShape ( PhysicsShape* pShape );
		virtual KDvoid				removeShape ( PhysicsShape* pShape );

		virtual KDvoid				update ( KDfloat fDelta );
    
		virtual KDvoid				debugDraw ( KDvoid );
    
		virtual KDint				collisionBeginCallback		( PhysicsContact& tContact );
		virtual KDint				collisionPreSolveCallback	( PhysicsContact& tContact );
		virtual KDvoid				collisionPostSolveCallback	( PhysicsContact& tContact );
		virtual KDvoid				collisionSeparateCallback	( PhysicsContact& tContact );
    
		virtual KDvoid				doAddBody			( PhysicsBody* pBody );
		virtual KDvoid				doRemoveBody		( PhysicsBody* pBody );
		
		virtual KDvoid				doAddJoint			( PhysicsJoint* pJoint );
		virtual KDvoid				doRemoveJoint		( PhysicsJoint* pJoint );
		
		virtual KDvoid				addBodyOrDelay		( PhysicsBody* pBody );
		virtual KDvoid				removeBodyOrDelay	( PhysicsBody* pBody );

		virtual KDvoid				addJointOrDelay		( PhysicsJoint* pJoint );
		virtual KDvoid				removeJointOrDelay	( PhysicsJoint* pJoint );

		virtual KDvoid				updateBodies ( KDvoid );
		virtual KDvoid				updateJoints ( KDvoid );
    
	protected :

		Vect						m_tGravity;
		KDfloat						m_fSpeed;
		PhysicsWorldInfo*			m_pInfo;
    
		Array*						m_pBodies;
		std::list<PhysicsJoint*>	m_aJoints;
		Scene*						m_pScene;
    
		KDbool						m_bDelayDirty;
		PhysicsDebugDraw*			m_pDebugDraw;
		KDint						m_nDebugDrawMask;
    
    
		Array*						m_pDelayAddBodies;
		Array*						m_pDelayRemoveBodies;
		std::vector<PhysicsJoint*>	m_aDelayAddJoints;
		std::vector<PhysicsJoint*>	m_aDelayRemoveJoints;
    
	protected :

		PhysicsWorld ( KDvoid );
		virtual ~PhysicsWorld ( KDvoid );
    
		friend class Sprite;
		friend class Scene;
		friend class PhysicsBody;
		friend class PhysicsShape;
		friend class PhysicsJoint;
		friend class PhysicsWorldCallback;
		friend class PhysicsDebugDraw;
};


class PhysicsDebugDraw
{
	protected :

		virtual KDbool				begin ( KDvoid );
		virtual KDvoid				end ( KDvoid );
		virtual KDvoid				drawShape ( PhysicsShape& tShape );
		virtual KDvoid				drawJoint ( PhysicsJoint& tJoint );
		virtual KDvoid				drawContact ( KDvoid );
    
	protected :

		PhysicsDebugDraw ( PhysicsWorld& tWorld );
		virtual ~PhysicsDebugDraw ( KDvoid );
    
	protected :

		DrawNode*					m_pDrawNode;
		PhysicsWorld&				m_tWorld;
    
		friend class PhysicsWorld;
};

NS_CC_END

#endif // CC_USE_PHYSICS
#endif // __CCPhysicsWorld_h__
