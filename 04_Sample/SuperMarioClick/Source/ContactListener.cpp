/* --------------------------------------------------------------------------
 *
 *      File            ContactListener.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Liu weimu on 5/21/13
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "ContactListener.h"
#include "MashRoom.h"
#include "Ball.h"
#include "SuperMarioClick.h"

KDvoid ContactListener::BeginContact ( b2Contact* pContact ) 
{
	b2Body*		pBodyA = pContact->GetFixtureA ( )->GetBody ( );
	b2Body*		pBodyB = pContact->GetFixtureB ( )->GetBody ( );

	BodyNode*	pBodyNodeA = (BodyNode*) pBodyA->GetUserData ( );
	BodyNode*	pBodyNodeB = (BodyNode*) pBodyB->GetUserData ( );

	MashRoom*	pMashRoom;
	Ball*		pBall;

	if ( ( ( pMashRoom = dynamic_cast<MashRoom*> ( pBodyNodeA ) ) && ( pBall = dynamic_cast<Ball*> ( pBodyNodeB ) ) ) ||
		 ( ( pMashRoom = dynamic_cast<MashRoom*> ( pBodyNodeB ) ) && ( pBall = dynamic_cast<Ball*> ( pBodyNodeA ) ) ) )
	{
		pMashRoom->setDoPlunge ( KD_TRUE );
		SuperMarioClick::sharedSuperMario ( )->setContactCount ( 1 );
	}
}

KDvoid ContactListener::PreSolve ( b2Contact* pContact, const b2Manifold* pOldManifold )
{

}

KDvoid ContactListener::PostSolve ( b2Contact* pContact, const b2ContactImpulse* pImpulse )
{

}

KDvoid ContactListener::EndContact ( b2Contact* pContact ) 
{

}
