/* --------------------------------------------------------------------------
 *
 *      File            CCCCSpaceManager.cpp
 *      Description     Manage the space for the application
 *      Author          Y.H Mun
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
 *      License aKDint with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCSpaceManager/CCSpaceManager.h"
#include "extensions/CCSpaceManager/cpSpaceSerializer/cpSpaceSerializer.h"
#include "cocoa/CCPointer.h"
#include "support/CCDirector.h"
#include "support/CCScheduler.h"
#include "XMChipmunk/chipmunk_unsafe.h"

NS_CC_BEGIN

//
// Private Method Declarations 
//
class RayCastInfoValue : public CCPointer
{
	public :

		RayCastInfoValue ( KDvoid* pPtr ) : CCPointer ( pPtr )
		{

		}

		virtual ~RayCastInfoValue ( KDvoid )
		{
			cpSegmentQueryInfo*  pInfo = (cpSegmentQueryInfo*) m_pPtr;
			kdFree ( pInfo );
		}

		static  RayCastInfoValue*  create ( KDvoid* pPtr )
		{
			RayCastInfoValue*  pRet = new RayCastInfoValue ( pPtr );

			if ( pRet )
			{
				pRet->autorelease ( );
			}

			return pRet;
		}
};

class SmgrInvocation : public CCObject
{
	public :

		static SmgrInvocation*  create ( cpCollisionType uType1, cpCollisionType uType2, CCObject* pTarget, SEL_SpaceHandler pSelector  )
		{
			SmgrInvocation*  pRet = new SmgrInvocation ( );
			if ( pRet && pRet->initWithTarget ( uType1, uType2, pTarget, pSelector ) )
			{
				pRet->autorelease ( );
			}
			else
			{
				CC_SAFE_DELETE ( pRet );
			}
			return pRet;
		}

		KDbool  initWithTarget ( cpCollisionType uType1, cpCollisionType uType2, CCObject* pTarget, SEL_SpaceHandler pSelector )
		{
			m_uType1	= uType1;
			m_uType2	= uType2;
			m_pTarget	= pTarget;
			m_pSelector = pSelector;

			return KD_TRUE;
		}

		KDbool  invoke ( CollisionMoment eMoment, cpArbiter* pArb, struct cpSpace* pSpace )
		{
			if ( m_pTarget && m_pSelector )
			{
				return ( m_pTarget->*m_pSelector ) ( eMoment, pArb, pSpace );
			}

			return KD_FALSE;
		}

		cpCollisionType		m_uType1;
		cpCollisionType		m_uType2;

		CCObject*			m_pTarget;
		SEL_SpaceHandler	m_pSelector;
};

typedef struct  
{
	cpLayers	layers;
	cpGroup		group;
	cpVect		at;
	KDfloat		radius;
	KDfloat		maxForce;
} ExplosionQueryContext;

/*
class cpSSDelegate : public cpSpaceSerializerDelegate 
{
	public :
		
		cpSSDelegate ( CCSpaceManagerSerializeDelegate* pDelegate ) 
		{
			m_pDelegate = pDelegate;
		}
		
		KDint  makeId ( cpShape* pShape ) 
		{

				return m_pDelegate->makeShapeId:pShape;		
		}
		
		KDint makeId(cpBody* pBody) 
		{
			if ([m_pDelegate respondsToSelector:@selector(makeBodyId:)])
				return [m_pDelegate makeBodyId:pBody];		
			else
				return CPSS_DEFAULT_MAKE_ID(pBody);
		}
		
		KDint makeId(cpConstraint* constraint) 
		{
			if ([m_pDelegate respondsToSelector:@selector(makeConstraintId:)])
				return [m_pDelegate makeConstraintId:constraint];		
			else
				return CPSS_DEFAULT_MAKE_ID(constraint);
		}
		
		KDbool writing(cpShape* pShape, KDint pShapeId) 
		{
			if ([m_pDelegate respondsToSelector:@selector(aboutToWriteShape:pShapeId:)])
				return [m_pDelegate aboutToWriteShape:pShape pShapeId:pShapeId];		
			else
				return KD_TRUE;
		}
		
		KDbool writing(cpBody* pBody, KDint pBodyId) 
		{
			if ([m_pDelegate respondsToSelector:@selector(aboutToWriteBody:pBodyId:)])
				return [m_pDelegate aboutToWriteBody:pBody pBodyId:pBodyId];		
			else
				return KD_TRUE;
		}
		
		KDbool writing(cpConstraint *constraint, KDint constraintId) 
		{
			if ([m_pDelegate respondsToSelector:@selector(aboutToWriteConstraint:constraintId:)])
				return [m_pDelegate aboutToWriteConstraint:constraint constraintId:constraintId];		
			else
				return KD_TRUE;
		}
		
		KDbool reading(cpShape* pShape, KDint pShapeId) 
		{
			if ([m_pDelegate respondsToSelector:@selector(aboutToReadShape:pShapeId:)])
				return [m_pDelegate aboutToReadShape:pShape pShapeId:pShapeId];		
			else
				return KD_TRUE;	
		}

		KDbool reading(cpBody* pBody, KDint pBodyId) 
		{
			if ([m_pDelegate respondsToSelector:@selector(aboutToReadBody:pBodyId:)])
				return [m_pDelegate aboutToReadBody:pBody pBodyId:pBodyId];		
			else
				return KD_TRUE;
		}

		KDbool reading(cpConstraint *constraint, KDint constraintId) 
		{
			if ([m_pDelegate respondsToSelector:@selector(aboutToReadConstraint:constraintId:)])
				return [m_pDelegate aboutToReadConstraint:constraint constraintId:constraintId];		
			else
				return KD_TRUE;
		}
		
	private :

		CCSpaceManagerSerializeDelegate*		m_pDelegate;
};
*/

typedef struct 
{
	cpCollisionType  a;
	cpCollisionType  b;
} cpCollisionTypePair;

static inline cpBool bbIntersects ( const cpBB a, const cpBB b )
{
	return ( a.l <= b.r && b.l <= a.r && a.b <= b.t && b.b <= a.t );
}

static KDvoid ExplosionQueryHelper ( cpBB* pBB, cpShape* pShape, ExplosionQueryContext* pContext )
{
	if ( !( pShape->group && pContext->group == pShape->group ) && ( pContext->layers & pShape->layers ) && bbIntersects ( *pBB, pShape->bb ) )
	{
		//incredibly cheesy explosion effect (works decent with small objects)
		cpVect  dxdy = cpvsub ( pShape->body->p, pContext->at );
		KDfloat distsq = cpvlengthsq ( dxdy );
		
		// [Factor] = [Distance]/[Explosion Radius] 
		// [Force] = (1.0 - [Factor]) * [Total Force]
		// Apply -> [Direction] * [Force]
		if ( distsq <= pContext->radius * pContext->radius )
		{
			//Distance
			KDfloat  dist = cpfsqrt ( distsq );
			
			//normalize for direction
			dxdy = cpvmult ( dxdy, 1.0f / dist );
			cpBodyApplyImpulse ( pShape->body, cpvmult ( dxdy, pContext->maxForce * ( 1.0f - ( dist / pContext->radius ) ) ), cpvzero );
		}
	}
}

static KDint handleInvocations ( CollisionMoment eMoment, cpArbiter* pArb, struct cpSpace* pSpace, KDvoid* pData )
{
	SmgrInvocation*  pInvocation = (SmgrInvocation*) pData;

	return pInvocation->invoke ( eMoment, pArb, pSpace );
}

static cpBool collBegin ( cpArbiter* pArb, struct cpSpace* pSpace, KDvoid* pData )
{
	return handleInvocations ( COLLISION_BEGIN, pArb, pSpace, pData );
}

static cpBool collPreSolve ( cpArbiter* pArb, struct cpSpace* pSpace, KDvoid* pData )
{
	return handleInvocations ( COLLISION_PRESOLVE, pArb, pSpace, pData );
}

static KDvoid collPostSolve ( cpArbiter* pArb, struct cpSpace* pSpace, KDvoid* pData )
{
	handleInvocations ( COLLISION_POSTSOLVE, pArb, pSpace, pData );
}

static KDvoid collSeparate ( cpArbiter* pArb, struct cpSpace* pSpace, KDvoid* pData )
{
	handleInvocations ( COLLISION_SEPARATE, pArb, pSpace, pData );
}

static cpBool collIgnore ( cpArbiter* pArb, struct cpSpace* pSpace, KDvoid* pData )
{
	return 0;
}

static KDvoid collectAllArbiters ( cpBody* pBody, cpArbiter* pArbiter, KDvoid* pOutArbiters )
{
	( (CCArray*) pOutArbiters )->addObject ( CCPointer::create ( pArbiter ) ); 
}

static KDvoid collectAllConstraints ( cpConstraint* pConstraint, KDvoid* pOutConstraints )
{
	( (CCArray*) pOutConstraints )->addObject ( CCPointer::create ( pConstraint ) );    
}

static KDvoid collectAllShapes ( cpShape* pShape, CCArray* pOutShapes )
{
	pOutShapes->addObject ( CCPointer::create ( pShape ) );  
}

