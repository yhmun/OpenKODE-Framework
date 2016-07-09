/* --------------------------------------------------------------------------
 *
 *      File            ContactListener.cpp    
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c)           CartoonSmart LLC. All rights reserved. 
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
#include "TheLevel.h"
#include "Ninja.h"
#include "Enemy.h"
#include "StackObject.h"
#include "GroundPlane.h"
#include "GameSounds.h"

KDvoid ContactListener::BeginContact ( b2Contact* pContact )
{
//	b2ContactListener::BeginContact ( pContact );
	
	b2Body*			pBodyA			= pContact->GetFixtureA ( )->GetBody ( );
	b2Body*			pBodyB			= pContact->GetFixtureB ( )->GetBody ( );

	BodyNode*		pBodyNodeA		= (BodyNode*) pBodyA->GetUserData ( );
	BodyNode*		pBodyNodeB		= (BodyNode*) pBodyB->GetUserData ( );

	GameSounds*		pSounds			= GameSounds::sharedGameSounds ( );
	TheLevel*		pTheLevel		= TheLevel::sharedLevel ( );	
	Ninja*			pTheNinja		= KD_NULL;
	Enemy*			pTheEnemy		= KD_NULL;
	StackObject*	pTheStackObject	= KD_NULL;
	GroundPlane*	pTheGroundPlane = KD_NULL;

	////////////////////////////////////////
	////////////////////////////////////////
	// NINJA NODES WITH GROUNDPLANE

	if ( ( ( pTheNinja = dynamic_cast<Ninja*> ( pBodyNodeA ) ) && ( pTheGroundPlane = dynamic_cast<GroundPlane*> ( pBodyNodeB ) ) )
	  || ( ( pTheNinja = dynamic_cast<Ninja*> ( pBodyNodeB ) ) && ( pTheGroundPlane = dynamic_cast<GroundPlane*> ( pBodyNodeA ) ) ) )
	{		
		pSounds->playStackImpactSound ( );

		pTheLevel->stopDotting ( );
		pTheLevel->showNinjaOnGround ( pTheNinja );
		pTheLevel->proceedToNextTurn ( pTheNinja );		
	} 	

	////////////////////////////////////////
	////////////////////////////////////////
	// NINJA NODES WITH StackObject
	if ( ( ( pTheNinja = dynamic_cast<Ninja*> ( pBodyNodeA ) ) && ( pTheStackObject = dynamic_cast<StackObject*> ( pBodyNodeB ) ) )
	  || ( ( pTheNinja = dynamic_cast<Ninja*> ( pBodyNodeB ) ) && ( pTheStackObject = dynamic_cast<StackObject*> ( pBodyNodeA ) ) ) )
	{	
		pSounds->playStackImpactSound ( );

		pTheLevel->stopDotting ( );
		pTheLevel->showNinjaImpactingStack ( pTheNinja );

		pTheStackObject->playBreakAnimationFromNinjaContact ( );

		if ( pTheStackObject->getPointValue ( ) != 0 ) 
		{
			// if it has a score value for impact with Ninja
			pTheLevel->showPoints ( pTheStackObject->getPointValue ( ), pTheStackObject->getPosition ( ), pTheStackObject->getSimpleScoreVisualFX ( ) );  // show points
			pTheStackObject->makeUnScoreable ( ); //prevents scoring off same object twice
		}	
	}

	////////////////////////////////////////
	////////////////////////////////////////
	// NINJA NODES WITH Enemy
	if ( ( ( pTheNinja = dynamic_cast<Ninja*> ( pBodyNodeA ) ) && ( pTheEnemy = dynamic_cast<Enemy*> ( pBodyNodeB ) ) )
	  || ( ( pTheNinja = dynamic_cast<Ninja*> ( pBodyNodeB ) ) && ( pTheEnemy = dynamic_cast<Enemy*> ( pBodyNodeA ) ) ) )
	{	
		pTheLevel->stopDotting ( );
		pTheLevel->showNinjaImpactingStack ( pTheNinja );				// applies to stack objects or enemies
				
		if ( pTheEnemy->isBreaksOnNextDamage ( ) )
		{			
			if ( pTheEnemy->getPointValue ( ) != 0 ) 
			{
				// if it has a score value for impact with Ninja				
				pTheLevel->showPoints ( pTheEnemy->getPointValue ( ), pTheEnemy->getPosition ( ), pTheEnemy->getSimpleScoreVisualFX ( ) );		// show points
				pTheEnemy->makeUnScoreable ( );							// prevents scoring off same object twice				
			}
			pTheEnemy->breakEnemy ( );			
		} 
		else 
		{			
			pTheEnemy->damageEnemy ( );			
		}
	} 

	////////////////////////////////////////
	////////////////////////////////////////
	// StackObject WITH Enemy
	if ( ( ( pTheStackObject = dynamic_cast<StackObject*> ( pBodyNodeA ) ) && ( pTheEnemy = dynamic_cast<Enemy*> ( pBodyNodeB ) ) )
	  || ( ( pTheStackObject = dynamic_cast<StackObject*> ( pBodyNodeB ) ) && ( pTheEnemy = dynamic_cast<Enemy*> ( pBodyNodeA ) ) ) )
	{				
		if ( pTheStackObject->isCanDamageEnemy ( ) && pTheEnemy->isDamagesFromDamageEnabledStackObjects ( ) )
		{			
			if ( pTheEnemy->isBreaksOnNextDamage ( ) )
			{				
				if ( pTheEnemy->getPointValue ( ) != 0 )
				{
					// if it has a score value for impact with Ninja
					pTheLevel->showPoints ( pTheEnemy->getPointValue ( ), pTheEnemy->getPosition ( ), pTheEnemy->getSimpleScoreVisualFX ( ) );		// show points

					pTheEnemy->makeUnScoreable ( );						// prevents scoring off same object twice					
				}
				pTheEnemy->breakEnemy ( );				
			}
			else 
			{				
				pTheEnemy->damageEnemy ( );				
			}						
		}				
	} 

	////////////////////////////////////////
	////////////////////////////////////////
	// Ground WITH Enemy
	if ( ( ( pTheGroundPlane = dynamic_cast<GroundPlane*> ( pBodyNodeA ) ) && ( pTheEnemy = dynamic_cast<Enemy*> ( pBodyNodeB ) ) )
	  || ( ( pTheGroundPlane = dynamic_cast<GroundPlane*> ( pBodyNodeB ) ) && ( pTheEnemy = dynamic_cast<Enemy*> ( pBodyNodeA ) ) ) )
	{					
		if ( pTheEnemy->isDamagesFromGroundContact ( ) )
		{			
			if ( pTheEnemy->isBreaksOnNextDamage ( ) )
			{				
				if ( pTheEnemy->getPointValue ( ) != 0 )
				{ 
					// if it has a score value for impact with Ninja
					pTheLevel->showPoints ( pTheEnemy->getPointValue ( ), pTheEnemy->getPosition ( ), pTheEnemy->getSimpleScoreVisualFX ( ) );				// show points

					pTheEnemy->makeUnScoreable ( );						// prevents scoring off same object twice						
				}
				pTheEnemy->breakEnemy ( );					
			}
			else 
			{				
				pTheEnemy->damageEnemy ( );				
			}						
		}				
	} 

	////////////////////////////////////////
	////////////////////////////////////////
	// Ground WITH StackObject
	if ( ( ( pTheGroundPlane = dynamic_cast<GroundPlane*> ( pBodyNodeA ) ) && ( pTheStackObject = dynamic_cast<StackObject*> ( pBodyNodeB ) ) )
	  || ( ( pTheGroundPlane = dynamic_cast<GroundPlane*> ( pBodyNodeB ) ) && ( pTheStackObject = dynamic_cast<StackObject*> ( pBodyNodeA ) ) ) )
	{		
		pTheStackObject->playBreakAnimationFromGroundContact ( );
		
		if ( pTheStackObject->getPointValue ( ) != 0 && pTheStackObject->isBreaksOnGroundContact ( ) ) 
		{ 
			// if it has a score value for impact with Ninja			
			pTheLevel->showPoints ( pTheStackObject->getPointValue ( ), pTheStackObject->getPosition ( ), pTheStackObject->getSimpleScoreVisualFX ( ) );	// show points
						
			pTheStackObject->makeUnScoreable ( );						// prevents scoring off same object twice
		}				
	} 
}

KDvoid ContactListener::EndContact ( b2Contact* pContact )
{
	b2ContactListener::EndContact ( pContact );
}

KDvoid ContactListener::PreSolve ( b2Contact* pContact, const b2Manifold* pOldManifold )
{

}

KDvoid ContactListener::PostSolve ( b2Contact* pContact, const b2ContactImpulse* pImpulse )
{

}