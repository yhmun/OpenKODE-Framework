/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsContact.h
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

#ifndef __CCPhysicsContact_h__
#define __CCPhysicsContact_h__

#include "CCPhysicsSetting.h"
#ifdef CC_USE_PHYSICS

#include "../base/CCObject.h"
#include "../base/CCGeometry.h"
#include "../2d/event_dispatcher/CCEventListenerCustom.h"
#include "../2d/event_dispatcher/CCEvent.h"

NS_CC_BEGIN

class PhysicsShape;
class PhysicsBody;
class PhysicsWorld;

class PhysicsContactInfo;


typedef struct PhysicsContactData
{
    Point			points [ PHYSICS_CONTACT_POINT_MAX ];
    KDint			count;
    Point			normal;
    
    PhysicsContactData ( KDvoid )
    : count ( 0 )
    {

	}
} PhysicsContactData;

/**
 * @brief Contact infomation. it will created automatically when two shape contact with each other. and it will destoried automatically when two shape separated.
 */
class PhysicsContact : Event
{
	public :
    
		enum class EventCode
		{
			NONE,
			BEGIN,
			PRESOLVE,
			POSTSOLVE,
			SEPERATE
		};
    
		/** get contact shape A. */
		inline PhysicsShape*							getShapeA ( KDvoid ) const { return m_pShapeA; }
		/** get contact shape B. */
		inline PhysicsShape*							getShapeB ( KDvoid ) const { return m_pShapeB; }
		/** get contact data */
		inline const PhysicsContactData*				getContactData ( KDvoid ) const { return m_pContactData; }
		/** get data. */
		inline KDvoid*									getData ( KDvoid ) const { return m_pData; }
		/**
		 *	@brief set data to contact. you must manage the memory yourself, Generally you can set data at contact begin, and distory it at contact seperate.
		 */
		inline KDvoid									setData ( KDvoid* pData ) { m_pData = pData; }
		/** get the event code */
		EventCode										getEventCode ( KDvoid ) const { return m_eEventCode; }
    
	private :

		static PhysicsContact*							construct ( PhysicsShape* a, PhysicsShape* b );

		KDbool											init ( PhysicsShape* a, PhysicsShape* b );
    
		KDvoid											setEventCode ( EventCode eEventCode ) { m_eEventCode = eEventCode; };
		inline KDbool									isNotificationEnabled ( KDvoid ) const { return m_bNotificationEnable; }
		inline KDvoid									setNotificationEnable ( KDbool bEnable ) { m_bNotificationEnable = bEnable; }
		inline PhysicsWorld*							getWorld ( KDvoid ) const { return m_pWorld; }
		inline KDvoid									setWorld ( PhysicsWorld* pWorld ) { m_pWorld = pWorld; }
		inline KDvoid									setResult ( KDbool bResult ) { m_bResult = bResult; }
		inline KDbool									resetResult ( KDvoid ) { KDbool bRet = m_bResult; m_bResult = true; return bRet; }
    
		KDvoid											generateContactData ( KDvoid );
    
	private :

		 PhysicsContact ( KDvoid );
		~PhysicsContact ( KDvoid );
    
	private :

		PhysicsWorld*									m_pWorld;
		PhysicsShape*									m_pShapeA;
		PhysicsShape*									m_pShapeB;
		EventCode										m_eEventCode;
		PhysicsContactInfo*								m_pInfo;
		KDbool											m_bNotificationEnable;
		KDbool											m_bBegin;
		KDbool											m_bResult;
    
		KDvoid*											m_pData;
		KDvoid*											m_pContactInfo;
		PhysicsContactData*								m_pContactData;
    
		friend class EventListenerPhysicsContact;
		friend class PhysicsWorldCallback;
		friend class PhysicsWorld;
};

/**
 *	@brief presolve value generated when onContactPreSolve called.
 */
class PhysicsContactPreSolve
{
	public :

		/** get elasticity between two bodies*/
		KDfloat											getElasticity ( KDvoid ) const;
		/** get friction between two bodies*/
		KDfloat											getFriction ( KDvoid ) const;
		/** get surface velocity between two bodies*/
		Point											getSurfaceVelocity ( KDvoid ) const;
		/** set the elasticity*/
		KDvoid											setElasticity ( KDfloat fElasticity );
		/** set the friction*/
		KDvoid											setFriction ( KDfloat fFriction );
		/** set the surface velocity*/
		KDvoid											setSurfaceVelocity ( const Vect& tVelocity );
		/** ignore the rest of the contact presolve and postsolve callbacks */
		KDvoid											ignore ( KDvoid );
    
	private :

		 PhysicsContactPreSolve ( PhysicsContactData* pData, KDvoid* pContactInfo );
		~PhysicsContactPreSolve ( KDvoid );
    
	private :

		KDfloat											m_fElasticity;
		KDfloat											m_fFriction;
		Point											m_tSurfaceVelocity;
		PhysicsContactData*								m_pPreContactData;
		KDvoid*											m_pContactInfo;
    
		friend class EventListenerPhysicsContact;
};

/*
 * @brief postsolve value generated when onContactPostSolve called.
 */