static KDvoid collectAllCollidingShapes ( cpShape* pShape, cpContactPointSet* pPoints, CCArray* pOutShapes )
{
	pOutShapes->addObject ( CCPointer::create ( pShape ) );
}

static KDvoid collectAllShapesOnBody ( cpBody* pBody, cpShape* pShape, KDvoid* pOutShapes )
{
	( (CCArray*) pOutShapes )->addObject ( CCPointer::create ( pShape ) );  
}

static KDvoid collectAllBodyConstraints ( cpBody* pBody, cpConstraint* pConstraint, KDvoid* pOutConstraints )
{
	( (CCArray*) pOutConstraints )->addObject ( CCPointer::create ( pConstraint ) );  
}

static KDvoid collectAllSegmentQueryInfos ( cpShape* pShape, cpFloat t, cpVect n, CCArray* pOutInfos )
{
	cpSegmentQueryInfo*  pInfo = (cpSegmentQueryInfo*) kdMalloc ( sizeof ( cpSegmentQueryInfo ) );

	pInfo->shape = pShape;
	pInfo->t = t;
	pInfo->n = n;

	pOutInfos->addObject ( RayCastInfoValue::create ( pInfo ) ); 
}

static KDvoid collectAllSegmentQueryShapes ( cpShape* pShape, cpFloat t, cpVect n, CCArray* pOutShapes )
{
	pOutShapes->addObject ( CCPointer::create ( pShape ) );
}

static KDvoid updateBBCache ( cpShape* pShape, KDvoid* pUnused )
{
	cpShapeCacheBB ( pShape );
}

static KDvoid _removeShape ( cpSpace* pSpace, KDvoid* pObj, KDvoid* pData )
{
	( (CCSpaceManager*) pData )->removeAndMaybeFreeShape ( (cpShape*) pObj, KD_FALSE );
}

static KDvoid _removeAndFreeShape ( cpSpace* pSpace, KDvoid* pShape, KDvoid* pData )
{
	( (CCSpaceManager*) pData )->removeAndMaybeFreeShape ( (cpShape*) pShape, KD_TRUE );
}

static KDvoid _removeBody ( cpSpace* pSpace, KDvoid* pObj, KDvoid* pData )
{
	( (CCSpaceManager*) pData )->removeAndMaybeFreeBody ( (cpBody*) pObj, KD_FALSE );
}

static KDvoid _removeAndFreeBody(cpSpace* pSpace, KDvoid* pBody, KDvoid* pData )
{
	( (CCSpaceManager*) pData )->removeAndMaybeFreeBody ( (cpBody*) pBody, KD_TRUE );
}

static KDvoid countBodyReferences ( cpBody* pBody, cpShape* pShape, KDvoid* pData )
{
    KDint*  pCount = (KDint*) pData;
    (*pCount)++;
}

static KDvoid clearBodyReferenceFromShape ( cpBody* pBody, cpShape* pShape, KDvoid* pData )
{
    pShape->body = KD_NULL;
}

static KDvoid freeShapesHelper ( cpShape* pShape, KDvoid* pData )
{
	cpSpace*  pSpace = (cpSpace*) pData;
    
    // Do this for rogue bodies (Free them, clear any references to ensure this only happens once)
	if ( pSpace )
    {
        cpBody* pBody = pShape->body;

        if ( pBody 
             && pBody != pSpace->staticBody					// not the spaces own static pBody
             && !cpSpaceContainsBody ( pSpace, pBody ) )	// rogue pBody
        {
            // clear refs from all pShapes
            cpBodyEachShape ( pBody, clearBodyReferenceFromShape, KD_NULL );
            cpBodyFree ( pBody );
        }
        
        cpShapeFree ( pShape );
    }
}

static KDvoid freeBodiesHelper ( cpBody* pBody, KDvoid* pData )
{  
	if ( pBody )
	{
        cpBodyFree ( pBody );
	}
}

static KDvoid freeConstraintsHelper ( cpConstraint* pConstraint, KDvoid* pData )
{   
	if ( pConstraint )
	{
        cpConstraintFree ( pConstraint );
	}
}

static KDvoid _addBody ( cpSpace* pSpace, KDvoid* pObj, KDvoid* pData )
{
	cpBody*  pBody = (cpBody*) pObj;
	
	if ( pBody->m != STATIC_MASS && !cpSpaceContainsBody ( pSpace, pBody ) )
	{
		cpSpaceAddBody ( pSpace, pBody );
	}
}

static KDvoid _addShape ( cpSpace* pSpace, KDvoid* pObj, KDvoid* pData )
{
	cpShape*  pShape = (cpShape*) pObj;
	cpSpaceAddShape ( pSpace, pShape );
}

static KDvoid removeCollision ( cpSpace* pSpace, KDvoid* pCollisionPair, KDvoid* pInvList )
{
    cpCollisionTypePair*  pIds		   = (cpCollisionTypePair*) pCollisionPair;
    CCArray*			  pInvocations = (CCArray*) pInvList;
    SmgrInvocation*		  pInvocation  = KD_NULL; 
    
    // Find our invocation info so we can remove it 
	CCObject*  pObject = KD_NULL;
    CCARRAY_FOREACH ( pInvocations, pObject ) 
    {
		pInvocation = (SmgrInvocation*) pObject;

		if ( ( pInvocation->m_uType1 == pIds->a && pInvocation->m_uType2 == pIds->b ) ||
             ( pInvocation->m_uType2 == pIds->a && pInvocation->m_uType1 == pIds->b ) )
        {
            pInvocations->removeObject ( pInvocation );
            break;
        }
    }
    
    cpSpaceRemoveCollisionHandler ( pSpace, pIds->a, pIds->b );
    
    // This was allocated earlier, free it now 
    kdFree ( pIds );
}

