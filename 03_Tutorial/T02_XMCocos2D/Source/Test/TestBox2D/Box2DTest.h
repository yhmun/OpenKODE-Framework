/* --------------------------------------------------------------------------
 *
 *      File            Box2DTest.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2006-2009 Erin Catto
 *
 *         http://www.cocos2d-x.org      
 *         http://www.box2d.org  
 *
 * --------------------------------------------------------------------------
 * 
 *      This software is provided 'as-is', without any express or implied
 *      warranty.  In no event will the authors be held liable for any damages
 *      arising from the use of this software.
 *      Permission is granted to anyone to use this software for any purpose,
 *      including commercial applications, and to alter it and redistribute it
 *      freely, subject to the following restrictions:
 *      1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *      2. Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *      3. This notice may not be removed or altered from any source distribution.
 *
 * -------------------------------------------------------------------------- */

#ifndef __Box2DTest_h__
#define __Box2DTest_h__

#include <XMBox2D/box2d.h>
#include "Box2DRender.h"

class  Test;
struct Settings;

typedef Test*			TestCreateFcn ( );

#define RAND_LIMIT		32767

/// Random number in range [-1,1]
inline KDfloat RandomFloat ( KDvoid )
{
	KDfloat		r = (KDfloat) ( kdRand ( ) & (RAND_LIMIT) );
	r /= RAND_LIMIT;
	r  = 2.0f * r - 1.0f;
	return r;
}

/// Random floating point number in range [lo, hi]
inline KDfloat RandomFloat ( KDfloat lo, KDfloat hi )
{
	KDfloat r = (KDfloat) ( kdRand ( ) & (RAND_LIMIT) );
	r /= RAND_LIMIT;
	r = ( hi - lo ) * r + lo;
	return r;
}

/// Test settings. Some can be controlled in the GUI.
struct Settings
{
	Settings ( KDvoid ) :
        viewCenter(0.0f, 20.0f),
        hz(60.0f),
        velocityIterations(8),
        positionIterations(3),
        drawShapes(1),
        drawJoints(1),
        drawAABBs(0),
        drawPairs(0),
        drawContactPoints(0),
        drawContactNormals(0),
        drawContactForces(0),
        drawFrictionForces(0),
        drawCOMs(0),
        drawStats(0),
        drawProfile(0),
        enableWarmStarting(1),
        enableContinuous(1),
        enableSubStepping(0),
        pause(0),
        singleStep(0)
	{
	
	}
	
    b2Vec2		viewCenter;
    KDfloat32	hz;
    KDint32		velocityIterations;
    KDint32		positionIterations;
    KDint32		drawShapes;
    KDint32		drawJoints;
    KDint32		drawAABBs;
    KDint32		drawPairs;
    KDint32		drawContactPoints;
    KDint32		drawContactNormals;
    KDint32		drawContactForces;
    KDint32		drawFrictionForces;
    KDint32		drawCOMs;
    KDint32		drawStats;
    KDint32		drawProfile;
    KDint32		enableWarmStarting;
    KDint32		enableContinuous;
    KDint32		enableSubStepping;
    KDint32		pause;
    KDint32		singleStep;
};

struct TestEntry
{
    const KDchar*    name;
	TestCreateFcn*   createFcn;
};

extern TestEntry g_testEntries[];
// This is called when a joint in the world is implicitly destroyed
// because an attached body is destroyed. This gives us a chance to
// nullify the mouse joint.
class DestructionListener : public b2DestructionListener
{
	public :

		KDvoid SayGoodbye ( b2Fixture* fixture ) { B2_NOT_USED ( fixture ); }
		KDvoid SayGoodbye ( b2Joint* joint );
		
		Test*      test;
};

const KDint k_maxContactPoints = 2048;

struct ContactPoint
{
	b2Fixture*    fixtureA;
	b2Fixture*    fixtureB;
	b2Vec2        normal;
	b2Vec2        position;
	b2PointState  state;
};

class Test : public b2ContactListener
{
	public :		
				 Test ( KDvoid );
		virtual ~Test ( KDvoid );
		
	public :	

		KDvoid  SetTextLine ( KDint line )
		{
			m_textLine = line;
		}

		KDvoid  DrawTitle ( KDint x, KDint y, const KDchar* string );

		virtual KDvoid  Step ( Settings* settings );

		virtual KDvoid  Keyboard ( KDuchar key )
		{
			B2_NOT_USED ( key );
		}

		virtual KDvoid  KeyboardUp ( KDuchar key )
		{
			B2_NOT_USED ( key );
		}

		KDvoid  ShiftMouseDown ( const b2Vec2& p );
		virtual KDbool  MouseDown ( const b2Vec2& p );
		virtual KDvoid  MouseUp ( const b2Vec2& p );
		KDvoid  MouseMove ( const b2Vec2& p );
		KDvoid  LaunchBomb ( KDvoid );
		KDvoid  LaunchBomb ( const b2Vec2& position, const b2Vec2& velocity );
		
		KDvoid  SpawnBomb ( const b2Vec2& worldPt );
		KDvoid  CompleteBombSpawn ( const b2Vec2& p );
		
		// Let derived tests know that a joint was destroyed.
		virtual KDvoid  JointDestroyed ( b2Joint* joint ) 
		{
			B2_NOT_USED ( joint ); 
		}
		
		// Callbacks for derived classes.
		virtual KDvoid  BeginContact ( b2Contact* contact )
		{
			B2_NOT_USED ( contact );
		}

		virtual KDvoid  EndContact ( b2Contact* contact )
		{
			B2_NOT_USED ( contact );
		}

		virtual KDvoid  PreSolve ( b2Contact* contact, const b2Manifold* oldManifold );

		virtual KDvoid  PostSolve ( const b2Contact* contact, const b2ContactImpulse* impulse )
		{
			B2_NOT_USED ( contact );
			B2_NOT_USED ( impulse );
		}

	protected :

		friend class DestructionListener;
		friend class BoundaryListener;
		friend class ContactListener;
		friend class TestBox2DBed;
		
		b2Body*					m_groundBody;
		b2AABB					m_worldAABB;
		ContactPoint			m_points[k_maxContactPoints];
		KDint					m_pointCount;
		DestructionListener		m_destructionListener;
		GLESDebugDraw			m_debugDraw;
		KDint					m_textLine;
		b2World*				m_world;
		b2Body*					m_bomb;
		b2MouseJoint*			m_mouseJoint;
		b2Vec2					m_bombSpawnPoint;
		KDbool					m_bombSpawning;
		b2Vec2					m_mouseWorld;
		KDint					m_stepCount;
		b2Profile				m_maxProfile;
		b2Profile				m_totalProfile;
};

#endif
