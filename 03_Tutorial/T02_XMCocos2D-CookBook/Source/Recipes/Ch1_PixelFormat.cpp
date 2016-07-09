/* --------------------------------------------------------------------------
 *
 *      File            Ch1_PixelFormat.cpp
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
#include "Ch1_PixelFormat.h"

KDbool Ch1_PixelFormat::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Set the pixel format before loading the image
	// RGBA 8888 image (32-bit)
	CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA8888 );

	CCSprite*		pSprite1 = CCSprite::create ( "gradient1.png" );
	pSprite1->setPosition ( ccp ( 120, 80 ) );
	this->addChild ( pSprite1 );	
	
	CCLabelTTF*		pType1 = CCLabelTTF::create ( "RGBA 8888 image (32-bit)", "Marker Felt.ttf", 16 );
	pType1->setPosition ( ccp ( 120, 80 ) );
	this->addChild ( pType1 );

	// Set the pixel format before loading the image
	// RGBA 4444 image (16-bit)
	CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGBA4444 );

	CCSprite*		pSprite2 = CCSprite::create ( "gradient2.png" );
	pSprite2->setPosition ( ccp ( 120, 240 ) );
	this->addChild ( pSprite2 );	
	
	CCLabelTTF*		pType2 = CCLabelTTF::create ( "RGBA 4444 image (16-bit)", "Marker Felt.ttf", 16 );
	pType2->setPosition ( ccp ( 120, 240 ) );
	this->addChild ( pType2 );

	// Set the pixel format before loading the image
	// RGB5A1 image (16-bit)
	CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGB5A1 );
	CCSprite*		pSprite3 = CCSprite::create ( "gradient3.png" );
	pSprite3->setPosition ( ccp ( 360, 80 ) );
	this->addChild ( pSprite3 );	
	
	CCLabelTTF*		pType3 = CCLabelTTF::create ( "RGB5A1 image (16-bit)", "Marker Felt.ttf", 16 );
	pType3->setPosition ( ccp ( 360, 80 ) );
	this->addChild ( pType3 );

	// Set the pixel format before loading the image
	// RGB565 image (16-bit)
	CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_RGB565 );
	CCSprite*		pSprite4 = CCSprite::create ( "gradient4.png" );
	pSprite4->setPosition ( ccp ( 360, 240 ) );
	this->addChild ( pSprite4 );	
	
	CCLabelTTF*		pType4 = CCLabelTTF::create ( "RGB565 image (16-bit)", "Marker Felt.ttf", 16 );
	pType4->setPosition ( ccp ( 360, 240 ) );
	this->addChild ( pType4 );
		
	// restore the default pixel format
	CCTexture2D::setDefaultAlphaPixelFormat ( kCCTexture2DPixelFormat_Default );

	return KD_TRUE;
}
