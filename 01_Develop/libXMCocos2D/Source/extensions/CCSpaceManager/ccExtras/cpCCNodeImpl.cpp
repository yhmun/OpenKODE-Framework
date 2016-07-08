/* --------------------------------------------------------------------------
 *
 *      File            cpCCNodeImpl.cpp
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
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCSpaceManager/CCSpaceManager.h"
#include "cocoa/CCPointer.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN

#define kSmgrEpsilon	0.001f

static KDvoid collectBodyShapes ( cpBody* pBody, cpShape* pShape, KDvoid* pData )
{
	CCArray*  pOutShapes = (CCArray*) pData;
	pOutShapes->addObject ( CCPointer::create ( pShape ) );
}

static KDvoid collectBodyFirstShape ( cpBody* pBody, cpShape* pShape, KDvoid* pData )
{
    cpShape**  pOutShape = (cpShape**) pData;
    *pOutShape = pShape;
}

static KDvoid rehashBodyShapes ( cpBody* pBody, cpShape* pShape, KDvoid* pData )
{
    CCSpaceManager*  pSpaceManager = (CCSpaceManager*) pData;
	pSpaceManager->rehashShape ( pShape );
}

static KDvoid freeBodyShapes ( cpBody* pBody, cpShape* pShape, KDvoid* pData )
{
    CCSpaceManager*  pSpaceManager = (CCSpaceManager*) pData;
	pSpaceManager->removeAndFreeShape ( pShape );
}

KDbool cpCCNodeImpl::init ( KDvoid )
{
	return this->initWithBody ( KD_NULL );
}

KDbool cpCCNodeImpl::initWithShape ( cpShape* pShape )
{
   return this->initWithBody ( pShape ? pShape->body : KD_NULL );
}

KDbool cpCCNodeImpl::initWithBody ( cpBody* pBody )
{
	m_fIntegrationDelta = 0.0f;
	this->setBody ( pBody );

	return KD_TRUE;
}

KDvoid cpCCNodeImpl::setShape ( cpShape* pShape ) 
{
	this->setBody ( pShape ? pShape->body : KD_NULL );
}

cpShape* cpCCNodeImpl::getShape ( KDvoid )
{
    cpShape*  pShape = KD_NULL;

    if ( m_pBody )
	{
        cpBodyEachShape ( m_pBody, collectBodyFirstShape, &pShape );
	}
    
    return pShape;
}

CCArray* cpCCNodeImpl::getShapes ( KDvoid )
{
	CCArray*  pShapes = CCArray::create ( );

	if ( m_pBody )
	{
		cpBodyEachShape ( m_pBody, collectBodyFirstShape, pShapes );
	}

	return pShapes;
}

KDvoid cpCCNodeImpl::setBody ( cpBody* pBody )
{
	m_pBody = pBody;
} 

cpBody* cpCCNodeImpl::getBody ( KDvoid )
{
	return m_pBody;
}

cpCCNodeImpl::cpCCNodeImpl ( KDvoid )
{
	m_pBody					= KD_NULL;
	m_pSpaceManager			= KD_NULL;
	m_bIgnoreRotation		= KD_FALSE;
	m_bAutoFreeShapeAndBody = KD_FALSE;
	m_fIntegrationDelta		= 0.0f;
}

cpCCNodeImpl::~cpCCNodeImpl ( KDvoid )
{
	if ( m_pBody && m_bAutoFreeShapeAndBody )
    {
		CCAssert ( m_pSpaceManager != KD_NULL, "" );

		cpShape*  pShape = this->getShape ( );
		if ( pShape )
		{
			pShape->data = KD_NULL;
		}

		if ( m_pBody )
		{
			m_pBody->data = KD_NULL;
		}

		cpBodyEachShape ( m_pBody, freeBodyShapes, m_pSpaceManager ); 
    }

	m_pBody = KD_NULL;
}

KDbool cpCCNodeImpl::setRotation ( KDfloat fRotation )
{
	if ( !m_bIgnoreRotation )
	{	
		// Needs a calculation for angular velocity and such
		if ( m_pBody != KD_NULL )
		{
			cpFloat  fRad = -CC_DEGREES_TO_RADIANS ( fRotation );

            // fuzzy equals
            if ( kdFabsf ( fRad - m_pBody->a ) > 0.0000001f )
			{
                cpBodySetAngle ( m_pBody, fRad );
			}
		}
	}	

	return !m_bIgnoreRotation;
}

KDvoid cpCCNodeImpl::setPosition ( const CCPoint& tPosition )
{
	if ( m_pBody != KD_NULL )
	{		
		cpVect  tPos = cpv ( tPosition );

		// If we're out of sync with chipmunk
        if ( kdFabsf ( m_pBody->p.x - tPos.x ) > kSmgrEpsilon || 
             kdFabsf ( m_pBody->p.y - tPos.y ) > kSmgrEpsilon )
		{
			// (Basic Euler integration)
			if ( m_fIntegrationDelta )
			{
				cpBodyActivate ( m_pBody );
				cpBodySetVel ( m_pBody, cpvmult ( cpvsub ( tPos, cpBodyGetPos ( m_pBody ) ), 1.0f / m_fIntegrationDelta ) );
			}
			
			// update position
			m_pBody->p = tPos;
			
			// If we're a static shape, we need to tell our space that we've changed
			if ( m_pSpaceManager && m_pBody->m == STATIC_MASS )
			{
                cpBodyEachShape ( m_pBody, rehashBodyShapes, (KDvoid*) m_pSpaceManager );                
			}
            else	// else activate!, could be sleeping
			{
				cpBodyActivate ( m_pBody );
			}
		}
	}
}

KDvoid cpCCNodeImpl::applyImpulse ( cpVect tImpulse, cpVect tOffset )
{
	if ( m_pBody != KD_NULL )
	{
		cpBodyApplyImpulse ( m_pBody, tImpulse, tOffset );
	}
}

KDvoid cpCCNodeImpl::applyForce ( cpVect tForce, cpVect tOffset )
{
	if ( m_pBody != KD_NULL )
	{
		cpBodyApplyForce ( m_pBody, tForce, tOffset );
	}
}

KDvoid cpCCNodeImpl::resetForces ( KDvoid )
{
	if ( m_pBody != KD_NULL )
	{
		cpBodyResetForces ( m_pBody );
	}
}

NS_CC_END