/* --------------------------------------------------------------------------
 *
 *      File            Ch1_ParticleEffects.cpp
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
#include "Ch1_ParticleEffects.h"
#include "Libraries/ParticleWaterfall.h"
#include "Libraries/ParticleWaterSprinkler.h"

static const KDchar*	l_aParticleEffectMethods [ ] =
{
	"ParticleExplosion"		,
	"ParticleFire"			,
	"ParticleFireworks"		,
	"ParticleFlower"		,
	"ParticleGalaxy"		,
	"ParticleMeteor"		,
	"ParticleRain"			,
	"ParticleSmoke"			,
	"ParticleSnow"			,
	"ParticleSpiral"		,
	"ParticleSun"			,
	"ParticleWaterfall"		,
	"ParticleWaterSprinkler",
};

KDbool Ch1_ParticleEffects::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	this->initSubMenus ( sizeof ( l_aParticleEffectMethods ) / sizeof ( l_aParticleEffectMethods [ 0 ] ) );

	return KD_TRUE;
}

const KDchar* Ch1_ParticleEffects::runSubAction ( KDint nIndex )
{
	CCNode*		pNode = this->getChildByTag ( TAG_EFFECT_NODE );
	if ( pNode )
	{
		pNode->stopAllActions ( );
		pNode->removeFromParent ( );
	}
	
	switch ( nIndex )
	{
		case  0 :	pNode = CCParticleExplosion		::create ( );	break;
		case  1 :	pNode = CCParticleFire			::create ( );	break;
		case  2 :	pNode = CCParticleFireworks		::create ( );	break;
		case  3 :	pNode = CCParticleFlower		::create ( );	break;
		case  4 :	pNode = CCParticleGalaxy		::create ( );	break;
		case  5 :	pNode = CCParticleMeteor		::create ( );	break;
		case  6 :	pNode = CCParticleRain			::create ( );	break;
		case  7 :	pNode = CCParticleSmoke			::create ( );	break;
		case  8 :	pNode = CCParticleSnow			::create ( );	break;
		case  9 :	pNode = CCParticleSpiral		::create ( );	break;
		case 10 :	pNode = CCParticleSun			::create ( );	break;
		case 11 :	pNode = ParticleWaterfall		::create ( );	break;
		case 12 :	pNode = ParticleWaterSprinkler	::create ( );	break;
	}

	this->addChild ( pNode, 1, TAG_EFFECT_NODE );

	pNode->setPosition ( ccp ( 240, 160 ) );
		
	if ( !kdStrcmp ( l_aParticleEffectMethods [ nIndex ], "ParticleWaterfall" ) ||
		 !kdStrcmp ( l_aParticleEffectMethods [ nIndex ], "ParticleRain" ) ||
		 !kdStrcmp ( l_aParticleEffectMethods [ nIndex ], "ParticleSnow" ) )
	{
		pNode->setPosition ( ccp ( 240, 280 ) );
	}

	return l_aParticleEffectMethods [ nIndex ];
}