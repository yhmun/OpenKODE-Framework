/* --------------------------------------------------------------------------------
 *
 *      File            RayCast.h
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

#ifndef __RayCast_h__
#define __RayCast_h__

// This test demonstrates how to use the world ray-cast feature.
// NOTE: we are intentionally filtering one of the polygons, therefore
// the ray will always miss one type of polygon.

// This callback finds the closest hit. Polygon 0 is filtered.
class RayCastClosestCallback : public b2RayCastCallback
{
	public :

		RayCastClosestCallback ( KDvoid );

	public :

		KDfloat		ReportFixture (	b2Fixture* pFixture, const b2Vec2& tPoint, const b2Vec2& tNormal, KDfloat fFraction );
	
	public :

		KDbool				m_bHit;
		b2Vec2				m_tPoint;
		b2Vec2				m_tNormal;
		KDfloat				m_fFraction;
		b2Fixture*			m_pFixture;
};

// This callback finds any hit. Polygon 0 is filtered.
class RayCastAnyCallback : public b2RayCastCallback
{
	public :

		RayCastAnyCallback ( KDvoid );

	public :

		KDfloat		ReportFixture (	b2Fixture* pFixture, const b2Vec2& tPoint, const b2Vec2& tNormal, KDfloat fFraction );

	public :

		KDbool				m_bHit;
		b2Vec2				m_tPoint;
		b2Vec2				m_tNormal;
		KDfloat				m_fFraction;
		b2Fixture*			m_pFixture;
};

// This ray cast collects multiple hits along the ray. Polygon 0 is filtered.
class RayCastMultipleCallback : public b2RayCastCallback
{
	public :

		enum
		{
			e_maxCount = 3
		};

		RayCastMultipleCallback ( KDvoid );

	public :

		KDfloat		ReportFixture (	b2Fixture* pFixture, const b2Vec2& tPoint, const b2Vec2& tNormal, KDfloat fFraction );

	public :

		b2Vec2				m_aPoints	[ e_maxCount ];
		b2Vec2				m_aNormals	[ e_maxCount ];
		KDfloat				m_aFractions[ e_maxCount ];
		b2Fixture*			m_aFixtures	[ e_maxCount ];
		KDint				m_nCount;
};

#endif	// __RayCast_h__
