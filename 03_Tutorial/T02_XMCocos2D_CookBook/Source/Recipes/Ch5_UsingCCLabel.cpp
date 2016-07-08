/* --------------------------------------------------------------------------
 *
 *      File            Ch5_UsingCCLabel.cpp
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
#include "Ch5_UsingCCLabel.h"

KDbool Ch5_UsingCCLabel::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// CCLabelAtlas for fixed-width bitmap fonts
	CCLabelAtlas*		pLabelAtlas = CCLabelAtlas::create ( "Atlas Label Test", "tuffy_bold_italic-charmap.png", 48, 65, ' ' ); 
	this->addChild ( pLabelAtlas, 0 );
	pLabelAtlas->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	pLabelAtlas->setScale ( 0.5f );
	pLabelAtlas->setPosition ( ccp ( 240, 220 ) );	
	pLabelAtlas->setColor ( ccc3 ( 0, 255, 0 ) );
	pLabelAtlas->runAction
	(
		CCRepeatForever::create 
		( 
			CCSequence::create
			(
				CCScaleTo::create ( 1.0f, 0.5f ),
				CCScaleTo::create ( 1.0f, 0.25f ), 
				KD_NULL
			)
		)
	);
	
	// CCLabelBMFont for variable-width bitmap fonts using FNT files
	CCLabelBMFont*		pLabelBMFont = CCLabelBMFont::create ( "Bitmap Label Test", "eurostile_30.fnt" );
	this->addChild ( pLabelBMFont, 0 );
	pLabelBMFont->setPosition ( ccp ( 240, 160 ) );

	CCObject*			pObject;
	CCARRAY_FOREACH ( pLabelBMFont->getChildren ( ), pObject ) 
	{
		CCSprite*		pChild = (CCSprite*) pObject;
		pChild->setColor ( ccc3 ( kdRand ( ) % 255, kdRand ( ) % 255, kdRand ( ) % 255 ) );
		pChild->runAction
		(
			CCRepeatForever::create 
			( 
				CCSequence::create
				(
					CCScaleTo::create ( kdRand ( ) % 2 + 1.f, 1.75f ),
					CCScaleTo::create ( kdRand ( ) % 2 + 1.f, 0.75f ), 
					KD_NULL
				)
			)
		);
	}

	// CCLabelTTF for true-type fonts
	CCLabelTTF*		pLabelTTF = CCLabelTTF::create ( "True-Type Label Test", "arial_narrow.otf", 32 );
	this->addChild ( pLabelTTF, 0 );
	pLabelTTF->setPosition ( ccp ( 240, 100 ) );
	pLabelTTF->setColor ( ccc3 ( 0, 0, 255 ) );
	pLabelTTF->runAction
	(
		CCRepeatForever::create 
		( 
			CCSequence::create
			(
				CCScaleTo::create ( 2.0f, 1.5f ),
				CCScaleTo::create ( 2.0f, 0.5f ), 
				KD_NULL
			)
		)
	);

	return KD_TRUE;
}
