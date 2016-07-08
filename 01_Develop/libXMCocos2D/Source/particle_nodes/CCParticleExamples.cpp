/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleExamples.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "particle_nodes/CCParticleExamples.h"
#include "support/CCDirector.h"
#include "support/CCPointExtension.h"
#include "support/CCImage.h"
#include "textures/CCTextureCache.h"
#include "firePngData.h"

NS_CC_BEGIN
    
static CCTexture2D* getDefaultTexture ( KDvoid )
{
    CCTexture2D*  pTexture = KD_NULL;
    do 
    {
        const KDchar*  szKey = "__firePngData";

        pTexture = CCTextureCache::sharedTextureCache ( )->textureForKey ( szKey );
        CC_BREAK_IF ( pTexture != KD_NULL );

        CCImage*  pImage = new CCImage ( );
		if ( pImage && pImage->initWithData ( (KDvoid*) __firePngData, sizeof ( __firePngData ) ) )
		{
			pImage->autorelease ( );
			pTexture = CCTextureCache::sharedTextureCache ( )->addCCImage ( pImage, szKey );
		}
		else
		{
			CC_SAFE_DELETE ( pImage );
		}      
    } while ( 0 );

    return pTexture;
}

//
// ParticleFire
//
CCParticleFire* CCParticleFire::create ( KDuint uNumberOfParticles )
{
    CCParticleFire*  pRet = new CCParticleFire ( );
    
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

KDbool CCParticleFire::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		this->setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		this->setGravity ( ccp ( 0, 0 ) );

		// Gravity Mode: radial acceleration
		this->setRadialAccel ( 0 );
		this->setRadialAccelVar ( 0 );

		// Gravity Mode: speed of particles
		this->setSpeed ( 60 );
		this->setSpeedVar ( 20 );		

		// starting angle
		m_fAngle = 90;
		m_fAngleVar = 10;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccp ( tWinSize.cx / 2, 60 ) );
		this->setPosVar ( ccp ( 40, 20 ) );

		// life of particles
		m_fLife = 3;
		m_fLifeVar = 0.25f;

		// size, in pixels
		m_fStartSize = 54.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per frame
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.76f;
		m_tStartColor.g = 0.25f;
		m_tStartColor.b = 0.12f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_TRUE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

//
// ParticleFireworks
//

CCParticleFireworks* CCParticleFireworks::create ( KDuint uNumberOfParticles )
{
    CCParticleFireworks*  pRet = new CCParticleFireworks ( );
    
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

KDbool CCParticleFireworks::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration= kCCParticleDurationInfinity;

		// Gravity Mode
		this->setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		this->setGravity ( ccp ( 0, -90 ) );

		// Gravity Mode:  radial
		this->setRadialAccel ( 0 );
		this->setRadialAccelVar ( 0 );

		//  Gravity Mode: speed of particles
		this->setSpeed ( 180 );
		this->setSpeedVar ( 50 );

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );

		// angle
		this->m_fAngle= 90;
		this->m_fAngleVar = 20;

		// life of particles
		this->m_fLife = 3.5f;
		this->m_fLifeVar = 1;

		// emits per frame
		this->m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.5f;
		m_tStartColor.g = 0.5f;
		m_tStartColor.b = 0.5f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.5f;
		m_tStartColorVar.g = 0.5f;
		m_tStartColorVar.b = 0.5f;
		m_tStartColorVar.a = 0.1f;
		m_tEndColor.r = 0.1f;
		m_tEndColor.g = 0.1f;
		m_tEndColor.b = 0.1f;
		m_tEndColor.a = 0.2f;
		m_tEndColorVar.r = 0.1f;
		m_tEndColorVar.g = 0.1f;
		m_tEndColorVar.b = 0.1f;
		m_tEndColorVar.a = 0.2f;

		// size, in pixels
		m_fStartSize = 8.0f;
		m_fStartSizeVar = 2.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_FALSE );
		return KD_TRUE;
	}

	return KD_FALSE;
}

