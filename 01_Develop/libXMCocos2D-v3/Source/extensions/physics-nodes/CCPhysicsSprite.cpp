/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsSprite.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org
 *      Copyright (c) 2012      Scott Lembcke and Howling Moon Software
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

#include "extensions/physics-nodes/CCPhysicsSprite.h"

NS_CC_EXT_BEGIN

PhysicsSprite::PhysicsSprite()
: m_bIgnoreBodyRotation(false)
, m_pCPBody(NULL)
, m_pB2Body(NULL)
, m_fPTMRatio(0.0f)
{}

PhysicsSprite* PhysicsSprite::create()
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

PhysicsSprite* PhysicsSprite::createWithTexture(Texture2D *pTexture)
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->initWithTexture(pTexture))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

PhysicsSprite* PhysicsSprite::createWithTexture(Texture2D *pTexture, const Rect& rect)
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->initWithTexture(pTexture, rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

PhysicsSprite* PhysicsSprite::createWithSpriteFrame(SpriteFrame *pSpriteFrame)
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->initWithSpriteFrame(pSpriteFrame))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

PhysicsSprite* PhysicsSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->initWithSpriteFrameName(pszSpriteFrameName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

PhysicsSprite* PhysicsSprite::create(const char *pszFileName)
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->initWithFile(pszFileName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

PhysicsSprite* PhysicsSprite::create(const char *pszFileName, const Rect& rect)
{
    PhysicsSprite* pRet = new PhysicsSprite();
    if (pRet && pRet->initWithFile(pszFileName, rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

// this method will only get called if the sprite is batched.
// return YES if the physic's values (angles, position ) changed.
// If you return NO, then getNodeToParentTransform won't be called.
bool PhysicsSprite::isDirty() const
{
    return true;
}

bool PhysicsSprite::isIgnoreBodyRotation() const
{
    return m_bIgnoreBodyRotation;
}

void PhysicsSprite::setIgnoreBodyRotation(bool bIgnoreBodyRotation)
{
    m_bIgnoreBodyRotation = bIgnoreBodyRotation;
}

// Override the setters and getters to always reflect the body's properties.
const Point& PhysicsSprite::getPosition() const
{
    return getPosFromPhysics();
}

void PhysicsSprite::getPosition(float* x, float* y) const
{
    if (x == NULL || y == NULL) {
        return;
    }
    const Point& pos = getPosFromPhysics();
    *x = pos.x;
    *y = pos.y;
}

float PhysicsSprite::getPositionX() const
{
    return getPosFromPhysics().x;
}

float PhysicsSprite::getPositionY() const
{
    return getPosFromPhysics().y;
}

//
// Chipmunk only
//

cpBody* PhysicsSprite::getCPBody() const
{
    return m_pCPBody;
}

void PhysicsSprite::setCPBody(cpBody *pBody)
{
    m_pCPBody = pBody;
}

//
// Box2d only
//

b2Body* PhysicsSprite::getB2Body() const
{
    return m_pB2Body;
}

void PhysicsSprite::setB2Body(b2Body *pBody)
{
    m_pB2Body = pBody;
}

float PhysicsSprite::getPTMRatio() const
{
    return m_fPTMRatio;
}

void PhysicsSprite::setPTMRatio(float fRatio)
{
    m_fPTMRatio = fRatio;
}

//
// Common to Box2d and Chipmunk
//

const Point& PhysicsSprite::getPosFromPhysics() const
{
    static Point s_physicPosion;

	if ( m_pCPBody )
	{
		cpVect cpPos = cpBodyGetPos(m_pCPBody);
		s_physicPosion = Point(cpPos.x, cpPos.y);
	}
	else if ( m_pB2Body )
	{
		b2Vec2 pos = m_pB2Body->GetPosition();
		float x = pos.x * m_fPTMRatio;
		float y = pos.y * m_fPTMRatio;
		s_physicPosion = Point(x,y);
	}

    return s_physicPosion;
}

void PhysicsSprite::setPosition(const Point &pos)
{
	if ( m_pCPBody )
	{
		cpVect cpPos = cpv(pos.x, pos.y);
		cpBodySetPos(m_pCPBody, cpPos);
	}
	else if ( m_pB2Body )
	{
		float angle = m_pB2Body->GetAngle();
		m_pB2Body->SetTransform(b2Vec2(pos.x / m_fPTMRatio, pos.y / m_fPTMRatio), angle);
	}
}

KDfloat PhysicsSprite::getRotation ( KDvoid ) const
{
	if ( !m_bIgnoreBodyRotation )
	{
		if ( m_pCPBody )
		{
			return -CC_RADIANS_TO_DEGREES ( cpBodyGetAngle ( m_pCPBody ) );
		}
		else if ( m_pB2Body )
		{
			return CC_RADIANS_TO_DEGREES ( m_pB2Body->GetAngle ( ) );
		}
	}

	return Sprite::getRotation ( );
}

void PhysicsSprite::setRotation(float fRotation)
{
    if (m_bIgnoreBodyRotation)
    {
        Sprite::setRotation(fRotation);
    }

    else if ( m_pCPBody )
    {
        cpBodySetAngle(m_pCPBody, -CC_DEGREES_TO_RADIANS(fRotation));
    }
    else if ( m_pB2Body )
    {
        b2Vec2 p = m_pB2Body->GetPosition();
        float radians = CC_DEGREES_TO_RADIANS(fRotation);
        m_pB2Body->SetTransform(p, radians);
    }
}

// returns the transform matrix according the Chipmunk Body values
const AffineTransform& PhysicsSprite::getNodeToParentTransform ( KDvoid ) const
{
    // Although scale is not used by physics engines, it is calculated just in case
	// the sprite is animated (scaled up/down) using actions.
	// For more info see: http://www.cocos2d-iphone.org/forum/topic/68990

	if ( m_pCPBody )
    {
		cpVect rot = (m_bIgnoreBodyRotation ? cpvforangle(-CC_DEGREES_TO_RADIANS(m_fRotationX)) : m_pCPBody->rot);
		float x = m_pCPBody->p.x + rot.x * -m_tAnchorPointInPoints.x * m_fScaleX - rot.y * -m_tAnchorPointInPoints.y * m_fScaleY;
		float y = m_pCPBody->p.y + rot.y * -m_tAnchorPointInPoints.x * m_fScaleX + rot.x * -m_tAnchorPointInPoints.y * m_fScaleY;

		if (m_bIgnoreAnchorPointForPosition)
		{
			x += m_tAnchorPointInPoints.x;
			y += m_tAnchorPointInPoints.y;
		}

		m_tTransform = AffineTransformMake ( rot.x * m_fScaleX, rot.y * m_fScaleX,
											 -rot.y * m_fScaleY, rot.x * m_fScaleY,
											 x,	y );
    }
    else if ( m_pB2Body )
    {
		b2Vec2 pos  = m_pB2Body->GetPosition();

		float x = pos.x * m_fPTMRatio;
		float y = pos.y * m_fPTMRatio;

		if (m_bIgnoreAnchorPointForPosition)
		{
			x += m_tAnchorPointInPoints.x;
			y += m_tAnchorPointInPoints.y;
		}

		// Make matrix
		float radians = m_pB2Body->GetAngle();
		float c = kdCosf(radians);
		float s = kdSinf(radians);

		if (!m_tAnchorPointInPoints.equals(Point::ZERO))
		{
			x += ((c * -m_tAnchorPointInPoints.x * m_fScaleX) + (-s * -m_tAnchorPointInPoints.y * m_fScaleY));
			y += ((s * -m_tAnchorPointInPoints.x * m_fScaleX) + (c * -m_tAnchorPointInPoints.y * m_fScaleY));
		}

		// Rot, Translate Matrix
		m_tTransform = AffineTransformMake( c * m_fScaleX,	s * m_fScaleX,
										 -s * m_fScaleY,	c * m_fScaleY,
										 x,	y );
	}

	return m_tTransform;
}

NS_CC_EXT_END
