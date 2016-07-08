/* --------------------------------------------------------------------------
 *
 *      File            Ch1_ColoringSprites.cpp
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
#include "Ch1_ColoringSprites.h"

enum 
{
	TAG_FADE_TO_BLACK	= 0,
	TAG_FADE_TO_WHITE	= 1,
	TAG_FADE_SPRITE		= 2,
	TAG_MOUNTAIN_BG		= 3,
	TAG_SUN_BG			= 4,
	TAG_GOOD_SAMURAI	= 5,
	TAG_BAD_SAMURAI		= 6,
	TAG_GROUND_GRADIENT = 7,
	TAG_RED_GRADIENT	= 8,
};

KDbool Ch1_ColoringSprites::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	this->initButtons ( );

	// The Fade Scene Sprite
	CCSprite*	pFadeSprite = CCSprite::create ( "blank.png" );
	pFadeSprite->setOpacity ( 0 );
	pFadeSprite->setPosition ( ccp ( 240, 160 ) );
	pFadeSprite->setTextureRect ( CCRect ( 0, 0, 480, 320 ) );
	this->addChild ( pFadeSprite, 3, TAG_FADE_SPRITE );
	
	// Draw the sky using blank.png
	this->drawColoredSpriteAt ( ccp ( 240, 190 ), CCRect ( 0, 0, 480, 260 ), ccc3 ( 150, 200, 200 ), 0 );

	// Draw the sun
	CCSprite*	pSun = CCSprite::create ( "fire.png" );
	pSun->setPosition ( ccp ( 50, 230 ) );
	pSun->setScale ( 3.0f );
	pSun->setColor ( ccc3 ( 255, 255, 0 ) );
	this->addChild ( pSun, 0, TAG_SUN_BG );	

	// Draw some mountains in the background
	CCSprite*	pMountains = CCSprite::create ( "mountains.png" );
	pMountains->setPosition ( ccp ( 250, 200 ) );
	pMountains->setScale ( 0.6f );
	this->addChild ( pMountains, 0, TAG_MOUNTAIN_BG );

	// Add a gradient below the mountains
	CCLayerGradient*	pGroundGradient = CCLayerGradient::create ( ccc4 ( 61, 33, 62, 255 ), ccc4 ( 65, 89, 54, 255 ), ccp ( 0, -1 ) );
	pGroundGradient->setContentSize ( ccs ( 480, 100 ) );
	pGroundGradient->setPosition ( ccp ( 0, 50 ) );
	this->addChild ( pGroundGradient, 0, TAG_GROUND_GRADIENT );

	// Add a sinister red glow gradient behind the evil samurai
	CCLayerGradient*	pRedGradient = CCLayerGradient::create ( ccc4 ( 0, 0, 0, 0 ), ccc4 ( 255, 0, 0, 100 ), ccp ( 1, 0 ) );
	pRedGradient->setContentSize ( ccs ( 200, 200 ) );
	pRedGradient->setPosition ( ccp ( 280, 60 ) );
	this->addChild ( pRedGradient, 0, TAG_RED_GRADIENT );
	
	// Draw dramatic movie bars
	this->drawColoredSpriteAt ( ccp ( 240, 290 ), CCRect ( 0, 0, 480, 60 ), ccc3 ( 0, 0, 0 ), 2 ); 
	this->drawColoredSpriteAt ( ccp ( 240,  30 ), CCRect ( 0, 0, 480, 60 ), ccc3 ( 0, 0, 0 ), 2 ); 

	// Draw the good samurai
	CCSprite*	pGoodSamurai = CCSprite::create ( "samurai_good.png" );
	pGoodSamurai->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	pGoodSamurai->setPosition ( ccp ( 100, 70) );
	pGoodSamurai->setScale ( 0.5f );
	this->addChild ( pGoodSamurai, 1, TAG_GOOD_SAMURAI );

	// Draw the evil samurai
	CCSprite*	pEvilSamurai = CCSprite::create ( "samurai_evil.png" );
	pEvilSamurai->setAnchorPoint ( ccp ( 0.5f, 0 ) );
	pEvilSamurai->setPosition ( ccp ( 370, 70) );
	pEvilSamurai->setFlipX ( KD_TRUE );
	pEvilSamurai->setScale ( 0.5f );
	this->addChild ( pEvilSamurai, 1, TAG_BAD_SAMURAI );

	// Make the swords glow
	this->glowAt ( ccp ( 230, 280 ), CCSize ( 3.0f, 11.0f ), ccc3 ( 0, 230, 255 ),  45.0f, pGoodSamurai );
	this->glowAt ( ccp (  70, 280 ), CCSize ( 3.0f, 11.0f ), ccc3 ( 255, 200, 2 ), -45.0f, pEvilSamurai );	

	return KD_TRUE;
}

KDvoid Ch1_ColoringSprites::initButtons ( KDvoid )
{
	CCMenuItemFont::setFontSize ( 16 );

	// 'Fade to Black' button
	CCMenuItemFont*		pFadeToBlack = CCMenuItemFont::create ( "FADE TO BLACK", this, menu_selector ( Ch1_ColoringSprites::fadeToBlackCallback ) );
	CCMenu*				pFadeToBlackMenu = CCMenu::create ( pFadeToBlack, KD_NULL );
    pFadeToBlackMenu->setPosition ( ccp ( 180, 20 ) );
	this->addChild ( pFadeToBlackMenu, 4, TAG_FADE_TO_BLACK );
	
	// 'Fade to White' button
	CCMenuItemFont*		pFadeToWhite = CCMenuItemFont::create ( "FADE TO WHITE", this, menu_selector ( Ch1_ColoringSprites::fadeToWhiteCallback ) );
	CCMenu*				pFadeToWhiteMenu = CCMenu::create ( pFadeToWhite, KD_NULL );
    pFadeToWhiteMenu->setPosition ( ccp ( 300, 20 ) );
	this->addChild ( pFadeToWhiteMenu, 4, TAG_FADE_TO_WHITE );	
}

KDvoid Ch1_ColoringSprites::fadeToBlackCallback ( CCObject* pSender )
{
	CCSprite*	pFadeSprite = (CCSprite*) this->getChildByTag ( TAG_FADE_SPRITE );
	pFadeSprite->stopAllActions ( );
	pFadeSprite->setColor ( ccc3 ( 0, 0, 0 ) );
	pFadeSprite->setOpacity ( 0 );
	pFadeSprite->runAction ( CCSequence::create ( CCFadeIn::create ( 2.f ), CCFadeOut::create ( 2.f ), KD_NULL ) );	
}

KDvoid Ch1_ColoringSprites::fadeToWhiteCallback ( CCObject* pSender )
{
	CCSprite*	pFadeSprite = (CCSprite*) this->getChildByTag ( TAG_FADE_SPRITE );
	pFadeSprite->stopAllActions ( );
	pFadeSprite->setColor ( ccc3 ( 255, 255, 255 ) );
	pFadeSprite->setOpacity ( 0 );
	pFadeSprite->runAction ( CCSequence::create ( CCFadeIn::create ( 2.f ), CCFadeOut::create ( 2.f ), KD_NULL ) );	
}

// Draw a colored sprite
KDvoid Ch1_ColoringSprites::drawColoredSpriteAt ( const CCPoint& tPosition, const CCRect& tRect, const ccColor3B& tColor, KDint nZOrder )
{
	CCSprite*	pSprite = CCSprite::create ( "blank.png" );
	pSprite->setPosition ( tPosition );
	pSprite->setTextureRect ( tRect );
	pSprite->setColor ( tColor );
	this->addChild ( pSprite );
	
	// Set Z Order
	this->reorderChild ( pSprite, nZOrder );
}

KDvoid Ch1_ColoringSprites::glowAt ( const CCPoint& tPosition, const CCSize& tSize, const ccColor3B& tColor, KDfloat fRotation, CCSprite* pSprite )
{
	CCSprite*	pGlowSprite = CCSprite::create ( "fire.png" );

	pGlowSprite->setColor ( tColor );
	pGlowSprite->setPosition ( tPosition );
	pGlowSprite->setRotation ( fRotation );

	ccBlendFunc		tBlendFunc = { GL_ONE, GL_ONE };
	pGlowSprite->setBlendFunc ( tBlendFunc  );
	pGlowSprite->runAction 
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCScaleTo::create ( 0.9f, tSize.cx, tSize.cy ),
				CCScaleTo::create ( 0.9f, tSize.cx * 0.75f, tSize.cy * 0.75f ),
				KD_NULL
			)
		)
	);

	pGlowSprite->runAction
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCFadeTo::create ( 0.9f, 150 ),
				CCFadeTo::create ( 0.9f, 255 ),
				KD_NULL
			)
		)
	);
	
	pSprite->addChild ( pGlowSprite );
}