//
// ParticleSun
//

CCParticleSun* CCParticleSun::create ( KDuint uNumberOfParticles )
{
    CCParticleSun*  pRet = new CCParticleSun ( );
    
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

KDbool CCParticleSun::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// additive
		this->setBlendAdditive ( KD_TRUE );

		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, 0 ) );

		// Gravity mode: radial acceleration
		setRadialAccel ( 0 );
		setRadialAccelVar ( 0 );

		// Gravity mode: speed of particles
		setSpeed ( 20 );
		setSpeedVar ( 5 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 360;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );
		setPosVar ( CCPointZero );

		// life of particles
		m_fLife = 1;
		m_fLifeVar = 0.5f;

		// size, in pixels
		m_fStartSize = 30.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per seconds
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.76f;
		m_tStartColor.g = 0.25f;
		m_tStartColor.b = 0.12f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		return KD_TRUE;
	}

	return KD_FALSE;
}

//
// ParticleGalaxy
//

CCParticleGalaxy* CCParticleGalaxy::create ( KDuint uNumberOfParticles )
{
    CCParticleGalaxy*  pRet = new CCParticleGalaxy ( );
    
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

KDbool CCParticleGalaxy::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, 0 ) );

		// Gravity Mode: speed of particles
		setSpeed ( 60 );
		setSpeedVar ( 10 );

		// Gravity Mode: radial
		setRadialAccel ( -80 );
		setRadialAccelVar ( 0 );

		// Gravity Mode: tangential
		setTangentialAccel ( 80 );
		setTangentialAccelVar ( 0 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 360;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );
		setPosVar ( CCPointZero );

		// life of particles
		m_fLife = 4;
		m_fLifeVar = 1;

		// size, in pixels
		m_fStartSize = 37.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.12f;
		m_tStartColor.g = 0.25f;
		m_tStartColor.b = 0.76f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_TRUE );
		return KD_TRUE;
	}

	return KD_FALSE;
}

//
// ParticleFlower
//

CCParticleFlower* CCParticleFlower::create ( KDuint uNumberOfParticles )
{
    CCParticleFlower*  pRet = new CCParticleFlower ( );
    
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

KDbool CCParticleFlower::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, 0 ) );

		// Gravity Mode: speed of particles
		setSpeed ( 80 );
		setSpeedVar ( 10 );

		// Gravity Mode: radial
		setRadialAccel ( -60 );
		setRadialAccelVar ( 0 );

		// Gravity Mode: tangential
		setTangentialAccel ( 15 );
		setTangentialAccelVar ( 0 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 360;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );
		setPosVar ( CCPointZero );

		// life of particles
		m_fLife = 4;
		m_fLifeVar = 1;

		// size, in pixels
		m_fStartSize = 30.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.50f;
		m_tStartColor.g = 0.50f;
		m_tStartColor.b = 0.50f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.5f;
		m_tStartColorVar.g = 0.5f;
		m_tStartColorVar.b = 0.5f;
		m_tStartColorVar.a = 0.5f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_TRUE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

//
// ParticleMeteor
//

CCParticleMeteor* CCParticleMeteor::create ( KDuint uNumberOfParticles )
{
    CCParticleMeteor*  pRet = new CCParticleMeteor ( );
    
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

KDbool CCParticleMeteor::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( -200, 200 ) );

		// Gravity Mode: speed of particles
		setSpeed ( 15 );
		setSpeedVar ( 5 );

		// Gravity Mode: radial
		setRadialAccel ( 0 );
		setRadialAccelVar ( 0 );

		// Gravity Mode: tangential
		setTangentialAccel ( 0 );
		setTangentialAccelVar ( 0 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 360;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );
		setPosVar ( CCPointZero );

		// life of particles
		m_fLife = 2;
		m_fLifeVar = 1;

		// size, in pixels
		m_fStartSize = 60.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = m_uTotalParticles / m_fLife;
 
		// color of particles
		m_tStartColor.r = 0.2f;
		m_tStartColor.g = 0.4f;
		m_tStartColor.b = 0.7f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.2f;
		m_tStartColorVar.a = 0.1f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_TRUE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