CCSpaceManager* CCSpaceManager::create ( KDvoid )
{
	CCSpaceManager*  pRet = new CCSpaceManager ( ); 

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSpaceManager* CCSpaceManager::create ( KDint nSize, KDint nCount )
{
	CCSpaceManager*  pRet = new CCSpaceManager ( ); 

	if ( pRet && pRet->initWithSize ( nSize, nCount ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCSpaceManager* CCSpaceManager::create ( cpSpace* pSpace )
{
	CCSpaceManager*  pRet = new CCSpaceManager ( ); 

	if ( pRet && pRet->initWithSpace ( pSpace ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}


KDbool CCSpaceManager::init ( KDvoid )
{
	// totally arbitrary initialization values
	return initWithSize ( 20, 50 );
}

KDbool CCSpaceManager::initWithSize ( KDint nSize, KDint nCount )
{
	if ( initWithSpace ( cpSpaceNew ( ) ) )
	{
		cpSpaceUseSpatialHash ( m_pSpace, (cpFloat) nSize, nCount );

		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool CCSpaceManager::initWithSpace ( cpSpace* pSpace )
{	
	static KDbool  bInitialized = KD_FALSE;	
	
	if ( !bInitialized )
	{
		cpInitChipmunk ( );
		bInitialized = KD_TRUE;
	}
	
	m_pSpace = pSpace;
	
	// hmmm this gravity is silly.... sorry -rkb
	m_pSpace->gravity = cpv ( 0, -9.8 * 10 );
	m_pSpace->sleepTimeThreshold = 0.4f;	// this is actually a "large" value
	// m_pSpace->idleSpeedThreshold = 0;		// default is zero, chipmunk decides best speed
	
	m_nSteps = 2;
	m_bRehashStaticEveryStep   = KD_FALSE;
	m_bCleanupBodyDependencies = KD_TRUE;
	m_fConstantDelta   = 0.0f;
	m_fTimeAccumulator = 0.0f;
	
	m_pInvocations = new CCArray ( );
	
	m_pIterateFunc = &smgrBasicIterateShapesFunc;
    m_pEachFunc    = &smgrBasicEachShapeOrBody;

	return KD_TRUE;
}

CCSpaceManager::CCSpaceManager ( KDvoid )
{
	m_pSpace       = KD_NULL;
	m_pInvocations = KD_NULL;

	m_pTopWall    = KD_NULL;
	m_pBottomWall = KD_NULL;
	m_pRightWall  = KD_NULL;
	m_pLeftWall   = KD_NULL;

	m_pIterateFunc = KD_NULL;
}

CCSpaceManager::~CCSpaceManager ( KDvoid )
{		
    if ( m_pSpace != KD_NULL )
	{
		// Clear all "unowned" static bodies
//		cpSpaceEachShape      ( m_pSpace, freeShapesHelper	   , m_pSpace );
//		cpSpaceEachBody       ( m_pSpace, freeBodiesHelper	   , m_pSpace );
//		cpSpaceEachConstraint ( m_pSpace, freeConstraintsHelper, m_pSpace );
		cpSpaceFree           ( m_pSpace );
	}	
	
	CC_SAFE_RELEASE ( m_pInvocations );

	this->stop ( );
}

cpBody* CCSpaceManager::getStaticBody ( KDvoid )
{
	return m_pSpace ? m_pSpace->staticBody : KD_NULL;
}

KDvoid CCSpaceManager::start ( KDfloat fDelta )
{	
	CCDirector::sharedDirector ( )->getScheduler ( )->scheduleSelector ( schedule_selector ( CCSpaceManager::step ), this, fDelta, KD_FALSE );
}

KDvoid CCSpaceManager::stop ( KDvoid )
{
	CCDirector::sharedDirector ( )->getScheduler ( )->unscheduleSelector ( schedule_selector ( CCSpaceManager::step ), this );
}
/*
KDbool CCSpaceManager::loadSpaceFromPath ( const KDchar* szFile, CCSpaceManagerSerializeDelegate* pDelegate )
{
	
	const KDchar*  szPath = CCFileUtils::sharedFileUtils ( )->fullPathFromRelativePath ( szFile );

	cpSSDelegate  cpssdel ( delegate );	
	cpSpaceSerializer tReader ( &cpssdel );

	return tReader.load ( m_pSpace, szPath );
	

	return KD_FALSE;
}

KDbool CCSpaceManager::saveSpaceToPath ( const KDchar* szFile, CCSpaceManagerSerializeDelegate* pDelegate )
{
	const KDchar*  szPath = CCFileUtils::sharedFileUtils ( )->fullPathFromRelativePath ( szFile );

	cpSSDelegate  cpssdel ( pDelegate );
	cpSpaceSerializer  tWriter ( &cpssdel );	
	return writer.save ( m_pSpace, szPath );
	

	return KD_FALSE;
}
*/

KDvoid CCSpaceManager::setGravity ( cpVect tGravity )
{
	m_pSpace->gravity = tGravity;
}

cpVect CCSpaceManager::getGravity ( KDvoid )
{
	return m_pSpace->gravity;
}

KDvoid CCSpaceManager::setDamping ( cpFloat fDamping )
{
	m_pSpace->damping = fDamping;
}

cpFloat CCSpaceManager::getDamping ( KDvoid )
{
	return m_pSpace->damping;
}


KDvoid CCSpaceManager::addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, CCSize tSize, cpVect tInset, cpFloat fRadius )
{
	addWindowContainmentWithFriction ( fFriction, fElasticity, CCRect ( CCPointZero, tSize ), tInset, fRadius );
}

KDvoid CCSpaceManager::addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, CCRect tRect, cpVect tInset, cpFloat fRadius )
{
    cpVect  bl = cpv ( tRect.origin.x + tInset.x, tRect.origin.y + tInset.y );
    cpVect  br = cpv ( tRect.size.cx  - tInset.x, tRect.origin.y + tInset.y );
    cpVect  tr = cpv ( tRect.size.cx  - tInset.x, tRect.size.cy  - tInset.y );
    cpVect  tl = cpv ( tRect.origin.x + tInset.x, tRect.size.cy  - tInset.y );
    
	m_pBottomWall = addSegmentAtWorldAnchor ( bl, br, STATIC_MASS, fRadius );	
	m_pTopWall    = addSegmentAtWorldAnchor ( tl, tr, STATIC_MASS, fRadius );
	m_pLeftWall   = addSegmentAtWorldAnchor ( bl, tl, STATIC_MASS, fRadius );
	m_pRightWall  = addSegmentAtWorldAnchor ( br, tr, STATIC_MASS, fRadius ); 
	
	m_pBottomWall->e = m_pTopWall->e = m_pLeftWall->e = m_pRightWall->e = fElasticity;
	m_pBottomWall->u = m_pTopWall->u = m_pLeftWall->u = m_pRightWall->u = fFriction;
}

KDvoid CCSpaceManager::addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, cpVect tInset )
{
	this->addWindowContainmentWithFriction ( fFriction, fElasticity, tInset, 1.0f );
}

KDvoid CCSpaceManager::addWindowContainmentWithFriction ( cpFloat fFriction, cpFloat fElasticity, cpVect tInset, cpFloat fRadius )
{
	const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );

	this->addWindowContainmentWithFriction ( fFriction, fElasticity, tWinSize, tInset, 1.0f );
}

KDvoid CCSpaceManager::step ( cpFloat fDelta )
{		
	// re-calculate static pShape positions if this is set
	if ( m_bRehashStaticEveryStep )
	{
		cpSpaceReindexStatic ( m_pSpace );
	}
	
	m_fLastDelta = fDelta;
	
	if ( !m_fConstantDelta )
	{	
		cpFloat  dt = fDelta / m_nSteps;
		for ( KDint i = 0; i < m_nSteps; i++ )
		{
			cpSpaceStep ( m_pSpace, dt );
		}
	}
	else 
	{
		cpFloat  dt = m_fConstantDelta / m_nSteps;
		fDelta += m_fTimeAccumulator;
		while ( fDelta >= m_fConstantDelta ) 
		{
			for ( KDint i = 0; i < m_nSteps; i++ )
			{
				cpSpaceStep ( m_pSpace, dt );
			}
			
			fDelta -= m_fConstantDelta;
		}
		
		m_fTimeAccumulator = fDelta > 0 ? fDelta : 0.0f;
	}
	
	if ( m_pIterateFunc )
	{
        m_pIterateFunc ( m_pSpace, m_pEachFunc );
	}
}

KDbool CCSpaceManager::isBodyShared ( cpBody* pBody )
{
    KDint  nCountShared = 0;		
    
    // anyone else have this pBody? - should prob not count static..?
    cpBodyEachShape ( pBody, countBodyReferences, &nCountShared );
    
    return ( nCountShared > 1 );
}

KDvoid CCSpaceManager::removeAndMaybeFreeBody ( cpBody* pBody, KDbool bFreeBody )
{
    // in this space?
    if ( cpSpaceContainsBody ( m_pSpace, pBody ) )
	{
        cpSpaceRemoveBody ( m_pSpace, pBody );
	}
    
    // Free it
    if ( bFreeBody )
    {
        // cleanup any constraints
        if ( m_bCleanupBodyDependencies )
		{
            this->removeAndFreeConstraintsOnBody ( pBody );
		}
        
        cpBodyFree ( pBody );
    }
}

KDvoid CCSpaceManager::removeAndMaybeFreeShape ( cpShape* pShape, KDbool bFreeShape )
{	
    // This space owns it?
    if ( cpSpaceContainsShape ( m_pSpace, pShape ) )
	{
       cpSpaceRemoveShape ( m_pSpace, pShape );
	}
	
	// Make sure it's not our static pBody
	if ( pShape->body != m_pSpace->staticBody )
	{
		// Checking if this pBody is shared....!
		// if not then get rid of it
		if ( !this->isBodyShared ( pShape->body ) )
		{
            this->removeAndMaybeFreeBody ( pShape->body, bFreeShape );
		}
	}
	
	if ( bFreeShape )
	{
		cpShapeFree ( pShape );	
	}
}

cpShape* CCSpaceManager::removeShape ( cpShape* pShape )
{
	if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, _removeShape, pShape, this );
	}
	else
	{
		this->removeAndMaybeFreeShape ( pShape, KD_FALSE );
	}
	
	return pShape;
}

KDvoid CCSpaceManager::removeAndFreeShape ( cpShape* pShape )
{
	if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, _removeAndFreeShape, pShape, this );
	}
	else
	{
		this->removeAndMaybeFreeShape ( pShape, KD_TRUE );
	}
}

KDvoid CCSpaceManager::setupDefaultShape ( cpShape* pShape )
{
	// Remember to set these later, if you want different values
	pShape->e = .5; 
	pShape->u = .5;
	pShape->collision_type = 0;
	pShape->data = KD_NULL;
}

cpShape* CCSpaceManager::addCircleAt ( cpVect tPos, cpFloat fMass, cpFloat fRadius )
{
    cpBody*  pBody;
	if ( fMass != STATIC_MASS )
	{
        pBody = cpBodyNew ( fMass, cpMomentForCircle ( fMass, fRadius, fRadius, cpvzero ) );
	}
	else
	{
        pBody = cpBodyNewStatic ( );
	}
    
    cpBodySetPos ( pBody, tPos );
    
    this->addBody ( pBody );
	
	return this->addCircleToBody ( pBody, fRadius );
}

cpShape* CCSpaceManager::addCircleToBody ( cpBody* pBody, cpFloat fRadius )
{
    return this->addCircleToBody ( pBody, fRadius, cpvzero );
}

cpShape* CCSpaceManager::addCircleToBody ( cpBody* pBody, cpFloat fRadius, cpVect tOffset )
{
    cpShape*  pShape;
    
    pShape = cpCircleShapeNew ( pBody, fRadius, cpvzero );
    cpCircleShapeSetOffset ( pShape, tOffset );
	
	this->setupDefaultShape ( pShape );
	this->addShape ( pShape );
    
    return pShape;
}

