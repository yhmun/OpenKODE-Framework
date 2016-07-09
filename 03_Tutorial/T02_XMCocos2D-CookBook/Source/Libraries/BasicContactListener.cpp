/* --------------------------------------------------------------------------
 *
 *      File            BasicContactFilter.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "BasicContactListener.h"
#include "GameArea2D.h"

KDvoid BasicContactListener::BeginContact ( b2Contact* pContact )
{
	b2Body*		pBodyA = pContact->GetFixtureA ( )->GetBody ( );
	b2Body*		pBodyB = pContact->GetFixtureB ( )->GetBody ( );

	if ( pBodyA && pBodyB )
	{
		GameObject*		pObjectA = (GameObject*) pBodyA->GetUserData ( );
		GameObject*		pObjectB = (GameObject*) pBodyB->GetUserData ( );

		if ( pObjectA && pObjectB )
		{
			GameArea2D*		pGameArea = (GameArea2D*) pObjectA->getGameArea ( );

			pGameArea->handleCollisionWithObjA ( pObjectA, pObjectB );	
		}
	}	
}

KDvoid BasicContactListener::EndContact ( b2Contact* pContact )
{

}

KDvoid BasicContactListener::PreSolve ( b2Contact* pContact, const b2Manifold* pOldManifold )
{

}

KDvoid BasicContactListener::PostSolve ( b2Contact* pContact, const b2ContactImpulse* pImpulse )
{

}