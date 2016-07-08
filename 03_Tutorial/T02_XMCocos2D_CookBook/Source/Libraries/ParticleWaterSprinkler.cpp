/* --------------------------------------------------------------------------
 *
 *      File            ParticleWaterSprinkler.cpp
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
#include "ParticleWaterSprinkler.h"

ParticleWaterSprinkler* ParticleWaterSprinkler::create ( KDuint uNumberOfParticles )
{
	ParticleWaterSprinkler*		pRet = new ParticleWaterSprinkler ( );

	if ( pRet && pRet->initWithTotalParticles ( uNumberOfParticles ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}
KDbool ParticleWaterSprinkler::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( !CCParticleFireworks::initWithTotalParticles ( uNumberOfParticles ) )
	{
		return KD_FALSE;
	}

	// color of particles
	m_tStartColor.r		= 0.4f;
	m_tStartColor.g		= 0.4f;
	m_tStartColor.b		= 1.0f;
	m_tStartColor.a		= 0.6f;

	m_tStartColorVar.r	= 0.0f;
	m_tStartColorVar.g	= 0.0f;
	m_tStartColorVar.b	= 0.0f;
	m_tStartColorVar.a	= 0.0f;

	m_tEndColor.r		= 0.5f;
	m_tEndColor.g		= 0.5f;
	m_tEndColor.b		= 0.5f;
	m_tEndColor.a		= 0.0f;

	m_tEndColorVar.r	= 0.0f;
	m_tEndColorVar.g	= 0.0f;
	m_tEndColorVar.b	= 0.0f;
	m_tEndColorVar.a	= 0.0f;

	// additive
	this->setBlendAdditive ( KD_FALSE );

	return KD_TRUE;	
}