cpShape* CCSpaceManager::addRectAt ( cpVect tPos, cpFloat fMass, cpFloat fWidth, cpFloat fHeight, cpFloat fRotation )
{	
    const cpFloat fHalfH = fHeight / 2.0f;
	const cpFloat fHalfW = fWidth  / 2.0f;

	return this->addPolyAt ( tPos, fMass, fRotation, 4, cpv ( -fHalfW,  fHalfH ),	// top-left  
														cpv (  fHalfW,  fHalfH ),	// top-right 
														cpv (  fHalfW, -fHalfH ),	// bottom-right 
														cpv ( -fHalfW, -fHalfH ) );	// bottom-left 
}

cpShape* CCSpaceManager::addRectToBody ( cpBody* pBody, cpFloat fWidth, cpFloat fHeight, cpFloat fRotation )
{
    return this->addRectToBody ( pBody, fWidth, fHeight, fRotation, cpvzero );
}

cpShape* CCSpaceManager::addRectToBody ( cpBody* pBody, cpFloat fWidth, cpFloat fHeight, cpFloat fRotation, cpVect tOffset )
{
    const cpFloat fHalfH = fHeight / 2.0f;
	const cpFloat fHalfW = fWidth  / 2.0f;

    cpVect  aVerts[4] = 
	{
        cpv ( -fHalfW,  fHalfH ),	// top-left  
        cpv (  fHalfW,  fHalfH ),	// top-right 
        cpv (  fHalfW, -fHalfH ),	// bottom-right 
        cpv ( -fHalfW, -fHalfH )    // bottom-left 
    };

    return this->addPolyToBody ( pBody, fRotation, 4, aVerts, tOffset );		                                                                        
}

cpShape* CCSpaceManager::addPolyAt ( cpVect tPos, cpFloat fMass, cpFloat fRotation, KDint nNumPoints, cpVect tPoint, ... )
{
	cpShape*  pShape = KD_NULL;
	
	KDVaListKHR  pArgs;
	KD_VA_START_KHR ( pArgs, tPoint );
		
	// Setup our vertices
	cpVect*   pVerts = new cpVect [ nNumPoints ];
	
	pVerts [ 0 ] = tPoint;
	for ( KDint i = 1; i < nNumPoints; i++ )
	{
		pVerts [ i ] = KD_VA_ARG ( pArgs, cpVect );
	}
		
	pShape = this->addPolyAt ( tPos, fMass, fRotation, nNumPoints, pVerts );
		
	KD_VA_END_KHR ( pArgs );
	
	CC_SAFE_DELETE_ARRAY ( pVerts );

	return pShape;	
}

cpShape* CCSpaceManager::addPolyAt ( cpVect tPos, cpFloat fMass, cpFloat fRotation, CCArray* pPoints )
{	
	// Setup our vertices
	KDint    nNumPoints = pPoints->count ( );
	cpVect*  pVerts = new cpVect [ nNumPoints ];

	for ( KDint i = 0; i < nNumPoints; i++ )
	{
		CCPoint*  pPoint = (CCPoint*) pPoints->objectAtIndex ( i );

		pVerts [ i ].x = pPoint->x;
		pVerts [ i ].y = pPoint->y;
	}

	return this->addPolyAt ( tPos, fMass, fRotation, nNumPoints, pVerts );	
}

// patch submitted by ja...@nuts.pl for c-style arrays
cpShape* CCSpaceManager::addPolyAt ( cpVect tPos, cpFloat fMass, cpFloat fRotation, KDint nNumPoints, cpVect* pPoints )
{
	if ( nNumPoints >= 3 )
	{		
		//Setup our poly pShape
		cpBody*  pBody;
        if ( fMass != STATIC_MASS )
		{
            pBody = cpBodyNew ( fMass, cpMomentForPoly ( fMass, nNumPoints, pPoints, cpvzero ) );
		}
        else
		{
            pBody = cpBodyNewStatic ( );
		}

        cpBodySetPos ( pBody, tPos );
        cpBodySetAngle ( pBody, fRotation );
        
        this->addBody ( pBody );
        
        // rotation should be zero now
        return this->addPolyToBody ( pBody, 0, nNumPoints, pPoints );
	}
    else
	{
        return KD_NULL;
	}
}

cpShape* CCSpaceManager::addPolyToBody ( cpBody* pBody, cpFloat fRotation, CCArray* pPoints )
{
    return this->addPolyToBody ( pBody, fRotation, pPoints, cpvzero );
}

cpShape* CCSpaceManager::addPolyToBody ( cpBody* pBody, cpFloat fRotation, CCArray* pPoints, cpVect tOffset )
{
    // Setup our vertices
	KDint    nNumPoints = pPoints->count ( );
	cpVect*  pVerts = new cpVect [ nNumPoints ];

	for ( KDint i = 0; i < nNumPoints; i++ )
	{
		CCPoint*  pPoint = (CCPoint*) pPoints->objectAtIndex ( i );

		pVerts [ i ].x = pPoint->x;
		pVerts [ i ].y = pPoint->y;
	}

	return this->addPolyToBody ( pBody, fRotation, nNumPoints, pVerts, tOffset );	
}

cpShape* CCSpaceManager::addPolyToBody ( cpBody* pBody, cpFloat fRotation, KDint nNumPoints, cpVect* pPoints )
{
    return this->addPolyToBody ( pBody, fRotation, nNumPoints, pPoints, cpvzero ); 
}

cpShape* CCSpaceManager::addPolyToBody ( cpBody* pBody, cpFloat fRotation, KDint nNumPoints, cpVect* pPoints, cpVect tOffset )
{
    cpShape*  pShape = KD_NULL;
    
    if ( nNumPoints >= 3 )
    {
        if ( fRotation != 0.0f )
        {
            cpVect  tAngle = cpvforangle ( fRotation );
            for ( KDint i = 0; i < nNumPoints; i++ )
			{
                pPoints [ i ] = cpvrotate ( pPoints [ i ], tAngle );
			}
        }
        
        pShape = cpPolyShapeNew ( pBody, nNumPoints, pPoints, tOffset );
		
		this->setupDefaultShape ( pShape );
		this->addShape ( pShape );
    }
    
    return pShape;
}

cpShape* CCSpaceManager::addSegmentAtWorldAnchor ( cpVect tFromPos, cpVect tToPos, cpFloat fMass, cpFloat fRadius )
{
	cpVect  tPos = cpvmult ( cpvsub ( tToPos, tFromPos ), 0.5f );
	return this->addSegmentAt ( cpvadd ( tFromPos, tPos ), cpvmult ( tPos, -1 ), tPos, fMass, fRadius );
}

cpShape* CCSpaceManager::addSegmentAt ( cpVect tPos, cpVect tFromPos, cpVect tToPos, cpFloat fMass, cpFloat fRadius )
{
    cpBody*  pBody;

	if ( fMass != STATIC_MASS )
	{
        pBody = cpBodyNew ( fMass, cpMomentForSegment ( fMass, tFromPos, tToPos ) );
	}
    else
	{
        pBody = cpBodyNewStatic ( );
	}
    
    cpBodySetPos ( pBody, tPos );
    
    this->addBody ( pBody );

    return this->addSegmentToBody ( pBody, tFromPos, tToPos, fRadius );
}

cpShape* CCSpaceManager::addSegmentToBody ( cpBody* pBody, cpVect tFromPos, cpVect tToPos, cpFloat fRadius )
{
    cpShape* pShape;

	pShape = cpSegmentShapeNew ( pBody, tFromPos, tToPos, fRadius );
	
	this->setupDefaultShape ( pShape );
	this->addShape ( pShape );
	
	return pShape;    
}

cpShape* CCSpaceManager::getShapeAt ( cpVect tPos, cpLayers uLayers, cpGroup uGroup )
{
	return cpSpacePointQueryFirst ( m_pSpace, tPos, uLayers, uGroup );
}

cpShape* CCSpaceManager::getShapeAt ( cpVect tPos )
{
	return this->getShapeAt ( tPos, CP_ALL_LAYERS, CP_NO_GROUP );
}

KDvoid CCSpaceManager::rehashActiveShapes ( KDvoid )
{
	cpSpatialIndexEach    ( m_pSpace->CP_PRIVATE ( activeShapes ), (cpSpatialIndexIteratorFunc) &updateBBCache, KD_NULL );
	cpSpatialIndexReindex ( m_pSpace->CP_PRIVATE ( activeShapes ) );
}

KDvoid CCSpaceManager::rehashStaticShapes ( KDvoid )
{
	cpSpaceReindexStatic ( m_pSpace );
}

KDvoid CCSpaceManager::rehashShape ( cpShape* pShape )
{
	cpSpaceReindexShape ( m_pSpace, pShape );
}

CCArray* CCSpaceManager::getShapesAt ( cpVect tPos, cpLayers uLayers, cpGroup uGroup )
{
	CCArray*  pShapes = CCArray::create ( );
	cpSpacePointQuery ( m_pSpace, tPos, uLayers, uGroup, (cpSpacePointQueryFunc) collectAllShapes, pShapes );
		
	return pShapes;
}

CCArray* CCSpaceManager::getShapesAt ( cpVect tPos )
{
	return this->getShapesAt ( tPos, CP_ALL_LAYERS, CP_NO_GROUP );
}

