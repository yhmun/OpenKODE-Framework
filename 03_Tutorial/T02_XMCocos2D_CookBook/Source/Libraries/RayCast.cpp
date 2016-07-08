/* --------------------------------------------------------------------------------
 *
 *      File            RayCast.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2006-2009 Erin Catto. All rights reserved.
 *
 *                    http://www.gphysics.com
 * 
 * --------------------------------------------------------------------------------
 * 
 *      This software is provided 'as-is', without any express or implied
 *      warranty.  In no event will the authors be held liable for any damages
 *      arising from the use of this software.
 *      Permission is granted to anyone to use this software for any purpose,
 *      including commercial applications, and to alter it and redistribute it
 *      freely, subject to the following restrictions:
 *      1. The origin of this software must not be misrepresented; you must not
 *         claim that you wrote the original software. If you use this software
 *         in a product, an acknowledgment in the product documentation would be
 *         appreciated but is not required.
 *      2. Altered source versions must be plainly marked as such, and must not be
 *         misrepresented as being the original software.
 *      3. This notice may not be removed or altered from any source distribution.
 *
 * -------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "RayCast.h"


RayCastClosestCallback::RayCastClosestCallback ( KDvoid )
{
	m_bHit = KD_FALSE;
}

KDfloat RayCastClosestCallback::ReportFixture (	b2Fixture* pFixture, const b2Vec2& tPoint, const b2Vec2& tNormal, KDfloat fFraction )
{
	b2Body*		pBody = pFixture->GetBody ( );
	KDvoid*		pUserData = pBody->GetUserData ( );

	if ( pUserData )
	{
		KDint	nIndex = *(KDint*) pUserData;
		if ( nIndex == 0 )
		{
			// filter
			return -1.0f;
		}
	}

	m_bHit		= KD_TRUE;
	m_tPoint	= tPoint;
	m_tNormal	= tNormal;
	m_fFraction = fFraction;
	m_pFixture	= pFixture;

	return fFraction;
}

RayCastAnyCallback::RayCastAnyCallback ( KDvoid )
{
	m_bHit = KD_FALSE;
}

KDfloat RayCastAnyCallback::ReportFixture (	b2Fixture* pFixture, const b2Vec2& tPoint, const b2Vec2& tNormal, KDfloat fFraction )
{
	b2Body*		pBody = pFixture->GetBody ( );
	KDvoid*		pUserData = pBody->GetUserData ( );

	if ( pUserData )
	{
		KDint	nIndex = *(KDint*) pUserData;
		if ( nIndex == 0 )
		{
			// filter
			return -1.0f;
		}
	}

	m_bHit		= KD_TRUE;
	m_tPoint	= tPoint;
	m_tNormal	= tNormal;
	m_fFraction = fFraction;
	m_pFixture	= pFixture;

	return 0.0f;
}

RayCastMultipleCallback::RayCastMultipleCallback ( KDvoid )
{
	m_nCount = 0;
}

KDfloat RayCastMultipleCallback::ReportFixture ( b2Fixture* pFixture, const b2Vec2& tPoint, const b2Vec2& tNormal, KDfloat fFraction )
{
	b2Body*		pBody = pFixture->GetBody ( );
	KDvoid*		pUserData = pBody->GetUserData ( );

	if ( pUserData )
	{
		KDint	nIndex = *(KDint*) pUserData;
		if ( nIndex == 0 )
		{
			// filter
			return -1.0f;
		}
	}
	b2Assert ( m_nCount < e_maxCount );

	m_aPoints	 [ m_nCount ] = tPoint;
	m_aNormals	 [ m_nCount ] = tNormal;
	m_aFractions [ m_nCount ] = fFraction;
	m_aFixtures	 [ m_nCount ] = pFixture;

	++m_nCount;

	if ( m_nCount == e_maxCount )
	{
		return 0.0f;
	}

	return 1.0f;
}
