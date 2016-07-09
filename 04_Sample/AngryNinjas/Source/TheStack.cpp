/* --------------------------------------------------------------------------
 *
 *      File            TheStack.cpp    
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
#include "TheStack.h"
#include "TheLevel.h"
#include "GameData.h"
#include "StackObject.h"
#include "Enemy.h"

TheStack* TheStack::create ( b2World* pWorld )
{
	TheStack*	pRet = new TheStack ( );

	if ( pRet && pRet->initStackWithWorld ( pWorld ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool TheStack::initStackWithWorld ( b2World* pWorld )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_pWorld = pWorld;

	if ( TheLevel::sharedLevel ( )->IS_IPAD )
	{
		m_nStackLocationX = 1400;	// base X starting point for the entire stack on the iPhone (make further tweaks using the  stackAdjustmentX var in the buildLevel function per level
		m_nStackLocationY =  100;	// base Y starting point for the entire stack on the iPhone (make further tweaks using the  stackAdjustmentY var in the buildLevel function per level
	}
	else
	{
		m_nStackLocationX = 900;	// base X starting point for the entire stack on the iPhone (make further tweaks using the  stackAdjustmentX var in the buildLevel function per level
		m_nStackLocationY =  35;	// base Y starting point for the entire stack on the iPhone (make further tweaks using the  stackAdjustmentY var in the buildLevel function per level
	}

	m_nCurrentLevel = GameData::sharedData ( )->getLevel ( ); 
	if ( m_nCurrentLevel % 2 == 0 )
	{
		this->buildLevel2 ( );
	}
	else
	{
		this->buildLevel1 ( );
	}

	return KD_TRUE;
}

KDvoid TheStack::buildLevel1 ( KDvoid )
{
	if ( TheLevel::sharedLevel ( )->IS_IPAD )
	{
		m_nStackAdjustmentX = -350;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationX	= m_nStackLocationX - m_nStackAdjustmentX;

		m_nStackAdjustmentY = 0;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationY	= m_nStackLocationY - m_nStackAdjustmentY;
	}
	else
	{
		m_nStackAdjustmentX = -100;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationX	= m_nStackLocationX - m_nStackAdjustmentX;

		m_nStackAdjustmentY = 0;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationY	= m_nStackLocationY - m_nStackAdjustmentY;
	}

	this->addChild ( StackObject::create ( m_pWorld, ccp (  0 + m_nStackLocationX,  65 + m_nStackLocationY ), "woodShape1"		, KD_FALSE, KD_TRUE, KD_TRUE , KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_FALSE, 100, SmokePuffs ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 95 + m_nStackLocationX,  65 + m_nStackLocationY ), "woodShape1"		, KD_FALSE, KD_TRUE, KD_TRUE , KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_FALSE, 100, SmokePuffs ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 47 + m_nStackLocationX, 145 + m_nStackLocationY ), "woodShape1"		, KD_FALSE, KD_TRUE, KD_TRUE , KD_FALSE, 0.25f, ShapeOfSourceImage	,   0, KD_FALSE, 100, Explosion  ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp (  0 + m_nStackLocationX, 225 + m_nStackLocationY ), "woodShape1"		, KD_FALSE, KD_TRUE, KD_TRUE , KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_FALSE, 100, Explosion  ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 95 + m_nStackLocationX, 225 + m_nStackLocationY ), "woodShape1"		, KD_FALSE, KD_TRUE, KD_TRUE , KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_FALSE, 100, Explosion  ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 50 + m_nStackLocationX, 305 + m_nStackLocationY ), "woodShape1"		, KD_FALSE, KD_TRUE, KD_TRUE , KD_FALSE, 0.25f, ShapeOfSourceImage	,   0, KD_FALSE, 100, SmokePuffs ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp (  0 + m_nStackLocationX, 345 + m_nStackLocationY ), "triangleMedium"	, KD_FALSE, KD_TRUE, KD_FALSE, KD_TRUE , 0.25f, Triangle			,   0, KD_FALSE, 100, SmokePuffs ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 95 + m_nStackLocationX, 345 + m_nStackLocationY ), "triangleMedium"	, KD_FALSE, KD_TRUE, KD_FALSE, KD_TRUE , 0.25f, Triangle			,   0, KD_FALSE, 100, Explosion  ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 50 + m_nStackLocationX, 350 + m_nStackLocationY ), "triangleLarge"	, KD_FALSE, KD_TRUE, KD_FALSE, KD_TRUE , 0.25f, Triangle			, 180, KD_FALSE, 500, SmokePuffs ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 25 + m_nStackLocationX, 394 + m_nStackLocationY ), "triangleSmall"	, KD_FALSE, KD_TRUE, KD_FALSE, KD_FALSE, 0.25f, Triangle			,   0, KD_FALSE, 100, Explosion  ), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 75 + m_nStackLocationX, 394 + m_nStackLocationY ), "triangleSmall"	, KD_FALSE, KD_TRUE, KD_FALSE, KD_FALSE, 0.25f, Triangle			,   0, KD_FALSE, 100, SmokePuffs ), DepthStack );
	
	this->addChild ( Enemy		::create ( m_pWorld, ccp ( 45 + m_nStackLocationX, 200 + m_nStackLocationY ), "mutantPepper"	, KD_TRUE , KD_TRUE, KD_TRUE , 1, KD_TRUE, 10, 1.0f, ShapeOfSourceImageButSlightlySmaller, 10000, SmokePuffs ), DepthStack );			
}

KDvoid TheStack::buildLevel2 ( KDvoid )
{
	if ( TheLevel::sharedLevel ( )->IS_IPAD )
	{
		m_nStackAdjustmentX = 0;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationX	= m_nStackLocationX - m_nStackAdjustmentX;

		m_nStackAdjustmentY = 0;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationY	= m_nStackLocationY - m_nStackAdjustmentY;
	}
	else
	{
		m_nStackAdjustmentX = -100;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationX	= m_nStackLocationX - m_nStackAdjustmentX;

		m_nStackAdjustmentY = 0;				// if you want to further adjust the stack's starting X location then change this value  (iphone)
		m_nStackLocationY	= m_nStackLocationY - m_nStackAdjustmentY;
	}

	// marble
	this->addChild ( StackObject::create ( m_pWorld, ccp (  40 + m_nStackLocationX,  40 + m_nStackLocationY ), "marbleSquare"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_TRUE ,   0, None					), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 190 + m_nStackLocationX,  40 + m_nStackLocationY ), "marbleSquare"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_TRUE ,   0, None					), DepthStack );

	// stone pillars
	this->addChild ( StackObject::create ( m_pWorld, ccp (   9 + m_nStackLocationX, 125 + m_nStackLocationY ), "stonePillar"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_TRUE,   0, None					), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp (  27 + m_nStackLocationX, 220 + m_nStackLocationY ), "stonePillar"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_FALSE, 0.25f, ShapeOfSourceImage	,  90, KD_TRUE,   0, None					), DepthStack );

	this->addChild ( StackObject::create ( m_pWorld, ccp (  45 + m_nStackLocationX, 210 + m_nStackLocationY ), "woodShape4"		, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, ShapeOfSourceImage	,  90, KD_TRUE, 100, Explosion				), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp (  95 + m_nStackLocationX, 210 + m_nStackLocationY ), "woodShape4"		, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, ShapeOfSourceImage	,  90, KD_TRUE, 100, Explosion				), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 145 + m_nStackLocationX, 210 + m_nStackLocationY ), "woodShape4"		, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, ShapeOfSourceImage	,  90, KD_TRUE, 100, Explosion				), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 200 + m_nStackLocationX, 210 + m_nStackLocationY ), "woodShape4"		, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, ShapeOfSourceImage	,  90, KD_TRUE, 100, Explosion				), DepthStack );

	this->addChild ( StackObject::create ( m_pWorld, ccp (  80 + m_nStackLocationX, 250 + m_nStackLocationY ), "stonePillar"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_TRUE , 0.5f , ShapeOfSourceImage	,  90, KD_TRUE,   0, None					), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 180 + m_nStackLocationX, 250 + m_nStackLocationY ), "stonePillar"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_TRUE , 0.5f , ShapeOfSourceImage	,  90, KD_TRUE,   0, None					), DepthStack );

	this->addChild ( StackObject::create ( m_pWorld, ccp (  95 + m_nStackLocationX, 285 + m_nStackLocationY ), "triangleMedium"	, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, Triangle			,   0, KD_TRUE, 100, Explosion				), DepthStack );
	this->addChild ( StackObject::create ( m_pWorld, ccp ( 181 + m_nStackLocationX, 285 + m_nStackLocationY ), "triangleMedium"	, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, Triangle			,   0, KD_TRUE, 100, Explosion				), DepthStack );

	this->addChild ( StackObject::create ( m_pWorld, ccp ( 138 + m_nStackLocationX, 280 + m_nStackLocationY ), "triangleSmall"	, KD_FALSE, KD_TRUE , KD_FALSE, KD_TRUE , 0.25f, Triangle			, 180, KD_TRUE, 500, Explosion				), DepthStack );

	this->addChild ( StackObject::create ( m_pWorld, ccp ( 127 + m_nStackLocationX, 340 + m_nStackLocationY ), "stonePillar"	, KD_FALSE, KD_FALSE, KD_FALSE, KD_TRUE , 0.5f , ShapeOfSourceImage	,   0, KD_TRUE,   0, None					), DepthStack );

	this->addChild ( Enemy		::create ( m_pWorld, ccp ( 118 + m_nStackLocationX,  45 + m_nStackLocationY ), "mutantPepper"	, KD_TRUE , KD_FALSE, KD_TRUE , 3, KD_TRUE, 10, 1.0f, ShapeOfSourceImage				  , 10000, SmokePuffs	), DepthStack );
	this->addChild ( Enemy		::create ( m_pWorld, ccp ( 185 + m_nStackLocationX, 120 + m_nStackLocationY ), "mutantPepper"	, KD_FALSE, KD_TRUE , KD_TRUE , 3, KD_TRUE, 10, 1.0f, ShapeOfSourceImageButSlightlySmaller, 10000, SmokePuffs	), DepthStack );
}
