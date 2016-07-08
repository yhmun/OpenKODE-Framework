/* --------------------------------------------------------------------------
 *
 *      File            Ch7_Waypoints.cpp
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
#include "Ch7_Waypoints.h"

KDbool Ch7_Waypoints::init ( KDvoid )
{	
	// Set game area size
	m_tGameAreaSize = ccp ( 1000 / PTM_RATIO, 600 / PTM_RATIO );		// Box2d units

	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "Tap the screen move the actor to that point.\nTap again to queue another waypoint." );

	// Add polygons
	this->addRandomPolygons ( 10 );
	
	// Create Actor
	this->addActor ( );
	
	// Add draw layer
	this->addDrawLayer ( );
	
	// Create level boundaries
	this->addLevelBoundaries ( );
	
	// Initial variables
	m_fCameraZoom = 0.45f;

	return KD_TRUE;
}

KDvoid Ch7_Waypoints::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );

	// Process actor waypoints
	m_pActor->processWaypoints ( );
	
	// Turn actor toward waypoints
	if ( m_pActor->getWaypoints ( )->count ( ) > 0 )
	{
		b2Body*		pBody = m_pActor->getBody ( );
		CCPoint		tMovementVector = ccp ( pBody->GetLinearVelocity ( ).x, pBody->GetLinearVelocity ( ).y );
		pBody->SetTransform ( pBody->GetPosition ( ), -1 * GameHelper::vectorToRadians ( tMovementVector ) + PI_CONSTANT / 2 );
	}
}

/// Add actor to scene 
KDvoid Ch7_Waypoints::addActor ( KDvoid )
{
	m_pActor = GameActor::create ( );
	m_pActor->setGameArea ( this );
	
	b2BodyDef*		pBodyDef = m_pActor->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( 240 / PTM_RATIO, 160 / PTM_RATIO );
	pBodyDef->userData = m_pActor;
		
	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	m_pActor->setBody ( pBody );
	
	b2CircleShape*	pShape = new b2CircleShape ( );
	m_pActor->setCircleShape ( pShape );
	pShape->m_radius = 0.5f;

	b2FixtureDef*	pFixtureDef = m_pActor->getFixtureDef ( );
	pFixtureDef->shape = pShape;
	pFixtureDef->isSensor = KD_FALSE;
	
	pBody->CreateFixture ( pFixtureDef );
}

/// Draw all waypoint lines 
KDvoid Ch7_Waypoints::drawLayer ( KDvoid )
{
	ccDrawColor4B ( 255, 255, 0, 128 );

	b2Vec2		tPosition = m_pActor->getBody ( )->GetPosition ( );
	CCPoint		tActorPosition = ccp ( tPosition.x * PTM_RATIO, tPosition.y * PTM_RATIO );
		
	CCArray*	pWaypoints = m_pActor->getWaypoints ( );

	if ( pWaypoints->count ( ) == 1 )
	{
		GameWaypoint*	pWaypoint = (GameWaypoint*) pWaypoints->objectAtIndex ( 0 );
		ccDrawLine ( tActorPosition, pWaypoint->getPosition ( ) );
	}
	else if ( pWaypoints->count ( ) > 1 )
	{
		for ( KDuint i = 0; i < pWaypoints->count ( ) - 1; i++ )
		{			
			GameWaypoint*	pWaypoint	  = (GameWaypoint*) pWaypoints->objectAtIndex ( i );
			GameWaypoint*	pWaypointNext = (GameWaypoint*) pWaypoints->objectAtIndex ( i + 1 );
			
			if ( i == 0 )
			{
				// From actor to first waypoint
				ccDrawLine ( tActorPosition, pWaypoint->getPosition ( ) );
				ccDrawLine ( pWaypoint->getPosition ( ), pWaypointNext->getPosition ( ) );
			}
			else
			{
				// From this waypoint to next one
				ccDrawLine ( pWaypoint->getPosition ( ), pWaypointNext->getPosition ( ) );
			}	
		}
	}
}
		
/// Add extra drawing layer 
KDvoid Ch7_Waypoints::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGameNode->addChild ( m_pDrawLayer, 10 );
}

/// Add a new waypoint when you touch the screen 
KDvoid Ch7_Waypoints::tapWithPoint ( const CCPoint& tPoint )
{
	GameWaypoint*	pWaypoint = GameWaypoint::create ( this->convertTouchCoord ( tPoint ), 1.f ); 
	pWaypoint->setPreCallback  ( ObjectCallback::create ( this, callfunc_selector ( Ch7_Waypoints::movingToWaypoint ) ) );
	pWaypoint->setPostCallback ( ObjectCallback::create ( this, callfunc_selector ( Ch7_Waypoints::reachedWaypoint ) ) );
	m_pActor->addWaypoint ( pWaypoint );
}
		
/// WP message callbacks
KDvoid Ch7_Waypoints::reachedWaypoint ( KDvoid )
{
	this->showMessage ( "Moving to WP" );
}

KDvoid Ch7_Waypoints::movingToWaypoint ( KDvoid )
{
	this->showMessage ( "Reached WP" );
}