//
// ParticleSpiral
//

CCParticleSpiral* CCParticleSpiral::create ( KDuint uNumberOfParticles )
{
    CCParticleSpiral*  pRet = new CCParticleSpiral ( );
    
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

KDbool CCParticleSpiral::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) ) 
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, 0 ) );

		// Gravity Mode: speed of particles
		setSpeed ( 150 );
		setSpeedVar ( 0 );

		// Gravity Mode: radial
		setRadialAccel ( -380 );
		setRadialAccelVar ( 0 );

		// Gravity Mode: tangential
		setTangentialAccel ( 45 );
		setTangentialAccelVar ( 0 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 0;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );
		setPosVar ( CCPointZero );

		// life of particles
		m_fLife = 12;
		m_fLifeVar = 0;

		// size, in pixels
		m_fStartSize = 20.0f;
		m_fStartSizeVar = 0.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.5f;
		m_tStartColor.g = 0.5f;
		m_tStartColor.b = 0.5f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.5f;
		m_tStartColorVar.g = 0.5f;
		m_tStartColorVar.b = 0.5f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.5f;
		m_tEndColor.g = 0.5f;
		m_tEndColor.b = 0.5f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.5f;
		m_tEndColorVar.g = 0.5f;
		m_tEndColorVar.b = 0.5f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_FALSE );
		return KD_TRUE;
	}

	return KD_FALSE;
}

//
// ParticleExplosion
//

CCParticleExplosion* CCParticleExplosion::create ( KDuint uNumberOfParticles )
{
    CCParticleExplosion*  pRet = new CCParticleExplosion ( );
    
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

KDbool CCParticleExplosion::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) ) 
	{
		// duration
		m_fDuration = 0.1f;

		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, 0 ) );

		// Gravity Mode: speed of particles
		setSpeed ( 70 );
		setSpeedVar ( 40 );

		// Gravity Mode: radial
		setRadialAccel ( 0 );
		setRadialAccelVar ( 0 );

		// Gravity Mode: tangential
		setTangentialAccel ( 0 );
		setTangentialAccelVar ( 0 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 360;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccpMid ( tWinSize ) );
		setPosVar ( CCPointZero );

		// life of particles
		m_fLife = 5.0f;
		m_fLifeVar = 2;

		// size, in pixels
		m_fStartSize = 15.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = m_uTotalParticles / m_fDuration;

		// color of particles
		m_tStartColor.r = 0.7f;
		m_tStartColor.g = 0.1f;
		m_tStartColor.b = 0.2f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.5f;
		m_tStartColorVar.g = 0.5f;
		m_tStartColorVar.b = 0.5f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.5f;
		m_tEndColor.g = 0.5f;
		m_tEndColor.b = 0.5f;
		m_tEndColor.a = 0.0f;
		m_tEndColorVar.r = 0.5f;
		m_tEndColorVar.g = 0.5f;
		m_tEndColorVar.b = 0.5f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_FALSE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

//
// ParticleSmoke
//