CCArray* CCSpaceManager::getShapesAt ( cpVect tPos, KDfloat fRadius, cpLayers uLayers, cpGroup uGroup )
{
	CCArray*  pShapes = CCArray::create ( );
	
	cpCircleShape  tCircle;
	cpCircleShapeInit ( &tCircle, this->getStaticBody ( ), fRadius, tPos );
	tCircle.shape.layers = uLayers;
	tCircle.shape.group  = uGroup;
	
	cpSpaceShapeQuery ( m_pSpace, (cpShape*) (&tCircle), (cpSpaceShapeQueryFunc) collectAllCollidingShapes, pShapes );
	
	return pShapes;
}

CCArray* CCSpaceManager::getShapesAt ( cpVect tPos, KDfloat fRadius )
{
	return this->getShapesAt ( tPos, fRadius, CP_ALL_LAYERS, CP_NO_GROUP );
}

CCArray* CCSpaceManager::getShapesOnBody ( cpBody* pBody )
{
	CCArray*  pShapes = CCArray::create ( ); 
    cpBodyEachShape ( pBody, collectAllShapesOnBody, pShapes );

    return pShapes;
}

cpShape* CCSpaceManager::getShapeFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup )
{
	return cpSpaceSegmentQueryFirst ( m_pSpace, tStart, tEnd, uLayers, uGroup, KD_NULL );
}

cpShape* CCSpaceManager::getShapeFromRayCastSegment ( cpVect tStart, cpVect tEnd )
{
	return this->getShapeFromRayCastSegment ( tStart, tEnd, CP_ALL_LAYERS, CP_NO_GROUP );
}

cpSegmentQueryInfo CCSpaceManager::getInfoFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup )
{
	cpSegmentQueryInfo  tInfo;
	cpSpaceSegmentQueryFirst ( m_pSpace, tStart, tEnd, uLayers, uGroup, &tInfo );
	
	return tInfo;
}
	 
cpSegmentQueryInfo CCSpaceManager::getInfoFromRayCastSegment ( cpVect tStart, cpVect tEnd )
{
	return this->getInfoFromRayCastSegment ( tStart, tEnd, CP_ALL_LAYERS, CP_NO_GROUP );
}

CCArray* CCSpaceManager::getShapesFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup )
{
	CCArray*  pArray = CCArray::create ( );
	
	cpSpaceSegmentQuery ( m_pSpace, tStart, tEnd, uLayers, uGroup, (cpSpaceSegmentQueryFunc) collectAllSegmentQueryShapes, pArray );
	
	return pArray;
}

CCArray* CCSpaceManager::getShapesFromRayCastSegment ( cpVect tStart, cpVect tEnd )
{
	return this->getShapesFromRayCastSegment ( tStart, tEnd, CP_ALL_LAYERS, CP_NO_GROUP );
}

CCArray* CCSpaceManager::getInfosFromRayCastSegment ( cpVect tStart, cpVect tEnd, cpLayers uLayers, cpGroup uGroup )
{
	CCArray*  pArray = CCArray::create ( );
	
	cpSpaceSegmentQuery ( m_pSpace, tStart, tEnd, uLayers, uGroup, (cpSpaceSegmentQueryFunc) collectAllSegmentQueryInfos, pArray );
	
	return pArray;
}

CCArray* CCSpaceManager::getInfosFromRayCastSegment ( cpVect tStart, cpVect tEnd )
{
	return this->getInfosFromRayCastSegment ( tStart, tEnd, CP_ALL_LAYERS, CP_NO_GROUP );
}

KDvoid CCSpaceManager::applyLinearExplosionAt ( cpVect tAt, cpFloat fRadius, cpFloat fMaxForce )
{	
	this->applyLinearExplosionAt ( tAt, fRadius, fMaxForce, CP_ALL_LAYERS, CP_NO_GROUP );
}

KDvoid CCSpaceManager::applyLinearExplosionAt ( cpVect tAt, cpFloat fRadius, cpFloat fMaxForce, cpLayers uLayers, cpGroup uGroup )
{
	cpBB  tBB = { tAt.x - fRadius, tAt.y - fRadius, tAt.x + fRadius, tAt.y + fRadius };
	ExplosionQueryContext  tContext = { uLayers, uGroup, tAt, fRadius, fMaxForce };
	cpSpatialIndexQuery ( m_pSpace->CP_PRIVATE ( activeShapes ), &tBB, tBB, (cpSpatialIndexQueryFunc) ExplosionQueryHelper, &tContext );
}

KDbool CCSpaceManager::isPersistentContactOnShape ( cpShape* pShape, cpShape* pShape2 )
{    
    CCArray*    pArray  = this->persistentContactInfosOnShape ( pShape );
	CCObject*   pObject = KD_NULL;
	CCARRAY_FOREACH ( pArray, pObject )
    {
        cpArbiter*  pArb = (cpArbiter*) pObject;
        
        CP_ARBITER_GET_SHAPES ( pArb, a, b )

        // If a or b is pShape2 return KD_TRUE 
        if ( pShape2 == a || pShape2 == b )
		{
            return KD_TRUE;
		}
    }
    
	return KD_FALSE;
}

cpShape* CCSpaceManager::persistentContactOnShape ( cpShape* pShape )
{
	cpShape*  pContactShape = KD_NULL;
    CCArray*  pArray = this->persistentContactInfosOnShape ( pShape );
    
	if ( pArray->count ( ) != 0 )
    { 
        cpArbiter*  pArb = (cpArbiter*) pArray->objectAtIndex ( 0 );
        
		CP_ARBITER_GET_SHAPES ( pArb, a, b )
        
        // Get the pShape that isn't the one passed in 
		pContactShape = ( a == pShape ) ? b : a;
	}

	return pContactShape;
}

cpArbiter* CCSpaceManager:: persistentContactInfoOnShape ( cpShape* pShape )
{
	cpArbiter*  pRet = KD_NULL;
    CCArray*  pArray = this->persistentContactInfosOnShape ( pShape );
    
    if ( pArray->count ( ) != 0 )
	{
        pRet = (cpArbiter*) pArray->objectAtIndex ( 0 );
	}
	
	return pRet;
}

CCArray* CCSpaceManager::persistentContactInfosOnShape ( cpShape* pShape )
{
	CCArray*  pArray = CCArray::create ( );
    cpBodyEachArbiter ( pShape->body, collectAllArbiters, pArray );
    
    return pArray;
}

CCArray* CCSpaceManager::getConstraints ( KDvoid )
{
    CCArray*  pConstraints = CCArray::create ( );
    cpSpaceEachConstraint ( m_pSpace, collectAllConstraints, pConstraints );
    
	return pConstraints;
}

CCArray* CCSpaceManager::getConstraintsOnBody ( cpBody* pBody )
{
	CCArray*  pConstraints = CCArray::create ( );
    cpBodyEachConstraint ( pBody, collectAllBodyConstraints, pConstraints );
    
	return pConstraints;
}

KDbool CCSpaceManager::isSpaceLocked ( )
{
    return m_pSpace->CP_PRIVATE ( locked ) != 0;
}

KDvoid CCSpaceManager::addShape ( cpShape* pShape )
{
	if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, _addShape, pShape, this );
	}
	else
	{
		_addShape ( m_pSpace, pShape, this );	
	}
}

KDvoid CCSpaceManager::removeAndFreeBody ( cpBody* pBody )
{
    if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, _removeAndFreeBody, pBody, this );
	}
	else
	{
		this->removeAndMaybeFreeBody ( pBody, KD_TRUE );
	}
}

KDvoid CCSpaceManager::removeBody ( cpBody* pBody )
{
  	if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, _removeBody, pBody, this );
	}
	else
	{
		this->removeAndMaybeFreeBody ( pBody, KD_FALSE );  
	}
}

KDvoid CCSpaceManager::addBody ( cpBody* pBody )
{
	if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, _addBody, pBody, this );
	}
	else
	{
		_addBody ( m_pSpace, pBody, this );	
	}
}

cpShape* CCSpaceManager::morphShapeToStatic ( cpShape* pShape )
{
	return this->morphShapeToActive ( pShape, STATIC_MASS );
}

