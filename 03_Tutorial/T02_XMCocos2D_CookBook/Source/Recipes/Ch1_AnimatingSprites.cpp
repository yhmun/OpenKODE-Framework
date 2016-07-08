/* --------------------------------------------------------------------------
 *
 *      File            Ch1_AnimatingSprites.cpp
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
#include "Ch1_AnimatingSprites.h"
#include "Libraries/SimpleAnimObject.h"

enum 
{
	TAG_CASTLE			= 0 ,
	TAG_LIGHTNING_BOLT	= 1 ,
	TAG_BATS			= 2 ,
};

// Bat animation types
enum 
{
	BAT_FLYING_UP		= 0 ,
	BAT_GLIDING_DOWN	= 1 ,
};

KDbool Ch1_AnimatingSprites::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Add our PLIST to the SpriteFrameCache
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "simple_bat.plist" );
	
	// Add castle background
	CCSprite*	pCastle = CCSprite::create ( "dracula_castle.jpg" );
	pCastle->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pCastle, 0, TAG_CASTLE );
	
	// Add a lightning bolt
	m_pLightningBolt = CCSprite::create ( "lightning_bolt.png" );
	m_pLightningBolt->setPosition ( ccp ( 240, 160 ) );
	m_pLightningBolt->setOpacity ( 64 );
	m_pLightningBolt->retain ( );

	// Add a sprite to make it light up other areas.
	m_pLightningGlow = CCSprite::create ( "lightning_glow.png" );
	m_pLightningGlow->setColor ( ccc3 ( 255, 255, 0 ) );
	m_pLightningGlow->setPosition ( ccp ( 240, 160 ) );
	m_pLightningGlow->setOpacity ( 100 );

	ccBlendFunc		tBlendFunc = { GL_ONE, GL_ONE };
	m_pLightningGlow->setBlendFunc ( tBlendFunc );
	m_pLightningBolt->addChild ( m_pLightningGlow );

	// Set a counter for lightning duration randomization
	m_nLightningRemoveCount = 0;

	// Bats Array Initialization
	m_pBats = CCArray::create ( );
	m_pBats->retain ( );

	// Add bats using a batch node.
	CCSpriteBatchNode*	pBatch1 = CCSpriteBatchNode::create ( "simple_bat.png", 10 );
	this->addChild ( pBatch1, 2, TAG_BATS );
	
	// Make them start flying up.
	for ( KDint x = 0; x < 10; x++ )
	{
		// Create SimpleAnimObject of bat
		SimpleAnimObject*	pBat = SimpleAnimObject::createWithBatchNode ( pBatch1, CCRect ( 0, 0, 48, 48 ) );
		pBatch1->addChild ( pBat );
		pBat->setPosition ( ccp ( kdRand ( ) % 400 + 40, kdRand ( ) % 150 + 150 ) );
	
		// Make the bat fly up. Get the animation delay (flappingSpeed).
		KDfloat		fFlappingSpeed = this->makeBatFlyUp ( pBat );
		
		// Base y velocity on flappingSpeed.
		pBat->setVelocity ( ccp ( ( kdRand ( ) % 1000 ) / 500 + 0.2f, 0.1f / fFlappingSpeed ) );
		
		// Add a pointer to this bat object to the CCArray
		m_pBats->addObject ( pBat );		
		
		//Set the bat's direction based on x velocity.
		if ( pBat->getVelocity ( ).x > 0 )
		{
			pBat->setFlipX ( KD_TRUE );
		}		
	}
	
	// Schedule physics updates
	this->schedule ( schedule_selector ( Ch1_AnimatingSprites::step ) );

	return KD_TRUE;
}

KDvoid Ch1_AnimatingSprites::onExit ( KDvoid )
{
	m_pBats->release ( );
	m_pLightningBolt->release ( );

	Recipe::onExit ( );
}

KDvoid Ch1_AnimatingSprites::step ( KDfloat fDelta )
{
	const CCSize&	tLyrSize = this->getContentSize ( );

	CCObject*	pObject;
	CCARRAY_FOREACH ( m_pBats, pObject )
	{
		// Get SimpleAnimObject out of CCArray of NSValue objects.
		SimpleAnimObject*	pBat = (SimpleAnimObject*) pObject;

		CCPoint		tPosition = pBat->getPosition ( );
		CCPoint		tVelocity = pBat->getVelocity ( );

		// Make sure bats don't fly off the screen
		if ( tPosition.x > tLyrSize.cx )
		{
			pBat->setVelocity ( ccp ( -tVelocity.x, tVelocity.y ) );
			pBat->setFlipX ( KD_FALSE );
		}
		else if ( tPosition.x < 0 )
		{
			pBat->setVelocity ( ccp ( -tVelocity.x, tVelocity.y ) );
			pBat->setFlipX ( KD_TRUE );
		}
		else if ( tPosition.y > tLyrSize.cy )
		{
			pBat->setVelocity ( ccp ( tVelocity.x, -tVelocity.y ) );
			this->makeBatGlideDown ( pBat );
		}
		else if ( tPosition.y < 0 )
		{
			pBat->setVelocity ( ccp ( tVelocity.x, -tVelocity.y ) );
			this->makeBatFlyUp ( pBat );
		}		

		// Randomly make them fly back up
		if ( kdRand ( ) % 100 == 7 )
		{
			KDint	nAnimationType = pBat->getAnimationType ( );
			
			tVelocity = pBat->getVelocity ( );

			if ( nAnimationType == BAT_GLIDING_DOWN )
			{
				this->makeBatFlyUp ( pBat ); 
				pBat->setVelocity ( ccp ( tVelocity.x, -tVelocity.y ) );				 
			}
			else if ( nAnimationType == BAT_FLYING_UP )
			{
				this->makeBatGlideDown ( pBat );				
				pBat->setVelocity ( ccp ( tVelocity.x, -tVelocity.y ) );					
			}
		}
		
		// Update bat position based on direction
		tVelocity = pBat->getVelocity ( );
		pBat->setPosition ( ccpAdd ( tPosition, tVelocity ) );
	}
	
	// Randomly make lightning strike
	if ( kdRand ( ) % 70 == 7 )
	{
		if ( m_nLightningRemoveCount < 0 )
		{
			this->addChild ( m_pLightningBolt, 1, TAG_LIGHTNING_BOLT );
			m_nLightningRemoveCount = kdRand ( ) % 5 + 5;
		}
	}
	
	// Count down
	m_nLightningRemoveCount--;
	
	// Clean up any old lightning bolts
	if ( m_nLightningRemoveCount == 0 )
	{
		this->removeChildByTag ( TAG_LIGHTNING_BOLT, KD_FALSE );
	}
}

KDfloat Ch1_AnimatingSprites::makeBatFlyUp ( SimpleAnimObject* pBat )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Randomize animation speed.
	KDfloat			fDelay = (KDfloat) ( kdRand ( ) % 5 + 5 ) / 80;
	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( fDelay );

	// Randomize animation frame order.
	KDint	nNum = kdRand ( ) % 4 + 1;
	for ( KDint i = 1; i <= 4; i++ ) 
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "simple_bat_0%i.png", nNum ) ) );
		nNum++;
		if ( nNum > 4 )
		{
			nNum = 1; 
		}
	}		
	
	// Stop any running animations and apply this one.
	pBat->stopAllActions ( );
	pBat->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );

	// Keep track of which animation is running.
	pBat->setAnimationType ( BAT_FLYING_UP );

	return fDelay;	// We return how fast the bat is flapping.
}

KDvoid Ch1_AnimatingSprites::makeBatGlideDown ( SimpleAnimObject* pBat )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Apply a simple single frame gliding animation.
	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 100.0f );
	pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( "simple_bat_01.png" ) );
	
	// Stop any running animations and apply this one.
	pBat->stopAllActions ( );
	pBat->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
	
	// Keep track of which animation is running.
	pBat->setAnimationType ( BAT_GLIDING_DOWN );
}