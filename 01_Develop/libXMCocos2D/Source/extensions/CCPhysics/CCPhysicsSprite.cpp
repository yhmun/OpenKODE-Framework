/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsSprite.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2013 cocos2d-x.org
 *      Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 *
 *         http://www.cocos2d-x.org      
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
#include "extensions/CCPhysics/CCPhysicsSprite.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCPhysicsSprite::CCPhysicsSprite ( KDvoid )
{
	m_bIgnoreBodyRotation	= KD_FALSE;
	m_pCPBody				= KD_NULL;
	m_pB2Body				= KD_NULL;
	m_fPTMRatio				= 0.0f;
}

CCPhysicsSprite::~CCPhysicsSprite ( KDvoid )
{

}

CCPhysicsSprite* CCPhysicsSprite::create ( KDvoid )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );

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

CCPhysicsSprite* CCPhysicsSprite::createWithTexture ( CCTexture2D* pTexture )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );
    
	if ( pRet && pRet->initWithTexture ( pTexture ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

CCPhysicsSprite* CCPhysicsSprite::createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );
    
	if ( pRet && pRet->initWithTexture ( pTexture, tRect ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

CCPhysicsSprite* CCPhysicsSprite::createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );
    
	if ( pRet && pRet->initWithSpriteFrame ( pSpriteFrame ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

CCPhysicsSprite* CCPhysicsSprite::createWithSpriteFrameName ( const KDchar* szSpriteFrameName )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );

    if ( pRet && pRet->initWithSpriteFrameName ( szSpriteFrameName ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

CCPhysicsSprite* CCPhysicsSprite::create ( const KDchar* szFileName )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );

    if ( pRet && pRet->initWithFile ( szFileName ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

CCPhysicsSprite* CCPhysicsSprite::create ( const KDchar* szFileName, const CCRect& tRect )
{
    CCPhysicsSprite*  pRet = new CCPhysicsSprite ( );

    if ( pRet && pRet->initWithFile ( szFileName, tRect ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

    return pRet;
}

// this method will only get called if the sprite is batched.
// return YES if the physic's values (angles, position ) changed.
// If you return NO, then nodeToParentTransform won't be called.
KDbool CCPhysicsSprite::isDirty ( KDvoid )
{
    return KD_TRUE;
}

KDbool CCPhysicsSprite::isIgnoreBodyRotation ( KDvoid ) const
{
    return m_bIgnoreBodyRotation;
}

KDvoid CCPhysicsSprite::setIgnoreBodyRotation ( KDbool bIgnoreBodyRotation )
{
    m_bIgnoreBodyRotation = bIgnoreBodyRotation;
}

cpBody* CCPhysicsSprite::getCPBody ( KDvoid ) const
{
    return m_pCPBody;
}

KDvoid CCPhysicsSprite::setCPBody ( cpBody* pBody )
{
    m_pCPBody = pBody;
	m_pB2Body = KD_NULL;
}

b2Body* CCPhysicsSprite::getB2Body ( KDvoid ) const
{
    return m_pB2Body;
}

KDvoid CCPhysicsSprite::setB2Body ( b2Body* pBody )
{
    m_pB2Body = pBody;
	m_pCPBody = KD_NULL;
}

KDfloat CCPhysicsSprite::getPTMRatio ( KDvoid ) const
{
    return m_fPTMRatio;
}

KDvoid CCPhysicsSprite::setPTMRatio ( KDfloat fRatio )
{
    m_fPTMRatio = fRatio;
}

// Override the setters and getters to always reflect the body's properties.
const CCPoint& CCPhysicsSprite::getPosition ( KDvoid )
{
	updatePosFromPhysics ( );
	return CCNode::getPosition ( );
}

KDvoid CCPhysicsSprite::getPosition ( KDfloat* x, KDfloat* y )
{
    updatePosFromPhysics ( );
    return CCNode::getPosition ( x, y );
}

KDfloat CCPhysicsSprite::getPositionX ( KDvoid )
{
    updatePosFromPhysics ( );
    return m_tPosition.x;
}

KDfloat CCPhysicsSprite::getPositionY ( KDvoid )
{
    updatePosFromPhysics ( );
    return m_tPosition.y;
}

KDvoid CCPhysicsSprite::setPosition ( const CCPoint& tPos )
{
	if ( m_pCPBody )
	{
		cpVect  tPos2 = cpv ( tPos.x, tPos.y );
		cpBodySetPos ( m_pCPBody, tPos2 );
	}
	else if ( m_pB2Body )
	{
		KDfloat  fAngle = m_pB2Body->GetAngle ( );
		m_pB2Body->SetTransform ( b2Vec2 ( tPos.x / m_fPTMRatio, tPos.y / m_fPTMRatio ), fAngle );
	}
}

KDfloat CCPhysicsSprite::getRotation ( KDvoid )
{
	KDfloat  fRotate = 0;

	if ( m_bIgnoreBodyRotation )
	{
		fRotate = CCSprite::getRotation ( );
	}
	else
	{
		if ( m_pCPBody )
		{
			fRotate = -CC_RADIANS_TO_DEGREES ( cpBodyGetAngle ( m_pCPBody ) );
		}
		else if ( m_pB2Body )
		{
			fRotate = CC_RADIANS_TO_DEGREES ( m_pB2Body->GetAngle ( ) );
		}
	}

	return fRotate;
}

KDvoid CCPhysicsSprite::setRotation ( KDfloat fRotation )
{
    if ( m_bIgnoreBodyRotation )
    {
        CCSprite::setRotation ( fRotation );
    }
    else
    {
		if ( m_pCPBody )
		{
			cpBodySetAngle ( m_pCPBody, -CC_DEGREES_TO_RADIANS ( fRotation ) );
		}
		else if ( m_pB2Body )
		{
			b2Vec2   tPos = m_pB2Body->GetPosition();
			KDfloat  fRadians = CC_DEGREES_TO_RADIANS ( fRotation );
			m_pB2Body->SetTransform ( tPos, fRadians );
		}
    }
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform CCPhysicsSprite::nodeToParentTransform ( KDvoid )
{
	if ( m_pCPBody )
	{
		cpVect   tRot = ( m_bIgnoreBodyRotation ? cpvforangle ( -CC_DEGREES_TO_RADIANS ( m_tRotation.x ) ) : m_pCPBody->rot );

		KDfloat  x = m_pCPBody->p.x + tRot.x * ( -m_tAnchorPointInPoints.x ) - tRot.y * ( -m_tAnchorPointInPoints.y );
		KDfloat  y = m_pCPBody->p.y + tRot.y * ( -m_tAnchorPointInPoints.x ) + tRot.x * ( -m_tAnchorPointInPoints.y );

		if ( m_bIgnoreAnchorPointForPosition )
		{
			x += m_tAnchorPointInPoints.x;
			y += m_tAnchorPointInPoints.y;
		}

		m_tTransform = CCAffineTransformMake ( tRot.x, tRot.y, -tRot.y, tRot.x, x, y );
	}
	else if ( m_pB2Body )
	{
	    b2Vec2   tPos = m_pB2Body->GetPosition ( );
	
		KDfloat  x = tPos.x * m_fPTMRatio;
		KDfloat  y = tPos.y * m_fPTMRatio;

		if ( m_bIgnoreAnchorPointForPosition )
		{
			x += m_tAnchorPointInPoints.x;
			y += m_tAnchorPointInPoints.y;
		}

		// Make matrix
		KDfloat  fRadians = m_pB2Body->GetAngle ( );
		KDfloat  c = kdCosf ( fRadians );
		KDfloat  s = kdSinf ( fRadians );
		
		// Although scale is not used by physics engines, it is calculated just in case
		// the sprite is animated (scaled up/down) using actions.
		// For more info see: http://www.cocos2d-iphone.org/forum/topic/68990
		if ( !m_tAnchorPointInPoints.equals ( CCPointZero ) )
		{
			x += ( ( c * -m_tAnchorPointInPoints.x * m_tScale.x ) + ( -s * -m_tAnchorPointInPoints.y * m_tScale.y ) );
			y += ( ( s * -m_tAnchorPointInPoints.x * m_tScale.x ) + (  c * -m_tAnchorPointInPoints.y * m_tScale.y ) );
		}
	    
		// Rot, Translate Matrix
		m_tTransform = CCAffineTransformMake ( c * m_tScale.x, s * m_tScale.x, -s * m_tScale.y, c * m_tScale.y, x, y );
	}
	
	return m_tTransform;
}

KDvoid CCPhysicsSprite::updatePosFromPhysics ( KDvoid )
{
	if ( m_pCPBody )
	{
		cpVect  tPos = cpBodyGetPos ( m_pCPBody );
		m_tPosition = ccp ( tPos.x, tPos.y );
	}
	else if ( m_pB2Body )
	{
	    b2Vec2  tPos = m_pB2Body->GetPosition ( );
		m_tPosition = ccp ( tPos.x * m_fPTMRatio, tPos.x * m_fPTMRatio );
	}
}

NS_CC_END
