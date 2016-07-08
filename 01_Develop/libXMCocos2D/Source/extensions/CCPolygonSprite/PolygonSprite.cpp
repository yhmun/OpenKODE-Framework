/* -----------------------------------------------------------------------------------
 *
 *      File            PolygonSprite.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      zeiteisens. All rights reserved. ( Created by Hanno Bruns on 24.06.12 )
 *
 *         http://precognitiveresearch.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCPolygonSprite/PolygonSprite.h"
#include "textures/CCTextureCache.h"

NS_CC_BEGIN

PolygonSprite::PolygonSprite ( KDvoid )
{
    m_fPTMRatio = 32;
}

PolygonSprite::~PolygonSprite ( KDvoid )
{
    
}

PolygonSprite* PolygonSprite::create ( const KDchar* szFilename, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio )
{
    PolygonSprite*	pRet = new PolygonSprite ( );

    if  ( pRet && pRet->initWithFile ( szFilename, pBody, bOriginal, fPTMRatio ) )
    {
        pRet->autorelease ( );
    }
    else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

PolygonSprite* PolygonSprite::createWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio )
{
    PolygonSprite*	pRet = new PolygonSprite ( );

    if  ( pRet && pRet->initWithTexture ( pTexture, pBody, bOriginal, fPTMRatio ) )
    {
        pRet->autorelease ( );
    }
    else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

PolygonSprite* PolygonSprite::createWithWorld ( b2World* pWorld )
{
    PolygonSprite*	pRet = new PolygonSprite ( );

    if  ( pRet && pRet->initWithWorld ( pWorld ) )
    {
        pRet->autorelease ( );
    }
    else
	{
		CC_SAFE_DELETE ( pRet );
	}

    return pRet;
}

KDbool PolygonSprite::initWithFile ( const KDchar* szFilename, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio )
{
    CCTexture2D*	pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( szFilename );
    return this->initWithTexture ( pTexture, pBody, bOriginal, fPTMRatio );
}

KDbool PolygonSprite::initWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio )
{
    b2Fixture*				pOriginalFixture = pBody->GetFixtureList ( );
    b2PolygonShape*			pShape = (b2PolygonShape*) pOriginalFixture->GetShape ( );
    KDint					nVertextCount = pShape->GetVertexCount ( );
	Vector2dVector			aPoints;
    
	m_fPTMRatio = fPTMRatio;

	for ( KDint i = 0; i < nVertextCount; i++ )
    {
		Vector2d	tPos ( pShape->GetVertex ( i ).x * m_fPTMRatio, pShape->GetVertex ( i ).y * m_fPTMRatio );
        aPoints.push_back ( tPos );
    }

    PRFilledPolygon::initWithPoints ( aPoints, pTexture );
    
	m_pBody = pBody;
    m_pBody->SetUserData ( this );
    m_bOriginal = bOriginal;
    m_tCentroid = m_pBody->GetLocalCenter ( );

    this->setAnchorPoint ( ccp ( m_tCentroid.x * m_fPTMRatio / m_pTexture->getContentSize ( ).cx, m_tCentroid.y * m_fPTMRatio / m_pTexture->getContentSize ( ).cy ) );

    return true;
}

KDbool PolygonSprite::initWithWorld ( b2World* pWorld )
{
	// nothing to do here
    return true;
}

KDvoid PolygonSprite::setPosition ( const CCPoint& tPosition )
{
    PRFilledPolygon::setPosition ( tPosition );

    m_pBody->SetTransform ( b2Vec2 ( tPosition.x / m_fPTMRatio, tPosition.y / m_fPTMRatio ), m_pBody->GetAngle ( ) );
}

b2Body* PolygonSprite::createBodyForWorld ( b2World* pWorld, b2Vec2 tPosition, KDfloat fRotation, b2Vec2* pVertices, KDint nVertexCount, KDfloat fDensity, KDfloat fFriction, KDfloat fRestitution )
{
    b2BodyDef		tBodyDef;
    tBodyDef.type	  = b2_dynamicBody;
    tBodyDef.position = tPosition;
    tBodyDef.angle	  = fRotation;

    b2Body*			pBody = pWorld->CreateBody ( &tBodyDef );    
    
	b2FixtureDef	tFixtureDef;
    tFixtureDef.density				= fDensity;
    tFixtureDef.friction			= fFriction;
    tFixtureDef.restitution			= fRestitution;
    tFixtureDef.filter.categoryBits = 0;
    tFixtureDef.filter.maskBits		= 0;
//	tFixtureDef.isSensor			= KD_TRUE;
    
    b2PolygonShape	tShape;
    tShape.Set ( pVertices, nVertexCount );

    tFixtureDef.shape = &tShape;
    pBody->CreateFixture ( &tFixtureDef );
    
    return pBody;
}

KDvoid PolygonSprite::activateCollisions ( KDvoid )
{
    b2Fixture*	pFixture = m_pBody->GetFixtureList ( );
    b2Filter	tFilter  = pFixture->GetFilterData ( );
    tFilter.categoryBits = 0x0001;
    tFilter.maskBits	 = 0x0001;
    pFixture->SetFilterData ( tFilter );
}

KDvoid PolygonSprite::deactivateCollisions ( KDvoid )
{
    b2Fixture*	pFixture = m_pBody->GetFixtureList ( );
    b2Filter	tFilter  = pFixture->GetFilterData ( );
    tFilter.categoryBits = 0;
    tFilter.maskBits	 = 0;
    pFixture->SetFilterData ( tFilter );
}

CCAffineTransform PolygonSprite::nodeToParentTransform ( KDvoid )
{    
    b2Vec2	tPos  = m_pBody->GetPosition ( );
    
    KDfloat		x = tPos.x * m_fPTMRatio;
    KDfloat		y = tPos.y * m_fPTMRatio;
    
    if ( m_bIgnoreAnchorPointForPosition )
	{
        x += getAnchorPointInPoints ( ).x;
        y += getAnchorPointInPoints ( ).y;
    }
    
    // Make matrix
    KDfloat		fRadians = m_pBody->GetAngle ( );
    KDfloat		c = kdCosf ( fRadians );
    KDfloat		s = kdSinf ( fRadians );
    
    if ( !m_tAnchorPointInPoints.equals ( CCPointZero ) )
	{
        x += c * -m_tAnchorPointInPoints.x + -s * -m_tAnchorPointInPoints.y;
        y += s * -m_tAnchorPointInPoints.x +  c * -m_tAnchorPointInPoints.y;
    }
    
    // Rot, Translate Matrix
    m_tTransform = CCAffineTransformMake ( c, s, -s, c, x, y );

    return m_tTransform;
}

NS_CC_END