class PhysicsContactPostSolve
{
	public :

		/** get elasticity between two bodies*/
		KDfloat											getElasticity ( KDvoid ) const;
		/** get friction between two bodies*/
		KDfloat											getFriction ( KDvoid ) const;
		/** get surface velocity between two bodies*/
		Point											getSurfaceVelocity ( KDvoid ) const;
    
	private :

		 PhysicsContactPostSolve ( KDvoid* pContactInfo );
		~PhysicsContactPostSolve ( KDvoid );
    
	private :

		KDvoid*											m_pContactInfo;
    
		friend class EventListenerPhysicsContact;
};

/* contact listener. it will recive all the contact callbacks. */
class EventListenerPhysicsContact : public EventListenerCustom
{
	public :

		/** create the listener */
		static EventListenerPhysicsContact*				create ( KDvoid );
		virtual KDbool									checkAvailable ( KDvoid ) override;
		virtual EventListenerPhysicsContact*			clone ( KDvoid ) override;
    
	protected :

		/**
		 *	it will be call when two body have contact.
		 *	if return false, it will not invoke callbacks
		 */
		virtual KDbool									hitTest ( PhysicsShape* pShapeA, PhysicsShape* pShapeB );
    
	public :

		/**
		 *	@brief it will called at two shapes start to contact, and only call it once.
		 */
		std::function<KDbool( EventCustom* pEvent, const PhysicsContact& tContact)>		onContactBegin;
		
		/**
		 *	@brief Two shapes are touching during this step. Return false from the callback to make world ignore the collision this step or true to process it normally. Additionally, you may override collision values, elasticity, or surface velocity values.
		 */
		std::function<KDbool(EventCustom* pEvent, const PhysicsContact& tContact, const PhysicsContactPreSolve& tSolve)>	onContactPreSolve;
		
		/**
		 *	@brief Two shapes are touching and their collision response has been processed. You can retrieve the collision impulse or kinetic energy at this time if you want to use it to calculate sound volumes or damage amounts. See cpArbiter for more info
		 */
		std::function<KDvoid(EventCustom* pEvent, const PhysicsContact& tContact, const PhysicsContactPostSolve& tSolve)>	onContactPostSolve;
		
		/**
		 *	@brief it will called at two shapes separated, and only call it once.
		 *	onContactBegin and onContactSeperate will called in pairs.
		 */
		std::function<KDvoid(EventCustom* pEvent, const PhysicsContact& tContact)>		onContactSeperate;
    
	protected :

		KDbool											init ( KDvoid );
		KDvoid											onEvent ( EventCustom* pEvent );
    
	protected :

				 EventListenerPhysicsContact ( KDvoid );
		virtual ~EventListenerPhysicsContact ( KDvoid );
    
		friend class PhysicsWorld;
};

/** this event listener only be called when bodyA and bodyB have contacts */
class EventListenerPhysicsContactWithBodies : public EventListenerPhysicsContact
{
	public :

		static EventListenerPhysicsContactWithBodies*	create ( PhysicsBody* pBodyA, PhysicsBody* pBodyB );
    
		virtual KDbool									hitTest ( PhysicsShape* pShapeA, PhysicsShape* pShapeB ) override;
		virtual EventListenerPhysicsContactWithBodies*	clone ( KDvoid ) override;
    
	protected :

		PhysicsBody*									m_pA;
		PhysicsBody*									m_pB;
    
	protected :

				 EventListenerPhysicsContactWithBodies ( KDvoid );
		virtual ~EventListenerPhysicsContactWithBodies ( KDvoid );
};

/** this event listener only be called when shapeA and shapeB have contacts */
class EventListenerPhysicsContactWithShapes : public EventListenerPhysicsContact
{
	public :

		static EventListenerPhysicsContactWithShapes*	create ( PhysicsShape* pShapeA, PhysicsShape* pShapeB );
    
		virtual KDbool									hitTest ( PhysicsShape* pShapeA, PhysicsShape* pShapeB ) override;
		virtual EventListenerPhysicsContactWithShapes*	clone ( KDvoid ) override;
    
	protected :

		PhysicsShape*									m_pA;
		PhysicsShape*									m_pB;
    
	protected :
				 EventListenerPhysicsContactWithShapes ( KDvoid );
		virtual ~EventListenerPhysicsContactWithShapes ( KDvoid );
};

/** this event listener only be called when shapeA or shapeB is in the group your specified */
class EventListenerPhysicsContactWithGroup : public EventListenerPhysicsContact
{
	public :

		static EventListenerPhysicsContactWithGroup*	create ( KDint nGroup );
    
		virtual KDbool									hitTest ( PhysicsShape* pShapeA, PhysicsShape* pShapeB ) override;
		virtual EventListenerPhysicsContactWithGroup*	clone ( KDvoid ) override;
    
	protected :

		KDint											m_nGroup;
    
	protected :
				 EventListenerPhysicsContactWithGroup ( KDvoid );
		virtual ~EventListenerPhysicsContactWithGroup ( KDvoid );
};

NS_CC_END

#endif	// CC_USE_PHYSICS
#endif	// __CCPhysicsContact_h__
