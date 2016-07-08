/* --------------------------------------------------------------------------
 *
 *      File            CCSpaceManager.h
 *      Description     Manage the space for the application
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

#ifndef __CCSpaceManager_h__
#define __CCSpaceManager_h__

// 0x00 HI ME LO
// 00   00 01 03
#define SPACE_MANAGER_VERSION	0x00000103

#include "ccExtras/cpCCNode.h"
#include "ccExtras/cpCCSprite.h"
#include "ccExtras/cpShapeNode.h"
#include "ccExtras/cpConstraintNode.h"

NS_CC_BEGIN

// A more definitive sounding define
#define STATIC_MASS	  KD_INFINITY

typedef KDvoid ( *smgrEachFunc )    ( KDvoid* obj, KDvoid* data );
typedef KDvoid ( *smgrIterateFunc ) ( cpSpace* space, smgrEachFunc func );

/** Collision Moments */
typedef enum 
{ 
	COLLISION_BEGIN		= 1	, 
	COLLISION_PRESOLVE		, 
	COLLISION_POSTSOLVE		, 
	COLLISION_SEPARATE
} CollisionMoment;

typedef KDbool ( CCObject::*SEL_SpaceHandler ) ( CollisionMoment, cpArbiter*, cpSpace* );

#define space_selector(_SELECTOR) (SEL_SpaceHandler)(&_SELECTOR)

/**
 *	Delegate for handling constraints that will be free'd by
 *	the method: removeAndFreeConstraintsOnBody 
 */
class cpConstraintCleanupDelegate : public CCObject
{
	public :

		virtual  KDvoid		aboutToFreeConstraint ( cpConstraint* pConstraint );
};

/** 
 *	Delegate for handling I/O
 *
 *	These functions are events that get fired when saving/loading
 *	a space, they enable you to use a specific id system (if you choose)
 *	as well as record any UI specific data at time of writing; the reading
 *	events will let you hook up your UI when an object has just been read,
 *	but not added to the space yet.
 */

class SpaceManagerSerializeDelegate
{
	public :

		virtual KDint		makeShapeId			   ( cpShape*      pShape      );
		virtual KDint		makeBodyId			   ( cpBody*       pBody       );
		virtual KDint		makeConstraintId	   ( cpConstraint* pConstraint );

		virtual KDbool		aboutToWriteShape	   ( cpShape*	   pShape	  , KDint nID );
		virtual KDbool		aboutToWriteBody	   ( cpBody*	   pBody	  , KDint nID );
		virtual KDbool		aboutToWriteConstraint ( cpConstraint* pConstraint, KDint nID );

		virtual KDbool		aboutToReadShape	   ( cpShape*	   pShape	  , KDint nID );
		virtual KDbool		aboutToReadBody		   ( cpBody*	   pBody	  , KDint nID );
		virtual KDbool		aboutToReadConstraint  ( cpConstraint* pConstraint, KDint nID );
};

/** The CCSpaceManager */
class CCSpaceManager : public CCObject
{
	public :

				 CCSpaceManager ( KDvoid );
		virtual ~CCSpaceManager ( KDvoid );

		/** Default creation method */
		static CCSpaceManager*		create ( KDvoid );

		/** 
		 *	Creation method
		 *	@param nSize  The average size of shapes in space
		 *	@param nCount The expected number of shapes in a space (larger is better)
		 */
		static CCSpaceManager*		create ( KDint nSize, KDint nCount );

		/* Creation method that takes a precreated space */
		static CCSpaceManager*		create ( cpSpace* pSpace );

	public :

		virtual KDbool		init ( KDvoid );

		/** 
		 *	Initialization method
		 *	@param nSize  The average size of shapes in space
		 *	@param nCount The expected number of shapes in a space (larger is better)
		 */
		virtual KDbool		initWithSize ( KDint nSize, KDint nCount );

		/* Initialization method that takes a precreated space */
		virtual KDbool		initWithSpace ( cpSpace* pSpace );

		/** Schedule a timed loop (against step:) using dt */
		KDvoid				start ( KDfloat fDelta = 0 );