cpShape* CCSpaceManager::morphShapeToActive ( cpShape* pShape, cpFloat fMass )
{
    // TODO: Make this truly static
    
    // Grab the current pBody
    cpBody*  pOldBody = pShape->body;
    cpBody*  pNewBody;
 
    // Remove the pShape from the space while we're messing with it
    this->removeShape ( pShape );
    
    if ( fMass == STATIC_MASS )
	{
        pNewBody = cpBodyNewStatic ( );
	}
    else
    {  
        cpFloat  fMoment = KD_INFINITY;
        
        switch ( pShape->CP_PRIVATE ( klass )->type )
        {
            case CP_CIRCLE_SHAPE :
                fMoment = cpMomentForCircle ( fMass, cpCircleShapeGetRadius ( pShape ), cpCircleShapeGetRadius ( pShape ), cpvzero );
                break;

            case CP_SEGMENT_SHAPE :
                fMoment = cpMomentForSegment ( fMass, cpSegmentShapeGetA ( pShape ), cpSegmentShapeGetB ( pShape ) );
                break;

            case CP_POLY_SHAPE :
                fMoment = cpMomentForPoly ( fMass, cpPolyShapeGetNumVerts ( pShape ), ( (cpPolyShape*) pShape )->verts, cpvzero );
                break;

            default :
                break;
        }
        
        pNewBody = cpBodyNew ( fMass, fMoment );
    }
    // Copy over all the fields that matter (hopefully)
    cpBodySetPos    ( pNewBody, cpBodyGetPos    ( pOldBody ) );
    cpBodySetAngle  ( pNewBody, cpBodyGetAngle  ( pOldBody ) );
    cpBodySetVel    ( pNewBody, cpBodyGetVel    ( pOldBody ) );
    cpBodySetAngVel ( pNewBody, cpBodyGetAngVel ( pOldBody ) );
    pNewBody->velocity_func = pOldBody->velocity_func;
    pNewBody->data = pOldBody->data;
    
    // If no one else is using this pBody get rid of it
    if ( this->isBodyShared ( pOldBody ) )
	{
        this->removeAndFreeBody ( pOldBody );
	}
    
    pShape->body = pNewBody;
    
    this->addShape ( pShape );
    
	if ( fMass != STATIC_MASS )
	{
        this->addBody ( pNewBody );
	}
    	
	return pShape;
}

cpShape* CCSpaceManager::morphShapeToKinematic ( cpShape* pShape )
{
    if ( cpSpaceContainsBody ( m_pSpace, pShape->body ) )
	{
        cpSpaceRemoveBody ( m_pSpace, pShape->body );
	}
	return pShape;
}

CCArray* CCSpaceManager::fragmentShape ( cpShape* pShape, KDint nPieces, KDfloat fMass )
{
	cpShapeType    eType = pShape->CP_PRIVATE ( klass )->type;
	CCArray*  pFragments = KD_NULL;
	
	if ( eType == CP_CIRCLE_SHAPE )
	{
		cpCircleShape*  pCircle = (cpCircleShape*) pShape;
		pFragments = this->fragmentCircle ( pCircle, nPieces, fMass );
	}
	else if ( eType == CP_SEGMENT_SHAPE )
	{
		cpSegmentShape*  pSegment = (cpSegmentShape*) pShape;
		pFragments = this->fragmentSegment ( pSegment, nPieces, fMass );
	}
	else if ( eType == CP_POLY_SHAPE )
	{
		cpPolyShape*  pPoly = (cpPolyShape*) pShape;
		
		// get a square grid size number
		nPieces = (KDint) kdSqrtf ( (KDfloat) nPieces );
		
		// only support rects right now
		pFragments = this->fragmentRect ( pPoly, nPieces, nPieces, fMass );
	}
	
	return pFragments;
}

CCArray* CCSpaceManager::fragmentRect ( cpPolyShape* pPoly, KDint nRows, KDint nCols, KDfloat fMass )
{
	CCArray*  pFragments = KD_NULL;
	cpBody*   pBody = ( (cpShape*) pPoly )->body;
	
	if ( pPoly->numVerts == 4 )
	{
		pFragments = CCArray::create ( );
		cpShape*  pFragment;
		
		// use the opposing endpoints (diagonal) to calc width & height
		KDfloat   w = kdFabsf ( pPoly->verts[0].x - pPoly->verts[1].x );
		KDfloat   h = kdFabsf ( pPoly->verts[1].y - pPoly->verts[2].y );
		
		KDfloat  fw = w / nCols;
		KDfloat  fh = h / nRows;
		
		for ( KDint i = 0; i < nCols; i++ )
		{
			for ( KDint j = 0; j < nRows; j++ )
			{
				cpVect  tPoint = cpvadd ( cpv ( fw / 2.0f, fh / 2.0f ), cpv ( ( i * fw ) - w / 2.0f, ( j * fh ) - h / 2.0f ) );
		
				tPoint = cpBodyLocal2World ( pBody, tPoint );
				
				pFragment = this->addRectAt ( tPoint, fMass, fw, fh, cpBodyGetAngle ( pBody ) );
				
				pFragments->addObject ( CCPointer::create ( pFragment ) );
			}
		}
		
		this->removeAndFreeShape ( (cpShape*) pPoly );
	}
	
	return pFragments;
}

CCArray* CCSpaceManager::fragmentCircle ( cpCircleShape* pCircle, KDint nPieces, KDfloat fMass )
{
	CCArray*   pFragments = CCArray::create ( );
	cpBody*    pBody = ( (cpShape*) pCircle )->body;
	KDfloat    fRadius = pCircle->r;
	
	cpShape*  pFragment;
	KDfloat   fRadians = 2 * KD_PI_F / nPieces;
	KDfloat   fA = fRadians;
	cpVect    tPt1, tPt2, tPt3, tAvg;
	
	tPt1 = cpv ( fRadius, 0 );
	
	for ( KDint i = 0; i < nPieces; i++ )
	{		
		tPt2 = cpvmult ( cpvforangle ( fA ), fRadius );
		
		// get the centroid
		tAvg = cpvmult ( cpvadd ( tPt1, tPt2 ), 1.0f / 3.0f );
		tPt3 = cpvadd ( pBody->p, tAvg );
		
		pFragment = this->addPolyAt ( tPt3, fMass, 0, 3, cpvsub ( cpvzero, tAvg ), cpvsub ( tPt2, tAvg ), cpvsub ( tPt1, tAvg ) );
		pFragments->addObject ( CCPointer::create ( pFragment ) );
		
		tPt1 = tPt2;
		fA += fRadians;
	}
	
	this->removeAndFreeShape ( (cpShape*) pCircle );
	
	return pFragments;
}

CCArray* CCSpaceManager::fragmentSegment( cpSegmentShape* pSegment, KDint nPieces, KDfloat fMass )
{
	CCArray*  pFragments = CCArray::create ( );
	cpBody*   pBody = ( (cpShape*) pSegment )->body;
	
	cpShape*  pFragment;
	cpVect    tPt   = pSegment->a;
	cpVect    tDiff = cpvsub ( pSegment->b, pSegment->a );
	cpVect    tDxdy = cpvmult ( tDiff, 1.0f / nPieces );
	KDfloat   fLen  = cpvlength ( tDxdy );
	KDfloat   fRad  = cpvtoangle ( tDiff );
	
	for ( KDint i = 0; i < nPieces; i++)
	{
		pFragment = this->addRectAt ( cpBodyLocal2World ( pBody, tPt ), fMass, fLen, pSegment->r * 2, fRad );
		pFragments->addObject ( CCPointer::create ( pFragment ) );
		tPt = cpvadd ( tPt, tDxdy );
	}
	
	this->removeAndFreeShape ( (cpShape*) pSegment );
	
	return pFragments;	
}

KDvoid CCSpaceManager::combineShapes ( cpShape* pShapes, ... )
{
	CCArray*  ss = CCArray::createWithCapacity ( 2 );
    
	KDVaListKHR  pArgs;
	KD_VA_START_KHR ( pArgs, pShapes );
	
	cpShape*  pShape = pShapes;
	cpBody*   pBody  = pShape->body; 
	
	// Setup initial data
	cpVect   mr = cpvmult(pBody->p, pBody->m);
	cpFloat  total_mass = pBody->m;
	ss->addObject ( CCPointer::create ( pShape ) );
	
	while ( ( pShape = KD_VA_ARG ( pArgs, cpShape* ) ) )
	{
		pBody = pShape->body;
		
		// Calculate the sum of the "first mass moments"
		// Treating each pShape/pBody as a particle
		mr = cpvadd ( mr, cpvmult ( pBody->p, pBody->m ) );
		total_mass += pBody->m;
		
		ss->addObject ( CCPointer::create ( pShape ) );
	}
	KD_VA_END_KHR ( pArgs );
    
	KDint count = ss->count ( );
	
	// Make sure no funny business
	if ( count > 1 )
	{
		// Calculate the center of mass
		cpVect    cm = cpvmult ( mr, 1.0f / ( total_mass ) );
		cpFloat  moi = 0;
		
		// Grab first pShape
		cpShape*  first_pShape = (cpShape*) ss->objectAtIndex ( 0 );
		
		// Calculate the new moment of inertia
		for ( KDint i = 0; i < count; i++ )
		{
			pShape = (cpShape*) ss->objectAtIndex ( i );
			pBody  = pShape->body;
			
			cpVect  offset = cpvsub ( pBody->p, cm );
			
			// apply the offset (based off type)
			this->offsetShape ( pShape, offset );
			
			// summation of inertia
			moi += ( pBody->i + pBody->m * cpvdot ( offset, offset ) );
			
			// Remove all but first pBody (for reuse)
			if ( i )
			{
                // This will correctly take the pShape from the pBody's list
                cpSpaceRemoveShape ( m_pSpace, pShape );
                
                // New pBody for this pShape
                cpShapeSetBody ( pShape, first_pShape->body );
                
                // cleanup old pBody, unless first
                if ( pBody != first_pShape->body )
				{
                    // Only remove if in the space
                    if ( cpSpaceContainsBody ( m_pSpace, pBody ) )
					{
                        cpSpaceRemoveBody ( m_pSpace, pBody );
					}
				
                    if ( pBody != m_pSpace->staticBody )
					{
                        cpBodyFree ( pBody );
					}
                }
                
                // Put the pShape back
                cpSpaceAddShape ( m_pSpace, pShape );
			}
		}
		
		// New mass and moment of inertia
		cpBodySetMass   ( first_pShape->body, total_mass );
		cpBodySetMoment ( first_pShape->body, moi );
		
		// New pos
		cpBodySetPos ( first_pShape->body, cm );
	}
}


