/* --------------------------------------------------------------------------
 *
 *      File            Ch4_Forces.cpp
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
#include "Ch4_Forces.h"
#include "Libraries/GameMisc.h"

KDbool Ch4_Forces::init ( KDvoid )
{	
	if ( !GameArea2D::init ( ) )
	{
		return KD_FALSE;
	}

	// Set our gravity to 0
	m_pWorld->SetGravity ( b2Vec2 ( 0, 0 ) );
		
	// Level background
	CCSprite*	pBG = CCSprite::create ( "solar_system_bg.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	m_pGameNode->addChild ( pBG, 0 );
	

	// Add Planets
	m_pPlanets = CCDictionary::create ( );
	m_pPlanets->retain ( );

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "solar_system.plist" );

	this->addPlanetWithSpriteFrameName ( "sun.png"		, ccp ( 240, 160 ) );
	this->addPlanetWithSpriteFrameName ( "mercury.png"	, ccp ( 210, 160 ) );
	this->addPlanetWithSpriteFrameName ( "venus.png"	, ccp ( 195, 160 ) );
	this->addPlanetWithSpriteFrameName ( "earth.png"	, ccp ( 170, 160 ) );
	this->addPlanetWithSpriteFrameName ( "mars.png"		, ccp ( 150, 160 ) );
	this->addPlanetWithSpriteFrameName ( "jupiter.png"	, ccp ( 120, 160 ) );
	this->addPlanetWithSpriteFrameName ( "saturn.png"	, ccp (  90, 160 ) );
	this->addPlanetWithSpriteFrameName ( "uranus.png"	, ccp (  60, 160 ) );
	this->addPlanetWithSpriteFrameName ( "neptune.png"	, ccp (  30, 160 ) );

	// Apply initial impulses to planets
	b2Body*		pBody;

	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "mercury.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 0.075f ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "venus.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 0.25f  ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "earth.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 0.45f  ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "mars.png"		) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 0.175f ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "jupiter.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 1.3f   ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "saturn.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 4.5f   ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "uranus.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 0.6f   ), pBody->GetPosition ( ) );
	pBody = ( (GameMisc*) m_pPlanets->objectForKey ( "neptune.png"	) )->getBody ( );	pBody->ApplyLinearImpulse ( b2Vec2 ( 0, 0.8f   ), pBody->GetPosition ( ) );

	// Fast forward about 16 seconds to create realistic orbits from the start
	for ( KDint i = 0; i < 1000; i++ )
	{
		this->step ( 0.016666667f );
	}

	return KD_TRUE;
}

KDvoid Ch4_Forces::onExit ( KDvoid )
{
	m_pPlanets->release ( );

	GameArea2D::onExit ( );
}

/// Every tick apply a force on each planet according to how large it is and how far it is from the sun.
///	This simulates heavenly rotation. 
KDvoid Ch4_Forces::step ( KDfloat fDelta )
{
	GameArea2D::step ( fDelta );
	
	GameMisc*	pSun = (GameMisc*) m_pPlanets->objectForKey ( "sun.png" );
	CCArray*	pKeys = m_pPlanets->allKeys ( );
	CCObject*	pObject;

	b2Vec2		tSunPos = pSun->getBody ( )->GetPosition ( );

	CCARRAY_FOREACH ( pKeys, pObject )
	{
		CCString*	pKey = (CCString*) pObject;

		if ( kdStrcmp ( pKey->getCString ( ), "sun.png" ) )
		{
			GameMisc*	pPlatnet = (GameMisc*) m_pPlanets->objectForKey ( pKey->getCString ( ) );
			b2Body*		pPlatnetBody = pPlatnet->getBody ( );
			b2Vec2		tPlatnetPos  = pPlatnetBody->GetPosition ( );

			CCPoint		tVect = ccp ( tSunPos.x - tPlatnetPos.x, tSunPos.y - tPlatnetPos.y );
			KDfloat		fPlanetSize = kdPowf ( pPlatnet->getSprite ( )->getContentSize ( ).cx, 2 );
			KDfloat		fDist = distanceBetweenPoints ( ccp ( tSunPos.x, tSunPos.y ), ccp ( tPlatnetPos.x, tPlatnetPos.y ) );			
			KDfloat		fMod  = fDist / fPlanetSize * 2000;
			
			pPlatnetBody->ApplyForce ( b2Vec2 ( tVect.x / fMod, tVect.y / fMod ), tPlatnetPos );
		}
	}
}

KDvoid Ch4_Forces::addPlanetWithSpriteFrameName ( const KDchar* szFrameName, const CCPoint& tPosition )
{
	GameMisc*		pPlanet = GameMisc::create ( );
	pPlanet->setGameArea ( this );

	b2BodyDef*		pBodyDef = pPlanet->getBodyDef ( );
	pBodyDef->type = b2_dynamicBody;
	pBodyDef->position.Set ( tPosition.x / PTM_RATIO, tPosition.y / PTM_RATIO );
	pBodyDef->userData = pPlanet;

	b2Body*			pBody = m_pWorld->CreateBody ( pBodyDef );
	pPlanet->setBody ( pBody );

	b2FixtureDef*	pFixtureDef = pPlanet->getFixtureDef ( );
	pFixtureDef->density = 1.0f;
	pFixtureDef->friction = 0.3f;
	pFixtureDef->restitution = 0.6f;
	pFixtureDef->filter.categoryBits = CB_PLANET;
	pFixtureDef->filter.maskBits = CB_NONE;

	CCSprite*		pSprite = CCSprite::createWithSpriteFrameName ( szFrameName );
	pSprite->setPosition ( tPosition );
	pPlanet->setSprite ( pSprite );

	KDfloat			fTextureSize = pSprite->getContentSize ( ).cx;
	KDfloat			fShapeSize = fTextureSize / 3;

	pSprite->setScale ( fShapeSize / fTextureSize * 2 );
	m_pGameNode->addChild ( pSprite, 2 );

	b2CircleShape*	pShape = new b2CircleShape ( );
	pPlanet->setCircleShape ( pShape );
	pShape->m_radius = fShapeSize / PTM_RATIO;

	pFixtureDef->shape = pShape;		
	pBody->CreateFixture ( pFixtureDef );

	m_pPlanets->setObject ( pPlanet, szFrameName );
	
	pBody->SetAngularVelocity ( -0.25f );
}
