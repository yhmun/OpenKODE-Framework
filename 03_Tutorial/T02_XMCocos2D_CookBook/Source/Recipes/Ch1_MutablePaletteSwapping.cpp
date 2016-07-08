/* --------------------------------------------------------------------------
 *
 *      File            Ch1_MutablePaletteSwapping.cpp
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
#include "Ch1_MutablePaletteSwapping.h"
#include "Libraries/CCTexture2DMutable.h"

KDbool Ch1_MutablePaletteSwapping::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Create a nice looking background
	CCSprite*	pBG = CCSprite::create ( "baseball_bg_01.png" );
	pBG->setPosition ( ccp ( 240, 160 ) );
	pBG->setOpacity ( 200 );
	this->addChild ( pBG, 0, 0 );

	/// Animate 4 different fielders with different color combinations 

	// Set color arrays
	ccColor4B	aColors1 [ ] =
	{ 
		ccc4 ( 255, 217, 161, 255 ),
		ccc4 ( 225, 225, 225, 255 ),
		ccc4 (   0,   0, 150, 255 ),
	};
	ccColor4B	aColors2 [ ] =
	{ 
		ccc4 ( 140, 100,  46, 255 ),
		ccc4 ( 150, 150, 150, 255 ),
		ccc4 ( 255,   0,   0, 255 ),
	};
	ccColor4B	aColors3 [ ] =
	{ 
		ccc4 ( 255, 217, 161, 255 ),
		ccc4 ( 115, 170, 115, 255 ),
		ccc4 ( 115, 170, 115, 255 ),
	};
	ccColor4B	aColors4 [ ] =
	{ 
		ccc4 ( 140, 100,  46, 255 ),
		ccc4 (  50,  50,  50, 255 ),
		ccc4 ( 255, 255,   0, 255 ),
	};

	// Create texture copy to use as an immutable guide.
	CCTexture2DMutable*		pTextureCopy = CCTexture2DMutable::createWithImage ( CCImage::create ( "fielder_run_sentinel_colors.png" ) );
	CCTexture2DMutable*		pSpriteTexture;

	// Create our sprites using mutable textures.
	CCSprite*	pSprite1 = CCSprite::createWithTexture ( CCTexture2DMutable::createWithImage ( CCImage::create ( "fielder_run_sentinel_colors.png" ) ) );
	CCSprite*	pSprite2 = CCSprite::createWithTexture ( CCTexture2DMutable::createWithImage ( CCImage::create ( "fielder_run_sentinel_colors.png" ) ) );
	CCSprite*	pSprite3 = CCSprite::createWithTexture ( CCTexture2DMutable::createWithImage ( CCImage::create ( "fielder_run_sentinel_colors.png" ) ) );
	CCSprite*	pSprite4 = CCSprite::createWithTexture ( CCTexture2DMutable::createWithImage ( CCImage::create ( "fielder_run_sentinel_colors.png" ) ) );

	// Set sprite positions
	pSprite1->setPosition ( ccp ( 125,  75 ) );
	pSprite2->setPosition ( ccp ( 125, 225 ) );
	pSprite3->setPosition ( ccp ( 325,  75 ) );
	pSprite4->setPosition ( ccp ( 325, 225 ) );

	// Swap colors in each sprite mutable texture and apply the changes.
	pSpriteTexture = (CCTexture2DMutable*) pSprite1->getTexture ( );
	this->swapColor ( ccc4 ( 0, 0, 255, 255 ), aColors1 [ 0 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 0, 255, 0, 255 ), aColors1 [ 1 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 255, 0, 0, 255 ), aColors1 [ 2 ], pSpriteTexture, pTextureCopy );
	pSpriteTexture->apply ( );

	pSpriteTexture = (CCTexture2DMutable*) pSprite2->getTexture ( );
	this->swapColor ( ccc4 ( 0, 0, 255, 255 ), aColors2 [ 0 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 0, 255, 0, 255 ), aColors2 [ 1 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 255, 0, 0, 255 ), aColors2 [ 2 ], pSpriteTexture, pTextureCopy );
	pSpriteTexture->apply ( );
	
	pSpriteTexture = (CCTexture2DMutable*) pSprite3->getTexture ( );
	this->swapColor ( ccc4 ( 0, 0, 255, 255 ), aColors3 [ 0 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 0, 255, 0, 255 ), aColors3 [ 1 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 255, 0, 0, 255 ), aColors3 [ 2 ], pSpriteTexture, pTextureCopy );
	pSpriteTexture->apply ( );
	
	pSpriteTexture = (CCTexture2DMutable*) pSprite4->getTexture ( );
	this->swapColor ( ccc4 ( 0, 0, 255, 255 ), aColors4 [ 0 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 0, 255, 0, 255 ), aColors4 [ 1 ], pSpriteTexture, pTextureCopy );
	this->swapColor ( ccc4 ( 255, 0, 0, 255 ), aColors4 [ 2 ], pSpriteTexture, pTextureCopy );
	pSpriteTexture->apply ( );

	// Finally, add the sprites to the scene.
	this->addChild ( pSprite1, 0, 0 );
	this->addChild ( pSprite2, 0, 1 );
	this->addChild ( pSprite3, 0, 2 );
	this->addChild ( pSprite4, 0, 3 );

	return KD_TRUE;
}

KDvoid Ch1_MutablePaletteSwapping::swapColor ( const ccColor4B& tColor1, const ccColor4B& tColor2, CCTexture2DMutable* pTexture, CCTexture2DMutable* pCopy )
{
	// Look through the texture, find all pixels of the specified color and change them.
	// We use a tolerance of 200 here.
	for ( KDuint x = 0; x < pTexture->getPixelsWide ( ); x++ )
	{
		for ( KDuint y = 0; y < pTexture->getPixelsHigh ( ); y++ )
		{
			if ( this->isColor ( pCopy->pixelAt ( ccp ( x, y ) ), tColor1, 200 ) )
			{
				pTexture->setPixelAt ( ccp ( x, y ), tColor2 );
			}
		}
	}		
}

KDbool Ch1_MutablePaletteSwapping::isColor ( const ccColor4B& tColor1, const ccColor4B& tColor2, KDint nTolerance )
{
	// If the colors are equal within a tolerance we change them.
	KDbool	bEqual = KD_TRUE;

	if ( kdAbs ( tColor1.r - tColor2.r ) +
		 kdAbs ( tColor1.g - tColor2.g ) + 
		 kdAbs ( tColor1.b - tColor2.b ) +
		 kdAbs ( tColor1.a - tColor2.a ) > nTolerance )
	{
		bEqual = KD_FALSE;
	}

	return bEqual;
}