		/** Stop the timed loop */
		KDvoid				stop ( KDvoid );

		/** load a cpSerializer file from a file (path), delegate can be nil */
		//KDbool  loadSpaceFromPath ( const KDchar* szFile, SpaceManagerSerializeDelegate* pDelegate );

		/** save a cpSerializer file to a resource file (path), delegate can be nil */
		//KDbool  saveSpaceToPath ( const KDchar* szFile, SpaceManagerSerializeDelegate* pDelegate );

		KDvoid				addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, CCSize tSize, cpVect tInset, cpFloat fRadius );

		KDvoid				addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, CCRect tRect, cpVect tInset, cpFloat fRadius );

		/** Convenience method for adding a containment rect around the view */
		KDvoid				addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, cpVect tInset );
		KDvoid				addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, cpVect tInset, cpFloat fRadius );

		/** Manually advance time within the space */
		KDvoid				step ( cpFloat fDelta );

		/** add a circle shape */
		cpShape*			addCircleAt ( cpVect tPos, cpFloat fMass, cpFloat fRadius );

		/** add a circle shape to existing body */
		cpShape*			addCircleToBody ( cpBody* pBody, cpFloat fRadius );

		/** add a circle shape to existing body, with offset */
		cpShape*			addCircleToBody ( cpBody* pBody, cpFloat fRadius, cpVect tOffset );

		/** add a rectangle shape */
		cpShape*			addRectAt ( cpVect tPos, cpFloat fMass, cpFloat fWidth, cpFloat fHeight, cpFloat fRotation );

		/** add a rectangle shape to existing body */
		cpShape*			addRectToBody ( cpBody* pBody, cpFloat fWidth, cpFloat fHeight, cpFloat fRotation );

		/** add a rectangle shape to existing body, with offset */
		cpShape*			addRectToBody ( cpBody* pBody, cpFloat fWidth, cpFloat fHeight, cpFloat fRotation, cpVect tOffset );

		/** add a polygon shape, convenience method */
		cpShape*			addPolyAt ( cpVect tPos, cpFloat fMass, cpFloat fRotation, KDint nNumPoints, cpVect tPoint, ... );

		/** add a polygon shape */
		cpShape*			addPolyAt ( cpVect tPos, cpFloat fMass, cpFloat fRotation, CCArray* pPoints );

		/** add a polygon shape */
		cpShape*			addPolyAt ( cpVect tPos, cpFloat fMass, cpFloat fRotation, KDint nNumPoints, cpVect* pPoints );

		/** add a polygon shape to existing body */
		cpShape*			addPolyToBody ( cpBody* pBody, cpFloat fRotation, CCArray* pPoints );

		/** add a polygon shape to existing body, with offset */
		cpShape*			addPolyToBody ( cpBody* pBody, cpFloat fRotation, CCArray* pPoints, cpVect tOffset );

		/** Alternative, add a polygon shape to existing body */
		cpShape*			addPolyToBody ( cpBody* pBody, cpFloat fRotation, KDint nNumPoints, cpVect* pPoints );

		/** Alternative, add a polygon shape to existing body, with offset */
		cpShape*			addPolyToBody ( cpBody* pBody, cpFloat fRotation, KDint nNumPoints, cpVect* pPoints, cpVect tOffset );

		/* add a segment shape using world coordinates */
		cpShape*			addSegmentAtWorldAnchor ( cpVect tFromPos, cpVect tToPos, cpFloat fMass, cpFloat fRadius );

		/* add a segment shape using local coordinates */
		cpShape*			addSegmentAt ( cpVect tPos, cpVect tFromPos, cpVect tToPos, cpFloat fMass, cpFloat fRadius );

		/* add a segment shape using local coordinates */
		cpShape*			addSegmentToBody ( cpBody* pBody, cpVect tFromPos, cpVect tToPos, cpFloat fRadius );

		/** Retrieve the first shape found at this position matching layers and group */
		cpShape*			getShapeAt ( cpVect tPos, cpLayers uLayers, cpGroup uGroup );

		/** Retrieve the first shape found at this position */
		cpShape*			getShapeAt ( cpVect tPos );

		/** Use if you need to call getShapes before you've actually started simulating */
		KDvoid				rehashActiveShapes ( KDvoid );

		/** Use if you move static shapes during simulation */
		KDvoid				rehashStaticShapes ( KDvoid );

		/** Use to only rehash one shape */
		KDvoid				rehashShape ( cpShape* pShape );

		/** Given a point, return an array of NSValues with a pointer to a cpShape */
		CCArray*			getShapesAt ( cpVect tPos, cpLayers uLayers, cpGroup uGroup );
		/** @see getShapesAt:layers:group: */
		CCArray*			getShapesAt ( cpVect tPos );

		/** Given a point and a radius, return an array of NSValues with a pointer to a cpShape */
		CCArray*			getShapesAt ( cpVect tPos, KDfloat fRadius, cpLayers uLayers, cpGroup uGroup );
		/** @see getShapesAt:radius:layers:group: */
		CCArray*			getShapesAt ( cpVect tPos, KDfloat fRadius );

		/** Get shapes that are using this body */
		CCArray*			getShapesOnBody( cpBody* pBody );

		/** Return first shape hit by the given raycast */
		cpShape*  			getShapeFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup );
		/** see getShapeFromRayCastSegment:end:layers:group: */
		cpShape*  			getShapeFromRayCastSegment ( cpVect tStart, cpVect tEnd );

		/** Return the info on the first shape hit by the given raycast */
		cpSegmentQueryInfo  getInfoFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup );
		/** see getInfoFromRayCastSegment:end:layers:group: */
		cpSegmentQueryInfo  getInfoFromRayCastSegment ( cpVect tStart, cpVect tEnd );

		/** Return an array of NSValues with a pointer to a cpShape that intersects the raycast */
		CCArray*  			getShapesFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup );
		/** see getShapesFromRayCastSegment:end:layers:group: */
		CCArray*  			getShapesFromRayCastSegment ( cpVect tStart, cpVect tEnd );

		/** Return an array of NSValues with a pointer to a cpSegmentQueryInfo, this array will clean up the infos when released */
		CCArray*  			getInfosFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup );
		/** see getInfosFromRayCastSegment:end:layers:group: */
		CCArray*  			getInfosFromRayCastSegment ( cpVect tStart, cpVect tEnd );

		/** Explosion, applied linear force to objects within radius */
		KDvoid  			applyLinearExplosionAt ( cpVect tAt, cpFloat fRadius, cpFloat fMaxForce );
		/** Explosion, applied linear force to objects within radius given a group and layer(s) */
		KDvoid  			applyLinearExplosionAt ( cpVect tAt, cpFloat fRadius, cpFloat fMaxForce, cpLayers uLayers, cpGroup uGroup );

		/** Queries the space as to whether these two shapes are in persistent contact */
		KDbool  			isPersistentContactOnShape ( cpShape* pShape, cpShape* pShape2 );

		/** Queries the space as to whether this shape has a persistent contact, returns the first arbiter info */
		cpArbiter*  		persistentContactInfoOnShape ( cpShape* pShape );

		/** Queries the space as to whether this shape has a persistent contact, returns the all arbiters  */
		CCArray* 			persistentContactInfosOnShape ( cpShape* pShape );

		/** 
		 *	Queries the space as to whether this shape has ANY persistent contact, It will return
		 *	the first shape it finds or NULL if nothing is found
		 */
		cpShape*  			persistentContactOnShape ( cpShape* pShape );

		/** Will return an array of NSValues that point to the cpConstraints */
		CCArray*  			getConstraints ( KDvoid );

		/** Will return an array of NSValues that point to the cpConstraints on given body */
		CCArray*  			getConstraintsOnBody( cpBody* pBody );

		/** Use when removing & freeing a shape AND it's body if the body is not used by other shapes */
		KDvoid  			removeAndFreeShape ( cpShape* pShape );

		/** Use when removing a shape, will pass ownership to caller */
		cpShape*  			removeShape ( cpShape* pShape );

		/** Manually add a shape to the space */
		KDvoid  			addShape ( cpShape* pShape );

		/** Use when removing & freeing a body */
		KDvoid  			removeAndFreeBody ( cpBody* pBody );

		/** Use when removing a body, will pass ownership to caller */
		KDvoid  			removeBody ( cpBody* pBody );

		/** Manually add a body to the space */
		KDvoid  			addBody ( cpBody* pBody );

		/** Check is body belongs to more than one shape */
		KDbool  			isBodyShared ( cpBody* pBody );

		/** This will force a shape into a static shape */
		cpShape*  			morphShapeToStatic ( cpShape* pShape );

		/** This will force a shape active and give it the given mass */
		cpShape*  			morphShapeToActive ( cpShape* pShape, cpFloat fMass );

		/** This will force a shape to be kinematic (body will not simulate) */
		cpShape*  			morphShapeToKinematic ( cpShape* pShape );

		/** This will take a shape (any) and split it into the number of pieces you specify,
			@return An CCArray* of NSValues* with cpShape* as the value (the fragments) or nil if failed
		 */
		CCArray*  			fragmentShape ( cpShape* pShape, KDint nPieces, KDfloat fMass );

		/** This will take a rect and split it into the number of pieces (Rows x Cols) you specify,
		 @return An CCArray* of NSValues* with cpShape* as the value (the fragments) or nil if failed
		 */
		CCArray*  			fragmentRect ( cpPolyShape* pPoly, KDint nRows, KDint nCols, KDfloat fMass );

		/** This will take a circle and split it into the number of pieces you specify,
		 @return An CCArray* of NSValues* with cpShape* as the value (the fragments) or nil if failed
		 */
		CCArray*  			fragmentCircle ( cpCircleShape* pCircle, KDint nPieces, KDfloat fMass );

		/** This will take a segment and split it into the number of pieces you specify,
		 @return An CCArray* of NSValues* with cpShape* as the value (the fragments) or nil if failed
		 */
		CCArray*  			fragmentSegment ( cpSegmentShape* pSegment, KDint nPieces, KDfloat fMass );

		/** Combine two shapes together by combining their bodies into one */
		KDvoid  			combineShapes ( cpShape* pShapes, ... );

		/** Offset shape from body using (circle:center, segment:endpoints, poly:vertices) */
		KDvoid  			offsetShape ( cpShape* pShape, cpVect tOffset );

		/** Unique Collision: will ignore the effects a collsion between types */
		KDvoid  			ignoreCollisionBetweenType ( KDuint uType1, KDuint uType2 );

		/** Register a collision callback between types */
		KDvoid  			addCollisionCallbackBetweenType ( KDuint uType1, KDuint uType2, CCObject* pTarget, SEL_SpaceHandler pSelector );

		/** Register a collision callback between types for the given collision moments */
		KDvoid  			addCollisionCallbackBetweenType ( KDuint uType1, KDuint uType2, CCObject* pTarget, SEL_SpaceHandler pSelector, CollisionMoment eMoments, ... );

		/** Unregister a collision callback between types */
		KDvoid  			removeCollisionCallbackBetweenType ( KDuint uType1, KDuint uType2 );

		/** Use when removing constraints, ownership is given to caller*/
		cpConstraint*		removeConstraint ( cpConstraint* pConstraint );

		/** This will remove and free the constraint */
		KDvoid  			removeAndFreeConstraint ( cpConstraint* pConstraint );

		/** This will calculate all constraints on a body and remove & free them */
		KDvoid  			removeAndFreeConstraintsOnBody ( cpBody* pBody );

		/** Add a spring to two bodies at the body anchor points */
		cpConstraint*		addSpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2, cpFloat fRest, cpFloat fStiff, cpFloat fDamp );
		/** Add a spring to two bodies at the body anchor points */
		cpConstraint*		addSpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRest, cpFloat fStiff, cpFloat fDamp );
		/** Add a spring to two bodies at the body anchor points */
		cpConstraint*		addSpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fStiff );

		/** Add a groove (aka sliding pin) between two bodies */
		cpConstraint*		addGrooveToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tGroove1, cpVect tGroove2, cpVect tAnchor2 );
		/** Add a groove (aka sliding pin) between two bodies */
		cpConstraint*		addGrooveToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fLength, KDbool bHoriz, cpVect tAnchor2 );
		/** Add a groove (aka sliding pin) between two bodies */
		cpConstraint*		addGrooveToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fLength, KDbool bHoriz );

		/** Add a sliding joint between two bodies */
		cpConstraint*		addSlideToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2, cpFloat fMin, cpFloat fMax );
		/** Add a sliding joint between two bodies */
		cpConstraint*		addSlideToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fMin, cpFloat fMax );

		/** Create a pin (rod) between two bodies */
		cpConstraint*		addPinToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2 );
		/** Create a pin (rod) between two bodies */
		cpConstraint*		addPinToBody ( cpBody* pToBody, cpBody* pFromBody );

		/** Add a shared point between two bodies that they may rotate around */
		cpConstraint*		addPivotToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2 );
		/** Add a shared point between two bodies that they may rotate around */
		cpConstraint*		addPivotToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr );
		/** Add a shared point between two bodies that they may rotate around */
		cpConstraint*		addPivotToBody ( cpBody* pToBody, cpBody* pFromBody );

		/** Add a motor that applys torque to a specified body(s) */
		cpConstraint*		addMotorToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRate );
		/** Add a motor that applys torque to a specified body(s) */
		cpConstraint*		addMotorToBody ( cpBody* toBody, cpFloat fRate );

		/** Add gears between two bodies */
		cpConstraint*		addGearToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fPhase, cpFloat fRatio );
		/** Add gears between two bodies */
		cpConstraint*		addGearToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRatio );

		/** This does not work yet */
		cpConstraint*		addBreakableToConstraint ( cpConstraint* pBreakConstraint, cpFloat fMax );

		/** Specify a min and a max a body can rotate relative to another body */
		cpConstraint*		addRotaryLimitToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fMin, cpFloat fMax );
		/** Specify a min and a max a body can rotate */
		cpConstraint*		addRotaryLimitToBody ( cpBody* toBody, cpFloat fMin, cpFloat fMax );

		/** Add a ratchet between two bodies */
		cpConstraint*		addRatchetToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fPhase, cpFloat fRatchet );
		/** Add a ratchet to a body */
		cpConstraint*		addRatchetToBody ( cpBody* pToBody, cpFloat fPhase, cpFloat fRatchet );

		/** Add a rotary spring between two bodies */
		cpConstraint*		addRotarySpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRestAngle, cpFloat fStiff, cpFloat fDamp );
		/** Add a rotary spring to a body */
		cpConstraint*		addRotarySpringToBody ( cpBody* toBody, cpFloat fRestAngle, cpFloat fStiff, cpFloat fDamp );

		/** Add a pulley between two bodies and another body which is the actual pulley */
		cpConstraint*		addPulleyToBody ( cpBody* pToBody, cpBody* pFromBody, cpBody* pPulleyBody,
											  cpVect tAnchor1, cpVect tAnchor2, cpVect tAnchor3a, cpVect tanchor3b, cpFloat fRatio );

		/** Add a pulley between two bodies */
		cpConstraint*		addPulleyToBody ( cpBody* pToBody, cpBody* pFromBody,
											  cpVect tAnchor1, cpVect tAnchor2, cpVect tAnchor3a, cpVect tanchor3b, cpFloat fRatio );
	public :

		/** The actual chipmunk space */
		CC_SYNTHESIZE_READONLY ( cpSpace*, m_pSpace, Space );

		/** The segment shapes that form the bounds of the window*/
		CC_SYNTHESIZE ( cpShape*, m_pTopWall   , TopWall    );
		CC_SYNTHESIZE ( cpShape*, m_pBottomWall, BottomWall );
		CC_SYNTHESIZE ( cpShape*, m_pRightWall , RightWall  );
		CC_SYNTHESIZE ( cpShape*, m_pLeftWall  , LeftWall   );
		 
		/** Number of steps (across dt) perform on each step call */
		CC_SYNTHESIZE ( KDint, m_nSteps, Steps );

		/** The dt value that was used in step last */
		CC_SYNTHESIZE_READONLY ( cpFloat, m_fLastDelta, LastDelta );

		/** The gravity of the space */
		CC_PROPERTY ( cpVect, m_tGravity, Gravity );

		/** The damping of the space (viscousity in "air") */
		CC_PROPERTY ( cpFloat, m_fDamping, Damping );

		/** If this is set to YES/TRUE then step will call rehashStatic before stepping */
		CC_SYNTHESIZE ( KDbool, m_bRehashStaticEveryStep, RehashStaticEveryStep );

		/** Set the iterateFunc; the function that will use "eachFunc" to sync chipmunk values */
		CC_SYNTHESIZE ( smgrIterateFunc, m_pIterateFunc, IterateFunc );

		/** Set the iterateFunc; the default will update cocosnodes for pos and rotation */
		CC_SYNTHESIZE ( smgrEachFunc, m_pEachFunc, EachFunc );

		/** A staticBody for any particular reusable purpose */
		CC_PROPERTY_READONLY ( cpBody*, m_pStaticBody, StaticBody );

		/** Whether or not this space is internally locked */
		virtual KDbool		isSpaceLocked ( KDvoid );

		/** If this is set to anything other than zero, the step routine will use its value as the dt (constant) */
		CC_SYNTHESIZE ( cpFloat, m_fConstantDelta, ConstantDelta );

		/** Setting this to YES/TRUE will also free contraints connected to a free'd shape */
		CC_SYNTHESIZE ( KDbool, m_bCleanupBodyDependencies, CleanupBodyDependencies );

		/** This will be called from all methods that auto-free constraints dependent on bodies being freed */
		CC_SYNTHESIZE_RETAIN ( cpConstraintCleanupDelegate*, m_pConstraintCleanupDelegate, ConstraintCleanupDelegate );

	public :

		KDvoid				setupDefaultShape ( cpShape* pShape );

		KDvoid				removeAndMaybeFreeShape ( cpShape* pShape, KDbool bFreeShape );
		KDvoid				removeAndMaybeFreeBody  ( cpBody*  pBody , KDbool bFreeBody  );

	protected :
		
		/* Internal devices (dev: Consider revamping) */
		CCArray*			m_pInvocations;
		
		/* If constant dt used, then this accumulates left over */
		KDfloat				m_fTimeAccumulator;		
};

/** iterateFunc for only active shapes */
KDvoid  smgrBasicIterateActiveShapesOnlyFunc ( cpSpace* pSpace, smgrEachFunc pFunc );

/** iterateFunc for both static and active */
KDvoid  smgrBasicIterateShapesFunc ( cpSpace* pSpace, smgrEachFunc pFunc );
KDvoid  smgrBasicEachShape         ( KDvoid* pShapePtr, KDvoid* pData );		/**< sync's CCNode to pos/rot of shape->body */
KDvoid  smgrBasicEachShapeOrBody   ( KDvoid* pShapePtr, KDvoid* pData );		/** sync CCNode to pos/rot of body */

/** experimental! do not use */
KDvoid  smgrEachShapeAsChildren ( KDvoid* pShapePtr, KDvoid* pData );

/** 
 *	iteratFunc for those who prefer body->data = CCNode 
 *
 *	TODO: figure out simplistic way to specify either method
 */
KDvoid		smgrBasicIterateBodiesFunc ( cpSpace* pSpace, smgrEachFunc pFunc );
KDvoid		smgrBasicEachBody ( KDvoid* pBodyPtr, KDvoid* pData );		/**< sync's CCNode to pos/rot of body */

NS_CC_END

#endif	// __CCSpaceManager_h__