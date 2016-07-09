/* -----------------------------------------------------------------------------------
 *
 *      File            PolygonSpriteEx.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Allen Benson G Tan on 5/19/12 
 *      Copyright (c) 2012      WhiteWidget Inc. All rights reserved.   
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
#include "PolygonSpriteEx.h"

PolygonSpriteEx* PolygonSpriteEx::createWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio )
{
	PolygonSpriteEx*	pRet = new PolygonSpriteEx ( );

	if ( pRet && pRet->initWithTexture ( pTexture, pBody, bOriginal, fPTMRatio ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}


PolygonSpriteEx::PolygonSpriteEx ( KDvoid )
{
	m_pSplurt = KD_NULL;
}

PolygonSpriteEx::~PolygonSpriteEx ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSplurt );
}

KDbool PolygonSpriteEx::initWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio )
{
	if ( !PolygonSprite::initWithTexture ( pTexture, pBody, bOriginal, fPTMRatio ) )
	{
		return KD_FALSE;
	}

	m_bSliceEntered		= KD_FALSE;
	m_bSliceExited		= KD_FALSE;

	m_tEntryPoint.SetZero ( );
	m_tExitPoint .SetZero ( );

	m_fSliceEntryTime	= 0;
	m_eState			= kStateIdle;

	return KD_TRUE;
}

b2Body* PolygonSpriteEx::createBodyForWorld ( b2World* pWorld, b2Vec2 tPosition, KDfloat fRotation, b2Vec2* pVertices, KDint nVertexCount, KDfloat fDensity, KDfloat fFriction, KDfloat fRestitution )
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
	tFixtureDef.isSensor			= KD_TRUE;
    
    b2PolygonShape	tShape;
    tShape.Set ( pVertices, nVertexCount );

    tFixtureDef.shape = &tShape;
    pBody->CreateFixture ( &tFixtureDef );
    
    return pBody;
}