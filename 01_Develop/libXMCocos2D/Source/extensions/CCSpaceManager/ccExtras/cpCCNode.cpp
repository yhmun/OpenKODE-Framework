/* --------------------------------------------------------------------------
 *
 *      File            cpCCNode.cpp
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

NS_CC_BEGIN

cpCCNode::cpCCNode ( KDvoid )
{
	m_pImplementation = KD_NULL;
}

cpCCNode::~cpCCNode ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pImplementation );
}

cpCCNode* cpCCNode::create ( KDvoid )
{
	cpCCNode*  pRet = new cpCCNode ( ); 

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

cpCCNode* cpCCNode::create ( cpShape* pShape )
{
	cpCCNode*  pRet = new cpCCNode ( ); 

	if ( pRet && pRet->initWithShape ( pShape ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool cpCCNode::init ( KDvoid )
{
	return this->initWithBody ( KD_NULL );
}

KDbool cpCCNode::initWithShape ( cpShape* pShape )
{
	if ( pShape )
	{
		pShape->data = this;
	}

	return this->initWithBody ( pShape ? pShape->body : KD_NULL );
}

KDbool cpCCNode::initWithBody ( cpBody* pBody )
{
	CPCCNODE_MEM_VARS_BODY_INIT ( pBody );

	CPCCNODE_SYNC_POS_ROT ( pBody );

	return KD_TRUE;
}

CCAffineTransform cpCCNode::nodeToParentTransform ( KDvoid )
{
	cpBody*  pBody = m_pImplementation->getBody ( );
	cpVect   tRot  = m_pImplementation->isIgnoreRotation ( ) ? cpvforangle ( -CC_DEGREES_TO_RADIANS ( m_tRotation.x ) ) : pBody->rot;

	KDfloat  x = pBody->p.x + tRot.x * -m_tAnchorPointInPoints.x - tRot.y * - m_tAnchorPointInPoints.y;
	KDfloat  y = pBody->p.y + tRot.y * -m_tAnchorPointInPoints.x + tRot.x * - m_tAnchorPointInPoints.y;

	if ( m_pImplementation->isIgnoreRotation ( ) )
	{
		x += m_tAnchorPointInPoints.x;
		y += m_tAnchorPointInPoints.y;
	}

	m_tTransform = CCAffineTransformMake ( tRot.x, tRot.y, -tRot.y, tRot.x, x, y );

	return m_tTransform;
}

KDvoid cpCCNode::setRotation ( KDfloat fRotation )
{
	if ( m_pImplementation->setRotation ( fRotation ) )
	{
		CCNode::setRotation ( fRotation );
	}
}

KDvoid cpCCNode::setPosition ( const CCPoint& tPosition )
{
	m_pImplementation->setPosition ( tPosition );
	CCNode::setPosition ( tPosition );
}

KDvoid cpCCNode::applyImpulse ( cpVect tImpulse, cpVect tOffset )
{
	m_pImplementation->applyImpulse ( tImpulse, tOffset );
}

KDvoid cpCCNode::applyForce ( cpVect tForce, cpVect tOffset )
{
	m_pImplementation->applyForce ( tForce, tOffset );
}

KDvoid cpCCNode::resetForces ( KDvoid )
{
	m_pImplementation->resetForces ( );
}

KDbool cpCCNode::isIgnoreRotation ( KDvoid )
{
	return m_pImplementation->isIgnoreRotation ( );
}

KDvoid cpCCNode::setIgnoreRotation ( KDbool bIgnore )
{
	m_pImplementation->setIgnoreRotation ( bIgnore );
}

KDbool cpCCNode::isAutoFreeShapeAndBody ( KDvoid )
{
	return m_pImplementation->isAutoFreeShapeAndBody ( );
}

KDvoid cpCCNode::setAutoFreeShapeAndBody ( KDbool bAuto )
{
	m_pImplementation->setAutoFreeShapeAndBody ( bAuto );
}

cpFloat cpCCNode::getIntegrationDelta ( KDvoid )
{
	return m_pImplementation->getIntegrationDelta ( );
}

KDvoid cpCCNode::setIntegrationDelta ( cpFloat fDelta )
{	
	m_pImplementation->setIntegrationDelta ( fDelta );
}

KDvoid cpCCNode::setShape ( cpShape* pShape )
{
	if ( pShape )
	{
		pShape->data = this;
	}

	this->setBody ( pShape ? pShape->body : KD_NULL );
}

cpShape* cpCCNode::getShape ( KDvoid )
{
	return m_pImplementation->getShape ( );
}

CCArray* cpCCNode::getShapes ( KDvoid )
{
	return m_pImplementation->getShapes ( );
}

KDvoid cpCCNode::setBody ( cpBody* pBody )
{
	if ( pBody )
	{
		pBody->data = this;
	}

	m_pImplementation->setBody ( pBody );
}

cpBody* cpCCNode::getBody ( KDvoid )
{
	return m_pImplementation->getBody ( );
}

KDvoid cpCCNode::setSpaceManager ( CCSpaceManager* pManager )
{
	m_pImplementation->setSpaceManager ( pManager );
}

CCSpaceManager* cpCCNode::getSpaceManager ( KDvoid )
{
	return m_pImplementation->getSpaceManager ( );
}

NS_CC_END


