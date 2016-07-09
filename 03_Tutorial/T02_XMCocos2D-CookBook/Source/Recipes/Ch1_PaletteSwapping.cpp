/* --------------------------------------------------------------------------
 *
 *      File            Ch1_PaletteSwapping.h
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
#include "Ch1_PaletteSwapping.h"

KDbool Ch1_PaletteSwapping::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Create a nice looking background
	CCSprite*	pBG = CCSprite::create ( "baseball_bg_02.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setOpacity ( 200 );
	this->addChild ( pBG, 0, 0 );

	/// Animate 4 different fielders with different color combinations 

	// Set color arrays
	ccColor3B	aColors1 [ ] =
	{ 
		ccc3 ( 255, 217, 161 ),		// Light skin tone
		ccc3 ( 225, 225, 225 ),		// Light gray
		ccc3 (   0,   0, 150 ),		// Dark blue
		ccc3 ( 255, 255, 255 ),		// White
	};
	ccColor3B	aColors2 [ ] =
	{ 
		ccc3 ( 140, 100,  46 ),		// Dark skin tone
		ccc3 ( 150, 150, 150 ),		// Gray
		ccc3 ( 255,   0,   0 ),		// Red
		ccc3 ( 255, 255, 255 ),		// White
	};
	ccColor3B	aColors3 [ ] =
	{ 
		ccc3 ( 255, 217, 161 ),		// Light skin tone
		ccc3 ( 115, 170, 115 ),		// Dull green
		ccc3 ( 115, 170, 115 ),		// Dull green
		ccc3 ( 255, 255, 255 ),		// White
	};
	ccColor3B	aColors4 [ ] =
	{ 
		ccc3 ( 140, 100,  46 ),		// Dark skin tone
		ccc3 (  50,  50,  50 ),		// Dark gray
		ccc3 ( 255, 255,   0 ),		// Yellow
		ccc3 ( 255, 255, 255 ),		// White
	};

	// Animate fielders with colors
	this->animateFielderWithColors ( aColors1, ccp ( 150,  70 ) );
	this->animateFielderWithColors ( aColors2, ccp ( 150, 200 ) );
	this->animateFielderWithColors ( aColors3, ccp ( 300, 200 ) );
	this->animateFielderWithColors ( aColors4, ccp ( 300,  70 ) );
	
	return KD_TRUE;
}

KDvoid Ch1_PaletteSwapping::animateFielderWithColors ( ccColor3B* pColors, const CCPoint& tPos )
{
	// The names of our layers
	const KDchar*	aLayers [ ] =
	{
		"skin"		 ,
		"uniform"	 ,
		"trim"		 ,
		"black_lines",
	};

	// Number of layers
	KDint	nNumLayers = 4;
	
	for ( KDint i = 0; i < nNumLayers; i++ )
	{
		const KDchar*	szLayerName = aLayers [ i ];
		ccColor3B		tColor = pColors [ i ];
		
		// We need each plist, the first frame name and finally a name for the animation
		const KDchar*	szPlistName      = ccszf ( "fielder_run_%s.plist", szLayerName );
		const KDchar*	szFirstFrameName = ccszf ( "fielder_run_%s_01.png", szLayerName );
//		const KDchar*	szAnimationName  = ccszf ( "fielder_run_%s", szLayerName );

		// Add plist frames to the SpriteFrameCache
		CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( szPlistName );

		// Get the first sprite frame
		CCSpriteFrame*	pFirstFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( szFirstFrameName );

		// Create our sprite
		CCSprite*		pSprite = CCSprite::createWithSpriteFrame ( pFirstFrame );

		// Set color and position
		pSprite->setPosition ( tPos );
		pSprite->setColor ( tColor );
	
		// Create the animation and add frames
		CCAnimation*	pAnimation = CCAnimation::create ( );
		pAnimation->setDelayPerUnit ( 0.15f );
		
		for ( KDint i = 1; i <= 8; i++ )
		{
			CCSpriteFrame*		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "fielder_run_%s_0%i.png", szLayerName, i ) );
			pAnimation->addSpriteFrame ( pFrame );
		}
	
		// Run the repeating animation
		pSprite->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );

		// Finally, add the sprite
		this->addChild ( pSprite );
	}
}