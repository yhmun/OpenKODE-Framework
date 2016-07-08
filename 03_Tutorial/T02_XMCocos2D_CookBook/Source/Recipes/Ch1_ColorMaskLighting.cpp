/* --------------------------------------------------------------------------
 *
 *      File            Ch1_ColorMaskLighting.cpp
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
#include "Ch1_ColorMaskLighting.h"
#include "Libraries/SimpleAnimObject.h"

enum
{
	TAG_CAVE_BG			= 0,
	TAG_DARKNESS_LAYER	= 1,
	TAG_BURN_SPRITE		= 2,
	TAG_LIGHT_SPRITE	= 3,
	TAG_MONK			= 4,
	TAG_CAVE_BATS		= 5,
};

enum 
{
	BAT_FLYING_UP		= 0,
	BAT_GLIDING_DOWN	= 1,
};

KDbool Ch1_ColorMaskLighting::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Add our PLISTs to the SpriteFrameCache singleton
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );
	pCache->addSpriteFramesWithFile ( "simple_bat.plist" );
	pCache->addSpriteFramesWithFile ( "monk_lantern.plist" );
	
	// Add cave background
	CCSprite*	pCaveBg = CCSprite::create ( "cave.png" );
	pCaveBg->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pCaveBg, 0, TAG_CAVE_BG );

	// Set up the burn sprite that will "knock out" parts of the darkness layer depending on the alpha value of the pixels in the image.
	m_pBurnSprite = SimpleAnimObject::create ( "fire.png" );
	m_pBurnSprite->setPosition ( ccp ( 50, 50 ) );
	m_pBurnSprite->setScale ( 10.f );
	m_pBurnSprite->setColor ( ccc3 ( 255, 255, 0 ) );	
	m_pBurnSprite->retain ( );
	
	ccBlendFunc		tBlendFunc = { GL_ZERO, GL_ONE_MINUS_SRC_ALPHA };
	m_pBurnSprite->setBlendFunc ( tBlendFunc );
	m_pBurnSprite->setVelocity ( ccp ( 1, 0 ) );

	// Add a 'light' sprite which additively blends onto the scene. This represents the cone of light created by the monk's candle.
	m_pLightSprite = SimpleAnimObject::create ( "fire.png" );
	m_pLightSprite->setPosition ( ccp ( 50, 50 ) );
	m_pLightSprite->setScale ( 10.0f );
	m_pLightSprite->setColor ( ccc3 ( 100, 100, 50 ) );

	ccBlendFunc		tBlendFunc2 = { GL_ONE, GL_ONE };
	m_pLightSprite->setBlendFunc ( tBlendFunc2 );
	m_pLightSprite->setVelocity ( ccp ( 1, 0 ) );
	this->addChild ( m_pLightSprite, 4, TAG_LIGHT_SPRITE );

	// Add the monk
	m_pMonkSprite = SimpleAnimObject::create ( );
	m_pMonkSprite->setPosition ( ccp ( 50, 50 ) );
	m_pMonkSprite->setVelocity ( ccp ( 1, 0 ) );
	this->addChild ( m_pMonkSprite, 4, TAG_MONK );

	// Animate the monk to simulate walking.
	CCAnimation*	pAnimation = CCAnimation::create ( );
	pAnimation->setDelayPerUnit ( 0.1f ) ;
	for ( KDint i = 1; i <= 5; i++ )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "monk_lantern_0%d.png", i ) ) );
	}
	for ( KDint i = 4; i >= 2; i-- )
	{
		pAnimation->addSpriteFrame ( pCache->spriteFrameByName ( ccszf ( "monk_lantern_0%d.png", i ) ) );		
	}
	m_pMonkSprite->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );

	// Add the 'darkness' layer. This simulates darkness in the cave.
	m_pDarknessLayer = CCRenderTexture::create ( 480, 320 );
	m_pDarknessLayer->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( m_pDarknessLayer, 0, TAG_DARKNESS_LAYER );

	// Initialize the array of bats.
	m_pBats = CCArray::create ( );
	m_pBats->retain ( );

	// Add bats using a batch node.
	CCSpriteBatchNode*	pBatch1 = CCSpriteBatchNode::create ( "simple_bat.png", 10 );
	this->addChild ( pBatch1, 2, TAG_CAVE_BATS );
	
	// Make them start flying up.
	for ( KDint x = 0; x < 30; x++ )
	{
		// Create SimpleAnimObject of bat
		SimpleAnimObject*	pBat = SimpleAnimObject::createWithBatchNode ( pBatch1, CCRect ( 0, 0, 48, 48 ) );
		pBatch1->addChild ( pBat );
		pBat->setScale ( 0.5f );
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
	this->schedule ( schedule_selector ( Ch1_ColorMaskLighting::step ) );

	return KD_TRUE;
}

KDvoid Ch1_ColorMaskLighting::onExit ( KDvoid )
{
	m_pBats->release ( );
	m_pBurnSprite->release ( );

	Recipe::onExit ( );
}

KDvoid Ch1_ColorMaskLighting::step ( KDfloat fDelta )
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
		if ( kdRand ( ) % 50 == 7 )
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

	// Clear the darkness layer for redrawing. Here we clear it to BLACK with 90% opacity.
	m_pDarknessLayer->clear ( 0.0f, 0.0f, 0.0f, 0.9f );
	
	// Begin the darkness layer drawing routine. This transforms to the proper location, among other things.
	m_pDarknessLayer->begin ( );
	
	// Limit drawing to the alpha channel.
	glColorMask ( GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE );
	
	// Draw the burn sprite only on the alpha channel.
	m_pBurnSprite->visit ( );
	
	// Reset glColorMask to allow drawing of colors.
	glColorMask ( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	
	// Finish transformation.
	m_pDarknessLayer->end ( );

	// Make the monk walk back and forth.
	const CCPoint&	tMonkPos = m_pMonkSprite->getPosition ( );
	if ( tMonkPos.x > 480 )
	{
		m_pMonkSprite ->setFlipX ( KD_TRUE );
		m_pBurnSprite ->setVelocity ( ccp ( -1, 0 ) );
		m_pLightSprite->setVelocity ( ccp ( -1, 0 ) );
		m_pMonkSprite ->setVelocity ( ccp ( -1, 0 ) );
	}
	else if ( tMonkPos.x < 0 ) 
	{
		m_pMonkSprite ->setFlipX ( KD_FALSE );
		m_pBurnSprite ->setVelocity ( ccp ( 1, 0 ) );
		m_pLightSprite->setVelocity ( ccp ( 1, 0 ) );
		m_pMonkSprite ->setVelocity ( ccp ( 1, 0 ) );
	}

	// Update our SimpleAnimObjects
	m_pBurnSprite ->update ( fDelta );
	m_pLightSprite->update ( fDelta );
	m_pMonkSprite ->update ( fDelta );
}

KDfloat Ch1_ColorMaskLighting::makeBatFlyUp ( SimpleAnimObject* pBat )
{
	CCSpriteFrameCache*		pCache = CCSpriteFrameCache::sharedSpriteFrameCache ( );

	// Randomize animation speed.
	KDfloat			fDelay = (KDfloat) ( kdRand ( ) % 5 + 5 ) / 120;
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

KDvoid Ch1_ColorMaskLighting::makeBatGlideDown ( SimpleAnimObject* pBat )
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