KDvoid CCSpaceManager::offsetShape ( cpShape* pShape, cpVect tOffset )
{
	switch ( pShape->CP_PRIVATE ( klass )->type )
	{
		case CP_CIRCLE_SHAPE :
			cpCircleShapeSetOffset ( pShape, tOffset );
			break;

		case CP_SEGMENT_SHAPE :
		{
			cpVect  tA = cpSegmentShapeGetA ( pShape );
			cpVect  tB = cpSegmentShapeGetB ( pShape );
			
			cpSegmentShapeSetEndpoints ( pShape, cpvadd ( tA, tOffset ), cpvadd ( tB, tOffset ) );
			break;
		}

		case CP_POLY_SHAPE :
		{
			KDint    nNumVerts = cpPolyShapeGetNumVerts ( pShape );
			cpVect*  pVerts = (cpVect*) kdMalloc ( sizeof ( cpVect ) * nNumVerts );
			
			// have to copy... oh well
			for ( KDint i = 0; i < nNumVerts; i++ )
			{
				pVerts[i] = cpPolyShapeGetVert ( pShape, i );
			}
			
			cpPolyShapeSetVerts ( pShape, nNumVerts, pVerts, tOffset );
			
			kdFree ( pVerts );
			break;
		}

		default :
			break;
	}	
}

cpConstraint* CCSpaceManager::removeConstraint ( cpConstraint* pConstraint )
{
    if ( cpSpaceContainsConstraint ( m_pSpace, pConstraint ) )
	{
        cpSpaceRemoveConstraint ( m_pSpace, pConstraint );	
	}
	
    return pConstraint;
}

KDvoid CCSpaceManager::removeAndFreeConstraint ( cpConstraint* pConstraint )
{
	this->removeConstraint ( pConstraint );
	cpConstraintFree ( pConstraint );
}

KDvoid CCSpaceManager::removeAndFreeConstraintsOnBody ( cpBody* pBody )
{
	CCArray*  pArray = CCArray::create ( );
    cpBodyEachConstraint ( pBody, collectAllBodyConstraints, pArray );

	cpConstraint*  pConstraint = KD_NULL;
	CCObject*      pObject     = KD_NULL;
	CCARRAY_FOREACH ( pArray, pObject )
    {
        // Callback for about to free constraint
        // reason: it's the only thing that may be deleted arbitrarily
        // because of the cleanupBodyDependencies
        pConstraint = (cpConstraint*) pObject;
        
        m_pConstraintCleanupDelegate->aboutToFreeConstraint ( pConstraint );
        cpSpaceRemoveConstraint ( m_pSpace, pConstraint );   
	}
}

cpConstraint* CCSpaceManager::addSpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2, cpFloat fRest, cpFloat fStiff, cpFloat fDamp )
{
	cpConstraint*  pSpring = cpDampedSpringNew ( pToBody, pFromBody, tAnchr1, tAnchr2, fRest, fStiff, fDamp );
	return cpSpaceAddConstraint ( m_pSpace, pSpring );
}

cpConstraint* CCSpaceManager::addSpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRest, cpFloat fStiff, cpFloat fDamp )
{
	return this->addSpringToBody ( pToBody, pFromBody, cpvzero, cpvzero, fRest, fStiff, fDamp );
}

cpConstraint* CCSpaceManager::addSpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fStiff )
{	
	return this->addSpringToBody ( pToBody, pFromBody, 0.0f, fStiff, 1.0f );
}

cpConstraint* CCSpaceManager::addGrooveToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tGroove1, cpVect tGroove2, cpVect tAnchor2 )
{
	cpConstraint*  pGroove = cpGrooveJointNew ( pToBody, pFromBody, tGroove1, tGroove2, tAnchor2 );
	return cpSpaceAddConstraint ( m_pSpace, pGroove );
}

cpConstraint* CCSpaceManager::addGrooveToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fLength, KDbool bHoriz, cpVect tAnchor2 )
{
	cpVect  tDiff = cpvzero;
	
	if ( bHoriz )
	{
		tDiff = cpv ( fLength / 2.0f, 0.0f );
	}
	else
	{
		tDiff = cpv ( 0.0f, fLength / 2.0f );
	}
	
	return this->addGrooveToBody ( pToBody, pFromBody, cpvsub ( pToBody->p, tDiff ), cpvadd ( pToBody->p, tDiff ), tAnchor2 );
}

cpConstraint* CCSpaceManager::addGrooveToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fLength, KDbool bHoriz )
{
	return this->addGrooveToBody ( pToBody, pFromBody, fLength, bHoriz, cpvzero );
}

cpConstraint* CCSpaceManager::addSlideToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2, cpFloat fMin, cpFloat fMax )
{	
	cpConstraint*  pSlide = cpSlideJointNew ( pToBody, pFromBody, tAnchr1, tAnchr2, fMin, fMax );
	return cpSpaceAddConstraint ( m_pSpace, pSlide );
}

cpConstraint* CCSpaceManager::addSlideToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fMin, cpFloat fMax )
{
	return this->addSlideToBody ( pToBody, pFromBody, cpvzero, cpvzero, fMin, fMax );
}

cpConstraint* CCSpaceManager::addPinToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2 )
{
	cpConstraint*  pPin = cpPinJointNew ( pToBody, pFromBody, tAnchr1, tAnchr2 );
	return cpSpaceAddConstraint ( m_pSpace, pPin );
}

cpConstraint* CCSpaceManager::addPinToBody ( cpBody* pToBody, cpBody* pFromBody )
{
	return this->addPinToBody ( pToBody, pFromBody, cpvzero, cpvzero );
}

cpConstraint* CCSpaceManager::addPivotToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr1, cpVect tAnchr2 )
{
	cpConstraint*  pPin = cpPivotJointNew2 ( pToBody, pFromBody, tAnchr1, tAnchr2 );
	return cpSpaceAddConstraint ( m_pSpace, pPin );
}

cpConstraint* CCSpaceManager::addPivotToBody ( cpBody* pToBody, cpBody* pFromBody, cpVect tAnchr )
{
	cpConstraint*  pPin = cpPivotJointNew ( pToBody, pFromBody, tAnchr );
	return cpSpaceAddConstraint ( m_pSpace, pPin );	
}

cpConstraint* CCSpaceManager::addPivotToBody ( cpBody* pToBody, cpBody* pFromBody )
{
	return this->addPivotToBody ( pToBody, pFromBody, cpvzero, cpvzero );
}

cpConstraint* CCSpaceManager::addMotorToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRate )
{
	cpConstraint*  pMotor = cpSimpleMotorNew ( pToBody, pFromBody, fRate );
	return cpSpaceAddConstraint ( m_pSpace, pMotor );
}

cpConstraint* CCSpaceManager::addMotorToBody ( cpBody* toBody, cpFloat fRate )
{
	return this->addMotorToBody ( toBody, m_pSpace->staticBody, fRate );
}

cpConstraint* CCSpaceManager::addGearToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fPhase, cpFloat fRatio )
{
	cpConstraint*  pGear = cpGearJointNew ( pToBody, pFromBody, fPhase, fRatio );
	return cpSpaceAddConstraint ( m_pSpace, pGear );
}

cpConstraint* CCSpaceManager::addGearToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRatio )
{
	return this->addGearToBody ( pToBody, pFromBody, 0.0f, fRatio );
}

cpConstraint* CCSpaceManager::addBreakableToConstraint ( cpConstraint* pBreakConstraint, cpFloat fMax )
{
	//cpConstraint*  pBreakable = cpBreakableJointNew ( breakConstraint, m_pSpace );
	//pBreakable->maxForce = fMax;
	//return cpSpaceAddConstraint ( m_pSpace, pBreakable );
	return KD_NULL;
}

cpConstraint* CCSpaceManager::addRotaryLimitToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fMin, cpFloat fMax )
{
	cpConstraint*  pRotaryLimit = cpRotaryLimitJointNew ( pToBody, pFromBody, fMin, fMax );
	return cpSpaceAddConstraint ( m_pSpace, pRotaryLimit );
}

cpConstraint* CCSpaceManager::addRotaryLimitToBody ( cpBody* toBody, cpFloat fMin, cpFloat fMax )
{
	return this->addRotaryLimitToBody ( toBody, m_pSpace->staticBody, fMin, fMax );
}

