/* --------------------------------------------------------------------------
 *
 *      File            TestParticle.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestParticle.h"

enum 
{
    kTagParticleCount = 1,
};

TestParticle* TestParticle::create ( KDvoid )
{
	TestParticle*  pLayer = KD_NULL;
	
	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new ParticleReorder						( ); break;
		case  1 : pLayer = new ParticleBatchHybrid					( ); break;
		case  2 : pLayer = new ParticleBatchMultipleEmitters		( ); break;
		case  3 : pLayer = new DemoFlower							( ); break;
		case  4 : pLayer = new DemoGalaxy       					( ); break;
		case  5 : pLayer = new DemoFirework     					( ); break;
		case  6 : pLayer = new DemoSpiral       					( ); break;
		case  7 : pLayer = new DemoSun          					( ); break;
		case  8 : pLayer = new DemoMeteor       					( ); break;
		case  9 : pLayer = new DemoFire         					( ); break;
		case 10 : pLayer = new DemoSmoke        					( ); break;
		case 11 : pLayer = new DemoExplosion    					( ); break;
		case 12 : pLayer = new DemoSnow         					( ); break;
		case 13 : pLayer = new DemoRain         					( ); break;
		case 14 : pLayer = new DemoBigFlower    					( ); break;
		case 15 : pLayer = new DemoRotFlower    					( ); break;
		case 16 : pLayer = new DemoModernArt    					( ); break;
		case 17 : pLayer = new DemoRing         					( ); break;
		case 18 : pLayer = new ParallaxParticle 					( ); break;
		case 19 : pLayer = new DemoParticleFromFile ( "BoilingFoam"   ); break;
		case 20 : pLayer = new DemoParticleFromFile ( "BurstPipe"	  ); break;
		case 21 : pLayer = new DemoParticleFromFile ( "Comet"		  ); break;
		case 22 : pLayer = new DemoParticleFromFile ( "Debian"		  ); break;
		case 23 : pLayer = new DemoParticleFromFile ( "ExplodingRing" ); break;
		case 24 : pLayer = new DemoParticleFromFile ( "LavaFlow"	  ); break;
		case 25 : pLayer = new DemoParticleFromFile ( "SpinningPeas"  ); break;
		case 26 : pLayer = new DemoParticleFromFile ( "SpookyPeas"	  ); break;
		case 27 : pLayer = new DemoParticleFromFile ( "Upsidedown"	  ); break;
		case 28 : pLayer = new DemoParticleFromFile ( "Flower"		  ); break;
		case 29 : pLayer = new DemoParticleFromFile ( "Spiral"		  ); break;
		case 30 : pLayer = new DemoParticleFromFile ( "Galaxy"		  ); break;
		case 31 : pLayer = new DemoParticleFromFile ( "Phoenix"		  ); break;
		case 32 : pLayer = new DemoParticleFromFile ( "lines"		  ); break;
		case 33 : pLayer = new RadiusMode1 							( ); break;
		case 34 : pLayer = new RadiusMode2							( ); break;		

		// v1.1 tests
		case 35 : pLayer = new MultipleParticleSystems				( ); break;
		case 36 : pLayer = new MultipleParticleSystemsBatched		( ); break;
		case 37 : pLayer = new AddAndDeleteParticleSystems			( ); break;
		case 38 : pLayer = new ReorderParticleSystems				( ); break;
		case 39 : pLayer = new PremultipliedAlphaTest				( ); break;
		case 40 : pLayer = new PremultipliedAlphaTest2				( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestParticle::count ( KDvoid )
{
	return 41;
}

KDbool TestParticle::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemFont::setFontSize ( 24 );

	CCMenuItemToggle*  pToggle = CCMenuItemToggle::createWithTarget
	(
		this, 
		menu_selector ( TestParticle::toggleCallback ), 
		CCMenuItemFont::create ( "Free Movement" ),
		CCMenuItemFont::create ( "Relative Movement" ),
		CCMenuItemFont::create ( "Grouped Movement" ),
		KD_NULL 
	);
	pToggle->setColor ( ccc3 ( 16, 255, 255 ) );
	pToggle->setPosition ( ccp ( 20, 100 ) );
	pToggle->setAnchorPoint ( ccp ( 0, 0 ) );
	this->addChild ( CCMenu::createWithItem ( pToggle ), 100 );	

	// moving background
	m_pBackground = CCSprite::create ( "Images/background3.png" );
	m_pBackground->setPosition ( ccpMid ( tLyrSize ) );
	
	CCActionInterval*   pMove = CCMoveBy::create ( 4, ccp ( 300, 0 ) );
	m_pBackground->runAction ( CCRepeatForever::create ( CCSequence::create ( pMove, pMove->reverse ( ), KD_NULL ) ) );
	this->addChild ( m_pBackground, 5 );
	
    CCLabelAtlas*  pLabelAtlas = CCLabelAtlas::create ( "0000", "xm_root/fps_images.png", 12, 32, '.' );  
	pLabelAtlas->setPosition ( ccp ( tLyrSize.cx - 80, 60 ) );
	pLabelAtlas->setScale ( 1.2f );
	this->addChild ( pLabelAtlas, 100, kTagParticleCount );

	this->setTouchEnabled ( KD_TRUE );
	this->scheduleUpdate ( );

	m_pEmitter = KD_NULL;

	return KD_TRUE;
}

KDvoid TestParticle::toggleCallback ( CCObject* pSender )
{
	if ( m_pEmitter )
	{
		tCCPositionType  eType;

		switch ( m_pEmitter->getPositionType ( ) )
		{
			case kCCPositionTypeGrouped  : eType = kCCPositionTypeFree;     break;
			case kCCPositionTypeFree     : eType = kCCPositionTypeRelative; break;
			case kCCPositionTypeRelative : eType = kCCPositionTypeGrouped;  break;
		}

		m_pEmitter->setPositionType ( eType );	
	}
}

KDvoid TestParticle::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	this->ccTouchesEnded ( pTouches, pEvent );
}

KDvoid TestParticle::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	this->ccTouchesEnded ( pTouches, pEvent );
}

KDvoid TestParticle::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*  pTouch = (CCTouch*) pTouches->anyObject ( );

	if ( m_pEmitter )
	{
        if ( m_pBackground )
        {
            m_pEmitter->setPosition ( m_pBackground->convertTouchToNodeSpace ( pTouch ) );
        }
        else
        {
            m_pEmitter->setPosition ( this->convertTouchToNodeSpace ( pTouch ) );
        }
	}
}

KDvoid TestParticle::update ( KDfloat fDelta )
{
	if ( m_pEmitter )
	{
		CCLabelAtlas*  pAtlas = (CCLabelAtlas*) getChildByTag ( kTagParticleCount );

		pAtlas->setString ( ccszf ( "%04d", m_pEmitter->getParticleCount ( ) ) );
	}
}

KDvoid TestParticle::setEmitterPosition ( KDvoid )
{
	if ( m_pEmitter )
	{
		const CCSize&  tSize = m_pBackground ? m_pBackground->getContentSize ( ) : this->getContentSize ( );

		m_pEmitter->setPosition ( ccpMid ( tSize ) );
	}
}

//------------------------------------------------------------------
//
// DemoFirework
//
//------------------------------------------------------------------
KDvoid DemoFirework::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleFireworks::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars.png" ) );
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoFirework::title ( KDvoid )
{
	return "ParticleFireworks";
}

//------------------------------------------------------------------
//
// DemoFire
//
//------------------------------------------------------------------
KDvoid DemoFire::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleFire::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );

	CCPoint  p = m_pEmitter->getPosition ( );
	m_pEmitter->setPosition ( ccp ( p.x, 100 ) );
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoFire::title ( KDvoid )
{
	return "ParticleFire";
}

//------------------------------------------------------------------
//
// DemoSun
//
//------------------------------------------------------------------
KDvoid DemoSun::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleSun::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoSun::title ( KDvoid )
{
	return "ParticleSun";
}

//------------------------------------------------------------------
//
// DemoGalaxy
//
//------------------------------------------------------------------
KDvoid DemoGalaxy::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleGalaxy::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( ); 
}

const KDchar* DemoGalaxy::title ( KDvoid )
{
	return "ParticleGalaxy";
}

//------------------------------------------------------------------
//
// DemoFlower
//
//------------------------------------------------------------------
KDvoid DemoFlower::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleFlower::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars.png" ) );
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoFlower::title ( KDvoid )
{
	return "ParticleFlower";
}

//------------------------------------------------------------------
//
// DemoBigFlower
//
//------------------------------------------------------------------
KDvoid DemoBigFlower::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = new CCParticleSystemQuad ( );
	m_pEmitter->autorelease ( );

	m_pEmitter->initWithTotalParticles ( 50 );		
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars.png" ) );
	m_pEmitter->setDuration ( -1 );
		
	// gravity
	m_pEmitter->setGravity ( CCPointZero );
	
	// angle
	m_pEmitter->setAngle ( 90 );
	m_pEmitter->setAngleVar ( 360 );
	
	// speed of particles
	m_pEmitter->setSpeed ( 160 );
	m_pEmitter->setSpeedVar ( 20 );
	
	// radial
	m_pEmitter->setRadialAccel ( -120 );
	m_pEmitter->setRadialAccelVar ( 0 );
	
	// tagential
	m_pEmitter->setTangentialAccel ( 30 );
	m_pEmitter->setTangentialAccelVar ( 0 );
	
	// emitter position
	m_pEmitter->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	m_pEmitter->setPosVar ( CCPointZero );
	
	// life of particles
	m_pEmitter->setLife ( 4 );
	m_pEmitter->setLifeVar ( 1 );
	
	// spin of particles
	m_pEmitter->setStartSpin ( 0 );
	m_pEmitter->setStartSizeVar ( 0 );
	m_pEmitter->setEndSpin ( 0 );
	m_pEmitter->setEndSpinVar ( 0 );
	
	// color of particles
	ccColor4F  startColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColor ( startColor );
	
	ccColor4F  startColorVar = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColorVar ( startColorVar );
	
	ccColor4F  endColor = { 0.1f, 0.1f, 0.1f, 0.2f };
	m_pEmitter->setEndColor ( endColor );

	ccColor4F endColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };
	m_pEmitter->setEndColorVar ( endColorVar );
	
	// size, in pixels
	m_pEmitter->setStartSize ( 80.0f );
	m_pEmitter->setStartSizeVar ( 40.0f );
	m_pEmitter->setEndSize ( kParticleStartSizeEqualToEndSize );

	// emits per second
	m_pEmitter->setEmissionRate ( m_pEmitter->getTotalParticles ( ) / m_pEmitter->getLife ( ) );
	
	// additive
	m_pEmitter->setBlendAdditive ( KD_TRUE );

	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoBigFlower::title ( KDvoid )
{
	return "ParticleBigFlower";
}

//------------------------------------------------------------------
//
// DemoRotFlower
//
//------------------------------------------------------------------
KDvoid DemoRotFlower::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = new CCParticleSystemQuad ( );
	m_pEmitter->autorelease ( );

	m_pEmitter->initWithTotalParticles ( 300 );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars2.png" ) );
	
	// duration
	m_pEmitter->setDuration ( -1 );
	
	// gravity
	m_pEmitter->setGravity ( CCPointZero );
	
	// angle
	m_pEmitter->setAngle ( 90 );
	m_pEmitter->setAngleVar ( 360 );
	 
	// speed of particles
	m_pEmitter->setSpeed ( 160 );
	m_pEmitter->setSpeedVar ( 20 );
	
	// radial
	m_pEmitter->setRadialAccel ( -120 );
	m_pEmitter->setRadialAccelVar ( 0 );
	
	// tagential
	m_pEmitter->setTangentialAccel ( 30 );
	m_pEmitter->setTangentialAccelVar ( 0 );
	
	// emitter position
	m_pEmitter->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	m_pEmitter->setPosVar ( CCPointZero );
	
	// life of particles
	m_pEmitter->setLife ( 3 );
	m_pEmitter->setLifeVar ( 1 );

	// spin of particles
	m_pEmitter->setStartSpin ( 0 );
	m_pEmitter->setStartSpinVar ( 0 );
	m_pEmitter->setEndSpin ( 0 );
	m_pEmitter->setEndSpinVar ( 2000 );
	
	// color of particles
	ccColor4F  startColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColor ( startColor );
	
	ccColor4F  startColorVar = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColorVar ( startColorVar );
	
	ccColor4F  endColor = { 0.1f, 0.1f, 0.1f, 0.2f };
	m_pEmitter->setEndColor ( endColor );
	
	ccColor4F  endColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };	
	m_pEmitter->setEndColorVar ( endColorVar );

	// size, in pixels
	m_pEmitter->setStartSize ( 30.0f );
	m_pEmitter->setStartSizeVar ( 00.0f );
	m_pEmitter->setEndSize ( kParticleStartSizeEqualToEndSize );
	
	// emits per second
	m_pEmitter->setEmissionRate ( m_pEmitter->getTotalParticles ( ) / m_pEmitter->getLife ( ) );

	// additive
	m_pEmitter->setBlendAdditive ( KD_FALSE );
	
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoRotFlower::title ( KDvoid )
{
	return "ParticleRotFlower";
}

//------------------------------------------------------------------
//
// DemoMeteor
//
//------------------------------------------------------------------
KDvoid DemoMeteor::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleMeteor::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pBackground->addChild ( m_pEmitter, 10 );	

	setEmitterPosition ( );
}

const KDchar* DemoMeteor::title ( KDvoid )
{
	return "ParticleMeteor";
}

//------------------------------------------------------------------
//
// DemoSpiral
//
//------------------------------------------------------------------
KDvoid DemoSpiral::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleSpiral::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	m_pBackground->addChild ( m_pEmitter, 10 );		
	
	setEmitterPosition ( );
}

const KDchar* DemoSpiral::title ( KDvoid )
{
	return "ParticleSpiral";
}

//------------------------------------------------------------------
//
// DemoExplosion
//
//------------------------------------------------------------------
KDvoid DemoExplosion::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleExplosion::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars.png" ) );
	m_pEmitter->setAutoRemoveOnFinish ( KD_TRUE );
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoExplosion::title ( KDvoid )
{
	return "ParticleExplosion";
}

//------------------------------------------------------------------
//
// DemoSmoke
//
//------------------------------------------------------------------
KDvoid DemoSmoke::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleSmoke::create ( );
	m_pBackground->addChild ( m_pEmitter, 10 );
		
	CCPoint  p = m_pEmitter->getPosition ( );
	m_pEmitter->setPosition ( ccp ( p.x, 100 ) );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
		
	setEmitterPosition ( );
}

const KDchar* DemoSmoke::title ( KDvoid )
{
	return "ParticleSmoke";
}

//------------------------------------------------------------------
//
// DemoSnow
//
//------------------------------------------------------------------
KDvoid DemoSnow::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );
 
	m_pEmitter = CCParticleSnow::create ( );
	
	CCPoint  p = m_pEmitter->getPosition ( );
	m_pEmitter->setPosition ( ccp ( p.x, p.y - 110 ) );
	m_pEmitter->setLife ( 3 );
	m_pEmitter->setLifeVar ( 1 );

	// gravity
	m_pEmitter->setGravity ( ccp ( 0, -10 ) );
		
	// speed of particles
	m_pEmitter->setSpeed ( 130 );
	m_pEmitter->setSpeedVar ( 30 );

	ccColor4F  startColor = m_pEmitter->getStartColor ( );
	startColor.r = 0.9f;
	startColor.g = 0.9f;
	startColor.b = 0.9f;
	m_pEmitter->setStartColor ( startColor );
	
	ccColor4F  startColorVar = m_pEmitter->getStartColorVar ( );
	startColorVar.b = 0.1f;
	m_pEmitter->setStartColorVar ( startColorVar );
	
	m_pEmitter->setEmissionRate ( m_pEmitter->getTotalParticles ( ) / m_pEmitter->getLife ( ) );
	
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/snow.png" ) );
	
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoSnow::title ( KDvoid )
{
	return "ParticleSnow";
}

//------------------------------------------------------------------
//
// DemoRain
//
//------------------------------------------------------------------
KDvoid DemoRain::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleRain::create ( );
		
	CCPoint  p = m_pEmitter->getPosition ( );
	m_pEmitter->setPosition ( ccp ( p.x, p.y - 100 ) );
	m_pEmitter->setLife ( 4 );
	
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoRain::title ( KDvoid )
{
	return "ParticleRain";
}

//------------------------------------------------------------------
//
// DemoModernArt
//
//------------------------------------------------------------------
KDvoid DemoModernArt::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = new CCParticleSystemQuad ( );
	m_pEmitter->autorelease ( );

	m_pEmitter->initWithTotalParticles ( 1000 );

	// duration
	m_pEmitter->setDuration ( -1 );
	
	// gravity
	m_pEmitter->setGravity ( CCPointZero );
	
	// angle
	m_pEmitter->setAngle ( 0 );
	m_pEmitter->setAngleVar ( 360 );
	
	// radial
	m_pEmitter->setRadialAccel ( 70 );
	m_pEmitter->setRadialAccelVar ( 10 );
	
	// tagential
	m_pEmitter->setTangentialAccel ( 80 );
	m_pEmitter->setTangentialAccelVar ( 0 );
	
	// speed of particles
	m_pEmitter->setSpeed ( 50 );
	m_pEmitter->setSpeedVar ( 10 );
	
	// emitter position
	m_pEmitter->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	m_pEmitter->setPosVar ( CCPointZero );
	
	// life of particles
	m_pEmitter->setLife ( 2.0f );
	m_pEmitter->setLifeVar ( 0.3f );
	
	// emits per frame
	m_pEmitter->setEmissionRate ( m_pEmitter->getTotalParticles ( ) / m_pEmitter->getLife ( ) );
	
	// color of particles
	ccColor4F  startColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColor ( startColor );
	
	ccColor4F  startColorVar = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColorVar ( startColorVar );
	
	ccColor4F  endColor = { 0.1f, 0.1f, 0.1f, 0.2f };
	m_pEmitter->setEndColor ( endColor );
	
	ccColor4F  endColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };	
	m_pEmitter->setEndColorVar ( endColorVar );
	
	// size, in pixels
	m_pEmitter->setStartSize ( 1.0f );
	m_pEmitter->setStartSizeVar ( 1.0f );
	m_pEmitter->setEndSize ( 32.0f );
	m_pEmitter->setEndSizeVar ( 8.0f );
	
	// texture
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	
	// additive
	m_pEmitter->setBlendAdditive ( KD_FALSE );
	
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoModernArt::title ( )
{
	return "Varying size";
}

//------------------------------------------------------------------
//
// DemoRing 
//
//------------------------------------------------------------------
KDvoid DemoRing::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );

	m_pEmitter = CCParticleFlower::create ( );

	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars.png" ) );
	m_pEmitter->setLifeVar ( 0 );
	m_pEmitter->setLife ( 10 );
	m_pEmitter->setSpeed ( 100 );
	m_pEmitter->setSpeedVar ( 0 );
	m_pEmitter->setEmissionRate ( 10000 );
	
	m_pBackground->addChild ( m_pEmitter, 10 );

	setEmitterPosition ( );
}

const KDchar* DemoRing::title ( KDvoid )
{
	return "Ring Demo";
}

//------------------------------------------------------------------
//
// ParallaxParticle
//
//------------------------------------------------------------------
KDvoid ParallaxParticle::onEnter ( KDvoid )
{
	TestParticle::onEnter ( );
	
	this->removeChild ( m_pBackground, KD_TRUE );
	m_pBackground = KD_NULL;

	CCParallaxNode*  p = CCParallaxNode::create ( ); 
	CCSprite*  p1 = CCSprite::create ( "Images/background3.png" );

	m_pEmitter = CCParticleFlower::create ( );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );		
	m_pEmitter->setPosition ( ccp ( 250, 200 ) );

	p1->addChild ( m_pEmitter, 10 );		
	p->addChild ( p1, 1, ccp ( 0.5f, 1 ), ccp ( 0, 250 ) );

	CCSprite*  p2 = CCSprite::create ( "Images/background3.png" );
	CCParticleSun*  par = CCParticleSun::create ( );

	par->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	p2->addChild ( par, 10 );
	p->addChild ( p2, 2, ccp ( 1.5f, 1 ), ccp ( 0,  50 ) );

	CCActionInterval*  move = CCMoveBy::create ( 4, ccp ( 300, 0 ) );
	p->runAction ( CCRepeatForever::create ( CCSequence::create ( move, move->reverse ( ), KD_NULL ) ) );	

	this->addChild ( p, 5 );
}

const KDchar* ParallaxParticle::title ( KDvoid )
{
	return "Parallax + Particles";
}

//------------------------------------------------------------------
//
// RadiusMode1
//
//------------------------------------------------------------------
KDvoid RadiusMode1::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

	this->removeChild ( m_pBackground, KD_TRUE );
	m_pBackground = KD_NULL;

    m_pEmitter = new CCParticleSystemQuad ( );
	m_pEmitter->autorelease ( );

	m_pEmitter->initWithTotalParticles ( 200 );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars2-grayscale.png" ) );

	// duration
	m_pEmitter->setDuration ( kCCParticleDurationInfinity );

	// radius mode
	m_pEmitter->setEmitterMode ( kCCParticleModeRadius );

	// radius mode: start and end radius in pixels
	m_pEmitter->setStartRadius ( 0 );
	m_pEmitter->setStartRadiusVar ( 0 );
	m_pEmitter->setEndRadius ( 160 );
	m_pEmitter->setEndRadiusVar ( 0 );

	// radius mode: degrees per second
	m_pEmitter->setRotatePerSecond ( 180 );
	m_pEmitter->setRotatePerSecondVar ( 0 );

	// angle
	m_pEmitter->setAngle ( 90 );
	m_pEmitter->setAngleVar ( 0 );

	// emitter position
	m_pEmitter->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	m_pEmitter->setPosVar ( CCPointZero );

	// life of particles
	m_pEmitter->setLife ( 5 );
	m_pEmitter->setLifeVar ( 0 );

	// spin of particles
	m_pEmitter->setStartSpin ( 0 );
	m_pEmitter->setStartSpinVar ( 0 );
	m_pEmitter->setEndSpin ( 0 );
	m_pEmitter->setEndSpinVar ( 0 );

	// color of particles
	ccColor4F  startColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColor ( startColor );

	ccColor4F  startColorVar = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColorVar ( startColorVar );

	ccColor4F  endColor = { 0.1f, 0.1f, 0.1f, 0.2f };
	m_pEmitter->setEndColor ( endColor );

	ccColor4F  endColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };	
	m_pEmitter->setEndColorVar ( endColorVar );

	// size, in pixels
	m_pEmitter->setStartSize ( 32 );
	m_pEmitter->setStartSizeVar ( 0 );
	m_pEmitter->setEndSize ( kCCParticleStartSizeEqualToEndSize );

	// emits per second
	m_pEmitter->setEmissionRate ( m_pEmitter->getTotalParticles ( ) / m_pEmitter->getLife ( ) );

	// additive
	m_pEmitter->setBlendAdditive ( KD_FALSE );

	this->addChild ( m_pEmitter, 10 );
}

const KDchar* RadiusMode1::title ( KDvoid )
{
    return "Radius Mode: Spiral";
}

//------------------------------------------------------------------
//
// RadiusMode2
//
//------------------------------------------------------------------
KDvoid RadiusMode2::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

	this->removeChild ( m_pBackground, KD_TRUE );
	m_pBackground = KD_NULL;

    m_pEmitter = new CCParticleSystemQuad ( );
	m_pEmitter->autorelease ( );

	m_pEmitter->initWithTotalParticles ( 200 );
	m_pEmitter->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/stars2-grayscale.png" ) );

	// duration
	m_pEmitter->setDuration ( kCCParticleDurationInfinity );

	// radius mode
	m_pEmitter->setEmitterMode ( kCCParticleModeRadius );

	// radius mode: start and end radius in pixels
	m_pEmitter->setStartRadius ( 100 );
	m_pEmitter->setStartRadiusVar ( 0 );
	m_pEmitter->setEndRadius ( kCCParticleStartRadiusEqualToEndRadius );
	m_pEmitter->setEndRadiusVar ( 0 );

	// radius mode: degrees per second
	m_pEmitter->setRotatePerSecond ( 45 );
	m_pEmitter->setRotatePerSecondVar ( 0 );

	// angle
	m_pEmitter->setAngle ( 90 );
	m_pEmitter->setAngleVar ( 0 );

	// emitter position
	m_pEmitter->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	m_pEmitter->setPosVar ( CCPointZero );

	// life of particles
	m_pEmitter->setLife ( 4 );
	m_pEmitter->setLifeVar ( 0 );

	// spin of particles
	m_pEmitter->setStartSpin ( 0 );
	m_pEmitter->setStartSpinVar ( 0 );
	m_pEmitter->setEndSpin ( 0 );
	m_pEmitter->setEndSpinVar ( 0 );

	// color of particles
	ccColor4F  startColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColor ( startColor );

	ccColor4F  startColorVar = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pEmitter->setStartColorVar ( startColorVar );

	ccColor4F  endColor = { 0.1f, 0.1f, 0.1f, 0.2f };
	m_pEmitter->setEndColor ( endColor );

	ccColor4F  endColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };	
	m_pEmitter->setEndColorVar ( endColorVar );

	// size, in pixels
	m_pEmitter->setStartSize ( 32 );
	m_pEmitter->setStartSizeVar ( 0 );
	m_pEmitter->setEndSize ( kCCParticleStartSizeEqualToEndSize );

	// emits per second
	m_pEmitter->setEmissionRate ( m_pEmitter->getTotalParticles ( ) / m_pEmitter->getLife ( ) );

	// additive
	m_pEmitter->setBlendAdditive ( KD_FALSE );

	this->addChild ( m_pEmitter, 10 );

}

const KDchar* RadiusMode2::title ( KDvoid )
{
    return "Radius Mode: Semi Circle";
}

//------------------------------------------------------------------
//
// DemoParticleFromFile
//
//------------------------------------------------------------------
KDvoid DemoParticleFromFile::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    m_pEmitter = new CCParticleSystemQuad ( );
	m_pEmitter->autorelease ( );
	
	std::string  filepath = "Particles/" + m_title + ".plist";
	m_pEmitter->initWithFile ( filepath.c_str ( ) );
		
	this->removeChild ( m_pBackground, KD_FALSE );
	m_pBackground = KD_NULL;

	this->addChild ( m_pEmitter, 10 );
		
	setEmitterPosition ( );
}

//------------------------------------------------------------------
//
// ParticleBatchHybrid
//
//------------------------------------------------------------------
KDvoid ParticleBatchHybrid::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    m_pEmitter = CCParticleSystemQuad::create ( "Particles/LavaFlow.plist" );

    CCParticleBatchNode*  pBatch = CCParticleBatchNode::createWithTexture ( m_pEmitter->getTexture ( ) );
    pBatch->addChild ( m_pEmitter );
    this->addChild ( pBatch, 10 );

	this->schedule ( schedule_selector ( ParticleBatchHybrid::switchRender ), 2.0f );

	CCNode*  pNode = CCNode::create ( );
	this->addChild ( pNode );

	m_pParent1 = pBatch;
	m_pParent2 = pNode;
}

KDvoid ParticleBatchHybrid::switchRender ( KDfloat fDelta )
{
     KDbool   usingBatch = ( m_pEmitter->getBatchNode ( ) != KD_NULL );
     m_pEmitter->removeFromParentAndCleanup ( KD_FALSE );
 
     CCNode*  newParent = ( usingBatch ? m_pParent2  : m_pParent1 );
     newParent->addChild ( m_pEmitter );
 
     CCLOG ( "Particle: Using new parent: %s", usingBatch ? "CCNode" : "CCParticleBatchNode" );
}

const KDchar* ParticleBatchHybrid::title ( KDvoid )
{
    return "Paticle Batch";
}

const KDchar* ParticleBatchHybrid::subtitle ( KDvoid )
{
    return "Hybrid: batched and non batched every 2 seconds";
}

//------------------------------------------------------------------
//
// ParticleBatchMultipleEmitters
//
//------------------------------------------------------------------
KDvoid ParticleBatchMultipleEmitters::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    CCParticleSystemQuad*  emitter1 = CCParticleSystemQuad::create ( "Particles/LavaFlow.plist" );
    emitter1->setStartColor ( ccc4F ( 1, 0, 0, 1 ) );

    CCParticleSystemQuad*  emitter2 = CCParticleSystemQuad::create ( "Particles/LavaFlow.plist" );
    emitter2->setStartColor ( ccc4F ( 0, 1, 0, 1 ) );
    
	CCParticleSystemQuad*  emitter3 = CCParticleSystemQuad::create ( "Particles/LavaFlow.plist" );
    emitter3->setStartColor ( ccc4F ( 0, 0, 1, 1 ) );

    const CCSize&  s = this->getContentSize ( );

    emitter1->setPosition ( ccp ( s.cx / 1.25f, s.cy / 1.25f ) );
    emitter2->setPosition ( ccp ( s.cx / 2    , s.cy / 2     ) );
    emitter3->setPosition ( ccp ( s.cx / 4    , s.cy / 4     ) );

    CCParticleBatchNode*  batch = CCParticleBatchNode::createWithTexture ( emitter1->getTexture ( ) );

    batch->addChild ( emitter1, 0 );
    batch->addChild ( emitter2, 0 );
    batch->addChild ( emitter3, 0 );

    addChild ( batch, 10 );
}

const KDchar* ParticleBatchMultipleEmitters::title ( KDvoid )
{
    return "Paticle Batch";
}

const KDchar* ParticleBatchMultipleEmitters::subtitle ( KDvoid )
{
    return "Multiple emitters. One Batch";
}

//------------------------------------------------------------------
//
// ParticleReorder
//
//------------------------------------------------------------------ 
KDvoid ParticleReorder::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    m_uOrder = 0;
    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    CCParticleSystem*  ignore = CCParticleSystemQuad::create ( "Particles/SmallSun.plist" );
    CCNode*  parent1 = CCNode::create ( );
    CCNode*  parent2 = CCParticleBatchNode::createWithTexture ( ignore->getTexture ( ) );
    ignore->unscheduleUpdate ( );

    for ( KDuint i = 0; i < 2; i++ ) 
    {
        CCNode*  parent = ( i == 0 ? parent1 : parent2 );

        CCParticleSystemQuad*  emitter1 = CCParticleSystemQuad::create ( "Particles/SmallSun.plist" );
        emitter1->setStartColor ( ccc4F ( 1, 0, 0, 1 ) );
		emitter1->setBlendAdditive ( KD_FALSE );
        
		CCParticleSystemQuad*  emitter2 = CCParticleSystemQuad::create ( "Particles/SmallSun.plist" );
        emitter2->setStartColor ( ccc4F ( 0, 1, 0, 1 ) );
		emitter2->setBlendAdditive ( KD_FALSE );

        CCParticleSystemQuad*  emitter3 = CCParticleSystemQuad::create ( "Particles/SmallSun.plist" );
        emitter3->setStartColor ( ccc4F ( 0, 0, 1, 1 ) );
		emitter3->setBlendAdditive ( KD_FALSE );

        const CCSize&  s = this->getContentSize ( );

        KDint  neg = ( i == 0 ? 1 : -1 );

        emitter1->setPosition ( ccp ( s.cx / 2 - 30, s.cy / 2 + 60 * neg ) );
        emitter2->setPosition ( ccp ( s.cx / 2     , s.cy / 2 + 60 * neg ) );
        emitter3->setPosition ( ccp ( s.cx / 2 + 30, s.cy / 2 + 60 * neg ) );

        parent->addChild ( emitter1, 0, 1 );
        parent->addChild ( emitter2, 0, 2 );
        parent->addChild ( emitter3, 0, 3 );

        addChild ( parent, 10, 1000 + i );
    }

    schedule ( schedule_selector ( ParticleReorder::reorderParticles ), 1.0f );
}

const KDchar* ParticleReorder::title ( KDvoid )
{
    return "Reordering particles";
}

const KDchar* ParticleReorder::subtitle ( KDvoid )
{
    return "Reordering particles with and without batches batches";
}

KDvoid ParticleReorder::reorderParticles ( KDfloat fDelta )
{
    for( KDint i = 0; i < 2; i++ ) 
	{
        CCNode*  parent = getChildByTag ( 1000 + i );

        CCNode*  child1 = parent->getChildByTag ( 1 );
        CCNode*  child2 = parent->getChildByTag ( 2 );
        CCNode*  child3 = parent->getChildByTag ( 3 );

        if ( m_uOrder % 3 == 0 )
		{
            parent->reorderChild ( child1, 1 );
            parent->reorderChild ( child2, 2 );
            parent->reorderChild ( child3, 3 );
        }
		else if ( m_uOrder % 3 == 1 ) 
		{
            parent->reorderChild ( child1, 3 );
            parent->reorderChild ( child2, 1 );
            parent->reorderChild ( child3, 2 );
        }
		else if ( m_uOrder % 3 == 2 )
		{
            parent->reorderChild ( child1, 2 );
            parent->reorderChild ( child2, 3 );
            parent->reorderChild ( child3, 1 );
        }
    }

    m_uOrder++;
}

class RainbowEffect : public CCParticleSystemQuad
{
	public :

		KDbool  init ( KDvoid );

		virtual KDbool  initWithTotalParticles ( KDuint uNumberOfParticles );

		virtual KDvoid  update ( KDfloat fDelta );
};

KDbool RainbowEffect::init ( KDvoid )
{
    return initWithTotalParticles ( 150 );
}

KDbool RainbowEffect::initWithTotalParticles ( KDuint uNumberOfParticles )
{
    if ( CCParticleSystemQuad::initWithTotalParticles ( uNumberOfParticles ) )
    {
        // additive
        setBlendAdditive ( KD_FALSE );

        // duration
        setDuration ( kCCParticleDurationInfinity );

        // Gravity Mode
        setEmitterMode ( kCCParticleModeGravity );

        // Gravity Mode: gravity
        setGravity ( ccp ( 0, 0 ) );

        // Gravity mode: radial acceleration
        setRadialAccel ( 0 );
        setRadialAccelVar ( 0 );

        // Gravity mode: speed of particles
        setSpeed ( 120 );
        setSpeedVar ( 0 );

        // angle
        setAngle ( 180 );
        setAngleVar ( 0 );

        // emitter position
        setPosition ( ccpMid ( this->getContentSize ( ) ) );
        setPosVar ( CCPointZero );

        // life of particles
        setLife ( 0.5f );
        setLifeVar ( 0 );

        // size, in pixels
        setStartSize ( 25.0f );
        setStartSizeVar ( 0 );
        setEndSize ( kCCParticleStartSizeEqualToEndSize );

        // emits per seconds
        setEmissionRate ( getTotalParticles ( ) / getLife ( ) );

        // color of particles
        setStartColor ( ccc4FFromccc4B ( ccc4 ( 50, 50, 50, 50 ) ) );
        setEndColor ( ccc4FFromccc4B ( ccc4 ( 0, 0, 0, 0 ) ) );

        m_tStartColorVar.r = 0.0f;
        m_tStartColorVar.g = 0.0f;
        m_tStartColorVar.b = 0.0f;
        m_tStartColorVar.a = 0.0f;
        m_tEndColorVar.r = 0.0f;
        m_tEndColorVar.g = 0.0f;
        m_tEndColorVar.b = 0.0f;
        m_tEndColorVar.a = 0.0f;

        setTexture(CCTextureCache::sharedTextureCache ( )->addImage ( "Images/particles.png" ) );
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid RainbowEffect::update ( KDfloat fDelta )
{
    m_fEmitCounter = 0;
    CCParticleSystemQuad::update ( fDelta );
}

//------------------------------------------------------------------
//
// MultipleParticleSystems
//
//------------------------------------------------------------------
KDvoid MultipleParticleSystems::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    CCTextureCache::sharedTextureCache ( )->addImage ( "Images/particles.png" ); 

    for ( KDint i = 0; i < 5; i++ ) 
	{
        CCParticleSystemQuad*  particleSystem = CCParticleSystemQuad::create ( "Particles/SpinningPeas.plist" );

        particleSystem->setPosition ( ccp ( i * 50, i * 50 ) );
        particleSystem->setPositionType ( kCCPositionTypeGrouped );
        addChild ( particleSystem );
    }

    m_pEmitter = KD_NULL;

}

const KDchar* MultipleParticleSystems::title ( KDvoid )
{
    return "Multiple particle systems";
}

const KDchar* MultipleParticleSystems::subtitle ( KDvoid )
{
    return "v1.1 test: FPS should be lower than next test";
}

KDvoid MultipleParticleSystems::update ( KDfloat fDelta )
{
    CCLabelAtlas *atlas = (CCLabelAtlas*) getChildByTag ( kTagParticleCount );

    KDuint      count = 0; 
    CCObject*   pObj = KD_NULL;
    CCARRAY_FOREACH ( getChildren ( ), pObj )
    {
        CCParticleSystem*  item = dynamic_cast<CCParticleSystem*> ( pObj );
        if ( item != KD_NULL )
        {
            count += item->getParticleCount ( );    
        }
    }

    KDchar str[100] = {0};
    kdSprintf ( str, "%4d", count );
    atlas->setString ( str );
}

//------------------------------------------------------------------
//
// MultipleParticleSystemsBatched
//
//------------------------------------------------------------------
KDvoid MultipleParticleSystemsBatched::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    CCParticleBatchNode*  batchNode = new CCParticleBatchNode ( );
    batchNode->initWithTexture ( KD_NULL, 3000 );

    addChild ( batchNode, 1, 2 );

    for ( KDint i = 0; i < 5; i++ )
	{
        CCParticleSystemQuad*  particleSystem = CCParticleSystemQuad::create ( "Particles/SpinningPeas.plist" );

        particleSystem->setPositionType ( kCCPositionTypeGrouped );         
        particleSystem->setPosition ( ccp ( i * 50, i * 50 ) );

        batchNode->setTexture ( particleSystem->getTexture ( ) );
        batchNode->addChild ( particleSystem );
    }

    batchNode->release ( );
}

KDvoid MultipleParticleSystemsBatched::update ( KDfloat fDelta )
{
    CCLabelAtlas*  atlas = (CCLabelAtlas*) getChildByTag ( kTagParticleCount );

    KDuint     count = 0; 
    CCNode*    batchNode = getChildByTag ( 2 );
    CCObject*  pObj = KD_NULL;
    CCARRAY_FOREACH ( batchNode->getChildren ( ), pObj )
    {
        CCParticleSystem*  item = dynamic_cast<CCParticleSystem*> ( pObj );
        if ( item != KD_NULL )
        {
            count += item->getParticleCount ( );    
        }
    }

    KDchar str[50] = {0};
    kdSprintf ( str, "%4d", count );
    atlas->setString ( str );
}

const KDchar* MultipleParticleSystemsBatched::title ( KDvoid )
{
    return "Multiple particle systems batched";
}

const KDchar* MultipleParticleSystemsBatched::subtitle ( KDvoid )
{
    return "v1.1 test: should perform better than previous test";
}

//------------------------------------------------------------------
//
// AddAndDeleteParticleSystems
//
//------------------------------------------------------------------
KDvoid AddAndDeleteParticleSystems::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    //adds the texture inside the plist to the texture cache
    m_pBatchNode = CCParticleBatchNode::createWithTexture ( (CCTexture2D*) KD_NULL );	
    addChild ( m_pBatchNode, 1, 2 );

    for ( KDint i = 0; i < 6; i++ )
	{
        CCParticleSystemQuad*  particleSystem = CCParticleSystemQuad::create ( "Particles/Spiral.plist" );
        m_pBatchNode->setTexture ( particleSystem->getTexture ( ) );

        particleSystem->setPositionType ( kCCPositionTypeGrouped );         
        particleSystem->setTotalParticles ( 200 );
        particleSystem->setPosition ( ccp ( i * 15 + 100, i * 15 + 100 ) );

        KDuint randZ = kdRand ( ) % 100; 
        m_pBatchNode->addChild ( particleSystem, randZ, -1 );
    }

#if !defined ( _WIN32 )
    schedule ( schedule_selector ( AddAndDeleteParticleSystems::removeSystem ), 0.5f );
#endif
}

KDvoid AddAndDeleteParticleSystems::removeSystem ( KDfloat fDelta )
{
    KDint  nChildrenCount = m_pBatchNode->getChildren ( )->count ( );
    if ( nChildrenCount > 0 ) 
    {
        CCLOG ( "remove random system" );
		KDuint  uRand =  kdRand ( ) % nChildrenCount;
        m_pBatchNode->removeChild ( (CCNode*) m_pBatchNode->getChildren ( )->objectAtIndex ( uRand ), KD_TRUE );

        CCParticleSystemQuad*  particleSystem = CCParticleSystemQuad::create ( "Particles/Spiral.plist" );

        // add new
        particleSystem->setPositionType ( kCCPositionTypeGrouped );         
        particleSystem->setTotalParticles ( 200 );
        particleSystem->setPosition ( ccp ( kdRand ( ) % 300, kdRand ( ) % 400 ) );

        CCLOG ( "add a new system" );
        KDuint randZ = kdRand ( ) % 100; 
        m_pBatchNode->addChild ( particleSystem, randZ, -1 );
    }
}

KDvoid AddAndDeleteParticleSystems::update ( KDfloat fDelta )
{
    CCLabelAtlas*  atlas = (CCLabelAtlas*) getChildByTag ( kTagParticleCount );

    KDuint     count = 0; 
    CCNode*    batchNode = getChildByTag ( 2 );
    CCObject*  pObj = KD_NULL;
    CCARRAY_FOREACH ( batchNode->getChildren ( ), pObj )
    {
        CCParticleSystem* item = dynamic_cast<CCParticleSystem*> ( pObj );
        if ( item != KD_NULL )
        {
            count += item->getParticleCount ( );    
        }
    }
    KDchar str[100] = {0};
    kdSprintf ( str, "%4d", count );
    atlas->setString ( str );
}

const KDchar* AddAndDeleteParticleSystems::title ( KDvoid )
{
    return "Add and remove Particle System";
}

const KDchar* AddAndDeleteParticleSystems::subtitle ( KDvoid )
{
    return "v1.1 test: every 2 sec 1 system disappear, 1 appears";
}

//------------------------------------------------------------------
//
// ReorderParticleSystems
//
//------------------------------------------------------------------
KDvoid ReorderParticleSystems::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    m_pBatchNode = CCParticleBatchNode::create ( "Images/stars-grayscale.png", 3000 );

    addChild ( m_pBatchNode, 1, 2 );

    for ( KDint i = 0; i<3; i++ )
	{
        CCParticleSystemQuad*  particleSystem = new CCParticleSystemQuad ( );
        particleSystem->initWithTotalParticles ( 200 );
        particleSystem->setTexture ( m_pBatchNode->getTexture ( ) );

        // duration
        particleSystem->setDuration ( kCCParticleDurationInfinity );

        // radius mode
        particleSystem->setEmitterMode ( kCCParticleModeRadius );

        // radius mode: 100 pixels from center
        particleSystem->setStartRadius ( 100 );
        particleSystem->setStartRadiusVar ( 0 );
        particleSystem->setEndRadius ( kCCParticleStartRadiusEqualToEndRadius );
        particleSystem->setEndRadiusVar ( 0 );    // not used when start == end

        // radius mode: degrees per second
        // 45 * 4 seconds of life = 180 degrees
        particleSystem->setRotatePerSecond ( 45 );
        particleSystem->setRotatePerSecondVar ( 0 );

        // angle
        particleSystem->setAngle ( 90 );
        particleSystem->setAngleVar ( 0 );

        // emitter position
        particleSystem->setPosVar ( CCPointZero );

        // life of particles
        particleSystem->setLife ( 4 );
        particleSystem->setLifeVar ( 0 );

        // spin of particles
        particleSystem->setStartSpin(0);
        particleSystem->setStartSpinVar(0);
        particleSystem->setEndSpin(0);
        particleSystem->setEndSpinVar(0);

        // color of particles
        KDfloat color[3] = { 0, 0, 0 };
        color[i] = 1;
        ccColor4F startColor = { color[0], color[1], color[2], 1.0f };
        particleSystem->setStartColor ( startColor );

        ccColor4F startColorVar = { 0, 0, 0, 0 };
        particleSystem->setStartColorVar ( startColorVar );

        ccColor4F endColor = startColor;
        particleSystem->setEndColor ( endColor );

        ccColor4F endColorVar = startColorVar;
        particleSystem->setEndColorVar ( endColorVar );

        // size, in pixels
        particleSystem->setStartSize ( 32 );
        particleSystem->setStartSizeVar ( 0 );
        particleSystem->setEndSize ( kCCParticleStartSizeEqualToEndSize );

        // emits per second
        particleSystem->setEmissionRate ( particleSystem->getTotalParticles ( ) / particleSystem->getLife ( ) );

        // additive
        particleSystem->setPosition ( ccp ( i * 10 + 120, 200 ) );

        m_pBatchNode->addChild ( particleSystem );
        particleSystem->setPositionType ( kCCPositionTypeFree );

        particleSystem->release ( );
        //[pBNode addChild:particleSystem z:10 tag:0);
    }

    schedule ( schedule_selector ( ReorderParticleSystems::reorderSystem ), 2.0f );
    m_pEmitter = KD_NULL;
}

KDvoid ReorderParticleSystems::reorderSystem ( KDfloat fDelta )
{
    CCParticleSystem*  system = (CCParticleSystem*) m_pBatchNode->getChildren ( )->objectAtIndex ( 1 );
    m_pBatchNode->reorderChild ( system, system->getZOrder ( ) - 1 );     
}

KDvoid ReorderParticleSystems::update ( KDfloat fDelta )
{
    CCLabelAtlas*  atlas = (CCLabelAtlas*) getChildByTag ( kTagParticleCount );

    KDuint     count = 0; 
    CCNode*    batchNode = getChildByTag(2);
    CCObject*  pObj = KD_NULL;
    CCARRAY_FOREACH ( batchNode->getChildren ( ), pObj )
    {
        CCParticleSystem* item = dynamic_cast<CCParticleSystem*> ( pObj );
        if ( item != KD_NULL )
        {
            count += item->getParticleCount ( );    
        }
    }

    KDchar str[100] = { 0 };
    kdSprintf ( str, "%4d", count );
    atlas->setString ( str );
}

const KDchar* ReorderParticleSystems::title ( KDvoid )
{
    return "reorder systems";
}

const KDchar* ReorderParticleSystems::subtitle ( KDvoid )
{
    return "changes every 2 seconds";
}

//------------------------------------------------------------------
//
// PremultipliedAlphaTest
//
//------------------------------------------------------------------
const KDchar* PremultipliedAlphaTest::title ( KDvoid )
{
    return "premultiplied alpha";
}

const KDchar* PremultipliedAlphaTest::subtitle ( KDvoid )
{
    return "no black halo, particles should fade out";
}

KDvoid PremultipliedAlphaTest::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    this->removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    m_pEmitter = CCParticleSystemQuad::create ( "Particles/BoilingFoam.plist" );

    // Particle Designer "normal" blend func causes black halo on premul textures (ignores multiplication)
    //this->emitter.blendFunc = (ccBlendFunc){ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };

    // Cocos2d "normal" blend func for premul causes alpha to be ignored (oversaturates colors)
    //ccBlendFunc tBlendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
    //m_pEmitter->setBlendFunc ( tBlendFunc );

    //CCAssert ( m_pEmitter->getOpacityModifyRGB ( ), "Particle texture does not have premultiplied alpha, test is useless");

    // Toggle next line to see old behavior
    //	this->emitter.opacityModifyRGB = NO;

    m_pEmitter->setStartColor( ccc4F ( 1, 1, 1, 1 ) );
    m_pEmitter->setEndColor  ( ccc4F ( 1, 1, 1, 0 ) );

    m_pEmitter->setStartColorVar ( ccc4F ( 0, 0, 0, 0 ) );
    m_pEmitter->setEndColorVar   ( ccc4F ( 0, 0, 0, 0 ) );

    this->addChild ( m_pEmitter, 10 );
}

//------------------------------------------------------------------
//
// PremultipliedAlphaTest2
//
//------------------------------------------------------------------

KDvoid PremultipliedAlphaTest2::onEnter ( KDvoid )
{
    TestParticle::onEnter ( );

    this->removeChild ( m_pBackground, KD_TRUE );
    m_pBackground = KD_NULL;

    m_pEmitter = CCParticleSystemQuad::create ( "Particles/TestPremultipliedAlpha.plist" );
    this->addChild ( m_pEmitter, 10 );
}

const KDchar* PremultipliedAlphaTest2::title ( KDvoid )
{
    return "premultiplied alpha 2";
}

const KDchar* PremultipliedAlphaTest2::subtitle ( KDvoid )
{
    return "Arrows should be faded";
}
