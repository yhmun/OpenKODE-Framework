/* --------------------------------------------------------------------------
 *
 *      File            Box2DTestEntries.cpp
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

#include "Precompiled.h"
#include "Box2DTest.h"
//#include "GLES-Render.h"

#include "Bed/AddPair.h"
#include "Bed/ApplyForce.h"
#include "Bed/BodyTypes.h"
#include "Bed/Breakable.h"
#include "Bed/Bridge.h"
#include "Bed/BulletTest.h"
#include "Bed/Cantilever.h"
#include "Bed/Car.h"
#include "Bed/ContinuousTest.h"
#include "Bed/Chain.h"
#include "Bed/CharacterCollision.h"
#include "Bed/CollisionFiltering.h"
#include "Bed/CollisionProcessing.h"
#include "Bed/CompoundShapes.h"
#include "Bed/Confined.h"
#include "Bed/DistanceTest.h"
#include "Bed/Dominos.h"
#include "Bed/DumpShell.h"
#include "Bed/DynamicTreeTest.h"
#include "Bed/EdgeShapes.h"
#include "Bed/EdgeTest.h"
#include "Bed/Gears.h"
#include "Bed/OneSidedPlatform.h"
#include "Bed/Pinball.h"
#include "Bed/PolyCollision.h"
#include "Bed/PolyShapes.h"
#include "Bed/Prismatic.h"
#include "Bed/Pulleys.h"
#include "Bed/Pyramid.h"
#include "Bed/RayCast.h"
#include "Bed/Revolute.h"
//#include "Bed/Rope.h"
#include "Bed/RopeJoint.h"
#include "Bed/SensorTest.h"
#include "Bed/ShapeEditing.h"
#include "Bed/SliderCrank.h"
#include "Bed/SphereStack.h"
#include "Bed/TheoJansen.h"
#include "Bed/Tiles.h"
#include "Bed/TimeOfImpact.h"
#include "Bed/Tumbler.h"
#include "Bed/VaryingFriction.h"
#include "Bed/VaryingRestitution.h"
#include "Bed/VerticalStack.h"
#include "Bed/Web.h"

TestEntry   g_testEntries[] =
{ 
    {"Tumbler", Tumbler::Create},
    {"Tiles", Tiles::Create},
    {"Dump Shell", DumpShell::Create},
    {"Gears", Gears::Create},
    {"Cantilever", Cantilever::Create},
    {"Varying Restitution", VaryingRestitution::Create},
    {"Character Collision", CharacterCollision::Create},
    {"Edge Test", EdgeTest::Create},
    {"Body Types", BodyTypes::Create},
    {"Shape Editing", ShapeEditing::Create},
    {"Car", Car::Create},
    {"Apply Force", ApplyForce::Create},
    {"Prismatic", Prismatic::Create},
    {"Vertical Stack", VerticalStack::Create},
    {"SphereStack", SphereStack::Create},
    {"Revolute", Revolute::Create},
    {"Pulleys", Pulleys::Create},
    {"Polygon Shapes", PolyShapes::Create},
    //{"Rope", Rope::Create},
    {"Web", Web::Create},
    {"RopeJoint", RopeJoint::Create},
    {"One-Sided Platform", OneSidedPlatform::Create},
    {"Pinball", Pinball::Create},
    {"Bullet Test", BulletTest::Create},
    {"Continuous Test", ContinuousTest::Create},
    {"Time of Impact", TimeOfImpact::Create},
    {"Ray-Cast", RayCast::Create},
    {"Confined", Confined::Create},
    {"Pyramid", Pyramid::Create},
    {"Theo Jansen's Walker", TheoJansen::Create},
    {"Edge Shapes", EdgeShapes::Create},
    {"PolyCollision", PolyCollision::Create},
    {"Bridge", Bridge::Create},
    {"Breakable", Breakable::Create},
    {"Chain", Chain::Create},
    {"Collision Filtering", CollisionFiltering::Create},
    {"Collision Processing", CollisionProcessing::Create},
    {"Compound Shapes", CompoundShapes::Create},
    {"Distance Test", DistanceTest::Create},
    {"Dominos", Dominos::Create},
    {"Dynamic Tree", DynamicTreeTest::Create},
    {"Sensor Test", SensorTest::Create},
    {"Slider Crank", SliderCrank::Create},
    {"Varying Friction", VaryingFriction::Create},
    {"Add Pair Stress Test", AddPair::Create},
};

KDint g_totalEntries = sizeof ( g_testEntries) / sizeof ( g_testEntries[0] );