cpConstraint* CCSpaceManager::addRatchetToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fPhase, cpFloat fRatchet )
{
	cpConstraint*  pRachet = cpRatchetJointNew ( pToBody, pFromBody, fPhase, fRatchet );
	return cpSpaceAddConstraint ( m_pSpace, pRachet );
}

cpConstraint* CCSpaceManager::addRatchetToBody ( cpBody* pToBody, cpFloat fPhase, cpFloat fRatchet )
{
	return this->addRatchetToBody ( pToBody, m_pSpace->staticBody, fPhase, fRatchet );
}

KDvoid CCSpaceManager::ignoreCollisionBetweenType ( KDuint uType1, KDuint uType2 )
{
	cpSpaceAddCollisionHandler ( m_pSpace, uType1, uType2, KD_NULL, collIgnore, KD_NULL, KD_NULL, KD_NULL );
}

cpConstraint* CCSpaceManager::addRotarySpringToBody ( cpBody* pToBody, cpBody* pFromBody, cpFloat fRestAngle, cpFloat fStiff, cpFloat fDamp )
{
	cpConstraint*  pRotarySpring = cpDampedRotarySpringNew( pToBody, pFromBody, fRestAngle, fStiff, fDamp );
	return cpSpaceAddConstraint ( m_pSpace, pRotarySpring );
}

cpConstraint* CCSpaceManager::addRotarySpringToBody ( cpBody* toBody, cpFloat fRestAngle, cpFloat fStiff, cpFloat fDamp )
{
	return this->addRotarySpringToBody ( toBody, m_pSpace->staticBody, fRestAngle, fStiff, fDamp );
}

cpConstraint* CCSpaceManager::addPulleyToBody ( cpBody* pToBody, cpBody* pFromBody, cpBody* pPulleyBody,
							                  cpVect tAnchor1, cpVect tAnchor2, cpVect tAnchor3a, cpVect tanchor3b, cpFloat fRatio )
{
	cpConstraint*  pPulley = cpPulleyJointNew ( pToBody, pFromBody, pPulleyBody, tAnchor1, tAnchor2, tAnchor3a, tanchor3b, fRatio );
	return cpSpaceAddConstraint ( m_pSpace, pPulley );
}

cpConstraint* CCSpaceManager::addPulleyToBody ( cpBody* pToBody, cpBody* pFromBody,
							                  cpVect tAnchor1, cpVect tAnchor2, cpVect tAnchor3a, cpVect tanchor3b, cpFloat fRatio )
{
	return this->addPulleyToBody ( pToBody, pFromBody, m_pSpace->staticBody, tAnchor1, tAnchor2, tAnchor3a, tanchor3b, fRatio );
}

KDvoid CCSpaceManager::addCollisionCallbackBetweenType ( KDuint uType1, KDuint uType2, CCObject* pTarget, SEL_SpaceHandler pSelector )
{
	SmgrInvocation*  pInvocation = SmgrInvocation::create ( uType1, uType2, pTarget, pSelector );

	// add the callback to chipmunk
	cpSpaceAddCollisionHandler ( m_pSpace, uType1, uType2, collBegin, collPreSolve, collPostSolve, collSeparate, pInvocation );
	
	// we'll keep a ref so it won't disappear, prob could just retain and clear hash later
	m_pInvocations->addObject ( pInvocation );
}

KDvoid CCSpaceManager::addCollisionCallbackBetweenType ( KDuint uType1, KDuint uType2, CCObject* pTarget, SEL_SpaceHandler pSelector, CollisionMoment eMoments, ... )
{
	SmgrInvocation*  pInvocation = SmgrInvocation::create ( uType1, uType2, pTarget, pSelector );

	cpCollisionBeginFunc		pBegin	   = KD_NULL;
	cpCollisionPreSolveFunc		pPreSolve  = KD_NULL;
	cpCollisionPostSolveFunc	pPostSolve = KD_NULL;
	cpCollisionSeparateFunc		pSeparate  = KD_NULL;

	KDVaListKHR  pArgs;
	KD_VA_START_KHR ( pArgs, eMoments );
	
	while ( eMoments != 0 )
	{
		switch ( eMoments ) 
		{
			case COLLISION_BEGIN	 : pBegin	  = collBegin;		break;
			case COLLISION_PRESOLVE  : pPreSolve  = collPreSolve;	break;
			case COLLISION_POSTSOLVE : pPostSolve = collPostSolve;	break;
			case COLLISION_SEPARATE  : pSeparate  = collSeparate;	break;

			default : break;
		}
		eMoments = (CollisionMoment) KD_VA_ARG ( pArgs, KDint );
	}

	KD_VA_END_KHR ( pArgs );
		
	// add the callback to chipmunk
	cpSpaceAddCollisionHandler ( m_pSpace, uType1, uType2, pBegin, pPreSolve, pPostSolve, pSeparate, pInvocation );
	
	// we'll keep a ref so it won't disappear, prob could just retain and clear hash later
	m_pInvocations->addObject ( pInvocation );
}

KDvoid CCSpaceManager::removeCollisionCallbackBetweenType ( KDuint uType1, KDuint uType2 )
{
	// Chipmunk hashes the invocation for us, we must pull it out
	cpCollisionTypePair*  pPair = (cpCollisionTypePair*) kdMalloc ( sizeof ( cpCollisionTypePair ) );

    pPair->a = uType1;
    pPair->b = uType2;
	
	// delete the invocation, if there is one
	if ( this->isSpaceLocked ( ) )
	{
		cpSpaceAddPostStepCallback ( m_pSpace, removeCollision, (KDvoid*) pPair, (KDvoid*) m_pInvocations );
	}
	else
	{
		removeCollision ( m_pSpace, (KDvoid*) pPair, (KDvoid*) m_pInvocations );
	}
}

KDvoid smgrBasicIterateShapesFunc ( cpSpace* pSpace, smgrEachFunc pFunc )
{
    cpSpaceEachShape ( pSpace, (cpSpaceShapeIteratorFunc) pFunc, KD_NULL );
}

KDvoid smgrBasicIterateActiveShapesOnlyFunc ( cpSpace* pSpace, smgrEachFunc pFunc )
{
    cpSpatialIndexEach ( pSpace->CP_PRIVATE ( activeShapes ), pFunc, KD_NULL );
}

// Look into position_func off of cpBody for more efficient sync 
KDvoid smgrBasicEachShape ( KDvoid* pShapePtr, KDvoid* pData )
{
	cpShape*  pShape = (cpShape*) pShapePtr;
	CCNode*   pNode  = (CCNode*)  pShape->data;
	
	if ( pNode ) 
	{
		cpBody*  pBody = pShape->body;

		pNode->setPosition ( pBody->p );
		pNode->setRotation ( CC_RADIANS_TO_DEGREES ( -pBody->a ) );
	}
}

KDvoid smgrBasicEachShapeOrBody ( KDvoid* pShapePtr, KDvoid* pData )
{
	cpShape*  pShape = (cpShape*) pShapePtr;
	CCNode*   pNode  = (CCNode*)  pShape->data;
    cpBody*   pBody  = pShape->body;
    
    if ( !pNode )
	{
        pNode = (CCNode*) pBody->data;
	}
	
	if ( pNode ) 
	{
		pNode->setPosition ( pBody->p );
		pNode->setRotation ( CC_RADIANS_TO_DEGREES ( -pBody->a ) );
	}
}

KDvoid smgrEachShapeAsChildren ( KDvoid* pShapePtr, KDvoid* pData )
{
	cpShape*  pShape = (cpShape*) pShapePtr;
	CCNode*   pNode  = (CCNode*)  pShape->data;

	if ( pNode ) 
	{
		cpBody*  pBody = pShape->body;
		CCNode*  pParent = pNode->getParent ( );

		if ( pParent )
		{
			pNode->setPosition ( pParent->convertToNodeSpace ( pBody->p ) );

			cpVect  tZ = cpv ( pNode->convertToNodeSpace ( CCPointZero ) );
			cpVect  tD = cpv ( pNode->convertToNodeSpace ( cpvforangle ( pBody->a ) ) );
			cpVect  tR = cpv ( cpvsub ( tD, tZ ) );
			KDfloat fAngle = cpvtoangle ( tR );

			pNode->setRotation ( CC_RADIANS_TO_DEGREES ( -fAngle ) );
		}
		else
		{
			pNode->setPosition ( pBody->p );
			pNode->setRotation ( CC_RADIANS_TO_DEGREES ( -pBody->a ) );
		}
	}
}

KDvoid smgrBasicIterateBodiesFunc ( cpSpace* pSpace, smgrEachFunc pFunc )
{
    cpSpaceEachBody ( pSpace, (cpSpaceBodyIteratorFunc) pFunc, KD_NULL );
}

KDvoid smgrBasicEachBody ( KDvoid* pBodyPtr, KDvoid* pData )
{
    cpBody*  pBody = (cpBody*) pBodyPtr;
	CCNode*  pNode = (CCNode*) pBody->data;
	
	if ( pNode ) 
	{
		pNode->setPosition ( pBody->p );
		pNode->setRotation ( CC_RADIANS_TO_DEGREES ( -pBody->a ) );
	}
}

NS_CC_END
