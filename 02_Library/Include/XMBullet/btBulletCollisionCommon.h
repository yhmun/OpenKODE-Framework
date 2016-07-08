/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include "XMBullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "XMBullet/BulletCollision/CollisionDispatch/btCollisionObject.h"

///Collision Shapes
#include "XMBullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btConeShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "XMBullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "XMBullet/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btEmptyShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "XMBullet/BulletCollision/CollisionShapes/btUniformScalingShape.h"

///Narrowphase Collision Detector
#include "XMBullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"

//btSphereBoxCollisionAlgorithm is broken, use gjk for now
//#include "XMBullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "XMBullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"

///Dispatching and generation of collision pairs (broadphase)
#include "XMBullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "XMBullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"
#include "XMBullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "XMBullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h"
#include "XMBullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"

///Math library & Utils
#include "XMBullet/LinearMath/btQuaternion.h"
#include "XMBullet/LinearMath/btTransform.h"
#include "XMBullet/LinearMath/btDefaultMotionState.h"
#include "XMBullet/LinearMath/btQuickprof.h"
#include "XMBullet/LinearMath/btIDebugDraw.h"
#include "XMBullet/LinearMath/btSerializer.h"


#endif //BULLET_COLLISION_COMMON_H