CCParticleSmoke* CCParticleSmoke::create ( KDuint uNumberOfParticles )
{
    CCParticleSmoke*  pRet = new CCParticleSmoke ( );
    
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

KDbool CCParticleSmoke::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Emitter mode: Gravity Mode
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, 0 ) );

		// Gravity Mode: radial acceleration
		setRadialAccel ( 0 );
		setRadialAccelVar ( 0 );

		// Gravity Mode: speed of particles
		setSpeed ( 25 );
		setSpeedVar ( 10 );

		// angle
		m_fAngle = 90;
		m_fAngleVar = 5;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccp ( tWinSize.cx / 2, 0 ) );
		setPosVar ( ccp ( 20, 0 ) );

		// life of particles
		m_fLife = 4;
		m_fLifeVar = 1;

		// size, in pixels
		m_fStartSize = 60.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per frame
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		// color of particles
		m_tStartColor.r = 0.8f;
		m_tStartColor.g = 0.8f;
		m_tStartColor.b = 0.8f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.02f;
		m_tStartColorVar.g = 0.02f;
		m_tStartColorVar.b = 0.02f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_FALSE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

//
// CCParticleSnow
//

CCParticleSnow* CCParticleSnow::create ( KDuint uNumberOfParticles )
{
    CCParticleSnow*  pRet = new CCParticleSnow ( );
    
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

KDbool CCParticleSnow::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) ) 
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// set gravity mode.
		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 0, -1 ) );

		// Gravity Mode: speed of particles
		setSpeed ( 5 );
		setSpeedVar ( 1 );

		// Gravity Mode: radial
		setRadialAccel ( 0 );
		setRadialAccelVar ( 1 );

		// Gravity mode: tangential
		setTangentialAccel ( 0 );
		setTangentialAccelVar ( 1 );

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy + 10 ) );
		setPosVar ( ccp ( tWinSize.cx / 2, 0 ) );

		// angle
		m_fAngle = -90;
		m_fAngleVar = 5;

		// life of particles
		m_fLife = 45;
		m_fLifeVar = 15;

		// size, in pixels
		m_fStartSize = 10.0f;
		m_fStartSizeVar = 5.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = 10;

		// color of particles
		m_tStartColor.r = 1.0f;
		m_tStartColor.g = 1.0f;
		m_tStartColor.b = 1.0f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 1.0f;
		m_tEndColor.g = 1.0f;
		m_tEndColor.b = 1.0f;
		m_tEndColor.a = 0.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_FALSE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

//
// CCParticleRain
//

CCParticleRain* CCParticleRain::create ( KDuint uNumberOfParticles )
{
    CCParticleRain*  pRet = new CCParticleRain ( );
    
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

KDbool CCParticleRain::initWithTotalParticles ( KDuint uNumberOfParticles )
{
	if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
	{
		// duration
		m_fDuration = kCCParticleDurationInfinity;

		setEmitterMode ( kCCParticleModeGravity );

		// Gravity Mode: gravity
		setGravity ( ccp ( 10, -10 ) );

		// Gravity Mode: radial
		setRadialAccel ( 0 );
		setRadialAccelVar ( 1 );

		// Gravity Mode: tangential
		setTangentialAccel ( 0 );
		setTangentialAccelVar ( 1 );

		// Gravity Mode: speed of particles
		setSpeed ( 130 );
		setSpeedVar ( 30 );

		// angle
		m_fAngle = -90;
		m_fAngleVar = 5;

		// emitter position
		const CCSize&  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
		this->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy ) );
		setPosVar ( ccp( tWinSize.cx / 2, 0 ) );

		// life of particles
		m_fLife = 4.5f;
		m_fLifeVar = 0;

		// size, in pixels
		m_fStartSize = 4.0f;
		m_fStartSizeVar = 2.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per second
		m_fEmissionRate = 20;

		// color of particles
		m_tStartColor.r = 0.7f;
		m_tStartColor.g = 0.8f;
		m_tStartColor.b = 1.0f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.7f;
		m_tEndColor.g = 0.8f;
		m_tEndColor.b = 1.0f;
		m_tEndColor.a = 0.5f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D*  pTexture = getDefaultTexture ( );
        if ( pTexture )
        {
            setTexture ( pTexture );
        }

		// additive
		this->setBlendAdditive ( KD_FALSE );
		return KD_TRUE;
	}
	return KD_FALSE;
}

NS_CC_END