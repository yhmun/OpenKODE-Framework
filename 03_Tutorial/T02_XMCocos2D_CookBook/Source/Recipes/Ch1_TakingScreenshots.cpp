/* --------------------------------------------------------------------------
 *
 *      File            Ch1_TakingScreenshots.cpp
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
#include "Ch1_TakingScreenshots.h"
#include "Libraries/Screenshot.h"

KDbool Ch1_TakingScreenshots::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	CCSprite*	pSprite = CCSprite::createWithTexture ( Screenshot::takeAsTexture2D ( ) );
	pSprite->setPosition ( ccp ( 240, 160 ) );
	pSprite->setScale ( 0.75f );
	this->addChild ( pSprite, 0, 0 );

	this->schedule ( schedule_selector ( Ch1_TakingScreenshots::step ), 1.f, 5, 1.f );

	return KD_TRUE;
}

KDvoid Ch1_TakingScreenshots::step ( KDfloat fDelta )
{
	this->removeChildByTag ( 0 );
		
	CCSprite*	pSprite = CCSprite::createWithTexture ( Screenshot::takeAsTexture2D ( ) );
	pSprite->setPosition ( ccp ( 240, 160 ) );
	pSprite->setScale ( 0.75f );
	this->addChild ( pSprite, 0, 0 );
}