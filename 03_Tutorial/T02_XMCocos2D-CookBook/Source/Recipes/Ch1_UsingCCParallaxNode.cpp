/* --------------------------------------------------------------------------
 *
 *      File            Ch1_UsingCCParallaxNode.cpp
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
#include "Ch1_UsingCCParallaxNode.h"

KDbool Ch1_UsingCCParallaxNode::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Create four parallax sprites, one for each layer
	CCSprite*	pParallaxLayer01 = CCSprite::create ( "parallax_layer_01.png" );	
	CCSprite*	pParallaxLayer02 = CCSprite::create ( "parallax_layer_02.png" );	
	CCSprite*	pParallaxLayer03 = CCSprite::create ( "parallax_layer_03.png" );	
	CCSprite*	pParallaxLayer04 = CCSprite::create ( "parallax_layer_04.png" );	
	
	// Create a parallax node and add all four sprites
	CCParallaxNode*		pParallaxNode = CCParallaxNode::create ( );
	pParallaxNode->setPosition ( ccp ( 0, 0 ) );
	pParallaxNode->addChild ( pParallaxLayer01, 1, ccp ( 0, 0 ), ccp ( 240, 200 ) );
	pParallaxNode->addChild ( pParallaxLayer02, 2, ccp ( 1, 0 ), ccp ( 240, 100 ) );
	pParallaxNode->addChild ( pParallaxLayer03, 3, ccp ( 2, 0 ), ccp ( 240, 100 ) );
	pParallaxNode->addChild ( pParallaxLayer04, 4, ccp ( 3, 0 ), ccp ( 240,  20 ) );
	this->addChild ( pParallaxNode, 0, 1 ); 
	
	// Move the node to the left then the right
	// This creates the effect that we are moving to the right then the left
	pParallaxNode->runAction
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCMoveBy::create ( 5.0f, ccp ( -80, 0 ) ),
				CCMoveBy::create ( 2.5f, ccp (  80, 0 ) ),
				KD_NULL
			)
		)
	);

	return KD_TRUE;
}
